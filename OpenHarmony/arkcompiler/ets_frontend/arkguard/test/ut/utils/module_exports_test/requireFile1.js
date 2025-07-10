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

const requireModule1 = require("./exportFile1");
const requireModule2 = require("./exportFile2");
const requireModule3 = require("./exportFile3");
const requireModule4 = require("./exportFile4");
const requireModule5 = require("../oh_modules/commonJsTest");
const {collectClass, testClass2} = require("../oh_modules/commonJsTest");
const {ExportClass, exportName, exportVersion, nestedObj, stringProp, 
  literalKey, myMethod, getProp, setProp, MyClass, shorthandValue, newProp1, spreadProp, elements, f} = require("../oh_modules/commonJsTest2");

let var1 = requireModule1.name;
let var2 = requireModule1.version;
let var3 = requireModule1.area;
let var4 = requireModule1.api;
let var5 = requireModule1.value;
let var6 = new requireModule1.A().export_prop2;
let var7 = new requireModule2().export_prop1;
let var8 = requireModule1.tesObj.prop5;
let var9 = requireModule3.nested.inner_prop4;
let var10 = requireModule4['property'];
let var11 = requireModule5.collectClass;
let var12 = requireModule5();