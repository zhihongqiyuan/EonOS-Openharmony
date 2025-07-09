#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright 2024 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import sys
import shutil
import argparse
import subprocess


def untar_file(tar_file_path, extract_path):
    try:
        tar_cmd = ['tar', '-zxf', tar_file_path, '-C', extract_path]
        subprocess.run(tar_cmd, check=True)
    except Exception as e:
        print("tar error!")
        return


def clear_dir(old_dir):
    if os.path.exists(old_dir):
        shutil.rmtree(old_dir)


def move_dir(src_dir, dst_dir):
    shutil.copytree(src_dir, dst_dir)


def main():
    args_parser = argparse.ArgumentParser()
    args_parser.add_argument('--gen-dir', help='generate path of log', required=True)
    args_parser.add_argument('--source-dir', help='generate path of log', required=True)
    args = args_parser.parse_args()

    target_dir = os.path.join(args.gen_dir, "include")
    clear_dir(target_dir)
    move_dir(os.path.join(args.source_dir, "include"), target_dir)


if __name__ == '__main__':
    sys.exit(main())
