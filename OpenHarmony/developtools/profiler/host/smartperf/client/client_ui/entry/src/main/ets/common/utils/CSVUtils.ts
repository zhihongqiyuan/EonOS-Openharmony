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

import { TGeneralInfo } from '../entity/DatabaseEntity';
import { TIndexInfo } from '../entity/DatabaseEntity';

//垂直生成 TGenneralInfo to string
export function csvGeneralInfo(tGeneralInfo: TGeneralInfo): string {
  let data = '';
  for (let k of Object.keys(tGeneralInfo)) {
    data += k + ',' + tGeneralInfo[k] + '\n';
  }
  return data;
}

//水平生成 TIndexInfo to string
export function csvTIndexInfo(tIndexInfos: Array<TIndexInfo>): string {
  let tittle = csvTIndexInfoTittle();
  let data = '';
  for (let index = 0; index < tIndexInfos.length; index++) {
    const t = tIndexInfos[index];
    for (let k of Object.keys(t)) {
      data += t[k] + ',';
    }
    data = data.substring(0, data.lastIndexOf(','));
    data += '\n';
  }
  let result = tittle + data;
  return result;
}

//水平生成 TIndexInfo TITTLE to string
export function csvTIndexInfoTittle(): string {
  let tIndexInfo: TIndexInfo = new TIndexInfo();
  let data = '';
  for (let k of Object.keys(tIndexInfo)) {
    data += k + ',';
  }
  data = data.substring(0, data.lastIndexOf(','));
  data += '\n';
  return data;
}
