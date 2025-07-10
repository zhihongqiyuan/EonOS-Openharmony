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
import { fileOpen, createGPData, isAccess } from '../base/BaseProfilerUtils';
import { BaseProfiler } from '../base/BaseProfiler';
import { CollectorType } from '../base/ProfilerConstant';

let GPU_CONFIG = {
  GPU_FREQ_PATH: [
    '/sys/class/devfreq/fde60000.gpu/cur_freq', //RK
    '/sys/class/devfreq/gpufreq/cur_freq',
  ],
  GPU_LOAD_PATH: [
    '/sys/class/devfreq/fde60000.gpu/load', //RK
    '/sys/class/devfreq/gpufreq/gpu_scene_aware/utilisation',
  ],
};

export class GPU extends BaseProfiler {
  private gpuMap: Map<String, String> = new Map();
  public static instance: GPU = null;
  public static getInstance() {
    if (this.instance == null) {
      this.instance = new GPU();
    }
    return this.instance;
  }

  init() {
    //初始化GPU
    return CollectorType.TYPE_GPU;
  }

  isSupport() {
    let curFreqPath = undefined;
    let curLoadPath = undefined;
    GPU_CONFIG.GPU_FREQ_PATH.forEach((path) => {
      if (isAccess(path)) {
        curFreqPath = path;
      }
    });
    GPU_CONFIG.GPU_LOAD_PATH.forEach((path) => {
      if (isAccess(path)) {
        curLoadPath = path;
      }
    });
    if (curFreqPath != undefined && curLoadPath != undefined) {
      return true;
    }
    return false;
  }

  readData() {
    let gpuFreq: String = '';
    let gpuLoad: String = '';
    GPU_CONFIG.GPU_FREQ_PATH.forEach((path) => {
      if (isAccess(path)) {
        gpuFreq = fileOpen(path);
      }
    });
    GPU_CONFIG.GPU_LOAD_PATH.forEach((path) => {
      if (isAccess(path)) {
        gpuLoad = fileOpen(path);
      }
    });
    let loadStr: string[] = gpuLoad.split('@');
    let load = '-1';
    if (loadStr.length > 0) {
      load = loadStr[0].toString();
    } else {
      load = gpuLoad.toString();
    }
    this.gpuMap.set('gpuFreq', gpuFreq);
    this.gpuMap.set('gpuLoad', load);
    return createGPData('GPU', this.gpuMap);
  }
}
