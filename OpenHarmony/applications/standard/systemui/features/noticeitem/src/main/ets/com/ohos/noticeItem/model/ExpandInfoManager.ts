/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

const TAG = 'ExpandInfosManager';

export class ExpandInfoManager {
  //该类主要维护ExpandInfo这个集合，用于存储UX界面中组通知是否展开的信息,存在该组名表示组通知需要展开，否则表示不展开
  static ExpandInfo = new Set();

  private getItemDataName(itemData): string{
    return `${TAG}_${itemData.bundleName}_${itemData.groupName}`;
  }

  addExpandInfo(itemData): void{
    ExpandInfoManager.ExpandInfo.add(this.getItemDataName(itemData))
    return;
  }

  hasExpandInfo(itemData): boolean{
    if(ExpandInfoManager.ExpandInfo.has(this.getItemDataName(itemData))){
      return true;
    } else {
      return false;
    }
  }

  deleteExpandInfo(itemData): void{
    if(ExpandInfoManager.ExpandInfo.has(this.getItemDataName(itemData))){
        ExpandInfoManager.ExpandInfo.delete(this.getItemDataName(itemData))
    }
    return;
  }

  deleteAllExpandInfo(): void{
    ExpandInfoManager.ExpandInfo.clear();
    return;
  }

}

let expandInfoManager = new ExpandInfoManager();

export default expandInfoManager;