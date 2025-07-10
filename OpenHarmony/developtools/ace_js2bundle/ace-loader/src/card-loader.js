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

import path from 'path'
import fs from 'fs'
import {
  getRequireString,
  jsonLoaders,
  logWarn
}
from './util'
import { parseFragment } from './parser'

function loader(source) {
  this.cacheable && this.cacheable()
  const options = {
    lang: {
      sass:['sass-loader'],
      scss:['sass-loader'],
      less:['less-loader']
    }
  }
  const customLang = options.lang || {}
  const resourcePath = this.resourcePath
  const fileName = resourcePath.replace(path.extname(resourcePath).toString(), '')
  let output = '//card_start\n'
  output += 'var card_template =' + getRequireString(this, jsonLoaders('template'), resourcePath)
  const styleInfo = findStyleFile(fileName)
  if (styleInfo.extStyle == true) {
    output += 'var card_style =' +
      getRequireString(this, jsonLoaders('style', customLang[styleInfo.type]), styleInfo.styleFileName)
  }
  output = addJson(this, output, fileName, '')

  const frag = parseFragment(source)
  const nameSet = new Set()
  if (frag.element) {
    frag.element.forEach(item => {
      let customElementName
      if (!item.src) {
        logWarn(this, [{
          reason: `ERROR: The attribute 'src' must be set in the custom element.`,
          line: item.node.__location.line,
          column: item.node.__location.col
        }])
        return
      }
      if (!item.src.match(/\.hml$/)) {
        item.src = item.src.concat('.hml')
      }
      const compResourcepath = path.join(resourcePath, '..', item.src)
      if (!fs.existsSync(compResourcepath)) {
        logWarn(this, [{
          reason: `ERROR: The custom element '${compResourcepath}' can not be found.`,
          line: item.node.__location.line,
          column: item.node.__location.col
        }])
        return
      }
      if (!item.name) {
        customElementName = path.parse(item.src).name.toLowerCase()
      } else {
        customElementName = item.name.toLowerCase()
      }
      if (nameSet.has(customElementName)) {
        logWarn(this, [{
          reason: `ERROR: The custom elements cannot have the same attribute 'name' or file name (case insensitive).`,
          line: item.node.__location.line,
          column: item.node.__location.col
        }])
        return
      } else {
        nameSet.add(customElementName)
      }
      const compFileName = compResourcepath.replace(path.extname(compResourcepath).toString(), '')
      const elementLastName = path.basename(compResourcepath).replace(path.extname(compResourcepath).toString(), '')
      output += `var card_element_template_${elementLastName} =` + getRequireString(this, jsonLoaders('template'),
        compResourcepath + `?${customElementName}#${fileName}`)
      const compStyleInfo = findStyleFile(compFileName)
      if (compStyleInfo.extStyle == true) {
        output += `var card_element_style_${elementLastName} =` +
          getRequireString(this, jsonLoaders('style', customLang[compStyleInfo.type]),
          compStyleInfo.styleFileName + `?${customElementName}#${fileName}`)
      }
      output = addJson(this, output, compFileName, `?${customElementName}#${fileName}`, elementLastName)
    })
  }
  output = output + '\n//card_end'
  return output
}

function findStyleFile (fileName) {
  let extStyle = false
  let styleFileName = fileName + '.css'
  let type = 'css'
  if (fs.existsSync(styleFileName)) {
    extStyle = true
    type = 'css'
  } else {
    styleFileName = fileName + '.less'
    if (fs.existsSync(styleFileName)) {
      extStyle = true
      type = 'less'
    } else {
      styleFileName = fileName + '.sass'
      if (fs.existsSync(styleFileName)) {
        extStyle = true
        type = 'sass'
      } else {
        styleFileName = fileName + '.scss'
        if (fs.existsSync(styleFileName)) {
          extStyle = true
          type = 'sass'
        } else {
          extStyle = false
        }
      }
    }
  }
  return {extStyle: extStyle, styleFileName: styleFileName, type: type}
}

function addJson(_this, output, fileName, query, elementLastName) {
  const content = `${elementLastName ? 'var card_element_json_' + elementLastName : 'var card_json'} =`
  if (fs.existsSync(fileName + '.json') && !fs.existsSync(fileName + '.js')) {
    output += content + getRequireString(_this, jsonLoaders('json'), fileName + '.json' + query)
  } else if (fs.existsSync(fileName + '.js') && !fs.existsSync(fileName + '.json')) {
    logWarn(_this, [{
      reason: `WARNING: The JS file '${fileName}.js' will be discarded in future version, ` +
        `use the JSON file '${fileName}.json' instead.`,
    }])
    output += content + getRequireString(_this, jsonLoaders('json'), fileName + '.js' + query)
  } else if (fs.existsSync(fileName + '.json') && fs.existsSync(fileName + '.js')) {
    logWarn(_this, [{
      reason: `WARNING: '${fileName}' cannot have the same name files '.json' and '.js', otherwise '.json' in default.`,
    }])
    output += content + getRequireString(_this, jsonLoaders('json'), fileName + '.json' + query)
  }
  return output
}

module.exports = loader