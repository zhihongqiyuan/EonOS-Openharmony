#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

import os
import sys
import optparse
import shutil

# d.ts directories to be deleted 需要排除的目录名称的列表
remove_list = ["@internal", "common", "form", "liteWearable", "config", "syscapCheck"]


# traversal all fill in project folder
# 2、复制文件：将输入路径中的文件和目录复制到输出路径，并且跳过某些指定的目录
def copy_files(input_path, output_path):
    for file in os.listdir(input_path):
        src = os.path.join(input_path, file)
        dst = os.path.join(output_path, file)
        if os.path.isdir(src) and (not file in remove_list):
            shutil.copytree(src, dst, dirs_exist_ok=True)
        elif os.path.isfile(src):
            shutil.copy(src, dst)


# 1、参数解析：通过 optparse 库解析命令行参数，获取输入路径 (--input) 和输出路径 (--output)。
def parse_args(args):
    parser = optparse.OptionParser()
    parser.add_option('--input', help='d.ts document input path')
    parser.add_option('--output', help='d.ts document output path')
    options, _ = parser.parse_args(args)
    return options


def main(argv):
    options = parse_args(argv)
    if not os.path.exists(options.output):
        os.makedirs(options.output)
    copy_files(options.input, options.output)


# 从指定的输入路径复制文件到输出路径，并在复制过程中 排除特定的文件夹。
if __name__ == "__main__":
    exit(main(sys.argv))
