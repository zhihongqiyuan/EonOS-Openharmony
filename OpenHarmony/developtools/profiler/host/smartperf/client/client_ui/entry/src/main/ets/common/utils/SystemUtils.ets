/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

import { fileOpen, travelFile } from '../profiler/base/BaseProfilerUtils';
import { ProcessRunningInfo } from '../utils/../entity/SystemEntity';
import appManager from '@ohos.application.appManager';

export function getCpuCoreInfo(): Array<String> {
  const basePath = '/sys/devices/system/cpu/cpufreq';
  let defaultPolicy = ['policy0', 'policy1', 'policy2'];
  var supportPolicy = [];
  let policyArr = travelFile(basePath, 'policy');

  policyArr.forEach((policy) => {
    defaultPolicy.forEach((defaultItem) => {
      if (defaultItem == policy) {
        supportPolicy.push(defaultItem);
      }
    });
  });
  var coreArr = [];
  for (var index = 0; index < supportPolicy.length; index++) {
    const policy = supportPolicy[index];
    var affectedCpus = fileOpen(basePath + '/' + policy + '/affected_cpus');
    coreArr.push(affectedCpus.charAt(0));
  }
  return coreArr;
}

export function getPidOfAbility(processName: string): Promise<String> {
  return appManager.getProcessRunningInfos().then((data) => {
    let processArr: ProcessRunningInfo[] = [...data];
    var pid = '-1';
    processArr.forEach((process) => {
      if (process.processName == processName) {
        pid = process.pid.toString();
      }
    });
    return pid;
  });
}
export function bubbleSort(arr: Array<number>): Array<number> {
  let final: number[] = [...arr];
  for (let i = 0; i < final.length; i++) {
    let isSwap: boolean = true;
    for (let j = 0; j < final.length - i - 1; j++) {
      // 如果前一个更大，则和后面的交换
      if (final[j] > final[j + 1]) {
        let temp = final[j + 1];
        final[j + 1] = final[j];
        final[j] = temp;
        isSwap = false;
      }
    }
    // 如果未冒泡，则已经排序完毕
    if (isSwap) {
      break;
    }
  }
  return final;
}
