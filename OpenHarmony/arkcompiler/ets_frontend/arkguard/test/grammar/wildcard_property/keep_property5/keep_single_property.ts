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

class KeepProperty5 {
    prop: string = '1';
    shouldbeobf: number = 1;
    a: number = 1; // keep
    b: number = 2; // keep
    'c': number = 3; // keep
    '?': number = 4; // keep
    '*': number = 5; // keep
    '/': number = 6;  // keep
    '**': number = 7;

    functionobf() {
        return `function name should be obfuscated.`;
    }

    // keep
    g() {
        return `function name should be kept.`;
    }
}