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
 * This test case is used to export child components containing the static variables for use in file 'validateStaticVariables.ets'.
 */
@Component
export struct StaticChildThree {
  static c1: string;
  c2: string = 'c2';
  build() {
    Column() {
      Text('c1: ' + StaticChildThree.c1)
      Text('c2: ' + this.c2)
    }
  }
}

@ComponentV2
export struct StaticChildFour {
  static d1: string;
  @Param static d4: string = 'd4';
  @Require @Param static d5: string = 'd5';
  build() {
    Column() {
      Text('d1: ' + StaticChildFour.d1)
      Text('d4: ' + StaticChildFour.d4)
      Text('d5: ' + StaticChildFour.d5)
    }
  }
}
