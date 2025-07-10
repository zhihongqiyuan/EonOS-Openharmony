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
import AbilityManager from '../../../../../../../../common/src/main/ets/default/abilitymanager/abilitymanager';
import IndexStyleConfiguration from './styleconfiguration';

const TAG = 'NotificationPanel-StyleManager';

export default class StyleManager {
  static readonly STANDARD_DISPLAY_WIDTH = 1280;
  static readonly STANDARD_DISPLAY_HEIGHT = 800;
  static maxWidth: number = StyleManager.STANDARD_DISPLAY_WIDTH;

  static setStyle(): void {
    Log.showDebug(TAG, 'setStyle');

    let dis = AbilityManager.getAbilityData(AbilityManager.ABILITY_NAME_NOTIFICATION_PANEL, 'dis');
    StyleManager.maxWidth = dis.width;

    // Index
    {
      let style = IndexStyleConfiguration.getIndexStyle();
      style.borderRadius = StyleManager.calcScaleSizePx(24);
    }

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