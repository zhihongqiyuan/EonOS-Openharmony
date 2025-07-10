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

/**
 * GPData实体类
 */
export class GPData {
  public moduleType: String;
  public timeStamp: number;
  public values: Map<String, String>;

  constructor(moduleType: String, timeStamp: number, values: Map<String, String>) {
    this.moduleType = moduleType;
    this.timeStamp = timeStamp;
    this.values = values;
  }

  toString() {
    var obj = Object.create(null);
    var iterator = this.values.keys();
    for (var i = 0; i < this.values.size; i++) {
      var key = iterator.next().value;
      obj[key] = this.values.get(key);
    }
    return (
      'GPData{' +
      "module='" +
      this.moduleType +
      "'" +
      ', timeStamp=' +
      this.timeStamp +
      ', values=' +
      JSON.stringify(obj) +
      '}'
    );
  }
}

/**
 * 任务列表页  任务概览页实体类
 */
export class TGeneralInfo {
  //目录生成的UUID  作为主键
  public sessionId: String;
  //任务Id
  public taskId: String;
  //测试游戏名称
  public appName: String;
  //测试游戏版本
  public appVersion: String;
  //游戏包名
  public packageName: String;
  //开始时间
  public startTime: number;
  //结束时间
  public endTime: number;
  //测试时长
  public testDuration: number;
  //任务名称
  public taskName: String;
  //测试方式  middle
  public testCase: String;
  //测试类型  smartPerf beta
  public testType: String;
  //用户名
  public user: String;
  //用户id
  public userId: String;
  //项目Id
  public projectId: String;
  //报告来源
  public dataSource: String;
  //sp版本
  public spVersion: String;
  //设备类型
  public deviceTypeName: String;
  //设备品牌
  public brand: String;
  //设备名称  XX-XXX(****00018)
  public deviceName: String;
  //设备型号
  public board: String;
  //手机版本
  public version: String;
  //芯片平台
  public plat: String;
  //大中小核心
  public cpuCluster: String;
  //设备sn号
  public sn: String;
  //分辨类
  public resolution: String;
  //当前亮度
  public screenBrightness: String;
  //设备当前音量
  public volume: String;
  //当前电池电量
  public batteryVolume: String;
  //是否是多路测试
  public isOnline: boolean;
  //设备号
  public deviceNum: number;
  //房间号
  public roomNumber: String;
  //任务类型 主控、被控
  public taskType: number;

  constructor(
    sessionId?: String,
    taskId?: String,
    appName?: String,
    appVersion?: String,
    packageName?: String,
    startTime?: number,
    endTime?: number,
    testDuration?: number,
    taskName?: String
  ) {
    this.sessionId = sessionId;
    this.taskId = taskId;
    this.appName = appName;
    this.appVersion = appVersion;
    this.packageName = packageName;
    this.startTime = startTime;
    this.endTime = endTime;
    this.testDuration = testDuration;
    this.taskName = taskName;
  }
}

/**
 * 任务详情实体类
 */
export class TIndexInfo {
  //时间戳 主键
  public timestamp: String;
  //任务ID
  public taskId: String;
  //场景标识
  public flag: String;
  //温度
  public shellBackTemp: String;
  public shellFrameTemp: String;
  public shellFrontTemp: String;
  public socThermalTemp: String;
  public systemHTemp: String;
  public gpuTemp: String;
  public ambientTemp: String;
  public batteryTemp: String;

  //ddr
  public ddrFrequency: String;
  //fps
  public fps: String;
  public fpsJitters: String;
  //cpu
  public cpu0Frequency: String;
  public cpu0Load: String;
  public cpu1Frequency: String;
  public cpu1Load: String;
  public cpu2Frequency: String;
  public cpu2Load: String;
  public cpu3Frequency: String;
  public cpu3Load: String;
  public cpu4Frequency: String;
  public cpu4Load: String;
  public cpu5Frequency: String;
  public cpu5Load: String;
  public cpu6Frequency: String;
  public cpu6Load: String;
  public cpu7Frequency: String;
  public cpu7Load: String;
  public cpuLoad: String;
  //gpu
  public gpuFrequency: String;
  public gpuLoad: String;
  //power
  public currentNow: String;
  public capacity: String;
  public enableHiz: String;
  public status: String;
  public voltageNow: String;
  //ram
  public pss: String;
  //HWCPipeGPU
  public cacheMisses: String;
  public instructions: String;
  //HWCPipeGPU
  public gpuCycles: String;
  public vertexComputeCycles: String;
  public fragmentCycles: String;
  public tilerCycles: String;
  public vertexComputeJobs: String;
  public fragmentJobs: String;
  public pixels: String;
  public earlyZTests: String;
  public earlyZKilled: String;
  public externalMemoryReadAccesses: String;
  public externalMemoryWriteAccesses: String;
  public externalMemoryReadBytes: String;
  public externalMemoryWriteBytes: String;
  public cacheWriteLookups: String;
  public cacheReadLookups: String;
  public externalMemoryWriteStalls: String;
  public externalMemoryReadStalls: String;
  public shaderCycles: String;
  public shaderArithmeticCycles: String;
  public shaderLoadStoreCycles: String;
  public shaderTextureCycles: String;

  //QPCounters
  public clocksSecond: String;
  public gpuUtilization: String;
  public gpuBusBusy: String;
  public verticesShadedSecond: String;
  public fragmentsShadedSecond: String;
  public texturesVertex: String;
  public texturesFragment: String;
  public aluVertex: String;
  public aluFragment: String;
  public timeShadingFragments: String;
  public timeShadingVertices: String;
  public timeCompute: String;
  public readTotal: String;
  public writeTotal: String;
  public textureMemoryReadBW: String;
  public vertexMemoryRead: String;
  public spMemoryRead: String;
  public qpGPUFrequency: String;
  //network
  public currNetworkType: String;
  public networkUpSpeed: String;
  public networkDownSpeed: String;
  public wlanSingleIntensity: String;
  public radioSingleIntensity: String;
  public networkDelaySdk: String;
  constructor(
    timestamp?: String,
    taskId?: String,
    cpu0Freq?: String,
    cpu1Freq?: String,
    cpu2Freq?: String,
    cpu3Freq?: String,
    cpu4Freq?: String,
    cpu5Freq?: String,
    cpu6Freq?: String,
    cpu7Freq?: String,
    cpu0Load?: string,
    cpu1Load?: string,
    cpu2Load?: string,
    cpu3Load?: string,
    cpu4Load?: string,
    cpu5Load?: string,
    cpu6Load?: string,
    cpu7Load?: string,
    gpuFreq?: String,
    gpuLoad?: String,
    ddrFreq?: String,
    shellFrame?: String,
    shellFront?: String,
    shellBack?: String,
    socThermal?: String,
    systemH?: String,
    gpu?: String,
    ambient?: String,
    battery?: String,
    currentNow?: String,
    voltageNow?: String,
    pss?: String,
    fps?: String,
    fpsJitters?: String,
    networkUpSpeed?: String,
    networkDownSpeed?: String
  ) {
    this.timestamp = timestamp;
    this.taskId = taskId;
    this.cpu0Frequency = cpu0Freq;
    this.cpu1Frequency = cpu1Freq;
    this.cpu2Frequency = cpu2Freq;
    this.cpu3Frequency = cpu3Freq;
    this.cpu4Frequency = cpu4Freq;
    this.cpu5Frequency = cpu5Freq;
    this.cpu6Frequency = cpu6Freq;
    this.cpu7Frequency = cpu7Freq;
    this.cpu0Load = cpu0Load;
    this.cpu1Load = cpu1Load;
    this.cpu2Load = cpu2Load;
    this.cpu3Load = cpu3Load;
    this.cpu4Load = cpu4Load;
    this.cpu5Load = cpu5Load;
    this.cpu6Load = cpu6Load;
    this.cpu7Load = cpu7Load;
    this.gpuFrequency = gpuFreq;
    this.gpuLoad = gpuLoad;
    this.ddrFrequency = ddrFreq;
    this.shellFrameTemp = shellFrame;
    this.shellFrontTemp = shellFront;
    this.shellBackTemp = shellBack;
    this.socThermalTemp = socThermal;
    this.systemHTemp = systemH;
    this.gpuTemp = gpu;
    this.ambientTemp = ambient;
    this.batteryTemp = battery;
    this.currentNow = currentNow;
    this.voltageNow = voltageNow;
    this.pss = pss;
    this.fps = fps;
    this.fpsJitters = fpsJitters;
    this.networkUpSpeed = networkUpSpeed;
    this.networkDownSpeed = networkDownSpeed;
  }

  setTimeStamp(timestamp: String) {
    this.timestamp = timestamp;
  }

  setTaskId(taskId: String) {
    this.taskId = taskId;
  }

  setCPUData(
    cpu0Freq: String,
    cpu1Freq: String,
    cpu2Freq: String,
    cpu3Freq: String,
    cpu4Freq: String,
    cpu5Freq: String,
    cpu6Freq: String,
    cpu7Freq: String
  ) {
    this.cpu0Frequency = cpu0Freq;
    this.cpu1Frequency = cpu1Freq;
    this.cpu2Frequency = cpu2Freq;
    this.cpu3Frequency = cpu3Freq;
    this.cpu4Frequency = cpu4Freq;
    this.cpu5Frequency = cpu5Freq;
    this.cpu6Frequency = cpu6Freq;
    this.cpu7Frequency = cpu7Freq;
  }

  setCPULoadData(
    cpu0Load: String,
    cpu1Load: String,
    cpu2Load: String,
    cpu3Load: String,
    cpu4Load: String,
    cpu5Load: String,
    cpu6Load: String,
    cpu7Load: String
  ) {
    this.cpu0Load = cpu0Load;
    this.cpu1Load = cpu1Load;
    this.cpu2Load = cpu2Load;
    this.cpu3Load = cpu3Load;
    this.cpu4Load = cpu4Load;
    this.cpu5Load = cpu5Load;
    this.cpu6Load = cpu6Load;
    this.cpu7Load = cpu7Load;
  }

  setGPUData(gpuFreq: String, gpuLoad: String) {
    this.gpuFrequency = gpuFreq;
    this.gpuLoad = gpuLoad;
  }

  setDDRData(ddrFreq: String) {
    this.ddrFrequency = ddrFreq;
  }

  setTempData(
    shellFrame: String,
    shellFront: String,
    shellBack: String,
    socThermal: String,
    systemH: String,
    gpu: String,
    ambient: String
  ) {
    this.shellFrameTemp = shellFrame;
    this.shellFrontTemp = shellFront;
    this.shellBackTemp = shellBack;
    this.socThermalTemp = socThermal;
    this.systemHTemp = systemH;
    this.gpuTemp = gpu;
    this.ambientTemp = ambient;
  }

  setPowerData(currentNow: String, voltageNow: String, batteryTemp: String) {
    this.currentNow = currentNow;
    this.voltageNow = voltageNow;
    this.batteryTemp = batteryTemp;
  }

  setFpsData(fps: String, fpsJitter: String) {
    this.fps = fps;
    this.fpsJitters = fpsJitter;
  }

  setRamData(pss: String) {
    this.pss = pss;
  }

  setNetWorkData(networkUpSpeed: String, networkDownSpeed: String) {
    this.networkDownSpeed = networkDownSpeed;
    this.networkUpSpeed = networkUpSpeed;
  }

  setDefaultValue() {
    let properties = Object.keys(this);
    properties.forEach((property) => {
      this[property] = '-1';
    });
  }
}

//dubai app功耗
export class TPowerAppInfo {
  //主键 自增
  public id: String;
  public taskId: String;
  public application: String;
  public power: String;
  public current: String;
  public percent: String;
  public color: Resource;
  constructor(
    id?: String,
    taskId?: String,
    application?: String,
    power?: String,
    current?: String,
    percent?: String,
    color?: Resource
  ) {
    this.id = id;
    this.taskId = taskId;
    this.application = application;
    this.power = power;
    this.current = current;
    this.percent = percent;
    this.color = color;
  }
  setColor(color?: Resource) {
    this.color = color;
  }
  setPower(power?: String) {
    this.power = power;
  }
  setPercent(percent?: String) {
    this.percent = percent;
  }
  setCurrent(current?: String) {
    this.current = current;
  }
}

//dubai 器件功耗
export class TPowerSensorInfo {
  //主键 自增
  public id: string;
  public taskId: string;
  public sensor: string;
  public power: string;
  public current: string;
  public percent: string;
  public color: Resource;
  constructor(taskId?: string, sensor?: string, power?: string, current?: string, percent?: string, color?: Resource) {
    this.taskId = taskId;
    this.sensor = sensor;
    this.power = power;
    this.current = current;
    this.percent = percent;
    this.color = color;
  }
  setPerenct(percent: string) {
    this.percent = percent;
  }
  setColor(color: Resource) {
    this.color = color;
  }
  setPower(power: string) {
    this.power = power;
  }
  setCurrent(current: string) {
    this.current = current;
  }
}
