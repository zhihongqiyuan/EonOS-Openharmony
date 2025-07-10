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
import { createGPData, extractNumber } from '../base/BaseProfilerUtils';
import { BaseProfiler } from '../base/BaseProfiler';
import { CollectorType } from '../base/ProfilerConstant';
import { SocketProfiler } from '../base/SocketProfiler';
import WorkerHandler from '../WorkerHandler';
let mainWorker = globalThis.MainWorker;

mainWorker.onmessage = function (result) {
  WorkerHandler.socketHandler(result);
};

export class RAM extends BaseProfiler implements SocketProfiler {
  private ramMap: Map<String, String> = new Map();
  public static instance: RAM = null;
  public static getInstance() {
    if (this.instance == null) {
      this.instance = new RAM();
    }
    return this.instance;
  }

  init() {
    //初始化RAM节点
    return CollectorType.TYPE_RAM;
  }

  isSupport() {
    if (globalThis.useDaemon) {
      return true;
    }
    return false;
  }

  readData() {
    if (globalThis.useDaemon) {
      this.readMessageQueue();
    }
    return createGPData('RAM', this.ramMap);
  }
  readMessageQueue() {
    mainWorker.postMessage({ ram: true, pkg: globalThis.collectPkg });
    if (globalThis.ramArr.length > 0) {
      let ramQueue: String[] = globalThis.ramArr;
      let curRAM = ramQueue.pop();
      this.ramMap.set('pss', extractNumber(curRAM));
    }
  }
}
