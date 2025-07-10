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

function testTryCatchFinally() {
    let a = 1;
    try {
        a = 2;
    } catch (e) {
        print(e);
        print(a);
        a = 3;
    }
    print(a);
    try {
        a = 4;
    } finally {
        print(a);
        a = 5;
    }
    print(a);
    try {
        a = 6;
    } catch (e) {
        print(e);
        print(a);
        a = 7;
    } finally {
        print(a);
        a = 8;
    }
    print(a);
}

function testSimpleThrow() {
    let a = 1;
    print(a);
    throw a;
    a = 2;
    print(a);
}

function testThrowInTry() {
    let a = 1;
    try {
        a = 2;
        throw a;
        a = 3;
    } catch (e) {
        print(e);
        print(a);
        a = 4;
    }
    print(a);
    try {
        a = 5;
        throw a;
        a = 6;
    } finally {
        print(a);
        a = 7;
    }
    print(a);
    try {
        a = 8;
        throw a;
        a = 9;
    } catch (e) {
        print(e);
        print(a);
        a = 10;
    } finally {
        print(a);
        a = 11;
    }
    print(a);
    a = 12;
    print(a);
}

function testComplexThrow() {
    let a = 1;
    try {
        try {
            a = 2;
            print(a);
            throw a;
            a = 3;
        } catch (e) {
            print(e);
            print(a);
            a = 4;
            throw a;
            a = 5;
        } finally {
            print(a);
            a = 6;
            throw a;
            a = 7;
        }
        print(a);
        a = 8;
    } catch (e) {
        print(e);
        print(a);
        a = 9;
    }
    print(a);
}

function testTryCatchFinallyWithReturn() {
    let a = 1;
    try {
        a = 2;
        print(a);
        a = 3;
        throw a;
        a = 4;
        print(a);
        return a;
    } catch (e) {
        print(e);
        print(a);
        return;
        a = 5;
    } finally {
        print(a);
        a = 6;
        print(a);
        return a;
    }
    a = 7;
    print(a);
    return a;
}

testTryCatchFinally();
try {
    testSimpleThrow();
} catch (e) {
    print(e);
}
try {
    testThrowInTry();
} catch (e) {
    print(e);
}
try {
    testComplexThrow();
} catch (e) {
    print(e);
}
try {
    print(testTryCatchFinallyWithReturn());
} catch (e) {
    print(e);
}