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
  @State ability: string = "MainAbility";
  @State moduleName: string = "entry";
  @State name: string = "Form_Js001";
  @State allowUpate: boolean = true;
  @State isShowing: boolean = true;
  @State canCreateForm: boolean = false;

  private dimension: FormDimension = FormDimension.Dimension_1_2;

  private TAG = `[FormComponent.host.g]`;

  private stateForm = "visible";
  private doStateForm = false;
  private stateIds:string[] = [];
  private deleteForm = false;
  private deleteId = "-1";
  private temporaryId="-1";
  private temporary = false;

  private formOnAcquiredEvent = "FMS_FormOnAcquired_commonEvent";
  private formOnErrorEvent = "FMS_FormOnError_commonEvent";
  private formOnDeletedEvent = "FMS_FormOnDeleted_commonEvent";
  private formOnStateEvent = "FMS_FormOnState_commonEvent";

  private subscriberDel:CommonEventManager.CommonEventSubscriber|null = null;
  private subscriberFormDeleteEvent:CommonEventManager.CommonEventSubscribeInfo = {
    events: ["FMS_FormDelete_commonEvent"],
  };

  private publishOnErrorCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} formOnErrorEventCallBack ====>`);
  }
  private publishOnDeletedCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} publishOnDeletedCallBack ====>`);
  }
  private publishOnStateCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} publishOnStateCallBack ====>`);
  }
  private publishOnAcquiredCallBack = () => {
    console.debug(`====>${this.TAG} formOnAcquiredEventCallBack ====>`);
  }
  private deleteCallBack = (err:BusinessError, data:CommonEventManager.CommonEventData) => {
    console.info(`!!!====>${this.TAG} deleteCallBack start:====>` + JSON.stringify(data));
    if(data.bundleName && data.bundleName != "com.ohos.st.formsystemhostg") {
      return;
    }
    formHost.deleteForm(data.data)
      .then((data2) => {
        console.info(`${this.TAG} deleteForm result:` + data2);
      });

    this.canCreateForm = false;
    console.info(`!!!====>${this.TAG} deleteCallBack end ====>`);
  }

  aboutToAppear() {
    console.log(`${this.TAG} aboutToAppear start`);
    commonEvent.createSubscriber(this.subscriberFormDeleteEvent).then(async (data) => {
        console.info(`====>${this.TAG} Subscriber FormDelete data:====>`, JSON.stringify(data)); 
        this.subscriberDel = data;     
        await commonEvent.subscribe(this.subscriberDel, this.deleteCallBack);
    })

    console.info(`${this.TAG} aboutToAppear:getWant start`);
    featureAbility.getWant()
    .then((want: Want) => {
      this.formId = parseInt(want.parameters?.formId ?? 0);
      this.name = want.parameters?.name ?? '';
      this.bundle =  want.parameters?.bundle ?? '';
      this.ability = want.parameters?.ability ?? '';
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
        this.temporaryId = want.parameters.temporaryId;
      }
      if(want.parameters) {
        this.deleteForm = want.parameters.deleteForm;
      }
      if(want.parameters) {
        this.deleteId = want.parameters.deleteId;
      }
      if(want.parameters) {
        this.stateForm = want.parameters.stateForm;
        this.doStateForm = true;
      }
      if(want.parameters) {
        this.stateIds = want.parameters.stateIds;
      }
      setTimeout(() => {
        this.canCreateForm = want.parameters?.isCreate ?? false;
        console.info(`${this.TAG} aboutToAppear:getWant canCreateForm:` + this.canCreateForm); 
      }, 10); 
      console.info(`${this.TAG} aboutToAppear:getWant end` + JSON.stringify(want));

      if(!(want.parameters?.isCreate ?? false) && this.deleteForm) {
        console.log(`${this.TAG} aboutToAppear:deleteForm start`);
        formHost.deleteForm(this.deleteId)
          .then((data) => {
          console.info(`${this.TAG} deleteForm result:` + data);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: "0",
            parameters: {
              "formId" : this.deleteId,
            }
          };
          setTimeout(()=>{
            console.log(`${this.TAG} publish formOnStateEvent:` + this.formOnDeletedEvent);
            commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
          }, 500);
        })
        .catch((error:BusinessError) => {
          console.info(`${this.TAG} deleteForm error:` + error);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: error.code.toString(),
            parameters: {
              "formId" : this.deleteId,
            }
          };
          commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
        });
      
        console.log(`${this.TAG} aboutToAppear:deleteForm end`);
      }

      if(!(want.parameters?.isCreate ?? false) && this.stateForm) {
        if(this.stateForm == "visible") {
          console.log(`${this.TAG} aboutToAppear:notifyVisibleForm start`);
          formHost.notifyVisibleForms(this.stateIds)
            .then((data) => {
            console.info(`${this.TAG} notifyVisibleForm result:` + data);
            console.info(`${this.TAG} notifyVisibleForm formId:` + this.stateIds[0]);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: "0",
              parameters: {
                "kind": "visible",
                "formId" : this.stateIds[0]
              }
            };
            setTimeout(()=>{
              console.log(`${this.TAG} publish formOnStateEvent:` + this.formOnStateEvent);
              commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
            }, 500);
          })
          .catch((error:BusinessError) => {
            console.info(`${this.TAG} notifyVisibleForm error:` + error);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: error.code.toString(),
              parameters: {
                "kind": "visible",
                "formId" : this.stateIds[0]
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
          });        
          console.log(`${this.TAG} aboutToAppear:notifyVisibleForm end`);
        } else if(this.stateForm == "invisible") {
          console.log(`${this.TAG} aboutToAppear:notifyInvisibleForm start`);
          formHost.notifyInvisibleForms(this.stateIds)
            .then((data) => {
            console.info(`${this.TAG} notifyInvisibleForm result:` + data);
            console.info(`${this.TAG} notifyInvisibleForm formId:` + this.stateIds[0]);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: "0",
              parameters: {
                "kind": "invisible",
                "formId" : this.stateIds[0]
              }
            };
            setTimeout(()=>{
              console.log(`${this.TAG} publish formOnStateEvent:` + this.formOnStateEvent);
              commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
            }, 500);
          })
          .catch((error:BusinessError) => {
            console.info(`${this.TAG} notifyInvisibleForm error:` + error);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: error.code.toString(),
              parameters: {
                "kind": "invisible",
                "formId" : this.stateIds[0]
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
          });
          console.log(`${this.TAG} aboutToAppear:notifyInvisibleForm end`);
        } else if(this.stateForm == "enable") {
          console.log(`${this.TAG} aboutToAppear:enableFormsUpdate start`);
          formHost.enableFormsUpdate(this.stateIds)
            .then((data) => {
            console.info(`${this.TAG} enableFormsUpdate result:` + data);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: "0",
              parameters: {
                "kind": "enable",
                "formId" : this.stateIds[0]
              }
            };
            setTimeout(()=>{
              console.log(`${this.TAG} publish formOnStateEvent:` + this.formOnStateEvent);
              commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
            }, 500);
          })
          .catch((error:BusinessError) => {
            console.info(`${this.TAG} enableFormsUpdate error:` + error);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: error.code.toString(),
              parameters: {
                "kind": "enable",
                "formId" : this.stateIds[0]
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
          });        
          console.log(`${this.TAG} aboutToAppear:enableFormsUpdate end`);
        } else {
          console.log(`${this.TAG} aboutToAppear:disableFormsUpdate start`);
          formHost.disableFormsUpdate(this.stateIds)
            .then((data) => {
            console.info(`${this.TAG} disableFormsUpdate result:` + data);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: "0",
              parameters: {
                "kind": "disable",
                "formId" : this.stateIds[0]
              }
            };
            setTimeout(()=>{
              console.log(`${this.TAG} publish formOnStateEvent:` + this.formOnStateEvent);
              commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
            }, 500);
          })
          .catch((error:BusinessError) => {
            console.info(`${this.TAG} disableFormsUpdate error:` + error);
            let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
              data: error.code.toString(),
              parameters: {
                "kind": "disable",
                "formId" : this.stateIds[0]
              }
            };
            commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
          });        
          console.log(`${this.TAG} aboutToAppear:disableFormsUpdate end`);
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
              console.log(`${this.TAG} onAcquired start`);
              console.log(`${this.TAG} get form, form id:` + form.id);
              this.formId = form.id;
              if(this.deleteForm) {
                console.log(`${this.TAG} deleteForm start`);
                if(this.deleteId == "self") {
                  this.deleteId = this.formId.toString();
                }
                setTimeout(() => {
                  formHost.deleteForm(this.deleteId)
                    .then((data) => {
                    console.info(`${this.TAG} deleteForm result:` + data);
                    let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                      data: "0",
                      parameters: {
                        "formId" : this.deleteId,
                      }
                    };
                    setTimeout(()=>{
                      console.log(`${this.TAG} publish formOnStateEvent:` + this.formOnDeletedEvent);
                      commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
                    }, 500);
                  })
                  .catch((error:BusinessError) => {
                    console.info(`${this.TAG} deleteForm error:` + error);
                    let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                      data: error.code.toString(),
                      parameters: {
                        "formId" : this.deleteId
                      }
                    };
                    commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
                  });
               
                  console.log(`${this.TAG} deleteForm end`);
                }, 500);
              } else if(this.doStateForm) {
                console.log(`${this.TAG} state start`);
                if((this.stateIds.length != 0) && (this.stateIds[0] == "self")) {
                  this.stateIds = [this.formId.toString()];
                }
                console.info(`${this.TAG} notifyVisibleForm stateIds:` + this.stateIds);
                setTimeout(() => {
                  if(this.stateForm == "visible") {
                    console.log(`${this.TAG} notifyVisibleForm start`);
                    formHost.notifyVisibleForms(this.stateIds)
                      .then((data) => {
                      console.info(`${this.TAG} notifyVisibleForm result:` + data);
                      console.info(`${this.TAG} notifyVisibleForm formId:` + this.stateIds[0]);
                      if(this.stateIds.length == 0) {
                        this.stateIds = ["-1"];
                      }
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "0",
                        parameters: {
                          "kind": "visible",
                          "formId" : this.stateIds[0]
                        }
                      };
                      setTimeout(()=>{
                        console.log(`${this.TAG} publish formOnStateEvent:` + this.formOnStateEvent);
                        commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
                      }, 500);
                    })
                    .catch((error:BusinessError) => {
                      console.info(`${this.TAG} notifyVisibleForm error:` + error);
                      if(this.stateIds.length == 0) {
                        this.stateIds = ["-1"];
                      }
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: error.code.toString(),
                        parameters: {
                          "kind": "visible",
                          "formId" : this.stateIds[0]
                        }
                      };
                      commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
                    });
                    console.log(`${this.TAG} notifyVisibleForm end`);
                  } else if(this.stateForm == "invisible") {
                    console.log(`${this.TAG} notifyInvisibleForm start`);
                    formHost.notifyInvisibleForms(this.stateIds)
                    .then((data) => {
                      console.info(`${this.TAG} notifyInvisibleForm result:` + data);
                      if(this.stateIds.length == 0) {
                        this.stateIds = ["-1"];
                      }
                      console.info(`${this.TAG} notifyInvisibleForm formId:` + this.stateIds[0]);
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "0",
                        parameters: {
                          "kind": "invisible",
                          "formId" : this.stateIds[0]
                        }
                      };
                      setTimeout(()=>{
                        console.log(`${this.TAG} publish formOnStateEvent:` + this.formOnStateEvent);
                        commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
                      }, 500);
                    })
                    .catch((error:BusinessError) => {
                      console.info(`${this.TAG} notifyInvisibleForm error:` + error.code);
                      if(this.stateIds.length == 0) {
                        this.stateIds = ["-1"];
                      }
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: error.code.toString(),
                        parameters: {
                          "kind": "invisible",
                          "formId" : this.stateIds[0]
                        }
                      };
                      commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
                    });
                    console.log(`${this.TAG} notifyInvisibleForm end`);
                  } else if(this.stateForm == "enable") {
                    console.log(`${this.TAG} enableFormsUpdate start`);
                    formHost.enableFormsUpdate(this.stateIds)
                      .then((data) => {
                      console.info(`${this.TAG} enableFormsUpdate result:` + data);
                      if(this.stateIds.length == 0) {
                        this.stateIds = ["-1"];
                      }
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "0",
                        parameters: {
                          "kind": "enable",
                          "formId" : this.stateIds[0]
                        }
                      };
                      setTimeout(()=>{
                        console.log(`${this.TAG} publish formOnStateEvent:` + this.formOnStateEvent);
                        commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
                      }, 500);
                    })
                    .catch((error:BusinessError) => {
                      console.info(`${this.TAG} enableFormsUpdate error:` + error);
                      if(this.stateIds.length == 0) {
                        this.stateIds = ["-1"];
                      }
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: error.code.toString(),
                        parameters: {
                          "kind": "enable",
                          "formId" : this.stateIds[0]
                        }
                      };
                      commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
                    });        
                    console.log(`${this.TAG} enableFormsUpdate end`);
                  } else {
                    console.log(`${this.TAG} disableFormsUpdate start`);
                    formHost.disableFormsUpdate(this.stateIds)
                      .then((data) => {
                      console.info(`${this.TAG} disableFormsUpdate result:` + data);
                      if(this.stateIds.length == 0) {
                        this.stateIds = ["-1"];
                      }
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "0",
                        parameters: {
                          "kind": "disable",
                          "formId" : this.stateIds[0]
                        }
                      };
                      setTimeout(()=>{
                        console.log(`${this.TAG} publish formOnStateEvent:` + this.formOnStateEvent);
                        commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
                      }, 500);
                    })
                    .catch((error:BusinessError) => {
                      console.info(`${this.TAG} disableFormsUpdate error:` + error);
                      if(this.stateIds.length == 0) {
                        this.stateIds = ["-1"];
                      }
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: error.code.toString(),
                        parameters: {
                          "kind": "disable",
                          "formId" : this.stateIds[0]
                        }
                      };
                      commonEvent.publish(this.formOnStateEvent, commonEventPublishData, this.publishOnStateCallBack);
                    });
                    console.log(`${this.TAG} disableFormsUpdate end`);
                  }
                }, 1000);                
              } else {
                let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: this.formId.toString(),
                    parameters: {
                      "formId" : this.formId.toString()
                    }
                };
                setTimeout(()=>{
                  console.log(`${this.TAG} publish formOnAcquiredEvent:` + this.formOnAcquiredEvent);
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, this.publishOnAcquiredCallBack);
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
              if(this.deleteForm && this.deleteId) {
                let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: error.msg,
                    parameters: {
                      "formId" : this.formId.toString()
                    }
                };
                commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
              } else {
                let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: error.msg,
                    parameters: {
                      "formId" : "-1"
                    }
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
