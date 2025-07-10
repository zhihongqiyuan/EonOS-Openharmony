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

import commonEvent from '@ohos.commonEvent';
import featureAbility from "@ohos.ability.featureAbility";
import formHost from '@ohos.application.formHost';
import CommonEventManager from '@ohos.commonEventManager';
import { BusinessError } from '@ohos.base';
import Want from '@ohos.application.Want';

interface InfoType {id:number}
interface ErroType {errcode: number, msg: string}

@Entry
@Component
struct Index {
  @State formId: number = 0;
  @State bundle: string = "com.acts.form.formsystemtestservicea.hmservice";
  @State ability: string = "com.acts.form.formsystemtestservicea.hmservice.MainAbility";
  @State moduleName: string = "entry";
  @State name: string = "Form_Js001";
  @State allowUpate: boolean = true;
  @State isShowing: boolean = true;
  @State canCreateForm: boolean = false;

  private dimension: FormDimension = FormDimension.Dimension_1_2;

  private TAG = `[FormComponent.host]`;

  private deleteForm = false;
  private deleteId = "-1";
  private temporaryId="-1";
  private temporary = false;
  private castForm = false;

  private formOnAcquiredEvent = "FMS_FormOnAcquired_commonEvent";
  private formCastEvent = "FMS_FormCast_commonEvent";
  private formOnErrorEvent = "FMS_FormOnError_commonEvent";

  private subscriberFormDeleteEvent:CommonEventManager.CommonEventSubscribeInfo = {
    events: ["FMS_FormDelete_commonEvent"],
  };
  private subscriberDle:CommonEventManager.CommonEventSubscriber|null = null;

  private publishOnErrorCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} formOnErrorEvent Publish CallBack ====>`);
  }
  private publishOnAcquiredCallBack = () => {
    console.debug(`====>${this.TAG} formOnAcquiredEvent Publish CallBack ====>`);
  }
  private publishCastCallBack = () => {
    console.debug(`====>${this.TAG} formCastEvent Publish CallBack ====>`);
  }
  private deleteCallBack = (err:BusinessError, data:CommonEventManager.CommonEventData) => {
    console.info(`!!!====>${this.TAG} formsystemhost deleteCallBack start:====>` + JSON.stringify(data));
    if(data.bundleName && data.bundleName != "com.ohos.st.formsystemhostb") {
      return;
    }
    formHost.deleteForm(data.data)
      .then((data2) => {
        console.info(`${this.TAG} deleteForm result:` + data2);
      });

    this.canCreateForm = false;
    console.info(`!!!====>${this.TAG} formsystemhost deleteCallBack end ====>`);
  }

  aboutToAppear() {
    console.log(`${this.TAG} aboutToAppear start`);
    commonEvent.createSubscriber(this.subscriberFormDeleteEvent).then(
      async (data:CommonEventManager.CommonEventSubscriber) => {
        console.info(`====>${this.TAG} formsystemhost Subscriber FormDelete data:====>`, JSON.stringify(data)); 
        this.subscriberDle = data;     
        await commonEvent.subscribe(this.subscriberDle, this.deleteCallBack);
    })
    console.info(`${this.TAG} aboutToAppear:getWant start`);
    featureAbility.getWant()
    .then((want: Want) => {
      this.formId = parseInt(want.parameters?.formId ?? 0);
      this.name = want.parameters?.name ?? "";
      this.bundle =  want.parameters?.bundle ?? "";
      this.ability = want.parameters?.ability ?? "";
      if(want.parameters) {
        this.moduleName = want.parameters.moduleName;
      }
      if(want.parameters) {
        this.temporary = want.parameters.temporary;
      }
      if(want.parameters) {
        this.dimension = want.parameters.dimension;
      }
      if(!this.dimension) {
        this.dimension = FormDimension.Dimension_1_2;
      }
      if(want.parameters) {
        this.castForm = want.parameters.castForm;
      }
      if(want.parameters) {
        this.temporaryId = want.parameters.temporaryId;
      }
      if(want.parameters) {
        this.deleteForm = want.parameters.deleteForm;
      }
      if(want.parameters) {
        this.deleteId = want.parameters.deleteId;
      }
      setTimeout(() => {
        this.canCreateForm = want.parameters?.isCreate ?? false;
        console.info(`${this.TAG} aboutToAppear:getWant canCreateForm:`+ this.canCreateForm); 
      }, 10); 
      console.info(`${this.TAG} aboutToAppear:getWant end`+ JSON.stringify(want));

      if(!(want.parameters?.isCreate ?? false) && this.deleteForm) {
        console.log(`${this.TAG} aboutToAppear:formsystemhostb deleteForm start`);
        formHost.deleteForm(this.deleteId)
          .then(() => {
          console.info(`${this.TAG} formsystemhostb deleteForm result:`);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: "",
            parameters: {
              "formId" : this.deleteId.toString()
            }
          };
          commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishOnErrorCallBack);
        })
        .catch((error:BusinessError) => {
          console.info(`${this.TAG} deleteForm error:` + error);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: error.code.toString(),
            parameters: {
              "formId" : this.deleteId.toString()
            }
          };
          commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishOnErrorCallBack);
        });
      
        console.log(`${this.TAG} formsystemhostb deleteForm end`);
      }
    })
    .catch((error: BusinessError) => {
      console.error(`${this.TAG} Operation failed. Cause: ` + JSON.stringify(error));
    })
    console.log(`${this.TAG} aboutToAppear end`);
  }

  build() {
      Column() {
        Text('form component test begin')
        Column() {
          if (this.canCreateForm) {
            FormComponent({
            id: this.formId,
            name: this.name,
            bundle: this.bundle,
            ability: this.ability,
            module: this.moduleName,
            dimension: this.dimension,
            temporary: this.temporary,
          })
            .allowUpdate(this.allowUpate)
            .visibility(this.isShowing ? Visibility.Visible : Visibility.Hidden)
            .onAcquired((form:InfoType) => {
              console.log(`${this.TAG} onAcquired start`);
              console.log(`${this.TAG} get form, form id:` + form.id);
              this.formId = form.id;
              if(this.castForm) {
                console.log(`${this.TAG} onAcquired:castTempForm start`);
                formHost.castTempForm(this.temporaryId)
                    .then(() => {
                    console.info(`${this.TAG} castTempForm result:`);
                     setTimeout(() => {
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "",
                        parameters: {
                          "formId" : this.formId.toString()
                        }
                      };
                      commonEvent.publish(this.formCastEvent, commonEventPublishData, this.publishCastCallBack);
                    }, 5);
                })
                .catch((error:BusinessError) => {
                    console.info(`${this.TAG} castTempForm error:` + error);
                      setTimeout(() => {
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: error.code.toString(),
                        parameters: {
                          "formId" : this.temporaryId.toString()
                        }
                      };
                      commonEvent.publish(this.formCastEvent, commonEventPublishData, this.publishCastCallBack);
                    }, 5);
                  });
               
                console.log(`${this.TAG} onAcquired:castTempForm end`);
              } else if(this.deleteForm) {
                console.log(`${this.TAG} onAcquired:deleteForm start`);
                if(this.deleteId == "self") {
                  this.deleteId = this.formId.toString();
                }
                setTimeout(() => {
                  formHost.deleteForm(this.deleteId)
                    .then((data) => {
                    console.info(`${this.TAG} deleteForm result:` + data);
                    let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                      data: data + "",
                      parameters: {
                        "formId" : this.deleteId.toString()
                      }
                    };
                    commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishOnErrorCallBack);
                  })
                  .catch((error:BusinessError) => {
                    console.info(`${this.TAG} deleteForm error:` + error);
                    let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                      data: error.code.toString(),
                      parameters: {
                        "formId" : this.deleteId.toString()
                      }
                    };
                    commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishOnErrorCallBack);
                  });               
                  console.log(`${this.TAG} onAcquired:deleteForm end`);
                }, 1000);                
              } else {
                console.log(`${this.TAG} onAcquired:else start`);
                let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: this.formId.toString()
                };
                setTimeout(()=>{
                  console.log(`${this.TAG} onAcquired:publish formOnAcquiredEvent:` + this.formOnAcquiredEvent);
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, this.publishOnAcquiredCallBack);
                }, 500);
                console.log(`${this.TAG} onAcquired:else end`);
              }
              console.log(`${this.TAG} onAcquired end`);
            })
            // .onUninstall((info) => {
            //   console.log(`${this.TAG} onUninstall:` + JSON.stringify(info));
            // })
            .onError((error:ErroType) => {
              console.log(`${this.TAG} error code:` + error.errcode);
              console.log(`${this.TAG} error msg:` + error.msg);
              if(this.deleteForm && this.deleteId) {
                let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                  data: error.msg,
                  parameters: {
                    "formId" : this.deleteId.toString()
                  }
                };
                commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishOnErrorCallBack);
              } else {
                let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: error.msg
                };
                commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishOnErrorCallBack);
                }
            })
          }
        }
        .backgroundColor(Color.White)
        Text('form component test end')
      }
      .backgroundColor(Color.White)
  }
}
