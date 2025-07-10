/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

import worker from '@ohos.worker';
import Log from '../../../../../../../common/src/main/ets/default/Log';
import PluginDataSourceManager from '../../../../../../../common/src/main/ets/plugindatasource/PluginDataSourceManager';
import Constants, { obtainMsg } from '../../../../../../../common/src/main/ets/plugindatasource/common/Constants';
import {writeFaultLog, FaultID} from '../../../../../../../common/src/main/ets/default/SysFaultLogger';

const parentPort = worker.parentPort;
const TAG = `${parentPort.name} Worker`;
Log.showInfo(TAG, 'Start.');

var sManager;

parentPort.onmessage = (msg) => {
  Log.showInfo(TAG, `onMessage, msg = ${JSON.stringify(msg)}`);
  let data = msg.data;
  switch (data.action) {
    case Constants.INIT_CONFIG:
      initConfig(data.data);
      break;
    case Constants.CLEAR_ALL:
      clearAll();
      break;
    case Constants.LOAD_DATA:
      loadData(data.data);
      break;
    case Constants.UPDATE_PLUGIN_COMPONENT_DATA:
      updatePluginComponentData(data.data);
      break;
    default:
      Log.showError(TAG, 'onMessage, unknown action type.');
  }
};

function initConfig(config) {
  Log.showDebug(TAG, `initConfig, config = ${JSON.stringify(config)}`);
  sManager = new PluginDataSourceManager({
    onItemAdd: (itemData) => {
      Log.showInfo(TAG, `sManager.onItemAdd, itemData = ${JSON.stringify(itemData)}`);
      itemData.label && (itemData.label = encodeURIComponent(itemData.label));
      parentPort.postMessage(obtainMsg(Constants.ADD_ITEM, itemData));
    },
    onItemRemove: (itemData) => {
      Log.showInfo(TAG, `sManager.onItemRemove, itemData = ${JSON.stringify(itemData)}`);
      parentPort.postMessage(obtainMsg(Constants.REMOVE_ITEM, itemData));
    },
    onLoadPluginComponentData: (itemData) => {
      Log.showInfo(TAG, `sManager.onLoadPluginComponentData, itemData = ${JSON.stringify(itemData)}`);
      parentPort.postMessage(obtainMsg(Constants.LOAD_PLUGIN_COMPONENT_DATA, itemData));
    },
  });
  sManager.initDataSource(config);
  parentPort.postMessage(obtainMsg(Constants.INIT_FINISH, {}));
}

function clearAll() {
  Log.showDebug(TAG, 'clearAll ');
  sManager?.clearAll();
}

function loadData(userId) {
  Log.showDebug(TAG, 'loadData ');
  sManager?.loadData(userId);
}

function updatePluginComponentData(pluginComponentData) {
  Log.showDebug(TAG, 'updatePluginComponentData ');
  sManager?.updatePluginComponentData(pluginComponentData);
}

parentPort.onclose = function () {
  Log.showDebug(TAG, 'onclose');
};

parentPort.onmessageerror = function () {
  Log.showError(TAG, 'onmessageerror');
};

parentPort.onerror = function (data) {
  Log.showError(
    TAG, `onerror, lineno = ${data.lineno}, msg = ${data.message}, filename = ${data.filename}, col = ${data.colno}`
  );
  writeFaultLog({CORE_SYSTEM: "com.ohos.systemui", TARGET_API: "systemui", FAULT_ID: FaultID.WORKER_ERROR
  , MSG: "Abnormal occurrence"});
};
