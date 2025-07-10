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
import { fileOpen, travelFile, createGPData, isAccess } from '../base/BaseProfilerUtils';
import { BaseProfiler } from '../base/BaseProfiler';
import StringUtils from '../../utils/StringUtils';
import { CollectorType } from '../base/ProfilerConstant';
import SPLogger from '../../../common/utils/SPLogger';
enum PowerConfig {
  //设备电源驱动默认节点
  POWER_PATH = '/sys/class/power_supply/Battery',
}

export class Power extends BaseProfiler {
  private powerMap: Map<String, String> = new Map();
  private enableSupportItem = ['current_now', 'voltage_now', 'charge_now', 'temp', 'status'];
  private supportItemKey = [];

  public static instance: Power = null;
  public static getInstance() {
    if (this.instance == null) {
      this.instance = new Power();
    }
    return this.instance;
  }

  init() {
    //初始化Power节点
    let pathNodes = travelFile(PowerConfig.POWER_PATH, '');
    pathNodes.forEach((path) => {
      this.enableSupportItem.forEach((item) => {
        if (path == item) {
          this.supportItemKey.push(item);
        }
      });
    });
    return CollectorType.TYPE_POWER;
  }

  isSupport() {
    if (isAccess(PowerConfig.POWER_PATH)) {
      return true;
    }
    return false;
  }

  readData() {
    if (this.supportItemKey.length > 0) {
      for (let powerKey of Array.from(new Set(this.supportItemKey))) {
        if (powerKey == 'current_now') {
          let powerValue = this.readCurrentNow();
          this.powerMap.set(powerKey, powerValue);
        } else if (powerKey == 'voltage_now') {
          let powerValue = this.readVoltageNow();
          this.powerMap.set(powerKey, powerValue);
        } else {
          let powerValue = fileOpen(PowerConfig.POWER_PATH + '/' + powerKey);
          this.powerMap.set(powerKey, powerValue);
        }
      }
    }
    return createGPData('Power', this.powerMap);
  }

  readFourTimesData() {
    if (this.supportItemKey.length > 0) {
      for (let powerKey of Array.from(new Set(this.supportItemKey))) {
        if (powerKey == 'current_now') {
          let powerValue = fileOpen(PowerConfig.POWER_PATH + '/' + powerKey);
          globalThis.powerCurArr.push(powerValue);
        }

        if (powerKey == 'voltage_now') {
          let powerValue = fileOpen(PowerConfig.POWER_PATH + '/' + powerKey);
          globalThis.powerVoltArr.push(powerValue);
        }
      }
    }
  }

  //TODO 电流是否取绝对值累加
  readCurrentNow(): string {
    let powerQueue: string[] = globalThis.powerCurArr;
    let lastValue = 0;
    while (powerQueue.length >= 4) {
      lastValue += StringUtils.s2L(powerQueue.pop().toString());
    }
    let avgCurrentNum = (lastValue / 4).toString();
    return avgCurrentNum;
  }

  //TODO 电流是否取绝对值累加
  readVoltageNow(): string {
    let powerQueue: string[] = globalThis.powerVoltArr;
    let lastValue = 0;
    while (powerQueue.length >= 4) {
      lastValue += StringUtils.s2L(powerQueue.pop().toString());
    }
    let avgCurrentNum = (lastValue / 4).toString();
    return avgCurrentNum;
  }
}
