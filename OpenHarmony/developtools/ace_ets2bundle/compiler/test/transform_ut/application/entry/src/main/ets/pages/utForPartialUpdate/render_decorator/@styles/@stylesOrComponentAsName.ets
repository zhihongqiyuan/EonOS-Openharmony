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
@Entry
@Component
struct Index {
  @Styles Index() {
    .backgroundColor(Color.Blue)
  }
  build() {
    Row() {
      Text('111')
        .width(30)
        .Index()
        .height(30)
      ComB();
      ComA();
      MyComponent();
    }
  }
}

@Component
struct ComA {
   @Styles ComA() {
    .backgroundColor(Color.Blue)
  }
  build() {
    Row() {
      Text('111')
        .ComA()
        .width(30)
        .height(30)
    }
  }
}

@Component
struct ComB {
   @Styles ComB() {
    .backgroundColor(Color.Blue)
  }
  build() {
    Row() {
      Text('111')
        .width(30)
        .height(30)
        .ComB()
    }
  }
}

@Component
struct MyComponent {
  @Styles MyComponent() {
    .backgroundColor(Color.Gray)
  }

  build() {
    Column() {
      Text('Text1')
        .fontSize(50)
        .fontColor(Color.White)
        .stateStyles({
          normal: this.MyComponent,
        })
    }
  }
}