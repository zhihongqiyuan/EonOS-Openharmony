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
export default struct PermissionListComponent {
  private permissionName: string | Resource = '';
  private permissionDescription: string | Resource = '';

  build() {
    Row() {
      Image($r('app.media.rect1'))
        .width($r('app.float.wh_value_7'))
        .height($r('app.float.wh_value_7'))
        .margin({
          left: $r('app.float.wh_value_36'),
          right: $r('app.float.wh_value_8') })
        .objectFit(ImageFit.Contain);

      Column() {
        Text(this.permissionName)
          .height($r('app.float.wh_value_22'))
          .fontColor($r('sys.color.ohos_id_color_text_primary'))
          .fontSize($r('app.float.font_16'))
          .fontWeight(FontWeight.Medium)
          .textAlign(TextAlign.Start)
          .maxLines(3)
          .textOverflow({ overflow: TextOverflow.Ellipsis })
          .visibility(('' === this.permissionName || 'undefined' === typeof (this.permissionName) ||
          null === this.permissionName) ? Visibility.None : Visibility.Visible)
          .margin({
            bottom: $r('app.float.wh_value_2'),
          });

        Text(this.permissionDescription)
          .height($r('app.float.wh_value_19'))
          .fontColor($r('sys.color.ohos_id_color_text_secondary'))
          .fontSize($r('app.float.font_14'))
          .fontWeight(FontWeight.Regular)
          .textAlign(TextAlign.Start)
          .maxLines(3)
          .textOverflow({ overflow: TextOverflow.Ellipsis })
          .visibility(('' === this.permissionDescription || 'undefined' === typeof (this.permissionDescription) ||
          null === this.permissionDescription) ? Visibility.None : Visibility.Visible)
      }
      .alignItems(HorizontalAlign.Start)
    }
    .height($r('app.float.wh_value_64'))
    .width('100%')
    .margin({
      top: $r('app.float.wh_value_12')
    })
    .align(Alignment.Start)
    .alignItems(VerticalAlign.Center)
  }
}