/*
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

import * as crypto from 'crypto';

export class ListUtil {
  public static readonly MAX_INIT_LEN: number = 0xFFFF;

  /**
   * get a list with element number filled for each element
   * @param length: list length you want init.
   */
  public static getInitList(length: number): number[] {
    if (isNaN(length) || length < 0 || length > this.MAX_INIT_LEN) {
      console.error(`array init length is invalid, should in range: [0, ${this.MAX_INIT_LEN}]`);
      return [];
    }

    return Array(length).fill(null).map((_, h) => h);
  }

  /**
   * shuffle list
   * @param originList: list to be shuffled
   */
  public static shuffle(originList: number[]): void {
    if (!originList) {
      return;
    }

    for (let i = originList.length; i > 0; i--) {
      let j = crypto.randomInt(originList.length);
      [originList[i - 1], originList[j]] = [originList[j], originList[i - 1]];
    }
  }

  /**
   * merge two list to one list of unique element
   * @param listA
   * @param listB
   * @param listC
   */
  public static uniqueMergeList(listA: string[], listB: string[], listC?: string[]): string[] {
    const firstList: string[] = listA ? listA : [];
    const secondList: string[] = listB ? listB : [];
    const thirdList: string[] = listC ? listC : [];

    const tmpList: string[] = [...firstList, ...secondList, ...thirdList];
    const elementSet: Set<string> = new Set<string>(tmpList);
    return Array.from(elementSet);
  }
}
