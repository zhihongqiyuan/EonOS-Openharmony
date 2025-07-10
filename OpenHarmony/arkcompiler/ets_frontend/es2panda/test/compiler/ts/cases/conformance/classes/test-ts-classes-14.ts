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


const computed1 : unique symbol = Symbol("symbol");

namespace ns {
    export class A {
        prop = "prop_ns.A";
        "str_prop" = "str_prop_ns.A";
        ["str_computed"] = "computed_ns.A";
        [computed1] = "symbol_ns.A";
    }

    export var varB = class B {
        prop = "prop_ns.B";
        "str_prop" = "str_prop_ns.B";
        ["str_computed"] = "computed_ns.B";
        [computed1] = "symbol_ns.B";
    }
}
var objNSA = new ns.A();
print(objNSA.prop);
print(objNSA["str_prop"]);
print(objNSA["str_computed"]);
print(objNSA[computed1]);

var objNSB = new ns.varB();
print(objNSB.prop);
print(objNSB["str_prop"]);
print(objNSB["str_computed"]);
print(objNSB[computed1]);
