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
# 使用示例 ./perf.sh ~/SourceData/ftrace_small.txt ~/SourceData/3.svg
set -e
# 导出火焰图
if [ "$#" -ne "2" ];then
    echo "使用示例 ./huoyantu.sh ~/SourceData/ftrace_small.txt ~/SourceData/3.svg"
    exit
fi
if [ ! -d "../FlameGraph" ];then
    cd ..
    git clone https://github.com/brendangregg/FlameGraph.git
    cd -
fi
if [ ! -f "./out/linux_debug/trace_streamer" ];then
    echo "need out/linux_debug/trace_streamer file"
    exit
fi
source=$1
dst=$2
sudo perf record -g -F 99 -- ./out/linux_debug/trace_streamer $source -e /tmp/1.db
sudo perf script -i perf.data > out.perf
../FlameGraph/stackcollapse-perf.pl out.perf > out.floded
../FlameGraph/flamegraph.pl out.floded > $dst