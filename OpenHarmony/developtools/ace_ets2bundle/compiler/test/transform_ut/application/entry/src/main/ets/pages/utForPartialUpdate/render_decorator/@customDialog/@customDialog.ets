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
@CustomDialog
struct DialogExample {
  @Prop count: number
  @Link isPlaying: boolean
  controller: CustomDialogController;
  termsToAccept: string = ""
  action1: () => void;
  action2: (x: number, s: string) => void;

  build() {
    Column(){
      Row(){
        Button('current count is: ' + this.count)
          .onClick(() => {
            this.count++;
          })
      }
      Row(){
        Button(this.isPlaying ? 'play' : 'pause')
          .onClick(() => {
            this.isPlaying = !this.isPlaying;
          })
      }
      Row() {
        Button ("Option A")
          .onClick(() => {
            this.controller.close();
            this.action1();
          })
        Button ("Option B")
          .onClick(() => {
            this.controller.close();
            this.action2(47, "Option B is great choice");
          })
      }
    }
  }
}

@Entry
@Component
struct CustomDialogUser {
  @State countInitValue: number = 10;
  @State playingInitValue: boolean = false;
  dialogController : CustomDialogController = new CustomDialogController({
    builder: DialogExample({
      termsToAccept: "Please accept the terms.",
      action1: this.onAccept,
      action2: this.existApp,
      count: this.countInitValue,
      isPlaying: $playingInitValue
    }),
    cancel: this.existApp,
    autoCancel: false
  });

  onAccept() {
    console.log("onAccept");
  }
  existApp() {
    console.log("Cancel dialog!");
  }

  build() {
    Column() {
      Text('current countInitValue is: ' + this.countInitValue)
        .fontSize(20)
      Text('current playingInitValue is: ' + this.playingInitValue)
        .fontSize(20)
      Button("Click to open Dialog -1")
        .onClick(() => {
          this.countInitValue--;
          this.dialogController.open()
        })
      Button("Click to close Dialog +1")
        .onClick(() => {
          this.countInitValue++;
          this.dialogController.close()
        })
    }
  }
}