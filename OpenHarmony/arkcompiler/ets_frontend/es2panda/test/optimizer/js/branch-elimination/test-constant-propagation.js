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

export let moduleVar = 1;
let lexVar = 1;

function foo() {
    let a = 1;
    let localVar = 2;
    if (a > 1) {
        print("bad");
    }

    localVar = a;
    if (a > 1) {
        localVar = 2;
    }
    if (localVar > 1) {
        print("bad");
    }

    lexVar = a;
    if (lexVar > 1) {
        print("good1");
    } else {
        print("good2");
    }

    moduleVar = a;
    if (moduleVar > 1) {
        print("good3");
    } else {
        print("good4");
    }

    let o = {fa: a}
    if (o.fa > 1) {
        print("good5");
    } else {
        print("good6");
    }
}

foo()