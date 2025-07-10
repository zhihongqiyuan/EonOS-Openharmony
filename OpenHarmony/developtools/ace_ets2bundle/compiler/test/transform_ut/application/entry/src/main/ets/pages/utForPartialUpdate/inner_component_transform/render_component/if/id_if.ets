/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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
struct MyComponent {
  private pass: boolean = true
  private count: number = 10
  private controller: TabsController = new TabsController()

  build() {
    Column() {
      if(this.pass) {
        if (this.count < 0) {
          Text('count is negative')
            .fontSize(32)
            .id('id1')
        } else if (this.count % 2 === 0) {
          Divider()
            .id('id2')
          Text('even')
            .fontSize(32)
            .id('id3')
        } else {
          Divider()
            .id('id4')
          Column() {
            Text('odd')
              .fontSize(32)
              .id('id5')
          }
          .id('id10')
        }
      } else {
        Text('fail')
          .id('id6')
          .fontSize(32)
      }

      if(this.pass) Text('odd2').fontSize(32).id('id7')

      List({space: 3}) {
        if (this.pass) {
          ListItem() {
            Row() {
              Text().fontSize(20).margin({left:10})
            }.margin({left: 10, right: 10}).id('id11')
          }
          .id('id8')
        }
      }

      Tabs({ barPosition: BarPosition.Start, controller: this.controller }) {
        if (this.pass) {
          TabContent() {
            Text('111').width('100%').height('20').backgroundColor(Color.Pink)
          }
          .tabBar('pink')
          .id('id9')
        }
      }

      if (this.pass) {
        if (this.count === 10) {
            Column() {
              Text('111')
            }
            XComponent({id: 'special', type: ''}).id('id12')
            Column() {
                Text('11')
            }
            .id('id13')
            Column() {
              Text('222')
            }
        }
      }

      if (this.pass) {
          Child()
            .id('id14')
          Text('111')
      }
    }
  }
}

@Component
struct Child {
    build() {
        Column() {
            Text('Child')
                .fontSize(50)
        }
    }
}