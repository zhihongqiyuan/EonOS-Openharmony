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
import { createGPData } from '../base/BaseProfilerUtils';
import { BaseProfiler } from '../base/BaseProfiler';
import { CollectorType } from '../base/ProfilerConstant';
import connection from '@ohos.net.connection';
import SPLogger from '../../../common/utils/SPLogger';
import { GPData } from '../../entity/DatabaseEntity';

export class NetWork extends BaseProfiler {
  private netMap: Map<String, String> = new Map();
  private catchNetUp = '-1';
  private catchNetDown = '-1';
  public static instance: NetWork = null;
  public static getInstance(): NetWork {
    if (this.instance == null) {
      this.instance = new NetWork();
    }
    return this.instance;
  }

  init(): CollectorType {
    //初始化NET
    connection.getDefaultNet().then(function (netHandle) {
      connection.getNetCapabilities(netHandle).then(function (info) {
        if (info !== undefined) {
          globalThis.isNetAlive = true;
        } else {
          globalThis.isNetAlive = false;
        }
      });
    });
    return CollectorType.TYPE_NET;
  }

  isSupport(): Boolean {
    SPLogger.DEBUG(NetWork.name, 'networkInfo support:' + globalThis.isNetAlive);
    return globalThis.isNetAlive;
  }

  readData(): GPData {
    this.handleMessage();
    this.netMap.set('netSpeedUp', this.catchNetUp);
    this.netMap.set('netSpeedDown', this.catchNetDown);
    return createGPData('NetWork', this.netMap);
  }

  handleMessage(): void {
    connection.getDefaultNet().then(function (netHandle) {
      connection.getNetCapabilities(netHandle).then(function (info) {
        SPLogger.DEBUG(NetWork.name, 'networkInfo up:' + info.linkUpBandwidthKbps);
        SPLogger.DEBUG(NetWork.name, 'networkInfo down:' + info.linkDownBandwidthKbps);
        this.catchNetDown = info.linkDownBandwidthKbps.toString();
        this.catchNetUp = info.linkUpBandwidthKbps.toString();
      });
    });
  }
}
