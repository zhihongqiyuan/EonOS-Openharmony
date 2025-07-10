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

// test support ets
// @KeepSymbol
export class MyClass01 {
  property01: string;
}

// test KeepSymbol & KeepAsConsumer
// @KeepSymbol
// @KeepAsConsumer
const MyVar01 = 1;

// @KeepAsConsumer
// @KeepSymbol
const MyVar02 = 1;

// @KeepSymbol Do not support
const MyVar03 = 1;

// Do not support @KeepSymbol
const MyVar04 = 1;