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

let number = {
    2e3 : 'scientific notation',
    1000 : 'decimal',
    NaN : NaN,
    0.0 : '0',
    Infinity : 'Infinity',
    1e-6 : 0.000001,
    0.000002 : 0.000002,
    1e-7 : 1e-7,
    0.0000002 : 2e-7,
    0.1 : 0.1,
    1.1 : 1.1,
    1234567890123456 : 1234567890123456,
    12345678901234567 : 12345678901234568,
    123456789012345678 : 123456789012345680,
    1e20 : 100000000000000000000,
    200000000000000000000 : 100000000000000000000,
    1e21 : 1e+21,
    2000000000000000000000 : 2e+21,
}

print(number[2e3]); // call key 2e3
print(number[2000]); // call key 2e3
print(number[1000]);
// Special testcases
print(number[NaN]);
print(number[0]); // equal to 0.0
print(number[Infinity]);
// Boundary value of size : 1e-6
print(number[1e-6]);
print(number[0.000001]);
print(number[0.000002]);
print(number[1e-7]);
print(number[0.0000001]);
print(number[0.0000002]);
// Boundary value of size : 1e0
print(number[0.1]);
print(number[1.1]);
// Boundary value of size : 1e21
print(number[1e20]);
print(number[100000000000000000000]);
print(number[200000000000000000000]);
print(number[1e21]);
print(number[1000000000000000000000]);
print(number[2000000000000000000000]);
// Precision digit : 17
print(number[1234567890123456]);
print(number[12345678901234567]);
print(number[12345678901234568]); // equal to 12345678901234567
print(number[123456789012345678]);
print(number[123456789012345680]); // equal to 123456789012345678
