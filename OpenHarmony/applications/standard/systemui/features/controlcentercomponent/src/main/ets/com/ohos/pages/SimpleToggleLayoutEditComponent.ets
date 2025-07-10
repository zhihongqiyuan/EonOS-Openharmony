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
import Constants from '../common/Constants';
import StyleConfiguration, { SimpleToggleLayoutEditComponentStyle, SimpleToggleLayoutEditUpTitleStyle,
  SimpleToggleLayoutEditOptMsgStyle } from '../common/StyleConfiguration';
import SimpleToggleLayoutEditGrid from './SimpleToggleLayoutEditGrid';
import SimpleToggleLayoutEditDialogComponent from './SimpleToggleLayoutEditDialogComponent';

const TAG = 'Control-SimpleToggleLayoutEditComponent';
const TAG_SimpleToggleLayoutEditUpTitle = 'Control-SimpleToggleLayoutEditUpTitle';
const TAG_SimpleToggleLayoutEditOptMsg = 'Control-SimpleToggleLayoutEditOptMsg';

@Component
export default struct SimpleToggleLayoutEditComponent {
  private mDisplayingToggles: string[] = [];
  private mHidingToggles: string[] = [];
  private mDefaultDisplayToggles: string[] = [];
  @Prop mColumnCount: number;
  private simpleToggleLayoutEditEndCallback: () => void = () => {};
  private onSaveDisplayingToggles: (toggles: string[]) => void = (toggles) => {};
  @State mNewDisplayingToggles: string[] = [];
  @State mNewHidingToggles: string[] = [];
  @State style: SimpleToggleLayoutEditComponentStyle = StyleConfiguration.getSimpleToggleLayoutEditComponentStyle();
  @State mOptMsg: Resource = $r('app.string.control_center_simple_toggle_layout_edit_opt_desc');
  private mDisplayingTogglesMaxCount: number = Constants.SIMPLE_TOGGLE_LAYOUT_MAX_TOGGLE_COUNT;
  private mDisplayingTogglesMinCount: number = Constants.SIMPLE_TOGGLE_LAYOUT_MIN_TOGGLE_COUNT;
  private mHidingTogglesMaxCount: number = 0;
  private mHidingTogglesMinCount: number = 0;
  @State mCurrentDragToggleName: string = '';
  private titleDisplayInside: boolean = false;
  private backDialogController: CustomDialogController | null = new CustomDialogController({
    builder: SimpleToggleLayoutEditDialogComponent({
      title: $r("app.string.control_center_simple_toggle_layout_edit_back_confirm"),
      leftButton: $r("app.string.control_center_simple_toggle_layout_edit_back_confirm_primary"),
      rightButton: $r("app.string.control_center_simple_toggle_layout_edit_back_confirm_secondary"),
      leftAction: (): void => this.callSimpleToggleLayoutEditEnd(),
      rightAction: (): void => this.onFinishBtnClick(),
    }),
    autoCancel: false,
    offset: { dx: 0, dy: 0 },
    customStyle: true,
    alignment: DialogAlignment.Center
  });
  private resetDialogController: CustomDialogController | null = new CustomDialogController({
    builder: SimpleToggleLayoutEditDialogComponent({
      title: $r("app.string.control_center_simple_toggle_layout_edit_reset_confirm"),
      leftButton: $r("app.string.control_center_simple_toggle_layout_edit_reset_confirm_primary"),
      rightButton: $r("app.string.control_center_simple_toggle_layout_edit_reset_confirm_secondary"),
      leftAction: () => {
      },
      rightAction: (): void => this.editResetConfirm(),
    }),
    autoCancel: false,
    offset: { dx: 0, dy: 0 },
    customStyle: true,
    alignment: DialogAlignment.Center
  });

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
    this.mNewDisplayingToggles = [...this.mDisplayingToggles];
    this.mNewHidingToggles = [...this.mHidingToggles];
    let allTogglesCount = this.mNewDisplayingToggles.length + this.mNewHidingToggles.length;
    this.mHidingTogglesMaxCount = allTogglesCount - this.mDisplayingTogglesMinCount;
    if (this.mHidingTogglesMaxCount < 0) {
      this.mHidingTogglesMaxCount = 0;
    };
    this.mHidingTogglesMinCount = allTogglesCount - this.mDisplayingTogglesMaxCount;
    if (this.mHidingTogglesMinCount < 0) {
      this.mHidingTogglesMinCount = 0;
    };
  }

  aboutToDisappear() {
    this.backDialogController = null
    this.resetDialogController = null

    Log.showInfo(TAG, 'aboutToDisappear ');
  }

  build() {
    Column() {
      if (!this.titleDisplayInside) {
        Column() {
          SimpleToggleLayoutEditUpTitle({
            simpleToggleLayoutEditEndCallback: (): void => this.onSimpleToggleLayoutEditEnd()
          })
        }
        .width('100%')
        .height(this.style.titleHeight)
        .margin({bottom: this.style.titleMarginBottom})
      }

      Column() {
        if (this.titleDisplayInside) {
          Column() {
            SimpleToggleLayoutEditUpTitle({
              simpleToggleLayoutEditEndCallback: (): void => this.onSimpleToggleLayoutEditEnd()
            })
          }
          .width('100%')
          .height(this.style.titleHeight)
        }
        Column() {
          SimpleToggleLayoutEditGrid({
            mToggles: $mNewDisplayingToggles,
            mMaxCount: this.mDisplayingTogglesMaxCount,
            mMinCount: this.mDisplayingTogglesMinCount,
            mColumnCount: this.mColumnCount,
            mGlobalDragToggleName: this.mCurrentDragToggleName,
            gridTag: 'displaying',
            onItemDragStart: (toggleName) => this.onDisplayingGridItemDragStart(toggleName),
            onItemDrop: (status) => this.onDisplayingGridItemDrop(status)
          })
        }
        .margin({top: this.style.upGridMarginTop, bottom: this.style.upGridMarginBottom, left: this.style.gridMarginLeft, right: this.style.gridMarginRight})

        Column() {
          Column() {
            SimpleToggleLayoutEditOptMsg({
              mOptMsg: $mOptMsg
            })
          }.margin({top: this.style.msgMarginTop})

          Column() {
            SimpleToggleLayoutEditGrid({
              mToggles: $mNewHidingToggles,
              mMaxCount: this.mHidingTogglesMaxCount,
              mMinCount: this.mHidingTogglesMinCount,
              mColumnCount: this.mColumnCount,
              mGlobalDragToggleName: this.mCurrentDragToggleName,
              gridTag: 'hiding',
              onItemDragStart: (toggleName) => this.onHidingGridItemDragStart(toggleName),
              onItemDrop: (status) => this.onHidingGridItemDrop(status)
            })
          }.margin({top: this.style.msgMarginBottom, bottom: this.style.btnMarginTop, left: this.style.gridMarginLeft, right: this.style.gridMarginRight})

          Grid() {
            GridItem() {
              Button({ type: ButtonType.Capsule, stateEffect: true }) {
                Text($r("app.string.control_center_simple_toggle_layout_edit_reset"))
                  .fontColor(this.style.editBtnFontColor)
                  .fontSize(this.style.editBtnFontSize)
                  .fontWeight(FontWeight.Medium)
              }
              .backgroundColor(this.style.editBtnBgColor)
              .width('100%')
              .height('100%')
              .onClick(this.onResetBtnClick.bind(this))
            }

            GridItem() {
              Button({ type: ButtonType.Capsule, stateEffect: true }) {
                Text($r("app.string.control_center_simple_toggle_layout_edit_finish"))
                  .fontColor(this.style.editBtnFontColor)
                  .fontSize(this.style.editBtnFontSize)
                  .fontWeight(FontWeight.Medium)
              }
              .backgroundColor(this.style.editBtnBgColor)
              .width('100%')
              .height('100%')
              .onClick(this.onFinishBtnClick.bind(this))
            }
          }
          .height(this.style.editBtnHeight)
          .margin({bottom: this.style.btnMarginBottom})
          .padding({left: this.style.editBtnMarginLeft, right: this.style.editBtnMarginRight})
          .columnsTemplate('1fr 1fr')
          .rowsTemplate('1fr')
          .columnsGap(this.style.editBtnSpace)
        }
        .width('100%')
        .backgroundColor(this.style.downAreaBgColor)

      }
      .margin({left: this.style.marginLeft, right: this.style.marginRight})
      .borderRadius(this.style.borderRadius)
      .backgroundColor(this.style.upAreaBgColor)
      .clip(true)

    }
    .width('100%')
    .margin({top: this.style.marginTop})
    .onTouch(this.onComponentTouch.bind(this))
  }

  onSimpleToggleLayoutEditEnd() {
    Log.showDebug(TAG, `onSimpleToggleLayoutEditEnd`);
    let changed = JSON.stringify(this.mDisplayingToggles) != JSON.stringify(this.mNewDisplayingToggles);
    Log.showDebug(TAG, `onSimpleToggleLayoutEditEnd, changed: ${changed}`);
    if (changed) {
      this.backDialogController?.open();
    } else {
      this.callSimpleToggleLayoutEditEnd();
    };
  }

  callSimpleToggleLayoutEditEnd() {
    Log.showDebug(TAG, `callSimpleToggleLayoutEditEnd`);
    if (this.simpleToggleLayoutEditEndCallback) {
      this.simpleToggleLayoutEditEndCallback();
    };
  }

  onResetBtnClick(event: ClickEvent) {
    Log.showDebug(TAG, `onResetBtnClick`);
    let equalDefault = JSON.stringify(this.mDefaultDisplayToggles) == JSON.stringify(this.mNewDisplayingToggles);
    Log.showDebug(TAG, `onResetBtnClick, equalDefault: ${equalDefault}`);
    if (!equalDefault) {
      this.resetDialogController?.open();
    };
  }

  editResetConfirm() {
    Log.showDebug(TAG, `editResetConfirm`);
    this.resetData();
    this.callSaveDisplayingToggles();
  }

  resetData(): void{
    Log.showDebug(TAG, `resetData`);
    let tempDisplayingToggles: string[] = [...this.mDefaultDisplayToggles];
    let tempHidingToggles: string[] = [];
    this.mDisplayingToggles.forEach((toggleName) => {
      if (tempDisplayingToggles.indexOf(toggleName) < 0) {
        tempHidingToggles.push(toggleName);
      };
    })
    this.mHidingToggles.forEach((toggleName) => {
      if (tempDisplayingToggles.indexOf(toggleName) < 0) {
        tempHidingToggles.push(toggleName);
      };
    })
    this.mNewDisplayingToggles = tempDisplayingToggles;
    this.mNewHidingToggles = tempHidingToggles;
    this.mDisplayingToggles = [...tempDisplayingToggles];
    this.mHidingToggles = [...tempHidingToggles];
    Log.showDebug(TAG, `resetData, mNewDisplayingToggles: ${JSON.stringify(this.mNewDisplayingToggles)}`);
    Log.showDebug(TAG, `resetData, mNewHidingToggles: ${JSON.stringify(this.mNewHidingToggles)}`);
  }

  onFinishBtnClick() {
    Log.showDebug(TAG, `onFinishBtnClick`);
    let changed = JSON.stringify(this.mDisplayingToggles) != JSON.stringify(this.mNewDisplayingToggles);
    Log.showDebug(TAG, `onFinishBtnClick, changed: ${changed}`);
    if (changed) {
      this.callSaveDisplayingToggles();
    }
    this.callSimpleToggleLayoutEditEnd();
  }

  callSaveDisplayingToggles(): void{
    Log.showDebug(TAG, `callSaveDisplayingToggles`);
    if (this.onSaveDisplayingToggles) {
      this.onSaveDisplayingToggles(this.mNewDisplayingToggles);
    };
  }

  async onDisplayingGridItemDragStart(toggleName: string): Promise<void> {
    Log.showDebug(TAG, `onDisplayingGridItemDragStart, toggleName: ${toggleName}`);
    this.mCurrentDragToggleName = toggleName;
    this.setOptMsg(toggleName);
  }

  async onDisplayingGridItemDrop(status: string): Promise<void> {
    Log.showDebug(TAG, `onDisplayingGridItemDrop, status: ${status}`);
    this.setOptMsg('');
  }

  async onHidingGridItemDragStart(toggleName: string): Promise<void> {
    Log.showDebug(TAG, `onHidingGridItemDragStart, toggleName: ${toggleName}`);
    this.mCurrentDragToggleName = toggleName;
    this.setOptMsg(toggleName);
  }

  async onHidingGridItemDrop(status: string): Promise<void> {
    Log.showDebug(TAG, `onHidingGridItemDrop, status: ${status}`);
    this.setOptMsg('');
  }

  setOptMsg(toggleName: string): void {
    Log.showInfo(TAG, `setOptMsg, toggleName: ${toggleName}, toggleName.length: ${toggleName.length}`);
    if (!toggleName || toggleName.length == 0) {
      this.mOptMsg = $r('app.string.control_center_simple_toggle_layout_edit_opt_desc');
      return;
    }
    if (this.mNewDisplayingToggles.indexOf(toggleName) >= 0) {
      if (this.mNewDisplayingToggles.length <= this.mDisplayingTogglesMinCount) {
        this.mOptMsg = $r('app.string.control_center_simple_toggle_layout_edit_opt_min', this.mDisplayingTogglesMinCount.toString());
      } else {
        this.mOptMsg = $r('app.string.control_center_simple_toggle_layout_edit_opt_drag');
      }
    } else if (this.mNewHidingToggles.indexOf(toggleName) >= 0) {
      if (this.mNewDisplayingToggles.length >= this.mDisplayingTogglesMaxCount) {
        this.mOptMsg = $r('app.string.control_center_simple_toggle_layout_edit_opt_max', this.mDisplayingTogglesMaxCount.toString());
      } else {
        this.mOptMsg = $r('app.string.control_center_simple_toggle_layout_edit_opt_drag');
      }
    };
  }

  onComponentTouch(event: TouchEvent) {
    Log.showDebug(TAG, `onComponentTouch`);
    event.stopPropagation();
  }
}

@Component
struct SimpleToggleLayoutEditUpTitle {
  private simpleToggleLayoutEditEndCallback: Function = () => {};
  @State style: SimpleToggleLayoutEditUpTitleStyle = StyleConfiguration.getSimpleToggleLayoutEditUpTitleStyle();

  aboutToAppear() {
    Log.showInfo(TAG_SimpleToggleLayoutEditUpTitle, 'aboutToAppear');
  }

  aboutToDisappear() {
    Log.showInfo(TAG_SimpleToggleLayoutEditUpTitle, 'aboutToDisappear ');
  }

  build() {
    Row() {
      Image($r('app.media.ic_public_back'))
        .margin({left: this.style.marginLeft, right: this.style.editTitleSpace})
        .objectFit(ImageFit.Contain)
        .size({ width: this.style.imageWidth,
          height: this.style.imageHeight })
        .fillColor(this.style.fontColor)
        .onClick(this.backClick.bind(this))
      Text($r('app.string.control_center_simple_toggle_layout_edit'))
        .fontColor(this.style.fontColor)
        .fontSize(this.style.titleFontSize)
        .fontWeight(FontWeight.Medium)
    }.width('100%')
    .height('100%')
  }

  backClick(event: ClickEvent) {
    Log.showDebug(TAG_SimpleToggleLayoutEditUpTitle, `backClick, event: ${JSON.stringify(event)}`);
    this.simpleToggleLayoutEditEndCallback();
  }
}

@Component
struct SimpleToggleLayoutEditOptMsg {
  @Link mOptMsg: Resource;
  @State style: SimpleToggleLayoutEditOptMsgStyle = StyleConfiguration.getSimpleToggleLayoutEditOptMsgStyle();

  aboutToAppear() {
    Log.showInfo(TAG_SimpleToggleLayoutEditOptMsg, 'aboutToAppear');
  }

  aboutToDisappear() {
    Log.showInfo(TAG_SimpleToggleLayoutEditOptMsg, 'aboutToDisappear ');
  }

  build() {
    Column() {
      Text(this.mOptMsg)
        .fontSize(this.style.fontSize)
        .fontColor(this.style.fontColor)
    }
    .height(this.style.height)
    .margin({left: this.style.marginLeftRight, right: this.style.marginLeftRight})
    .justifyContent(FlexAlign.Center)
  }
}