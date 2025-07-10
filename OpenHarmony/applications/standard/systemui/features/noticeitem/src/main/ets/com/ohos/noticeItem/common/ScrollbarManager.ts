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
import Log from '../../../../../../../../../common/src/main/ets/default/Log';

const TAG = 'ScrollbarManager';

export default class ScrollbarManager {
  static notificationScrollBar = new Set<Scroller>();

  static add(scroller: Scroller): void {
    let res = ScrollbarManager.notificationScrollBar.add(scroller);
    Log.showInfo(TAG, `add set's size:${res.size}`);
  }

  static delete(scroller: Scroller): void {
    Log.showInfo(TAG, 'delete');
    ScrollbarManager.notificationScrollBar.delete(scroller);
  }

  static clear(): void {
    Log.showInfo(TAG, 'clear');
    ScrollbarManager.notificationScrollBar.clear();
  }

  static restoreOtherScroll(scroller: Scroller): void {
    Log.showInfo(TAG, 'restoreOtherScroll');
    if (scroller.currentOffset().xOffset > 0) {
      ScrollbarManager.notificationScrollBar.forEach((item) => {
        if (item !== scroller && item.currentOffset().xOffset > 0) {
          item.scrollEdge(Edge.Start);
        }
      });
      ScrollbarManager.clear();
      ScrollbarManager.notificationScrollBar.add(scroller);
    }
  }
}
