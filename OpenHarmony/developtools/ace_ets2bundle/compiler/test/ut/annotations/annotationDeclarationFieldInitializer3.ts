/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

exports.etsAnnotationsEnable = true;
exports.emitIntermediateTS = true;

exports.source = `
const a = "a"
const b = "b"

@interface Anno {
    d  = !a
    r = a && b
    s = a || b
    t = a < b
    u = a <= b
    v = a > b
    w = a >= b
    x = a === a
    y = a !== a
    z = a == a
}
`

exports.expectResult = `let __generate__Id: number = 0;
function generateId(): string {
    return "annotationDeclarationFieldInitializer3_" + ++__generate__Id;
}
const a = "a";
const b = "b";
@interface __$$ETS_ANNOTATION$$__Anno {
    d: boolean = false;
    r: string = "b";
    s: string = "a";
    t: boolean = true;
    u: boolean = true;
    v: boolean = false;
    w: boolean = false;
    x: boolean = true;
    y: boolean = false;
    z: boolean = true;
}
`