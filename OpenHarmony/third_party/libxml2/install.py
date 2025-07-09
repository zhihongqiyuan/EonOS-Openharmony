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


def untar_file(tar_file_path, extract_path, args):
    try:
        if os.path.exists(extract_path):
            rm_cmd = ['rm', '-rf', extract_path]
            subprocess.run(rm_cmd, check=True)

        tar_cmd = ['tar', '-xvf', tar_file_path, '-C', args.gen_dir]
        subprocess.run(tar_cmd, check=True)

    except Exception as e:
        print("tar error!")
        return


def apply_patch(patch_file, target_dir):
    try:
        if not os.path.exists(target_dir):
            return

        patch_cmd = ['patch', '-p1', "--fuzz=0", "--no-backup-if-mismatch", '-i', patch_file, '-d', target_dir]
        subprocess.run(patch_cmd, check=True)

    except Exception as e:
        print("apply_patch error!")
        return


def do_patch(args, target_dir):
    patch_file = [
        "Backport-CVE-2025-32414-python-Read-at-most-len-4-ch-c.patch",
        "Backport-CVE-2025-32415-schemas-Fix-heap-buffer-over-c.patch",
        "Fix_XML_PARSE_NOBLANKS_dropping_non-whitespace_text.patch"
    ]

    for patch in patch_file:
        file_path = os.path.join(args.source_file, patch)
        apply_patch(file_path, target_dir)


def main():
    libpng_path = argparse.ArgumentParser()
    libpng_path.add_argument('--gen-dir', help='generate path of libxml2')
    libpng_path.add_argument('--source-file', help='libxml2 source compressed dir')
    args = libpng_path.parse_args()
    tar_file_path = os.path.join(args.source_file, "libxml2-2.14.0.tar.xz")
    target_dir = os.path.join(args.gen_dir, "libxml2-2.14.0")
    untar_file(tar_file_path, target_dir, args)
    do_patch(args, target_dir)
    return 0


if __name__ == '__main__':
    sys.exit(main())
