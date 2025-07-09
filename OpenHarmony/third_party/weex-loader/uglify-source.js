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
const uglifyJS = require('uglify-js');

const SOURCE_POSITION = 2;
readCode(process.argv[SOURCE_POSITION]);

function readCode(inputPath) {
  if (fs.existsSync(inputPath)) {
    const files = fs.readdirSync(inputPath);
    files.forEach(function(file) {
      const filePath = path.join(inputPath, file);
      if (fs.existsSync(filePath)) {
        const fileStat = fs.statSync(filePath);
        if (fileStat.isFile()) {
          const code = fs.readFileSync(filePath, 'utf-8');
          uglifyCode(code, filePath);
        }
        if (fileStat.isDirectory()) {
          readCode(filePath);
        }
      }
    });
  }
}

function uglifyCode(code, outPath) {
  const uglifyCode = uglifyJS.minify(code).code;
  fs.writeFileSync(outPath, uglifyCode);
}
