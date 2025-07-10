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
struct NamespaceComponent1 {
  @Link NamespaceComponent1Link1: any
  @Link NamespaceComponent1Link2: number
  @Link NamespaceComponent1Link3: boolean
  @Link NamespaceComponent1Link4: string

  private myVar: number = 0
  private myVar2: number

  build() {
    Column(){
      Text('NamespaceComponent1')
        .fontSize(20)
      Text('NamespaceComponent1Link1: ' + JSON.stringify(this.NamespaceComponent1Link1))
        .fontSize(20)
      Text('NamespaceComponent1Link2: ' + JSON.stringify(this.NamespaceComponent1Link2))
        .fontSize(20)
      Text('NamespaceComponent1Link3: ' + JSON.stringify(this.NamespaceComponent1Link3))
        .fontSize(20)
      Text('NamespaceComponent1Link4: ' + JSON.stringify(this.NamespaceComponent1Link4))
        .fontSize(20)
      Text('myVar: ' + JSON.stringify(this.myVar))
        .fontSize(20)
      Text('myVar2: ' + JSON.stringify(this.myVar2))
        .fontSize(20)
    }
  }
}

@Component
struct NamespaceComponent2 {
  @Link NamespaceComponent2Link1: any
  @Link NamespaceComponent2Link2: number
  @Link NamespaceComponent2Link3: boolean
  @Link NamespaceComponent2Link4: string

  private myVar: number = 0
  private myVar2: number

  build() {
    Column(){
      Text('NamespaceComponent2')
        .fontSize(20)
      Text('NamespaceComponent2Link1: ' + JSON.stringify(this.NamespaceComponent2Link1))
        .fontSize(20)
      Text('NamespaceComponent2Link2: ' + JSON.stringify(this.NamespaceComponent2Link2))
        .fontSize(20)
      Text('NamespaceComponent2Link3: ' + JSON.stringify(this.NamespaceComponent2Link3))
        .fontSize(20)
      Text('NamespaceComponent2Link4: ' + JSON.stringify(this.NamespaceComponent2Link4))
        .fontSize(20)
      Text('myVar: ' + JSON.stringify(this.myVar))
        .fontSize(20)
      Text('myVar2: ' + JSON.stringify(this.myVar2))
        .fontSize(20)
    }
  }
}

@Component
struct NamespaceComponent3 {
  @Link NamespaceComponent3Link1: any
  @Link NamespaceComponent3Link2: number
  @Link NamespaceComponent3Link3: boolean
  @Link NamespaceComponent3Link4: string

  private myVar: number = 0
  private myVar2: number

  build() {
    Column(){
      Text('NamespaceComponent3')
        .fontSize(20)
      Text('NamespaceComponent3Link1: ' + JSON.stringify(this.NamespaceComponent3Link1))
        .fontSize(20)
      Text('NamespaceComponent3Link2: ' + JSON.stringify(this.NamespaceComponent3Link2))
        .fontSize(20)
      Text('NamespaceComponent3Link3: ' + JSON.stringify(this.NamespaceComponent3Link3))
        .fontSize(20)
      Text('NamespaceComponent3Link4: ' + JSON.stringify(this.NamespaceComponent3Link4))
        .fontSize(20)
      Text('myVar: ' + JSON.stringify(this.myVar))
        .fontSize(20)
      Text('myVar2: ' + JSON.stringify(this.myVar2))
        .fontSize(20)
    }
  }
}

export { NamespaceComponent1, NamespaceComponent2 }

export default NamespaceComponent3
