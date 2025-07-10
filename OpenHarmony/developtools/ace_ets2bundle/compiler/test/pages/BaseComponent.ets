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
function textExtend(fontsize: number){
  Text('Builder').fontSize(fontsize)
}

@Component
struct BaseComponent {
  @Link testStr: string
  @Link testNum: number
  @Link testObj: object

  build() {
    Column(){
      Text('BaseComponent')
        .fontSize(20)
      Text('testStr: ' + JSON.stringify(this.testStr))
        .fontSize(20)
      Text('testNum: ' + JSON.stringify(this.testNum))
        .fontSize(20)
      Text('testObj: ' + JSON.stringify(this.testObj))
        .fontSize(20)
    }
  }
}

export { textExtend, BaseComponent }
