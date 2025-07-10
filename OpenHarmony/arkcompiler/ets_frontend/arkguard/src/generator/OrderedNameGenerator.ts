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

import type {INameGenerator, NameGeneratorOptions} from './INameGenerator';

/**
 * @desc simple ordered name generator, e.g.: a, b, c, d, ..., a1, b1, c1, ...
 */
export class OrderedNameGenerator implements INameGenerator {
  private mCharIndex: number;
  private mLoopNumber: number;
  private mReservedNames: Set<string>;

  private readonly CHAR_COUNT: number = 26;
  private readonly CHAR_CODE_A: number = 97;

  constructor(options?: NameGeneratorOptions) {
    this.mCharIndex = 0;
    this.mLoopNumber = 0;
    this.mReservedNames = options?.reservedNames;
  }

  private updateElements(): void {
    this.mCharIndex = (this.mCharIndex + 1) % this.CHAR_COUNT;
    if (this.mCharIndex === 0) {
      this.mLoopNumber += 1;
    }
  }

  public getName(): string {
    let generatedName: string = String.fromCharCode(this.CHAR_CODE_A + this.mCharIndex);
    if (this.mLoopNumber > 0) {
      generatedName += this.mLoopNumber;
    }

    this.updateElements();
    if (this.mReservedNames?.has(generatedName)) {
      return this.getName();
    }

    return generatedName;
  }

  public reset(): void {
    this.mCharIndex = 0;
    this.mLoopNumber = 0;
  }
}
