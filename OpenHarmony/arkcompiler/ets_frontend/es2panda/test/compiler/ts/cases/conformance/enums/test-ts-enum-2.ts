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


enum tsEnum3 {
    a,
    b = 10 + 10 * 10 / 2 -1,
    c = 3,
    d = 1 << 5,
    "e"
}

enum tsEnum3 {
    f = d + tsEnum3.c + 10,
    g = 20 + 30 * 2 && 1,
    h = "abcd".length,
    i = "str" + "_i"
}

print(tsEnum3.a);
print(tsEnum3.b);
print(tsEnum3.e);
print(tsEnum3.f);
print(tsEnum3.g);
print(tsEnum3.h);
print(tsEnum3.i);

print(tsEnum3[33]);
print(tsEnum3[45]);
print(tsEnum3[4]);
print(tsEnum3["str" + "_i"]);