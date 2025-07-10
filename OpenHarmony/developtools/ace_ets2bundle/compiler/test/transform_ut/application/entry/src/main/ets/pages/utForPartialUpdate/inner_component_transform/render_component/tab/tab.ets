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
struct TabSimple {
  private controller: TabsController = new TabsController();
  build() {
    Column() {
      Tabs({ barPosition: BarPosition.Start, index: 1, controller: this.controller}) {
        TabContent() {
          Flex() {
            Column() {
              Text('text1')
              .height(100)
              .width(200)
              Text('xxx')
              .height(100)
              .width(200)
            }
            .height(100)
            .width(200)
          }
            .height(100)
            .width(200)
        }
        .tabBar("TabBar")
        .height(100)
        .width(200)
        TabContent() {
          Text('text2')
        }
        .tabBar("TabBar 2")
        .height(100)
        .width(200)
      }
    }
  }
}