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
echo "target_operator = $target_operator"
if [ "$target" != "trace" ] && [ "$target" != "linux" ] && [ "$target" != "windows" ] &&
    [ "$target" != "macx" ] && [ "$target" != "trace_streamer" ] && [ "$target" != "wasm" ] &&
    [ "$target" != "test" ] && [ "$target" != "spb" ] && [ "$target" != "fuzz" ] &&
    [ "$target" != "protoc" ] && [ "$target" != "sdkdemo" ] && [ "$target" != "sdkdemotest" ];then
    echo "failed"
    exit
fi
if [ "$target_operator" != "" ] && [ "$target_operator" != "debug" ] && [ "$target_operator" != "release" ] && [ "$target_operator" != "clean" ];then
    if [ "$target_operator" == "protoc" ];then
    target=$target_operator
    else
    echo "failed"
    exit
    fi
fi
if [ "$target_operator" == "debug" ];then
    is_debug="true"
elif [ "$target_operator" == "clean" ];then
    is_clean="true"
else
    is_debug="false"
fi
echo "platform is $target_os"
echo "isdebug: $is_debug"
echo "isclean: $is_clean"
if [ "$is_debug" != "false" ];then
    ext="_debug"
fi

if [ "$target" == "test" ] || [ "$target" == "fuzz" ] || [ "$target" == "wasm" ] || [ "$target" == "sdkdemo" ] || [ "$target" == "sdkdemotest" ];then
    target_dir=$target
else
    target_dir=$target_os
fi
if [ "$target" == "trace_streamer" ] || [ "$target" == "trace" ] || [ "$target" == "spb" ] || [ "$target" == "protoc" ];then
    target_dir=$target_os
fi
echo "target_dir:" "$target_dir"
echo "target:" "$target"

out_dir=out/$target_dir$ext
if [ "$is_clean" == "true" ];then
    prebuilts/"$gn_path"/"$gn" gen "$out_dir" --clean
    prebuilts/"$gn_path"/"$ninja" -C "$out_dir" -t clean
else
    prebuilts/"$gn_path"/"$gn" gen "$out_dir" --args='is_debug='"$is_debug"' target="'"$target"'" target_os="'"$target_os"'" is_independent_compile=true'' use_local_emsdk='"$use_local_emsdk"" $enable_all_plugins_str"
    echo "begin to build ..."
    prebuilts/"$gn_path"/"$ninja" -C "$out_dir"
fi
if [ "$out_dir" == "out/macx" ];then
    ./mac_depend.sh
fi