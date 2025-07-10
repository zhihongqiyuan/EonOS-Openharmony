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
function f0(v0) {}
(function (v4 = () => arguments){
    var v5;
    let v6 = v4();
    let v7;
    {
        function f3(){};
        v7 =f3;
        Reflect.set([null], 0, 0, v6);
        function f4() {
            v5[5];
        }
    }
    f0(v7 !== v6);
})();
print("success");
