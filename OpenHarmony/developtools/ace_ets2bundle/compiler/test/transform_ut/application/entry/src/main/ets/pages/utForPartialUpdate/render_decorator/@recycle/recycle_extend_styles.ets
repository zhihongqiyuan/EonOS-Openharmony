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
@Extend(Button) function fancybut(color:string|Color){
  .backgroundColor(color)
  .width(200)
  .height(100)
}

@Reusable
@Component
struct ExtendComponent {
  @State width_value: string = "100%"
  build() {
    Column(){
      Button("Fancy Button").fancybut(Color.Green)
      Button("Fancy Button").fancybut(Color.Green).height(100).width(this.width_value)
    }
  }
}

@Styles function globalFancy() {
  .backgroundColor(Color.Red)
}

@Reusable
@Component
struct StylesComponent {
  enable: boolean = true
  @State width_value: string = "100%"
  @State size_value: number = 50
  @Styles componentFancy() {
    .backgroundColor(Color.Blue)
    .width(this.width_value)
  }
  build() {
    Column({ space: 10 }) {
      Text("Fancy")
        .globalFancy()
        .width(this.width_value)
        .height(100)
      Text("Fancy")
        .componentFancy()
        .fontSize(this.size_value)
        .height(100)
      Button() {
        Text("Fancy")
          .globalFancy()
          .fontSize(this.size_value)
          .height(100)
      }
      .enabled(this.enable)
      .onClick(() => {
        this.enable = false
      })
      .componentFancy()
      .height(100)
      .stateStyles({
        normal: {
          .backgroundColor(Color.Green)
          .width(this.width_value)
        },
        disabled: this.componentFancy,
        pressed: globalFancy
      })
    }
  }
}