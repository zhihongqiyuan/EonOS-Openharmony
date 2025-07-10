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

var path = require('path')
var fs = require('fs')

var ResourcePlugin = require('./lib/resource-plugin')
var ResultStates = require('./lib/compile-plugin')
var GenBinPlugin = require('./lib/genBin-plugin')
var GenAbcPlugin = require('./lib/genAbc-plugin').GenAbcPlugin
var AfterEmitPlugin = require('./lib/cardJson-plugin').AfterEmitPlugin
const ReadJsonPlugin = require('./lib/read-json-plugin')

const { PLATFORM }= require('./lib/lite/lite-enum')
const util = require('./lib/util')
const TerserPlugin = require('terser-webpack-plugin')
const CopyPlugin = require("copy-webpack-plugin")
const webpack = require('webpack')
let watchMode = (process.env.watchMode && process.env.watchMode === 'true') || false
const {
  deleteFolderRecursive,
  readManifest,
  loadEntryObj,
  hashProjectPath,
  checkMultiResourceBuild,
  readWorkerFile,
  compareCache,
  parseAbilityName
} = require('./main.product')

const richModule = {
  rules: [
    {
      test: /\.visual$/,
      use: [{
        loader: path.resolve(__dirname, './lib/loader-gen.js')
      }]
    },
    {
      test: /(\.hml)(\?[^?]+)?$/,
      use: [{
        loader: path.resolve(__dirname, './index.js')
      }]
    },
    {
      test: /\.png$/,
      use: [{
        loader: 'file-loader',
        options: {
          name: '[name].[ext]',
          outputPath: 'common'
        }
      }]
    },
    {
      test: /\.css$/,
      use: [{
        loader: 'css-loader'
      }]
    },
    {
      test: /\.less$/,
      use: [{
        loader: 'less-loader'
      }]
    },
    {
      test: /\.(scss|sass)$/,
      use: [{
        loader: 'style-loader!css-loader!sass-loader'
      }]
    },
    {
      test: /\.jsx?$/,
      type: 'javascript/auto',
      use: [
        {
          loader: path.resolve(__dirname, 'lib/module-script.js')
        },
        {
          loader: util.loadBabelModule('babel-loader'),
          options: {
            presets: [util.loadBabelModule('@babel/preset-env')],
            targets: 'node 8',
            plugins: [
              [
                util.loadBabelModule('@babel/plugin-transform-modules-commonjs'),
                {'allowTopLevelThis': true}
              ],
              util.loadBabelModule('@babel/plugin-proposal-class-properties')
            ],
            compact: false
          }
        }
      ]
    }
  ]
}

const cardModule = {
  rules: [
    {
      test: /\.visual$/,
      use: [{
        loader: path.resolve(__dirname, './lib/loader-gen.js')
      }]
    },
    {
      test: /\.hml$/,
      use: [{
        loader: path.resolve(__dirname, './lib/card-loader.js')
      }]
    },
    {
      test: /\.css$/,
      use: [{
        loader: 'css-loader'
      }]
    },
    {
      test: /\.less$/,
      use: [{
        loader: 'less-loader'
      }]
    },
    {
      test: /\.(scss|sass)$/,
      use: [{
        loader: 'css-loader!sass-loader'
      }]
    },
    {
      test: /\.jsx?$/,
      type: 'javascript/auto',
      use: [
        {
          loader: path.resolve(__dirname, 'lib/module-script.js')
        },
        {
          loader: util.loadBabelModule('babel-loader'),
          options: {
            presets: [util.loadBabelModule('@babel/preset-env')],
            targets: 'node 8',
            plugins: [util.loadBabelModule('@babel/plugin-transform-modules-commonjs'),
            util.loadBabelModule('@babel/plugin-proposal-class-properties')],
            compact: false
          }
        }
      ]
    }
  ]
}

let config = {
  cache: {
    type: 'filesystem'
  },
  watch: watchMode,
  watchOptions: {
    aggregateTimeout: 10,
    poll: false,
    ignored: ["**/node_modules", "**/oh_modules", "**/*.json~"]
  },
  optimization: {
    moduleIds: 'deterministic',
    chunkIds: 'deterministic',
  },
  output: {
    filename: '[name].js',
    pathinfo: false,
    devtoolModuleFilenameTemplate: (info) => {
      const newInfo = info.absoluteResourcePath.replace(process.env.projectRootPath + path.sep, '')
        .replace(process.env.projectRootPath + path.sep, '')
        .replace(path.join(__dirname, path.sep), '');
      return newInfo;
    },
    globalObject: 'globalThis'
  },
  devtool: 'nosources-source-map',
  mode: 'development',
  module: richModule,
  node: {
    global: false
  },
  stats: 'none'
}

function setConfigs(env) {
  if (process.env.aceModuleJsonPath || env.aceModuleJsonPath) {
    process.env.compileMode = 'moduleJson';
  }
  process.env.error = env.error === undefined ? true : env.error
  process.env.warning = env.warning === undefined ? true : env.warning
  process.env.note = env.note === undefined ? true : env.note
  process.env.buildMode = env.buildMode || 'debug'
  process.env.logLevel = env.logLevel || '1'
  process.env.isPreview = env.isPreview || false
  process.env.projectPath = env.aceModuleRoot || process.env.aceModuleRoot || process.cwd();
  hashProjectPath(process.env.projectPath);
  process.env.buildPath = env.aceModuleBuild || process.env.aceModuleBuild ||
    path.resolve(process.env.projectPath, 'build');
  process.env.cachePath = env.cachePath || process.env.cachePath || path.resolve(__dirname, 'node_modules/.cache');
  process.env.aceManifestPath = process.env.aceManifestPath || path.resolve(process.env.projectPath, 'manifest.json');
  process.env.abilityType = process.env.abilityType || 'page';
  process.env.DEVICE_LEVEL = env.DEVICE_LEVEL || process.env.DEVICE_LEVEL || 'rich';
  process.env.aceModuleJsonPath = env.aceModuleJsonPath || process.env.aceModuleJsonPath;
  process.env.aceProfilePath = env.aceProfilePath || process.env.aceProfilePath;
  process.env.watchCSSFiles = process.env.watchCSSFiles || path.resolve(process.env.cachePath, '.rich_cache', 'preview_css.json');
  watchMode = (process.env.watchMode && process.env.watchMode === 'true') ||
    (env.watchMode && env.watchMode === 'true') || false;
  if (process.env.abilityType === 'page' || process.env.abilityType === 'form') {
    const manifest = readManifest(process.env.aceManifestPath)
    if (process.env.compileMode !== 'moduleJson') {
      process.env.DEVICE_LEVEL = manifest.type === 'form' ? 'card' : 'rich'
    }
    process.env.PLATFORM_VERSION = PLATFORM.VERSION6;
    const version = parseInt(manifest.minPlatformVersion);
    if (version == 5) {
      process.env.PLATFORM_VERSION = PLATFORM.VERSION5;
    }
    if (version <= 4) {
      process.env.PLATFORM_VERSION = PLATFORM.VERSION3;
    }
  }
  process.env.aceBuildJson = env.aceBuildJson || process.env.aceBuildJson;
  checkMultiResourceBuild(process.env.aceBuildJson);
}

function setArkPlugin(env, workerFile) {
  if (env.isPreview === "true" || env.compilerType && env.compilerType === 'ark') {
    let arkDir = path.join(__dirname, 'bin', 'ark');
    if (env.arkFrontendDir) {
      arkDir = env.arkFrontendDir;
    }
    let nodeJs = 'node';
    if (env.nodeJs) {
      nodeJs = env.nodeJs;
    }
    config.plugins.push(new GenAbcPlugin(process.env.buildPath, arkDir, nodeJs, workerFile,
      env.buildMode === 'debug'))
    if (env.buildMode === 'release') {
      config.output.path = path.join(process.env.cachePath, "releaseAssets",
        path.basename(process.env.buildPath));
      process.env.configOutput = config.output.path;
    }
  } else {
    if (env.deviceType) {
      let deviceArr = env.deviceType.split(/,/)
      let isDefault = deviceArr.indexOf('tv') >= 0 || deviceArr.indexOf('wearable') >= 0 ? true : false
      if (isDefault) {
        config.plugins.push(new GenBinPlugin(process.env.buildPath, path.join(__dirname, 'bin', workerFile)))
      }
    }
  }
}

function existsPackageJson(config, rootPackageJsonPath, modulePackageJsonPath) {
  if (config.cache) {
    config.cache.buildDependencies = {
      config: []
    };
    if (fs.existsSync(rootPackageJsonPath)) {
      config.cache.buildDependencies.config.push(rootPackageJsonPath);
    }
    if (fs.existsSync(modulePackageJsonPath)) {
      config.cache.buildDependencies.config.push(modulePackageJsonPath);
    }
  }
}

function excludeWorker(workerFile, name) {
  if (workerFile) {
    return Object.keys(workerFile).includes(name);
  }
  return /^\.\/workers\//.test(name);
}

module.exports = (env) => {
  setConfigs(env);
  compareCache(path.resolve(process.env.cachePath, '.rich_cache'));
  const workerFile = readWorkerFile();
  if (process.env.compileMode === 'moduleJson') {
    process.env.DEVICE_LEVEL = 'card';
    config.entry = {};
  } else {
    deleteFolderRecursive(process.env.buildPath);
    config.entry = loadEntryObj(process.env.projectPath, process.env.DEVICE_LEVEL,
      process.env.abilityType, process.env.aceManifestPath);
    existsPackageJson(config, path.resolve(process.env.projectPath, '../../../../../package.json'),
      path.resolve(process.env.projectPath, '../../../../package.json'));
  }
  config.cache.cacheDirectory = path.resolve(process.env.cachePath, '.rich_cache',
    path.basename(process.env.projectPath));
  config.output.path = path.resolve(__dirname, process.env.buildPath)
  config.plugins = [
    new ResourcePlugin(process.env.projectPath, process.env.buildPath,
      process.env.aceManifestPath, process.env.watchCSSFiles, workerFile),
    new ResultStates({
      build: process.env.buildPath
    }),
    new webpack.DefinePlugin({
      STANDARD: JSON.stringify(true),
      LITE: JSON.stringify(false)
    })
  ]
  config.resolve = {
    modules: [
      process.env.projectPath,
      path.join(process.env.projectPath, '../../../../../'),
      path.join(__dirname, 'node_modules'),
      './node_modules',
      './oh_modules'
    ],
    descriptionFiles: ['package.json', 'oh-package.json5'],
    plugins: [new ReadJsonPlugin()],
  }
  if (fs.existsSync(path.resolve(process.env.projectPath, 'i18n'))) {
    config.plugins.push(new CopyPlugin({
      patterns: [
        {
          from: path.resolve(process.env.projectPath, 'i18n'),
          to: path.resolve(process.env.buildPath, 'i18n'),
          noErrorOnMissing: true
        }
      ]
    }))
  }
  if (process.env.aceConfigPath && fs.existsSync(process.env.aceConfigPath)) {
    config.plugins.push(new CopyPlugin({
      patterns: [
        {
          from: path.resolve(process.env.aceConfigPath),
          to: path.resolve(process.env.buildPath, 'config.json'),
          noErrorOnMissing: true
        }
      ]
    }))
  }
  if (process.env.DEVICE_LEVEL === 'card') {
    config.module = cardModule
    config.plugins.push(new AfterEmitPlugin())
    config.optimization = {};
    setArkPlugin(env, workerFile);
  } else {
    if (process.env.compileMode !== 'moduleJson' && process.env.abilityType === 'page') {
      Object.assign(config.optimization, {
        splitChunks: {
          chunks(chunk) {
            return !excludeWorker(workerFile, chunk.name) && !/^\.\/TestAbility/.test(chunk.name);
          },
          minSize: 0,
          cacheGroups: {
            vendors: {
              test: /[\\/](node|oh)_modules[\\/]/,
              priority: 20,
              name: "vendors",
            },
            commons: {
              test: /\.js|css|hml$/,
              name: 'commons',
              priority: 10,
              minChunks: 2,
            }
          }
        },
      });
    }
    setArkPlugin(env, workerFile);
    if (env.sourceMap === 'none') {
      config.devtool = false
    }
    if (env.buildMode === 'release') {
      config.mode = 'production';
      Object.assign(config.optimization, {
        minimize: true,
        minimizer: [new TerserPlugin({
          terserOptions: {
            compress: {
              defaults: false,
              dead_code: true,
              collapse_vars: true,
              unused: true,
              drop_debugger: true,
              if_return: true,
              reduce_vars: true,
              join_vars: false,
              sequences: 0,
            },
            format: {
              semicolons: false,
              beautify: true,
              braces: true,
              indent_level: 2,
            },
          },
        })],
      });
      config.output.devtoolModuleFilenameTemplate = (info) => {
        const newInfo = info.absoluteResourcePath.replace(process.env.projectRootPath + path.sep, '')
          .replace(process.env.projectRootPath + path.sep, '')
          .replace(path.join(__dirname, path.sep), '');
        return newInfo;
      }
      config.output.sourceMapFilename = '_releaseMap/[name].js.map'
    }
  }
  if (process.env.abilityType === 'testrunner') {
    config.module.rules = [];
    config.module.rules.unshift({
      test: /TestRunner/,
      use: [{
        loader: path.resolve(__dirname, './index.js')
      }]
    })
  } else {
    config.module.rules.unshift({
      test: parseAbilityName(process.env.abilityType, process.env.projectPath),
      use: [{
        loader: path.resolve(__dirname, './index.js')
      }]
    })
  }

  config.output.library = process.env.hashProjectPath;
  return config
}
