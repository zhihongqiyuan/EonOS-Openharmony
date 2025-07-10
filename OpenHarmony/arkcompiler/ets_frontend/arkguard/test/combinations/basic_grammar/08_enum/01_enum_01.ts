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

import assert from 'assert'

enum A1 {
  prop1 = 1,
  prop2 = 2
}
assert(A1.prop1 === 1);
assert(A1.prop2 === 2);

enum Direction {
  up = 1,
  down,
  left,
  right,
}
const direction: Direction = Direction.up
assert(direction === 1);
assert(Direction.up === 1);
assert(Direction.down === 2);
assert(Direction.left === 3);
assert(Direction.right === 4);
assert(Direction[1] === 'up');
assert(Direction[3] === 'left');
assert(Direction[4] === 'right');

var A2;
(function (A2) {
    A2[A2["prop1"] = 1] = "prop1";
    A2[A2["prop2"] = 2] = "prop2";
})(A2 || (A2 = {}));
assert(A2.prop1 === 1);
assert(A2.prop2 === 2);
assert(A2[1] === 'prop1');
assert(A2[2] === 'prop2');

let val =1;
enum A3 {
  prop1 = 1,
  prop2 = prop1 + val + 1,
}
assert(A3.prop1 === 1);
assert(A3.prop2 === 3);
assert(A3[1] === 'prop1');

enum Foo {
  a = 2,
  b = 3,
}
assert(Foo.a === 2);
assert(Foo.b === 3);
assert(Foo[2] === 'a');
assert(Foo[3] === 'b');

enum Bar {
  a = (1).valueOf(),
  b = Foo.a,
  c = Foo.b.valueOf(),
}
assert(Bar.a === 1);
assert(Bar.b === 2);
assert(Bar.c === 3);
assert(Bar[1] === 'a');
assert(Bar[2] === 'b');
assert(Bar[3] === 'c');

module M {
  export namespace N {
    export enum E1 {
      a = 1,
    }
  }
}
assert(M.N.E1.a === 1);
assert(M.N.E1[1] === 'a');

module M {
  export namespace N {
    export enum E1 {
      b = M.N.E1.a + 1,
    }
  }
}
assert(M.N.E1.b === 2);
assert(M.N.E1[2] === 'b');

export enum MouseButton {
  LEFT_BUTTON = 1,
  RIGHT_BUTTON = 2,
  MIDDLE_BUTTON = 4,
  XBUTTON1_BUTTON = 5,
  XBUTTON2_BUTTON = 6,
  NO_BUTTON = 0,
}

export const DOMMouseButton = {
  '-1': MouseButton.NO_BUTTON,
  '0': MouseButton.LEFT_BUTTON,
  '1': MouseButton.MIDDLE_BUTTON,
  '2': MouseButton.RIGHT_BUTTON,
  '3': MouseButton.XBUTTON1_BUTTON,
  '4': MouseButton.XBUTTON2_BUTTON,
}
assert(DOMMouseButton['-1'] === 0)
assert(DOMMouseButton['0'] === 1)
assert(DOMMouseButton['1'] === 4)
assert(DOMMouseButton['2'] === 2)
assert(DOMMouseButton['3'] === 5)
assert(DOMMouseButton['4'] === 6)

export enum Foo2 {
  A = 1 << 1,
  B = 1 << 2,
}
assert(Foo2.A === 2);
assert(Foo2.B === 4);
assert(Foo2[2] === 'A');
assert(Foo2[4] === 'B');