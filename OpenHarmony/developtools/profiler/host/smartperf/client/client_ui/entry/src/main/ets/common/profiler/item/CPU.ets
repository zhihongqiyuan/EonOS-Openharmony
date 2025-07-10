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
import fileio from '@ohos.fileio';
import { fileOpen, travelFile, createGPData, isAccess } from '../base/BaseProfilerUtils';
import { BaseProfiler } from '../base/BaseProfiler';
import { CollectorType } from '../base/ProfilerConstant';
import { GPData } from '../../entity/DatabaseEntity';
import SPLogger from '../../utils/SPLogger';
enum CpuConfig {
  CPU_BASE = '/sys/devices/system/cpu',
  CPU_CUR_FREQ = '/cpufreq/scaling_cur_freq',
  CPU_LOAD = '/proc/stat',
}

export class CPU extends BaseProfiler {
  private cpuMap: Map<String, String> = new Map();
  private cpuCoreNum: number;
  private prebufferArr = ['', '', '', '', '', '', '', '', ''];

  public static instance: CPU = null;
  public static getInstance() {
    if (this.instance == null) {
      this.instance = new CPU();
    }
    return this.instance;
  }

  init() {
    //初始化CPU 核心数 节点路径
    this.cpuCoreNum = travelFile(CpuConfig.CPU_BASE, 'cpu[0-9]').length;
    return CollectorType.TYPE_CPU;
  }

  isSupport() {
    if (globalThis.useDaemon) {
      return true;
    }
    return false;
  }

  readData() {
    for (var i = 0; i < this.cpuCoreNum; i++) {
      const path = CpuConfig.CPU_BASE + '/cpu' + i + CpuConfig.CPU_CUR_FREQ;
      var cpu = fileOpen(path);
      this.cpuMap.set('cpu' + i + 'Freq', cpu);
    }
    return createGPData('CPU', this.cpuMap);
  }

  readCPULoad(): GPData {
    const path = CpuConfig.CPU_LOAD;
    var workLoadArr = [];
    try {
      var fd = -1;
      fd = fileio.openSync(path, 0o0);
      let buf = new ArrayBuffer(2048);
      fileio.readSync(fd, buf);
      let cpuStr: String = String.fromCharCode.apply(null, new Uint8Array(buf));
      let cpuStrArr = [];
      cpuStr = cpuStr.substring(0, cpuStr.indexOf('intr'));
      let nextj = 0;
      let j;
      for (var i = 1; i < cpuStr.length; i++) {
        if (cpuStr.charAt(i) == 'c') {
          j = nextj;
          nextj = i;
          cpuStrArr.push(cpuStr.substring(j, nextj));
        }
      }
      cpuStrArr.push(cpuStr.substring(nextj, nextj + 50));
      let buffer = '';
      for (let index = 1; index < cpuStrArr.length; index++) {
        buffer = cpuStrArr[index];
        let load = this.calCPULoad(buffer, this.prebufferArr[index]);
        workLoadArr.push(load);
        this.prebufferArr[index] = buffer;
      }
    } catch (err) {
    } finally {
      fileio.closeSync(fd);
    }

    let map = new Map();
    for (let index = 0; index < workLoadArr.length; index++) {
      const element = workLoadArr[index];
      map.set('cpu' + index + 'Load', element);
    }
    return createGPData('CPULoad', map);
  }

  calCPULoad(buffer: string, preBuffer: string): number {
    if (preBuffer.length == 0) {
      return -1;
    }
    let timeArr: string[] = buffer.split(' ');
    let preTimeArr: string[] = preBuffer.split(' ');

    timeArr.reverse().pop();
    preTimeArr.reverse().pop();
    timeArr.reverse();
    preTimeArr.reverse();

    let time = this.ArrStr2Number(timeArr);
    let preTime = this.ArrStr2Number(preTimeArr);

    let user = time[0] + time[1] - preTime[0] - preTime[1];
    let sys = time[2] - preTime[2];
    let idle = time[3] - preTime[3];
    let iowait = time[4] - preTime[4];
    let irq = time[5] + time[6] - preTime[5] - preTime[6];
    let total = user + sys + idle + iowait + irq;
    if (user < 0 || sys < 0 || idle < 0 || iowait < 0 || irq < 0) {
      return 0;
    }
    let preUser = (user * 100.0) / total;
    let preSys = (sys * 100.0) / total;
    let preIowait = (iowait * 100.0) / total;
    let preIrq = (irq * 100.0) / total;
    let workload = preUser + preSys + preIowait + preIrq;
    return Number(workload.toFixed(2)).valueOf();
  }

  ArrStr2Number(arr: Array<String>): Array<number> {
    let result = [];
    for (var index = 0; index < arr.length; index++) {
      const element = arr[index].replace('\n', '');
      result.push(`${element}`.valueOf());
    }
    return result;
  }
}
