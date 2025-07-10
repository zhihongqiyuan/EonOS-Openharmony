/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

import {Log, sEventManager, unsubscribe} from '@ohos/common'
import ViewModel from '../../vm/accountsViewModel';
import Constants from '../../common/constants';
import {UserData} from '../../data/userData';
import {ACCOUNTS_REFRESH_EVENT} from '../../model/accountsModel'

const TAG = 'ScreenLock-Accounts'

@Component
export default struct Accounts {
  @State accountList: UserData[] = [];
  private mViewModel: ViewModel = new ViewModel();
  @StorageLink('screenlockdirection') screenlockdirection: number = 1
  unSubscriber?: unsubscribe;

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear`);
    this.unSubscriber = sEventManager.subscribe(ACCOUNTS_REFRESH_EVENT, (data: UserData[]) => {
      this.accountList = data;
    });
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`);
    this.unSubscriber && this.unSubscriber();
    this.unSubscriber = undefined;
  }

  build() {
    Row() {
      if (this.accountList.length > 1) {
        List({ space: this.screenlockdirection ==  1 ? Constants.ACCOUNT_SPACE : Constants.ACCOUNT_SPACE_PORTRAIT }) {
          ForEach(this.accountList, (item: UserData) => {
            ListItem() {
              UserItem({ userItem: item, viewModel: this.mViewModel })
            }
          })
        }
        .listDirection(Axis.Horizontal)
      }
    }
  }
}

@Component
struct UserItem {
  @State userItem: UserData | null = null;
  private viewModel: ViewModel | undefined = undefined;

  build() {
    Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center }) {
      if (this.userItem?.userIconPath.length == 0) {
        Image($r('app.media.ic_user_portrait'))
          .width(Constants.SHORTCUT_CIRCLE_WIDTH)
          .height(Constants.SHORTCUT_CIRCLE_WIDTH)
      } else {
        Image(this.userItem?.userIconPath)
          .width(Constants.SHORTCUT_CIRCLE_WIDTH)
          .height(Constants.SHORTCUT_CIRCLE_WIDTH)
      }
      Text(this.userItem?.userName)
        .fontSize(Constants.SHORTCUT_TEXT_SIZE)
        .fontColor($r('app.color.shortcut_text_color'))
        .margin({ top: $r("app.float.accounts_block") })
    }
    .onClick(event => {
        this.viewModel?.onUserClick(this.userItem?.userId);
    })
  }
}