/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
struct TextAreaExample {
  @State insertValue: string = ""
  @State deleteValue: string = ""
  @State insertOffset: number = 0
  @State deleteOffset: number = 0
  @State deleteDirection: number = 0
  @State message:string=''
  @State message1:string=''
  @State message2:string=''
  @State message3:string=''

  build() {
    Row() {
      Column() {
        Text(this.message).id('TextArea_1')
        Text(this.message1).id('TextArea_2')
        Text(this.message2).id('TextArea_3')
        Text(this.message3).id('TextArea_4')
        TextArea({ text: "支持插入回调" })
          .id('TextArea_5')
          .width(300)
          .height(60)
          .onWillInsert(() => {
           this.message = 'onWillInsert'
            console.log('1111')
            return true;
          })
          .onDidInsert(() => {
            this.message1 = 'onDidInsert'
            console.log('22')
          })
        TextArea({ text: "支持删除回调" })
          .id('TextArea_6')
          .width(300)
          .height(60)
          .onWillDelete(() => {
            this.message2 = 'onWillDelete'
            return true;
          })
          .onDidDelete(() => {
            this.message3 = 'onDidDelete'
          })
        Text("deleteValue:" + this.deleteValue + "  deleteOffset:" + this.deleteOffset).height(30)
        Text("deleteDirection:" + (this.deleteDirection == 0 ? "BACKWARD" : "FORWARD")).height(30)

      }.width('100%')
    }
    .height('100%')
  }
}