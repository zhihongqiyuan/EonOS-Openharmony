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
const a = 1
const b = 2

@interface Anno {
    d  = !a
    e: number = a | b
    f: number = a & b
    g: number = a >> b
    h: number = a >>> b
    i: number = a << b
    j: number = a ^ b
    k: number = a * b
    l: number = a / b
    m: number = a + b
    o: number = a - b
    p: number = a % b
    q: number = a ** b
    r: number = a && b
    s: number = (a - 1) || b
    t = a < 2
    u = a <= 1
    v = a > 2
    w = a >= 1
    x = a === 1
    y = a !== 1
    z = a == 1
}
`

exports.expectResult = `let __generate__Id: number = 0;
function generateId(): string {
    return "annotationDeclarationFieldInitializer1_" + ++__generate__Id;
}
const a = 1;
const b = 2;
@interface __$$ETS_ANNOTATION$$__Anno {
    d: boolean = false;
    e: number = 3;
    f: number = 0;
    g: number = 0;
    h: number = 0;
    i: number = 4;
    j: number = 3;
    k: number = 2;
    l: number = 0.5;
    m: number = 3;
    o: number = -1;
    p: number = 1;
    q: number = 1;
    r: number = 2;
    s: number = 2;
    t: boolean = true;
    u: boolean = true;
    v: boolean = false;
    w: boolean = true;
    x: boolean = true;
    y: boolean = false;
    z: boolean = true;
}
`