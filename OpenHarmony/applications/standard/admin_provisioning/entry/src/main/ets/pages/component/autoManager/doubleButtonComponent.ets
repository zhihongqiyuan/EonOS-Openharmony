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
import baseData from '../../../common/baseData'
import logger from '../../../common/logger'
import resetFactory from '../../../common/resetFactory'
import utils from '../../../common/utils'
import Want from '@ohos.app.ability.Want';
import common from '@ohos.app.ability.common';

const TAG = 'DoubleButtonComponent';

@Component
export default struct DoubleButtonComponent {
  private nextFlag: boolean = false;
  private returnDialogFlag: boolean = true;
  @StorageLink('manageBundleName') nextBundleName: string = '';
  buttonDialog: CustomDialogController = new CustomDialogController({
    builder: DialogButtonLayout(),
    autoCancel: false,
    alignment: DialogAlignment.Center,
    gridCount: 4,
  })

  build() {
    Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
      Row() {
        Image($r('app.media.ic_public_arrow_left'))
          .width($r('app.float.wh_value_24'))
          .height($r('app.float.wh_value_24'))
          .margin({
            right: $r('app.float.wh_value_8')
          })
          .objectFit(ImageFit.Contain)

        Text($r('app.string.returnVal'))
          .fontSize($r('app.float.font_vp_16'))
          .fontFamily('HarmonyHeiTi')
          .fontWeight(FontWeight.Medium)
          .lineHeight($r('app.float.lineHeight_vp_22'))
          .textAlign(TextAlign.Start)
          .onClick(() => {
            if (this.returnDialogFlag) {
              this.buttonDialog.open();
            } else {
              (getContext(this) as common.UIAbilityContext).terminateSelf();
            }
          })
      }
      .justifyContent(FlexAlign.Center)
      .align(Alignment.Start)

      Row() {
        Text($r('app.string.nextVal'))
          .fontSize($r('app.float.font_vp_16'))
          .fontFamily('HarmonyHeiTi')
          .fontWeight(FontWeight.Medium)
          .lineHeight($r('app.float.lineHeight_vp_22'))
          .textAlign(TextAlign.End)
          .visibility(this.nextFlag ? Visibility.None : Visibility.Visible)
          .onClick(() => {
            this.startSpecificActionAbility();
          })

        Image($r('app.media.ic_public_arrow_right'))
          .width($r('app.float.wh_value_24'))
          .height($r('app.float.wh_value_24'))
          .margin({
            left: $r('app.float.wh_value_8')
          })
          .visibility(this.nextFlag ? Visibility.None : Visibility.Visible)
          .objectFit(ImageFit.Contain)
      }
      .justifyContent(FlexAlign.Center)
      .align(Alignment.End)
    }
    .width('100%')
    .height('48vp')
  }

  async startSpecificActionAbility() {
    let actionVal = 'ohos.action.ENTERPRISE_DEVICE_ADMIN_POLICY_COMPLIANCE';
    let queryWant: Want = { action: actionVal, bundleName: this.nextBundleName };
    logger.info(TAG, 'startSpecificActionAbility startAbility in:');

    let data: bundle.AbilityInfo[] = [];
    try {
      data = await bundle.queryAbilityInfo(queryWant, bundle.AbilityFlag.GET_ABILITY_INFO_DEFAULT,
      baseData.DEFAULT_USER_ID);
    } catch (e) {
      logger.error(TAG, 'startSpecificActionAbility queryAbilityByWant try fail! ' + JSON.stringify(e));
    }

    if (utils.isValid(data) && data.length > 0) {
      logger.info(TAG, 'startSpecificActionAbility data len=' + data.length + ' | data content=' + data[0].name)
      try {
        await (getContext(this) as common.UIAbilityContext).startAbility({
          bundleName: this.nextBundleName,
          abilityName: data[0].name
        });
      } catch (e) {
        logger.error(TAG, 'startSpecificActionAbility startAbility fail! ' + e)
      }
    }
    await (getContext(this) as common.UIAbilityContext).terminateSelf();
  }
}

@CustomDialog
struct DialogButtonLayout {
  controller?: CustomDialogController

  build() {
    Column() {
      Column() {
        Text($r('app.string.tips'))
          .fontFamily('HarmonyHeiTi')
          .fontWeight(FontWeight.Medium)
          .lineHeight('28vp')
          .fontSize('20vp')
          .height('28vp')
          .width('100%')
          .margin({
            left: '23vp',
            right: '23vp',
            top: '37vp',
            bottom: '17.5vp'
          })
      }
      .width('100%')
      .padding({
        left: '23vp',
        right: '23vp',
      })

      Column() {
        Text($r('app.string.tipsInfo'))
          .fontFamily('PingFang SC')
          .fontWeight(FontWeight.Regular)
          .lineHeight('22.5vp')
          .fontSize('16vp')
          .width('100%')
          .margin({
            left: '23vp',
            right: '23vp',
            bottom: '9vp',
          })

        Flex({ justifyContent: FlexAlign.Center, alignItems: ItemAlign.Center }) {
          Button() {
            Text($r('app.string.returnVal'))
              .fontFamily('HarmonyHeiTi')
              .fontWeight(FontWeight.Medium)
              .lineHeight('22vp')
              .fontSize('16vp')
              .fontColor(0x007DFF)
          }
          .backgroundColor('#FFFFFF')
          .height('40vp')
          .flexGrow(1)
          .onClick(() => {
            this.controller?.close();
          })

          Divider()
            .height('24vp')
            .strokeWidth('1vp')
            .vertical(true)
            .color('#E3E3E3')

          Button() {
            Text($r('app.string.CancelSetting'))
              .fontFamily('HarmonyHeiTi')
              .fontWeight(FontWeight.Medium)
              .lineHeight('22vp')
              .fontSize('16vp')
              .fontColor(0x007DFF)
          }
          .backgroundColor('#FFFFFF')
          .height('40vp')
          .flexGrow(1)
          .onClick(() => {
            this.controller?.close();
            resetFactory.rebootAndCleanUserData();
          })
        }
        .width('100%')
        .height('56vp')
      }
      .padding({
        left: '23vp',
        right: '23vp'
      })
      .width('100%')
    }
    .width('100%')
  }
}