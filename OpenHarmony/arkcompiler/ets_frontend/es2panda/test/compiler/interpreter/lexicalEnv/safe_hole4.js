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
function test_func7(f) {}
let test_gc;
(function () {
    test_gc = function() {};
})();
function f0(v0) {}
(function (v4 = () => arguments) {
    v4[Symbol.toStringTag] = "hello2";
})();
(function (v12 = () => v13, {
    arguments: {}
}) {})(undefined, {
    arguments: {}
});
try {
    (function (v16 = () => v18, v17 = v16(), {
        arguments: v18
    }) {
        f0(v17[1] === undefined);
        test_func7(() => {
            return f0(v17.length === 3);
        });
        var v19;
        {
            function f10() {
                v19[18];
            }
        }
    })(undefined, undefined, {
        arguments: {}
    }); 
} catch (e) {
    print("Failed")
}
