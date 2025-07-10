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

// @KeepAsConsumer
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
  // @KeepAsConsumer
  export const nsConstValue02 = 1;
  // @KeepAsConsumer
  export function nsFunction02(): void {};
  // @KeepAsConsumer
  export class nsClass02 {
    classProp02 = 1;
  };
  export class nsClass02_01 {
    classProp02_01 = 1;
    // @KeepAsConsumer
    constructor(para: number) {
      this.classProp02_01 = para;
    }
  };
  export class nsClass02_02 {
    // @KeepAsConsumer
    classProp02_02 = 1;
    constructor(para: number) {
      this.classProp02_02 = para;
    }
  };
  // @KeepAsConsumer
  export interface nsInterface02 {
    interfaceProp02: string;
  };
  export interface nsInterface02_01 {
    // @KeepAsConsumer
    interfaceProp02_01: string;
  };
  // @KeepAsConsumer
  export enum nsEnum02 {
    RED02
  }
  export enum nsEnum02_01 {
    // @KeepAsConsumer
    RED02_01
  }
  // @KeepAsConsumer
  export namespace InnerNamespace02 {
    export const innerConstValue02 = 1;
  }
  // @KeepAsConsumer
  export type nsType02 = "A";
  const noExportedValue02 = 1;
}

namespace MyNamespace03 {
  // @KeepAsConsumer
  const nsConstValue03 = 1;
  // @KeepAsConsumer
  function nsFunction03(): void {};
  // @KeepAsConsumer
  class nsClass03 {
    classProp03 = 1;
  };
  class nsClass03_01 {
    classProp03_01 = 1;
    // @KeepAsConsumer
    constructor(para: number) {
      this.classProp03_01 = para;
    }
  };
  class nsClass03_02 {
    // @KeepAsConsumer
    classProp03_02 = 1;
    constructor(para: number) {
      this.classProp03_02 = para;
    }
  };
  // @KeepAsConsumer
  interface nsInterface03 {
    interfaceProp03: string;
  };
  interface nsInterface03_01 {
    // @KeepAsConsumer
    interfaceProp03_01: string;
  };
  // @KeepAsConsumer
  enum nsEnum03 {
    RED03
  }
  enum nsEnum03_01 {
    // @KeepAsConsumer
    RED03_01
  }
  // @KeepAsConsumer
  namespace InnerNamespace03 {
    const innerConstValue03 = 1;
  }
  // @KeepAsConsumer
  type nsType03 = "A";
  const noExportedValue03 = 1;
}

// @KeepAsConsumer
export namespace MyNamespace04 {
  export namespace MyNamespace05 {
    export namespace MyNamespace06 {
      namespace MyNamespace07 {
        export class MyNamespace07Class {}
      }
      // @KeepAsConsumer
      namespace MyNamespace08 {
        export class MyNamespace08Class {}
      }
      export namespace MyNamespace09 {
        export class MyNamespace09Class {}
      }
    }
  }
}