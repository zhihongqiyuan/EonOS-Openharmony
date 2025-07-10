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
export default struct EntryComponent {
  @Prop appIcon: string = '';
  @Prop appTitle: string = '';

  build() {
    Flex({ justifyContent: FlexAlign.SpaceBetween }) {
      Row() {
        Image(this.appIcon)
          .width($r('app.float.wh_value_40'))
          .height($r('app.float.wh_value_40'))
          .margin({
            left: $r('app.float.wh_value_36'),
            right: $r('app.float.wh_value_16')
          })
          .visibility('' === this.appIcon ? Visibility.None : Visibility.Visible)
          .objectFit(ImageFit.Contain);

        Text(this.appTitle)
          .fontColor($r('sys.color.ohos_id_color_text_primary'))
          .fontSize($r('app.float.font_16'))
          .fontWeight(FontWeight.Medium)
          .textAlign(TextAlign.Start)
          .maxLines(3)
          .textOverflow({ overflow: TextOverflow.Ellipsis })
      }
      .alignItems(VerticalAlign.Center)
      .align(Alignment.Start)
      .height($r('app.float.wh_value_64'))
      .width('100%')
    }
  }
}