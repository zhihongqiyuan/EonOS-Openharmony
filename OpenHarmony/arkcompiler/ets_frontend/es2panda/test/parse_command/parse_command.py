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

from abc import ABC, abstractmethod
import argparse
import glob
import json
import os
import re
import shutil
import stat
import subprocess
import sys
import threading

SUCCESS_STATUS = 0
CRASH_STATUS = -1
TIMEOUT_STATUS = -2

WORKLOAD_URL = 'https://gitee.com/xliu-huanwei/ark-workload.git'
BENCHMARK_PATH = '../benchmark'
SKIP_TEST = 'skip_tests.json'
DISCREPANCY_REPORT = 'discrepancy_report'
HTML_CONTENT = \
"""
<!DOCTYPE html>
<html>
<head>
    <title>Instruction Discrepancy Report</title>
    <style>
        table {
            width: 50%;
            border-collapse: collapse;
            margin: auto;
        }
        th, td {
            padding: 8px;
            text-align: center;
            border: 1px solid black;
            white-space: nowrap;
        }
        th:nth-child(2), td:nth-child(2) {
            text-align: left;
        }
        h1 {
            text-align: center;
        }
    </style>
</head>
<body>
    <h1>Instruction Discrepancy Report</h1>
    <table>
        <tr>
            <th>No</th>
            <th>Case Path</th>
            <th>es2abc Instruction Number</th>
            <th>v8 Instruction Number</th>
            <th>es2abc/v8 Ratio</th>
        </tr>
"""


def is_file(parser, arg):
    if not os.path.isfile(arg):
        parser.error(f'[ERROR]: The file "{arg}" does not exist!')
    return os.path.abspath(arg)


def is_directory(parser, arg):
    if not os.path.isdir(arg):
        parser.error(f'[ERROR]: The directory "{arg}" does not exist!')
    return os.path.abspath(arg)


def check_timeout(val):
    val = int(val)
    if val <= 0:
        raise argparse.ArgumentTypeError(f'[ERROR]: {val} is an invalid timeout value')
    return val


def get_args():
    description = "Generate a report on the difference between the number of v8 and es2abc bytecode instructions."
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('-v8', '-d8', '--d8_path', dest='d8_path', type=lambda arg : is_file(parser, arg),
                        help='Path to the V8 executable d8', required=True)
    parser.add_argument('-es2abc', '--es2abc_path', dest='es2abc_path', type=lambda arg : is_file(parser, arg),
                        help='Path to the executable program es2abc', required=True)
    parser.add_argument('--timeout', dest='timeout', type=check_timeout, default=180,
                        help='Time limits for use case execution (In seconds)')
    parser.add_argument('--add_case', dest='case', type=lambda arg : is_file(parser, arg),
                        help='Add the file path of a single test case to be executed', nargs='+')
    parser.add_argument('--add_case_dir', dest='case_dir', type=lambda arg : is_directory(parser, arg),
                        help='Add the directory where the test cases are to be executed', nargs='+')
    return parser.parse_args()


def assemble_command(command_list: list = None, executable_program_path: str = None,
                     file_path: str = None, parameters: list = None):
    if command_list is None:
        command_list = [executable_program_path, file_path]
        if parameters:
            command_list.extend(parameters)
    else:
        if executable_program_path:
            command_list[0] = executable_program_path
        if file_path:
            command_list[1] = file_path
    return command_list


class CaseManager:
    def __init__(self, args, skip_test_flag=True):
        self.test_root = os.path.dirname(os.path.abspath(__file__))
        self.args = args
        self.case_list = []
        self.skip_tests_info = os.path.join(self.test_root, SKIP_TEST)
        self.report_path = os.path.join(self.test_root, DISCREPANCY_REPORT)

        self.get_test_case()

        if skip_test_flag and os.path.exists(self.skip_tests_info):
            self.skip_cases()

        self.case_list.sort()

        # Creating dictionary: {file_path : [d8 status, es2abc status]}
        self.crash_dict = {file_path : [SUCCESS_STATUS, SUCCESS_STATUS] for file_path in self.case_list}

    def get_test_case(self):
        def add_directory_to_case_list(case_dir, case_list, extension='js', recursive=True):
            if not os.path.isdir(case_dir):
                print(f'[ERROR]: add_directory_to_case_list failed! {case_dir} does not exist!')
                return False
            glob_expression = os.path.join(case_dir, f'**/*.{extension}')
            files_list = glob.glob(glob_expression, recursive=recursive)
            for file in files_list:
                abs_file_path = os.path.abspath(file)
                if abs_file_path not in case_list:   # make sure no duplicate case
                    case_list.append(abs_file_path)
            return True

        if self.args.case is not None:
            for case in self.args.case:
                abs_file_path = os.path.abspath(case)
                if abs_file_path not in self.case_list:
                    self.case_list.append(abs_file_path)

        if self.args.case_dir is not None:
            for case in self.args.case_dir:
                add_directory_to_case_list(case, self.case_list)

        cur_dir = os.getcwd()
        os.chdir(self.test_root)

        # add workload cases
        case_dir_path = self.pull_cases_from_repo(WORKLOAD_URL, 'test_cases/ark-workload')
        if case_dir_path:
            print('[INFO]: pull workload cases Success!')
            case_dir_path = os.path.join(case_dir_path, 'weekly_workload', 'js')
            add_directory_to_case_list(case_dir_path, self.case_list)

        # add benchmark cases
        sys.path.insert(0, BENCHMARK_PATH)
        from utils import DEFAULT_TESTCASES_DIR, pull_cases, clear_folder_shutil
        benchmark_case_path = os.path.join(BENCHMARK_PATH, DEFAULT_TESTCASES_DIR)
        clear_folder_shutil(benchmark_case_path)
        pull_benchmark_cases_success = pull_cases()
        if pull_benchmark_cases_success:
            print('[INFO]: pull benchmark cases Success!')
            add_directory_to_case_list(benchmark_case_path, self.case_list)

        os.chdir(cur_dir)

    def git_clone(self, git_url, code_dir, pull=False):
        cur_dir = os.getcwd()
        cmd = ['git', 'clone', git_url, code_dir]
        if pull:
            os.chdir(code_dir)
            cmd = ['git', 'pull']
        process = subprocess.Popen(cmd)
        process.wait()
        os.chdir(cur_dir)
        result = True
        if process.returncode:
            print(f"\n[ERROR]: git clone or pull '{git_url}' Failed!")
            result = False
        return result

    def pull_cases_from_repo(self, case_url, case_dir):
        dir_path = os.path.join(self.test_root, case_dir)
        pull = False
        if os.path.exists(dir_path):
            pull = True
        clone_result = self.git_clone(case_url, dir_path, pull)
        if not clone_result:
            return None
        return dir_path
    
    def skip_cases(self):
        with open(self.skip_tests_info, 'r') as f:
            data = json.load(f)

        skip_case_list = []
        for reason_files_dict in data:
            skip_case_list.extend([os.path.abspath(os.path.join(self.test_root, case))
                                   for case in reason_files_dict['files']])
        
        self.case_list = [case for case in self.case_list if case not in skip_case_list]

    def calculate_ratio_for_discrepancy_report(self, d8_instruction_number, es2abc_instruction_number):
        if d8_instruction_number <= 0 or es2abc_instruction_number <= 0:
            return 'Invalid Ratio'
        ratio = (float(es2abc_instruction_number) / d8_instruction_number) * 100
        return f'{ratio:.2f}%'

    def generate_discrepancy_report(self, d8_output, es2abc_output):
        global HTML_CONTENT
        case_number = 1
        for d8_item, es2abc_item in zip(d8_output, es2abc_output):
            d8_instruction_number = d8_item[1]
            es2abc_instruction_number = es2abc_item[1]

            if self.crash_dict[d8_item[0]][0] == CRASH_STATUS:
                d8_instruction_number = \
                    "<span style='color:Red; font-weight:bold;'>{}</span>".format(d8_item[1])
            elif self.crash_dict[d8_item[0]][0] == TIMEOUT_STATUS:
                d8_instruction_number = \
                    "<span style='color:DarkRed; font-weight:bold;'>{}</span>".format(d8_item[1])

            if self.crash_dict[es2abc_item[0]][1] == CRASH_STATUS:
                es2abc_instruction_number = \
                    "<span style='color:Red; font-weight:bold;'>{}</span>".format(es2abc_item[1])
            elif self.crash_dict[es2abc_item[0]][1] == TIMEOUT_STATUS:
                es2abc_instruction_number = \
                    "<span style='color:DarkRed; font-weight:bold;'>{}</span>".format(es2abc_item[1])
            case_path = os.path.relpath(d8_item[0], self.test_root)
            html_content_of_case_info = f"""<tr>
                                    <td>{case_number}</td>
                                    <td>{case_path}</td>
                                    <td>{es2abc_instruction_number}</td>
                                    <td>{d8_instruction_number}</td>
                                    <td>{self.calculate_ratio_for_discrepancy_report(int(d8_item[1]),
                                         int(es2abc_item[1]))}</td>
                                </tr>"""
            HTML_CONTENT = "{}{}".format(HTML_CONTENT, html_content_of_case_info)
            case_number += 1

        html_content_of_end_tag = "</table></body></html>"
        HTML_CONTENT = "{}{}".format(HTML_CONTENT, html_content_of_end_tag)

        flags = os.O_RDWR | os.O_CREAT
        mode = stat.S_IWUSR | stat.S_IRUSR
        with os.fdopen(os.open(self.report_path + '.html', flags, mode), 'w') as f:
            f.truncate()
            f.write(HTML_CONTENT)


class Runner(ABC):
    def __init__(self, command_list:list, case_manager:CaseManager):
        self.output = []
        self.command_list = command_list
        self.case_manager = case_manager

    @abstractmethod
    def run(self):
        pass


class D8Runner(Runner):
    def __init__(self, command_list, case_manager):
        super().__init__(command_list, case_manager)

    def run(self):
        for file_path in self.case_manager.case_list:
            d8_case_path = file_path.replace('.js', '.mjs')
            shutil.copyfile(file_path, d8_case_path)
            instruction_number = RecognizeInstructionMethod.recognize_d8_bytecode_instruction(
                self.command_list, d8_case_path, self.case_manager)
            if os.path.exists(d8_case_path):
                os.remove(d8_case_path)
            self.output.append((file_path, instruction_number))


class ES2ABCRunner(Runner):
    def __init__(self, command_list, case_manager):
        super().__init__(command_list, case_manager)

    def run(self):
        for file_path in self.case_manager.case_list:
            instruction_number = RecognizeInstructionMethod.recognize_es2abc_bytecode_instruction(
                self.command_list, file_path, self.case_manager)
            self.output.append((file_path, instruction_number))


class RecognizeInstructionMethod:
    @staticmethod
    def recognize_d8_bytecode_instruction(command_list, file_path, case_manager):
        original_file_path = file_path.replace('.mjs', '.js')
        command_list = assemble_command(command_list, file_path=file_path)
        process = subprocess.Popen(command_list, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        try:
            output, _ = process.communicate(timeout=case_manager.args.timeout)
        except subprocess.TimeoutExpired:
            print(f"[WARNING]: v8 - Killed! Timeout: {file_path}")
            process.kill()
            case_manager.crash_dict[original_file_path][0] = TIMEOUT_STATUS
            return -1   # Script execution timeout sets the instruction number to -1
        process.wait()

        if process.returncode != 0:
            case_manager.crash_dict[original_file_path][0] = CRASH_STATUS
            print(f'[WARNING]: v8 - Crashed! {file_path}')

        decoded_output = output.decode('utf-8', errors='ignore')

        instruction_pattern = r'0x[0-9a-fA-F]+ @\s+\d+\s+:\s+([0-9a-fA-F\s]+)\s+(.*)$'
        result = re.finditer(instruction_pattern, decoded_output, re.MULTILINE)
        instruction_number = sum(1 for _ in result)
        return instruction_number

    @staticmethod
    def recognize_es2abc_bytecode_instruction(command_list, file_path, case_manager):
        command_list = assemble_command(command_list, file_path=file_path)
        process = subprocess.Popen(command_list, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        try:
            output, _ = process.communicate(timeout=case_manager.args.timeout)
        except subprocess.TimeoutExpired:
            print(f"[WARNING]: es2abc - Killed! Timeout: {file_path}")
            process.kill()
            case_manager.crash_dict[file_path][1] = TIMEOUT_STATUS
            return -1    # Script execution timeout sets the instructions number to -1
        process.wait()

        if process.returncode != 0:
            case_manager.crash_dict[file_path][1] = CRASH_STATUS
            print(f'[WARNING]: es2abc - Crashed! {file_path}')

        decoded_output = output.decode('utf-8', errors='ignore')
        lines = decoded_output.split('\n')
        instruction_number = 0
        for line in lines:
            if 'instructions_number:' in line:
                instruction_number = line.split('instructions_number:')[-1].strip()
                break
        return instruction_number


def main():
    args = get_args()

    d8_command = assemble_command(executable_program_path=args.d8_path, parameters=['--print-bytecode'])
    es2abc_command = assemble_command(executable_program_path=args.es2abc_path,
                                      parameters=['--module', '--dump-size-stat', '--output=/dev/null'])

    case_manager = CaseManager(args)
    d8_runner = D8Runner(d8_command, case_manager)
    es2abc_runner = ES2ABCRunner(es2abc_command, case_manager)

    if len(case_manager.case_list):
        thread_d8 = threading.Thread(target=d8_runner.run)
        thread_es2abc = threading.Thread(target=es2abc_runner.run)
        thread_d8.start()
        thread_es2abc.start()
        thread_d8.join()
        thread_es2abc.join()

    case_manager.generate_discrepancy_report(d8_runner.output, es2abc_runner.output)


if __name__ == "__main__":
    main()