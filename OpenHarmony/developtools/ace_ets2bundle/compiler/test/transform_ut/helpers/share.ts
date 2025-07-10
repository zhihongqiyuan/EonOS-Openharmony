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
import {
	ProjectConfig,
	ArkProjectConfig
} from './projectConfig';
import {
  Logger
} from './logger';
import {
	CacheStoreManager
} from './cache';
import { 
  OH_MODULES_OHOS_HYPIUM, 
  OH_MODULES_OHPM_HYPIUM 
} from './pathConfig';
import { 
  ARKUI_SUBSYSTEM_CODE, 
  TSC_SYSTEM_CODE 
} from './common';

class Share {
	projectConfig: ProjectConfig;
	symlinkMap = {};
  currentModuleMetaMap = {};

	allComponents?: Map<string, Array<string>>;
  allFiles?: Set<string>;
	cache?: Map<string, object>;
  cacheStoreManager?: CacheStoreManager;

  private _allowedLoggerPrefix: string[] = [];
  private _enableLogger: boolean = false;
  private _arkProjectConfig: ArkProjectConfig;

  get arkProjectConfig(): ArkProjectConfig {
    return this._arkProjectConfig;
  }

  set arkProjectConfig(newVal: ArkProjectConfig) {
    this.projectConfig.concat(newVal);
    this._arkProjectConfig = newVal;
  }

	constructor() {
		this.projectConfig = new ProjectConfig();
	}

  public setEnableLogger(newVal: boolean) {
    this._enableLogger = newVal
  }

  public setAllowedLoggerPrefix(newVal: string[]) {
    this._allowedLoggerPrefix = newVal;
  }

  public getHvigorConsoleLogger(key: string): object {
    return this.getLogger(key);
  }

	public getLogger(prefix: string): object {
    if (!this._enableLogger) {
      return null;
    }

    if (!this._allowedLoggerPrefix.includes(prefix)) {
      return null;
    }

    const logger = Logger.getLogger(prefix);
    if (!logger || logger == undefined) {
      return Logger.createLogger(prefix);
    }
    return logger;
	}

  public flushLogger(): void {
		return Logger.flush();
	}

	public removeLogger(prefix: string): void {
		return Logger.removeLogger(prefix);
	}

	public scan(projectRoot: string, testcase: string, pagePaths: string[] = []) {
    if (!testcase) {
      return;
    }
    this.projectConfig.scan(projectRoot, testcase, pagePaths);
    this.symlinkMap[`${this.projectConfig.projectTopDir}/${OH_MODULES_OHPM_HYPIUM}`] = [
      `${this.projectConfig.projectTopDir}/${OH_MODULES_OHOS_HYPIUM}`
    ];
  }

	public initWithCache(): void {
    this.cache = new Map<string, object>();
    this.cacheStoreManager = undefined;
  }

	public initWithCacheStoreManager(): void {
    this.cache = undefined;
    this.cacheStoreManager = new CacheStoreManager();
  }

  public initWithoutCache(): void {
    this.cache = undefined;
    this.cacheStoreManager = undefined;
  }
}

export {
	Share
}