#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright (c) 2023 Huawei Device Co., Ltd.
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


import os
import shutil
import subprocess
import sys
import time
import traceback
import zipfile

import json5

import performance_config


class PerformanceBuild():
    def __init__(self, config_input, mail_obj):
        self.config = None
        self.first_line_in_avg_excel = ""
        self.preview_all_time_dic = {}
        self.preview_avg_time_dic = {}
        self.time_avg_dic = {}
        self.all_time_dic = {}
        self.size_avg_dic = {}
        self.all_size_dic = {}
        self.mail_helper = None
        self.mail_msg = ''
        self.mail_helper = mail_obj
        self.config = config_input
        self.prj_name = ''
        self.timeout = 1800
        self.error_log_str = ''

    @staticmethod
    def append_into_dic(key, value, dic):
        if key not in dic:
            dic[key] = []
        dic[key].append(value)

    @staticmethod
    def add_code(code_path, start_pos, end_pos, code_str, lines):
        with open(code_path, 'r+', encoding='UTF-8') as modified_file:
            content = modified_file.read()
            add_str_end_pos = content.find(end_pos)
            if add_str_end_pos == -1:
                print(f'Can not find code : {end_pos} in {code_path}, please check config')
                return
            add_str_start_pos = content.find(start_pos)
            if add_str_start_pos == -1:
                if lines == 0:
                    return
                add_str_start_pos = add_str_end_pos
            content_add = ""
            for i in range(lines, 0, -1):
                if "%d" in code_str:
                    content_add = content_add + code_str % i
                else:
                    content_add = content_add + code_str
            content = content[:add_str_start_pos] + content_add + content[add_str_end_pos:]
            modified_file.seek(0)
            modified_file.write(content)
            modified_file.truncate()

    @staticmethod
    def add_row(context):
        return rf'<tr align="center">{context}</tr>'

    @staticmethod
    def add_td(context):
        return rf'<td>{context}</td>'

    @staticmethod
    def add_th(context):
        return rf'<th  width="30%">{context}</th>'

    @staticmethod
    def test_type_title(context):
        return rf'<tr><th bgcolor="PaleGoldenRod" align="center" colspan="3">{context}</th></tr>'

    @staticmethod
    def app_title(context):
        return rf'<th bgcolor="SkyBlue" colspan="3"><font size="4">{context}</font></th>'

    def start(self):
        self.init()
        self.start_test()
        self.write_mail_msg()
        os.chdir(self.config.project_path)

    def init(self):
        if self.config.ide == performance_config.IdeType.DevEco:
            os.environ['path'] = self.config.node_js_path + ";" + os.environ['path']
        os.chdir(self.config.project_path)
        os.environ['path'] = os.path.join(self.config.jbr_path, "bin") + ";" + os.environ['path']
        os.environ['JAVA_HOME'] = self.config.jbr_path
        self.config.cmd_prefix = os.path.join(self.config.project_path, self.config.cmd_prefix)
        self.config.debug_package_path = os.path.join(self.config.project_path, self.config.debug_package_path)
        self.config.release_package_path = os.path.join(self.config.project_path, self.config.release_package_path)
        self.config.incremental_code_path = os.path.join(self.config.project_path, self.config.incremental_code_path)
        self.config.json5_path = os.path.join(self.config.project_path, self.config.json5_path)
        if self.config.developing_test_data_path:
            self.config.build_times = 3
        else:
            cmd = [self.config.node_js_path, self.config.cmd_prefix, "--stop-daemon"]
            print(f'cmd: {cmd}')
            subprocess.Popen(cmd, stderr=sys.stderr,
                             stdout=sys.stdout).communicate(timeout=self.timeout)

    def add_incremental_code(self, lines):
        PerformanceBuild.add_code(self.config.incremental_code_path,
                self.config.incremental_code_start_pos,
                self.config.incremental_code_end_pos,
                self.config.incremental_code_str,
                lines)

    def revert_incremental_code(self):
        self.add_incremental_code(0)

    def reset(self):
        self.first_line_in_avg_excel = ""
        self.preview_all_time_dic = {}
        self.preview_avg_time_dic = {}
        self.time_avg_dic = {}
        self.all_time_dic = {}
        self.size_avg_dic = {}
        self.all_size_dic = {}
        self.error_log_str = ''
        self.revert_incremental_code()

    def clean_project(self):
        if not self.config.developing_test_data_path:
            cmd = [self.config.node_js_path, self.config.cmd_prefix, "clean"]
            print(f'cmd: {cmd}')
            subprocess.Popen(cmd, stderr=sys.stderr,
                             stdout=sys.stdout).communicate(timeout=self.timeout)

    def get_bytecode_size(self, is_debug):
        if self.config.developing_test_data_path:
            # test data for size
            PerformanceBuild.append_into_dic("ets/mudules.abc rawSize", 44444, self.all_size_dic)
            PerformanceBuild.append_into_dic("ets/mudules.abc Compress_size", 33333, self.all_size_dic)
            PerformanceBuild.append_into_dic("ets/mudules2.abc rawSize", 44444, self.all_size_dic)
            PerformanceBuild.append_into_dic("ets/mudules2.abc Compress_size", 33333, self.all_size_dic)
            return
        package_path = self.config.debug_package_path if is_debug else self.config.release_package_path
        package = zipfile.ZipFile(package_path)
        extension_name = ".abc" if self.config.ide == performance_config.IdeType.DevEco else ".dex"
        for info in package.infolist():
            if info.filename.endswith(extension_name):
                name_str1 = info.filename + " rawSize"
                name_str2 = info.filename + " compress_size"
                PerformanceBuild.append_into_dic(name_str1, info.file_size, self.all_size_dic)
                PerformanceBuild.append_into_dic(name_str2, info.compress_size, self.all_size_dic)

    def get_build_time(self, report_path, time_dic):
        event_obj = None
        with open(report_path, 'r+', encoding='UTF-8') as report:
            event_obj = json5.load(report)['events']
        if not event_obj:
            raise Exception('Open report json failed')
        found_error = False
        for node in event_obj:
            if node['head']['type'] == "log" and node['additional']['logType'] == 'error':
                self.error_log_str = self.error_log_str + node['head']['name']
                found_error = True
            if found_error:
                continue

            build_time = 0
            task_name = node['head']['name']
            if node['head']['type'] == "mark":
                if node['additional']['markType'] == 'history':
                    build_time = (node['body']['endTime'] - node['body']['startTime']) / 1000000000
                    task_name = "total build cost"
                else:
                    continue
            elif node['head']['type'] == "continual":
                build_time = node['additional']['totalTime'] / 1000000000
            else:
                continue
            PerformanceBuild.append_into_dic(task_name, build_time, time_dic)
        if found_error:
            raise Exception('Build Failed')

    def collect_preview_build_data(self, report_path):
        self.get_build_time(report_path, self.preview_all_time_dic)

    def collect_build_data(self, is_debug, report_path):
        self.get_build_time(report_path, self.all_time_dic)
        self.get_bytecode_size(is_debug)

    def get_report_path(self, cmd_suffix):
        reports_before = []
        report_dir = '.hvigor/report'
        if os.path.exists(report_dir):
            reports_before = os.listdir(report_dir)
        cmd = [self.config.node_js_path, self.config.cmd_prefix, *cmd_suffix]
        print(f'cmd: {cmd}')
        subprocess.Popen(cmd, stderr=sys.stderr,
                         stdout=sys.stdout).communicate(timeout=self.timeout)
        report_path = (set(os.listdir(report_dir)) - set(reports_before)).pop()
        return os.path.join(report_dir, report_path)

    def preview_build(self, is_debug):
        cache_path = os.path.join(self.config.project_path, *self.config.preview_cache_path)
        if os.path.exists(cache_path):
            shutil.rmtree(cache_path)
        cmd_suffix = self.config.preview_debug_suffix if is_debug else self.config.preview_release_suffix
        report_path = self.get_report_path(cmd_suffix)
        self.collect_preview_build_data(report_path)

    def build(self, is_debug):
        cmd_suffix = self.config.cmd_debug_suffix if is_debug else self.config.cmd_release_suffix
        report_path = self.get_report_path(cmd_suffix)
        self.collect_build_data(is_debug, report_path)

    def start_build(self, is_debug):
        if self.config.developing_test_data_path:
            # test data
            self.collect_build_data(is_debug, os.path.join(os.path.dirname(__file__),
                                                           self.config.developing_test_data_path))
            return True

        self.build(is_debug)
        return True

    def get_millisecond(self, time_string):
        if self.config.ide != performance_config.IdeType.DevEco and not self.config.developing_test_data_path:
            return int(time_string)
        else:
            cost_time = 0
            res = time_string.split(" min ")
            target_str = ""
            if len(res) > 1:
                cost_time = int(res[0]) * 60000
                target_str = res[1]
            else:
                target_str = res[0]
            res = target_str.split(" s ")
            if len(res) > 1:
                cost_time = cost_time + int(res[0]) * 1000
                target_str = res[1]
            else:
                target_str = res[0]
            res = target_str.split(" ms")
            if len(res) > 1:
                cost_time = cost_time + int(res[0])
            return cost_time

    def cal_preview_build_avg_time(self):
        self.first_line_in_avg_excel = self.first_line_in_avg_excel + "\n"
        for key in self.preview_all_time_dic:
            task_count = len(self.preview_all_time_dic[key])
            has_task = True
            if task_count != 2 * self.config.build_times:
                if task_count == self.config.build_times:
                    has_task = False
                else:
                    continue
            # average of first build
            sum_build_time = 0
            for i in range(0, self.config.build_times):
                index = i * 2
                if not has_task:
                    self.preview_all_time_dic[key].insert(index + 1, 0)
                sum_build_time = sum_build_time + self.preview_all_time_dic[key][index]
            cost = round(sum_build_time / self.config.build_times, 2)
            PerformanceBuild.append_into_dic(key, cost, self.preview_avg_time_dic)
            # average of incremental build
            sum_build_time = 0
            for i in range(1, len(self.preview_all_time_dic[key]), 2):
                sum_build_time = sum_build_time + self.preview_all_time_dic[key][i]
            cost = round(sum_build_time / self.config.build_times, 2)
            PerformanceBuild.append_into_dic(key, cost, self.preview_avg_time_dic)

    def cal_incremental_avg_time(self, all_time_dic, avg_time_dic):
        self.first_line_in_avg_excel = self.first_line_in_avg_excel + "\n"
        for key in all_time_dic:
            task_count = len(all_time_dic[key])
            has_task = True
            if task_count != 2 * self.config.build_times:
                if task_count == self.config.build_times:
                    has_task = False
                else:
                    continue
            # average of first build
            sum_build_time = 0
            for i in range(0, self.config.build_times):
                index = i * 2
                if not has_task:
                    all_time_dic[key].insert(index + 1, 0)
                sum_build_time = sum_build_time + all_time_dic[key][index]
            cost = round(sum_build_time / self.config.build_times, 2)
            PerformanceBuild.append_into_dic(key, cost, avg_time_dic)
            # average of incremental build
            sum_build_time = 0
            for i in range(1, len(all_time_dic[key]), 2):
                sum_build_time = sum_build_time + all_time_dic[key][i]
            cost = round(sum_build_time / self.config.build_times, 2)
            PerformanceBuild.append_into_dic(key, cost, avg_time_dic)

    def cal_incremental_avg_size(self):
        total_raw_size = []
        total_compressed_size = []
        for i in range(0, self.config.build_times * 2):
            total_raw_size.append(0)
            total_compressed_size.append(0)
            for key in self.all_size_dic:
                if "raw" in key:
                    total_raw_size[i] += self.all_size_dic[key][i]
                else:
                    total_compressed_size[i] += self.all_size_dic[key][i]
        self.all_size_dic["total_raw_size"] = total_raw_size
        self.all_size_dic["total_compressed_size"] = total_compressed_size
        for key in self.all_size_dic:
            # sizes should be the same, just check
            full_first_size = self.all_size_dic[key][0]
            for i in range(0, len(self.all_size_dic[key]), 2):
                if full_first_size != self.all_size_dic[key][i]:
                    full_first_size = -1
                    break
            PerformanceBuild.append_into_dic(key, full_first_size, self.size_avg_dic)

            incremental_first_size = self.all_size_dic[key][1]
            for i in range(1, len(self.all_size_dic[key]), 2):
                if incremental_first_size != self.all_size_dic[key][i]:
                    incremental_first_size = -1
                    break
            PerformanceBuild.append_into_dic(key, incremental_first_size, self.size_avg_dic)

    def cal_incremental_avg(self):
        if self.config.preview_build:
            self.cal_incremental_avg_time(self.preview_all_time_dic, self.preview_avg_time_dic)
        self.cal_incremental_avg_time(self.all_time_dic, self.time_avg_dic)
        self.cal_incremental_avg_size()

    def add_preview_build_time_pic_data(self, dic, is_debug):
        for key in dic:
            if "total" in key:
                preview_build_time = dic[key][0]
                break
        self.mail_helper.add_pic_data(self.config.name, is_debug, [preview_build_time], 2)

    def add_time_pic_data(self, dic, is_debug):
        for key in dic:
            if "total" in key:
                full_time = dic[key][0]
                incremental_time = dic[key][1]
                break
        self.mail_helper.add_pic_data(self.config.name, is_debug, [full_time, incremental_time], 0)

    def add_size_pic_data(self, dic, is_debug):
        for key in dic:
            full_size = dic[key][0]
        self.mail_helper.add_pic_data(self.config.name, is_debug, [full_size], 3)

    def write_mail_files(self, dic):
        if not hasattr(self.config, 'show_time_detail_filter'):
            return ''
        msg = ''
        rows = ''
        first_row = ""
        first_line_res = self.first_line_in_avg_excel.replace("\n", "").split(",")
        for i in first_line_res:
            first_row += PerformanceBuild.add_th(i)
        rows += PerformanceBuild.add_row(first_row)

        show_dic = []
        for k in self.config.show_time_detail_filter:
            if k in dic:
                show_dic.append(k)
        for key in show_dic:
            content_row = PerformanceBuild.add_th(key)
            for v in dic[key]:
                content_row += PerformanceBuild.add_td(f'{v} s')
            rows += PerformanceBuild.add_row(content_row)
        msg += rows
        return msg

    def write_from_dic(self, file_path, first_line, dic):
        content_list = []
        if first_line:
            content_list.append(first_line)
        for key in dic:
            content_list.append(key)
            for v in dic[key]:
                content_list.append(",")
                content_list.append(str(v))
            content_list.append("\n")
        content = "".join(content_list)
        self.mail_helper.add_logs_file(file_path, content.encode())

    def write_logs_from_dic(self, path_prefix, log_filename, source_dic, need_first_line):
        file_path = self.config.output_split.join((path_prefix, log_filename))
        file_path = os.path.join(self.prj_name, file_path)
        first_line = self.first_line_in_avg_excel if need_first_line else None
        self.write_from_dic(file_path, first_line, source_dic)
        return

    def generate_full_and_incremental_results(self, is_debug):
        path_prefix = self.config.output_split.join(
            (self.config.ide_filename[self.config.ide - 1],
            self.config.debug_or_release[0 if is_debug else 1],
            self.config.build_type_of_log[0])
        )
        temp_mail_msg = ""
        # write all build time log
        self.write_logs_from_dic(path_prefix, self.config.log_filename[2], self.all_time_dic, False)
        # write avg build time, html msg and picture data
        self.write_logs_from_dic(path_prefix, self.config.log_filename[3], self.time_avg_dic, True)
        temp_mail_msg += self.write_mail_files(self.time_avg_dic)
        # write preview avg build time
        if self.config.preview_build:
            self.add_preview_build_time_pic_data(self.preview_avg_time_dic, is_debug)
        self.add_time_pic_data(self.time_avg_dic, is_debug)
        # write all size of abc log
        self.write_logs_from_dic(path_prefix, self.config.log_filename[0], self.all_size_dic, False)
        # write avg abc size, html msg and picture data
        self.write_logs_from_dic(path_prefix, self.config.log_filename[1], self.size_avg_dic, True)
        self.add_size_pic_data(self.size_avg_dic, is_debug)

        # write html message
        if self.config.send_mail and hasattr(self.config, 'show_time_detail_filter'):
            temp_mail_msg = '<table width="100%" border=1 cellspacing=0 cellpadding=0 align="center">' + \
                PerformanceBuild.app_title(self.config.name + (' Debug' if is_debug else ' Release')) + \
                temp_mail_msg + '</table>'
            self.mail_msg += temp_mail_msg

    def error_handle(self, is_debug, log_type):
        build_mode = 'Debug' if is_debug else 'Release'
        log_type_str = 'full_build' if log_type == performance_config.LogType.FULL else 'incremental_build'
        self.mail_helper.add_failed_project(self.prj_name, build_mode, log_type_str)
        save_name = build_mode + '_' + os.path.basename(self.config.error_filename)
        print(self.error_log_str)
        self.mail_helper.add_logs_file(os.path.join(self.prj_name, save_name),
                                       self.error_log_str)

    def full_and_incremental_build(self, is_debug):
        log_type = performance_config.LogType.FULL
        try:
            self.reset()
            self.prj_name = os.path.basename(self.config.project_path)
            self.first_line_in_avg_excel = self.first_line_in_avg_excel + ",first build,incremental build"
            for i in range(self.config.build_times):
                self.clean_project()
                print(f"fullbuild: {'Debug' if is_debug else 'Release'}, {i + 1}/{self.config.build_times}")
                log_type = performance_config.LogType.FULL
                if self.config.preview_build:
                    self.preview_build(is_debug)
                self.start_build(is_debug)
                self.add_incremental_code(1)
                print(f"incremental: {'Debug' if is_debug else 'Release'}, {i + 1}/{self.config.build_times}")
                log_type = performance_config.LogType.INCREMENTAL
                self.start_build(is_debug)
                self.revert_incremental_code()
            self.cal_incremental_avg()
            self.generate_full_and_incremental_results(is_debug)
        except Exception as e:
            err_msg = traceback.format_exc()
            self.error_log_str = f'error:\n{self.error_log_str}\n{err_msg}'
            self.error_handle(is_debug, log_type)

    def start_test(self):
        self.full_and_incremental_build(True)
        self.full_and_incremental_build(False)
        self.reset()

    def write_mail_msg(self):
        if self.config.send_mail:
            self.mail_helper.add_msg(self.mail_msg)


def run(config_input, mail_obj):
    start_time = time.time()
    PerformanceBuild(config_input, mail_obj).start()
    print("Test [%s] finished at: %s\n" \
          "total cost: %ds"
          % (os.path.basename(config_input.project_path),
             time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()),
             time.time() - start_time))