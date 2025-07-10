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
#./build.sh test clean
set -e
./build.sh test $1 $2
rm -rf out/test/*.xml
rm -rf out/test_debug/*.xml
find out/test -name "*.gcda" -print0 | xargs -0 rm -rf
find out/test_debug -name "*.gcda" -print0 | xargs -0 rm -rf
mkdir -p out/test/data/resource
mkdir -p out/test_debug/data/resource
cp test/resource/* out/test/data/resource/
cp test/resource/* out/test_debug/data/resource/
cd out/test
./trace_streamer_ut
