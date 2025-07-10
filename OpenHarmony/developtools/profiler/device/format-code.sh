#!/bin/bash
#
# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set -e

DIR=$(dirname $(realpath ${BASH_SOURCE[0]}))
TOP=$(realpath $DIR/../../..)
CLANG_DIR=$TOP/prebuilts/clang/ohos/linux-x86_64/llvm/bin
GN_DIR=$TOP/prebuilts/build-tools/linux-x86/bin

export PATH=$CLANG_DIR:$GN_DIR:$PATH
echo "formatting C/C++ code ..."
find $@ -name '*.h' -o -name '*.cpp' | xargs dos2unix
find $@ -name '*.h' -o -name '*.cpp' | xargs chmod -x
find $@ -name '*.h' -o -name '*.cpp' | xargs clang-format --verbose -i

echo "formatting GN code ..."
find $@ -name '*.gn' -o -name '*.gni' | xargs gn format
