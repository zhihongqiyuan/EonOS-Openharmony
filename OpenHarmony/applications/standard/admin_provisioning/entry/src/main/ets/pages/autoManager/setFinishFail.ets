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

import resetFactory from '../../common/resetFactory'
import utils from '../../common/utils'

@Entry
@Component
struct SetFinishFail {
  @State textContext: Resource = $r('app.string.settingFailed');
  @State textFail: Resource = $r('app.string.settingFailedInfo');

  build() {
    Column() {
      GridContainer({
        columns: utils.isLargeDevice(),
        sizeType: SizeType.Auto,
        gutter: '12vp',
        margin: '12vp'
      }) {
        Column() {
          Image($r('app.media.ic_public_detail'))
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
              Span(this.textFail)
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
            Text($r('app.string.resetFactorySet'))
              .fontSize($r('app.float.font_vp_16'))
              .fontFamily('HarmonyHeiTi')
              .fontWeight(FontWeight.Medium)
              .fontColor(0xFFFFFF)
              .lineHeight($r('app.float.lineHeight_vp_22'))
          }
          .backgroundColor(0x007DFF)
          .onClick(() => {
            resetFactory.rebootAndCleanUserData();
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
        .margin({ bottom: '164vp' })
      }
    }
    .backgroundColor(0xF1F3F5)
    .width('100%')
    .height('100%')
  }
}