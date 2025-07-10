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
import SwitchUserManager, { UserInfo } from '../../../../../../../../common/src/main/ets/default/SwitchUserManager';
import CheckEmptyUtils from '../../../../../../../../common/src/main/ets/default/CheckEmptyUtils';
import SettingsUtil from '../../../../../../../../common/src/main/ets/default/SettingsUtil';
import AbilityManager from '../../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import PluginDataSourceAdapter from '../../../../../../../../common/src/main/ets/plugindatasource/PluginDataSourceAdapter';
import { ItemComponentData, FilterData
} from '../../../../../../../../common/src/main/ets/plugindatasource/common/Constants';
import Constants, { ControlComponentData, ControlCenterConfig } from '../common/Constants';

export interface ControlCenterListener {
  setComplexToggleLayout(layout: string[]): void;

  setSimpleToggleLayout(layout: string[]): void;

  setItemData(id: string, itemData: ControlComponentData): void;
}

const TAG = 'ControlCenterModel';
const SETTINGS_CONTROL_SIMPLE_TOGGLE_LAYOUT = 'settings.control.simpleToggleLayout';
const CONTROLCENTER_SOURCE_CONFIG = {
  action: 'com.ohos.systemui.action.TOGGLE',
  filterDatas: new Array<FilterData>(),
  loaderConfig: {
    MetaSource: {
      action: 'com.ohos.systemui.action.TOGGLE',
      permission: null,
    },
    PluginSourceLoader: {
      action: 'com.ohos.systemui.action.TOGGLE',
      permission: null,
    },
  },
};

function parseData(itemData: ItemComponentData): ControlComponentData {
  let { toggleType, ...other } = itemData.actionData?.extra;
  return {
    ...itemData,
    toggleType: toggleType ?? Constants.TOGGLE_TYPE_SIMPLE,
    actionData: { ...itemData.actionData, extra: undefined },
  };
}

export class ControlCenterService {
  mIsStart = false;
  mAdapter: PluginDataSourceAdapter;
  mListener: ControlCenterListener | undefined;
  mConfig: ControlCenterConfig;
  mComplexToggleLayoutTemplate: string[];
  mSimpleToggleLayoutTemplate: string[];
  mAllComplexToggles: string[] = [];
  mAllSimpleToggles: string[] = [];
  mComplexToggleLayout: string[];
  mSimpleToggleLayout: string[];

  constructor() {
    Log.showDebug(TAG, 'constructor');
  }

  startService(config: ControlCenterConfig, moduleName: string): void {
    if (this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'start ControlCenterService.');
    this.mIsStart = true;

    this.parseConfig(config);

    SwitchUserManager.getInstance().registerListener(this);
    CONTROLCENTER_SOURCE_CONFIG.filterDatas = config.MetaToggles;
    this.mAdapter = new PluginDataSourceAdapter(TAG, AbilityManager.getContext(AbilityManager.ABILITY_NAME_CONTROL_PANEL), this, moduleName);
    this.mAdapter.setWant(globalThis[Constants.PLUGIN_COMPONENT_OWNER_WANT_KEY]);
    this.mAdapter.initDataSource(CONTROLCENTER_SOURCE_CONFIG);
  }

  stopService(): void {
    if (!this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'stop ControlCenterService.');
    this.mIsStart = false;
    this.mAdapter.clearAll();
  }

  parseConfig(config: ControlCenterConfig): void {
    Log.showDebug(TAG, `parseConfig, config: ${JSON.stringify(config)}`);
    this.mConfig = config;

    this.mComplexToggleLayoutTemplate = [];
    config.ComplexToggleLayout.forEach((name: string) => {
      this.mComplexToggleLayoutTemplate.push(name);
    });
    this.loadSimpleToggleLayoutTemplate();
    Log.showDebug(TAG,
      `parseConfig, ComplexToggleLayoutTemplate: ${JSON.stringify(this.mComplexToggleLayoutTemplate)}`);
    Log.showDebug(TAG,
      `parseConfig, SimpleToggleLayoutTemplate: ${JSON.stringify(this.mSimpleToggleLayoutTemplate)}`);

    config.LocalToggles.ComplexToggles.forEach((name: string) => {
      this.mAllComplexToggles.push(name);
    });
    Log.showDebug(TAG, `parseConfig, mAllComplexToggles: ${JSON.stringify(this.mAllComplexToggles)}`);
    this.calcComplexToggleLayout();
    config.LocalToggles.SimpleToggles.forEach((name: string) => {
      typeof name === 'string' && this.mAllSimpleToggles.push(name);
    });
    Log.showDebug(TAG, `parseConfig, allSimpleToggles: ${JSON.stringify(this.mAllSimpleToggles)}`);
    this.calcSimpleToggleLayout();
  }

  loadSimpleToggleLayoutTemplate(): void{
    Log.showDebug(TAG, 'loadSimpleToggleLayoutTemplate');
    this.mSimpleToggleLayoutTemplate = [];
    let simpleToggleLayout = this.getSimpleToggleLayoutFromSettings();
    Log.showDebug(TAG, `simpleToggleLayout: ${JSON.stringify(simpleToggleLayout)}`);
    if (simpleToggleLayout) {
      this.mSimpleToggleLayoutTemplate = simpleToggleLayout;
    } else {
      this.mConfig.DefaultSimpleToggleLayout.forEach((name: string) => {
        this.mSimpleToggleLayoutTemplate.push(name);
      });
      this.setSimpleToggleLayoutToSettings(this.mSimpleToggleLayoutTemplate);
    }
    Log.showDebug(TAG, `loadSimpleToggleLayoutTemplate, mSimpleToggleLayoutTemplate: ${JSON.stringify(this.mSimpleToggleLayoutTemplate)}`);
  }

  calcComplexToggleLayout(): void {
    Log.showDebug(TAG, 'calcComplexToggleLayout');

    let complexToggleLayout: string[] = [];
    this.mComplexToggleLayoutTemplate.forEach((name) => {
      if (this.mAllComplexToggles.indexOf(name) >= 0) {
        complexToggleLayout.push(name);
      }
    });

    Log.showDebug(TAG, `calcComplexToggleLayout, complexToggleLayout: ${JSON.stringify(complexToggleLayout)}`);
    this.mComplexToggleLayout = complexToggleLayout;
    this.mListener?.setComplexToggleLayout(complexToggleLayout);
  }

  calcSimpleToggleLayout(): void {
    Log.showDebug(TAG, 'calcSimpleToggleLayout');

    let simpleToggleLayout: string[] = [];
    this.mSimpleToggleLayoutTemplate.forEach((name) => {
      if (this.mAllSimpleToggles.indexOf(name) >= 0) {
        simpleToggleLayout.push(name);
      }
    });

    Log.showDebug(TAG, `calcSimpleToggleLayout, simpleToggleLayout: ${JSON.stringify(simpleToggleLayout)}`);
    this.mSimpleToggleLayout = simpleToggleLayout;
    this.mListener?.setSimpleToggleLayout(simpleToggleLayout);
  }

  userChange(userInfo: UserInfo): void {
    Log.showInfo(TAG, `userChange userInfo ${userInfo}`);
    this.mAdapter.loadData(userInfo.userId);
  }

  registerListener(listener: ControlCenterListener): void {
    Log.showInfo(TAG, `registerListener, listener: ${listener}`);
    this.mListener = listener;
  }

  initFinish(): void {
    Log.showDebug(TAG, 'initFinish');
    SwitchUserManager.getInstance()
      .getCurrentUserInfo()
      .then((userInfo) => {
        this.mAdapter.loadData(userInfo.userId);
      }).catch((err) => {
    });
  }

  onItemAdd(itemData: ItemComponentData): void {
    Log.showDebug(TAG, `onItemAdd, itemData: ${JSON.stringify(itemData)}`);
    let controlData: ControlComponentData = parseData(itemData);
    let id: string = controlData.id;
    this.mListener?.setItemData(id, controlData);
    if (controlData.toggleType == Constants.TOGGLE_TYPE_COMPLEX) {
      if (this.mAllComplexToggles.indexOf(id) < 0) {
        this.mAllComplexToggles.push(id);
        Log.showDebug(TAG, `onItemAdd, mAllComplexToggles: ${JSON.stringify(this.mAllComplexToggles)}`);
        this.calcComplexToggleLayout();
      }
    } else {
      if (this.mAllSimpleToggles.indexOf(id) < 0) {
        this.mAllSimpleToggles.push(id);
        Log.showDebug(TAG, `onItemAdd, mAllSimpleToggles: ${JSON.stringify(this.mAllSimpleToggles)}`);
        this.calcSimpleToggleLayout();
      }
    }
  }

  onItemRemove(itemData: ItemComponentData): void {
    Log.showDebug(TAG, `onItemRemove, itemData: ${JSON.stringify(itemData)}`);
    let id: string = itemData.id;
    if (this.mAllComplexToggles.indexOf(id) >= 0) {
      this.mAllComplexToggles.splice(this.mAllComplexToggles.indexOf(id), 1);
      Log.showDebug(TAG, `onItemRemove, mAllComplexToggles: ${JSON.stringify(this.mAllComplexToggles)}`);
      this.calcComplexToggleLayout();
    } else if (this.mAllSimpleToggles.indexOf(id) >= 0) {
      this.mAllSimpleToggles.splice(this.mAllSimpleToggles.indexOf(id), 1);
      Log.showDebug(TAG, `onItemRemove, mAllSimpleToggles: ${JSON.stringify(this.mAllSimpleToggles)}`);
      this.calcSimpleToggleLayout();
    }
    this.mListener?.setItemData(id, undefined);
  }

  getSimpleToggleLayoutFromSettings(): string[] {
    Log.showDebug(TAG, 'getSimpleToggleLayoutFromSettings');
    let value = SettingsUtil.getValue(SETTINGS_CONTROL_SIMPLE_TOGGLE_LAYOUT);
    let simpleToggleLayout: string[] = null;
    if (!CheckEmptyUtils.isEmpty(value)) {
      simpleToggleLayout = JSON.parse(value) as string[];
    }
    Log.showDebug(TAG, `getSimpleToggleLayoutFromSettings, simpleToggleLayout: ${JSON.stringify(simpleToggleLayout)}`);
    return simpleToggleLayout;
  }

  setSimpleToggleLayoutToSettings(simpleToggleLayout: string[]): void {
    Log.showDebug(TAG, `setSimpleToggleLayoutToSettings, simpleToggleLayout: ${JSON.stringify(simpleToggleLayout)}`);
    let value: string = JSON.stringify(simpleToggleLayout);
    SettingsUtil.setValue(SETTINGS_CONTROL_SIMPLE_TOGGLE_LAYOUT, value);
  }

  getHidingSimpleToggles(): string[] {
    Log.showDebug(TAG, 'getHidingSimpleToggles');
    let hidingSimpleToggles: string[] = [];
    this.mAllSimpleToggles.forEach((toggleName) => {
      if (this.mSimpleToggleLayout.indexOf(toggleName) < 0) {
        hidingSimpleToggles.push(toggleName);
      }
    });
    Log.showDebug(TAG, `getHidingSimpleToggles, hidingSimpleToggles: ${JSON.stringify(hidingSimpleToggles)}`);
    return hidingSimpleToggles;
  }

  getDefaultSimpleToggleLayout(): string[] {
    Log.showDebug(TAG, 'getDefaultSimpleToggleLayout');
    let defaultToggles: string[] = [];
    this.mConfig.DefaultSimpleToggleLayout.forEach((toggleName: string) => {
      if (this.mAllSimpleToggles.indexOf(toggleName) >= 0) {
        defaultToggles.push(toggleName);
      }
    });
    Log.showDebug(TAG, `getDefaultSimpleToggleLayout, defaultToggles: ${JSON.stringify(defaultToggles)}`);
    return defaultToggles;
  }

  saveSimpleToggleLayout(layout: string[]): void{
    Log.showDebug(TAG, `saveSimpleToggleLayout, layout: ${JSON.stringify(layout)}`);
    this.setSimpleToggleLayoutToSettings(layout);
    this.mSimpleToggleLayoutTemplate = [];
    layout.forEach((name) => {
      this.mSimpleToggleLayoutTemplate.push(name);
    });
    Log.showDebug(TAG, `saveSimpleToggleLayout, mSimpleToggleLayoutTemplate: ${JSON.stringify(this.mSimpleToggleLayoutTemplate)}`);
    this.calcSimpleToggleLayout();
  }
}

let sControlCenterService = createOrGet(ControlCenterService, TAG);

export default sControlCenterService;