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

try {} catch (e) {
    const dummy = new Proxy(function () {
        return dummy;
    },handler);
}
function test_func2(obj, type) {
    for (let name of Object.getOwnPropertyNames(obj)) {}
}
function test_func8(obj, seed) {
    let prop = test_func2(obj);
}
let test_gc;
(function () {
    test_gc = function() {};
})();
try{
    (function (v4 = () => arguments){
        var v5;
        let v6 = v4();
        if (v6 != null && typeof v6 == "object") Object.defineProperty(v6, test_func8(v6, 104270),{
            value: v6
        });
        {
            function f4() {
                try {
                    v5[5];
                } catch (e) {}
            }
        }
    })();
} catch(e) {}
print("success");
