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
import os
import shutil
import subprocess

from config import BUILD_HERMES_CMDS, HERMES_BUILD_LOG_PATH, HERMES_CLONE_PATH, REPORT_DIR, LOG_FILE, \
    CASE_URLS, CUR_FILE_DIR, DEFAULT_TESTCASES_DIR, SUNSPIDER, KRAKEN, \
    CASE_LIST, KRAKENBENCHMARK_CASE_PATH, KRAKEN_CASE_PATH, SUNSPIDER_CASE_PATH, JS_FILE_SUFFIX, \
    CASE_DATA_SUFFIX, HTML_TEMPLATE, DEFAULT_TIME, AVERAGE_TIME, HTML_SCRIPT, SELECTED_PARAMETERS_BASE, \
    HTML_TABLE_DEFAULT, HTML_TABLE_COMP, HERMES_GIT_URL, ES2ABC, HERMES


def write_result(log_str):
    write_log_file(os.path.join(REPORT_DIR, LOG_FILE), log_str)


def write_log_file(file_path, log_str):
    with open(file_path, mode='a+') as f:
        f.write(log_str)


def traverse_dir(path):
    file_paths = {}
    for file in os.listdir(path):
        file_path = os.path.join(path, file)
        if os.path.isdir(file_path):
            file_paths.update(traverse_dir(file_path))
        else:
            dir_name = os.path.dirname(file_path)
            if dir_name not in file_paths:
                file_paths[dir_name] = [file_path]
            else:
                file_paths[dir_name].append(file_path)
    return file_paths


def run_cmd_cwd(cmds):
    p = subprocess.Popen(cmds, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.wait()
    return p.returncode


def git_clone(git_url, code_dir):
    cmd = ['git', 'clone', git_url, code_dir]
    ret = run_cmd_cwd(cmd)
    result = True
    if ret:
        print(f"\n error: Cloning '{git_url}' failed.")
        result = False
    return result


def clear_folder_shutil(path):
    if os.path.exists(path):
        shutil.rmtree(path)
    os.mkdir(path)


def remove_dir(path):
    if os.path.exists(path):
        shutil.rmtree(path)


def remove_file(file):
    if os.path.exists(file):
        os.remove(file)


def mkdir(path):
    if not os.path.exists(path):
        os.makedirs(path)


def merge_files(file_list):
    base_file = file_list[0]
    with open(base_file, "a+") as output_file:
        for i in range(1, len(file_list)):
            with open(file_list[i], "r") as file:
                output_file.write(file.read())


def pull_build_hermes():
    result = git_clone(HERMES_GIT_URL, HERMES_CLONE_PATH)
    if not result:
        return False
    for cmd in BUILD_HERMES_CMDS:
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        while True:
            output = p.stdout.readline().decode()
            if output == '' and p.poll() is not None:
                break
            if output:
                write_log_file(HERMES_BUILD_LOG_PATH, f'{output.strip()} \n')
        if p.returncode:
            print(f"\n error: build hermes failed. {' '.join(cmd)}")
            return False
    return True


def pull_cases():
    for case_git_info in CASE_URLS:
        case_dir_name = case_git_info.get("name", None)
        case_url = case_git_info.get("url", None)
        if not case_dir_name or not case_url:
            continue
        dir_path = os.path.join(CUR_FILE_DIR, DEFAULT_TESTCASES_DIR, case_dir_name)
        if case_dir_name != SUNSPIDER:
            clear_folder_shutil(dir_path)
            clone_result = git_clone(case_url, dir_path)
            if not clone_result:
                return False
        if case_dir_name == KRAKEN:
            kraken_case_path = os.path.join(CUR_FILE_DIR, DEFAULT_TESTCASES_DIR, SUNSPIDER)
            shutil.copytree(dir_path, kraken_case_path)
        if case_dir_name == KRAKEN:
            tests_dir_path = os.path.join(dir_path, KRAKENBENCHMARK_CASE_PATH, KRAKEN_CASE_PATH)
            filter_list = CASE_LIST.get(KRAKEN)
        elif case_dir_name == SUNSPIDER:
            tests_dir_path = os.path.join(dir_path, KRAKENBENCHMARK_CASE_PATH, SUNSPIDER_CASE_PATH)
            filter_list = CASE_LIST.get(SUNSPIDER)
        else:
            tests_dir_path = dir_path
            filter_list = CASE_LIST.get(case_dir_name)
        filter_case(case_dir_name, tests_dir_path, dir_path, filter_list)
    return True


def filter_case(case_dir_name, tests_dir_path, dir_path, case_list):
    del_file_paths = []
    if case_dir_name in [SUNSPIDER, KRAKEN]:
        for file in os.listdir(tests_dir_path):
            del_file_paths.append(process_file(file, case_list, dir_path, tests_dir_path))
    for file in os.listdir(dir_path):
        if file not in case_list:
            del_file_paths.append(os.path.join(dir_path, file))
        else:
            case_path = os.path.join(dir_path, file)
            case_data = case_path.replace(JS_FILE_SUFFIX, CASE_DATA_SUFFIX)
            if os.path.exists(case_data):
                merge_files([case_path, case_data])
    delete_files(del_file_paths)


def process_file(file, case_list, dir_path, tests_dir_path):
    if file not in case_list:
        return os.path.join(dir_path, file)
    else:
        need_case_path = os.path.join(tests_dir_path, file)
        case_data_file = file.replace(JS_FILE_SUFFIX, CASE_DATA_SUFFIX)
        need_case_data_path = os.path.join(tests_dir_path, case_data_file)
        if os.path.exists(need_case_data_path):
            merge_files([need_case_path, need_case_data_path])
        shutil.move(need_case_path, dir_path)
        return ''


def delete_files(file_paths):
    for file_path in file_paths:
        if not file_path:
            continue
        if os.path.isdir(file_path):
            remove_dir(file_path)
        else:
            remove_file(file_path)


def write_html(data, save_path, selected_params, info=False):
    html_report = HTML_TEMPLATE + HTML_TABLE_DEFAULT
    default_engine = True
    result_data = data[ES2ABC]
    result_data_len = len(result_data)
    if len(data) > 1:
        html_report = HTML_TEMPLATE + HTML_TABLE_COMP
        default_engine = False
        hermes_data = data[HERMES]
        if result_data_len == len(hermes_data):
            for i in range(result_data_len):
                result_data[i] += hermes_data[i]
        else:
            print("Failed to generate comparison results, hermes or es2abc data is missing")
            return False
    html_report = html_report.replace(SELECTED_PARAMETERS_BASE, selected_params)
    if info:
        html_report = html_report.replace(DEFAULT_TIME, AVERAGE_TIME)
    for row_data in result_data:
        if not info:
            html_report = f"""{html_report}<tr><td><a href="#" onclick="redirect(\'{row_data[0]}\')">{row_data[0]}</a>
                        </td><td>{row_data[4]}</td><td>{row_data[1]} ms</td><td>{row_data[2]}</td>"""
            if not default_engine:
                html_report = f"{html_report}<td>{row_data[6]} ms</td><td>{row_data[7]}</td>"
        else:
            html_report = (f"{html_report}<tr><td>{row_data[0]}</td><td>{row_data[3]}</td><td>{row_data[1]} ms</td>"
                           f"<td>{row_data[2]}</td>")
            if not default_engine:
                html_report = f"{html_report}<td>{row_data[5]} ms</td><td>{row_data[6]}</td>"
        html_report = f"{html_report}</tr>"
    html_report = f'{html_report}{HTML_SCRIPT}'
    with open(save_path, 'w') as f:
        f.write(html_report)
    return True

