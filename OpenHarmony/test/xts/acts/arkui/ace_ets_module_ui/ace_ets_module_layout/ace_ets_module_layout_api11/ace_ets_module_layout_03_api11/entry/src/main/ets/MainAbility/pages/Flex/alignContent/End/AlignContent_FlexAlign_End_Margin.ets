/*
* Copyright (c) 2023 iSoftStone Information Technology (Group) Co.,Ltd.
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
 
@Entry
@Component
struct AlignContent_FlexAlign_End_Margin {
  build() {
    Column() {
      Flex({
        direction: FlexDirection.Row,
        wrap: FlexWrap.Wrap,
        alignContent: FlexAlign.End,
        alignItems: ItemAlign.Start
      }) {
        Text('1').width('40%').height('20%').backgroundColor(0xF5DEB3).key('AlignContentFlexEndMargin_flex_1')
        Text('2').width('40%').height('30%').backgroundColor(0xD2B48C).key('AlignContentFlexEndMargin_flex_2')
        Text('3').width('40%').height('50%').backgroundColor(0xD2B48C).key('AlignContentFlexEndMargin_flex_3')
        Text('4').width('40%').height('60%').backgroundColor(0xF5DEB3).key('AlignContentFlexEndMargin_flex_4')

      }
      .key('AlignContentFlexEndMargin_flex')
      .size({ width: '90%', height: '90%' })
      .backgroundColor(0xAFEEEE)
      .margin(5)

    }
    .width('100%')
    .alignItems(HorizontalAlign.Start)
    .key('AlignContentFlexEndMargin_Column')
  }
}
