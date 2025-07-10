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
struct LinkComponent1 {
  @Link LinkComponent1Link1: any
  @Link LinkComponent1Link2: number
  @Link LinkComponent1Link3: boolean
  @Link LinkComponent1Link4: string
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
struct LinkComponent2 {
  @Link LinkComponent2Link1: any
  @Link LinkComponent2Link2: number
  @Link LinkComponent2Link3: boolean
  @Link LinkComponent2Link4: string
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
struct LinkComponent3 {
  @Link LinkComponent3Link1: any
  @Link LinkComponent3Link2: number
  @Link LinkComponent3Link3: boolean
  @Link LinkComponent3Link4: string
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
struct LinkComponent4 {
  @Link LinkComponent3Link1: any
  @Link LinkComponent3Link2: number
  @Link LinkComponent3Link3: boolean
  @Link LinkComponent3Link4: string
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

export { LinkComponent1 as LinkComponent, LinkComponent2, LinkComponent3 }

export default LinkComponent4
