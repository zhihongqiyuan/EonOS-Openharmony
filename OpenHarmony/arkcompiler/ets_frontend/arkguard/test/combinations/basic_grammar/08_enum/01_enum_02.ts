/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import assert from 'assert'

enum A {
    prop1 = 1,
    prop2 = "2",
}
A.prop1
A.prop2

assert(A.prop1 === 1);
assert(A.prop2 === "2");
assert(A[1] === 'prop1');

const enum B {
    prop1 = 1,
    prop2 = "2",
}
B.prop1
B.prop2

assert(B.prop1 === 1);
assert(B.prop2 === "2");