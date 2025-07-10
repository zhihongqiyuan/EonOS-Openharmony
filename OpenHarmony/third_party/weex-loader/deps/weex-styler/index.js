'use strict'

var css = require('css')
var util = require('./lib/util')
var validateItem = require('./lib/validator').validate
var fs = require('fs')
var path = require('path')
var lodash = require('lodash')

var SELECTOR_MATCHER = /^[\.#]?[A-Za-z0-9_\-:]+$/
var DESCENDANT_SELECTOR_MATCHER = /^([.#]?[A-Za-z0-9_-]+(\s+|\s*>\s*))+([.#]?[A-Za-z0-9_\-:]+)$/
var IMPORT_MATCHER = /(['"]([^()]+?)['"])|(['"]([^()]+?)['"]\s+(only|not)?\s?(screen)?\s?((and|or|,|not|landscape)?\s?[(]([^()])+[)]\s*)+)/g
var LENGTH_REGEXP = /^[-+]?\d*\.?\d+(\S*)$/
const CARD_SELECTOR = /^[\.#][A-Za-z0-9_\-]+$/
const card = process.env.DEVICE_LEVEL === 'card'
var ALL_SELECTOR_MATCHER = /^\*$/
var ATTRIBUTE_SELECTOR = /^\[+(?![0-9])\w{0,}(\s*=\s*)((?![0-9])\w{0,}|\"\w{0,}\")\]+$/
var ELEMENT_AND_ELEMENT = /^[a-zA-Z][a-zA-Z-]{0,}\s{0,}(\+\s{0,}[a-zA-Z][a-zA-Z-]{0,})+$/
var CONTENT_ID = /^[a-zA-Z][a-zA-Z-]{0,}([#][a-zA-Z][a-zA-Z0-9-]{0,})(.+?::(after|before))+$/

/**
 * expand margin、padding、border、borderWidth、borderColor、borderStyle properties、animation
 *
 * @param {object} subResult
 * @param {String} camelCasedName
 * @param {object} ruleResult
 */
function expand (subResult, camelCasedName, ruleResult) {
  if (camelCasedName === 'border') {
    subResult.value.forEach(function (item) {
      if (item.type === 'Width' || item.type === 'Color' || item.type === 'Style') {
        const spliceName = [camelCasedName + 'Top' + item.type, camelCasedName + 'Right' + item.type, camelCasedName +
          'Bottom' + item.type, camelCasedName + 'Left' + item.type]
        util.splitAttr(ruleResult, item.value, spliceName)
      }
      else {
        ruleResult[camelCasedName + item.type] = item.value
      }
    })
  }
  else if (['borderTop', 'borderRight', 'borderBottom', 'borderLeft'].includes(camelCasedName)) {
    subResult.value.forEach(function (item) {
      ruleResult[camelCasedName + item.type] = item.value
    })
  }
  else if (camelCasedName === 'margin' || camelCasedName === 'padding') {
    const spliceName = [camelCasedName + 'Top', camelCasedName + 'Right',
      camelCasedName + 'Bottom', camelCasedName + 'Left']
    util.splitAttr(ruleResult, subResult.value, spliceName)
  }
  else if (camelCasedName === 'borderWidth') {
    util.splitAttr(ruleResult, subResult.value, ['borderTopWidth', 'borderRightWidth',
      'borderBottomWidth', 'borderLeftWidth'])
  }
  else if (camelCasedName === 'borderColor') {
    util.splitAttr(ruleResult, subResult.value, ['borderTopColor', 'borderRightColor',
      'borderBottomColor', 'borderLeftColor'])
  }
  else if (camelCasedName === 'borderStyle') {
    util.splitAttr(ruleResult, subResult.value, ['borderTopStyle', 'borderRightStyle',
      'borderBottomStyle', 'borderLeftStyle'])
  }
  else if (camelCasedName === 'borderRadius') {
    util.splitAttr(ruleResult, subResult.value, ['borderBottomLeftRadius', 'borderBottomRightRadius',
      'borderTopLeftRadius', 'borderTopRightRadius'])
  }
  else if (camelCasedName === 'gridGap') {
    util.splitAttr(ruleResult, subResult.value, ['gridRowsGap', 'gridColumnsGap'])
  }
  else if (camelCasedName === 'boxShadow') {
    subResult.value.forEach(function (item) {
      if (item.type === 'H' || item.type === 'V' || item.type === 'Blur' || item.type === 'Spread' ||
        item.type === 'Color') {
        util.splitAttr(ruleResult, item.value, [camelCasedName + item.type])
      }
    })
  }
  else if (camelCasedName === 'animation') {
    Object.assign(ruleResult, subResult.value);
  }
  else {
    // never to do
  }
}

/**
 * expand flex style
 *
 * @param {object} rule
 * @param {Array} ruleLog
 */
function flexExpand(rule, ruleLog) {
  for (let i = 0; i < rule.declarations.length; i++) {
    let declaration = rule.declarations[i]
    if (declaration.property === 'flex') {
      let values =  declaration.value.split(/\s+/)
      rule.declarations.splice(i, 1)
      if (values.length === 1) {
        checkFlexOne(rule, ruleLog, declaration, values, i)
      } else if (values.length === 2) {
        checkFlexTwo(rule, ruleLog, declaration, values, i)
      } else if (values.length === 3) {
        checkFlexThree(rule, ruleLog, declaration, values, i)
      } else {
        ruleLog.push({
          line: declaration.position.start.line,
          column: declaration.position.start.column,
          reason: 'ERROR: Value `' + declaration.value + '` of the `' +
            declaration.property + '` attribute is incorrect.'
        })
      }
    }
  }
}

function getUnit(value) {
  value = value.toString().trim()
  let match = value.match(LENGTH_REGEXP)
  if (match) {
    let unit = match[1]
    if (unit) {
      if (unit === 'px') {
        return "px"
      }
    } else {
      return "none"
    }
  }
  return null
}

function checkFlexOne(rule, ruleLog, declaration, values, i) {
  const array = ['none', 'auto', 'initial']
  if (array.includes(values[0])) {
    rule.declarations.splice(i, 0, {type: 'declaration', property: 'flex',
      value: values[0], position: declaration.position})
  } else if (getUnit(values[0]) === 'px') {
    rule.declarations.splice(i, 0, {type: 'declaration', property: 'flex-basis',
      value: values[0], position: declaration.position})
  } else if (getUnit(values[0]) === 'none') {
    rule.declarations.splice(i, 0, {type: 'declaration', property: 'flex-grow',
      value: values[0], position: declaration.position})
  } else {
    ruleLog.push({
      line: declaration.position.start.line,
      column: declaration.position.start.column,
      reason: 'ERROR: Value `' + declaration.value + '` of the `' + declaration.property +
        '` attribute is incorrect.' + 'It must be a number, a number with unit `' + 'px`' +
        ', none, auto, or initial.'
    })
  }
}

function checkFlexTwo(rule, ruleLog, declaration, values, i) {
  if (getUnit(values[0]) === 'none') {
    rule.declarations.splice(i, 0, {type: 'declaration', property: 'flex-grow',
      value: values[0], position: declaration.position})
    if (getUnit(values[1]) === 'px') {
      rule.declarations.splice(i, 0, {type: 'declaration', property: 'flex-basis',
        value: values[1], position: declaration.position})
    } else if (getUnit(values[1]) === 'none') {
      rule.declarations.splice(i, 0, {type: 'declaration', property: 'flex-shrink',
        value: values[1], position: declaration.position})
    } else {
      ruleLog.push({
        line: declaration.position.start.line,
        column: declaration.position.start.column,
        reason: 'ERROR: Value `' + declaration.value + '` of the `' + declaration.property +
          '` attribute is incorrect. Value `' + values[1] +
          '` must be a number or a number with unit `' + 'px`.'
      })
    }
  } else {
    ruleLog.push({
      line: declaration.position.start.line,
      column: declaration.position.start.column,
      reason: 'ERROR: Value `' + declaration.value + '` of the `' +
      declaration.property + '` attribute is incorrect. Value `' + values[0] + '` must be a number.'
    })
  }
}

function checkFlexThree(rule, ruleLog, declaration, values, i) {
  if (getUnit(values[0]) === 'none' && getUnit(values[1]) === 'none' && getUnit(values[2]) === 'px') {
    rule.declarations.splice(i, 0, {type: 'declaration', property: 'flex-grow',
      value: values[0], position: declaration.position})
    rule.declarations.splice(i, 0, {type: 'declaration', property: 'flex-shrink',
      value: values[1], position: declaration.position})
    rule.declarations.splice(i, 0, {type: 'declaration', property: 'flex-basis',
      value: values[2], position: declaration.position})
  } else {
    ruleLog.push({
      line: declaration.position.start.line,
      column: declaration.position.start.column,
      reason: 'ERROR: Value `' + declaration.value + '` of the `' + declaration.property +
        '` attribute is incorrect. It must be in the format of (1, 1, 1px).'
    })
  }
}

/**
 * Parse `<style>` code to a JSON Object and log errors & warnings
 *
 * @param {string} code
 * @param {function} done which will be called with
 * - err:Error
 * - data.jsonStyle{}: `classname.propname.value`-like object
 * - data.log[{line, column, reason}]
 */
function parse(code, done, resourcePath) {
  var ast, err, jsonStyle = {}, log = []

  // css parse
  ast = css.parse(code, {silent: true, source: resourcePath});

  // catch syntax error
  if (ast.stylesheet.parsingErrors && ast.stylesheet.parsingErrors.length) {
    err = ast.stylesheet.parsingErrors
    err.forEach(function (error) {
      log.push({line: error.line, column: error.column, reason: error.toString().replace('Error', 'ERROR')})
    })
  }

  // walk all
  /* istanbul ignore else */
  if (ast && ast.type === 'stylesheet' && ast.stylesheet &&
      ast.stylesheet.rules && ast.stylesheet.rules.length) {
    ast.stylesheet.rules.forEach(function (rule) {
      var type = rule.type
      var ruleResult = {}
      var ruleLog = []

      if (type === 'rule') {
        if (rule.declarations && rule.declarations.length) {
          flexExpand(rule, ruleLog)

          rule.declarations.forEach(function (declaration) {
            var subType = declaration.type
            var name, value, line, column, subResult, camelCasedName

            /* istanbul ignore if */
            if (subType !== 'declaration') {
              return
            }

            name = declaration.property
            value = declaration.value

            // validate declarations and collect them to result
            camelCasedName = util.hyphenedToCamelCase(name)
            subResult = validateItem(camelCasedName, value)

            // expand margin、padding、border、borderWidth、borderColor、borderStyle properties、animation
            if (subResult.value && Object.values(util.SPLECIAL_ATTR).indexOf(camelCasedName) !== -1) {
              expand(subResult, camelCasedName, ruleResult)
            }

            /* istanbul ignore else */
            if ((typeof subResult.value === 'number' || typeof subResult.value === 'string')
              && !Object.values(util.SPLECIAL_ATTR).includes(camelCasedName)) {
              ruleResult[camelCasedName] = subResult.value
            }
            if (subResult.log) {
              subResult.log.line = declaration.position.start.line
              subResult.log.column = declaration.position.start.column
              ruleLog.push(subResult.log)
            }
          })

          if (card && rule.selectors.length > 1) {
            log.push({
              line: rule.position.start.line,
              column: rule.position.start.column,
              reason: 'ERROR: The `' + rule.selectors.join(', ') + '` selector is not supported.'
            })
          } else {
            rule.selectors.forEach(function (selector) {
              const flag = card ? selector.match(CARD_SELECTOR) :
                selector.match(SELECTOR_MATCHER) || selector.match(DESCENDANT_SELECTOR_MATCHER) ||
                  selector.match(ALL_SELECTOR_MATCHER) || selector.match(ATTRIBUTE_SELECTOR) ||
                  selector.match(ELEMENT_AND_ELEMENT) || selector.match(CONTENT_ID)
              if (flag) {
                var className = selector

                // handle pseudo class
                var pseudoIndex = className.indexOf(':')
                if (pseudoIndex > -1) {
                  var pseudoCls = className.slice(pseudoIndex)
                  className = className.slice(0, pseudoIndex)
                  var pseudoRuleResult = {}
                  Object.keys(ruleResult).forEach(function (prop) {
                    pseudoRuleResult[prop + pseudoCls] = ruleResult[prop]
                  })
                  ruleResult = pseudoRuleResult
                }

                // merge style
                Object.keys(ruleResult).forEach(function (prop) {
                  // handle transition
                  if (prop.indexOf('transition') === 0 && prop !== 'transition') {
                    var realProp = prop.replace('transition', '')
                    realProp = realProp[0].toLowerCase() + realProp.slice(1)
                    jsonStyle['@TRANSITION'] = jsonStyle['@TRANSITION'] || {}
                    jsonStyle['@TRANSITION'][className] = jsonStyle['@TRANSITION'][className] || {}
                    jsonStyle['@TRANSITION'][className][realProp] = ruleResult[prop]
                  }

                  jsonStyle[className] = jsonStyle[className] || {}
                  jsonStyle[className][prop] = ruleResult[prop]
                })
              } else {
                log.push({
                  line: rule.position.start.line,
                  column: rule.position.start.column,
                  reason: 'ERROR: The `' + selector + '` selector is not supported.'
                })
              }
            })
          }
          log = log.concat(ruleLog)
        }
      }
      /* istanbul ignore else */
      else if (type === 'font-face') {
        /* istanbul ignore else */
        if (rule.declarations && rule.declarations.length) {
          rule.declarations.forEach(function (declaration) {
            /* istanbul ignore if */
            if (declaration.type !== 'declaration') {
              return
            }
            var name = util.hyphenedToCamelCase(declaration.property)
            var value = declaration.value
            if (name === 'fontFamily' && '\"\''.indexOf(value[0]) > -1) {
              value = value.slice(1, value.length - 1)
            }
            ruleResult[name] = value
          })
          if (!jsonStyle['@FONT-FACE']) {
            jsonStyle['@FONT-FACE'] = []
          }
          jsonStyle['@FONT-FACE'].push(ruleResult)
        }
      }
      else if (type === 'import') {
        parseImport(resourcePath, rule, jsonStyle, log)
      }
      else if (type === 'keyframes' && !card) {
        if (!jsonStyle['@KEYFRAMES']) {
          jsonStyle['@KEYFRAMES'] = {}
        }
        var keyName = rule.name
        jsonStyle['@KEYFRAMES'][keyName] = []
        if (rule.keyframes && rule.keyframes.length) {
          if (card) {
            log.push({
              line: rule.position.start.line,
              column: rule.position.start.column,
              reason: 'ERROR: The keyframes is not supported!'
            })
          } else {
            rule.keyframes.forEach(function (keyframe) {

              var keyframeType = keyframe.type

              /* istanbul ignore if */
              if (keyframeType !== 'keyframe') {
                return
              }

              if (keyframe.declarations && keyframe.declarations.length) {
                keyframe.declarations.forEach(function (declaration) {
                  var subType = declaration.type
                  var name, value, line, column, subResult, camelCasedName

                  /* istanbul ignore if */
                  if (subType !== 'declaration') {
                    return
                  }

                  name = declaration.property
                  value = declaration.value

                  // validate declarations and collect them to result
                  camelCasedName = util.hyphenedToCamelCase(name)
                  subResult = validateItem(camelCasedName, value)

                  // expand margin、padding、border、borderWidth、borderColor、borderStyle properties
                  if (subResult.value && Object.values(util.SPLECIAL_ATTR).indexOf(camelCasedName) !== -1) {
                    expand(subResult, camelCasedName, ruleResult)
                  }
                  /* istanbul ignore else */
                  if ((typeof subResult.value === 'number' || typeof subResult.value === 'string')
                    && !Object.values(util.SPLECIAL_ATTR).includes(camelCasedName)) {
                    ruleResult[camelCasedName] = subResult.value
                  }
                  if (subResult.log) {
                    subResult.log.line = declaration.position.start.line
                    subResult.log.column = declaration.position.start.column
                    ruleLog.push(subResult.log)
                  }
                })
              }

              if (keyframe.values[0] === 'from') {
                var keyframeResult = {}
                Object.keys(ruleResult).forEach(function (prop) {
                  keyframeResult[prop] = ruleResult[prop]
                })
                keyframeResult['time'] = 0
                jsonStyle['@KEYFRAMES'][keyName].push(keyframeResult)
              }
              if (keyframe.values[0] === 'to') {
                var keyframeResult = {}
                Object.keys(ruleResult).forEach(function (prop) {
                  keyframeResult[prop] = ruleResult[prop]
                })
                keyframeResult['time'] = 100
                jsonStyle['@KEYFRAMES'][keyName].push(keyframeResult)
              }
              var patt = new RegExp(/^(100|[1-9]?\d)%$/)
              if (patt.test(keyframe.values[0])) {
                var keyframeResult = {}
                Object.keys(ruleResult).forEach(function (prop) {
                  keyframeResult[prop] = ruleResult[prop]
                })
                keyframeResult['time'] = keyframe.values[0].replace("%", "")
                jsonStyle['@KEYFRAMES'][keyName].push(keyframeResult)
              }
            })
            log = log.concat(ruleLog)
          }
        }
      }
      else if (type === 'media') {
        if (!jsonStyle['@MEDIA']) {
          jsonStyle['@MEDIA'] = []
        }
        var condition =  rule.media
        var mediaObj = {}
        mediaObj['condition'] = condition

        if (rule.rules && rule.rules.length) {
          rule.rules.forEach(function(rule) {
            ruleResult = {}
            if (rule.type === 'import') {
              parseImport(resourcePath, rule, mediaObj, log)
            }
            if (rule.declarations && rule.declarations.length) {
              flexExpand(rule, ruleLog)
              rule.declarations.forEach(function (declaration) {
                var subType = declaration.type
                var name, value, line, column, subResult, camelCasedName

                /* istanbul ignore if */
                if (subType !== 'declaration') {
                  return
                }

                name = declaration.property
                value = declaration.value

                // validate declarations and collect them to result
                camelCasedName = util.hyphenedToCamelCase(name)
                subResult = validateItem(camelCasedName, value)
                // expand margin、padding、border、borderWidth、borderColor、borderStyle properties
                if (subResult.value && Object.values(util.SPLECIAL_ATTR).indexOf(camelCasedName) !== -1) {
                  expand(subResult, camelCasedName, ruleResult)
                }

                /* istanbul ignore else */
                if ((typeof subResult.value === 'number' || typeof subResult.value === 'string')
                  && !Object.values(util.SPLECIAL_ATTR).includes(camelCasedName)) {
                  ruleResult[camelCasedName] = subResult.value
                }
                if (subResult.log) {
                  subResult.log.line = declaration.position.start.line
                  subResult.log.column = declaration.position.start.column
                  ruleLog.push(subResult.log)
                }
              })
              rule.selectors.forEach(function (selector) {
                if (selector.match(SELECTOR_MATCHER) || selector.match(DESCENDANT_SELECTOR_MATCHER)) {
                  var className = selector

                  // handle pseudo class
                  var pseudoIndex = className.indexOf(':')
                  if (pseudoIndex > -1) {
                    var pseudoCls = className.slice(pseudoIndex)
                    className = className.slice(0, pseudoIndex)
                    var pseudoRuleResult = {}
                    Object.keys(ruleResult).forEach(function (prop) {
                      pseudoRuleResult[prop + pseudoCls] = ruleResult[prop]
                    })
                    ruleResult = pseudoRuleResult
                  }
                  // merge style
                  Object.keys(ruleResult).forEach(function (prop) {
                    // handle transition
                    if (prop.indexOf('transition') === 0 && prop !== 'transition') {
                      var realProp = prop.replace('transition', '')
                      realProp = realProp[0].toLowerCase() + realProp.slice(1)
                      mediaObj['@TRANSITION'] = mediaObj['@TRANSITION'] || {}
                      mediaObj['@TRANSITION'][className] = mediaObj['@TRANSITION'][className] || {}
                      mediaObj['@TRANSITION'][className][realProp] = ruleResult[prop]
                    }
                    mediaObj[className] = mediaObj[className] || {}
                    mediaObj[className][prop] = ruleResult[prop]
                  })
                } else {
                  log.push({
                    line: rule.position.start.line,
                    column: rule.position.start.column,
                    reason: 'ERROR: The `' + selector + '` selector is not supported.'
                  })
                }
              })
              log = log.concat(ruleLog)
            }
          })
        }
        jsonStyle['@MEDIA'].push(mediaObj)
      }
    })
  }

  done(err, {jsonStyle: jsonStyle, log: log})
}

function parseImport(resourcePath, rule, jsonStyle, log) {
  if(!resourcePath) {
    return
  }
  const resourcePath_ = resourcePath
  let importString = rule.import
  let importPath
  let mediaString = ''
  let source = ''
  if (importString.match(IMPORT_MATCHER)) {
    let filePath = importString.match(/['"]([^()]+?)['"]/)
    importPath = filePath[1]
    mediaString = importString.replace(importPath, '').replace(/['"]/g, '')
  }
  if(/^(\.)|(\.\.)\//.test(importPath)) {
    resourcePath = resourcePath.substring(0, resourcePath.lastIndexOf(path.sep) + 1);
    importPath = path.resolve(resourcePath, importPath)
  }
  if (!fs.existsSync(importPath)) {
    const fileSearch = findFile(importPath);
    if (fileSearch.result == true) {
      importPath = fileSearch.filePath;
    } else {
      writeErrorOption();
      log.push({
        line: rule.position.start.line,
        column: rule.position.start.column,
        reason: 'ERROR: no such file or directory, open ' + importPath
      });
      return;
    }
  }
  source = fs.readFileSync(importPath).toString();
  addPreviewCSS(importPath, resourcePath_);
  if (mediaString.length !== 0) {
    source = '@media ' + mediaString + '{\n' + source + '\n}'
  }
  parse(source, (err, obj) => {
    if (err) {
      throw(err)
    } else {
      jsonStyle = Object.assign(jsonStyle, obj.jsonStyle)
    }
  }, importPath)
}

function addPreviewCSS(importPath, resourcePath) {
  importPath = path.join(importPath);
  resourcePath = path.join(resourcePath);
  if (fs.existsSync(process.env.watchCSSFiles)) {
    const content = JSON.parse(fs.readFileSync(process.env.watchCSSFiles));
    if (content['entry'] && content['entry'][resourcePath]) {
      content[importPath] = content[importPath] || [];
      content[importPath].push(resourcePath);
      content[importPath] = lodash.uniqWith(content[importPath], lodash.isEqual);
    } else if (content[resourcePath]) {
      content[importPath] = content[importPath] || [];
      content[importPath].push(...content[resourcePath]);
      content[importPath] = lodash.uniqWith(content[importPath], lodash.isEqual);
    }
    content["atime"] = content["atime"] || {};
    content["atime"][importPath] = fs.statSync(importPath).atime.toString();
    fs.writeFileSync(process.env.watchCSSFiles, JSON.stringify(content, null, 2));
  }
}

function findFile(importPath) {
  const resultObject = {
    result: false
  };
  if (!importPath || !process.env.resolveModules) {
    return resultObject;
  }
  try {
    const modules = JSON.parse(process.env.resolveModules);
    modules.forEach(item => {
      if (fs.existsSync(item)) {
        if (fs.existsSync(path.join(item, importPath))) {
          resultObject.result = true;
          resultObject.filePath = path.join(item, importPath);
          return resultObject;
        }
      } else {
        const resolveItem = path.resolve(__dirname, item);
        if (fs.existsSync(resolveItem)) {
          resultObject.result = true;
          resultObject.filePath = path.join(resolveItem, importPath);
          return resultObject;
        }
      }
    });
  } catch (error) {
    resultObject.result = false;
  }
  return resultObject;
}

function writeErrorOption() {
  if (fs.existsSync(process.env.watchCSSFiles)) {
    const content = JSON.parse(fs.readFileSync(process.env.watchCSSFiles));
    content['clear'] = true;
    fs.writeFileSync(process.env.watchCSSFiles, JSON.stringify(content, null, 2));
  }
}

/**
 * Validate a JSON Object and log errors & warnings
 *
 * @param {object} json
 * @param {function} done which will be called with
 * - err:Error
 * - data.jsonStyle{}: `classname.propname.value`-like object
 * - data.log[{reason}]
 */
function validate(json, done) {
  var log = []
  var err

  try {
    json = JSON.parse(JSON.stringify(json))
  }
  catch (e) {
    err = e
    json = {}
  }

  Object.keys(json).forEach(function (selector) {
    var declarations = json[selector]

    Object.keys(declarations).forEach(function (name) {
      var value = declarations[name]
      var result = validateItem(name, value)

      if (typeof result.value === 'number' || typeof result.value === 'string') {
        declarations[name] = result.value
      }
      else {
        delete declarations[name]
      }

      if (result.log) {
        log.push(result.log)
      }
    })
  })

  done(err, {
    jsonStyle: json,
    log: log
  })
}

module.exports = {
  parse: parse,
  validate: validate,
  validateItem: validateItem,
  util: util,
  expand: expand,
  getUnit: getUnit,
}

