#!/bin/bash
# Copyright (c) 2024 Huawei Device Co., Ltd.
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

# Install typescript from third_party/typescript
# Must run this script in arkguard root directory.

set -e

if [ ! -d "../compiler" ]; then
    echo "Error: must run this script in ace_ets2bundle/compier root directory"
    exit 1
fi

TYPESCRIPT_ROOT_DIR="../../../third_party/typescript"
ARKGUARD_ROOT_DIR="../../../arkcompiler/ets_frontend/arkguard"
old_work_dir=$(pwd)

# Step 1: build arkguard
echo "-----------------------------start building arkguard-----------------------------"
cd $ARKGUARD_ROOT_DIR
npm install
./scripts/install_tsc.sh
npm run build
package_name=$(npm pack | grep "arkguard-1")
echo $package_name
tar -xvf $package_name
current_dir=$(pwd)
package_path="$current_dir/package"
echo "-----------------------------end building arkguard-----------------------------"

cd $old_work_dir
target_dir=./node_modules/arkguard

echo "move $package_path to override $target_dir"
if [ -d "$target_dir" ]; then
    rm -r $target_dir
fi

mv $package_path $target_dir

# Step 2: build typescript
echo "-----------------------------start building typescript-----------------------------"
cd $TYPESCRIPT_ROOT_DIR
npm install
npm run build
package_name=$(npm pack | grep "ohos-typescript")
echo $package_name
tar -xvf $package_name
current_dir=$(pwd)
tsc_package_path="$current_dir/package"
echo "-----------------------------end building typescript-----------------------------"

cd $old_work_dir
target_dir=./node_modules/typescript

echo "move $tsc_package_path to override $target_dir"
if [ -d "$target_dir" ]; then
    rm -r $target_dir
fi

mv $tsc_package_path $target_dir
