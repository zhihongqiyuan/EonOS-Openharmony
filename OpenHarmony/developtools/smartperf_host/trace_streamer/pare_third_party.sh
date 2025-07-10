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
patch='patch'
sed='sed'
cp='cp'
rm='rm'
root_dir=$(pwd)
echo "build root dir is $root_dir"

git config --global core.longpaths true
case "$OSTYPE" in
  darwin*)  sed="gsed" ;;
esac
if [ ! -d "third_party" ];then
    mkdir third_party
fi
cd third_party

if [ ! -f "sqlite/BUILD.gn" ];then
    rm -rf sqlite
    git clone  git@gitee.com:openharmony/third_party_sqlite.git
    if [ -d "third_party_sqlite" ];then
        mv third_party_sqlite sqlite
        cd sqlite
        git reset --hard d21e412dbc6f2cdde2e4c9828e2450fcfca4fbe9
        cd ..
        $cp ../prebuilts/patch_sqlite/sqlite3build.gn ../third_party/sqlite/BUILD.gn
    fi
fi
if [ ! -f "protobuf/BUILD.gn" ];then
    rm -rf protobuf
    git clone  git@gitee.com:openharmony/third_party_protobuf.git
    if [ -d "third_party_protobuf" ];then
        mv third_party_protobuf protobuf
        cd protobuf
        git reset --hard aceafed4cf26d7a6be8169ae887cc13b749d5515
        cd ..
        $cp ../prebuilts/patch_protobuf/protobufbuild.gn ../third_party/protobuf/BUILD.gn
    fi
fi

if [ ! -f "zlib/BUILD.gn" ];then
    git clone --depth=1 git@gitee.com:openharmony/third_party_zlib.git zlib
fi

if [ ! -f "bzip2/BUILD.gn" ];then
    git clone --depth=1 git@gitee.com:openharmony/third_party_bzip2.git bzip2
    cd bzip2
    ./install.sh $(pwd)
    cd ..
fi

if [ ! -f "googletest/BUILD.gn" ];then
    rm -rf googletest
    git clone --depth=1 git@gitee.com:openharmony/third_party_googletest.git
    if [ -d "third_party_googletest" ];then
        mv third_party_googletest googletest
        $cp ../prebuilts/patch_googletest/googletestbuild.gn ../third_party/googletest/BUILD.gn
        $patch -p1 < ../prebuilts/patch_googletest/gtest.patch
    fi
fi

if [ ! -f "json/BUILD.gn" ];then
    rm -rf json
    git clone --depth=1 git@gitee.com:openharmony/third_party_json.git
    if [ -d "third_party_json" ];then
        mv third_party_json json
    fi
fi

if [ ! -d "libbpf" ];then
    git clone --depth=1 git@gitee.com:openharmony/third_party_libbpf.git libbpf
fi

if [ ! -d "hiviewdfx/faultloggerd" ];then
   git clone --depth=1 git@gitee.com:openharmony/hiviewdfx_faultloggerd.git hiviewdfx/faultloggerd/
   cd hiviewdfx/faultloggerd
   $patch -p1  < ../../../prebuilts/patch_hiperf/hiviewdfx_faultloggerd.patch
   cd ../../
fi

if [ ! -f "hiperf/BUILD.gn" ];then
    git clone --depth=1 -b adjust_build_for_smartperf_host git@gitee.com:li-yiting880505/developtools_hiperf_1.git hiperf
fi

if [ ! -f "bounds_checking_function/BUILD.gn" ];then
    git clone --depth=1 git@gitee.com:openharmony/third_party_bounds_checking_function.git bounds_checking_function
fi

if [ ! -d "commonlibrary/c_utils" ];then
    git clone --depth=1 git@gitee.com:openharmony/commonlibrary_c_utils.git commonlibrary/c_utils
fi

if [ ! -f "profiler/device/plugins/ftrace_plugin/include/ftrace_common_type.h" ];then
    rm -rf profiler
    git clone --depth=1 git@gitee.com:openharmony/developtools_profiler.git
    if [ -d "developtools_profiler" ];then
        mkdir -p profiler/device/plugins/ftrace_plugin/include
        $cp developtools_profiler/device/plugins/ftrace_plugin/include/ftrace_common_type.h profiler/device/plugins/ftrace_plugin/include
        $cp developtools_profiler/device/plugins/ftrace_plugin/include/ftrace_namespace.h profiler/device/plugins/ftrace_plugin/include
        rm -rf developtools_profiler
    fi
fi

if [ ! -d "llvm-project" ];then
    rm -rf llvm-project
    git clone --depth=1 git@gitee.com:openharmony/third_party_llvm-project.git
    if [ -d "third_party_llvm-project" ];then
        mv third_party_llvm-project llvm-project
        cd llvm-project
        $patch -p1 < ../../prebuilts/patch_llvm/llvm.patch
        cd $root_dir
        # Symbolic link llvm sub folder of llvm-project source code to build root.
        # REF: https://gitee.com/openharmony/third_party_llvm-project/blob/master/llvm/utils/gn/.gn
        ln -s $root_dir/third_party/llvm-project/llvm llvm
    fi
fi
