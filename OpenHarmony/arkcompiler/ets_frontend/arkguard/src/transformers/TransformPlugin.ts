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

import type {IOptions} from '../configs/IOptions';
import type {Node, TransformerFactory} from 'typescript';

export interface TransformPlugin {
  name: string;
  order: number
  createTransformerFactory: (option: IOptions) => TransformerFactory<Node>;
}

// The order of identifier obfuscation should be as early as possible, because create or update nodes will cause the NodeFlags of the parent node 
// on the ast to be cleared. NodeFlags will affect the binding result. The following scenarios have been found to affect the obfuscation function:
// Problem scenario: A export name in a namespace scope within a declaration file
// ```
// declare namespace ns {
//   class A { // -keep-global-name A is invalid.
//       temp: number;
//   }
// }
// ```
// Problem description: because the property temp is obfuscated, a node update occurs, causing -keep-global-name A to be invalid.
export enum TransformerOrder {
  SHORTHAND_PROPERTY_TRANSFORMER,
  RENAME_IDENTIFIER_TRANSFORMER,
  VIRTUAL_CONSTRUCTOR_TRANSFORMER,
  DISABLE_CONSOLE_TRANSFORMER,
  RENAME_PROPERTIES_TRANSFORMER,
  RENAME_FILE_NAME_TRANSFORMER
}