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
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import AbilityManager from '../../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import CommonStyleConfiguration from '../../../../../../../../common/src/main/ets/default/StyleConfiguration';
import StatusBarStyleConfiguration from '../../../../../../../../features/statusbarcomponent/src/main/ets/com/ohos/common/StyleConfiguration';
import BatteryStyleConfiguration from '../../../../../../../../features/batterycomponent/src/main/ets/default/common/StyleConfiguration';
import ClockStyleConfiguration from '../../../../../../../../features/clockcomponent/src/main/ets/default/common/StyleConfiguration';
import WifiStyleConfiguration from '../../../../../../../../features/wificomponent/src/main/ets/default/common/StyleConfiguration';
import BluetoothStyleConfiguration from '../../../../../../../../features/bluetoothcomponent/src/main/ets/com/ohos/common/StyleConfiguration';
import SignalStyleConfiguration from '../../../../../../../../features/signalcomponent/src/main/ets/default/common/StyleConfiguration';
import CapsuleStyleConfiguration from '../../../../../../../../features/capsulecomponent/src/main/ets/default/common/StyleConfiguration';
import LocationStyleConfiguration from '../../../../../../../../features/locationcomponent/src/main/ets/com/ohos/common/StyleConfiguration';
import RingModeStyleConfiguration from '../../../../../../../../features/ringmodecomponent/src/main/ets/com/ohos/common/StyleConfiguration';

const TAG = './styleconfigurationer';

export default class StyleManager {
  static readonly STANDARD_DISPLAY_WIDTH = 1280;
  static readonly STANDARD_DISPLAY_HEIGHT = 800;
  static maxWidth: number = StyleManager.STANDARD_DISPLAY_WIDTH;

  static setStyle(): void {
    Log.showDebug(TAG, 'setStyle');

    let config = AbilityManager.getAbilityData(AbilityManager.ABILITY_NAME_STATUS_BAR, 'config');
    StyleManager.maxWidth = config.maxWidth;

    StyleManager.setCommonStyle();
    StyleManager.setVerticalStatusBarItemLoadComponentStyle();
    StyleManager.setStatusBarNotificationIconStyle();
    StyleManager.setIconItemComponentStyle();
    StyleManager.setPluginIconItemComponentStyle();
    StyleManager.setClockComponentStyle();
    StyleManager.setBatteryComponentStyle();
    StyleManager.setBatteryPicStyle();
    StyleManager.setSignalComponentStyle();
    StyleManager.setStatusBarBluetoothComponentStyle();
    StyleManager.setStartsBarLocationComponentStyle();
    StyleManager.setStatusRingModeComponentStyle();
    StyleManager.setStartsBarWifiComponentStyle();
    StyleManager.setCapsuleComponentStyle();
  }
  // Common
  private static setCommonStyle(): void {
    let style = CommonStyleConfiguration.getCommonStyle();
    style.statusBarFontSize = StyleManager.calcScaleSizePx(14);
    style.statusBarIconWidth = StyleManager.calcScaleSizePx(20);
    style.statusBarIconHeight = StyleManager.calcScaleSizePx(20);
    style.statusBarMarginLeftRight = $r("app.float.status_bar_margin_left_right");
  }

  // StatusBar-VerticalStatusBarItemLoadComponent
  private static setVerticalStatusBarItemLoadComponentStyle(): void {
    let style = StatusBarStyleConfiguration.getVerticalStatusBarItemLoadComponentStyle();
    style.statusBarVerticalComponentHeight = StyleManager.calcScaleSize(64);
  }

  // StatusBar-NotificationIcon
  private static setStatusBarNotificationIconStyle(): void {
    let style = StatusBarStyleConfiguration.getStatusBarNotificationIconStyle();
    style.iconWidth = StyleManager.calcScaleSizePx(16);
    style.iconHeight = StyleManager.calcScaleSizePx(16);
    style.iconSpace = StyleManager.calcScaleSizePx(6);
  }

  // StatusBar-IconComponent
  private static setIconItemComponentStyle(): void {
    let style = StatusBarStyleConfiguration.getIconItemComponentStyle();
    style.stackHeight = StyleManager.calcScaleSizePx(8 * 2 + 18);
    style.stackPadding = StyleManager.calcScaleSizePx(8);
    style.stackBorderRadius = StyleManager.calcScaleSizePx(8);
    style.stackBgColorSelected = '#33000000';
    style.stackBgColorUnSelected = '#00000000';
    style.componentSpace = StyleManager.calcScaleSizePx(8);
    style.iconWidth = StyleManager.calcScaleSizePx(18);
    style.iconHeight = StyleManager.calcScaleSizePx(18);
    style.marginLeft = StyleManager.calcScaleSizePx(4);
    style.marginRight = StyleManager.calcScaleSizePx(4);
  }

  // StatusBar-PluginIconItemComponent
  private static setPluginIconItemComponentStyle(): void {
    let style = StatusBarStyleConfiguration.getPluginIconItemComponentStyle();
    style.iconWidth = StyleManager.calcScaleSize(32);
  }

  // Clock
  private static setClockComponentStyle(): void {
    let style = ClockStyleConfiguration.getClockComponentStyle();
    style.statusBarClockMaxWidth = StyleManager.calcScaleSizePx(37);
  }

  // Battery-Icon
  private static setBatteryComponentStyle(): void {
    let style = BatteryStyleConfiguration.getBatteryComponentStyle();
    style.componentGap = StyleManager.calcScaleSizePx(6);
  }

  // Battery-Pic
  private static setBatteryPicStyle(): void {
    let style = BatteryStyleConfiguration.getBatteryPicStyle();
    style.picGap = StyleManager.calcScaleSizePx(1);
    style.picBodyWidth = StyleManager.calcScaleSizePx(18.75);
    style.picBodyHeight = StyleManager.calcScaleSizePx(10.83);
    style.picBodyPadding = StyleManager.calcScaleSizePx(1);
    style.picBodyBorderWidth = StyleManager.calcScaleSizePx(1);
    style.picBorderRadius = StyleManager.calcScaleSizePx(2);
    style.picHeadBorderRadius = StyleManager.calcScaleSizePx(1);
    style.picChargingColor = '#00ff21';
    style.picLevelLowColor = '#ff0000';
    style.picHeadWidth = StyleManager.calcScaleSizePx(1.5);
    style.picHeadHeight = StyleManager.calcScaleSizePx(5);
  }

  // Signal-Icon
  private static setSignalComponentStyle(): void {
    let style = SignalStyleConfiguration.getSignalComponentStyle();
    style.cellularImageWidth = StyleManager.calcScaleSizePx(25);
    style.cellularImageHeight = StyleManager.calcScaleSizePx(20);
    style.statusBarSignalTypeFontSize = StyleManager.calcScaleSizePx(7);
    style.statusBarSignalUnknownFontSize = StyleManager.calcScaleSizePx(12);
    style.signalTextMaxWeight = StyleManager.calcScaleSizePx(100);
    style.netSignalTextMaxWidth = StyleManager.calcScaleSizePx(18);
  }

  // Bluetooth -Icon
  private static setStatusBarBluetoothComponentStyle(): void {
    let style = BluetoothStyleConfiguration.getStatusBarBluetoothComponentStyle();
    style.staticBarBluetoothWidth = StyleManager.calcScaleSizePx(18);
    style.staticBarBluetoothHeight = StyleManager.calcScaleSizePx(20);
  }

  // Location-Icon
  private static setStartsBarLocationComponentStyle(): void {
    let style = LocationStyleConfiguration.getStartsBarLocationComponentStyle();
    style.statusBarLocationWidth = StyleManager.calcScaleSizePx(18);
    style.statusBarLocationHeight = StyleManager.calcScaleSizePx(20);
  }

  // RingMode-Icon
  private static setStatusRingModeComponentStyle(): void {
    let style = RingModeStyleConfiguration.getStatusRingModeComponentStyle();
    style.statusBarRingModeWidth = StyleManager.calcScaleSizePx(20);
    style.statusBarRingModeHeight = StyleManager.calcScaleSizePx(20);
  }

  // Wifi-Icon
  private static setStartsBarWifiComponentStyle(): void {
    let style = WifiStyleConfiguration.getStartsBarWifiComponentStyle();
    style.statusBarWifiWidth = StyleManager.calcScaleSizePx(20);
    style.statusBarWifiHeight = StyleManager.calcScaleSizePx(20);
  }

  // Capsule-Icon
  private static setCapsuleComponentStyle(): void {
    let style = CapsuleStyleConfiguration.getCapsuleComponentStyle();
    style.greenCapsulePhoneWidth = StyleManager.calcScaleSizePx(15);
    style.greenCapsulePhoneHeight = StyleManager.calcScaleSizePx(15);
    style.greenCapsuleHeight = StyleManager.calcScaleSizePx(30);
    style.greenCapsuleTextColor = '#CCFFFFFF';
    style.greenCapsuleTextMarginLeftRight = StyleManager.calcScaleSizePx(10);
    style.greenCapsuleRadius = StyleManager.calcScaleSizePx(24);
    style.greenCapsuleBackgroundColor = '#64BB5C';
    style.maxLines = 1;
  }

  static number2px(n: number): string {
    return n.toString() + 'px';
  }

  static calcScaleSize(n: number): number {
    return n * StyleManager.maxWidth / StyleManager.STANDARD_DISPLAY_WIDTH;
  }

  static calcScaleSizePx(n: number): string {
    return StyleManager.number2px(StyleManager.calcScaleSize(n));
  }
}