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

import Log from '../../../../../../../../common/src/main/ets/default/Log';
import createOrGet from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';
import { ControlComponentData, ControlCenterConfig } from '../common/Constants';
import ControlCenterService from '../model/ControlCenterService';
import { PluginType, PluginComponentData
} from '../../../../../../../../common/src/main/ets/plugindatasource/common/Constants';

export const CONTROL_CENTER_COMPLEX_TOGGLE_LAYOUT_KEY = 'ControlCenterComplexToggleLayout';

export const CONTROL_CENTER_SIMPLE_TOGGLE_LAYOUT_KEY = 'ControlCenterSimpleToggleLayout';

const TAG = 'ControlCenterVM';

export class ControlCenterVM {
  mIsStart = false;
  mComplexToggleLayout: SubscribedAbstractProperty<string[]>;
  mSimpleToggleLayout: SubscribedAbstractProperty<string[]>;

  constructor() {
    Log.showDebug(TAG, 'constructor');
    this.mComplexToggleLayout = AppStorage.SetAndLink(CONTROL_CENTER_COMPLEX_TOGGLE_LAYOUT_KEY, new Array<string>());
    this.mSimpleToggleLayout = AppStorage.SetAndLink(CONTROL_CENTER_SIMPLE_TOGGLE_LAYOUT_KEY, new Array<string>());
    ControlCenterService.registerListener(this);
  }

  initViewModel(config: ControlCenterConfig, moduleName: string): void {
    if (this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, `initViewModel, config: ${JSON.stringify(config)} `);
    this.mIsStart = true;

    ControlCenterService.startService(config, moduleName);
  }

  setComplexToggleLayout(layout: string[]): void{
    Log.showDebug(TAG, `setComplexToggleLayout, layout: ${JSON.stringify(layout)}`);
    if (JSON.stringify(layout) != JSON.stringify(this.mComplexToggleLayout.get())) {
      this.mComplexToggleLayout.set(layout);
    }
    Log.showDebug(TAG, `setComplexToggleLayout, mComplexToggleLayout: ${JSON.stringify(this.mComplexToggleLayout.get())}`);
  }

  setSimpleToggleLayout(layout: string[]): void{
    Log.showDebug(TAG, `setSimpleToggleLayout, layout: ${JSON.stringify(layout)}`);
    if (JSON.stringify(layout) != JSON.stringify(this.mSimpleToggleLayout.get())) {
      this.mSimpleToggleLayout.set(layout);
    }
    Log.showDebug(TAG, `setSimpleToggleLayout, mSimpleToggleLayout: ${JSON.stringify(this.mSimpleToggleLayout.get())}`);
  }

  setItemData(id: string, itemData: ControlComponentData): void{
    Log.showDebug(TAG, `setItemData, id: ${id} itemData: ${JSON.stringify(itemData)}`);
    let storageKey: string = 'ControlCenter_' + id;
    if (itemData) {
      AppStorage.SetOrCreate(storageKey, itemData);
    } else {
      let deleteRs: boolean = AppStorage.Delete(storageKey);
      Log.showDebug(TAG, `setItemData, AppStorage.Delete rs: ${deleteRs} `);
    }
    this.setPluginData(id, itemData);
  }


  setPluginData(id: string, itemData: ControlComponentData): void{
    Log.showInfo(TAG, `setPluginData, itemData: ${JSON.stringify(itemData)}`);
    if (itemData && itemData.pluginType == PluginType.PLUGIN_COMPONENT) {
      let data = undefined;
      if (itemData.actionData.pluginData && itemData.actionData.pluginData.data) {
        data = JSON.parse(JSON.stringify(itemData.actionData.pluginData.data));
      }
      let pluginData = this.getPluginData(id);
      pluginData.template = itemData.actionData.pluginData.template;
      pluginData.data = data;
      Log.showInfo(TAG, `setPluginData, pluginData: ${JSON.stringify(pluginData)} `);
    } else {
      let storageKey = 'ControlCenter_PluginIcon_' + id;
      if (AppStorage.Has(storageKey)) {
        let deleteRs: boolean = AppStorage.Delete(storageKey);
        Log.showInfo(TAG, `setPluginData, AppStorage.Delete rs: ${String(deleteRs)} `);
      }
    }
  }

  getPluginData(id: string): PluginComponentData {
    Log.showInfo(TAG, `getPluginData, id: ${id}`);
    let storageKey = 'ControlCenter_PluginIcon_' + id;
    if (!AppStorage.Has(storageKey)) {
      AppStorage.SetOrCreate(storageKey, new PluginComponentData());
    }
    return AppStorage.Get(storageKey);
  }

  getDisplayingSimpleToggles(): string[]{
    let simpleToggles = this.mSimpleToggleLayout.get();
    Log.showDebug(TAG, `getDisplayingSimpleToggles, simpleToggles: ${JSON.stringify(simpleToggles)}`);
    return simpleToggles;
  }

  getHidingSimpleToggles(): string[] {
    Log.showDebug(TAG, 'getHidingSimpleToggles');
    return ControlCenterService.getHidingSimpleToggles();
  }

  getDefaultSimpleToggleLayout(): string[] {
    Log.showDebug(TAG, 'getDefaultSimpleToggleLayout');
    return ControlCenterService.getDefaultSimpleToggleLayout();
  }

  saveSimpleToggleLayout(layout: string[]): void{
    Log.showDebug(TAG, `saveSimpleToggleLayout, layout: ${JSON.stringify(layout)}`);
    ControlCenterService.saveSimpleToggleLayout(layout);
  }
}

let sControlCenterVM = createOrGet(ControlCenterVM, TAG);

export default sControlCenterVM;