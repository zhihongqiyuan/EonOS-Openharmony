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
@interface Anno {
    a: number[] = [1, 2]
}

@Anno
class C {}
`

exports.expectResult = `let __generate__Id: number = 0;
function generateId(): string {
    return "annotationApplication6_" + ++__generate__Id;
}
@interface __$$ETS_ANNOTATION$$__Anno {
    a: number[] = [1, 2];
}
@__$$ETS_ANNOTATION$$__Anno({ a: [1, 2] })
class C {
}
`