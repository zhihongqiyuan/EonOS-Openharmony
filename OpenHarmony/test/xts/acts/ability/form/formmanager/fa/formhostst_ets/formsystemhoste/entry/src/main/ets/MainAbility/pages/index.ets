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
import featureAbility from '@ohos.ability.featureAbility';
import formHost from '@ohos.application.formHost';
import formProvider from '@ohos.application.formProvider';
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
  @State ability: string = "MainAbility";
  @State moduleName: string = "entry";
  @State name: string = "Form_Js001";
  @State allowUpate: boolean = true;
  @State isShowing: boolean = true;
  @State canCreateForm: boolean = false;

  private dimension: FormDimension = FormDimension.Dimension_1_2;

  private TAG = `[FormComponent.host.e]`;

  private requestForm = false;
  private requestId = "-1";
  private refreshForm = false;
  private refreshId = "-1";
  private refreshTime = 5;
  private notifyVisibleForms = false;
  private notifyVisibleId = "-1";
  private deleteForm = false;
  private deleteId = "-1";
  private temporaryId = "-1";
  private temporary = false;

  private formOnAcquiredEvent = "FMS_FormOnAcquired_commonEvent";
  private formOnErrorEvent = "FMS_FormOnError_commonEvent";
  private formOnDeletedEvent = "FMS_FormOnDeleted_commonEvent";
  private formOnDynamicRefreshEvent = "FMS_FormDynamicRefresh_commonEvent";
  private formOnRequestEvent = "FMS_FormOnRequest_commonEvent";

  private subscriberDel:CommonEventManager.CommonEventSubscriber|null = null;
  private subscriberFormDeleteEvent:CommonEventManager.CommonEventSubscribeInfo = {
    events: ["FMS_FormDelete_commonEvent"],
  };

  private publishOnErrorCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} formsystemhostc formOnErrorEventCallBack ====>`);
  }
  private publishOnDeletedCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} formsystemhostc publishOnDeletedCallBack ====>`);
  }
  private formOnDynamicRefreshCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} formsystemhostc formOnDynamicRefreshEventk ====>`);
  }
  private formOnRequestCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} formsystemhostc formOnRequestCallBack ====>`);
  }
  private publishOnAcquiredCallBack = () => {
    console.debug(`====>${this.TAG} formsystemhostc formOnAcquiredEventCallBack ====>`);
  }

  private deleteCallBack = (err:BusinessError, data:CommonEventManager.CommonEventData) => {
    console.info(`!!!====>${this.TAG} formsystemhost deleteCallBack start:====>` + JSON.stringify(data));
    if (data.bundleName && data.bundleName != "com.ohos.st.formsystemhost") {
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
    commonEvent.createSubscriber(this.subscriberFormDeleteEvent).then(async (data) => {
      console.info(`====>${this.TAG} formsystemhost Subscriber FormDelete data:====>`, JSON.stringify(data));
      this.subscriberDel = data;
      await commonEvent.subscribe(this.subscriberDel, this.deleteCallBack);
    })

    console.info(`${this.TAG} aboutToAppear:getWant start`);
    featureAbility.getWant()
      .then((want: Want) => {
        this.formId = parseInt(want.parameters?.formId ?? 0);
        this.name = want.parameters?.name ?? '';
        this.bundle = want.parameters?.bundle ?? '';
        this.ability = want.parameters?.ability ?? '';
        if (want.parameters) {
          this.moduleName = want.parameters.moduleName;
        }
        if (want.parameters) {
          this.temporary = want.parameters.temporary;
        }
        if (want.parameters) {
          this.dimension = want.parameters.dimension;
        }
        if (!this.dimension) {
          this.dimension = FormDimension.Dimension_1_2;
        }
        if (want.parameters) {
          this.temporaryId = want.parameters.temporaryId;
        }
        if (want.parameters) {
          this.deleteForm = want.parameters.deleteForm;
        }
        if (want.parameters) {
          this.deleteId = want.parameters.deleteId;
        }
        if (want.parameters) {
          this.refreshForm = want.parameters.refreshForm;
        }
        if (want.parameters) {
          this.refreshId = want.parameters.refreshId;
        }
        if (want.parameters) {
          this.refreshTime = want.parameters.refreshTime;
        }
        if (want.parameters) {
          this.requestForm = want.parameters.requestForm;
        }
        if (want.parameters) {
          this.requestId = want.parameters.requestId;
        }
        if (want.parameters) {
          this.notifyVisibleForms = want.parameters.notifyVisibleForms;
        }
        if (want.parameters) {
          this.notifyVisibleId = want.parameters.notifyVisibleId;
        }
        setTimeout(() => {
          this.canCreateForm = want.parameters?.isCreate ?? false;
          console.info(`${this.TAG} aboutToAppear:getWant canCreateForm:` + this.canCreateForm);
        }, 10);
        console.info(`${this.TAG} aboutToAppear:getWant end` + JSON.stringify(want));

        if (!(want.parameters?.isCreate ?? false) && this.deleteForm) {
          console.log(`${this.TAG} aboutToAppear:formsystemhost deleteForm start`);
          formHost.deleteForm(this.deleteId)
            .then((data) => {
              console.info(`${this.TAG} formsystemhost deleteForm result:` + data);
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: "0",
                parameters: {
                  "formId": this.deleteId.toString()
                }
              };
              commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
            })
            .catch((error:BusinessError) => {
              console.info(`${this.TAG} formsystemhost deleteForm error:` + error);
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: error.code.toString(),
                parameters: {
                  "formId": this.deleteId.toString()
                }
              };
              commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
            });

          console.log(`${this.TAG} aboutToAppear:formsystemhost deleteForm end`);
        }
        if (!(want.parameters?.isCreate??false) && this.refreshForm) {
          console.log(`${this.TAG} aboutToAppear:formsystemhost refreshForm start`);
          formProvider.setFormNextRefreshTime(this.refreshId, this.refreshTime)
            .then((data) => {
              console.info(`${this.TAG} formsystemhost refreshForm result:` + data);
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: "0",
                parameters: {
                  "formId": this.refreshId.toString()
                }
              };
              commonEvent.publish(this.formOnDynamicRefreshEvent, commonEventPublishData, this.formOnDynamicRefreshCallBack);
            })
            .catch((error:BusinessError) => {
              console.info(`${this.TAG} formsystemhost refreshForm error:` + JSON.stringify(error));
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: error.code.toString(),
                parameters: {
                  "formId": this.refreshId.toString()
                }
              };
              commonEvent.publish(this.formOnDynamicRefreshEvent, commonEventPublishData, this.formOnDynamicRefreshCallBack);
            });

          console.log(`${this.TAG} aboutToAppear:formsystemhost deleteForm end`);
        }
        if (!(want.parameters?.isCreate??false) && this.requestForm) {
          console.log(`${this.TAG} aboutToAppear:requestForm start`);
          formHost.requestForm(this.requestId)
            .then((data) => {
              console.info(`${this.TAG} requestForm result:` + data);
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: "0",
                parameters: {
                  "formId": this.requestId
                }
              };
              commonEvent.publish(this.formOnRequestEvent, commonEventPublishData, this.formOnRequestCallBack);
            })
            .catch((error:BusinessError) => {
              console.info(`${this.TAG} requestForm error:` + error.toString());
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: error.code.toString(),
                parameters: {
                  "formId": this.requestId
                }
              };
              commonEvent.publish(this.formOnRequestEvent, commonEventPublishData, this.formOnRequestCallBack);
            });

          console.log(`${this.TAG} aboutToAppear:requestForm end`);
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
              console.log(`${this.TAG} onAcquired start`);
              console.log(`${this.TAG} get form, form id:` + form.id);
              this.formId = form.id;
              if (this.deleteForm) {
                console.log(`${this.TAG} onAcquired:deleteForm start`);
                if (this.deleteId == "self") {
                  this.deleteId = this.formId.toString();
                }
                setTimeout(() => {
                  formHost.deleteForm(this.deleteId)
                    .then((data) => {
                      console.info(`${this.TAG} deleteForm result:` + data);
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "0",
                        parameters: {
                          "formId": this.deleteId.toString()
                        }
                      };
                      commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
                    })
                    .catch((error:BusinessError) => {
                      console.info(`${this.TAG} formsystemhost deleteForm error:` + error);
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: error.code.toString(),
                        parameters: {
                          "formId": this.deleteId.toString()
                        }
                      };
                      commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
                    });

                  console.log(`${this.TAG} onAcquired:deleteForm end`);
                }, 1000);
              } else if (this.requestForm) {
                console.log(`${this.TAG} onAcquired:requestForm start`);
                if (this.requestId == "self") {
                  this.requestId = this.formId.toString();
                }
                setTimeout(() => {
                  formHost.requestForm(this.requestId)
                    .then((data) => {
                      console.info(`${this.TAG} requestForm result:` + data);
                      let delFormId = this.requestId;
                      if (this.requestId == "0" || this.requestId == "-1") {
                        delFormId = this.formId.toString();
                      }
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "0",
                        parameters: {
                          "formId": delFormId
                        }
                      };
                      commonEvent.publish(this.formOnRequestEvent, commonEventPublishData, this.formOnRequestCallBack);
                    })
                    .catch((error:BusinessError) => {
                      console.info(`${this.TAG} requestForm error:` + error);
                      let delFormId = this.requestId;
                      if (this.requestId == "0" || this.requestId == "-1") {
                        delFormId = this.formId.toString();
                      }
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: error.code.toString(),
                        parameters: {
                          "formId": delFormId
                        }
                      };
                      commonEvent.publish(this.formOnRequestEvent, commonEventPublishData, this.formOnRequestCallBack);
                    });

                  console.log(`${this.TAG} onAcquired:requestForm end`);
                }, 1000);
              } else if (this.refreshForm) {
                console.log(`${this.TAG} onAcquired:formsystemhost refreshForm start`);
                if (this.refreshId == "self") {
                  this.refreshId = this.formId.toString();
                }
                setTimeout(() => {
                  formProvider.setFormNextRefreshTime(this.refreshId, this.refreshTime)
                    .then((data) => {
                      console.info(`${this.TAG} formsystemhost refreshForm result:` + data);
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "0",
                        parameters: {
                          "formId": this.refreshId.toString()
                        }
                      };
                      commonEvent.publish(this.formOnDynamicRefreshEvent, commonEventPublishData, this.formOnDynamicRefreshCallBack);
                    })
                    .catch((error:BusinessError) => {
                      console.info(`${this.TAG} formsystemhost refreshForm error:` + error);
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: error.code.toString(),
                        parameters: {
                          "formId": this.refreshId.toString()
                        }
                      };
                      commonEvent.publish(this.formOnDynamicRefreshEvent, commonEventPublishData, this.formOnDynamicRefreshCallBack);
                    });
                    console.log(`${this.TAG} onAcquired:formsystemhost refreshForm end`);
                }, 1000);
              } else if (this.notifyVisibleForms) {
                console.log(`${this.TAG} onAcquired:formsystemhost notifyVisibleForms start`);
                if (this.notifyVisibleId == "self") {
                  this.notifyVisibleId = this.formId.toString();
                }
                setTimeout(() => {
                  formHost.notifyVisibleForms([this.notifyVisibleId])
                    .then((data) => {
                      console.info(`${this.TAG} formsystemhost notifyVisibleForms result:` + data);
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "0",
                        parameters: {
                          "formId": this.notifyVisibleId.toString()
                        }
                      };
                    })
                    .catch((error:BusinessError) => {
                      console.info(`${this.TAG} formsystemhost notifyVisibleForms error:` + error);
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: error.code.toString(),
                        parameters: {
                          "formId": this.notifyVisibleId.toString()
                        }
                      };
                    });
                    console.log(`${this.TAG} onAcquired:formsystemhost notifyVisibleForms end`);
                }, 1000);
              } else {
                console.log(`${this.TAG} onAcquired:else start`);
                let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                  data: this.formId.toString()
                };
                setTimeout(()=>{
                  console.log(`${this.TAG} publish formOnAcquiredEvent:` + this.formOnAcquiredEvent);
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, this.publishOnAcquiredCallBack);
                  console.log(`${this.TAG} onAcquired:else end`);
                }, 500);
              }
              console.log(`${this.TAG} onAcquired end`);
            })
            .onUninstall((info:InfoType) => {
              console.log(`${this.TAG} onUninstall:` + JSON.stringify(info));
            })
            .onError((error) => {
              console.log(`${this.TAG} error code:` + error.errcode);
              console.log(`${this.TAG} error msg:` + error.msg);
              if (this.deleteForm && this.deleteId) {
                let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                  data: error.msg,
                  parameters: {
                    "formId": this.formId.toString()
                  }
                };
                commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
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
