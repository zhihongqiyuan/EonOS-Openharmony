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

@Component
struct DefaultComponent {
  @Link DefaultComponentLink1: any
  @Link DefaultComponentLink2: number
  @Link DefaultComponentLink3: boolean
  @Link DefaultComponentLink4: string

  private myVar: number = 0
  private myVar2: number

  build() {
    Column(){
      Text('DefaultComponent')
        .fontSize(20)
      Text('DefaultComponentLink1: ' + JSON.stringify(this.DefaultComponentLink1))
        .fontSize(20)
      Text('DefaultComponentLink2: ' + JSON.stringify(this.DefaultComponentLink2))
        .fontSize(20)
      Text('DefaultComponentLink3: ' + JSON.stringify(this.DefaultComponentLink3))
        .fontSize(20)
      Text('DefaultComponentLink4: ' + JSON.stringify(this.DefaultComponentLink4))
        .fontSize(20)
      Text('myVar: ' + JSON.stringify(this.myVar))
        .fontSize(20)
      Text('myVar2: ' + JSON.stringify(this.myVar2))
        .fontSize(20)
    }
  }
}

export default DefaultComponent
