/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
const moduleSource = require('./module-source');

const exists = function(src, dst, callback) {
  if (src.match(/\/test$/)) {
    return;
  }
  fs.exists(dst, function(exists) {
    if (exists) {
      callback(src, dst);
    } else {
      fs.mkdir(dst, function() {
          callback(src, dst);
      });
    }
  });
};

stat = fs.stat;
const copy = function(src, dst) {
  fs.readdir(src, function(err, paths) {
      if (err) {
          throw err;
      }
      paths.forEach(function(_path) {
        copyForEach(src, dst, _path);
      });
  });
};

function copyForEach(src, dst, _path) {
  let _src = src + '/' + _path;
  let _dst = dst + '/' + _path;
  let readable = null;
  let writable = null;
  stat(_src, function(err, st) {
    if (err) {
        throw err;
    }
    if (st.isFile()) {
      const pathInfo = path.parse(_src);
      if (pathInfo.name === 'gulpfile' || pathInfo.ext !== '.js') {
        return;
      }
      readable = fs.createReadStream(_src);
      writable = fs.createWriteStream(_dst);
      readable.pipe(writable);
    } else if (st.isDirectory()) {
      exists(_src, _dst, copy);
    }
  });
}

moduleSource.copyResource(path.resolve(__dirname, './third_party/weex-loader/deps/weex-scripter'), process.argv[2] + '/scripter');
moduleSource.copyResource(path.resolve(__dirname, './third_party/weex-loader/deps/weex-styler'), process.argv[2] + '/styler');
moduleSource.copyResource(path.resolve(__dirname, './third_party/parse5/packages/parse5/dist/cjs'), process.argv[2] + '/parse');
