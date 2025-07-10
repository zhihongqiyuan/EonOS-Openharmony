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

/*
 * 计算工具类
 */
export default class GameUtils {
  // 30,40,60 连续丢帧>=3帧 为一般卡顿
  private static GENERAL_CATON_FLAG_LOW: number = 3;
  // 30,40,60 连续丢帧>=6帧 为严重卡顿
  private static CRITICAL_CATON_FLAG_LOW: number = 6;
  // 30,40,60 连续丢帧>=9帧 为致命卡顿
  private static FATAL_CATON_FLAG_LOW: number = 9;
  // 90,120 连续丢帧>=4帧 为一般卡顿
  private static GENERAL_CATON_FLAG_HIGHT: number = 4;
  // 90,120 连续丢帧>=8帧 为严重卡顿
  private static CRITICAL_CATON_FLAG_HIGHT: number = 8;
  // 90,120 连续丢帧>=12帧 为致命卡顿
  private static FATAL_CATON_FLAG_HIGHT: number = 12;

  // 连续丢帧数组长度
  private static JANK_SIZE: number = 21;
  private static JANK_RANGE_ARRAY_120 = new Array<number>(GameUtils.JANK_SIZE);
  private static JANK_RANGE_ARRAY_90 = new Array<number>(GameUtils.JANK_SIZE);
  private static JANK_RANGE_ARRAY_60 = new Array<number>(GameUtils.JANK_SIZE);
  private static JANK_RANGE_ARRAY_40 = new Array<number>(GameUtils.JANK_SIZE);
  private static JANK_RANGE_ARRAY_30 = new Array<number>(GameUtils.JANK_SIZE);
  private constructor() {}

  /**
   * 方法描述 计算满帧
   *
   * @param maxFps maxFps
   * @return java.lang.Integer get full fps
   */
  static getFullFps(maxFps: number): number {
    /*关于满帧判定，把所有帧率做一次遍历:
     * 1、只要有>=100帧，则判断为120帧
     * 2、否则只要有>=70 且所有帧都小于100，则判断为90帧
     * 1、否则只要有>=44 并且 所有帧都小于70的情况，就按照满帧60帧
     * 2、只要有>=34，且所有帧率均小于44的，就按照满帧40帧
     * 3、如果所有帧率均小于34帧的，就按照满帧30
     */

    if (maxFps >= 100) {
      return 120;
    } else if (maxFps >= 70) {
      return 90;
    } else if (maxFps >= 44) {
      return 60;
    } else if (maxFps >= 34) {
      return 40;
    }
    return 30;
  }

  /**
   * 获得低帧率的计算标准
   */
  private static getBaseLowFPS(gameFPS: number): number {
    if (gameFPS >= 60) {
      return 45;
    } else if (gameFPS == 40) {
      return 30;
    } else if (gameFPS == 30) {
      return 25;
    }
    return 20; // 25时候返回20
  }

  /**
   * 方法描述 计算帧率中位数
   *
   * @param fpsList fpsList
   * @return java.lang.Integer fps
   */
  private static getMedianFrameRadio(fpsArray: Array<number>): number {
    if (null == fpsArray || fpsArray.length == 0) {
      return null;
    }

    fpsArray.sort();

    let length = fpsArray.length;

    if (length % 2 == 0) {
      return (fpsArray[fpsArray.length / 2] + fpsArray[fpsArray.length / 2 - 1]) / 2; // 偶数个取中间两个数的平均数
    } else {
      return fpsArray[fpsArray.length / 2]; // 奇数个取最中间那个数
    }
  }

  /**
   * 判断电池容量/平均电流等数据是否有效
   * @param value
   */
  public static isFloatDataValid(value: number): boolean {
    if (null == value || value < 0 || Math.abs(value - 0) < 0.001) {
      return false;
    }
    return true;
  }

  /**
   * 方法描述 计算kpi抖动率的得分(y=100*EXP(-0.458*x))
   *
   * @param param param
   * @return java.lang.Integer 得分
   */
  private static getFpsJitterScore(param: number): number {
    if (null == param) {
      return null;
    }
    return Math.round(100.0 * Math.exp(-0.458 * 100 * param));
  }

  public static createJankStr(jankSrcStr: string, fullFPS: number, jankMap: Map<string, number>): string {
    if (null == jankSrcStr || jankSrcStr == '') {
      return 'NA';
    }

    // 初始化连续丢帧数组
    GameUtils.initJankArray(fullFPS);

    let allDrawFrame: string[] = jankSrcStr.split(',');
    let jitters = new Array<number>(allDrawFrame.length);
    for (let i = 0; i < allDrawFrame.length; i++) {
      try {
        jitters[i] = parseInt(allDrawFrame[i]);
      } catch (e) {
        e.printStackTrace();
      }
    }

    let jitterStr = 'NA';
    let jankCountMap = new Map<number, number>();
    jitters.forEach((jank) => {
      let doubleJank = jank / 1e6;
      let jankRange = 0;
      if (fullFPS == 120) {
        jankRange = GameUtils.getJankRange(GameUtils.JANK_RANGE_ARRAY_120, 2, doubleJank);
      } else if (fullFPS == 90) {
        jankRange = GameUtils.getJankRange(GameUtils.JANK_RANGE_ARRAY_90, 2, doubleJank);
      } else if (fullFPS == 60) {
        jankRange = GameUtils.getJankRange(GameUtils.JANK_RANGE_ARRAY_60, 2, doubleJank);
      } else if (fullFPS == 40) {
        jankRange = GameUtils.getJankRange(GameUtils.JANK_RANGE_ARRAY_40, 2, doubleJank);
      } else {
        jankRange = GameUtils.getJankRange(GameUtils.JANK_RANGE_ARRAY_30, 2, doubleJank);
      }

      if (jankRange != 0) {
        if (jankCountMap.get(jankRange) == null) {
          jankCountMap.set(jankRange, 1);
        } else {
          jankCountMap.set(jankRange, jankCountMap.get(jankRange) + 1);
        }
      }
    });

    for (let j = 2; j <= 22; j++) {
      if (!jankCountMap.has(j)) {
        jankCountMap.set(j, 0);
      }
    }

    let jitterBuilder = [];

    let allKeysIterator = jankCountMap.keys();

    for (var i = 0; i < jankCountMap.size; i++) {
      let key: number = allKeysIterator.next().value;
      let jankKey = key == 22 ? '>20' : String(key - 1);
      jitterBuilder
        .concat(jankKey)
        .concat(':')
        .concat(String(jankCountMap.get(key)))
        .concat(';');
      jankMap.set(jankKey, jankCountMap.get(key));
    }

    jitterStr = jitterBuilder.slice(0, jitterBuilder.length);
    return jitterStr;
  }

  private static initJankArray(fullFps: number) {
    switch (fullFps) {
      case 120:
        GameUtils.calJankArray(GameUtils.JANK_RANGE_ARRAY_120, 8.333);
        break;
      case 90:
        GameUtils.calJankArray(GameUtils.JANK_RANGE_ARRAY_90, 11.1111);
        break;
      case 60:
        GameUtils.calJankArray(GameUtils.JANK_RANGE_ARRAY_60, 16.667);
        break;
      case 40:
        GameUtils.calJankArray(GameUtils.JANK_RANGE_ARRAY_40, 25.0);
        break;
      case 30:
        GameUtils.calJankArray(GameUtils.JANK_RANGE_ARRAY_30, 33.333);
        break;
      default:
        break;
    }
  }

  /**
   * 方法描述
   *
   * @param jankRangeArray 连续丢帧数组
   * @param defaultJankTime 默认单帧时间
   */
  private static calJankArray(jankRangeArray: Array<number>, defaultJankTime: number) {
    if (jankRangeArray[0] < 0.01) {
      for (let i = 1; i <= GameUtils.JANK_SIZE; i++) {
        jankRangeArray[i - 1] = (defaultJankTime * i + defaultJankTime * (i + 1)) / 2;
      }
    }
  }

  private static getJankRange(jankRangeArray: Array<number>, currRange: number, jank: number): number {
    if (currRange > 22) {
      return 0;
    }
    if (currRange == 2) {
      if (jank < jankRangeArray[currRange - 2]) {
        return 0;
      }
    }
    if (currRange == 22) {
      if (jank >= jankRangeArray[currRange - 2]) {
        return currRange;
      } else {
        return 0;
      }
    }
    if (jank >= jankRangeArray[currRange - 2] && jank < jankRangeArray[currRange - 1]) {
      return currRange;
    } else {
      return GameUtils.getJankRange(jankRangeArray, currRange + 1, jank);
    }
  }
}
