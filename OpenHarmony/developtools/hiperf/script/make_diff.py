#!/usr/bin/env python
# -*- coding: utf-8 -*-
#   Copyright (c) 2021 Huawei Device Co., Ltd.
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

import os
import sys
import time
import argparse
from hiperf_utils import get_lib
from hiperf_utils import dir_check
from hiperf_utils import file_check


def get_used_binaries(perf_data_base, perf_data_target, report_file,
                      local_lib_base, local_lib_target):
    if local_lib_base:
        get_lib().ReportUnwindJson(perf_data_base.encode("utf-8"),
                                   'json_base.txt'.encode("utf-8"),
                                   local_lib_base.encode("utf-8"))
    else:
        get_lib().ReportJson(perf_data_base.encode("utf-8"),
                             'json_base.txt'.encode("utf-8"))

    if local_lib_target:
        get_lib().ReportUnwindJson(perf_data_target.encode("utf-8"),
                                   'json_target.txt'.encode("utf-8"),
                                   local_lib_target.encode("utf-8"))
    else:
        get_lib().ReportJson(perf_data_target.encode("utf-8"),
                             'json_target.txt'.encode("utf-8"))
    time.sleep(2)

    with open('json_base.txt', 'r') as json_file:
        all_json_base = json_file.read()
    with open('json_target.txt', 'r') as json_file:
        all_json_target = json_file.read()

    with open('report-diff.html', 'r', encoding='utf-8') as html_file:
        html_str = html_file.read()

    with open(report_file, 'w', encoding='utf-8') as report_html_file:
        combine_html(all_json_base, all_json_target)
        report_html_file.write(html_str + combine_html(all_json_base, all_json_target))

    dirname, _ = os.path.split(os.path.abspath(sys.argv[0]))
    abs_path = os.path.join(dirname, report_file)
    print("save to %s success" % abs_path)
    os.remove("json_base.txt")
    os.remove("json_target.txt")



def combine_html(json_1, json_2):
    return '\n<script id = "record_data_diff_1" type = "application/json">' + \
           json_1 + '</script>\n' \
                    '<script id = "record_data_diff_2" type = "application/json">' + \
           json_2 + '</script> </body> </html>'


def main():
    parser = argparse.ArgumentParser(description=""" To make a report, you 
    need to enter the data source and the path of the report.""")
    parser.add_argument('-b', '--base', default='perf.data',
                        type=file_check, help=""" The path of base profiling 
                        data.""")
    parser.add_argument('-t', '--target', default='perf.data',
                        type=file_check, help=""" The path of target profiling 
                            data.""")
    parser.add_argument('-r', '--report_html', default='hiperf_report.html',
                        help="""the path of the report.""")
    parser.add_argument('-lb', '--lib_base_dir', type=dir_check,
                        help="""Path to find symbol dir use to
                         base data offline unwind stack""")
    parser.add_argument('-lt', '--lib_target_dir', type=dir_check,
                        help="""Path to find symbol dir use to
                            target data offline unwind stack""")
    args = parser.parse_args()

    get_used_binaries(args.base,args.target, args.report_html,
                      args.lib_base_dir, args.lib_target_dir)


if __name__ == '__main__':
    main()
