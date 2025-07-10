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
  ProjectConfig,
  IArkProjectConfig
} from "./project_config";
import {
  OH_MODULES_OHPM_HYPIUM,
  OH_MODULES_OHOS_HYPIUM,
  MOCK_CONFIG_PATH
} from "./path_config";

class Logger {
  private prefix: string;
  private messsage: string;
  static instances = [];

  constructor(prefix: string) {
    this.prefix = prefix;
  }

  public debug(color: string, msg: string, reset: string) {
    console.debug(`${color}${this.prefix}: ${JSON.stringify(msg)}${reset}`);
  }

  public warn(color: string, msg: string, reset: string) {
    console.warn(`${color}${this.prefix}: ${JSON.stringify(msg)}${reset}`);
  }

  public error(color: string, errormsg: string, reset: string) {
    this.messsage = errormsg.toString();
  }

  public warn(color: string, msg: string) {
    this.messsage = color.toString();
  }

  public getPrefix() {
    return this.prefix;
  }

  public static getLogger(prefix): object {
    for (const instance of Logger.instances) {
      if (instance.getPrefix() == prefix) {
        return instance;
      }
    }
  }

  public static createLogger(prefix) {
    const logger = new Logger(prefix);
    Logger.instances.push(logger);
    return logger;
  }
}

class HvigorConsoleLogger {

  private prefix: string;
  private messsage: string;
  static instances = [];

  constructor(prefix: string) {
    this.prefix = prefix;
  }

  public printError(error: object): void {
    this.messsage = error.code.toString();
  }

  public printErrorAndExit(error: object): void {
    this.messsage = error.code.toString();
  }

  public getPrefix(): string {
    return this.prefix;
  }

  public static getLogger(prefix): Object {
    for (const instance of HvigorConsoleLogger.instances) {
      if (instance.getPrefix() == prefix) {
        return instance;
      }
    }
  }

  public static createLogger(prefix): HvigorConsoleLogger {
    const logger = new HvigorConsoleLogger(prefix);
    HvigorConsoleLogger.instances.push(logger);
    return logger;
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

class Share {
  projectConfig: ProjectConfig;
  arkProjectConfig: IArkProjectConfig;
  symlinkMap = {};
  currentModuleMetaMap = {};

  allComponents?: Map<string, Array<string>>;
  allFiles?: Set<string>;
  cache?: Map<string, object>;
  cacheStoreManager?: CacheStoreManager;

  constructor(buildMode: string) {
    this.projectConfig = new ProjectConfig(buildMode);
  }

  public throwArkTsCompilerError(error: object) {
    console.error(JSON.stringify(error));
  }

  public getLogger(prefix: string): Logger {
    const logger = Logger.getLogger(prefix);
    if (!logger || logger == undefined) {
      return Logger.createLogger(prefix);
    }
    return logger;
  }

  public getHvigorConsoleLogger(prefix: string): HvigorConsoleLogger {
    const logger = HvigorConsoleLogger.getLogger(prefix);
    if (!logger || logger == undefined) {
      return HvigorConsoleLogger.createLogger(prefix);
    }
    return logger;
  }

  public importResolver (moduleRequest: string): Object | undefined {
    if (moduleRequest === "@ohos/library////\\\\/") {
      return {
        normalizedPath: "@ohos/library/Index",
        packageName: "@ohos/library"
      }
    }
    if (moduleRequest === "@ohos/library/src/main/ets////") {
      return {
        normalizedPath: "@ohos/library/src/main/ets/Index",
        packageName: "@ohos/library"
      }
    }
    if (moduleRequest === "bytecode_alias_oh///\\\/") {
      return {
        normalizedPath: "bytecode_alias_oh/Index",
        packageName: "bytecode_alias_oh"
      }
    }
    return undefined;
  }
  public scan(testcase: string) {
    if (!testcase) {
      return;
    }
    this.projectConfig.scan(testcase);
    this.symlinkMap[`${this.projectConfig.projectTopDir}/${OH_MODULES_OHPM_HYPIUM}`] = [
      `${this.projectConfig.projectTopDir}/${OH_MODULES_OHOS_HYPIUM}`
    ];
  }

  public setMockParams() {
    this.projectConfig.setMockParams({ mockConfigPath: MOCK_CONFIG_PATH });
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

export default Share;
