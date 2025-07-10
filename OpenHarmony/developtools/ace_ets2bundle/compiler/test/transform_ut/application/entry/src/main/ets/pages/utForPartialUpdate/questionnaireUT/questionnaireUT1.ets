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

class BottomControllerParam {
  playedTime: string = ''
  totalTime: string = ''
  playedTimeNumber: number = 0
}

@Entry
@Component
struct VideoPlayer {
  private totalTime: string = ''
  @State showUl: boolean = true
  @State currentPlayedTime: string = '00:00'
  @State currentPlayedTimeNumber: number = 0
  @State isFull: boolean = false

  @Builder bottomController($$: BottomControllerParam) {
    Text('PlayTime:' + $$.playedTimeNumber)
      .fontSize(13)
      .margin({left: 4, bottom: 12})
      .id('play_time')
    Slider({value: $$.playedTimeNumber, min: 0, max: 1000, step: 1, style: SliderStyle.OutSet})
      .width('100%')
      .margin({left: 8, right: 8})
      .trackColor(Color.Gray)
      .showSteps(true)
      .id('slider')
  }
  build() {
    Column(){
      this.bottomController({playedTime: this.currentPlayedTime, totalTime: this.totalTime, playedTimeNumber: this.currentPlayedTimeNumber})
    }
  }
}

export enum VideoState {
  Init,
  Buffering,
  Playing,
  Pause,
  Stop,
  Finish
}
