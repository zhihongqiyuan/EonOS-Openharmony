/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

import { ObjectPool } from './ObjectPool';
import { Poolable } from './Poolable';

export default class MPPointF extends Poolable {
  private static pool: ObjectPool<MPPointF> = ObjectPool.create(32, new MPPointF(0, 0)).setReplenishPercentage(0.5);
  public x: number;
  public y: number;

  public constructor(x?: number, y?: number) {
    super();
    this.x = x;
    this.y = y;
  }

  public static getInstance(x?: number, y?: number, copy?: MPPointF): MPPointF {
    if (copy != null) {
      let result: MPPointF = this.pool.get();
      result.x = copy.x;
      result.y = copy.y;
      return result;
    }
    if (x != null && y != null) {
      let result: MPPointF = this.pool.get();
      result.x = x;
      result.y = y;
      return result;
    }
    return this.pool.get();
  }

  public static recycleInstance(instance: MPPointF) {
    this.pool.recycle(instance);
  }

  public static recycleInstances(instances: Array<MPPointF>) {
    MPPointF.pool.recycleArray(instances);
  }

  public getX(): number {
    return this.x;
  }

  public getY(): number {
    return this.y;
  }

  instantiate(): Poolable {
    return new MPPointF(0, 0);
  }
}
