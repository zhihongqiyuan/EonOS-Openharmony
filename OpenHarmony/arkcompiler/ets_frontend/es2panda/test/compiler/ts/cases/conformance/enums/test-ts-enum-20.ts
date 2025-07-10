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


namespace ns2 {
    export namespace ns1 {
        export namespace ns1 {
            export namespace ns {
                export namespace ns{
                    enum E {a = "str"}
                    enum E {b = a}
                    export enum E1 {a=E.b}
                    print(E1.a);
                    print(E1["str"]);
                }
            }
            export enum E2 {a = ns.ns.E1.a}
            print(E2.a);
            print(E2["str"]);
        }
    }
    enum E3 {a = ns2.ns1.ns1.E2.a}
    print(E3.a);
    print(E3["str"]);
}