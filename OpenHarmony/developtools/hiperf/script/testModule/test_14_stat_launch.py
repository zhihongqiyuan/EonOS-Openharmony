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
import threading


class MyThread(threading.Thread):
    def run(self):
        time.sleep(1)
        subprocess.call('hdc shell aa force-stop com.example.insight_test_stage')
        time.sleep(2)
        subprocess.call('hdc shell aa start -a EntryAbility -b com.example.insight_test_stage')


class TestStatLaunch:
    res = False

    def setup(self):
        print("test_stat_launch setup")

    def record_cmd(self):
        if os.path.exists('./perf_launch.json') == True:
            os.remove('./perf_launch.json')
        subprocess.call("hdc shell rm /data/local/tmp/hiprofiler_data.htrace /data/local/tmp/perf.data /data/local/tmp/perf_launch.json")
        subprocess.call('hdc shell aa start -a EntryAbility -b com.example.insight_test_stage')
        thread = MyThread()
        thread.start()
        output = os.popen('hdc shell hiperf stat -d 5 --app com.example.insight_test_stage --restart').read()
        thread.join()
        return output

    @pytest.mark.L0
    def test_stat_launch(self):
        subprocess.call('hdc shell rm -rf /data/local/tmp/85dcdfa')
        subprocess.call('hdc shell aa force-stop com.example.insight_test_stage')
        subprocess.call('hdc shell mkdir /data/local/tmp/85dcdfa')
        subprocess.call('hdc file send .\libraryhsp1-default-signed.hsp /data/local/tmp/85dcdfa')
        subprocess.call('hdc file send .\entry-default-signed.hap /data/local/tmp/85dcdfa')
        subprocess.call('hdc file send .\libraryhsp2cpp-default-signed.hsp /data/local/tmp/85dcdfa')
        subprocess.call('hdc shell bm install -p /data/local/tmp/85dcdfa')
        subprocess.call('hdc shell rm -rf /data/local/tmp/85dcdfa')
        subprocess.call('hdc file send config_launch.txt /data/local/tmp')
        output = self.record_cmd()
        subprocess.call('hdc shell aa force-stop com.example.insight_test_stage')
        arr = ["hw-branch-instructions", "hw-branch-misses", "hw-cpu-cycles", "hw-instructions"]
        for a in arr:
            assert a in output
