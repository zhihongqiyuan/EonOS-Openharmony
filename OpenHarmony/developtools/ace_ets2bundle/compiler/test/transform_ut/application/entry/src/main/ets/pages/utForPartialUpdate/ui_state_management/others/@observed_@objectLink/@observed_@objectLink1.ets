/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
@Observed
class TestObserved_ObjectLink {
  name: string = "TestObserved_ObjectLink";
}

@Entry
@Component
struct TestObjectLink {
  @State state_value: TestObserved_ObjectLink = new TestObserved_ObjectLink();

  build() {
    RelativeContainer() {
      TestObjectLink_Child({ objectLinkValue: this.state_value })
    }
    .height('100%')
    .width('100%')
  }
}

@Component
struct TestObjectLink_Child {
  @ObjectLink objectLinkValue: TestObserved_ObjectLink;

  build() {
    Text(this.objectLinkValue.name)
  }
}