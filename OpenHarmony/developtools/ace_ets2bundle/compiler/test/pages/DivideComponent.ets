/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

@Builder
function textStyles(){
  Text('Builder').backgroundColor(Color.Red)
}

@Component
struct DivideComponent {
  @Link testNum1: number
  @Link testNum2: number

  build() {
    Column(){
      Text('DivideComponent')
        .fontSize(20)
      Text('DivideResult: ' + JSON.stringify(this.testNum1 / this.testNum2))
        .fontSize(20)
    }
  }
}

export { textStyles, DivideComponent }
