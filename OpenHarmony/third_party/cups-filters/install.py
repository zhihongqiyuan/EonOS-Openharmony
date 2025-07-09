#!/usr/bin/env python
# -*- coding: utf-8 -*-
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

import argparse
import os
import subprocess
import sys
import shutil

def copy_file(dir, dest):
    src_name = '/cupsfilters.convs.in'
    dest_name = '/cupsfilters.convs'
    src_file = dir + src_name
    dest_file = dest + dest_name
    shutil.copy2(src_file, dest_file)

def apply_patch(patch_file, target_dir):
    try:
        if not os.path.exists(target_dir):
            return
        patch_cmd = ['patch', '-p1', "--fuzz=0", "--no-backup-if-mismatch", '-i', patch_file, '-d', target_dir]
        subprocess.run(patch_cmd, check=True)
    except Exception as e:
        print("apply_patch error!")
        return


def do_patch(target_dir):
    patch_file = [
        "ohos_pdftoraster.patch",
        "ohos_ghostscript.patch",
        "ohos_pdftopdf.patch",
        "backport-CVE-2023-24805.patch",
    ]

    for patch in patch_file:
        apply_patch(patch, target_dir)


def main():
    path = argparse.ArgumentParser()
    path.add_argument('--gen-dir', help='generate path of log', required=True)
    path.add_argument('--source-dir', help='generate path of log', required=True)
    args = path.parse_args()
    convs_dir = os.path.join(args.source_dir, "mime")

    do_patch(args.source_dir)
    copy_file(convs_dir, args.gen_dir)
    return 0

if __name__ == '__main__':
    sys.exit(main())
