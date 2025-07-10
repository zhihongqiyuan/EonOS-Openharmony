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
struct AMDComponent {
  @Link AMDComponentLink1: any
  @Link AMDComponentLink2: number
  @Link AMDComponentLink3: boolean
  @Link AMDComponentLink4: string

  private myVar: number = 0
  private myVar2: number

  build() {
    Column(){
      Text('AMDComponent')
        .fontSize(20)
      Text('AMDComponentLink1: ' + JSON.stringify(this.AMDComponentLink1))
        .fontSize(20)
      Text('AMDComponentLink2: ' + JSON.stringify(this.AMDComponentLink2))
        .fontSize(20)
      Text('AMDComponentLink3: ' + JSON.stringify(this.AMDComponentLink3))
        .fontSize(20)
      Text('AMDComponentLink4: ' + JSON.stringify(this.AMDComponentLink4))
        .fontSize(20)
      Text('myVar: ' + JSON.stringify(this.myVar))
        .fontSize(20)
      Text('myVar2: ' + JSON.stringify(this.myVar2))
        .fontSize(20)
    }
  }
}

export = AMDComponent
