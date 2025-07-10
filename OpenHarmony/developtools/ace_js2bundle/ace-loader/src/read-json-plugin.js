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

const JSON5 = require("json5");

module.exports = class ReadJsonPlugin {
  apply(resolver) {
    if (resolver && resolver.fileSystem && resolver.fileSystem.readFile) {
      resolver.fileSystem.readJson = (filepath, callback) => {
        resolver.fileSystem.readFile(filepath, (error, content) => {
          if (error) {
            return callback(error);
          }
          if (!content || content.length === 0) {
            return callback(new Error("No file content"));
          }
          let data;
          try {
            if (/\.json5$/.test(filepath)) {
              data = JSON5.parse(content.toString("utf-8"));
            } else {
              data = JSON.parse(content.toString("utf-8"));
            }
          } catch (e) {
            return callback(e);
          }
          callback(null, data);
        });
      };
    }
  }
};
