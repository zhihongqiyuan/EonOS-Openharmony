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
let test_gc;
(function () {
    test_gc = function() {};
})();
function f0(v3 = 10, v4 = function (){
    return v3;
}) {
    try {
        v3 = 20;
    } catch (e) {}
    try {
        'pybfr'.replace(v4, '');
    } catch (e) {}
    try {
        if (v3 === 20) {} else {
            try {
                Set.prototype.add = () => {};
            } catch (e) {}
        }
    } catch (e) {}
}
try {
    if (f0()() === 20) {} else {}
} catch (e) {}
print("success");
