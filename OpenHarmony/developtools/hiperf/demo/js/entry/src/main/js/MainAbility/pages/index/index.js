/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import hiperf from '@ohos.hiperf';

export default {
  data: {
    clickTimes: 0,
    total: 20,
    output: '',
    sleepTicks: 0,
  },

  onInit() {
    this.output = this.clickTimes;
  },

  sleep(delay) {
    let start = (new Date()).getTime();
    while (((new Date()).getTime() - start) < delay) {
      this.sleepTicks++;
    }
  },

  optionOnclick: function () {
    const PERIOD_NUM = 100;
    const STOP_TIME = 2;
    const FREQ_NUM = 500;
    const CPU_PERCENT = 25;
    this.clickTimes++;
    console.info('optionOnclick' + this.clickTimes);
    let result;
    this.output = '';

    result = hiperf.resetOption();
    this.output += ('ResetOption: ' + result + '\n');

    result = hiperf.setOutputFilename('/data/accounts/account_0/appdata/com.example.hiperfdemo/files/perf.data');
    this.output += ('SetOutputFilename: ' + result + '\n');

    result = hiperf.getOutputFileName();
    this.output += ('GetOutputFileName: ' + result + '\n');

    result = hiperf.setTargetSystemWide(true);

    result = hiperf.setCompressData(true);

    result = hiperf.setSelectCpus('1,2,3,4');

    result = hiperf.setTimeStopSec(STOP_TIME);

    result = hiperf.setFrequency(FREQ_NUM);

    result = hiperf.setPeriod(PERIOD_NUM);

    result = hiperf.setSelectEvents('hw-cpu-cycles,hw-instructions');

    result = hiperf.setSelectGroups('hw-cpu-cycles,hw-instructions');

    result = hiperf.setNoInherit(true);

    result = hiperf.setSelectPids('1,2,3');

    result = hiperf.setSelectTids('1,2,3');

    result = hiperf.setExcludePerf(true);

    result = hiperf.setCpuPercent(CPU_PERCENT);

    result = hiperf.setOffCPU(false);

    result = hiperf.setCallGraph('dwarf');

    result = hiperf.setDelayUnwind(true);

    result = hiperf.setDisableUnwind(true);

    result = hiperf.setDisableCallstackMerge(true);

    result = hiperf.setSymbolDir('/data/local/tmp');

    result = hiperf.setDataLimit('10M');

    result = hiperf.setAppPackage('com.example.hiperfdemo');

    result = hiperf.setClockId('monotonic');

    result = hiperf.setVecBranchSampleTypes('any_call');

    result = hiperf.setMmapPages(1024);

    result = hiperf.getOptionVecString();
    this.output += ('GetOptionVecString: ' + result + '\n');

    result = hiperf.startWithOption();
    this.output += ('StartWithOption: ' + result + '\n');
  },

  recordOnclick: function () {
    const SLEEP_TIME = 1000;
    this.clickTimes++;
    console.info('onclick recordOnclick' + this.clickTimes);
    this.output = ('recordOnclick\n' + this.clickTimes + '\n');

    let result;

    result = hiperf.setDebugMode();
    this.output += ('SetDebugMode: ' + result + '\n');
    if (!result) {
      return;
    }

    result = hiperf.setup('/data/accounts/account_0/appdata/com.example.hiperfdemo/files');
    this.output += ('Setup: ' + result + '\n');
    if (!result) {
      return;
    }

    result = hiperf.isReady();
    this.output += ('IsReady: ' + result + '\n');

    if (!result) {
      return;
    }

    result = hiperf.getOutputPerfDataPath();
    this.output += ('GetOutputPerfDataPath: ' + result + '\n');

    result = hiperf.getCommandPath();
    this.output += ('GetCommandPath: ' + result + '\n');

    result = hiperf.start();
    this.output += ('Start: ' + result + '\n');
    if (!result) {
      return;
    }
    this.sleep(SLEEP_TIME);

    result = hiperf.pause();
    this.output += ('Pause: ' + result + '\n');
    if (!result) {
      return;
    }
    this.sleep(SLEEP_TIME);

    result = hiperf.resume();
    this.output += ('Resume: ' + result + '\n');
    if (!result) {
      return;
    }
    this.sleep(SLEEP_TIME);

    result = hiperf.stop();
    this.output += ('Stop: ' + result + '\n');
  },

  testOnclick: function ()
  {
    this.clickTimes++;
    console.info('onclick testOnclick' + this.clickTimes);
    this.output = this.clickTimes;
  }
};