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
@Builder
function MyBuilder(value: string, size: number) {
  Column() {
    Text(value)
      .fontSize(size)
  }
}

let globalBuilder: WrappedBuilder<[string, number]> = wrapBuilder(MyBuilder);

@Entry
@Component
struct TestWrapperBuilder {
  @State message: string = 'Hello World';

  build() {
    Row() {
      Column() {
        Text("wrapperBuilder 场景一：")
          .fontSize(20)
        globalBuilder.builder(this.message, 50)
        Divider()
        Text("wrapperBuilder 场景二：")
          .fontSize(20)
        TestWrapperBuilderChild()
        Divider()
        Text("wrapperBuilder 场景三：")
          .fontSize(20)
        TestWrapperBuilderChildTwo()
      }
      .width('100%')
    }
    .height('100%')
  }
}

@Builder
function YourBuilder(value: string, size: number) {
  Column() {
    Text(value)
      .fontSize(size)
      .fontColor(Color.Pink)
  }
}

const builderArr: WrappedBuilder<[string, number]>[] = [wrapBuilder(MyBuilder), wrapBuilder(YourBuilder)];

@ComponentV2
struct TestWrapperBuilderChild {
  @Builder
  testBuilder() {
    ForEach(builderArr, (item: WrappedBuilder<[string, number]>) => {
      item.builder('Hello World', 30)
    })
  }

  build() {
    Row() {
      Column() {
        this.testBuilder()
      }
    }
  }
}

class Tmp {
  paramA2: string = 'hello';
}

@Builder
function overBuilder(param: Tmp) {
  Column() {
    Text(`wrapBuilderValue:${param.paramA2}`)
  }
}

const wBuilder: WrappedBuilder<[Tmp]> = wrapBuilder(overBuilder);

@Component
struct TestWrapperBuilderChildTwo {
  @State label: Tmp = new Tmp();

  build() {
    Column() {
      wBuilder.builder({ paramA2: this.label.paramA2 })
      Button('Click me change UI update')
        .onClick(() => {
          this.label.paramA2 = 'ArkUI';
        })
    }
  }
}