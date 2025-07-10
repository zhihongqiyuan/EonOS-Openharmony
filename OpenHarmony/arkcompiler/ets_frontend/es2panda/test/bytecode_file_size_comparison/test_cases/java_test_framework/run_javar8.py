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
import shutil

JAVA_CASES = 'java_cases'
DEX_FILE = 'classes.dex'
DEX_SIZE_DATA = 'dex_size.dat'
SOURCE_SIZE_DATA = 'source_size.dat'
R8_CLASS = 'com.android.tools.r8.R8'
PG_CONF = 'pg.cfg'


def is_file(parser, arg):
    if not os.path.isfile(arg):
        parser.error("The file '%s' does not exist" % arg)
    return os.path.abspath(arg)

def is_dir(parser, arg):
    if not os.path.isdir(arg):
        parser.error("The dir '%s' does not exist" % arg)
    return os.path.abspath(arg)

def check_timeout(value):
    ivalue = int(value)
    if ivalue <= 0:
        raise argparse.ArgumentTypeError("%s is an invalid timeout value" % value)
    return ivalue


def get_args():
    description = "Execute Java test cases and generate a data file with the bytecode file size of the test cases."
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('--java-bin-path', dest='java_bin_path', type=lambda arg : is_dir(parser, arg),
                        help='Path to java root', required=True)
    parser.add_argument('--r8-path', dest='r8_path', type=lambda arg : is_file(parser, arg),
                        help='Path to the executable program r8', required=True)
    parser.add_argument('--rt-path', dest='rt_path', type=lambda arg : is_file(parser, arg),
                        help='Path to the jre/lib/rt.jar', required=True)
    parser.add_argument('--timeout', type=check_timeout, dest='timeout', default=180, 
                        help='Time limits for use case execution (In seconds)')
    return parser.parse_args()


class JavaR8Runner:
    def __init__(self, args):
        self.args = args
        self.test_root = os.path.dirname(os.path.abspath(__file__))
        self.output = {}
        self.source_output = {}
        self.case_list = []
        self.cmd_list = [args.java_bin_path, args.r8_path, args.rt_path] # Contain [java_bin_path, r8_path, rt_path]

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
        source_size_data = os.path.join(self.test_root, SOURCE_SIZE_DATA)
        flags = os.O_RDWR | os.O_CREAT
        mode = stat.S_IWUSR | stat.S_IRUSR
        with os.fdopen(os.open(dex_size_data, flags, mode), 'w') as f:
            f.truncate()
            json.dump(self.output, f)
        with os.fdopen(os.open(source_size_data, flags, mode), 'w') as f:
            f.truncate()
            json.dump(self.source_output, f)

    def run(self):
        self.case_list.sort()
        [java_bin_path, r8_path, rt_path] = copy.deepcopy(self.cmd_list)
        javac_path = '/'.join([java_bin_path, 'javac'])
        jar_path = '/'.join([java_bin_path, 'jar'])
        java_path = '/'.join([java_bin_path, 'java'])
        cmd_jar2dex_base = [java_path, '-cp', r8_path, R8_CLASS, '--release', '--pg-conf', PG_CONF, '--lib', rt_path]
        for file_path in self.case_list:
            child_path = file_path[:file_path.rfind('.')]
            cmd_java2class = [javac_path, file_path, '-d', child_path]
            case_name = self.get_case_name(file_path)
            jar_name = ''.join([child_path, '/', case_name, '.jar'])
            cmd_class2jar = [jar_path ,'cvf', jar_name, '-C', child_path, '.']
            cmd_jar2dex = copy.copy(cmd_jar2dex_base)
            cmd_jar2dex.extend(['--output', child_path, jar_name])
            try:
                subprocess.run(cmd_java2class, timeout=self.args.timeout)
                subprocess.run(cmd_class2jar, timeout=self.args.timeout)
                subprocess.run(cmd_jar2dex, timeout=self.args.timeout)

            except subprocess.TimeoutExpired:
                print(f'[WARNING]: Timeout! {file_path}')
            except Exception as e:
                print(f"[ERROR]: {e}")
            finally:
                dex_file = os.path.join(child_path, DEX_FILE)
                dex_file_size = 0
                if os.path.exists(dex_file):
                    dex_file_size = os.path.getsize(dex_file)
                shutil.rmtree(child_path)
                self.output[self.get_case_name(file_path)] = dex_file_size
                self.source_output[self.get_case_name(file_path)] = os.path.getsize(file_path)
                print(f'[INFO]: FINISH: {file_path}!')

        self.save_output_to_file()


def main():
    args = get_args()
    java_runner = JavaR8Runner(args)
    java_runner.add_directory(JAVA_CASES, '.java')
    java_runner.run()


if __name__ == "__main__":
    main()