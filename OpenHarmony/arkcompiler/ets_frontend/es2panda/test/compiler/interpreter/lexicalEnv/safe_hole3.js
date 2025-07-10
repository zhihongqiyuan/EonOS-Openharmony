/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

'use strict';

try {} catch (e) {
    const dummy = new Proxy(function () {
        return dummy;
    },handler);
}
let test_gc;
(function () {
    test_gc = function() {};
})();
function f0(v0, v1= "Bad assertion") {}
f0.aarEq = function f2(v5, v6, v7 = `Expected «${v6}» to be «${v5}».`){};
{
    let v11 = [];
    let v12 = new Proxy(Object.create(null, {
        x: {
            enumerable: true,
            configurable: true,
            value: 0
        }
    }), {
        getOwnPropertyDescriptor(v13, v14) {
            v11.push(`gopd ${v14}`);
            v13[test_func8(v13, 1040732)] = true, test_gc();
        }
    });
    try {
        for (let v15 in v12) {
            if (v15 != null && typeof v15 == "object") Object.defineProperty(v15, test_func8(v15, 647748), {
                get: function () {},
                set: function (value) {
                    v15[test_func8(v15, 586990)], test_gc();
                }
            })
        }
    } catch (e) {}
    try {
        f0.aarEq(["gopd x", "enumerate x"], v11);
    }catch (e) {}
}
print("success");
