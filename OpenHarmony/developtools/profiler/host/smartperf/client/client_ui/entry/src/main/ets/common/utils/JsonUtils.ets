/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

export default {
  _mapToJson(map: Map<string, object>): string {
    var obj = Object.create(null);

    var iterator = map.keys();
    for (var i = 0; i < map.size; i++) {
      var key = iterator.next().value;
      obj[key] = map.get(key);
    }
    return JSON.stringify(obj);
  },
};
