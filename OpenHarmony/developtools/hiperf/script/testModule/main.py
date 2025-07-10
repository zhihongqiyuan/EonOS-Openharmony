#!/usr/bin/env python
# -*- coding: utf-8 -*-
#   Copyright (c) 2025 Huawei Device Co., Ltd.
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
from hiperf_utils import PerformanceProfile
from hiperf_utils import get_arg_list
from hiperf_utils import dir_check
from hiperf_utils import get_lib
from record_control import check_args
from test_02_recv_binary_cache import GetLibFiles


def get_used_binaries(perf_data, report_file, binary_cache, html_template):
    if binary_cache:
        get_lib().ReportUnwindJson(perf_data.encode("utf-8"),
                                   'json.txt'.encode("utf-8"),
                                   binary_cache.encode("utf-8"))
    else:
        get_lib().ReportJson(perf_data.encode("utf-8"),
                             'json.txt'.encode("utf-8"))
    time.sleep(2)
    with open('json.txt', 'r') as json_file:
        all_json = json_file.read()
        template = os.path.join(html_template, './testModule/report.html')
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


def add_target_argument(parser):
    target_group = parser.add_argument_group(title='Select profiling target') \
        .add_mutually_exclusive_group(required=True)
    target_group.add_argument('-app', '--package_name',
                              help="""Collect profile info for an OHOS app""")

    target_group.add_argument('-lp', '--local_program',
                              help="""Collect profile info
                              for an local program.""")

    target_group.add_argument('-cmd',
                              help="""Running a command on the OHOS device.
                              like as : -cmd "'ps -ef'".
                              the ps will open as child process of hiperf 
                              and sample this process.""")

    target_group.add_argument('-p', '--pid', nargs='*',
                              help="""Limit the process id of the collection
                              target.""")

    target_group.add_argument('-t', '--tid', nargs='*',
                              help="""Limit the thread id of the collection 
                              target.""")

    target_group.add_argument('-sw', '--system_wide', action='store_true',
                              help="""Collect system-wide information. 
                              This requires CAP_PERFMON (since Linux 5.8) or
                              CAP_SYS_ADMIN capability or a 
                              /proc/sys/kernel/perf_event_paranoid 
                              value of less than 1.""")


def add_record_argument(parser):
    record_group = parser.add_argument_group('Select recording options')
    record_group.add_argument('-a', '--ability',
                              help="""Used with -p. Profile the launch time of 
                              an ability in an OHOS app. The app will be started or 
                              restarted to run the ability. 
                              Like : -a .MainAbility """)

    record_group.add_argument('-d', '--record_options',
                              default='-f 1000 -d 10 -s dwarf',
                              help="""Set recording options for `hiperf record`
                              command. Default is "'-f 1000 -d 10 -s dwarf'".""")

    record_group.add_argument('-o', '--output_perf_data', default='perf.data',
                              help='The path to store profiling data. '
                              'Default is perf.data.')
    record_group.add_argument('-lib', '--local_lib_dir', type=dir_check,
                              help="""When profiling an OHOS app containing 
                              local thelocal libraries are usually stripped and lake
                              of symbols and debug information to provide good 
                              profiling result. By using -lib, you tell 
                              command_script.py the path storing unstripped local 
                              libraries, and script will search all shared libraries
                              with suffix .so in the directory. Then the local
                              libraries will be downloaded on device and collected 
                              in build_cache.""")


def add_recver_argument(parser):
    recver_group = parser.add_argument_group('Recver options')

    recver_group.add_argument('-i', '--perf_data', default='perf.data',
                              help=""" The path of profiling data.""")
    recver_group.add_argument('-l', '--local_library_dir', type=dir_check, nargs='+',
                              help="""Path to find debug version of local shared
                              libraries used in the app.""", action='append')
    recver_group.add_argument('-c', '--copy_symbol_from_device', default='0',
                              help=""" Copy symbol files from device.""")


def add_report_argument(parser):
    report_group = parser.add_argument_group('Report options')
    report_group.add_argument('-r', '--report_html', default='hiperf_report.html',
                              help="""the path of the report.""")
    report_group.add_argument('-m', '--html_template', default='./',
                              help=""" The path of report html template
                              """)
    report_group.add_argument('-b', '--binary_cache', type=dir_check, default='./testModule/binary_cache',
                              help="""Path to find symbol dir use to
                              do offline unwind stack""")


def parser_add_argument():
    description = "Collect performance sampling information of" \
                  " running [command]."
    parser = argparse.ArgumentParser(description=description)
    add_target_argument(parser)
    add_record_argument(parser)

    other_group = parser.add_argument_group('Other options')
    other_group.add_argument('--not_hdc_root', action='store_true',
                             help="""Force hdc to run in non root mode. """)

    add_recver_argument(parser)
    add_report_argument(parser)
    args = parser.parse_args()
    return args


def main(args):
    check_args(args)
    profiler = PerformanceProfile(args)
    profiler.profile()
    recver = GetLibFiles()
    library_dirs = get_arg_list(args.local_library_dir)
    recver.recv_binary_cache(args.perf_data, library_dirs,
                             args.copy_symbol_from_device == '1' or args.copy_symbol_from_device == 'true')
    get_used_binaries(args.perf_data, args.report_html, args.binary_cache, args.html_template)
    return True


if __name__ == '__main__':
    main(parser_add_argument())
