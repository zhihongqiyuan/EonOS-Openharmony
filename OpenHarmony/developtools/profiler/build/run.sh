#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
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
TOP=$(realpath $DIR/../../..)
SUBSYS=developtools
HOST_OUT=$TOP/hos/out/hos-arm/clang_x64
LIB_PATH=$LIB_PATH:$HOST_OUT/devtools/devtools
LIB_PATH=$LIB_PATH:$HOST_OUT/$SUBSYS/$SUBSYS
LIB_PATH=$LIB_PATH:$HOST_OUT/common/common
LIB_PATH=$LIB_PATH:$LD_LIBRARY_PATH

# remove old coverage data
find $HOST_OUT -name '*.gcda' | xargs rm -v

# run UT executable
echo LD_LIBRARY_PATH=$LIB_PATH $@
LD_LIBRARY_PATH=$LIB_PATH $@
