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
const SingleEntryPlugin = require('webpack/lib/SingleEntryPlugin');

const CUSTOM_THEME_PROP_GROUPS = require('./theme/customThemeStyles');
const OHOS_THEME_PROP_GROUPS = require('./theme/ohosStyles');
import { mkDir } from './util';
import { multiResourceBuild } from '../main.product';

const FILE_EXT_NAME = ['.js', '.css', '.jsx', '.less', '.sass', '.scss', '.md', '.DS_Store', '.hml', '.json'];
const red = '\u001b[31m';
const reset = '\u001b[39m';
let input = '';
let output = '';
let manifestFilePath = '';
let watchCSSFiles;
let shareThemePath = '';
let internalThemePath = '';
let resourcesPath;
let sharePath;
let workerFile;

function copyFile(input, output) {
  try {
    if (fs.existsSync(input)) {
      const parent = path.join(output, '..');
      if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
        mkDir(parent);
      }
      const pathInfo = path.parse(input);
      const entryObj = addPageEntryObj();
      const indexPath = pathInfo.dir + path.sep + pathInfo.name + '.hml?entry';
      for (const key in entryObj) {
        if (entryObj[key] === indexPath) {
          return;
        }
      }
      if (pathInfo.ext === '.json' && (pathInfo.dir === shareThemePath ||
        pathInfo.dir === internalThemePath)) {
        if (themeFileBuild(input, output)) {
          return;
        }
      }
      const readStream = fs.createReadStream(input);
      const writeStream = fs.createWriteStream(output);
      readStream.pipe(writeStream);
      readStream.on('close', function() {
        writeStream.end();
      });
    }
  } catch (e) {
    if (e && /ERROR: /.test(e)) {
      throw e;
    } else {
      throw new Error(`${red}Failed to build file ${input}.${reset}`).message;
    }
  }
}

function circularFile(inputPath, outputPath, ext) {
  const realPath = path.join(inputPath, ext);
  if (!fs.existsSync(realPath) || realPath === output) {
    return;
  }
  fs.readdirSync(realPath).forEach(function(file_) {
    const file = path.join(realPath, file_);
    const fileStat = fs.statSync(file);
    if (fileStat.isFile()) {
      const baseName = path.basename(file);
      const extName = path.extname(file);
      const outputFile = path.join(outputPath, ext, path.basename(file_));
      if (outputFile === path.join(output, 'manifest.json')) {
        return;
      }
      if (FILE_EXT_NAME.indexOf(extName) < 0 && baseName !== '.DS_Store') {
        toCopyFile(file, outputFile, fileStat)
      } else if (extName === '.json') {
        const parent = path.join(file, '..');
        const parent_ = path.join(parent, '..');
        if (path.parse(parent).name === 'i18n' && path.parse(parent_).name === 'share') {
          toCopyFile(file, outputFile, fileStat)
        } else if (file.toString().startsWith(resourcesPath) || file.toString().startsWith(sharePath)) {
          toCopyFile(file, outputFile, fileStat)
        }
      }
    } else if (fileStat.isDirectory()) {
      circularFile(inputPath, outputPath, path.join(ext, file_));
    }
  });
}

function toCopyFile(file, outputFile, fileStat) {
  if (fs.existsSync(outputFile)) {
    const outputFileStat = fs.statSync(outputFile);
    if (outputFileStat.isFile() && fileStat.size !== outputFileStat.size) {
      copyFile(file, outputFile);
    }
  } else {
    copyFile(file, outputFile);
  }
}

class ResourcePlugin {
  constructor(input_, output_, manifestFilePath_, watchCSSFiles_, workerFile_ = null) {
    input = input_;
    output = output_;
    manifestFilePath = manifestFilePath_;
    watchCSSFiles = watchCSSFiles_;
    shareThemePath = path.join(input_, '../share/resources/styles');
    internalThemePath = path.join(input_, 'resources/styles');
    resourcesPath = path.resolve(input_, 'resources');
    sharePath = path.resolve(input_, '../share/resources');
    workerFile = workerFile_;
  }
  apply(compiler) {
    compiler.hooks.beforeCompile.tap('resource Copy', () => {
      if (multiResourceBuild.value) {
        const res = multiResourceBuild.value.res;
        if (res) {
          res.forEach(item => {
            circularFile(path.resolve(input, item), path.resolve(output, item), '');
          })
        }
      } else {
        circularFile(input, output, '');
      }
      circularFile(input, output, '../share');
    });
    compiler.hooks.watchRun.tap('i18n', (comp) => {
      checkRemove(comp);
    });
    compiler.hooks.normalModuleFactory.tap('OtherEntryOptionPlugin', () => {
      if (process.env.abilityType === 'testrunner') {
        checkTestRunner(input, entryObj);
        for (const key in entryObj) {
          const singleEntry = new SingleEntryPlugin('', entryObj[key], key);
          singleEntry.apply(compiler);
        }
      } else {
        const cssFiles = readCSSInfo(watchCSSFiles);
        if (process.env.DEVICE_LEVEL === 'card' && process.env.compileMode !== 'moduleJson') {
          for(const entryKey in compiler.options.entry) {
            setCSSEntry(cssFiles, entryKey);
          }
          writeCSSInfo(watchCSSFiles, cssFiles);
          return;
        }
        addPageEntryObj();
        entryObj = Object.assign(entryObj, abilityEntryObj);
        for (const key in entryObj) {
          if (!compiler.options.entry[key]) {
            const singleEntry = new SingleEntryPlugin('', entryObj[key], key);
            singleEntry.apply(compiler);
          }
          setCSSEntry(cssFiles, key);
        }
        writeCSSInfo(watchCSSFiles, cssFiles);
      }
    });
    compiler.hooks.done.tap('copyManifest', () => {
      copyManifest();
    });
  }
}

function checkRemove(comp) {
  const removedFiles = comp.removedFiles || [];
  removedFiles.forEach(file => {
    if (file.indexOf(process.env.projectPath) > -1 && path.extname(file) === '.json' &&
      file.indexOf('i18n') > -1) {
      const buildFilePath = file.replace(process.env.projectPath, process.env.buildPath);
      if (fs.existsSync(buildFilePath)) {
        fs.unlinkSync(buildFilePath);
      }
    }
  })
}

function copyManifest() {
  copyFile(manifestFilePath, path.join(output, 'manifest.json'));
  if (fs.existsSync(path.join(output, 'app.js'))) {
    fs.utimesSync(path.join(output, 'app.js'), new Date(), new Date());
  }
}

let entryObj = {};
let configPath;

function addPageEntryObj() {
  entryObj = {};
  if (process.env.abilityType === 'page') {
    let jsonContent;
    if (multiResourceBuild.value && Object.keys(multiResourceBuild.value).length) {
      jsonContent = multiResourceBuild.value;
    } else {
      jsonContent = readManifest(manifestFilePath);
    }
    const pages = jsonContent.pages;
    if (pages === undefined) {
      throw Error('ERROR: missing pages').message;
    }
    pages.forEach((element) => {
      const sourcePath = element.replace(/^\.\/js\//, '');
      const hmlPath = path.join(input, sourcePath + '.hml');
      const aceSuperVisualPath = process.env.aceSuperVisualPath || '';
      const visualPath = path.join(aceSuperVisualPath, sourcePath + '.visual');
      const isHml = fs.existsSync(hmlPath);
      const isVisual = fs.existsSync(visualPath);
      const projectPath = process.env.projectPath;
      if (isHml && isVisual) {
        console.error('ERROR: ' + sourcePath + ' cannot both have hml && visual');
      } else if (isHml) {
        entryObj['./' + sourcePath] = path.resolve(projectPath, './' + sourcePath + '.hml?entry');
      } else if (isVisual) {
        entryObj['./' + sourcePath] = path.resolve(aceSuperVisualPath, './' + sourcePath +
          '.visual?entry');
      } else {
        entryErrorLog(sourcePath);
      }
    });
  }
  if (process.env.isPreview !== 'true' && process.env.DEVICE_LEVEL === 'rich') {
    loadWorker(entryObj);
  }
  return entryObj;
}

function entryErrorLog(sourcePath) {
  if (process.env.watchMode && process.env.watchMode === 'true') {
    console.error('COMPILE RESULT:FAIL ');
    console.error('ERROR: Invalid route ' + sourcePath +
      '. Verify the route infomation' + (configPath ?  " in the " + configPath : '') +
      ', and then restart the Previewer.');
    return;
  } else {
    throw Error(
      '\u001b[31m' + 'ERROR: Invalid route ' + sourcePath +
      '. Verify the route infomation' + (configPath ?  " in the " + configPath : '') +
      ', and then restart the Build.').message;
  }  
}

let abilityEntryObj = {};
function addAbilityEntryObj(moduleJson) {
  abilityEntryObj = {};
  const entranceFiles = readAbilityEntrance(moduleJson);
  entranceFiles.forEach(filePath => {
    const key = filePath.replace(/^\.\/js\//, './').replace(/\.js$/, '');
    abilityEntryObj[key] = path.resolve(process.env.projectPath, '../', filePath);
  });
  return abilityEntryObj;
}

function readAbilityEntrance(moduleJson) {
  const entranceFiles = [];
  if (moduleJson.module) {
    if (moduleJson.module.srcEntrance) {
      entranceFiles.push(moduleJson.module.srcEntrance);
    }
    if (moduleJson.module.abilities && moduleJson.module.abilities.length) {
      readEntrances(moduleJson.module.abilities, entranceFiles);
    }
    if (moduleJson.module.extensionAbilities && moduleJson.module.extensionAbilities.length) {
      readEntrances(moduleJson.module.extensionAbilities, entranceFiles);
    }
  }
  return entranceFiles;
}

function readEntrances(abilities, entranceFiles) {
  abilities.forEach(ability => {
    if ((!ability.type || ability.type !== 'form') && ability.srcEntrance) {
      entranceFiles.push(ability.srcEntrance);
    }
  });
}

function readManifest(manifestFilePath) {
  let manifest = {};
  try {
    if (fs.existsSync(manifestFilePath)) {
      configPath = manifestFilePath;
      const jsonString = fs.readFileSync(manifestFilePath).toString();
      manifest = JSON.parse(jsonString);
    } else if (process.env.aceModuleJsonPath && fs.existsSync(process.env.aceModuleJsonPath)) {
      buildManifest(manifest);
   } else {
    throw Error('\u001b[31m' + 'ERROR: the manifest.json or module.json is lost.' +
      '\u001b[39m').message;
   }
   process.env.minPlatformVersion = manifest.minPlatformVersion;
  } catch (e) {
    throw Error('\u001b[31m' + 'ERROR: the manifest.json or module.json file format is invalid.' +
      '\u001b[39m').message;
  }
  return manifest;
}

function readModulePages(moduleJson) {
  if (moduleJson.module.pages) {
    const modulePagePath = path.resolve(process.env.aceProfilePath,
      `${moduleJson.module.pages.replace(/\$profile\:/, '')}.json`);
    if (fs.existsSync(modulePagePath)) {
      configPath = modulePagePath;
      const pagesConfig = JSON.parse(fs.readFileSync(modulePagePath, 'utf-8'));
      return pagesConfig.src;
    }
  }
}

function readFormPages(moduleJson) {
  const pages = [];
  if (moduleJson.module.extensionAbilities && moduleJson.module.extensionAbilities.length) {
    moduleJson.module.extensionAbilities.forEach(extensionAbility => {
      if (extensionAbility.type && extensionAbility.type === 'form' && extensionAbility.metadata &&
        extensionAbility.metadata.length) {
        extensionAbility.metadata.forEach(item => {
          if (item.resource && /\$profile\:/.test(item.resource)) {
            parseFormConfig(item.resource, pages);
          }
        });
      }
    });
  }
  return pages;
}

function parseFormConfig(resource, pages) {
  const resourceFile = path.resolve(process.env.aceProfilePath,
    `${resource.replace(/\$profile\:/, '')}.json`);
  if (fs.existsSync(resourceFile)) {
    configPath = resourceFile;
    const pagesConfig = JSON.parse(fs.readFileSync(resourceFile, 'utf-8'));
    if (pagesConfig.forms && pagesConfig.forms.length) {
      pagesConfig.forms.forEach(form => {
        if (form.src && (form.uiSyntax === 'hml' || !form.uiSyntax)) {
          pages.push(form.src);
        }
      });
    }
  }
}

function buildManifest(manifest) {
  try {
    const moduleJson =  JSON.parse(fs.readFileSync(process.env.aceModuleJsonPath).toString());
    if (process.env.DEVICE_LEVEL === 'rich') {
      manifest.pages = readModulePages(moduleJson);
      manifest.abilityEntryObj = addAbilityEntryObj(moduleJson);
    }
    if (process.env.DEVICE_LEVEL === 'card') {
      manifest.pages = readFormPages(moduleJson);
    }
    manifest.minPlatformVersion = moduleJson.app.minAPIVersion;
  } catch (e) {
    throw Error("\x1B[31m" + 'ERROR: the module.json file is lost or format is invalid.' +
      "\x1B[39m").message;
  }
}

function loadWorker(entryObj) {
  if (workerFile) {
    entryObj = Object.assign(entryObj, workerFile);
  } else {
    const workerPath = path.resolve(input, 'workers');
    if (fs.existsSync(workerPath)) {
      const workerFiles = [];
      readFile(workerPath, workerFiles);
      workerFiles.forEach((item) => {
        if (/\.js$/.test(item)) {
          const relativePath = path.relative(workerPath, item)
            .replace(/\.js$/, '').replace(/\\/g, '/');
          entryObj[`./workers/` + relativePath] = item;
        }
      });
    }
  }
}

function validateWorkOption() {
  if (process.env.aceBuildJson && fs.existsSync(process.env.aceBuildJson)) {
    const workerConfig = JSON.parse(fs.readFileSync(process.env.aceBuildJson).toString());
    if(workerConfig.workers) {
      return true;
    }
  }
  return false;
}

function filterWorker(workerPath) {
  return /\.(ts|js)$/.test(workerPath);
}

function readFile(dir, utFiles) {
  try {
    const files = fs.readdirSync(dir);
    files.forEach((element) => {
      const filePath = path.join(dir, element);
      const status = fs.statSync(filePath);
      if (status.isDirectory()) {
        readFile(filePath, utFiles);
      } else {
        utFiles.push(filePath);
      }
    });
  } catch (e) {
    console.error(e.message);
  }
}

function themeFileBuild(customThemePath, customThemeBuild) {
  if (fs.existsSync(customThemePath)) {
    const themeContent = JSON.parse(fs.readFileSync(customThemePath));
    const newContent = {};
    if (themeContent && themeContent['style']) {
      newContent['style'] = {};
      const styleContent = themeContent['style'];
      Object.keys(styleContent).forEach(function(key) {
        const customKey = CUSTOM_THEME_PROP_GROUPS[key];
        const ohosKey = OHOS_THEME_PROP_GROUPS[key];
        if (ohosKey) {
          newContent['style'][ohosKey] = styleContent[key];
        } else if (customKey) {
          newContent['style'][customKey] = styleContent[key];
        } else {
          newContent['style'][key] = styleContent[key];
        }
      });
      fs.writeFileSync(customThemeBuild, JSON.stringify(newContent, null, 2));
      return true;
    }
  }
  return false;
}

function readCSSInfo(watchCSSFiles) {
  if (fs.existsSync(watchCSSFiles)) {
    return JSON.parse(fs.readFileSync(watchCSSFiles));
  } else {
    return {};
  }
}

function writeCSSInfo(filePath, infoObject) {
  if (!(process.env.tddMode === 'true') && !(fs.existsSync(path.resolve(filePath, '..')) &&
    fs.statSync(path.resolve(filePath, '..')).isDirectory())) {
    mkDir(path.resolve(filePath, '..'));
  }
  if (fs.existsSync(filePath)) {
    fs.unlinkSync(filePath);
  }
  if (fs.existsSync(path.resolve(filePath, '..')) && fs.statSync(path.resolve(filePath, '..')).isDirectory()) {
    fs.writeFileSync(filePath, JSON.stringify(infoObject, null, 2));
  }
}

function setCSSEntry(cssfiles, key) {
  cssfiles["entry"] = cssfiles["entry"] || {};
  if (fs.existsSync(path.join(input, key + '.css'))) {
    cssfiles["entry"][path.join(input, key + '.css')] = true;
  }
}

function checkTestRunner(projectPath, entryObj) {
  const files = [];
  walkSync(projectPath, files, entryObj, projectPath);
}

function walkSync(filePath_, files, entryObj, projectPath) {
  fs.readdirSync(filePath_).forEach(function (name) {
    const filePath = path.join(filePath_, name);
    const stat = fs.statSync(filePath);
    if (stat.isFile()) {
      const extName = '.js';
      if (path.extname(filePath) === extName) {
        const key = filePath.replace(projectPath, '').replace(extName, '');
        entryObj[`./${key}`] = filePath;
      } else if (stat.isDirectory()) {
        walkSync(filePath, files, entryObj, projectPath);
      }
    }
  })
}

module.exports = ResourcePlugin;
