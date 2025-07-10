#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (c) 2024 Huawei Device Co., Ltd.
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
import difflib
import logging
import os
import re
import shutil
import subprocess
import sys
import time

from concurrent.futures import ThreadPoolExecutor, wait, ALL_COMPLETED

logging.basicConfig(stream=sys.stdout, level=logging.INFO)

EXTENTION_JS = ".js"
EXTENTION_TS = ".ts"
EXTENTION_DTS = ".d.ts"
EXTENTION_DETS = ".d.ets"


class Extension:
    TS = ".ts"
    DTS = ".d.ts"
    JS = ".js"
    CJS = ".cjs"
    MJS = ".mjs"
    JSON = ".json"
    ETS = ".ets"
    DETS = ".d.ets"


FILE_EXTENSION_LIST = [
    Extension.DETS,
    Extension.ETS,
    Extension.DTS,
    Extension.TS,
    Extension.JS,
    Extension.JSON,
]

CONFIG_FILE_NAME = "obfConfig.json"
PRINT_UNOBFUSCATION_SUFFIX = "keptNames.unobf.json"
EXPECTED_UNOBFUSCATION_SUFFIX = "_expected_unobf.txt"
TEST_TYPE = 'grammar'

POOL_THREAD_COUNT = os.cpu_count()

NO_NEED_RUN_WITH_NODE_FILES = [
    "name_as_export_api_1.ts",
    "name_as_import_api_1.ts",
    "ohmurl_test.ts",
    "ohmurl_test_new.ts",
    "export_struct_transform_class.ts",
    "nosymbolIdentifierTest.ts",
    '02_transformed_struct_01.ts',
    'importFile1.ts',
    'importFile2.ts',
    'importFile3.ts',
    'namespaceexport1.ts',
    'noExistPath1.ts',
    'noExistPath2.ts',
    'noExistPath3.ts',
    'innerTest1.ts',
    'innerTest2.ts',
    'innerTest3.ts',
    'sameNameTest1.ts',
    'sameNameTest2.ts',
    'sameNameTest3.ts'
]


SOURCE_EXPECT_MAP = {}
SOURCE_EXPECT_MAP['grammar'] = {'test/local': 'test/grammar'}
SOURCE_EXPECT_MAP['combinations'] = {'test/local/combinations': 'test/combinations_expect'}
# For debug
VERBOSE = False


def thread_pool(action, data_list):
    all_task = []
    with ThreadPoolExecutor(max_workers=POOL_THREAD_COUNT) as pool:
        for data in data_list:
            all_task.append(pool.submit(action, data))

        wait(all_task, return_when=ALL_COMPLETED)
        result = [i.result() for i in all_task]

        return result


def has_js_or_ts_files(file_name_list):
    for one in file_name_list:
        if (
            one.endswith(EXTENTION_JS)
            or one.endswith(EXTENTION_TS)
            or one.endswith(EXTENTION_DETS)
        ):
            return True
    return False


class FileSuffix:
    def __init__(self, file_name, suffix):
        self.file_name = file_name
        self.suffix = suffix


def get_file_suffix(file_path) -> FileSuffix:
    for ext in FILE_EXTENSION_LIST:
        if file_path.endswith(ext):
            file_path_without_suffix = file_path[: -len(ext)]
            return FileSuffix(file_path_without_suffix, ext)
    return FileSuffix(file_path, "")


def should_run_with_node(file_path):
    for one in NO_NEED_RUN_WITH_NODE_FILES:
        if file_path.endswith(one):
            return False

    if file_path.endswith(EXTENTION_JS):
        return True
    elif (
        file_path.endswith(EXTENTION_TS)
        and not file_path.endswith(EXTENTION_DETS)
        and not file_path.endswith(EXTENTION_DTS)
    ):
        return True
    else:
        return False


def run_file_with_node(file_path):
    cmd = "node ./node_modules/ts-node/dist/bin.js %s" % file_path
    if VERBOSE:
        logging.info(cmd)
    return run_cmd(cmd)


class Task:
    def __init__(self, work_dir, obf_config_path, test_type):
        self.work_dir = work_dir
        self.obf_config_path = obf_config_path
        self.test_type = test_type


def obfuscate_dir(task: Task):
    config_file_path = task.obf_config_path
    work_dir = task.work_dir
    test_type = task.test_type
    cmd = "node lib/cli/SecHarmony.js %s --config-path %s --test-type %s" % (
        work_dir,
        config_file_path,
        test_type
    )
    if VERBOSE:
        logging.info("running test: %s", cmd)
    return run_cmd(cmd)


def list_all_js_or_ts_files(directory, file_list=None):
    if file_list is None:
        file_list = []

    files = sorted(os.listdir(directory))
    for one in files:
        abs_path = os.path.join(directory, one)
        if os.path.isdir(abs_path):
            file_list = list_all_js_or_ts_files(abs_path, file_list)
        elif abs_path.endswith(EXTENTION_JS):
            file_list.append(abs_path)
        elif abs_path.endswith(EXTENTION_TS):
            file_list.append(abs_path)
        elif abs_path.endswith(EXTENTION_DETS):
            file_list.append(abs_path)
        elif abs_path.endswith(EXTENTION_DTS):
            file_list.append(abs_path)

    return file_list


def current_time_second():
    return time.time()


def run_cmd(cmd):
    cmd = re.split("\\s+", cmd)
    process = subprocess.Popen(cmd, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    timeout = 60 * 5
    try:
        out, err = process.communicate(timeout=timeout)
    except subprocess.TimeoutExpired:
        process.kill()
        logging.info("running cmd timeout")

    if err and VERBOSE:
        logging.info("running cmd failed: ", err.decode("utf-8"))

    if VERBOSE:
        logging.info(cmd, "returncode = ", process.returncode)
    return process.returncode


class TimeUnit:
    def __init__(self, tag, duration_second) -> None:
        self.tag = tag
        self.duration_second = duration_second


class TestStat:
    def __init__(self):
        self.total_count = 0
        self.success_count = 0
        self.failed_count = 0
        self.failed_cases = []


class Runner:
    def __init__(self, test_filter, root_dir, test_type):
        self.test_filter = test_filter
        self.test_type = test_type
        self.obfuscation_tasks = []
        self.obfuscate_result = TestStat()
        self.run_with_node_result = TestStat()
        self.content_compare_result = TestStat()

        # For record duration
        self.time_list = []
        self.tag = None
        self.start_time = 0

        # all source files in test directory
        self.all_file_list = []

        self.obfscated_cache_root_dir = os.path.normpath(root_dir)

        self.__init()

    def start_record(self, tag):
        self.start_time = current_time_second()
        self.tag = tag

    def end_record(self):
        unit = TimeUnit(self.tag, current_time_second() - self.start_time)
        self.time_list.append(unit)
        self.tag = None
        self.start_time = 0

    def add_task(self, work_dir, obf_config_path):
        task = Task(work_dir, obf_config_path, self.test_type)
        self.obfuscation_tasks.append(task)

    def obfuscate(self):
        self.start_record("obfuscate")

        results = thread_pool(obfuscate_dir, self.obfuscation_tasks)

        for i, result in enumerate(results):
            self.obfuscate_result.total_count += 1
            if result == 0:
                self.obfuscate_result.success_count += 1
            else:
                self.obfuscate_result.failed_count += 1
                self.obfuscate_result.failed_cases.append(self.obfuscation_tasks[i])

        self.end_record()
        self.clear_obfuscation_tasks()

    def clear_obfuscation_tasks(self):
        self.obfuscation_tasks.clear()

    def run_with_node(self):
        self.start_record("run_with_node")
        self.__init_before_run_with_node()

        run_with_node_list = []
        for one in self.all_file_list:
            if should_run_with_node(one):
                run_with_node_list.append(one)

        results = thread_pool(run_file_with_node, run_with_node_list)

        for i, result in enumerate(results):
            self.run_with_node_result.total_count += 1
            if result == 0:
                self.run_with_node_result.success_count += 1
            else:
                self.run_with_node_result.failed_count += 1
                logging.info(
                    "run with node failed: return code = %d, %s", results[i], run_with_node_list[i]
                )
                self.run_with_node_result.failed_cases.append(run_with_node_list[i])

        self.end_record()

    def content_compare(self):
        self.start_record("content_compare")
        # For content compare
        for one in self.all_file_list:
            self.__compare_content(one)

        self.end_record()

    def traverse_dirs(self, root_dir, root_config_path):
        files = sorted(os.listdir(root_dir))

        current_config = None
        if CONFIG_FILE_NAME in files:
            current_config = os.path.join(root_dir, CONFIG_FILE_NAME)

        target_config = current_config
        if not target_config:
            target_config = root_config_path

        if has_js_or_ts_files(files):
            self.add_task(root_dir, target_config)
            return

        for one in files:
            sub_path = os.path.join(root_dir, one)
            if os.path.isdir(sub_path):
                self.traverse_dirs(sub_path, target_config)

    def print_summary(self):
        logging.info("------------------------------- Duration ----------------------------------------")
        for unit in self.time_list:
            logging.info("%s: %f", unit.tag, unit.duration_second)

        logging.info("----------------------------- Grammar Test summary -----------------------------")

        if self.obfuscate_result.failed_count > 0:
            logging.info("obfuscation failed cases:")
            for one in self.obfuscate_result.failed_cases:
                logging.info(one)

        if self.run_with_node_result.failed_count > 0:
            logging.info("run with node failed cases:")
            for one in self.run_with_node_result.failed_cases:
                logging.info(one)

        if self.content_compare_result.failed_count > 0:
            logging.info("content compare failed cases:")
            for one in self.content_compare_result.failed_cases:
                logging.info(one)

        logging.info("obfuscation passed    : %d/%d failed: %d", self.obfuscate_result.success_count,
            self.obfuscate_result.total_count, self.obfuscate_result.failed_count)
        logging.info("run with node passed  : %d/%d failed: %d", self.run_with_node_result.success_count,
            self.run_with_node_result.total_count, self.run_with_node_result.failed_count)
        logging.info("content compare passed: %d/%d failed: %d", self.content_compare_result.success_count,
            self.content_compare_result.total_count, self.content_compare_result.failed_count)

    def returncode(self):
        if (
            self.obfuscate_result.failed_count
            + self.run_with_node_result.failed_count
            + self.content_compare_result.failed_count
            > 0
        ):
            return -1
        return 0

    def has_failed_cases(self):
        return (self.obfuscate_result.failed_count > 0) or \
        (self.run_with_node_result.failed_count > 0) or \
        (self.content_compare_result.failed_count > 0)

    def get_expect_path(self, file_path):
        base_dir = os.path.normpath(os.path.join(os.path.dirname(__file__), '..'))
        source_expect = SOURCE_EXPECT_MAP[self.test_type]
        for source, expect in source_expect.items():
            if file_path.startswith(os.path.join(base_dir, source)):
                return file_path.replace(source, expect)

    def __filter_files(self, file_list):
        target = []
        for one in file_list:
            if re.match(f".*{self.test_filter}.*", one):
                target.append(one)
        return target

    def __init(self):
        try:
            shutil.rmtree(self.obfscated_cache_root_dir)
            logging.info("Directory %s deleted successfully", self.obfscated_cache_root_dir)
        except FileNotFoundError:
            logging.info("Directory %s not found", self.obfscated_cache_root_dir)
        except OSError as e:
            logging.info("Error deleting directory %s: %s", self.obfscated_cache_root_dir, e)

    def __init_before_run_with_node(self):
        self.all_file_list = list_all_js_or_ts_files(self.obfscated_cache_root_dir)
        self.all_file_list = self.__filter_files(self.all_file_list)

        if VERBOSE:
            for i, one in enumerate(self.all_file_list):
                logging.info("%d -> %s ", i, one)

    def __compare_expected(
        self, file_path, actual, expectation, actual_path, expectation_file
    ):
        self.content_compare_result.total_count += 1
        if actual.strip() == expectation.strip():
            self.content_compare_result.success_count += 1
        else:
            self.content_compare_result.failed_count += 1
            self.content_compare_result.failed_cases.append(file_path)
            diff = difflib.ndiff(actual.splitlines(), expectation.splitlines())
            logging.info("compare file, actual path:")
            logging.info(actual_path)
            logging.info("compare file, expect path:")
            logging.info(expectation_file)
            logging.info("\n".join(diff))

    def __compare_content(self, file_path):
        source_path = self.get_expect_path(file_path)
        source_suffix = get_file_suffix(source_path)
        expectation_path = f"{source_suffix.file_name}_expected.txt"
        result_suffix = get_file_suffix(file_path)
        result_cache_path = f"{result_suffix.file_name}.ts.cache.json"
        expectation_cache_path = f"{source_suffix.file_name}_expected_cache.txt"
        result_map_path = f"{result_suffix.file_name}.ts.map"
        expectation_map_path = f"{source_suffix.file_name}_expected_map.txt"

        result_unobfuscation_path = os.path.join(
            os.path.dirname(result_suffix.file_name), PRINT_UNOBFUSCATION_SUFFIX
        )
        expect_unobfuscation_path = (
            f"{source_suffix.file_name}{EXPECTED_UNOBFUSCATION_SUFFIX}"
        )

        if os.path.exists(expectation_path):
            with open(file_path) as actual_file, open(
                expectation_path
            ) as expectation_file:
                actual = actual_file.read()
                expectation = expectation_file.read()
                self.__compare_expected(
                    file_path, actual, expectation, file_path, expectation_file
                )

        if os.path.exists(expectation_cache_path) and os.path.exists(result_cache_path):
            with open(result_cache_path) as actual_file, open(
                expectation_cache_path
            ) as expectation_file:
                actual = actual_file.read()
                expectation = expectation_file.read()
                self.__compare_expected(
                    file_path,
                    actual,
                    expectation,
                    result_cache_path,
                    expectation_cache_path,
                )

        if os.path.exists(expect_unobfuscation_path) and os.path.exists(
            result_unobfuscation_path
        ):
            with open(result_unobfuscation_path) as actual_file, open(
                expect_unobfuscation_path
            ) as expectation_file:
                actual = actual_file.read()
                expectation = expectation_file.read()
                self.__compare_expected(
                    file_path,
                    actual,
                    expectation,
                    result_unobfuscation_path,
                    expect_unobfuscation_path,
                )

        if os.path.exists(expectation_map_path) and os.path.exists(result_map_path):
            with open(result_map_path) as actual_file, open(
                expectation_map_path
            ) as expectation_file:
                actual = actual_file.read()
                expectation = expectation_file.read()
                self.__compare_expected(
                    file_path,
                    actual,
                    expectation,
                    result_map_path,
                    expectation_map_path,
                )


def parse_args():
    # Create an ArgumentParser object
    parser = argparse.ArgumentParser(
        description="Script to run arkguard grammar tests"
    )

    # Define positional argument
    parser.add_argument(
        "--test-filter",
        type=str,
        default="",
        help="only run the cases match the filter",
    )

    # Parse the arguments from the command line
    args = parser.parse_args()

    return args


def main():
    args = parse_args()

    root_dir = os.path.join(os.path.dirname(__file__), "../test/grammar")
    root_dir = os.path.normpath(root_dir)

    root_config = os.path.join(root_dir, CONFIG_FILE_NAME)

    local_root_dir = os.path.join(os.path.dirname(__file__), "../test/local")
    runner = Runner(args.test_filter, local_root_dir, TEST_TYPE)
    runner.traverse_dirs(root_dir, root_config)
    runner.obfuscate()
    runner.run_with_node()
    runner.content_compare()

    runner.print_summary()
    return runner.returncode()

if __name__ == '__main__':
    code = main()
    sys.exit(code)
