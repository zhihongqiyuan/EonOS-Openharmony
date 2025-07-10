/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

import {TestSize} from './virtualTest_nokeep2';

export interface WinSize {
  sizeParam1: number,
  sizeParam2: number
}

interface TestType {
  prop1: number,
  prop2: string
}

export declare struct ShareControlPage {
  ctrlMenuSize: TestSize.sizeParam4;
  windowSize: WinSize;
  parameter: TestType;
  build(): void;
}