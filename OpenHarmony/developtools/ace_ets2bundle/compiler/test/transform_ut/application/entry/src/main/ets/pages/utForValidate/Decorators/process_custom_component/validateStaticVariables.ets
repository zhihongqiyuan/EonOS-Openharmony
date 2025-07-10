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
 * This test case is used to verify that a static variable is initialized through the component constructor.
 */
import { StaticChildThree, StaticChildFour } from './exportStaticVariables';

@Entry
@Component
struct TestStaticVariableAssignment {
  build() {
    Column() {
      StaticChildOne({ a1: 'a11', a2: 'a22' })
      StaticChildTwo({
        b1: 'b11',
        b4: 'b44',
        b5: 'b55'
      })
      StaticChildThree({ c1: 'c11', c2: 'c22' })
      StaticChildFour({
        d1: 'd11',
        d4: 'd44',
        d5: 'd55'
      })
    }
    .height('100%')
    .width('100%')
  }
}

@Component
struct StaticChildOne {
  static a1: string;
  a2: string = 'a2';
  build() {
    Column() {
      Text('a1: ' + StaticChildOne.a1)
      Text('a2: ' + this.a2)
    }
  }
}

@ComponentV2
struct StaticChildTwo {
  static b1: string;
  @Param static b4: string = 'b4';
  @Require @Param static b5: string = 'b5';
  build() {
    Column() {
      Text('b1: ' + StaticChildTwo.b1)
      Text('b4: ' + StaticChildTwo.b4)
      Text('b5: ' + StaticChildTwo.b5)
    }
  }
}
