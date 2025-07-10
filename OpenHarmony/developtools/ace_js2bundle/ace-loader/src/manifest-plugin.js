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

const { DEVICE_LEVEL } = require('./lite/lite-enum');
module.exports = function(source, map) {
  this.cacheable && this.cacheable()
  const callback = this.async()

  if (process.env.DEVICE_LEVEL === DEVICE_LEVEL.LITE) {
    callback(null, JSON.stringify({ 'manifest.json': 'content' }), map)
  } else {
    callback(null, source, map)
  }
};
