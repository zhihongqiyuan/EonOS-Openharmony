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

let ohIndirectObj = {
  ohIndirectProp1: 123
}

let ohShorthand = {
  ohShorthandProp: 123
}

module.exports = {
  // PropertyAssignment
  ohPropertyAssignment1: 123,
  'ohPropertyAssignment2': 123,
  ['ohPropertyAssignment3']: 123,
  ohPropertyAssignment4: ohIndirectObj,

  // ShorthandPropertyAssignment
  ohShorthand,

  // MethodDeclaration
  ohMethod1() {},
  'ohMethod2'() {},
  ['ohMethod3']() {},

  // AccessorDeclaration
  get ohGetProp1() {},
  get 'ohGetProp2'() {},
  get ['ohGetProp3']() {},
  set ohSetProp1(value) {},
  set 'ohSetProp2'(value) {},
  set ['ohSetProp3'](value) {},
}

exports.ohExportElement1 = 1;
class ohIndirectClass1 {
  ohIndirectProp2 = 123;
}
exports.ohExportElement2 = ohIndirectClass1;
exports.ohExportElement3 = class ohIndirectClass2 {
  ohIndirectProp3 = 123;
}
exports.ohExportElement4 = class {
  ohIndirectProp4 = 123;
}

module.exports.ohExportElement5 = 1;
module.exports['ohExportElement6'] = 1;
class ohIndirectClass3 {
  ohIndirectProp5 = 123;
}
module.exports.ohExportElement7 = ohIndirectClass3;
exports.ohExportElement8 = class ohIndirectClass4 {
  ohIndirectProp6 = 123;
}
module.exports.ohExportElement9 = class {
  ohIndirectProp7 = 123;
}
