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
DIR=$(dirname $(realpath ${BASH_SOURCE[0]}))
TOP=$(realpath $DIR/../../../)
echo $DIR
cd $DIR

if [ -z "$1" ]; then
    echo "no product name"
    exit -1
fi

BUILD_OUT=$TOP/out/$1
HOST_OUT=$TOP/out/host/developtools/hiperf
BUILD_PATH=/developtools/hiperf
if [ -d "$HOST_OUT" ]; then
    echo "remove $HOST_OUT"
    rm -rf $HOST_OUT
fi
mkdir -p $HOST_OUT

# script
cp -vf *.py $HOST_OUT
cp -vf *.html $HOST_OUT

# bin
HOST_LINUX_PATH=$BUILD_OUT/clang_x64$BUILD_PATH
HOST_LINUX_OUT_PATH=$HOST_OUT/bin/linux/x86_64
mkdir -p $HOST_LINUX_OUT_PATH

HOST_WINDOWS_PATH=$BUILD_OUT/mingw_x86_64$BUILD_PATH
HOST_WINDOWS_OUT_PATH=$HOST_OUT/bin/windows/x86_64
mkdir -p $HOST_WINDOWS_OUT_PATH

OHOS_ARM_PATH=$BUILD_OUT$BUILD_PATH
OHOS_ARM_OUT_PATH=$HOST_OUT/bin/ohos/arm
mkdir -p $OHOS_ARM_OUT_PATH

cp -vf $HOST_LINUX_PATH/hiperf_host  $HOST_LINUX_OUT_PATH
cp -vf $HOST_LINUX_PATH/*.so  $HOST_LINUX_OUT_PATH

cp -vf $HOST_WINDOWS_PATH/hiperf_host.exe  $HOST_WINDOWS_OUT_PATH
cp -vf $HOST_WINDOWS_PATH/*.dll  $HOST_WINDOWS_OUT_PATH

cp -vf $OHOS_ARM_PATH/hiperf $OHOS_ARM_OUT_PATH

cd $HOST_OUT/../
rm -f hiperf.tar.gz
tar czvf hiperf.tar.gz hiperf
cd -
