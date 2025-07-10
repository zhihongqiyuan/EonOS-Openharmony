/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
const regex = new RegExp(
    '(?:[\\^\\$\\+\\*\\?\$\$\$\$\\{\\}\\\\\\|\\/\\."\'<>:]{2,4}?)' +
    '(?:(?:\\w(?!\\d))|(?:[\\s\\0\\b]{2}?(?=(?:[\\d|\\p{Lu}]))))' +
    '[\\p{Ll}|1-2]{2,5}?\\s*',
    'guis'
  );
const str = "^^^^\0\r12aaA \\\\\naBb  *\n\0Cc  \\\b2dd  \"\'\r3eE  :<>\n\rcff  \[\/\0dgGGg   \\\b4HH  \\\(w1iii  \|\|zABC$";
const matches = str.split(regex);
print(JSON.stringify(matches));
