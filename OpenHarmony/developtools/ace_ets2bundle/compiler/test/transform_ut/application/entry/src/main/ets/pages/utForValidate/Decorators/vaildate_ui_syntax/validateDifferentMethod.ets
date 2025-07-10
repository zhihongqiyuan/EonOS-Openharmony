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
declare const CustomMethod: PropertyDecorator;

@Entry
@Component
struct ValidateDifferentMethodHomeComponent {
  build() {
    Column() {}
  }
}
@Builder
@Styles
function testFunction() {
  Text("testFunction")
}

@Builder
@CustomMethod
function testFunction1() {
  Text("testFunction1")
}

@Builder
function testFunction2() {
  Text("testFunction2")
}

@Styles
function testFunction3() {
  .width(100)
}

@Component
struct ValidateDifferentMethodTestComponent {
  build() {

  }

  @Builder
  @Styles
  testMethod() {

  }

  @Builder
  @CustomMethod
  testMethod1() {

  }
}

@ComponentV2
struct ValidateDifferentMethodTestComponent1 {
  build() {

  }

  @Builder
  @Monitor("aa")
  testMethod2() {

  }

  @Builder
  @Computed
  testMethod3() {

  }

  @Builder
  @Styles
  testMethod5() {

  }
}