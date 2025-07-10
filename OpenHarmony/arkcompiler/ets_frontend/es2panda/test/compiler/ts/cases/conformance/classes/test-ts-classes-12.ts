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


const computed1: unique symbol = Symbol("symbol1");

class Base {
    prop = "base_prop";
    prop1 = "base_prop1";
    prop2 = "base_prop2";
    "str_prop" = "base_str_prop";
    1 = "base_1";
    [computed1] = "base_computed1";
    static "s_str_prop" = "base_static_str_prop";
}

class A extends Base {
    "str_prop" = "A_str_prop";
    [computed1] = "A_computed1";
    static "s_str_prop" = "A_s_str";
}

class B extends Base {
    prop:string;
    constructor() {
        super();
        this.prop = "ctor_B_prop";
        this.prop1 = "ctor_B_prop1";
        this["str_prop"] = "B_ctor_str_prop";
    }
    prop1 = "B_prop1";
    "str_prop" = "B_str";
}

class C extends Base {
    constructor(public prop = "tsparam_prop") {
        super();
        this[computed1] = "C_computed1";
    }
}

var objA = new A();
print(objA["str_prop"]);
print(objA[computed1]);
print(A["s_str_prop"]);

var objB = new B();
print(objB.prop);
print(objB.prop1);
print(objB.prop2);
print(objB["str_prop"]);

var objC = new C();
print(objC.prop);
print(objC[computed1]);
