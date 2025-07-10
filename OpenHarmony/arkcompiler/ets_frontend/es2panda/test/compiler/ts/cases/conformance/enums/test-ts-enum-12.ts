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


enum tsEnum1 {
    A,
    B="Str_B",
    C=B,
    D=A,
    E=C
}

print(tsEnum1.A);
print(tsEnum1[0]);
print(tsEnum1.B);
print(tsEnum1.C);
print(tsEnum1.D);
print(tsEnum1.E);
print(tsEnum1["Str_B"]);