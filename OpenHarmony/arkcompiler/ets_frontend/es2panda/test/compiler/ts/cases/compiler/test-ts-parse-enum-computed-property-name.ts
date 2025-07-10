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

enum Enum1 {
    enumP12,
    "enumP13",
    'enumP14',
    [`enumP15`],
    ['enumP16'],
    ["enumP17"],
}

print(Enum1.enumP12);
print(Enum1.enumP13);
print(Enum1.enumP14);
print(Enum1.enumP15);
print(Enum1.enumP16);
print(Enum1.enumP17);
