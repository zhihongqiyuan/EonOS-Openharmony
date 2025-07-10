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

import Ability from '@ohos.app.ability.UIAbility';
import { initDb } from '../common/database/LocalRepository';
import { FloatWindowFun } from '../common/ui/floatwindow/FloatWindowFun';
import { NetWork } from '../common/profiler/item/NetWork';
import BundleManager from '../common/utils/BundleMangerUtils';
import display from '@ohos.display'; // 导入模块
import WorkerHandler from '../common/profiler/WorkerHandler';
import worker from '@ohos.worker';

let MainWorker = new worker.Worker('entry/ets/workers/worker.js');
globalThis.MainWorker = MainWorker;

MainWorker.onmessage = function (result): void {
  WorkerHandler.socketHandler(result);
};

let abilityWindowStage;
export default class MainAbility extends Ability {
  onCreate(): void {
    console.log('cm-MainAbility-onCreate');
    globalThis.showFloatingWindow = false;
    BundleManager.getAppList().then((appList) => {
      globalThis.appList = appList;
    });
  }
  onDestroy(): void {
    console.log('cm-MainAbility-onDestroy');
    MainWorker.terminate();
  }
  onWindowStageCreate(windowStage): void {
    globalThis.abilityContext = this.context;
    abilityWindowStage = windowStage;
    abilityWindowStage.setUIContent(this.context, 'pages/LoginPage', null);
    globalThis.useDaemon = false;
    display.getDefaultDisplay().then(
      (disp) => {
        globalThis.screenWith = disp.width;
        if (globalThis.screenWith > 1400) {
          globalThis.coefficient = 1;
          console.log(
            'globalThis.screenWith :coefficient-- ' + globalThis.coefficient
          );
        } else if (
          globalThis.screenWith > 800 &&
          globalThis.screenWith < 1400
        ) {
          globalThis.coefficient = 1.8;
          console.log(
            'globalThis.screenWith :coefficient-- ' + globalThis.coefficient
          );
        } else {
          globalThis.coefficient = 1;
          console.log(
            'globalThis.screenWith :coefficient-- ' + globalThis.coefficient
          );
        }
        console.log('globalThis.screenWith : ' + globalThis.screenWith);
      },
      (err) => {
        console.log(
          'display.getDefaultDisplay failed, error : ' + JSON.stringify(err)
        );
      }
    );
  }
  onWindowStageDestroy(): void {}
  onForeground(): void {
    console.log('cm-MainAbility-onForeground');
    initDb();
    FloatWindowFun.initAllFun();
    //NetWork.getInstance().init()
    MainWorker.postMessage({ testConnection: true });
  }
  onBackground(): void {}
}
