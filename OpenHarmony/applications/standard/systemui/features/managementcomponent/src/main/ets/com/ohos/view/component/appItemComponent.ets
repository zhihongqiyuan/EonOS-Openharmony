/**
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
import ConfigData from '../../common/constants';
import Log from '../../../../../../../../../common/src/main/ets/default/Log';
import NotificationListener from '../../model/notificationListener';
import Router from '@system.router'

const TAG = 'ManagementComponent-AppItemComponent';

@Component
export default struct AppItemComponent {
  @State canNotice: boolean= false;
  @State appIcon: string ='';
  @State appTitle: string ='';
  @State appSummary: string ='';
  @State appValue: string ='';
  @State appArrow: string ='';
  @State appArrowStyle: string ='';
  @State appUri: string ='';
  @State appSwitch: number = 0;
  @State appBundleName: string ='';
  @State appUid: number = 0;

  build() {
    Flex({ justifyContent: FlexAlign.SpaceBetween }) {
      Row() {
        if ('' !== this.appIcon) {
          if (this.appSwitch === 2 || this.appSwitch === 3) {
            Image(this.appIcon)
              .width($r('app.float.appitem_notice_info_icon_width_l'))
              .height($r('app.float.appitem_notice_info_icon_height_l'))
              .margin({ right: $r('app.float.appitem_icon_info_margin_r') })
              .objectFit(ImageFit.Contain);
          } else {
            Image(this.appIcon)
              .width($r('app.float.appitem_notice_info_icon_width'))
              .height($r('app.float.appitem_notice_info_icon_height'))
              .margin({ right: $r('app.float.appitem_icon_info_list_margin_r') })
              .objectFit(ImageFit.Contain);
          }
        }
        Column() {
          Text(this.appTitle)
            .fontColor($r('sys.color.ohos_id_color_text_primary'))
            .fontSize($r('sys.float.ohos_id_text_size_body1'))
            .fontWeight(FontWeight.Medium)
            .textAlign(TextAlign.Start)
            .maxLines(ConfigData.MAX_LINES_3)
            .textOverflow({ overflow: TextOverflow.Ellipsis })
            .margin({
              top: $r('app.float.appitem_label_margin_t'),
              bottom: $r('app.float.appitem_label_margin_b'),
              left: $r('app.float.page_notice_info_label_margin')
            });
          Row() {
            if (this.appSwitch === 3) {
              if (parseInt(this.appValue) == 0) {
                Text($r('app.string.slot_unknown'))
                  .fontColor($r('sys.color.ohos_id_color_text_primary'))
                  .fontSize($r('sys.float.ohos_id_text_size_body2'))
                  .textAlign(TextAlign.Start)
                  .maxLines(ConfigData.MAX_LINES_1)
                  .textOverflow({ overflow: TextOverflow.Ellipsis })
                  .visibility(Visibility.Visible)
                  .margin({
                    bottom: $r('app.float.appitem_version_margin_b'),
                    left: $r('app.float.page_notice_info_label_margin')
                  });
              } else if (parseInt(this.appValue) == 1) {
                Text($r('app.string.slot_social_communication'))
                  .fontColor($r('sys.color.ohos_id_color_text_primary'))
                  .fontSize($r('sys.float.ohos_id_text_size_body2'))
                  .textAlign(TextAlign.Start)
                  .maxLines(ConfigData.MAX_LINES_1)
                  .textOverflow({ overflow: TextOverflow.Ellipsis })
                  .visibility(Visibility.Visible)
                  .margin({
                    bottom: $r('app.float.appitem_version_margin_b'),
                    left: $r('app.float.page_notice_info_label_margin')
                  });
              } else if (parseInt(this.appValue) == 2) {
                Text($r('app.string.slot_service_information'))
                  .fontColor($r('sys.color.ohos_id_color_text_primary'))
                  .fontSize($r('sys.float.ohos_id_text_size_body2'))
                  .textAlign(TextAlign.Start)
                  .maxLines(ConfigData.MAX_LINES_1)
                  .textOverflow({ overflow: TextOverflow.Ellipsis })
                  .visibility(Visibility.Visible)
                  .margin({
                    bottom: $r('app.float.appitem_version_margin_b'),
                    left: $r('app.float.page_notice_info_label_margin')
                  });
              } else if (parseInt(this.appValue) == 3) {
                Text($r('app.string.slot_content_information'))
                  .fontColor($r('sys.color.ohos_id_color_text_primary'))
                  .fontSize($r('sys.float.ohos_id_text_size_body2'))
                  .textAlign(TextAlign.Start)
                  .maxLines(ConfigData.MAX_LINES_1)
                  .textOverflow({ overflow: TextOverflow.Ellipsis })
                  .visibility(Visibility.Visible)
                  .margin({
                    bottom: $r('app.float.appitem_version_margin_b'),
                    left: $r('app.float.page_notice_info_label_margin')
                  });
              } else if (parseInt(this.appValue) == 0xFFFF) {
                Text($r('app.string.slot_other'))
                  .fontColor($r('sys.color.ohos_id_color_text_primary'))
                  .fontSize($r('sys.float.ohos_id_text_size_body2'))
                  .textAlign(TextAlign.Start)
                  .maxLines(ConfigData.MAX_LINES_1)
                  .textOverflow({ overflow: TextOverflow.Ellipsis })
                  .visibility(Visibility.Visible)
                  .margin({
                    bottom: $r('app.float.appitem_version_margin_b'),
                    left: $r('app.float.page_notice_info_label_margin')
                  });
              } else {
                Text('')
                  .fontColor($r('sys.color.ohos_id_color_text_primary'))
                  .fontSize($r('sys.float.ohos_id_text_size_body2'))
                  .textAlign(TextAlign.Start)
                  .maxLines(ConfigData.MAX_LINES_1)
                  .textOverflow({ overflow: TextOverflow.Ellipsis })
                  .visibility(Visibility.Visible)
                  .margin({
                    bottom: $r('app.float.appitem_version_margin_b'),
                    left: $r('app.float.page_notice_info_label_margin')
                  });
              }
            } else {
              Text($r('app.string.version'))
                .fontColor($r('sys.color.ohos_id_color_text_primary'))
                .fontSize($r('sys.float.ohos_id_text_size_body2'))
                .textAlign(TextAlign.Start)
                .maxLines(ConfigData.MAX_LINES_1)
                .textOverflow({ overflow: TextOverflow.Ellipsis })
                .visibility(Visibility.Visible)
                .margin({
                  bottom: $r('app.float.appitem_version_margin_b'),
                  left: $r('app.float.page_notice_info_label_margin')
                });
              Text(this.appSummary)
                .fontColor($r('sys.color.ohos_id_color_text_primary'))
                .fontSize($r('sys.float.ohos_id_text_size_body2'))
                .textAlign(TextAlign.Start)
                .maxLines(ConfigData.MAX_LINES_1)
                .textOverflow({ overflow: TextOverflow.Ellipsis })
                .visibility(Visibility.Visible)
                .margin({ bottom: $r('app.float.appitem_version_margin_b') });
            }
          }
        }
        .alignItems(HorizontalAlign.Start);
      }
      .flexShrink(0)
      .height(ConfigData.WH_100_100)
      .alignItems(VerticalAlign.Center)
      .align(Alignment.Start)

      Row() {
        if (this.appSwitch === 0) {
          Text(this.appValue)
            .fontColor($r('sys.color.ohos_id_color_text_secondary'))
            .fontSize($r('sys.float.ohos_id_text_size_body2'))
            .margin({ right: $r('app.float.appitem_version_margin_r') })
            .align(Alignment.End);
          Image(this.appArrow)
            .width($r('app.float.itemComp_image_width'))
            .height($r('app.float.itemComp_image_height'))
            .visibility(Visibility.Visible);
        } else if (this.appSwitch === 2) {
          Text(this.appValue)
            .width($r('app.float.itemComp_image_width'))
            .height($r('app.float.itemComp_image_height'))
            .margin({ right: $r('app.float.appitem_version_margin_r') })
            .align(Alignment.End);
          Image($r('app.media.ic_settings_arrow'))
            .width($r('app.float.appitem_notice_infopop_icon_width'))
            .height($r('app.float.appitem_notice_infopop_icon_height'))
            .margin({ right: $r('app.float.appitem_notice_info_icon_margin_r') })
            .visibility(Visibility.None);
        } else if (this.appSwitch === 3) {
          Text('')
            .fontSize($r('app.float.appitem_version_font'))
            .fontColor($r('app.color.appitem_version_fontcolor'))
            .margin({ right: $r('app.float.appitem_version_margin_r') })
            .align(Alignment.End);
          Image($r('app.media.ic_settings_arrow'))
            .width($r('app.float.appitem_notice_infopop_icon_width'))
            .height($r('app.float.appitem_notice_infopop_icon_height'))
            .margin({ right: $r('app.float.appitem_notice_infopop_icon_margin_r') })
            .visibility(Visibility.None);
        } else {
          Toggle({ type: ToggleType.Switch, isOn: this.canNotice })
            .align(Alignment.End)
            .selectedColor($r("app.color.font_color_007DFF"))
            .width($r('app.float.toggle_comp_width'))
            .height($r('app.float.toggle_comp_height'))
            .onChange((flag) => {
              Log.showInfo(TAG, `Toggle onChange param: data:${flag} `);
              NotificationListener.enableNotification({ bundle: this.appBundleName, uid: this.appUid }, flag);
            })
        }
      }
      .height(ConfigData.WH_100_100)
      .alignItems(VerticalAlign.Center)
      .align(Alignment.End);
    }
    .height($r('app.float.page_notice_app_item_row_height'))
    .width(ConfigData.WH_100_100)
  }

  aboutToAppear(): void{
    Log.showInfo(TAG, `aboutToAppear`);
    if (this.appSwitch === 1) {
      NotificationListener.isNotificationEnabled({ bundle: this.appBundleName, uid: this.appUid })
        .then((stateValue) => {
          Log.showInfo(TAG, `NotificationListener.isNotificationEnabled data:${JSON.stringify(stateValue)}`);
          this.canNotice = stateValue;
        })
        .catch((error: Error) => {
          Log.showError(TAG, `NotificationListener.isNotificationEnabled error:${JSON.stringify(error)}`);
        });

      NotificationListener.register({ bundle: this.appBundleName,
        onEnableChanged: (stateValue: boolean) => {
          Log.showDebug(TAG, `aboutToAppear listener call`);
          this.canNotice = stateValue;
        } });
    }
  }

  aboutToDisappear(): void{
    Log.showInfo(TAG, `aboutToDisappear`);
  }

  onBackPress() {
    Log.showDebug(TAG, `onBackPress`);
    Router.back();
  }
}