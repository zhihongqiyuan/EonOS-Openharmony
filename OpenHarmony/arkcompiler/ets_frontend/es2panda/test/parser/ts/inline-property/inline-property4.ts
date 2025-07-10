/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

class D1 {
    constructor() {
    }
    prop = "str1";
    1 = "str2";
    "str_prop" = "str3";
    ["computed2"] = "str5";
    [2] = "str6";
}

const computed1: unique symbol = Symbol("symbol1");
const part1 = "dynamic";
const part2 = "property";

// 2
class D2 {
    constructor() {
        this[computed1] = "str4";  
        this["computed2"] = "str5";
        this[`${part1}_${part2}`] = "str6";
        this[1 + 2] = "str7";
    }
}