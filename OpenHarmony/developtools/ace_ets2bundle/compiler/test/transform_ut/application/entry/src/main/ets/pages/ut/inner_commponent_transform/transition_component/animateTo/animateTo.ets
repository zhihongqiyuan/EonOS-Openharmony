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
struct TransitionExample {
  @State btnW: number = 50
  @State btnH: number = 50
  @State btn1: boolean = false
  @State show: string = "show"
  @State color: Color = Color.Red;
  @State width1: number = 100;
  @State height1: number = 100;
  @State opacity1: number = 1;
  @State borderRaius1: number = 10
  build() {
    Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center,}) {
      Button(this.show)
        .onClick(() => {
          animateTo({ duration: 1000 }, () => {
            this.btn1 = !this.btn1
            if(this.btn1){
              this.show = "hide"
            }else{
              this.show = "show"
            }
          })
        })
      if (this.btn1) {
        Button()
          .width("80%").height(30)
          .transition({ type: TransitionType.Insert, scale: {x:0,y:1.0}})
          .transition({ type: TransitionType.Delete, scale: { x: 1.0, y: 0.0 }})
      }
      Button('animation')
        .width(this.btnW).height(this.btnH)
        .onClick(() => {
          this.btnW += 50
        })
        .animation({
          duration: 1000, // 动画时长
          curve: Curve.EaseOut, // 动画曲线
          delay: 200, // 动画延迟
          iterations: 1, // 播放次数
          playMode: PlayMode.Normal // 动画模式
        })
      Column({space: 5}) {
        Column()
          .opacity(this.opacity1)
          .backgroundColor(this.color)
          .animation({duration: 1000})
          .width(this.width1)
          .animation({duration: 2000})
          .height(this.height1)
          .borderRadius(this.borderRaius1)
          .animation({duration: 3000})
          .onClick(() => {
            this.color = Color.Green
            this.borderRaius1 = 20
            this.opacity1 = 0.5
            this.height1 = 200
            this.width1 = 200
          })
      }
      .width("100%")
      .height("100%")
    }.height(400).width("100%").padding({top:100})
  }
}