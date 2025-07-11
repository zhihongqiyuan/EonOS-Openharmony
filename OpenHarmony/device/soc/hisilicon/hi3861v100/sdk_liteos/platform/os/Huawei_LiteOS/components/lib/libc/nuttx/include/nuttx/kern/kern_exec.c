/*
 * Copyright (c) 2000-2020 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/* Copyright (c) 1995 NeXT Computer, Inc. All Rights Reserved */
/*
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */

/*-
 * Copyright (c) 1982, 1986, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	from: @(#)kern_exec.c	8.1 (Berkeley) 6/10/93
 */
/*
 * NOTICE: This file was modified by SPARTA, Inc. in 2005 to introduce
 * support for mandatory and extensible security protections.  This notice
 * is included in support of clause 2.2 (b) of the Apple Public License,
 * Version 2.0.
 */
#include <machine/reg.h>
#include <machine/cpu_capabilities.h>

#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/filedesc.h>
#include <sys/kernel.h>
#include <sys/proc_internal.h>
#include <sys/kauth.h>
#include <sys/user.h>
#include <sys/socketvar.h>
#include <sys/malloc.h>
#include <sys/namei.h>
#include <sys/mount_internal.h>
#include <sys/vnode_internal.h>
#include <sys/file_internal.h>
#include <sys/stat.h>
#include <sys/uio_internal.h>
#include <sys/acct.h>
#include <sys/exec.h>
#include <sys/kdebug.h>
#include <sys/signal.h>
#include <sys/aio_kern.h>
#include <sys/sysproto.h>
#include <sys/sysctl.h>
#include <sys/persona.h>
#include <sys/reason.h>
#if SYSV_SHM
#include <sys/shm_internal.h>           /* shmexec() */
#endif
#include <sys/ubc_internal.h>           /* ubc_map() */
#include <sys/spawn.h>
#include <sys/spawn_internal.h>
#include <sys/process_policy.h>
#include <sys/codesign.h>
#include <sys/random.h>
#include <crypto/sha1.h>

#include <libkern/libkern.h>
#include <libkern/amfi/amfi.h>
#include <libkern/crypto/sha2.h>
#include <security/audit/audit.h>

#include <ipc/ipc_types.h>

#include <mach/mach_param.h>
#include <mach/mach_types.h>
#include <mach/port.h>
#include <mach/task.h>
#include <mach/task_access.h>
#include <mach/thread_act.h>
#include <mach/vm_map.h>
#include <mach/mach_vm.h>
#include <mach/vm_param.h>
#include <mach_debug/mach_debug_types.h>

#include <kern/sched_prim.h> /* thread_wakeup() */
#include <kern/affinity.h>
#include <kern/assert.h>
#include <kern/ipc_kobject.h>
#include <kern/task.h>
#include <kern/thread.h>
#include <kern/coalition.h>
#include <kern/policy_internal.h>
#include <kern/kalloc.h>
#include <kern/zalloc.h> /* zone_userspace_reboot_checks() */

#include <os/log.h>

#if CONFIG_MACF
#include <security/mac_framework.h>
#include <security/mac_mach_internal.h>
#endif

#if CONFIG_AUDIT
#include <bsm/audit_kevents.h>
#endif

#if CONFIG_ARCADE
#include <kern/arcade.h>
#endif

#include <vm/vm_map_xnu.h>
#include <vm/vm_kern_xnu.h>
#include <vm/vm_protos.h>
#include <vm/vm_fault.h>
#include <vm/vm_pageout_xnu.h>
#include <vm/pmap.h>
#include <vm/vm_reclaim_xnu.h>

#include <kdp/kdp_dyld.h>

#include <machine/machine_routines.h>
#include <machine/pal_routines.h>

#include <pexpert/pexpert.h>

#if CONFIG_MEMORYSTATUS
#include <sys/kern_memorystatus.h>
#endif

#include <IOKit/IOBSD.h>

#include "kern_exec_internal.h"

#include <CodeSignature/Entitlements.h>

#include <mach/exclaves.h>


extern boolean_t vm_darkwake_mode;

/* enable crash reports on various exec failures */
static TUNABLE(bool, bootarg_execfailurereports, "execfailurecrashes", false);

#if XNU_TARGET_OS_OSX
#if __has_feature(ptrauth_calls)
static TUNABLE(bool, bootarg_arm64e_preview_abi, "-arm64e_preview_abi", false);
#endif /* __has_feature(ptrauth_calls) */

#if DEBUG || DEVELOPMENT
static TUNABLE(bool, unentitled_ios_sim_launch, "unentitled_ios_sim_launch", false);
#endif /* DEBUG || DEVELOPMENT */
#endif /* XNU_TARGET_OS_OSX */

#if DEVELOPMENT || DEBUG
os_log_t exec_log_handle = NULL;
#define EXEC_LOG(fmt, ...)      \
do {    \
	if (exec_log_handle) {      \
	        os_log_with_type(exec_log_handle, OS_LOG_TYPE_INFO, "exec - %s:%d " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);    \
	}   \
} while (0)
#else /* DEVELOPMENT || DEBUG */
#define EXEC_LOG(fmt, ...)  do { } while (0)
#endif /* DEVELOPMENT || DEBUG */

#if CONFIG_DTRACE
/* Do not include dtrace.h, it redefines kmem_[alloc/free] */
extern void dtrace_proc_exec(proc_t);
extern void (*dtrace_proc_waitfor_exec_ptr)(proc_t);

/*
 * Since dtrace_proc_waitfor_exec_ptr can be added/removed in dtrace_subr.c,
 * we will store its value before actually calling it.
 */
static void (*dtrace_proc_waitfor_hook)(proc_t) = NULL;

#include <sys/dtrace_ptss.h>
#endif

#if __has_feature(ptrauth_calls)
static TUNABLE_DEV_WRITEABLE(int, vm_shared_region_per_team_id,
    "vm_shared_region_per_team_id", 1);
static TUNABLE_DEV_WRITEABLE(int, vm_shared_region_by_entitlement,
    "vm_shared_region_by_entitlement", 1);

/* Upon userland request, reslide the shared cache. */
static TUNABLE_DEV_WRITEABLE(int, vm_shared_region_reslide_aslr,
    "vm_shared_region_reslide_aslr",
#if CONFIG_RESLIDE_SHARED_CACHE
    1
#else
    0
#endif /* CONFIG_RESLIDE_SHARED_CACHE */
    );

/*
 * Flag to control what processes should get shared cache randomize resliding
 * after a fault in the shared cache region:
 *
 * 0 - all processes get a new randomized slide
 * 1 - only platform processes get a new randomized slide
 */
TUNABLE_DEV_WRITEABLE(int, vm_shared_region_reslide_restrict,
    "vm_shared_region_reslide_restrict", 1);

#if DEVELOPMENT || DEBUG
SYSCTL_INT(_vm, OID_AUTO, vm_shared_region_per_team_id,
    CTLFLAG_RW, &vm_shared_region_per_team_id, 0, "");
SYSCTL_INT(_vm, OID_AUTO, vm_shared_region_by_entitlement,
    CTLFLAG_RW, &vm_shared_region_by_entitlement, 0, "");
SYSCTL_INT(_vm, OID_AUTO, vm_shared_region_reslide_restrict,
    CTLFLAG_RW, &vm_shared_region_reslide_restrict, 0, "");
SYSCTL_INT(_vm, OID_AUTO, vm_shared_region_reslide_aslr,
    CTLFLAG_RW, &vm_shared_region_reslide_aslr, 0, "");
#endif
#endif /* __has_feature(ptrauth_calls) */

#if DEVELOPMENT || DEBUG
static TUNABLE(bool, enable_dext_coredumps_on_panic, "dext_panic_coredump", true);
#else
static TUNABLE(bool, enable_dext_coredumps_on_panic, "dext_panic_coredump", false);
#endif
extern kern_return_t kern_register_userspace_coredump(task_t task, const char * name);
#define USERSPACE_COREDUMP_PANIC_ENTITLEMENT "com.apple.private.enable-coredump-on-panic"
#define USERSPACE_COREDUMP_PANIC_SEED_ENTITLEMENT \
	"com.apple.private.enable-coredump-on-panic-seed-privacy-approved"

extern void proc_apply_task_networkbg_internal(proc_t, thread_t);
extern void task_set_did_exec_flag(task_t task);
extern void task_clear_exec_copy_flag(task_t task);
proc_t proc_exec_switch_task(proc_t old_proc, proc_t new_proc, task_t old_task,
    task_t new_task, struct image_params *imgp, void **inherit);
boolean_t task_is_active(task_t);
boolean_t thread_is_active(thread_t thread);
void thread_copy_resource_info(thread_t dst_thread, thread_t src_thread);
void *ipc_importance_exec_switch_task(task_t old_task, task_t new_task);
extern void ipc_importance_release(void *elem);
extern boolean_t task_has_watchports(task_t task);
extern void task_set_no_smt(task_t task);
#if defined(HAS_APPLE_PAC)
char *task_get_vm_shared_region_id_and_jop_pid(task_t task, uint64_t *jop_pid);
#endif
task_t convert_port_to_task(ipc_port_t port);

#if CONFIG_EXCLAVES
int task_add_conclave(task_t task, void *vnode, int64_t off, const char *task_conclave_id);
kern_return_t task_inherit_conclave(task_t old_task, task_t new_task, void *vnode, int64_t off);
#endif /* CONFIG_EXCLAVES */

/*
 * Mach things for which prototypes are unavailable from Mach headers
 */
extern void ipc_task_enable(task_t task);
extern void ipc_task_reset(task_t task);
extern void ipc_thread_reset(thread_t thread);

#if DEVELOPMENT || DEBUG
void task_importance_update_owner_info(task_t);
#endif

extern struct savearea *get_user_regs(thread_t);

__attribute__((noinline)) int __EXEC_WAITING_ON_TASKGATED_CODE_SIGNATURE_UPCALL__(mach_port_t task_access_port, int32_t new_pid);

#include <kern/thread.h>
#include <kern/task.h>
#include <kern/ast.h>
#include <kern/mach_loader.h>
#include <kern/mach_fat.h>
#include <mach-o/fat.h>
#include <mach-o/loader.h>
#include <machine/vmparam.h>
#include <sys/imgact.h>

#include <sys/sdt.h>


/*
 * EAI_ITERLIMIT	The maximum number of times to iterate an image
 *			activator in exec_activate_image() before treating
 *			it as malformed/corrupt.
 */
#define EAI_ITERLIMIT           3

/*
 * For #! interpreter parsing
 */
#define IS_WHITESPACE(ch) ((ch == ' ') || (ch == '\t'))
#define IS_EOL(ch) ((ch == '#') || (ch == '\n'))

extern vm_map_t bsd_pageable_map;
extern const struct fileops vnops;
extern int nextpidversion;


#define USER_ADDR_ALIGN(addr, val) \
	( ( (user_addr_t)(addr) + (val) - 1) \
	        & ~((val) - 1) )

/*
 * For subsystem root support
 */
#define SPAWN_SUBSYSTEM_ROOT_ENTITLEMENT "com.apple.private.spawn-subsystem-root"

/*
 * Allow setting p_crash_behavior to trigger panic on crash
 */
#define SPAWN_SET_PANIC_CRASH_BEHAVIOR "com.apple.private.spawn-panic-crash-behavior"

/* Platform Code Exec Logging */
static int platform_exec_logging = 0;

SYSCTL_DECL(_security_mac);

SYSCTL_INT(_security_mac, OID_AUTO, platform_exec_logging, CTLFLAG_RW, &platform_exec_logging, 0,
    "log cdhashes for all platform binary executions");

static os_log_t peLog = OS_LOG_DEFAULT;

struct exception_port_action_t {
	ipc_port_t port;
	_ps_port_action_t *port_action;
};

struct exec_port_actions {
	uint32_t exception_port_count;
	uint32_t portwatch_count;
	uint32_t registered_count;
	struct exception_port_action_t *excport_array;
	ipc_port_t *portwatch_array;
	ipc_port_t registered_array[TASK_PORT_REGISTER_MAX];
};

struct image_params;    /* Forward */
static int exec_activate_image(struct image_params *imgp);
static int exec_copyout_strings(struct image_params *imgp, user_addr_t *stackp);
static int load_return_to_errno(load_return_t lrtn);
static int execargs_alloc(struct image_params *imgp);
static int execargs_free(struct image_params *imgp);
static int exec_check_permissions(struct image_params *imgp);
static int exec_extract_strings(struct image_params *imgp);
static int exec_add_apple_strings(struct image_params *imgp, const load_result_t *load_result, task_t task);
static int exec_handle_sugid(struct image_params *imgp);
static int sugid_scripts = 0;
SYSCTL_INT(_kern, OID_AUTO, sugid_scripts, CTLFLAG_RW | CTLFLAG_LOCKED, &sugid_scripts, 0, "");
static kern_return_t create_unix_stack(vm_map_t map, load_result_t* load_result, proc_t p);
static int copyoutptr(user_addr_t ua, user_addr_t ptr, int ptr_size);
static void exec_resettextvp(proc_t, struct image_params *);
static int process_signature(proc_t, struct image_params *);
static void exec_prefault_data(proc_t, struct image_params *, load_result_t *);
static errno_t exec_handle_port_actions(struct image_params *imgp,
    struct exec_port_actions *port_actions);
static errno_t exec_handle_exception_port_actions(const struct image_params *imgp,
    const struct exec_port_actions *port_actions);
static errno_t exec_handle_spawnattr_policy(proc_t p, thread_t thread, int psa_apptype, uint64_t psa_qos_clamp,
    task_role_t psa_darwin_role, struct exec_port_actions *port_actions);
static void exec_port_actions_destroy(struct exec_port_actions *port_actions);

/*
 * exec_add_user_string
 *
 * Add the requested string to the string space area.
 *
 * Parameters;	struct image_params *		image parameter block
 *		user_addr_t			string to add to strings area
 *		int				segment from which string comes
 *		boolean_t			TRUE if string contributes to NCARGS
 *
 * Returns:	0			Success
 *		!0			Failure errno from copyinstr()
 *
 * Implicit returns:
 *		(imgp->ip_strendp)	updated location of next add, if any
 *		(imgp->ip_strspace)	updated byte count of space remaining
 *		(imgp->ip_argspace) updated byte count of space in NCARGS
 */
__attribute__((noinline))
static int
exec_add_user_string(struct image_params *imgp, user_addr_t str, int seg, boolean_t is_ncargs)
{
	int error = 0;

	do {
		size_t len = 0;
		int space;

		if (is_ncargs) {
			space = imgp->ip_argspace; /* by definition smaller than ip_strspace */
		} else {
			space = imgp->ip_strspace;
		}

		if (space <= 0) {
			error = E2BIG;
			break;
		}

		if (!UIO_SEG_IS_USER_SPACE(seg)) {
			char *kstr = CAST_DOWN(char *, str);     /* SAFE */
			error = copystr(kstr, imgp->ip_strendp, space, &len);
		} else {
			error = copyinstr(str, imgp->ip_strendp, space, &len);
		}

		imgp->ip_strendp += len;
		imgp->ip_strspace -= len;
		if (is_ncargs) {
			imgp->ip_argspace -= len;
		}
	} while (error == ENAMETOOLONG);

	return error;
}

/*
 * dyld is now passed the executable path as a getenv-like variable
 * in the same fashion as the stack_guard and malloc_entropy keys.
 */
#define EXECUTABLE_KEY "executable_path="

/*
 * exec_save_path
 *
 * To support new app package launching for Mac OS X, the dyld needs the
 * first argument to execve() stored on the user stack.
 *
 * Save the executable path name at the bottom of the strings area and set
 * the argument vector pointer to the location following that to indicate
 * the start of the argument and environment tuples, setting the remaining
 * string space count to the size of the string area minus the path length.
 *
 * Parameters;	struct image_params *		image parameter block
 *		char *				path used to invoke program
 *		int				segment from which path comes
 *
 * Returns:	int			0	Success
 *		EFAULT				Bad address
 *	copy[in]str:EFAULT			Bad address
 *	copy[in]str:ENAMETOOLONG		Filename too long
 *
 * Implicit returns:
 *		(imgp->ip_strings)		saved path
 *		(imgp->ip_strspace)		space remaining in ip_strings
 *		(imgp->ip_strendp)		start of remaining copy area
 *		(imgp->ip_argspace)		space remaining of NCARGS
 *		(imgp->ip_applec)		Initial applev[0]
 *
 * Note:	We have to do this before the initial namei() since in the
 *		path contains symbolic links, namei() will overwrite the
 *		original path buffer contents.  If the last symbolic link
 *		resolved was a relative pathname, we would lose the original
 *		"path", which could be an absolute pathname. This might be
 *		unacceptable for dyld.
 */
static int
exec_save_path(struct image_params *imgp, user_addr_t path, int seg, const char **excpath)
{
	int error;
	size_t len;
	char *kpath;

	// imgp->ip_strings can come out of a cache, so we need to obliterate the
	// old path.
	memset(imgp->ip_strings, '\0', strlen(EXECUTABLE_KEY) + MAXPATHLEN);

	len = MIN(MAXPATHLEN, imgp->ip_strspace);

	switch (seg) {
	case UIO_USERSPACE32:
	case UIO_USERSPACE64:   /* Same for copyin()... */
		error = copyinstr(path, imgp->ip_strings + strlen(EXECUTABLE_KEY), len, &len);
		break;
	case UIO_SYSSPACE:
		kpath = CAST_DOWN(char *, path); /* SAFE */
		error = copystr(kpath, imgp->ip_strings + strlen(EXECUTABLE_KEY), len, &len);
		break;
	default:
		error = EFAULT;
		break;
	}

	if (!error) {
		bcopy(EXECUTABLE_KEY, imgp->ip_strings, strlen(EXECUTABLE_KEY));
		len += strlen(EXECUTABLE_KEY);

		imgp->ip_strendp += len;
		imgp->ip_strspace -= len;

		if (excpath) {
			*excpath = imgp->ip_strings + strlen(EXECUTABLE_KEY);
		}
	}

	return error;
}

/*
 * exec_reset_save_path
 *
 * If we detect a shell script, we need to reset the string area
 * state so that the interpreter can be saved onto the stack.
 *
 * Parameters;	struct image_params *		image parameter block
 *
 * Returns:	int			0	Success
 *
 * Implicit returns:
 *		(imgp->ip_strings)		saved path
 *		(imgp->ip_strspace)		space remaining in ip_strings
 *		(imgp->ip_strendp)		start of remaining copy area
 *		(imgp->ip_argspace)		space remaining of NCARGS
 *
 */
static int
exec_reset_save_path(struct image_params *imgp)
{
	imgp->ip_strendp = imgp->ip_strings;
	imgp->ip_argspace = NCARGS;
	imgp->ip_strspace = (NCARGS + PAGE_SIZE);

	return 0;
}

/*
 * exec_shell_imgact
 *
 * Image activator for interpreter scripts.  If the image begins with
 * the characters "#!", then it is an interpreter script.  Verify the
 * length of the script line indicating the interpreter is not in
 * excess of the maximum allowed size.  If this is the case, then
 * break out the arguments, if any, which are separated by white
 * space, and copy them into the argument save area as if they were
 * provided on the command line before all other arguments.  The line
 * ends when we encounter a comment character ('#') or newline.
 *
 * Parameters;	struct image_params *	image parameter block
 *
 * Returns:	-1			not an interpreter (keep looking)
 *		-3			Success: interpreter: relookup
 *		>0			Failure: interpreter: error number
 *
 * A return value other than -1 indicates subsequent image activators should
 * not be given the opportunity to attempt to activate the image.
 */
static int
exec_shell_imgact(struct image_params *imgp)
{
	char *vdata = imgp->ip_vdata;
	char *ihp;
	char *line_startp, *line_endp;
	char *interp;

	/*
	 * Make sure it's a shell script.  If we've already redirected
	 * from an interpreted file once, don't do it again.
	 */
	if (vdata[0] != '#' ||
	    vdata[1] != '!' ||
	    (imgp->ip_flags & IMGPF_INTERPRET) != 0) {
		return -1;
	}

	if (imgp->ip_origcputype != 0) {
		/* Fat header previously matched, don't allow shell script inside */
		return -1;
	}

	imgp->ip_flags |= IMGPF_INTERPRET;
	imgp->ip_interp_sugid_fd = -1;
	imgp->ip_interp_buffer[0] = '\0';

	/* Check to see if SUGID scripts are permitted.  If they aren't then
	 * clear the SUGID bits.
	 * imgp->ip_vattr is known to be valid.
	 */
	if (sugid_scripts == 0) {
		imgp->ip_origvattr->va_mode &= ~(VSUID | VSGID);
	}

	/* Try to find the first non-whitespace character */
	for (ihp = &vdata[2]; ihp < &vdata[IMG_SHSIZE]; ihp++) {
		if (IS_EOL(*ihp)) {
			/* Did not find interpreter, "#!\n" */
			return ENOEXEC;
		} else if (IS_WHITESPACE(*ihp)) {
			/* Whitespace, like "#!    /bin/sh\n", keep going. */
		} else {
			/* Found start of interpreter */
			break;
		}
	}

	if (ihp == &vdata[IMG_SHSIZE]) {
		/* All whitespace, like "#!           " */
		return ENOEXEC;
	}

	line_startp = ihp;

	/* Try to find the end of the interpreter+args string */
	for (; ihp < &vdata[IMG_SHSIZE]; ihp++) {
		if (IS_EOL(*ihp)) {
			/* Got it */
			break;
		} else {
			/* Still part of interpreter or args */
		}
	}

	if (ihp == &vdata[IMG_SHSIZE]) {
		/* A long line, like "#! blah blah blah" without end */
		return ENOEXEC;
	}

	/* Backtrack until we find the last non-whitespace */
	while (IS_EOL(*ihp) || IS_WHITESPACE(*ihp)) {
		ihp--;
	}

	/* The character after the last non-whitespace is our logical end of line */
	line_endp = ihp + 1;

	/*
	 * Now we have pointers to the usable part of:
	 *
	 * "#!  /usr/bin/int first    second   third    \n"
	 *      ^ line_startp                       ^ line_endp
	 */

	/* copy the interpreter name */
	interp = imgp->ip_interp_buffer;
	for (ihp = line_startp; (ihp < line_endp) && !IS_WHITESPACE(*ihp); ihp++) {
		*interp++ = *ihp;
	}
	*interp = '\0';

	exec_reset_save_path(imgp);
	exec_save_path(imgp, CAST_USER_ADDR_T(imgp->ip_interp_buffer),
	    UIO_SYSSPACE, NULL);

	/* Copy the entire interpreter + args for later processing into argv[] */
	interp = imgp->ip_interp_buffer;
	for (ihp = line_startp; (ihp < line_endp); ihp++) {
		*interp++ = *ihp;
	}
	*interp = '\0';

#if CONFIG_SETUID
	/*
	 * If we have an SUID or SGID script, create a file descriptor
	 * from the vnode and pass /dev/fd/%d instead of the actual
	 * path name so that the script does not get opened twice
	 */
	if (imgp->ip_origvattr->va_mode & (VSUID | VSGID)) {
		proc_t p;
		struct fileproc *fp;
		int fd;
		int error;

		p = vfs_context_proc(imgp->ip_vfs_context);
		error = falloc_exec(p, imgp->ip_vfs_context, &fp, &fd);
		if (error) {
			return error;
		}

		fp->fp_glob->fg_flag = FREAD;
		fp->fp_glob->fg_ops = &vnops;
		fp_set_data(fp, imgp->ip_vp);

		proc_fdlock(p);
		procfdtbl_releasefd(p, fd, NULL);
		fp_drop(p, fd, fp, 1);
		proc_fdunlock(p);
		vnode_ref(imgp->ip_vp);

		imgp->ip_interp_sugid_fd = fd;
	}
#endif /* CONFIG_SETUID */

	return -3;
}



/*
 * exec_fat_imgact
 *
 * Image activator for fat 1.0 binaries.  If the binary is fat, then we
 * need to select an image from it internally, and make that the image
 * we are going to attempt to execute.  At present, this consists of
 * reloading the first page for the image with a first page from the
 * offset location indicated by the fat header.
 *
 * Parameters;	struct image_params *	image parameter block
 *
 * Returns:	-1			not a fat binary (keep looking)
 *		-2			Success: encapsulated binary: reread
 *		>0			Failure: error number
 *
 * Important:	This image activator is byte order neutral.
 *
 * Note:	A return value other than -1 indicates subsequent image
 *		activators should not be given the opportunity to attempt
 *		to activate the image.
 *
 *              If we find an encapsulated binary, we make no assertions
 *		about its  validity; instead, we leave that up to a rescan
 *		for an activator to claim it, and, if it is claimed by one,
 *		that activator is responsible for determining validity.
 */
static int
exec_fat_imgact(struct image_params *imgp)
{
	proc_t p = vfs_context_proc(imgp->ip_vfs_context);
	kauth_cred_t cred = kauth_cred_proc_ref(p);
	struct fat_header *fat_header = (struct fat_header *)imgp->ip_vdata;
	struct _posix_spawnattr *psa = NULL;
	struct fat_arch fat_arch;
	int resid, error;
	load_return_t lret;

	if (imgp->ip_origcputype != 0) {
		/* Fat header previously matched, don't allow another fat file inside */
		error = -1; /* not claimed */
		goto bad;
	}

	/* Make sure it's a fat binary */
	if (OSSwapBigToHostInt32(fat_header->magic) != FAT_MAGIC) {
		error = -1; /* not claimed */
		goto bad;
	}

	/* imgp->ip_vdata has PAGE_SIZE, zerofilled if the file is smaller */
	lret = fatfile_validate_fatarches((vm_offset_t)fat_header, PAGE_SIZE,
	    (off_t)imgp->ip_vattr->va_data_size);
	if (lret != LOAD_SUCCESS) {
		error = load_return_to_errno(lret);
		goto bad;
	}

	/* If posix_spawn binprefs exist, respect those prefs. */
	psa = (struct _posix_spawnattr *) imgp->ip_px_sa;
	if (psa != NULL && psa->psa_binprefs[0] != 0) {
		uint32_t pr = 0;

		/* Check each preference listed against all arches in header */
		for (pr = 0; pr < NBINPREFS; pr++) {
			cpu_type_t pref = psa->psa_binprefs[pr];
			cpu_type_t subpref = psa->psa_subcpuprefs[pr];

			if (pref == 0) {
				/* No suitable arch in the pref list */
				error = EBADARCH;
				goto bad;
			}

			if (pref == CPU_TYPE_ANY) {
				/* Fall through to regular grading */
				goto regular_grading;
			}

			lret = fatfile_getbestarch_for_cputype(pref,
			    subpref,
			    (vm_offset_t)fat_header,
			    PAGE_SIZE,
			    imgp,
			    &fat_arch);
			if (lret == LOAD_SUCCESS) {
				goto use_arch;
			}
		}

		/* Requested binary preference was not honored */
		error = EBADEXEC;
		goto bad;
	}

regular_grading:
	/* Look up our preferred architecture in the fat file. */
	lret = fatfile_getbestarch((vm_offset_t)fat_header,
	    PAGE_SIZE,
	    imgp,
	    &fat_arch,
	    (p->p_flag & P_AFFINITY) != 0);
	if (lret != LOAD_SUCCESS) {
		error = load_return_to_errno(lret);
		goto bad;
	}

use_arch:
	/* Read the Mach-O header out of fat_arch */
	error = vn_rdwr(UIO_READ, imgp->ip_vp, imgp->ip_vdata,
	    PAGE_SIZE, fat_arch.offset,
	    UIO_SYSSPACE, (IO_UNIT | IO_NODELOCKED),
	    cred, &resid, p);
	if (error) {
		if (error == ERESTART) {
			error = EINTR;
		}
		goto bad;
	}

	if (resid) {
		memset(imgp->ip_vdata + (PAGE_SIZE - resid), 0x0, resid);
	}

	/* Success.  Indicate we have identified an encapsulated binary */
	error = -2;
	imgp->ip_arch_offset = (user_size_t)fat_arch.offset;
	imgp->ip_arch_size = (user_size_t)fat_arch.size;
	imgp->ip_origcputype = fat_arch.cputype;
	imgp->ip_origcpusubtype = fat_arch.cpusubtype;

bad:
	kauth_cred_unref(&cred);
	return error;
}

static int
activate_exec_state(task_t task, proc_t p, thread_t thread, load_result_t *result)
{
	int ret;

	(void)task_set_dyld_info(task, MACH_VM_MIN_ADDRESS, 0, false);
	task_set_64bit(task, result->is_64bit_addr, result->is_64bit_data);
	if (result->is_64bit_addr) {
		OSBitOrAtomic(P_LP64, &p->p_flag);
		get_bsdthread_info(thread)->uu_flag |= UT_LP64;
	} else {
		OSBitAndAtomic(~((uint32_t)P_LP64), &p->p_flag);
		get_bsdthread_info(thread)->uu_flag &= ~UT_LP64;
	}
	task_set_mach_header_address(task, result->mach_header);

	ret = thread_state_initialize(thread);
	if (ret != KERN_SUCCESS) {
		return ret;
	}

	if (result->threadstate) {
		uint32_t *ts = result->threadstate;
		uint32_t total_size = (uint32_t)result->threadstate_sz;

		while (total_size > 0) {
			uint32_t flavor = *ts++;
			uint32_t size = *ts++;

			ret = thread_setstatus(thread, flavor, (thread_state_t)ts, size);
			if (ret) {
				return ret;
			}
			ts += size;
			total_size -= (size + 2) * sizeof(uint32_t);
		}
	}

	thread_setentrypoint(thread, result->entry_point);

	return KERN_SUCCESS;
}

#if (DEVELOPMENT || DEBUG)
extern char panic_on_proc_crash[];
extern int use_panic_on_proc_crash;

extern char panic_on_proc_exit[];
extern int use_panic_on_proc_exit;

extern char panic_on_proc_spawn_fail[];
extern int use_panic_on_proc_spawn_fail;

static inline void
set_crash_behavior_from_bootarg(proc_t p)
{
	if (use_panic_on_proc_crash && strcmp(p->p_comm, panic_on_proc_crash) == 0) {
		printf("will panic on proc crash: %s\n", p->p_comm);
		p->p_crash_behavior |= POSIX_SPAWN_PANIC_ON_CRASH;
	}

	if (use_panic_on_proc_exit && strcmp(p->p_comm, panic_on_proc_exit) == 0) {
		printf("will panic on proc exit: %s\n", p->p_comm);
		p->p_crash_behavior |= POSIX_SPAWN_PANIC_ON_EXIT;
	}

	if (use_panic_on_proc_spawn_fail && strcmp(p->p_comm, panic_on_proc_spawn_fail) == 0) {
		printf("will panic on proc spawn fail: %s\n", p->p_comm);
		p->p_crash_behavior |= POSIX_SPAWN_PANIC_ON_SPAWN_FAIL;
	}
}
#endif

void
set_proc_name(struct image_params *imgp, proc_t p)
{
	uint64_t buflen = imgp->ip_ndp->ni_cnd.cn_namelen;
	const int p_name_len = sizeof(p->p_name) - 1;
	const int p_comm_len = sizeof(p->p_comm) - 1;

	if (buflen > p_name_len) {
		buflen = p_name_len;
	}

	bcopy((caddr_t)imgp->ip_ndp->ni_cnd.cn_nameptr, (caddr_t)p->p_name, buflen);
	p->p_name[buflen] = '\0';

	if (buflen > p_comm_len) {
		static_assert(MAXCOMLEN + 1 == sizeof(p->p_comm));
		buflen = p_comm_len;
	}

	bcopy((caddr_t)imgp->ip_ndp->ni_cnd.cn_nameptr, (caddr_t)p->p_comm, buflen);
	p->p_comm[buflen] = '\0';

#if (DEVELOPMENT || DEBUG)
	/*
	 * This happens during image activation, so the crash behavior flags from
	 * posix_spawn will have already been set. So we don't have to worry about
	 * this being overridden.
	 */
	set_crash_behavior_from_bootarg(p);
#endif
}

#if __has_feature(ptrauth_calls)
/**
 * Returns a team ID string that may be used to assign a shared region.
 *
 * Platform binaries do not have team IDs and will return NULL.  Non-platform
 * binaries without a team ID will be assigned an artificial team ID of ""
 * (empty string) so that they will not be assigned to the default shared
 * region.
 *
 * @param imgp image parameter block
 * @return NULL if this is a platform binary, or an appropriate team ID string
 *         otherwise
 */
static inline const char *
get_teamid_for_shared_region(struct image_params *imgp)
{
	assert(imgp->ip_vp != NULL);

	const char *ret = csvnode_get_teamid(imgp->ip_vp, imgp->ip_arch_offset);
	if (ret) {
		return ret;
	}

	struct cs_blob *blob = csvnode_get_blob(imgp->ip_vp, imgp->ip_arch_offset);
	if (csblob_get_platform_binary(blob)) {
		return NULL;
	} else {
		static const char *NO_TEAM_ID = "";
		return NO_TEAM_ID;
	}
}

/**
 * Determines whether ptrauth should be enabled for the provided arm64 CPU subtype.
 *
 * @param cpusubtype Mach-O style CPU subtype
 * @return whether the CPU subtype matches arm64e with the current ptrauth ABI
 */
static inline bool
arm64_cpusubtype_uses_ptrauth(cpu_subtype_t cpusubtype)
{
	return (cpusubtype & ~CPU_SUBTYPE_MASK) == CPU_SUBTYPE_ARM64E &&
	       CPU_SUBTYPE_ARM64_PTR_AUTH_VERSION(cpusubtype) == CPU_SUBTYPE_ARM64_PTR_AUTH_CURRENT_VERSION;
}

#endif /* __has_feature(ptrauth_calls) */

/**
 * Returns whether a type/subtype slice matches the requested
 * type/subtype.
 *
 * @param mask Bits to mask from the requested/tested cpu type
 * @param req_cpu Requested cpu type
 * @param req_subcpu Requested cpu subtype
 * @param test_cpu Tested slice cpu type
 * @param test_subcpu Tested slice cpu subtype
 */
boolean_t
binary_match(cpu_type_t mask, cpu_type_t req_cpu,
    cpu_subtype_t req_subcpu, cpu_type_t test_cpu,
    cpu_subtype_t test_subcpu)
{
	if ((test_cpu & ~mask) != (req_cpu & ~mask)) {
		return FALSE;
	}

	test_subcpu &= ~CPU_SUBTYPE_MASK;
	req_subcpu  &= ~CPU_SUBTYPE_MASK;

	if (test_subcpu != req_subcpu && req_subcpu != (CPU_SUBTYPE_ANY & ~CPU_SUBTYPE_MASK)) {
		return FALSE;
	}

	return TRUE;
}


// Check entitlements to see if this is a hardened runtime binary.
// Save this in load_result until later for two purposes:
// 1. Once the task is created, we can mark it as hardened runtime if needed
// 2. we can propagate which entitlements are present to the apple array
static inline void
encode_HR_entitlement(const char *entitlement, HR_flags_t mask,
    const struct image_params *imgp, load_result_t *load_result)
{
	if (IOVnodeHasEntitlement(imgp->ip_vp, (int64_t)imgp->ip_arch_offset, entitlement)) {
		load_result->hardened_runtime_binary |= mask;
	}
}

/*
 * If the passed in executable's vnode should use the RSR
 * shared region, then this should return TRUE, otherwise, return FALSE.
 */
static uint32_t rsr_current_version = 0;
boolean_t (*rsr_check_vnode)(void *vnode) = NULL;

boolean_t
vnode_is_rsr(vnode_t vp)
{
	if (!(vnode_isreg(vp) && vnode_tag(vp) == VT_APFS)) {
		return FALSE;
	}

	if (rsr_check_vnode != NULL && rsr_check_vnode((void *)vp)) {
		return TRUE;
	}
	return FALSE;
}

/* Runtime security mitigations */

/*
 * Hardened-process.
 *
 * Security mitigations follow the notion of "hardened-process": binaries that we
 * have identified as being security critical. They are identified by the
 * com.apple.developer.hardened-process entitlement, which is required to further
 * configure the other security mitigations.
 */
#define SPAWN_ENABLE_HARDENED_PROCESS "com.apple.developer.hardened-process"

/*
 * Hardened-Heap.
 *
 * This mitigation extends libmalloc xzone with a number of security features,
 * most notably increasing the number of buckets and adding guard pages.
 * The presence of the entitlement opts the binary into the feature.
 */
#define SPAWN_ENABLE_HARDENED_HEAP "com.apple.developer.hardened-process.hardened-heap"



/*
 * Enablement of hardened-heap is pretty straightforward and solely depends on
 * the presence of the associated entitlement.
 */
static inline void
exec_setup_hardened_heap(struct image_params *imgp, task_t task)
{
	if (IOVnodeHasEntitlement(imgp->ip_vp, (int64_t)imgp->ip_arch_offset,
	    SPAWN_ENABLE_HARDENED_HEAP)) {
		task_set_hardened_heap(task);
	}
}



/*
 * This routine configures the various runtime mitigations we can apply to a process.
 * Returns true on success, false on failure. Failure will be fatal in exec_mach_imgact().
 */
static inline errno_t
exec_setup_runtime_mitigations(struct image_params *imgp, __unused load_result_t *load_result,
    __unused task_t old_task, task_t new_task, __unused vm_map_t map, __unused proc_t proc)
{
	errno_t retval = 0;

	/*
	 * Hardened-heap enables a set of extra security features in our system memory allocator.
	 */
	exec_setup_hardened_heap(imgp, new_task);


	return retval;
}

uint32_t
rsr_get_version(void)
{
	return os_atomic_load(&rsr_current_version, relaxed);
}

void
rsr_bump_version(void)
{
	os_atomic_inc(&rsr_current_version, relaxed);
}

#if XNU_TARGET_OS_OSX
static int
rsr_version_sysctl SYSCTL_HANDLER_ARGS
{
#pragma unused(arg1, arg2, oidp)
	int value = rsr_get_version();
	int error = SYSCTL_OUT(req, &value, sizeof(int));
	if (error) {
		return error;
	}

	if (!req->newptr) {
		return 0;
	}

	error = SYSCTL_IN(req, &value, sizeof(int));
	if (error) {
		return error;
	}
	if (value != 0) {
		rsr_bump_version();
	}
	return 0;
}


SYSCTL_PROC(_vm, OID_AUTO, shared_region_control,
    CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_LOCKED | CTLFLAG_MASKED,
    0, 0, rsr_version_sysctl, "I", "");
#endif /* XNU_TARGET_OS_OSX */

/*
 * exec_mach_imgact
 *
 * Image activator for mach-o 1.0 binaries.
 *
 * Parameters;	struct image_params *	image parameter block
 *
 * Returns:	-1			not a fat binary (keep looking)
 *		-2			Success: encapsulated binary: reread
 *		>0			Failure: error number
 *		EBADARCH		Mach-o binary, but with an unrecognized
 *					architecture
 *		ENOMEM			No memory for child process after -
 *					can only happen after vfork()
 *
 * Important:	This image activator is NOT byte order neutral.
 *
 * Note:	A return value other than -1 indicates subsequent image
 *		activators should not be given the opportunity to attempt
 *		to activate the image.
 */
static int
exec_mach_imgact(struct image_params *imgp)
{
	struct mach_header *mach_header = (struct mach_header *)imgp->ip_vdata;
	proc_t                  p = vfs_context_proc(imgp->ip_vfs_context);
	int                     error = 0;
	task_t                  task;
	task_t                  new_task = NULL;    /* protected by vfexec */
	thread_t                thread;
	struct uthread          *uthread;
	vm_map_switch_context_t switch_ctx;
	vm_map_t old_map = VM_MAP_NULL;
	vm_map_t map = VM_MAP_NULL;
	load_return_t           lret;
	load_result_t           load_result = {};
	struct _posix_spawnattr *psa = NULL;
	int                     spawn = (imgp->ip_flags & IMGPF_SPAWN);
	const int               vfexec = 0;
	int                     exec = (imgp->ip_flags & IMGPF_EXEC);
	os_reason_t             exec_failure_reason = OS_REASON_NULL;
	boolean_t               reslide = FALSE;
	char *                  userspace_coredump_name = NULL;

	/*
	 * make sure it's a Mach-O 1.0 or Mach-O 2.0 binary; the difference
	 * is a reserved field on the end, so for the most part, we can
	 * treat them as if they were identical. Reverse-endian Mach-O
	 * binaries are recognized but not compatible.
	 */
	if ((mach_header->magic == MH_CIGAM) ||
	    (mach_header->magic == MH_CIGAM_64)) {
		error = EBADARCH;
		goto bad;
	}

	if ((mach_header->magic != MH_MAGIC) &&
	    (mach_header->magic != MH_MAGIC_64)) {
		error = -1;
		goto bad;
	}

	if (mach_header->filetype != MH_EXECUTE) {
		error = -1;
		goto bad;
	}

	if (imgp->ip_origcputype != 0) {
		/* Fat header previously had an idea about this thin file */
		if (imgp->ip_origcputype != mach_header->cputype ||
		    imgp->ip_origcpusubtype != mach_header->cpusubtype) {
			error = EBADARCH;
			goto bad;
		}
	} else {
		imgp->ip_origcputype = mach_header->cputype;
		imgp->ip_origcpusubtype = mach_header->cpusubtype;
	}

	task = current_task();
	thread = current_thread();
	uthread = get_bsdthread_info(thread);

	if ((mach_header->cputype & CPU_ARCH_ABI64) == CPU_ARCH_ABI64) {
		imgp->ip_flags |= IMGPF_IS_64BIT_ADDR | IMGPF_IS_64BIT_DATA;
	}


	/* If posix_spawn binprefs exist, respect those prefs. */
	psa = (struct _posix_spawnattr *) imgp->ip_px_sa;
	if (psa != NULL && psa->psa_binprefs[0] != 0) {
		int pr = 0;
		for (pr = 0; pr < NBINPREFS; pr++) {
			cpu_type_t pref = psa->psa_binprefs[pr];
			cpu_subtype_t subpref = psa->psa_subcpuprefs[pr];

			if (pref == 0) {
				/* No suitable arch in the pref list */
				error = EBADARCH;
				goto bad;
			}

			if (pref == CPU_TYPE_ANY) {
				/* Jump to regular grading */
				goto grade;
			}

			if (binary_match(CPU_ARCH_MASK, pref, subpref,
			    imgp->ip_origcputype, imgp->ip_origcpusubtype)) {
				goto grade;
			}
		}
		error = EBADARCH;
		goto bad;
	}
grade:
	if (!grade_binary(imgp->ip_origcputype, imgp->ip_origcpusubtype & ~CPU_SUBTYPE_MASK,
	    imgp->ip_origcpusubtype & CPU_SUBTYPE_MASK, TRUE)) {
		error = EBADARCH;
		goto bad;
	}

	if (validate_potential_simulator_binary(imgp->ip_origcputype, imgp,
	    imgp->ip_arch_offset, imgp->ip_arch_size) != LOAD_SUCCESS) {
#if __x86_64__
		const char *excpath;
		error = exec_save_path(imgp, imgp->ip_user_fname, imgp->ip_seg, &excpath);
		os_log_error(OS_LOG_DEFAULT, "Unsupported 32-bit executable: \"%s\"", (error) ? imgp->ip_vp->v_name : excpath);
#endif
		error = EBADARCH;
		goto bad;
	}

#if defined(HAS_APPLE_PAC)
	assert(mach_header->cputype == CPU_TYPE_ARM64
	    );

	if ((mach_header->cputype == CPU_TYPE_ARM64 &&
	    arm64_cpusubtype_uses_ptrauth(mach_header->cpusubtype))
	    ) {
		imgp->ip_flags &= ~IMGPF_NOJOP;
	} else {
		imgp->ip_flags |= IMGPF_NOJOP;
	}
#endif

	/* Copy in arguments/environment from the old process */
	error = exec_extract_strings(imgp);
	if (error) {
		goto bad;
	}

	AUDIT_ARG(argv, imgp->ip_startargv, imgp->ip_argc,
	    imgp->ip_endargv - imgp->ip_startargv);
	AUDIT_ARG(envv, imgp->ip_endargv, imgp->ip_envc,
	    imgp->ip_endenvv - imgp->ip_endargv);

	/* reset local idea of thread, uthread, task */
	thread = imgp->ip_new_thread;
	uthread = get_bsdthread_info(thread);
	task = new_task = get_threadtask(thread);

	/*
	 *	Load the Mach-O file.
	 *
	 * NOTE: An error after this point  indicates we have potentially
	 * destroyed or overwritten some process state while attempting an
	 * execve() following a vfork(), which is an unrecoverable condition.
	 * We send the new process an immediate SIGKILL to avoid it executing
	 * any instructions in the mutated address space. For true spawns,
	 * this is not the case, and "too late" is still not too late to
	 * return an error code to the parent process.
	 */

	/*
	 * Actually load the image file we previously decided to load.
	 */
	lret = load_machfile(imgp, mach_header, thread, &map, &load_result);
	if (lret != LOAD_SUCCESS) {
		error = load_return_to_errno(lret);

		KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
		    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_BAD_MACHO, 0, 0);
		if (lret == LOAD_BADMACHO_UPX) {
			set_proc_name(imgp, p);
			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_UPX);
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
		} else {
			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_BAD_MACHO);

			if (bootarg_execfailurereports) {
				set_proc_name(imgp, p);
				exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			}
		}

		exec_failure_reason->osr_flags |= OS_REASON_FLAG_CONSISTENT_FAILURE;

		goto badtoolate;
	}

	assert(imgp->ip_free_map == NULL);


	// It's safe to check entitlements anytime after `load_machfile` if you check
	// based on the vnode in imgp. We must perform this entitlement check
	// before we start using load_result->hardened_runtime_binary further down
	load_result.hardened_runtime_binary = 0;
	encode_HR_entitlement(kCSWebBrowserHostEntitlement, BrowserHostEntitlementMask, imgp, &load_result);
	encode_HR_entitlement(kCSWebBrowserGPUEntitlement, BrowserGPUEntitlementMask, imgp, &load_result);
	encode_HR_entitlement(kCSWebBrowserNetworkEntitlement, BrowserNetworkEntitlementMask, imgp, &load_result);
	encode_HR_entitlement(kCSWebBrowserWebContentEntitlement, BrowserWebContentEntitlementMask, imgp, &load_result);

	/*
	 * ERROR RECOVERY
	 *
	 * load_machfile() returned the new VM map ("map") but we haven't
	 * committed to it yet.
	 * Any error path between here and the point where we commit to using
	 * the new "map" (with swap_task_map()) should deallocate "map".
	 */

#ifndef KASAN
	/*
	 * Security: zone sanity checks on fresh boot or initproc re-exec.
	 * launchd by design does not tear down its own service port on USR (rdar://72797967),
	 * which means here is the earliest point we can assert on empty service port label zone,
	 * after load_machfile() above terminates old launchd's IPC space.
	 *
	 * Disable on KASAN builds since zone_size_allocated() accounts for elements
	 * under quarantine.
	 */
	if (task_pid(task) == 1) {
		zone_userspace_reboot_checks();
	}
#endif

	proc_lock(p);
	p->p_cputype = imgp->ip_origcputype;
	p->p_cpusubtype = imgp->ip_origcpusubtype;
	proc_setplatformdata(p, load_result.ip_platform, load_result.lr_min_sdk, load_result.lr_sdk);

	vm_map_set_size_limit(map, proc_limitgetcur(p, RLIMIT_AS));
	vm_map_set_data_limit(map, proc_limitgetcur(p, RLIMIT_DATA));
	vm_map_set_user_wire_limit(map, (vm_size_t)proc_limitgetcur(p, RLIMIT_MEMLOCK));

#if XNU_TARGET_OS_OSX
	if (proc_platform(p) == PLATFORM_IOS) {
		assert(vm_map_is_alien(map));
	} else {
		assert(!vm_map_is_alien(map));
	}
#endif /* XNU_TARGET_OS_OSX */
	proc_unlock(p);

	/*
	 * Setup runtime mitigations.
	 */
	if ((error = exec_setup_runtime_mitigations(imgp, &load_result, current_task(), new_task, map, p)) != 0) {
		set_proc_name(imgp, p);
		exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_BAD_MACHO);
		if (bootarg_execfailurereports) {
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_CONSISTENT_FAILURE;
		}
		/* release new address space since we won't use it */
		imgp->ip_free_map = map;
		map = VM_MAP_NULL;
		goto badtoolate;
	}

	/*
	 * Set code-signing flags if this binary is signed, or if parent has
	 * requested them on exec.
	 */
	if (load_result.csflags & CS_VALID) {
		imgp->ip_csflags |= load_result.csflags &
		    (CS_VALID | CS_SIGNED | CS_DEV_CODE | CS_LINKER_SIGNED |
		    CS_HARD | CS_KILL | CS_RESTRICT | CS_ENFORCEMENT | CS_REQUIRE_LV |
		    CS_FORCED_LV | CS_ENTITLEMENTS_VALIDATED | CS_NO_UNTRUSTED_HELPERS | CS_RUNTIME |
		    CS_ENTITLEMENT_FLAGS |
		    CS_EXEC_SET_HARD | CS_EXEC_SET_KILL | CS_EXEC_SET_ENFORCEMENT);
	} else {
		imgp->ip_csflags &= ~CS_VALID;
	}

	if (proc_getcsflags(p) & CS_EXEC_SET_HARD) {
		imgp->ip_csflags |= CS_HARD;
	}
	if (proc_getcsflags(p) & CS_EXEC_SET_KILL) {
		imgp->ip_csflags |= CS_KILL;
	}
	if (proc_getcsflags(p) & CS_EXEC_SET_ENFORCEMENT) {
		imgp->ip_csflags |= CS_ENFORCEMENT;
	}
	if (proc_getcsflags(p) & CS_EXEC_INHERIT_SIP) {
		if (proc_getcsflags(p) & CS_INSTALLER) {
			imgp->ip_csflags |= CS_INSTALLER;
		}
		if (proc_getcsflags(p) & CS_DATAVAULT_CONTROLLER) {
			imgp->ip_csflags |= CS_DATAVAULT_CONTROLLER;
		}
		if (proc_getcsflags(p) & CS_NVRAM_UNRESTRICTED) {
			imgp->ip_csflags |= CS_NVRAM_UNRESTRICTED;
		}
	}

#if __has_feature(ptrauth_calls) && defined(XNU_TARGET_OS_OSX)
	/*
	 * ptrauth version 0 is a preview ABI.  Developers can opt into running
	 * their own arm64e binaries for local testing, with the understanding
	 * that future OSes may break ABI.
	 */
	if ((imgp->ip_origcpusubtype & ~CPU_SUBTYPE_MASK) == CPU_SUBTYPE_ARM64E &&
	    CPU_SUBTYPE_ARM64_PTR_AUTH_VERSION(imgp->ip_origcpusubtype) == 0 &&
	    !load_result.platform_binary &&
	    !bootarg_arm64e_preview_abi) {
		static bool logged_once = false;
		set_proc_name(imgp, p);

		printf("%s: not running binary \"%s\" built against preview arm64e ABI\n", __func__, p->p_name);
		if (!os_atomic_xchg(&logged_once, true, relaxed)) {
			printf("%s: (to allow this, add \"-arm64e_preview_abi\" to boot-args)\n", __func__);
		}

		exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_BAD_MACHO);
		if (bootarg_execfailurereports) {
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_CONSISTENT_FAILURE;
		}

		/* release new address space since we won't use it */
		imgp->ip_free_map = map;
		map = VM_MAP_NULL;
		goto badtoolate;
	}

	if ((imgp->ip_origcpusubtype & ~CPU_SUBTYPE_MASK) != CPU_SUBTYPE_ARM64E &&
	    imgp->ip_origcputype == CPU_TYPE_ARM64 &&
	    load_result.platform_binary &&
	    (imgp->ip_flags & IMGPF_DRIVER) != 0) {
		set_proc_name(imgp, p);
		printf("%s: disallowing arm64 platform driverkit binary \"%s\", should be arm64e\n", __func__, p->p_name);
		exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_BAD_MACHO);
		if (bootarg_execfailurereports) {
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_CONSISTENT_FAILURE;
		}

		/* release new address space since we won't use it */
		imgp->ip_free_map = map;
		map = VM_MAP_NULL;
		goto badtoolate;
	}
#endif /* __has_feature(ptrauth_calls) && defined(XNU_TARGET_OS_OSX) */



	/*
	 * Set up the shared cache region in the new process.
	 *
	 * Normally there is a single shared region per architecture.
	 * However on systems with Pointer Authentication, we can create
	 * multiple shared caches with the amount of sharing determined
	 * by team-id or entitlement. Inherited shared region IDs are used
	 * for system processes that need to match and be able to inspect
	 * a pre-existing task.
	 */
	int cpu_subtype = 0;     /* all cpu_subtypes use the same shared region */
#if __has_feature(ptrauth_calls)
	char *shared_region_id = NULL;
	size_t len;
	char *base;
	const char *cbase;
#define HARDENED_RUNTIME_CONTENT_ID "C-"
#define TEAM_ID_PREFIX "T-"
#define ENTITLE_PREFIX "E-"
#define SR_PREFIX_LEN  2
#define SR_ENTITLEMENT "com.apple.pac.shared_region_id"

	if (cpu_type() == CPU_TYPE_ARM64 &&
	    arm64_cpusubtype_uses_ptrauth(p->p_cpusubtype) &&
	    (imgp->ip_flags & IMGPF_NOJOP) == 0) {
		assertf(p->p_cputype == CPU_TYPE_ARM64,
		    "p %p cpu_type() 0x%x p->p_cputype 0x%x p->p_cpusubtype 0x%x",
		    p, cpu_type(), p->p_cputype, p->p_cpusubtype);

		/*
		 * arm64e uses pointer authentication, so request a separate
		 * shared region for this CPU subtype.
		 */
		cpu_subtype = p->p_cpusubtype & ~CPU_SUBTYPE_MASK;

		/*
		 * Determine which shared cache to select based on being told,
		 * matching a team-id or matching an entitlement.
		 */
		if (load_result.hardened_runtime_binary & BrowserWebContentEntitlementMask) {
			len = sizeof(HARDENED_RUNTIME_CONTENT_ID);
			shared_region_id = kalloc_data(len, Z_WAITOK | Z_NOFAIL);
			strlcpy(shared_region_id, HARDENED_RUNTIME_CONTENT_ID, len);
		} else if (imgp->ip_inherited_shared_region_id) {
			len = strlen(imgp->ip_inherited_shared_region_id);
			shared_region_id = kalloc_data(len + 1, Z_WAITOK | Z_NOFAIL);
			memcpy(shared_region_id, imgp->ip_inherited_shared_region_id, len + 1);
		} else if ((cbase = get_teamid_for_shared_region(imgp)) != NULL) {
			len = strlen(cbase);
			if (vm_shared_region_per_team_id) {
				shared_region_id = kalloc_data(len + SR_PREFIX_LEN + 1,
				    Z_WAITOK | Z_NOFAIL);
				memcpy(shared_region_id, TEAM_ID_PREFIX, SR_PREFIX_LEN);
				memcpy(shared_region_id + SR_PREFIX_LEN, cbase, len + 1);
			}
		} else if ((base = IOVnodeGetEntitlement(imgp->ip_vp,
		    (int64_t)imgp->ip_arch_offset, SR_ENTITLEMENT)) != NULL) {
			len = strlen(base);
			if (vm_shared_region_by_entitlement) {
				shared_region_id = kalloc_data(len + SR_PREFIX_LEN + 1,
				    Z_WAITOK | Z_NOFAIL);
				memcpy(shared_region_id, ENTITLE_PREFIX, SR_PREFIX_LEN);
				memcpy(shared_region_id + SR_PREFIX_LEN, base, len + 1);
			}
			/* Discard the copy of the entitlement */
			kfree_data(base, len + 1);
		}
	}

	if (imgp->ip_flags & IMGPF_RESLIDE) {
		reslide = TRUE;
	}

	/* use "" as the default shared_region_id */
	if (shared_region_id == NULL) {
		shared_region_id = kalloc_data(1, Z_WAITOK | Z_ZERO | Z_NOFAIL);
	}

	/* ensure there's a unique pointer signing key for this shared_region_id */
	shared_region_key_alloc(shared_region_id,
	    imgp->ip_inherited_shared_region_id != NULL, imgp->ip_inherited_jop_pid);
	task_set_shared_region_id(task, shared_region_id);
	shared_region_id = NULL;
#endif /* __has_feature(ptrauth_calls) */

#if CONFIG_ROSETTA
	if (imgp->ip_flags & IMGPF_ROSETTA) {
		OSBitOrAtomic(P_TRANSLATED, &p->p_flag);
	} else if (p->p_flag & P_TRANSLATED) {
		OSBitAndAtomic(~P_TRANSLATED, &p->p_flag);
	}
#endif

	int cputype = cpu_type();

	uint32_t rsr_version = 0;
#if XNU_TARGET_OS_OSX
	if (vnode_is_rsr(imgp->ip_vp)) {
		rsr_version = rsr_get_version();
		os_atomic_or(&p->p_ladvflag, P_RSR, relaxed);
		os_atomic_or(&p->p_vfs_iopolicy, P_VFS_IOPOLICY_ALTLINK, relaxed);
	}
#endif /* XNU_TARGET_OS_OSX */

	error = vm_map_exec(map, task, load_result.is_64bit_addr,
	    (void *)p->p_fd.fd_rdir, cputype, cpu_subtype, reslide,
	    (imgp->ip_flags & IMGPF_DRIVER) != 0,
	    rsr_version);

	if (error) {
		KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
		    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_MAP_EXEC_FAILURE, 0, 0);

		exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_MAP_EXEC_FAILURE);
		if (bootarg_execfailurereports) {
			set_proc_name(imgp, p);
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_CONSISTENT_FAILURE;
		}
		/* release new address space since we won't use it */
		imgp->ip_free_map = map;
		map = VM_MAP_NULL;
		goto badtoolate;
	}

	/*
	 * Close file descriptors which specify close-on-exec.
	 */
	fdt_exec(p, vfs_context_ucred(imgp->ip_vfs_context),
	    psa != NULL ? psa->psa_flags : 0, imgp->ip_new_thread, exec);

	/*
	 * deal with set[ug]id.
	 */
	error = exec_handle_sugid(imgp);
	if (error) {
		KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
		    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_SUGID_FAILURE, 0, 0);

		exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_SUGID_FAILURE);
		if (bootarg_execfailurereports) {
			set_proc_name(imgp, p);
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
		}

		/* release new address space since we won't use it */
		imgp->ip_free_map = map;
		map = VM_MAP_NULL;
		goto badtoolate;
	}

	/*
	 * Commit to new map.
	 *
	 * Swap the new map for the old for target task, which consumes
	 * our new map reference but each leaves us responsible for the
	 * old_map reference.  That lets us get off the pmap associated
	 * with it, and then we can release it.
	 *
	 * The map needs to be set on the target task which is different
	 * than current task, thus swap_task_map is used instead of
	 * vm_map_switch.
	 */
	old_map = swap_task_map(task, thread, map);
#if MACH_ASSERT
	/*
	 * Reset the pmap's process info to prevent ledger checks
	 * which might fail due to the ledgers being shared between
	 * the old and new pmaps.
	 */
	vm_map_pmap_set_process(old_map, -1, "<old_map>");
#endif /* MACH_ASSERT */
	imgp->ip_free_map = old_map;
	old_map = NULL;

	lret = activate_exec_state(task, p, thread, &load_result);
	if (lret != KERN_SUCCESS) {
		KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
		    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_ACTV_THREADSTATE, 0, 0);

		exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_ACTV_THREADSTATE);
		if (bootarg_execfailurereports) {
			set_proc_name(imgp, p);
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
		}

		goto badtoolate;
	}

	/*
	 * deal with voucher on exec-calling thread.
	 */
	if (imgp->ip_new_thread == NULL) {
		thread_set_mach_voucher(current_thread(), IPC_VOUCHER_NULL);
	}

	/* Make sure we won't interrupt ourself signalling a partial process */
	if (!vfexec && !spawn && (p->p_lflag & P_LTRACED)) {
		psignal(p, SIGTRAP);
	}

	if (load_result.unixproc &&
	    create_unix_stack(get_task_map(task),
	    &load_result,
	    p) != KERN_SUCCESS) {
		error = load_return_to_errno(LOAD_NOSPACE);

		KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
		    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_STACK_ALLOC, 0, 0);

		exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_STACK_ALLOC);
		if (bootarg_execfailurereports) {
			set_proc_name(imgp, p);
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
		}

		goto badtoolate;
	}

	if (load_result.hardened_runtime_binary) {
		if (cs_debug) {
			printf("setting hardened runtime with entitlement mask= "
			    "0x%x on task: pid = %d\n",
			    load_result.hardened_runtime_binary,
			    proc_getpid(p));
		}
		task_set_hardened_runtime(task, true);
	}

	/*
	 * The load result will have already been munged by AMFI to include the
	 * platform binary flag if boot-args dictated it (AMFI will mark anything
	 * that doesn't go through the upcall path as a platform binary if its
	 * enforcement is disabled).
	 */
	if (load_result.platform_binary) {
		if (cs_debug) {
			printf("setting platform binary on task: pid = %d\n", proc_getpid(p));
		}

		/*
		 * We must use 'task' here because the proc's task has not yet been
		 * switched to the new one.
		 */
		task_set_platform_binary(task, TRUE);
	} else {
		if (cs_debug) {
			printf("clearing platform binary on task: pid = %d\n", proc_getpid(p));
		}

		task_set_platform_binary(task, FALSE);
	}

#if XNU_TARGET_OS_OSX
	/* Disable mach hardening for all 1P tasks which load 3P plugins */
	if (imgp->ip_flags & IMGPF_3P_PLUGINS) {
		if (cs_debug) {
			printf("Disabling some mach hardening on task due to 3P plugins: pid = %d\n", proc_getpid(p));
		}
		task_disable_mach_hardening(task);
	}
#if DEVELOPMENT || DEBUG
	/* Disable mach hardening for all tasks if amfi_get_out_of_my_way is set.
	 * Customers will have to turn SIP off to use this boot-arg, and so this is
	 * only needed internally since we disable this feature when SIP is off. */
	if (AMFI_bootarg_disable_mach_hardening) {
		if (cs_debug) {
			printf("Disabling some mach hardening on task due to AMFI boot-args: pid = %d\n", proc_getpid(p));
		}
		task_disable_mach_hardening(task);
	}
#endif /* DEVELOPMENT || DEBUG */
#endif /* XNU_TARGET_OS_OSX */

	/*
	 * Set starting EXC_GUARD and control port behavior for task now that
	 * platform and hardened runtime is set. Use the name directly from imgp since we haven't
	 * set_proc_name() yet. Also make control port for the task and main thread
	 * immovable/pinned based on task's option.
	 *
	 * Must happen before main thread port copyout in exc_add_apple_strings.
	 */
	task_set_exc_guard_ctrl_port_default(task, thread,
	    imgp->ip_ndp->ni_cnd.cn_nameptr,
	    (unsigned)imgp->ip_ndp->ni_cnd.cn_namelen,
	    proc_is_simulated(p),
	    load_result.ip_platform,
	    load_result.lr_sdk);

	error = exec_add_apple_strings(imgp, &load_result, task);     /* copies out main thread port */

	if (error) {
		KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
		    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_APPLE_STRING_INIT, 0, 0);

		exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_APPLE_STRING_INIT);
		if (bootarg_execfailurereports) {
			set_proc_name(imgp, p);
			exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
		}
		goto badtoolate;
	}

	/* Switch to target task's map to copy out strings */
	switch_ctx = vm_map_switch_to(get_task_map(task));

	if (load_result.unixproc) {
		user_addr_t     ap;

		/*
		 * Copy the strings area out into the new process address
		 * space.
		 */
		ap = p->user_stack;
		error = exec_copyout_strings(imgp, &ap);
		if (error) {
			vm_map_switch_back(switch_ctx);

			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_COPYOUT_STRINGS, 0, 0);

			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_COPYOUT_STRINGS);
			if (bootarg_execfailurereports) {
				set_proc_name(imgp, p);
				exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			}
			goto badtoolate;
		}
		/* Set the stack */
		thread_setuserstack(thread, ap);
	}

	if (load_result.dynlinker || load_result.is_rosetta) {
		user_addr_t        ap;
		int                     new_ptr_size = (imgp->ip_flags & IMGPF_IS_64BIT_ADDR) ? 8 : 4;

		/* Adjust the stack */
		ap = thread_adjuserstack(thread, -new_ptr_size);
		error = copyoutptr(load_result.mach_header, ap, new_ptr_size);

		if (error) {
			vm_map_switch_back(switch_ctx);

			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_COPYOUT_DYNLINKER, 0, 0);

			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_COPYOUT_DYNLINKER);
			if (bootarg_execfailurereports) {
				set_proc_name(imgp, p);
				exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			}
			goto badtoolate;
		}
		error = task_set_dyld_info(task, load_result.all_image_info_addr,
		    load_result.all_image_info_size, false);
		if (error) {
			vm_map_switch_back(switch_ctx);

			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_SET_DYLD_INFO, 0, 0);

			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_SET_DYLD_INFO);
			if (bootarg_execfailurereports) {
				set_proc_name(imgp, p);
				exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			}
			error = EINVAL;
			goto badtoolate;
		}
	} else {
		/*
		 * No dyld or rosetta loaded, set the TF_DYLD_ALL_IMAGE_FINAL bit on task.
		 */
		error = task_set_dyld_info(task, MACH_VM_MIN_ADDRESS,
		    0, true);
		if (error) {
			vm_map_switch_back(switch_ctx);

			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_SET_DYLD_INFO, 0, 0);

			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_SET_DYLD_INFO);
			if (bootarg_execfailurereports) {
				set_proc_name(imgp, p);
				exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			}
			error = EINVAL;
			goto badtoolate;
		}
	}

#if CONFIG_ROSETTA
	if (load_result.is_rosetta) {
		// Add an fd for the executable file for Rosetta's use
		int main_binary_fd;
		struct fileproc *fp;

		error = falloc_exec(p, imgp->ip_vfs_context, &fp, &main_binary_fd);
		if (error) {
			vm_map_switch_back(switch_ctx);

			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_MAIN_FD_ALLOC, 0, 0);

			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_MAIN_FD_ALLOC);
			if (bootarg_execfailurereports) {
				set_proc_name(imgp, p);
				exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			}
			goto badtoolate;
		}

		error = VNOP_OPEN(imgp->ip_vp, FREAD, imgp->ip_vfs_context);
		if (error) {
			vm_map_switch_back(switch_ctx);

			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_MAIN_FD_ALLOC, 0, 0);

			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_MAIN_FD_ALLOC);
			if (bootarg_execfailurereports) {
				set_proc_name(imgp, p);
				exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			}
			goto cleanup_rosetta_fp;
		}

		fp->fp_glob->fg_flag = FREAD;
		fp->fp_glob->fg_ops = &vnops;
		fp_set_data(fp, imgp->ip_vp);

		proc_fdlock(p);
		procfdtbl_releasefd(p, main_binary_fd, NULL);
		fp_drop(p, main_binary_fd, fp, 1);
		proc_fdunlock(p);

		vnode_ref(imgp->ip_vp);

		// Pass the dyld load address, main binary fd, and dyld fd on the stack
		uint64_t ap = thread_adjuserstack(thread, -24);

		error = copyoutptr((user_addr_t)load_result.dynlinker_fd, ap, 8);
		if (error) {
			vm_map_switch_back(switch_ctx);

			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_COPYOUT_ROSETTA, 0, 0);

			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_COPYOUT_ROSETTA);
			if (bootarg_execfailurereports) {
				set_proc_name(imgp, p);
				exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			}
			goto cleanup_rosetta_fp;
		}

		error = copyoutptr(load_result.dynlinker_mach_header, ap + 8, 8);
		if (error) {
			vm_map_switch_back(switch_ctx);

			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_COPYOUT_ROSETTA, 0, 0);

			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_COPYOUT_ROSETTA);
			if (bootarg_execfailurereports) {
				set_proc_name(imgp, p);
				exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			}
			goto cleanup_rosetta_fp;
		}

		error = copyoutptr((user_addr_t)main_binary_fd, ap + 16, 8);
		if (error) {
			vm_map_switch_back(switch_ctx);

			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_COPYOUT_ROSETTA, 0, 0);

			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_COPYOUT_ROSETTA);
			if (bootarg_execfailurereports) {
				set_proc_name(imgp, p);
				exec_failure_reason->osr_flags |= OS_REASON_FLAG_GENERATE_CRASH_REPORT;
			}
			goto cleanup_rosetta_fp;
		}

cleanup_rosetta_fp:
		if (error) {
			fp_free(p, load_result.dynlinker_fd, load_result.dynlinker_fp);
			fp_free(p, main_binary_fd, fp);
			goto badtoolate;
		}
	}

#endif

	/* Avoid immediate VM faults back into kernel */
	exec_prefault_data(p, imgp, &load_result);

	vm_map_switch_back(switch_ctx);

	/*
	 * Reset signal state.
	 */
	execsigs(p, thread);

	/*
	 * need to cancel async IO requests that can be cancelled and wait for those
	 * already active.  MAY BLOCK!
	 */
	_aio_exec( p );

#if SYSV_SHM
	/* FIXME: Till vmspace inherit is fixed: */
	if (!vfexec && p->vm_shm) {
		shmexec(p);
	}
#endif
#if SYSV_SEM
	/* Clean up the semaphores */
	semexit(p);
#endif

	/*
	 * Remember file name for accounting.
	 */
	p->p_acflag &= ~AFORK;

	set_proc_name(imgp, p);

#if CONFIG_SECLUDED_MEMORY
	if (secluded_for_apps &&
	    load_result.platform_binary) {
		if (strncmp(p->p_name,
		    "Camera",
		    sizeof(p->p_name)) == 0) {
			task_set_could_use_secluded_mem(task, TRUE);
		} else {
			task_set_could_use_secluded_mem(task, FALSE);
		}
		if (strncmp(p->p_name,
		    "mediaserverd",
		    sizeof(p->p_name)) == 0) {
			task_set_could_also_use_secluded_mem(task, TRUE);
		}
		if (strncmp(p->p_name,
		    "cameracaptured",
		    sizeof(p->p_name)) == 0) {
			task_set_could_also_use_secluded_mem(task, TRUE);
		}
	}
#endif /* CONFIG_SECLUDED_MEMORY */

#if __arm64__
	if (load_result.legacy_footprint) {
		task_set_legacy_footprint(task);
	}
#endif /* __arm64__ */

	pal_dbg_set_task_name(task);

#if DEVELOPMENT || DEBUG
	/*
	 * Update the pid an proc name for importance base if any
	 */
	task_importance_update_owner_info(task);
#endif

	proc_setexecutableuuid(p, &load_result.uuid[0]);

#if CONFIG_DTRACE
	dtrace_proc_exec(p);
#endif

	if (kdebug_enable) {
		long args[4] = {};

		uintptr_t fsid = 0, fileid = 0;
		if (imgp->ip_vattr) {
			uint64_t fsid64 = vnode_get_va_fsid(imgp->ip_vattr);
			fsid   = (uintptr_t)fsid64;
			fileid = (uintptr_t)imgp->ip_vattr->va_fileid;
			// check for (unexpected) overflow and trace zero in that case
			if (fsid != fsid64 || fileid != imgp->ip_vattr->va_fileid) {
				fsid = fileid = 0;
			}
		}
		KERNEL_DEBUG_CONSTANT_IST1(TRACE_DATA_EXEC, proc_getpid(p), fsid, fileid, 0,
		    (uintptr_t)thread_tid(thread));

		extern void kdebug_proc_name_args(struct proc *proc, long args[static 4]);
		kdebug_proc_name_args(p, args);
		KERNEL_DEBUG_CONSTANT_IST1(TRACE_STRING_EXEC, args[0], args[1],
		    args[2], args[3], (uintptr_t)thread_tid(thread));
	}


	/*
	 * If posix_spawned with the START_SUSPENDED flag, stop the
	 * process before it runs.
	 */
	if (imgp->ip_px_sa != NULL) {
		psa = (struct _posix_spawnattr *) imgp->ip_px_sa;
		if (psa->psa_flags & POSIX_SPAWN_START_SUSPENDED) {
			proc_lock(p);
			p->p_stat = SSTOP;
			proc_unlock(p);
			(void) task_suspend_internal(task);
		}
	}

	/*
	 * mark as execed
	 */
	OSBitOrAtomic(P_EXEC, &p->p_flag);
	proc_resetregister(p);
	if (p->p_pptr && (p->p_lflag & P_LPPWAIT)) {
		proc_lock(p);
		p->p_lflag &= ~P_LPPWAIT;
		proc_unlock(p);
		wakeup((caddr_t)p->p_pptr);
	}

	/*
	 * Set up dext coredumps on kernel panic.
	 * This requires the following:
	 * - dext_panic_coredump=1 boot-arg (enabled by default on DEVELOPMENT, DEBUG and certain Seed builds)
	 * - process must be a driver
	 * - process must have the com.apple.private.enable-coredump-on-panic entitlement, and the
	 *   entitlement has a string value.
	 * - process must have the com.apple.private.enable-coredump-on-panic-seed-privacy-approved
	 *   entitlement (Seed builds only).
	 *
	 * The core dump file name is formatted with the entitlement string value, followed by a hyphen
	 * and the process PID.
	 */
	if (enable_dext_coredumps_on_panic &&
	    (imgp->ip_flags & IMGPF_DRIVER) != 0 &&
	    (userspace_coredump_name = IOVnodeGetEntitlement(imgp->ip_vp,
	    (int64_t)imgp->ip_arch_offset, USERSPACE_COREDUMP_PANIC_ENTITLEMENT)) != NULL) {
		size_t userspace_coredump_name_len = strlen(userspace_coredump_name);

		char core_name[MACH_CORE_FILEHEADER_NAMELEN];
		/* 16 - NULL char - strlen("-") - maximum of 5 digits for pid */
		snprintf(core_name, MACH_CORE_FILEHEADER_NAMELEN, "%.9s-%d", userspace_coredump_name, proc_getpid(p));

		kern_register_userspace_coredump(task, core_name);

		/* Discard the copy of the entitlement */
		kfree_data(userspace_coredump_name, userspace_coredump_name_len + 1);
		userspace_coredump_name = NULL;
	}

	goto done;

badtoolate:
	/* Don't allow child process to execute any instructions */
	if (!spawn) {
		{
			assert(exec_failure_reason != OS_REASON_NULL);
			if (bootarg_execfailurereports) {
				set_proc_name(imgp, current_proc());
			}
			psignal_with_reason(current_proc(), SIGKILL, exec_failure_reason);
			exec_failure_reason = OS_REASON_NULL;

			if (exec) {
				/* Terminate the exec copy task */
				task_terminate_internal(task);
			}
		}

		/* We can't stop this system call at this point, so just pretend we succeeded */
		error = 0;
	} else {
		os_reason_free(exec_failure_reason);
		exec_failure_reason = OS_REASON_NULL;
	}

done:
	if (load_result.threadstate) {
		kfree_data(load_result.threadstate, load_result.threadstate_sz);
		load_result.threadstate = NULL;
	}

bad:
	/* If we hit this, we likely would have leaked an exit reason */
	assert(exec_failure_reason == OS_REASON_NULL);
	return error;
}




/*
 * Our image activator table; this is the table of the image types we are
 * capable of loading.  We list them in order of preference to ensure the
 * fastest image load speed.
 *
 * XXX hardcoded, for now; should use linker sets
 */
struct execsw {
	int(*const ex_imgact)(struct image_params *);
	const char *ex_name;
}const execsw[] = {
	{ exec_mach_imgact, "Mach-o Binary" },
	{ exec_fat_imgact, "Fat Binary" },
	{ exec_shell_imgact, "Interpreter Script" },
	{ NULL, NULL}
};


/*
 * exec_activate_image
 *
 * Description:	Iterate through the available image activators, and activate
 *		the image associated with the imgp structure.  We start with
 *		the activator for Mach-o binaries followed by that for Fat binaries
 *		for Interpreter scripts.
 *
 * Parameters:	struct image_params *	Image parameter block
 *
 * Returns:	0			Success
 *		ENOEXEC			No activator for image.
 *		EBADEXEC		The executable is corrupt/unknown
 *	execargs_alloc:EINVAL		Invalid argument
 *	execargs_alloc:EACCES		Permission denied
 *	execargs_alloc:EINTR		Interrupted function
 *	execargs_alloc:ENOMEM		Not enough space
 *	exec_save_path:EFAULT		Bad address
 *	exec_save_path:ENAMETOOLONG	Filename too long
 *	exec_check_permissions:EACCES	Permission denied
 *	exec_check_permissions:ENOEXEC	Executable file format error
 *	exec_check_permissions:ETXTBSY	Text file busy [misuse of error code]
 *	exec_check_permissions:???
 *	namei:???
 *	vn_rdwr:???			[anything vn_rdwr can return]
 *	<ex_imgact>:???			[anything an imgact can return]
 *	EDEADLK				Process is being terminated
 */
static int
exec_activate_image(struct image_params *imgp)
{
	struct nameidata *ndp = NULL;
	const char *excpath;
	int error;
	int resid;
	int once = 1;   /* save SGUID-ness for interpreted files */
	int i;
	int itercount = 0;
	proc_t p = vfs_context_proc(imgp->ip_vfs_context);

	/*
	 * For exec, the translock needs to be taken on old proc and not
	 * on new shadow proc.
	 */
	if (imgp->ip_flags & IMGPF_EXEC) {
		p = current_proc();
	}

	error = execargs_alloc(imgp);
	if (error) {
		goto bad_notrans;
	}

	error = exec_save_path(imgp, imgp->ip_user_fname, imgp->ip_seg, &excpath);
	if (error) {
		goto bad_notrans;
	}

	/* Use excpath, which contains the copyin-ed exec path */
	DTRACE_PROC1(exec, uintptr_t, excpath);

	ndp = kalloc_type(struct nameidata, Z_WAITOK | Z_ZERO | Z_NOFAIL);

	NDINIT(ndp, LOOKUP, OP_LOOKUP, FOLLOW | LOCKLEAF | AUDITVNPATH1,
	    UIO_SYSSPACE, CAST_USER_ADDR_T(excpath), imgp->ip_vfs_context);

again:
	error = namei(ndp);
	if (error) {
		if (error == ERESTART) {
			error = EINTR;
		}
		goto bad_notrans;
	}
	imgp->ip_ndp = ndp;     /* successful namei(); call nameidone() later */
	imgp->ip_vp = ndp->ni_vp;       /* if set, need to vnode_put() at some point */

	/*
	 * Before we start the transition from binary A to binary B, make
	 * sure another thread hasn't started exiting the process.  We grab
	 * the proc lock to check p_lflag initially, and the transition
	 * mechanism ensures that the value doesn't change after we release
	 * the lock.
	 */
	proc_lock(p);
	if (p->p_lflag & P_LEXIT) {
		error = EDEADLK;
		proc_unlock(p);
		goto bad_notrans;
	}
	error = proc_transstart(p, 1, 0);
	proc_unlock(p);
	if (error) {
		goto bad_notrans;
	}

	error = exec_check_permissions(imgp);
	if (error) {
		goto bad;
	}

	/* Copy; avoid invocation of an interpreter overwriting the original */
	if (once) {
		once = 0;
		*imgp->ip_origvattr = *imgp->ip_vattr;
	}

	error = vn_rdwr(UIO_READ, imgp->ip_vp, imgp->ip_vdata, PAGE_SIZE, 0,
	    UIO_SYSSPACE, IO_NODELOCKED,
	    vfs_context_ucred(imgp->ip_vfs_context),
	    &resid, vfs_context_proc(imgp->ip_vfs_context));
	if (error) {
		goto bad;
	}

	if (resid) {
		memset(imgp->ip_vdata + (PAGE_SIZE - resid), 0x0, resid);
	}

encapsulated_binary:
	/* Limit the number of iterations we will attempt on each binary */
	if (++itercount > EAI_ITERLIMIT) {
		error = EBADEXEC;
		goto bad;
	}
	error = -1;
	for (i = 0; error == -1 && execsw[i].ex_imgact != NULL; i++) {
		error = (*execsw[i].ex_imgact)(imgp);

		switch (error) {
		/* case -1: not claimed: continue */
		case -2:                /* Encapsulated binary, imgp->ip_XXX set for next iteration */
			goto encapsulated_binary;

		case -3:                /* Interpreter */
#if CONFIG_MACF
			/*
			 * Copy the script label for later use. Note that
			 * the label can be different when the script is
			 * actually read by the interpreter.
			 */
			if (imgp->ip_scriptlabelp) {
				mac_vnode_label_free(imgp->ip_scriptlabelp);
				imgp->ip_scriptlabelp = NULL;
			}
			imgp->ip_scriptlabelp = mac_vnode_label_alloc(NULL);
			if (imgp->ip_scriptlabelp == NULL) {
				error = ENOMEM;
				break;
			}
			mac_vnode_label_copy(mac_vnode_label(imgp->ip_vp),
			    imgp->ip_scriptlabelp);

			/*
			 * Take a ref of the script vnode for later use.
			 */
			if (imgp->ip_scriptvp) {
				vnode_put(imgp->ip_scriptvp);
				imgp->ip_scriptvp = NULLVP;
			}
			if (vnode_getwithref(imgp->ip_vp) == 0) {
				imgp->ip_scriptvp = imgp->ip_vp;
			}
#endif

			nameidone(ndp);

			vnode_put(imgp->ip_vp);
			imgp->ip_vp = NULL;     /* already put */
			imgp->ip_ndp = NULL; /* already nameidone */

			/* Use excpath, which exec_shell_imgact reset to the interpreter */
			NDINIT(ndp, LOOKUP, OP_LOOKUP, FOLLOW | LOCKLEAF,
			    UIO_SYSSPACE, CAST_USER_ADDR_T(excpath), imgp->ip_vfs_context);

			proc_transend(p, 0);
			goto again;

		default:
			break;
		}
	}

	if (error == -1) {
		error = ENOEXEC;
	} else if (error == 0) {
		if (imgp->ip_flags & IMGPF_INTERPRET && ndp->ni_vp) {
			AUDIT_ARG(vnpath, ndp->ni_vp, ARG_VNODE2);
		}

		/*
		 * Call out to allow 3rd party notification of exec.
		 * Ignore result of kauth_authorize_fileop call.
		 */
		if (kauth_authorize_fileop_has_listeners()) {
			kauth_authorize_fileop(vfs_context_ucred(imgp->ip_vfs_context),
			    KAUTH_FILEOP_EXEC,
			    (uintptr_t)ndp->ni_vp, 0);
		}
	}
bad:
	proc_transend(p, 0);

bad_notrans:
	if (imgp->ip_strings) {
		execargs_free(imgp);
	}
	if (imgp->ip_ndp) {
		nameidone(imgp->ip_ndp);
	}
	kfree_type(struct nameidata, ndp);

	return error;
}


/*
 * exec_validate_spawnattr_policy
 *
 * Description: Validates the entitlements required to set the apptype.
 *
 * Parameters:  int psa_apptype         posix spawn attribute apptype
 *
 * Returns:     0                       Success
 *              EPERM                   Failure
 */
static errno_t
exec_validate_spawnattr_policy(int psa_apptype)
{
	if ((psa_apptype & POSIX_SPAWN_PROC_TYPE_MASK) != 0) {
		int proctype = psa_apptype & POSIX_SPAWN_PROC_TYPE_MASK;
		if (proctype == POSIX_SPAWN_PROC_TYPE_DRIVER) {
			if (!IOCurrentTaskHasEntitlement(POSIX_SPAWN_ENTITLEMENT_DRIVER)) {
				return EPERM;
			}
		}
	}

	return 0;
}

/*
 * exec_handle_spawnattr_policy
 *
 * Description: Decode and apply the posix_spawn apptype, qos clamp, and watchport ports to the task.
 *
 * Parameters:  proc_t p                process to apply attributes to
 *              int psa_apptype         posix spawn attribute apptype
 *
 * Returns:     0                       Success
 */
static errno_t
exec_handle_spawnattr_policy(proc_t p, thread_t thread, int psa_apptype, uint64_t psa_qos_clamp,
    task_role_t psa_darwin_role, struct exec_port_actions *port_actions)
{
	int apptype     = TASK_APPTYPE_NONE;
	int qos_clamp   = THREAD_QOS_UNSPECIFIED;
	task_role_t role = TASK_UNSPECIFIED;

	if ((psa_apptype & POSIX_SPAWN_PROC_TYPE_MASK) != 0) {
		int proctype = psa_apptype & POSIX_SPAWN_PROC_TYPE_MASK;

		switch (proctype) {
		case POSIX_SPAWN_PROC_TYPE_DAEMON_INTERACTIVE:
			apptype = TASK_APPTYPE_DAEMON_INTERACTIVE;
			break;
		case POSIX_SPAWN_PROC_TYPE_DAEMON_STANDARD:
			apptype = TASK_APPTYPE_DAEMON_STANDARD;
			break;
		case POSIX_SPAWN_PROC_TYPE_DAEMON_ADAPTIVE:
			apptype = TASK_APPTYPE_DAEMON_ADAPTIVE;
			break;
		case POSIX_SPAWN_PROC_TYPE_DAEMON_BACKGROUND:
			apptype = TASK_APPTYPE_DAEMON_BACKGROUND;
			break;
		case POSIX_SPAWN_PROC_TYPE_APP_DEFAULT:
			apptype = TASK_APPTYPE_APP_DEFAULT;
			break;
		case POSIX_SPAWN_PROC_TYPE_DRIVER:
			apptype = TASK_APPTYPE_DRIVER;
			break;
		default:
			apptype = TASK_APPTYPE_NONE;
			/* TODO: Should an invalid value here fail the spawn? */
			break;
		}
	}

	if (psa_qos_clamp != POSIX_SPAWN_PROC_CLAMP_NONE) {
		switch (psa_qos_clamp) {
		case POSIX_SPAWN_PROC_CLAMP_UTILITY:
			qos_clamp = THREAD_QOS_UTILITY;
			break;
		case POSIX_SPAWN_PROC_CLAMP_BACKGROUND:
			qos_clamp = THREAD_QOS_BACKGROUND;
			break;
		case POSIX_SPAWN_PROC_CLAMP_MAINTENANCE:
			qos_clamp = THREAD_QOS_MAINTENANCE;
			break;
		default:
			qos_clamp = THREAD_QOS_UNSPECIFIED;
			/* TODO: Should an invalid value here fail the spawn? */
			break;
		}
	}

	if (psa_darwin_role != PRIO_DARWIN_ROLE_DEFAULT) {
		proc_darwin_role_to_task_role(psa_darwin_role, &role);
	}

	if (apptype != TASK_APPTYPE_NONE ||
	    qos_clamp != THREAD_QOS_UNSPECIFIED ||
	    role != TASK_UNSPECIFIED ||
	    port_actions->portwatch_count) {
		proc_set_task_spawnpolicy(proc_task(p), thread, apptype, qos_clamp, role,
		    port_actions->portwatch_array, port_actions->portwatch_count);
	}

	if (port_actions->registered_count) {
		if (_kernelrpc_mach_ports_register3(proc_task(p),
		    port_actions->registered_array[0],
		    port_actions->registered_array[1],
		    port_actions->registered_array[2])) {
			return EINVAL;
		}
		/* mach_ports_register() consumed the array */
		bzero(port_actions->registered_array,
		    sizeof(port_actions->registered_array));
		port_actions->registered_count = 0;
	}

	return 0;
}

static void
exec_port_actions_destroy(struct exec_port_actions *port_actions)
{
	if (port_actions->excport_array) {
		for (uint32_t i = 0; i < port_actions->exception_port_count; i++) {
			ipc_port_t port = NULL;
			if ((port = port_actions->excport_array[i].port) != NULL) {
				ipc_port_release_send(port);
			}
		}
		kfree_type(struct exception_port_action_t, port_actions->exception_port_count,
		    port_actions->excport_array);
	}

	if (port_actions->portwatch_array) {
		for (uint32_t i = 0; i < port_actions->portwatch_count; i++) {
			ipc_port_t port = NULL;
			if ((port = port_actions->portwatch_array[i]) != NULL) {
				ipc_port_release_send(port);
			}
		}
		kfree_type(ipc_port_t, port_actions->portwatch_count,
		    port_actions->portwatch_array);
	}

	for (uint32_t i = 0; i < port_actions->registered_count; i++) {
		ipc_port_t port = NULL;
		if ((port = port_actions->registered_array[i]) != NULL) {
			ipc_port_release_send(port);
		}
	}
}

/*
 * exec_handle_port_actions
 *
 * Description:	Go through the _posix_port_actions_t contents,
 *              calling task_set_special_port, task_set_exception_ports
 *              and/or audit_session_spawnjoin for the current task.
 *
 * Parameters:	struct image_params *	Image parameter block
 *
 * Returns:	0			Success
 *              EINVAL			Failure
 *              ENOTSUP			Illegal posix_spawn attr flag was set
 */
static errno_t
exec_handle_port_actions(struct image_params *imgp,
    struct exec_port_actions *actions)
{
	_posix_spawn_port_actions_t pacts = imgp->ip_px_spa;
#if CONFIG_AUDIT
	proc_t p = vfs_context_proc(imgp->ip_vfs_context);
#endif
	_ps_port_action_t *act = NULL;
	task_t task = get_threadtask(imgp->ip_new_thread);
	ipc_port_t port = NULL;
	errno_t ret = 0;
	int i = 0, portwatch_i = 0, registered_i = 0, excport_i = 0;
	kern_return_t kr;
	boolean_t task_has_watchport_boost = task_has_watchports(current_task());
	boolean_t in_exec = (imgp->ip_flags & IMGPF_EXEC);
	int ptrauth_task_port_count = 0;

	for (i = 0; i < pacts->pspa_count; i++) {
		act = &pacts->pspa_actions[i];

		switch (act->port_type) {
		case PSPA_SPECIAL:
#if CONFIG_AUDIT
		case PSPA_AU_SESSION:
#endif
			break;
		case PSPA_EXCEPTION:
			if (++actions->exception_port_count > TASK_MAX_EXCEPTION_PORT_COUNT) {
				ret = EINVAL;
				goto done;
			}
			break;
		case PSPA_IMP_WATCHPORTS:
			if (++actions->portwatch_count > TASK_MAX_WATCHPORT_COUNT) {
				ret = EINVAL;
				goto done;
			}
			break;
		case PSPA_REGISTERED_PORTS:
			if (++actions->registered_count > TASK_PORT_REGISTER_MAX) {
				ret = EINVAL;
				goto done;
			}
			break;
		case PSPA_PTRAUTH_TASK_PORT:
			if (++ptrauth_task_port_count > 1) {
				ret = EINVAL;
				goto done;
			}
			break;
		default:
			ret = EINVAL;
			goto done;
		}
	}

	if (actions->exception_port_count) {
		actions->excport_array = kalloc_type(struct exception_port_action_t,
		    actions->exception_port_count, Z_WAITOK | Z_ZERO);

		if (actions->excport_array == NULL) {
			ret = ENOMEM;
			goto done;
		}
	}
	if (actions->portwatch_count) {
		if (in_exec && task_has_watchport_boost) {
			ret = EINVAL;
			goto done;
		}
		actions->portwatch_array = kalloc_type(ipc_port_t,
		    actions->portwatch_count, Z_WAITOK | Z_ZERO);
		if (actions->portwatch_array == NULL) {
			ret = ENOMEM;
			goto done;
		}
	}

	for (i = 0; i < pacts->pspa_count; i++) {
		act = &pacts->pspa_actions[i];

		if (MACH_PORT_VALID(act->new_port)) {
			kr = ipc_typed_port_copyin_send(get_task_ipcspace(current_task()),
			    act->new_port, IKOT_UNKNOWN, &port);

			if (kr != KERN_SUCCESS) {
				ret = EINVAL;
				goto done;
			}
		} else {
			/* it's NULL or DEAD */
			port = CAST_MACH_NAME_TO_PORT(act->new_port);
		}

		switch (act->port_type) {
		case PSPA_SPECIAL:
			kr = task_set_special_port(task, act->which, port);

			if (kr != KERN_SUCCESS) {
				ret = EINVAL;
			}
			break;

#if CONFIG_AUDIT
		case PSPA_AU_SESSION:
			ret = audit_session_spawnjoin(p, port);
			if (ret) {
				/* audit_session_spawnjoin() has already dropped the reference in case of error. */
				goto done;
			}

			break;
#endif
		case PSPA_EXCEPTION:
			assert(excport_i < actions->exception_port_count);
			/* hold on to this till end of spawn */
			actions->excport_array[excport_i].port_action = act;
			actions->excport_array[excport_i].port = port;
			excport_i++;
			break;
		case PSPA_IMP_WATCHPORTS:
			assert(portwatch_i < actions->portwatch_count);
			/* hold on to this till end of spawn */
			actions->portwatch_array[portwatch_i++] = port;
			break;
		case PSPA_REGISTERED_PORTS:
			assert(registered_i < actions->registered_count);
			/* hold on to this till end of spawn */
			actions->registered_array[registered_i++] = port;
			break;

		case PSPA_PTRAUTH_TASK_PORT:
#if (DEVELOPMENT || DEBUG)
#if defined(HAS_APPLE_PAC)
			{
				task_t ptr_auth_task = convert_port_to_task(port);

				if (ptr_auth_task == TASK_NULL) {
					ret = EINVAL;
					break;
				}

				imgp->ip_inherited_shared_region_id =
				    task_get_vm_shared_region_id_and_jop_pid(ptr_auth_task,
				    &imgp->ip_inherited_jop_pid);

				/* Deallocate task ref returned by convert_port_to_task */
				task_deallocate(ptr_auth_task);
			}
#endif /* HAS_APPLE_PAC */
#endif /* (DEVELOPMENT || DEBUG) */

			/* consume the port right in case of success */
			ipc_port_release_send(port);
			break;
		default:
			ret = EINVAL;
			break;
		}

		if (ret) {
			/* action failed, so release port resources */
			ipc_port_release_send(port);
			break;
		}
	}

done:
	if (0 != ret) {
		DTRACE_PROC1(spawn__port__failure, mach_port_name_t, act->new_port);
	}
	return ret;
}


/*
 * exec_handle_exception_port_actions
 *
 * Description:	Go through the saved exception ports in exec_port_actions,
 *              calling task_set_exception_ports for the current Task.
 *              This must happen after image activation, and after exec_resettextvp()
 *				because task_set_exception_ports checks the `TF_PLATFORM` bit and entitlements.
 *
 * Parameters:	struct image_params *		Image parameter block
 *                              struct exec_port_actions *  Saved Port Actions
 *
 * Returns:	0			Success
 *              EINVAL			task_set_exception_ports failed
 */
static errno_t
exec_handle_exception_port_actions(const struct image_params *imgp,
    const struct exec_port_actions *actions)
{
	task_t task = get_threadtask(imgp->ip_new_thread);

	for (int i = 0; i < actions->exception_port_count; i++) {
		ipc_port_t port = actions->excport_array[i].port;
		_ps_port_action_t *act = actions->excport_array[i].port_action;
		assert(act != NULL);
		kern_return_t kr = task_set_exception_ports(task, act->mask, port,
		    act->behavior, act->flavor);
		if (kr != KERN_SUCCESS) {
			DTRACE_PROC1(spawn__exception__port__failure, mach_port_name_t, act->new_port);
			return EINVAL;
		}
		actions->excport_array[i].port = NULL;
	}

	return 0;
}


/*
 * exec_handle_file_actions
 *
 * Description:	Go through the _posix_file_actions_t contents applying the
 *		open, close, and dup2 operations to the open file table for
 *		the current process.
 *
 * Parameters:	struct image_params *	Image parameter block
 *
 * Returns:	0			Success
 *		???
 *
 * Note:	Actions are applied in the order specified, with the credential
 *		of the parent process.  This is done to permit the parent
 *		process to utilize POSIX_SPAWN_RESETIDS to drop privilege in
 *		the child following operations the child may in fact not be
 *		normally permitted to perform.
 */
static int
exec_handle_file_actions(struct image_params *imgp, short psa_flags)
{
	int error = 0;
	int action;
	proc_t p = vfs_context_proc(imgp->ip_vfs_context);
	kauth_cred_t p_cred = vfs_context_ucred(imgp->ip_vfs_context);
	_posix_spawn_file_actions_t px_sfap = imgp->ip_px_sfa;
	int ival[2];            /* dummy retval for system calls) */
#if CONFIG_AUDIT
	struct uthread *uthread = current_uthread();
#endif

	for (action = 0; action < px_sfap->psfa_act_count; action++) {
		_psfa_action_t *psfa = &px_sfap->psfa_act_acts[action];

		switch (psfa->psfaa_type) {
		case PSFA_OPEN: {
			/*
			 * Open is different, in that it requires the use of
			 * a path argument, which is normally copied in from
			 * user space; because of this, we have to support an
			 * open from kernel space that passes an address space
			 * context of UIO_SYSSPACE, and casts the address
			 * argument to a user_addr_t.
			 */
			struct vnode_attr *vap;
			struct nameidata *ndp;
			int mode = psfa->psfaa_openargs.psfao_mode;
			int origfd;
			struct {
				struct vnode_attr va;
				struct nameidata nd;
			} *__open_data;

			__open_data = kalloc_type(typeof(*__open_data), Z_WAITOK | Z_ZERO);
			if (__open_data == NULL) {
				error = ENOMEM;
				break;
			}

			vap = &__open_data->va;
			ndp = &__open_data->nd;

			VATTR_INIT(vap);
			/* Mask off all but regular access permissions */
			mode = ((mode & ~p->p_fd.fd_cmask) & ALLPERMS) & ~S_ISTXT;
			VATTR_SET(vap, va_mode, mode & ACCESSPERMS);

			AUDIT_SUBCALL_ENTER(OPEN, p, uthread);

			NDINIT(ndp, LOOKUP, OP_OPEN, FOLLOW | AUDITVNPATH1, UIO_SYSSPACE,
			    CAST_USER_ADDR_T(psfa->psfaa_openargs.psfao_path),
			    imgp->ip_vfs_context);

			error = open1(imgp->ip_vfs_context, ndp,
			    psfa->psfaa_openargs.psfao_oflag,
			    vap, NULL, NULL, &origfd, AUTH_OPEN_NOAUTHFD);

			kfree_type(typeof(*__open_data), __open_data);

			AUDIT_SUBCALL_EXIT(uthread, error);

			/*
			 * If there's an error, or we get the right fd by
			 * accident, then drop out here.  This is easier than
			 * reworking all the open code to preallocate fd
			 * slots, and internally taking one as an argument.
			 */
			if (error || origfd == psfa->psfaa_filedes) {
				break;
			}

			/*
			 * If we didn't fall out from an error, we ended up
			 * with the wrong fd; so now we've got to try to dup2
			 * it to the right one.
			 */
			AUDIT_SUBCALL_ENTER(DUP2, p, uthread);
			error = dup2(p, p_cred, origfd, psfa->psfaa_filedes, ival);
			AUDIT_SUBCALL_EXIT(uthread, error);
			if (error) {
				break;
			}

			/*
			 * Finally, close the original fd.
			 */
			AUDIT_SUBCALL_ENTER(CLOSE, p, uthread);
			error = close_nocancel(p, p_cred, origfd);
			AUDIT_SUBCALL_EXIT(uthread, error);
		}
		break;

		case PSFA_DUP2: {
			AUDIT_SUBCALL_ENTER(DUP2, p, uthread);
			error = dup2(p, p_cred, psfa->psfaa_filedes,
			    psfa->psfaa_dup2args.psfad_newfiledes, ival);
			AUDIT_SUBCALL_EXIT(uthread, error);
		}
		break;

		case PSFA_FILEPORT_DUP2: {
			ipc_port_t port;
			kern_return_t kr;
			int origfd;

			if (!MACH_PORT_VALID(psfa->psfaa_fileport)) {
				error = EINVAL;
				break;
			}

			kr = ipc_typed_port_copyin_send(get_task_ipcspace(current_task()),
			    psfa->psfaa_fileport, IKOT_FILEPORT, &port);

			if (kr != KERN_SUCCESS) {
				error = EINVAL;
				break;
			}

			error = fileport_makefd(p, port, 0, &origfd);

			if (IPC_PORT_NULL != port) {
				ipc_typed_port_release_send(port, IKOT_FILEPORT);
			}

			if (error || origfd == psfa->psfaa_dup2args.psfad_newfiledes) {
				break;
			}

			AUDIT_SUBCALL_ENTER(DUP2, p, uthread);
			error = dup2(p, p_cred, origfd,
			    psfa->psfaa_dup2args.psfad_newfiledes, ival);
			AUDIT_SUBCALL_EXIT(uthread, error);
			if (error) {
				break;
			}

			AUDIT_SUBCALL_ENTER(CLOSE, p, uthread);
			error = close_nocancel(p, p_cred, origfd);
			AUDIT_SUBCALL_EXIT(uthread, error);
		}
		break;

		case PSFA_CLOSE: {
			AUDIT_SUBCALL_ENTER(CLOSE, p, uthread);
			error = close_nocancel(p, p_cred, psfa->psfaa_filedes);
			AUDIT_SUBCALL_EXIT(uthread, error);
		}
		break;

		case PSFA_INHERIT: {
			struct fileproc *fp;

			/*
			 * Check to see if the descriptor exists, and
			 * ensure it's -not- marked as close-on-exec.
			 *
			 * Attempting to "inherit" a guarded fd will
			 * result in a error.
			 */

			proc_fdlock(p);
			if ((fp = fp_get_noref_locked(p, psfa->psfaa_filedes)) == NULL) {
				error = EBADF;
			} else if (fp->fp_guard_attrs) {
				error = fp_guard_exception(p, psfa->psfaa_filedes,
				    fp, kGUARD_EXC_NOCLOEXEC);
			} else {
				fp->fp_flags &= ~FP_CLOEXEC;
				error = 0;
			}
			proc_fdunlock(p);
		}
		break;

		case PSFA_CHDIR: {
			/*
			 * Chdir is different, in that it requires the use of
			 * a path argument, which is normally copied in from
			 * user space; because of this, we have to support a
			 * chdir from kernel space that passes an address space
			 * context of UIO_SYSSPACE, and casts the address
			 * argument to a user_addr_t.
			 */
			struct nameidata *nd;
			nd = kalloc_type(struct nameidata,
			    Z_WAITOK | Z_ZERO | Z_NOFAIL);

			AUDIT_SUBCALL_ENTER(CHDIR, p, uthread);
			NDINIT(nd, LOOKUP, OP_CHDIR, FOLLOW | AUDITVNPATH1, UIO_SYSSPACE,
			    CAST_USER_ADDR_T(psfa->psfaa_chdirargs.psfac_path),
			    imgp->ip_vfs_context);

			error = chdir_internal(p, imgp->ip_vfs_context, nd, 0);
			kfree_type(struct nameidata, nd);
			AUDIT_SUBCALL_EXIT(uthread, error);
		}
		break;

		case PSFA_FCHDIR: {
			AUDIT_SUBCALL_ENTER(FCHDIR, p, uthread);
			error = fchdir(p, imgp->ip_vfs_context,
			    psfa->psfaa_filedes, false);
			AUDIT_SUBCALL_EXIT(uthread, error);
		}
		break;

		default:
			error = EINVAL;
			break;
		}

		/* All file actions failures are considered fatal, per POSIX */

		if (error) {
			if (PSFA_OPEN == psfa->psfaa_type) {
				DTRACE_PROC1(spawn__open__failure, uintptr_t,
				    psfa->psfaa_openargs.psfao_path);
			} else {
				DTRACE_PROC1(spawn__fd__failure, int, psfa->psfaa_filedes);
			}
			break;
		}
	}

	if (error != 0 || (psa_flags & POSIX_SPAWN_CLOEXEC_DEFAULT) == 0) {
		return error;
	}

	/*
	 * If POSIX_SPAWN_CLOEXEC_DEFAULT is set, behave (during
	 * this spawn only) as if "close on exec" is the default
	 * disposition of all pre-existing file descriptors.  In this case,
	 * the list of file descriptors mentioned in the file actions
	 * are the only ones that can be inherited, so mark them now.
	 *
	 * The actual closing part comes later, in fdt_exec().
	 */
	proc_fdlock(p);
	for (action = 0; action < px_sfap->psfa_act_count; action++) {
		_psfa_action_t *psfa = &px_sfap->psfa_act_acts[action];
		int fd = psfa->psfaa_filedes;

		switch (psfa->psfaa_type) {
		case PSFA_DUP2:
		case PSFA_FILEPORT_DUP2:
			fd = psfa->psfaa_dup2args.psfad_newfiledes;
			OS_FALLTHROUGH;
		case PSFA_OPEN:
		case PSFA_INHERIT:
			*fdflags(p, fd) |= UF_INHERIT;
			break;

		case PSFA_CLOSE:
		case PSFA_CHDIR:
		case PSFA_FCHDIR:
			/*
			 * Although PSFA_FCHDIR does have a file descriptor, it is not
			 * *creating* one, thus we do not automatically mark it for
			 * inheritance under POSIX_SPAWN_CLOEXEC_DEFAULT. A client that
			 * wishes it to be inherited should use the PSFA_INHERIT action
			 * explicitly.
			 */
			break;
		}
	}
	proc_fdunlock(p);

	return 0;
}

#if CONFIG_MACF
/*
 * Check that the extension's data is within the bounds of the
 * allocation storing all extensions' data
 */
static inline errno_t
exec_spawnattr_validate_policyext_data(const struct ip_px_smpx_s *px_s,
    const _ps_mac_policy_extension_t *ext)
{
	uint64_t dataend;

	if (__improbable(os_add_overflow(ext->dataoff, ext->datalen, &dataend))) {
		return EOVERFLOW;
	}
	if (__improbable(dataend > px_s->datalen)) {
		return EINVAL;
	}

	return 0;
}

/*
 * exec_spawnattr_getmacpolicyinfo
 */
void *
exec_spawnattr_getmacpolicyinfo(const void *macextensions, const char *policyname, size_t *lenp)
{
	const struct ip_px_smpx_s *px_s = macextensions;
	const struct _posix_spawn_mac_policy_extensions *psmx = NULL;
	int i;

	if (px_s == NULL) {
		return NULL;
	}

	psmx = px_s->array;
	if (psmx == NULL) {
		return NULL;
	}

	for (i = 0; i < psmx->psmx_count; i++) {
		const _ps_mac_policy_extension_t *extension = &psmx->psmx_extensions[i];
		if (strncmp(extension->policyname, policyname, sizeof(extension->policyname)) == 0) {
			if (__improbable(exec_spawnattr_validate_policyext_data(px_s, extension))) {
				panic("invalid mac policy extension data");
			}
			if (lenp != NULL) {
				*lenp = (size_t)extension->datalen;
			}
			return (void *)((uintptr_t)px_s->data + extension->dataoff);
		}
	}

	if (lenp != NULL) {
		*lenp = 0;
	}
	return NULL;
}

static int
spawn_copyin_macpolicyinfo(const struct user__posix_spawn_args_desc *px_args,
    struct ip_px_smpx_s *pxsp)
{
	_posix_spawn_mac_policy_extensions_t psmx = NULL;
	uint8_t *data = NULL;
	uint64_t datalen = 0;
	uint64_t dataoff = 0;
	int error = 0;

	bzero(pxsp, sizeof(*pxsp));

	if (px_args->mac_extensions_size < PS_MAC_EXTENSIONS_SIZE(1) ||
	    px_args->mac_extensions_size > PAGE_SIZE) {
		error = EINVAL;
		goto bad;
	}

	psmx = kalloc_data(px_args->mac_extensions_size, Z_WAITOK);
	if (psmx == NULL) {
		error = ENOMEM;
		goto bad;
	}

	error = copyin(px_args->mac_extensions, psmx, px_args->mac_extensions_size);
	if (error) {
		goto bad;
	}

	size_t extsize = PS_MAC_EXTENSIONS_SIZE(psmx->psmx_count);
	if (extsize == 0 || extsize > px_args->mac_extensions_size) {
		error = EINVAL;
		goto bad;
	}

	for (int i = 0; i < psmx->psmx_count; i++) {
		_ps_mac_policy_extension_t *extension = &psmx->psmx_extensions[i];
		if (extension->datalen == 0 || extension->datalen > PAGE_SIZE) {
			error = EINVAL;
			goto bad;
		}
		if (__improbable(os_add_overflow(datalen, extension->datalen, &datalen))) {
			error = ENOMEM;
			goto bad;
		}
	}

	data = kalloc_data((vm_size_t)datalen, Z_WAITOK);
	if (data == NULL) {
		error = ENOMEM;
		goto bad;
	}

	for (int i = 0; i < psmx->psmx_count; i++) {
		_ps_mac_policy_extension_t *extension = &psmx->psmx_extensions[i];

#if !__LP64__
		if (extension->data > UINT32_MAX) {
			goto bad;
		}
#endif
		error = copyin((user_addr_t)extension->data, &data[dataoff], (size_t)extension->datalen);
		if (error) {
			error = ENOMEM;
			goto bad;
		}
		extension->dataoff = dataoff;
		dataoff += extension->datalen;
	}

	pxsp->array = psmx;
	pxsp->data = data;
	pxsp->datalen = datalen;
	return 0;

bad:
	kfree_data(psmx, px_args->mac_extensions_size);
	kfree_data(data, (vm_size_t)datalen);
	return error;
}
#endif /* CONFIG_MACF */

#if CONFIG_COALITIONS
static inline void
spawn_coalitions_release_all(coalition_t coal[COALITION_NUM_TYPES])
{
	for (int c = 0; c < COALITION_NUM_TYPES; c++) {
		if (coal[c]) {
			coalition_remove_active(coal[c]);
			coalition_release(coal[c]);
		}
	}
}
#endif

#if CONFIG_PERSONAS
static int
spawn_validate_persona(struct _posix_spawn_persona_info *px_persona)
{
	int error = 0;
	struct persona *persona = NULL;
	kauth_cred_t mycred = kauth_cred_get();

	if (!IOCurrentTaskHasEntitlement( PERSONA_MGMT_ENTITLEMENT)) {
		return EPERM;
	}

	if (px_persona->pspi_flags & POSIX_SPAWN_PERSONA_GROUPS) {
		if (px_persona->pspi_ngroups > NGROUPS_MAX) {
			return EINVAL;
		}
	}

	persona = persona_lookup(px_persona->pspi_id);
	if (!persona) {
		return ESRCH;
	}

	// non-root process should not be allowed to set persona with uid/gid 0
	if (!kauth_cred_issuser(mycred) &&
	    (px_persona->pspi_uid == 0 || px_persona->pspi_gid == 0)) {
		return EPERM;
	}

	persona_put(persona);
	return error;
}

static bool
kauth_cred_model_setpersona(
	kauth_cred_t            model,
	struct _posix_spawn_persona_info *px_persona)
{
	bool updated = false;

	if (px_persona->pspi_flags & POSIX_SPAWN_PERSONA_UID) {
		updated |= kauth_cred_model_setresuid(model,
		    px_persona->pspi_uid,
		    px_persona->pspi_uid,
		    px_persona->pspi_uid,
		    KAUTH_UID_NONE);
	}

	if (px_persona->pspi_flags & POSIX_SPAWN_PERSONA_GID) {
		updated |= kauth_cred_model_setresgid(model,
		    px_persona->pspi_gid,
		    px_persona->pspi_gid,
		    px_persona->pspi_gid);
	}

	if (px_persona->pspi_flags & POSIX_SPAWN_PERSONA_GROUPS) {
		updated |= kauth_cred_model_setgroups(model,
		    px_persona->pspi_groups,
		    px_persona->pspi_ngroups,
		    px_persona->pspi_gmuid);
	}

	return updated;
}

static int
spawn_persona_adopt(proc_t p, struct _posix_spawn_persona_info *px_persona)
{
	struct persona *persona = NULL;

	/*
	 * we want to spawn into the given persona, but we want to override
	 * the kauth with a different UID/GID combo
	 */
	persona = persona_lookup(px_persona->pspi_id);
	if (!persona) {
		return ESRCH;
	}

	return persona_proc_adopt(p, persona,
	           ^bool (kauth_cred_t parent __unused, kauth_cred_t model) {
		return kauth_cred_model_setpersona(model, px_persona);
	});
}
#endif

#if __arm64__
#if DEVELOPMENT || DEBUG
TUNABLE(int, legacy_footprint_entitlement_mode, "legacy_footprint_entitlement_mode",
    LEGACY_FOOTPRINT_ENTITLEMENT_IGNORE);

__startup_func
static void
legacy_footprint_entitlement_mode_init(void)
{
	/*
	 * legacy_footprint_entitlement_mode specifies the behavior we want associated
	 * with the entitlement. The supported modes are:
	 *
	 * LEGACY_FOOTPRINT_ENTITLEMENT_IGNORE:
	 *	Indicates that we want every process to have the memory accounting
	 *	that is available in iOS 12.0 and beyond.
	 *
	 * LEGACY_FOOTPRINT_ENTITLEMENT_IOS11_ACCT:
	 *	Indicates that for every process that has the 'legacy footprint entitlement',
	 *      we want to give it the old iOS 11.0 accounting behavior which accounted some
	 *	of the process's memory to the kernel.
	 *
	 * LEGACY_FOOTPRINT_ENTITLEMENT_LIMIT_INCREASE:
	 *      Indicates that for every process that has the 'legacy footprint entitlement',
	 *	we want it to have a higher memory limit which will help them acclimate to the
	 *	iOS 12.0 (& beyond) accounting behavior that does the right accounting.
	 *      The bonus added to the system-wide task limit to calculate this higher memory limit
	 *      is available in legacy_footprint_bonus_mb.
	 */

	if (legacy_footprint_entitlement_mode < LEGACY_FOOTPRINT_ENTITLEMENT_IGNORE ||
	    legacy_footprint_entitlement_mode > LEGACY_FOOTPRINT_ENTITLEMENT_LIMIT_INCREASE) {
		legacy_footprint_entitlement_mode = LEGACY_FOOTPRINT_ENTITLEMENT_LIMIT_INCREASE;
	}
}
STARTUP(TUNABLES, STARTUP_RANK_MIDDLE, legacy_footprint_entitlement_mode_init);
#else
const int legacy_footprint_entitlement_mode = LEGACY_FOOTPRINT_ENTITLEMENT_IGNORE;
#endif

static inline void
proc_legacy_footprint_entitled(proc_t p, task_t task)
{
#pragma unused(p)
	boolean_t legacy_footprint_entitled;

	switch (legacy_footprint_entitlement_mode) {
	case LEGACY_FOOTPRINT_ENTITLEMENT_IGNORE:
		/* the entitlement is ignored */
		break;
	case LEGACY_FOOTPRINT_ENTITLEMENT_IOS11_ACCT:
		/* the entitlement grants iOS11 legacy accounting */
		legacy_footprint_entitled = memorystatus_task_has_legacy_footprint_entitlement(proc_task(p));
		if (legacy_footprint_entitled) {
			task_set_legacy_footprint(task);
		}
		break;
	case LEGACY_FOOTPRINT_ENTITLEMENT_LIMIT_INCREASE:
		/* the entitlement grants a footprint limit increase */
		legacy_footprint_entitled = memorystatus_task_has_legacy_footprint_entitlement(proc_task(p));
		if (legacy_footprint_entitled) {
			task_set_extra_footprint_limit(task);
		}
		break;
	default:
		break;
	}
}

static inline void
proc_ios13extended_footprint_entitled(proc_t p, task_t task)
{
#pragma unused(p)
	boolean_t ios13extended_footprint_entitled;

	/* the entitlement grants a footprint limit increase */
	ios13extended_footprint_entitled = memorystatus_task_has_ios13extended_footprint_limit(proc_task(p));
	if (ios13extended_footprint_entitled) {
		task_set_ios13extended_footprint_limit(task);
	}
}

static inline void
proc_increased_memory_limit_entitled(proc_t p, task_t task)
{
	if (memorystatus_task_has_increased_debugging_memory_limit_entitlement(task)) {
		memorystatus_act_on_entitled_developer_task_limit(p);
	} else if (memorystatus_task_has_increased_memory_limit_entitlement(task)) {
		memorystatus_act_on_entitled_task_limit(p);
	}
}

/*
 * Check for any of the various entitlements that permit a higher
 * task footprint limit or alternate accounting and apply them.
 */
static inline void
proc_footprint_entitlement_hacks(proc_t p, task_t task)
{
	proc_legacy_footprint_entitled(p, task);
	proc_ios13extended_footprint_entitled(p, task);
	proc_increased_memory_limit_entitled(p, task);
}
#endif /* __arm64__ */

/*
 * Processes with certain entitlements are granted a jumbo-size VM map.
 */
static inline void
proc_apply_jit_and_vm_policies(struct image_params *imgp, proc_t p, task_t task)
{
#if CONFIG_MACF
	bool jit_entitled = false;
#endif /* CONFIG_MACF */
	bool needs_jumbo_va = false;
	bool needs_extra_jumbo_va = false;
	struct _posix_spawnattr *psa = imgp->ip_px_sa;

#if CONFIG_MACF
	jit_entitled = (mac_proc_check_map_anon(p, proc_ucred_unsafe(p),
	    0, 0, 0, MAP_JIT, NULL) == 0);
	needs_jumbo_va = jit_entitled || IOTaskHasEntitlement(task,
	    "com.apple.developer.kernel.extended-virtual-addressing") ||
	    memorystatus_task_has_increased_memory_limit_entitlement(task) ||
	    memorystatus_task_has_increased_debugging_memory_limit_entitlement(task);
#else
#pragma unused(p)
#endif /* CONFIG_MACF */


	if (needs_jumbo_va) {
		vm_map_set_jumbo(get_task_map(task));
	}

	if (psa && psa->psa_max_addr) {
		vm_map_set_max_addr(get_task_map(task), psa->psa_max_addr, false);
	}

#if CONFIG_MAP_RANGES
	if (task_is_hardened_binary(task) && !proc_is_simulated(p)) {
		/*
		 * This must be done last as it needs to observe
		 * any kind of VA space growth that was requested.
		 * This is used by the secure allocator, so
		 * must be applied to all hardened binaries
		 */
#if XNU_TARGET_OS_IOS && EXTENDED_USER_VA_SUPPORT
		needs_extra_jumbo_va = IOTaskHasEntitlement(task,
		    "com.apple.kernel.large-file-virtual-addressing");
#endif /* XNU_TARGET_OS_IOS && EXTENDED_USER_VA_SUPPORT */
		vm_map_range_configure(get_task_map(task), needs_extra_jumbo_va);
	}
#else
#pragma unused(needs_extra_jumbo_va)
#endif /* CONFIG_MAP_RANGES */

#if CONFIG_MACF
	if (jit_entitled) {
		vm_map_set_jit_entitled(get_task_map(task));

	}
#endif /* CONFIG_MACF */

#if XNU_TARGET_OS_OSX
	/* TPRO cannot be enforced on binaries that load 3P plugins on macos - rdar://107420220 */
	const bool task_loads_3P_plugins = imgp->ip_flags & IMGPF_3P_PLUGINS;
#endif /* XNU_TARGET_OS_OSX */

	if (task_is_hardened_binary(task)
#if XNU_TARGET_OS_OSX
	    && !task_loads_3P_plugins
#endif /* XNU_TARGET_OS_OSX */
	    ) {
		/*
		 * Pre-emptively disable TPRO remapping for
		 * hardened binaries (which do not load 3P plugins)
		 */
		vm_map_set_tpro_enforcement(get_task_map(task));
	}
}

static int
spawn_posix_cred_adopt(proc_t p,
    struct _posix_spawn_posix_cred_info *px_pcred_info)
{
	int error = 0;

	if (px_pcred_info->pspci_flags & POSIX_SPAWN_POSIX_CRED_GID) {
		struct setgid_args args = {
			.gid = px_pcred_info->pspci_gid,
		};
		error = setgid(p, &args, NULL);
		if (error) {
			return error;
		}
	}

	if (px_pcred_info->pspci_flags & POSIX_SPAWN_POSIX_CRED_GROUPS) {
		error = setgroups_internal(p,
		    px_pcred_info->pspci_ngroups,
		    px_pcred_info->pspci_groups,
		    px_pcred_info->pspci_gmuid);
		if (error) {
			return error;
		}
	}

	if (px_pcred_info->pspci_flags & POSIX_SPAWN_POSIX_CRED_UID) {
		struct setuid_args args = {
			.uid = px_pcred_info->pspci_uid,
		};
		error = setuid(p, &args, NULL);
		if (error) {
			return error;
		}
	}
	return 0;
}

/*
 * posix_spawn
 *
 * Parameters:	uap->pid		Pointer to pid return area
 *		uap->fname		File name to exec
 *		uap->argp		Argument list
 *		uap->envp		Environment list
 *
 * Returns:	0			Success
 *		EINVAL			Invalid argument
 *		ENOTSUP			Not supported
 *		ENOEXEC			Executable file format error
 *	exec_activate_image:EINVAL	Invalid argument
 *	exec_activate_image:EACCES	Permission denied
 *	exec_activate_image:EINTR	Interrupted function
 *	exec_activate_image:ENOMEM	Not enough space
 *	exec_activate_image:EFAULT	Bad address
 *	exec_activate_image:ENAMETOOLONG	Filename too long
 *	exec_activate_image:ENOEXEC	Executable file format error
 *	exec_activate_image:ETXTBSY	Text file busy [misuse of error code]
 *	exec_activate_image:EAUTH	Image decryption failed
 *	exec_activate_image:EBADEXEC	The executable is corrupt/unknown
 *	exec_activate_image:???
 *	mac_execve_enter:???
 *
 * TODO:	Expect to need __mac_posix_spawn() at some point...
 *		Handle posix_spawnattr_t
 *		Handle posix_spawn_file_actions_t
 */
int
posix_spawn(proc_t ap, struct posix_spawn_args *uap, int32_t *retval)
{
	proc_t p = ap;
	user_addr_t pid = uap->pid;
	int ival[2];            /* dummy retval for setpgid() */
	char *subsystem_root_path = NULL;
	struct image_params *imgp = NULL;
	struct vnode_attr *vap = NULL;
	struct vnode_attr *origvap = NULL;
	struct uthread  *uthread = 0;   /* compiler complains if not set to 0*/
	int error, sig;
	int is_64 = IS_64BIT_PROCESS(p);
	struct vfs_context context;
	struct user__posix_spawn_args_desc px_args = {};
	struct _posix_spawnattr px_sa = {};
	_posix_spawn_file_actions_t px_sfap = NULL;
	_posix_spawn_port_actions_t px_spap = NULL;
	struct __kern_sigaction vec;
	boolean_t spawn_no_exec = FALSE;
	boolean_t proc_transit_set = TRUE;
	boolean_t proc_signal_set = TRUE;
	boolean_t exec_done = FALSE;
	os_reason_t exec_failure_reason = NULL;

	struct exec_port_actions port_actions = { };
	vm_size_t px_sa_offset = offsetof(struct _posix_spawnattr, psa_ports);
	task_t old_task = current_task();
	task_t new_task = NULL;
	boolean_t should_release_proc_ref = FALSE;
	void *inherit = NULL;
	uint8_t crash_behavior = 0;
	uint64_t crash_behavior_deadline = 0;
#if CONFIG_EXCLAVES
	char *task_conclave_id = NULL;
#endif
#if CONFIG_PERSONAS
	struct _posix_spawn_persona_info *px_persona = NULL;
#endif
	struct _posix_spawn_posix_cred_info *px_pcred_info = NULL;
	struct {
		struct image_params imgp;
		struct vnode_attr va;
		struct vnode_attr origva;
	} *__spawn_data;

	/*
	 * Allocate a big chunk for locals instead of using stack since these
	 * structures are pretty big.
	 */
	__spawn_data = kalloc_type(typeof(*__spawn_data), Z_WAITOK | Z_ZERO);
	if (__spawn_data == NULL) {
		error = ENOMEM;
		goto bad;
	}
	imgp = &__spawn_data->imgp;
	vap = &__spawn_data->va;
	origvap = &__spawn_data->origva;

	/* Initialize the common data in the image_params structure */
	imgp->ip_user_fname = uap->path;
	imgp->ip_user_argv = uap->argv;
	imgp->ip_user_envv = uap->envp;
	imgp->ip_vattr = vap;
	imgp->ip_origvattr = origvap;
	imgp->ip_vfs_context = &context;
	imgp->ip_flags = (is_64 ? IMGPF_WAS_64BIT_ADDR : IMGPF_NONE);
	imgp->ip_seg = (is_64 ? UIO_USERSPACE64 : UIO_USERSPACE32);
	imgp->ip_mac_return = 0;
	imgp->ip_px_persona = NULL;
	imgp->ip_px_pcred_info = NULL;
	imgp->ip_cs_error = OS_REASON_NULL;
	imgp->ip_simulator_binary = IMGPF_SB_DEFAULT;
	imgp->ip_subsystem_root_path = NULL;
	imgp->ip_inherited_shared_region_id = NULL;
	imgp->ip_inherited_jop_pid = 0;
	uthread_set_exec_data(current_uthread(), imgp);

	if (uap->adesc != USER_ADDR_NULL) {
		if (is_64) {
			error = copyin(uap->adesc, &px_args, sizeof(px_args));
		} else {
			struct user32__posix_spawn_args_desc px_args32;

			error = copyin(uap->adesc, &px_args32, sizeof(px_args32));

			/*
			 * Convert arguments descriptor from external 32 bit
			 * representation to internal 64 bit representation
			 */
			px_args.attr_size = px_args32.attr_size;
			px_args.attrp = CAST_USER_ADDR_T(px_args32.attrp);
			px_args.file_actions_size = px_args32.file_actions_size;
			px_args.file_actions = CAST_USER_ADDR_T(px_args32.file_actions);
			px_args.port_actions_size = px_args32.port_actions_size;
			px_args.port_actions = CAST_USER_ADDR_T(px_args32.port_actions);
			px_args.mac_extensions_size = px_args32.mac_extensions_size;
			px_args.mac_extensions = CAST_USER_ADDR_T(px_args32.mac_extensions);
			px_args.coal_info_size = px_args32.coal_info_size;
			px_args.coal_info = CAST_USER_ADDR_T(px_args32.coal_info);
			px_args.persona_info_size = px_args32.persona_info_size;
			px_args.persona_info = CAST_USER_ADDR_T(px_args32.persona_info);
			px_args.posix_cred_info_size = px_args32.posix_cred_info_size;
			px_args.posix_cred_info = CAST_USER_ADDR_T(px_args32.posix_cred_info);
			px_args.subsystem_root_path_size = px_args32.subsystem_root_path_size;
			px_args.subsystem_root_path = CAST_USER_ADDR_T(px_args32.subsystem_root_path);
			px_args.conclave_id_size = px_args32.conclave_id_size;
			px_args.conclave_id = CAST_USER_ADDR_T(px_args32.conclave_id);
		}
		if (error) {
			goto bad;
		}

		if (px_args.attr_size != 0) {
			/*
			 * We are not copying the port_actions pointer,
			 * because we already have it from px_args.
			 * This is a bit fragile: <rdar://problem/16427422>
			 */

			if ((error = copyin(px_args.attrp, &px_sa, px_sa_offset)) != 0) {
				goto bad;
			}

			imgp->ip_px_sa = &px_sa;
		}
		if (px_args.file_actions_size != 0) {
			/* Limit file_actions to allowed number of open files */
			size_t maxfa_size = PSF_ACTIONS_SIZE(proc_limitgetcur_nofile(p));

			if (px_args.file_actions_size < PSF_ACTIONS_SIZE(1) ||
			    maxfa_size == 0 || px_args.file_actions_size > maxfa_size) {
				error = EINVAL;
				goto bad;
			}

			px_sfap = kalloc_data(px_args.file_actions_size, Z_WAITOK);
			if (px_sfap == NULL) {
				error = ENOMEM;
				goto bad;
			}
			imgp->ip_px_sfa = px_sfap;

			if ((error = copyin(px_args.file_actions, px_sfap,
			    px_args.file_actions_size)) != 0) {
				goto bad;
			}

			/* Verify that the action count matches the struct size */
			size_t psfsize = PSF_ACTIONS_SIZE(px_sfap->psfa_act_count);
			if (psfsize == 0 || psfsize != px_args.file_actions_size) {
				error = EINVAL;
				goto bad;
			}
		}
		if (px_args.port_actions_size != 0) {
			/* Limit port_actions to one page of data */
			if (px_args.port_actions_size < PS_PORT_ACTIONS_SIZE(1) ||
			    px_args.port_actions_size > PAGE_SIZE) {
				error = EINVAL;
				goto bad;
			}

			px_spap = kalloc_data(px_args.port_actions_size, Z_WAITOK);
			if (px_spap == NULL) {
				error = ENOMEM;
				goto bad;
			}
			imgp->ip_px_spa = px_spap;

			if ((error = copyin(px_args.port_actions, px_spap,
			    px_args.port_actions_size)) != 0) {
				goto bad;
			}

			/* Verify that the action count matches the struct size */
			size_t pasize = PS_PORT_ACTIONS_SIZE(px_spap->pspa_count);
			if (pasize == 0 || pasize != px_args.port_actions_size) {
				error = EINVAL;
				goto bad;
			}
		}
#if CONFIG_PERSONAS
		/* copy in the persona info */
		if (px_args.persona_info_size != 0 && px_args.persona_info != 0) {
			/* for now, we need the exact same struct in user space */
			if (px_args.persona_info_size != sizeof(*px_persona)) {
				error = ERANGE;
				goto bad;
			}

			px_persona = kalloc_data(px_args.persona_info_size, Z_WAITOK);
			if (px_persona == NULL) {
				error = ENOMEM;
				goto bad;
			}
			imgp->ip_px_persona = px_persona;

			if ((error = copyin(px_args.persona_info, px_persona,
			    px_args.persona_info_size)) != 0) {
				goto bad;
			}
			if ((error = spawn_validate_persona(px_persona)) != 0) {
				goto bad;
			}
		}
#endif
		/* copy in the posix cred info */
		if (px_args.posix_cred_info_size != 0 && px_args.posix_cred_info != 0) {
			/* for now, we need the exact same struct in user space */
			if (px_args.posix_cred_info_size != sizeof(*px_pcred_info)) {
				error = ERANGE;
				goto bad;
			}

			if (!kauth_cred_issuser(kauth_cred_get())) {
				error = EPERM;
				goto bad;
			}

			px_pcred_info = kalloc_data(px_args.posix_cred_info_size, Z_WAITOK);
			if (px_pcred_info == NULL) {
				error = ENOMEM;
				goto bad;
			}
			imgp->ip_px_pcred_info = px_pcred_info;

			if ((error = copyin(px_args.posix_cred_info, px_pcred_info,
			    px_args.posix_cred_info_size)) != 0) {
				goto bad;
			}

			if (px_pcred_info->pspci_flags & POSIX_SPAWN_POSIX_CRED_GROUPS) {
				if (px_pcred_info->pspci_ngroups > NGROUPS_MAX) {
					error = EINVAL;
					goto bad;
				}
			}
		}
#if CONFIG_MACF
		if (px_args.mac_extensions_size != 0) {
			if ((error = spawn_copyin_macpolicyinfo(&px_args, (struct ip_px_smpx_s *)&imgp->ip_px_smpx)) != 0) {
				goto bad;
			}
		}
#endif /* CONFIG_MACF */
		if ((px_args.subsystem_root_path_size > 0) && (px_args.subsystem_root_path_size <= MAXPATHLEN)) {
			/*
			 * If a valid-looking subsystem root has been
			 * specified...
			 */
			if (IOTaskHasEntitlement(old_task, SPAWN_SUBSYSTEM_ROOT_ENTITLEMENT)) {
				/*
				 * ...AND the parent has the entitlement, copy
				 * the subsystem root path in.
				 */
				subsystem_root_path = zalloc_flags(ZV_NAMEI,
				    Z_WAITOK | Z_ZERO | Z_NOFAIL);

				if ((error = copyin(px_args.subsystem_root_path, subsystem_root_path, px_args.subsystem_root_path_size))) {
					goto bad;
				}

				/* Paranoia */
				subsystem_root_path[px_args.subsystem_root_path_size - 1] = 0;
			}
		}
#if CONFIG_EXCLAVES

		/*
		 * Calling exclaves_boot_wait() ensures that the conclave name
		 * id will only be set when exclaves are actually
		 * supported/enabled. In practice this will never actually block
		 * as by the time this is called the system will have booted to
		 * EXCLAVECORE if it's supported/enabled.
		 */
		if ((px_args.conclave_id_size > 0) && (px_args.conclave_id_size <= MAXCONCLAVENAME) &&
		    (exclaves_boot_wait(EXCLAVES_BOOT_STAGE_EXCLAVECORE) == KERN_SUCCESS)) {
			if (px_args.conclave_id) {
				if (imgp->ip_px_sa != NULL && (px_sa.psa_flags & POSIX_SPAWN_SETEXEC)) {
					/* Conclave id could be set only for true spawn */
					error = EINVAL;
					goto bad;
				}
				task_conclave_id = kalloc_data(MAXCONCLAVENAME,
				    Z_WAITOK | Z_ZERO | Z_NOFAIL);
				if ((error = copyin(px_args.conclave_id, task_conclave_id, MAXCONCLAVENAME))) {
					goto bad;
				}
				task_conclave_id[MAXCONCLAVENAME - 1] = 0;
			}
		}
#endif
	}

	if (IOTaskHasEntitlement(old_task, SPAWN_SET_PANIC_CRASH_BEHAVIOR)) {
		/* Truncate to uint8_t since we only support 2 flags for now */
		crash_behavior = (uint8_t)px_sa.psa_crash_behavior;
		crash_behavior_deadline = px_sa.psa_crash_behavior_deadline;
	}

	/* set uthread to parent */
	uthread = current_uthread();

	/*
	 * <rdar://6640530>; this does not result in a behaviour change
	 * relative to Leopard, so there should not be any existing code
	 * which depends on it.
	 */

	if (imgp->ip_px_sa != NULL) {
		struct _posix_spawnattr *psa = (struct _posix_spawnattr *) imgp->ip_px_sa;
		if ((psa->psa_options & PSA_OPTION_PLUGIN_HOST_DISABLE_A_KEYS) == PSA_OPTION_PLUGIN_HOST_DISABLE_A_KEYS) {
			imgp->ip_flags |= IMGPF_PLUGIN_HOST_DISABLE_A_KEYS;
		}


#if (DEVELOPMENT || DEBUG)
		if ((psa->psa_options & PSA_OPTION_ALT_ROSETTA) == PSA_OPTION_ALT_ROSETTA) {
			imgp->ip_flags |= (IMGPF_ROSETTA | IMGPF_ALT_ROSETTA);
		}
#endif /* (DEVELOPMENT || DEBUG) */

		if ((error = exec_validate_spawnattr_policy(psa->psa_apptype)) != 0) {
			goto bad;
		}
	}

	/*
	 * If we don't have the extension flag that turns "posix_spawn()"
	 * into "execve() with options", then we will be creating a new
	 * process which does not inherit memory from the parent process,
	 * which is one of the most expensive things about using fork()
	 * and execve().
	 */
	if (imgp->ip_px_sa == NULL || !(px_sa.psa_flags & POSIX_SPAWN_SETEXEC)) {
		/* Set the new task's coalition, if it is requested.  */
		coalition_t coal[COALITION_NUM_TYPES] = { COALITION_NULL };
#if CONFIG_COALITIONS
		int i, ncoals;
		kern_return_t kr = KERN_SUCCESS;
		struct _posix_spawn_coalition_info coal_info;
		int coal_role[COALITION_NUM_TYPES];

		if (imgp->ip_px_sa == NULL || !px_args.coal_info) {
			goto do_fork1;
		}

		memset(&coal_info, 0, sizeof(coal_info));

		if (px_args.coal_info_size > sizeof(coal_info)) {
			px_args.coal_info_size = sizeof(coal_info);
		}
		error = copyin(px_args.coal_info,
		    &coal_info, px_args.coal_info_size);
		if (error != 0) {
			goto bad;
		}

		ncoals = 0;
		for (i = 0; i < COALITION_NUM_TYPES; i++) {
			uint64_t cid = coal_info.psci_info[i].psci_id;
			if (cid != 0) {
				/*
				 * don't allow tasks which are not in a
				 * privileged coalition to spawn processes
				 * into coalitions other than their own
				 */
				if (!task_is_in_privileged_coalition(proc_task(p), i) &&
				    !IOTaskHasEntitlement(proc_task(p), COALITION_SPAWN_ENTITLEMENT)) {
					coal_dbg("ERROR: %d not in privilegd "
					    "coalition of type %d",
					    proc_getpid(p), i);
					spawn_coalitions_release_all(coal);
					error = EPERM;
					goto bad;
				}

				coal_dbg("searching for coalition id:%llu", cid);
				/*
				 * take a reference and activation on the
				 * coalition to guard against free-while-spawn
				 * races
				 */
				coal[i] = coalition_find_and_activate_by_id(cid);
				if (coal[i] == COALITION_NULL) {
					coal_dbg("could not find coalition id:%llu "
					    "(perhaps it has been terminated or reaped)", cid);
					/*
					 * release any other coalition's we
					 * may have a reference to
					 */
					spawn_coalitions_release_all(coal);
					error = ESRCH;
					goto bad;
				}
				if (coalition_type(coal[i]) != i) {
					coal_dbg("coalition with id:%lld is not of type:%d"
					    " (it's type:%d)", cid, i, coalition_type(coal[i]));
					spawn_coalitions_release_all(coal);
					error = ESRCH;
					goto bad;
				}
				coal_role[i] = coal_info.psci_info[i].psci_role;
				ncoals++;
			}
		}
		if (ncoals < COALITION_NUM_TYPES) {
			/*
			 * If the user is attempting to spawn into a subset of
			 * the known coalition types, then make sure they have
			 * _at_least_ specified a resource coalition. If not,
			 * the following fork1() call will implicitly force an
			 * inheritance from 'p' and won't actually spawn the
			 * new task into the coalitions the user specified.
			 * (also the call to coalitions_set_roles will panic)
			 */
			if (coal[COALITION_TYPE_RESOURCE] == COALITION_NULL) {
				spawn_coalitions_release_all(coal);
				error = EINVAL;
				goto bad;
			}
		}
do_fork1:
#endif /* CONFIG_COALITIONS */

		/*
		 * note that this will implicitly inherit the
		 * caller's persona (if it exists)
		 */
		error = fork1(p, &imgp->ip_new_thread, PROC_CREATE_SPAWN, coal);
		/* returns a thread and task reference */

		if (error == 0) {
			new_task = get_threadtask(imgp->ip_new_thread);
		}
#if CONFIG_COALITIONS
		/* set the roles of this task within each given coalition */
		if (error == 0) {
			kr = coalitions_set_roles(coal, new_task, coal_role);
			if (kr != KERN_SUCCESS) {
				error = EINVAL;
			}
			if (kdebug_debugid_enabled(MACHDBG_CODE(DBG_MACH_COALITION,
			    MACH_COALITION_ADOPT))) {
				for (i = 0; i < COALITION_NUM_TYPES; i++) {
					if (coal[i] != COALITION_NULL) {
						/*
						 * On 32-bit targets, uniqueid
						 * will get truncated to 32 bits
						 */
						KDBG_RELEASE(MACHDBG_CODE(
							    DBG_MACH_COALITION,
							    MACH_COALITION_ADOPT),
						    coalition_id(coal[i]),
						    get_task_uniqueid(new_task));
					}
				}
			}
		}

		/* drop our references and activations - fork1() now holds them */
		spawn_coalitions_release_all(coal);
#endif /* CONFIG_COALITIONS */
		if (error != 0) {
			goto bad;
		}
		imgp->ip_flags |= IMGPF_SPAWN;  /* spawn w/o exec */
		spawn_no_exec = TRUE;           /* used in later tests */
	} else {
		/* Adjust the user proc count */
		(void)chgproccnt(kauth_getruid(), 1);
		/*
		 * For execve case, create a new proc, task and thread
		 * but don't make the proc visible to userland. After
		 * image activation, the new proc would take place of
		 * the old proc in pid hash and other lists that make
		 * the proc visible to the system.
		 */
		imgp->ip_new_thread = cloneproc(old_task, NULL, p, CLONEPROC_EXEC);

		/* task and thread ref returned by cloneproc */
		if (imgp->ip_new_thread == NULL) {
			(void)chgproccnt(kauth_getruid(), -1);
			error = ENOMEM;
			goto bad;
		}

		new_task = get_threadtask(imgp->ip_new_thread);
		imgp->ip_flags |= IMGPF_EXEC;
	}

	p = (proc_t)get_bsdthreadtask_info(imgp->ip_new_thread);

	if (spawn_no_exec) {
		/*
		 * We had to wait until this point before firing the
		 * proc:::create probe, otherwise p would not point to the
		 * child process.
		 */
		DTRACE_PROC1(create, proc_t, p);
	}
	assert(p != NULL);

	if (subsystem_root_path) {
		/* If a subsystem root was specified, swap it in */
		char * old_subsystem_root_path = p->p_subsystem_root_path;
		p->p_subsystem_root_path = subsystem_root_path;
		subsystem_root_path = old_subsystem_root_path;
	}

	p->p_crash_behavior = crash_behavior;
	p->p_crash_behavior_deadline = crash_behavior_deadline;

	p->p_crash_count = px_sa.psa_crash_count;
	p->p_throttle_timeout = px_sa.psa_throttle_timeout;

	/* We'll need the subsystem root for setting up Apple strings */
	imgp->ip_subsystem_root_path = p->p_subsystem_root_path;

	context.vc_thread = imgp->ip_new_thread;
	context.vc_ucred = proc_ucred_unsafe(p);  /* in init */

	/*
	 * Post fdt_fork(), pre exec_handle_sugid() - this is where we want
	 * to handle the file_actions.
	 */

	/* Has spawn file actions? */
	if (imgp->ip_px_sfa != NULL) {
		/*
		 * The POSIX_SPAWN_CLOEXEC_DEFAULT flag
		 * is handled in exec_handle_file_actions().
		 */
#if CONFIG_AUDIT
		/*
		 * The file actions auditing can overwrite the upath of
		 * AUE_POSIX_SPAWN audit record.  Save the audit record.
		 */
		struct kaudit_record *save_uu_ar = uthread->uu_ar;
		uthread->uu_ar = NULL;
#endif
		error = exec_handle_file_actions(imgp,
		    imgp->ip_px_sa != NULL ? px_sa.psa_flags : 0);
#if CONFIG_AUDIT
		/* Restore the AUE_POSIX_SPAWN audit record. */
		uthread->uu_ar = save_uu_ar;
#endif
		if (error != 0) {
			goto bad;
		}
	}

	/* Has spawn port actions? */
	if (imgp->ip_px_spa != NULL) {
#if CONFIG_AUDIT
		/*
		 * Do the same for the port actions as we did for the file
		 * actions.  Save the AUE_POSIX_SPAWN audit record.
		 */
		struct kaudit_record *save_uu_ar = uthread->uu_ar;
		uthread->uu_ar = NULL;
#endif
		error = exec_handle_port_actions(imgp, &port_actions);
#if CONFIG_AUDIT
		/* Restore the AUE_POSIX_SPAWN audit record. */
		uthread->uu_ar = save_uu_ar;
#endif
		if (error != 0) {
			goto bad;
		}
	}

	/* Has spawn attr? */
	if (imgp->ip_px_sa != NULL) {
		/*
		 * Reset UID/GID to parent's RUID/RGID; This works only
		 * because the operation occurs before the call
		 * to exec_handle_sugid() by the image activator called
		 * from exec_activate_image().
		 *
		 * POSIX requires that any setuid/setgid bits on the process
		 * image will take precedence over the spawn attributes
		 * (re)setting them.
		 *
		 * Modifications to p_ucred must be guarded using the
		 * proc's ucred lock. This prevents others from accessing
		 * a garbage credential.
		 */
		if (px_sa.psa_flags & POSIX_SPAWN_RESETIDS) {
			kauth_cred_proc_update(p, PROC_SETTOKEN_NONE,
			    ^bool (kauth_cred_t parent __unused, kauth_cred_t model){
				return kauth_cred_model_setuidgid(model,
				kauth_cred_getruid(parent),
				kauth_cred_getrgid(parent));
			});
		}

		if (imgp->ip_px_pcred_info) {
			if (!spawn_no_exec) {
				error = ENOTSUP;
				goto bad;
			}

			error = spawn_posix_cred_adopt(p, imgp->ip_px_pcred_info);
			if (error != 0) {
				goto bad;
			}
		}

#if CONFIG_PERSONAS
		if (imgp->ip_px_persona != NULL) {
			if (!spawn_no_exec) {
				error = ENOTSUP;
				goto bad;
			}

			/*
			 * If we were asked to spawn a process into a new persona,
			 * do the credential switch now (which may override the UID/GID
			 * inherit done just above). It's important to do this switch
			 * before image activation both for reasons stated above, and
			 * to ensure that the new persona has access to the image/file
			 * being executed.
			 */
			error = spawn_persona_adopt(p, imgp->ip_px_persona);
			if (error != 0) {
				goto bad;
			}
		}
#endif /* CONFIG_PERSONAS */
#if !SECURE_KERNEL
		/*
		 * Disable ASLR for the spawned process.
		 *
		 * But only do so if we are not embedded + RELEASE.
		 * While embedded allows for a boot-arg (-disable_aslr)
		 * to deal with this (which itself is only honored on
		 * DEVELOPMENT or DEBUG builds of xnu), it is often
		 * useful or necessary to disable ASLR on a per-process
		 * basis for unit testing and debugging.
		 */
		if (px_sa.psa_flags & _POSIX_SPAWN_DISABLE_ASLR) {
			OSBitOrAtomic(P_DISABLE_ASLR, &p->p_flag);
		}
#endif /* !SECURE_KERNEL */

		/* Randomize high bits of ASLR slide */
		if (px_sa.psa_flags & _POSIX_SPAWN_HIGH_BITS_ASLR) {
			imgp->ip_flags |= IMGPF_HIGH_BITS_ASLR;
		}

#if !SECURE_KERNEL
		/*
		 * Forcibly disallow execution from data pages for the spawned process
		 * even if it would otherwise be permitted by the architecture default.
		 */
		if (px_sa.psa_flags & _POSIX_SPAWN_ALLOW_DATA_EXEC) {
			imgp->ip_flags |= IMGPF_ALLOW_DATA_EXEC;
		}
#endif /* !SECURE_KERNEL */

#if     __has_feature(ptrauth_calls)
		if (vm_shared_region_reslide_aslr && is_64 && (px_sa.psa_flags & _POSIX_SPAWN_RESLIDE)) {
			imgp->ip_flags |= IMGPF_RESLIDE;
		}
#endif /* __has_feature(ptrauth_calls) */

		if ((px_sa.psa_apptype & POSIX_SPAWN_PROC_TYPE_MASK) ==
		    POSIX_SPAWN_PROC_TYPE_DRIVER) {
			imgp->ip_flags |= IMGPF_DRIVER;
		}
	}

	/*
	 * Disable ASLR during image activation.  This occurs either if the
	 * _POSIX_SPAWN_DISABLE_ASLR attribute was found above or if
	 * P_DISABLE_ASLR was inherited from the parent process.
	 */
	if (p->p_flag & P_DISABLE_ASLR) {
		imgp->ip_flags |= IMGPF_DISABLE_ASLR;
	}

	/*
	 * Clear transition flag so we won't hang if exec_activate_image() causes
	 * an automount (and launchd does a proc sysctl to service it).
	 *
	 * <rdar://problem/6848672>, <rdar://problem/5959568>.
	 */
	proc_transend(p, 0);
	proc_transit_set = 0;

	if (!spawn_no_exec) {
		/*
		 * Clear the signal lock in case of exec, since
		 * image activation uses psignal on child process.
		 */
		proc_signalend(p, 0);
		proc_signal_set = 0;
	}

#if MAC_SPAWN   /* XXX */
	if (uap->mac_p != USER_ADDR_NULL) {
		error = mac_execve_enter(uap->mac_p, imgp);
		if (error) {
			goto bad;
		}
	}
#endif
	/*
	 * Activate the image.
	 * Warning: If activation failed after point of no return, it returns error
	 * as 0 and pretends the call succeeded.
	 */
	error = exec_activate_image(imgp);
#if defined(HAS_APPLE_PAC)
	const uint8_t disable_user_jop = imgp->ip_flags & IMGPF_NOJOP ? TRUE : FALSE;
	ml_task_set_jop_pid_from_shared_region(new_task, disable_user_jop);
	ml_task_set_disable_user_jop(new_task, disable_user_jop);
	ml_thread_set_disable_user_jop(imgp->ip_new_thread, disable_user_jop);
	ml_thread_set_jop_pid(imgp->ip_new_thread, new_task);
#endif

	/*
	 * If you've come here to add support for some new HW feature or some per-process or per-vmmap
	 * or per-pmap flag that needs to be set before the process runs, or are in general lost, here
	 * is some help. This summary was accurate as of Jul 2022. Use git log as needed. This comment
	 * is here to prevent a recurrence of rdar://96307913
	 *
	 * In posix_spawn, following is what happens:
	 * 1. Lots of prep and checking work
	 * 2. Image activation via exec_activate_image(). The new task will get a new pmap here
	 * 3. More prep work. (YOU ARE HERE)
	 * 4. exec_resettextvp() is called
	 * 5. At this point it is safe to check entitlements and code signatures
	 * 6. task_clear_return_wait(get_threadtask(imgp->ip_new_thread), TCRW_CLEAR_INITIAL_WAIT);
	 *    The new thread is allowed to run in kernel. It cannot yet get to userland
	 * 7. More things done here. This is your chance to affect the task before it runs in
	 *    userspace
	 * 8. task_clear_return_wait(get_threadtask(imgp->ip_new_thread), TCRW_CLEAR_FINAL_WAIT);
	 *     The new thread is allowed to run in userland
	 */

	if (error == 0 && !spawn_no_exec) {
		p = proc_exec_switch_task(current_proc(), p, old_task, new_task, imgp, &inherit);
		/* proc ref returned */
		should_release_proc_ref = TRUE;
	}

	if (error == 0) {
		/* process completed the exec, but may have failed after point of no return */
		exec_done = TRUE;
	}

#if CONFIG_EXCLAVES
	if (!error && task_conclave_id != NULL) {
		kern_return_t kr;
		kr = task_add_conclave(new_task, imgp->ip_vp, (int64_t)imgp->ip_arch_offset,
		    task_conclave_id);
		if (kr != KERN_SUCCESS) {
			error = EINVAL;
			goto bad;
		}
	}
#endif

	if (!error && imgp->ip_px_sa != NULL) {
		thread_t child_thread = imgp->ip_new_thread;
		uthread_t child_uthread = get_bsdthread_info(child_thread);

		/*
		 * Because of POSIX_SPAWN_SETEXEC, we need to handle this after image
		 * activation, else when image activation fails (before the point of no
		 * return) would leave the parent process in a modified state.
		 */
		if (px_sa.psa_flags & POSIX_SPAWN_SETPGROUP) {
			struct setpgid_args spga;
			spga.pid = proc_getpid(p);
			spga.pgid = px_sa.psa_pgroup;
			/*
			 * Effectively, call setpgid() system call; works
			 * because there are no pointer arguments.
			 */
			if ((error = setpgid(p, &spga, ival)) != 0) {
				goto bad_px_sa;
			}
		}

		if (px_sa.psa_flags & POSIX_SPAWN_SETSID) {
			error = setsid_internal(p);
			if (error != 0) {
				goto bad_px_sa;
			}
		}

		/*
		 * If we have a spawn attr, and it contains signal related flags,
		 * the we need to process them in the "context" of the new child
		 * process, so we have to process it following image activation,
		 * prior to making the thread runnable in user space.  This is
		 * necessitated by some signal information being per-thread rather
		 * than per-process, and we don't have the new allocation in hand
		 * until after the image is activated.
		 */

		/*
		 * Mask a list of signals, instead of them being unmasked, if
		 * they were unmasked in the parent; note that some signals
		 * are not maskable.
		 */
		if (px_sa.psa_flags & POSIX_SPAWN_SETSIGMASK) {
			child_uthread->uu_sigmask = (px_sa.psa_sigmask & ~sigcantmask);
		}
		/*
		 * Default a list of signals instead of ignoring them, if
		 * they were ignored in the parent.  Note that we pass
		 * spawn_no_exec to setsigvec() to indicate that we called
		 * fork1() and therefore do not need to call proc_signalstart()
		 * internally.
		 */
		if (px_sa.psa_flags & POSIX_SPAWN_SETSIGDEF) {
			vec.sa_handler = SIG_DFL;
			vec.sa_tramp = 0;
			vec.sa_mask = 0;
			vec.sa_flags = 0;
			for (sig = 1; sig < NSIG; sig++) {
				if (px_sa.psa_sigdefault & (1 << (sig - 1))) {
					error = setsigvec(p, child_thread, sig, &vec, spawn_no_exec);
				}
			}
		}

		/*
		 * Activate the CPU usage monitor, if requested. This is done via a task-wide, per-thread CPU
		 * usage limit, which will generate a resource exceeded exception if any one thread exceeds the
		 * limit.
		 *
		 * Userland gives us interval in seconds, and the kernel SPI expects nanoseconds.
		 */
		if ((px_sa.psa_cpumonitor_percent != 0) && (px_sa.psa_cpumonitor_percent < UINT8_MAX)) {
			/*
			 * Always treat a CPU monitor activation coming from spawn as entitled. Requiring
			 * an entitlement to configure the monitor a certain way seems silly, since
			 * whomever is turning it on could just as easily choose not to do so.
			 */
			error = proc_set_task_ruse_cpu(proc_task(p),
			    TASK_POLICY_RESOURCE_ATTRIBUTE_NOTIFY_EXC,
			    (uint8_t)px_sa.psa_cpumonitor_percent,
			    px_sa.psa_cpumonitor_interval * NSEC_PER_SEC,
			    0, TRUE);
		}


		if (px_pcred_info &&
		    (px_pcred_info->pspci_flags & POSIX_SPAWN_POSIX_CRED_LOGIN)) {
			/*
			 * setlogin() must happen after setsid()
			 */
			setlogin_internal(p, px_pcred_info->pspci_login);
		}

bad_px_sa:
		if (error != 0) {
			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_BAD_PSATTR, 0, 0);
			exec_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_BAD_PSATTR);
		}
	}

bad:

	if (error == 0) {
		/* reset delay idle sleep status if set */
#if CONFIG_DELAY_IDLE_SLEEP
		if ((p->p_flag & P_DELAYIDLESLEEP) == P_DELAYIDLESLEEP) {
			OSBitAndAtomic(~((uint32_t)P_DELAYIDLESLEEP), &p->p_flag);
		}
#endif /* CONFIG_DELAY_IDLE_SLEEP */
		/* upon  successful spawn, re/set the proc control state */
		if (imgp->ip_px_sa != NULL) {
			switch (px_sa.psa_pcontrol) {
			case POSIX_SPAWN_PCONTROL_THROTTLE:
				p->p_pcaction = P_PCTHROTTLE;
				break;
			case POSIX_SPAWN_PCONTROL_SUSPEND:
				p->p_pcaction = P_PCSUSP;
				break;
			case POSIX_SPAWN_PCONTROL_KILL:
				p->p_pcaction = P_PCKILL;
				break;
			case POSIX_SPAWN_PCONTROL_NONE:
			default:
				p->p_pcaction = 0;
				break;
			}
			;
		}
		exec_resettextvp(p, imgp);

		/*
		 * Enable new task IPC access if exec_activate_image() returned an
		 * active task. (Checks active bit in ipc_task_enable() under lock).
		 * Must enable after resettextvp so that task port policies are not evaluated
		 * until the csblob in the textvp is accurately reflected.
		 */
		vm_map_setup(get_task_map(new_task), new_task);
		ipc_task_enable(new_task);

		/* Set task exception ports now that we can check entitlements */
		if (imgp->ip_px_spa != NULL) {
			error = exec_handle_exception_port_actions(imgp, &port_actions);
		}

#if CONFIG_MEMORYSTATUS
		/* Set jetsam priority for DriverKit processes */
		if (px_sa.psa_apptype == POSIX_SPAWN_PROC_TYPE_DRIVER) {
			px_sa.psa_priority = JETSAM_PRIORITY_DRIVER_APPLE;
		}

		/* Has jetsam attributes? */
		if (imgp->ip_px_sa != NULL && (px_sa.psa_jetsam_flags & POSIX_SPAWN_JETSAM_SET)) {
			int32_t memlimit_active = px_sa.psa_memlimit_active;
			int32_t memlimit_inactive = px_sa.psa_memlimit_inactive;

			memstat_priority_options_t priority_options = MEMSTAT_PRIORITY_OPTIONS_NONE;
			if ((px_sa.psa_jetsam_flags & POSIX_SPAWN_JETSAM_USE_EFFECTIVE_PRIORITY)) {
				priority_options |= MEMSTAT_PRIORITY_IS_EFFECTIVE;
			}
			memorystatus_set_priority(p, px_sa.psa_priority, 0,
			    priority_options);

			memlimit_options_t memlimit_options = MEMLIMIT_OPTIONS_NONE;
			if ((px_sa.psa_jetsam_flags & POSIX_SPAWN_JETSAM_MEMLIMIT_ACTIVE_FATAL)) {
				memlimit_options |= MEMLIMIT_ACTIVE_FATAL;
			}
			if ((px_sa.psa_jetsam_flags & POSIX_SPAWN_JETSAM_MEMLIMIT_INACTIVE_FATAL)) {
				memlimit_options |= MEMLIMIT_INACTIVE_FATAL;
			}
			if (px_sa.psa_jetsam_flags & POSIX_SPAWN_JETSAM_HIWATER_BACKGROUND) {
				/*
				 * With 2-level high-water-mark support,
				 * POSIX_SPAWN_JETSAM_HIWATER_BACKGROUND is no longer relevant,
				 * as background limits are described via the inactive limit
				 * slots. However, if the
				 * POSIX_SPAWN_JETSAM_HIWATER_BACKGROUND is passed in, we
				 * attempt to mimic previous behavior by forcing the BG limit
				 * data into the inactive/non-fatal mode and force the active
				 * slots to hold system_wide/fatal mode.
				 */
				memlimit_options |= MEMLIMIT_ACTIVE_FATAL;
				memlimit_options &= ~MEMLIMIT_INACTIVE_FATAL;
				memlimit_active = -1;
			}
			memorystatus_set_memlimits(p, memlimit_active, memlimit_inactive,
			    memlimit_options);
		}

		/* Has jetsam relaunch behavior? */
		if (imgp->ip_px_sa != NULL && (px_sa.psa_jetsam_flags & POSIX_SPAWN_JETSAM_RELAUNCH_BEHAVIOR_MASK)) {
			/*
			 * Launchd has passed in data indicating the behavior of this process in response to jetsam.
			 * This data would be used by the jetsam subsystem to determine the position and protection
			 * offered to this process on dirty -> clean transitions.
			 */
			int relaunch_flags = P_MEMSTAT_RELAUNCH_UNKNOWN;
			switch (px_sa.psa_jetsam_flags & POSIX_SPAWN_JETSAM_RELAUNCH_BEHAVIOR_MASK) {
			case POSIX_SPAWN_JETSAM_RELAUNCH_BEHAVIOR_LOW:
				relaunch_flags = P_MEMSTAT_RELAUNCH_LOW;
				break;
			case POSIX_SPAWN_JETSAM_RELAUNCH_BEHAVIOR_MED:
				relaunch_flags = P_MEMSTAT_RELAUNCH_MED;
				break;
			case POSIX_SPAWN_JETSAM_RELAUNCH_BEHAVIOR_HIGH:
				relaunch_flags = P_MEMSTAT_RELAUNCH_HIGH;
				break;
			default:
				break;
			}
			memorystatus_relaunch_flags_update(p, relaunch_flags);
		}

#endif /* CONFIG_MEMORYSTATUS */
		if (imgp->ip_px_sa != NULL && px_sa.psa_thread_limit > 0) {
			task_set_thread_limit(new_task, (uint16_t)px_sa.psa_thread_limit);
		}

#if CONFIG_PROC_RESOURCE_LIMITS
		if (imgp->ip_px_sa != NULL && (px_sa.psa_port_soft_limit > 0 || px_sa.psa_port_hard_limit > 0)) {
			task_set_port_space_limits(new_task, (uint32_t)px_sa.psa_port_soft_limit,
			    (uint32_t)px_sa.psa_port_hard_limit);
		}

		if (imgp->ip_px_sa != NULL && (px_sa.psa_filedesc_soft_limit > 0 || px_sa.psa_filedesc_hard_limit > 0)) {
			proc_set_filedesc_limits(p, (int)px_sa.psa_filedesc_soft_limit,
			    (int)px_sa.psa_filedesc_hard_limit);
		}
		if (imgp->ip_px_sa != NULL && (px_sa.psa_kqworkloop_soft_limit > 0 || px_sa.psa_kqworkloop_hard_limit > 0)) {
			proc_set_kqworkloop_limits(p, (int)px_sa.psa_kqworkloop_soft_limit,
			    (int)px_sa.psa_kqworkloop_hard_limit);
		}
#endif /* CONFIG_PROC_RESOURCE_LIMITS */
	}


	/*
	 * If we successfully called fork1() or cloneproc, we always need
	 * to do this. This is because we come back from that call with
	 * signals blocked in the child, and we have to unblock them, for exec
	 * case they are unblocked before activation, but for true spawn case
	 * we want to wait until after we've performed any spawn actions.
	 * This has to happen before process_signature(), which uses psignal.
	 */
	if (proc_transit_set) {
		proc_transend(p, 0);
	}

	/*
	 * Drop the signal lock on the child which was taken on our
	 * behalf by forkproc()/cloneproc() to prevent signals being
	 * received by the child in a partially constructed state.
	 */
	if (proc_signal_set) {
		proc_signalend(p, 0);
	}

	if (error == 0) {
		/*
		 * We need to initialize the bank context behind the protection of
		 * the proc_trans lock to prevent a race with exit. We can't do this during
		 * exec_activate_image because task_bank_init checks entitlements that
		 * aren't loaded until subsequent calls (including exec_resettextvp).
		 */
		error = proc_transstart(p, 0, 0);

		if (error == 0) {
			task_bank_init(new_task);
			proc_transend(p, 0);
		}

#if __arm64__
		proc_footprint_entitlement_hacks(p, new_task);
#endif /* __arm64__ */

#if XNU_TARGET_OS_OSX
#define SINGLE_JIT_ENTITLEMENT "com.apple.security.cs.single-jit"
		if (IOTaskHasEntitlement(new_task, SINGLE_JIT_ENTITLEMENT)) {
			vm_map_single_jit(get_task_map(new_task));
		}
#endif /* XNU_TARGET_OS_OSX */

#if __has_feature(ptrauth_calls)
		task_set_pac_exception_fatal_flag(new_task);
#endif /* __has_feature(ptrauth_calls) */
		task_set_jit_flags(new_task);
	}

	/* Inherit task role from old task to new task for exec */
	if (error == 0 && !spawn_no_exec) {
		proc_inherit_task_role(new_task, old_task);
	}

#if CONFIG_ARCADE
	if (error == 0) {
		/*
		 * Check to see if we need to trigger an arcade upcall AST now
		 * that the vnode has been reset on the task.
		 */
		arcade_prepare(new_task, imgp->ip_new_thread);
	}
#endif /* CONFIG_ARCADE */

	if (error == 0) {
		proc_apply_jit_and_vm_policies(imgp, p, new_task);
	}

	/* Clear the initial wait on the thread before handling spawn policy */
	if (imgp && imgp->ip_new_thread) {
		task_clear_return_wait(get_threadtask(imgp->ip_new_thread), TCRW_CLEAR_INITIAL_WAIT);
	}

	/*
	 * Apply the spawnattr policy, apptype (which primes the task for importance donation),
	 * and bind any portwatch ports to the new task.
	 * This must be done after the exec so that the child's thread is ready,
	 * and after the in transit state has been released, because priority is
	 * dropped here so we need to be prepared for a potentially long preemption interval
	 *
	 * TODO: Consider splitting this up into separate phases
	 */
	if (error == 0 && imgp->ip_px_sa != NULL) {
		struct _posix_spawnattr *psa = (struct _posix_spawnattr *) imgp->ip_px_sa;

		error = exec_handle_spawnattr_policy(p, imgp->ip_new_thread, psa->psa_apptype, psa->psa_qos_clamp,
		    psa->psa_darwin_role, &port_actions);
	}

	/* Transfer the turnstile watchport boost to new task if in exec */
	if (error == 0 && !spawn_no_exec) {
		task_transfer_turnstile_watchports(old_task, new_task, imgp->ip_new_thread);
	}

	if (error == 0 && imgp->ip_px_sa != NULL) {
		struct _posix_spawnattr *psa = (struct _posix_spawnattr *) imgp->ip_px_sa;

		if (psa->psa_no_smt) {
			task_set_no_smt(new_task);
		}
		if (psa->psa_tecs) {
			task_set_tecs(new_task);
		}
	}

	if (error == 0 && imgp->ip_px_sa != NULL) {
		struct _posix_spawnattr *psa = (struct _posix_spawnattr *) imgp->ip_px_sa;

		if (psa->psa_options & PSA_OPTION_DATALESS_IOPOLICY) {
			struct _iopol_param_t iop_param = {
				.iop_scope = IOPOL_SCOPE_PROCESS,
				.iop_iotype = IOPOL_TYPE_VFS_MATERIALIZE_DATALESS_FILES,
				.iop_policy = psa->psa_dataless_iopolicy,
			};
			error = iopolicysys_vfs_materialize_dataless_files(p, IOPOL_CMD_SET, iop_param.iop_scope,
			    iop_param.iop_policy, &iop_param);
		}
	}

	if (error == 0) {
		/* Apply the main thread qos */
		thread_t main_thread = imgp->ip_new_thread;
		task_set_main_thread_qos(new_task, main_thread);
	}

	/*
	 * Release any ports we kept around for binding to the new task
	 * We need to release the rights even if the posix_spawn has failed.
	 */
	if (imgp->ip_px_spa != NULL) {
		exec_port_actions_destroy(&port_actions);
	}

	/*
	 * We have to delay operations which might throw a signal until after
	 * the signals have been unblocked; however, we want that to happen
	 * after exec_resettextvp() so that the textvp is correct when they
	 * fire.
	 */
	if (error == 0) {
		error = process_signature(p, imgp);

		/*
		 * Pay for our earlier safety; deliver the delayed signals from
		 * the incomplete spawn process now that it's complete.
		 */
		if (imgp != NULL && spawn_no_exec && (p->p_lflag & P_LTRACED)) {
			psignal_vfork(p, proc_task(p), imgp->ip_new_thread, SIGTRAP);
		}

		if (error == 0 && !spawn_no_exec) {
			extern uint64_t kdp_task_exec_meta_flags(task_t task);
			KDBG(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXEC),
			    proc_getpid(p), kdp_task_exec_meta_flags(proc_task(p)));
		}
	}

	if (spawn_no_exec) {
		/* flag the 'fork' has occurred */
		proc_knote(p->p_pptr, NOTE_FORK | proc_getpid(p));
	}

	/* flag exec has occurred, notify only if it has not failed due to FP Key error */
	if (!error && ((p->p_lflag & P_LTERM_DECRYPTFAIL) == 0)) {
		proc_knote(p, NOTE_EXEC);
	}

	if (imgp != NULL) {
		uthread_set_exec_data(current_uthread(), NULL);
		if (imgp->ip_vp) {
			vnode_put(imgp->ip_vp);
		}
		if (imgp->ip_scriptvp) {
			vnode_put(imgp->ip_scriptvp);
		}
		if (imgp->ip_strings) {
			execargs_free(imgp);
		}
		if (imgp->ip_free_map) {
			/* Free the map after dropping iocount on vnode to avoid deadlock */
			vm_map_deallocate(imgp->ip_free_map);
		}
		kfree_data(imgp->ip_px_sfa,
		    px_args.file_actions_size);
		kfree_data(imgp->ip_px_spa,
		    px_args.port_actions_size);
#if CONFIG_PERSONAS
		kfree_data(imgp->ip_px_persona,
		    px_args.persona_info_size);
#endif
		kfree_data(imgp->ip_px_pcred_info,
		    px_args.posix_cred_info_size);

		if (subsystem_root_path != NULL) {
			zfree(ZV_NAMEI, subsystem_root_path);
		}
#if CONFIG_MACF
		struct ip_px_smpx_s *px_s = &imgp->ip_px_smpx;
		kfree_data(px_s->array, px_args.mac_extensions_size);
		kfree_data(px_s->data, (vm_size_t)px_s->datalen);

		if (imgp->ip_execlabelp) {
			mac_cred_label_free(imgp->ip_execlabelp);
			imgp->ip_execlabelp = NULL;
		}
		if (imgp->ip_scriptlabelp) {
			mac_vnode_label_free(imgp->ip_scriptlabelp);
			imgp->ip_scriptlabelp = NULL;
		}
		if (imgp->ip_cs_error != OS_REASON_NULL) {
			os_reason_free(imgp->ip_cs_error);
			imgp->ip_cs_error = OS_REASON_NULL;
		}
		if (imgp->ip_inherited_shared_region_id != NULL) {
			kfree_data(imgp->ip_inherited_shared_region_id,
			    strlen(imgp->ip_inherited_shared_region_id) + 1);
			imgp->ip_inherited_shared_region_id = NULL;
		}
#endif
	}

#if CONFIG_DTRACE
	if (spawn_no_exec) {
		/*
		 * In the original DTrace reference implementation,
		 * posix_spawn() was a libc routine that just
		 * did vfork(2) then exec(2).  Thus the proc::: probes
		 * are very fork/exec oriented.  The details of this
		 * in-kernel implementation of posix_spawn() is different
		 * (while producing the same process-observable effects)
		 * particularly w.r.t. errors, and which thread/process
		 * is constructing what on behalf of whom.
		 */
		if (error) {
			DTRACE_PROC1(spawn__failure, int, error);
		} else {
			DTRACE_PROC(spawn__success);
			/*
			 * Some DTrace scripts, e.g. newproc.d in
			 * /usr/bin, rely on the the 'exec-success'
			 * probe being fired in the child after the
			 * new process image has been constructed
			 * in order to determine the associated pid.
			 *
			 * So, even though the parent built the image
			 * here, for compatibility, mark the new thread
			 * so 'exec-success' fires on it as it leaves
			 * the kernel.
			 */
			dtrace_thread_didexec(imgp->ip_new_thread);
		}
	} else {
		if (error) {
			DTRACE_PROC1(exec__failure, int, error);
		} else {
			dtrace_thread_didexec(imgp->ip_new_thread);
		}
	}

	if ((dtrace_proc_waitfor_hook = dtrace_proc_waitfor_exec_ptr) != NULL) {
		(*dtrace_proc_waitfor_hook)(p);
	}
#endif

#if CONFIG_AUDIT
	if (!error && AUDIT_ENABLED() && p) {
		/* Add the CDHash of the new process to the audit record */
		uint8_t *cdhash = cs_get_cdhash(p);
		if (cdhash) {
			AUDIT_ARG(data, cdhash, sizeof(uint8_t), CS_CDHASH_LEN);
		}
	}
#endif

	/* terminate the new task if exec failed  */
	if (new_task != NULL && task_is_exec_copy(new_task)) {
		task_terminate_internal(new_task);
	}

	if (exec_failure_reason && !spawn_no_exec) {
		psignal_with_reason(p, SIGKILL, exec_failure_reason);
		exec_failure_reason = NULL;
	}

	/* Return to both the parent and the child? */
	if (imgp != NULL && spawn_no_exec) {
		/*
		 * If the parent wants the pid, copy it out
		 */
		if (error == 0 && pid != USER_ADDR_NULL) {
			_Static_assert(sizeof(pid_t) == 4, "posix_spawn() assumes a 32-bit pid_t");
			bool aligned = (pid & 3) == 0;
			if (aligned) {
				(void)copyout_atomic32(proc_getpid(p), pid);
			} else {
				(void)suword(pid, proc_getpid(p));
			}
		}
		retval[0] = error;

		/*
		 * If we had an error, perform an internal reap ; this is
		 * entirely safe, as we have a real process backing us.
		 */
		if (error) {
			proc_list_lock();
			p->p_listflag |= P_LIST_DEADPARENT;
			proc_list_unlock();
			proc_lock(p);
			/* make sure no one else has killed it off... */
			if (p->p_stat != SZOMB && p->exit_thread == NULL) {
				p->exit_thread = current_thread();
				p->p_posix_spawn_failed = true;
				proc_unlock(p);
				exit1(p, 1, (int *)NULL);
			} else {
				/* someone is doing it for us; just skip it */
				proc_unlock(p);
			}
		}
	}

	/*
	 * Do not terminate the current task, if proc_exec_switch_task did not
	 * switch the tasks, terminating the current task without the switch would
	 * result in loosing the SIGKILL status.
	 */
	if (task_did_exec(old_task)) {
		/* Terminate the current task, since exec will start in new task */
		task_terminate_internal(old_task);
	}

	/* Release the thread ref returned by cloneproc/fork1 */
	if (imgp != NULL && imgp->ip_new_thread) {
		/* clear the exec complete flag if there is an error before point of no-return */
		uint32_t clearwait_flags = TCRW_CLEAR_FINAL_WAIT;
		if (!spawn_no_exec && !exec_done && error != 0) {
			clearwait_flags |= TCRW_CLEAR_EXEC_COMPLETE;
		}
		/* wake up the new thread */
		task_clear_return_wait(get_threadtask(imgp->ip_new_thread), clearwait_flags);
		thread_deallocate(imgp->ip_new_thread);
		imgp->ip_new_thread = NULL;
	}

	/* Release the ref returned by cloneproc/fork1 */
	if (new_task) {
		task_deallocate(new_task);
		new_task = NULL;
	}

	if (should_release_proc_ref) {
		proc_rele(p);
	}

	kfree_type(typeof(*__spawn_data), __spawn_data);

	if (inherit != NULL) {
		ipc_importance_release(inherit);
	}

#if CONFIG_EXCLAVES
	if (task_conclave_id != NULL) {
		kfree_data(task_conclave_id, MAXCONCLAVENAME);
	}
#endif

	assert(spawn_no_exec || exec_failure_reason == NULL);
	return error;
}

/*
 * proc_exec_switch_task
 *
 * Parameters:  old_proc		proc before exec
 *		new_proc		proc after exec
 *		old_task		task before exec
 *		new_task		task after exec
 *		imgp			image params
 *		inherit			resulting importance linkage
 *
 * Returns: proc.
 *
 * Note: The function will switch proc in pid hash from old proc to new proc.
 * The switch needs to happen after draining all proc refs and inside
 * a proc list lock. In the case of failure to switch the proc, which
 * might happen if the process received a SIGKILL or jetsam killed it,
 * it will make sure that the new tasks terminates. User proc ref returned
 * to caller.
 *
 * This function is called after point of no return, in the case
 * failure to switch, it will terminate the new task and swallow the
 * error and let the terminated process complete exec and die.
 */
proc_t
proc_exec_switch_task(proc_t old_proc, proc_t new_proc, task_t old_task, task_t new_task, struct image_params *imgp, void **inherit)
{
	boolean_t task_active;
	boolean_t proc_active;
	boolean_t thread_active;
	boolean_t reparent_traced_child = FALSE;
	thread_t old_thread = current_thread();
	thread_t new_thread = imgp->ip_new_thread;

	thread_set_exec_promotion(old_thread);
	old_proc = proc_refdrain_will_exec(old_proc);

	new_proc = proc_refdrain_will_exec(new_proc);
	/* extra proc ref returned to the caller */

	assert(get_threadtask(new_thread) == new_task);
	task_active = task_is_active(new_task);
	proc_active = !(old_proc->p_lflag & P_LEXIT);

	/* Check if the current thread is not aborted due to SIGKILL */
	thread_active = thread_is_active(old_thread);

	/*
	 * Do not switch the proc if the new task or proc is already terminated
	 * as a result of error in exec past point of no return
	 */
	if (proc_active && task_active && thread_active) {
		uthread_t new_uthread = get_bsdthread_info(new_thread);
		uthread_t old_uthread = current_uthread();

		/* Clear dispatchqueue and workloop ast offset */
		new_proc->p_dispatchqueue_offset = 0;
		new_proc->p_dispatchqueue_serialno_offset = 0;
		new_proc->p_dispatchqueue_label_offset = 0;
		new_proc->p_return_to_kernel_offset = 0;
		new_proc->p_pthread_wq_quantum_offset = 0;

		/* If old_proc is session leader, change the leader to new proc */
		session_replace_leader(old_proc, new_proc);

		proc_lock(old_proc);

		/* Copy the signal state, dtrace state and set bsd ast on new thread */
		act_set_astbsd(new_thread);
		new_uthread->uu_siglist |= old_uthread->uu_siglist;
		new_uthread->uu_siglist |= old_proc->p_siglist;
		new_uthread->uu_sigwait = old_uthread->uu_sigwait;
		new_uthread->uu_sigmask = old_uthread->uu_sigmask;
		new_uthread->uu_oldmask = old_uthread->uu_oldmask;
		new_uthread->uu_exit_reason = old_uthread->uu_exit_reason;
#if CONFIG_DTRACE
		new_uthread->t_dtrace_sig = old_uthread->t_dtrace_sig;
		new_uthread->t_dtrace_stop = old_uthread->t_dtrace_stop;
		new_uthread->t_dtrace_resumepid = old_uthread->t_dtrace_resumepid;
		assert(new_uthread->t_dtrace_scratch == NULL);
		new_uthread->t_dtrace_scratch = old_uthread->t_dtrace_scratch;

		old_uthread->t_dtrace_sig = 0;
		old_uthread->t_dtrace_stop = 0;
		old_uthread->t_dtrace_resumepid = 0;
		old_uthread->t_dtrace_scratch = NULL;
#endif

#if CONFIG_PROC_UDATA_STORAGE
		new_proc->p_user_data = old_proc->p_user_data;
#endif /* CONFIG_PROC_UDATA_STORAGE */

		/* Copy the resource accounting info */
		thread_copy_resource_info(new_thread, current_thread());

		/* Clear the exit reason and signal state on old thread */
		old_uthread->uu_exit_reason = NULL;
		old_uthread->uu_siglist = 0;

		task_set_did_exec_flag(old_task);
		task_clear_exec_copy_flag(new_task);

		task_copy_fields_for_exec(new_task, old_task);

		/*
		 * Need to transfer pending watch port boosts to the new task
		 * while still making sure that the old task remains in the
		 * importance linkage. Create an importance linkage from old task
		 * to new task, then switch the task importance base of old task
		 * and new task. After the switch the port watch boost will be
		 * boosting the new task and new task will be donating importance
		 * to old task.
		 */
		*inherit = ipc_importance_exec_switch_task(old_task, new_task);

		/* Transfer parent's ptrace state to child */
		new_proc->p_lflag &= ~(P_LTRACED | P_LSIGEXC | P_LNOATTACH);
		new_proc->p_lflag |= (old_proc->p_lflag & (P_LTRACED | P_LSIGEXC | P_LNOATTACH));
		new_proc->p_oppid = old_proc->p_oppid;

		if (old_proc->p_pptr != new_proc->p_pptr) {
			reparent_traced_child = TRUE;
			new_proc->p_lflag |= P_LTRACE_WAIT;
		}

		proc_unlock(old_proc);

		/* Update the list of proc knotes */
		proc_transfer_knotes(old_proc, new_proc);

		/* Update the proc interval timers */
		proc_inherit_itimers(old_proc, new_proc);

		proc_list_lock();

		/* Insert the new proc in child list of parent proc */
		p_reparentallchildren(old_proc, new_proc);

		/* Switch proc in pid hash */
		phash_replace_locked(old_proc, new_proc);

		/* Transfer the shadow flag to old proc */
		os_atomic_andnot(&new_proc->p_refcount, P_REF_SHADOW, relaxed);
		os_atomic_or(&old_proc->p_refcount, P_REF_SHADOW, relaxed);

		/* Change init proc if launchd exec */
		if (old_proc == initproc) {
			/* Take the ref on new proc after proc_refwake_did_exec */
			initproc = new_proc;
			/* Drop the proc ref on old proc */
			proc_rele(old_proc);
		}

		proc_list_unlock();
#if CONFIG_EXCLAVES
		if (task_inherit_conclave(old_task, new_task, imgp->ip_vp,
		    (int64_t)imgp->ip_arch_offset) != KERN_SUCCESS) {
			task_terminate_internal(new_task);
		}
#endif
	} else {
		task_terminate_internal(new_task);
	}

	proc_refwake_did_exec(new_proc);
	proc_refwake_did_exec(old_proc);

	/* Take a ref on initproc if it changed */
	if (new_proc == initproc) {
		initproc = proc_ref(new_proc, false);
		assert(initproc != PROC_NULL);
	}

	thread_clear_exec_promotion(old_thread);
	proc_rele(old_proc);

	if (reparent_traced_child) {
		proc_t pp = proc_parent(old_proc);
		assert(pp != PROC_NULL);

		proc_reparentlocked(new_proc, pp, 1, 0);
		proc_rele(pp);

		proc_lock(new_proc);
		new_proc->p_lflag &= ~P_LTRACE_WAIT;
		proc_unlock(new_proc);
	}

	return new_proc;
}

/*
 * execve
 *
 * Parameters:	uap->fname		File name to exec
 *		uap->argp		Argument list
 *		uap->envp		Environment list
 *
 * Returns:	0			Success
 *	__mac_execve:EINVAL		Invalid argument
 *	__mac_execve:ENOTSUP		Invalid argument
 *	__mac_execve:EACCES		Permission denied
 *	__mac_execve:EINTR		Interrupted function
 *	__mac_execve:ENOMEM		Not enough space
 *	__mac_execve:EFAULT		Bad address
 *	__mac_execve:ENAMETOOLONG	Filename too long
 *	__mac_execve:ENOEXEC		Executable file format error
 *	__mac_execve:ETXTBSY		Text file busy [misuse of error code]
 *	__mac_execve:???
 *
 * TODO:	Dynamic linker header address on stack is copied via suword()
 */
/* ARGSUSED */
int
execve(proc_t p, struct execve_args *uap, int32_t *retval)
{
	struct __mac_execve_args muap;
	int err;

	memoryshot(DBG_VM_EXECVE, DBG_FUNC_NONE);

	muap.fname = uap->fname;
	muap.argp = uap->argp;
	muap.envp = uap->envp;
	muap.mac_p = USER_ADDR_NULL;
	err = __mac_execve(p, &muap, retval);

	return err;
}

/*
 * __mac_execve
 *
 * Parameters:	uap->fname		File name to exec
 *		uap->argp		Argument list
 *		uap->envp		Environment list
 *		uap->mac_p		MAC label supplied by caller
 *
 * Returns:	0			Success
 *		EINVAL			Invalid argument
 *		ENOTSUP			Not supported
 *		ENOEXEC			Executable file format error
 *	exec_activate_image:EINVAL	Invalid argument
 *	exec_activate_image:EACCES	Permission denied
 *	exec_activate_image:EINTR	Interrupted function
 *	exec_activate_image:ENOMEM	Not enough space
 *	exec_activate_image:EFAULT	Bad address
 *	exec_activate_image:ENAMETOOLONG	Filename too long
 *	exec_activate_image:ENOEXEC	Executable file format error
 *	exec_activate_image:ETXTBSY	Text file busy [misuse of error code]
 *	exec_activate_image:EBADEXEC	The executable is corrupt/unknown
 *	exec_activate_image:???
 *	mac_execve_enter:???
 *
 * TODO:	Dynamic linker header address on stack is copied via suword()
 */
int
__mac_execve(proc_t p, struct __mac_execve_args *uap, int32_t *retval __unused)
{
	struct image_params *imgp = NULL;
	struct vnode_attr *vap = NULL;
	struct vnode_attr *origvap = NULL;
	int error;
	int is_64 = IS_64BIT_PROCESS(p);
	struct vfs_context context;
	struct uthread  *uthread = NULL;
	task_t old_task = current_task();
	task_t new_task = NULL;
	boolean_t should_release_proc_ref = FALSE;
	boolean_t exec_done = FALSE;
	void *inherit = NULL;
	struct {
		struct image_params imgp;
		struct vnode_attr va;
		struct vnode_attr origva;
	} *__execve_data;

	/* Allocate a big chunk for locals instead of using stack since these
	 * structures are pretty big.
	 */
	__execve_data = kalloc_type(typeof(*__execve_data), Z_WAITOK | Z_ZERO);
	if (__execve_data == NULL) {
		error = ENOMEM;
		goto exit_with_error;
	}
	imgp = &__execve_data->imgp;
	vap = &__execve_data->va;
	origvap = &__execve_data->origva;

	/* Initialize the common data in the image_params structure */
	imgp->ip_user_fname = uap->fname;
	imgp->ip_user_argv = uap->argp;
	imgp->ip_user_envv = uap->envp;
	imgp->ip_vattr = vap;
	imgp->ip_origvattr = origvap;
	imgp->ip_vfs_context = &context;
	imgp->ip_flags = (is_64 ? IMGPF_WAS_64BIT_ADDR : IMGPF_NONE) | ((p->p_flag & P_DISABLE_ASLR) ? IMGPF_DISABLE_ASLR : IMGPF_NONE);
	imgp->ip_seg = (is_64 ? UIO_USERSPACE64 : UIO_USERSPACE32);
	imgp->ip_mac_return = 0;
	imgp->ip_cs_error = OS_REASON_NULL;
	imgp->ip_simulator_binary = IMGPF_SB_DEFAULT;
	imgp->ip_subsystem_root_path = NULL;
	uthread_set_exec_data(current_uthread(), imgp);

#if CONFIG_MACF
	if (uap->mac_p != USER_ADDR_NULL) {
		error = mac_execve_enter(uap->mac_p, imgp);
		if (error) {
			goto exit_with_error;
		}
	}
#endif
	uthread = current_uthread();
	{
		imgp->ip_flags |= IMGPF_EXEC;

		/* Adjust the user proc count */
		(void)chgproccnt(kauth_getruid(), 1);
		/*
		 * For execve case, create a new proc, task and thread
		 * but don't make the proc visible to userland. After
		 * image activation, the new proc would take place of
		 * the old proc in pid hash and other lists that make
		 * the proc visible to the system.
		 */
		imgp->ip_new_thread = cloneproc(old_task, NULL, p, CLONEPROC_EXEC);
		/* task and thread ref returned by cloneproc */
		if (imgp->ip_new_thread == NULL) {
			(void)chgproccnt(kauth_getruid(), -1);
			error = ENOMEM;
			goto exit_with_error;
		}

		new_task = get_threadtask(imgp->ip_new_thread);
	}


	p = (proc_t)get_bsdthreadtask_info(imgp->ip_new_thread);

	context.vc_thread = imgp->ip_new_thread;
	context.vc_ucred = kauth_cred_proc_ref(p);      /* XXX must NOT be kauth_cred_get() */

	imgp->ip_subsystem_root_path = p->p_subsystem_root_path;

	proc_transend(p, 0);
	proc_signalend(p, 0);

	/*
	 * Activate the image.
	 * Warning: If activation failed after point of no return, it returns error
	 * as 0 and pretends the call succeeded.
	 */
	error = exec_activate_image(imgp);
	/* thread and task ref returned for vfexec case */

	if (imgp->ip_new_thread != NULL) {
		/*
		 * task reference might be returned by exec_activate_image
		 * for vfexec.
		 */
		new_task = get_threadtask(imgp->ip_new_thread);
#if defined(HAS_APPLE_PAC)
		ml_task_set_disable_user_jop(new_task, imgp->ip_flags & IMGPF_NOJOP ? TRUE : FALSE);
		ml_thread_set_disable_user_jop(imgp->ip_new_thread, imgp->ip_flags & IMGPF_NOJOP ? TRUE : FALSE);
#endif
	}

	if (!error) {
		p = proc_exec_switch_task(current_proc(), p, old_task, new_task, imgp, &inherit);
		/* proc ref returned */
		should_release_proc_ref = TRUE;
	}

	kauth_cred_unref(&context.vc_ucred);

	if (!error) {
		exec_done = TRUE;
		assert(imgp->ip_new_thread != NULL);

		exec_resettextvp(p, imgp);
		/*
		 * Enable new task IPC access if exec_activate_image() returned an
		 * active task. (Checks active bit in ipc_task_enable() under lock).
		 * Must enable after resettextvp so that task port policies are not evaluated
		 * until the csblob in the textvp is accurately reflected.
		 */
		vm_map_setup(get_task_map(new_task), new_task);
		ipc_task_enable(new_task);
		error = process_signature(p, imgp);
	}

#if defined(HAS_APPLE_PAC)
	if (imgp->ip_new_thread && !error) {
		ml_task_set_jop_pid_from_shared_region(new_task, imgp->ip_flags & IMGPF_NOJOP);
		ml_thread_set_jop_pid(imgp->ip_new_thread, new_task);
	}
#endif /* defined(HAS_APPLE_PAC) */

	/* flag exec has occurred, notify only if it has not failed due to FP Key error */
	if (exec_done && ((p->p_lflag & P_LTERM_DECRYPTFAIL) == 0)) {
		proc_knote(p, NOTE_EXEC);
	}

	if (imgp->ip_vp != NULLVP) {
		vnode_put(imgp->ip_vp);
	}
	if (imgp->ip_scriptvp != NULLVP) {
		vnode_put(imgp->ip_scriptvp);
	}
	if (imgp->ip_free_map) {
		/* Free the map after dropping iocount on vnode to avoid deadlock */
		vm_map_deallocate(imgp->ip_free_map);
	}
	if (imgp->ip_strings) {
		execargs_free(imgp);
	}
#if CONFIG_MACF
	if (imgp->ip_execlabelp) {
		mac_cred_label_free(imgp->ip_execlabelp);
		imgp->ip_execlabelp = NULL;
	}
	if (imgp->ip_scriptlabelp) {
		mac_vnode_label_free(imgp->ip_scriptlabelp);
		imgp->ip_scriptlabelp = NULL;
	}
#endif
	if (imgp->ip_cs_error != OS_REASON_NULL) {
		os_reason_free(imgp->ip_cs_error);
		imgp->ip_cs_error = OS_REASON_NULL;
	}

	if (!error) {
		/*
		 * We need to initialize the bank context behind the protection of
		 * the proc_trans lock to prevent a race with exit. We can't do this during
		 * exec_activate_image because task_bank_init checks entitlements that
		 * aren't loaded until subsequent calls (including exec_resettextvp).
		 */
		error = proc_transstart(p, 0, 0);
	}

	if (!error) {
		task_bank_init(new_task);
		proc_transend(p, 0);

		// Don't inherit crash behavior across exec, but preserve crash behavior from bootargs
		p->p_crash_behavior = 0;
		p->p_crash_behavior_deadline = 0;
#if (DEVELOPMENT || DEBUG)
		set_crash_behavior_from_bootarg(p);
#endif

#if __arm64__
		proc_footprint_entitlement_hacks(p, new_task);
#endif /* __arm64__ */

#if XNU_TARGET_OS_OSX
		if (IOTaskHasEntitlement(new_task, SINGLE_JIT_ENTITLEMENT)) {
			vm_map_single_jit(get_task_map(new_task));
		}
#endif /* XNU_TARGET_OS_OSX */

		/* Sever any extant thread affinity */
		thread_affinity_exec(current_thread());

		/* Inherit task role from old task to new task for exec */
		proc_inherit_task_role(new_task, old_task);

		thread_t main_thread = imgp->ip_new_thread;

		task_set_main_thread_qos(new_task, main_thread);

#if __has_feature(ptrauth_calls)
		task_set_pac_exception_fatal_flag(new_task);
#endif /* __has_feature(ptrauth_calls) */
		task_set_jit_flags(new_task);

#if CONFIG_ARCADE
		/*
		 * Check to see if we need to trigger an arcade upcall AST now
		 * that the vnode has been reset on the task.
		 */
		arcade_prepare(new_task, imgp->ip_new_thread);
#endif /* CONFIG_ARCADE */

		proc_apply_jit_and_vm_policies(imgp, p, new_task);

		if (vm_darkwake_mode == TRUE) {
			/*
			 * This process is being launched when the system
			 * is in darkwake. So mark it specially. This will
			 * cause all its pages to be entered in the background Q.
			 */
			task_set_darkwake_mode(new_task, vm_darkwake_mode);
		}

#if CONFIG_DTRACE
		dtrace_thread_didexec(imgp->ip_new_thread);

		if ((dtrace_proc_waitfor_hook = dtrace_proc_waitfor_exec_ptr) != NULL) {
			(*dtrace_proc_waitfor_hook)(p);
		}
#endif

#if CONFIG_AUDIT
		if (!error && AUDIT_ENABLED() && p) {
			/* Add the CDHash of the new process to the audit record */
			uint8_t *cdhash = cs_get_cdhash(p);
			if (cdhash) {
				AUDIT_ARG(data, cdhash, sizeof(uint8_t), CS_CDHASH_LEN);
			}
		}
#endif
	} else {
		DTRACE_PROC1(exec__failure, int, error);
	}

exit_with_error:

	/* terminate the new task it if exec failed  */
	if (new_task != NULL && task_is_exec_copy(new_task)) {
		task_terminate_internal(new_task);
	}

	if (imgp != NULL) {
		/* Clear the initial wait on the thread transferring watchports */
		if (imgp->ip_new_thread) {
			task_clear_return_wait(get_threadtask(imgp->ip_new_thread), TCRW_CLEAR_INITIAL_WAIT);
		}

		/* Transfer the watchport boost to new task */
		if (!error) {
			task_transfer_turnstile_watchports(old_task,
			    new_task, imgp->ip_new_thread);
		}
		/*
		 * Do not terminate the current task, if proc_exec_switch_task did not
		 * switch the tasks, terminating the current task without the switch would
		 * result in loosing the SIGKILL status.
		 */
		if (task_did_exec(old_task)) {
			/* Terminate the current task, since exec will start in new task */
			task_terminate_internal(old_task);
		}

		/* Release the thread ref returned by cloneproc */
		if (imgp->ip_new_thread) {
			/* clear the exec complete flag if there is an error before point of no-return */
			uint32_t clearwait_flags = TCRW_CLEAR_FINAL_WAIT;
			if (!exec_done && error != 0) {
				clearwait_flags |= TCRW_CLEAR_EXEC_COMPLETE;
			}
			/* wake up the new exec thread */
			task_clear_return_wait(get_threadtask(imgp->ip_new_thread), clearwait_flags);
			thread_deallocate(imgp->ip_new_thread);
			imgp->ip_new_thread = NULL;
		}
	}

	/* Release the ref returned by fork_create_child */
	if (new_task) {
		task_deallocate(new_task);
		new_task = NULL;
	}

	if (should_release_proc_ref) {
		proc_rele(p);
	}

	uthread_set_exec_data(current_uthread(), NULL);
	kfree_type(typeof(*__execve_data), __execve_data);

	if (inherit != NULL) {
		ipc_importance_release(inherit);
	}

	return error;
}


/*
 * copyinptr
 *
 * Description:	Copy a pointer in from user space to a user_addr_t in kernel
 *		space, based on 32/64 bitness of the user space
 *
 * Parameters:	froma			User space address
 *		toptr			Address of kernel space user_addr_t
 *		ptr_size		4/8, based on 'froma' address space
 *
 * Returns:	0			Success
 *		EFAULT			Bad 'froma'
 *
 * Implicit returns:
 *		*ptr_size		Modified
 */
static int
copyinptr(user_addr_t froma, user_addr_t *toptr, int ptr_size)
{
	int error;

	if (ptr_size == 4) {
		/* 64 bit value containing 32 bit address */
		unsigned int i = 0;

		error = copyin(froma, &i, 4);
		*toptr = CAST_USER_ADDR_T(i);   /* SAFE */
	} else {
		error = copyin(froma, toptr, 8);
	}
	return error;
}


/*
 * copyoutptr
 *
 * Description:	Copy a pointer out from a user_addr_t in kernel space to
 *		user space, based on 32/64 bitness of the user space
 *
 * Parameters:	ua			User space address to copy to
 *		ptr			Address of kernel space user_addr_t
 *		ptr_size		4/8, based on 'ua' address space
 *
 * Returns:	0			Success
 *		EFAULT			Bad 'ua'
 *
 */
static int
copyoutptr(user_addr_t ua, user_addr_t ptr, int ptr_size)
{
	int error;

	if (ptr_size == 4) {
		/* 64 bit value containing 32 bit address */
		unsigned int i = CAST_DOWN_EXPLICIT(unsigned int, ua);   /* SAFE */

		error = copyout(&i, ptr, 4);
	} else {
		error = copyout(&ua, ptr, 8);
	}
	return error;
}


/*
 * exec_copyout_strings
 *
 * Copy out the strings segment to user space.  The strings segment is put
 * on a preinitialized stack frame.
 *
 * Parameters:	struct image_params *	the image parameter block
 *		int *			a pointer to the stack offset variable
 *
 * Returns:	0			Success
 *		!0			Faiure: errno
 *
 * Implicit returns:
 *		(*stackp)		The stack offset, modified
 *
 * Note:	The strings segment layout is backward, from the beginning
 *		of the top of the stack to consume the minimal amount of
 *		space possible; the returned stack pointer points to the
 *		end of the area consumed (stacks grow downward).
 *
 *		argc is an int; arg[i] are pointers; env[i] are pointers;
 *		the 0's are (void *)NULL's
 *
 * The stack frame layout is:
 *
 *      +-------------+ <- p->user_stack
 *      |     16b     |
 *      +-------------+
 *      | STRING AREA |
 *      |      :      |
 *      |      :      |
 *      |      :      |
 *      +- -- -- -- --+
 *      |  PATH AREA  |
 *      +-------------+
 *      |      0      |
 *      +-------------+
 *      |  applev[n]  |
 *      +-------------+
 *             :
 *             :
 *      +-------------+
 *      |  applev[1]  |
 *      +-------------+
 *      | exec_path / |
 *      |  applev[0]  |
 *      +-------------+
 *      |      0      |
 *      +-------------+
 *      |    env[n]   |
 *      +-------------+
 *             :
 *             :
 *      +-------------+
 *      |    env[0]   |
 *      +-------------+
 *      |      0      |
 *      +-------------+
 *      | arg[argc-1] |
 *      +-------------+
 *             :
 *             :
 *      +-------------+
 *      |    arg[0]   |
 *      +-------------+
 *      |     argc    |
 * sp-> +-------------+
 *
 * Although technically a part of the STRING AREA, we treat the PATH AREA as
 * a separate entity.  This allows us to align the beginning of the PATH AREA
 * to a pointer boundary so that the exec_path, env[i], and argv[i] pointers
 * which preceed it on the stack are properly aligned.
 */
__attribute__((noinline))
static int
exec_copyout_strings(struct image_params *imgp, user_addr_t *stackp)
{
	proc_t p = vfs_context_proc(imgp->ip_vfs_context);
	int     ptr_size = (imgp->ip_flags & IMGPF_IS_64BIT_ADDR) ? 8 : 4;
	int     ptr_area_size;
	void *ptr_buffer_start, *ptr_buffer;
	size_t string_size;

	user_addr_t     string_area;    /* *argv[], *env[] */
	user_addr_t     ptr_area;       /* argv[], env[], applev[] */
	user_addr_t argc_area;  /* argc */
	user_addr_t     stack;
	int error;

	unsigned i;
	struct copyout_desc {
		char    *start_string;
		int             count;
#if CONFIG_DTRACE
		user_addr_t     *dtrace_cookie;
#endif
		boolean_t       null_term;
	} descriptors[] = {
		{
			.start_string = imgp->ip_startargv,
			.count = imgp->ip_argc,
#if CONFIG_DTRACE
			.dtrace_cookie = &p->p_dtrace_argv,
#endif
			.null_term = TRUE
		},
		{
			.start_string = imgp->ip_endargv,
			.count = imgp->ip_envc,
#if CONFIG_DTRACE
			.dtrace_cookie = &p->p_dtrace_envp,
#endif
			.null_term = TRUE
		},
		{
			.start_string = imgp->ip_strings,
			.count = 1,
#if CONFIG_DTRACE
			.dtrace_cookie = NULL,
#endif
			.null_term = FALSE
		},
		{
			.start_string = imgp->ip_endenvv,
			.count = imgp->ip_applec - 1, /* exec_path handled above */
#if CONFIG_DTRACE
			.dtrace_cookie = NULL,
#endif
			.null_term = TRUE
		}
	};

	stack = *stackp;

	/*
	 * All previous contributors to the string area
	 * should have aligned their sub-area
	 */
	if (imgp->ip_strspace % ptr_size != 0) {
		error = EINVAL;
		goto bad;
	}

	/* Grow the stack down for the strings we've been building up */
	string_size = imgp->ip_strendp - imgp->ip_strings;
	stack -= string_size;
	string_area = stack;

	/*
	 * Need room for one pointer for each string, plus
	 * one for the NULLs terminating the argv, envv, and apple areas.
	 */
	ptr_area_size = (imgp->ip_argc + imgp->ip_envc + imgp->ip_applec + 3) * ptr_size;
	stack -= ptr_area_size;
	ptr_area = stack;

	/* We'll construct all the pointer arrays in our string buffer,
	 * which we already know is aligned properly, and ip_argspace
	 * was used to verify we have enough space.
	 */
	ptr_buffer_start = ptr_buffer = (void *)imgp->ip_strendp;

	/*
	 * Need room for pointer-aligned argc slot.
	 */
	stack -= ptr_size;
	argc_area = stack;

	/*
	 * Record the size of the arguments area so that sysctl_procargs()
	 * can return the argument area without having to parse the arguments.
	 */
	proc_lock(p);
	p->p_argc = imgp->ip_argc;
	p->p_argslen = (int)(*stackp - string_area);
	proc_unlock(p);

	/* Return the initial stack address: the location of argc */
	*stackp = stack;

	/*
	 * Copy out the entire strings area.
	 */
	error = copyout(imgp->ip_strings, string_area,
	    string_size);
	if (error) {
		goto bad;
	}

	for (i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++) {
		char *cur_string = descriptors[i].start_string;
		int j;

#if CONFIG_DTRACE
		if (descriptors[i].dtrace_cookie) {
			proc_lock(p);
			*descriptors[i].dtrace_cookie = ptr_area + ((uintptr_t)ptr_buffer - (uintptr_t)ptr_buffer_start); /* dtrace convenience */
			proc_unlock(p);
		}
#endif /* CONFIG_DTRACE */

		/*
		 * For each segment (argv, envv, applev), copy as many pointers as requested
		 * to our pointer buffer.
		 */
		for (j = 0; j < descriptors[i].count; j++) {
			user_addr_t cur_address = string_area + (cur_string - imgp->ip_strings);

			/* Copy out the pointer to the current string. Alignment has been verified  */
			if (ptr_size == 8) {
				*(uint64_t *)ptr_buffer = (uint64_t)cur_address;
			} else {
				*(uint32_t *)ptr_buffer = (uint32_t)cur_address;
			}

			ptr_buffer = (void *)((uintptr_t)ptr_buffer + ptr_size);
			cur_string += strlen(cur_string) + 1; /* Only a NUL between strings in the same area */
		}

		if (descriptors[i].null_term) {
			if (ptr_size == 8) {
				*(uint64_t *)ptr_buffer = 0ULL;
			} else {
				*(uint32_t *)ptr_buffer = 0;
			}

			ptr_buffer = (void *)((uintptr_t)ptr_buffer + ptr_size);
		}
	}

	/*
	 * Copy out all our pointer arrays in bulk.
	 */
	error = copyout(ptr_buffer_start, ptr_area,
	    ptr_area_size);
	if (error) {
		goto bad;
	}

	/* argc (int32, stored in a ptr_size area) */
	error = copyoutptr((user_addr_t)imgp->ip_argc, argc_area, ptr_size);
	if (error) {
		goto bad;
	}

bad:
	return error;
}


/*
 * exec_extract_strings
 *
 * Copy arguments and environment from user space into work area; we may
 * have already copied some early arguments into the work area, and if
 * so, any arguments opied in are appended to those already there.
 * This function is the primary manipulator of ip_argspace, since
 * these are the arguments the client of execve(2) knows about. After
 * each argv[]/envv[] string is copied, we charge the string length
 * and argv[]/envv[] pointer slot to ip_argspace, so that we can
 * full preflight the arg list size.
 *
 * Parameters:	struct image_params *	the image parameter block
 *
 * Returns:	0			Success
 *		!0			Failure: errno
 *
 * Implicit returns;
 *		(imgp->ip_argc)		Count of arguments, updated
 *		(imgp->ip_envc)		Count of environment strings, updated
 *		(imgp->ip_argspace)	Count of remaining of NCARGS
 *		(imgp->ip_interp_buffer)	Interpreter and args (mutated in place)
 *
 *
 * Note:	The argument and environment vectors are user space pointers
 *		to arrays of user space pointers.
 */
__attribute__((noinline))
static int
exec_extract_strings(struct image_params *imgp)
{
	int error = 0;
	int     ptr_size = (imgp->ip_flags & IMGPF_WAS_64BIT_ADDR) ? 8 : 4;
	int new_ptr_size = (imgp->ip_flags & IMGPF_IS_64BIT_ADDR) ? 8 : 4;
	user_addr_t     argv = imgp->ip_user_argv;
	user_addr_t     envv = imgp->ip_user_envv;

	/*
	 * Adjust space reserved for the path name by however much padding it
	 * needs. Doing this here since we didn't know if this would be a 32-
	 * or 64-bit process back in exec_save_path.
	 */
	while (imgp->ip_strspace % new_ptr_size != 0) {
		*imgp->ip_strendp++ = '\0';
		imgp->ip_strspace--;
		/* imgp->ip_argspace--; not counted towards exec args total */
	}

	/*
	 * From now on, we start attributing string space to ip_argspace
	 */
	imgp->ip_startargv = imgp->ip_strendp;
	imgp->ip_argc = 0;

	if ((imgp->ip_flags & IMGPF_INTERPRET) != 0) {
		user_addr_t     arg;
		char *argstart, *ch;

		/* First, the arguments in the "#!" string are tokenized and extracted. */
		argstart = imgp->ip_interp_buffer;
		while (argstart) {
			ch = argstart;
			while (*ch && !IS_WHITESPACE(*ch)) {
				ch++;
			}

			if (*ch == '\0') {
				/* last argument, no need to NUL-terminate */
				error = exec_add_user_string(imgp, CAST_USER_ADDR_T(argstart), UIO_SYSSPACE, TRUE);
				argstart = NULL;
			} else {
				/* NUL-terminate */
				*ch = '\0';
				error = exec_add_user_string(imgp, CAST_USER_ADDR_T(argstart), UIO_SYSSPACE, TRUE);

				/*
				 * Find the next string. We know spaces at the end of the string have already
				 * been stripped.
				 */
				argstart = ch + 1;
				while (IS_WHITESPACE(*argstart)) {
					argstart++;
				}
			}

			/* Error-check, regardless of whether this is the last interpreter arg or not */
			if (error) {
				goto bad;
			}
			if (imgp->ip_argspace < new_ptr_size) {
				error = E2BIG;
				goto bad;
			}
			imgp->ip_argspace -= new_ptr_size; /* to hold argv[] entry */
			imgp->ip_argc++;
		}

		if (argv != 0LL) {
			/*
			 * If we are running an interpreter, replace the av[0] that was
			 * passed to execve() with the path name that was
			 * passed to execve() for interpreters which do not use the PATH
			 * to locate their script arguments.
			 */
			error = copyinptr(argv, &arg, ptr_size);
			if (error) {
				goto bad;
			}
			if (arg != 0LL) {
				argv += ptr_size; /* consume without using */
			}
		}

		if (imgp->ip_interp_sugid_fd != -1) {
			char temp[19]; /* "/dev/fd/" + 10 digits + NUL */
			snprintf(temp, sizeof(temp), "/dev/fd/%d", imgp->ip_interp_sugid_fd);
			error = exec_add_user_string(imgp, CAST_USER_ADDR_T(temp), UIO_SYSSPACE, TRUE);
		} else {
			error = exec_add_user_string(imgp, imgp->ip_user_fname, imgp->ip_seg, TRUE);
		}

		if (error) {
			goto bad;
		}
		if (imgp->ip_argspace < new_ptr_size) {
			error = E2BIG;
			goto bad;
		}
		imgp->ip_argspace -= new_ptr_size; /* to hold argv[] entry */
		imgp->ip_argc++;
	}

	while (argv != 0LL) {
		user_addr_t     arg;

		error = copyinptr(argv, &arg, ptr_size);
		if (error) {
			goto bad;
		}

		if (arg == 0LL) {
			break;
		}

		argv += ptr_size;

		/*
		 * av[n...] = arg[n]
		 */
		error = exec_add_user_string(imgp, arg, imgp->ip_seg, TRUE);
		if (error) {
			goto bad;
		}
		if (imgp->ip_argspace < new_ptr_size) {
			error = E2BIG;
			goto bad;
		}
		imgp->ip_argspace -= new_ptr_size; /* to hold argv[] entry */
		imgp->ip_argc++;
	}

	/* Save space for argv[] NULL terminator */
	if (imgp->ip_argspace < new_ptr_size) {
		error = E2BIG;
		goto bad;
	}
	imgp->ip_argspace -= new_ptr_size;

	/* Note where the args ends and env begins. */
	imgp->ip_endargv = imgp->ip_strendp;
	imgp->ip_envc = 0;

	/* Now, get the environment */
	while (envv != 0LL) {
		user_addr_t     env;

		error = copyinptr(envv, &env, ptr_size);
		if (error) {
			goto bad;
		}

		envv += ptr_size;
		if (env == 0LL) {
			break;
		}
		/*
		 * av[n...] = env[n]
		 */
		error = exec_add_user_string(imgp, env, imgp->ip_seg, TRUE);
		if (error) {
			goto bad;
		}
		if (imgp->ip_argspace < new_ptr_size) {
			error = E2BIG;
			goto bad;
		}
		imgp->ip_argspace -= new_ptr_size; /* to hold envv[] entry */
		imgp->ip_envc++;
	}

	/* Save space for envv[] NULL terminator */
	if (imgp->ip_argspace < new_ptr_size) {
		error = E2BIG;
		goto bad;
	}
	imgp->ip_argspace -= new_ptr_size;

	/* Align the tail of the combined argv+envv area */
	while (imgp->ip_strspace % new_ptr_size != 0) {
		if (imgp->ip_argspace < 1) {
			error = E2BIG;
			goto bad;
		}
		*imgp->ip_strendp++ = '\0';
		imgp->ip_strspace--;
		imgp->ip_argspace--;
	}

	/* Note where the envv ends and applev begins. */
	imgp->ip_endenvv = imgp->ip_strendp;

	/*
	 * From now on, we are no longer charging argument
	 * space to ip_argspace.
	 */

bad:
	return error;
}

/*
 * Libc has an 8-element array set up for stack guard values.  It only fills
 * in one of those entries, and both gcc and llvm seem to use only a single
 * 8-byte guard.  Until somebody needs more than an 8-byte guard value, don't
 * do the work to construct them.
 */
#define GUARD_VALUES 1
#define GUARD_KEY "stack_guard="

/*
 * System malloc needs some entropy when it is initialized.
 */
#define ENTROPY_VALUES 2
#define ENTROPY_KEY "malloc_entropy="

/*
 * libplatform needs a random pointer-obfuscation value when it is initialized.
 */
#define PTR_MUNGE_VALUES 1
#define PTR_MUNGE_KEY "ptr_munge="

/*
 * System malloc engages nanozone for UIAPP.
 */
#define NANO_ENGAGE_KEY "MallocNanoZone=1"

/*
 * Used to pass experiment flags up to libmalloc.
 */
#define LIBMALLOC_EXPERIMENT_FACTORS_KEY "MallocExperiment="

/*
 * Passes information about hardened runtime entitlements to libsystem/libmalloc
 */
#define HARDENED_RUNTIME_KEY "HardenedRuntime="

#define PFZ_KEY "pfz="
extern user32_addr_t commpage_text32_location;
extern user64_addr_t commpage_text64_location;

extern uuid_string_t bootsessionuuid_string;
static TUNABLE(uint32_t, exe_boothash_salt, "exe_boothash_salt", 0);

__startup_func
static void
exe_boothash_salt_generate(void)
{
	if (!PE_parse_boot_argn("exe_boothash_salt", NULL, 0)) {
		read_random(&exe_boothash_salt, sizeof(exe_boothash_salt));
	}
}
STARTUP(EARLY_BOOT, STARTUP_RANK_MIDDLE, exe_boothash_salt_generate);


#define MAIN_STACK_VALUES 4
#define MAIN_STACK_KEY "main_stack="

#define FSID_KEY "executable_file="
#define DYLD_FSID_KEY "dyld_file="
#define CDHASH_KEY "executable_cdhash="
#define DYLD_FLAGS_KEY "dyld_flags="
#define SUBSYSTEM_ROOT_PATH_KEY "subsystem_root_path="
#define APP_BOOT_SESSION_KEY "executable_boothash="
#if __has_feature(ptrauth_calls)
#define PTRAUTH_DISABLED_FLAG "ptrauth_disabled=1"
#define DYLD_ARM64E_ABI_KEY "arm64e_abi="
#endif /* __has_feature(ptrauth_calls) */
#define MAIN_TH_PORT_KEY "th_port="

#define FSID_MAX_STRING "0x1234567890abcdef,0x1234567890abcdef"

#define HEX_STR_LEN 18 // 64-bit hex value "0x0123456701234567"
#define HEX_STR_LEN32 10 // 32-bit hex value "0x01234567"

#if XNU_TARGET_OS_OSX && _POSIX_SPAWN_FORCE_4K_PAGES && PMAP_CREATE_FORCE_4K_PAGES
#define VM_FORCE_4K_PAGES_KEY "vm_force_4k_pages=1"
#endif /* XNU_TARGET_OS_OSX && _POSIX_SPAWN_FORCE_4K_PAGES && PMAP_CREATE_FORCE_4K_PAGES */

static int
exec_add_entropy_key(struct image_params *imgp,
    const char *key,
    int values,
    boolean_t embedNUL)
{
	const int limit = 8;
	uint64_t entropy[limit];
	char str[strlen(key) + (HEX_STR_LEN + 1) * limit + 1];
	if (values > limit) {
		values = limit;
	}

	read_random(entropy, sizeof(entropy[0]) * values);

	if (embedNUL) {
		entropy[0] &= ~(0xffull << 8);
	}

	int len = scnprintf(str, sizeof(str), "%s0x%llx", key, entropy[0]);
	size_t remaining = sizeof(str) - len;
	for (int i = 1; i < values && remaining > 0; ++i) {
		size_t start = sizeof(str) - remaining;
		len = scnprintf(&str[start], remaining, ",0x%llx", entropy[i]);
		remaining -= len;
	}

	return exec_add_user_string(imgp, CAST_USER_ADDR_T(str), UIO_SYSSPACE, FALSE);
}

/*
 * Build up the contents of the apple[] string vector
 */
#if (DEVELOPMENT || DEBUG)
extern uint64_t dyld_flags;
#endif

#if __has_feature(ptrauth_calls)
static inline bool
is_arm64e_running_as_arm64(const struct image_params *imgp)
{
	return (imgp->ip_origcpusubtype & ~CPU_SUBTYPE_MASK) == CPU_SUBTYPE_ARM64E &&
	       (imgp->ip_flags & IMGPF_NOJOP);
}
#endif /* __has_feature(ptrauth_calls) */

_Atomic uint64_t libmalloc_experiment_factors = 0;

static int
exec_add_apple_strings(struct image_params *imgp,
    const load_result_t *load_result, task_t task)
{
	int error;
	int img_ptr_size = (imgp->ip_flags & IMGPF_IS_64BIT_ADDR) ? 8 : 4;
	thread_t new_thread;
	ipc_port_t sright;
	uint64_t local_experiment_factors = 0;

	/* exec_save_path stored the first string */
	imgp->ip_applec = 1;

	/* adding the pfz string */
	{
		char pfz_string[strlen(PFZ_KEY) + HEX_STR_LEN + 1];

		if (img_ptr_size == 8) {
			__assert_only size_t ret = snprintf(pfz_string, sizeof(pfz_string), PFZ_KEY "0x%llx", commpage_text64_location);
			assert(ret < sizeof(pfz_string));
		} else {
			snprintf(pfz_string, sizeof(pfz_string), PFZ_KEY "0x%x", commpage_text32_location);
		}
		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(pfz_string), UIO_SYSSPACE, FALSE);
		if (error) {
			printf("Failed to add the pfz string with error %d\n", error);
			goto bad;
		}
		imgp->ip_applec++;
	}

	/* adding the NANO_ENGAGE_KEY key */
	if (imgp->ip_px_sa) {
		struct _posix_spawnattr* psa = (struct _posix_spawnattr *) imgp->ip_px_sa;
		int proc_flags = psa->psa_flags;

		if ((proc_flags & _POSIX_SPAWN_NANO_ALLOCATOR) == _POSIX_SPAWN_NANO_ALLOCATOR) {
			const char *nano_string = NANO_ENGAGE_KEY;
			error = exec_add_user_string(imgp, CAST_USER_ADDR_T(nano_string), UIO_SYSSPACE, FALSE);
			if (error) {
				goto bad;
			}
			imgp->ip_applec++;
		}
	}

	/*
	 * Supply libc with a collection of random values to use when
	 * implementing -fstack-protector.
	 *
	 * (The first random string always contains an embedded NUL so that
	 * __stack_chk_guard also protects against C string vulnerabilities)
	 */
	error = exec_add_entropy_key(imgp, GUARD_KEY, GUARD_VALUES, TRUE);
	if (error) {
		goto bad;
	}
	imgp->ip_applec++;

	/*
	 * Supply libc with entropy for system malloc.
	 */
	error = exec_add_entropy_key(imgp, ENTROPY_KEY, ENTROPY_VALUES, FALSE);
	if (error) {
		goto bad;
	}
	imgp->ip_applec++;

	/*
	 * Supply libpthread & libplatform with a random value to use for pointer
	 * obfuscation.
	 */
	error = exec_add_entropy_key(imgp, PTR_MUNGE_KEY, PTR_MUNGE_VALUES, FALSE);
	if (error) {
		goto bad;
	}
	imgp->ip_applec++;

	/*
	 * Add MAIN_STACK_KEY: Supplies the address and size of the main thread's
	 * stack if it was allocated by the kernel.
	 *
	 * The guard page is not included in this stack size as libpthread
	 * expects to add it back in after receiving this value.
	 */
	if (load_result->unixproc) {
		char stack_string[strlen(MAIN_STACK_KEY) + (HEX_STR_LEN + 1) * MAIN_STACK_VALUES + 1];
		snprintf(stack_string, sizeof(stack_string),
		    MAIN_STACK_KEY "0x%llx,0x%llx,0x%llx,0x%llx",
		    (uint64_t)load_result->user_stack,
		    (uint64_t)load_result->user_stack_size,
		    (uint64_t)load_result->user_stack_alloc,
		    (uint64_t)load_result->user_stack_alloc_size);
		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(stack_string), UIO_SYSSPACE, FALSE);
		if (error) {
			goto bad;
		}
		imgp->ip_applec++;
	}

	if (imgp->ip_vattr) {
		uint64_t fsid    = vnode_get_va_fsid(imgp->ip_vattr);
		uint64_t fsobjid = imgp->ip_vattr->va_fileid;

		char fsid_string[strlen(FSID_KEY) + strlen(FSID_MAX_STRING) + 1];
		snprintf(fsid_string, sizeof(fsid_string),
		    FSID_KEY "0x%llx,0x%llx", fsid, fsobjid);
		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(fsid_string), UIO_SYSSPACE, FALSE);
		if (error) {
			goto bad;
		}
		imgp->ip_applec++;
	}

	if (imgp->ip_dyld_fsid || imgp->ip_dyld_fsobjid) {
		char fsid_string[strlen(DYLD_FSID_KEY) + strlen(FSID_MAX_STRING) + 1];
		snprintf(fsid_string, sizeof(fsid_string),
		    DYLD_FSID_KEY "0x%llx,0x%llx", imgp->ip_dyld_fsid, imgp->ip_dyld_fsobjid);
		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(fsid_string), UIO_SYSSPACE, FALSE);
		if (error) {
			goto bad;
		}
		imgp->ip_applec++;
	}

	uint8_t cdhash[SHA1_RESULTLEN];
	int cdhash_errror = ubc_cs_getcdhash(imgp->ip_vp, imgp->ip_arch_offset, cdhash, NULL);
	if (cdhash_errror == 0) {
		char hash_string[strlen(CDHASH_KEY) + 2 * SHA1_RESULTLEN + 1];
		strncpy(hash_string, CDHASH_KEY, sizeof(hash_string));
		char *p = hash_string + sizeof(CDHASH_KEY) - 1;
		for (int i = 0; i < SHA1_RESULTLEN; i++) {
			snprintf(p, 3, "%02x", (int) cdhash[i]);
			p += 2;
		}
		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(hash_string), UIO_SYSSPACE, FALSE);
		if (error) {
			goto bad;
		}
		imgp->ip_applec++;

		/* hash together cd-hash and boot-session-uuid */
		uint8_t sha_digest[SHA256_DIGEST_LENGTH];
		SHA256_CTX sha_ctx;
		SHA256_Init(&sha_ctx);
		SHA256_Update(&sha_ctx, &exe_boothash_salt, sizeof(exe_boothash_salt));
		SHA256_Update(&sha_ctx, bootsessionuuid_string, sizeof(bootsessionuuid_string));
		SHA256_Update(&sha_ctx, cdhash, sizeof(cdhash));
		SHA256_Final(sha_digest, &sha_ctx);
		char app_boot_string[strlen(APP_BOOT_SESSION_KEY) + 2 * SHA1_RESULTLEN + 1];
		strncpy(app_boot_string, APP_BOOT_SESSION_KEY, sizeof(app_boot_string));
		char *s = app_boot_string + sizeof(APP_BOOT_SESSION_KEY) - 1;
		for (int i = 0; i < SHA1_RESULTLEN; i++) {
			snprintf(s, 3, "%02x", (int) sha_digest[i]);
			s += 2;
		}
		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(app_boot_string), UIO_SYSSPACE, FALSE);
		if (error) {
			goto bad;
		}
		imgp->ip_applec++;
	}
#if (DEVELOPMENT || DEBUG)
	if (dyld_flags) {
		char dyld_flags_string[strlen(DYLD_FLAGS_KEY) + HEX_STR_LEN + 1];
		snprintf(dyld_flags_string, sizeof(dyld_flags_string), DYLD_FLAGS_KEY "0x%llx", dyld_flags);
		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(dyld_flags_string), UIO_SYSSPACE, FALSE);
		if (error) {
			goto bad;
		}
		imgp->ip_applec++;
	}
#endif
	if (imgp->ip_subsystem_root_path) {
		size_t buffer_len = MAXPATHLEN + strlen(SUBSYSTEM_ROOT_PATH_KEY);
		char subsystem_root_path_string[buffer_len];
		int required_len = snprintf(subsystem_root_path_string, buffer_len, SUBSYSTEM_ROOT_PATH_KEY "%s", imgp->ip_subsystem_root_path);

		if (((size_t)required_len >= buffer_len) || (required_len < 0)) {
			error = ENAMETOOLONG;
			goto bad;
		}

		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(subsystem_root_path_string), UIO_SYSSPACE, FALSE);
		if (error) {
			goto bad;
		}

		imgp->ip_applec++;
	}
#if __has_feature(ptrauth_calls)
	if (is_arm64e_running_as_arm64(imgp)) {
		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(PTRAUTH_DISABLED_FLAG), UIO_SYSSPACE, FALSE);
		if (error) {
			goto bad;
		}

		imgp->ip_applec++;
	}
#endif /* __has_feature(ptrauth_calls) */


#if __has_feature(ptrauth_calls) && defined(XNU_TARGET_OS_OSX)
	{
		char dyld_abi_string[strlen(DYLD_ARM64E_ABI_KEY) + 8];
		strlcpy(dyld_abi_string, DYLD_ARM64E_ABI_KEY, sizeof(dyld_abi_string));
		bool allowAll = bootarg_arm64e_preview_abi;
		strlcat(dyld_abi_string, (allowAll ? "all" : "os"), sizeof(dyld_abi_string));
		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(dyld_abi_string), UIO_SYSSPACE, FALSE);
		if (error) {
			goto bad;
		}

		imgp->ip_applec++;
	}
#endif
	/*
	 * Add main thread mach port name
	 * +1 uref on main thread port, this ref will be extracted by libpthread in __pthread_init
	 * and consumed in _bsdthread_terminate. Leaking the main thread port name if not linked
	 * against libpthread.
	 */
	if ((new_thread = imgp->ip_new_thread) != THREAD_NULL) {
		thread_reference(new_thread);
		sright = convert_thread_to_port_pinned(new_thread);
		task_t new_task = get_threadtask(new_thread);
		mach_port_name_t name = ipc_port_copyout_send(sright, get_task_ipcspace(new_task));
		char port_name_hex_str[strlen(MAIN_TH_PORT_KEY) + HEX_STR_LEN32 + 1];
		snprintf(port_name_hex_str, sizeof(port_name_hex_str), MAIN_TH_PORT_KEY "0x%x", name);

		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(port_name_hex_str), UIO_SYSSPACE, FALSE);
		if (error) {
			goto bad;
		}
		imgp->ip_applec++;
	}

#if XNU_TARGET_OS_OSX && _POSIX_SPAWN_FORCE_4K_PAGES && PMAP_CREATE_FORCE_4K_PAGES
	if (imgp->ip_px_sa != NULL) {
		struct _posix_spawnattr* psa = (struct _posix_spawnattr *) imgp->ip_px_sa;
		if (psa->psa_flags & _POSIX_SPAWN_FORCE_4K_PAGES) {
			const char *vm_force_4k_string = VM_FORCE_4K_PAGES_KEY;
			error = exec_add_user_string(imgp, CAST_USER_ADDR_T(vm_force_4k_string), UIO_SYSSPACE, FALSE);
			if (error) {
				goto bad;
			}
			imgp->ip_applec++;
		}
	}
#endif /* XNU_TARGET_OS_OSX && _POSIX_SPAWN_FORCE_4K_PAGES && PMAP_CREATE_FORCE_4K_PAGES */

	/* adding the libmalloc experiment string */
	local_experiment_factors = os_atomic_load_wide(&libmalloc_experiment_factors, relaxed);
	if (__improbable(local_experiment_factors != 0)) {
		char libmalloc_experiment_factors_string[strlen(LIBMALLOC_EXPERIMENT_FACTORS_KEY) + HEX_STR_LEN + 1];

		snprintf(
			libmalloc_experiment_factors_string,
			sizeof(libmalloc_experiment_factors_string),
			LIBMALLOC_EXPERIMENT_FACTORS_KEY "0x%llx",
			local_experiment_factors);
		error = exec_add_user_string(
			imgp,
			CAST_USER_ADDR_T(libmalloc_experiment_factors_string),
			UIO_SYSSPACE,
			FALSE);
		if (error) {
			printf("Failed to add the libmalloc experiment factors string with error %d\n", error);
			goto bad;
		}
		imgp->ip_applec++;
	}

	/*
	 * Push down the task security configuration. To reduce confusion when userland parses the information
	 * still push an empty security configuration if nothing is active.
	 */
	{
		#define SECURITY_CONFIG_KEY "security_config="
		char security_config_str[strlen(SECURITY_CONFIG_KEY) + HEX_STR_LEN + 1];

		snprintf(security_config_str, sizeof(security_config_str),
		    SECURITY_CONFIG_KEY "0x%x", task_get_security_config(task));

		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(security_config_str), UIO_SYSSPACE, FALSE);
		if (error) {
			printf("Failed to add the security config string with error %d\n", error);
			goto bad;
		}
		imgp->ip_applec++;
	}




	if (load_result->hardened_runtime_binary) {
		const size_t HR_STRING_SIZE = sizeof(HARDENED_RUNTIME_KEY) + HR_FLAGS_NUM_NIBBLES + 2 + 1;
		char hardened_runtime[HR_STRING_SIZE];
		snprintf(hardened_runtime, HR_STRING_SIZE, HARDENED_RUNTIME_KEY"0x%x", load_result->hardened_runtime_binary);
		error = exec_add_user_string(imgp, CAST_USER_ADDR_T(hardened_runtime), UIO_SYSSPACE, FALSE);
		if (error) {
			printf("Failed to add hardened runtime flag with error %d\n", error);
			goto bad;
		}
		imgp->ip_applec++;
	}
	/* Align the tail of the combined applev area */
	while (imgp->ip_strspace % img_ptr_size != 0) {
		*imgp->ip_strendp++ = '\0';
		imgp->ip_strspace--;
	}

bad:
	return error;
}

/*
 * exec_check_permissions
 *
 * Description:	Verify that the file that is being attempted to be executed
 *		is in fact allowed to be executed based on it POSIX file
 *		permissions and other access control criteria
 *
 * Parameters:	struct image_params *	the image parameter block
 *
 * Returns:	0			Success
 *		EACCES			Permission denied
 *		ENOEXEC			Executable file format error
 *		ETXTBSY			Text file busy [misuse of error code]
 *	vnode_getattr:???
 *	vnode_authorize:???
 */
static int
exec_check_permissions(struct image_params *imgp)
{
	struct vnode *vp = imgp->ip_vp;
	struct vnode_attr *vap = imgp->ip_vattr;
	proc_t p = vfs_context_proc(imgp->ip_vfs_context);
	int error;
	kauth_action_t action;

	/* Only allow execution of regular files */
	if (!vnode_isreg(vp)) {
		return EACCES;
	}

	/* Get the file attributes that we will be using here and elsewhere */
	VATTR_INIT(vap);
	VATTR_WANTED(vap, va_uid);
	VATTR_WANTED(vap, va_gid);
	VATTR_WANTED(vap, va_mode);
	VATTR_WANTED(vap, va_fsid);
	VATTR_WANTED(vap, va_fsid64);
	VATTR_WANTED(vap, va_fileid);
	VATTR_WANTED(vap, va_data_size);
	if ((error = vnode_getattr(vp, vap, imgp->ip_vfs_context)) != 0) {
		return error;
	}

	/*
	 * Ensure that at least one execute bit is on - otherwise root
	 * will always succeed, and we don't want to happen unless the
	 * file really is executable.
	 */
	if (!vfs_authopaque(vnode_mount(vp)) && ((vap->va_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) == 0)) {
		return EACCES;
	}

	/* Disallow zero length files */
	if (vap->va_data_size == 0) {
		return ENOEXEC;
	}

	imgp->ip_arch_offset = (user_size_t)0;
#if __LP64__
	imgp->ip_arch_size = vap->va_data_size;
#else
	if (vap->va_data_size > UINT32_MAX) {
		return ENOEXEC;
	}
	imgp->ip_arch_size = (user_size_t)vap->va_data_size;
#endif

	/* Disable setuid-ness for traced programs or if MNT_NOSUID */
	if ((vp->v_mount->mnt_flag & MNT_NOSUID) || (p->p_lflag & P_LTRACED)) {
		vap->va_mode &= ~(VSUID | VSGID);
	}

	/*
	 * Disable _POSIX_SPAWN_ALLOW_DATA_EXEC and _POSIX_SPAWN_DISABLE_ASLR
	 * flags for setuid/setgid binaries.
	 */
	if (vap->va_mode & (VSUID | VSGID)) {
		imgp->ip_flags &= ~(IMGPF_ALLOW_DATA_EXEC | IMGPF_DISABLE_ASLR);
	}

#if CONFIG_MACF
	error = mac_vnode_check_exec(imgp->ip_vfs_context, vp, imgp);
	if (error) {
		return error;
	}
#endif

	/* Check for execute permission */
	action = KAUTH_VNODE_EXECUTE;
	/* Traced images must also be readable */
	if (p->p_lflag & P_LTRACED) {
		action |= KAUTH_VNODE_READ_DATA;
	}
	if ((error = vnode_authorize(vp, NULL, action, imgp->ip_vfs_context)) != 0) {
		return error;
	}

#if 0
	/* Don't let it run if anyone had it open for writing */
	vnode_lock(vp);
	if (vp->v_writecount) {
		panic("going to return ETXTBSY %x", vp);
		vnode_unlock(vp);
		return ETXTBSY;
	}
	vnode_unlock(vp);
#endif

	/* XXX May want to indicate to underlying FS that vnode is open */

	return error;
}


/*
 * exec_handle_sugid
 *
 * Initially clear the P_SUGID in the process flags; if an SUGID process is
 * exec'ing a non-SUGID image, then  this is the point of no return.
 *
 * If the image being activated is SUGID, then replace the credential with a
 * copy, disable tracing (unless the tracing process is root), reset the
 * mach task port to revoke it, set the P_SUGID bit,
 *
 * If the saved user and group ID will be changing, then make sure it happens
 * to a new credential, rather than a shared one.
 *
 * Set the security token (this is probably obsolete, given that the token
 * should not technically be separate from the credential itself).
 *
 * Parameters:	struct image_params *	the image parameter block
 *
 * Returns:	void			No failure indication
 *
 * Implicit returns:
 *		<process credential>	Potentially modified/replaced
 *		<task port>		Potentially revoked
 *		<process flags>		P_SUGID bit potentially modified
 *		<security token>	Potentially modified
 */
__attribute__((noinline))
static int
exec_handle_sugid(struct image_params *imgp)
{
	proc_t                  p = vfs_context_proc(imgp->ip_vfs_context);
	kauth_cred_t            cred = vfs_context_ucred(imgp->ip_vfs_context);
	int                     i;
	int                     leave_sugid_clear = 0;
	int                     mac_reset_ipc = 0;
	int                     error = 0;
#if CONFIG_MACF
	int                     mac_transition, disjoint_cred = 0;
	int             label_update_return = 0;

	/*
	 * Determine whether a call to update the MAC label will result in the
	 * credential changing.
	 *
	 * Note:	MAC policies which do not actually end up modifying
	 *		the label subsequently are strongly encouraged to
	 *		return 0 for this check, since a non-zero answer will
	 *		slow down the exec fast path for normal binaries.
	 */
	mac_transition = mac_cred_check_label_update_execve(
		imgp->ip_vfs_context,
		imgp->ip_vp,
		imgp->ip_arch_offset,
		imgp->ip_scriptvp,
		imgp->ip_scriptlabelp,
		imgp->ip_execlabelp,
		p,
		&imgp->ip_px_smpx);
#endif

	OSBitAndAtomic(~((uint32_t)P_SUGID), &p->p_flag);

	/*
	 * Order of the following is important; group checks must go last,
	 * as we use the success of the 'ismember' check combined with the
	 * failure of the explicit match to indicate that we will be setting
	 * the egid of the process even though the new process did not
	 * require VSUID/VSGID bits in order for it to set the new group as
	 * its egid.
	 *
	 * Note:	Technically, by this we are implying a call to
	 *		setegid() in the new process, rather than implying
	 *		it used its VSGID bit to set the effective group,
	 *		even though there is no code in that process to make
	 *		such a call.
	 */
	if (((imgp->ip_origvattr->va_mode & VSUID) != 0 &&
	    kauth_cred_getuid(cred) != imgp->ip_origvattr->va_uid) ||
	    ((imgp->ip_origvattr->va_mode & VSGID) != 0 &&
	    ((kauth_cred_ismember_gid(cred, imgp->ip_origvattr->va_gid, &leave_sugid_clear) || !leave_sugid_clear) ||
	    (kauth_cred_getgid(cred) != imgp->ip_origvattr->va_gid)))) {
#if CONFIG_MACF
/* label for MAC transition and neither VSUID nor VSGID */
handle_mac_transition:
#endif

#if CONFIG_SETUID
		/*
		 * Replace the credential with a copy of itself if euid or
		 * egid change.
		 *
		 * Note:	setuid binaries will automatically opt out of
		 *		group resolver participation as a side effect
		 *		of this operation.  This is an intentional
		 *		part of the security model, which requires a
		 *		participating credential be established by
		 *		escalating privilege, setting up all other
		 *		aspects of the credential including whether
		 *		or not to participate in external group
		 *		membership resolution, then dropping their
		 *		effective privilege to that of the desired
		 *		final credential state.
		 *
		 * Modifications to p_ucred must be guarded using the
		 * proc's ucred lock. This prevents others from accessing
		 * a garbage credential.
		 */

		if (imgp->ip_origvattr->va_mode & VSUID) {
			kauth_cred_proc_update(p, PROC_SETTOKEN_NONE,
			    ^bool (kauth_cred_t parent __unused, kauth_cred_t model) {
				return kauth_cred_model_setresuid(model,
				KAUTH_UID_NONE,
				imgp->ip_origvattr->va_uid,
				imgp->ip_origvattr->va_uid,
				KAUTH_UID_NONE);
			});
		}

		if (imgp->ip_origvattr->va_mode & VSGID) {
			kauth_cred_proc_update(p, PROC_SETTOKEN_NONE,
			    ^bool (kauth_cred_t parent __unused, kauth_cred_t model) {
				return kauth_cred_model_setresgid(model,
				KAUTH_GID_NONE,
				imgp->ip_origvattr->va_gid,
				imgp->ip_origvattr->va_gid);
			});
		}
#endif /* CONFIG_SETUID */

#if CONFIG_MACF
		/*
		 * If a policy has indicated that it will transition the label,
		 * before making the call into the MAC policies, get a new
		 * duplicate credential, so they can modify it without
		 * modifying any others sharing it.
		 */
		if (mac_transition) {
			/*
			 * This hook may generate upcalls that require
			 * importance donation from the kernel.
			 * (23925818)
			 */
			thread_t thread = current_thread();
			thread_enable_send_importance(thread, TRUE);
			kauth_proc_label_update_execve(p,
			    imgp->ip_vfs_context,
			    imgp->ip_vp,
			    imgp->ip_arch_offset,
			    imgp->ip_scriptvp,
			    imgp->ip_scriptlabelp,
			    imgp->ip_execlabelp,
			    &imgp->ip_csflags,
			    &imgp->ip_px_smpx,
			    &disjoint_cred,                     /* will be non zero if disjoint */
			    &label_update_return);
			thread_enable_send_importance(thread, FALSE);

			if (disjoint_cred) {
				/*
				 * If updating the MAC label resulted in a
				 * disjoint credential, flag that we need to
				 * set the P_SUGID bit.  This protects
				 * against debuggers being attached by an
				 * insufficiently privileged process onto the
				 * result of a transition to a more privileged
				 * credential.
				 */
				leave_sugid_clear = 0;
			}

			imgp->ip_mac_return = label_update_return;
		}

		mac_reset_ipc = mac_proc_check_inherit_ipc_ports(p, p->p_textvp, p->p_textoff, imgp->ip_vp, imgp->ip_arch_offset, imgp->ip_scriptvp);

#endif  /* CONFIG_MACF */

		/*
		 * If 'leave_sugid_clear' is non-zero, then we passed the
		 * VSUID and MACF checks, and successfully determined that
		 * the previous cred was a member of the VSGID group, but
		 * that it was not the default at the time of the execve,
		 * and that the post-labelling credential was not disjoint.
		 * So we don't set the P_SUGID or reset mach ports and fds
		 * on the basis of simply running this code.
		 */
		if (mac_reset_ipc || !leave_sugid_clear) {
			/*
			 * Have mach reset the task and thread ports.
			 * We don't want anyone who had the ports before
			 * a setuid exec to be able to access/control the
			 * task/thread after.
			 */
			ipc_task_reset((imgp->ip_new_thread != NULL) ?
			    get_threadtask(imgp->ip_new_thread) : proc_task(p));
			ipc_thread_reset((imgp->ip_new_thread != NULL) ?
			    imgp->ip_new_thread : current_thread());
		}

		if (!leave_sugid_clear) {
			/*
			 * Flag the process as setuid.
			 */
			OSBitOrAtomic(P_SUGID, &p->p_flag);

			/*
			 * Radar 2261856; setuid security hole fix
			 * XXX For setuid processes, attempt to ensure that
			 * stdin, stdout, and stderr are already allocated.
			 * We do not want userland to accidentally allocate
			 * descriptors in this range which has implied meaning
			 * to libc.
			 */
			for (i = 0; i < 3; i++) {
				if (fp_get_noref_locked(p, i) != NULL) {
					continue;
				}

				/*
				 * Do the kernel equivalent of
				 *
				 *      if i == 0
				 *              (void) open("/dev/null", O_RDONLY);
				 *      else
				 *              (void) open("/dev/null", O_WRONLY);
				 */

				struct fileproc *fp;
				int indx;
				int flag;
				struct nameidata *ndp = NULL;

				if (i == 0) {
					flag = FREAD;
				} else {
					flag = FWRITE;
				}

				if ((error = falloc_exec(p, imgp->ip_vfs_context,
				    &fp, &indx)) != 0) {
					continue;
				}

				ndp = kalloc_type(struct nameidata,
				    Z_WAITOK | Z_ZERO | Z_NOFAIL);

				NDINIT(ndp, LOOKUP, OP_OPEN, FOLLOW, UIO_SYSSPACE,
				    CAST_USER_ADDR_T("/dev/null"),
				    imgp->ip_vfs_context);

				if ((error = vn_open(ndp, flag, 0)) != 0) {
					fp_free(p, indx, fp);
					kfree_type(struct nameidata, ndp);
					break;
				}

				struct fileglob *fg = fp->fp_glob;

				fg->fg_flag = flag;
				fg->fg_ops = &vnops;
				fp_set_data(fp, ndp->ni_vp);

				vnode_put(ndp->ni_vp);

				proc_fdlock(p);
				procfdtbl_releasefd(p, indx, NULL);
				fp_drop(p, indx, fp, 1);
				proc_fdunlock(p);

				kfree_type(struct nameidata, ndp);
			}
		}
	}
#if CONFIG_MACF
	else {
		/*
		 * We are here because we were told that the MAC label will
		 * be transitioned, and the binary is not VSUID or VSGID; to
		 * deal with this case, we could either duplicate a lot of
		 * code, or we can indicate we want to default the P_SUGID
		 * bit clear and jump back up.
		 */
		if (mac_transition) {
			leave_sugid_clear = 1;
			goto handle_mac_transition;
		}
	}

#endif  /* CONFIG_MACF */

	/* Update the process' identity version and set the security token.
	 * Also, ensure we always see a modified identity version (rdar://129775819).
	 */
	int previous_pid_version = proc_get_ro(p)->p_idversion;
	int new_pid_version;
	do {
		new_pid_version = OSIncrementAtomic(&nextpidversion);
	} while (new_pid_version == previous_pid_version);
	proc_setpidversion(p, new_pid_version);
	task_set_uniqueid(proc_task(p));

	/*
	 * Implement the semantic where the effective user and group become
	 * the saved user and group in exec'ed programs.
	 */
	kauth_cred_proc_update(p, PROC_SETTOKEN_ALWAYS,
	    ^bool (kauth_cred_t parent __unused, kauth_cred_t model) {
		posix_cred_t pcred = posix_cred_get(model);

		if (pcred->cr_svuid == pcred->cr_uid &&
		pcred->cr_svgid == pcred->cr_gid) {
		        return false;
		}

		pcred->cr_svuid = pcred->cr_uid;
		pcred->cr_svgid = pcred->cr_gid;
		return true;
	});

	return error;
}


/*
 * create_unix_stack
 *
 * Description:	Set the user stack address for the process to the provided
 *		address.  If a custom stack was not set as a result of the
 *		load process (i.e. as specified by the image file for the
 *		executable), then allocate the stack in the provided map and
 *		set up appropriate guard pages for enforcing administrative
 *		limits on stack growth, if they end up being needed.
 *
 * Parameters:	p			Process to set stack on
 *		load_result		Information from mach-o load commands
 *		map			Address map in which to allocate the new stack
 *
 * Returns:	KERN_SUCCESS		Stack successfully created
 *		!KERN_SUCCESS		Mach failure code
 */
__attribute__((noinline))
static kern_return_t
create_unix_stack(vm_map_t map, load_result_t* load_result,
    proc_t p)
{
	mach_vm_size_t          size, prot_size;
	mach_vm_offset_t        addr, prot_addr;
	kern_return_t           kr;

	mach_vm_address_t       user_stack = load_result->user_stack;

	proc_lock(p);
	p->user_stack = (uintptr_t)user_stack;
	if (load_result->custom_stack) {
		p->p_lflag |= P_LCUSTOM_STACK;
	}
	proc_unlock(p);
	if (vm_map_page_shift(map) < (int)PAGE_SHIFT) {
		DEBUG4K_LOAD("map %p user_stack 0x%llx custom %d user_stack_alloc_size 0x%llx\n", map, user_stack, load_result->custom_stack, load_result->user_stack_alloc_size);
	}

	if (load_result->user_stack_alloc_size > 0) {
		/*
		 * Allocate enough space for the maximum stack size we
		 * will ever authorize and an extra page to act as
		 * a guard page for stack overflows. For default stacks,
		 * vm_initial_limit_stack takes care of the extra guard page.
		 * Otherwise we must allocate it ourselves.
		 */
		if (mach_vm_round_page_overflow(load_result->user_stack_alloc_size, &size)) {
			return KERN_INVALID_ARGUMENT;
		}
		addr = vm_map_trunc_page(load_result->user_stack - size,
		    vm_map_page_mask(map));
		kr = mach_vm_allocate_kernel(map, &addr, size,
		    VM_MAP_KERNEL_FLAGS_FIXED(.vm_tag = VM_MEMORY_STACK));
		if (kr != KERN_SUCCESS) {
			// Can't allocate at default location, try anywhere
			addr = 0;
			kr = mach_vm_allocate_kernel(map, &addr, size,
			    VM_MAP_KERNEL_FLAGS_ANYWHERE(.vm_tag = VM_MEMORY_STACK));
			if (kr != KERN_SUCCESS) {
				return kr;
			}

			user_stack = addr + size;
			load_result->user_stack = (user_addr_t)user_stack;

			proc_lock(p);
			p->user_stack = (uintptr_t)user_stack;
			proc_unlock(p);
		}

		load_result->user_stack_alloc = (user_addr_t)addr;

		/*
		 * And prevent access to what's above the current stack
		 * size limit for this process.
		 */
		if (load_result->user_stack_size == 0) {
			load_result->user_stack_size = proc_limitgetcur(p, RLIMIT_STACK);
			prot_size = vm_map_trunc_page(size - load_result->user_stack_size, vm_map_page_mask(map));
		} else {
			prot_size = PAGE_SIZE;
		}

		prot_addr = addr;
		kr = mach_vm_protect(map,
		    prot_addr,
		    prot_size,
		    FALSE,
		    VM_PROT_NONE);
		if (kr != KERN_SUCCESS) {
			(void)mach_vm_deallocate(map, addr, size);
			return kr;
		}
	}

	return KERN_SUCCESS;
}

#include <sys/reboot.h>

/*
 * load_init_program_at_path
 *
 * Description:	Load the "init" program; in most cases, this will be "launchd"
 *
 * Parameters:	p			Process to call execve() to create
 *					the "init" program
 *		scratch_addr		Page in p, scratch space
 *		path			NULL terminated path
 *
 * Returns:	KERN_SUCCESS		Success
 *		!KERN_SUCCESS           See execve/mac_execve for error codes
 *
 * Notes:	The process that is passed in is the first manufactured
 *		process on the system, and gets here via bsd_ast() firing
 *		for the first time.  This is done to ensure that bsd_init()
 *		has run to completion.
 *
 *		The address map of the first manufactured process matches the
 *		word width of the kernel. Once the self-exec completes, the
 *		initproc might be different.
 */
static int
load_init_program_at_path(proc_t p, user_addr_t scratch_addr, const char* path)
{
	int retval[2];
	int error;
	struct execve_args init_exec_args;
	user_addr_t argv0 = USER_ADDR_NULL, argv1 = USER_ADDR_NULL;

	/*
	 * Validate inputs and pre-conditions
	 */
	assert(p);
	assert(scratch_addr);
	assert(path);

	/*
	 * Copy out program name.
	 */
	size_t path_length = strlen(path) + 1;
	argv0 = scratch_addr;
	error = copyout(path, argv0, path_length);
	if (error) {
		return error;
	}

	scratch_addr = USER_ADDR_ALIGN(scratch_addr + path_length, sizeof(user_addr_t));

	/*
	 * Put out first (and only) argument, similarly.
	 * Assumes everything fits in a page as allocated above.
	 */
	if (boothowto & RB_SINGLE) {
		const char *init_args = "-s";
		size_t init_args_length = strlen(init_args) + 1;

		argv1 = scratch_addr;
		error = copyout(init_args, argv1, init_args_length);
		if (error) {
			return error;
		}

		scratch_addr = USER_ADDR_ALIGN(scratch_addr + init_args_length, sizeof(user_addr_t));
	}

	if (proc_is64bit(p)) {
		user64_addr_t argv64bit[3] = {};

		argv64bit[0] = argv0;
		argv64bit[1] = argv1;
		argv64bit[2] = USER_ADDR_NULL;

		error = copyout(argv64bit, scratch_addr, sizeof(argv64bit));
		if (error) {
			return error;
		}
	} else {
		user32_addr_t argv32bit[3] = {};

		argv32bit[0] = (user32_addr_t)argv0;
		argv32bit[1] = (user32_addr_t)argv1;
		argv32bit[2] = USER_ADDR_NULL;

		error = copyout(argv32bit, scratch_addr, sizeof(argv32bit));
		if (error) {
			return error;
		}
	}

	/*
	 * Set up argument block for fake call to execve.
	 */
	init_exec_args.fname = argv0;
	init_exec_args.argp = scratch_addr;
	init_exec_args.envp = USER_ADDR_NULL;

	/*
	 * So that init task is set with uid,gid 0 token
	 *
	 * The access to the cred is safe:
	 * the proc isn't running yet, it's stable.
	 */
	set_security_token(p, proc_ucred_unsafe(p));

	return execve(p, &init_exec_args, retval);
}

static const char * init_programs[] = {
#if DEBUG
	"/usr/appleinternal/sbin/launchd.debug",
#endif
#if DEVELOPMENT || DEBUG
	"/usr/appleinternal/sbin/launchd.development",
#endif
	"/sbin/launchd",
};

/*
 * load_init_program
 *
 * Description:	Load the "init" program; in most cases, this will be "launchd"
 *
 * Parameters:	p			Process to call execve() to create
 *					the "init" program
 *
 * Returns:	(void)
 *
 * Notes:	The process that is passed in is the first manufactured
 *		process on the system, and gets here via bsd_ast() firing
 *		for the first time.  This is done to ensure that bsd_init()
 *		has run to completion.
 *
 *		In DEBUG & DEVELOPMENT builds, the launchdsuffix boot-arg
 *		may be used to select a specific launchd executable. As with
 *		the kcsuffix boot-arg, setting launchdsuffix to "" or "release"
 *		will force /sbin/launchd to be selected.
 *
 *              Search order by build:
 *
 * DEBUG	DEVELOPMENT	RELEASE		PATH
 * ----------------------------------------------------------------------------------
 * 1		1		NA		/usr/appleinternal/sbin/launchd.$LAUNCHDSUFFIX
 * 2		NA		NA		/usr/appleinternal/sbin/launchd.debug
 * 3		2		NA		/usr/appleinternal/sbin/launchd.development
 * 4		3		1		/sbin/launchd
 */
void
load_init_program(proc_t p)
{
	uint32_t i;
	int error;
	vm_map_t map = current_map();
	mach_vm_offset_t scratch_addr = 0;
	mach_vm_size_t map_page_size = vm_map_page_size(map);

#if DEVELOPMENT || DEBUG
	/* Use the opportunity to initialize exec's debug log stream */
	exec_log_handle = os_log_create("com.apple.xnu.bsd", "exec");
#endif /* DEVELOPMENT || DEBUG */

	(void) mach_vm_allocate_kernel(map, &scratch_addr, map_page_size,
	    VM_MAP_KERNEL_FLAGS_ANYWHERE());
#if CONFIG_MEMORYSTATUS
	(void) memorystatus_init_at_boot_snapshot();
#endif /* CONFIG_MEMORYSTATUS */

#if DEBUG || DEVELOPMENT
	/* Check for boot-arg suffix first */
	char launchd_suffix[64];
	if (PE_parse_boot_argn("launchdsuffix", launchd_suffix, sizeof(launchd_suffix))) {
		char launchd_path[128];
		boolean_t is_release_suffix = ((launchd_suffix[0] == 0) ||
		    (strcmp(launchd_suffix, "release") == 0));

		if (is_release_suffix) {
			printf("load_init_program: attempting to load /sbin/launchd\n");
			error = load_init_program_at_path(p, (user_addr_t)scratch_addr, "/sbin/launchd");
			if (!error) {
				return;
			}

			panic("Process 1 exec of launchd.release failed, errno %d", error);
		} else {
			strlcpy(launchd_path, "/usr/appleinternal/sbin/launchd.", sizeof(launchd_path));
			strlcat(launchd_path, launchd_suffix, sizeof(launchd_path));

			printf("load_init_program: attempting to load %s\n", launchd_path);
			error = load_init_program_at_path(p, (user_addr_t)scratch_addr, launchd_path);
			if (!error) {
				return;
			} else if (error != ENOENT) {
				printf("load_init_program: failed loading %s: errno %d\n", launchd_path, error);
			}
		}
	}
#endif

	error = ENOENT;
	for (i = 0; i < sizeof(init_programs) / sizeof(init_programs[0]); i++) {
		printf("load_init_program: attempting to load %s\n", init_programs[i]);
		error = load_init_program_at_path(p, (user_addr_t)scratch_addr, init_programs[i]);
		if (!error) {
			return;
		} else if (error != ENOENT) {
			printf("load_init_program: failed loading %s: errno %d\n", init_programs[i], error);
		}
	}

	panic("Process 1 exec of %s failed, errno %d", ((i == 0) ? "<null>" : init_programs[i - 1]), error);
}

/*
 * load_return_to_errno
 *
 * Description:	Convert a load_return_t (Mach error) to an errno (BSD error)
 *
 * Parameters:	lrtn			Mach error number
 *
 * Returns:	(int)			BSD error number
 *		0			Success
 *		EBADARCH		Bad architecture
 *		EBADMACHO		Bad Mach object file
 *		ESHLIBVERS		Bad shared library version
 *		ENOMEM			Out of memory/resource shortage
 *		EACCES			Access denied
 *		ENOENT			Entry not found (usually "file does
 *					does not exist")
 *		EIO			An I/O error occurred
 *		EBADEXEC		The executable is corrupt/unknown
 */
static int
load_return_to_errno(load_return_t lrtn)
{
	switch (lrtn) {
	case LOAD_SUCCESS:
		return 0;
	case LOAD_BADARCH:
		return EBADARCH;
	case LOAD_BADMACHO:
	case LOAD_BADMACHO_UPX:
		return EBADMACHO;
	case LOAD_SHLIB:
		return ESHLIBVERS;
	case LOAD_NOSPACE:
	case LOAD_RESOURCE:
		return ENOMEM;
	case LOAD_PROTECT:
		return EACCES;
	case LOAD_ENOENT:
		return ENOENT;
	case LOAD_IOERROR:
		return EIO;
	case LOAD_DECRYPTFAIL:
		return EAUTH;
	case LOAD_FAILURE:
	default:
		return EBADEXEC;
	}
}

#include <mach/mach_types.h>
#include <mach/vm_prot.h>
#include <mach/semaphore.h>
#include <mach/sync_policy.h>
#include <kern/clock.h>
#include <mach/kern_return.h>

/*
 * execargs_alloc
 *
 * Description:	Allocate the block of memory used by the execve arguments.
 *		At the same time, we allocate a page so that we can read in
 *		the first page of the image.
 *
 * Parameters:	struct image_params *	the image parameter block
 *
 * Returns:	0			Success
 *		EINVAL			Invalid argument
 *		EACCES			Permission denied
 *		EINTR			Interrupted function
 *		ENOMEM			Not enough space
 *
 * Notes:	This is a temporary allocation into the kernel address space
 *		to enable us to copy arguments in from user space.  This is
 *		necessitated by not mapping the process calling execve() into
 *		the kernel address space during the execve() system call.
 *
 *		We assemble the argument and environment, etc., into this
 *		region before copying it as a single block into the child
 *		process address space (at the top or bottom of the stack,
 *		depending on which way the stack grows; see the function
 *		exec_copyout_strings() for details).
 *
 *		This ends up with a second (possibly unnecessary) copy compared
 *		with assembing the data directly into the child address space,
 *		instead, but since we cannot be guaranteed that the parent has
 *		not modified its environment, we can't really know that it's
 *		really a block there as well.
 */


static int execargs_waiters = 0;
static LCK_MTX_DECLARE_ATTR(execargs_cache_lock, &proc_lck_grp, &proc_lck_attr);

static void
execargs_lock_lock(void)
{
	lck_mtx_lock_spin(&execargs_cache_lock);
}

static void
execargs_lock_unlock(void)
{
	lck_mtx_unlock(&execargs_cache_lock);
}

static wait_result_t
execargs_lock_sleep(void)
{
	return lck_mtx_sleep(&execargs_cache_lock, LCK_SLEEP_DEFAULT, &execargs_free_count, THREAD_INTERRUPTIBLE);
}

static kern_return_t
execargs_purgeable_allocate(char **execarg_address)
{
	mach_vm_offset_t addr = 0;
	kern_return_t kr = mach_vm_allocate_kernel(bsd_pageable_map, &addr,
	    BSD_PAGEABLE_SIZE_PER_EXEC,
	    VM_MAP_KERNEL_FLAGS_ANYWHERE(.vmf_purgeable = true));
	*execarg_address = (char *)addr;
	assert(kr == KERN_SUCCESS);
	return kr;
}

static kern_return_t
execargs_purgeable_reference(void *execarg_address)
{
	int state = VM_PURGABLE_NONVOLATILE;
	kern_return_t kr = vm_map_purgable_control(bsd_pageable_map,
	    (vm_offset_t) execarg_address, VM_PURGABLE_SET_STATE, &state);

	assert(kr == KERN_SUCCESS);
	return kr;
}

static kern_return_t
execargs_purgeable_volatilize(void *execarg_address)
{
	int state = VM_PURGABLE_VOLATILE | VM_PURGABLE_ORDERING_OBSOLETE;
	kern_return_t kr;
	kr = vm_map_purgable_control(bsd_pageable_map,
	    (vm_offset_t) execarg_address, VM_PURGABLE_SET_STATE, &state);

	assert(kr == KERN_SUCCESS);

	return kr;
}

static void
execargs_wakeup_waiters(void)
{
	thread_wakeup(&execargs_free_count);
}

static int
execargs_alloc(struct image_params *imgp)
{
	kern_return_t kret;
	wait_result_t res;
	int i, cache_index = -1;

	execargs_lock_lock();

	while (execargs_free_count == 0) {
		execargs_waiters++;
		res = execargs_lock_sleep();
		execargs_waiters--;
		if (res != THREAD_AWAKENED) {
			execargs_lock_unlock();
			return EINTR;
		}
	}

	execargs_free_count--;

	for (i = 0; i < execargs_cache_size; i++) {
		vm_offset_t element = execargs_cache[i];
		if (element) {
			cache_index = i;
			imgp->ip_strings = (char *)(execargs_cache[i]);
			execargs_cache[i] = 0;
			break;
		}
	}

	assert(execargs_free_count >= 0);

	execargs_lock_unlock();

	if (cache_index == -1) {
		kret = execargs_purgeable_allocate(&imgp->ip_strings);
	} else {
		kret = execargs_purgeable_reference(imgp->ip_strings);
	}

	assert(kret == KERN_SUCCESS);
	if (kret != KERN_SUCCESS) {
		return ENOMEM;
	}

	/* last page used to read in file headers */
	imgp->ip_vdata = imgp->ip_strings + (NCARGS + PAGE_SIZE);
	imgp->ip_strendp = imgp->ip_strings;
	imgp->ip_argspace = NCARGS;
	imgp->ip_strspace = (NCARGS + PAGE_SIZE);

	return 0;
}

/*
 * execargs_free
 *
 * Description:	Free the block of memory used by the execve arguments and the
 *		first page of the executable by a previous call to the function
 *		execargs_alloc().
 *
 * Parameters:	struct image_params *	the image parameter block
 *
 * Returns:	0			Success
 *		EINVAL			Invalid argument
 *		EINTR			Oeration interrupted
 */
static int
execargs_free(struct image_params *imgp)
{
	kern_return_t kret;
	int i;
	boolean_t needs_wakeup = FALSE;

	kret = execargs_purgeable_volatilize(imgp->ip_strings);

	execargs_lock_lock();
	execargs_free_count++;

	for (i = 0; i < execargs_cache_size; i++) {
		vm_offset_t element = execargs_cache[i];
		if (element == 0) {
			execargs_cache[i] = (vm_offset_t) imgp->ip_strings;
			imgp->ip_strings = NULL;
			break;
		}
	}

	assert(imgp->ip_strings == NULL);

	if (execargs_waiters > 0) {
		needs_wakeup = TRUE;
	}

	execargs_lock_unlock();

	if (needs_wakeup == TRUE) {
		execargs_wakeup_waiters();
	}

	return kret == KERN_SUCCESS ? 0 : EINVAL;
}

void
uthread_set_exec_data(struct uthread *uth, struct image_params *imgp)
{
	uth->uu_save.uus_exec_data.imgp = imgp;
}

size_t
thread_get_current_exec_path(char *path, size_t size)
{
	struct uthread *uth = current_uthread();
	struct image_params *imgp = uth->uu_save.uus_exec_data.imgp;
	size_t string_size = 0;
	char *exec_path;

	if (path == NULL || imgp == NULL || imgp->ip_strings == NULL) {
		return 0;
	}

	exec_path = imgp->ip_strings + strlen(EXECUTABLE_KEY);
	string_size = imgp->ip_strendp - exec_path;
	string_size = MIN(MAXPATHLEN, string_size);
	string_size = MIN(size, string_size);

	string_size = strlcpy(path, exec_path, string_size);
	return string_size;
}
static void
exec_resettextvp(proc_t p, struct image_params *imgp)
{
	vnode_t vp;
	off_t offset;
	vnode_t tvp  = p->p_textvp;
	int ret;

	vp = imgp->ip_vp;
	offset = imgp->ip_arch_offset;

	if (vp == NULLVP) {
		panic("exec_resettextvp: expected valid vp");
	}

	ret = vnode_ref(vp);
	proc_lock(p);
	if (ret == 0) {
		p->p_textvp = vp;
		p->p_textoff = offset;
	} else {
		p->p_textvp = NULLVP;   /* this is paranoia */
		p->p_textoff = 0;
	}
	proc_unlock(p);

	if (tvp != NULLVP) {
		if (vnode_getwithref(tvp) == 0) {
			vnode_rele(tvp);
			vnode_put(tvp);
		}
	}
}

// Includes the 0-byte (therefore "SIZE" instead of "LEN").
static const size_t CS_CDHASH_STRING_SIZE = CS_CDHASH_LEN * 2 + 1;

static void
cdhash_to_string(char str[CS_CDHASH_STRING_SIZE], uint8_t const * const cdhash)
{
	static char const nibble[] = "0123456789abcdef";

	/* Apparently still the safest way to get a hex representation
	 * of binary data.
	 * xnu's printf routines have %*D/%20D in theory, but "not really", see:
	 * <rdar://problem/33328859> confusion around %*D/%nD in printf
	 */
	for (int i = 0; i < CS_CDHASH_LEN; ++i) {
		str[i * 2] = nibble[(cdhash[i] & 0xf0) >> 4];
		str[i * 2 + 1] = nibble[cdhash[i] & 0x0f];
	}
	str[CS_CDHASH_STRING_SIZE - 1] = 0;
}

/*
 * __EXEC_WAITING_ON_TASKGATED_CODE_SIGNATURE_UPCALL__
 *
 * Description: Waits for the userspace daemon to respond to the request
 *              we made. Function declared non inline to be visible in
 *		stackshots and spindumps as well as debugging.
 */
__attribute__((noinline)) int
__EXEC_WAITING_ON_TASKGATED_CODE_SIGNATURE_UPCALL__(mach_port_t task_access_port, int32_t new_pid)
{
	return find_code_signature(task_access_port, new_pid);
}

/*
 * Update signature dependent process state, called by
 * process_signature.
 */
static int
proc_process_signature(proc_t p, os_reason_t *signature_failure_reason)
{
	int error = 0;
	char const *error_msg = NULL;

	kern_return_t kr = machine_task_process_signature(proc_get_task_raw(p), proc_platform(p), proc_sdk(p), &error_msg);

	if (kr != KERN_SUCCESS) {
		error = EINVAL;

		if (error_msg != NULL) {
			uint32_t error_msg_len = (uint32_t)strlen(error_msg) + 1;
			mach_vm_address_t data_addr = 0;
			int reason_error = 0;
			int kcdata_error = 0;

			os_reason_t reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_SECURITY_POLICY);
			reason->osr_flags = OS_REASON_FLAG_GENERATE_CRASH_REPORT | OS_REASON_FLAG_CONSISTENT_FAILURE;

			if ((reason_error = os_reason_alloc_buffer_noblock(reason,
			    kcdata_estimate_required_buffer_size(1, error_msg_len))) == 0 &&
			    (kcdata_error = kcdata_get_memory_addr(&reason->osr_kcd_descriptor,
			    EXIT_REASON_USER_DESC, error_msg_len,
			    &data_addr)) == KERN_SUCCESS) {
				kern_return_t mc_error = kcdata_memcpy(&reason->osr_kcd_descriptor, (mach_vm_address_t)data_addr,
				    error_msg, error_msg_len);

				if (mc_error != KERN_SUCCESS) {
					printf("process_signature: failed to copy reason string (kcdata_memcpy error: %d)\n",
					    mc_error);
				}
			} else {
				printf("failed to allocate space for reason string (os_reason_alloc_buffer error: %d, kcdata error: %d, length: %u)\n",
				    reason_error, kcdata_error, error_msg_len);
			}

			assert(*signature_failure_reason == NULL); // shouldn't have gotten so far
			*signature_failure_reason = reason;
		}
	}
	return error;
}

static int
process_signature(proc_t p, struct image_params *imgp)
{
	mach_port_t port = IPC_PORT_NULL;
	kern_return_t kr = KERN_FAILURE;
	int error = EACCES;
	boolean_t unexpected_failure = FALSE;
	struct cs_blob *csb;
	boolean_t require_success = FALSE;
	int spawn = (imgp->ip_flags & IMGPF_SPAWN);
	const int vfexec = 0;
	os_reason_t signature_failure_reason = OS_REASON_NULL;

	/*
	 * Override inherited code signing flags with the
	 * ones for the process that is being successfully
	 * loaded
	 */
	proc_lock(p);
	proc_csflags_update(p, imgp->ip_csflags);
	proc_unlock(p);

	/* Set the switch_protect flag on the map */
	if (proc_getcsflags(p) & (CS_HARD | CS_KILL)) {
		vm_map_switch_protect(get_task_map(proc_task(p)), TRUE);
	}
	/* set the cs_enforced flags in the map */
	if (proc_getcsflags(p) & CS_ENFORCEMENT) {
		vm_map_cs_enforcement_set(get_task_map(proc_task(p)), TRUE);
	} else {
		vm_map_cs_enforcement_set(get_task_map(proc_task(p)), FALSE);
	}

	/*
	 * image activation may be failed due to policy
	 * which is unexpected but security framework does not
	 * approve of exec, kill and return immediately.
	 */
	if (imgp->ip_mac_return != 0) {
		KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
		    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_SECURITY_POLICY, 0, 0);
		signature_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_SECURITY_POLICY);
		error = imgp->ip_mac_return;
		unexpected_failure = TRUE;
		goto done;
	}

	if (imgp->ip_cs_error != OS_REASON_NULL) {
		signature_failure_reason = imgp->ip_cs_error;
		imgp->ip_cs_error = OS_REASON_NULL;
		error = EACCES;
		goto done;
	}

	/* call the launch constraints hook */
	os_reason_t launch_constraint_reason;
	if ((error = mac_proc_check_launch_constraints(p, imgp, &launch_constraint_reason)) != 0) {
		signature_failure_reason = launch_constraint_reason;
		goto done;
	}

#if XNU_TARGET_OS_OSX
	/* Check for platform passed in spawn attr if iOS binary is being spawned */
	if (proc_platform(p) == PLATFORM_IOS) {
		struct _posix_spawnattr *psa = (struct _posix_spawnattr *) imgp->ip_px_sa;
		if (psa == NULL || psa->psa_platform == 0) {
			boolean_t no_sandbox_entitled = FALSE;
#if DEBUG || DEVELOPMENT
			/*
			 * Allow iOS binaries to spawn on internal systems
			 * if no-sandbox entitlement is present of unentitled_ios_sim_launch
			 * boot-arg set to true
			 */
			if (unentitled_ios_sim_launch) {
				no_sandbox_entitled = TRUE;
			} else {
				no_sandbox_entitled = IOVnodeHasEntitlement(imgp->ip_vp,
				    (int64_t)imgp->ip_arch_offset, "com.apple.private.security.no-sandbox");
			}
#endif /* DEBUG || DEVELOPMENT */
			if (!no_sandbox_entitled) {
				signature_failure_reason = os_reason_create(OS_REASON_EXEC,
				    EXEC_EXIT_REASON_WRONG_PLATFORM);
				error = EACCES;
				goto done;
			}
			printf("Allowing spawn of iOS binary %s since it has "
			    "com.apple.private.security.no-sandbox entitlement or unentitled_ios_sim_launch "
			    "boot-arg set to true\n", p->p_name);
		} else if (psa->psa_platform != PLATFORM_IOS) {
			/* Simulator binary spawned with wrong platform */
			signature_failure_reason = os_reason_create(OS_REASON_EXEC,
			    EXEC_EXIT_REASON_WRONG_PLATFORM);
			error = EACCES;
			goto done;
		} else {
			printf("Allowing spawn of iOS binary %s since correct platform was passed in spawn\n",
			    p->p_name);
		}
	}
#endif /* XNU_TARGET_OS_OSX */

	/* If the code signature came through the image activation path, we skip the
	 * taskgated / externally attached path. */
	if (imgp->ip_csflags & CS_SIGNED) {
		error = 0;
		goto done;
	}

	/* The rest of the code is for signatures that either already have been externally
	 * attached (likely, but not necessarily by a previous run through the taskgated
	 * path), or that will now be attached by taskgated. */

	kr = task_get_task_access_port(proc_task(p), &port);
	if (KERN_SUCCESS != kr || !IPC_PORT_VALID(port)) {
		error = 0;
		if (require_success) {
			KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
			    proc_getpid(p), OS_REASON_CODESIGNING, CODESIGNING_EXIT_REASON_TASK_ACCESS_PORT, 0, 0);
			signature_failure_reason = os_reason_create(OS_REASON_CODESIGNING, CODESIGNING_EXIT_REASON_TASK_ACCESS_PORT);
			error = EACCES;
		}
		goto done;
	}

	/*
	 * taskgated returns KERN_SUCCESS if it has completed its work
	 * and the exec should continue, KERN_FAILURE if the exec should
	 * fail, or it may error out with different error code in an
	 * event of mig failure (e.g. process was signalled during the
	 * rpc call, taskgated died, mig server died etc.).
	 */

	kr = __EXEC_WAITING_ON_TASKGATED_CODE_SIGNATURE_UPCALL__(port, proc_getpid(p));
	switch (kr) {
	case KERN_SUCCESS:
		error = 0;
		break;
	case KERN_FAILURE:
		error = EACCES;

		KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
		    proc_getpid(p), OS_REASON_CODESIGNING, CODESIGNING_EXIT_REASON_TASKGATED_INVALID_SIG, 0, 0);
		signature_failure_reason = os_reason_create(OS_REASON_CODESIGNING, CODESIGNING_EXIT_REASON_TASKGATED_INVALID_SIG);
		goto done;
	default:
		error = EACCES;

		KERNEL_DEBUG_CONSTANT(BSDDBG_CODE(DBG_BSD_PROC, BSD_PROC_EXITREASON_CREATE) | DBG_FUNC_NONE,
		    proc_getpid(p), OS_REASON_EXEC, EXEC_EXIT_REASON_TASKGATED_OTHER, 0, 0);
		signature_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_TASKGATED_OTHER);
		unexpected_failure = TRUE;
		goto done;
	}

	/* Only do this if exec_resettextvp() did not fail */
	if (p->p_textvp != NULLVP) {
		csb = ubc_cs_blob_get(p->p_textvp, -1, -1, p->p_textoff);

		if (csb != NULL) {
			/* As the enforcement we can do here is very limited, we only allow things that
			 * are the only reason why this code path still exists:
			 * Adhoc signed non-platform binaries without special cs_flags and without any
			 * entitlements (unrestricted ones still pass AMFI). */
			if (
				/* Revalidate the blob if necessary through bumped generation count. */
				(ubc_cs_generation_check(p->p_textvp) == 0 ||
				ubc_cs_blob_revalidate(p->p_textvp, csb, imgp, 0, proc_platform(p)) == 0) &&
				/* Only CS_ADHOC, no CS_KILL, CS_HARD etc. */
				(csb->csb_flags & CS_ALLOWED_MACHO) == CS_ADHOC &&
				/* If it has a CMS blob, it's not adhoc. The CS_ADHOC flag can lie. */
				csblob_find_blob_bytes((const uint8_t *)csb->csb_mem_kaddr, csb->csb_mem_size,
				CSSLOT_SIGNATURESLOT,
				CSMAGIC_BLOBWRAPPER) == NULL &&
				/* It could still be in a trust cache (unlikely with CS_ADHOC), or a magic path. */
				csb->csb_platform_binary == 0 &&
				/* No entitlements, not even unrestricted ones. */
				csb->csb_entitlements_blob == NULL &&
				csb->csb_der_entitlements_blob == NULL) {
				proc_lock(p);
				proc_csflags_set(p, CS_SIGNED | CS_VALID);
				proc_unlock(p);
			} else {
				uint8_t cdhash[CS_CDHASH_LEN];
				char cdhash_string[CS_CDHASH_STRING_SIZE];
				proc_getcdhash(p, cdhash);
				cdhash_to_string(cdhash_string, cdhash);
				printf("ignoring detached code signature on '%s' with cdhash '%s' "
				    "because it is invalid, or not a simple adhoc signature.\n",
				    p->p_name, cdhash_string);
			}
		}
	}

done:
	if (0 == error) {
		/*
		 * Update the new process's signature-dependent process state.
		 * state.
		 */

		error = proc_process_signature(p, &signature_failure_reason);
	}

	if (0 == error) {
		/*
		 * Update the new main thread's signature-dependent thread
		 * state. This was also called when the thread was created,
		 * but for the main thread the signature was not yet attached
		 * at that time.
		 */
		kr = thread_process_signature(imgp->ip_new_thread, proc_get_task_raw(p));

		if (kr != KERN_SUCCESS) {
			error = EINVAL;
			signature_failure_reason = os_reason_create(OS_REASON_EXEC, EXEC_EXIT_REASON_MACHINE_THREAD);
		}
	}

	if (0 == error) {
		/* The process's code signature related properties are
		 * fully set up, so this is an opportune moment to log
		 * platform binary execution, if desired. */
		if (platform_exec_logging != 0 && csproc_get_platform_binary(p)) {
			uint8_t cdhash[CS_CDHASH_LEN];
			char cdhash_string[CS_CDHASH_STRING_SIZE];
			proc_getcdhash(p, cdhash);
			cdhash_to_string(cdhash_string, cdhash);

			os_log(peLog, "CS Platform Exec Logging: Executing platform signed binary "
			    "'%s' with cdhash %s\n", p->p_name, cdhash_string);
		}
	} else {
		if (!unexpected_failure) {
			proc_csflags_set(p, CS_KILLED);
		}
		/* make very sure execution fails */
		if (vfexec || spawn) {
			assert(signature_failure_reason != OS_REASON_NULL);
			psignal_vfork_with_reason(p, proc_task(p), imgp->ip_new_thread,
			    SIGKILL, signature_failure_reason);
			signature_failure_reason = OS_REASON_NULL;
			error = 0;
		} else {
			assert(signature_failure_reason != OS_REASON_NULL);
			psignal_with_reason(p, SIGKILL, signature_failure_reason);
			signature_failure_reason = OS_REASON_NULL;
		}
	}

	if (port != IPC_PORT_NULL) {
		ipc_port_release_send(port);
	}

	/* If we hit this, we likely would have leaked an exit reason */
	assert(signature_failure_reason == OS_REASON_NULL);
	return error;
}

/*
 * Typically as soon as we start executing this process, the
 * first instruction will trigger a VM fault to bring the text
 * pages (as executable) into the address space, followed soon
 * thereafter by dyld data structures (for dynamic executable).
 * To optimize this, as well as improve support for hardware
 * debuggers that can only access resident pages present
 * in the process' page tables, we prefault some pages if
 * possible. Errors are non-fatal.
 */
#ifndef PREVENT_CALLER_STACK_USE
#define PREVENT_CALLER_STACK_USE __attribute__((noinline))
#endif

/*
 * Prefaulting dyld data does not work (rdar://76621401)
 */
#define FIXED_76621401 0
static void PREVENT_CALLER_STACK_USE
exec_prefault_data(
	__unused proc_t p,
	__unused struct image_params *imgp,
	__unused load_result_t *load_result)
{
#if FIXED_76621401
	int ret;
	size_t expected_all_image_infos_size;
#endif /* FIXED_76621401 */
	kern_return_t kr;

	/*
	 * Prefault executable or dyld entry point.
	 */
	if (vm_map_page_shift(current_map()) < (int)PAGE_SHIFT) {
		DEBUG4K_LOAD("entry_point 0x%llx\n", (uint64_t)load_result->entry_point);
	}
	kr = vm_fault(current_map(),
	    vm_map_trunc_page(load_result->entry_point,
	    vm_map_page_mask(current_map())),
	    VM_PROT_READ | VM_PROT_EXECUTE,
	    FALSE, VM_KERN_MEMORY_NONE,
	    THREAD_UNINT, NULL, 0);
	if (kr != KERN_SUCCESS) {
		DEBUG4K_ERROR("map %p va 0x%llx -> 0x%x\n", current_map(), (uint64_t)vm_map_trunc_page(load_result->entry_point, vm_map_page_mask(current_map())), kr);
	}

#if FIXED_76621401
	if (imgp->ip_flags & IMGPF_IS_64BIT_ADDR) {
		expected_all_image_infos_size = sizeof(struct user64_dyld_all_image_infos);
	} else {
		expected_all_image_infos_size = sizeof(struct user32_dyld_all_image_infos);
	}

	/* Decode dyld anchor structure from <mach-o/dyld_images.h> */
	if (load_result->dynlinker &&
	    load_result->all_image_info_addr &&
	    load_result->all_image_info_size >= expected_all_image_infos_size) {
		union {
			struct user64_dyld_all_image_infos      infos64;
			struct user32_dyld_all_image_infos      infos32;
		} all_image_infos;

		/*
		 * Pre-fault to avoid copyin() going through the trap handler
		 * and recovery path.
		 */
		if (vm_map_page_shift(current_map()) < (int)PAGE_SHIFT) {
			DEBUG4K_LOAD("all_image_info_addr 0x%llx\n", load_result->all_image_info_addr);
		}
		kr = vm_fault(current_map(),
		    vm_map_trunc_page(load_result->all_image_info_addr,
		    vm_map_page_mask(current_map())),
		    VM_PROT_READ | VM_PROT_WRITE,
		    FALSE, VM_KERN_MEMORY_NONE,
		    THREAD_UNINT, NULL, 0);
		if (kr != KERN_SUCCESS) {
//			printf("%s:%d map %p va 0x%llx -> 0x%x\n", __FUNCTION__, __LINE__, current_map(), vm_map_trunc_page(load_result->all_image_info_addr, vm_map_page_mask(current_map())), kr);
		}
		if ((load_result->all_image_info_addr & PAGE_MASK) + expected_all_image_infos_size > PAGE_SIZE) {
			/* all_image_infos straddles a page */
			kr = vm_fault(current_map(),
			    vm_map_trunc_page(load_result->all_image_info_addr + expected_all_image_infos_size - 1,
			    vm_map_page_mask(current_map())),
			    VM_PROT_READ | VM_PROT_WRITE,
			    FALSE, VM_KERN_MEMORY_NONE,
			    THREAD_UNINT, NULL, 0);
			if (kr != KERN_SUCCESS) {
//				printf("%s:%d map %p va 0x%llx -> 0x%x\n", __FUNCTION__, __LINE__, current_map(), vm_map_trunc_page(load_result->all_image_info_addr + expected_all_image_infos_size -1, vm_map_page_mask(current_map())), kr);
			}
		}

		if (vm_map_page_shift(current_map()) < (int)PAGE_SHIFT) {
			DEBUG4K_LOAD("copyin(0x%llx, 0x%lx)\n", load_result->all_image_info_addr, expected_all_image_infos_size);
		}
		ret = copyin((user_addr_t)load_result->all_image_info_addr,
		    &all_image_infos,
		    expected_all_image_infos_size);
		if (ret == 0 && all_image_infos.infos32.version >= DYLD_ALL_IMAGE_INFOS_ADDRESS_MINIMUM_VERSION) {
			user_addr_t notification_address;
			user_addr_t dyld_image_address;
			user_addr_t dyld_version_address;
			user_addr_t dyld_all_image_infos_address;
			user_addr_t dyld_slide_amount;

			if (imgp->ip_flags & IMGPF_IS_64BIT_ADDR) {
				notification_address = (user_addr_t)all_image_infos.infos64.notification;
				dyld_image_address = (user_addr_t)all_image_infos.infos64.dyldImageLoadAddress;
				dyld_version_address = (user_addr_t)all_image_infos.infos64.dyldVersion;
				dyld_all_image_infos_address = (user_addr_t)all_image_infos.infos64.dyldAllImageInfosAddress;
			} else {
				notification_address = all_image_infos.infos32.notification;
				dyld_image_address = all_image_infos.infos32.dyldImageLoadAddress;
				dyld_version_address = all_image_infos.infos32.dyldVersion;
				dyld_all_image_infos_address = all_image_infos.infos32.dyldAllImageInfosAddress;
			}

			/*
			 * dyld statically sets up the all_image_infos in its Mach-O
			 * binary at static link time, with pointers relative to its default
			 * load address. Since ASLR might slide dyld before its first
			 * instruction is executed, "dyld_slide_amount" tells us how far
			 * dyld was loaded compared to its default expected load address.
			 * All other pointers into dyld's image should be adjusted by this
			 * amount. At some point later, dyld will fix up pointers to take
			 * into account the slide, at which point the all_image_infos_address
			 * field in the structure will match the runtime load address, and
			 * "dyld_slide_amount" will be 0, if we were to consult it again.
			 */

			dyld_slide_amount = (user_addr_t)load_result->all_image_info_addr - dyld_all_image_infos_address;

#if 0
			kprintf("exec_prefault: 0x%016llx 0x%08x 0x%016llx 0x%016llx 0x%016llx 0x%016llx\n",
			    (uint64_t)load_result->all_image_info_addr,
			    all_image_infos.infos32.version,
			    (uint64_t)notification_address,
			    (uint64_t)dyld_image_address,
			    (uint64_t)dyld_version_address,
			    (uint64_t)dyld_all_image_infos_address);
#endif

			if (vm_map_page_shift(current_map()) < (int)PAGE_SHIFT) {
				DEBUG4K_LOAD("notification_address 0x%llx dyld_slide_amount 0x%llx\n", (uint64_t)notification_address, (uint64_t)dyld_slide_amount);
			}
			kr = vm_fault(current_map(),
			    vm_map_trunc_page(notification_address + dyld_slide_amount,
			    vm_map_page_mask(current_map())),
			    VM_PROT_READ | VM_PROT_EXECUTE,
			    FALSE, VM_KERN_MEMORY_NONE,
			    THREAD_UNINT, NULL, 0);
			if (kr != KERN_SUCCESS) {
//				printf("%s:%d map %p va 0x%llx -> 0x%x\n", __FUNCTION__, __LINE__, current_map(), vm_map_trunc_page(notification_address + dyld_slide_amount, vm_map_page_mask(current_map())), kr);
			}
			if (vm_map_page_shift(current_map()) < (int)PAGE_SHIFT) {
				DEBUG4K_LOAD("dyld_image_address 0x%llx dyld_slide_amount 0x%llx\n", (uint64_t)dyld_image_address, (uint64_t)dyld_slide_amount);
			}
			kr = vm_fault(current_map(),
			    vm_map_trunc_page(dyld_image_address + dyld_slide_amount,
			    vm_map_page_mask(current_map())),
			    VM_PROT_READ | VM_PROT_EXECUTE,
			    FALSE, VM_KERN_MEMORY_NONE,
			    THREAD_UNINT, NULL, 0);
			if (kr != KERN_SUCCESS) {
//				printf("%s:%d map %p va 0x%llx -> 0x%x\n", __FUNCTION__, __LINE__, current_map(), vm_map_trunc_page(dyld_image_address + dyld_slide_amount, vm_map_page_mask(current_map())), kr);
			}
			if (vm_map_page_shift(current_map()) < (int)PAGE_SHIFT) {
				DEBUG4K_LOAD("dyld_version_address 0x%llx dyld_slide_amount 0x%llx\n", (uint64_t)dyld_version_address, (uint64_t)dyld_slide_amount);
			}
			kr = vm_fault(current_map(),
			    vm_map_trunc_page(dyld_version_address + dyld_slide_amount,
			    vm_map_page_mask(current_map())),
			    VM_PROT_READ,
			    FALSE, VM_KERN_MEMORY_NONE,
			    THREAD_UNINT, NULL, 0);
			if (kr != KERN_SUCCESS) {
//				printf("%s:%d map %p va 0x%llx -> 0x%x\n", __FUNCTION__, __LINE__, current_map(), vm_map_trunc_page(dyld_version_address + dyld_slide_amount, vm_map_page_mask(current_map())), kr);
			}
			if (vm_map_page_shift(current_map()) < (int)PAGE_SHIFT) {
				DEBUG4K_LOAD("dyld_all_image_infos_address 0x%llx dyld_slide_amount 0x%llx\n", (uint64_t)dyld_version_address, (uint64_t)dyld_slide_amount);
			}
			kr = vm_fault(current_map(),
			    vm_map_trunc_page(dyld_all_image_infos_address + dyld_slide_amount,
			    vm_map_page_mask(current_map())),
			    VM_PROT_READ | VM_PROT_WRITE,
			    FALSE, VM_KERN_MEMORY_NONE,
			    THREAD_UNINT, NULL, 0);
			if (kr != KERN_SUCCESS) {
//				printf("%s:%d map %p va 0x%llx -> 0x%x\n", __FUNCTION__, __LINE__, current_map(), vm_map_trunc_page(dyld_all_image_infos_address + dyld_slide_amount, vm_map_page_mask(current_map())), kr);
			}
		}
	}
#endif /* FIXED_76621401 */
}

static int
sysctl_libmalloc_experiments SYSCTL_HANDLER_ARGS
{
#pragma unused(oidp, arg2, req)
	int changed;
	errno_t error;
	uint64_t value = os_atomic_load_wide(&libmalloc_experiment_factors, relaxed);

	error = sysctl_io_number(req, value, sizeof(value), &value, &changed);
	if (error) {
		return error;
	}

	if (changed) {
		os_atomic_store_wide(&libmalloc_experiment_factors, value, relaxed);
	}

	return 0;
}

EXPERIMENT_FACTOR_PROC(_kern, libmalloc_experiments, CTLTYPE_QUAD | CTLFLAG_RW, 0, 0, &sysctl_libmalloc_experiments, "A", "");

SYSCTL_NODE(_kern, OID_AUTO, sec_transition,
    CTLFLAG_RD | CTLFLAG_LOCKED, 0, "sec_transition");

#if DEBUG || DEVELOPMENT
static int
sysctl_setup_ensure_pidversion_changes_on_exec(__unused int64_t in, int64_t *out)
{
	// Tweak nextpidversion to try to trigger a reuse (unless the exec code is doing the right thing)
	int current_pid_version = proc_get_ro(current_proc())->p_idversion;
	nextpidversion = current_pid_version;
	*out = 0;
	return KERN_SUCCESS;
}

SYSCTL_TEST_REGISTER(setup_ensure_pidversion_changes_on_exec, sysctl_setup_ensure_pidversion_changes_on_exec);
#endif /* DEBUG || DEVELOPMENT */
