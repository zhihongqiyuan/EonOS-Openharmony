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
import { CatchTraceStatus } from './base/ProfilerConstant';

export default class WorkerHandler {
  static socketHandler(result): void {
    let arr = result.data.split('$');
    switch (arr[0]) {
      case 'RAM':
        globalThis.ramArr.push(arr[1]);
        break;
      case 'FPS':
        globalThis.fpsArr.push(arr[1]);
        globalThis.fpsJitterArr.push(arr[2]);
        globalThis.timerFps = arr[1];
        if (
          globalThis.catchTraceState === CatchTraceStatus.catch_trace_start ||
          globalThis.catchTraceState === CatchTraceStatus.catch_trace_finish ||
          globalThis.catchTraceState ===
            CatchTraceStatus.catch_trace_first_running
        ) {
          if (
            globalThis.fpsJitterArr !== undefined &&
            globalThis.fpsJitterArr !== null &&
            globalThis.fpsJitterArr !== ''
          ) {
            let tempQueue: Array<String> = globalThis.fpsJitterArr;
            let curJitter = tempQueue.pop();
            let tempJitterArr = curJitter.split('==');
            for (let i = 0; i < tempJitterArr.length; i++) {
              let tmp = tempJitterArr[i];
              let jitter = parseInt(tmp) / 1e6;
              if (jitter > 100) {
                globalThis.jitterTrace = true;
                return;
              }
            }
          }
        }
        break;
      case 'UdpStatus':
        let isSocketConnect = Number(arr[1]).valueOf();
        if (isSocketConnect > 0) {
          globalThis.useDaemon = true;
        } else {
          globalThis.useDaemon = false;
        }
      default:
        break;
    }
  }
}
