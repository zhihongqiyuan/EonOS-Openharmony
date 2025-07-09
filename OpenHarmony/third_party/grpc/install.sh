#!/bin/bash
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation version 2.1
# of the License.
#
# Copyright(c) 2023 Huawei Device Co., Ltd.

set -e
cd $1
{
flock -x 100
if [ -d "grpc" ];then
    rm -rf grpc
fi
tar -zxvf grpc-1.70.1.tar.gz
mv grpc-1.70.1 grpc
cd $1/grpc
flock -u 100
} 100<>$1/lock_file.lock
exit 0