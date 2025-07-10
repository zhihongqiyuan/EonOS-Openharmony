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


import { type I1 } from './test-ts-export';
import { type I2, v1 } from './test-ts-export';
import { type I3 as I, v2 } from './test-ts-export';
import v3, { type I4, type I5 } from './test-ts-export';
import type { I6 } from './test-ts-export';

print(v1);
print(v2);
print(v3);

var a1: I1 = { p: "a1" };
var a2: I2 = { p: "a2" };
var a3: I = { p: "a3" };
var a4: I4 = { p: "a4" };
var a5: I5 = { p: "a5" };
var a6: I6 = { p: "a6" };

print(a1.p);
print(a2.p);
print(a3.p);
print(a4.p);
print(a5.p);
print(a6.p);
