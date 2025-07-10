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

import returnInfo from '../component/autoManager/returnInfo'
import termsListComponent from '../component/autoManager/termsListComponent'
import utils from '../../common/utils'

@Entry
@Component
struct TermsShowPage {
  @StorageLink('manageTermsName') manageTermsName: string = AppStorage.Get('manageTermsName') as string;
  @StorageLink('manageTermsContent') manageTermsContent: string[] = AppStorage.Get('manageTermsContent') as string[];

  build() {
    Column() {
      GridContainer({
        columns: utils.isLargeDevice(),
        sizeType: SizeType.Auto,
        gutter: '12vp',
        margin: '12vp'
      }) {
        Column() {
          returnInfo({ headName: $r('app.string.terms') })
        }
        .width('100%')
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
          termsListComponent({
            enterpriseCustomTerms: $manageTermsName,
            contentOfEnterpriseTerms: $manageTermsContent,
          })
        }
        .useSizeType({
          xs: { span: 8, offset: 0 }, sm: { span: 8, offset: 0 },
          md: { span: 8, offset: 0 }, lg: { span: 8, offset: 2 }
        })
        .width('100%')
        .height('100%')
      }
      .layoutWeight(1)
    }
    .backgroundColor(0xF1F3F5)
    .width('100%')
    .height('100%')
  }
}