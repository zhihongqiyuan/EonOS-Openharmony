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


class A {
    accessor member:string = "str";
    public accessor member1:string = "str1";
    protected accessor member2:string = "str2";
    private accessor member3:string = "str3";

    testInternalProperties() {
        print(this.member2);
        this.member2 = "test";
        print(this.member2);
        
        print(this.member3);
        this.member3 = "test";
        print(this.member3);
    }
}

let a = new A();
print(a.member)
a.member = "test"
print(a.member)

print(a.member1)
a.member1 = "test"
print(a.member1)

a.testInternalProperties();

