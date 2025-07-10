#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
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

THIS_DIR=$(dirname ${BASH_SOURCE[0]})
PROJECT_TOP=$(realpath $THIS_DIR/../../..)

OHOS_X64_OUT=$PROJECT_TOP/$2/
LIBCXX_X64_OUT=$PROJECT_TOP/$1/ndk/libcxx/linux_x86_64
SUBSYS_X64_OUT=$PROJECT_TOP/$2/developtools/hiprofiler

NUM_ARGS=$#
# 获取最后一个参数的索引
LAST_ARG_INDEX=$(($NUM_ARGS))
# 获取最后一个参数
INDEP_COMPLIER=${!LAST_ARG_INDEX}
if [[ "$INDEP_COMPLIER" == true ]]; then
  PROTOC=$PROJECT_TOP/binarys/third_party/protobuf/innerapis/protoc/clang_x64/libs/protoc
else
  PROTOC=$PROJECT_TOP/$2/thirdparty/protobuf/protoc
fi

OPT_PLUGIN_PATH=$PROJECT_TOP/$2/developtools/hiprofiler/protoencoder_plugin
OPT_PLUGIN=--plugin=protoc-gen-opt=$PROJECT_TOP/$2/developtools/hiprofiler/protoencoder_plugin
OPT_OUT=--opt_out

PYTHON_SHELL=$THIS_DIR/make_proto.py # shell path
TMP=$2
PROTO_OUT_DIR="$PROJECT_TOP/${TMP%/*}/$3" # path of the new proto file
echo "1 = $1"
echo "2 = $2"
PARAMS=$*
PARAMS_FILTER="$1 $2 $3"
# for python : copy and change source .proto file
TARFLAG="--proto_path"
PARAMS_SOURCE_TMP="${PARAMS#*${TARFLAG}}" # get source proto file path
IFS=' ' read -ra ADDR <<< "$PARAMS_SOURCE_TMP"
PARAMS_SOURCE_TMP_JOIN=""
for (( i=0; i<${#ADDR[@]} -1 ; i++ ))
do
    PARAMS_SOURCE_TMP_JOIN="${PARAMS_SOURCE_TMP_JOIN} ${ADDR[$i]}"
done
PARAMS_SOURCE_TMP="${PARAMS_SOURCE_TMP_JOIN}"
PARAMS_SOURCES=""
NUM=0
for ITM in ${PARAMS_SOURCE_TMP[@]} # filter the TARGLAG
do
  if [ $NUM -gt 0 ]; then
    PARAMS_SOURCES="$PARAMS_SOURCES$ITM "
  fi
  NUM=$[$NUM + 1]
done

PARAMS_PRINT="$PROTO_OUT_DIR $PARAMS_SOURCES" # splice the python args
echo "EXEC: python $PYTHON_SHELL $PARAMS_PRINT"
python $PYTHON_SHELL $PARAMS_PRINT
# end python

# creat pb file
PARAMS_SRC=${PARAMS:${#PARAMS_FILTER}}
IFS=' ' read -ra PARAMS_ARR <<< "$PARAMS_SRC"
PARAMS_SRC_JOIN=""
for (( i=0; i<${#PARAMS_ARR[@]} -1 ; i++ ))
do
    PARAMS_SRC_JOIN="${PARAMS_SRC_JOIN} ${PARAMS_ARR[$i]}"
done
PARAMS_SRC="${PARAMS_SRC_JOIN}"

PARAMS_REPLACE=${PARAMS_SOURCES//.proto/_standard.proto} # make the new proto file name. like "_standard.proto"
PARAMS_STANDARD=" --proto_path $PROTO_OUT_DIR " # add proto_path，$PROTO_OUT_DIR is the pb file path
for VAR in ${PARAMS_REPLACE[@]}
do
  PARAMS_STANDARD="$PARAMS_STANDARD$PROTO_OUT_DIR/${VAR##*/} " # add .proto file name to args
done
PARAMS_ALL="$PARAMS_SRC $PARAMS_STANDARD" # add new argument list to old argument list


PARAMS_LITE_REPLACE=${PARAMS_SOURCES//.proto/_lite.proto} # make the new proto file name. like "_lite.proto"
PARAMS_LITE=" --proto_path $PROTO_OUT_DIR " # add proto_path，$PROTO_OUT_DIR is the pb file path

for VAR in ${PARAMS_LITE_REPLACE[@]}
do
  PARAMS_LITE="$PARAMS_LITE$PROTO_OUT_DIR/${VAR##*/} " # add .proto file name to args
done
PARAMS_ALL="$PARAMS_SRC $PARAMS_STANDARD $PARAMS_LITE" # add new argument list to old argument list

# avoid conflict, param4=--plugin* means ipc plugin, generate encode file if opt plugin exist
if [[ "$4" != --plugin* ]]; then
  if [ -f "$OPT_PLUGIN_PATH" ]; then
    echo "generate protobuf optimize code OPT_PLUGIN = $OPT_PLUGIN"
    echo "optimize EXEC: LD_LIBRARY_PATH=$LIBCXX_X64_OUT:$SUBSYS_X64_OUT $PROTOC $OPT_PLUGIN $OPT_OUT $5 $PARAMS_ALL"
    LD_LIBRARY_PATH=$LIBCXX_X64_OUT:$SUBSYS_X64_OUT exec $PROTOC $OPT_PLUGIN $OPT_OUT $5 $PARAMS_ALL
  fi
fi

echo "EXEC: LD_LIBRARY_PATH=$LIBCXX_X64_OUT:$SUBSYS_X64_OUT $PROTOC $PARAMS_ALL"
LD_LIBRARY_PATH=$LIBCXX_X64_OUT:$SUBSYS_X64_OUT exec $PROTOC $PARAMS_ALL
