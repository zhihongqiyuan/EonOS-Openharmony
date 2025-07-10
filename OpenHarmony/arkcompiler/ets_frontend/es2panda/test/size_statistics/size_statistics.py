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
import multiprocessing
import os
import stat
import subprocess
import sys

WORKLOAD_PATH = 'test_cases/ark-workload/weekly_workload'
PARSE_COMMAND_PATH = '../parse_command'
SIZE_PERCENTAGE_REPORT = 'size_percentage_report.html'
HTML_CONTENT = \
"""
<!DOCTYPE html>
<html>
<head>
    <title>Size Percentage Report</title>
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
        th:last-child, td:last-child {
            white-space: pre-wrap;
            text-align: left;
        }
        h1 {
            text-align: center;
        }
        .percentage_table {
            width: 100%;
            border-collapse: collapse;
            margin-top: -35px;
        }
        .percentage_table th, .percentage_table td {
            padding: 4px;
            border: none;
        }
        .percentage_table td {
            padding-right: 10px;
            line-height: 1;
            padding-top: 0;
        }
        .percentage_table .item_section {
            text-align: left;
        }
    </style>
</head>
<body>
    <h1>Size Percentage Report</h1>
    <table>
        <tr>
            <th>No</th>
            <th>Case Path</th>
            <th>ABC Size</th>
            <th>Percentage of Sections</th>
        </tr>
"""


def is_directory(parser, arg):
    if not os.path.isdir(arg):
        parser.error("The directory '%s' does not exist" % arg)
    return os.path.abspath(arg)


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
    description = "Generate statistics on the percentage of size corresponding to each case."
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('--es2abc-path', dest='es2abc_path', type=lambda arg : is_file(parser, arg),
                        help='Path to the executable program es2abc', required=True)
    parser.add_argument('--no-progress', action='store_false', dest='progress', default=True, 
                        help='Don\'t show progress bar')
    parser.add_argument('--timeout', type=check_timeout, dest='timeout', default=180, 
                        help='Time limits for use case execution (In seconds)')
    return parser.parse_args()


class Test:
    def __init__(self, test_path, flags):
        self.test_path = test_path
        self.test_case_name = self.test_path.split('/')[-1]
        self.flags = flags
        self.output = None
    
    def __lt__(self, other):
        return self.test_path < other.test_path

    def run(self, runner):
        test_abc_name = self.test_case_name[:self.test_case_name.rfind('.')] + '.abc'
        test_abc_path = os.path.join(os.path.dirname(self.test_path), test_abc_name)
        cmd = runner.cmd_prefix + [runner.es2abc_path]
        cmd.extend(self.flags)
        cmd.extend(["--output=" + test_abc_path])
        cmd.append(self.test_path)

        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = process.communicate(timeout=runner.args.timeout)
        self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")

        if os.path.exists(test_abc_path):
            self.abc_size = os.path.getsize(test_abc_path)
            self.percentage_of_sections = self.output
            os.remove(test_abc_path)
        else:
            self.abc_size = 0
            self.percentage_of_sections = 'NULL'
        
        self.test_case_name = os.path.relpath(self.test_path, runner.test_root)
        return self


class Runner:
    def __init__(self, args):
        self.args = args
        self.test_root = os.path.dirname(os.path.abspath(__file__))
        self.es2abc_path = args.es2abc_path
        self.size_percentage_statistics_file = os.path.join(self.test_root, SIZE_PERCENTAGE_REPORT)
        self.case_list = []     # store abs case_path
        self.tests = []         # store Test obj
        self.cmd_prefix = []
        self.results = []

    def wrap_cases(self, flags, func=Test):
        self.tests += list(map(lambda f: func(f, flags), self.case_list))
        self.case_list.clear()

    def add_case(self, case_path, extension, flags):
        abs_case_path = os.path.abspath(case_path)
        if abs_case_path not in self.case_list and abs_case_path.endswith(extension):
            self.case_list.append(case_path)
        self.wrap_cases(flags)

    def add_directory(self, directory, extension, flags):
        glob_expression = os.path.join(os.path.abspath(directory), "**/*%s" % (extension))
        cases = glob(glob_expression, recursive=True)
        for case in cases:
            self.add_case(case, extension, flags)

    def run_test(self, test):
        return test.run(self)

    def run(self):
        pool = multiprocessing.Pool()
        result_iter = pool.imap_unordered(self.run_test, self.tests, chunksize=32)
        pool.close()

        if self.args.progress:
            from tqdm import tqdm
            result_iter = tqdm(result_iter, total=len(self.tests))

        self.results = [result for result in result_iter]
        self.results.sort()
        pool.join()

        self.generate_size_percentage_report()

    def format_sections_for_percentage_report(self, output, percentage_of_sections_list):
        lines = output.split('\n')
        start_flag = False
        for line in lines:
            if start_flag:
                if 'percent' in line:
                    [item_section_and_item_size, percentage] = line.split(',')
                    [item_section, item_size] = item_section_and_item_size.split(':')
                    percentage = percentage.split(':')[-1].strip()
                    percentage_of_sections_list.append([item_section.strip(), item_size.strip(), \
                                                        percentage.strip()])

            if 'Panda file size statistic:' in line:
                start_flag = True

    def generate_size_percentage_report(self):
        global HTML_CONTENT
        for number, result in enumerate(self.results, 1):
            percentage_of_sections_list = []
            self.format_sections_for_percentage_report(result.percentage_of_sections, percentage_of_sections_list)
            percentage_of_sections_list.sort(key=lambda percentage_of_section: int(percentage_of_section[1]), 
                                             reverse=True)

            html_content_of_result = f"""
                <tr>
                    <td>{number}</td>
                    <td>{result.test_case_name}</td>
                    <td>{result.abc_size}</td>
            """
            HTML_CONTENT = "{}{}".format(HTML_CONTENT, html_content_of_result)

            header = [''] * 3
            if len(percentage_of_sections_list):
                header = ['section', 'size', 'percentage']
                html_content_of_percentage = f"""
                    <td>
                        <table class='percentage_table'>
                        <tr>
                            <th>{header[0]}</th>
                            <th>{header[1]}</th>
                            <th>{header[2]}</th>
                        </tr>
                """
                HTML_CONTENT = "{}{}".format(HTML_CONTENT, html_content_of_percentage)

                total_size = 0
                for [item_section, item_size, percentage] in percentage_of_sections_list:
                    html_content_of_sections = f"""
                            <tr>
                                <td class='item_section'>{item_section}</td>
                                <td>{item_size}</td>
                                <td>{percentage}</td>
                            </tr>
                    """
                    HTML_CONTENT = "{}{}".format(HTML_CONTENT, html_content_of_sections)
                    total_size += int(item_size)

                html_content_of_total_size = f"""
                            <tr>
                                <td class='item_section'>total size</td>
                                <td>{total_size}</td>
                                <td></td>
                            </tr>
                """
                HTML_CONTENT = "{}{}".format(HTML_CONTENT, html_content_of_total_size)

                HTML_CONTENT = "{}</table></td></tr>".format(HTML_CONTENT)
            else:
                html_content_of_not_available = f"<td>N/A</td></tr>"
                HTML_CONTENT = "{}{}".format(HTML_CONTENT, html_content_of_not_available)

        html_content_of_end_tag = "</table></body></html>"
        HTML_CONTENT = "{}{}".format(HTML_CONTENT, html_content_of_end_tag)

        flags = os.O_RDWR | os.O_CREAT
        mode = stat.S_IWUSR | stat.S_IRUSR
        with os.fdopen(os.open(self.size_percentage_statistics_file, flags, mode), 'w') as report:
            report.truncate()
            report.write(HTML_CONTENT)


def main():
    args = get_args()

    runner = Runner(args)
    
    # Get CaseManager from parse_command
    cur_dir = os.getcwd()
    os.chdir(runner.test_root)
    sys.path.insert(0, PARSE_COMMAND_PATH)
    from parse_command import CaseManager
    os.chdir(cur_dir)

    # Add benchmark cases and workload cases (js)
    args_for_case_manager = copy.copy(args)
    setattr(args_for_case_manager, 'case', None)
    setattr(args_for_case_manager, 'case_dir', None)
    case_manager = CaseManager(args_for_case_manager, skip_test_flag=False)
    for case_path in case_manager.case_list:
        runner.add_case(case_path, '.js', ['--module', '--dump-file-item-size'])

    # Add workload cases (ts)
    ts_workload_path = os.path.join(case_manager.test_root, WORKLOAD_PATH, 'ts')
    runner.add_directory(ts_workload_path, '.ts', ['--module', '--dump-file-item-size'])

    runner.run()


if __name__ == "__main__":
    main()