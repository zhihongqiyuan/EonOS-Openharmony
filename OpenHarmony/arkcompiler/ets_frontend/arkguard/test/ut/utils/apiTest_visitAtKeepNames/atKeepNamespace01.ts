/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

// @KeepSymbol
namespace MyNamespace01 {
  export const nsConstValue01 = 1;
  export function nsFunction01(): void {};
  export class nsClass01 {
    classProp01 = 1;
  };
  export interface nsInterface01 {
    interfaceProp01: string;
  };
  export enum nsEnum01 {
    RED01
  }
  export namespace InnerNamespace01 {
    export const innerConstValue01 = 1;
  }
  export type nsType01 = "A";
  const noExportedValue01 = 1;
}

namespace MyNamespace02 {
  // @KeepSymbol
  export const nsConstValue02 = 1;
  // @KeepSymbol
  export function nsFunction02(): void {};
  // @KeepSymbol
  export class nsClass02 {
    classProp02 = 1;
  };
  export class nsClass02_01 {
    classProp02_01 = 1;
    // @KeepSymbol
    constructor(para: number) {
      this.classProp02_01 = para;
    }
  };
  export class nsClass02_02 {
    // @KeepSymbol
    classProp02_02 = 1;
    constructor(para: number) {
      this.classProp02_02 = para;
    }
  };
  // @KeepSymbol
  export interface nsInterface02 {
    interfaceProp02: string;
  };
  export interface nsInterface02_01 {
    // @KeepSymbol
    interfaceProp02_01: string;
  };
  // @KeepSymbol
  export enum nsEnum02 {
    RED02
  }
  export enum nsEnum02_01 {
    // @KeepSymbol
    RED02_01
  }
  // @KeepSymbol
  export namespace InnerNamespace02 {
    export const innerConstValue02 = 1;
  }
  // @KeepSymbol
  export type nsType02 = "A";
  const noExportedValue02 = 1;
}

namespace MyNamespace03 {
  // @KeepSymbol
  const nsConstValue03 = 1;
  // @KeepSymbol
  function nsFunction03(): void {};
  // @KeepSymbol
  class nsClass03 {
    classProp03 = 1;
  };
  class nsClass03_01 {
    classProp03_01 = 1;
    // @KeepSymbol
    constructor(para: number) {
      this.classProp03_01 = para;
    }
  };
  class nsClass03_02 {
    // @KeepSymbol
    classProp03_02 = 1;
    constructor(para: number) {
      this.classProp03_02 = para;
    }
  };
  // @KeepSymbol
  interface nsInterface03 {
    interfaceProp03: string;
  };
  interface nsInterface03_01 {
    // @KeepSymbol
    interfaceProp03_01: string;
  };
  // @KeepSymbol
  enum nsEnum03 {
    RED03
  }
  enum nsEnum03_01 {
    // @KeepSymbol
    RED03_01
  }
  // @KeepSymbol
  namespace InnerNamespace03 {
    const innerConstValue03 = 1;
  }
  // @KeepSymbol
  type nsType03 = "A";
  const noExportedValue03 = 1;
}

// @KeepSymbol
export namespace MyNamespace04 {
  export namespace MyNamespace05 {
    export namespace MyNamespace06 {
      namespace MyNamespace07 {
        export class MyNamespace07Class {}
      }
      // @KeepSymbol
      namespace MyNamespace08 {
        export class MyNamespace08Class {}
      }
      export namespace MyNamespace09 {
        export class MyNamespace09Class {}
      }
    }
  }
}