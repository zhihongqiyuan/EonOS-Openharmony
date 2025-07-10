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
import doubleButtonComponent from '../component/autoManager/doubleButtonComponent'
import utils from '../../common/utils'

@Entry
@Component
struct SetFinishSuccess {
  @State textContext: Resource = $r('app.string.setupComplete');
  @State textManager: string = getContext().resourceManager.getStringSync($r('app.string.unitManage').id);
  private textContent: string = getContext()
    .resourceManager
    .getStringSync($r('app.string.textManageSuccessContent').id, this.textManager);
  private getTextList: (source: string, target: string) => Array<string> = (source: string, target: string) => {
    let result: string[] = [];
    if (source) {
      let position: number = source.indexOf(target);
      let before: string = source.substring(0, position);
      let after: string = source.substring(position + target.length);
      result.push(before);
      result.push(target);
      result.push(after);
    }
    return result;
  }

  build() {
    Column() {
      GridContainer({
        columns: utils.isLargeDevice(),
        sizeType: SizeType.Auto,
        gutter: '12vp',
        margin: '12vp'
      }) {
        Column() {
          Image($r('app.media.ic_public_todo'))
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
              ForEach(this.getTextList(this.textContent, this.textManager), (item: string) => {
                Span(item)
                  .fontColor(item === this.textManager ? 0x007DFF : 0x000000)
                  .onClick(() => {
                    if (item === this.textManager) {
                      router.pushUrl({ url: 'pages/autoManager/unitManagerShowPage' })
                    }
                  })
              })
            }
            .fontWeight(FontWeight.Regular)
            .fontSize($r('app.float.font_vp_16'))
            .fontFamily('HarmonyHeiTi')
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
          doubleButtonComponent()
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
}