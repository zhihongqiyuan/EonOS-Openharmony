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

const local = new LocalStorage();
@Entry(local)
@Component
struct WidgetCard {
  @LocalStorageProp('newValue') newValue: string = 'Origin';
  @LocalStorageProp('image') image: string = 'app.media.startIcon';
  build(){
    Row() {
      Column() {
        Text(this.newValue)
        .fontSize($r('app.float.font_size'))
        .fontColor($r('app.color.item_title_font'))
        }
      }
    .size({width:'100%',height:'100%'})
    .backgroundImage($r(this.image))
    .backgroundImageSize({ width: '100%', height: '100%' })
  }
}