/*
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

import web_webview from '@ohos.web.webview';
import baseData from '../../common/baseData'
import logger from '../../common/logger'
import returnInfo from '../component/autoManager/returnInfo'
import utils from '../../common/utils'

const TAG = 'UnitManagerShowPage';

@Entry
@Component
struct UnitManagerShowPage {
  @StorageLink('manageUrl') manageUrl: string = AppStorage.Get('manageUrl') as string;
  webviewController: web_webview.WebviewController = new web_webview.WebviewController();

  build() {
    Column() {
      GridContainer({
        columns: utils.isLargeDevice(),
        sizeType: SizeType.Auto,
        gutter: '12vp',
        margin: '12vp'
      }) {
        Column() {
          returnInfo({ headName: $r('app.string.manageInfo') })
        }
        .useSizeType({
          xs: { span: 8, offset: 0 }, sm: { span: 8, offset: 0 },
          md: { span: 8, offset: 0 }, lg: { span: 12, offset: 0 }
        })
      }

      GridContainer({
        columns: utils.isLargeDevice(),
        sizeType: SizeType.Auto,
        gutter: '12vp',
        margin: '12vp'
      }) {
        Column() {
          Web({ src: this.manageUrl, controller: this.webviewController })
            .cacheMode(CacheMode.Online)
            .fileAccess(false)
            .javaScriptAccess(false)
            .onErrorReceive((event?: ErrorReceiveEvent) => {
              logger.warn(TAG, 'error code=' + event?.error.getErrorCode() + ', error info=' +
              event?.error.getErrorInfo())
            })
            .onHttpErrorReceive((event?: HttpErrorReceiveEvent) => {
              logger.warn(TAG, 'response code=' + event?.response.getResponseCode() + ', response message=' +
              event?.response.getReasonMessage())
            })
            .onRenderExited((event?: RenderExitedEvent) => {
              logger.warn(TAG, 'renderExitReason=' + event?.renderExitReason);
            })
            .useSizeType({
              xs: { span: 4, offset: 2 }, sm: { span: 4, offset: 2 },
              md: { span: 4, offset: 2 }, lg: { span: 4, offset: 2 }
            })
        }
        .useSizeType({
          xs: { span: 8, offset: 0 }, sm: { span: 8, offset: 0 },
          md: { span: 8, offset: 0 }, lg: { span: 8, offset: 2 }
        })
      }
    }
    .backgroundColor(0xF1F3F5)
    .alignItems(HorizontalAlign.Center)
    .width('100%')
    .height('100%')
  }

  aboutToAppear() {
    logger.info(TAG, 'enter UnitManagerShowPage')
    if (!this.manageUrl.startsWith(baseData.HTTPS)) {
      this.manageUrl = '';
    }
  }
}

interface ErrorReceiveEvent {
  request: WebResourceRequest,
  error: WebResourceError
}

interface HttpErrorReceiveEvent {
  request: WebResourceRequest,
  response: WebResourceResponse
}

interface RenderExitedEvent {
  renderExitReason: RenderExitReason
}