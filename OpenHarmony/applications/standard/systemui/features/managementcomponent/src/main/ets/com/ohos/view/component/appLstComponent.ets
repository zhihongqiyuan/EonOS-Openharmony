/**
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
import ConfigData from '../../common/constants';
import AppItemComponent from './appItemComponent';
import Router from '@system.router';
import Log from '../../../../../../../../../common/src/main/ets/default/Log';
import ViewModel from '../../vm/appLstComponentViewModel';
import { BundleItemData }from '../../model/bundleResourceModel';

const TAG = 'ManagementComponent-AppLstComponent';

@Component
export default struct AppLstComponent {
  @StorageLink('appManagementList') appList: BundleItemData[]= [];

  aboutToAppear(): void{
    Log.showInfo(TAG, `aboutToAppear`);
    Log.showWarn(TAG, `myTest`);
    ViewModel.viewModelInit();
  }

  aboutToDisappear(): void{
    Log.showInfo(TAG, `aboutToDisappear`);
  }

  onPageShow(): void{
    Log.showInfo(TAG, `onPageShow`);
  }

  onBackPress() {
    Log.showInfo(TAG, `onBackPress`);
  }

  build() {
    List() {
      ForEach(this.appList, (item: BundleItemData) => {
        ListItem() {
          AppItemComponent({
            appIcon: item.appIcon,
            appTitle: item.appTitle,
            appSummary: item.appSummary,
            appValue: item.appValue,
            appArrow: item.appArrow as string,
            appArrowStyle: '',
            appUri: item.appUri,
            appBundleName: item.appBundleName,
            appUid: item.appUid,
            appSwitch: 0
          })
        }
        .onClick(() => {
          Log.showInfo(TAG, `onClick`);
          Router.push({ uri: item.appUri, params: item });
        })
        .height($r('app.float.appitem_item_height'))
      });
    }
    .divider({
      strokeWidth: 1,
      color: $r('app.color.divider_color'),
      startMargin: $r('app.float.divider_margin_l_info')
    })
    .width(ConfigData.WH_100_100)
    .align(Alignment.Top)
    .visibility(Visibility.Visible)
    .flexShrink(1)
    .zIndex(0)
    .border({ width: $r('app.float.border_width'), color: Color.White,
      radius: $r('app.float.border_radius') })
    .backgroundColor(Color.White)
    .clip(true)
    .padding({
      left: $r('app.float.itemComp_padding_l'),
      right: $r('app.float.itemComp_padding_r')
    })
  }
}