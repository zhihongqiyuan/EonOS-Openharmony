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
import path from 'path';

import {
  OHOS_PLUGIN,
  NATIVE_MODULE,
  SYSTEM_PLUGIN
} from '../../../lib/pre_define';
import {
  preprocessExtend,
} from '../../../lib/validate_ui_syntax';

function normalizeFileContent(content: string): string {
  // Replace all types of line endings with a single newline character
  const normalizedLineEndings = content.replace(/\r\n|\r/g, '\n');

  // Remove leading and trailing whitespace from each line
  const normalizedWhitespace = normalizedLineEndings.split('\n').map(line => line.trim()).join('\n');

  // Remove empty lines
  const normalizedEmptyLines = normalizedWhitespace.split('\n').filter(line => line !== '').join('\n');

  return normalizedEmptyLines;
}

function processSystemApi(content: string): string {
  const REG_SYSTEM = 
    /import\s+(.+)\s+from\s+['"]@(system|ohos)\.(\S+)['"]|import\s+(.+)\s*=\s*require\(\s*['"]@(system|ohos)\.(\S+)['"]\s*\)/g;
  const REG_LIB_SO =
    /import\s+(.+)\s+from\s+['"]lib(\S+)\.so['"]|import\s+(.+)\s*=\s*require\(\s*['"]lib(\S+)\.so['"]\s*\)/g;
  const newContent = content.replace(REG_LIB_SO, (_, item1, item2, item3, item4) => {
    const libSoValue = item1 || item3;
    const libSoKey = item2 || item4;
    return `var ${libSoValue} = globalThis.requireNapi("${libSoKey}", true);`;
  }).replace(REG_SYSTEM, (item, item1, item2, item3, item4, item5, item6, item7) => {
    let moduleType = item2 || item5;
    let systemKey = item3 || item6;
    let systemValue = item1 || item4;
    if (NATIVE_MODULE.has(`${moduleType}.${systemKey}`)) {
      item = `var ${systemValue} = globalThis.requireNativeModule('${moduleType}.${systemKey}')`;
    } else if (moduleType === SYSTEM_PLUGIN) {
      item = `var ${systemValue} = isSystemplugin('${systemKey}', '${SYSTEM_PLUGIN}') ? ` +
        `globalThis.systemplugin.${systemKey} : globalThis.requireNapi('${systemKey}')`;
    } else if (moduleType === OHOS_PLUGIN) {
      item = `var ${systemValue} = globalThis.requireNapi('${systemKey}') || ` +
        `(isSystemplugin('${systemKey}', '${OHOS_PLUGIN}') ? ` +
        `globalThis.ohosplugin.${systemKey} : isSystemplugin('${systemKey}', '${SYSTEM_PLUGIN}') ` +
        `? globalThis.systemplugin.${systemKey} : undefined)`;
    }
    return item;
  });
  return newContent;
}

export function cleanCopyRight(str: string): string {
  const copyrightBlockRegex = /(?:\/\*.*Copyright \([c|C]\) [- \d]+ [\w ]+\., Ltd\..*\*\/)/gs;

  return str.replace(copyrightBlockRegex, '');
}

export function processExecInStr(str: string): string {
  const regex = /\${(.*?)}/g;
  return str.replace(regex, (match, p1) => {
    return eval(p1);
  });
}

export function parseFileNameFromPath(filePath: string): string {
  return path.basename(filePath, path.extname(filePath));
}

export function parseLog(log: string): string {
  try {
    const regexPattern = /(:\d+:\d+)?\n(.*)/gm;
    const matchResult = regexPattern.exec(log);
  
    const logInfo: string = matchResult[2].trim();
    return logInfo;
  } catch {
    return log;
  }
}

export function parseCode(code: string): string {
  return normalizeFileContent(cleanCopyRight(code));
}

export function sourceReplace(source: string): string {
  let content: string = source;
  content = preprocessExtend(content);
  content = processSystemApi(content);
  return content;
}
