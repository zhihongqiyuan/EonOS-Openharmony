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

// export
export let export1 = 1;
export class MyExport {
    exportedProp = 1;
}
export let usb = 3; // lang

// string property
class MyString {
    'string1' = 1;
    'string2' = 2;
    'unit' = 3; // lang
}

// toplevel
function toplevel1(): number { // conf
    return 1;
}
function toplevel2(): number {
    return 2;
}

// property
class MyClass {
    property1 = 1; // conf
    exportedProp = 1;
    a = 2; // lang
    abcde(){}; // conf
    string1 = '1';
}

// local variable and parameter
function add(param1: number, param2: number): number { // lang
    let variable1 = param1;
    let variable2 = param2;
    return variable1 + variable2;
}

function myfunc(input: number): number {
    return input + 1;
}
