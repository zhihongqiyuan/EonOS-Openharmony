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

import loaderUtils from 'loader-utils';

const codegen = require("./codegen/index.js");

module.exports = function (source, map) {
  this.cacheable && this.cacheable();
  const callback = this.async();

  const parsed = codegen.genHmlAndCss(source);
  const loaderQuery = loaderUtils.getOptions(this) || {};
  if (parsed.errorType && parsed.errorType !== '') {
    callback(parsed.errorType + ' : ' + parsed.errorMessage, '');
  } else {
    switch (loaderQuery.type) {
      case 'template':
        callback(null, parsed.hmlCss.hml, map);
        break;
      case 'style':
        callback(null, parsed.hmlCss.css, map);
        break;
      case 'json':
        callback(null, parsed.hmlCss.json, map);
        break;
    }
  }
};
