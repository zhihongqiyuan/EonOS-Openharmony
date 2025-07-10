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
import { TestComponent } from '../../../../../test/TestComponent';
import { Animal } from '../../../../../test/TsModule';

@Entry
@Component
struct MyComponent {
  private pass: boolean = true
  private count: number = 10
  build() {
    Column() {
      if(this.pass) {
        if (this.count < 0) {
          Text('count is negative')
            .fontSize(32)
        } else if (this.count % 2 === 0) {
          Divider()
          Text('even')
            .fontSize(32)
        } else {
          Divider()
          Text('odd')
            .fontSize(32)
        }
      }else{
        Text('fail')
          .fontSize(32)
      }

      if(Animal.Dog){
        TestComponent({content: 'if (import enum)'})
        Divider()
      }
    }
  }
}