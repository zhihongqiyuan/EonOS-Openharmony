/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import logger from '../../common/logger'

const TAG: string = 'PageComponent';

@Component
export struct PageComponent {
  @BuilderParam content: () => void;
  onPageShow?: () => void;
  onPageHide?: () => void;

  build() {
    Stack() {
      Column() {
        if (this.content) {
          this.content();
        }
      }
    }
    .backgroundColor($r('sys.color.ohos_id_color_card_bg'))
  }

  aboutToAppear(): void {
    logger.info(TAG, 'aboutToAppear')
  }

  aboutToDisappear(): void {
    logger.info(TAG, 'aboutToDisappear')
  }
}