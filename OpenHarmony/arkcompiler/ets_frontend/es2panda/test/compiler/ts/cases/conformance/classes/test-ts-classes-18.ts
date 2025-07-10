/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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


class Base {
    prop = 1;
}

class A extends Base {
    constructor() {
        super();
        this.prop = 4;
    }
    prop = 2;
    prop2 = this.prop;
}


class B extends Base {
    constructor(public prop = 3) {
        super();
        this.prop = 4;
    }
    prop2 = this.prop;
}

var a = new A();
print(a.prop);
print(a.prop2);

var b = new B();
print(b.prop);
print(b.prop2);
