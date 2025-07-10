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

import {fooTestPlus} from './export_file';
import {PersonTest1} from './export_file'
import assert from 'assert';
import path from 'path'

assert(fooTestPlus(1, 2) === 3, 'success');
let instance = new PersonTest1();

enum FoodList {
  MEAT = 0,
  FISH,
  WATER,
  RICE,
  APPLE
}
instance.eatFood(FoodList[0]);
assert(instance.eatedFood.toString() === 'MEAT', 'success');

const fileName: string = path.basename(__filename);
assert(fileName !== 'import_dir.ts', 'success');