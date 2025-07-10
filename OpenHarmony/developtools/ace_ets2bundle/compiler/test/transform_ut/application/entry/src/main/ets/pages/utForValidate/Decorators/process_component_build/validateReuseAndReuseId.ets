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
   This test case verifies the use of the resue and reuseId attributes.
 */
@Entry
@Component
struct V111 {
  build() {}
}

@ComponentV2
struct V222 {
  build() {
    Column() {
      V111().reuse({})
      V222().reuse({})
      V111R().reuse({})
      V222R().reuse({})
      V222R().reuseId("1")
    }
  }
}

@ReusableV2
@ComponentV2
struct V222R {
  build() {}
}


@Reusable
@Component
struct V111R {
  build() {}
}
