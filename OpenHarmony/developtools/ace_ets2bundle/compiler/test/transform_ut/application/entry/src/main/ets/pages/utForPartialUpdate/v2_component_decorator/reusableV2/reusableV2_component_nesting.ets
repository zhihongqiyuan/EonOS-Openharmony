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
 * This test case is used to verify the compilation results when the component decorated by @ReusableV2 is reused.
 */
import { V1Child2, V2Child2, ReuseV2Child3 } from './reusableV2_exportComponent';

@Entry
@ComponentV2
struct ParentOne {
  @Local message: string = 'Hello World';
  build() {
    Column() {
      ReuseV2Child1({a: 'aaa', b: 'bbb', e1: (num: number) => { this.message += num; }})
      ReuseV2Child2().reuse({ reuseId: ()=> "idOne" })
      V2Child()
      V1Child()
      V1Child2()
      V2Child2()
      ReuseV2Child3().reuse({ reuseId: ()=> "idThree" })
    }
  }
}

@ReusableV2
@ComponentV2
struct ReuseV2Child1 {
  @Param a: string = 'a';
  @Require @Once @Param b: string;
  @Event e1:(num: number)=>void = ()=>{};
  build() {}
}

@ReusableV2
@ComponentV2
struct ReuseV2Child2 {
  build() {}
}

@ComponentV2
struct V2Child {
  build() {}
}

@Component
struct V1Child {
  build() {}
}
