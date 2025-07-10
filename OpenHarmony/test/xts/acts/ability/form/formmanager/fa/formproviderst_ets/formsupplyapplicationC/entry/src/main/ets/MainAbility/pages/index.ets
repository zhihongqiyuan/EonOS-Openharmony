/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

import featureAbility from "@ohos.ability.featureAbility";
import Want from '@ohos.app.ability.Want';
import formBindingData from "@ohos.application.formBindingData";
import formManager from '@ohos.application.formProvider';
import commonEvent from '@ohos.commonEvent';
import { BusinessError } from '@ohos.base';

interface FormDataType {
  temperature: string, 
  time: string
}

interface DataObjType {
  temperature: string,
  time: string,
  test: number,
  test3: boolean
};

interface ParametersType {
  formId : string
}

interface CommonEventPublishDataType {
  data: string,
  parameters: ParametersType
};

@Entry
@Component
struct Index {
  @State formId: string = "0";
  @State opt : string = "updateForm";
  @State formData : FormDataType= {temperature: "14c", time: "12:00"};
  @State nextTime: number = 5;

  private formUpdateFormEvent = "FMS_FormUpdateRefresh_commonEvent";
  private formOnDynamicRefreshEvent = "FMS_FormDynamicRefresh_commonEvent";

  private publishUpdateFormCallBack() {
    console.debug("====>form supply B publishUpdateFormCallBack ====>");
  }
  private formOnDynamicRefreshCallBack() {
    console.debug("====>formsystemhostc formOnDynamicRefreshEventk ====>");
  }

  aboutToAppear() {
    console.log(`form supply B aboutToAppear start`);
    console.error('form supply B getWant');
    featureAbility.getWant()
      .then((want: Want) => {
        this.formId = JSON.stringify(want!.parameters!.formId);
        this.opt = JSON.stringify(want!.parameters!.opt);
        if(want!.parameters!.formData) {
          this.formData = want!.parameters!.formData as FormDataType;
        }
        if(want!.parameters!.nextTime) {
          this.nextTime = Number(JSON.stringify(want!.parameters!.nextTime));
        }
        console.error('form supply B getWant end'+ JSON.stringify(want));

        if(this.opt == "updateForm") {
          console.log("form supply B updateForm start");
          let dataObj : DataObjType = {
            temperature: "14c",
            time: "12:00",
            test: 11,
            test3: true
          };
          let bindingData = formBindingData.createFormBindingData(JSON.stringify(dataObj));
          formManager.updateForm(this.formId, bindingData)
            .then((data) => {
              console.info('form supply B updateForm result:' + data);
              let commonEventPublishData : CommonEventPublishDataType= {
                data: JSON.stringify(data),
                parameters: {
                  formId : this.formId
                }
              };
              commonEvent.publish(this.formUpdateFormEvent, commonEventPublishData, this.publishUpdateFormCallBack);
            });

          console.log("form supply B deleteForm end");
        }

        if(this.opt == "setNextRefreshTime") {
          console.log("form supply B setNextRefreshTime start");
          formManager.setFormNextRefreshTime(this.formId, this.nextTime)
            .then((data) => {
              console.info('form supply B setNextRefreshTime result:' + data);
              let commonEventPublishData: CommonEventPublishDataType = {
                data: JSON.stringify(data),
                parameters: {
                  formId : this.formId
                } as ParametersType
              };
              commonEvent.publish(this.formOnDynamicRefreshEvent, commonEventPublishData, this.formOnDynamicRefreshCallBack);
            });

          console.log("form supply B setNextRefreshTime end");
        }

      })
      .catch((error: BusinessError) => {
        console.error('form supply B Operation failed. Cause: ' + JSON.stringify(error));
      })
    console.log(`form supply aboutToAppear end`);
  }
  build() {
    Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center }) {
      Text('Hello World')
        .fontSize(50)
        .fontWeight(FontWeight.Bold)
      Button() {
        Text('next page')
          .fontSize(25)
          .fontWeight(FontWeight.Bold)
      }.type(ButtonType.Capsule)
      .margin({
        top: 20
      })
      .backgroundColor('#0D9FFB')
    }
    .width('100%')
    .height('100%')
  }
}