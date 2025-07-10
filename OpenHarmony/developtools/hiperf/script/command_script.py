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
import time
import argparse
import subprocess
from hiperf_utils import HdcInterface
from hiperf_utils import get_build_id
from hiperf_utils import get_architecture
from hiperf_utils import PerformanceProfile
from hiperf_utils import dir_check
from hiperf_utils import get_hiperf_binary_path
from hiperf_utils import bytes_to_str
from hiperf_utils import str_to_bytes
from hiperf_utils import remove


def check_args(args):
    if (not args.package_name) and args.ability:
        raise Exception('-a can only be used when profiling an OHOS '
                        'package_name.')
    return True


def parser_add_argument():
    description = "Collect performance sampling information of" \
                  " running [command]."
    parser = argparse.ArgumentParser(description=description)
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
    record_group = parser.add_argument_group('Select recording options')
    record_group.add_argument('-a', '--ability',
                              help="""Used with -p. Profile the launch time of 
                        an ability in an OHOS app. The app will be started or 
                        restarted to run the ability. 
                        Like : -a .MainAbility """)

    record_group.add_argument('-r', '--record_options',
                              default='-f 1000 -d 10 -s dwarf',
                              help="""Set recording options for `hiperf record`
                         command. Default is "'-f 1000 -d 10 -s dwarf'".""")

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

    record_group.add_argument('-o', '--output_perf_data', default='perf.data',
                              help='The path to store profiling data. '
                                   'Default is perf.data.')

    other_group = parser.add_argument_group('Other options')

    other_group.add_argument('--not_hdc_root', action='store_true',
                             help="""Force hdc to run in non root mode. """)
    args = parser.parse_args()
    return args


def main(args):
    check_args(args)
    profiler = PerformanceProfile(args)
    profiler.profile()
    return True


if __name__ == '__main__':
    main(parser_add_argument())
