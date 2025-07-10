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

@Component
export default struct ItemComponent {
  @Link title: Resource;
  @Link value: string;
  @Link arrow: Resource;

  build() {
    Row() {
      Text(this.title)
        .fontSize($r('sys.float.ohos_id_text_size_body1'))
        .fontColor($r('app.color.itemComp_title_color'))
        .width(ConfigData.WH_45_100)
        .textAlign(TextAlign.Start)
        .fontWeight(FontWeight.Medium)
      Blank()
      Text(this.value)
        .fontSize($r('app.float.itemComp_value_font'))
        .fontColor($r('app.color.itemComp_font_color'))
      Image(this.arrow)
        .width($r('app.float.itemComp_image_width'))
        .height($r('app.float.itemComp_image_height'))
        .align(Alignment.End)
        .margin({left:$r('app.float.itemComp_image_margin_left')})
    }
    .width(ConfigData.WH_100_100)
    .height($r('app.float.itemComp_height'))
    .alignItems(VerticalAlign.Center)
    .padding({
      left: $r('sys.float.ohos_id_default_padding_start'),
      right: $r('sys.float.ohos_id_default_padding_end')
    })
    .border({ width: $r('app.float.border_width'), color: Color.White, radius: $r('app.float.border_radius') })
    .backgroundColor(Color.White)
  }
}
