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

const OHOS_THEME_PROP_GROUPS = require('./theme/ohosStyles')
module.exports = function (source) {
  let result
  // target format: this.$r("ohos.id_color_background") or this.$r('ohos.id_color_background')  or
  // "this.$r('ohos.id_color_background')"
  let ResourceRefReg = /"?this\s*\.\$r\s*\(\s*((['"]ohos\.(?<resName>\w+)['"]))\s*\)"?/g
  while (result = ResourceRefReg.exec(source)) {
    const resourceName = result.groups['resName']
    if (resourceName && OHOS_THEME_PROP_GROUPS[resourceName]) {
      const resourceId = "\"@ohos_id_" + OHOS_THEME_PROP_GROUPS[resourceName] + "\""
      source = source.replace(result[0], resourceId)
      // update the start position of the next match
      ResourceRefReg.lastIndex -= result[0].length - resourceId.length
    }
  }

  // target format: this.$r("sys.type.id_color_background") or this.$r('sys.type.id_color_background') or
  // "this.$r('sys.type.id_color_background')"
  // The "type" field can be "float", "color", "string" and so on.
  let SysResourceTypeRefReg = /"?this\s*\.\$r\s*\(\s*((['"]sys\.(?<resType>\w+)\.(?<resName>\w+)['"]))\s*\)"?/g
  while (result = SysResourceTypeRefReg.exec(source)) {
    const resourceName = result.groups['resName']
    const resourceType = result.groups['resType']
    if (resourceName && resourceType && OHOS_THEME_PROP_GROUPS[resourceName]) {
      const resourceId = "\"@sys." + resourceType + "." + OHOS_THEME_PROP_GROUPS[resourceName] + "\""
      source = source.replace(result[0], resourceId)
      // update the start position of the next match
      SysResourceTypeRefReg.lastIndex -= result[0].length - resourceId.length
    }
  }

  // target format: this.$r("app.type.developer_defined_color") or this.$r('app.type.developer_defined_color') or
  // "this.$r('app.type.developer_defined_color')"
  // The "type" field can be "float", "color", "string" and so on.
  let AppResourceTypeRefReg = /"?this\s*\.\$r\s*\(\s*((['"]app\.(?<resType>\w+)\.(?<resName>\w+)['"]))\s*\)"?/g
  while (result = AppResourceTypeRefReg.exec(source)) {
    const resourceName = result.groups['resName']
    const resourceType = result.groups['resType']
    if (resourceName && resourceType) {
      const resourceId = "\"@app." + resourceType + "." + resourceName + "\""
      source = source.replace(result[0], resourceId)
      // update the start position of the next match
      AppResourceTypeRefReg.lastIndex -= result[0].length - resourceId.length
    }
  }

  return source
}