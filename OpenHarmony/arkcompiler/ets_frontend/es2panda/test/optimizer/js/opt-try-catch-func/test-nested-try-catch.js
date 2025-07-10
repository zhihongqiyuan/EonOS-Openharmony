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

function testNestedTryBlock() {
    let a = 1;
    try {
        try {
            throw a;
            a = 2;
        } catch (e) {
            print(e)
            print(a);
            throw a;
        }
    } catch (e) {
        print(e);
        try {
            a = 3;
            throw a;
            a = 4;
        } finally {
            print(a);
            a = 5;
        }
    } finally {
        print (a);
        try {
            a = 6;
            throw a;
            a = 7;
        } catch (e) {
            print(e);
            print(a);
            a = 8
            throw a;
            a = 9;
        } finally {
            print(a);
            a = 10;
        }
    }
}

function testTryBlockWithLoop() {
    let i = 0, j = 0, a = 1;
    try {
        outer: while (i < 10) {
            i += 1;
            try {
                inner: while (j < 10) {
                    j += 1;
                    try {
                        try {
                            if (i === 3) {
                                continue outer;
                            }
                            if (i === 5) {
                                break outer;
                            }
                            if (j === 3) {
                                continue;
                            }
                            if (j === 5) {
                                break;
                            }
                            if (i === 7 && j === 5) {
                                throw a;
                            }
                            if (i === 8 && j === 5) {
                                return a;
                            }
                        } finally {
                            print(a);
                            a = 2;
                        }
                    } catch (e) {
                        print(e);
                        print(a);
                        a = 3;
                    } finally {
                        print(a);
                        a = 4;
                    }
                }
            } finally {
                print(a);
                a = 6;
            }
        }
    } finally {
        print(a);
        a = 7;
    }
}

try {
    testNestedTryBlock();
} catch (e) {
    print(e);
}
try {
    testTryBlockWithLoop();
} catch (e) {
    print(e);
}