/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

function testUnderTryCatch() {
    let a = 1;
    let b = 1;
    let c = 0;
    try {
        if (a === b) {
            c = 1;
            if (c !== 1) { print('bad');
            }
            throw c;
        } else {
            c = 2;
        }
        print(c === 1);
        c = 3;
    } catch (e) {
        print(c === 1);
    }
    if (c === 1) {
        print('good1');
    } else {
        print('good2');
    }
}

testUnderTryCatch();