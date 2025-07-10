/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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


class ShadowsInfo { }

const Exec = (cb: () => void) => { cb(); };

function Func1() {
    type ShadowsInfo = string;
    var v:ShadowsInfo="str";
    print(v);
    Exec(() => { Func2()(ShadowsInfo.prototype) });
}

function Func2() {
    function innerFunc(target: any) {
        const xxx = target.constructor;
        print("helloWorld");
    }
    return innerFunc;
}

Func1();
