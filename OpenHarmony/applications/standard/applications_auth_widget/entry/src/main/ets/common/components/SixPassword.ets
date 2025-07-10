/**
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

import AuthUtils from '../utils/AuthUtils';
import Constants, { WantParams } from '../vm/Constants';

const MAX_LENGTH = 6;

@Component
export default struct SixPassword {
  @Link inputValue: string;
  @Link isEdit: boolean;
  @Link @Watch('onTextValueChange') textValue: string;
  @StorageLink('passwordArrayNumber') passwordArrayNumber: string[] = [];

  onTextValueChange(): void {
    this.passwordArrayNumber = this.textValue.replace(new RegExp('[^\\d]', 'g'), '').split('');
  }

  clearPassword(): void {
    this.passwordArrayNumber = [];
    this.textValue = '';
    this.inputValue = ' ';
  }

  aboutToDisappear(): void {
    this.clearPassword();
  }

  build() {
    Column() {
      if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
        Scroll() {
          Column() {
            Text((AppStorage.get('wantParams') as WantParams)?.title)
	            .draggable(false)
              .fontSize($r('sys.float.ohos_id_text_size_body1'))
              .fontColor($r('sys.color.ohos_id_color_text_primary'))
              .fontWeight(FontWeight.Medium)

            Column() {
              Stack() {
                List({ space: 4 }) {
                  ForEach(['', '', '', '', '', ''], (item: string, index?: number) => {
                    if (index !== undefined) {
                      ListItem() {
                        Button()
                          .border({
                            color: $r('sys.color.ohos_id_color_primary'),
                            style: BorderStyle.Solid,
                            width: this.textValue.length !== 0 && index < this.textValue.length
                              ? $r('app.float.button_border_width') : 1
                          })
                          .type(ButtonType.Circle)
                          .backgroundColor(Color.White)
                          .width($r('app.float.input_btn_size'))
                          .height($r('app.float.input_btn_size'))
                          .borderRadius($r('app.float.input_btn_size'))
                          .margin(index > 0 ?
                            {
                              left: $r('app.float.input_btn_padding_around'),
                              top: $r('app.float.margin_12')
                            } : { top: $r('app.float.margin_12') })
                          .focusable(true)
                      }
                    }
                  }, (item: string) => item)
                }
                .listDirection(Axis.Horizontal)
                .height($r('app.float.input_height'))

                TextInput({ placeholder: '', text: this.passwordArrayNumber?.join('') })
                  .draggable(false)
                  .onChange(async (value: string) => {
                    this.passwordArrayNumber = value.replace(new RegExp('[^\\d]', 'g'), '').split('');
                    this.textValue = this.passwordArrayNumber?.join('');
                    if (this.textValue?.length === MAX_LENGTH) {
                      AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                    }
                  })
                  .backgroundImageSize(ImageSize.Auto)
                  .enabled(this.isEdit)
                  .maxLength(MAX_LENGTH)
                  .visibility(Visibility.Visible)
                  .opacity(0)
                  .caretColor('transparent')
                  .id('pinSix')
              }
            }
            .margin({ top: $r('app.float.content_padding') })

            Text(this.inputValue)
              .draggable(false)
              .fontSize($r('sys.float.ohos_id_text_size_body2'))
              .margin({ top: $r('app.float.element_margin') })
              .fontColor($r('sys.color.ohos_id_color_warning'))
          }
          .margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
        }
        .width('100%')
        .height($r('app.float.scroll_height_88'))
        .margin({ top: $r('app.float.title_padding_top') })
        .scrollable(ScrollDirection.Vertical)
        .scrollBarColor(Color.Gray)
      } else {
        Text((AppStorage.get('wantParams') as WantParams)?.title)
          .draggable(false)
          .margin({ top: $r('app.float.title_padding_top') })
          .fontSize($r('sys.float.ohos_id_text_size_body1'))
          .fontColor($r('sys.color.ohos_id_color_text_primary'))
          .height($r('app.float.size_24'))
          .fontWeight(FontWeight.Medium)
        Column() {
          Stack() {
            List({ space: 4 }) {
              ForEach(['', '', '', '', '', ''], (item: string, index?: number) => {
                if (index !== undefined) {
                  ListItem() {
                    Button()
                      .border({
                        color: $r('sys.color.ohos_id_color_primary'),
                        style: BorderStyle.Solid,
                        width: this.textValue.length !== 0 && index < this.textValue.length
                          ? $r('app.float.button_border_width') : 1
                      })
                      .type(ButtonType.Circle)
                      .backgroundColor(Color.White)
                      .width($r('app.float.input_btn_size'))
                      .height($r('app.float.input_btn_size'))
                      .borderRadius($r('app.float.input_btn_size'))
                      .margin(index > 0 ?
                        {
                          left: $r('app.float.input_btn_padding_around'),
                          top: $r('app.float.margin_12')
                        } : { top: $r('app.float.margin_12') })
                      .focusable(true)
                  }
                }
              }, (item: string) => item)
            }
            .listDirection(Axis.Horizontal)
            .height($r('app.float.input_height'))

            TextInput({ placeholder: '', text: this.passwordArrayNumber?.join('') })
              .draggable(false)
              .onChange(async (value: string) => {
                this.passwordArrayNumber = value.replace(new RegExp('[^\\d]', 'g'), '').split('');
                this.textValue = this.passwordArrayNumber?.join('');
                if (this.textValue?.length === MAX_LENGTH) {
                  AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                }
              })
              .backgroundImageSize(ImageSize.Auto)
              .enabled(this.isEdit)
              .maxLength(MAX_LENGTH)
              .visibility(Visibility.Visible)
              .opacity(0)
              .caretColor('transparent')
              .id('pinSix')
          }
        }
        .margin({ top: $r('app.float.content_padding') })

        Text(this.inputValue)
          .draggable(false)
          .fontSize($r('sys.float.ohos_id_text_size_body2'))
          .margin({ top: $r('app.float.element_margin') })
          .fontColor($r('sys.color.ohos_id_color_warning'))
      }

    }
  }
}