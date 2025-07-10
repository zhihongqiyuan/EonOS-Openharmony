/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

// testReplacementWoAt
import * as a from "@normalized:N&module&bundle&lib/importfile1&1.0.0";
import * as b from "@normalized:N&module&bundle&lib/importfile1&";
import * as c from "@normalized:N&&bundle&lib/importfile1&1.0.0";
import * as d from "@normalized:N&&bundle&lib/importfile1&";

// No-effect since no version info in compileContextInfo.json
import * as e from "@normalized:N&&&lib2/importfile3&";

//testReplacementWithAt
import * as f from "@normalized:N&module&&@ohos/lib/importfile2&1.0.0";
import * as g from "@normalized:N&module&&@ohos/lib/importfile2&";
import * as h from "@normalized:N&&&@ohos/lib/importfile2&1.0.0";
import * as i from "@normalized:N&&&@ohos/lib/importfile2&";

// No-effect since no version info in compileContextInfo.json
import * as j from "@normalized:N&&&@ohos/lib2/importfile4&";

function foo() {
    print([a, b, c, d, e, f, g, h, i, j]);
}

export @interface __$$ETS_ANNOTATION$$__Anno2 {
    b: boolean = true
}

@__$$ETS_ANNOTATION$$__Anno2()
class A { }