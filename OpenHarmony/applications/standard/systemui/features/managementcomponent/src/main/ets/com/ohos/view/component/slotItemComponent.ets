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

const TAG = 'NotificationManagement-SlotItemComponent';

@Component
export default struct SlotItemComponent {
  @State slotType: number = 0

  build() {
    Flex({ justifyContent: FlexAlign.SpaceBetween }) {
      Row() {
        if (this.slotType === 0) {
          Text($r('app.string.slot_unknown'))
            .fontColor($r('sys.color.ohos_id_color_text_primary'))
            .fontSize($r('sys.float.ohos_id_text_size_body1'))
            .textAlign(TextAlign.Start)
        }
        if (this.slotType === 1) {
          Text($r('app.string.slot_social_communication'))
            .fontColor($r('sys.color.ohos_id_color_text_primary'))
            .fontSize($r('sys.float.ohos_id_text_size_body1'))
            .textAlign(TextAlign.Start)
        }
        if (this.slotType === 2) {
          Text($r('app.string.slot_service_information'))
            .fontColor($r('sys.color.ohos_id_color_text_primary'))
            .fontSize($r('sys.float.ohos_id_text_size_body1'))
            .textAlign(TextAlign.Start)
        }
        if (this.slotType === 3) {
          Text($r('app.string.slot_content_information'))
            .fontColor($r('sys.color.ohos_id_color_text_primary'))
            .fontSize($r('sys.float.ohos_id_text_size_body1'))
            .textAlign(TextAlign.Start)
        }
        if (this.slotType === 0xFFFF) {
          Text($r('app.string.slot_other'))
            .fontColor($r('sys.color.ohos_id_color_text_primary'))
            .fontSize($r('sys.float.ohos_id_text_size_body1'))
            .textAlign(TextAlign.Start)
        }
      }
      .alignItems(VerticalAlign.Center)
      .align(Alignment.Start)
      .padding({ left: $r('app.float.divider_margin_l') })
      .height(ConfigData.WH_100_100)

      if (this.slotType !== 0) {
        Row() {
          Image($r('app.media.ic_settings_arrow'))
            .width($r('app.float.slotComp_image_width'))
            .height($r('app.float.slotComp_image_height'))
            .visibility(Visibility.Visible);
        }
        .alignItems(VerticalAlign.Center)
        .align(Alignment.End)
        .padding({ right: $r('app.float.divider_margin_r') })
        .height(ConfigData.WH_100_100)
      }
    }
    .width(ConfigData.WH_100_100)
    .height($r('app.float.switchComp_height'))
  }

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToDisAppear`);
  }
}