#!/bin/bash
# Copyright (c) 2021 Huawei Device Co., Ltd.
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

echo "use protoc to do the prebuild (gn unable set LD_LIBRARY_PATH)"

echo "pwd $(pwd)"

MYDIR=$(dirname $0)
BUILD_TOP=$MYDIR/../../../
echo MYDIR $MYDIR
echo BUILD_TOP $BUILD_TOP

protoc_cmdline=$*
echo protoc_cmdline $protoc_cmdline

# search one by bone
PREBUILD_HOST_CLANG_RUNTIME=$BUILD_TOP/prebuilts/clang/host/linux-x86/clang-r353983c/lib64
PREBUILD_OHOS_CLANG_RUNTIME=$BUILD_TOP/prebuilts/clang/ohos/linux-x86_64/llvm/lib
export LD_LIBRARY_PATH=$PREBUILD_HOST_CLANG_RUNTIME:$PREBUILD_OHOS_CLANG_RUNTIME:$LD_LIBRARY_PATH

PROTO_PATH="../../../binarys/third_party/protobuf/innerapis/protoc/clang_x64/libs/protoc"
echo PROTO_PATH $PROTO_PATH
if [ -e "$PROTO_PATH" ]; then
    if [[ $(pwd) == *src_test* ]]; then
        cmd="$PROTO_PATH --proto_path ../../../developtools/hiperf/proto --cpp_out ../../../out/default/src_test/gen/developtools/hiperf/proto ../../../developtools/hiperf/proto/report_sample.proto"
    elif [[ $(pwd) == *test* ]]; then
        cmd="$PROTO_PATH --proto_path ../../../developtools/hiperf/proto --cpp_out ../../../out/default/test/gen/developtools/hiperf/proto ../../../developtools/hiperf/proto/report_sample.proto"
    else
        cmd="$PROTO_PATH --proto_path ../../../developtools/hiperf/proto --cpp_out ../../../out/default/src/gen/developtools/hiperf/proto ../../../developtools/hiperf/proto/report_sample.proto"
    fi
    echo $cmd
    $cmd
else
    cmd="$protoc_cmdline"
    echo $cmd
    $cmd
fi
