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

import {
  ARK_COMPILER_META_INFO,
  IS_CACHE_INVALID
} from '../../../../lib/fast_build/ark_compiler/common/ark_define';

class Cache {
  private cacheInfo: Map<string, Object>;

  constructor() {
    this.cacheInfo = new Map<string, Object>();
    this.cacheInfo.set(ARK_COMPILER_META_INFO, {});
    this.cacheInfo.set(IS_CACHE_INVALID, undefined);
  }

  public delete(key: string) { }

  public get(key: string) {
    if (this.cacheInfo.has(key)) {
      return this.cacheInfo.get(key);
    }
    return undefined;
  }

  public has(key: string) { }

  public set(key: string, value: any) {
    if (this.cacheInfo.has(key)) {
      this.cacheInfo.set(key, value);
    }
  }
}

export default Cache;
