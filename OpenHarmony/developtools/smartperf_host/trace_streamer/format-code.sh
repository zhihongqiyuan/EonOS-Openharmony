#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
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
set -e
target_os="linux"
gn="gn"
case "$OSTYPE" in
  solaris*) echo "SOLARIS" ;;
  darwin*)  target_os="macx" ;;
  linux*)   target_os="linux"  ;;
  bsd*)     echo "is bsd os" ;;
  msys*)    target_os="windows" gn="gn.exe" ;;
  *)        echo "unknown: $OSTYPE" ;;
esac
PRJ_ROOT_DIR=$(readlink -f -- "$(dirname $0)/")
cd ${PRJ_ROOT_DIR}
FORMAT_DIR_LIST=(
    "${PRJ_ROOT_DIR}/build"
    "${PRJ_ROOT_DIR}/gn"
    "${PRJ_ROOT_DIR}/sdk"
    "${PRJ_ROOT_DIR}/src"
    "${PRJ_ROOT_DIR}/test"
    "${PRJ_ROOT_DIR}/trace_extend/src"
    "${PRJ_ROOT_DIR}/trace_extend/test"
    "${PRJ_ROOT_DIR}/prebuilts/fuzz"
    "${PRJ_ROOT_DIR}/prebuilts/linux"
    "${PRJ_ROOT_DIR}/prebuilts/macx"
    "${PRJ_ROOT_DIR}/prebuilts/patch_bounds_checking_function"
    "${PRJ_ROOT_DIR}/prebuilts/patch_googletest"
    "${PRJ_ROOT_DIR}/prebuilts/patch_hiperf"
    "${PRJ_ROOT_DIR}/prebuilts/patch_libunwind"
    "${PRJ_ROOT_DIR}/prebuilts/patch_perf_event"
    "${PRJ_ROOT_DIR}/prebuilts/patch_protobuf"
    "${PRJ_ROOT_DIR}/prebuilts/patch_sqlite"
)
echo "formatting code ..."
for d in ${FORMAT_DIR_LIST[@]}; do
    for f in $(find $d -type f -not -name '*sql.c' -regex '.*\.\(cpp\|hpp\|c\|h\)'); do
        dos2unix $f
        chmod -x $f
        clang-format --verbose -i $f
    done
    for f in $(find $d -type f -not -name '*sql.c' -regex '.*\.\(gn\|gni\)'); do
        ./prebuilts/$target_os/gn format $f
    done
done
echo "formatting code over"
