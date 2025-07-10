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

const fs = require('fs')
const path = require('path')
const process = require('child_process')
const qjsc = path.join(__dirname, '..', 'bin', 'qjsc')
const checkWorksFile = require('./genAbc-plugin').checkWorksFile

const forward = '(global.___mainEntry___ = function (globalObjects) {' + '\n' +
              '  const define = globalObjects.define;' + '\n' +
              '  const require = globalObjects.require;' + '\n' +
              '  const bootstrap = globalObjects.bootstrap;' + '\n' +
              '  const register = globalObjects.register;' + '\n' +
              '  const render = globalObjects.render;' + '\n' +
              '  const $app_define$ = globalObjects.$app_define$;' + '\n' +
              '  const $app_bootstrap$ = globalObjects.$app_bootstrap$;' + '\n' +
              '  const $app_require$ = globalObjects.$app_require$;' + '\n' +
              '  const history = globalObjects.history;' + '\n' +
              '  const Image = globalObjects.Image;' + '\n' +
              '  const OffscreenCanvas = globalObjects.OffscreenCanvas;' + '\n' +
              '  (function(global) {' + '\n' +
              '    "use strict";' + '\n'
const last = '\n' + '})(this.__appProto__);' + '\n' + '})'
const firstFileEXT = '.jtc'
const sencondFileEXT = '.c'
const lastFileEXT = '.bin'
let output
let webpackPath
let workerFile = null;

class GenBinPlugin {
  constructor(output_, webpackPath_, workerFile_) {
    output = output_
    webpackPath = webpackPath_
    workerFile = workerFile_
  }
  apply(compiler) {
    if (!fs.existsSync(path.resolve(webpackPath, 'qjsc.exe')) && !fs.existsSync(path.resolve(webpackPath, 'qjsc'))) {
      return
    }
    compiler.hooks.emit.tap('GenBinPlugin', (compilation) => {
      const assets = compilation.assets
      const keys = Object.keys(assets)
      keys.forEach(key => {
        // choice *.js
        if (output && webpackPath && path.extname(key) === '.js') {
          let newContent = assets[key].source()
          if (checkWorksFile(key, workerFile)) {
            newContent = forward + newContent + last
          }
          const keyPath = key.replace(/\.js$/, firstFileEXT)
          writeFileSync(newContent, path.resolve(output, keyPath), key)
        }
      })
    })
  }
}

function writeFileSync(inputString, output, jsBundleFile) {
  const parent = path.join(output, '..')
  if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
    mkDir(parent)
  }
  fs.writeFileSync(output, inputString)
  if (fs.existsSync(output)){
    qjscFirst(output, output.replace(/\.jtc$/, sencondFileEXT))
  } else {
    console.error('\u001b[31m', `Failed to convert file ${jsBundleFile} to bin. ${output} is lost`, '\u001b[39m')
  }
}

function mkDir(path_) {
  const parent = path.join(path_, '..')
  if (!(fs.existsSync(parent) && !fs.statSync(parent).isFile())) {
    mkDir(parent)
  }
  fs.mkdirSync(path_)
}

function qjscFirst(inputPath, outputPath) {
  const cmd = `"${qjsc}" -o "${outputPath}" -N buf -c "${inputPath}"`
  try {
    process.execSync(cmd)
  } catch (e) {
    console.error('\u001b[31m', `Failed to convert file ${inputPath} to bin`, '\u001b[39m')
  }
  if (fs.existsSync(inputPath)) {
    fs.unlinkSync(inputPath)
    qjscSecond(outputPath)
  } else {
    console.error('\u001b[31m', `Failed to convert file ${inputPath} to bin. ${inputPath} is lost`, '\u001b[39m')
  }
}

function qjscSecond(filePath) {
  let data = fs.readFileSync(filePath, 'utf8')
  data = data.substr(data.indexOf('{') + 1, data.indexOf('}') - data.indexOf('{') - 1).trim()
  const lastFilePath = filePath.replace(/\.c$/, lastFileEXT)
  const parent = path.join(lastFilePath, '..')
  if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
    mkDir(parent)
  }
  fs.writeFileSync(lastFilePath, toBuffer(data))
  if (fs.existsSync(filePath)) {
    fs.unlinkSync(filePath)
  } else {
    console.error('\u001b[31m', `Failed to clean file ${filePath}.`, '\u001b[39m')
  }
}

function toBuffer(str) {
  const bytes = str.split(',')
  const b = Buffer.alloc(bytes.length)
  for (let i = 0; i < bytes.length; i++) {
    b[i] = bytes[i]
  }
  return b
}

module.exports = GenBinPlugin
