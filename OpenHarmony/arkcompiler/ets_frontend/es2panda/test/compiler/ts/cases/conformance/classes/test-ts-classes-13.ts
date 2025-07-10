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


class A {
    prop = "str1";
}
var obj = new A();
print(obj.prop);

namespace ns {
    export class NSEA {
        prop = "Str_NSEA";
    }
}
var objNSEA = new ns.NSEA();
print(objNSEA.prop);

var cExpr = class B {
    prop = "str_B";
}
var objCExpr = new cExpr();
print(objCExpr.prop);

namespace ns1 {
    export var nsClass = class NSB {
        prop = "str_NSB";
    }
}
var objNSClass = new ns1.nsClass();
print(objNSClass.prop);
