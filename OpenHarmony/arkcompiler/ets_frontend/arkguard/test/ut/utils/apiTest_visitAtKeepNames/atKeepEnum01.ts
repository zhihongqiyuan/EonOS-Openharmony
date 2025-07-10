/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

// @KeepSymbol
enum Color01 {
  RED01
}

// @KeepSymbol
export enum Color02 {
  RED02
}

enum Color03 {
  // @KeepSymbol
  RED03,
  BLUE03
}

export enum Color04 {
  RED04,
  // @KeepSymbol
  BLUE04
}

export enum Color05 {
  // @KeepSymbol
  'RED05',
  BLUE05
}