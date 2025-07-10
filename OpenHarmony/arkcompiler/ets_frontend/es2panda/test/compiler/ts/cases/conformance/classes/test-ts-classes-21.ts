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


class B {
    a: number = 1;
    b?: string;
    c(): any { return 2; };
    d?(): string;
    get x() {
        return false;
    }
    set x(v: boolean) {
    }
    e?: number;
    f?(): number;
}

var b = new B();

if (b.e) {
    print(b.a)
} else {
    print(b.c())
}
