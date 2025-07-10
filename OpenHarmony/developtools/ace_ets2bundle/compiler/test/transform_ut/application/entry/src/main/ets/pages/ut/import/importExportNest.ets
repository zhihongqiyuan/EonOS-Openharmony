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
import { tExtend, tStyles, DivideTest, Base } from '../../../test/ImportNestAll';

@Entry
@Component
struct ImportTest {
  @State testText1: string = 'Hello'
  @State testText2: string = 'World'
  @State testText3: string = 'Test'
  @State testText4: string = 'Component'

  @State testState1: string = 'Base'
  @State testState2: number = 0
  @State testState3: object = { name: 'Base' }
  @State testState4: number = 3
  @State testState5: number = 10

  build() {
    Column() {
      Text(this.testText1)
        .fontSize(50)
      tExtend(20)
      Text(this.testText2)
      tStyles()
      Button(this.testText3)
      Text(this.testText4)
        .fontSize(50)

      Base({
        testStr: $testState1,
        testNum: $testState2,
        testObj: $testState3
      })
      DivideTest({
        testNum1: $testState4,
        testNum2: $testState5
      })
    }
  }
}