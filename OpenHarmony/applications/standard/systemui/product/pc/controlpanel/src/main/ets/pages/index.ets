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
import Log from '../../../../../../../common/src/main/ets/default/Log'
import CommonStyleManager from '../../../../../../../common/src/main/ets/default/CommonStyleManager'
import HeightConfigUtils from '../../../../../../../common/src/main/ets/default/heightcofigUtils/HeightConfigUtils'
import AbilityManager from '../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager'
import StyleConfiguration, { IndexStyle } from './common/StyleConfiguration'
import StyleManager from './common/StyleManager'
import ControlCenterComponent from './control'
import WindowManager, {
  WindowType,
  WINDOW_SHOW_HIDE_EVENT,
  ArgsInfo,
} from "../../../../../../../common/src/main/ets/default/WindowManager";
import EventManager, { unsubscribe } from "../../../../../../../common/src/main/ets/default/event/EventManager";
import { START_ABILITY_EVENT } from "../../../../../../../common/src/main/ets/default/event/EventUtil";
import MultimodalInputManager, {
  MultiKeyCode,
} from "../../../../../../../common/src/main/ets/default/MultimodalInputManager";
import { Rect } from '@ohos/common'
import display from "@ohos.display"

const TAG = 'ControlPanel-Index'
const SHOW_EVENT = "showControlCenter";
const HIDE_EVENT = "hideControlCenter";

interface DisplayInfo {
  width: number;
  height: number;
}

@Entry
@Component
struct Index {
  @StorageLink('StatusCoefficient') StatusCoefficient: number = 1.0
  @State style: IndexStyle = new IndexStyle()
  @State mBackground: PixelMap | undefined = undefined;
  @State mOpacity: number = 0.0;
  @State mWidthSize: number = 0.2
  @State mHeightSize: number = 0.2
  @State mHeightPx: number = 0
  @State mHeightConfigUtils: HeightConfigUtils | undefined = undefined;
  mClearCallbacks: Array<unsubscribe> = [];
  mShowAnimReady: boolean = false;

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear, start`)

    CommonStyleManager.setAbilityPageName(TAG)
    this.style = StyleConfiguration.getIndexStyle()
    StyleManager.setStyle()

    let rect: Rect = AbilityManager.getAbilityData(AbilityManager.ABILITY_NAME_CONTROL_PANEL, 'rect')
    this.mHeightPx = rect.height

    this.mHeightConfigUtils = new HeightConfigUtils();

    let StatusCoefficient = AppStorage.SetAndLink("StatusCoefficient", 1.0);
    StatusCoefficient.set(this.mHeightConfigUtils.getStatusCoefficient());

    this.initWindowPolicy();
    Log.showDebug(TAG, `aboutToAppear, end`)
  }

  onPageShow() {
    Log.showInfo(TAG, `onPageShow`)
  }

  onPageHide() {
    Log.showInfo(TAG, `onPageHide`)
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`)
    this.mClearCallbacks.forEach((unsubscribe) => unsubscribe());
    this.mClearCallbacks.length = 0;
  }

  build() {
    Column(){
      Stack({ alignContent: Alignment.Top }) {
        Image(this.mBackground)
          .width('100%')
          .height('100%')
          .objectFit(ImageFit.Fill)
        Scroll(new Scroller()) {
          Column() {
            ControlCenterComponent({
              modeChangeCallback: (isEdit: boolean): void => this.onModeChange(isEdit)
            })
          }
          .width('100%')
          .onAreaChange((e, e2) => {
            Log.showInfo(TAG, `onAreaChange, e: ${JSON.stringify(e)} e2: ${JSON.stringify(e2)}`);
          })
        }
        .scrollBarColor(Color.Gray)
        .scrollBarWidth(10)
        .width('100%')
        .height('100%')
        .backgroundColor($r('app.color.default_background'))
      }
      .scale({
        x: this.mWidthSize,
        y: this.mHeightSize,
        z: 1,
        centerX: '100%',
        centerY: '0%'
      })
      .opacity(this.mOpacity)
      .clip(true)
      .borderRadius(this.style.borderRadius)
      .width('97%')
      .height(0.97 * this.mHeightPx + 'px')
    }
    .width('100%')
    .height('100%')
    .alignItems(HorizontalAlign.End)
  }

  initWindowPolicy() {
    Log.showDebug(TAG, `init controlcenter panel window Policy`);
    this.mClearCallbacks.push(
    EventManager.subscribe(SHOW_EVENT, () => this.showWindow()),
    EventManager.subscribe(HIDE_EVENT, () => this.hideWindow()),
    EventManager.subscribe(START_ABILITY_EVENT, () => this.hideWindow()),
    EventManager.subscribe(WINDOW_SHOW_HIDE_EVENT, (args: ArgsInfo) => {
      let windowName = args.windowName;
      let isShow = args?.isShow;
      Log.showInfo(TAG, `WINDOW_SHOW_HIDE_EVENT windowName: ${windowName}, isShow: ${isShow}`);
      windowName == WindowType.NOTIFICATION_PANEL && isShow && this.hideWindow();
      windowName == WindowType.CONTROL_PANEL && isShow && (this.mShowAnimReady = true);
    }),
    EventManager.subscribe('ControlWindowResizeEvent',async (args: ArgsInfo) => {
      let windowName = args.windowName;
      let rect = args?.rect;
      let dis = await display.getDefaultDisplaySync();
      Log.showInfo(TAG, `ControlWindowResizeEvent: ${windowName}, isShow: ${rect}`);
      AbilityManager.setAbilityData(AbilityManager.ABILITY_NAME_CONTROL_PANEL, 'rect', rect);
      AbilityManager.setAbilityData(AbilityManager.ABILITY_NAME_CONTROL_PANEL, 'dis', {
        width: dis.width,
        height: dis.height,
      });
      WindowManager.resetSizeWindow(windowName, rect).then(
      ).then(() => {
      }).catch((err: Error) => {
      });
    }),
    MultimodalInputManager.subscribeCombinationKey([MultiKeyCode.WIN, MultiKeyCode.I], (data) => {
      let windowInfo = WindowManager.getWindowInfo(WindowType.CONTROL_PANEL);
      Log.showInfo(TAG, `on CombinationKeyEvent: data: ${data}, windowInfo: ${windowInfo?.visibility}`);
      if (windowInfo) {
          windowInfo.visibility
          ? this.hideWindow()
          : this.showWindow();
      }
    })
    );
  }

  _animateTo(config: AnimateParam, callback: () => void) {
    animateTo(config, callback)
    setTimeout(config.onFinish, config.duration as number + Number(config.delay))
  }

  showAnimation(){

    //init page state
    this.mOpacity = 0;
    this.mWidthSize = 0.7;
    this.mHeightSize = 0.7;

    //show animation
    animateTo({
      duration: 200,
      curve: Curve.Friction
    }, () => {
      this.mOpacity = 1;
      this.mWidthSize = 1.03;
      this.mHeightSize = 1.03;
    })
    animateTo({
      duration:100,
      curve: Curve.Friction,
      delay: 200
    }, () =>{
      this.mWidthSize = 1;
      this.mHeightSize = 1;
    })
  }

  hideAnimation(){
    //hide animation
    animateTo({
      duration: 100,
      curve: Curve.Friction,
      onFinish: () => {
        WindowManager.hideWindow(WindowType.CONTROL_PANEL);
      }
    }, () => {
      this.mWidthSize = 0.7;
      this.mHeightSize = 0.7;
      this.mOpacity = 0;
    })
  }

  hideWindow() {
    this.hideAnimation();
  }

  showWindow() {
    WindowManager.showWindow(WindowType.CONTROL_PANEL)
    this.showAnimation();
  }

  onModeChange(isEdit: boolean): void {
    Log.showDebug(TAG, `onModeChange, isEdit: ${isEdit}`)
    let initRect = AbilityManager.getAbilityData(AbilityManager.ABILITY_NAME_CONTROL_PANEL, 'rect') as ArgsInfo
    let newRect = initRect as Rect
    if (isEdit) {
      let dis = AbilityManager.getAbilityData(AbilityManager.ABILITY_NAME_CONTROL_PANEL, 'dis') as DisplayInfo
      newRect.height = StyleManager.calcScaleSize(346)
    }
    Log.showDebug(TAG, `onModeChange, newRect: ${JSON.stringify(newRect)}`)
    this._animateTo({
      duration: 300,
      tempo: 1.0,
      curve: Curve.Friction,
      delay: 0,
      iterations: 1,
      playMode: PlayMode.Normal,
      onFinish: () => {
        Log.showInfo(TAG, `onModeChange, show anim finish.`)
        if (initRect.height && newRect.height <= initRect.height) {
          WindowManager.resetSizeWindow(WindowType.CONTROL_PANEL, newRect)
        }
      }
    }, () => {
      Log.showInfo(TAG, `onModeChange, animateTo`)
      if (initRect.height && newRect.height > initRect.height) {
        WindowManager.resetSizeWindow(WindowType.CONTROL_PANEL, newRect)
      }
      this.mHeightPx = newRect.height
    })
  }
}
