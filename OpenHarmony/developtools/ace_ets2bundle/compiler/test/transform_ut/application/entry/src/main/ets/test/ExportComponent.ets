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

import DefaultComponent from "./DefaultComponent"

@Component
struct ExportComponent1 {
  @Link ExportComponent1Link1: any
  @Link ExportComponent1Link2: number
  @Link ExportComponent1Link3: boolean
  @Link ExportComponent1Link4: string
  @State indexState1: any = { count: 0 }
  @State indexState2: number = 0
  @State indexState3: boolean = false
  @State indexState4: string = 'Home'

  build() {
    Row() {
      DefaultComponent({
        DefaultComponentLink1: $indexState1,
        DefaultComponentLink2: $indexState2,
        DefaultComponentLink3: $indexState3,
        DefaultComponentLink4: $indexState4,
        myVar: 100,
        myVar2: 100
      })
    }
  }
}

@Component
struct ExportComponent2 {
  @Link ExportComponent2Link1: any
  @Link ExportComponent2Link2: number
  @Link ExportComponent2Link3: boolean
  @Link ExportComponent2Link4: string
  @State indexState1: any = { count: 0 }
  @State indexState2: number = 0
  @State indexState3: boolean = false
  @State indexState4: string = 'Home'

  build() {
    Row() {
      DefaultComponent({
        DefaultComponentLink1: $indexState1,
        DefaultComponentLink2: $indexState2,
        DefaultComponentLink3: $indexState3,
        DefaultComponentLink4: $indexState4,
        myVar: 100,
        myVar2: 100
      })
    }
  }
}

@Component
struct ExportComponent3 {
  @Link ExportComponent3Link1: any
  @Link ExportComponent3Link2: number
  @Link ExportComponent3Link3: boolean
  @Link ExportComponent3Link4: string
  @State indexState1: any = { count: 0 }
  @State indexState2: number = 0
  @State indexState3: boolean = false
  @State indexState4: string = 'Home'

  build() {
    Row() {
      DefaultComponent({
        DefaultComponentLink1: $indexState1,
        DefaultComponentLink2: $indexState2,
        DefaultComponentLink3: $indexState3,
        DefaultComponentLink4: $indexState4,
        myVar: 100,
        myVar2: 100
      })
    }
  }
}

@Component
export default struct ExportComponent4 {
  @Link ExportComponent4Link1: any
  @Link ExportComponent4Link2: number
  @Link ExportComponent4Link3: boolean
  @Link ExportComponent4Link4: string
  @State indexState1: any = { count: 0 }
  @State indexState2: number = 0
  @State indexState3: boolean = false
  @State indexState4: string = 'Home'

  build() {
    Row() {
      DefaultComponent({
        DefaultComponentLink1: $indexState1,
        DefaultComponentLink2: $indexState2,
        DefaultComponentLink3: $indexState3,
        DefaultComponentLink4: $indexState4,
        myVar: 100,
        myVar2: 100
      })
    }
  }
}

export { ExportComponent1 as ExportComponent, ExportComponent2, ExportComponent3 }
