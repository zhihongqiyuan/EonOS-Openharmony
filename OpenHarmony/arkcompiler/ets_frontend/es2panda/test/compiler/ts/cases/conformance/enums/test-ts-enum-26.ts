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


namespace ns101 {
    export namespace ns102{
        export namespace ns103{
            export enum AAA{
                a="str"
            }
            export import eee=AAA;
        }
        export import ie5=ns103.AAA;
    }
    export import ie2=ns102.ns103;
    export import ie3=ns101.ie2;
    export import ie4=ns102.ns103.AAA;
    
}

import ie1 = ns101.ns102;

enum E {
    a=ie1.ns103.AAA.a,
    b=ns101.ie2.eee.a,
    c=ns101.ie3.AAA.a,
    d=ns101.ie4.a,
    e=ie1.ns103.eee.a,
    f=ie1.ie5.a,
    g=ns101.ns102.ie5.a
}

print(E.a);
print(E.f);
print(E["str"]);