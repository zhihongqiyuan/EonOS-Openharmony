/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
import StringUtils from '../../../utils/StringUtils';
/**
 * 处理丢帧
 * @param fullFps
 */
let TAG = 'HandleLostFrame';

export default class HandleLostFrame {
  private mFullFps: number;
  private mJankArray: Array<number> = Array(21);

  constructor(fullFps: number) {
    this.mFullFps = fullFps;
    let frameTime = 1000 / this.mFullFps;

    for (let i = 1; i < 22; i++) {
      this.mJankArray[i - 1] = (frameTime * i + frameTime * (i + 1)) / 2;
    }
  }

  getJankMap(jankSrcStr: String): Map<number, number> {
    if (jankSrcStr === '' || jankSrcStr === undefined) {
      return new Map();
    }
    let allDrawFrame: string[] = jankSrcStr.split(',');
    let jitters = Array<number>(allDrawFrame.length);

    for (let i = 0; i < allDrawFrame.length; i++) {
      console.log(TAG + 'allDrawFrame[i]' + i + allDrawFrame[i]);
      if (!isNaN(StringUtils.s2L(allDrawFrame[i]))) {
        jitters[i] = StringUtils.s2L(allDrawFrame[i]);
      }
    }
    let jankCountMap: Map<number, number> = new Map();

    for (let i of jitters.keys()) {
      let doubleJank = jitters[i] / 1e6;
      console.log(TAG + 'for jitters[' + i + ']' + doubleJank);
      let jankRange = 0;
      jankRange = this.getJankRange(2, doubleJank);
      console.log(TAG + 'for jankRange' + jankRange);
      if (jankRange !== 0) {
        console.log(
          TAG + 'for jitters[' + jankRange + ']' + jankCountMap.get(jankRange)
        );
        if (jankCountMap.get(jankRange) == null) {
          jankCountMap.set(jankRange, 1);
        } else {
          jankCountMap.set(jankRange, jankCountMap.get(jankRange) + 1);
        }
      }
    }
    for (let i = 2; i < 22; i++) {
      if (!jankCountMap.has(i)) {
        jankCountMap.set(i, 0);
      }
    }
    return jankCountMap;
  }

  private getJankRange(currRange: number, jank: number): number {
    if (currRange > 22) {
      return 0;
    }
    if (currRange === 2) {
      if (jank < this.mJankArray[currRange - 2]) {
        return 0;
      }
    }
    if (currRange === 22) {
      if (jank >= this.mJankArray[currRange - 2]) {
        return currRange;
      } else {
        return 0;
      }
    }
    if (
      jank >= this.mJankArray[currRange - 2] &&
      jank < this.mJankArray[currRange - 1]
    ) {
      return currRange;
    } else {
      return this.getJankRange(currRange + 1, jank);
    }
  }
}
