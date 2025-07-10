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
#
import pytest
import subprocess
import json
from hiperf_utils import validate_json


class TestParseABC:
    res = False

    def setup(self):
        print('test_parse_abc setup')

    def fun(self, jo, val):
        call_tack = jo['callStack']
        if jo['symbol'] == val:
            self.res = True
            return

        for a in call_tack:
            self.fun(a, val)

    @pytest.mark.L0
    def test_parse_abc(self):
        subprocess.call('hdc shell rm /data/local/tmp/perf_parse_abc.json')
        subprocess.call('hdc file send perf_parse_abc.data /data/local/tmp')
        subprocess.call('hdc shell hiperf report --json -i /data/local/tmp/perf_parse_abc.data -o /data/local/tmp/perf_parse_abc.json')
        subprocess.call('hdc file recv /data/local/tmp/perf_parse_abc.json')
        with open('./perf_parse_abc.json', 'r') as file:
            json_data = file.read()
            assert validate_json(json_data) == True
            jo = json.loads(json_data)
            arr = jo['symbolsFileList']
            index = 0
            for a in arr:
                if a.find('.abc') > 0:
                    break
                index = index + 1
            print(index)
            arrmap = jo['SymbolMap']
            val = 0
            for a in arrmap:
                if arrmap[a]['file'] == index and 'UpdateElement:' in arrmap[a]['symbol']:
                    print(arrmap[a]['symbol'])
                    val = int(a)
                    print(val)
                    break
            record = jo['recordSampleInfo']
            for a in record:
                processes = a['processes']
                for b in processes:
                    threads = b['threads']
                    for c in threads:
                        print(c['tid'])
                        call_stack = c['CallOrder']['callStack']
                        for d in call_stack:
                            self.fun(d, val)
        assert self.res == True
