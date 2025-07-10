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
const enum E {
    A,
    B
}

@interface Anno {
    a: number = +E.A
    b: number = -E.A
    c: number = ~E.A
    d  = !E.A
    e: number = E.A | E.B
    f: number = E.A & E.B
    g: number = E.A >> E.B
    h: number = E.A >>> E.B
    i: number = E.A << E.B
    j: number = E.A ^ E.B
    k: number = E.A * E.B
    l: number = E.A / E.B
    m: number = E.A + E.B
    o: number = E.A - E.B
    p: number = E.A % E.B
    q: number = E.A ** E.B
    r: number = E.A && E.B
    s: number = E.A || E.B
    t = E.A < E.B
    u = E.A <= E.B
    v = E.A > E.B
    w = E.A >= E.B
    x = E.A === E.A
    y = E.B !== E.B
    z = E.A == E.A
}
`

exports.expectResult = `let __generate__Id: number = 0;
function generateId(): string {
    return "annotationDeclarationFieldInitializer6_" + ++__generate__Id;
}
const enum E {
    A = 0,
    B = 1
}
@interface __$$ETS_ANNOTATION$$__Anno {
    a: number = 0;
    b: number = 0;
    c: number = -1;
    d: boolean = true;
    e: number = 1;
    f: number = 0;
    g: number = 0;
    h: number = 0;
    i: number = 0;
    j: number = 1;
    k: number = 0;
    l: number = 0;
    m: number = 1;
    o: number = -1;
    p: number = 0;
    q: number = 0;
    r: number = 0;
    s: number = 1;
    t: boolean = true;
    u: boolean = true;
    v: boolean = false;
    w: boolean = false;
    x: boolean = true;
    y: boolean = false;
    z: boolean = true;
}
`