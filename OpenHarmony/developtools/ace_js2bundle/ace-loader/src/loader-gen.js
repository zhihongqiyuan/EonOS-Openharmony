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

import loaderUtils from 'loader-utils'
import path from 'path'

import {
  getNameByPath,
  getRequireString,
  stringifyLoaders,
  jsonLoaders,
  loadBabelModule
}
from './util'

const loaderPath = __dirname
const defaultLoaders = {
  none: '',
  main: path.resolve(loaderPath, 'loader.js'),
  template: path.resolve(loaderPath, 'template.js'),
  style: path.resolve(loaderPath, 'style.js'),
  script: path.resolve(loaderPath, 'script.js'),
  json: path.resolve(loaderPath, 'json.js'),
  babel: loadBabelModule('babel-loader'),
  manifest: path.resolve(loaderPath, 'manifest-loader.js'),
  extgen: path.resolve(loaderPath, 'extgen.js')
}

function getLoaderString (type, config) {
  config = config || {}
  const customLoader = loadCustomLoader(config)
  let loaders
  switch (type) {
    case 'main':
      return mainLoaderString(loaders)
    case 'element':
      return elementLoaderString(loaders, config)
    case 'template':
      return templateLoaderString(loaders, config, customLoader)
    case 'style':
      return styleLoaderString(loaders, config, customLoader)
    case 'script':
      return scriptLoaderString(loaders, config, customLoader)
    case 'config':
      return configLoaderString(loaders, config)
    case 'data':
      return dataLoaderString(loaders, config)
  }
}

function loadCustomLoader (config) {
  if (config.lang && config.customLang[config.lang]) {
    return loadBabelModule(config.customLang[config.lang][0])
  }
}

function mainLoaderString (loaders) {
  loaders = [{
    name: defaultLoaders.main
  }]
  return stringifyLoaders(loaders)
}

function elementLoaderString (loaders, config) {
  loaders = [{
    name: defaultLoaders.main,
    query: {
      element: config.source ? undefined : true
    }
  }]
  return stringifyLoaders(loaders)
}

function templateLoaderString (loaders, config, customLoader) {
  loaders = [{
    name: defaultLoaders.json
  }, {
    name: defaultLoaders.template
  }]
  if (customLoader) {
    loaders = loaders.concat(customLoader)
  }
  loaders.push({
    name: defaultLoaders.extgen,
    query: {
      type: 'template'
    }
  })

  return stringifyLoaders(loaders)
}

function styleLoaderString (loaders, config, customLoader) {
  loaders = [{
    name: defaultLoaders.json
  }, {
    name: defaultLoaders.style
  }]
  if (customLoader) {
    loaders = loaders.concat(customLoader)
  }
  loaders.push({
    name: defaultLoaders.extgen,
    query: {
      type: 'style'
    }
  })
  return stringifyLoaders(loaders)
}

function scriptLoaderString (loaders, config, customLoader) {
  if (process.env.DEVICE_LEVEL === 'card') {
    loaders = [{
      name: defaultLoaders.json
    }]
    if (customLoader) {
      loaders = loaders.concat(customLoader);
    }
    loaders.push({
      name: defaultLoaders.extgen,
      query: {
        type: 'json'
      }
    })
  } else {
    loaders = [{
      name: defaultLoaders.script
    }]
    if (customLoader) {
      loaders = loaders.concat(customLoader)
    } else {
      loaders.push({
        name: defaultLoaders.babel,
        query: {
          presets: [loadBabelModule('@babel/preset-env')],
          targets: 'node 8',
          plugins: [loadBabelModule('@babel/plugin-transform-modules-commonjs')],
          comments: 'false'
        }
      })
    }
  }
  return stringifyLoaders(loaders)
}

function configLoaderString (loaders) {
  loaders = [{
    name: defaultLoaders.json
  }]
  return stringifyLoaders(loaders)
}

function dataLoaderString (loaders) {
  loaders = [{
    name: defaultLoaders.json
  }]
  return stringifyLoaders(loaders)
}

function codegenHmlAndCss() {
  const options = {
    lang: {
      sass:['sass-loader'],
      scss:['sass-loader'],
      less:['less-loader']
    }
  }
  const customLang = options.lang || {}
  const loaderQuery = loaderUtils.getOptions(this) || {}
  const isElement = loaderQuery.element
  const resourceQuery = this.resourceQuery && loaderUtils.parseQuery(this.resourceQuery) || {}
  const isEntry = resourceQuery.entry
  let output = ''
  let jsFileName = this.resourcePath.replace(process.env.aceSuperVisualPath, process.env.projectPath)
  jsFileName = jsFileName.substr(0, jsFileName.length - 6) + 'js';

  if (process.env.DEVICE_LEVEL === 'card') {
    output = '//card_start\n'
    output += `var card_template =` + getRequireString(this,
      jsonLoaders('template', undefined, true, 'template'), this.resourcePath)
    output += `var card_style =` + getRequireString(this,
      jsonLoaders('style', undefined, true, 'style'), this.resourcePath)
    output += `var card_json =` + getRequireString(this,
      jsonLoaders('json', undefined, true, 'json'), this.resourcePath)
    output += '\n//card_end'
  } else {
    const that = this
    output = 'var $app_script$ = ' + generateOutput(that, 'script', jsFileName, isElement)
    output += 'var $app_template$ = ' + generateOutput(that, 'template', jsFileName, isElement)
    output += 'var $app_style$ = ' + generateOutput(that, 'style', jsFileName, isElement)
    output += generateOutput(that, 'others', jsFileName, isElement)
    if (isEntry) {
      output += `$app_bootstrap$('@app-component/${getNameByPath(this.resourcePath)}'`
        + ',undefined' + ',undefined' + `)`
    }
  }
  return output
}

function generateOutput(that, type, jsFileName, isElement, customLang) {
  let result
  switch (type) {
    case 'script':
      result = getRequireString(that, getLoaderString('script', {
        customLang,
        lang: undefined,
        element: undefined,
        elementName: undefined,
        source: jsFileName
      }), jsFileName)
      break
    case 'template':
      result = getRequireString(that, getLoaderString('template', {
        customLang,
        lang: undefined,
        element: isElement,
        elementName: undefined,
        source: that.resourcePath
      }), that.resourcePath)
      break
    case 'style':
      result = getRequireString(that, getLoaderString('style', {
        customLang,
        lang: undefined,
        element: isElement,
        elementName: undefined,
        source: that.resourcePath
      }), that.resourcePath)
      break
    case 'others':
      result = `
      $app_define$('@app-component/${getNameByPath(that.resourcePath)}', [],
        function($app_require$, $app_exports$, $app_module$) {
      ` + `
      $app_script$($app_module$, $app_exports$, $app_require$)
      if ($app_exports$.__esModule && $app_exports$.default) {
      $app_module$.exports = $app_exports$.default
      }
      ` + `
      $app_module$.exports.template = $app_template$
      ` + `
      $app_module$.exports.style = $app_style$
      ` + `
      })
      `
      break
  }
  return result
}

module.exports = codegenHmlAndCss