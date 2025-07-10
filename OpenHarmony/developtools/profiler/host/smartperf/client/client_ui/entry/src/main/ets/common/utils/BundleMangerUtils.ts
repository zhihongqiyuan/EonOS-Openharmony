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

import bundle from '@ohos.bundle';
import ResMgr from '@ohos.resourceManager';
import { AppInfoItem } from '../entity/LocalConfigEntity';
import SPLogger from '../utils/SPLogger';
/**
 * 包管理 获取应用列表、icon、app名称
 */
const TAG = 'BundleManager';
export default class BundleManager {
  //根据包名获取base64
  static async getIconByBundleName(
    mBundleNameArr: Array<String>
  ): Promise<Map<string, string>> {
    let mBundleNames = Array.from(new Set(mBundleNameArr));
    let mMap = new Map();
    let want = {
      action: 'action.system.home',
      entities: ['entity.system.home'],
    };
    bundle
      .queryAbilityByWant(want, 1)
      .then(async (data) => {
        SPLogger.INFO(
          TAG,
          'getIconByBundleName data length [' + data.length + ']'
        );
        for (let j = 0; j < data.length; j++) {
          let bundleName = data[j].bundleName;
          for (let i = 0; i < mBundleNames.length; i++) {
            if (mBundleNames[i] === bundleName) {
              let bundleContext = globalThis.abilityContext.createBundleContext(
                mBundleNames[i]
              );
              await bundleContext.resourceManager
                .getMediaBase64(data[j].iconId)
                .then((value) => {
                  if (value != null) {
                    mMap.set(mBundleNames[i], value);
                  }
                });
            }
          }
        }
      })
      .catch((error) => {
        SPLogger.ERROR(
          TAG,
          'Operation failed. Cause: ' + JSON.stringify(error)
        );
        console.error(
          'BundleManager ... Operation failed. Cause: ' + JSON.stringify(error)
        );
      });
    return mMap;
  }

  //获取app列表
  static async getAppList(): Promise<Array<AppInfoItem>> {
    let appInfoList = new Array<AppInfoItem>();
    let want = {
      action: 'action.system.home',
      entities: ['entity.system.home'],
    };
    bundle
      .queryAbilityByWant(want, 1)
      .then(async (data) => {
        SPLogger.INFO(
          TAG,
          'xxx getAllApplicationInfo data length [' + data.length + ']'
        );
        for (let i = 0; i < data.length; i++) {
          let bundleName = data[i].bundleName;
          let bundleContext = globalThis.abilityContext.createBundleContext(
            data[i].bundleName
          );
          try {
            let appName = await bundleContext.resourceManager.getString(
              data[i].labelId
            );
            let icon = await bundleContext.resourceManager.getMediaBase64(
              data[i].iconId
            );
            bundle.getBundleInfo(bundleName, 1).then((bundleData) => {
              BundleManager.getAbility(bundleName).then((abilityName) => {
                console.info(
                  'index[' + i + '].getAbility for begin data: ',
                  abilityName
                );
                appInfoList.push(
                  new AppInfoItem(
                    bundleName,
                    appName,
                    bundleData.versionName,
                    icon,
                    abilityName
                  )
                );
              });
            });
          } catch (err) {
            SPLogger.ERROR(
              TAG,
              'index[' + i + ']  getAllApplicationInfo err' + err
            );
          }
        }
      })
      .catch((error) => {
        SPLogger.ERROR(
          TAG,
          'Operation failed. Cause: ' + JSON.stringify(error)
        );
        console.error(
          'BundleManager ... Operation failed. Cause: ' + JSON.stringify(error)
        );
      });
    return appInfoList;
  }
  //获取启动ability
  static async getAbility(bundleName: string): Promise<string> {
    let abilityName = '';
    try {
      await bundle
        .queryAbilityByWant(
          {
            bundleName: bundleName,
            entities: ['entity.system.home'],
            action: 'action.system.home',
          },
          1,
          100
        )
        .then((abilityInfo) => {
          if (abilityInfo != null) {
            abilityName = abilityInfo[0].name;
          }
        });
    } catch (err) {
      SPLogger.ERROR(TAG, 'index[' + bundleName + '] getAbility err' + err);
    }
    SPLogger.INFO(
      TAG,
      'index[' + bundleName + '] getAbility abilityName: ' + abilityName
    );
    return abilityName;
  }
}
