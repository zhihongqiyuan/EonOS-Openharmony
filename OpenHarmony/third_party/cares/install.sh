#!/bin/bash

# Copyright (c) 2022 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

SRC_DIR="$1"
CODE_DIR="$2"
OPEN_EULER_CARES_SOURCE_PATH="c-ares-1.18.1"
OPEN_EULER_CARES_TAR="c-ares-1.18.1.tar.gz"

set -e
if [ "$SRC_DIR" == "" ] || [ "$CODE_DIR" == "" ]; then
    exit 1
fi

if [ -d "$CODE_DIR" ]; then
    rm -rf "$CODE_DIR"
fi

mkdir -p $CODE_DIR

tar zxvf $SRC_DIR/$OPEN_EULER_CARES_TAR -C $CODE_DIR

_all_patchs=(
    "0000-Use-RPM-compiler-options.patch"
    "backport-disable-live-tests.patch"
    "backport-add-str-len-check-in-config_sortlist-to-avoid-stack-overflow.patch"
    "backport-CVE-2023-32067.patch"
    "backport-001-CVE-2023-31130.patch"
    "backport-002-CVE-2023-31130.patch"
    "backport-003-CVE-2023-31130.patch"
    "backport-001-CVE-2023-31147.patch"
    "backport-002-CVE-2023-31124_CVE-2023-31147.patch"
    "backport-003-CVE-2023-31147.patch"
    "backport-004-CVE-2023-31147.patch"
    "backport-005-CVE-2023-31147.patch"
    "backport-CVE-2023-31124.patch"
    "backport-CVE-2024-25629.patch"
    "0001-ADD-OHOS-DNS-PROXY-BY-NETSYS.patch"
    "0002-ADD-OHOS-IPV6-PROXY-BY-NETSYS.patch"
)
for filename in "${_all_patchs[@]}"
  do
    patch -d $CODE_DIR/$OPEN_EULER_CARES_SOURCE_PATH -p1 < $SRC_DIR/$filename --fuzz=0 --no-backup-if-mismatch
  done
exit 0
