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

THIS_FILE_PATH = os.path.dirname(os.path.abspath(__file__))

def untar_file(tar_file_path, extract_path, extract_dir_path):
    try:
        if os.path.exists(extract_path):
            rm_cmd = ['rm', '-rf', extract_path]
            subprocess.run(rm_cmd, check=True)

        tar_cmd = ['tar', '-zxvf', tar_file_path, '-C', extract_dir_path]
        print("tar_cmd:{}".format(tar_cmd))
        subprocess.run(tar_cmd, check=True)

    except Exception as e:
        print("tar error!")
        return


def apply_patch(patch_file, target_dir):
    try:
        if not os.path.exists(target_dir):
            return

        patch_cmd = ['patch', '-p1', "--fuzz=0", "--no-backup-if-mismatch", '-i', patch_file, '-d', target_dir]
        print("patch_cmd:{}".format(patch_cmd))
        subprocess.run(patch_cmd, check=True)

    except Exception as e:
        print("apply_patch error!")
        return


def do_patch(args, target_dir):
    patch_file = []

    for patch in patch_file:
        file_path = os.path.join(args.source_file, patch)
        apply_patch(file_path, target_dir)

def do_copy(source_dir, target_dir):
    try:
        cp_cmd = ["cp", "-rf", source_dir, target_dir]
        print("cp_cmd:{}".format(cp_cmd))
        subprocess.run(cp_cmd, check=True)
    except Exception as e:
        print("copy error!")
        return
def do_remove(target_dir):
    try:
        rm_cmd = ["rm", "-rf", target_dir]
        subprocess.run(rm_cmd, check=True)
    except Exception as e:
        print("remove dir:%s error!" % target_dir)
        return

def main():
    libpng_path = argparse.ArgumentParser()
    libpng_path.add_argument('--gen-dir', help='generate path of jsoncpp')
    libpng_path.add_argument('--source-file', help='jsoncpp source compressed dir')
    args = libpng_path.parse_args()
    tar_file_path = os.path.join(args.source_file, "jsoncpp-1.9.6.tar.gz")
    tmp_dir = os.path.join(THIS_FILE_PATH, "jsoncpp-1.9.6")
    untar_file(tar_file_path, tmp_dir, THIS_FILE_PATH)
    do_patch(args, tmp_dir)
    do_copy(tmp_dir, args.gen_dir)
    do_remove(tmp_dir)
    return 0


if __name__ == '__main__':
    sys.exit(main())
