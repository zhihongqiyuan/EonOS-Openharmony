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

import Log from '../../../../../../../../common/src/main/ets/default/Log';
import SimpleToggleLoadComponent from './SimpleToggleLoadComponent';
import StyleConfiguration, { SimpleToggleLayoutEditGridStyle } from '../common/StyleConfiguration';

const TAG = 'Control-SimpleToggleLayoutEditGrid';

@Component
export default struct SimpleToggleLayoutEditGrid {
  @Link mToggles: string[];
  private mMaxCount: number = 0;
  private mMinCount: number = 0;
  @Prop mColumnCount: number;
  @Prop mGlobalDragToggleName: string;
  private gridTag: string  = '';
  private logTag: string = '';
  private onItemDragStart: (toggleName: string) => void = () => {};
  private onItemDrop: (status: string) => void = () => {};
  @State style: SimpleToggleLayoutEditGridStyle = StyleConfiguration.getSimpleToggleLayoutEditGridStyle();
  private mCurrentDragToggleName: string = '';

  @Builder DragComponent() {
    Column() {
      SimpleToggleLoadComponent({
        keyId: this.mCurrentDragToggleName,
        mEditMode: true,
        mDragMode: true,
      })
    }
    .width(this.style.dragBgSize)
    .height(this.style.dragBgSize)
  }

  aboutToAppear() {
    this.logTag = TAG + '-' + this.gridTag
    Log.showInfo(this.logTag, this.gridTag + 'aboutToAppear');
  }

  aboutToDisappear() {
    Log.showInfo(this.logTag, 'aboutToDisappear ');
  }

  build() {
    Grid() {
      ForEach(this.mToggles, (componentName: string) => {
        GridItem() {
          Column() {
            SimpleToggleLoadComponent({
              keyId: componentName,
              mEditMode: true
            })
          }.width('100%')
          .height(108)
        }
      }, (componentName: string) => componentName)
      if (this.mToggles.length == 0) {
        GridItem() {
        }
      }
    }
    .supportAnimation(true)
    .height(this.calcGridHeight(Math.ceil(this.mToggles.length / this.mColumnCount), this.style.rowHeight, this.style.rowGap))
    .width('100%')
    .columnsTemplate(this.calcColumnsTemplate(this.mColumnCount))
    .editMode(true)
    .maxCount(this.calcMaxRowCount(this.mMaxCount, this.mColumnCount))
    .minCount(this.calcMinRowCount(this.mMinCount, this.mColumnCount))
    .cellLength(this.style.rowHeight)
    .rowsGap(this.style.rowGap + 'px')
    .columnsGap(this.style.columnGap)
    .layoutDirection(1)
    .direction(0)
    .onItemDragEnter(this.onItemDragEnter.bind(this))
    .onItemDragMove(this.onItemDragMove.bind(this))
    .onItemDragLeave(this.onItemDragLeave.bind(this))
    .onItemDragStart((event, itemIndex) => {
      this.onGridItemDragStart(event, itemIndex)
      return this.DragComponent
    })
    .onItemDrop(this.onGridItemDrop.bind(this))
  }

  calcGridHeight(rowCount: number, rowHeight: number, rowGap: number) {
    Log.showDebug(this.logTag, `calcGridHeight, rowCount: ${rowCount} rowHeight: ${rowHeight} rowGap: ${rowGap}`);
    let height = rowCount * rowHeight + (rowCount - 1) * rowGap;
    if (height <= 0) {
      height = 108;
    }
    Log.showDebug(this.logTag, `calcGridHeight, height: ${height}`);
    return height + 'px';
  }

  calcColumnsTemplate(columnCount: number): string{
    Log.showDebug(this.logTag, `calcColumnsTemplate, columnCount: ${columnCount}`);
    let columnsTemplate = '1fr';
    for (let i = 1;i < columnCount; i++) {
      columnsTemplate += ' 1fr';
    }
    Log.showDebug(this.logTag, `calcColumnsTemplate, columnsTemplate: ${columnsTemplate}`);
    return columnsTemplate;
  }

  calcMaxRowCount(maxCount: number, columnCount: number): number{
    Log.showDebug(this.logTag, `calcMaxRowCount, maxCount: ${maxCount} columnCount: ${columnCount}`);
    let maxRowCount = Math.ceil(maxCount / columnCount);
    if (maxRowCount < 1) {
      maxRowCount = 1;
    };
    Log.showDebug(this.logTag, `calcMaxRowCount, maxRowCount: ${maxRowCount}`);
    return maxRowCount;
  }

  calcMinRowCount(minCount: number, columnCount: number): number{
    Log.showDebug(this.logTag, `calcMinRowCount, minCount: ${minCount} columnCount: ${columnCount}`);
    let minRowCount = Math.ceil(minCount / columnCount);
    if (minRowCount < 1) {
      minRowCount = 1;
    }
    Log.showDebug(this.logTag, `calcMinRowCount, minRowCount: ${minRowCount}`);
    return minRowCount;
  }

  onItemDragEnter(event: ItemDragInfo) {
    Log.showDebug(this.logTag, `onItemDragEnter, event: ${JSON.stringify(event)}`);
  }

  onItemDragMove(event: ItemDragInfo, itemIndex: number, insertIndex: number) {
    Log.showDebug(this.logTag, `onItemDragMove, itemIndex: ${itemIndex} insertIndex: ${insertIndex} event: ${JSON.stringify(event)}`);
  }

  onItemDragLeave(event: ItemDragInfo, itemIndex: number) {
    Log.showDebug(this.logTag, `onItemDragLeave, itemIndex: ${itemIndex} event: ${JSON.stringify(event)}`);
  }

  onGridItemDragStart(event: ItemDragInfo, itemIndex: number) {
    Log.showDebug(this.logTag, `onGridItemDragStart, itemIndex: ${itemIndex} event: ${JSON.stringify(event)}`);
    this.mCurrentDragToggleName = this.mToggles[itemIndex];
    if (this.onItemDragStart) {
      this.onItemDragStart(this.mCurrentDragToggleName);
    };
    Log.showDebug(this.logTag, `onGridItemDragStart, mCurrentDragToggleName: ${this.mCurrentDragToggleName}`);
  }

  onGridItemDrop(event: ItemDragInfo, itemIndex: number, insertIndex: number, isSuccess: boolean) {
    Log.showDebug(this.logTag, `onGridItemDrop, itemIndex: ${itemIndex} insertIndex: ${insertIndex} isSuccess: ${isSuccess} event: ${JSON.stringify(event)}`);
    if (!isSuccess) {
      this.callOnItemDrop('fail');
      return;
    };
    if (itemIndex == insertIndex) {
      this.callOnItemDrop('cancel');
      return;
    };
    let status = 'success';
    let currentDragToggleName = this.mCurrentDragToggleName;
    let globalDragToggleName = this.mGlobalDragToggleName;
    Log.showDebug(this.logTag, `onItemDrop, currentDragToggleName: ${currentDragToggleName}`);
    Log.showDebug(this.logTag, `onItemDrop, globalDragToggleName: ${globalDragToggleName}`);
    if (itemIndex >= 0 && insertIndex >= 0) { // move
      Log.showDebug(this.logTag, `onItemDrop, move`);
      if (itemIndex > insertIndex) {
        this.mToggles.splice(insertIndex, 0, currentDragToggleName);
        this.mToggles.splice(itemIndex + 1, 1);
      } else {
        this.mToggles.splice(insertIndex + 1, 0, currentDragToggleName);
        this.mToggles.splice(itemIndex, 1);
      };
    } else if (itemIndex == -1 && insertIndex >= 0) { // insert
      Log.showDebug(this.logTag, `onItemDrop, insert`);
      if (this.mToggles.length < this.mMaxCount) {
        this.mToggles.splice(insertIndex, 0, globalDragToggleName);
      } else {
        this.mToggles = JSON.parse(JSON.stringify(this.mToggles));
        status = 'cancel';
      };
    } else if (itemIndex >= 0 && insertIndex == -1) { // delete
      Log.showDebug(this.logTag, `onItemDrop, delete`);
      if (this.mToggles.length > this.mMinCount) {
        this.mToggles.splice(itemIndex, 1);
      } else {
        this.mToggles = JSON.parse(JSON.stringify(this.mToggles));
        status = 'cancel';
      };
    };
    this.callOnItemDrop(status);
  }

  callOnItemDrop(status: string): void {
    Log.showDebug(this.logTag, `callOnItemDrop, status: ${status}`);
    if (this.onItemDrop) {
      this.onItemDrop(status);
    };
    this.mCurrentDragToggleName = '';
  }
}
