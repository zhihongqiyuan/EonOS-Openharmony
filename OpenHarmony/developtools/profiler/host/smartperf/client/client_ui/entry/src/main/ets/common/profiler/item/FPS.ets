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
import SPLogger from '../../../common/utils/SPLogger';

import worker from '@ohos.worker';
import WorkerHandler from '../WorkerHandler';
let mainWorker = globalThis.MainWorker;

mainWorker.onmessage = function (result) {
  WorkerHandler.socketHandler(result);
};

export class FPS extends BaseProfiler implements SocketProfiler {
  private fpsMap: Map<String, String> = new Map;

  public static instance: FPS = null;
  public static getInstance() {
    if (this.instance == null) {
      this.instance = new FPS();
    }
    return this.instance;
  }

  init() {
    //初始化FPS
    return CollectorType.TYPE_FPS;
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
    return createGPData('FPS', this.fpsMap);
  }

  readMessageQueue() {
    mainWorker.postMessage({ fps: true, pkg: globalThis.collectPkg });
    if (globalThis.fpsArr.length > 0) {
      let fpsQueue: String[] = globalThis.fpsArr;
      let fpsJitterQueue: String[] = globalThis.fpsJitterArr;
      let curFPS = fpsQueue.pop();
      globalThis.timerFps = curFPS;
      let curFPSJitter = fpsJitterQueue.pop();
      let fpsJitters = '"' + curFPSJitter.split('==').join(',') + '"';
      this.fpsMap.set('fpsJitters', fpsJitters);
      this.fpsMap.set('fps', extractNumber(curFPS));
    }
  }
}
