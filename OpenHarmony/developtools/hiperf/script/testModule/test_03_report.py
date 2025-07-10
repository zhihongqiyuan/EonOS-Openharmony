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
import pytest
import os
import argparse
import json
from hiperf_utils import dir_check
from hiperf_utils import file_check
from hiperf_utils import get_used_binaries
from hiperf_utils import validate_json


class TestReport:
    def setup(self):
        print("TestReport setup")

    @pytest.mark.L0
    def test_report(self):
        parser = argparse.ArgumentParser(description=""" To make a report, you 
        need to enter the data source and the path of the report.""")
        parser.add_argument('-i', '--perf_data', default='perf.data',
                            type=file_check, help=""" The path of profiling 
                            data.""")
        parser.add_argument('-r', '--report_html', default='hiperf_report.html',
                            help="""the path of the report.""")
        parser.add_argument('-l', '--local_lib_dir', type=dir_check, default='./testModule/binary_cache',
                            help="""Path to find symbol dir use to
                            do offline unwind stack""")
        args = parser.parse_args()

        get_used_binaries(args.perf_data, args.report_html, args.local_lib_dir)

        with open('./json.txt', 'r') as file:
            json_data = file.read()
            assert validate_json(json_data) == True
            jo = json.loads(json_data)
            record = jo['recordSampleInfo']
            for a in record:
                processes = a['processes']
                for b in processes:
                    threads = b['threads']
                    for c in threads:
                        print(c['tid'])
                        call_stack = c['CallOrder']['callStack']
                        assert len(call_stack) > 0

        assert os.path.exists('./hiperf_report.html') == True
