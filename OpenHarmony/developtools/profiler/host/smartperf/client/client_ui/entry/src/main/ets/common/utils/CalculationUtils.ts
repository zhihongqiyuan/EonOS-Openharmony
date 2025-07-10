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

/**
 * 计算工具类
 * @param fpsList
 * @param gameFPS
 */
export default class CalculationUtils {
  public TAG: String = 'CalculationTest';
  public size: number;
  public gameFPS: number;
  //所有fps的集合
  public fpsList: Array<number> = [];
  //最低帧数
  public point: number;
  //低于point帧数的数量
  public count: number = 0;
  //所有帧数总和
  private fpsSum: number;

  constructor(fpsList: Array<number>, gameFPS: number) {
    let removeList: Array<number> = new Array();
    for (let i = 0; i < fpsList.length; i++) {
      if (i < 0) {
        removeList.push(i);
      }
    }
    fpsList.filter((item) => {
      return removeList.indexOf(item) < 0;
    });
    this.fpsList = fpsList;
    this.size = fpsList.length;
    this.gameFPS = gameFPS;
    this.point =
      gameFPS === 30 ? 25 : gameFPS === 40 ? 30 : gameFPS === 60 ? 45 : 10;
  }

  //抖动率计算
  Jitter_rate(): number {
    let jitterRite = 0;
    let size = this.fpsList.length;

    let numOfDiff3To = 0;
    let numOfDiff5To = 0;
    let numOfDiff8To = 0;
    let numOfDiff12To = 0;
    let numOfDiff16To = 0;

    let numOfDiff3To5 = 0;
    let numOfDiff5To8 = 0;
    let numOfDiff8To12 = 0;
    let numOfDiff6To10 = 0;
    let numOfDiff10To16 = 0;

    for (let i = 1; i < size; i++) {
      let diff = Math.abs(this.fpsList[i] - this.fpsList[i - 1]);
      if (diff > 3) {
        numOfDiff3To++;
      }
      if (diff > 5) {
        numOfDiff5To++;
      }
      if (diff > 8) {
        numOfDiff8To++;
      }
      if (diff > 12) {
        numOfDiff12To++;
      }
      if (diff > 16) {
        numOfDiff16To++;
      }
      if (diff > 3 && diff <= 5) {
        numOfDiff3To5++;
      }
      if (diff > 5 && diff <= 8) {
        numOfDiff5To8++;
      }
      if (diff > 8 && diff <= 12) {
        numOfDiff8To12++;
      }
      if (diff > 6 && diff <= 10) {
        numOfDiff6To10++;
      }
      if (diff > 10 && diff <= 16) {
        numOfDiff10To16++;
      }
    }

    let percentOf3To = 0;
    let percentOf5To = 0;
    let percentOf8To = 0;
    let percentOf12To = 0;
    let percentOf16To = 0;

    let percentOf3To5 = 0;
    let percentOf5To8 = 0;
    let percentOf8To12 = 0;
    let percentOf6To10 = 0;
    let percentOf10To16 = 0;

    if (size !== 1) {
      percentOf3To = numOfDiff3To / size;
      percentOf5To = numOfDiff5To / size;
      percentOf8To = numOfDiff8To / size;
      percentOf12To = numOfDiff12To / size;
      percentOf16To = numOfDiff16To / size;

      percentOf3To5 = numOfDiff3To5 / size;
      percentOf5To8 = numOfDiff5To8 / size;
      percentOf8To12 = numOfDiff8To12 / size;
      percentOf6To10 = numOfDiff6To10 / size;
      percentOf10To16 = numOfDiff10To16 / size;
    }
    if (this.gameFPS === 25 || this.gameFPS === 30) {
      jitterRite = percentOf3To * 100;
    }
    if (this.gameFPS === 40 || this.gameFPS === 45) {
      jitterRite = (percentOf3To5 * 0.4 + percentOf5To * 0.6) * 100;
    }
    if (this.gameFPS === 60) {
      jitterRite =
        (percentOf3To5 * 0.2 + percentOf5To8 * 0.3 + percentOf8To * 0.5) * 100;
    }
    if (this.gameFPS === 90) {
      jitterRite =
        (percentOf5To8 * 0.2 + percentOf8To12 * 0.3 + percentOf12To * 0.5) *
        100;
    }
    if (this.gameFPS === 120) {
      jitterRite =
        (percentOf6To10 * 0.2 + percentOf10To16 * 0.3 + percentOf16To * 0.5) *
        100;
    }
    return jitterRite;
  }

  //低帧率计算
  Low_Frame_Rate(): number {
    for (let i = 0; i < this.fpsList.length; i++) {
      if (this.fpsList[i] < this.point) {
        this.count++;
      }
    }
    return (this.count / this.size) * 100;
  }

  //卡顿次数 100ms
  calculateCaton(fpsJitterList: Array<string>): number {
    let num = 0;
    for (let i = 0; i < fpsJitterList.length; i++) {
      let jitter = fpsJitterList[i].split(';;');
      for (let j = 0; j < jitter.length; j++) {
        let n = Number(jitter[j]);
        if (!isNaN(n) && n / 1e6 > 100) {
          num++;
        }
      }
    }
    return num;
  }

  //满帧计算
  static calculateFPSNew(fpsList: Array<number>): number {
    let FPS = 30;
    let moreThan34Count = 0;
    let moreThan44Count = 0;
    let moreThan70Count = 0;
    let moreThan100Count = 0;
    for (let i in fpsList) {
      if (fpsList[i] >= 100) {
        moreThan100Count++;
      } else if (fpsList[i] >= 70) {
        moreThan70Count++;
      } else if (fpsList[i] >= 44) {
        moreThan44Count++;
      } else if (fpsList[i] >= 34) {
        moreThan34Count++;
      }
    }
    if (moreThan100Count >= 1) {
      FPS = 120;
    } else if (moreThan70Count >= 1) {
      FPS = 90;
    } else if (moreThan44Count >= 1) {
      FPS = 60;
    } else if (moreThan34Count >= 1) {
      FPS = 45;
    }
    return FPS;
  }
}
