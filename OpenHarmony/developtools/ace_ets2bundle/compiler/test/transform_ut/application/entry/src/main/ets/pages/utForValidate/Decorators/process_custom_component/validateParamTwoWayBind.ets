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
const gloabel_value: string = "Foo"
const obj: paramA = {
  gloabel_value:  "Foo"
}
class paramA {
  gloabel_value: string = "Foo"
}
@Entry
@ComponentV2
struct validateParamTwoWayBind {
  @Local local_value: string = "Foo"
  obj: paramA = {
    gloabel_value:  "Foo"
  }
  aa() {
    return {
      gloabel_value:  "Foo"
    } as paramA
  }
  bb() {
    return "Foo";
  }
  build() {
    Column() {
      testParamChild1({value: this.local_value!!})
      testParamChild2({paramValue: this.obj?.gloabel_value!!})
      testParamChild2({paramValue: this.aa()?.gloabel_value!!})
      testParamChild2({paramValue: this.bb()!!})
      testParamChild2({paramValue: "hello"!!})
    }
  }
}

@ComponentV2
struct testParamChild1 {
  @Param value: string = "hello"
  build() {}
}

@ComponentV2
struct testParamChild2 {
  @Param paramValue: string = "hello"
  @Event $paramValue: (value: string) => void = (value: string) => {}
  build() {}
}