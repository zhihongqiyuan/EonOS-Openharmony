/**
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
struct List_lanes_1 {
  @State addLanes: number = 1;
  private content: string = 'List_lanes Page';

  onBuildDone() {
    console.info('List_lanes page build done called');
  }

  build() {
    Column() {
      List({ space: 20, initialIndex: 0 }) {
        ListItem() {
          Text('1')
            .width('100%')
            .height('100%')
            .fontSize(16)
        }
        .width('40%')
        .height('15%')
        .key('List_lanes_001')
        .editable(true)
        .backgroundColor(0xF5DEB3)

        ListItem() {
          Text('2')
            .width('100%')
            .height('100%')
            .fontSize(16)
        }
        .width('40%')
        .height('15%')
        .key('List_lanes_002')
        .editable(true)
        .backgroundColor(0xD2B48C)

        ListItem() {
          Text('3')
            .width('100%')
            .height('100%')
            .fontSize(16)
        }
        .width('40%')
        .height('15%')
        .key('List_lanes_003')
        .editable(true)
        .backgroundColor(0xffa500)

        ListItem() {
          Text('4')
            .width('100%')
            .height('100%')
            .fontSize(16)
        }
        .width('40%')
        .height('15%')
        .key('List_lanes_004')
        .editable(true)
        .backgroundColor(0xFFFFFF)
      }
      .height('80%')
      .width('80%')
      .key('List_lanes_1_01')
      .backgroundColor(0xAFEEEE)
      .lanes(this.addLanes)
      .scrollBar(BarState.On)
    }.width('100%').height('100%').backgroundColor(0xDCDCDC)
  }
}