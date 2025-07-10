/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

import audio from '@ohos.multimedia.audio';
import VolumeModel from '../VolumeModel';
import Log from '../../../../../../../common/src/main/ets/default/Log';

const TAG = 'Control-volumeComponent'
const volumeType = audio.AudioVolumeType.MEDIA;

@Component
export struct MyVol {
  @StorageLink('VolumeValue') volumeValue: number = 0;
  @State volume : any = {
  minValue: 0,
  maxValue: 15,
  value: 7
  };

  aboutToAppear() {
    this.initVolume()
    Log.showInfo(TAG,'Finished init Volume!');
    VolumeModel.init();
    VolumeModel.registerVolume();
    VolumeModel.unRegisterVolume();
  }

  aboutToDisappear () {
    Log.showInfo(TAG,'aboutToDisappear');
  }

  initVolume() {
    VolumeModel.getMaxVolume(this.volume, volumeType);
    VolumeModel.getMinVolume(this.volume, volumeType);
    Log.showInfo(TAG, `initVolume ${this.volume.minValue} ${this.volume.maxValue} `);
  }

  setVolume(value) {
    Log.showInfo(TAG, `setVolume = ${value}`);
    VolumeModel.setVolume(value);
  }


  build() {
    Flex({ direction: FlexDirection.Row, justifyContent: FlexAlign.Start, alignItems: ItemAlign.Center }) {
      Column() {
        Text($r('app.string.volume_control'))
          .fontSize($r('app.float.control_common_font_size')).alignSelf(ItemAlign.Center)

      }
      .width('15%')
      .constraintSize({ maxWidth: $r("app.float.font_constraint_maxSize") })
      .padding({ left: $r('app.float.slider_text_padding_left') })

      Flex({ direction: FlexDirection.Row, justifyContent: FlexAlign.Center, alignItems: ItemAlign.Center }) {
        Slider({
          value: this.volumeValue,
          min: this.volume.minValue,
          max: this.volume.maxValue,
          step: 1,
          style: SliderStyle.InSet
        })
          .width('100%')
          .blockColor(Color.Blue)
          .trackColor(Color.Grey)
          .selectedColor(Color.Blue)
          .onChange((value: number) => {
            this.volume.value = value
            this.setVolume(this.volume)
        })
      }
      .width('85%')
    }
    .height('100%')
    .width('100%')
    .backgroundColor($r('app.color.volume_bg_color'))
    .borderRadius($r('app.float.volume_border_radius'))
  }
}