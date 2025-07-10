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

Description: output test results
"""

import copy
import logging
import os
import time

import pandas

import options

full_compile_tests = ["full_compile",
                      "import_ordinary_ohpm_package",
                      "import_special_ohpm_package",
                      "import_static_library",
                      "import_share_library",
                      "import_so_file",
                      "has_syntax_error_in_js",
                      "use_normalize_ohmurl",
                      "module_name_is_inconsistent",
                      ]

incremental_compile_tests = ["no_change",
                             "add_oneline",
                             "add_file",
                             "add_nonexistent_file",
                             "delete_file",
                             "modify_error_then_fix",
                             "add_error_page_then_fix",
                             "add_error_non_page_then_fix",
                             "build_entry_then_har",
                             "build_har_then_entry",
                             "build_entry_then_hsp",
                             "build_hsp_then_entry",
                             "build_hsp_then_ohos",
                             "build_entry_then_ohos",
                             "build_entry_then_preview_build",
                             "reverse_hap_mode",
                             "change_module_name",
                             "modify_sdk_version",
                             ]

bytecode_har_compile_tests = ["build_bytecode_har",
                               "build_har_then_bytecode_har",
                               "import_bytecode_static_library",
                               ]

external_compile_tests = ["import_external_share_library",
                          "import_external_static_library",
                          "full_compile_external_static_library",
                          "full_compile_external_share_library"
                          ]

preview_compile_tests = ["preview_compile",
                         "build_entry_then_preview",
                         "build_modify_file_name",
                         "build_generate_sourcemap",
                         "tigger_incremental_build",
                         "has_arkui_error",
                         "sdk_path_has_special_char",
                         "modify_hello_world_then_fix"
                         ]

other_tests = ["binary_consistency",
               "break_continue_compile",
               "compile_with_error",
               "compile_with_exceed_length",
               "ohos_test"
               ]


class TestResult:
    def __init__(self):
        self.passed = []
        self.failed = []
        self.time = 0.0


def print_result(test_result, test_tasks):
    logging.info("========================================")
    logging.info("Test finished. The result is as following:")
    logging.info("=====> Summary")
    logging.info("Total test number: %s, took time: %.3f s",
                 len(test_tasks), test_result.time)
    logging.info("Passed test number: %s", len(test_result.passed))
    logging.info("Failed test number: %s", len(test_result.failed))

    logging.info("=====> Detail Information")
    logging.info("-----")
    idx = 1
    for task in test_tasks:
        logging.info("task index: %d", idx)
        idx = idx + 1
        logging.info("task name: %s", task.name)
        logging.info("task type: %s", task.type)
        # print full compile result
        logging.info("--full compilation result:")
        for full_task in task.full_compilation_info.values():
            logging.info("full test: %s", full_task.name)
            logging.info("debug: %s, abc_size(byte) %s, time(s) %s, error message: %s",
                         full_task.debug_info.result,
                         full_task.debug_info.abc_size,
                         full_task.debug_info.time,
                         full_task.debug_info.error_message)
            logging.info("release: %s, abc_size(byte) %s, time(s) %s, error message: %s",
                         full_task.release_info.result,
                         full_task.release_info.abc_size,
                         full_task.release_info.time,
                         full_task.debug_info.error_message)

        # print incremental compile result
        logging.info("--incremental compilation result:")
        for inc_task in task.incre_compilation_info.values():
            logging.info("incre test: %s", inc_task.name)
            logging.info("debug: %s, abc_size(byte) %s, time(s) %s, error message: %s",
                         inc_task.debug_info.result,
                         inc_task.debug_info.abc_size,
                         inc_task.debug_info.time,
                         inc_task.debug_info.error_message)
            logging.info("release: %s, abc_size(byte) %s, time(s) %s, error message: %s",
                         inc_task.release_info.result,
                         inc_task.release_info.abc_size,
                         inc_task.release_info.time,
                         inc_task.release_info.error_message)

        # print bytecode har compile result
        logging.info("--bytecode har compilation result:")
        for byte_task in task.bytecode_har_compilation_info.values():
            logging.info("bytecode har test: %s", byte_task.name)
            logging.info("debug: %s, abc_size(byte) %s, time(s) %s, error message: %s",
                         byte_task.debug_info.result,
                         byte_task.debug_info.abc_size,
                         byte_task.debug_info.time,
                         byte_task.debug_info.error_message)
            logging.info("release: %s, abc_size(byte) %s, time(s) %s, error message: %s",
                         byte_task.release_info.result,
                         byte_task.release_info.abc_size,
                         byte_task.release_info.time,
                         byte_task.release_info.error_message)

        # print external compile result
        logging.info("--external compilation result:")
        for external_task in task.external_compilation_info.values():
            logging.info("external test: %s", external_task.name)
            logging.info("debug: %s, abc_size(byte) %s, time(s) %s, error message: %s",
                         external_task.debug_info.result,
                         external_task.debug_info.abc_size,
                         external_task.debug_info.time,
                         external_task.debug_info.error_message)
            logging.info("release: %s, abc_size(byte) %s, time(s) %s, error message: %s",
                         external_task.release_info.result,
                         external_task.release_info.abc_size,
                         external_task.release_info.time,
                         external_task.release_info.error_message)

        # print preview compile result
        for name, task_info in task.preview_compilation_info.items():
            logging.info("--test name: %s", name)
            logging.info("result: %s, error message: %s",
                         task_info.result,
                         task_info.error_message)

        # print other tests result
        for name, task_info in task.other_tests.items():
            logging.info("--test name: %s", name)
            logging.info("result: %s, error message: %s",
                         task_info.result,
                         task_info.error_message)

        logging.info("-----")
        logging.info("========================================")


def is_compilation_passed(task_info, compile_mode):
    if options.arguments.compile_mode not in ['all', compile_mode]:
        return True

    if len(task_info) == 0:
        return False

    passed_debug = True
    passed_release = True
    for task_name, inc_task in task_info.items():
        if options.arguments.hap_mode in ['all', 'release']:
            if inc_task.release_info.result == options.TaskResult.undefined:
                continue
            passed_release = passed_release and inc_task.release_info.result == options.TaskResult.passed
        if options.arguments.hap_mode == ['all', 'debug']:
            if inc_task.debug_info.result == options.TaskResult.undefined:
                continue
            passed_debug = passed_debug and inc_task.debug_info.result == options.TaskResult.passed

    return passed_debug and passed_release


def is_task_passed(task):
    passed = is_compilation_passed(task.full_compilation_info, 'full') and \
             is_compilation_passed(task.incre_compilation_info, 'incremental') and \
             is_compilation_passed(task.bytecode_har_compilation_info, 'bytecode_har') and \
             is_compilation_passed(task.external_compilation_info, 'external')

    for test in task.preview_compilation_info.values():
        passed = passed and (test.result == options.TaskResult.passed)

    for test in task.other_tests.values():
        passed = passed and (test.result == options.TaskResult.passed)

    return passed


def collect_result(test_result, test_tasks, start_time):
    for task in test_tasks:
        if not is_task_passed(task):
            test_result.failed.append(task)
        else:
            test_result.passed.append(task)

    end_time = time.time()
    test_result.time = round(end_time - start_time, 3)


def get_result_symbol(result_type):
    if result_type == options.TaskResult.passed:
        return '√'
    elif result_type == options.TaskResult.failed:
        return '×'
    else:
        return '-'


def generate_summary_data(test_result, test_tasks):
    # collect summary data
    passed_task_name_list = []
    for task in test_result.passed:
        passed_task_name_list.append(task.name)
    failed_task_name_list = []
    for task in test_result.failed:
        failed_task_name_list.append(task.name)

    summary_data = {
        'Total Test Number': len(test_tasks),
        'Passed Test Number': len(test_result.passed),
        'Failed Test Number': len(test_result.failed),
        'Passed Tests': ','.join(passed_task_name_list),
        'Failed Tests': ','.join(failed_task_name_list),
        'Test Took Time(s)': test_result.time
    }

    return summary_data


def generate_detail_data(test_tasks):
    time_size_data = []
    result_data = []

    idx = 0
    for task in test_tasks:
        idx += 1
        task_time_size_data = {
            'Task Index': idx,
            'Task Name': task.name
        }
        task_result_data = copy.deepcopy(task_time_size_data)
        task_result_data['Task Type'] = ','.join(task.type)

        full_compilation_debug, full_compilation_release = get_full_build_test_result(task, task_result_data,
                                                                                      task_time_size_data)

        get_incremental_build_test_result(task, task_result_data, task_time_size_data)

        get_bytecode_har_build_test_result(task, task_result_data, task_time_size_data)

        get_external_build_test_result(task, task_result_data, task_time_size_data)

        get_preview_build_test_result(task, task_result_data)

        get_other_test_result(task, task_result_data)

        task_time_size_data['[Abc Size(byte)]\n[Debug]'] = full_compilation_debug.abc_size
        task_time_size_data['[Abc Size(byte)]\n[Release]'] = full_compilation_release.abc_size
        time_size_data.append(task_time_size_data)
        result_data.append(task_result_data)

    detail_data = {
        'result_data': result_data,
        'time_size_data': time_size_data
    }
    return detail_data


def get_test_tesult(test, task_result_data, compilation_info):
    debug_result = options.TaskResult.undefined
    debug_runtime_result = options.TaskResult.undefined
    release_result = options.TaskResult.undefined
    release_runtime_result = options.TaskResult.undefined
    if test in compilation_info.keys():
        task_info = compilation_info[test]
        debug_result = task_info.debug_info.result
        debug_runtime_result = task_info.debug_info.runtime_result
        release_result = task_info.release_info.result
        release_runtime_result = task_info.release_info.runtime_result
    task_result_data[f'[Debug]\n{test}'] = get_result_symbol(
        debug_result)
    task_result_data[f'[Debug-runtime]\n{test}'] = get_result_symbol(
        debug_runtime_result)
    task_result_data[f'[Release]\n{test}'] = get_result_symbol(
        release_result)
    task_result_data[f'[Release-runtime]\n{test}'] = get_result_symbol(
        release_runtime_result)


def get_full_build_test_result(task, task_result_data, task_time_size_data):
    for test in full_compile_tests:
        get_test_tesult(test, task_result_data, task.full_compilation_info)

        if test == 'full_compile':
            debug_test_time = 0
            release_test_time = 0
            if test in task.full_compilation_info.keys():
                full_task_info = task.full_compilation_info[test]
                debug_test_time = full_task_info.debug_info.time
                release_test_time = full_task_info.release_info.time

            task_time_size_data[
                '[Full Compilation]\n[Debug]\n[Compilation Time(s)]'] = debug_test_time
            task_time_size_data[
                '[Full Compilation]\n[Release]\n[Compilation Time(s)]'] = release_test_time

    return (task.full_compilation_info['full_compile'].debug_info,
            task.full_compilation_info['full_compile'].release_info)


def get_incremental_build_test_result(task, task_result_data, task_time_size_data):
    for test in incremental_compile_tests:
        get_test_tesult(test, task_result_data, task.incre_compilation_info)

        if test == 'add_oneline':
            debug_test_time = 0
            release_test_time = 0
            if test in task.incre_compilation_info.keys():
                inc_task_info = task.incre_compilation_info[test]
                debug_test_time = inc_task_info.debug_info.time
                release_test_time = inc_task_info.release_info.time

            task_time_size_data[
                '[Incremental Compilation]\n[Debug]\n[Compilation Time(s)]'] = debug_test_time
            task_time_size_data[
                '[Incremental Compilation]\n[Release]\n[Compilation Time(s)]'] = release_test_time


def get_bytecode_har_build_test_result(task, task_result_data, task_time_size_data):
    for test in bytecode_har_compile_tests:
        get_test_tesult(test, task_result_data, task.bytecode_har_compilation_info)

        if test == 'build_bytecode_har':
            debug_test_time = 0
            release_test_time = 0
            if test in task.bytecode_har_compilation_info.keys():
                inc_task_info = task.bytecode_har_compilation_info[test]
                debug_test_time = inc_task_info.debug_info.time
                release_test_time = inc_task_info.release_info.time

            task_time_size_data[
                '[Bytecode Har Compilation]\n[Debug]\n[Compilation Time(s)]'] = debug_test_time
            task_time_size_data[
                '[Bytecode Har Compilation]\n[Release]\n[Compilation Time(s)]'] = release_test_time


def get_external_build_test_result(task, task_result_data, task_time_size_data):
    for test in external_compile_tests:
        get_test_tesult(test, task_result_data, task.external_compilation_info)

        if test == 'import_external_share_library':
            debug_test_time = 0
            release_test_time = 0
            if test in task.external_compilation_info.keys():
                inc_task_info = task.external_compilation_info[test]
                debug_test_time = inc_task_info.debug_info.time
                release_test_time = inc_task_info.release_info.time

            task_time_size_data[
                '[External Compilation]\n[Debug]\n[Compilation Time(s)]'] = debug_test_time
            task_time_size_data[
                '[External Compilation]\n[Release]\n[Compilation Time(s)]'] = release_test_time


def get_preview_build_test_result(task, task_result_data):
    for test in preview_compile_tests:
        result = options.TaskResult.undefined
        runtime_result = options.TaskResult.undefined
        if test in task.preview_compilation_info.keys():
            task_info = task.preview_compilation_info[test]
            result = task_info.result
            runtime_result = task_info.runtime_result

        task_result_data[f'{test}'] = get_result_symbol(result)
        task_result_data[f'{test}-runtime'] = get_result_symbol(runtime_result)


def get_other_test_result(task, task_result_data):
    for test in other_tests:
        result = options.TaskResult.undefined
        runtime_result = options.TaskResult.undefined
        if test in task.other_tests.keys():
            task_info = task.other_tests[test]
            result = task_info.result
            runtime_result = task_info.runtime_result
        task_result_data[f'{test}'] = get_result_symbol(result)
        task_result_data[f'{test}-runtime'] = get_result_symbol(runtime_result)


def rotate_data(df):
    num_rows, num_cols = df.shape
    rotated_df = pandas.DataFrame(columns=range(num_rows), index=range(num_cols))
    for i in range(num_rows):
        for j in range(num_cols):
            rotated_df.iloc[j, i] = df.iloc[i, j]
    return rotated_df


def get_merge_data(rotated_df):
    data = rotated_df.iloc[3:, :].values.tolist()
    merged_data = []
    for i in range(0, len(data) - 1, 2):
        row = [value for sublist in zip(data[i], data[i + 1]) for value in sublist]
        merged_data.append(row)
    return merged_data


def generate_content_section(section_title, tests, start_index, merged_data):
    section_content = f'<tr><th rowspan={len(tests) * 2}>{section_title}</th>'
    for index, item in enumerate(tests):
        debug_result = ''.join([f'<th>{column}</th>' for column in merged_data[start_index]])
        section_content = ''.join(
            [section_content, f'<th rowspan="2">{item}</th><th>[Debug]</th>', debug_result, '</tr>'])
        release_result = ''.join([f'<th>{column}</th>' for column in merged_data[start_index + 1]])
        section_content = ''.join([section_content, '<tr><th>[Release]</th>', release_result, '</tr>'])
        start_index += 2
    return section_content, start_index


def get_result_table_content(result_df_rotate):
    start_index = 0
    merged_data = get_merge_data(result_df_rotate)
    # Full Compilation section
    full_compile_section, start_index = generate_content_section("Full Compilation", full_compile_tests, start_index,
                                                                 merged_data)
    content = full_compile_section

    # Incremental Compilation section
    incremental_compile_section, start_index = generate_content_section("Incremental Compilation",
                                                                        incremental_compile_tests, start_index,
                                                                        merged_data)
    content += incremental_compile_section

    # Bytecode Har Compilation section
    bytecode_har_compile_section, start_index = generate_content_section("Bytecode Har Compilation",
                                                                          bytecode_har_compile_tests, start_index,
                                                                          merged_data)
    content += bytecode_har_compile_section

    # External Compilation section
    external_compile_section, start_index = generate_content_section("External Compilation",
                                                                     external_compile_tests, start_index,
                                                                     merged_data)
    content += external_compile_section

    content += f'<tr><th colspan=2 rowspan={len(preview_compile_tests)}>Preview Compilation</th>'
    for index, item in enumerate(preview_compile_tests):
        preview_result = ''.join([f'<th>{column}</th>' for column in merged_data[start_index]])
        content = ''.join([content, f'<th>{item}</th>', preview_result, '</tr>'])
        start_index = start_index + 1

    # Other Test section
    content += f'<tr><th colspan=2 rowspan={len(other_tests)}>Other Tests</th>'
    for index, item in enumerate(other_tests):
        other_result = ''.join([f'<th>{column}</th>' for column in merged_data[start_index]])
        content = ''.join([content, f'<th>{item}</th>', other_result, '</tr>'])
        start_index = start_index + 1

    return content


def generate_data_html(summary_data, detail_data):
    # summary table
    key_value_pairs = [
        f'<tr><td>{key}</td><td>{value}</td></tr>' for key, value in summary_data.items()]
    summary_table_content = ''.join(key_value_pairs)
    summary_table = f'<table id=sdk>{summary_table_content}</table>'

    # time and size table
    time_size_data = detail_data.get('time_size_data')
    time_size_df = pandas.DataFrame(time_size_data)

    time_size_table_header = '<tr>' + \
        ''.join(
            [f'<th rowspan="2">{column}</th>' for column in time_size_df.columns[:2]])
    time_size_table_header += '<th colspan="2">Full Compilation Time(s)</th>' + \
                              f'<th colspan="2">Incremental Compilation Time(s)</th>' + \
                              f'<th colspan="2">Bytecode Har Compilation Time(s)</th>' + \
                              f'<th colspan="2">External Compilation Time(s)</th>' + \
                              f'<th colspan="2">Abc Size(byte)</th></tr>'
    time_size_table_sub_header = '<tr>' + \
        f'<th>[Debug]</th><th>[Release]</th>' * 5 + '</tr>'

    time_size_table_content = ''.join([
        '<tr>' + ''.join([f'<td>{value}</td>' for _,
                         value in row.items()]) + '</tr>'
        for _, row in time_size_df.iterrows()
    ])
    time_size_table = f'<table id=sdk> \
        {time_size_table_header}{time_size_table_sub_header}{time_size_table_content}</table>'

    # result table
    result_data = detail_data.get('result_data')
    result_df = pandas.DataFrame(result_data)
    result_df_rotate = rotate_data(result_df)

    result_table_header = '<tr><th colspan="3">Task Index</th>' + \
            ''.join(
            [f'<th colspan="2">{column}</th>' for column in result_df.iloc[:, 0].tolist()]) + '</tr>'
    result_table_header += '<tr><th colspan="3">Task Name</th>' + \
           ''.join(
               [f'<th colspan="2">{column}</th>' for column in result_df.iloc[:, 1].tolist()]) + '</tr>'
    result_table_header += '<tr><th colspan="3">Task Type</th>' + \
                           ''.join(
                               [f'<th colspan="2">{column}</th>' for column in result_df.iloc[:, 2].tolist()]) + '</tr>'
    result_table_sub_header = f"<tr><th colspan=3>Build && Run </th>{'<th>[build]</th><th>[runtime]</th>' * result_df.shape[0]}</tr>"
    result_table_content = get_result_table_content(result_df_rotate)

    result_table = f'<table id=sdk> \
        {result_table_header}{result_table_sub_header}{result_table_content}</table>'

    return summary_table, time_size_table, result_table


def get_html_style():
    html_style = '''
         #sdk body {
            font-family: Arial, sans-serif;
            margin: 20px;
        }
        #sdk h2 {
            color: #333;
        }
        #sdk {
            border-collapse: collapse;
            width: 100%;
            margin-bottom: 20px;
        }
        #sdk th, #sdk td {
            padding: 8px;
            border: 1px solid #ddd;
        }
        #sdk th {
            background-color: #f2f2f2;
            font-weight: bold;
        }
        #sdk tr:nth-child(odd) {
            background-color: #f9f9f9;
        }
    '''
    return html_style


def generate_report_html(summary_data, detail_data):
    [summary_table, time_size_table, result_table] = generate_data_html(
        summary_data, detail_data)
    html_style = get_html_style()

    html_content = f'''
    <html>
    <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <style>
    {html_style}
    </style>
    </head>
    <body>
    <h2>SDK Test Results</h2>
    <h3>Summary</h3>
    {summary_table}
    <h3>Detail Information</h3>
    <h4>Test Result</h4>
    {result_table}
    <h4>Compilation Time And Abc Size</h4>
    {time_size_table}
    <p>
    Notes:<br>
    1. Incremental compilation time refers to add-one line incremental compile.<br>
    2. For details compile output or error message during compile, please refer to attachment of log file.<br>
    3. For sdk commit tags, please refer to attachment of manifest file(to be added).
    </p>
    </body>
    </html>
    '''

    daily_report_file = options.configs.get('output_html_file')
    with open(daily_report_file, 'w', encoding='utf-8') as report:
        report.write(html_content)


def generate_log_file():
    logger = logging.getLogger()
    if not hasattr(logger.handlers[0], 'baseFilename'):
        return
    log_file = logger.handlers[0].baseFilename
    logger.handlers[0].close()
    output_log_file = options.configs.get('log_file')
    os.rename(log_file, output_log_file)


def generate_result_reports(test_result, test_tasks):
    summary_data = generate_summary_data(test_result, test_tasks)
    detail_data = generate_detail_data(test_tasks)
    generate_report_html(summary_data, detail_data)
    generate_log_file()


def process_test_result(test_tasks, start_time):
    test_result = TestResult()

    collect_result(test_result, test_tasks, start_time)
    print_result(test_result, test_tasks)
    generate_result_reports(test_result, test_tasks)

