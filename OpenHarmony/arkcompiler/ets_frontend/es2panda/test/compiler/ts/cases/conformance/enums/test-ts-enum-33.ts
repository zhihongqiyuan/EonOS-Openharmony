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


function f() {
    enum E {
        a = ns1.E1.a,
        b = ns2.E2.b,
        c = ns1.E1.a + ns2.E2.b,
    }
    print(E.a);
    print(E["str"]);
    print(E["strstr"]);
}

namespace ns1 {
    export enum E1{
        a="str",
    }
}

namespace ns2 {
    export enum E2 {
        a = "str",
        c = a,
        b = c
    }
}

f();