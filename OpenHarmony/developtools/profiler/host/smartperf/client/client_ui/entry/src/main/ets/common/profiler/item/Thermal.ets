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
import { fileOpen, createGPData, isAccess } from '../base/BaseProfilerUtils';
import { BaseProfiler } from '../base/BaseProfiler';
import { CollectorType } from '../base/ProfilerConstant';
import SPLogger from '../../../common/utils/SPLogger';
enum ThermalConfig {
  THERMAL_BASE = '/sys/devices/virtual/thermal',
}

export class Thermal extends BaseProfiler {
  private thermalMap: Map<String, String> = new Map();
  private enableSupportItem = ['gpu-thermal', 'soc-thermal', 'shell_back', 'shell_front', 'shell_frame'];
  private supportItemMap = new Map();

  public static instance: Thermal = null;
  public static getInstance() {
    if (this.instance == null) {
      this.instance = new Thermal();
    }
    return this.instance;
  }

  init() {
    //初始化Thermal node
    var dir;
    dir = fileio.opendirSync(ThermalConfig.THERMAL_BASE);
    do {
      var dirent = dir.readSync();
      if (dirent == null) {
        break;
      }
      let name: String = dirent.name;
      SPLogger.DEBUG(Thermal.name, 'thermal_zone name:' + name);
      if (name.match('thermal_zone') && name.match('thermal_zone1') == null) {
        let typeName = fileOpen(ThermalConfig.THERMAL_BASE + '/' + dirent.name + '/type');
        if (this.enableSupportItem.length > 0) {
          this.enableSupportItem.find((item: String) => {
            if (typeName.match(item.toString())) {
              SPLogger.DEBUG(Thermal.name, 'thermal_zone match name:' + typeName);
              this.supportItemMap.set(name, item);
              SPLogger.DEBUG(Thermal.name, 'thermal_zone match name this.supportItemMap:' + this.supportItemMap.size);
            }
          });
        }
      }
    } while (dirent != null);
    return CollectorType.TYPE_TEMPERATURE;
  }

  isSupport() {
    if (isAccess(ThermalConfig.THERMAL_BASE)) {
      return true;
    }
    return false;
  }

  readData() {
    if (this.supportItemMap.size > 0) {
      let timeZones = this.supportItemMap.keys();
      for (let timeZone of timeZones) {
        let tempValue = fileOpen(ThermalConfig.THERMAL_BASE + '/' + timeZone + '/temp');
        this.thermalMap.set(this.supportItemMap.get(timeZone), tempValue);
      }
    }
    return createGPData('Temp', this.thermalMap);
  }
}
