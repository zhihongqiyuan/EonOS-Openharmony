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
gn_path="$1"
if [ ! -d "prebuilts/$gn_path" ];then
    mkdir prebuilts/$gn_path
fi
if [ ! -f "prebuilts/$gn_path/gn" ] || [ ! -f "prebuilts/$gn_path/ninja" ];then
    if [ "$gn_path" == "linux" ];then
        curl https://repo.huaweicloud.com/openharmony/compiler/gn/1717/linux/gn-linux-x86-1717.tar.gz  --output gn.tar.gz
        tar -xvf gn.tar.gz --directory=prebuilts/$gn_path/
        curl https://repo.huaweicloud.com/openharmony/compiler/ninja/1.11.0/linux/ninja-linux-x86-1.11.0.tar.gz  --output ninja.tar.gz
        tar -xvf ninja.tar.gz --directory=prebuilts/$gn_path/
    elif [ "$gn_path" == "macx" ];then
        curl https://repo.huaweicloud.com/openharmony/compiler/gn/2024/darwin/gn-darwin-x86-20230425.tar.gz --output gn.tar.gz
        tar -xvf gn.tar.gz --directory=prebuilts/$gn_path/
        curl https://repo.huaweicloud.com/openharmony/compiler/ninja/1.11.0/darwin/ninja-darwin-x86-1.11.0.tar.gz  --output ninja.tar.gz
        tar -xvf ninja.tar.gz --directory=prebuilts/$gn_path/
    elif [ "$gn_path" == "windows" ];then
        curl https://repo.huaweicloud.com/openharmony/compiler/gn/1744/windows/gn-windows-amd64.zip --output gn.zip
        unzip gn.zip -d prebuilts/$gn_path/
        curl https://repo.huaweicloud.com/openharmony/compiler/ninja/1.9.0/windows/ninja-win.zip  --output ninja.zip
        unzip ninja.zip -d prebuilts/$gn_path/
    fi
fi
