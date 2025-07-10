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

// Exporting declarations
export const v1 = 1, v2 = 2;
export let v3 = 3;
export var v4 = 4;
export function f1() {}
export class C1 {}
export const { v5, v6 } = { v5: 5, v6: 6 };

// Export list
const v7 = 7, v8 = 8;
export { v7, v8 as v10, v7 as v9, v8 };

// Aggregating modules
// According to spec, v2 should use 2 in current file, instead of 100 in test-export-default-const
export * from "@normalized:N&&&myapp/test-export-default-const&";
export * as m1 from "@normalized:N&&&myapp/test-export-default-const&";
export { default, default as v11, v2 as v12} from "@normalized:N&&&myapp/test-export-default-const&";