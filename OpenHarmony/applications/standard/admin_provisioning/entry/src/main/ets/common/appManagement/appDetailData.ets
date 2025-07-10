/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

import bundle from '@ohos.bundle.bundleManager';
import accountManager, { UserId } from '../accountManager';
import baseData from '../baseData';
import logger from '../logger'
import utils from '../utils'
import { AppPermission, MyApplicationInfo } from '../myApplicationInfo'
import Want from '@ohos.app.ability.Want';
import resourceManager from '@ohos.resourceManager';
import common from '@ohos.app.ability.common';

let defaultIcon = $r('app.media.icon');
const TAG = 'AppDetailData';

const permissionsDetailList: AppPermission[] = [
  {
    permissionName: 'ohos.permission.ENTERPRISE_SET_DATETIME',
    permissionLabel: $r('app.string.permissionLabel'),
    permissionDescription: $r('app.string.permissionDescription')
  },
];

export class AppDetailData {
  public ret: boolean = true;

  async checkAppItem(elementNameVal: Want) {
    logger.info(TAG, 'checkAppItem in bundleName:' + elementNameVal.bundleName + ' | abilityName:' +
    elementNameVal.abilityName);
    let want: Want = {
      'bundleName': elementNameVal.bundleName,
      'abilityName': elementNameVal.abilityName
    };
    let userId: UserId = { localId: 0 };
    let retVal = await accountManager.getAccountUserId(userId);
    if (!retVal) {
      logger.warn(TAG, 'checkAppItem getAccountUserId fail!');
      this.ret = false;
      return this.ret;
    }
    let data: bundle.ExtensionAbilityInfo[] = [];
    try {
      await bundle.queryExtensionAbilityInfo(want, bundle.ExtensionAbilityType.ENTERPRISE_ADMIN,
      bundle.ExtensionAbilityFlag.GET_EXTENSION_ABILITY_INFO_WITH_APPLICATION, userId.localId).then((result) => {
        data = result;
      })
    } catch (e) {
      logger.error(TAG, 'checkAppItem queryExtensionAbilityInfo try fail! ' + JSON.stringify(e));
    }
    if (!utils.isValid(data) || data.length <= 0) {
      logger.warn(TAG, 'checkAppItem queryExtensionAbilityInfo fail! data is null');
      this.ret = false;
      return this.ret;
    }
    logger.info(TAG, 'checkAppItem success');
    return this.ret;
  }

  async getBundleInfoItem(bundleName: string, appInfo: MyApplicationInfo) {
    logger.info(TAG, 'getBundleInfoItem in bundleName:' + bundleName);
    let userId: UserId = { localId: 0 };
    let retVal = await accountManager.getAccountUserId(userId);
    if (!retVal) {
      logger.warn(TAG, 'getBundleInfoItem getAccountUserId fail!');
      return;
    }
    let data = await bundle.getBundleInfo(bundleName, bundle.BundleFlag.GET_BUNDLE_INFO_WITH_REQUESTED_PERMISSION |
    bundle.BundleFlag.GET_BUNDLE_INFO_WITH_APPLICATION, userId.localId);
    await this.getResourceItem(bundleName, data, appInfo);
    await this.getPermissionList(data, appInfo);
    logger.info(TAG, 'getBundleInfoItem out');
  }

  async getAppInfoLabel(resMgr: resourceManager.ResourceManager, id: number) {
    let label = await resMgr.getString(id);
    logger.info(TAG, 'getAppInfoLabel start label:' + label);
    if (!utils.isValid(label) || label === baseData.EMPTY_STR) {
      return baseData.EMPTY_STR;
    }
    return label;
  }

  async getAppInfoIcon(resMgr: resourceManager.ResourceManager, id: number) {
    let iconVal = await resMgr.getMediaContentBase64Sync(id);
    logger.info(TAG, 'getAppInfoIcon start iconVal:' + iconVal);
    if (!utils.isValid(iconVal) || iconVal === baseData.EMPTY_STR) {
      return baseData.EMPTY_STR;
    }
    return iconVal;
  }

  async getResourceItem(bundleName: string, data: bundle.BundleInfo, appInfo: MyApplicationInfo) {
    logger.info(TAG, 'getResourceItem getResourceManager in');
    let bundleContext = getContext().createBundleContext(bundleName);
    let resMgr = bundleContext.resourceManager;
    let appInfoTemp = data.appInfo;
    let label = '';
    let iconVal = '';

    if (appInfoTemp.labelId > 0) {
      label = await this.getAppInfoLabel(resMgr, appInfoTemp.labelId);
      logger.info(TAG, 'getResourceItem success appInfo.label:' + label);
    } else {
      label = appInfoTemp.label;
      logger.info(TAG, 'getResourceItem defaults appInfo.label:' + label);
    }

    if (appInfoTemp.iconId > 0) {
      iconVal = await this.getAppInfoIcon(resMgr, appInfoTemp.iconId);
      if (iconVal === baseData.EMPTY_STR) {
        let icon: resourceManager.Resource = {
          bundleName: defaultIcon.bundleName,
          moduleName: defaultIcon.moduleName,
          id: defaultIcon.id
        }
        resMgr.getMediaContentBase64(icon).then((data) => {
          appInfo.appIcon = data;
        })
        appInfo.appTitle = label;
      }
      appInfo.appIcon = iconVal;
      appInfo.appTitle = label;
      AppStorage.SetOrCreate('applicationInfo', appInfo);
    }
    logger.info(TAG, 'getResourceItem getResourceManager out');
  }

  async terminateAbilityPage() {
    logger.info(TAG, 'terminateAbilityPage in:');
    await (getContext(this) as common.UIAbilityContext).terminateSelf();
  }

  getPermissionInfoVal(permissionName: string): number {
    for (let i = 0; i < permissionsDetailList.length; i++) {
      if (permissionsDetailList[i].permissionName === permissionName) {
        return i;
      }
    }
    return -1;
  }

  async getPermissionList(data: bundle.BundleInfo, appInfo: MyApplicationInfo) {
    let permissions = data.reqPermissionDetails;
    if (permissions !== null && permissions.length !== 0) {
      logger.info(TAG, 'getPermissionList permission length:' + permissions.length);
      for (let i = 0; i < permissions.length; i++) {
        logger.info(TAG, 'getPermissionList permission is in val:' + permissions[i].name);
        let j = this.getPermissionInfoVal(permissions[i].name);
        if (j >= 0) {
          appInfo.appPermissionList.push({
            permissionName: permissionsDetailList[j].permissionName,
            permissionLabel: permissionsDetailList[j].permissionLabel,
            permissionDescription: permissionsDetailList[j].permissionDescription,
          });
        }
      }
      logger.info(TAG, 'getPermissionList permission appInfo.appPermissionList:' +
      JSON.stringify(appInfo.appPermissionList));
    }
    logger.info(TAG, 'getPermissionList permission out');
  }
}

let appDetailData = new AppDetailData();

export default appDetailData as AppDetailData;