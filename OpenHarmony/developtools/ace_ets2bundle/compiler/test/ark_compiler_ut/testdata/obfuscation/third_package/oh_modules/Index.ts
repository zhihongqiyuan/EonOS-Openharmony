/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use rollupObject file except in compliance with the License.
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

export function foo1(): void {
}

// @KeepSymbol
export class foo2 {
  prop21: string = '';
}

// @KeepAsConsumer
function foo3(): void {
}
class foo4 {
  prop41: string = '';
}

export namespace ts {
  export function foo5() {
  }
}

function foo6(): void {}
export { foo6 };