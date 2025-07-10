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

import enterpriseDeviceManager from '@ohos.enterprise.adminManager';
import router from '@ohos.router';
import accountManager, { UserId } from '../../common/accountManager'
import appDetailData from '../../common/appManagement/appDetailData'
import baseData from '../../common/baseData'
import doubleButtonComponent from '../component/autoManager/doubleButtonComponent'
import logger from '../../common/logger'
import utils from '../../common/utils'
import Want from '@ohos.app.ability.Want';
import adminManager from '@ohos.enterprise.adminManager';

const TAG = 'LoadingInfo';

@Entry
@Component
struct LoadingInfo {
  @StorageLink('manageAbilityName') manageAbilityName: string = '';
  @StorageLink('manageBundleName') manageBundleName: string = '';
  @StorageLink('manageEnterpriseName') manageEnterpriseName: string = '';
  @StorageLink('manageEnterpriseDescription') manageEnterpriseDescription: string = '';

  build() {
    Column() {
      Stack({ alignContent: Alignment.BottomEnd }) {
        GridContainer({
          columns: utils.isLargeDevice(),
          sizeType: SizeType.Auto,
          gutter: '12vp',
          margin: '12vp'
        }) {
          Column() {
            LoadingProgress()
              .width('88vp')
              .height('88vp')
              .color(0x000000)
              .opacity(0.6)
              .margin({ bottom: '16vp' })

            Text($r('app.string.loadingSearch'))
              .fontWeight(FontWeight.Regular)
              .fontSize('14vp')
              .opacity(0.9)
              .lineHeight('19.6vp')
              .fontFamily('HarmonyHeiTi')
              .height('19vp')
          }
          .useSizeType({
            xs: { span: 8, offset: 0 }, sm: { span: 8, offset: 0 },
            md: { span: 8, offset: 0 }, lg: { span: 8, offset: 2 }
          })
          .height('100%')
          .justifyContent(FlexAlign.Center)
        }

        GridContainer({
          columns: utils.isLargeDevice(),
          sizeType: SizeType.Auto,
          gutter: '12vp',
          margin: '12vp'
        }) {
          Column() {
            doubleButtonComponent({ nextFlag: true })
          }
          .justifyContent(FlexAlign.End)
          .width('100%')
          .margin({ bottom: '16vp' })
          .useSizeType({
            xs: { span: 8, offset: 0 }, sm: { span: 8, offset: 0 },
            md: { span: 8, offset: 0 }, lg: { span: 12, offset: 0 }
          })
        }
      }
      .width('100%')
      .height('100%')
    }
    .width('100%')
    .height('100%')
    .backgroundColor(0xF1F3F5)
  }

  aboutToAppear() {
    this.checkAppValidAndActivate();
  }

  async checkAppValidAndActivate() {
    let admin: Want = {
      bundleName: this.manageBundleName,
      abilityName: this.manageAbilityName
    }

    let enterpriseInfo: adminManager.EnterpriseInfo = {
      name: this.manageEnterpriseName,
      description: this.manageEnterpriseDescription
    }
    logger.info(TAG, 'enter loadingInfo admin=' + JSON.stringify(admin) + ' | enterpriseInfo=' +
    JSON.stringify(enterpriseInfo));

    let ret = await appDetailData.checkAppItem(admin);
    if (!(ret)) {
      logger.info(TAG, 'checkAppItem fail');
      router.pushUrl({ url: 'pages/autoManager/setFinishFail' });
      return;
    }

    let userId: UserId = { localId: 0 };
    let retVal = await accountManager.getAccountUserId(userId);
    logger.info(TAG, 'getAccountUserId retVal : ' + retVal);
    logger.info(TAG, 'getAccountUserId userId : ' + userId.localId);
    if (!retVal || userId.localId !== baseData.DEFAULT_USER_ID) {
      logger.info(TAG, 'getAccountUserId fail');
      router.pushUrl({ url: 'pages/autoManager/setFinishFail' });
      return;
    }

    logger.info(TAG, 'enter loadingInfo start enableAdmin!');
    enterpriseDeviceManager.enableAdmin(admin, enterpriseInfo,
    enterpriseDeviceManager.AdminType.ADMIN_TYPE_SUPER, baseData.DEFAULT_USER_ID, err => {
        if (err !== null) {
          logger.info(TAG, 'enter loadingInfo errCode : ' + err.code + ' errMessage : ' + err.message);
          router.pushUrl({ url: 'pages/autoManager/setFinishFail' });
        } else {
          router.pushUrl({ url: 'pages/autoManager/setFinishSuccess' });
        }
      });
  }
}