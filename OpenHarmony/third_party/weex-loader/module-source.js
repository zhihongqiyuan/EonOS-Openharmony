/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
      const _src = src + '/' + _path;
      const _dst = dst + '/' + _path;
      let readable;
      let writable;
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
    });
  });
};

function copyResource(src, dist) {
  exists(path.resolve(__dirname, src), dist, copy);
}

const TARGET_POSITION = 2;
copyResource(path.resolve(__dirname, './deps/weex-scripter'), process.argv[TARGET_POSITION] + '/scripter');
copyResource(path.resolve(__dirname, './deps/weex-styler'), process.argv[TARGET_POSITION] + '/styler');
