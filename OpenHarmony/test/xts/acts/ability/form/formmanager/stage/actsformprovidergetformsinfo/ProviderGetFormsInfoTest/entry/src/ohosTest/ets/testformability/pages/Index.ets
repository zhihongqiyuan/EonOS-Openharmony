/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
 
import formBindingData from '@ohos.app.form.formBindingData';
import { AddFormMenuItem } from '@ohos.arkui.advanced.FormMenu';
import hilog from '@ohos.hilog';

@Entry
@Component
struct Index {
  @State message: string = 'DEMO';
  @State message2: string = '应用组件加桌';
  @State compId: string = 'add_form';

  @Builder
  MenuBuilder() {
    Menu() {
       AddFormMenuItem(
         {
           bundleName: 'com.example.menuprovider',
           abilityName: 'EntryFormAbility',
           parameters: {
             'ohos.extra.param.key.form_dimension': 2,
             'ohos.extra.param.key.form_name': 'widget',
             'ohos.extra.param.key.module_name': 'entry'
           },
         },
         this.compId,
         {
           formBindingData: formBindingData.createFormBindingData({
           }),
           callback:
             (code, formId) => {
               hilog.info(0xF811, "JSAPP", "AddFormMenuItemSuccessAndFormId " +formId +"FormID  " +code +"ReturnCode");
             },
           style: {
             options: {
               startIcon: $r("app.media.icon"), 
               content: "添加到桌面",
               endIcon: $r("app.media.icon")
             }
           }
         }
       )
    }
  }

  build() {
    Row() {
      Column() {
        Text(this.message)
          .align(Alignment.Center)
          .fontSize(20)
          .margin({ top: 50, left: 10, right: 10, bottom: 50 })
        Text(this.message2)
          .align(Alignment.Center)
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
          .margin({ top: 10, left: 10, right: 10, bottom: 50 })
        Button('组件A')
          .align(Alignment.Center)
          .fontSize(20)
          .fontColor('red')
          .id(this.compId)
          .bindMenu(this.MenuBuilder())
          .margin({ top: 10, left: 10, right: 10, bottom: 50 })
      }
      .width('40%')
      .height('50%')
    }
  }
}