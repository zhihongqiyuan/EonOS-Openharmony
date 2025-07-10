/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

import { StartTestTitleComponent } from '../common/ui/StartTestTitleComponent';
import { questionList, QuestionItem } from '../common/entity/LocalConfigEntity';

@Entry
@Component
struct Question {
  build() {
    Column() {
      StartTestTitleComponent({ title: '常见问题' })
      QuestionComponent()
    }
  }
}


@Component
struct QuestionComponent {
  @State questionList: Array<QuestionItem> = questionList

  build() {
    Column() {
      List() {
        ForEach(this.questionList, (questionItem) => {
          ListItem() {
            Flex({
              direction: FlexDirection.Column,
              alignItems: ItemAlign.Start,
              justifyContent: FlexAlign.SpaceBetween
            }) {
              Text(questionItem.question)
                .fontSize(18)
                .fontColor('#333333')
                .margin({ top: 10, left: 15, right: 15, bottom: 5 })
              Text(questionItem.answer)
                .fontSize(15).fontColor('#666666')
                .margin({ left: 18, right: 18, bottom: 10 })
            }.width('100%')
            //            .borderWidth(1).borderRadius(5)
            .margin({ bottom: 2, top: 2 })
            .shadow({ radius: 10, color: Color.Gray, offsetX: 10, offsetY: 5 })
          }
        }, questionItem => questionItem.question)
      }.edgeEffect(EdgeEffect.None) // 滑动到边缘无效果
      .chainAnimation(false) // 联动特效关闭
    }.width('95%').height('88%')
  }
}