#!/usr/bin/env python3
# coding: utf-8

"""
Copyright (c) 2021 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Description: process options and configs for test suite
"""

import argparse
import logging
import os
from enum import Enum

import yaml
import json5

import utils


arguments = {}
configs = {}


class TaskResult(Enum):
    undefined = 0
    passed = 1
    failed = 2


class OutputType(Enum):
    unsigned = 0
    signed = 1
    har = 2


class CompilationInfo:
    def __init__(self):
        self.result = TaskResult.undefined
        self.runtime_result = TaskResult.undefined
        self.error_message = ''
        self.time = 0
        self.abc_size = 0


class FullCompilationInfo:
    def __init__(self):
        self.debug_info = CompilationInfo()
        self.release_info = CompilationInfo()
        self.name = ''


class IncCompilationInfo:
    def __init__(self):
        self.debug_info = CompilationInfo()
        self.release_info = CompilationInfo()
        self.name = ''


class BytecodeHarCompilationInfo:
    def __init__(self):
        self.debug_info = CompilationInfo()
        self.release_info = CompilationInfo()
        self.name = ''


class ExternalCompilationInfo:
    def __init__(self):
        self.debug_info = CompilationInfo()
        self.release_info = CompilationInfo()
        self.name = ''


class BackupInfo:
    def __init__(self):
        self.cache_path = ''
        self.cache_debug = ''
        self.preview_cache_debug = ''
        self.hsp_signed_output_debug = ''
        self.cache_release = ''
        self.preview_cache_release = ''
        self.hsp_signed_output_release = ''
        self.output_debug = []
        self.output_release = []


class TestTask:
    def __init__(self):
        self.name = ''
        self.path = ''
        self.bundle_name = ''
        self.ability_name = ''
        self.type = ''
        self.build_path = []
        self.output_hap_path = ''
        self.output_hap_path_signed = ''
        self.output_app_path = ''
        self.inc_modify_file = []

        self.full_compilation_info = {}
        self.incre_compilation_info = {}
        self.bytecode_har_compilation_info = {}
        self.external_compilation_info = {}
        self.preview_compilation_info = {}
        self.other_tests = {}

        self.backup_info = BackupInfo()


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--sdkPath', type=str, dest='sdk_path', default='',
                        help='specify sdk path if need to update sdk. Default to use sdk specify in config.yaml')
    parser.add_argument('--buildMode', type=str, dest='build_mode', default='all',
                        choices=['all', 'assemble', 'preview', 'hotreload', 'hotfix'],
                        help='specify build mode')
    parser.add_argument('--hapMode', type=str, dest='hap_mode', default='all',
                        choices=['all', 'debug', 'release'],
                        help='specify hap mode')
    parser.add_argument('--compileMode', type=str, dest='compile_mode', default='all',
                        choices=['all', 'full', 'incremental', 'bytecode_har', 'external'],
                        help='specify compile mode')
    parser.add_argument('--testCase', type=str, dest='test_case', default='all',
                        choices=['all', 'fa', 'stage', 'compatible8', 'js'],
                        help='specify test cases')
    parser.add_argument('--testHap', type=str, dest='test_hap', default='all',
                        help="specify test haps, option can be 'all' or a list of haps seperated by ','")
    parser.add_argument('--imagePath', type=str, dest='image_path', default='',
                        help='specify image path if need to update rk/phone images. Default not to update image')
    parser.add_argument('--runHaps', dest='run_haps', action='store_true', default=False,
                        help='specify whether to verify by running the haps on board.')
    parser.add_argument('--logLevel', type=str, dest='log_level', default='error',
                        choices=['debug', 'info', 'warn', 'error'],
                        help='specify log level of test suite')
    parser.add_argument('--logFile', type=str, dest='log_file', default='',
                        help='specify the file log outputs to, empty string will output to console')
    parser.add_argument('--compileTimeout', type=int, dest='compile_timeout', default=1800,
                        help='specify deveco compilation timeout')
    global arguments
    arguments = parser.parse_args()


def parse_configs():
    config_yaml = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'config.yaml')
    with open(config_yaml, 'r') as config_file:
        global configs
        configs = yaml.safe_load(config_file)


def get_sdk_path_in_the_project(task_path):
    profile_file = os.path.join(task_path, 'build-profile.json5')
    with open(profile_file, 'r', encoding='utf-8') as file:
        profile_data = json5.load(file)
        api_version = profile_data['app']['products'][0]['compatibleSdkVersion']
        if isinstance(api_version, int):
            openharmony_sdk_path = configs.get('deveco_openharmony_sdk_path')
            return os.path.join(openharmony_sdk_path, str(api_version))
        else:
            harmonyos_sdk_path = configs.get('deveco_harmonyos_sdk_path')
            api_version_file_map = configs.get('api_version_file_name_map')
            file_name = api_version_file_map.get(api_version)
            return os.path.join(harmonyos_sdk_path, file_name, 'openharmony')


def get_ark_disasm_path(task_path):
    ark_disasm = 'ark_disasm.exe' if utils.is_windows() else 'ark_disasm'
    sdk_path = get_sdk_path_in_the_project(task_path)
    return os.path.join(sdk_path, 'toolchains', ark_disasm)


def create_test_tasks(haps_list):
    task_list = []
    test_cases = 'all' if arguments.test_case == 'all' else []
    test_haps = 'all' if arguments.test_hap == 'all' else []
    if test_cases != 'all':
        test_cases = arguments.test_case.split(',')
    if test_haps != 'all':
        test_haps = arguments.test_hap.split(',')

    for hap in haps_list:
        if test_cases == 'all' or test_haps == 'all' \
           or (test_cases and (hap['type'][0] in test_cases)) \
           or (test_haps and (hap['name'] in test_haps)):
            if not os.path.exists(hap['path']):
                logging.warning("Path of hap %s dosen't exist: %s", hap['name'], hap['path'])
                continue
            task = TestTask()
            task.name = hap['name']
            task.path = hap['path']
            task.bundle_name = hap['bundle_name']
            task.ability_name = hap['ability_name']
            task.type = hap['type']
            task.hap_module = hap['hap_module']
            task.hap_module_path = hap['hap_module_path']
            # If it is not a stage model, this HSP module cannot be created.
            task.hsp_module = hap.get('hsp_module', '')
            task.hsp_module_path = hap.get('hsp_module_path', '')
            task.cpp_module = hap['cpp_module']
            task.cpp_module_path = hap['cpp_module_path']
            task.har_module = hap['har_module']
            task.har_module_path = hap['har_module_path']
            task.build_path = hap['build_path']
            task.preview_path = hap['preview_path']
            task.cache_path = hap['cache_path']
            task.preview_cache_path = hap['preview_cache_path']
            task.hap_output_path = hap['hap_output_path']
            task.hap_output_path_signed = hap['hap_output_path_signed']
            task.har_output_path_har = hap['har_output_path_har']
            task.hsp_output_path = hap.get('hsp_output_path', '')
            task.hsp_output_path_signed = hap.get('hsp_output_path_signed', '')
            task.hsp_output_path_har = hap.get('hsp_output_path_har', '')
            task.cpp_output_path = hap.get('cpp_output_path')
            task.cpp_output_path_signed = hap.get('cpp_output_path_signed')
            task.main_pages_json_path = hap['main_pages_json_path']
            task.inc_modify_file = hap['inc_modify_file']
            task.har_modify_file = hap['har_modify_file']
            task.hsp_modify_file = hap.get('hsp_modify_file', '')
            task.cpp_modify_file = hap['cpp_modify_file']
            task.backup_info.cache_path = os.path.join(task.path, 'test_suite_cache')
            task.ark_disasm_path = get_ark_disasm_path(task.path)

            task_list.append(task)

    return task_list


def process_options():
    parse_args()
    utils.init_logger(arguments.log_level, arguments.log_file)
    parse_configs()
    return create_test_tasks(configs.get('haps'))