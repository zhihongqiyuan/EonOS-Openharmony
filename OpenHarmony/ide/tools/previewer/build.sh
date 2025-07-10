#!/bin/bash
# Copyright (c) 2023 Huawei Device Co., Ltd.
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
BIN_PATH=$(dirname $0)
COMPLIER_PATH_USE=""
#read config.
while read line;do
    eval "$line"
done < config.cfg

#clean
if [ -d "build" ]; then
  rm -rf build
fi
if [ -d "output" ]; then
  rm -rf output
fi

#build
if [ "$1" == "clean" ]; then
  echo "clean finished"
else
  echo "build start"
  mkdir build 
  mkdir output
  cd build
  if [ -f  "$COMPLIER_PATH_LOCAL"/bin/clang ]; then
    COMPLIER_PATH_USE=$(dirname $(readlink -f "$COMPLIER_PATH_LOCAL"/clang-mingw))
  fi
  if [ -f "$COMPLIER_PATH"/bin/clang ]; then
    COMPLIER_PATH_USE=$COMPLIER_PATH
  fi
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../output .. -DCMAKE_TOOLCHAIN_FILE=$BIN_PATH/cmake/mingw_clang_noerror.cmake -DTOOLCHAIN_PATH="$COMPLIER_PATH_USE" -DENGINE_TYPE="$1"
  cpu_processor_num=$(grep processor /proc/cpuinfo | wc -l)
  job_num=$(expr "$cpu_processor_num" \* 2)
  echo Parallel job num is "$job_num"
  make -j"$job_num"
  if [ "$1" == "THIN" ]
  then
    mv Simulator.exe ../output
  elif [ "$1" == "RICH" ]
  then
    mv Previewer.exe ../output
  fi
  echo "build finished"
fi
