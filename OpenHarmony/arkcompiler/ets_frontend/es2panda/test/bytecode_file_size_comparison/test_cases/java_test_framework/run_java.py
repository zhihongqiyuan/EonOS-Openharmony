#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright (c) 2024 Huawei Device Co., Ltd.
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

from glob import glob
import argparse
import copy
import json
import os
import stat
import subprocess

JAVA_CASES = 'java_cases'
DEX_FILE = 'classes.dex'
DEX_SIZE_DATA = 'dex_size.dat'


def is_file(parser, arg):
    if not os.path.isfile(arg):
        parser.error("The file '%s' does not exist" % arg)
    return os.path.abspath(arg)


def check_timeout(value):
    ivalue = int(value)
    if ivalue <= 0:
        raise argparse.ArgumentTypeError("%s is an invalid timeout value" % value)
    return ivalue


def get_args():
    description = "Execute Java test cases and generate a data file with the bytecode file size of the test cases."
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('--javac-path', dest='javac_path', type=lambda arg : is_file(parser, arg),
                        help='Path to javac compiler', required=True)
    parser.add_argument('--d8-path', dest='d8_path', type=lambda arg : is_file(parser, arg),
                        help='Path to the executable program d8', required=True)
    parser.add_argument('--timeout', type=check_timeout, dest='timeout', default=180, 
                        help='Time limits for use case execution (In seconds)')
    return parser.parse_args()


class JavaD8Runner:
    def __init__(self, args):
        self.args = args
        self.test_root = os.path.dirname(os.path.abspath(__file__))
        self.output = {}
        self.case_list = []
        self.cmd_list = [[args.javac_path], [args.d8_path]] # Contain [cmd_java2class, cmd_class2dex]

    @staticmethod
    def get_case_name(case_path):
        filename = case_path.split('/')[-1]
        case_name = filename[:filename.rfind('.')]
        return case_name

    def add_case(self, case_path, extension):
        if not os.path.isabs(case_path):
            case_path = os.path.join(self.test_root, case_path)
        abs_case_path = os.path.abspath(case_path)
        if abs_case_path not in self.case_list and abs_case_path.endswith(extension):
            self.case_list.append(case_path)

    def add_directory(self, directory, extension):
        if not os.path.isabs(directory):
            directory = os.path.join(self.test_root, directory)
        glob_expression = os.path.join(os.path.abspath(directory), "**/*%s" % (extension))
        cases = glob(glob_expression, recursive=True)
        for case in cases:
            self.add_case(case, extension)

    def save_output_to_file(self):
        dex_size_data = os.path.join(self.test_root, DEX_SIZE_DATA)
        flags = os.O_RDWR | os.O_CREAT
        mode = stat.S_IWUSR | stat.S_IRUSR
        with os.fdopen(os.open(dex_size_data, flags, mode), 'w') as f:
            f.truncate()
            json.dump(self.output, f)

    def run(self):
        self.case_list.sort()
        for file_path in self.case_list:
            [cmd_java2class, cmd_class2dex] = copy.deepcopy(self.cmd_list)
            cmd_java2class.extend([file_path])
            class_files = []
            case_root = os.path.dirname(file_path)
            try:
                subprocess.run(cmd_java2class, timeout=self.args.timeout)

                for file in os.listdir(case_root):
                    if file.endswith('.class'):
                        class_files.append(os.path.join(case_root, file))

                cmd_class2dex.extend(class_files)
                cmd_class2dex.extend(['--output', case_root])

                subprocess.run(cmd_class2dex, timeout=self.args.timeout)

            except subprocess.TimeoutExpired:
                print(f'[WARNING]: Timeout! {file_path}')
            except Exception as e:
                print(f"[ERROR]: {e}")
            finally:
                for class_file in class_files:
                    if os.path.exists(class_file):
                        os.remove(class_file)

                dex_file = os.path.join(case_root, DEX_FILE)
                dex_file_size = 0
                if os.path.exists(dex_file):
                    dex_file_size = os.path.getsize(dex_file)
                    os.remove(dex_file)
                self.output[self.get_case_name(file_path)] = dex_file_size
                print(f'[INFO]: FINISH: {file_path}!')

        self.save_output_to_file()


def main():
    args = get_args()
    java_runner = JavaD8Runner(args)
    java_runner.add_directory(JAVA_CASES, '.java')
    java_runner.run()


if __name__ == "__main__":
    main()