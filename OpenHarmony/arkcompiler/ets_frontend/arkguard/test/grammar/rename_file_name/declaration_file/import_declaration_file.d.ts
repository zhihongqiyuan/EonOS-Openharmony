/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import { declaration_class, declaration_func } from './export_func_class'
import { tomAddr, tomAge } from './folder_declaration/export_var'
import { MainPage, DogClass } from './declaration_struct'

declare function printName(name: string): void;

declare function printFunc(funcName: string): void;

declare function defaultFunction01(para1: string, para2: number): void

declare class myClassTemp {
    tempPapa1: string;
    tempPara2: number;
}