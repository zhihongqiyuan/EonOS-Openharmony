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

import AbilityManager from '../../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import display from '@ohos.display'
import Log from '../../../../../../../../common/src/main/ets/default/Log'
import ResourceUtil from '../../../../../../../../common/src/main/ets/default/ResourceUtil'
import CommonStyleConfiguration from '../../../../../../../../common/src/main/ets/default/StyleConfiguration'

const TAG = 'StatusBarConfiguration';

var statusbarPosition;
var shortSideLength = '0';
var direction = -1;

var maxWidth;
var maxHeight;
var minHeight;
var realWidth;
var realHeight;
var xCoordinate = 0;
var yCoordinate = 0;

enum Position {
  NOT_CONFIGURED,
  LEFT_POSITION,
  TOP_POSITION,
  RIGHT_POSITION,
  BOTTOM_POSITION
}

/**
 * Get window size.
 */
class StatusBarConfiguration {
  async initStatusBarConfiguration() {
    Log.showInfo(TAG, 'initWindowManager');
    minHeight = 0;

    await display.getDefaultDisplay()
      .then(dis => {
        Log.showInfo(TAG, `initWindowManager dis ${JSON.stringify(dis)}`);
        maxWidth = dis.width;
        maxHeight = dis.height;
        Log.showInfo(TAG, `initWindowManager maxWidth ${maxWidth} maxHeight ${maxHeight} minHeight ${minHeight}`);
      })
      .catch((err) => {
        Log.showError(TAG, `getDefaultDisplay err:${JSON.stringify(err)}`);
      });
  }

  async getDirectionAndPosition() {
    Log.showInfo(TAG, 'getDirectionAndPosition');
    await ResourceUtil.initResourceManager(AbilityManager.ABILITY_NAME_STATUS_BAR);
    if (maxWidth > maxHeight) {
      direction = 1;
    } else {
      direction = 2;
    }
    let style: any = CommonStyleConfiguration.getCommonStyle()
    let deviceTypeInfo = style.deviceTypeInfo
    if (direction == -1) {
      Log.showInfo(TAG, 'direction is -1');
      statusbarPosition = await ResourceUtil.getString($r("app.string.status_bar_position_landscape"))
      shortSideLength = deviceTypeInfo == 'phone' ? await ResourceUtil.getString($r("app.string.phone_status_bar_size_landscape"))
                                                  : await ResourceUtil.getString($r("app.string.status_bar_size_landscape"));
    } else if (direction == 1) {
      Log.showInfo(TAG, 'direction is 1');
      statusbarPosition = await ResourceUtil.getString($r("app.string.status_bar_position_landscape"))
      shortSideLength = deviceTypeInfo == 'phone' ? await ResourceUtil.getString($r("app.string.phone_status_bar_size_landscape"))
                                                  : await ResourceUtil.getString($r("app.string.status_bar_size_landscape"));
    } else {
      Log.showInfo(TAG, 'direction is 2');
      statusbarPosition = await ResourceUtil.getString($r("app.string.status_bar_position_portrait"))
      shortSideLength = deviceTypeInfo == 'phone' ? await ResourceUtil.getString($r("app.string.phone_status_bar_size_portrait"))
                                                  : await ResourceUtil.getString($r("app.string.status_bar_size_portrait"));
    }
    shortSideLength = parseInt(shortSideLength) + '';
    Log.showInfo(TAG, 'direction = ' + direction + 'statusbarPosition = ' + statusbarPosition +
    'shortSideLength = ' + shortSideLength);
  }

/**
   * Get status bar configuration
   */
  public async getConfiguration() {
    await this.initStatusBarConfiguration();
    await this.getDirectionAndPosition();
    let showHorizontal = false;
    let ableToMaximize = false;
    if (statusbarPosition == Position.TOP_POSITION || statusbarPosition == Position.BOTTOM_POSITION) {
      Log.showInfo(TAG, `getConfiguration`);
      showHorizontal = true;
      minHeight = parseInt(shortSideLength);
      realWidth = maxWidth;
      realHeight = parseInt(shortSideLength);
      if (statusbarPosition == Position.BOTTOM_POSITION) {
        yCoordinate = parseInt(maxHeight) - parseInt(shortSideLength);
      }
      ableToMaximize = true;
    } else if (statusbarPosition == Position.LEFT_POSITION || statusbarPosition == Position.RIGHT_POSITION) {
      showHorizontal = false;
      ableToMaximize = false;
      minHeight = parseInt(shortSideLength);
      realWidth = parseInt(shortSideLength);
      realHeight = maxHeight;
      if (statusbarPosition == Position.RIGHT_POSITION) {
        xCoordinate = parseInt(maxWidth) - parseInt(shortSideLength);
      }
    } else {
      realWidth = 0;
      realHeight = 0;
    }
    Log.showInfo(TAG, `initWindowManager xCoordinate ${xCoordinate} yCoordinate ${yCoordinate} realWidth ${realWidth} realHeight ${realHeight}`);

    var configuration = {
      maxWidth: maxWidth,
      maxHeight: maxHeight,
      minHeight: minHeight,
      showHorizontal: showHorizontal,
      ableToMaximize: ableToMaximize,
      direction:direction,
      realWidth: realWidth,
      realHeight: realHeight,
      xCoordinate: xCoordinate,
      yCoordinate: yCoordinate
    }
    return configuration;
  }
}

let statusBarConfiguration = new StatusBarConfiguration();

export default statusBarConfiguration;