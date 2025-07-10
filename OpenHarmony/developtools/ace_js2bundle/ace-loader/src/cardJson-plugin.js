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

const path = require('path')
const { Compilation } = require('webpack')

class AfterEmitPlugin {
  constructor() {
  }

  apply(compiler) {
    compiler.hooks.thisCompilation.tap('card', (compilation) => {
      compilation.hooks.processAssets.tapAsync(
        {
          name: 'MyPlugin',
          stage: Compilation.PROCESS_ASSETS_STAGE_REPOR,
        },
        (assets, back) => {
          const keys = Object.keys(assets)
          keys.forEach(key => {
            if ('./' + process.env.abilityType + '.js' !== key) {
              sourceChange(key, assets, compilation);
            }
          });
          back && back();
        }
      );
    })
  }
}

function sourceChange(key, assets, compilation) {
  try {
    const extName = path.extname(key);
    if (extName === '.js') {
      const jsonOut = {};
      const source = assets[key].source();
      const str = source.match(/card_start((\s||\S)*)card_end/)[1];
      const array = str.split('\n');
      array.forEach(element => {
        elementChange(element, source, jsonOut);
      });
      const assetReplace = {};
      Object.keys(jsonOut).forEach(function (jsonOutKey) {
        toAddJson(assetReplace, jsonOutKey, JSON.parse(jsonOut[jsonOutKey]), compilation, path.join(process.env.projectPath, key));
      });
      assets[key.replace(extName, '') + '.json'] = {
        source: function () {
          return JSON.stringify(assetReplace, null, 2);
        },
        size: function () {
          return JSON.stringify(assetReplace, null, 2).size;
        }
      }
      delete assets[key];
    }
  } catch (e) {
    compilation.errors.push({ message: 'errorStartERROR File：' + key + 
      '\n' + ` ${e.message}` + 'errorEnd' });
  }
}

function elementChange(element, source, jsonOut) {
  if (element.trim() === '' || element.trim() === '//') {
  } else {
    const key = element.match(/var ((\s||\S)*) =/)[1];
    const value = element.match(/"((\s||\S)*)"/)[1];
    const replaceSource = source.replace(element, '').toString();
    const partStart = replaceSource.indexOf(value);
    const subSource = replaceSource.substr(partStart);
    const partEnd = subSource.indexOf('/***/ })');
    let out = subSource.substr(0, partEnd).match(/module\.exports \= ((\s||\S)*)/)[1].trim();
    if (out.indexOf('JSON.parse(') === 0) {
      out = JSON.stringify(eval(out));
    }
    if (out.substr(out.length - 1, 1) === ';') {
      out = out.substr(0, out.length - 1);
    }
    jsonOut[key] = out;
  }
}

function toAddJson(assetReplace, key, value, compilation, sourceKey) {
  assetReplace['template'] = assetReplace['template'] || {};
  assetReplace['styles'] = assetReplace['styles'] || {};
  assetReplace['data'] = assetReplace['data'] || {};
  assetReplace['actions'] = assetReplace['actions'] || {};
  assetReplace['apiVersion'] = assetReplace['apiVersion'] || {};
  
  switch(key) {
    case 'card_template':
      assetReplace['template'] = value;
      break;
    case 'card_style':
      assetReplace['styles'] = value;
      break;
    case 'card_json':
      if (value) {
        if (value.data) {
          assetReplace['data'] = validateData(value.data, compilation, sourceKey);
        }
        if (value.actions) {
          assetReplace['actions'] = processActions(value.actions, compilation, sourceKey);
        }
        if (value.apiVersion) {
          assetReplace['apiVersion'] = validateData(value.apiVersion, compilation, sourceKey);
        }
        if (value.props) {
          assetReplace['props'] = replacePropsArray(value.propsValue, compilation, sourceKey);
        }
      }
      break;
    default:
      addElement(assetReplace, key, value, compilation, sourceKey);
      break;
  }
}

function addElement(assetReplace, key, value, compilation, sourceKey) {
  const keyName = key.substr(key.lastIndexOf('_') + 1, key.length - key.lastIndexOf('_') + 1);
  assetReplace[keyName] = assetReplace[keyName] || {};
  assetReplace[keyName]['template'] = assetReplace[keyName]['template'] || {};
  assetReplace[keyName]['styles'] = assetReplace[keyName]['styles'] || {};
  assetReplace[keyName]['data'] = assetReplace[keyName]['data'] || {};
  assetReplace[keyName]['actions'] = assetReplace[keyName]['actions'] || {};
  assetReplace[keyName]['apiVersion'] = assetReplace[keyName]['apiVersion'] || {};

  switch(key.replace(keyName, '')) {
    case 'card_element_template_':
      assetReplace[keyName]['template'] = value;
      break;
    case 'card_element_style_':
      assetReplace[keyName]['styles'] = value;
      break;
    case 'card_element_json_':
      if (value) {
        if (value.data) {
          assetReplace[keyName]['data'] = validateData(value.data, compilation, sourceKey);
        }
        if (value.actions) {
          assetReplace[keyName]['actions'] = processActions(value.actions, compilation, sourceKey);
        }
        if (value.apiVersion) {
          assetReplace[keyName]['apiVersion'] = validateData(value.apiVersion, compilation, sourceKey);
        }
        if (value.props) {
          assetReplace[keyName]['props'] = replacePropsArray(value.propsValue, compilation, sourceKey);
        }
      }
      break;
    default:
      break;
  }
}

function replacePropsArray(propsValue, _this, key) {
  if (!propsValue) {
    return propsValue;
  }
  if (Array.isArray(propsValue)) {
    const propsObject = {};
    propsValue.forEach(item => {
      if (typeof(item) !== 'string') {
        _this.warnings.push({message: 'warnStartWARNING File：' + key +
          '\n' + `The props value type should be 'string', not '${typeof(item)}' in props array in custom elements.` + 'warnEnd'});
      }
      propsObject[item] = { 'default': '' };
    });
    propsValue = propsObject;
  } else if (Object.prototype.toString.call(propsValue) === '[object Object]') {
    Object.keys(propsValue).forEach(item => {
      if (Object.prototype.toString.call(propsValue[item]) !== '[object Object]') {
        _this.warnings.push({message: 'warnStartWARNING File：' + key +
          '\n' + 'The props default value type can only be Object in custom elements.' + 'warnEnd'});
      }
      if (!propsValue[item].hasOwnProperty('default')) {
        propsValue[item] = { 'default': '' }
      }
    });
  } else {
    _this.warnings.push({message: 'warnStartWARNING File：' + key +
      '\n' + 'The props type can only be Array or Object in custom elements.' + 'warnEnd'});
  }
  return propsValue;
}

function processActions(actionsValue, _this, key) {
  if (Object.prototype.toString.call(actionsValue) === '[object Object]') {
    Object.keys(actionsValue).forEach(item => {
      if (actionsValue[item].method) {
        if (typeof(actionsValue[item].method) === 'string') {
          if (actionsValue[item].method.toLowerCase() !== actionsValue[item].method) {
            _this.warnings.push({message: 'warnStartWARNING File：' + key +
              '\n' + `WARNING: The key method '${actionsValue[item].method}' in the actions don't support uppercase letters.` + 'warnEnd'});
            actionsValue[item].method = actionsValue[item].method.toLowerCase();
          }
        } else {
          _this.warnings.push({message: 'warnStartWARNING File：' + key +
            '\n' + `WARNING: The key method type in the actions should be 'string', not '${typeof(actionsValue[item].method)}'.` + 'warnEnd'});
        }
      }
    })
  } else {
    if (actionsValue) {
      _this.warnings.push({message: 'warnStartWARNING File：' + key +
        '\n' + 'WARNING: The actions value type can only be Object.' + 'warnEnd'});
    }
  }
  return actionsValue;
}

function validateData(dataValue, _this, key) {
  if (dataValue && Object.prototype.toString.call(dataValue) !== '[object Object]') {
    _this.warnings.push({message: 'warnStartWARNING File：' + key +
      '\n' + 'WARNING: The data value type can only be Object.' + 'warnEnd'});
  }
  return dataValue;
}

module.exports = {
  AfterEmitPlugin: AfterEmitPlugin
}