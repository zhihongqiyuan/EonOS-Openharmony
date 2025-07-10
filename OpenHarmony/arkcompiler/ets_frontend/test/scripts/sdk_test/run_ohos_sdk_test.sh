#!/bin/bash


# Copyright (c) 2024 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Description: output test results

set -e

function usage() {
    echo "Usage: $0 [--mode <debug|release>]"
    exit 1
}

modes=("debug" "release")
clean_command="hvigorw clean"
debug_compile_command="hvigorw assembleHap"
release_compile_command="hvigorw assembleApp"

while [[ $# -gt 0 ]]; do
    case "$1" in
        --mode)
            modes=("$2")
            shift 2
            ;;
        *)
            usage
            ;;
    esac
done

file="./entry/src/main/ets/pages/Index.ets"
if [ ! -f "$file" ]; then
    echo "Error: File $file does not exist in the current directory."
    exit 1
fi

function compile_and_collect_result() {
    local compile_command=$1
    compile_output=$($compile_command 2>&1)
    if [ $? -eq 0 ]; then
        echo "Pass"
    else
        echo "Fail"
    fi
}

function compile_and_display() {
    local mode=$1
    local compile_command=$2

    $clean_command
    full_result=$(compile_and_collect_result "$compile_command")

    echo "\nconsole.log('$mode')" >> "$file"

    increment_result=$(compile_and_collect_result "$compile_command")

    printf "%-20s %-10s\n" "cases" "result ($mode)"
    printf "%-20s %-10s\n" "full build" "$full_result"
    printf "%-20s %-10s\n" "incremental build" "$increment_result"
}

function get_compile_command() {
    local mode=$1
    if [ "$mode" == "debug" ]; then
        echo $debug_compile_command
    elif [ "$mode" == "release" ]; then
        echo $release_compile_command
    else
        echo ""
    fi
}

for mode in "${modes[@]}"; do
    compile_command=$(get_compile_command "$mode")
    if [ -n "$compile_command" ]; then
        compile_and_display "$mode" "$compile_command"
    else
        echo "Invalid mode. Use 'debug' or 'release'."
        usage
    fi
done
