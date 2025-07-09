#!/bin/bash
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation version 2.1
# of the License.
#
# Copyright(c) 2023 Huawei Device Co., Ltd.

set -e
set -x
mkdir -p $1
cd $1
touch test.lock
(
    flock -x 200
if [ -d "e2fsprogs" ];then
    rm -rf e2fsprogs
fi
tar xvf $2/e2fsprogs.tar.xz
cd e2fsprogs
patch -p1 < $2/1001-image-make.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $2/1002-add-header-file-to-musl-compile-mk2efs.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $2/1003-add-dac-config.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $2/1004-modify-code-to-compile.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $2/1005-read-vfat-chinese-label.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $2/1006-add-hmfs-for-blkid.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $2/1007-blkid-support-skip-specified-filesystem.patch --fuzz=0 --no-backup-if-mismatch
exit 0
)200>test.lock
