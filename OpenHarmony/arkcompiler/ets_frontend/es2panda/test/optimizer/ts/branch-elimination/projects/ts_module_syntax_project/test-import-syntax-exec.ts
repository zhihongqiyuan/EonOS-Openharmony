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

import v1 from "@normalized:N&&&myapp/test-export-default-const&";
import v2 from "@normalized:N&&&myapp/test-export-default-let&";
import v3 from "@normalized:N&&&myapp/test-export-default-value&";
import * as m1 from "@normalized:N&&&myapp/test-export-syntax&";
import { v1 as v4, v2 as v5, v6, v7, v10 as v8, m1 as m2, v12 as v9 } from "@normalized:N&&&myapp/test-export-syntax&";
import { default as v10 } from "@normalized:N&&&myapp/test-export-default-const&";
import v0099  from "@normalized:N&&&myapp/test-export-syntax&"
import v11 from "@normalized:N&&&myapp/test-export-syntax&";

if (m1.v1 !== 1 || v4 !== 1 || v5 !== 2 || v7 !== 7 || v8 !== 8) {
    print("bad");
}
if (v1 !== 1) {
    print("good1");
}
if (v2 !== 1) {
    print("good2");
}
if (v3 !== 2) {
    print("good3");
}
if (v6 !== 6) {
    print("good4");
}
if (v9 !== 100) {
    print("good5");
}
if (v10 !== 1) {
    print("good6");
}
if (v11 !== 1) {
    print("good7");
}
if (v0099 !== 1) {
    print("good8");
}