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
PARAMS=$*
SOURCE="${BASH_SOURCE[0]}"
cd "$(dirname "${SOURCE}")"
. build/build_stanalone_plugins.sh
set_enable_plugin_array "true"
set_enable_extend_plugin_array "false"
set_enable_macro_switch_array "false"
./pare_third_party.sh
choose_os_type
./dl_tools.sh $gn_path
while [[ $# -gt 0 ]]; do
    case "$1" in
        -e)
            enable_plugin "$2"
            shift 2;;
        -d)
            enable_extend_plugin "$2"
            shift 2;;
        -m)
            enable_macro "$2"
            shift 2;;
        -h)
            help $0
            shift;;
        -l)
            list_all_plugins
            shift;;
        *)
            other_params+=("$1")
            shift;;
    esac
done
set -- "${other_params[@]}"
if [ "$#" -ne 0 ] && { [ "$1" == "sdkdemo" ] || [ "$1" == "wasm" ] || [ "$1" == "test" ] || [ "$1" == "fuzz" ]; };then
    prepare_proto $1
fi
prepare_windows $1 $2
target_operator="$2"
if [ "$#" -ne "0" ];then
    if [ "$1" == "wasm" ] || [ "$1" == "sdkdemo" ];then
        if command -v em++ &> /dev/null; then
            use_local_emsdk="true"
        else
            ./dl_emsdk.sh
            use_local_emsdk="false"
        fi
        target="$1"
    fi
    if [ "$1" == "test" ];then
        target="test"
        set_enable_plugin_array "true"
        set_enable_macro_switch_array "true"
    fi
    if [ "$1" == "fuzz" ];then
        target="fuzz"
    fi
    if [ "$1" == "protoc" ];then
        target="protoc"
    fi
    if [ "$1" == "sdkdemotest" ];then
        target="sdkdemotest"
    fi
    if [ "$1" == "spb" ];then
        target="spb"
    fi
fi
if [ "$target" == "wasm" ] && [ "$target_os" == "windows" ];then
    echo "!!!build wasm on winows will occur unknown error, strongly suggest you build wasm on linux(Ubuntu)"
    exit
fi
set_enable_all_plugins_str
. ./build_operator.sh