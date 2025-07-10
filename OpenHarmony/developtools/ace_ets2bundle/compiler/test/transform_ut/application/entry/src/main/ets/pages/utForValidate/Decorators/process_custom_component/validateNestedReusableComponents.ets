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

/*
   This test case is used to verify nested use of components.
 */
import {V1C,V1R,V2C,V2R} from  './exportComponents'
@Entry
@Component
struct V11 {
  build() {
    Column() {
      V1R()
      V1C()
      V2C()
      V2R()
      V11()
      V22()
      V11R()
      V22R()
    }
  }
}

@ComponentV2
struct V22 {
  build() {
    Column() {
      V1R()
      V1C()
      V2C()
      V2R()
      V11()
      V22()
      V11R()
      V22R()
    }
  }
}

@ReusableV2
@ComponentV2
struct V22R {
  build() {
    Column() {
      V1R()
      V1C()
      V2C()
      V2R()
      V11()
      V22()
      V11R()
      V22R()
    }
  }
}


@Reusable
@Component
struct V11R {
  build() {
    Column() {
      V1R()
      V1C()
      V2C()
      V2R()
      V11()
      V22()
      V11R()
      V22R()
    }
  }
}
