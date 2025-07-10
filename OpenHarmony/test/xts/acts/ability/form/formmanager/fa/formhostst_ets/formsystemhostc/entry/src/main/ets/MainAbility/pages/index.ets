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
import formManager from '@ohos.application.formHost';
import systemTime from '@ohos.systemTime';
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

  private TAG = `[FormComponent.host.c]`;

  private releaseForm = false;
  private releaseId = "-1";
  private deleteForm = false;
  private deleteId = "-1";
  private temporaryId="-1";
  private temporary = false;
  private castForm = false;
  private sendCastForm = false;
  private sendCastFormMsg = "";

  private formOnAcquiredEvent = "FMS_FormOnAcquired_commonEvent";
  private formCastEvent = "FMS_FormCast_commonEvent";
  private formOnErrorEvent = "FMS_FormOnError_commonEvent";
  private formOnDeletedEvent = "FMS_FormOnDeleted_commonEvent";
  private formOnReleasedEvent = "FMS_FormOnReleased_commonEvent";

  private subscriberDel:CommonEventManager.CommonEventSubscriber|null = null;
  private subscriberFormDeleteEvent:CommonEventManager.CommonEventSubscribeInfo = {
    events: ["FMS_FormDelete_commonEvent"],
  };
  private subscriberTimeChange:CommonEventManager.CommonEventSubscriber|null = null;
  private subscriberTimeChangeEvent:CommonEventManager.CommonEventSubscribeInfo = {
    events: ["FMS_TimeChange_commonEvent"],
  };

  private publishOnErrorCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} formsystemhostc formOnErrorEventCallBack ====>`);
  }
  private publishOnDeletedCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} formsystemhostc publishOnDeletedCallBack ====>`);
  }
  private publishOnReleasedCallBack = () => {
    this.canCreateForm = false;
    console.debug(`====>${this.TAG} formsystemhostc publishOnReleasedCallBackk ====>`);
  }
  private publishOnAcquiredCallBack = () => {
    console.debug(`====>${this.TAG} formsystemhostc formOnAcquiredEventCallBack ====>`);
  }
  private publishCastCallBack = () => {
    console.debug(`====>${this.TAG} formsystemhostc formCastEventCallBack ====>`);
  }

  private deleteCallBack = (err:BusinessError, data:CommonEventManager.CommonEventData) => {
    console.info(`!!!====>${this.TAG} deleteCallBack start:====>` + JSON.stringify(data));
    if(data.bundleName && data.bundleName != "com.ohos.st.formsystemhostc") {
      return;
    }
    formManager.deleteForm(data.data)
      .then((data2) => {
        console.info(`${this.TAG} deleteForm result:` + data2);
      });

    this.canCreateForm = false;
    console.info(`!!!====>${this.TAG} deleteCallBack end ====>`);
  }
  
  private timeChangeCallBack = (err:BusinessError, data:CommonEventManager.CommonEventData) => {
    console.info(`!!!====>${this.TAG} timeChangeCallBack start:====>` + JSON.stringify(data));
    if(data.bundleName && data.bundleName != "com.ohos.st.formsystemhostc") {
      return;
    }
    this.changeTime(data.parameters?.hour ?? 0, data.parameters?.min??0, data.parameters?.second??0);
    console.info(`!!!====>${this.TAG} timeChangeCallBack end ====>`);
  }

  /**
  *
  * @param hour hour of 24H clock
  * @param min minute
  * @param second second
  */
  private changeTime(hour: number, min: number, second: number): void {
    let datetime = new Date();
    let year = datetime.getFullYear();
    let month = datetime.getMonth() + 1;
    let day = datetime.getDate();
    let formatTime = year + '-' + this.fill(month) + '-' + this.fill(day)
      + 'T' + this.fill(hour) + ':' + this.fill(min) + ':' + this.fill(second);
    let s = (new Date(formatTime)).getTime();

    systemTime.setTime(s).then(data => {
      console.log(`${this.TAG} set time success: ${formatTime}, result:${data}`);
    }).catch((error:BusinessError) => {
      console.log(`${this.TAG} set time failure: ${JSON.stringify(error)}`);
    })
  }

  private fill(value: number): string {
    return (value > 9 ? "" : "0") + value;
  }
  aboutToAppear() {
    console.log(`${this.TAG} aboutToAppear start`);
    commonEvent.createSubscriber(this.subscriberFormDeleteEvent).then(async (data) => {
        console.info(`====>${this.TAG} Subscriber FormDelete data:====>`, JSON.stringify(data)); 
        this.subscriberDel = data;     
        await commonEvent.subscribe(this.subscriberDel, this.deleteCallBack);
    })

    commonEvent.createSubscriber(this.subscriberTimeChangeEvent).then(async (data) => {
        console.info(`====>${this.TAG} Subscriber TimeChange data:====>`, JSON.stringify(data)); 
        this.subscriberTimeChange = data;     
        await commonEvent.subscribe(this.subscriberTimeChange, this.timeChangeCallBack);
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
      if(want.parameters) {
        this.releaseForm = want.parameters.releaseForm;
      }
      if(want.parameters) {
        this.releaseId = want.parameters.releaseId;
      }

      if(want.parameters) {
        this.sendCastForm = want.parameters.sendCastForm;
      }
      if(want.parameters) {
        this.sendCastFormMsg = want.parameters.sendCastFormMsg;
      }

      setTimeout(() => {
        this.canCreateForm = want.parameters?.isCreate ?? false;
        console.info(`${this.TAG} aboutToAppear:getWant canCreateForm:`+ this.canCreateForm); 
      }, 10); 
      console.info(`${this.TAG} aboutToAppear:getWant end`+ JSON.stringify(want));
      if(!(want.parameters?.isCreate ?? false) && this.sendCastForm) {
        console.log(`${this.TAG} aboutToAppear:sendCastForm start`);
        setTimeout(() => {
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: this.sendCastFormMsg,
            parameters: {
              "formId" : ""
            }
          };
          commonEvent.publish(this.formCastEvent, commonEventPublishData, this.publishCastCallBack);
        }, 5);              
        console.log(`${this.TAG} aboutToAppear:sendCastForm end`);
      }
      if(!(want.parameters?.isCreate ?? false) && this.deleteForm) {
        console.log(`${this.TAG} aboutToAppear:deleteForm start`);
        formManager.deleteForm(this.deleteId)
          .then((data) => {
          console.info(`${this.TAG} deleteForm result:` + data);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: "0",
            parameters: {
              "formId" : this.deleteId
            }
          };
          setTimeout(()=>{
            console.log(`${this.TAG} publish formOnAcquiredEvent:` + this.formOnDeletedEvent);
            commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
          }, 500);
        })
        .catch((error:BusinessError) => {
          console.info(`${this.TAG} deleteForm error:` + error);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: error.code.toString(),
            parameters: {
              "formId" : this.deleteId.toString()
            }
          };
          commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
        });
      
        console.log(`${this.TAG} aboutToAppear:deleteForm end`);
      }
      if(!(want.parameters?.isCreate??false) && this.releaseForm) {
        console.log(`${this.TAG} aboutToAppear:releaseForm start`);
        formManager.releaseForm(this.releaseId, true)
        .then((data) => {
          console.info(`${this.TAG} releaseForm result:` + data);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: "0",
            parameters: {
              "formId" : this.releaseId
            }
          };
          commonEvent.publish(this.formOnReleasedEvent, commonEventPublishData, this.publishOnReleasedCallBack);
        })
        .catch((error:BusinessError) => {
          console.info(`${this.TAG} releaseForm error:` + error);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            data: error.code.toString(),
            parameters: {
              "formId" : this.releaseId
            }
          };
          commonEvent.publish(this.formOnReleasedEvent, commonEventPublishData, this.publishOnReleasedCallBack);
        });
      
        console.log(`${this.TAG} aboutToAppear:releaseForm end`);
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
                formManager.castTempForm(this.temporaryId)
                    .then((data) => {
                    console.info(`${this.TAG} castTempForm result:` + data);
                     setTimeout(() => {
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "0",
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
                          "formId" : this.formId.toString()
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
                  formManager.deleteForm(this.deleteId)
                    .then((data) => {
                    console.info(`${this.TAG} deleteForm result:` + data);
                    let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                      data: "0",
                      parameters: {
                        "formId" : this.deleteId
                      }
                    };
                    commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.publishOnDeletedCallBack);
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
               
                  console.log(`${this.TAG} onAcquired:deleteForm end`);
                }, 1000);                
              } else if(this.releaseForm) {
                console.log(`${this.TAG} onAcquired:releaseForm start`);
                if(this.releaseId == "self") {
                  this.releaseId = this.formId.toString();
                }
                setTimeout(() => {
                  formManager.releaseForm(this.releaseId, true)
                    .then((data) => {
                    console.info(`${this.TAG} releaseForm result:` + data);
                    let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                      data: "0",
                      parameters: {
                        "formId" : this.releaseId
                      }
                    };
                    commonEvent.publish(this.formOnReleasedEvent, commonEventPublishData, this.publishOnReleasedCallBack);
                  })
                  .catch((error:BusinessError) => {
                    console.info(`${this.TAG} releaseForm error:` + error);
                    let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                      data: error.code.toString(),
                      parameters: {
                        "formId" : this.releaseId
                      }
                    };
                    commonEvent.publish(this.formOnReleasedEvent, commonEventPublishData, this.publishOnReleasedCallBack);
                  });
               
                  console.log(`${this.TAG} onAcquired:releaseForm end`);
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
            // .onUninstall((info) => {
            //   console.log("[FormComponent] onUninstall:" + JSON.stringify(info));
            // })
            .onError((error:ErroType) => {
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