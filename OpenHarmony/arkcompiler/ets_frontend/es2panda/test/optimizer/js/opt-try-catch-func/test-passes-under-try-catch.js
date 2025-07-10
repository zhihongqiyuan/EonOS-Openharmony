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

let globalCond = true;
function testCatchPhiDataflow() {
    let a = '0';
    try {
        if (globalCond) {
            a = '1';
            throw a;
        } else {
            a = '2';
        }
    } catch (e) {
        a = e;
    }
    print(a);
}

function testTryWithAccCatchPhi() {
    let a = 1;
    try {
        throw a;
    } catch (e) {
        print(e);
    }
}

function testTryWithMoveConstants() {
    let a = 1;
    try {
        a = 2;
        throw a;
    } catch (e) {
        print(a === 2);
    }
    try {
        a = 3;
        throw a;
    } catch (e) {
        print(a === 3);
    }
}

function testTryWithRegAccAlloc() {
    let a = 's1';
    let b = 's2';
    let c = a + b;
    print(c);
    try {
        a = 's1';
        b = 's2';
        throw 1;
    } catch (e) {
        c = a + b;
        print(c);
    }
    c = a + b;
    print(c);
}

testCatchPhiDataflow();
testTryWithAccCatchPhi();
testTryWithMoveConstants();
testTryWithRegAccAlloc();
