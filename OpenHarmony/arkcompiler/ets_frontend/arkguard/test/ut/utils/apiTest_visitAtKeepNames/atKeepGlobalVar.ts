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
let globalVar01 = 1;

// @KeepSymbol
const globalFunc01 = function(): void{};

// @KeepSymbol
const globalMyClass01 = class{};

// @KeepSymbol
export let globalVar02 = 1;

// @KeepSymbol
export const globalFunc02 = function(): void{};

// @KeepSymbol
export const globalMyClass02 = class{};

// @KeepAsConsumer
let globalVar03 = 1;

// @KeepAsConsumer
const globalFunc03 = function(): void{};

// @KeepAsConsumer
const globalMyClass03 = class{};

// @KeepAsConsumer
export let globalVar04 = 1;

// @KeepAsConsumer
export const globalFunc04 = function(): void{};

// @KeepAsConsumer
export const globalMyClass04 = class{};

let globalVar05 = 1;

export let globalVar06 = 1;