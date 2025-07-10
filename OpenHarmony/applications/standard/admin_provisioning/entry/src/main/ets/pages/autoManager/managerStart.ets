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

import router from '@ohos.router';
import utils from '../../common/utils'
import doubleButtonComponent from '../component/autoManager/doubleButtonComponent'
import logger from '../../common/logger'
import Want from '@ohos.app.ability.Want';
import common from '@ohos.app.ability.common';

const TAG = 'ManagerStart';

@Entry
@Component
struct ManagerStart {
  @State textContext: Resource = $r('app.string.unitManageDevice');
  @State textManager: Resource = $r('app.string.unitManage');
  @State textManagerBefore: Resource = $r('app.string.textManagerBefore');
  @State textManagerAfter: Resource = $r('app.string.textManagerAfter');
  @State textTerms: Resource = $r('app.string.textTerms');
  @State textTermsBefore: Resource = $r('app.string.textTermsBefore');
  private storage = LocalStorage.getShared();

  build() {
    Column() {
      GridContainer({
        columns: utils.isLargeDevice(),
        sizeType: SizeType.Auto,
        gutter: '12vp',
        margin: '12vp'
      }) {
        Column() {
          Image($r('app.media.ic_start'))
            .width($r('app.float.wh_value_40'))
            .height($r('app.float.wh_value_40'))
            .margin({ top: '72vp', bottom: '16vp' })
            .objectFit(ImageFit.Contain)

          Text(this.textContext)
            .height('40vp')
            .textAlign(TextAlign.Center)
            .fontWeight(FontWeight.Medium)
            .lineHeight($r('app.float.lineHeight_vp_41'))
            .fontSize($r('app.float.font_vp_30'))
            .fontFamily('HarmonyHeiTi')
        }
        .useSizeType({
          xs: { span: 8, offset: 0 }, sm: { span: 8, offset: 0 },
          md: { span: 8, offset: 0 }, lg: { span: 8, offset: 2 }
        })
        .width('100%')
        .align(Alignment.Center)

        Column() {
          Row() {
            Text() {
              Span(this.textTermsBefore)
                .fontWeight(FontWeight.Regular)
                .fontSize($r('app.float.font_vp_16'))
                .fontFamily('HarmonyHeiTi')

              Span(this.textTerms)
                .decoration({ type: TextDecorationType.None })
                .fontColor(0x007DFF)
                .fontSize($r('app.float.font_vp_16'))
                .fontFamily('HarmonyHeiTi')
                .fontWeight(FontWeight.Regular)
                .onClick(() => {
                  router.pushUrl({ url: 'pages/autoManager/termsShowPage' })
                })

              Span(this.textManagerBefore)
                .fontWeight(FontWeight.Regular)
                .fontSize($r('app.float.font_vp_16'))
                .fontFamily('HarmonyHeiTi')

              Span(this.textManager)
                .decoration({ type: TextDecorationType.None })
                .fontColor(0x007DFF)
                .fontSize($r('app.float.font_vp_16'))
                .fontFamily('HarmonyHeiTi')
                .fontWeight(FontWeight.Regular)
                .onClick(() => {
                  router.pushUrl({ url: 'pages/autoManager/unitManagerShowPage' })
                })

              Span(this.textManagerAfter)
                .fontWeight(FontWeight.Regular)
                .fontSize($r('app.float.font_vp_16'))
                .fontFamily('HarmonyHeiTi')
            }
            .lineHeight($r('app.float.lineHeight_vp_21_5'))
          }
          .useSizeType({
            xs: { span: 8, offset: 0 }, sm: { span: 8, offset: 0 },
            md: { span: 8, offset: 0 }, lg: { span: 8, offset: 2 }
          })
          .width('100%')
          .margin({ top: '48vp' })
          .justifyContent(FlexAlign.Center)
        }
        .width('100%')
      }

      Blank()

      GridContainer({
        columns: utils.isLargeDevice(),
        sizeType: SizeType.Auto,
        gutter: '12vp',
        margin: '12vp'
      }) {
        Column() {
          Button() {
            Text($r('app.string.buttonAcceptAndContinue'))
              .fontSize($r('app.float.font_vp_16'))
              .fontFamily('HarmonyHeiTi')
              .fontWeight(FontWeight.Medium)
              .fontColor(0xFFFFFF)
              .lineHeight($r('app.float.lineHeight_vp_22'))
          }
          .backgroundColor(0x007DFF)
          .onClick(() => {
            router.pushUrl({ url: 'pages/autoManager/loadingInfo' })
          })
          .width('100%')
          .height('40vp')
          .useSizeType({
            xs: { span: 2, offset: 3 }, sm: { span: 2, offset: 3 },
            md: { span: 2, offset: 3 }, lg: { span: 2, offset: 3 }
          })
        }
        .width('100%')
        .useSizeType({
          xs: { span: 8, offset: 0 }, sm: { span: 8, offset: 0 },
          md: { span: 8, offset: 0 }, lg: { span: 8, offset: 2 }
        })
        .margin({ bottom: '100vp' })
      }

      GridContainer({
        columns: utils.isLargeDevice(),
        sizeType: SizeType.Auto,
        gutter: '12vp',
        margin: '12vp'
      }) {
        Column() {
          doubleButtonComponent({ nextFlag: true, returnDialogFlag: false })
        }
        .useSizeType({
          xs: { span: 8, offset: 0 }, sm: { span: 8, offset: 0 },
          md: { span: 8, offset: 0 }, lg: { span: 12, offset: 0 }
        })
        .width('100%')
        .margin({ bottom: '16vp' })
      }
    }
    .backgroundColor(0xF1F3F5)
    .width('100%')
    .height('100%')
  }

  aboutToAppear() {
    this.parseManageParameter();
  }

  parseManageParameter() {
    this.storage = LocalStorage.getShared();
    let data = this.storage.get<Want>('autoManagerAbilityWant');

    if (!utils.checkObjPropertyValid(data, 'parameters.elementName.abilityName') ||
    !utils.checkObjPropertyValid(data, 'parameters.enterprise.name') ||
    !utils.isValid((data.parameters?.elementName as Record<string, string>).bundleName) ||
    !utils.isValid(data.parameters?.url) ||
    !utils.isValid(data.parameters?.termsName) ||
    (!utils.isValid(data.parameters?.termsContent) || !(data.parameters?.termsContent instanceof Array)) ||
    !utils.isValid((data.parameters?.enterprise as Record<string, string>).description)) {
      logger.error(TAG, 'parseManageParameter fail! parameters is not valid');
      (getContext(this) as common.UIAbilityContext).terminateSelf();
      return;
    }

    let mapVal = new Map([
      ['manageAbilityName', (data.parameters?.elementName as Record<string, string>).abilityName],
      ['manageBundleName', (data.parameters?.elementName as Record<string, string>).bundleName],
      ['manageUrl', data.parameters?.url],
      ['manageTermsName', data.parameters?.termsName],
      ['manageTermsContent', data.parameters?.termsContent],
      ['manageEnterpriseName', (data.parameters?.enterprise as Record<string, string>).name],
      ['manageEnterpriseDescription', (data.parameters?.enterprise as Record<string, string>).description]
    ]);

    for (let item of Array.from(mapVal.keys())) {
      AppStorage.setOrCreate(item, mapVal.get(item));
    }
  }
}