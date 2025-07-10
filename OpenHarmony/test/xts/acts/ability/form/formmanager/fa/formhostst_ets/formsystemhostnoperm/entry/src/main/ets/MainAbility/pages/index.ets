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

  private TAG = `[FormComponent.host.noperm]`;

  private stateForm = "visible";
  private stateIds = [];
  private requestForm = false;
  private requestId = "-1";
  private releaseForm = false;
  private releaseId = "-1";
  private deleteForm = false;
  private deleteId = "-1";
  private temporary = false;
  private castForm = false;

  private formOnErrorEvent = "FMS_FormOnError_commonEvent";
  private formOnDeletedEvent = "FMS_FormOnDeleted_commonEvent";
  private formOnReleasedEvent = "FMS_FormOnReleased_commonEvent";
  private formOnRequestEvent = "FMS_FormOnRequest_commonEvent";
  private formOnStateEvent = "FMS_FormOnState_commonEvent";
  
  private publishCallBack = () => {
    console.debug(`====>${this.TAG} formOnErrorEvent Publish CallBack ====>`);
  }

  aboutToAppear() {
    console.error(`${this.TAG} getWant`);
    featureAbility.getWant()
    .then((want: Want) => {
      this.formId = parseInt(want.parameters?.formId ?? 0);
      this.name = want.parameters?.name ?? "";
      this.bundle = want.parameters?.bundle ?? "";
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
      if(want.parameters) {
        this.castForm = want.parameters.castForm;
      }
      if(want.parameters) {
        this.deleteForm = want.parameters.deleteForm;
      }
      if(want.parameters) {
        this.deleteId = want.parameters.deleteId;
      }
      if(want.parameters) {
        this.releaseForm = want.parameters.releaseForm;
      }
      if(want.parameters) {
        this.releaseId = want.parameters.releaseId;
      }
      if(want.parameters) {
        this.requestForm = want.parameters.requestForm;
      }
      if(want.parameters) {
        this.requestId = want.parameters.requestId;
      }
      if(want.parameters) {
        this.stateForm = want.parameters.stateForm;
      }
      if(want.parameters) {
        this.stateIds = want.parameters.stateIds;
      }

      setTimeout(() => {
        this.canCreateForm = want.parameters?.isCreate ?? false;
        console.error(`${this.TAG} getWant canCreateForm:` + this.canCreateForm); 
      }, 10); 
      console.error(`${this.TAG} getWant end` + JSON.stringify(want));

      if(!(want.parameters?.isCreate ?? false) && this.deleteForm) {
        console.log(`${this.TAG} formsystemhostnoperm deleteForm start`);
        formHost.deleteForm(this.deleteId)
            .then((data) => {
            console.info(`${this.TAG} formsystemhostnoperm deleteForm result:` + data);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: data + "",
              parameters: {
                "formId" : this.deleteId.toString()
              }
            };
            commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishCallBack);
          })
          .catch((error:BusinessError) => {
            console.info(`${this.TAG} formsystemhostnoperm deleteForm error:` + error);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: error.code.toString(),
              parameters: {
                "formId" : this.deleteId.toString()
              }
            };
            commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishCallBack);
        });
        
        console.log(`${this.TAG} formsystemhostnoperm deleteForm end`);
      }

      if(!(want.parameters?.isCreate ?? false) && this.releaseForm) {
        console.log(`${this.TAG} formsystemhost releaseForm start`);
        formHost.releaseForm(this.releaseId)
          .then((data) => {
          console.info(`${this.TAG} formsystemhost releaseForm result:` + data);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: data + "",
              parameters: {
                "formId" : this.requestId.toString()
              }
          };
          commonEvent.publish(this.formOnReleasedEvent, commonEventPublishData, () => this.publishCallBack());
        })
        .catch((error:BusinessError)  => {
          console.info(`${this.TAG} formsystemhost releaseForm error:` + error);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: error.code.toString(),
            parameters: {
              "formId" : this.releaseId.toString()
            }
          };
          commonEvent.publish(this.formOnReleasedEvent, commonEventPublishData, this.publishCallBack);
        });
      
        console.log(`${this.TAG} formsystemhost releaseForm end`);
      }

      if(!(want.parameters?.isCreate ?? false) && this.requestForm) {
        console.log(`${this.TAG} requestForm start`);
        formHost.requestForm(this.requestId)
            .then((data) => {
            console.info(`${this.TAG} requestForm result:` + data);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: data + "",
              parameters: {
                "formId" : this.requestId
              }
            };
            commonEvent.publish(this.formOnRequestEvent, commonEventPublishData, () => this.publishCallBack());
          })
          .catch((error:BusinessError) => {
            console.info(`${this.TAG} requestForm error:` + error);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: error.code.toString(),
              parameters: {
                "formId" : this.requestId
              }
            };
            commonEvent.publish(this.formOnRequestEvent, commonEventPublishData, this.publishCallBack);
          });
        
          console.log(`${this.TAG} requestForm end`);              
      }
      if(!(want.parameters?.isCreate ?? false) && this.stateForm) {        
        if(this.stateForm == "visible") {
          console.log(`${this.TAG} formsystemhost notifyVisibleForm start`);
          formHost.notifyVisibleForms(this.stateIds)
            .then((data) => {
            console.info(`${this.TAG} formsystemhost notifyVisibleForm result:` + data);
            let retData = "0";
            if((data + "") != "0") {
              retData = data + "";
            }
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: retData,
              parameters: {
                "formId" : this.stateIds.toString()
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishCallBack);
          })
          .catch((error:BusinessError) => {
            console.info(`${this.TAG} formsystemhost notifyVisibleForm error:` + error);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: error.code.toString(),
              parameters: {
                "formId" : this.stateIds.toString()
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishCallBack);
          });        
          console.log(`${this.TAG} formsystemhost notifyVisibleForm end`);
        } else if(this.stateForm == "invisible") {
          console.log(`${this.TAG} formsystemhost notifyInvisibleForm start`);
          formHost.notifyInvisibleForms(this.stateIds)
            .then((data) => {
            console.info(`${this.TAG} formsystemhost notifyInvisibleForm result:` + data);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: data + "",
              parameters: {
                "formId" : this.stateIds.toString()
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, () => this.publishCallBack());
          })
          .catch((error:BusinessError) => {
            console.info(`${this.TAG} formsystemhost notifyInvisibleForm error:` + error);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: error.code.toString(),
              parameters: {
                "formId" : this.stateIds.toString()
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishCallBack);
          });
          console.log(`${this.TAG} formsystemhost notifyInvisibleForm end`);
        } else if(this.stateForm == "enable") {
          console.log(`${this.TAG} formsystemhost enableFormsUpdate start`);
          formHost.enableFormsUpdate(this.stateIds)
            .then((data) => {
            console.info(`${this.TAG} formsystemhost enableFormsUpdate result:` + data);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: data + "",
              parameters: {
                "formId" : this.stateIds.toString()
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, () => this.publishCallBack());
          })
          .catch((error:BusinessError) => {
            console.info(`${this.TAG} formsystemhost enableFormsUpdate error:` + error);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: error.code.toString(),
              parameters: {
                "formId" : this.stateIds.toString()
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishCallBack);
          });        
          console.log(`${this.TAG} formsystemhost enableFormsUpdate end`);
        } else {
          console.log(`${this.TAG} formsystemhost disableFormsUpdate start`);
          formHost.disableFormsUpdate(this.stateIds)
            .then((data) => {
            console.info(`${this.TAG} formsystemhost disableFormsUpdate result:` + data);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: data + "",
              parameters: {
                "formId" : this.stateIds.toString()
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, () => this.publishCallBack());
          })
          .catch((error:BusinessError) => {
            console.info(`${this.TAG} formsystemhost disableFormsUpdate error:` + error);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: error.code.toString(),
              parameters: {
                "formId" : this.stateIds.toString()
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishCallBack);
          });        
          console.log(`${this.TAG} formsystemhost disableFormsUpdate end`);
        }
      }

    })
    .catch((error:BusinessError) => {
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
              this.formId = form.id;
              console.log(`${this.TAG} get form, form id:` + form.id);              
            })
            .onError((error) => {
              console.log(`${this.TAG} error code:` + error.errcode);
              console.log(`${this.TAG} error msg:` + error.msg);
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: error.msg                 
              };
              commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishCallBack);
            })
          }
        }
        .backgroundColor(Color.White)
        Text('form component test end')
      }
      .backgroundColor(Color.White)
  }
}
