#!/usr/bin/env python3
# coding: utf-8

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

Description: prepare environment for test
"""

import logging
import os
import shutil

import options
from utils import is_mac, is_linux


def setup_env():
    old_env = os.environ.copy()
    old_env_path = old_env['PATH']

    deveco_path = options.configs.get('deveco_path')
    java_home = os.path.join(deveco_path, 'jbr')
    tool_home = os.path.join(deveco_path)
    sdk_home = os.path.join(tool_home, 'sdk')
    node_js_path = os.path.join(deveco_path, 'tools', 'node')
    if is_mac():
        node_js_path = os.path.join(node_js_path, 'bin')
        tool_home = os.path.join(tool_home, 'Contents ')
    java_path = os.path.join(java_home, 'bin')
    ohpm_path = os.path.join(tool_home, 'tools', 'ohpm', 'bin')
    hvigor_path = os.path.join(tool_home, 'tools', 'hvigor', 'bin')

    os.environ['PATH'] = os.pathsep.join(
        [java_path, tool_home, sdk_home, ohpm_path, hvigor_path, node_js_path]) + os.pathsep + old_env_path
    os.environ['JAVA_HOME'] = java_home


def check_deveco_env():
    if is_linux():
        return False

    java_path = os.path.join(options.configs.get('deveco_path'), 'jbr')
    if not os.path.exists(java_path):
        logging.error("Java not found!")
        return False

    node_path = os.path.join(options.configs.get('deveco_path'), 'tools', 'node')
    if not os.path.exists(node_path):
        logging.error("Node js not found!")
        return False

    return True


def clean_log():
    output_log_file = options.configs.get('log_file')
    daily_report_file = options.configs.get('output_html_file')
    pictures_dic = options.configs.get('pictures_dic')
    if os.path.exists(output_log_file):
        os.remove(output_log_file)
    if os.path.exists(daily_report_file):
        os.remove(daily_report_file)
    if os.path.exists(pictures_dic):
        shutil.rmtree(pictures_dic)


def prepare_test_env():
    clean_log()
    prepared = check_deveco_env()
    setup_env()
    return prepared
