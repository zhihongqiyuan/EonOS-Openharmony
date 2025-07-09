#!/bin/bash
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation version 2.1
# of the License.
#
# Copyright(c) 2023 Huawei Device Co., Ltd.

set -e
cd $1
if [ -d "re2" ];then
    rm -rf re2
fi
tar zxvf re2-2024-07-02.tar.gz
mv re2-2024-07-02 re2
exit 0