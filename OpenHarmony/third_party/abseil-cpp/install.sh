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
if [ -d "abseil-cpp" ];then
    rm -rf abseil-cpp
fi
tar zxvf $2/abseil-cpp-20250127.0.tar.gz
mv abseil-cpp-20250127.0 abseil-cpp
cd abseil-cpp
flock -u 100
} 100<>lock_file.lock
exit 0