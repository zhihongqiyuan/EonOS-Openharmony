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

const a = 1;
export {a};
export {a as a1};
export {b};
export {b as b1};
let b = 2;
export const c = 3;
const d = 4;
export {d as d1, d as d2};

function foo() {
    if (a > 1) {
        print("bad");
    }
    if (b > 2) {
        print("good1");
    } else {
        print("good2");
    }
    if (c > 3) {
        print("bad");
    }
    if (d > 4) {
        print("bad");
    }
}