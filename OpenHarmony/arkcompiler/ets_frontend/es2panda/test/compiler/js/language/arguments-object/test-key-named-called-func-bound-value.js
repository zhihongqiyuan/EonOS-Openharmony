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

var obj = {
    2e3: function () {
        "show source";
        return "Successfully called key 2e3";
    },
    1000: function () {
        "show source";
        return "Successfully called key 1000";
    },
    NaN: function () {
        "show source";
        return "Successfully called key NaN";
    },
    0.0: function () {
        "show source";
        return "Successfully called key 0.0";
    },
    Infinity: function () {
        "show source";
        return "Successfully called key Infinity";
    },
    1e-6: function () {
        "show source";
        return "Successfully called key 1e-6";
    },
    0.000002: function () {
        "show source";
        return "Successfully called key 0.000002";
    },
    1e-7: function () {
        "show source";
        return "Successfully called key 1e-7";
    },
    0.0000002: function () {
        "show source";
        return "Successfully called key 0.0000002";
    },
    0.1: function () {
        "show source";
        return "Successfully called key 0.1";
    },
    1.1: function () {
        "show source";
        return "Successfully called key 1.1";
    },
    1e20: function () {
        "show source";
        return "Successfully called key 1e20";
    },
    200000000000000000000: function () {
        "show source";
        return "Successfully called key 200000000000000000000";
    },
    1e21: function () {
        "show source";
        return "Successfully called key 1e21";
    },
    2000000000000000000000: function () {
        "show source";
        return "Successfully called key 2000000000000000000000";
    },
    1234567890123456: function () {
        "show source";
        return "Successfully called key 1234567890123456";
    },
    12345678901234567: function () {
        "show source";
        return "Successfully called key 12345678901234567";
    },
    12345678901234567: function () {
        "show source";
        return "Successfully called key 12345678901234567";
    },
    123456789012345678: function () {
        "show source";
        return "Successfully called key 123456789012345678";
    },
    123456789012345678: function () {
        "show source";
        return "Successfully called key 123456789012345678";
    },
};

print(obj[2e3]()); // call key 2e3
print(obj[2000]()); // call key 2e3
print(obj[1000]());
// Special testcases
print(obj[NaN]());
print(obj[0]()); // 0.0 == 0
print(obj[Infinity]());
// Boundary value of size : 1e-6
print(obj[1e-6]());
print(obj[0.000001]());
print(obj[0.000002]());
print(obj[1e-7]());
print(obj[0.0000001]());
print(obj[0.0000002]());
// Boundary value of size : 1e0
print(obj[0.1]());
print(obj[1.1]());
// Boundary value of size : 1e21
print(obj[1e20]());
print(obj[100000000000000000000]());
print(obj[200000000000000000000]());
print(obj[1e21]());
print(obj[1000000000000000000000]());
print(obj[2000000000000000000000]());
// Precision digit : 17
print(obj[1234567890123456]());
print(obj[12345678901234567]());
print(obj[12345678901234568]()); // equal to 12345678901234567
print(obj[123456789012345678]());
print(obj[123456789012345680]()); // equal to 123456789012345678
