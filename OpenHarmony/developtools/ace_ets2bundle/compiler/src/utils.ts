/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
import ts from 'typescript';
import fs from 'fs';
import os from 'os';
import uglifyJS from 'uglify-js';

import {
  partialUpdateConfig,
  projectConfig,
  globalProgram
} from '../main';
import { createHash } from 'crypto';
import type { Hash } from 'crypto';
import {
  AUXILIARY,
  EXTNAME_ETS,
  EXTNAME_JS,
  MAIN,
  FAIL,
  TEMPORARY,
  ESMODULE,
  $$,
  EXTEND_DECORATORS,
  COMPONENT_EXTEND_DECORATOR,
  COMPONENT_ANIMATABLE_EXTEND_DECORATOR,
  COMPONENT_CONSTRUCTOR_LOCALSTORAGE_TYPE_PU,
  GET_SHARED,
  COMPONENT_CONSTRUCTOR_UNDEFINED,
  USE_SHARED_STORAGE,
  STORAGE
} from './pre_define';
import { 
  ERROR_DESCRIPTION,
  HvigorLogInfo,
  HvigorErrorInfo
} from './hvigor_error_code/hvigor_error_info';

export enum LogType {
  ERROR = 'ERROR',
  WARN = 'WARN',
  NOTE = 'NOTE'
}
export const TEMPORARYS: string = 'temporarys';
export const BUILD: string = 'build';
export const SRC_MAIN: string = 'src/main';

const red: string = '\u001b[31m';
const reset: string = '\u001b[39m';

const WINDOWS: string = 'Windows_NT';
const LINUX: string = 'Linux';
const MAC: string = 'Darwin';
const HARMONYOS: string = 'HarmonyOS';

export interface LogInfo extends HvigorLogInfo {
  type: LogType,
  message: string,
  pos?: number,
  line?: number,
  column?: number,
  fileName?: string
}

export const repeatLog: Map<string, LogInfo> = new Map();

export interface IFileLog {
  sourceFile: ts.SourceFile | undefined;
  errors: LogInfo[];
  cleanUp(): void
}

export function buildErrorInfoFromLogInfo(info: LogInfo, fileName: string): HvigorErrorInfo {
  const positionMessage: string = info.line && info.column ? `${fileName}:${info.line}:${info.column}` : fileName;
  return new HvigorErrorInfo()
    .setCode(info.code)
    .setDescription(info.description ?? ERROR_DESCRIPTION)
    .setCause(info.cause ?? info.message)
    .setPosition(`File: ${positionMessage}`)
    .setSolutions(info.solutions ?? []);
}

export function isHvigorLogInfo(logInfo: LogInfo): boolean {
  // Hvigor logger requires code as mendatory attribute.
  // ArkUI warnings or info are not using Hvigor logger to emit.
  return logInfo.code !== undefined;
}

export function emitLogInfo(loader: Object, infos: LogInfo[], fastBuild: boolean = false,
  resourcePath: string | null = null, hvigorLogger: Object | undefined = undefined): void {
  if (infos && infos.length) {
    infos.forEach((item) => {
      hvigorLogger ? emitLogInfoFromHvigorLogger(hvigorLogger, item, loader, fastBuild, resourcePath) 
        : emitLogInfoFromLoader(loader, item, fastBuild, resourcePath);
    });
  }
}

function emitLogInfoFromHvigorLogger(hvigorLogger: Object, info: LogInfo, loader: Object, 
  fastBuild: boolean, resourcePath: string | null): void {
  if (!isHvigorLogInfo(info)) {
    emitLogInfoFromLoader(loader, info, fastBuild, resourcePath);
    return;
  }
  const errorInfo: HvigorErrorInfo = buildErrorInfoFromLogInfo(info, info.fileName || resourcePath);
  switch (info.type) {
    case LogType.ERROR:
      hvigorLogger.printError(errorInfo);
      break;
    case LogType.WARN:
      hvigorLogger.printWarn(errorInfo.cause);
      break;
    case LogType.NOTE:
      hvigorLogger.printInfo(errorInfo.cause);
      break;
  }
}

function emitLogInfoFromLoader(loader: Object, info: LogInfo, fastBuild: boolean = false,
  resourcePath: string | null = null): void {
  const message: string = fastBuild ? getMessage(info.fileName || resourcePath, info, true)
    : getMessage(info.fileName || loader.resourcePath, info);
  switch (info.type) {
    case LogType.ERROR:
      fastBuild ? loader.error(message) : loader.emitError(message);
      break;
    case LogType.WARN:
      fastBuild ? loader.warn(message) : loader.emitWarning(message);
      break;
    case LogType.NOTE:
      fastBuild ? loader.info(message) : loader.emitWarning(message);
      break;
  }
}

export function addLog(
  type: LogType, 
  message: string, 
  pos: number, 
  log: LogInfo[],
  sourceFile: ts.SourceFile,
  hvigorLogInfo?: HvigorLogInfo
): void {
  const posOfNode: ts.LineAndCharacter = sourceFile.getLineAndCharacterOfPosition(pos);
  log.push({
    type: type,
    message: message,
    line: posOfNode.line + 1,
    column: posOfNode.character + 1,
    fileName: sourceFile.fileName,
    ...hvigorLogInfo
  });
}

export function getMessage(fileName: string, info: LogInfo, fastBuild: boolean = false): string {
  let message: string;
  if (info.line && info.column) {
    message = `BUILD${info.type} File: ${fileName}:${info.line}:${info.column}\n ${info.message}`;
  } else {
    message = `BUILD${info.type} File: ${fileName}\n ${info.message}`;
  }
  if (fastBuild) {
    message = message.replace(/^BUILD/, 'ArkTS:');
  }
  return message;
}

export function getTransformLog(transformLog: IFileLog): LogInfo[] {
  const sourceFile: ts.SourceFile = transformLog.sourceFile;
  const logInfos: LogInfo[] = transformLog.errors.map((item) => {
    if (item.pos || item.pos === 0) {
      if (!item.column || !item.line) {
        const posOfNode: ts.LineAndCharacter = sourceFile.getLineAndCharacterOfPosition(item.pos);
        item.line = posOfNode.line + 1;
        item.column = posOfNode.character + 1;
      }
    } else {
      item.line = item.line || undefined;
      item.column = item.column || undefined;
    }
    if (!item.fileName) {
      item.fileName = sourceFile.fileName;
    }
    return item;
  });
  return logInfos;
}

class ComponentInfo {
  private _id: number = 0;
  private _componentNames: Set<string> = new Set(['ForEach']);
  public set id(id: number) {
    this._id = id;
  }
  public get id(): number {
    return this._id;
  }
  public set componentNames(componentNames: Set<string>) {
    this._componentNames = componentNames;
  }
  public get componentNames(): Set<string> {
    return this._componentNames;
  }
}

export let componentInfo: ComponentInfo = new ComponentInfo();

export function hasDecorator(node: ts.MethodDeclaration | ts.FunctionDeclaration |
  ts.StructDeclaration | ts.ClassDeclaration, decortorName: string,
  customBuilder: ts.Decorator[] = null, log: LogInfo[] = null): boolean {
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(node);
  if (decorators && decorators.length) {
    const extendResult = {
      Extend: false,
      AnimatableExtend: false
    };
    for (let i = 0; i < decorators.length; i++) {
      const originalDecortor: string = decorators[i].getText().replace(/\(.*\)$/, '').replace(/\s*/g, '').trim();
      if (log && EXTEND_DECORATORS.includes(decortorName)) {
        if (originalDecortor === COMPONENT_EXTEND_DECORATOR) {
          extendResult.Extend = true;
        }
        if (originalDecortor === COMPONENT_ANIMATABLE_EXTEND_DECORATOR) {
          extendResult.AnimatableExtend = true;
        }
      } else {
        if (originalDecortor === decortorName) {
          if (customBuilder) {
            customBuilder.push(...decorators.slice(i + 1), ...decorators.slice(0, i));
          }
          return true;
        }
      }
    }
    if (log && extendResult.Extend && extendResult.AnimatableExtend) {
      log.push({
        type: LogType.ERROR,
        message: `The function can not be decorated by '@Extend' and '@AnimatableExtend' at the same time.`,
        pos: node.getStart(),
        code: '10905111'
      });
    }
    return (decortorName === COMPONENT_EXTEND_DECORATOR && extendResult.Extend) ||
      (decortorName === COMPONENT_ANIMATABLE_EXTEND_DECORATOR && extendResult.AnimatableExtend);
  }
  return false;
}

const STATEMENT_EXPECT: number = 1128;
const SEMICOLON_EXPECT: number = 1005;
const STATESTYLES_EXPECT: number = 1003;
export const IGNORE_ERROR_CODE: number[] = [STATEMENT_EXPECT, SEMICOLON_EXPECT, STATESTYLES_EXPECT];

export function readFile(dir: string, utFiles: string[]): void {
  try {
    const files: string[] = fs.readdirSync(dir);
    files.forEach((element) => {
      const filePath: string = path.join(dir, element);
      const status: fs.Stats = fs.statSync(filePath);
      if (status.isDirectory()) {
        readFile(filePath, utFiles);
      } else {
        utFiles.push(filePath);
      }
    });
  } catch (e) {
    console.error(red, 'ArkTS ERROR: ' + e, reset);
  }
}

export function circularFile(inputPath: string, outputPath: string): void {
  if (!inputPath || !outputPath) {
    return;
  }
  fs.readdir(inputPath, function (err, files) {
    if (!files) {
      return;
    }
    files.forEach(file => {
      const inputFile: string = path.resolve(inputPath, file);
      const outputFile: string = path.resolve(outputPath, file);
      const fileStat: fs.Stats = fs.statSync(inputFile);
      if (fileStat.isFile()) {
        copyFile(inputFile, outputFile);
      } else {
        circularFile(inputFile, outputFile);
      }
    });
  });
}

function copyFile(inputFile: string, outputFile: string): void {
  try {
    const parent: string = path.join(outputFile, '..');
    if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
      mkDir(parent);
    }
    if (fs.existsSync(outputFile)) {
      return;
    }
    const readStream: fs.ReadStream = fs.createReadStream(inputFile);
    const writeStream: fs.WriteStream = fs.createWriteStream(outputFile);
    readStream.pipe(writeStream);
    readStream.on('close', function () {
      writeStream.end();
    });
  } catch (err) {
    throw err.message;
  }
}

export function mkDir(path_: string): void {
  const parent: string = path.join(path_, '..');
  if (!(fs.existsSync(parent) && !fs.statSync(parent).isFile())) {
    mkDir(parent);
  }
  fs.mkdirSync(path_);
}

export function toUnixPath(data: string): string {
  if (/^win/.test(require('os').platform())) {
    const fileTmps: string[] = data.split(path.sep);
    const newData: string = path.posix.join(...fileTmps);
    return newData;
  }
  return data;
}

export function tryToLowerCasePath(filePath: string): string {
  return toUnixPath(filePath).toLowerCase();
}

export function toHashData(path: string): string {
  const content: string = fs.readFileSync(path).toString();
  const hash: Hash = createHash('sha256');
  hash.update(content);
  return hash.digest('hex');
}

export function writeFileSync(filePath: string, content: string): void {
  if (!fs.existsSync(filePath)) {
    const parent: string = path.join(filePath, '..');
    if (!(fs.existsSync(parent) && !fs.statSync(parent).isFile())) {
      mkDir(parent);
    }
  }
  fs.writeFileSync(filePath, content);
}
export function genLoaderOutPathOfHar(filePath: string, cachePath: string, buildPath: string, moduleRootPath: string, projectRootPath): string {
  filePath = toUnixPath(filePath);
  buildPath = toUnixPath(buildPath);
  const cacheRootPath: string = toUnixPath(cachePath);
  const moduleName = toUnixPath(moduleRootPath).replace(toUnixPath(projectRootPath), '');
  const relativeFilePath: string = filePath.replace(cacheRootPath, '').replace(moduleName, '');
  const output: string = path.join(buildPath, relativeFilePath);
  return output;
}

export function genTemporaryPath(filePath: string, projectPath: string, buildPath: string, projectConfig: Object,
  metaInfo: Object, buildInHar: boolean = false): string {
  filePath = toUnixPath(filePath).replace(/\.[cm]js$/, EXTNAME_JS);
  projectPath = toUnixPath(projectPath);

  if (process.env.compileTool === 'rollup') {
    let relativeFilePath: string = '';
    if (metaInfo) {
      if (metaInfo.isLocalDependency) {
        // When buildInHar and compileHar are both True,
        // this is the path under the PackageHar directory being spliced ​​together.
        // Here, only the relative path based on moduleRootPath needs to be retained.
        // eg. moduleA/index.js --> index.js --> PackageHar/index.js
        // eg. moduleA/src/main/ets/test.js --> src/main/ets/test.js --> PackageHar/src/main/ets/test.js
        const moduleName: string = buildInHar && projectConfig.compileHar ? '' : metaInfo.moduleName;
        relativeFilePath = filePath.replace(toUnixPath(metaInfo.belongModulePath), moduleName);
      } else {
        relativeFilePath = filePath.replace(toUnixPath(metaInfo.belongProjectPath), '');
      }
    } else {
      relativeFilePath = filePath.replace(toUnixPath(projectConfig.projectRootPath), '');
    }
    const output: string = path.join(buildPath, relativeFilePath);
    return output;
  }

  if (isPackageModulesFile(filePath, projectConfig)) {
    const packageDir: string = projectConfig.packageDir;
    const fakePkgModulesPath: string = toUnixPath(path.join(projectConfig.projectRootPath, packageDir));
    let output: string = '';
    if (filePath.indexOf(fakePkgModulesPath) === -1) {
      const hapPath: string = toUnixPath(projectConfig.projectRootPath);
      const tempFilePath: string = filePath.replace(hapPath, '');
      const relativeFilePath: string = tempFilePath.substring(tempFilePath.indexOf(packageDir) + packageDir.length + 1);
      output = path.join(buildPath, buildInHar ? '' : TEMPORARY, packageDir, MAIN, relativeFilePath);
    } else {
      output = filePath.replace(fakePkgModulesPath,
        path.join(buildPath, buildInHar ? '' : TEMPORARY, packageDir, AUXILIARY));
    }
    return output;
  }

  if (filePath.indexOf(projectPath) !== -1) {
    const relativeFilePath: string = filePath.replace(projectPath, '');
    const output: string = path.join(buildPath, buildInHar ? '' : TEMPORARY, relativeFilePath);
    return output;
  }

  return '';
}

export function isPackageModulesFile(filePath: string, projectConfig: Object): boolean {
  filePath = toUnixPath(filePath);
  const hapPath: string = toUnixPath(projectConfig.projectRootPath);
  const tempFilePath: string = filePath.replace(hapPath, '');
  const packageDir: string = projectConfig.packageDir;
  if (tempFilePath.indexOf(packageDir) !== -1) {
    const fakePkgModulesPath: string = toUnixPath(path.resolve(projectConfig.projectRootPath, packageDir));
    if (filePath.indexOf(fakePkgModulesPath) !== -1) {
      return true;
    }
    if (projectConfig.modulePathMap) {
      for (const key in projectConfig.modulePathMap) {
        const value: string = projectConfig.modulePathMap[key];
        const fakeModulePkgModulesPath: string = toUnixPath(path.resolve(value, packageDir));
        if (filePath.indexOf(fakeModulePkgModulesPath) !== -1) {
          return true;
        }
      }
    }
  }

  return false;
}

export interface GeneratedFileInHar {
  sourcePath: string;
  sourceCachePath?: string;
  obfuscatedSourceCachePath?: string;
  originalDeclarationCachePath?: string;
  originalDeclarationContent?: string;
  obfuscatedDeclarationCachePath?: string;
}

export const harFilesRecord: Map<string, GeneratedFileInHar> = new Map();

export function generateSourceFilesInHar(sourcePath: string, sourceContent: string, suffix: string,
  projectConfig: Object, modulePathMap?: Object): void {
  const belongModuleInfo: Object = getBelongModuleInfo(sourcePath, modulePathMap, projectConfig.projectRootPath);
  // compileShared: compile shared har of project
  let jsFilePath: string = genTemporaryPath(sourcePath,
    projectConfig.compileShared ? projectConfig.projectRootPath : projectConfig.moduleRootPath,
    projectConfig.compileShared || projectConfig.byteCodeHar ? path.resolve(projectConfig.aceModuleBuild, '../etsFortgz') : projectConfig.cachePath,
    projectConfig, belongModuleInfo, projectConfig.compileShared);
  if (!jsFilePath.match(new RegExp(projectConfig.packageDir))) {
    jsFilePath = jsFilePath.replace(/\.ets$/, suffix).replace(/\.ts$/, suffix);
    if (projectConfig.obfuscateHarType === 'uglify' && suffix === '.js') {
      sourceContent = uglifyJS.minify(sourceContent).code;
    }
    // collect the declaration files for obfuscation
    if (projectConfig.compileMode === ESMODULE && (/\.d\.e?ts$/).test(jsFilePath)) {
      sourcePath = toUnixPath(sourcePath);
      const genFilesInHar: GeneratedFileInHar = {
        sourcePath: sourcePath,
        originalDeclarationCachePath: jsFilePath,
        originalDeclarationContent: sourceContent
      };
      harFilesRecord.set(sourcePath, genFilesInHar);
      return;
    } else {
      mkdirsSync(path.dirname(jsFilePath));
    }
    fs.writeFileSync(jsFilePath, sourceContent);
  }
}

export function mkdirsSync(dirname: string): boolean {
  if (fs.existsSync(dirname)) {
    return true;
  } else if (mkdirsSync(path.dirname(dirname))) {
    fs.mkdirSync(dirname);
    return true;
  }

  return false;
}

export function nodeLargeOrEqualTargetVersion(targetVersion: number): boolean {
  const currentNodeVersion: number = parseInt(process.versions.node.split('.')[0]);
  if (currentNodeVersion >= targetVersion) {
    return true;
  }

  return false;
}

export function removeDir(dirName: string): void {
  if (fs.existsSync(dirName)) {
    if (nodeLargeOrEqualTargetVersion(16)) {
      fs.rmSync(dirName, { recursive: true });
    } else {
      fs.rmdirSync(dirName, { recursive: true });
    }
  }
}

export function parseErrorMessage(message: string): string {
  const messageArrary: string[] = message.split('\n');
  let logContent: string = '';
  messageArrary.forEach(element => {
    if (!(/^at/.test(element.trim()))) {
      logContent = logContent + element + '\n';
    }
  });
  return logContent;
}

export function isWindows(): boolean {
  return os.type() === WINDOWS;
}

export function isLinux(): boolean {
  return os.type() === LINUX;
}

export function isMac(): boolean {
  return os.type() === MAC;
}

export function isHarmonyOs(): boolean {
  return os.type() === HARMONYOS;
}

export function maxFilePathLength(): number {
  if (isWindows()) {
    return 32766;
  } else if (isLinux() || isHarmonyOs()) {
    return 4095;
  } else if (isMac()) {
    return 1016;
  } else {
    return -1;
  }
}

export function validateFilePathLength(filePath: string, logger: Object): boolean {
  if (maxFilePathLength() < 0) {
    logger.error(red, 'Unknown OS platform', reset);
    process.exitCode = FAIL;
    return false;
  } else if (filePath.length > 0 && filePath.length <= maxFilePathLength()) {
    return true;
  } else if (filePath.length > maxFilePathLength()) {
    logger.error(red, `The length of ${filePath} exceeds the limitation of current platform, which is ` +
      `${maxFilePathLength()}. Please try moving the project folder to avoid deeply nested file path and try again`,
    reset);
    process.exitCode = FAIL;
    return false;
  } else {
    logger.error(red, 'Validate file path failed', reset);
    process.exitCode = FAIL;
    return false;
  }
}

export function validateFilePathLengths(filePaths: Array<string>, logger: any): boolean {
  filePaths.forEach((filePath) => {
    if (!validateFilePathLength(filePath, logger)) {
      return false;
    }
    return true;
  });
  return true;
}

export function unlinkSync(filePath: string): void {
  if (fs.existsSync(filePath)) {
    fs.unlinkSync(filePath);
  }
}

export function getExtensionIfUnfullySpecifiedFilepath(filePath: string): string {
  if (fs.existsSync(filePath) && fs.statSync(filePath).isFile()) {
    return '';
  }

  let extension: string = EXTNAME_ETS;
  if (fs.existsSync(filePath + '.ts') && fs.statSync(filePath + '.ts').isFile()) {
    extension = '.ts';
  } else if (fs.existsSync(filePath + '.d.ts') && fs.statSync(filePath + '.d.ts').isFile()) {
    extension = '.d.ts';
  } else if (fs.existsSync(filePath + '.d.ets') && fs.statSync(filePath + '.d.ets').isFile()) {
    extension = '.d.ets';
  } else if (fs.existsSync(filePath + '.js') && fs.statSync(filePath + '.js').isFile()) {
    extension = '.js';
  } else if (fs.existsSync(filePath + '.json') && fs.statSync(filePath + '.json').isFile()) {
    extension = '.json';
  }

  return extension;
}

export function shouldWriteChangedList(watchModifiedFiles: string[],
  watchRemovedFiles: string[]): boolean {
  if (projectConfig.compileMode === ESMODULE && process.env.watchMode === 'true' && !projectConfig.isPreview &&
    projectConfig.changedFileList && (watchRemovedFiles.length + watchModifiedFiles.length)) {
    if (process.env.compileTool !== 'rollup') {
      if (!(watchModifiedFiles.length === 1 &&
        watchModifiedFiles[0] === projectConfig.projectPath && !watchRemovedFiles.length)) {
        return true;
      } else {
        return false;
      }
    }
    return true;
  }
  return false;
}

interface HotReloadIncrementalTime {
  hotReloadIncrementalStartTime: string;
  hotReloadIncrementalEndTime: string;
}

export const hotReloadIncrementalTime: HotReloadIncrementalTime = {
  hotReloadIncrementalStartTime: '',
  hotReloadIncrementalEndTime: ''
};

interface FilesObj {
  modifiedFiles: string[],
  removedFiles: string[]
}

let allModifiedFiles: Set<string> = new Set();

export function getHotReloadFiles(watchModifiedFiles: string[],
  watchRemovedFiles: string[], hotReloadSupportFiles: Set<string>): FilesObj {
  hotReloadIncrementalTime.hotReloadIncrementalStartTime = new Date().getTime().toString();
  watchRemovedFiles = watchRemovedFiles.map(file => path.relative(projectConfig.projectPath, file));
  allModifiedFiles = new Set([...allModifiedFiles, ...watchModifiedFiles
    .filter(file => fs.statSync(file).isFile() &&
      (hotReloadSupportFiles.has(file) || !['.ets', '.ts', '.js'].includes(path.extname(file))))
    .map(file => path.relative(projectConfig.projectPath, file))]
    .filter(file => !watchRemovedFiles.includes(file)));
  return {
    modifiedFiles: [...allModifiedFiles],
    removedFiles: [...watchRemovedFiles]
  };
}

export function getResolveModules(projectPath: string, faMode: boolean): string[] {
  if (faMode) {
    return [
      path.resolve(projectPath, '../../../../../'),
      path.resolve(projectPath, '../../../../' + projectConfig.packageDir),
      path.resolve(projectPath, '../../../../../' + projectConfig.packageDir),
      path.resolve(projectPath, '../../')
    ];
  } else {
    return [
      path.resolve(projectPath, '../../../../'),
      path.resolve(projectPath, '../../../' + projectConfig.packageDir),
      path.resolve(projectPath, '../../../../' + projectConfig.packageDir),
      path.resolve(projectPath, '../')
    ];
  }
}

export function writeUseOSFiles(useOSFiles: Set<string>): void {
  let info: string = '';
  if (!fs.existsSync(projectConfig.aceSoPath)) {
    const parent: string = path.resolve(projectConfig.aceSoPath, '..');
    if (!(fs.existsSync(parent) && !fs.statSync(parent).isFile())) {
      mkDir(parent);
    }
  } else {
    info = fs.readFileSync(projectConfig.aceSoPath, 'utf-8') + '\n';
  }
  fs.writeFileSync(projectConfig.aceSoPath, info + Array.from(useOSFiles).join('\n'));
}


export function writeCollectionFile(cachePath: string, appCollection: Map<string, Set<string>>,
  allComponentsOrModules: Map<string, Array<string>>, fileName: string, allFiles: Set<string> = null,
  widgetPath: string = undefined): void {
  for (let key of appCollection.keys()) {
    if (appCollection.get(key).size === 0) {
      allComponentsOrModules.delete(key);
      continue;
    }
    if (allFiles && !allFiles.has(key)) {
      continue;
    }
    const newKey: string = projectConfig.projectRootPath ? path.relative(projectConfig.projectRootPath, key) : key;
    allComponentsOrModules.set(newKey, Array.from(appCollection.get(key)));
  }
  const content: string = JSON.stringify(Object.fromEntries(allComponentsOrModules), null, 2);
  writeFileSync(path.resolve(cachePath, fileName), content);
  if (widgetPath) {
    writeFileSync(path.resolve(widgetPath, fileName), content);
  }
}

export function getAllComponentsOrModules(allFiles: Set<string>,
  cacheCollectionFileName: string): Map<string, Array<string>> {
  const cacheCollectionFilePath: string = path.resolve(projectConfig.cachePath, cacheCollectionFileName);
  const allComponentsOrModules: Map<string, Array<string>> = new Map();
  if (!fs.existsSync(cacheCollectionFilePath)) {
    return allComponentsOrModules;
  }
  const lastComponentsOrModules = require(cacheCollectionFilePath);
  for (let key in lastComponentsOrModules) {
    if (allFiles.has(key)) {
      allComponentsOrModules.set(key, lastComponentsOrModules[key]);
    }
  }
  return allComponentsOrModules;
}

export function getPossibleBuilderTypeParameter(parameters: ts.ParameterDeclaration[]): string[] {
  const parameterNames: string[] = [];
  if (!partialUpdateConfig.builderCheck) {
    if (isDollarParameter(parameters)) {
      parameters[0].type.members.forEach((member) => {
        if (member.name && ts.isIdentifier(member.name)) {
          parameterNames.push(member.name.escapedText.toString());
        }
      });
    } else {
      parameters.forEach((parameter) => {
        if (parameter.name && ts.isIdentifier(parameter.name)) {
          parameterNames.push(parameter.name.escapedText.toString());
        }
      });
    }
  }
  return parameterNames;
}

function isDollarParameter(parameters: ts.ParameterDeclaration[]): boolean {
  return parameters.length === 1 && parameters[0].name && ts.isIdentifier(parameters[0].name) &&
    parameters[0].name.escapedText.toString() === $$ && parameters[0].type && ts.isTypeLiteralNode(parameters[0].type) &&
    parameters[0].type.members && parameters[0].type.members.length > 0;
}

interface ChildrenCacheFile {
  fileName: string,
  mtimeMs: number,
  hash: string
}

export interface CacheFile {
  mtimeMs: number,
  hash: string,
  children: Array<ChildrenCacheFile>,
}

export interface RouterInfo {
  name: string,
  buildFunction: string,
}

// Global Information & Method
export class ProcessFileInfo {
  buildStart: boolean = true;
  wholeFileInfo: { [id: string]: SpecialArkTSFileInfo | TSFileInfo } = {}; // Save ArkTS & TS file's infomation
  transformedFiles: Set<string> = new Set(); // ArkTS & TS Files which should be transformed in this compilation
  cachedFiles: string[] = []; // ArkTS & TS Files which should not be transformed in this compilation
  shouldHaveEntry: string[] = []; // Which file should have @Entry decorator
  resourceToFile: { [resource: string]: Set<string> } = {}; // Resource is used by which file
  lastResourceList: Set<string> = new Set();
  resourceList: Set<string> = new Set(); // Whole project resource
  shouldInvalidFiles: Set<string> = new Set();
  resourceTableChanged: boolean = false;
  currentArkTsFile: SpecialArkTSFileInfo;
  reUseProgram: boolean = false;
  resourcesArr: Set<string> = new Set();
  lastResourcesSet: Set<string> = new Set();
  transformCacheFiles: { [fileName: string]: CacheFile } = {};
  processBuilder: boolean = false;
  processGlobalBuilder: boolean = false;
  processLocalBuilder: boolean = false;
  builderLikeCollection: Set<string> = new Set();
  newTsProgram: ts.Program;
  changeFiles: string[] = [];
  isFirstBuild: boolean = true;
  processForEach: number = 0;
  processLazyForEach: number = 0;
  processRepeat: boolean = false;
  isAsPageImport: boolean = false;
  overallObjectLinkCollection: Map<string, Set<string>> = new Map();
  overallLinkCollection: Map<string, Set<string>> = new Map();
  overallBuilderParamCollection: Map<string, Set<string>> = new Map();
  lazyForEachInfo: {
    forEachParameters: ts.ParameterDeclaration,
    isDependItem: boolean
  } = {
      forEachParameters: null,
      isDependItem: false
    };
  routerInfo: Map<string, Array<RouterInfo>> = new Map();
  hasLocalBuilderInFile: boolean = false;

  addGlobalCacheInfo(resourceListCacheInfo: string[],
    resourceToFileCacheInfo: { [resource: string]: Set<string> },
    cacheFile: { [fileName: string]: CacheFile }): void {
    if (this.buildStart) {
      for (const element in resourceToFileCacheInfo) {
        this.resourceToFile[element] = new Set(resourceToFileCacheInfo[element]);
      }
      this.lastResourceList = new Set(resourceListCacheInfo);
    }
    if (this.resourceTableChanged) {
      this.compareResourceDiff();
    }
    if (cacheFile) {
      this.transformCacheFiles = cacheFile;
    }
  }

  addFileCacheInfo(id: string, fileCacheInfo: fileInfo): void {
    if (fileCacheInfo && process.env.compileMode === 'moduleJson') {
      if (Array.isArray(fileCacheInfo.fileToResourceList)) {
        fileCacheInfo.fileToResourceList = new Set(fileCacheInfo.fileToResourceList);
      } else {
        fileCacheInfo.fileToResourceList = new Set();
      }
    }
    if (id.match(/(?<!\.d)\.(ets)$/)) {
      this.wholeFileInfo[id] = new SpecialArkTSFileInfo(fileCacheInfo);
    } else if (id.match(/(?<!\.d)\.(ts)$/) && process.env.compileMode === 'moduleJson') {
      this.wholeFileInfo[id] = new TSFileInfo(fileCacheInfo);
    }
  }

  collectTransformedFiles(id: string): void {
    if (id.match(process.env.compileMode === 'moduleJson' ? /(?<!\.d)\.(ets|ts)$/ : /(?<!\.d)\.(ets)$/)) {
      this.transformedFiles.add(id);
    }
  }

  collectCachedFiles(id: string): void {
    if (id.match(process.env.compileMode === 'moduleJson' ? /(?<!\.d)\.(ets|ts)$/ : /(?<!\.d)\.(ets)$/)) {
      this.cachedFiles.push(id);
    }
  }

  judgeShouldHaveEntryFiles(entryFileWithoutEntryDecorator: Set<string>): void {
    this.shouldHaveEntry = Object.values(projectConfig.entryObj as string[]).filter((item) => {
      return !entryFileWithoutEntryDecorator.has(item.toLowerCase()) && item.match(/(?<!\.d)\.(ets)$/);
    });
  }

  saveCacheFileInfo(cache): void {
    if (process.env.compileMode === 'moduleJson') {
      const fileCacheInfo: { [id: string]: fileInfo | tsFileInfo } = cache.get('fileCacheInfo') || {};
      const resourceToFileCacheInfo = cache.get('resourceToFileCacheInfo') || {};
      for (const i in resourceToFileCacheInfo) {
        resourceToFileCacheInfo[i] = new Set(resourceToFileCacheInfo[i]);
      }
      const resourceToFile: { [resource: string]: Set<string> | string[] } = Object.assign(resourceToFileCacheInfo, this.resourceToFile);
      for (const id of this.transformedFiles) {
        fileCacheInfo[id] = this.wholeFileInfo[id].fileInfo;
        for (const resource of this.wholeFileInfo[id].newFileToResourceList) {
          if (!(fileCacheInfo[id].fileToResourceList as Set<string>).has(resource)) {
            this.resourceToFileBecomeSet(resourceToFile, resource, id);
          }
        }
        for (const resource of fileCacheInfo[id].fileToResourceList) {
          if (!this.wholeFileInfo[id].newFileToResourceList.has(resource)) {
            (resourceToFile[resource] as Set<string>).delete(id);
          }
        }
        fileCacheInfo[id].fileToResourceList = [...this.wholeFileInfo[id].newFileToResourceList];
      }
      for (const id of this.cachedFiles) {
        fileCacheInfo[id].fileToResourceList = [...fileCacheInfo[id].fileToResourceList];
      }
      this.resourceToFile = resourceToFile as { [resource: string]: Set<string> };
      for (const resource in resourceToFile) {
        resourceToFile[resource] = [...resourceToFile[resource]];
      }
      cache.set('fileCacheInfo', fileCacheInfo);
      cache.set('resourceListCacheInfo', [...this.resourceList]);
      cache.set('resourceToFileCacheInfo', resourceToFile);
    } else {
      const cacheInfo: { [id: string]: fileInfo } = cache.get('fileCacheInfo') || {};
      for (const id of this.transformedFiles) {
        cacheInfo[id] = this.wholeFileInfo[id].fileInfo;
      }
      cache.set('fileCacheInfo', cacheInfo);
    }
  }

  resourceToFileBecomeSet(resourceToFile: { [resource: string]: Set<string> | string[] }, resource: string, id: string): void {
    if (!resourceToFile[resource]) {
      resourceToFile[resource] = new Set();
    }
    if (resourceToFile[resource] instanceof Set) {
      resourceToFile[resource].add(id);
    } else if (Array.isArray(resourceToFile[resource])) {
      resourceToFile[resource] = new Set(resourceToFile[resource]);
      resourceToFile[resource].add(id);
    } else {
      return;
    }
  }

  updateResourceList(resource: string): void {
    this.resourceList.add(resource);
  }

  compareResourceDiff(): void {
    // delete resource
    for (const resource of this.lastResourceList) {
      if (!this.resourceList.has(resource) && this.resourceToFile[resource]) {
        this.resourceToFile[resource].forEach(file => {
          this.shouldInvalidFiles.add(file);
        });
      }
    }
    // create resource
    for (const resource of this.resourceList) {
      if (!this.resourceToFile[resource]) {
        this.resourceToFile[resource] = new Set();
      }
      if (!this.lastResourceList.has(resource)) {
        this.resourceToFile[resource].forEach(file => {
          this.shouldInvalidFiles.add(file);
        });
      }
    }
  }

  collectResourceInFile(resource: string, file: string): void {
    if (this.wholeFileInfo[file]) {
      this.wholeFileInfo[file].newFileToResourceList.add(resource);
    }
  }

  clearCollectedInfo(cache): void {
    this.buildStart = false;
    this.resourceTableChanged = false;
    this.isAsPageImport = false;
    this.saveCacheFileInfo(cache);
    this.transformedFiles = new Set();
    this.cachedFiles = [];
    this.lastResourceList = new Set([...this.resourceList]);
    this.shouldInvalidFiles.clear();
    this.resourcesArr.clear();
  }
  setCurrentArkTsFile(): void {
    this.currentArkTsFile = new SpecialArkTSFileInfo();
  }
  getCurrentArkTsFile(): SpecialArkTSFileInfo {
    return this.currentArkTsFile;
  }
}

export let storedFileInfo: ProcessFileInfo = new ProcessFileInfo();

export interface fileInfo extends tsFileInfo {
  hasEntry: boolean; // Has @Entry decorator or not
}

export interface tsFileInfo {
  fileToResourceList: Set<string> | string[]; // How much Resource is used
}

// Save single TS file information
class TSFileInfo {
  fileInfo: tsFileInfo = {
    fileToResourceList: new Set()
  };
  newFileToResourceList: Set<string> = new Set();
  constructor(cacheInfo: fileInfo, etsFile?: boolean) {
    if (!etsFile) {
      this.fileInfo = cacheInfo || this.fileInfo;
    }
  }
}

// Save single ArkTS file information
class SpecialArkTSFileInfo extends TSFileInfo {
  fileInfo: fileInfo = {
    hasEntry: false,
    fileToResourceList: new Set()
  };
  recycleComponents: Set<string> = new Set([]);
  reuseComponentsV2: Set<string> = new Set([]);
  compFromDETS: Set<string> = new Set();
  animatableExtendAttribute: Map<string, Set<string>> = new Map();
  pkgName: string;

  constructor(cacheInfo?: fileInfo) {
    super(cacheInfo, true);
    this.fileInfo = cacheInfo || this.fileInfo;
  }

  get hasEntry(): boolean {
    return this.fileInfo.hasEntry;
  }
  set hasEntry(value: boolean) {
    this.fileInfo.hasEntry = value;
  }
}

export function setChecker(): void {
  if (globalProgram.program) {
    globalProgram.checker = globalProgram.program.getTypeChecker();
    globalProgram.strictChecker = globalProgram.program.getLinterTypeChecker();
  } else if (globalProgram.watchProgram) {
    globalProgram.checker = globalProgram.watchProgram.getCurrentProgram().getProgram().getTypeChecker();
  }
}
export interface ExtendResult {
  decoratorName: string;
  componentName: string;
}

export function resourcesRawfile(rawfilePath: string, resourcesArr: Set<string>, resourceName: string = ''): void {
  if (fs.existsSync(process.env.rawFileResource) && fs.statSync(rawfilePath).isDirectory()) {
    const files: string[] = fs.readdirSync(rawfilePath);
    files.forEach((file: string) => {
      if (fs.statSync(path.join(rawfilePath, file)).isDirectory()) {
        resourcesRawfile(path.join(rawfilePath, file), resourcesArr, resourceName ? resourceName + '/' + file : file);
      } else {
        if (resourceName) {
          resourcesArr.add(resourceName + '/' + file);
        } else {
          resourcesArr.add(file);
        }
      }
    });
  }
}

export function differenceResourcesRawfile(oldRawfile: Set<string>, newRawfile: Set<string>): boolean {
  if (oldRawfile.size !== 0 && oldRawfile.size === newRawfile.size) {
    for (const singleRawfiles of oldRawfile.values()) {
      if (!newRawfile.has(singleRawfiles)) {
        return true;
      }
    }
    return false;
  } else if (oldRawfile.size === 0 && oldRawfile.size === newRawfile.size) {
    return false;
  } else {
    return true;
  }
}

export function isString(text: unknown): text is string {
  return typeof text === 'string';
}

function getRollupCacheStoreKey(projectConfig: object): string {
  let keyInfo: string[] = [projectConfig.compileSdkVersion, projectConfig.compatibleSdkVersion, projectConfig.runtimeOS,
    projectConfig.etsLoaderPath];
  return keyInfo.join('#');
}

function getRollupCacheKey(projectConfig: object): string {
  let isWidget: string = projectConfig.widgetCompile ? 'widget' : 'non-widget';
  let ohosTestInfo: string = 'non-ohosTest';
  if (projectConfig.testFrameworkPar) {
    ohosTestInfo = JSON.stringify(projectConfig.testFrameworkPar);
  }

  let keyInfo: string[] = [projectConfig.entryModuleName, projectConfig.targetName, isWidget, ohosTestInfo,
    projectConfig.needCoverageInsert, projectConfig.isOhosTest];
  return keyInfo.join('#');
}

function clearRollupCacheStore(cacheStoreManager: object, currentKey: string): void {
  if (!cacheStoreManager) {
    return;
  }

  for (let key of cacheStoreManager.keys()) {
    if (key !== currentKey) {
      cacheStoreManager.unmount(key);
    }
  }
}

export function startTimeStatisticsLocation(startTimeEvent: CompileEvent): void {
  if (startTimeEvent) {
    startTimeEvent.start();
  }
}

export function stopTimeStatisticsLocation(stopTimeEvent: CompileEvent): void {
  if (stopTimeEvent) {
    stopTimeEvent.stop();
  }
}
export let resolveModuleNamesTime: CompileEvent;
export class CompilationTimeStatistics {
  hookEventFactory: HookEventFactoryType;
  createProgramTime: CompileEvent;
  runArkTSLinterTime: CompileEvent;
  diagnosticTime: CompileEvent;
  scriptSnapshotTime: CompileEvent;
  processImportTime: CompileEvent;
  processComponentClassTime: CompileEvent;
  validateEtsTime: CompileEvent;
  tsProgramEmitTime: CompileEvent;
  shouldEmitJsTime: CompileEvent;
  transformNodesTime: CompileEvent;
  emitTime: CompileEvent;
  printNodeTime: CompileEvent;
  noSourceFileRebuildProgramTime: CompileEvent;
  etsTransformBuildStartTime: CompileEvent;
  etsTransformLoadTime: CompileEvent;
  processKitImportTime: CompileEvent;
  processUISyntaxTime: CompileEvent;
  constructor(share, pluginName: string, hookName: string) {
    if (share && share.getHookEventFactory) {
      if (pluginName === 'etsChecker' && hookName === 'buildStart' && share.getHookEventFactory(pluginName, hookName)) {
        this.hookEventFactory = share.getHookEventFactory(pluginName, hookName);
        this.createProgramTime = this.hookEventFactory.createEvent('createProgram');
        this.runArkTSLinterTime = this.hookEventFactory.createEvent('arkTSLinter');
        this.diagnosticTime = this.hookEventFactory.createEvent('diagnostic');
        this.scriptSnapshotTime = this.createProgramTime.createSubEvent('scriptSnapshot');
        resolveModuleNamesTime = this.hookEventFactory.createEvent('resolveModuleNames');
      } else if (pluginName === 'etsTransform' && hookName === 'transform' && share.getHookEventFactory(pluginName, hookName)) {
        this.hookEventFactory = share.getHookEventFactory(pluginName, hookName);
        this.validateEtsTime = this.hookEventFactory.createEvent('validateEts');
        this.tsProgramEmitTime = this.hookEventFactory.createEvent('tsProgramEmit');
        this.shouldEmitJsTime = this.hookEventFactory.createEvent('shouldEmitJs');
        this.transformNodesTime = this.tsProgramEmitTime.createSubEvent('transformNodes');
        this.emitTime = this.tsProgramEmitTime.createSubEvent('emit');
        this.printNodeTime = this.hookEventFactory.createEvent('printNode');
        this.noSourceFileRebuildProgramTime = this.hookEventFactory.createEvent('noSourceFileRebuildProgram');
        this.processKitImportTime = this.tsProgramEmitTime.createSubEvent('processKitImport');
        this.processUISyntaxTime = this.tsProgramEmitTime.createSubEvent('processUISyntax');
        this.processImportTime = this.processUISyntaxTime.createSubEvent('processImport');
        this.processComponentClassTime = this.processUISyntaxTime.createSubEvent('processComponentClass');
      } else if (pluginName === 'etsTransform' && hookName === 'buildStart' && share.getHookEventFactory(pluginName, hookName)) {
        this.hookEventFactory = share.getHookEventFactory(pluginName, hookName);
        this.etsTransformBuildStartTime = this.hookEventFactory.createEvent('etsTransformBuildStart');
      } else if (pluginName === 'etsTransform' && hookName === 'load' && share.getHookEventFactory(pluginName, hookName)) {
        this.hookEventFactory = share.getHookEventFactory(pluginName, hookName);
        this.etsTransformLoadTime = this.hookEventFactory.createEvent('etsTransformLoad');
      }
    }
  }
}

interface HookEventFactoryType {
  createEvent(name: string): CompileEvent | undefined;
}

type CompileEventState = 'created' | 'beginning' | 'running' | 'failed' | 'success' | 'warn';
interface CompileEvent {
  start(time?: number): CompileEvent;
  stop(state?: CompileEventState, time?: number): void;
  startAsyncEvent(time: number): CompileEvent;
  stopAsyncEvent(state?: CompileEventState, TIME?: number): void;
  createSubEvent(name: string): CompileEvent;
}

export function resetUtils(): void {
  componentInfo = new ComponentInfo();
  harFilesRecord.clear();
  storedFileInfo = new ProcessFileInfo();
}

export function getStoredFileInfo(): ProcessFileInfo {
  return storedFileInfo;
}

export class EntryOptionValue {
  routeName: ts.Expression;
  storage: ts.Expression;
  useSharedStorage: ts.Expression;
}

function sharedNode(): ts.CallExpression {
  return ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_LOCALSTORAGE_TYPE_PU),
      ts.factory.createIdentifier(GET_SHARED)
    ),
    undefined,
    []
  );
}

export function createGetShared(entryOptionValue: EntryOptionValue): ts.ConditionalExpression {
  return ts.factory.createConditionalExpression(
    entryOptionValue.useSharedStorage,
    ts.factory.createToken(ts.SyntaxKind.QuestionToken),
    sharedNode(),
    ts.factory.createToken(ts.SyntaxKind.ColonToken),
    entryOptionValue.storage || ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_UNDEFINED)
  );
}

export function createGetSharedForVariable(entryOptionNode: ts.Expression, isShare: boolean = true): ts.ConditionalExpression {
  return ts.factory.createConditionalExpression(
    ts.factory.createPropertyAccessExpression(
      entryOptionNode,
      ts.factory.createIdentifier(USE_SHARED_STORAGE)
    ),
    ts.factory.createToken(ts.SyntaxKind.QuestionToken),
    sharedNode(),
    ts.factory.createToken(ts.SyntaxKind.ColonToken),
    isShare ? ts.factory.createPropertyAccessExpression(
      entryOptionNode, ts.factory.createIdentifier(STORAGE)) :
      ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_UNDEFINED)
  );
}

export function judgeUseSharedStorageForExpresion(entryOptionNode: ts.Expression): ts.BinaryExpression {
  return ts.factory.createBinaryExpression(
    entryOptionNode,
    ts.factory.createToken(ts.SyntaxKind.AmpersandAmpersandToken),
    ts.factory.createBinaryExpression(
      ts.factory.createPropertyAccessExpression(
        entryOptionNode,
        ts.factory.createIdentifier(USE_SHARED_STORAGE)
      ),
      ts.factory.createToken(ts.SyntaxKind.ExclamationEqualsToken),
      ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_UNDEFINED)
    )
  );
}

export function getRollupCache(rollupShareObject: object, projectConfig: object, key: string): object | undefined {
  if (!rollupShareObject) {
    return undefined;
  }

  // Preferentially get cache object from the rollup’s cache interface.
  if (rollupShareObject.cache) {
    // Only the cache object’s name as the cache key is required.
    return rollupShareObject.cache.get(key);
  }

  // Try to get cache object from the rollup's cacheStoreManager interface.
  if (rollupShareObject.cacheStoreManager) {
    // The cache under cacheStoreManager is divided into two layers. The key for the first layer of cache is determined
    // by the SDK and runtimeOS, accessed through the `mount` interface. The key for the second layer of cache is
    // determined by the compilation task and the name of the cache object,
    // accessed through `getCache` or `setCache` interface.
    const cacheStoreKey: string = getRollupCacheStoreKey(projectConfig);
    const cacheServiceKey: string = getRollupCacheKey(projectConfig) + '#' + key;

    // Clear the cache if the SDK path or runtimeOS changed
    clearRollupCacheStore(rollupShareObject.cacheStoreManager, cacheStoreKey);
    return rollupShareObject.cacheStoreManager.mount(cacheStoreKey).getCache(cacheServiceKey);
  }

  return undefined;
}

export function setRollupCache(rollupShareObject: object, projectConfig: object, key: string, value: object): void {
  if (!rollupShareObject) {
    return;
  }

  // Preferentially set cache object to the rollup’s cache interface.
  if (rollupShareObject.cache) {
    // Only the cache object’s name as the cache key is required.
    rollupShareObject.cache.set(key, value);
    return;
  }

  // Try to set cache object to the rollup's cacheStoreManager interface.
  if (rollupShareObject.cacheStoreManager) {
    const cacheStoreKey: string = getRollupCacheStoreKey(projectConfig);
    const cacheServiceKey: string = getRollupCacheKey(projectConfig) + '#' + key;

    rollupShareObject.cacheStoreManager.mount(cacheStoreKey).setCache(cacheServiceKey, value);
  }
}

export function removeDecorator(decorators: readonly ts.Decorator[], decoratorName: string): readonly ts.Decorator[] {
  return decorators.filter((item: ts.Node) => {
    if (ts.isDecorator(item) && ts.isIdentifier(item.expression) &&
      item.expression.escapedText.toString() === decoratorName) {
      return false;
    }
    return true;
  });
}

export function isFileInProject(filePath: string, projectRootPath: string): boolean {
  const relativeFilePath: string = toUnixPath(path.relative(toUnixPath(projectRootPath), toUnixPath(filePath)));
  // When processing ohmurl, hsp's filePath is consistent with moduleRequest
  return fs.existsSync(filePath) && fs.statSync(filePath).isFile() && !relativeFilePath.startsWith('../');
}

export function getProjectRootPath(filePath: string, projectConfig: Object, rootPathSet: Object): string {
  if (rootPathSet) {
    for (const rootPath of rootPathSet) {
      if (isFileInProject(filePath, rootPath)) {
        return rootPath;
      }
    }
  }
  return projectConfig.projectRootPath;
}

export function getBelongModuleInfo(filePath: string, modulePathMap: Object, projectRootPath: string): Object {
  for (const moduleName of Object.keys(modulePathMap)) {
    if (toUnixPath(filePath).startsWith(toUnixPath(modulePathMap[moduleName]) + '/')) {
      return {
        isLocalDependency: true,
        moduleName: moduleName,
        belongModulePath: modulePathMap[moduleName]
      };
    }
  }
  return {
    isLocalDependency: false,
    moduleName: '',
    belongModulePath: projectRootPath
  };
}