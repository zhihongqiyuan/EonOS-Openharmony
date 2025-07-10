/**
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
import Log from '../../../../../../../../../common/src/main/ets/default/Log';
import ConfigData, { DoNotDisturbType } from '../../common/constants';
import ViewModel from '../../vm/noDisturbComponentViewModel'

const TAG = 'ManagementComponent-NoDisturbComponent';

@Component
export default struct NoDisturbComponent {
  @State mViewModel: ViewModel = new ViewModel()
  @Prop @Watch('onRefresh') refreshRequest: number
  mPrevReapMode: number = 0;
  mPrevStartTime: Date= new Date()
  mPrevEndTime: Date= new Date()
  mPrevGetCnt: number = 0;
  mPrevPrompt: string = '';
  @State mRefreshCnt: number = 0;

  aboutToAppear(): void{
    Log.showInfo(TAG, `aboutToAppear`);
    this.mViewModel.viewModelInit();
    this.mPrevGetCnt = 0;
    this.mRefreshCnt++;
  }

  onRefresh(): void {
    Log.showInfo(TAG, `onRefresh`);
    this.mViewModel.viewModelInit();
    this.mPrevGetCnt = 0;
    this.mRefreshCnt++;
  }

  build() {
    Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.SpaceBetween }) {
      Row() {
        Column() {
          Row() {
            Text($r('app.string.noDisturb'))
              .fontSize($r('app.float.itemComp_titel_font'))
              .fontColor($r('app.color.itemComp_title_color'))
              .textAlign(TextAlign.Start)
              .fontWeight(FontWeight.Medium)
          }

          Row() {
            if (this.mRefreshCnt > 0) {
              if (this.mViewModel.repeatMode == DoNotDisturbType.TYPE_ONCE) {
                Text($r('app.string.noDisturb_once_label'))
                  .fontSize($r('app.float.itemComp_value_font'))
                  .fontColor($r('app.color.itemComp_font_color'))
                  .textAlign(TextAlign.Start)
                Text(this.mViewModel.prompt)
                  .fontSize($r('app.float.itemComp_value_font'))
                  .fontColor($r('app.color.itemComp_font_color'))
                  .textAlign(TextAlign.Start)
                  .margin({ top: $r("app.float.noDisturb_text_margin_vertical") })
              } else if (this.mViewModel.repeatMode == DoNotDisturbType.TYPE_DAILY) {
                Text($r('app.string.noDisturb_daily_label'))
                  .fontSize($r('app.float.itemComp_value_font'))
                  .fontColor($r('app.color.itemComp_font_color'))
                  .textAlign(TextAlign.Start)
                Text(this.mViewModel.prompt)
                  .fontSize($r('app.float.itemComp_value_font'))
                  .fontColor($r('app.color.itemComp_font_color'))
                  .textAlign(TextAlign.Start)
                  .margin({ top: $r("app.float.noDisturb_text_margin_vertical") })
              } else if (this.mViewModel.repeatMode == DoNotDisturbType.TYPE_CLEARLY) {
                Text($r('app.string.noDisturb_clearly_label'))
                  .fontSize($r('app.float.itemComp_value_font'))
                  .fontColor($r('app.color.itemComp_font_color'))
                  .textAlign(TextAlign.Start)
                Text(this.mViewModel.prompt)
                  .fontSize($r('app.float.itemComp_value_font'))
                  .fontColor($r('app.color.itemComp_font_color'))
                  .textAlign(TextAlign.Start)
                  .margin({ top: $r("app.float.noDisturb_text_margin_vertical") })
              } else {
                Text($r('app.string.noDisturb_closed'))
                  .fontSize($r('app.float.itemComp_value_font'))
                  .fontColor($r('app.color.itemComp_font_color'))
                  .textAlign(TextAlign.Start)
                  .margin({ top: $r("app.float.noDisturb_text_margin_vertical") })
              }
            } else {
              Blank();
            }
          }
        }
        .alignItems(HorizontalAlign.Start)
      }
      .padding({ left: $r('sys.float.ohos_id_default_padding_start') })
      .flexShrink(1)
      .align(Alignment.Start)
      .alignItems(VerticalAlign.Center)

      Row() {
        Toggle({ type: ToggleType.Switch, isOn: this.mViewModel.isEffective })
          .align(Alignment.End)
          .width($r('app.float.toggle_comp_width'))
          .height($r('app.float.toggle_comp_height'))
          .selectedColor($r("app.color.font_color_007DFF"))
          .onChange((data) => {
            Log.showInfo(TAG, `Toggle onChange param: data = ${data}`);
            if (this.mPrevGetCnt == 0) {
              this.mPrevReapMode = this.mViewModel.repeatMode
              if (this.mPrevReapMode == DoNotDisturbType.TYPE_NONE) {
                this.mPrevReapMode = DoNotDisturbType.TYPE_DAILY;
                this.mPrevStartTime = this.mViewModel.getDateByHHMI(ConfigData.TIME_EMPTY);
                this.mPrevEndTime = this.mViewModel.getDateByHHMI(ConfigData.TIME_EMPTY);
              } else {
                this.mPrevStartTime = this.mViewModel.defaultStartTime;
                this.mPrevEndTime = this.mViewModel.defaultEndTime;
              }
              this.mPrevGetCnt++;
            }
            this.mViewModel.isEffective = data;
            this.mViewModel.repeatMode = this.mViewModel.isEffective ? this.mPrevReapMode : DoNotDisturbType.TYPE_NONE;
            if (this.mViewModel.repeatMode == DoNotDisturbType.TYPE_NONE) {
              this.mViewModel.refreshDate.bind(this.mViewModel)(this.mViewModel.repeatMode,
              this.mViewModel.getDateByHHMI(ConfigData.TIME_EMPTY),
              this.mViewModel.getDateByHHMI(ConfigData.TIME_EMPTY));
            } else {
              this.mViewModel.refreshDate.bind(this.mViewModel)(this.mViewModel.repeatMode,
                this.mPrevStartTime,
                this.mPrevEndTime);
            }
            this.mViewModel.setNoDisturbingDate.bind(this.mViewModel)();
            this.mViewModel.setCluesWithoutSetEffect.bind(this.mViewModel)();
            this.mRefreshCnt++
          })

      }
      .flexShrink(0)
      .align(Alignment.End)
      .alignItems(VerticalAlign.Center)
      .padding({ right: $r('sys.float.ohos_id_default_padding_end') })
    }
    .width(ConfigData.WH_100_100)
    .height($r('app.float.itemComp_noDisturb_height'))
    .border({ width: $r('app.float.border_width'), color: Color.White,
      radius: $r('app.float.border_radius') })
    .backgroundColor(Color.White)
  }
}
