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


namespace ns {
    export enum E {
        a=`123`
    }
}

enum E1 {
    a=ns["E"].a
}

enum E2 {
    b=ns.E.a
}

enum E3 {
    c=ns[`E`].a
}

print(E1["123"]);
print(E2["123"]);
print(E3["123"]);