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
import os
import time
import json
from hiperf_utils import validate_json


class TestLaunch:
    res = False

    def setup(self):
        print("test_launch setup")

    def record_cmd(self):
        if os.path.exists('./perf_launch.json') == True:
            os.remove('./perf_launch.json')
        subprocess.call("hdc shell rm /data/local/tmp/hiprofiler_data.htrace /data/local/tmp/perf.data /data/local/tmp/perf_launch.json")
        subprocess.call('hdc shell aa force-stop com.example.insight_test_stage')
        subprocess.Popen(f'hdc shell "hiprofiler_cmd -o /data/local/tmp/hiprofiler_data.htrace -t 10 -s -k -c /data/local/tmp/config_launch.txt &"')
        time.sleep(5)
        subprocess.call('hdc shell aa start -a EntryAbility -b com.example.insight_test_stage')
        time.sleep(20)
        subprocess.call('hdc shell hiperf report --json -i /data/local/tmp/perf.data -o /data/local/tmp/perf_launch.json')
        subprocess.call("hdc file recv /data/local/tmp/perf_launch.json ./")

    @pytest.mark.L0
    def test_launch(self):
        subprocess.call('hdc shell rm -rf /data/local/tmp/85dcdfa')
        subprocess.call('hdc shell aa force-stop com.example.insight_test_stage')
        subprocess.call('hdc shell mkdir /data/local/tmp/85dcdfa')
        subprocess.call('hdc file send .\libraryhsp1-default-signed.hsp /data/local/tmp/85dcdfa')
        subprocess.call('hdc file send .\entry-default-signed.hap /data/local/tmp/85dcdfa')
        subprocess.call('hdc file send .\libraryhsp2cpp-default-signed.hsp /data/local/tmp/85dcdfa')
        subprocess.call('hdc shell bm install -p /data/local/tmp/85dcdfa')
        subprocess.call('hdc shell rm -rf /data/local/tmp/85dcdfa')
        subprocess.call('hdc shell setenforce 0')
        subprocess.call('hdc file send config_launch.txt /data/local/tmp')
        self.record_cmd()
        subprocess.call('hdc shell aa force-stop com.example.insight_test_stage')
        subprocess.call('hdc shell setenforce 1')
        assert os.path.exists('./perf_launch.json') == True

        with open('perf_launch.json', 'r') as file:
            json_data = file.read()
            assert validate_json(json_data) == True
            jo = json.loads(json_data)
            arr = jo['threadNameMap']
            count = 0
            for a in arr:
                print(arr[a])
                count = count + 1
                if arr[a] == 'OS_DfxWatchdog':
                    self.res = True
            print(count)

        assert self.res == True
