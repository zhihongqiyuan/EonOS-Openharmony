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


def get_used_binaries(perf_data, command):
    get_lib().Report(perf_data.encode("utf-8"),
                     'report.txt'.encode("utf-8"), command.encode())

    with open('report.txt', 'r') as report_file:
        print(report_file.read())

    dirname, _ = os.path.split(os.path.abspath(sys.argv[0]))
    abs_path = os.path.join(dirname, 'report.txt')
    print("save to %s success" % abs_path)


def main():
    parser = argparse.ArgumentParser(description=""" To make a report, you 
    need to enter the data source and the path of the report.""")
    parser.add_argument('-i', '--perf_data', default='perf.data',
                        type=file_check, help=""" The path of profiling 
                        data.""")
    parser.add_argument('-l', '--local_lib_dir', type=dir_check,
                        help="""Path to find symbol dir use to
                         do offline unwind stack""")
    parser.add_argument('-s', '--call_stack', action='store_true',
                        help="""print the call_stack""")
    args = parser.parse_args()

    command = ""
    if args.call_stack:
        command += "-s "
    if args.local_lib_dir:
        command += "--symbol-dir " + args.local_lib_dir

    get_used_binaries(args.perf_data, command)


if __name__ == '__main__':
    main()
