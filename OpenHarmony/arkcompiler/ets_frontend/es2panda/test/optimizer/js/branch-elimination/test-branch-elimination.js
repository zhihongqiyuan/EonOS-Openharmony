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

function testTypeConvert() {
    let a = 0;
    if (a == 0 && a == 0.0) {
        print("good");
    } else {
        print("bad")
    }
    if (a == "0" || a === "0" || a == false || a === false) {
        print("good1");
    } else {
        print("good2");
    }
    if (a <= "0" || a >= "0" || a <= false || a >= false) {
        print("good3");
    } else {
        print("good4");
    }
    if (a < "0" || a > "0" || a < false || a > false) {
        print("good5");
    } else {
        print("good6");
    }
}

function testIntNumber() {
    let init = 1;
    let a = init;
    if (false) {
        a = 10;
    }
    if (a != 1 || a == 0 || a !== 1 || a === 0 || a < 0 || a > 1 || a <= 0 || a >= 2) {
        print("bad");
    } else {
        print("good");
    }
}

function testFloatNumber() {
    let init = 0.1;
    let a = init;
    if (false) {
        a = 10.0;
    }
    if (a != 0.1 || a == 1.1 || a !== 0.1 || a === 1.1 || a < 0.1 || a > 0.1 || a <= 0.01 || a >= 1.1) {
        print("bad");
    } else {
        print("good");
    }
}


function testBool() {
    let init = false;
    let a = init;
    if (a) {
        a = true;
    }
    if (a != false || a == true || a !== false || a === true || a < false || a > true) {
        print("bad");
    } else {
        print("good");
    }
}

function testString() {
    let init = "0";
    let a = init;
    if (false) {
        a = "1";
    }
    if (a != "0" || a == "1" || a !== "0" || a === "1") {
        print("bad");
    } else {
        print("good");
    }
    // not folding </<=/>/>= for strings
    if (a <= "0" || a >= "1" || a < "0" || a > "1") {
        print("good2");
    } else {
        print("good3");
    }
}

function testBBStruct() {
    if (false || true) {
        print("good");
    }
    if (false && true) {
        print("bad");
    }
    if (true && false) {
        print("bad");
    }
    if (false) {
        if (true) {
            print("bad");
        }
    }
    if (true) {
        if (false) {
            print("bad");
        }
    }
    while (false) {
        print("bad");
    }
}

testTypeConvert();
testIntNumber();
testFloatNumber();
testBool();
testString();
testBBStruct();