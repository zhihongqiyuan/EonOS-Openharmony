/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import common from '@ohos.app.ability.common';

export class TimeUtils {
  getFreezingTimeNm(freezingMillisecond: number, context: common.ExtensionContext): string {
    const HOUR_NM = context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hour').id);
    const MINUTE_NM = context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_minutes').id);
    const SECOND_NM = context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_seconds').id);
    const ONE_MINUTE = 60;
    const RATE = 1000;
    let minute = Math.floor(freezingMillisecond / (ONE_MINUTE * RATE));
    let second = Math.round((freezingMillisecond % (ONE_MINUTE * RATE)) / RATE);
    let timeName = '';
    if (minute > 1) {
      let hour = minute / ONE_MINUTE;
      let minutes = minute % ONE_MINUTE + 1;
      if (hour > 1) {
        timeName += hour + HOUR_NM;
      } else {
        timeName += minutes + MINUTE_NM;
      }
    } else {
      timeName += second + SECOND_NM;
    }
    return timeName;
  }
}

let mTimeUtils = new TimeUtils();

export default mTimeUtils as TimeUtils;
