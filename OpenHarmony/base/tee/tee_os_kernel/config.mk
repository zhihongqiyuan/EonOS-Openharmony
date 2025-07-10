# Copyright (c) 2023 Institute of Parallel And Distributed Systems (IPADS), Shanghai Jiao Tong University (SJTU)
# Licensed under the Mulan PSL v2.
# You can use this software according to the terms and conditions of the Mulan PSL v2.
# You may obtain a copy of Mulan PSL v2 at:
#     http://license.coscl.org.cn/MulanPSL2
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
# PURPOSE.
# See the Mulan PSL v2 for more details.

# string config
STR_CONFIGS = \
CHCORE_COMPILER=clang \
CHCORE_CROSS_COMPILE=aarch64-linux-ohos- \
CHCORE_PLAT=rk3568 \
CHCORE_ARCH=aarch64 \
CHCORE_SPD=opteed

# bool config
BOOL_CONFIGS = \
CHCORE_VERBOSE_BUILD=OFF \
CHCORE_ENABLE_FMAP=ON \
CHCORE_USER_DEBUG=OFF \
CHCORE_MINI=ON \
CHCORE_OH_TEE=ON \
CHCORE_KERNEL_DEBUG=OFF \
CHCORE_KERNEL_TEST=OFF \
CHCORE_KERNEL_RT=OFF


# set the config in Makefile
$(foreach CONFIG,$(STR_CONFIGS) $(BOOL_CONFIGS), \
		$(eval KEY = $(word 1,$(subst =, ,$(CONFIG)))) \
		$(eval VALUE = $(word 2,$(subst =, ,$(CONFIG)))) \
		$(eval $(KEY)=$(VALUE)) \
		)

# set CONFIG_FLAGS
$(foreach CONFIG,$(STR_CONFIGS), \
		$(eval CONFIG_FLAGS += -D$(CONFIG)) \
		)
$(foreach CONFIG,$(BOOL_CONFIGS), \
		$(eval KEY = $(word 1,$(subst =, ,$(CONFIG)))) \
		$(eval VALUE = $(word 2,$(subst =, ,$(CONFIG)))) \
		$(eval CONFIG_FLAGS += $(if $(filter ON,$(VALUE)),-D$(KEY),)) \
		)
CONFIG_FLAGS += -DCHCORE_ARCH_$(shell echo $(CHCORE_ARCH) | tr '[:lower:]' '[:upper:]')
CONFIG_FLAGS += -DCHCORE_PLAT_$(shell echo $(CHCORE_PLAT) | tr '[:lower:]' '[:upper:]')
CONFIG_FLAGS += -DCHCORE_SPD_$(shell echo $(CHCORE_SPD) | tr '[:lower:]' '[:upper:]')
CONFIG_FLAGS += -DCHCORE
