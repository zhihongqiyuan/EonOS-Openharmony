/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import adminManager from '@ohos.enterprise.adminManager';
import Want from '@ohos.app.ability.Want';
import UIExtensionContentSession from '@ohos.app.ability.UIExtensionContentSession';
import { BusinessError } from '@ohos.base';
import deviceInfo from '@ohos.deviceInfo';
import logger from '../../common/logger';
import { PageComponent } from './PageComponent';
import baseData from '../../common/baseData'

const TAG: string = 'deployComponent';

@Component
export struct deployComponent {
  private title: Resource = $r('app.string.unitManageDevice');
  private autoDeployText: Resource = $r('app.string.AutoDeployText');
  private storage = LocalStorage.getShared();
  private abilityInfo: Want | undefined;
  private enterpriseInfo: adminManager.EnterpriseInfo | undefined;
  private adminType: adminManager.AdminType | undefined;
  private session: UIExtensionContentSession | undefined;

  build() {
    PageComponent({
      content: () => {
        this.buildPageContent();
      }
    })
  }

  @Builder
  buildPageContent() {
    if (deviceInfo.deviceType === 'phone') {
      this.phonePageContent();
    } else {
      this.pcPageContent();
    }
  }

  @Builder
  phonePageContent() {
    Column() {
      GridRow({ columns : 4, gutter: { x: 12 } }) {
        GridCol({ span : 4}) {
          Flex({
            direction: FlexDirection.Column,
            justifyContent: FlexAlign.Start,
            alignItems: ItemAlign.Center
          }) {
            Column() {
              Column() {
                Column() {
                  Image($r('app.media.ic_back'))
                    .width($r('app.float.wh_value_24'))
                    .height($r('app.float.wh_value_24'))
                    .onClick(() => {
                      this.terminateAbility(baseData.OOBE_PRE_STEP_CODE);
                    })
                }
                .width('100%')
                .margin({ top : $r('app.float.wh_value_36') })
                .height($r('app.float.wh_value_56'))
                .alignItems(HorizontalAlign.Start)
                .justifyContent(FlexAlign.Center)

                Image($r('app.media.mdm_start'))
                  .width($r('app.float.wh_value_48'))
                  .height($r('app.float.wh_value_48'))
                  .margin({ top : $r('app.float.wh_value_12'), bottom : $r('app.float.wh_value_12') })
                  .objectFit(ImageFit.Contain)
                  .align(Alignment.Center)

                Text(this.title)
                  .fontWeight(FontWeight.Medium)
                  .fontSize($r('sys.float.ohos_id_text_size_headline6'))
                  .fontColor($r('sys.color.ohos_id_color_text_primary'))
                  .maxLines(2)
                  .height($r('app.float.wh_value_40'))
              }
              .alignItems(HorizontalAlign.Center)
            }
            .margin({ bottom: $r('app.float.wh_value_48')})
            .flexShrink(0)
            .width('100%')

            Column() {
              Text() {
                Span(this.autoDeployText)
                  .fontWeight(FontWeight.Medium)
                  .lineHeight($r('app.float.lineHeight_vp_25'))
                  .fontSize($r('sys.float.ohos_id_text_size_sub_title2'))
                  .fontFamily('HarmonyHeiTi')
              }
              .width('100%')
              .textAlign(TextAlign.Start)
              .maxLines(2)
              .align(Alignment.Center)
            }
            .flexGrow(1)
            .width('100%')

            Column() {
              Button() {
                Text($r('app.string.continue'))
                  .fontSize($r('app.float.font_vp_16'))
                  .fontFamily('HarmonyHeiTi')
                  .fontWeight(FontWeight.Medium)
                  .fontColor(0xFFFFFF)
                  .lineHeight($r('app.float.wh_value_22'))
              }
              .backgroundColor(0x0A59F7)
              .onClick(() => {
                this.EnableAdmin();
              })
              .width($r('app.float.button_continue_width'))
              .height($r('app.float.wh_value_40'))
              .margin({ bottom: $r('app.float.value_100')})
            }
            .flexShrink(0)
            .width('100%')
            .padding({
              top: $r('app.float.wh_value_16'),
              bottom: $r('sys.float.ohos_id_default_padding_bottom_fixed')
            })
          }.size({ width: '100%', height: '100%' })
        }.size({ width: '100%', height: '100%' })
      }
      .height('100%')
      .width('100%')
      .backgroundColor($r('sys.color.ohos_id_color_sub_background'))
    }
    .padding({
      left: $r('sys.float.ohos_id_max_padding_start'),
      right: $r('sys.float.ohos_id_max_padding_end')
    })
    .size({ width: '100%', height: '100%' })
    .backgroundColor($r('sys.color.ohos_id_color_sub_background'))
  }

  @Builder
  pcPageContent() {
    Column() {
      GridRow({ columns : 12, gutter: { x: 12 } }) {
        GridCol({ span : 8, offset: 2 }) {
          Column() {
            Column() {
              Column() {
                Image($r('app.media.mdm_start'))
                  .width($r('app.float.wh_value_48'))
                  .height($r('app.float.wh_value_48'))
                  .margin({ top : $r('app.float.value_100'), bottom : $r('app.float.wh_value_12') })
                  .objectFit(ImageFit.Contain)
                  .align(Alignment.Center)

                Text(this.title)
                  .textAlign(TextAlign.Center)
                  .fontWeight(FontWeight.Medium)
                  .lineHeight($r('app.float.value_35'))
                  .fontSize($r('app.float.font_vp_30'))
                  .maxLines(2)
                  .fontFamily('HarmonyHeiTi')
                  .height($r('app.float.wh_value_40'))
              }
              .width('100%')
              .height('100%')
              .margin({ bottom: $r('app.float.wh_value_48')})
              .alignItems(HorizontalAlign.Center)
            }
            .width('100%')
            .height($r('app.float.value_248'))

            Column() {
              Text() {
                Span(this.autoDeployText)
                  .fontWeight(FontWeight.Medium)
                  .lineHeight($r('app.float.lineHeight_vp_25'))
                  .fontSize($r('app.float.font_vp_18'))
                  .fontFamily('HarmonyHeiTi')
              }
              .margin({top: $r('app.float.value_32')})
              .textAlign(TextAlign.Start)
              .maxLines(2)
            }
            .borderRadius($r('sys.float.ohos_id_corner_radius_default_l'))
            .backgroundColor($r('sys.color.ohos_id_color_background'))
            .width('100%')
            .layoutWeight(1);

            Column() {
              Row() {
                Button() {
                  Text($r('app.string.PreviousStep'))
                    .fontSize($r('app.float.font_vp_16'))
                    .fontFamily('HarmonyHeiTi')
                    .fontWeight(FontWeight.Medium)
                    .fontColor(0x0A59F7)
                    .lineHeight($r('app.float.lineHeight_vp_22'))
                }
                .backgroundColor(0xEEEEEE)
                .onClick(() => {
                  this.terminateAbility(baseData.OOBE_PRE_STEP_CODE);
                })
                .width($r('app.float.value_302'))
                .height($r('app.float.wh_value_40'))
                .margin({ right: $r('app.float.font_16'), bottom: $r('app.float.value_100') })

                Button() {
                  Text($r('app.string.continue'))
                    .fontSize($r('app.float.font_vp_16'))
                    .fontFamily('HarmonyHeiTi')
                    .fontWeight(FontWeight.Medium)
                    .fontColor(0xFFFFFF)
                    .lineHeight($r('app.float.wh_value_22'))
                }
                .backgroundColor(0x0A59F7)
                .onClick(() => {
                  this.EnableAdmin();
                })
                .width($r('app.float.value_302'))
                .height($r('app.float.wh_value_40'))
                .margin({ bottom: $r('app.float.value_100')})
              }
              .margin({top: $r('app.float.wh_value_16')})
              .alignItems(VerticalAlign.Bottom)
              .justifyContent(FlexAlign.Center)
              .width('100%')
            }
            .height($r('app.float.value_156'))
          }
        }
      }
    }
    .size({ width: '100%', height: '100%' })
    .backgroundColor($r('sys.color.ohos_id_color_sub_background'))
  }

  aboutToAppear() {
    logger.info(TAG, 'aboutToAppear');
    this.storage = LocalStorage.getShared();
    this.abilityInfo = this.storage.get<Want>('adminInfo');
    this.enterpriseInfo = this.storage.get<adminManager.EnterpriseInfo>('enterpriseInfo');
    this.adminType = this.storage.get<adminManager.AdminType>('adminType');
    this.session = this.storage.get<UIExtensionContentSession>('session');
  }

  private terminateAbility(flag: number) {
    this.session?.terminateSelfWithResult({
      resultCode: flag,
      want: this.abilityInfo
    });
    this.storage.clear();
  }

  private EnableAdmin() {
    let admin: Want = this.abilityInfo;
    let entInfo: adminManager.EnterpriseInfo = this.enterpriseInfo;
    try {
      adminManager.enableAdmin(admin, entInfo, this.adminType).then(() => {
        logger.info(TAG, 'enableAdmin success');
        this.terminateAbility(baseData.OOBE_NEXT_STEP_CODE);
      }).catch((err: BusinessError) => {
        logger.error(TAG, `enableAdmin failed, errCode: ${err.code}, errMessage: ${err.message}`);
        this.terminateAbility(baseData.ENABLE_ADMIN_FAILED);
      })
    } catch (err) {
      logger.error(TAG, `enableAdmin failed, errCode: ${err.code}, errMessage: ${err.message}`);
      this.terminateAbility(baseData.ENABLE_ADMIN_FAILED);
    }
  }
}



