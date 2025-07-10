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

import {describe, it} from 'mocha';
import {assert} from 'chai';
import {getNameGenerator, NameGeneratorType} from '../../src/generator/NameFactory';
import {NameGeneratorOptions} from '../../src/generator/INameGenerator';

const orderedName = [
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z', 'a1', 'b1', 'c1', 'd1',
  'e1', 'f1', 'g1', 'h1', 'i1', 'j1', 'k1', 'l1', 'm1', 'n1',
  'o1', 'p1', 'q1', 'r1', 's1', 't1', 'u1', 'v1', 'w1', 'x1',
  'y1', 'z1', 'a2', 'b2', 'c2', 'd2', 'e2', 'f2', 'g2', 'h2',
  'i2', 'j2', 'k2', 'l2', 'm2', 'n2', 'o2', 'p2', 'q2', 'r2',
  's2', 't2', 'u2', 'v2', 'w2', 'x2', 'y2', 'z2', 'a3', 'b3',
  'c3', 'd3', 'e3', 'f3', 'g3', 'h3', 'i3', 'j3', 'k3', 'l3',
  'm3', 'n3', 'o3', 'p3', 'q3', 'r3', 's3', 't3', 'u3', 'v3',
  'w3', 'x3', 'y3', 'z3'
];

describe("test for name generator", function () {
  describe('ordered name generator test', function () {
    it('ordered name generator check', function () {
      const orderedGenerator = getNameGenerator(NameGeneratorType.ORDERED);

      for (let i = 0; i < 104; i++) {
        assert.strictEqual(orderedGenerator.getName(), orderedName[i]);
      }
    });
  });
});
