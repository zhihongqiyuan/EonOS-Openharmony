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


enum tsEnum2 {
    a,
    b = 10 + 10 * 10 / 2 -1,
    c = 3,
    d = 1 << 5,
    "e",
    f = d + tsEnum2.c + 10,
    g = 20 + 30 * 2 && 1,
    h = "abcd".length,
    length = h + "abc".length,
    i = "str" + "_i"
}

print(tsEnum2.a);
print(tsEnum2.b);
print(tsEnum2.e);
print(tsEnum2.f);
print(tsEnum2.g);
print(tsEnum2.h);
print(tsEnum2.length);
print(tsEnum2.i);

print(tsEnum2[33]);
print(tsEnum2[45]);
print(tsEnum2[tsEnum2.g]);
print(tsEnum2["abcd".length]);
print(tsEnum2[7]);
print(tsEnum2["str_i"]);