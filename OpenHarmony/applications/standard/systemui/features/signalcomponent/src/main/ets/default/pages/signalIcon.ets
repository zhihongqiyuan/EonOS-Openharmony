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

import Constants from '../common/constants';
import mSignalModel from '../signalModel';
import Log from '../../../../../../../common/src/main/ets/default/Log'
import { TintContentInfo } from '../../../../../../../common/src/main/ets/default/TintStateManager'
import StyleConfigurationCommon, { CommonStyle
} from '../../../../../../../common/src/main/ets/default/StyleConfiguration'
import StyleConfiguration, { SignalComponentStyle } from '../common/StyleConfiguration'
import ViewModel from '../viewmodel/SignalVM'

const TAG = 'SignalComponent-SignalIcon'

@Component
export default struct SignalIcon {
  @StorageLink('cellularLevel') cellularLevel: number = Constants.CELLULAR_NO_SIM_CARD
  @StorageLink('cellularType') cellularType: number = Constants.RADIO_TECHNOLOGY_UNKNOWN
  @StorageLink('networkState') networkState: string = Constants.NET_NULL
  @StorageLink('signalObserved') signalObserved: boolean = false
  @StorageLink('Airplane_Status') airplaneStatus: boolean = false;
  @State mTintContentInfo: TintContentInfo = ViewModel.getTintContentInfo()
  @State styleCommon: CommonStyle = StyleConfigurationCommon.getCommonStyle()
  @State style: SignalComponentStyle = StyleConfiguration.getSignalComponentStyle()

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
    if (!this.signalObserved) {
      mSignalModel.initSignalModel();
      this.signalObserved = true;
    }
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  build() {
    Row() {
      if (!this.airplaneStatus) {
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
        Text(this.updateNetworkState(this.networkState))
          .fontSize(this.styleCommon.statusBarFontSize)
          .fontColor(this.mTintContentInfo.contentColor)
          .textOverflow({ overflow: TextOverflow.Ellipsis })
          .constraintSize({ maxWidth: this.style.signalTextMaxWeight })
          .fontWeight(FontWeight.Medium)
          .flexShrink(0)
          .maxLines(1)
          .textAlign(TextAlign.Center)
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')

        Stack({ alignContent: Alignment.TopStart }) {
          Text(this.updateCellularType(this.cellularType))
            .fontSize(this.cellularType == Constants.RADIO_TECHNOLOGY_UNKNOWN ? this.style.statusBarSignalUnknownFontSize : this.style.statusBarSignalTypeFontSize)
            .fontColor(this.mTintContentInfo.contentColor)
            .width(this.style.netSignalTextMaxWidth)
            .fontWeight(FontWeight.Bold)
            .textAlign(TextAlign.Center)
            .textAlign(TextAlign.Start)
            .margin({left:$r("app.float.signal_component_text_margin_left")})
          Image(this.updateCellularImage(this.cellularLevel))
            .objectFit(ImageFit.Contain)
            .width(this.style.cellularImageWidth)
            .height(this.style.cellularImageHeight)
            .fillColor(this.mTintContentInfo.contentColor)
        }.flexShrink(1)

        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
      }
    }
    .height('100%')
    .opacity($r("app.float.icon_component_opacity"))
  }

  /**
   * Get the string of cellular type
   *
   * @param {number} type - number of cellular type
   * @return {string} typeString type of cellular type
   */
  private updateCellularType(signalType: number): Resource {
    let typeString: Resource;
    switch (signalType) {
      case Constants.RADIO_TECHNOLOGY_UNKNOWN:
        typeString = $r('app.string.signal_null');
        break;
      case Constants.RADIO_TECHNOLOGY_GSM:
      case Constants.RADIO_TECHNOLOGY_1XRTT:
        typeString = $r('app.string.2G');
        break;
      case Constants.RADIO_TECHNOLOGY_WCDMA:
      case Constants.RADIO_TECHNOLOGY_HSPA:
      case Constants.RADIO_TECHNOLOGY_HSPAP:
      case Constants.RADIO_TECHNOLOGY_TD_SCDMA:
      case Constants.RADIO_TECHNOLOGY_EVDO:
      case Constants.RADIO_TECHNOLOGY_EHRPD:
        typeString = $r('app.string.3G');
        break;
      case Constants.RADIO_TECHNOLOGY_LTE:
      case Constants.RADIO_TECHNOLOGY_LTE_CA:
      case Constants.RADIO_TECHNOLOGY_IWLAN:
        typeString = $r('app.string.4G');
        break;
      case Constants.RADIO_TECHNOLOGY_NR:
        typeString = $r('app.string.5G');
        break;
      default:
        typeString = $r('app.string.signal_null');
        break;
    }
    return typeString;
  }

  /**
   * Get the cellular signal image
   *
   * @param {number} level - signal level from signalModel
   * @return {string} cellularImage image of cellular signal
   */
  private updateCellularImage(level: number) {
    let cellularImage: Resource;
    switch (level) {
      case Constants.CELLULAR_SIGNAL_NO:
        cellularImage = $r('app.media.ic_statusbar_signal_no');
        break;
      case Constants.CELLULAR_SIGNAL_MIN:
        cellularImage = $r('app.media.ic_statusbar_signal_1');
        break;
      case Constants.CELLULAR_SIGNAL_LOW:
        cellularImage = $r('app.media.ic_statusbar_signal_2');
        break;
      case Constants.CELLULAR_SIGNAL_HALF:
        cellularImage = $r('app.media.ic_statusbar_signal_3');
        break;
      case Constants.CELLULAR_SIGNAL_HIGH:
        cellularImage = $r('app.media.ic_statusbar_signal_4');
        break;
      case Constants.CELLULAR_SIGNAL_FULL:
        cellularImage = $r('app.media.ic_statusbar_signal_full');
        break;
      case Constants.CELLULAR_NO_SIM_CARD:
      default:
        cellularImage = $r('app.media.ic_statusbar_signal_no');
        break;
    }
    return cellularImage;
  }

  /**
   * Get the NetworkState signal name
   *
   * @param {string} netWorkState - network state from signal model
   * @return {string} vendor's name or signal state
   */
  private updateNetworkState(netWorkState: string) {
    let networkStateName: string | Resource;
    if (netWorkState == Constants.NET_NULL) {
      networkStateName = $r('app.string.net_null');
    } else {
      networkStateName = netWorkState;
    }
    return networkStateName;
  }
}