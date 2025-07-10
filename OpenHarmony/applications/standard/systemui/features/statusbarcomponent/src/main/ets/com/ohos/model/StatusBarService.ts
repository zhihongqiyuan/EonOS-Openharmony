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
import { FASlotName } from '../../../../../../../../common/src/main/ets/default/Constants';
import createOrGet from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';
import SwitchUserManager, { UserInfo } from '../../../../../../../../common/src/main/ets/default/SwitchUserManager';
import { ItemComponentData, PluginComponentData, FilterData
} from '../../../../../../../../common/src/main/ets/plugindatasource/common/Constants';
import PluginDataSourceAdapter from '../../../../../../../../common/src/main/ets/plugindatasource/PluginDataSourceAdapter';
import AbilityManager from '../../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import Constants, { StatusBarData, StatusBarComponentData, LayoutGroup, StatusBarConfig } from '../common/Constants';
import { parseEventString } from '../../../../../../../../common/src/main/ets/default/event/EventUtil';

export interface StatusBarListener {
  setStatusBarLayout(layout: string[][]): void;

  setStatusBarEmptyWidth(width: number): void;

  setItemData(id: string, itemData: StatusBarComponentData): void;
}

const TAG = 'StatusBarModel';
const STATUSBAR_SOURCE_CONFIG = {
  action: 'com.ohos.systemui.action.BAR_ICON',
  filterDatas: new Array<FilterData>(),
  loaderConfig: {
    MetaSource: {
      action: 'com.ohos.systemui.action.BAR_ICON',
      permission: null,
    },
    PluginSourceLoader: {
      action: 'com.ohos.systemui.action.BAR_ICON',
      permission: null,
    },
  },
};

function parseItemData(itemData: ItemComponentData): StatusBarComponentData {
  let { isShowLabel, isShowIcon, selectedClickAction, relationWindowId, ...other } = itemData.actionData?.extra;
  let selectAction = parseEventString(selectedClickAction);
  let statusBarItemData: StatusBarComponentData = {
    ...itemData,
    isShowLabel: isShowLabel ? isShowLabel !== false : false,
    isShowIcon: isShowIcon ? isShowIcon !== false : true,
    canSelect: selectAction ? true : false,
    relationWindowId: relationWindowId,
    actionData: {
      ...itemData.actionData,
      selectedClickAction: selectAction,
      extra: undefined,
    },
  };
  return statusBarItemData;
}

export class StatusBarService {
  mIsStart = false;
  mAdapter: PluginDataSourceAdapter;
  mListener: StatusBarListener | undefined;
  mConfig: StatusBarConfig;
  mStatusBarData: StatusBarData;
  mStatusBarLayoutGroupTemplate: string[][] = [];
  mStatusBarAllLayout: string[];

  constructor() {
    Log.showInfo(TAG, 'constructor');
  }

  setStatusBarData(data: StatusBarData): void{
    Log.showInfo(TAG, 'setStatusBarData');
    this.mStatusBarData = data;
  }

  startService(config: StatusBarConfig, moduleName: string): void {
    if (this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'start StatusBarService.');
    this.mIsStart = true;

    this.parseConfig(config);

    SwitchUserManager.getInstance().registerListener(this);
    STATUSBAR_SOURCE_CONFIG.filterDatas = config.MetaSlots;
    this.mAdapter = new PluginDataSourceAdapter(TAG, AbilityManager.getContext(AbilityManager.ABILITY_NAME_STATUS_BAR), this, moduleName);
    this.mAdapter.setWant(globalThis[Constants.PLUGIN_COMPONENT_OWNER_WANT_KEY]);
    this.mAdapter.initDataSource(STATUSBAR_SOURCE_CONFIG);
  }

  stopService(): void {
    if (!this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'stop StatusBarService.');
    this.mIsStart = false;

    this.mAdapter.clearAll();
  }

  parseConfig(config: StatusBarConfig): void {
    Log.showInfo(TAG, `parseConfig, config: ${JSON.stringify(config)}`);
    this.mConfig = config;

    const groupIds = [Constants.GROUP_ID_LEFT, Constants.GROUP_ID_CENTER, Constants.GROUP_ID_RIGHT];
    groupIds.forEach((groupId) => {
      for (const groupInfo of config.LayoutGroups) {
        if (groupId == groupInfo.id) {
          this.mStatusBarLayoutGroupTemplate.push([...groupInfo.Components]);
          break;
        }
      }
    });
    Log.showInfo(TAG, `parseConfig, statusBarLayoutGroupTemplate: ${JSON.stringify(this.mStatusBarLayoutGroupTemplate)}`);

    this.mStatusBarAllLayout = [...config.LocalSlots];
    this.onDisplayRotate(0);
  }

  calcStatusBarLayout(): void {
    Log.showInfo(TAG, 'calcStatusBarLayout');
    let statusBarLayout: string[][] = [];
    this.mStatusBarLayoutGroupTemplate.forEach(componentsTemplate => {
      let components: string [] = [];
      for (let componentTemplate of componentsTemplate) {
        if (this.mStatusBarAllLayout.indexOf(componentTemplate) >= 0) {
          components.push(componentTemplate);
        }
      }
      statusBarLayout.push(components);
    });
    Log.showDebug(TAG, `calcStatusBarLayout, statusBarLayout: ${JSON.stringify(statusBarLayout)}`);
    this.mListener?.setStatusBarLayout(statusBarLayout);
  }

  userChange(userInfo: UserInfo): void {
    Log.showInfo(TAG, 'userChange');
    this.mAdapter.loadData(userInfo.userId);
  }

  initFinish(): void {
    Log.showInfo(TAG, 'initFinish');
    SwitchUserManager.getInstance()
      .getCurrentUserInfo()
      .then((userInfo) => {
        this.mAdapter.loadData(userInfo.userId);
      }).catch((err) => {
    });
  }

  registerListener(listener: StatusBarListener | undefined): void {
    Log.showInfo(TAG, `registerListener, listener: ${listener}`);
    this.mListener = listener;
  }

  onItemAdd(itemData: ItemComponentData): void {
    Log.showInfo(TAG, 'onItemAdd');
    let statusBarData: StatusBarComponentData = parseItemData(itemData);
    let id = itemData.id;
    this.mListener?.setItemData(id, statusBarData);
    if (this.mStatusBarAllLayout.indexOf(id) < 0) {
      this.mStatusBarAllLayout.push(id);
      Log.showInfo(TAG, `onItemAdd, mStatusBarAllLayout: ${JSON.stringify(this.mStatusBarAllLayout)}`);
      this.calcStatusBarLayout();
    }
  }

  onItemRemove(itemData: ItemComponentData): void {
    Log.showInfo(TAG, 'onItemRemove');
    let id = itemData.id;
    if (this.mStatusBarAllLayout.indexOf(id) >= 0) {
      this.mStatusBarAllLayout.splice(this.mStatusBarAllLayout.indexOf(id), 1);
      Log.showInfo(TAG, `onItemRemove, mStatusBarAllLayout: ${JSON.stringify(this.mStatusBarAllLayout)}`);
      this.calcStatusBarLayout();
    }
    this.mListener?.setItemData(id, undefined);
  }

  onDisplayRotate(rotation: number): void {
    Log.showInfo(TAG, `onDisplayRotate, rotation: ${rotation}`);
    let position: string = this.calcEmptyAreaPosition(rotation);
    this.onEmptyAreaChange(position, rotation);
  }

  calcEmptyAreaPosition(rotation: number): string {
    Log.showInfo(TAG, `calcEmptyAreaPosition, rotation: ${rotation}`);
    let configEmptyPosition = this.mConfig.emptyPosition;
    if (!configEmptyPosition || configEmptyPosition.x1 == configEmptyPosition.x2 || configEmptyPosition.y1 == configEmptyPosition.y2) {
      return null;
    }

    let leftRightArea = this.calcStatusBarLeftRightArea();
    let statusBarLeft = leftRightArea.left;
    let statusBarRight = leftRightArea.right;

    let emptyPosition;
    if (rotation == 0) {
      emptyPosition = { ...configEmptyPosition };
    } else if (rotation == 90) {
      emptyPosition = {
        x1: this.mStatusBarData.displayWidth - configEmptyPosition.y2,
        y1: configEmptyPosition.x1,
        x2: this.mStatusBarData.displayWidth - configEmptyPosition.y1,
        y2: configEmptyPosition.x2
      };
    } else if (rotation == 180) {
      emptyPosition = {
        x1: this.mStatusBarData.displayWidth - configEmptyPosition.x2,
        y1: this.mStatusBarData.displayHeight - configEmptyPosition.y2,
        x2: this.mStatusBarData.displayWidth - configEmptyPosition.x1,
        y2: this.mStatusBarData.displayHeight - configEmptyPosition.y1,
      };
    } else {
      emptyPosition = {
        x1: configEmptyPosition.y1,
        y1: this.mStatusBarData.displayHeight - configEmptyPosition.x2,
        x2: configEmptyPosition.y2,
        y2: this.mStatusBarData.displayHeight - configEmptyPosition.x1
      };
    }

    let isGlandLeft = this.isEmptyAreaGlandStatusBar(emptyPosition, statusBarLeft);
    let isGlandRight = this.isEmptyAreaGlandStatusBar(emptyPosition, statusBarRight);
    Log.showInfo(TAG, `calcEmptyAreaPosition, gland: ${isGlandLeft} ${isGlandRight}`);
    if (isGlandLeft && isGlandRight) {
      return Constants.EMPTY_AREA_POSITION_CENTER;
    } else if (isGlandLeft) {
      return Constants.EMPTY_AREA_POSITION_LEFT;
    } else if (isGlandRight) {
      return Constants.EMPTY_AREA_POSITION_RIGHT;
    } else {
      return null;
    }
  }

  calcStatusBarLeftRightArea(): {
    left: {
      x1: number;
      y1: number;
      x2: number;
      y2: number;
    },
    right: {
      x1: number;
      y1: number;
      x2: number;
      y2: number;
    }
  }{
    let statusBarLeft;
    let statusBarRight;
    if (this.mStatusBarData.showHorizontal) {
      statusBarLeft = {
        x1: this.mStatusBarData.left,
        y1: this.mStatusBarData.top,
        x2: this.mStatusBarData.left + this.mStatusBarData.realWidth / 2,
        y2: this.mStatusBarData.top + this.mStatusBarData.realHeight
      };
      statusBarRight = {
        x1: statusBarLeft.x2,
        y1: statusBarLeft.y1,
        x2: this.mStatusBarData.left + this.mStatusBarData.realWidth,
        y2: statusBarLeft.y2
      };
    } else {
      statusBarLeft = {
        x1: this.mStatusBarData.left,
        y1: this.mStatusBarData.top,
        x2: this.mStatusBarData.left + this.mStatusBarData.realWidth,
        y2: this.mStatusBarData.top + this.mStatusBarData.realHeight / 2
      };
      statusBarRight = {
        x1: statusBarLeft.x1,
        y1: statusBarLeft.y2,
        x2: statusBarLeft.x2,
        y2: this.mStatusBarData.top + this.mStatusBarData.realHeight
      };
    }
    Log.showInfo(TAG, `calcEmptyAreaPosition, statusBarLeft: ${JSON.stringify(statusBarLeft)} statusBarRight: ${JSON.stringify(statusBarRight)}`);
    return { left: statusBarLeft, right: statusBarRight };
  }

  isEmptyAreaGlandStatusBar(emptyPosition: {
    x1: number;
    y1: number;
    x2: number;
    y2: number;
  }, statusBarArea: {
    x1: number;
    y1: number;
    x2: number;
    y2: number;
  }): boolean {
    Log.showInfo(TAG, `isEmptyAreaGlandStatusBar, emptyPosition: ${JSON.stringify(emptyPosition)} statusBarArea: ${JSON.stringify(statusBarArea)}`);
    let ex1 = emptyPosition.x1;
    let ey1 = emptyPosition.y1;
    let ex2 = emptyPosition.x2;
    let ey2 = emptyPosition.y2;
    let x1 = statusBarArea.x1;
    let y1 = statusBarArea.y1;
    let x2 = statusBarArea.x2;
    let y2 = statusBarArea.y2;

    if (ex1 >= x1 && ex1 <= x2 && ey1 >= y1 && ey1 <= y2) {
      return true;
    } else if (ex2 >= x1 && ex2 <= x2 && ey1 >= y1 && ey1 <= y2) {
      return true;
    } else if (ex1 >= x1 && ex1 <= x2 && ey2 >= y1 && ey2 <= y2) {
      return true;
    } else if (ex2 >= x1 && ex2 <= x2 && ey2 >= y1 && ey2 <= y2) {
      return true;
    }
    return false;
  }

  onEmptyAreaChange(position: string, rotation: number): void {
    Log.showInfo(TAG, `onEmptyAreaChange, position: ${position} rotation: ${rotation}`);
    this.calcEmptyWidth(rotation);
    this.setEmptyAreaToLayoutTemplate(position);
    let id = FASlotName.EMPTY;
    if (this.mStatusBarAllLayout.indexOf(id) >= 0) {
      this.mStatusBarAllLayout.splice(this.mStatusBarAllLayout.indexOf(id), 1);
    }
    if (position) {
      this.mStatusBarAllLayout.push(id);
    }
    Log.showInfo(TAG, `onEmptyAreaChange, mStatusBarAllLayout: ${JSON.stringify(this.mStatusBarAllLayout)}`);
    this.calcStatusBarLayout();
  }

  setEmptyAreaToLayoutTemplate(position: string): void {
    Log.showInfo(TAG, `setEmptyAreaToLayoutTemplate, position: ${position}`);
    for (let componentsTemplate of this.mStatusBarLayoutGroupTemplate) {
      let index = componentsTemplate.indexOf(FASlotName.EMPTY);
      if (index >= 0) {
        componentsTemplate.splice(index, 1);
        break;
      }
    }
    if (position == Constants.EMPTY_AREA_POSITION_LEFT) {
      this.mStatusBarLayoutGroupTemplate[0].splice(0, 0, FASlotName.EMPTY);
    } else if (position == Constants.EMPTY_AREA_POSITION_CENTER) {
      this.mStatusBarLayoutGroupTemplate[1].splice(0, 0, FASlotName.EMPTY);
    } else if (position == Constants.EMPTY_AREA_POSITION_RIGHT) {
      this.mStatusBarLayoutGroupTemplate[2].push(FASlotName.EMPTY);
    }
    Log.showInfo(TAG, `setEmptyAreaToLayoutTemplate, template: ${JSON.stringify(this.mStatusBarLayoutGroupTemplate)}`);
  }

  calcEmptyWidth(rotation: number): void {
    Log.showInfo(TAG, `calcEmptyWidth, rotation: ${rotation}`);
    let width = 0;
    if (this.mConfig.emptyPosition) {
      if (rotation == 0 || rotation == 180) {
        width = this.mConfig.emptyPosition.x2 - this.mConfig.emptyPosition.x1;
      } else {
        width = this.mConfig.emptyPosition.y2 - this.mConfig.emptyPosition.y1;
      }
    }
    Log.showInfo(TAG, `calcEmptyWidth, width: ${width}`);
    this.mListener?.setStatusBarEmptyWidth(width);
  }
}

let sStatusBarService = createOrGet(StatusBarService, TAG);

export default sStatusBarService;