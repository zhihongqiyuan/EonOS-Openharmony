/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


export interface I1 { p: string }
export interface I2 { p: string }
export interface I3 { p: string }

export var v1: number = 1;
export var v2: number = 2;
var v3: number = 3;
export default v3;

interface I4 { p: string }
interface I5 { p: string }
interface II6 { p: string }

export type { I4 };
export { type I5, type II6 as I6 };
