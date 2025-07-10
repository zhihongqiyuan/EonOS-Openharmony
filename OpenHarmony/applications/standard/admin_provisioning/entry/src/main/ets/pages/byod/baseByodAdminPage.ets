/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

const TAG = 'BaseByodAdminPage';

@Component
export struct BaseByodAdminPage {
  @BuilderParam topBuilderParam: () => void = this.doNothingBuilder;
  @BuilderParam centerBuilderParam: () => void = this.doNothingBuilder;
  @BuilderParam bottomBuilderParam: () => void = this.doNothingBuilder;
  @Prop listTitle: Resource | string = '';

  @Builder
  doNothingBuilder() {
  };

  build() {
    Column() {
      Column() {
        Column() {
          this.topBuilderParam();
        }
        .width('100%')
        .height('56vp')
        .padding({
          top: '14.5vp',
          bottom: '14.5vp'
        })
        .alignItems(HorizontalAlign.Start)

        Column() {
          List() {
            ListItem() {
              Text(this.listTitle)
                .fontColor($r('sys.color.ohos_id_color_text_primary'))
                .fontWeight(FontWeight.Bold)
                .lineHeight('24vp')
                .fontSize('18vp')
            }
            .padding({
              top: '24vp',
              bottom: '8vp'
            })
            .margin({
              bottom: '8vp'
            })

            this.centerBuilderParam();
          }
          .width('100%')
          .height('100%')
        }
        .height('80%')
      }

      Column() {
        this.bottomBuilderParam();
      }
      .margin({
        bottom: '16vp'
      })
    }
    .width('100%')
    .height('100%')
    .justifyContent(FlexAlign.SpaceBetween)
    .padding({
      left: '16vp',
      right: '16vp'
    })
  }
}

@Preview
@Component
export struct BottomButton {
  @Require @Prop description: string | Resource = '';
  @Require buttonWidth: string = '';
  @Prop isEnabled: boolean = true;

  build() {
    Column() {
      Button() {
        Text(this.description)
          .fontSize('16vp')
          .lineHeight('21vp')
          .fontFamily('HarmonyHeiTi')
          .fontWeight(FontWeight.Medium)
          .fontColor($r('sys.color.font_emphasize'))
      }
      .backgroundColor($r('sys.color.comp_background_tertiary'))
      .width(this.buttonWidth)
      .height('40vp')
      .enabled(this.isEnabled)
    }
  }
}
