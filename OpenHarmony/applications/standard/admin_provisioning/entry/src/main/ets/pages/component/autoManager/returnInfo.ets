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

@Component
export default struct ReturnInfo {
  private headName: string | Resource = '';

  build() {
    Row() {
      Image($r('app.media.ic_back'))
        .width($r('app.float.wh_value_24'))
        .height($r('app.float.wh_value_24'))
        .borderRadius($r('app.float.wh_value_15'))
        .margin({
          right: $r('app.float.wh_value_16'),
        })
        .backgroundColor($r('app.color.color_00000000_transparent'))
        .visibility(Visibility.Visible)
        .onClick(() => {
            router.back();
        })
      Text(this.headName)
        .fontFamily('HarmonyHeiTi')
        .height($r('app.float.wh_value_56'))
        .fontColor(0x000000)
        .fontSize('20vp')
        .lineHeight('28vp')
        .fontWeight(FontWeight.Medium)
        .textAlign(TextAlign.Start)
    }
    .margin({ top: '28vp' })
    .width('100%')
    .height($r('app.float.wh_value_56'))
    .alignItems(VerticalAlign.Center)
    .align(Alignment.Start)
  }
}