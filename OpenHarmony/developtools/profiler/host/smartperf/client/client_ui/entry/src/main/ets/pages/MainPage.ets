/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

import { Mine } from '../common/ui/main/Mine';
import { Home } from '../common/ui/main/Home';
import { Report } from '../common/ui/main/Report';
import { HomeBottomPage } from '../common/ui/main/HomeBottomPage';
/*
 * 主页 程序主页面
 */
@Entry
@Component
struct Main {
  @State currentPage: number = 0

  build() {
    Column() {
      Column() {
        if (this.currentPage == 0) {
          Home()
        } else if (this.currentPage == 1) {
          Report()
        } else if (this.currentPage == 2) {
          Mine()
        }
      }.width('100%').height('88%')
      .flexGrow(1)

      HomeBottomPage({ currentPage: $currentPage })
    }
  }
}









