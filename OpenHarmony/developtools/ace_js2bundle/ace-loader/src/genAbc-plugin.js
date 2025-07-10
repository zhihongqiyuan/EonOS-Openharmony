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

const fs = require('fs');
const path = require('path');
const cluster = require('cluster');
const process = require('process');
const crypto = require('crypto');
const events = require('events');
const os = require('os');
const childProcess = require('child_process');

const forward = '(global.___mainEntry___ = function (globalObjects) {' + '\n' +
              '  var define = globalObjects.define;' + '\n' +
              '  var require = globalObjects.require;' + '\n' +
              '  var bootstrap = globalObjects.bootstrap;' + '\n' +
              '  var register = globalObjects.register;' + '\n' +
              '  var render = globalObjects.render;' + '\n' +
              '  var $app_define$ = globalObjects.$app_define$;' + '\n' +
              '  var $app_bootstrap$ = globalObjects.$app_bootstrap$;' + '\n' +
              '  var $app_require$ = globalObjects.$app_require$;' + '\n' +
              '  var history = globalObjects.history;' + '\n' +
              '  var Image = globalObjects.Image;' + '\n' +
              '  var OffscreenCanvas = globalObjects.OffscreenCanvas;' + '\n' +
              '  (function(global) {' + '\n' +
              '    "use strict";' + '\n';
const last = '\n' + '})(this.__appProto__);' + '\n' + '})';
const genAbcScript = 'gen-abc.js';
let output;
let isWin = false;
let isMac = false;
let isDebug = false;
let arkDir;
let nodeJs;
let intermediateJsBundle = [];
let workerFile = null;
let fileterIntermediateJsBundle = [];
let hashJsonObject = {};
let buildPathInfo = "";
const SUCCESS = 0;
const FAIL = 1;
const red = '\u001b[31m';
const reset = '\u001b[39m';
const blue = '\u001b[34m';
const hashFile = 'gen_hash.json';
const ARK = '/ark/';
const NODE_MODULES = 'node_modules';
const TEMPORARY = 'temporary';
const TS2ABC = 'ts2abc';
const ES2ABC = 'es2abc';
const WINDOWS = 'Windows_NT';
const LINUX = 'Linux';
const OPENHARMONY = 'openharmony';
const MAC = 'Darwin';
const FILESINFO_TXT = 'filesInfo.txt';
const manageBunldeWorkersScript = 'manage-bundle-workers.js';
const PREBUILDINFO_JSON = 'preBuildInfo.json';

class GenAbcPlugin {
  constructor(output_, arkDir_, nodeJs_, workerFile_, isDebug_) {
    output = output_;
    arkDir = arkDir_;
    nodeJs = nodeJs_;
    isDebug = isDebug_;
    workerFile = workerFile_;
  }
  apply(compiler) {
    if (fs.existsSync(path.resolve(arkDir, 'build-win'))) {
      isWin = true;
    } else if (fs.existsSync(path.resolve(arkDir, 'build-mac'))) {
      isMac = true;
    } else if (!fs.existsSync(path.resolve(arkDir, 'build'))) {
      console.error(red, 'ERROR find build fail', reset);
      process.exitCode = FAIL;
      return;
    }

    if (!checkNodeModules()) {
      process.exitCode = FAIL;
      return;
    }

    // for preview mode max listeners
    events.EventEmitter.defaultMaxListeners = 100;

    compiler.hooks.emit.tap('GenAbcPlugin', (compilation) => {
      const assets = compilation.assets;
      const keys = Object.keys(assets);
      buildPathInfo = output;
      keys.forEach(key => {
        // choice *.js
        if (output && path.extname(key) === '.js') {
          let newContent = assets[key].source();
          if (checkWorksFile(key, workerFile) && key !== 'commons.js' && key !== 'vendors.js') {
            newContent = forward + newContent + last;
          }
          if (key === 'commons.js' || key === 'vendors.js' || !checkWorksFile(key, workerFile)) {
            newContent = `\n\n\n\n\n\n\n\n\n\n\n\n\n\n` + newContent;
          }
          const keyPath = key.replace(/\.js$/, ".temp.js");
          writeFileSync(newContent, output, keyPath, key, true);
        } else if (output && path.extname(key) === '.json' &&
          process.env.DEVICE_LEVEL === 'card' && process.env.configOutput && !checkI18n(key)) {
          writeFileSync(assets[key].source(), output, key, key, false);
        }
      })
    });
    compiler.hooks.afterEmit.tap('GenAbcPluginMultiThread', () => {
      if (intermediateJsBundle.length === 0) {
        return;
      }
      buildPathInfo = output;
      clearWebpackCacheByBuildInfo();
      setPrebuildInfo();
      processMultiThreadEntry();
    });
  }
}

function processMultiThreadEntry() {
  if (isTs2Abc() || process.env.minPlatformVersion === "8") {
      invokeWorkerToGenAbc();
  } else if (isEs2Abc()) {
    generateAbcByEs2AbcOfBundleMode(intermediateJsBundle);
  } else {
    console.debug(red, `ERROR please set panda module`, reset);
  }
}

function checkI18n(key) {
  const outI18nPath = path.resolve(process.env.configOutput, key);
  const projectI18nPath = outI18nPath.replace(output, process.env.projectPath);
  if (projectI18nPath.indexOf(
    path.resolve(__dirname, process.env.projectPath, 'i18n')) > -1) {
    return true;
  }
  return false;
}

function checkWorksFile(assetPath, workerFile) {
  if (workerFile === null) {
    if (assetPath.search("./workers/") !== 0) {
      return true;
    } else {
      return false;
    }
  } else {
    for (const key in workerFile) {
      let keyExt = key + '.js';
      if (keyExt === assetPath) {
        return false;
      }
    }
  }

  return true;
}

function writeFileSync(inputString, buildPath, keyPath, jsBundleFile, isToBin) {
    let output = path.resolve(buildPath, keyPath);
    validateFilePathLength(output);
    let parent = path.join(output, '..');
    if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
        mkDir(parent);
    }
    if (!isToBin) {
      fs.writeFileSync(output, inputString);
      return;
    }
    let cacheOutputPath = "";
    if (process.env.cachePath) {
      let buildDirArr = buildPathInfo.split(path.sep);
      let abilityDir = buildDirArr[buildDirArr.length - 1];
      cacheOutputPath = path.join(process.env.cachePath, TEMPORARY, abilityDir, keyPath);
    } else {
      cacheOutputPath = output;
    }
    validateFilePathLength(cacheOutputPath);
    parent = path.join(cacheOutputPath, '..');
    if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
      mkDir(parent);
    }
    fs.writeFileSync(cacheOutputPath, inputString);
    if (fs.existsSync(cacheOutputPath)) {
      let fileSize = fs.statSync(cacheOutputPath).size;
      let sourceFile = output.replace(/\.temp\.js$/, "_.js");
      if (!isDebug && (process.env.aceBuildJson && fs.existsSync(process.env.aceBuildJson))) {
        const buildJsonInfo = JSON.parse(fs.readFileSync(process.env.aceBuildJson).toString());
        sourceFile = toUnixPath(sourceFile.replace(buildJsonInfo.projectRootPath + path.sep, ''));
      } else {
        sourceFile = toUnixPath(sourceFile.replace(process.env.projectRootPath + path.sep, ''));
      }
      output = toUnixPath(output);
      cacheOutputPath = toUnixPath(cacheOutputPath);
      intermediateJsBundle.push({
        path: output, size: fileSize, cacheOutputPath: cacheOutputPath, sourceFile: sourceFile
      });
    } else {
      console.debug(red, `ERROR Failed to convert file ${jsBundleFile} to bin. ${output} is lost`, reset);
      process.exitCode = FAIL;
    }
}

function mkDir(path_) {
    const parent = path.join(path_, '..');
    if (!(fs.existsSync(parent) && !fs.statSync(parent).isFile())) {
        mkDir(parent);
    }
    fs.mkdirSync(path_);
}

function getSmallestSizeGroup(groupSize) {
  let groupSizeArray = Array.from(groupSize);
  groupSizeArray.sort(function(g1, g2) {
    return g1[1] - g2[1]; // sort by size
  });
  return groupSizeArray[0][0];
}

function splitJsBundlesBySize(bundleArray, groupNumber) {
  let result = [];
  if (bundleArray.length < groupNumber) {
    for (let value of bundleArray) {
      result.push([value]);
    }
    return result;
  }

  bundleArray.sort(function(f1, f2) {
    return f2.size - f1.size;
  });
  let groupFileSize = new Map();
  for (let i = 0; i < groupNumber; ++i) {
    result.push([]);
    groupFileSize.set(i, 0);
  }

  let index = 0;
  while(index < bundleArray.length) {
    let smallestGroup = getSmallestSizeGroup(groupFileSize);
    result[smallestGroup].push(bundleArray[index]);
    let sizeUpdate = groupFileSize.get(smallestGroup) + bundleArray[index].size;
    groupFileSize.set(smallestGroup, sizeUpdate);
    index++;
  }
  return result;
}

function invokeWorkerToGenAbc() {
  if (process.env.isPreview === "true") {
    process.exitCode = SUCCESS;
  }
  let maxWorkerNumber = isEs2Abc() ? os.cpus().length : 3;
  const abcArgs = initAbcEnv();
  let cmdPrefix = initCmdPrefix(abcArgs);

  filterIntermediateJsBundleByHashJson(buildPathInfo, intermediateJsBundle);
  const splitedBundles = splitJsBundlesBySize(fileterIntermediateJsBundle, maxWorkerNumber);
  const workerNumber = maxWorkerNumber < splitedBundles.length ? maxWorkerNumber : splitedBundles.length;

  try  {
    if (process.env.isPreview === 'true') {
      processWorkersOfPreviewMode(splitedBundles, cmdPrefix, workerNumber);
    } else {
      processWorkersOfBuildMode(splitedBundles, cmdPrefix, workerNumber);
    }
  } catch (e) {
    console.debug(red, `ERROR failed to generate abc. Error message: ${e} `, reset);
    process.env.abcCompileSuccess = 'false';
    if (process.env.isPreview !== 'true') {
      process.exit(FAIL);
    }
  }
}

function clearGlobalInfo() {
  if (process.env.isPreview !== "true") {
    intermediateJsBundle = [];
  }
  fileterIntermediateJsBundle = [];
  hashJsonObject = {};
}

function filterIntermediateJsBundleByHashJson(buildPath, inputPaths) {
  inputPaths = removeDuplicateInfoOfBundleList(inputPaths);

  for (let i = 0; i < inputPaths.length; ++i) {
    fileterIntermediateJsBundle.push(inputPaths[i]);
  }
  const hashFilePath = genHashJsonPath(buildPath);
  if (hashFilePath.length == 0) {
    return;
  }
  let updateJsonObject = {};
  let jsonObject = {};
  let jsonFile = "";
  if (fs.existsSync(hashFilePath)) {
    jsonFile = fs.readFileSync(hashFilePath).toString();
    jsonObject = JSON.parse(jsonFile);
    fileterIntermediateJsBundle = [];
    for (let i = 0; i < inputPaths.length; ++i) {
      const cacheOutputPath = inputPaths[i].cacheOutputPath;
      const cacheAbcFilePath = cacheOutputPath.replace(/\.temp\.js$/, '.abc');
      if (!fs.existsSync(cacheOutputPath)) {
        console.debug(red, `ERROR ${cacheOutputPath} is lost`, reset);
        process.exitCode = FAIL;
        break;
      }

      if (fs.existsSync(cacheAbcFilePath)) {
        const hashInputContentData = toHashData(cacheOutputPath);
        const hashAbcContentData = toHashData(cacheAbcFilePath);
        if (jsonObject[cacheOutputPath] === hashInputContentData && jsonObject[cacheAbcFilePath] === hashAbcContentData) {
          updateJsonObject[cacheOutputPath] = hashInputContentData;
          updateJsonObject[cacheAbcFilePath] = hashAbcContentData;
        } else {
          fileterIntermediateJsBundle.push(inputPaths[i]);
        }
      } else {
        fileterIntermediateJsBundle.push(inputPaths[i]);
      }
    }
  }

  hashJsonObject = updateJsonObject;
}

function writeHashJson() {
  for (let i = 0; i < fileterIntermediateJsBundle.length; ++i) {
    const cacheOutputPath = fileterIntermediateJsBundle[i].cacheOutputPath;
    const cacheAbcFilePath = cacheOutputPath.replace(/\.temp\.js$/, '.abc');
    if (!fs.existsSync(cacheOutputPath) || !fs.existsSync(cacheAbcFilePath)) {
      console.debug(red, `ERROR ${cacheOutputPath} is lost`, reset);
      process.exitCode = FAIL;
      break;
    }
    const hashInputContentData = toHashData(cacheOutputPath);
    const hashAbcContentData = toHashData(cacheAbcFilePath);
    hashJsonObject[cacheOutputPath] = hashInputContentData;
    hashJsonObject[cacheAbcFilePath] = hashAbcContentData;
  }
  const hashFilePath = genHashJsonPath(buildPathInfo);
  if (hashFilePath.length == 0) {
    return;
  }
  fs.writeFileSync(hashFilePath, JSON.stringify(hashJsonObject));
}

function genHashJsonPath(buildPath) {
  buildPath = toUnixPath(buildPath);
  if (process.env.cachePath) {
    if (!fs.existsSync(process.env.cachePath) || !fs.statSync(process.env.cachePath).isDirectory()) {
      return '';
    }
    let buildDirArr = buildPathInfo.split(path.sep);
    let abilityDir = buildDirArr[buildDirArr.length - 1];
    let hashJsonPath = path.join(process.env.cachePath, TEMPORARY, abilityDir, hashFile);
    validateFilePathLength(hashJsonPath);
    let parent = path.join(hashJsonPath, '..');
    if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
      mkDir(parent);
    }
    return hashJsonPath;
  } else if (buildPath.indexOf(ARK) >= 0) {
    const dataTmps = buildPath.split(ARK);
    const hashPath = path.join(dataTmps[0], ARK);
    if (!fs.existsSync(hashPath) || !fs.statSync(hashPath).isDirectory()) {
      return '';
    }
    let hashJsonPath = path.join(hashPath, hashFile);
    validateFilePathLength(hashJsonPath);
    return hashJsonPath;
  } else {
    return '';
  }
}

function toUnixPath(data) {
  if (/^win/.test(require('os').platform())) {
    const fileTmps = data.split(path.sep);
    const newData = path.posix.join(...fileTmps);
    return newData;
  }
  return data;
}

function toHashData(path) {
  const content = fs.readFileSync(path);
  const hash = crypto.createHash('sha256');
  hash.update(content);
  return hash.digest('hex');
}

module.exports = {
  GenAbcPlugin: GenAbcPlugin,
  checkWorksFile: checkWorksFile
}

function copyFileCachePathToBuildPath() {
  for (let i = 0; i < intermediateJsBundle.length; ++i) {
    const abcFile = intermediateJsBundle[i].path.replace(/\.temp\.js$/, ".abc");
    const cacheOutputPath = intermediateJsBundle[i].cacheOutputPath;
    const cacheAbcFilePath = intermediateJsBundle[i].cacheOutputPath.replace(/\.temp\.js$/, ".abc");
    if (!fs.existsSync(cacheAbcFilePath)) {
      console.debug(red, `ERROR ${cacheAbcFilePath} is lost`, reset);
      process.exitCode = FAIL;
      break;
    }
    let parent = path.join(abcFile, '..');
    if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
      mkDir(parent);
    }
    // for preview mode, cache path and old abc file both exist, should copy abc file for updating
    if (process.env.cachePath !== undefined) {
      fs.copyFileSync(cacheAbcFilePath, abcFile);
    }
    if (process.env.cachePath === undefined && fs.existsSync(cacheOutputPath)) {
      fs.unlinkSync(cacheOutputPath);
    }
  }
}

function processExtraAssetForBundle() {
  writeHashJson();
  copyFileCachePathToBuildPath();
  clearGlobalInfo();
}

function checkNodeModules() {
  if (process.env.panda === TS2ABC) {
    let arkEntryPath = path.join(arkDir, 'build');
    if (isWin) {
      arkEntryPath = path.join(arkDir, 'build-win');
    } else if (isMac) {
      arkEntryPath = path.join(arkDir, 'build-mac');
    }
    let nodeModulesPath = path.join(arkEntryPath, NODE_MODULES);
    validateFilePathLength(nodeModulesPath);
    if (!(fs.existsSync(nodeModulesPath) && fs.statSync(nodeModulesPath).isDirectory())) {
      console.error(red, `ERROR: node_modules for ark compiler not found.
        Please make sure switch to non-root user before runing "npm install" for safity requirements and try re-run "npm install" under ${arkEntryPath}`, reset);
      return false;
    }
  }

  return true;
}

function initAbcEnv() {
  let args = [];
  if (process.env.minPlatformVersion === "8") {
    process.env.panda = TS2ABC;
    let js2abc = path.join(arkDir, 'build', 'legacy_api8', 'src', 'index.js');
    if (isWin) {
      js2abc = path.join(arkDir, 'build-win', 'legacy_api8', 'src', 'index.js');
    } else if (isMac) {
      js2abc = path.join(arkDir, 'build-mac', 'legacy_api8', 'src', 'index.js');
    }
    validateFilePathLength(js2abc);

    js2abc = '"' + js2abc + '"';
    args = [
      '--expose-gc',
      js2abc
    ];
    if (isDebug) {
      args.push('--debug');
    }
  } else if (process.env.panda === TS2ABC) {
    let js2abc = path.join(arkDir, 'build', 'src', 'index.js');
    if (isWin) {
      js2abc = path.join(arkDir, 'build-win', 'src', 'index.js');
    } else if (isMac) {
      js2abc = path.join(arkDir, 'build-mac', 'src', 'index.js');
    }
    validateFilePathLength(js2abc);

    js2abc = '"' + js2abc + '"';
    args = [
      '--expose-gc',
      js2abc
    ];
    if (isDebug) {
      args.push('--debug');
    }
  } else if (process.env.panda === ES2ABC  || process.env.panda === 'undefined' || process.env.panda === undefined) {
    let es2abc = path.join(arkDir, 'build', 'bin', 'es2abc');
    if (isWin) {
      es2abc = path.join(arkDir, 'build-win', 'bin', 'es2abc.exe');
    } else if (isMac) {
      es2abc = path.join(arkDir, 'build-mac', 'bin', 'es2abc');
    }
    validateFilePathLength(es2abc);

    args = [
      '"' + es2abc + '"'
    ];
    if (isDebug) {
      args.push('--debug-info');
    }
  }  else {
    console.debug(red, `ERROR: please set panda module`, reset);
  }

  return args;
}

export function isWindows() {
  return os.type() === WINDOWS;
}

export function isLinux() {
  return os.type() === LINUX;
}

export function isMacOs() {
  return os.type() === MAC;
}

export function isOpenHarmony() {
  return os.platform() === OPENHARMONY;
}

export function maxFilePathLength() {
  if (isWindows()) {
    return 32766;
  } else if (isLinux() || isOpenHarmony()) {
    return 4095;
  } else if (isMacOs()) {
    return 1016;
  } else {
    return -1;
  }
}

export function validateFilePathLength(filePath) {
  if (maxFilePathLength() < 0) {
    console.error("Unknown OS platform");
    process.exitCode = FAIL;
    return false;
  } else if (filePath.length > 0 && filePath.length <= maxFilePathLength()) {
    return true;
  } else if (filePath.length > maxFilePathLength()) {
    console.error(`The length of ${filePath} exceeds the limitation of current platform, which is ` +
    `${maxFilePathLength()}. Please try moving the project folder to avoid deeply nested file path and try again`);
    process.exitCode = FAIL;
    return false;
  } else {
    console.error("Validate file path failed");
    process.exitCode = FAIL;
    return false;
  }
}

export function isEs2Abc() {
  return process.env.panda === ES2ABC  || process.env.panda === 'undefined' || process.env.panda === undefined;
}

export function isTs2Abc() {
  return process.env.panda === TS2ABC;
}

function generateAbcByEs2AbcOfBundleMode(inputPaths) {
  filterIntermediateJsBundleByHashJson(buildPathInfo, inputPaths);
  if (fileterIntermediateJsBundle.length === 0) {
    processExtraAssetForBundle();
    return;
  }
  let filesInfoPath = generateFileOfBundle(fileterIntermediateJsBundle);
  const fileThreads = os.cpus().length < 16 ? os.cpus().length : 16;
  let genAbcCmd =
      `${initAbcEnv().join(' ')} "@${filesInfoPath}" --file-threads "${fileThreads}"`;

  console.debug('gen abc cmd is: ', genAbcCmd);
  try {
    if (process.env.isPreview === 'true') {
      childProcess.execSync(genAbcCmd);
      processExtraAssetForBundle();
    } else {
      const child = childProcess.exec(genAbcCmd);
      child.on('exit', (code) => {
        if (code === FAIL) {
          console.debug(red, "ERROR failed to execute es2abc", reset);
          process.exit(FAIL);
        }
        if (process.env.cachePath === undefined) {
          unlinkSync(filesInfoPath);
        }
        processExtraAssetForBundle();
      });

      child.on('error', (err) => {
        console.debug(red, err.toString(), reset);
        process.exit(FAIL);
      });

      child.stderr.on('data', (data) => {
        console.debug(red, data.toString(), reset);
      });
    }
  } catch (e) {
    console.debug(red, `ERROR failed to generate abc with filesInfo ${filesInfoPath}. Error message: ${e} `, reset);
    process.env.abcCompileSuccess = 'false';
    if (process.env.isPreview !== 'true') {
      process.exit(FAIL);
    }
  } finally {
    if (process.env.isPreview === 'true') {
      if (process.env.cachePath === undefined) {
        unlinkSync(filesInfoPath);
      }
    }
  }
}

function generateFileOfBundle(inputPaths) {
  let filesInfoPath = buildCachePath(FILESINFO_TXT);
  inputPaths = removeDuplicateInfoOfBundleList(inputPaths);

  let filesInfo = '';
  inputPaths.forEach(info => {
    const cacheOutputPath = info.cacheOutputPath;
    const recordName = 'null_recordName';
    const moduleType = 'script';
    const sourceFile = info.sourceFile;
    const abcFilePath = cacheOutputPath.replace(/\.temp\.js$/, ".abc");
    filesInfo += `${cacheOutputPath};${recordName};${moduleType};${sourceFile};${abcFilePath}\n`;
  });
  fs.writeFileSync(filesInfoPath, filesInfo, 'utf-8');

  return filesInfoPath;
}

function removeDuplicateInfoOfBundleList(inputPaths) {
  let tempInputPaths = [];
  inputPaths.forEach((item) => {
    const check = tempInputPaths.every((newItem) => {
      return item.path !== newItem.path;
    });
    if (check) {
      tempInputPaths.push(item);
    }
  });
  inputPaths = tempInputPaths;

  return inputPaths;
}

function buildCachePath(tailName) {
  let pathName = process.env.cachePath !== undefined ?
      path.join(process.env.cachePath, tailName) : path.join(buildPathInfo, tailName);
  validateFilePathLength(pathName);

  return pathName;
}

function unlinkSync(filePath) {
  if (fs.existsSync(filePath)) {
    fs.unlinkSync(filePath);
  }
}

function initCmdPrefix(abcArgs) {
  let cmdPrefix = "";
  if (process.env.panda === TS2ABC) {
    cmdPrefix = `${nodeJs} ${abcArgs.join(' ')}`;
  } else if (process.env.panda === ES2ABC  || process.env.panda === 'undefined' || process.env.panda === undefined) {
    cmdPrefix = `${abcArgs.join(' ')}`;
  } else {
    console.debug(red, `ERROR please set panda module`, reset);
  }

  return cmdPrefix;
}

function processWorkersOfPreviewMode(splittedData, cmdPrefix, workerNumber) {
  let processEnv = Object.assign({}, process.env);
  let arkEnvParams = {
    'splittedData': JSON.stringify(splittedData),
    'cmdPrefix': cmdPrefix,
    'workerNumber': workerNumber.toString()
  };
  processEnv.arkEnvParams = JSON.stringify(arkEnvParams);

  let genAbcCmd = `${nodeJs} "${path.resolve(__dirname, manageBunldeWorkersScript)}"`;
  childProcess.execSync(genAbcCmd, {env: processEnv});
  processExtraAssetForBundle();
}

function processWorkersOfBuildMode(splittedData, cmdPrefix, workerNumber) {
  const clusterNewApiVersion = 16;
  const currentNodeVersion = parseInt(process.version.split(".")[0]);
  const useNewApi = currentNodeVersion >= clusterNewApiVersion;

  if (useNewApi && cluster.isPrimary || !useNewApi && cluster.isMaster) {
    if (useNewApi) {
      cluster.setupPrimary({
        exec: path.resolve(__dirname, genAbcScript)
      });
    } else {
      cluster.setupMaster({
        exec: path.resolve(__dirname, genAbcScript)
      });
    }

    for (let i = 0; i < workerNumber; ++i) {
      let workerData = {
        'inputs': JSON.stringify(splittedData[i]),
        'cmd': cmdPrefix
      };
      cluster.fork(workerData);
    }

    cluster.on('exit', (worker, code, signal) => {
      if (code === FAIL) {
        process.exitCode = FAIL;
      }
      console.debug(`worker ${worker.process.pid} finished`);
    });

    process.on('exit', (code) => {
      if (process.exitCode !== FAIL && process.env.isPreview !== 'true') {
        processExtraAssetForBundle();
      }
    });
  }
}

function clearWebpackCacheByBuildInfo() {
  if (!process.env.cachePath) {
    return;
  }

  // clear&update cache dir when build info is different from last time
  const cachePrebuildInfoPath = path.join(process.env.cachePath, PREBUILDINFO_JSON);
  if (fs.existsSync(cachePrebuildInfoPath)) {
    let cachedJson = undefined;
    try {
      cachedJson = JSON.parse(fs.readFileSync(cachePrebuildInfoPath).toString());
    } catch {
      removeHashJsonFile();
      return;
    }
    // api version is 8 or 9
    if (cachedJson && cachedJson.minAPIVersion &&
      cachedJson.minAPIVersion.toString() !== process.env.minPlatformVersion) {
      removeHashJsonFile();
    }
  }
}

function setPrebuildInfo() {
  if (process.env.cachePath && process.env.minPlatformVersion) {
    let cachedJson = {};
    const cachePrebuildInfoPath = path.join(process.env.cachePath, PREBUILDINFO_JSON);
    validateFilePathLength(cachePrebuildInfoPath);
    cachedJson.minAPIVersion = process.env.minPlatformVersion;
    fs.writeFile(cachePrebuildInfoPath, JSON.stringify(cachedJson, null, 2), 'utf-8',
      (err) => {
        if (err) {
          logger.error(red, `ArkTS:ERROR Failed to write bundle build info.`, reset);
        }
      }
    );
  }
}

function removeHashJsonFile() {
  const hashFilePath = genHashJsonPath(buildPathInfo);
  if (hashFilePath.length !== 0 && fs.existsSync(hashFilePath)) {
    fs.unlinkSync(hashFilePath);
  }
}
