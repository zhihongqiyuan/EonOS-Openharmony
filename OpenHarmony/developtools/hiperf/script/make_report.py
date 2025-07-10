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


def get_used_binaries(perf_data, report_file, local_lib_dir, html_template):
    if local_lib_dir:
        get_lib().ReportUnwindJson(perf_data.encode("utf-8"),
                                   'json.txt'.encode("utf-8"),
                                   local_lib_dir.encode("utf-8"))
    else:
        get_lib().ReportJson(perf_data.encode("utf-8"),
                             'json.txt'.encode("utf-8"))
    time.sleep(2)
    with open('json.txt', 'r', errors='ignore') as json_file:
        all_json = json_file.read()
        template = os.path.join(html_template, 'report.html')
    with open(template, 'r', encoding='utf-8') as html_file:
        html_str = html_file.read()
    with open(report_file, 'w', encoding='utf-8') as report_html_file:
        report_html_file.write(html_str + all_json + '</script>'
                                      ' </body>'
                                      ' </html>')
    dirname, _ = os.path.split(os.path.abspath(sys.argv[0]))
    abs_path = os.path.join(dirname, report_file)
    print("save to %s success" % abs_path)
    os.remove('json.txt')


def main():
    parser = argparse.ArgumentParser(description=""" To make a report, you 
    need to enter the data source and the path of the report.""")
    parser.add_argument('-i', '--perf_data', default='perf.data',
                        type=file_check, help=""" The path of profiling 
                        data.""")
    parser.add_argument('-r', '--report_html', default='hiperf_report.html',
                        help="""the path of the report.""")
    parser.add_argument('-l', '--local_lib_dir', type=dir_check, default='./binary_cache',
                        help="""Path to find symbol dir use to
                         do offline unwind stack""")
    parser.add_argument('-t', '--html_template', default='./',
                        type=dir_check, help=""" The path of report html template
                        """)
    args = parser.parse_args()

    get_used_binaries(args.perf_data, args.report_html, args.local_lib_dir, args.html_template)


if __name__ == '__main__':
    main()
