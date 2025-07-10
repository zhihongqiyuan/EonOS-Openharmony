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


class Base {
    static a = class A {
        static v = "A";
    }

    static aa = class AA {
        v = "AA";
    }

    aaa = class AAA {
        static v = "AAA";
    }

    aaaa = class AAAA {
        v = "AAAA";
    }
}

let base = new Base();

print(Base.a.v);
print(new Base.aa().v);
print(base.aaa.v);
print(new base.aaaa().v);
