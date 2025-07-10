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

const TAG = 'SwipeLayout_FocusCallback';

class FocusCallback{
  static callback: () => void = null;
  static lastFocusId: string = null;

  setCallback(name: string, callback: () => void): void{
    if(FocusCallback.lastFocusId != name){
      if(FocusCallback.callback != null){
        FocusCallback.callback();
      }
      FocusCallback.callback = callback;
      FocusCallback.lastFocusId = name;
    }
    return;
  }

  deleteCallback(name: string): void{
    if( name == FocusCallback.lastFocusId){
      FocusCallback.lastFocusId = null;
      FocusCallback.callback = null;
    }
    return;
  }

  getLastFocusId(): string{
    return FocusCallback.lastFocusId;
  }
}

export function getId(itemData, isGroup): string{
  if(isGroup == false){
    return `swipelayout_item_${itemData.hashcode}_${itemData.timestamp}`;
  } else {
    return `swipelayout_group_${itemData.hashcode}_${itemData.timestamp}`;
  }
}

let focusCallBack = new FocusCallback();

export default focusCallBack;