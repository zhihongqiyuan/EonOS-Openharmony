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

class Person5 {
    private age: number = 10;
    name: string = 'Tony';

    prop_func1 = function () {
        console.log('func_exp');
    };

    prop_func2 = function foo () {
        console.log('func_exp2');
    };

    prop_arrow = () => {
        console.log('arrow');
    };

    prop_class = class {
        #pri_prop: string = 'myproperty';
        constructor() {
            this.#pri_prop = 'newproperty';
        }
    };
}