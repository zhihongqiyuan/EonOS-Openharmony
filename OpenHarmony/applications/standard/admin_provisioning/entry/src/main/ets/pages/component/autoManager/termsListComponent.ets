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

@Component
export default
struct TermsListComponent {
  @Link enterpriseCustomTerms: string;
  @Link contentOfEnterpriseTerms: string[];

  build() {
    Column() {
      List() {
        ListItem() {
          Column() {
            Row() {
              Text($r('app.string.deviceManageInfo'))
                .height('41vp')
                .textAlign(TextAlign.Center)
                .fontWeight(FontWeight.Medium)
                .lineHeight($r('app.float.lineHeight_vp_41'))
                .fontSize($r('app.float.font_vp_30'))
                .fontFamily('HarmonyHeiTi')
                .margin({ top: '43.5vp', bottom: '48vp' })
            }
            .justifyContent(FlexAlign.Center)
            .width('100%')

            Row() {
              Text($r('app.string.device_mgr_info_content'))
                .fontWeight(FontWeight.Regular)
                .lineHeight('21.5vp')
                .opacity(0.9)
                .fontSize('16vp')
                .fontFamily('HarmonyHeiTi')
                .margin({ bottom: '59vp' })
            }
            .width('100%')

            Row() {
              Text(this.enterpriseCustomTerms)
                .textAlign(TextAlign.Center)
                .fontWeight(FontWeight.Medium)
                .lineHeight($r('app.float.lineHeight_vp_41'))
                .fontSize($r('app.float.font_vp_24'))
                .fontFamily('HarmonyHeiTi')
                .margin({ bottom: '48vp' })
            }
            .justifyContent(FlexAlign.Center)
            .width('100%')
          }
        }

        ListItem() {
          Column() {
            ForEach(this.contentOfEnterpriseTerms, (item: string, index) => {
              Row() {
                Text(item)
                  .fontWeight(FontWeight.Regular)
                  .lineHeight('21.5vp')
                  .opacity(0.9)
                  .fontSize($r('app.float.font_vp_16'))
                  .fontFamily('HarmonyHeiTi')
                  .margin({
                    bottom: (index === this.contentOfEnterpriseTerms.length - 1) ? '0vp' : '12vp'
                  })
              }
              .width('100%')
            }, (item: string) => JSON.stringify(item));
          }
          .flexShrink(1)
        }
      }
    }
    .width('100%')
    .height('100%')
  }
}