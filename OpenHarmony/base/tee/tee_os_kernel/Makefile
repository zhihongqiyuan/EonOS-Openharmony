# Copyright (c) 2023 Institute of Parallel And Distributed Systems (IPADS), Shanghai Jiao Tong University (SJTU)
# Licensed under the Mulan PSL v2.
# You can use this software according to the terms and conditions of the Mulan PSL v2.
# You may obtain a copy of Mulan PSL v2 at:
#     http://license.coscl.org.cn/MulanPSL2
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
# PURPOSE.
# See the Mulan PSL v2 for more details.

all: user kernel
include config.mk

Q=@

# libc
LIBC_PROJ_DIR := $(realpath user/chcore-libc)
ifeq ($(OH_DIR),)
OH_DIR := $(realpath ../../../)
endif
LIBC_DIR := $(LIBC_PROJ_DIR)/musl-libc
LIBCHCORE_DIR := $(LIBC_PROJ_DIR)/libchcore
LIBCHCORE_SCRIPTS_DIR := $(LIBCHCORE_DIR)/cmake
LIBCHCORE_OVERRIDES_DIR := $(LIBCHCORE_DIR)/porting/overrides
LIBCHCORE_PATCHES_DIR := $(LIBCHCORE_DIR)/porting/patches
LIBCHCORE_ARCH_INCLUDES_DIR := $(LIBCHCORE_DIR)/arch/$(CHCORE_ARCH)
SECURE_FUNCTION_DIR := $(OH_DIR)/third_party/bounds_checking_function
MUSL_LIBC_DIR := $(OH_DIR)/third_party/musl
LIB_OHTEE_OBJS = $(wildcard user/chcore-libs/sys-libs/libohtee/*.c.o)

ifneq ($(CHCORE_COMPILER_DIR),)
export CHCORE_COMPILER_DIR := $(CHCORE_COMPILER_DIR)
else
# use clang 15.0.4 by default
export CHCORE_COMPILER_DIR := $(OH_DIR)/prebuilts/clang/ohos/linux-x86_64/llvm/
endif

libc:
	# copy musl-libc first
	cp -rL $(MUSL_LIBC_DIR) $(LIBC_DIR)
	rm $(LIBC_DIR)/bundle.json
	cd $(LIBC_DIR) && git checkout 6227345be13b537704a72fc59ebf1735271b52d2 && cd -
	$(Q)bash $(LIBCHCORE_SCRIPTS_DIR)/do_override_dir.sh $(LIBCHCORE_OVERRIDES_DIR) $(LIBC_DIR) \
	&& bash $(LIBCHCORE_SCRIPTS_DIR)/do_patch_dir.sh $(LIBCHCORE_PATCHES_DIR) $(LIBC_DIR) \
	&& cd $(LIBC_DIR) \
	&& ./configure --prefix=$(LIBC_DIR)/install \
		--syslibdir=$(LIBC_DIR)/lib \
		--target=$(CHCORE_CROSS_COMPILE) \
		--with-malloc=oldmalloc \
		COMPILER_DIR=$(CHCORE_COMPILER_DIR) \
		COMPILER=$(CHCORE_COMPILER_DIR)/bin/$(CHCORE_COMPILER) \
		CROSS_COMPILE=$(CHCORE_COMPILER_DIR)/bin/llvm- \
		CFLAGS="$(CONFIG_FLAGS) -I$(LIBCHCORE_ARCH_INCLUDES_DIR) -Wno-int-conversion" \
	&& make -j$(shell nproc) > /dev/null \
	&& make install

# common flags for userspace targets (libs, system servers and apps)
override CC := $(LIBC_DIR)/install/bin/musl-clang
override AR := $(LIBC_DIR)/install/bin/musl-ar
override OBJCOPY := $(CHCORE_COMPILER_DIR)/bin/llvm-objcopy
override OBJDUMP := $(CHCORE_COMPILER_DIR)/bin/llvm-objdump
override RANLIB := $(CHCORE_COMPILER_DIR)/bin/llvm-ranlib
override DEFAULT_CFLAGS = -Wall -O3 $(CONFIG_FLAGS) \
	-I$(realpath user/chcore-libs/sys-libs/libohtee/include)

# bounds checking function
SECURE_FUNCTION_OPTION = -fPIC -fstack-protector-all -Wformat=2 \
			 -Wfloat-equal -Wshadow -Wconversion \
			 -Wformat-security -Wextra \
			 --param ssp-buffer-size=4 -Warray-bounds \
			 -Wpointer-arith -Wcast-qual -Wstrict-prototypes \
			 -Wstrict-overflow=1 -Wstrict-aliasing=2 \
			 -Wswitch -Wswitch-default -Wmissing-prototypes \
			 -I$(SECURE_FUNCTION_DIR)/include \
			 -I$(SECURE_FUNCTION_SOURCE_DIR) \
			 -I$(LIBC_DIR)/install/include

SECURE_FUNCTION_CFLAG = -Wall -DNDEBUG -O2 $(SECURE_FUNCTION_OPTION)

SECURE_FUNCTION_SOURCE_DIR=$(SECURE_FUNCTION_DIR)/src/
SECURE_FUNCTION_SOURCES=$(patsubst $(SECURE_FUNCTION_SOURCE_DIR)/%,%,$(wildcard $(SECURE_FUNCTION_SOURCE_DIR)/*.c))
SECURE_FUNCTION_OBJ_DIR := secure_obj
SECURE_FUNCTION_OBJS := $(patsubst %.c,$(SECURE_FUNCTION_OBJ_DIR)/%.o,$(SECURE_FUNCTION_SOURCES))

$(SECURE_FUNCTION_OBJ_DIR)/%.o: $(SECURE_FUNCTION_SOURCE_DIR)/%.c libc
	$(CHCORE_COMPILER_DIR)/bin/$(CHCORE_COMPILER) --target=aarch64-linux-gnu $(SECURE_FUNCTION_CFLAG) -c $< -o $@

$(SECURE_FUNCTION_OBJ_DIR):
	mkdir -p $(SECURE_FUNCTION_OBJ_DIR)



# link things together: musl-libc, libohtee and secure function
srcdir = user/chcore-libc/musl-libc

MALLOC_DIR = oldmalloc
ARCH = aarch64
SRC_DIRS = $(addprefix $(srcdir)/,src/* src/malloc/$(MALLOC_DIR) crt ldso)
BASE_GLOBS = $(addsuffix /*.c,$(SRC_DIRS))
ARCH_GLOBS = $(addsuffix /$(ARCH)/*.[csS],$(SRC_DIRS))
BASE_SRCS = $(sort $(wildcard $(BASE_GLOBS)))
ARCH_SRCS = $(sort $(wildcard $(ARCH_GLOBS)))
BASE_OBJS = $(patsubst $(srcdir)/%,%.o,$(basename $(BASE_SRCS)))
ARCH_OBJS = $(patsubst $(srcdir)/%,%.o,$(basename $(ARCH_SRCS)))
REPLACED_OBJS = $(sort $(subst /$(ARCH)/,/,$(ARCH_OBJS)))
OBJS = $(addprefix obj/, $(filter-out $(REPLACED_OBJS), $(sort $(BASE_OBJS) $(ARCH_OBJS))))

ALL_OBJS = $(addprefix $(srcdir)/,$(OBJS))
LIBC_OBJS = $(filter $(srcdir)/obj/src/%,$(ALL_OBJS)) $(filter $(srcdir)/obj/compat/%,$(ALL_OBJS))
LDSO_OBJS = $(filter $(srcdir)/obj/ldso/%,$(ALL_OBJS:%.o=%.lo))
CRT_OBJS = $(filter $(srcdir)/obj/crt/%,$(ALL_OBJS))

AOBJS = $(LIBC_OBJS)
LOBJS = $(LIBC_OBJS:.o=.lo)
CFLAGS_ALL = -std=c99 -ffreestanding -nostdinc -D_XOPEN_SOURCE=700 -Os -pipe
libc_shared: libc libohtee.so $(SECURE_FUNCTION_OBJ_DIR) $(SECURE_FUNCTION_OBJS) 
	${CHCORE_COMPILER_DIR}/bin/${CHCORE_COMPILER} --target=aarch64-linux-gnu -fuse-ld=lld $(CFLAGS_ALL) -nostdlib -shared \
	-Wl,-e,_dlstart -o libc_shared.so $(LOBJS) $(LDSO_OBJS) -L${CHCORE_COMPILER_DIR}/lib/clang/current/lib/aarch64-linux-ohos/ -lclang_rt.builtins $(SECURE_FUNCTION_OBJS) $(LIB_OHTEE_OBJS) 
	$(shell mkdir -p ramdisk-dir)
	$(Q)cp libc_shared.so ramdisk-dir


# flags for libs
# shared and static compile flags
override DEFAULT_LIB_CFLAGS := $(DEFAULT_CFLAGS) -fPIC
# shared link flags
override DEFAULT_SHARED_LIB_LDFLAGS := $(DEFAULT_LDFLAGS)  -fPIC -shared

# flags for system servers and apps
# shared and static compile flags
override DEFAULT_USER_CFLAGS := $(DEFAULT_CFLAGS) -fPIE
# dynamic link flags
override DEFAULT_DYNAMIC_USER_LDFLAGS := $(DEFAULT_LDFLAGS) -pie
# static link flags
override DEFAULT_STATIC_USER_LDFLAGS := $(DEFAULT_LDFLAGS) -static

LIB_DIR := $(realpath user/chcore-libs/sys-libs)
SERVER_DIR := $(realpath user/system-services/system-servers)
ROOT_DIR := $(realpath .)

USER_MAKE_PARAM := CC=$(CC) \
				   AR=$(AR) \
				   OBJCOPY=$(OBJCOPY) \
				   OBJDUMP=$(OBJDUMP) \
				   RANLIB=$(RANLIB) \
				   DEFAULT_LIB_CFLAGS="$(DEFAULT_LIB_CFLAGS)" \
				   DEFAULT_SHARED_LIB_LDFLAGS="$(DEFAULT_SHARED_LIB_LDFLAGS)" \
				   DEFAULT_USER_CFLAGS="$(DEFAULT_USER_CFLAGS)" \
				   DEFAULT_DYNAMIC_USER_LDFLAGS="$(DEFAULT_DYNAMIC_USER_LDFLAGS)" \
				   DEFAULT_STATIC_USER_LDFLAGS="$(DEFAULT_STATIC_USER_LDFLAGS)" \
				   LIB_DIR=$(LIB_DIR) \
				   SERVER_DIR=$(SERVER_DIR) \
				   ROOT_DIR=$(ROOT_DIR) \
				   Q=$(Q)

USER_TARGETS := libfs_base.a libohtee.so
USER_TARGETS += tmpfs.srv chanmgr.srv fsm.srv procmgr
USER_TARGET_DIR_MAP := \
			libohtee.so=$(LIB_DIR)/libohtee \
			libfs_base.a=$(SERVER_DIR)/fs_base \
			tmpfs.srv=$(SERVER_DIR)/tmpfs \
			chanmgr.srv=$(SERVER_DIR)/chanmgr \
			fsm.srv=$(SERVER_DIR)/fsm \
			procmgr=$(SERVER_DIR)/procmgr

.PHONY: ramdisk user $(USER_TARGETS) clean

define target_dir
	$(word 2,$(subst =, ,$(filter $(1)=%,$(USER_TARGET_DIR_MAP))))
endef

# declare dependencies betwee libs, system servers and apps
# ramdisk indicates which targets should be put in the ramdisk
ramdisk: libohtee.so chanmgr.srv
	$(shell mkdir -p ramdisk-dir)
	$(Q)cp $(foreach target,$^,$(call target_dir,$(target))/$(target)) ramdisk-dir

tmpfs.srv: ramdisk libfs_base.a libc_shared
procmgr: fsm.srv tmpfs.srv

# make all targets
user: $(USER_TARGETS)
$(USER_TARGETS): libc
	make $(USER_MAKE_PARAM) -C $(call target_dir,$@) $@

# kernel
KERNEL_MAKE_PARAM := PROCMGR=$(SERVER_DIR)/procmgr/procmgr \
					 Q=$(Q)

.PHONY: kernel
kernel: procmgr libc_shared
	$(Q)make $(KERNEL_MAKE_PARAM) -C kernel CC=$(CHCORE_COMPILER_DIR)/bin/$(CHCORE_COMPILER)

# related to clean
clean-user-%:
	make $(USER_MAKE_PARAM) -C $(call target_dir,$*) clean

clean: $(addprefix clean-user-,$(USER_TARGETS))
	$(Q)rm -rf ramdisk-dir
	$(Q)make $(KERNEL_MAKE_PARAM) -C kernel clean
	$(Q)rm -rf $(LIBC_DIR)
	$(Q)rm -rf $(SECURE_FUNCTION_OBJ_DIR)
	$(Q)rm -f libc_shared.so
