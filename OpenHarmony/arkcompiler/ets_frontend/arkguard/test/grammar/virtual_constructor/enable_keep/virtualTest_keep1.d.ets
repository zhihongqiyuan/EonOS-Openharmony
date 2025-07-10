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

import {ExportType} from './virtualTest_keep2'

export interface ExportInterface {
  param1: number,
  param2: number
}

interface ToplevelInterface {
  param3: string
}

export declare struct KeepStruct {
  structParam1: ExportType.typeParam1
  structParam2: ExportInterface
  structParam3: ToplevelInterface;
  build(): void;
}
