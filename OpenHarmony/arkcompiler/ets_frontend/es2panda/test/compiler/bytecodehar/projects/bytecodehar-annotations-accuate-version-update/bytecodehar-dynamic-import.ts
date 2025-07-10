/**
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

export let test: string = "test";

function testReplacementWoAt() {
    import("@normalized:N&module&bundle&lib/importfile1&1.0.0");
    import("@normalized:N&module&bundle&lib/importfile1&");
    import("@normalized:N&&bundle&lib/importfile1&1.0.0");
    import("@normalized:N&&bundle&lib/importfile1&");

    // No-effect since no version info in compileContextInfo.json
    import("@normalized:N&&&lib2/importfile3&");
}

function testReplacementWithAt() {
    import("@normalized:N&module&&@ohos/lib/importfile2&1.0.0");
    import("@normalized:N&module&&@ohos/lib/importfile2&");
    import("@normalized:N&&&@ohos/lib/importfile2&1.0.0");
    import("@normalized:N&&&@ohos/lib/importfile2&");

    // No-effect since no version info in compileContextInfo.json
    import("@normalized:N&&&@ohos/lib2/importfile4&");
}

function testNonOhmurl() {
    import("aaa");
}

function testNonConst() {
    // Take effect only under release due to bcopt
    let a = "@normalized:N&&bundle&lib/importfile1&"
    import(a);

    a = "@normalized:N&&bundle&lib/importfile1&";
    let b = "@normalized:N&&bundle&lib/importfile1&";
    import(a);

    a = "@normalized:N&&bundle&lib/importfile1&";
    "@normalized:N&&bundle&lib/importfile1&";
    import(a);
}

export @interface __$$ETS_ANNOTATION$$__Anno1 {
    b: boolean = true
}

@__$$ETS_ANNOTATION$$__Anno1()
class A { }