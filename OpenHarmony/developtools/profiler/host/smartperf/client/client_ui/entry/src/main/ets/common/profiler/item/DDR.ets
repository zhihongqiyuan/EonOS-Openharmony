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
import SPLogger from '../../../common/utils/SPLogger';

enum DdrConfig {
  DDR_BASE = '/sys/class/devfreq/ddrfreq/cur_freq',
}

export class DDR extends BaseProfiler {
  private ddrMap: Map<String, String> = new Map();

  public static instance: DDR = null;
  public static getInstance() {
    if (this.instance == null) {
      this.instance = new DDR();
    }
    return this.instance;
  }

  init() {
    //初始化DDR 节点
    return CollectorType.TYPE_DDR;
  }

  isSupport() {
    if (isAccess(DdrConfig.DDR_BASE)) {
      return true;
    }
    return false;
  }

  readData() {
    const path = DdrConfig.DDR_BASE;
    let ddr = fileOpen(path);
    this.ddrMap.set('ddrFreq', ddr);
    return createGPData('DDR', this.ddrMap);
  }
}
