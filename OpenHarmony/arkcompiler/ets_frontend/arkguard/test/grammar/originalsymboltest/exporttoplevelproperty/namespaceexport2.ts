/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

type toplevelElement4 = string; // export and export obfuscation
type toplevelElement5 = number; // toplevel obfuscation
type toplevelElement6 = boolean; // kept
declare module TestNs2 {
  export { toplevelElement4, toplevelElement6 };
  export { toplevelElement5 as te5 };
}
declare const nsElement1: TestNs2.toplevelElement4;
declare const nsElement2: TestNs2.te5;
export {};