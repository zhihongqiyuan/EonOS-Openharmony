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

let projectIndirectObj = {
  indirectProp1: 123
}

let projectShorthand = {
  projectShorthandProp: 123
}

module.exports = {
  // PropertyAssignment
  projectPropertyAssignment1: 123,
  'projectPropertyAssignment2': 123,
  ['projectPropertyAssignment3']: 123,
  projectPropertyAssignment4: projectIndirectObj,

  // ShorthandPropertyAssignment
  projectShorthand,

  // MethodDeclaration
  projectMethod1() {},
  'projectMethod2'() {},
  ['projectMethod3']() {},

  // AccessorDeclaration
  get projectGetProp1() {},
  get 'projectGetProp2'() {},
  get ['projectGetProp3']() {},
  set projectSetProp1(value) {},
  set 'projectSetProp2'(value) {},
  set ['projectSetProp3'](value) {},
}

exports.projectExportElement1 = 1;
class indirectClass1 {
  indirectProp2 = 123;
}
exports.projectExportElement2 = indirectClass1;
exports.projectExportElement3 = class indirectClass2 {
  indirectProp3 = 123;
}
exports.projectExportElement4 = class {
  indirectProp4 = 123;
}

module.exports.projectExportElement5 = 1;
module.exports['projectExportElement6'] = 1;
class indirectClass3 {
  indirectProp5 = 123;
}
module.exports.projectExportElement7 = indirectClass3;
exports.projectExportElement8 = class indirectClass4 {
  indirectProp6 = 123;
}
module.exports.projectExportElement9 = class {
  indirectProp7 = 123;
}
