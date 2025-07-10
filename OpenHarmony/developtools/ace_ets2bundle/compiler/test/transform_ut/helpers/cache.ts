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
class Cache {
  private cacheInfo: Map<string, Object>;

  constructor() {
    this.cacheInfo = new Map<string, Object>();
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

class CacheInCacheStoreManager {
	cache: Map<string, object>;

	constructor() {
		this.cache = new Map<string, object>();
	}

	public getCache(key: string): object {
		return this.cache.get(key);
	}

	public setCache(key: string, value: object): void {
		this.cache.set(key, value);
	}
}

class CacheStoreManager {
	cacheInCacheStoreManager: Map<string, CacheInCacheStoreManager>;

	constructor() {
		this.cacheInCacheStoreManager = new Map<string, CacheInCacheStoreManager>();
	}

	public mount(cacheStoreManagerKey: string): CacheInCacheStoreManager {
		let cacheInCacheStoreManagerValue: CacheInCacheStoreManager | undefined =
			this.cacheInCacheStoreManager.get(cacheStoreManagerKey);

		if (!cacheInCacheStoreManagerValue) {
			cacheInCacheStoreManagerValue = new CacheInCacheStoreManager();
			this.cacheInCacheStoreManager.set(cacheStoreManagerKey, cacheInCacheStoreManagerValue);
		}

		return cacheInCacheStoreManagerValue;
	}

	public unmount(cacheStoreManagerKey: string): void {
		this.cacheInCacheStoreManager?.delete(cacheStoreManagerKey);
	}

	public keys(): IterableIterator<string> {
		return this.cacheInCacheStoreManager?.keys();
	}
}

export {
	Cache,
	CacheInCacheStoreManager,
	CacheStoreManager
}