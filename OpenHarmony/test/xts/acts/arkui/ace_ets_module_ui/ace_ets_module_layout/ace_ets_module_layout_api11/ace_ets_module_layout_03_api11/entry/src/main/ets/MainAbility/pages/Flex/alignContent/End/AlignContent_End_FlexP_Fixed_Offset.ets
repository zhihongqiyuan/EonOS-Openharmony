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
struct AlignContent_End_FlexP_Fixed_Offset {
  alignContent: number = 0

  build() {
    Column() {
      Text('AlignContent_End_FlexP_Fixed_Offset').fontSize(9).fontColor(0xCCCCCC).width('90%')
      Flex({
        direction: FlexDirection.Row,
        wrap: FlexWrap.Wrap,
        alignContent: FlexAlign.End,
        alignItems: ItemAlign.Start
      }) {
        Text('1')
          .width(150)
          .height(50)
          .backgroundColor(0xF5DEB3)
          .key('AlignContent_End_FlexP_Fixed_Offset_1')
          .offset({ x: 15, y: 30 })
        Text('2').width(150).height(100).backgroundColor(0xD2B48C).key('AlignContent_End_FlexP_Fixed_Offset_2')
        Text('3').width(150).height(150).backgroundColor(0xD2B48C).key('AlignContent_End_FlexP_Fixed_Offset_3')
        Text('4').width(150).height(200).backgroundColor(0xF5DEB3).key('AlignContent_End_FlexP_Fixed_Offset_4')
      }
      .key('AlignContent_End_FlexP_Fixed_Offset_flex001')
      .width(500)
      .height(400)
      .backgroundColor(0xAFEEEE)

    }.width('100%')
  }
}
