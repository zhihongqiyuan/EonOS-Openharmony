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
import fileio from '@ohos.fileio';
import util from '@ohos.util';
import { GPData } from '../../entity/DatabaseEntity';
import SPLogger from '../../utils/SPLogger';

const BUFF_SIZE = 1024;
const TAG = 'BaseProfilerUtils';
/**
 * 构造GPData
 */
export function createGPData(moduleType: string, values: Map<String, String>): GPData {
  var now = new Date();
  return new GPData(moduleType, now.getTime(), values);
}

export function isAccess(path: string): boolean {
  var isAccess = false;
  try {
    fileio.accessSync(path);
    isAccess = true;
  } catch (err) {
    SPLogger.DEBUG(TAG, 'accessSync failed with error:' + err + 'path:' + path);
  }
  return isAccess;
}

/**
 * 通用文件节点打开
 * @param path
 */
export function fileOpen(path: string): String {
  if (!isAccess(path)) {
    return 'null';
  }

  try {
    var fd = -1;
    fd = fileio.openSync(path, 0o0);
    let buf = new ArrayBuffer(BUFF_SIZE);
    fileio.readSync(fd, buf);
    var result: String = String.fromCharCode.apply(null, new Uint8Array(buf));
    return util.printf('%s', result.substring(0, lastIndex(result))).toString();
  } catch (err) {
    SPLogger.ERROR(TAG, 'fileOpen path:' + path + ' error:' + err);
    let errStr: String = err;
    if (errStr.indexOf('denied') > 0) {
      return 'null';
    }
  } finally {
    fileio.closeSync(fd);
  }
  return 'null';
}

/**
 * 通用遍历目录节点
 * @param path
 * @param regexp
 */
export function travelFile(path: string, regexp: string): Array<String> {
  let dir;
  let fileNames = [];

  if (!isAccess(path)) {
    return [];
  }

  try {
    dir = fileio.opendirSync(path);
    do {
      var dirent = dir.readSync();
      if (dirent == null) {
        break;
      }
      let name: String = dirent.name;
      if (name.match(regexp)) {
        fileNames.push(name);
      }
      if (regexp == '') {
        fileNames.push(name);
      }
    } while (dirent != null);
  } catch (err) {
    SPLogger.ERROR(TAG, 'travelFile get err:' + err);
    SPLogger.ERROR(TAG, 'travelFile err path:' + path);
  } finally {
    dir.closeSync();
  }
  return fileNames;
}

/**
 * 返回字符结尾符坐标
 * @param str
 */
function lastIndex(str) {
  var index = -1;
  for (var i = 0; i < str.length; i++) {
    var temp = str.charCodeAt(i).toString(16);
    if (temp == 'a') {
      return i;
    }
  }
  return index;
}

/**
 * 睡眠函数
 * @param numberMillis
 */
export function sleep(numberMillis) {
  var now = new Date();
  var exitTime = now.getTime() + numberMillis;
  while (true) {
    now = new Date();
    if (now.getTime() > exitTime) {
      return;
    }
  }
}
/**
 * 提取字符串数字
 */
export function extractNumber(originStr) {
  let result = '';
  for (var index = 0; index < originStr.length; index++) {
    const element: string = originStr[index];
    if (element.match('^[0-9]*$')) {
      result += element;
    }
  }
  return result;
}
