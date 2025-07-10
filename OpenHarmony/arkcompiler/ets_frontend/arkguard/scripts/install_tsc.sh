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

if [ ! -d "../arkguard" ]; then
    echo "Error: must run this script in arkguard root directory"
    exit 1
fi

TYPESCRIPT_ROOT_DIR="../../../third_party/typescript"
old_work_dir=$(pwd)

echo "-----------------------------start building typescript-----------------------------"
cd $TYPESCRIPT_ROOT_DIR
package_name=$(npm pack | grep "ohos-typescript")
echo $package_name
tar -xvf $package_name
current_dir=$(pwd)
package_path="$current_dir/package"
echo "-----------------------------end building typescript-----------------------------"

cd $old_work_dir
target_dir=./node_modules/typescript

echo "move $package_path to override $target_dir"

if [ -d "$target_dir" ]; then
    rm -r "$target_dir"
else
    echo "$target_dir does not exist. No need to remove."
fi

mv $package_path $target_dir
echo "------------------------------------finished-------------------------------------"
