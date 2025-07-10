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

import CommonStyleManager from '../../../../../../../common/src/main/ets/default/CommonStyleManager';

const TAG = 'signal-StyleConfiguration';

export class SignalComponentStyle {
  cellularImageWidth: Length = $r('app.float.signal_component_icon_width');
  cellularImageHeight: Length = $r('app.float.signal_component_icon_height');
  statusBarSignalTypeFontSize: Length = $r('app.float.status_bar_signal_type_font_size');
  statusBarSignalUnknownFontSize: Length = $r('app.float.status_bar_signal_unknown_font_size');
  signalTextMaxWeight: Length = $r('app.float.signal_text_max_width');
  netSignalTextMaxWidth: Length = $r('app.float.status_bar_signal_net_signal_text_max_width');
}

export default class StyleConfiguration {
  static getSignalComponentStyle(): SignalComponentStyle {
    const key: string = TAG + '-SignalComponent';
    return CommonStyleManager.getStyle(key, SignalComponentStyle);
  }
}