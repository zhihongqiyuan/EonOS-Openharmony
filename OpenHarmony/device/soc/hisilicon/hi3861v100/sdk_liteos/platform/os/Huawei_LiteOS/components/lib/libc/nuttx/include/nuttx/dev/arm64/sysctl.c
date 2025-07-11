/*
 * Copyright (c) 2003-2007 Apple Inc. All rights reserved.
 */
#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/sysctl.h>

#include <machine/machine_routines.h>

#include <mach/host_info.h>
#include <mach/mach_host.h>
#include <arm/cpuid.h>
#include <kern/hvg_hypercall.h>
#include <vm/pmap.h>
#include <kern/zalloc.h>
#include <libkern/libkern.h>
#include <pexpert/device_tree.h>
#include <kern/task.h>
#include <vm/vm_protos.h>

#if HYPERVISOR
#include <kern/hv_support.h>
#include <kern/bits.h>
#endif

extern uint64_t wake_abstime;

#if DEVELOPMENT || DEBUG
/* Various tuneables to modulate selection of WFE in the idle path */
extern int wfe_rec_max;
extern int wfe_allowed;

extern int wfe_rec_none;
extern uint32_t idle_proximate_timer_wfe;
extern uint32_t idle_proximate_io_wfe_masked;
extern uint32_t idle_proximate_io_wfe_unmasked;

static
SYSCTL_INT(_machdep, OID_AUTO, wfe_rec_max,
    CTLFLAG_RW, &wfe_rec_max, 0,
    "");

static
SYSCTL_INT(_machdep, OID_AUTO, wfe_allowed,
    CTLFLAG_RW, &wfe_allowed, 0,
    "");

static
SYSCTL_INT(_machdep, OID_AUTO, idle_timer_wfe,
    CTLFLAG_RW, &idle_proximate_timer_wfe, 0,
    "");

static
SYSCTL_INT(_machdep, OID_AUTO, idle_io_wfe_masked,
    CTLFLAG_RW, &idle_proximate_io_wfe_masked, 0,
    "");
static
SYSCTL_INT(_machdep, OID_AUTO, idle_io_wfe_unmasked,
    CTLFLAG_RW, &idle_proximate_io_wfe_unmasked, 0,
    "");

static
SYSCTL_INT(_machdep, OID_AUTO, wfe_rec_none,
    CTLFLAG_RW, &wfe_rec_none, 0,
    "");

extern uint64_t wfe_rec_override_mat;
SYSCTL_QUAD(_machdep, OID_AUTO, wfe_rec_override_mat,
    CTLFLAG_RW, &wfe_rec_override_mat,
    "");

extern uint64_t wfe_rec_clamp;
SYSCTL_QUAD(_machdep, OID_AUTO, wfe_rec_clamp,
    CTLFLAG_RW, &wfe_rec_clamp,
    "");

#endif

static
SYSCTL_QUAD(_machdep, OID_AUTO, wake_abstime,
    CTLFLAG_RD, &wake_abstime,
    "Absolute Time at the last wakeup");

static int
sysctl_time_since_reset SYSCTL_HANDLER_ARGS
{
#pragma unused(arg1, arg2, oidp)
	uint64_t return_value = ml_get_time_since_reset();
	return SYSCTL_OUT(req, &return_value, sizeof(return_value));
}

SYSCTL_PROC(_machdep, OID_AUTO, time_since_reset,
    CTLFLAG_RD | CTLTYPE_QUAD | CTLFLAG_LOCKED,
    0, 0, sysctl_time_since_reset, "I",
    "Continuous time since last SOC boot/wake started");

static int
sysctl_wake_conttime SYSCTL_HANDLER_ARGS
{
#pragma unused(arg1, arg2, oidp)
	uint64_t return_value = ml_get_conttime_wake_time();
	return SYSCTL_OUT(req, &return_value, sizeof(return_value));
}

SYSCTL_PROC(_machdep, OID_AUTO, wake_conttime,
    CTLFLAG_RD | CTLTYPE_QUAD | CTLFLAG_LOCKED,
    0, 0, sysctl_wake_conttime, "I",
    "Continuous Time at the last wakeup");

#if defined(HAS_IPI)
static int
cpu_signal_deferred_timer(__unused struct sysctl_oid *oidp, __unused void *arg1, __unused int arg2, struct sysctl_req *req)
{
	int new_value = 0;
	int changed   = 0;

	int old_value = (int)ml_cpu_signal_deferred_get_timer();

	int error = sysctl_io_number(req, old_value, sizeof(int), &new_value, &changed);

	if (error == 0 && changed) {
		ml_cpu_signal_deferred_adjust_timer((uint64_t)new_value);
	}

	return error;
}

SYSCTL_PROC(_machdep, OID_AUTO, deferred_ipi_timeout,
    CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_LOCKED,
    0, 0,
    cpu_signal_deferred_timer, "I", "Deferred IPI timeout (nanoseconds)");

#endif /* defined(HAS_IPI) */

/*
 * For source compatibility, here's some machdep.cpu mibs that
 * use host_info() to simulate reasonable answers.
 */

SYSCTL_NODE(_machdep, OID_AUTO, cpu, CTLFLAG_RW | CTLFLAG_LOCKED, 0,
    "CPU info");

static int
arm_host_info SYSCTL_HANDLER_ARGS
{
	__unused struct sysctl_oid *unused_oidp = oidp;

	host_basic_info_data_t hinfo;
	mach_msg_type_number_t count = HOST_BASIC_INFO_COUNT;
#define BSD_HOST        1
	kern_return_t kret = host_info((host_t)BSD_HOST,
	    HOST_BASIC_INFO, (host_info_t)&hinfo, &count);
	if (KERN_SUCCESS != kret) {
		return EINVAL;
	}

	if (sizeof(uint32_t) != arg2) {
		panic("size mismatch");
	}

	uintptr_t woffset = (uintptr_t)arg1 / sizeof(uint32_t);
	uint32_t datum = *(uint32_t *)(((uint32_t *)&hinfo) + woffset);
	return SYSCTL_OUT(req, &datum, sizeof(datum));
}

/*
 * machdep.cpu.cores_per_package
 *
 * x86: derived from CPUID data.
 * ARM: how many physical cores we have in the AP; aka hw.physicalcpu_max
 */
static
SYSCTL_PROC(_machdep_cpu, OID_AUTO, cores_per_package,
    CTLTYPE_INT | CTLFLAG_RD | CTLFLAG_LOCKED,
    (void *)offsetof(host_basic_info_data_t, physical_cpu_max),
    sizeof(integer_t),
    arm_host_info, "I", "CPU cores per package");

/*
 * machdep.cpu.core_count
 *
 * x86: derived from CPUID data.
 * ARM: # active physical cores in the AP; aka hw.physicalcpu
 */
static
SYSCTL_PROC(_machdep_cpu, OID_AUTO, core_count,
    CTLTYPE_INT | CTLFLAG_RD | CTLFLAG_LOCKED,
    (void *)offsetof(host_basic_info_data_t, physical_cpu),
    sizeof(integer_t),
    arm_host_info, "I", "Number of enabled cores per package");

/*
 * machdep.cpu.logical_per_package
 *
 * x86: derived from CPUID data. Returns ENOENT if HTT bit not set, but
 *      most x64 CPUs have that, so assume it's available.
 * ARM: total # logical cores in the AP; aka hw.logicalcpu_max
 */
static
SYSCTL_PROC(_machdep_cpu, OID_AUTO, logical_per_package,
    CTLTYPE_INT | CTLFLAG_RD | CTLFLAG_LOCKED,
    (void *)offsetof(host_basic_info_data_t, logical_cpu_max),
    sizeof(integer_t),
    arm_host_info, "I", "CPU logical cpus per package");

/*
 * machdep.cpu.thread_count
 *
 * x86: derived from CPUID data.
 * ARM: # active logical cores in the AP; aka hw.logicalcpu
 */
static
SYSCTL_PROC(_machdep_cpu, OID_AUTO, thread_count,
    CTLTYPE_INT | CTLFLAG_RD | CTLFLAG_LOCKED,
    (void *)offsetof(host_basic_info_data_t, logical_cpu),
    sizeof(integer_t),
    arm_host_info, "I", "Number of enabled threads per package");

static SECURITY_READ_ONLY_LATE(char*) brand_string = NULL;
static SECURITY_READ_ONLY_LATE(size_t) brand_string_len = 0;

/*
 * SecureDTLookupEntry() is only guaranteed to work before PE_init_iokit(),
 * so we load the brand string (if available) in a startup handler.
 */
__startup_func
static void
sysctl_load_brand_string(void)
{
	DTEntry node;
	void const *value = NULL;
	unsigned int size = 0;

	if (kSuccess != SecureDTLookupEntry(0, "/product", &node)) {
		return;
	}

	if (kSuccess != SecureDTGetProperty(node, "product-soc-name", (void const **) &value, &size)) {
		return;
	}

	if (size == 0) {
		return;
	}

	brand_string = zalloc_permanent(size, ZALIGN_NONE);
	if (brand_string == NULL) {
		return;
	}

	memcpy(brand_string, value, size);
	brand_string_len = size;
}
STARTUP(SYSCTL, STARTUP_RANK_MIDDLE, sysctl_load_brand_string);

/*
 * machdep.cpu.brand_string
 *
 * x86: derived from CPUID data.
 * ARM: Grab the product string from the device tree, if it exists.
 *      Otherwise, cons something up from the CPUID register.
 *      the value is already exported via the commpage. So keep it simple.
 */
static int
make_brand_string SYSCTL_HANDLER_ARGS
{
	__unused struct sysctl_oid *unused_oidp = oidp;
	__unused void *unused_arg1 = arg1;
	__unused int unused_arg2 = arg2;

	if (brand_string != NULL) {
		return SYSCTL_OUT(req, brand_string, brand_string_len);
	}

	const char *impl;

	switch (cpuid_info()->arm_info.arm_implementor) {
	case CPU_VID_APPLE:
		impl = "Apple";
		break;
	case CPU_VID_ARM:
		impl = "ARM";
		break;
	default:
		impl = "ARM architecture";
		break;
	}

	char buf[80];
	snprintf(buf, sizeof(buf), "%s processor", impl);
	return SYSCTL_OUT(req, buf, strlen(buf) + 1);
}

SYSCTL_PROC(_machdep_cpu, OID_AUTO, brand_string,
    CTLTYPE_STRING | CTLFLAG_RD | CTLFLAG_LOCKED,
    0, 0, make_brand_string, "A", "CPU brand string");


static int
virtual_address_size SYSCTL_HANDLER_ARGS
{
#pragma unused(arg1, arg2, oidp)
	int return_value = 64 - T0SZ_BOOT;
	return SYSCTL_OUT(req, &return_value, sizeof(return_value));
}

static
SYSCTL_PROC(_machdep, OID_AUTO, virtual_address_size,
    CTLTYPE_INT | CTLFLAG_RD | CTLFLAG_LOCKED,
    0, 0, virtual_address_size, "I",
    "Number of addressable bits in userspace virtual addresses");


#if DEVELOPMENT || DEBUG
extern uint64_t TLockTimeOut;
SYSCTL_QUAD(_machdep, OID_AUTO, tlto,
    CTLFLAG_RW | CTLFLAG_LOCKED, &TLockTimeOut,
    "Ticket spinlock timeout (MATUs): use with care");

extern uint32_t timebase_validation;
SYSCTL_UINT(_machdep, OID_AUTO, timebase_validation,
    CTLFLAG_RW | CTLFLAG_LOCKED, &timebase_validation, 0,
    "Monotonicity validation of kernel mach_absolute_time()");

#if __WKDM_ISA_2P_WORKAROUND__
extern uint64_t wkdmdretries, wkdmdretriespb;
extern uint32_t simulate_wkdm2p_error, wkdm_isa_2p_war_required;
SYSCTL_QUAD(_machdep, OID_AUTO, wkdmdretries,
    CTLFLAG_RW | CTLFLAG_LOCKED, &wkdmdretries,
    "Number of WKDM errata retries");
SYSCTL_QUAD(_machdep, OID_AUTO, wkdmdretriespb,
    CTLFLAG_RW | CTLFLAG_LOCKED, &wkdmdretriespb,
    "Number of retries where payload was on page boundary");
SYSCTL_UINT(_machdep, OID_AUTO, simulate_wkdm2p_error,
    CTLFLAG_RW | CTLFLAG_LOCKED,
    &simulate_wkdm2p_error, 0, "");
SYSCTL_UINT(_machdep, OID_AUTO, wkdm_isa_2p_war_required,
    CTLFLAG_RW | CTLFLAG_LOCKED,
    &wkdm_isa_2p_war_required, 0, "");
#endif /* __WKDM_ISA_2P_WORKAROUND__ */


/*
 * macro to generate a sysctl machdep.cpu.sysreg_* for a given system register
 * using __builtin_arm_rsr64.
 */
#define SYSCTL_PROC_MACHDEP_CPU_SYSREG(name)                            \
static int                                                              \
sysctl_sysreg_##name SYSCTL_HANDLER_ARGS                                \
{                                                                       \
_Pragma("unused(arg1, arg2, oidp)")                                     \
	uint64_t return_value = __builtin_arm_rsr64(#name);                 \
	return SYSCTL_OUT(req, &return_value, sizeof(return_value));        \
}                                                                       \
SYSCTL_PROC(_machdep_cpu, OID_AUTO, sysreg_##name,                      \
    CTLFLAG_RD | CTLTYPE_QUAD | CTLFLAG_LOCKED,                         \
    0, 0, sysctl_sysreg_##name, "Q",                                    \
    #name " register on the current CPU");


// CPU system registers
// ARM64: AArch64 Vector Base Address Register
SYSCTL_PROC_MACHDEP_CPU_SYSREG(VBAR_EL1);
// ARM64: AArch64 Memory Attribute Indirection Register
SYSCTL_PROC_MACHDEP_CPU_SYSREG(MAIR_EL1);
// ARM64: AArch64 Translation table base register 1
SYSCTL_PROC_MACHDEP_CPU_SYSREG(TTBR1_EL1);
// ARM64: AArch64 System Control Register
SYSCTL_PROC_MACHDEP_CPU_SYSREG(SCTLR_EL1);
// ARM64: AArch64 Translation Control Register
SYSCTL_PROC_MACHDEP_CPU_SYSREG(TCR_EL1);
// ARM64: AArch64 Memory Model Feature Register 0
SYSCTL_PROC_MACHDEP_CPU_SYSREG(ID_AA64MMFR0_EL1);
// ARM64: AArch64 Instruction Set Attribute Register 1
SYSCTL_PROC_MACHDEP_CPU_SYSREG(ID_AA64ISAR1_EL1);
#if APPLE_ARM64_ARCH_FAMILY
// Apple ID Register
SYSCTL_PROC_MACHDEP_CPU_SYSREG(AIDR_EL1);
#endif /* APPLE_ARM64_ARCH_FAMILY */

#endif /* DEVELOPMENT || DEBUG */


#ifdef ML_IO_TIMEOUTS_ENABLED
/*
 * Timeouts for ml_{io|phys}_{read|write}...
 * RO on DEVELOPMENT/DEBUG kernels.
 */

#if DEVELOPMENT || DEBUG
#define MMIO_TIMEOUT_FLAGS (CTLFLAG_KERN | CTLFLAG_RW | CTLFLAG_LOCKED)
#else
#define MMIO_TIMEOUT_FLAGS (CTLFLAG_KERN | CTLFLAG_RD | CTLFLAG_LOCKED)
#endif

SYSCTL_QUAD(_machdep, OID_AUTO, report_phy_read_delay, MMIO_TIMEOUT_FLAGS,
    &report_phy_read_delay_to, "Maximum time before io/phys read gets reported or panics");
SYSCTL_QUAD(_machdep, OID_AUTO, report_phy_write_delay, MMIO_TIMEOUT_FLAGS,
    &report_phy_write_delay_to, "Maximum time before io/phys write gets reported or panics");
SYSCTL_QUAD(_machdep, OID_AUTO, trace_phy_read_delay, MMIO_TIMEOUT_FLAGS,
    &trace_phy_read_delay_to, "Maximum time before io/phys read gets ktraced");
SYSCTL_QUAD(_machdep, OID_AUTO, trace_phy_write_delay, MMIO_TIMEOUT_FLAGS,
    &trace_phy_write_delay_to, "Maximum time before io/phys write gets ktraced");

SYSCTL_INT(_machdep, OID_AUTO, phy_read_delay_panic, CTLFLAG_KERN | CTLFLAG_RW | CTLFLAG_LOCKED,
    &phy_read_panic, 0, "if set, report-phy-read-delay timeout panics");
SYSCTL_INT(_machdep, OID_AUTO, phy_write_delay_panic, CTLFLAG_KERN | CTLFLAG_RW | CTLFLAG_LOCKED,
    &phy_write_panic, 0, "if set, report-phy-write-delay timeout panics");

#if ML_IO_SIMULATE_STRETCHED_ENABLED
SYSCTL_QUAD(_machdep, OID_AUTO, sim_stretched_io_ns, CTLFLAG_KERN | CTLFLAG_RW | CTLFLAG_LOCKED,
    &simulate_stretched_io, "simulate stretched io in ml_read_io, ml_write_io");
#endif /* ML_IO_SIMULATE_STRETCHED_ENABLED */

#endif /* ML_IO_TIMEOUTS_ENABLED */

int opensource_kernel = 1;
SYSCTL_INT(_kern, OID_AUTO, opensource_kernel, CTLFLAG_KERN | CTLFLAG_RD | CTLFLAG_LOCKED,
    &opensource_kernel, 0, "Opensource Kernel");

static int
machdep_ptrauth_enabled SYSCTL_HANDLER_ARGS
{
#pragma unused(arg1, arg2, oidp)

#if __has_feature(ptrauth_calls)
	task_t task = current_task();
	int ret = !ml_task_get_disable_user_jop(task);
#else
	const int ret = 0;
#endif

	return SYSCTL_OUT(req, &ret, sizeof(ret));
}

SYSCTL_PROC(_machdep, OID_AUTO, ptrauth_enabled,
    CTLTYPE_INT | CTLFLAG_KERN | CTLFLAG_RD,
    0, 0,
    machdep_ptrauth_enabled, "I", "");

#if CONFIG_TELEMETRY && (DEBUG || DEVELOPMENT)
extern unsigned long trap_telemetry_reported_events;
SYSCTL_ULONG(_debug, OID_AUTO, trap_telemetry_reported_events,
    CTLFLAG_RD | CTLFLAG_LOCKED, &trap_telemetry_reported_events,
    "Number of trap telemetry events successfully reported");

extern unsigned long trap_telemetry_capacity_dropped_events;
SYSCTL_ULONG(_debug, OID_AUTO, trap_telemetry_capacity_dropped_events,
    CTLFLAG_RD | CTLFLAG_LOCKED, &trap_telemetry_capacity_dropped_events,
    "Number of trap telemetry events which were dropped due to a full RSB");
#endif /* CONFIG_TELEMETRY && (DEBUG || DEVELOPMENT) */


#if DEBUG || DEVELOPMENT
/* A sysctl that can be used to check if the platform supports DRAM ECC and error injection. */
static int
dram_ecc_error_injection_capable SYSCTL_HANDLER_ARGS
{
#pragma unused(arg1, arg2, req)
	int capable = 0;

/* T6041 does not support error injection. */

	return SYSCTL_OUT(req, &capable, sizeof(capable));
}
SYSCTL_PROC(_vm, OID_AUTO, dram_ecc_error_injection_capable, CTLTYPE_INT | CTLFLAG_RD | CTLFLAG_LOCKED,
    0, 0, &dram_ecc_error_injection_capable, "I", "");
#endif /* DEBUG || DEVELOPMENT */

