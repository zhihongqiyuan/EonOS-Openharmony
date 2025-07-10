#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (c) 2023 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import argparse
import math
import os
import time
from config import COMP, DEFAULT_RUNS, PARSE_ONLY, OPT_LEVEL, RUNS_NUM, DEFAULT_PARAMS, CUR_FILE_DIR, \
    DEFAULT_TESTCASES_DIR, SELECTED_PARAMETERS, JS_FILE_SUFFIX, TS_FILE_SUFFIX, REPORT_DIR, TEST_RESULT_FILE, \
    DEFAULT_OUTPUT_DIR, ABC_FILE_SUFFIX, DEFAULT_HERMES_PARAMS, DEFAULT_HERMES_PATH, HERMES_FILE_SUFFIX, \
    HERMES_CODE_PATH, ES2ABC, HERMES
from utils import traverse_dir, write_result, write_html, run_cmd_cwd, clear_folder_shutil, pull_cases, remove_dir, \
    pull_build_hermes


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--es2abc-tool',
                        required=True,
                        help="es2abc tool path")
    parser.add_argument('--runs', required=False, type=int,
                        default=DEFAULT_RUNS,
                        help='Number of runs')
    parser.add_argument('--opt-level', required=False, choices=['0', '1', '2'],
                        help="Specifies the compiler optimization level")
    parser.add_argument('--parse-only', required=False,
                        default=False, action='store_true',
                        help="During the test, only the input file is parsed")
    parser.add_argument('--engine-comp', required=False,
                        default=False, action='store_true',
                        help="Compared to the other engine")
    arguments = parser.parse_args()
    return arguments


class Es2AbcBenchMarks:
    def __init__(self, args):
        self.args = args
        self.results = []
        self.cmds = {}
        self.select_params = []

    def parse_args(self):
        cmds = {ES2ABC: [self.args.es2abc_tool]}
        if self.args.parse_only and not self.args.engine_comp:
            cmds[ES2ABC].append('--parse-only')
            self.select_params.append(f'{PARSE_ONLY}:True')
        else:
            self.select_params.append(f'{PARSE_ONLY}:False')
        if self.args.opt_level and not self.args.engine_comp:
            cmds[ES2ABC].append(f'--opt-level {self.args.opt_level}')
            self.select_params.append(f'{OPT_LEVEL}:{self.args.opt_level}')
        else:
            self.select_params.append(f'{OPT_LEVEL}:0')
        self.select_params.append(f'{RUNS_NUM}:{self.args.runs}')
        cmds[ES2ABC] += DEFAULT_PARAMS
        if self.args.engine_comp:
            cmds[HERMES] = [DEFAULT_HERMES_PATH] + DEFAULT_HERMES_PARAMS
            self.select_params.append(f'{COMP}:True')
        else:
            self.select_params.append(f'{COMP}:False')
        self.cmds = cmds

    def run(self):
        test_case_path = os.path.join(CUR_FILE_DIR, DEFAULT_TESTCASES_DIR)
        test_cases_files = traverse_dir(test_case_path)
        self.parse_args()
        html_data = {}
        selected_params = f'{SELECTED_PARAMETERS}: {"; ".join(self.select_params)}'
        write_result(selected_params)
        for dir_name, file_paths in test_cases_files.items():
            case_dir_name = os.path.basename(dir_name)
            case_info_html_data = {}
            case_info_path = os.path.join(REPORT_DIR, f'{case_dir_name}.html')
            print(f"---------------------------{case_dir_name}--------------------------------")
            for engine_type in self.cmds:
                print(f">>>engine {engine_type}")
                case_execution_time_sum, means = self.run_benchmark(file_paths, engine_type, case_dir_name,
                                                                    case_info_html_data)
                print(f">>>Done. \n")
                case_execution_time_sum_avg = str(case_execution_time_sum)
                score_sum = self.args.runs / self.gen_score(means)
                test_result = [f'{case_dir_name}', case_execution_time_sum_avg, f'{score_sum * 100}'.split('.')[0],
                               case_info_path, self.args.runs]
                if engine_type in html_data:
                    html_data[engine_type].append(test_result)
                else:
                    html_data[engine_type] = [test_result]
            write_html(case_info_html_data, case_info_path, selected_params, info=True)
            print("-------------------------------------------------------------------\n")
        write_html(html_data, os.path.join(REPORT_DIR, TEST_RESULT_FILE), selected_params, info=False)

    def run_benchmark(self, file_paths, engine_type, case_dir_name, case_info_html_data):
        case_execution_time_sum = 0
        means = []
        for file_path in file_paths:
            file_name = os.path.basename(file_path)
            if not (file_name.endswith(JS_FILE_SUFFIX) or file_name.endswith(TS_FILE_SUFFIX)):
                continue
            print(f'Running {file_name.replace(JS_FILE_SUFFIX, "").replace(TS_FILE_SUFFIX, "")} ')
            case_execution_time, case_execution_times = self.run_single_benchmark(file_path, engine_type)
            case_execution_time_ms = str(case_execution_time * 1000).split(".")[0]
            case_execution_time_sum += int(case_execution_time_ms)
            mean = self.gen_score(case_execution_times)
            means.append(mean)
            score = self.args.runs / mean
            score = f'{score * 100}'.split('.')[0]
            log_str = (f'engine {engine_type} case: {os.path.join(case_dir_name, file_name)} number of runs: '
                       f'{self.args.runs} avg time: {case_execution_time_ms}ms  Score:{score}\n')
            single_case_result = [f'{os.path.join(case_dir_name, file_name)}', case_execution_time_ms, score,
                                  self.args.runs]
            if engine_type in case_info_html_data:
                case_info_html_data[engine_type].append(single_case_result)
            else:
                case_info_html_data[engine_type] = [single_case_result]
            write_result(log_str)
        return case_execution_time_sum, means

    def run_single_benchmark(self, file_path, engine_type):
        elapsed_sum = 0
        case_execution_times = []
        new_cmds = [i for i in self.cmds[engine_type]]
        output_file = os.path.join(DEFAULT_OUTPUT_DIR, os.path.basename(file_path).
                                   replace(JS_FILE_SUFFIX, ABC_FILE_SUFFIX).
                                   replace(TS_FILE_SUFFIX, ABC_FILE_SUFFIX))
        if engine_type == ES2ABC:
            new_cmds += [file_path, "--output", output_file]
        else:
            new_cmds += [output_file.replace(ABC_FILE_SUFFIX, HERMES_FILE_SUFFIX), file_path]
        for _ in range(self.args.runs):
            start = time.time()
            run_cmd_cwd(new_cmds)
            elapsed_sum += time.time() - start
            case_execution_times.append(elapsed_sum)
        return elapsed_sum / self.args.runs, case_execution_times

    @staticmethod
    def gen_score(numbers):
        log = 0
        for num in numbers:
            log += math.log(num)
        mean = math.exp(log / len(numbers))
        return mean


def prepare_and_run(args):
    clear_folder_shutil(DEFAULT_OUTPUT_DIR)
    clear_folder_shutil(REPORT_DIR)
    clear_folder_shutil(DEFAULT_TESTCASES_DIR)
    pull_cases_success = pull_cases()
    if pull_cases_success:
        abc_runner = Es2AbcBenchMarks(args)
        abc_runner.run()
        remove_dir(DEFAULT_OUTPUT_DIR)
    print("> > > Done.")


def main():
    args = parse_args()
    print("> > > benchmark running")
    if args.engine_comp:
        clear_folder_shutil(HERMES_CODE_PATH)
        print("Building Hermes...")
        build_result = pull_build_hermes()
        if not build_result:
            raise Exception('build hermes failed.')
    prepare_and_run(args)


if __name__ == "__main__":
    main()
