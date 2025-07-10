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

import { Poolable } from './Poolable';
import { ObjectPool } from './ObjectPool';
import { JArrayList } from './JArrayList';


export default class MPPointD extends Poolable {
  private static pool: ObjectPool<MPPointD> = ObjectPool.create(64, new MPPointD(0, 0)).setReplenishPercentage(0.5);

  public static getInstance(x: number, y: number): MPPointD {
    var result: MPPointD = this.pool.get();
    result.x = x;
    result.y = y;
    return result;
  }

  public static recycleInstance(instance: MPPointD) {
    this.pool.recycle(instance);
  }

  public instantiate(): Poolable {
    return new MPPointD(0, 0);
  }

  public x: number;
  public y: number;

  private constructor(x: number, y: number) {
    super();
    this.x = x;
    this.y = y;
  }


  public toString(): string {
    return 'MPPointD, x: ' + this.x + ', y: ' + this.y;
  }
}
