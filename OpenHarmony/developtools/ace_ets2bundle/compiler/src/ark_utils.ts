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

import path from 'path';
import fs from 'fs';
import type sourceMap from 'source-map';

import { minify, MinifyOutput } from 'terser';
import {
  endFilesEvent,
  endSingleFileEvent,
  EventList,
  MemoryUtils,
  nameCacheMap,
  performancePrinter,
  ProjectCollections,
  startFilesEvent,
  startSingleFileEvent,
} from 'arkguard';
import type { HvigorErrorInfo } from 'arkguard';
import {
  OH_MODULES,
  SEPARATOR_AT,
  SEPARATOR_BITWISE_AND,
  SEPARATOR_SLASH
} from './fast_build/ark_compiler/common/ark_define';
import {
  ARKTS_MODULE_NAME,
  PACKAGES,
  TEMPORARY,
  ZERO,
  ONE,
  EXTNAME_JS,
  EXTNAME_TS,
  EXTNAME_MJS,
  EXTNAME_CJS,
  EXTNAME_ABC,
  EXTNAME_ETS,
  EXTNAME_TS_MAP,
  EXTNAME_JS_MAP,
  ESMODULE,
  FAIL,
  TS2ABC,
  ES2ABC,
  EXTNAME_PROTO_BIN,
  NATIVE_MODULE
} from './pre_define';
import {
  isMac,
  isWindows,
  isPackageModulesFile,
  genTemporaryPath,
  getExtensionIfUnfullySpecifiedFilepath,
  mkdirsSync,
  toUnixPath,
  validateFilePathLength,
  harFilesRecord,
  getProjectRootPath
} from './utils';
import type { GeneratedFileInHar } from './utils';
import {
  extendSdkConfigs,
  projectConfig,
  sdkConfigPrefix
} from '../main';
import {
  getRelativeSourcePath,
  mangleFilePath,
  setNewNameCache,
  getNameCacheByPath,
  setUnobfuscationNames,
  writeObfuscatedFile
} from './fast_build/ark_compiler/common/ob_config_resolver';
import { moduleRequestCallback } from './fast_build/system_api/api_check_utils';
import { SourceMapGenerator } from './fast_build/ark_compiler/generate_sourcemap';
import { sourceFileBelongProject } from './fast_build/ark_compiler/module/module_source_file';
import { MemoryMonitor } from './fast_build/meomry_monitor/rollup-plugin-memory-monitor';
import { MemoryDefine } from './fast_build/meomry_monitor/memory_define';
import {
  ArkTSInternalErrorDescription,
  ArkTSErrorDescription,
  ErrorCode
} from './fast_build/ark_compiler/error_code';
import {
  LogData,
  LogDataFactory
} from './fast_build/ark_compiler/logger';
import * as ts from 'typescript';

export const SRC_MAIN: string = 'src/main';

export let newSourceMaps: Object = {};

export const packageCollection: Map<string, Array<string>> = new Map();
// Splicing ohmurl or record name based on filePath and context information table.
export function getNormalizedOhmUrlByFilepath(filePath: string, projectConfig: Object, logger: Object,
  pkgParams: Object, importerFile: string): string {
  const { pkgName, pkgPath, isRecordName } = pkgParams;
  // rollup uses commonjs plugin to handle commonjs files,
  // the commonjs files are prefixed with '\x00' and need to be removed.
  if (filePath.startsWith('\x00')) {
    filePath = filePath.replace('\x00', '');
  }
  let unixFilePath: string = toUnixPath(filePath);
  unixFilePath = unixFilePath.substring(0, filePath.lastIndexOf('.')); // remove extension
  // case1: /entry/src/main/ets/xxx/yyy
  // case2: /entry/src/ohosTest/ets/xxx/yyy
  // case3: /node_modules/xxx/yyy
  // case4: /entry/node_modules/xxx/yyy
  // case5: /library/node_modules/xxx/yyy
  // case6: /library/index.ts
  // ---> @normalized:N&<moduleName>&<bunldName>&<packageName>/entry/ets/xxx/yyy&<version>
  let pkgInfo = projectConfig.pkgContextInfo[pkgName];
  if (!pkgInfo || !pkgPath) {
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_FAILED_TO_RESOLVE_OHM_URL,
      ArkTSErrorDescription,
      'Failed to resolve OhmUrl. ' +
      `Failed to get a resolved OhmUrl for "${filePath}" imported by "${importerFile}".`,
      '',
      [`Check whether the "${pkgName}" module which ${filePath} belongs to is correctly configured.`,
       'Check the corresponding file name is correct(including case-sensitivity).']
    );
    logger.printError(errInfo);
    return filePath;
  }
  let projectFilePath: string = unixFilePath.replace(toUnixPath(pkgPath), '');
  let recordName = `${pkgInfo.bundleName}&${pkgName}${projectFilePath}&${pkgInfo.version}`;
  if (isRecordName) {
    // record name style: <bunldName>&<packageName>/entry/ets/xxx/yyy&<version>
    return recordName;
  }
  return `${pkgInfo.isSO ? 'Y' : 'N'}&${pkgInfo.moduleName}&${recordName}`;
}

export function getOhmUrlByFilepath(filePath: string, projectConfig: Object, logger: Object, namespace?: string,
  importerFile?: string): string {
  // remove '\x00' from the rollup virtual commonjs file's filePath
  if (filePath.startsWith('\x00')) {
    filePath = filePath.replace('\x00', '');
  }
  let unixFilePath: string = toUnixPath(filePath);
  unixFilePath = unixFilePath.substring(0, filePath.lastIndexOf('.')); // remove extension
  const REG_PROJECT_SRC: RegExp = /(\S+)\/src\/(?:main|ohosTest)\/(ets|js|mock)\/(\S+)/;

  const packageInfo: string[] = getPackageInfo(projectConfig.aceModuleJsonPath);
  const bundleName: string = packageInfo[0];
  const moduleName: string = packageInfo[1];
  const moduleRootPath: string = toUnixPath(projectConfig.modulePathMap[moduleName]);
  const projectRootPath: string = toUnixPath(getProjectRootPath(filePath, projectConfig, projectConfig?.rootPathSet));
  // case1: /entry/src/main/ets/xxx/yyy     ---> @bundle:<bundleName>/entry/ets/xxx/yyy
  // case2: /entry/src/ohosTest/ets/xxx/yyy ---> @bundle:<bundleName>/entry_test@entry/ets/xxx/yyy
  // case3: /node_modules/xxx/yyy           ---> @package:pkg_modules/xxx/yyy
  // case4: /entry/node_modules/xxx/yyy     ---> @package:pkg_modules@entry/xxx/yyy
  // case5: /library/node_modules/xxx/yyy   ---> @package:pkg_modules@library/xxx/yyy
  // case6: /library/index.ts               ---> @bundle:<bundleName>/library/index
  const projectFilePath: string = unixFilePath.replace(projectRootPath, '');
  const packageDir: string = projectConfig.packageDir;
  const result: RegExpMatchArray | null = projectFilePath.match(REG_PROJECT_SRC);
  if (result && result[1].indexOf(packageDir) === -1) {
    const relativePath = processSrcMain(result, projectFilePath);
    if (namespace && moduleName !== namespace) {
      return `${bundleName}/${moduleName}@${namespace}/${relativePath}`;
    }
    return `${bundleName}/${moduleName}/${relativePath}`;
  }

  const processParams: Object = {
    projectFilePath,
    unixFilePath,
    packageDir,
    projectRootPath,
    moduleRootPath,
    projectConfig,
    namespace,
    logger,
    importerFile,
    originalFilePath: filePath
  };
  return processPackageDir(processParams);
}

function processSrcMain(result: RegExpMatchArray | null, projectFilePath: string): string {
  let langType: string = result[2];
  let relativePath: string = result[3];
  // case7: /entry/src/main/ets/xxx/src/main/js/yyy ---> @bundle:<bundleName>/entry/ets/xxx/src/main/js/yyy
  const REG_SRC_MAIN: RegExp = /src\/(?:main|ohosTest)\/(ets|js)\//;
  const srcMainIndex: number = result[1].search(REG_SRC_MAIN);
  if (srcMainIndex !== -1) {
    relativePath = projectFilePath.substring(srcMainIndex).replace(REG_SRC_MAIN, '');
    langType = projectFilePath.replace(relativePath, '').match(REG_SRC_MAIN)[1];
  }
  return `${langType}/${relativePath}`;
}

function processPackageDir(params: Object): string {
  const { projectFilePath, unixFilePath, packageDir, projectRootPath, moduleRootPath,
    projectConfig, namespace, logger, importerFile, originalFilePath } = params;
  if (projectFilePath.indexOf(packageDir) !== -1) {
    if (compileToolIsRollUp()) {
      const tryProjectPkg: string = toUnixPath(path.join(projectRootPath, packageDir));
      if (unixFilePath.indexOf(tryProjectPkg) !== -1) {
        return unixFilePath.replace(tryProjectPkg, `${packageDir}`).replace(new RegExp(packageDir, 'g'), PACKAGES);
      }

      // iterate the modulePathMap to find the module which contains the pkg_module's file
      for (const moduleName in projectConfig.modulePathMap) {
        const modulePath: string = projectConfig.modulePathMap[moduleName];
        const tryModulePkg: string = toUnixPath(path.resolve(modulePath, packageDir));
        if (unixFilePath.indexOf(tryModulePkg) !== -1) {
          return unixFilePath.replace(tryModulePkg, `${packageDir}@${moduleName}`).replace(new RegExp(packageDir, 'g'), PACKAGES);
        }
      }

      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.ETS2BUNDLE_EXTERNAL_FAILED_TO_RESOLVE_OHM_URL,
        ArkTSErrorDescription,
        'Failed to resolve OhmUrl. ' +
        `Failed to get a resolved OhmUrl for "${originalFilePath}" imported by "${importerFile}".`,
        '',
        [`Check whether the module which ${originalFilePath} belongs to is correctly configured.`,
         'Check the corresponding file name is correct(including case-sensitivity).']
      );
      logger.printError(errInfo);
      return originalFilePath;
    }

    // webpack with old implematation
    const tryProjectPkg: string = toUnixPath(path.join(projectRootPath, packageDir));
    if (unixFilePath.indexOf(tryProjectPkg) !== -1) {
      return unixFilePath.replace(tryProjectPkg, `${packageDir}/${ONE}`).replace(new RegExp(packageDir, 'g'), PACKAGES);
    }

    const tryModulePkg: string = toUnixPath(path.join(moduleRootPath, packageDir));
    if (unixFilePath.indexOf(tryModulePkg) !== -1) {
      return unixFilePath.replace(tryModulePkg, `${packageDir}/${ZERO}`).replace(new RegExp(packageDir, 'g'), PACKAGES);
    }
  }

  const packageInfo: string[] = getPackageInfo(projectConfig.aceModuleJsonPath);
  const bundleName: string = packageInfo[0];
  const moduleName: string = packageInfo[1];
  for (const key in projectConfig.modulePathMap) {
    const moduleRootPath: string = toUnixPath(projectConfig.modulePathMap[key]);
    if (unixFilePath.indexOf(moduleRootPath + '/') !== -1) {
      const relativeModulePath: string = unixFilePath.replace(moduleRootPath + '/', '');
      if (namespace && moduleName !== namespace) {
        return `${bundleName}/${moduleName}@${namespace}/${relativeModulePath}`;
      }
      return `${bundleName}/${moduleName}/${relativeModulePath}`;
    }
  }

  const errInfo: LogData = LogDataFactory.newInstance(
    ErrorCode.ETS2BUNDLE_EXTERNAL_FAILED_TO_RESOLVE_OHM_URL,
    ArkTSErrorDescription,
    'Failed to resolve OhmUrl. ' +
    `Failed to get a resolved OhmUrl for "${originalFilePath}" imported by "${importerFile}".`,
    '',
    [`Check whether the module which ${originalFilePath} belongs to is correctly configured.`,
     'Check the corresponding file name is correct(including case-sensitivity).']
  );
  logger.printError(errInfo);
  return originalFilePath;
}


export function getOhmUrlBySystemApiOrLibRequest(moduleRequest: string, config?: Object, logger?: Object,
  importerFile?: string, useNormalizedOHMUrl: boolean = false): string {
  // 'arkui-x' represents cross platform related APIs, processed as 'ohos'
  const REG_SYSTEM_MODULE: RegExp = new RegExp(`@(${sdkConfigPrefix})\\.(\\S+)`);
  const REG_LIB_SO: RegExp = /lib(\S+)\.so/;

  if (REG_SYSTEM_MODULE.test(moduleRequest.trim())) {
    return moduleRequest.replace(REG_SYSTEM_MODULE, (_, moduleType, systemKey) => {
      let moduleRequestStr = '';
      if (extendSdkConfigs) {
        moduleRequestStr = moduleRequestCallback(moduleRequest, _, moduleType, systemKey);
      }
      if (moduleRequestStr !== '') {
        return moduleRequestStr;
      }
      const systemModule: string = `${moduleType}.${systemKey}`;
      if (NATIVE_MODULE.has(systemModule)) {
        return `@native:${systemModule}`;
      } else if (moduleType === ARKTS_MODULE_NAME) {
        // @arkts.xxx -> @ohos:arkts.xxx
        return `@ohos:${systemModule}`;
      } else {
        return `@ohos:${systemKey}`;
      };
    });
  }
  if (REG_LIB_SO.test(moduleRequest.trim())) {
    if (useNormalizedOHMUrl) {
      const pkgInfo = config.pkgContextInfo[moduleRequest];
      if (!pkgInfo) {
        const errInfo: LogData = LogDataFactory.newInstance(
          ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_GET_PKG_CONTENT_INFO,
          ArkTSInternalErrorDescription,
          `Can not get pkgContextInfo of package '${moduleRequest}' ` +
          `which being imported by '${importerFile}'`
        );
        logger?.printError(errInfo);
        return moduleRequest;
      }
      const isSo = pkgInfo.isSO ? 'Y' : 'N';
      return `@normalized:${isSo}&${pkgInfo.moduleName}&${pkgInfo.bundleName}&${moduleRequest}&${pkgInfo.version}`;
    }
    return moduleRequest.replace(REG_LIB_SO, (_, libsoKey) => {
      return `@app:${projectConfig.bundleName}/${projectConfig.moduleName}/${libsoKey}`;
    });
  }
  return undefined;
}

export function genSourceMapFileName(temporaryFile: string): string {
  let abcFile: string = temporaryFile;
  if (temporaryFile.endsWith(EXTNAME_TS)) {
    abcFile = temporaryFile.replace(/\.ts$/, EXTNAME_TS_MAP);
  } else {
    abcFile = temporaryFile.replace(/\.js$/, EXTNAME_JS_MAP);
  }
  return abcFile;
}

export function getBuildModeInLowerCase(projectConfig: Object): string {
  return (compileToolIsRollUp() ? projectConfig.buildMode : projectConfig.buildArkMode).toLowerCase();
}

/**
 * This Api only used by webpack compiling process - js-loader
 * @param sourcePath The path in build cache dir
 * @param sourceCode The intermediate js source code
 */
export function writeFileSyncByString(sourcePath: string, sourceCode: string, projectConfig: Object, logger: Object): void {
  const filePath: string = genTemporaryPath(sourcePath, projectConfig.projectPath, process.env.cachePath,
    projectConfig, undefined);
  if (filePath.length === 0) {
    return;
  }
  mkdirsSync(path.dirname(filePath));
  if (/\.js$/.test(sourcePath)) {
    sourceCode = transformModuleSpecifier(sourcePath, sourceCode, projectConfig);
    if (projectConfig.buildArkMode === 'debug') {
      fs.writeFileSync(filePath, sourceCode);
      return;
    }
    writeObfuscatedSourceCode({content: sourceCode, buildFilePath: filePath, relativeSourceFilePath: ''},
      logger, projectConfig);
  }
  if (/\.json$/.test(sourcePath)) {
    fs.writeFileSync(filePath, sourceCode);
  }
}

export function transformModuleSpecifier(sourcePath: string, sourceCode: string, projectConfig: Object): string {
  // replace relative moduleSpecifier with ohmURl
  const REG_RELATIVE_DEPENDENCY: RegExp = /(?:import|from)(?:\s*)['"]((?:\.\/|\.\.\/)[^'"]+|(?:\.\/?|\.\.\/?))['"]/g;
  const REG_HAR_DEPENDENCY: RegExp = /(?:import|from)(?:\s*)['"]([^\.\/][^'"]+)['"]/g;
  // replace requireNapi and requireNativeModule with import
  const REG_REQUIRE_NATIVE_MODULE: RegExp = /var (\S+) = globalThis.requireNativeModule\(['"](\S+)['"]\);/g;
  const REG_REQUIRE_NAPI_APP: RegExp = /var (\S+) = globalThis.requireNapi\(['"](\S+)['"], true, ['"](\S+)['"]\);/g;
  const REG_REQUIRE_NAPI_OHOS: RegExp = /var (\S+) = globalThis.requireNapi\(['"](\S+)['"]\);/g;

  return sourceCode.replace(REG_HAR_DEPENDENCY, (item, moduleRequest) => {
    return replaceHarDependency(item, moduleRequest, projectConfig);
  }).replace(REG_RELATIVE_DEPENDENCY, (item, moduleRequest) => {
    return replaceRelativeDependency(item, moduleRequest, toUnixPath(sourcePath), projectConfig);
  }).replace(REG_REQUIRE_NATIVE_MODULE, (_, moduleRequest, moduleName) => {
    return `import ${moduleRequest} from '@native:${moduleName}';`;
  }).replace(REG_REQUIRE_NAPI_APP, (_, moduleRequest, soName, moudlePath) => {
    return `import ${moduleRequest} from '@app:${moudlePath}/${soName}';`;
  }).replace(REG_REQUIRE_NAPI_OHOS, (_, moduleRequest, moduleName) => {
    return `import ${moduleRequest} from '@ohos:${moduleName}';`;
  });
}

function removeSuffix(filePath: string): string {
  const SUFFIX_REG = /\.(?:d\.)?(ets|ts|mjs|cjs|js)$/;
  return filePath.split(path.sep).join('/').replace(SUFFIX_REG, '');
}

export function getNormalizedOhmUrlByModuleRequest(moduleInfoByModuleRequest: Object, projectConfig: Object,
  logger?: Object): string {
  const normalizedPath = moduleInfoByModuleRequest.normalizedPath;
  const pkgName = moduleInfoByModuleRequest.packageName;
  const pkgInfo: Object = projectConfig.pkgContextInfo[pkgName];
  if (!normalizedPath || !pkgName || !pkgInfo) {
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_PACKAGE_NOT_FOUND_IN_CONTEXT_INFO,
      ArkTSInternalErrorDescription,
      `Failed to find package '${pkgName}'. ` +
      `Failed to obtain package '${pkgName}' from the package context information.`
    );
    logger?.printError(errInfo);
    return normalizedPath;
  }
  const isSo = pkgInfo.isSO ? 'Y' : 'N';
  return `@normalized:${isSo}&${pkgInfo.moduleName}&${pkgInfo.bundleName}&${toUnixPath(normalizedPath)}&${pkgInfo.version}`;
}

export function getNormalizedOhmUrlByAliasName(aliasName: string, projectConfig: Object,
  logger?: Object, filePath?: string): string {
  let pkgName: string = aliasName;
  const aliasPkgNameMap: Map<string, string> = projectConfig.dependencyAliasMap;
  if (aliasPkgNameMap.has(aliasName)) {
    pkgName = aliasPkgNameMap.get(aliasName);
  }
  const pkgInfo: Object = projectConfig.pkgContextInfo[pkgName];
  if (!pkgInfo) {
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_PACKAGE_NOT_FOUND_IN_CONTEXT_INFO,
      ArkTSInternalErrorDescription,
      `Failed to find package '${pkgName}'. ` +
      `Failed to obtain package '${pkgName}' from the package context information.`
    );
    logger.printError(errInfo);
  }
  let normalizedPath: string = '';
  if (!filePath) {
    if (!pkgInfo.entryPath) {
      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.ETS2BUNDLE_INTERNAL_PACKAGE_ENTRY_FILE_NOT_FOUND,
        ArkTSInternalErrorDescription,
        `Failed to find entry file of '${pkgName}'. ` +
        `Failed to obtain the entry file information of '${pkgName}' from the package context information.`
      );
      logger.printError(errInfo);
    }
    normalizedPath = `${pkgName}/${toUnixPath(pkgInfo.entryPath)}`;
    normalizedPath = removeSuffix(normalizedPath);
  } else {
    const relativePath = toUnixPath(filePath).replace(aliasName, '');
    normalizedPath = `${pkgName}${relativePath}`;
  }
  const isSo = pkgInfo.isSO ? 'Y' : 'N';
  return `@normalized:${isSo}&${pkgInfo.moduleName}&${pkgInfo.bundleName}&${normalizedPath}&${pkgInfo.version}`;
}

export function getOhmUrlByByteCodeHar(moduleRequest: string, projectConfig: Object, rollupObject: Object, logger?: Object):
  string | undefined {
  if (projectConfig.byteCodeHarInfo) {
    const moduleInfoByModuleRequest: Object = rollupObject.share?.importResolver?.(moduleRequest);
    if (moduleInfoByModuleRequest) {
       return getNormalizedOhmUrlByModuleRequest(moduleInfoByModuleRequest, projectConfig, logger);
    }
    let aliasName: string = getAliasNameFromPackageMap(projectConfig.byteCodeHarInfo, moduleRequest);
    if (aliasName) {
      return getNormalizedOhmUrlByAliasName(aliasName, projectConfig, logger);
    }
    for (const byteCodeHarName in projectConfig.byteCodeHarInfo) {
      if (moduleRequest.startsWith(byteCodeHarName + '/')) {
        return getNormalizedOhmUrlByAliasName(byteCodeHarName, projectConfig, logger, moduleRequest);
      }
    }
  }
  return undefined;
}

function getAliasNameFromPackageMap(externalPkgMap: Object, moduleRequest: string): string | undefined {
  // Matches strings that contain zero or more `/` or `\` characters
  const ONLY_SLASHES_REGEX: RegExp = /^\/*$|^\\*$/;
  const keys: string[] = Object.keys(externalPkgMap);
  for (const key of keys) {
    if (moduleRequest === key) {
      return key;
    }
    // In the following cases, the moduleRequest matches the aliasName field in the packageMap
    // case1: key = "hsp", moduleRequest = "hsp/"
    // case2: key = "hsp", moduleRequest = "hsp\\"
    if (moduleRequest.length > key.length && moduleRequest.startsWith(key)) {
      const remaining: string = moduleRequest.replace(key, '');
      if (ONLY_SLASHES_REGEX.test(remaining)) {
        return key;
      }
    }
  }
  return undefined;
}

export function getOhmUrlByExternalPackage(moduleRequest: string, projectConfig: Object, rollupObject?: Object, logger?: Object,
  useNormalizedOHMUrl: boolean = false): string | undefined {
  // The externalPkgMap store the ohmurl with the alias of hsp package and the hars depended on bytecode har.
  let externalPkgMap: Object = Object.assign({}, projectConfig.hspNameOhmMap, projectConfig.harNameOhmMap);
  if (Object.keys(externalPkgMap).length !== 0) {
    const moduleInfoByModuleRequest: Object = rollupObject?.share?.importResolver?.(moduleRequest);
    if (useNormalizedOHMUrl && moduleInfoByModuleRequest) {
       return getNormalizedOhmUrlByModuleRequest(moduleInfoByModuleRequest, projectConfig, logger);
    }
    let aliasName: string = getAliasNameFromPackageMap(externalPkgMap, moduleRequest);
    if (aliasName) {
      if (useNormalizedOHMUrl) {
        return getNormalizedOhmUrlByAliasName(aliasName, projectConfig, logger);
      }
      // case1: "@ohos/lib" ---> "@bundle:bundleName/lib/ets/index"
      return externalPkgMap[moduleRequest];
    }

    for (const externalPkgName in externalPkgMap) {
      if (moduleRequest.startsWith(externalPkgName + '/')) {
        if (useNormalizedOHMUrl) {
          return getNormalizedOhmUrlByAliasName(externalPkgName, projectConfig, logger, moduleRequest);
        }
        // case2: "@ohos/lib/src/main/ets/pages/page1" ---> "@bundle:bundleName/lib/ets/pages/page1"
        const idx: number = externalPkgMap[externalPkgName].split('/', 2).join('/').length;
        const ohmName: string = externalPkgMap[externalPkgName].substring(0, idx);
        if (moduleRequest.indexOf(externalPkgName + '/' + SRC_MAIN) === 0) {
          return moduleRequest.replace(externalPkgName + '/' + SRC_MAIN, ohmName);
        } else {
          return moduleRequest.replace(externalPkgName, ohmName);
        }
      }
    }
  }
  return undefined;
}

function replaceHarDependency(item: string, moduleRequest: string, projectConfig: Object): string {
  const hspOhmUrl: string | undefined = getOhmUrlByExternalPackage(moduleRequest, projectConfig);
  if (hspOhmUrl !== undefined) {
    return item.replace(/(['"])(?:\S+)['"]/, (_, quotation) => {
      return quotation + hspOhmUrl + quotation;
    });
  }
  return item;
}

function locateActualFilePathWithModuleRequest(absolutePath: string): string {
  if (!fs.existsSync(absolutePath) || !fs.statSync(absolutePath).isDirectory()) {
    return absolutePath;
  }

  const filePath: string = absolutePath + getExtensionIfUnfullySpecifiedFilepath(absolutePath);
  if (fs.existsSync(filePath) && fs.statSync(filePath).isFile()) {
    return absolutePath;
  }

  return path.join(absolutePath, 'index');
}

function replaceRelativeDependency(item: string, moduleRequest: string, sourcePath: string, projectConfig: Object): string {
  if (sourcePath && projectConfig.compileMode === ESMODULE) {
    // remove file extension from moduleRequest
    const SUFFIX_REG: RegExp = /\.(?:[cm]?js|[e]?ts|json)$/;
    moduleRequest = moduleRequest.replace(SUFFIX_REG, '');

    // normalize the moduleRequest
    item = item.replace(/(['"])(?:\S+)['"]/, (_, quotation) => {
      let normalizedModuleRequest: string = toUnixPath(path.normalize(moduleRequest));
      if (moduleRequest.startsWith('./')) {
        normalizedModuleRequest = './' + normalizedModuleRequest;
      }
      return quotation + normalizedModuleRequest + quotation;
    });

    const filePath: string =
      locateActualFilePathWithModuleRequest(path.resolve(path.dirname(sourcePath), moduleRequest));
    const result: RegExpMatchArray | null =
      filePath.match(/(\S+)(\/|\\)src(\/|\\)(?:main|ohosTest)(\/|\\)(ets|js)(\/|\\)(\S+)/);
    if (result && projectConfig.aceModuleJsonPath) {
      const npmModuleIdx: number = result[1].search(/(\/|\\)node_modules(\/|\\)/);
      const projectRootPath: string = projectConfig.projectRootPath;
      if (npmModuleIdx === -1 || npmModuleIdx === projectRootPath.search(/(\/|\\)node_modules(\/|\\)/)) {
        const packageInfo: string[] = getPackageInfo(projectConfig.aceModuleJsonPath);
        const bundleName: string = packageInfo[0];
        const moduleName: string = packageInfo[1];
        moduleRequest = `@bundle:${bundleName}/${moduleName}/${result[5]}/${toUnixPath(result[7])}`;
        item = item.replace(/(['"])(?:\S+)['"]/, (_, quotation) => {
          return quotation + moduleRequest + quotation;
        });
      }
    }
  }
  return item;
}

/**
 * Informantion of build files
 */
export interface ModuleInfo {
  content: string,
  /**
   * the path in build cache dir
   */
  buildFilePath: string,
  /**
   * the `originSourceFilePath` relative to project root dir.
   */
  relativeSourceFilePath: string,
  /**
   * the origin source file path will be set with rollup moduleId when obfuscate intermediate js source code,
   * whereas be set with tsc node.fileName when obfuscate intermediate ts source code.
   */
  originSourceFilePath?: string,
  rollupModuleId?: string
}

export async function writeObfuscatedSourceCode(moduleInfo: ModuleInfo, logger: Function | Object,
  projectConfig: Object, rollupNewSourceMaps: Object = {}): Promise<void> {
  /**
   * Obfuscation mode rules:
   * - When isArkguardEnabled=true (source code obfuscation): Obfuscate ALL files
   * - When isBytecodeObfEnabled=true (bytecode obfuscation): Only decl files will be obfuscated
   * - These two flags are mutually exclusive (only one can be true at a time)
   */
  let shouldSkipObfuscation = false;
  if (projectConfig.isBytecodeObfEnabled) {
    shouldSkipObfuscation = !/\.d\.e?ts$/.test(moduleInfo.buildFilePath);
  }

  const isObfuscatorEnabled = projectConfig.arkObfuscator &&
    (projectConfig.isArkguardEnabled || !shouldSkipObfuscation);
  if (compileToolIsRollUp() && isObfuscatorEnabled) {
    startFilesEvent(moduleInfo.buildFilePath);
    const recordInfo = MemoryMonitor.recordStage(MemoryDefine.WRITE_OBFUSCATED_SOURCE_CODE);
    const previousStageSourceMap: sourceMap.RawSourceMap | undefined = getPreviousStageSourceMap(moduleInfo, rollupNewSourceMaps);
    collectObfuscationFileContent(moduleInfo, projectConfig, previousStageSourceMap);
    // We should skip obfuscate here if we need reObfuscate all files, since they will all be obfuscated later in function 'reObfuscate'.
    if (!projectConfig.arkObfuscator.shouldReObfuscate) {
      MemoryUtils.tryGC();
      await writeArkguardObfuscatedSourceCode(moduleInfo, logger as Function, projectConfig, previousStageSourceMap);
      MemoryUtils.tryGC();
    }
    MemoryMonitor.stopRecordStage(recordInfo);
    endFilesEvent(moduleInfo.buildFilePath, undefined, true);
    return;
  }
  mkdirsSync(path.dirname(moduleInfo.buildFilePath));
  if (!compileToolIsRollUp()) {
    await writeMinimizedSourceCode(moduleInfo.content, moduleInfo.buildFilePath, logger, projectConfig.compileHar);
    return;
  }

  if (moduleInfo.originSourceFilePath) {
    const originSourceFilePath = toUnixPath(moduleInfo.originSourceFilePath);
    let genFileInHar: GeneratedFileInHar = harFilesRecord.get(originSourceFilePath);

    if (!genFileInHar) {
      genFileInHar = { sourcePath: originSourceFilePath };
    }
    if (!genFileInHar.sourceCachePath) {
      genFileInHar.sourceCachePath = toUnixPath(moduleInfo.buildFilePath);
    }
    harFilesRecord.set(originSourceFilePath, genFileInHar);
  }

  writeFileSyncCaseAware(moduleInfo.buildFilePath, moduleInfo.content);
}

export function getPreviousStageSourceMap(moduleInfo: ModuleInfo, rollupNewSourceMaps: Object = {}): sourceMap.RawSourceMap | undefined {
  const isDeclaration = (/\.d\.e?ts$/).test(moduleInfo.buildFilePath);
  const sourceMapGeneratorInstance = SourceMapGenerator.getInstance();
  let previousStageSourceMap: sourceMap.RawSourceMap | undefined = undefined;
  if (moduleInfo.relativeSourceFilePath.length > 0 && !isDeclaration) {
    const selectedFilePath = sourceMapGeneratorInstance.isNewSourceMaps() ? moduleInfo.rollupModuleId! : moduleInfo.relativeSourceFilePath;
    previousStageSourceMap = sourceMapGeneratorInstance.getSpecifySourceMap(rollupNewSourceMaps, selectedFilePath) as sourceMap.RawSourceMap;
  }
  return previousStageSourceMap;
}

/**
 * collect current obfuscated input content, used for re-obfuscate.
 */
export function collectObfuscationFileContent(moduleInfo: ModuleInfo, projectConfig: Object,
  previousStageSourceMap: sourceMap.RawSourceMap | undefined): void {
  const arkObfuscator = projectConfig.arkObfuscator;
  const isDeclaration = (/\.d\.e?ts$/).test(moduleInfo.buildFilePath);
  const isOriginalDeclaration = (/\.d\.e?ts$/).test(moduleInfo.originSourceFilePath);

  // We only collect non-declaration files, unless it's a user-written declaration file
  if (arkObfuscator.filePathManager && (!isDeclaration || isOriginalDeclaration)) {
    const fileContent: ProjectCollections.FileContent = {
      moduleInfo: moduleInfo,
      previousStageSourceMap: previousStageSourceMap as ts.RawSourceMap,
    };
    arkObfuscator.fileContentManager.updateFileContent(fileContent);
  }
}

/**
 * This Api only be used by rollup compiling process & only be
 * exported for unit test.
 */
export async function writeArkguardObfuscatedSourceCode(moduleInfo: ModuleInfo, printObfLogger: Function,
  projectConfig: Object, previousStageSourceMap: sourceMap.RawSourceMap | undefined): Promise<void> {
  const arkObfuscator = projectConfig.arkObfuscator;
  const isDeclaration = (/\.d\.e?ts$/).test(moduleInfo.buildFilePath);
  const packageDir = projectConfig.packageDir;
  const projectRootPath = projectConfig.projectRootPath;
  const useNormalized = projectConfig.useNormalizedOHMUrl;
  const localPackageSet = projectConfig.localPackageSet;
  const useTsHar = projectConfig.useTsHar;
  const sourceMapGeneratorInstance = SourceMapGenerator.getInstance();

  const historyNameCache: Map<string, string> = getNameCacheByPath(moduleInfo, isDeclaration, projectRootPath);

  let mixedInfo: { content: string, sourceMap?: Object, nameCache?: Object, unobfuscationNameMap?: Map<string, Set<string>>};
  let projectInfo: {
    packageDir: string,
    projectRootPath: string,
    localPackageSet: Set<string>,
    useNormalized: boolean,
    useTsHar: boolean
  } = { packageDir, projectRootPath, localPackageSet, useNormalized, useTsHar };
  let filePathObj = { buildFilePath: moduleInfo.buildFilePath, relativeFilePath: moduleInfo.relativeSourceFilePath };

  try {
    startSingleFileEvent(EventList.OBFUSCATE, performancePrinter.timeSumPrinter, filePathObj.buildFilePath);
    mixedInfo = await arkObfuscator.obfuscate(moduleInfo.content, filePathObj, previousStageSourceMap,
      historyNameCache, moduleInfo.originSourceFilePath, projectInfo);
    endSingleFileEvent(EventList.OBFUSCATE, performancePrinter.timeSumPrinter);
  } catch (err) {
    const errorInfo: string = `ArkTS:INTERNAL ERROR: Failed to obfuscate file '${moduleInfo.relativeSourceFilePath}' with arkguard. ${err}`;
    const errorCodeInfo: HvigorErrorInfo = {
      code: '10810001',
      description: 'ArkTS compiler Error',
      cause: `ArkTS:INTERNAL ERROR: Failed to obfuscate file '${moduleInfo.relativeSourceFilePath}' with arkguard. ${err}`,
      position: moduleInfo.relativeSourceFilePath,
      solutions: [`Please modify the code based on the error information.`],
    };
    printObfLogger(errorInfo, errorCodeInfo, 'error');
  }

  if (mixedInfo.sourceMap && !isDeclaration) {
    const selectedFilePath = sourceMapGeneratorInstance.isNewSourceMaps() ? moduleInfo.rollupModuleId! : moduleInfo.relativeSourceFilePath;
    mixedInfo.sourceMap.sources = [moduleInfo.relativeSourceFilePath];
    sourceMapGeneratorInstance.fillSourceMapPackageInfo(moduleInfo.rollupModuleId!, mixedInfo.sourceMap);
    sourceMapGeneratorInstance.updateSourceMap(selectedFilePath, mixedInfo.sourceMap);
  }

  setNewNameCache(mixedInfo.nameCache, isDeclaration, moduleInfo, projectConfig);

  setUnobfuscationNames(mixedInfo.unobfuscationNameMap, moduleInfo.relativeSourceFilePath, isDeclaration);

  const newFilePath: string = tryMangleFileName(moduleInfo.buildFilePath, projectConfig, moduleInfo.originSourceFilePath);
  if (newFilePath !== moduleInfo.buildFilePath && !isDeclaration) {
    sourceMapGeneratorInstance.saveKeyMappingForObfFileName(moduleInfo.rollupModuleId!);
  }

  writeObfuscatedFile(newFilePath, mixedInfo.content ?? '');
}
/**
 * This function will be called when obfuscating sourceFile and declaration file.
 * Declaration file is not affected by bytecode obfuscation.
 * When bytecode obfuscation is enabel, the name of sourceFile will not be obfuscated.
 * Because harFilesRecord is collected in this function
 * This parameter is added to process sourceFile and return early only when bytecode obfuscation is turned on.
 * It does not affect the original call effect.
 */
export function tryMangleFileName(filePath: string, projectConfig: Object, originalFilePath: string, skipObf: boolean = false): string {
  originalFilePath = toUnixPath(originalFilePath);
  let isOhModule = isPackageModulesFile(originalFilePath, projectConfig);
  let genFileInHar: GeneratedFileInHar = harFilesRecord.get(originalFilePath);
  if (!genFileInHar) {
    genFileInHar = { sourcePath: originalFilePath };
    harFilesRecord.set(originalFilePath, genFileInHar);
  }
  if (skipObf) {
    return filePath;
  }
  if (projectConfig.obfuscationMergedObConfig?.options?.enableFileNameObfuscation && !isOhModule) {
    const mangledFilePath: string = mangleFilePath(filePath);
    if ((/\.d\.e?ts$/).test(filePath)) {
      genFileInHar.obfuscatedDeclarationCachePath = mangledFilePath;
    } else {
      genFileInHar.obfuscatedSourceCachePath = mangledFilePath;
    }
    filePath = mangledFilePath;
  } else if (!(/\.d\.e?ts$/).test(filePath)) {
    genFileInHar.sourceCachePath = filePath;
  }
  return filePath;
}

export async function mangleDeclarationFileName(printObfLogger: Function, projectConfig: Object,
  sourceFileBelongProject: Map<string, string>): Promise<void> {
  for (const [sourcePath, genFilesInHar] of harFilesRecord) {
    if (genFilesInHar.originalDeclarationCachePath && genFilesInHar.originalDeclarationContent) {
      let filePath = genFilesInHar.originalDeclarationCachePath;
      let relativeSourceFilePath = getRelativeSourcePath(filePath,
         projectConfig.projectRootPath, sourceFileBelongProject.get(toUnixPath(filePath)));
      await writeObfuscatedSourceCode({
          content: genFilesInHar.originalDeclarationContent,
          buildFilePath: genFilesInHar.originalDeclarationCachePath,
          relativeSourceFilePath: relativeSourceFilePath,
          originSourceFilePath: sourcePath
        }, printObfLogger, projectConfig, {});
    }
  }
}

export async function writeMinimizedSourceCode(content: string, filePath: string, logger: Object,
  isHar: boolean = false): Promise<void> {
  let result: MinifyOutput;
  try {
    const minifyOptions = {
      compress: {
        join_vars: false,
        sequences: 0,
        directives: false
      }
    };
    if (!isHar) {
      minifyOptions.format = {
        semicolons: false,
        beautify: true,
        indent_level: 2
      };
    }
    result = await minify(content, minifyOptions);
  } catch {
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_SOURCE_CODE_OBFUSCATION_FAILED,
      ArkTSInternalErrorDescription,
      `Failed to obfuscate source code for ${filePath}`
    );
    logger.printError(errInfo);
  }

  fs.writeFileSync(filePath, result.code);
}

//Writes declaration files in debug mode
export function writeDeclarationFiles(compileMode: string): void {
  if (compileToolIsRollUp() && compileMode === ESMODULE) {
    for (const genFilesInHar of harFilesRecord.values()) {
      if (genFilesInHar.originalDeclarationCachePath && genFilesInHar.originalDeclarationContent) {
        mkdirsSync(path.dirname(genFilesInHar.originalDeclarationCachePath));
        writeFileSyncCaseAware(genFilesInHar.originalDeclarationCachePath, genFilesInHar.originalDeclarationContent);
      }
    }
  }
}

export function genBuildPath(filePath: string, projectPath: string, buildPath: string, projectConfig: Object): string {
  filePath = toUnixPath(filePath);
  if (filePath.endsWith(EXTNAME_MJS)) {
    filePath = filePath.replace(/\.mjs$/, EXTNAME_JS);
  }
  if (filePath.endsWith(EXTNAME_CJS)) {
    filePath = filePath.replace(/\.cjs$/, EXTNAME_JS);
  }
  projectPath = toUnixPath(projectPath);

  if (isPackageModulesFile(filePath, projectConfig)) {
    const packageDir: string = projectConfig.packageDir;
    const fakePkgModulesPath: string = toUnixPath(path.join(projectConfig.projectRootPath, packageDir));
    let output: string = '';
    if (filePath.indexOf(fakePkgModulesPath) === -1) {
      const hapPath: string = toUnixPath(projectConfig.projectRootPath);
      const tempFilePath: string = filePath.replace(hapPath, '');
      const sufStr: string = tempFilePath.substring(tempFilePath.indexOf(packageDir) + packageDir.length + 1);
      output = path.join(projectConfig.nodeModulesPath, ZERO, sufStr);
    } else {
      output = filePath.replace(fakePkgModulesPath, path.join(projectConfig.nodeModulesPath, ONE));
    }
    return output;
  }

  if (filePath.indexOf(projectPath) !== -1) {
    const sufStr: string = filePath.replace(projectPath, '');
    const output: string = path.join(buildPath, sufStr);
    return output;
  }

  return '';
}

export function getPackageInfo(configFile: string): Array<string> {
  if (packageCollection.has(configFile)) {
    return packageCollection.get(configFile);
  }
  const data: Object = JSON.parse(fs.readFileSync(configFile).toString());
  const bundleName: string = data.app.bundleName;
  const moduleName: string = data.module.name;
  packageCollection.set(configFile, [bundleName, moduleName]);
  return [bundleName, moduleName];
}

/**
 * This Api only used by webpack compiling process - result_process
 * @param sourcePath The path in build cache dir
 * @param sourceContent The intermediate js source code
 */
export function generateSourceFilesToTemporary(sourcePath: string, sourceContent: string, sourceMap: Object,
  projectConfig: Object, logger: Object): void {
    let jsFilePath: string = genTemporaryPath(sourcePath, projectConfig.projectPath, process.env.cachePath,
      projectConfig, undefined);
  if (jsFilePath.length === 0) {
    return;
  }
  if (jsFilePath.endsWith(EXTNAME_ETS)) {
    jsFilePath = jsFilePath.replace(/\.ets$/, EXTNAME_JS);
  } else {
    jsFilePath = jsFilePath.replace(/\.ts$/, EXTNAME_JS);
  }
  let sourceMapFile: string = genSourceMapFileName(jsFilePath);
  if (sourceMapFile.length > 0 && projectConfig.buildArkMode === 'debug') {
    let source = toUnixPath(sourcePath).replace(toUnixPath(projectConfig.projectRootPath) + '/', '');
    // adjust sourceMap info
    sourceMap.sources = [source];
    sourceMap.file = path.basename(sourceMap.file);
    delete sourceMap.sourcesContent;
    newSourceMaps[source] = sourceMap;
  }
  sourceContent = transformModuleSpecifier(sourcePath, sourceContent, projectConfig);

  mkdirsSync(path.dirname(jsFilePath));
  if (projectConfig.buildArkMode === 'debug') {
    fs.writeFileSync(jsFilePath, sourceContent);
    return;
  }

  writeObfuscatedSourceCode({content: sourceContent, buildFilePath: jsFilePath, relativeSourceFilePath: ''},
    logger, projectConfig);
}

export function genAbcFileName(temporaryFile: string): string {
  let abcFile: string = temporaryFile;
  if (temporaryFile.endsWith(EXTNAME_TS)) {
    abcFile = temporaryFile.replace(/\.ts$/, EXTNAME_ABC);
  } else {
    abcFile = temporaryFile.replace(/\.js$/, EXTNAME_ABC);
  }
  return abcFile;
}

export function isOhModules(projectConfig: Object): boolean {
  return projectConfig.packageDir === OH_MODULES;
}

export function isEs2Abc(projectConfig: Object): boolean {
  return projectConfig.pandaMode === ES2ABC || projectConfig.pandaMode === 'undefined' ||
    projectConfig.pandaMode === undefined;
}

export function isTs2Abc(projectConfig: Object): boolean {
  return projectConfig.pandaMode === TS2ABC;
}

export function genProtoFileName(temporaryFile: string): string {
  return temporaryFile.replace(/\.(?:[tj]s|json)$/, EXTNAME_PROTO_BIN);
}

export function genMergeProtoFileName(temporaryFile: string): string {
  let protoTempPathArr: string[] = temporaryFile.split(TEMPORARY);
  const sufStr: string = protoTempPathArr[protoTempPathArr.length - 1];
  let protoBuildPath: string = path.join(process.env.cachePath, 'protos', sufStr);

  return protoBuildPath;
}

export function removeDuplicateInfo(moduleInfos: Array<any>): Array<any> {
  const tempModuleInfos: any[] = Array<any>();
  moduleInfos.forEach((item) => {
    let check: boolean = tempModuleInfos.every((newItem) => {
      return item.tempFilePath !== newItem.tempFilePath;
    });
    if (check) {
      tempModuleInfos.push(item);
    }
  });
  moduleInfos = tempModuleInfos;

  return moduleInfos;
}

export function buildCachePath(tailName: string, projectConfig: Object, logger: Object): string {
  let pathName: string = process.env.cachePath !== undefined ?
    path.join(projectConfig.cachePath, tailName) : path.join(projectConfig.aceModuleBuild, tailName);
  validateFilePathLength(pathName, logger);
  return pathName;
}

export function getArkBuildDir(arkDir: string): string {
  if (isWindows()) {
    return path.join(arkDir, 'build-win');
  } else if (isMac()) {
    return path.join(arkDir, 'build-mac');
  } else {
    return path.join(arkDir, 'build');
  }
}

export function getBuildBinDir(arkDir: string): string {
  return path.join(getArkBuildDir(arkDir), 'bin');
}

export function cleanUpUtilsObjects(): void {
  newSourceMaps = {};
  nameCacheMap.clear();
  packageCollection.clear();
}

export function compileToolIsRollUp(): boolean {
  return process.env.compileTool === 'rollup';
}

export function transformOhmurlToRecordName(ohmurl: string): string {
  // @normalized:N&<moduleName>&<bunldName>&<packageName>/entry/ets/xxx/yyy&<version>
  // ----> <bunldName>&<packageName>/entry/ets/xxx/yyy&<version>
  return ohmurl.split(SEPARATOR_BITWISE_AND).slice(2).join(SEPARATOR_BITWISE_AND);
}

export function transformOhmurlToPkgName(ohmurl: string): string {
  let normalizedPath: string = ohmurl.split(SEPARATOR_BITWISE_AND)[3];
  let paths: Array<string> = normalizedPath.split(SEPARATOR_SLASH);
  if (normalizedPath.startsWith(SEPARATOR_AT)) {
    // Spec: If the normalized import path starts with '@', the package name is before the second '/' in the normalized
    // import path, like:  @aaa/bbb/ccc/ddd ---> package name is @aaa/bbb
    return paths.slice(0, 2).join(SEPARATOR_SLASH);
  }
  return paths[0];
}

export function writeFileSyncCaseAware(filePath: fs.PathOrFileDescriptor, data: string | NodeJS.ArrayBufferView, options?: fs.WriteFileOptions): void {
    // If filePath is not a file descriptor number (i.e., it's a file path) and the file at that path already exists,
    // delete the existing file first to ensure the written file name matches the input.
    if (typeof filePath !== 'number' && fs.existsSync(filePath)) {
      fs.unlinkSync(filePath);
    }
    fs.writeFileSync(filePath, data, options);
}