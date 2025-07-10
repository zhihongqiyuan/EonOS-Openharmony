/**
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

import {test as test1} from '@normalized:N&&&@ohos/bytecodehar2/bytecodehar2-file1&2.0.0'

export function test() {
    print('bytecodehar1.file1.test');
    import('@normalized:N&&&sourcehar/sourcehar-file1&2.0.0')
}

function A(a) {
    this.a = a
    this.b = 1
    this.c = 2
}

export @interface __$$ETS_ANNOTATION$$__Anno1 {
    b: boolean = true
}

@__$$ETS_ANNOTATION$$__Anno1()
class B { }
