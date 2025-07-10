/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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


const computed1: unique symbol = Symbol("symbol1");

class A {
    prop = "str1";
    1 = "str2";
    "str_prop" = "str3";
    [computed1] = "str4";
    ["computed2"] = "str5";
    [2] = "str6";
    static sprop = "s_str1";
    static 1 = "s_str2";
    static "s_str_prop" = "s_str3";
    static [computed1] = "s_str4";
    static ["computed3"] = "s_str5";
    static [3] = "s_str6";
    static "str_prop" = "s_str7";
}

var obj = new A();
print(obj.prop);
print(obj[1]);
print(obj["str_prop"]);
print(obj[computed1]);
print(obj["computed2"]);
print(obj[2]);

print(A.sprop);
print(A[1]);
print(A["s_str_prop"]);
print(A[computed1]);
print(A["computed3"]);
print(A[3]);
print(A["str_prop"]);
