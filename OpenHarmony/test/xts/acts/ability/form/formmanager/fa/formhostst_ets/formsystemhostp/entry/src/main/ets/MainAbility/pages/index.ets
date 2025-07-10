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
import formHost from '@ohos.application.formHost';
import commonEvent from '@ohos.commonEvent';
import CommonEventManager from '@ohos.commonEventManager';
import { BusinessError } from '@ohos.base';
import Want from '@ohos.application.Want';

interface InfoType {id:number}
interface ErroType {errcode: number, msg: string}

@Entry
@Component
struct Index {
  @State bundle: string = "com.acts.form.formsystemtestservicea.hmservice";
  @State ability: string = "com.acts.form.formsystemtestservicea.hmservice.FormAbility";
  @State moduleName: string = "entry";
  @State name: string = "Form_Js001";
  @State allowUpate: boolean = true;
  @State isShowing: boolean = true;
  @State optType: number = 0;
  private dimension: FormDimension = FormDimension.Dimension_1_2;

  private TAG = `[FormComponent.host.p]`;

  private normal = 0;
  private temporary = 0;
  private normaldel = 0;
  private temporarydel = 0;
  private normalFormIds: string[] = [];
  private tempFormIds: string[] = [];

  private deleteIds: string[] = [];
  private delCount = 0;

  private normalFormCounts: number[] = [];
  private tempFormCounts: number[] = [];

  private castFormId = "-1";

  private formOnAcquiredEvent = "FMS_FormOnAcquired_commonEvent";
  private formOnErrorEvent = "FMS_FormOnError_commonEvent";
  private formCastEvent = "FMS_FormCast_commonEvent";

  private subscriberDel:CommonEventManager.CommonEventSubscriber|null = null;
  private subscriberFormDeleteEvent:CommonEventManager.CommonEventSubscribeInfo = {
    events: ["FMS_FormDelete_commonEvent"],
  };

  private subscriberSendCastForm:CommonEventManager.CommonEventSubscriber|null = null;
  private subscriberSendCastFormEvent:CommonEventManager.CommonEventSubscribeInfo = {
    events: ["FMS_SendCastForm_commonEvent"],
  };

  private publishOnErrorCallBack() {
    console.debug(`====> ${this.TAG} formsystemhostp formOnErrorEvent Publish CallBack ====>`);
  }
  private publishOnAcquiredCallBack() {
    console.debug(`====> ${this.TAG} formsystemhostp formOnAcquiredEvent Publish CallBack ====>`);
  }
  private publishDeleteCallBack() {
    console.debug(`====> ${this.TAG} formsystemhostp publishDeleteCallBack Publish CallBack ====>`);
  }
  private publishCastCallBack() {
    console.debug(`====>${this.TAG} formCastEvent Publish CallBack ====>`);
  }
  private startAbility(bundleName:string, abilityName:string, sendMsg:string) {
    featureAbility.startAbility({
      want: {
        bundleName: bundleName,
        abilityName: abilityName,
        parameters: {
          "formId": "0",
          "name": "Form_Js001",
          "bundle": "com.acts.form.formsystemtestservicea.hmservice",
          "ability": "com.acts.form.formsystemtestservicea.hmservice.FormAbility",
          "moduleName": "entry",
          "temporary": false,
          "sendCastForm": true,
          "sendCastFormMsg": sendMsg,
          "isCreate": false
        }
      }
    }).then((res: number) => {
      console.info(`${this.TAG} formsystemhostp featureAbility.startAbility res: ${JSON.stringify(res)}`);
    }).catch((err: BusinessError) => {
      console.info(`${this.TAG} formsystemhostp featureAbility.startAbility error: ${JSON.stringify(err)}`);
    });
  }
  private publishOnSendCastForm(err:BusinessError, data:CommonEventManager.CommonEventData) {
    console.info(`!!!====>${this.TAG} formsystemhostp OnSendCastForm start:====>` + JSON.stringify(data));
    if (data.bundleName && data.bundleName != "com.ohos.st.formsystemhostp") {
      return;
    }
    let formId: string = data.parameters?.castFormId ?? '';
    setTimeout(() => {
      console.info(`${this.TAG} castTempForm start, formId: ` + formId);
      formHost.castTempForm(formId)
        .then(() => {
          console.info(`${this.TAG} castTempForm result:`);
          this.startAbility("com.ohos.st.formsystemhostc", "com.ohos.st.formsystemhostc.MainAbility", '');
        })
        .catch((error:BusinessError) => {
          console.info(`${this.TAG} castTempForm error:` + JSON.stringify(error));
          this.startAbility("com.ohos.st.formsystemhostc", "com.ohos.st.formsystemhostc.MainAbility", error.code.toString());
        });
      console.info(`${this.TAG} castTempForm end`);
    }, 5000);
    console.info(`!!!====>${this.TAG} formsystemhostp OnSendCastForm end`);
  }

  private deleteCallBack(err:BusinessError, data:CommonEventManager.CommonEventData) {
    console.info(`!!!====>${this.TAG} formsystemhostp deleteCallBack start:====>` + JSON.stringify(data));
    const delFormIds: string[] = data.parameters?.formIds ?? [];
    console.info(`${this.TAG} data size ${delFormIds.length}`);

    console.info(`${this.TAG} data.subscriber ${JSON.stringify(this.subscriberDel)}`);
    commonEvent.unsubscribe(this.subscriberDel, () => {
      console.info(`====> ${this.TAG} formsystemhostp optType unSubscribeDeleteCallback CallBack====>`);
    });
    commonEvent.unsubscribe(this.subscriberSendCastForm, () => {
      console.info(`====> ${this.TAG} formsystemhostp optType unSubscribeCastFormCallback CallBack====>`);
    });
    console.info(`${this.TAG} data.bundleName ${JSON.stringify(data.bundleName)}`);
    console.info(`${this.TAG} tof ${"com.ohos.st.formsystemhostp" != data.bundleName}`);
    if ("com.ohos.st.formsystemhostp" != data.bundleName) {
      return;
    }
    delFormIds.forEach(async (formId:string, index:number) => {
      console.info(`${this.TAG} formsystemhostp deleteForm ${index}`);
      try {
        const res = await formHost.deleteForm(formId);
        console.info(`${this.TAG} formsystemhostp deleteForm ${index} ${formId} result: ${JSON.stringify(res)}`);
      } catch (error) {
        console.info(`${this.TAG} formsystemhostp deleteForm ${index} ${formId} error: ${JSON.stringify(error)}`);
      }
    });
    console.info(`!!!====> ${this.TAG} formsystemhostp optType deleteCallBack end ====>`);
  }

  aboutToAppear() {
    console.info(`${this.TAG} aboutToAppear start`);
    commonEvent.createSubscriber(this.subscriberFormDeleteEvent).then(async (data) => {
      console.info(`====>${this.TAG} formsystemhostp Subscriber FormDelete data:====>`, JSON.stringify(data));
      this.subscriberDel = data;
      commonEvent.subscribe(this.subscriberDel, (err:BusinessError, data:CommonEventManager.CommonEventData) => {
        if ("com.ohos.st.formsystemhostp" != data.bundleName) {
          return;
        } else {
          console.info(`!!!====>${this.TAG} formsystemhostp deleteCallBack start:====>` + JSON.stringify(data));
          const delFormIds: string[] = data.parameters?.formIds ?? [];
          console.info(`${this.TAG} data size ${delFormIds.length}`);
          commonEvent.unsubscribe(this.subscriberDel, () => {
            console.info(`====> ${this.TAG} formsystemhostp optType unSubscribeDeleteCallback CallBack====>`);
          });
          commonEvent.unsubscribe(this.subscriberSendCastForm, () => {
            console.info(`====> ${this.TAG} formsystemhostp optType unSubscribeCastFormCallback CallBack====>`);
          });
          let delCounter = 0;
          delFormIds.forEach((formId:string) => {
            formHost.deleteForm(formId, (error, data) => {
              console.info(`${this.TAG} formsystemhostp deleteForm ${++delCounter} ${formId} data: ${JSON.stringify(data)} err: ${JSON.stringify(error)}`);
            });
          });
        }
      });
    });

    commonEvent.createSubscriber(this.subscriberSendCastFormEvent).then(async (data) => {
      console.info(`====>${this.TAG} formsystemhostp Subscriber SendCastForm data:====>`, JSON.stringify(data));
      this.subscriberSendCastForm = data;
      await commonEvent.subscribe(this.subscriberSendCastForm, this.publishOnSendCastForm);
    });

    featureAbility.getWant()
      .then((want: Want) => {
        console.info(` ${this.TAG} formsystemhostp getWant:` + JSON.stringify(want));

        this.name = want.parameters?.name ?? "";
        this.bundle = want.parameters?.bundle ?? "";
        this.ability = want.parameters?.ability ?? "";
        if (want.parameters) {
          this.moduleName = want.parameters.moduleName;
        }

        if (want.parameters) {
          this.dimension = want.parameters.dimension;
        }

        if (want.parameters) {
          this.normal = want.parameters.normal;
        }

        if (want.parameters) {
          this.temporary = want.parameters.temporary;
        }
        if (want.parameters) {
          this.normaldel = want.parameters.normaldel;
        }

        if (want.parameters) {
          this.temporarydel = want.parameters.temporarydel;
        }

        if (want.parameters) {
          this.deleteIds = want.parameters.deleteIds;
        }
        if (want.parameters) {
          this.castFormId = want.parameters.castFormId;
        }

        this.makeCount();

        setTimeout(() => {
          this.optType = want.parameters?.optType ?? 0;
          console.info(` ${this.TAG} formsystemhostp getWant optType:` + this.optType);
        }, 10);

        if (this.castFormId != "-1" && (want.parameters?.optType ?? 0) == 3) {
          console.info(`${this.TAG} aboutToAppear:castTempForm start`);
          commonEvent.unsubscribe(this.subscriberDel, () => {
            console.info(`====> ${this.TAG} formsystemhostp optType unSubscribeDeleteCallback CallBack====>`);
          });
          commonEvent.unsubscribe(this.subscriberSendCastForm, () => {
            console.info(`====> ${this.TAG} formsystemhostp optType unSubscribeCastFormCallback CallBack====>`);
          });

          formHost.castTempForm(this.castFormId)
            .then((data) => {
              console.info(`${this.TAG} castTempForm result:` + data);
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: (data + "") || `0`,
                bundleName: "com.ohos.st.formsystemhostp",
                parameters: {
                  formId: this.castFormId.toString()
                }
              };
              commonEvent.publish(this.formCastEvent, commonEventPublishData, this.publishCastCallBack);
            })
            .catch((error:BusinessError) => {
              console.info(`${this.TAG} castTempForm error:` + JSON.stringify(error));
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: error.code.toString(),
                bundleName: "com.ohos.st.formsystemhostp",
                parameters: {
                  errMsg: error.message,
                  formId: this.castFormId.toString()
                }
              };
              commonEvent.publish(this.formCastEvent, commonEventPublishData, this.publishCastCallBack);
            });
          console.info(`${this.TAG} aboutToAppear:castTempForm end`);
        }

        if ((want.parameters?.optType ?? 0) == 2 && this.deleteIds.length > 0) {
          for (let iIndex = 0; iIndex < this.deleteIds.length; ++iIndex) {
            console.info(`${this.TAG} aboutToAppear:formsystemhostp deleteForm start`);
            formHost.deleteForm(this.deleteIds[iIndex])
              .then((data) => {
                console.info(`${this.TAG} formsystemhostp deleteForm result:` + data);
                this.delCount++;

                if (this.delCount == this.deleteIds.length) {
                  let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: data + "",
                    bundleName: "com.ohos.st.formsystemhostp",
                    parameters: {
                      "formIds": this.deleteIds
                    }
                  };
                  commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishDeleteCallBack);
                }
              })
              .catch((error:BusinessError) => {
                console.info(` ${this.TAG} formsystemhostp deleteForm error:` + error);
                let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                  data: error.code.toString(),
                  bundleName: "com.ohos.st.formsystemhostp",
                  parameters: {
                    "formIds": [this.deleteIds[iIndex]]
                  }
                };
                commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishOnErrorCallBack);
              });
            console.info(`${this.TAG} aboutToAppear:formsystemhostp deleteForm end`);
          }
        }
      })
      .catch((error:BusinessError) => {
        console.error(` ${this.TAG} formsystemhostp Operation failed. Cause: ` + JSON.stringify(error));
      })
    console.info(`${this.TAG} aboutToAppear end`);
  }

  private makeCount() {
    console.info(` ${this.TAG} formsystemhostp makeCount start.`);
    for (let nNormal = 0; nNormal < this.normal; nNormal++) {
      this.normalFormCounts.push(nNormal);
    }
    console.info(` ${this.TAG} formsystemhostp this.normalFormCounts.length:` + this.normalFormCounts.length);

    for (let nTemp = 0; nTemp < this.temporary; nTemp++) {
      this.tempFormCounts.push(nTemp);
    }
    console.info(` ${this.TAG} formsystemhostp this.tempFormCounts.length:` + this.tempFormCounts.length);
  }

  build() {
    Scroll(new Scroller()) {
      Column() {
        if (this.optType == 1) {
          Text(' [FormComponent] formsystemhostp normal form component test begin')
          Grid() {
            ForEach(this.normalFormCounts, (item:number) => {
              GridItem() {
                FormComponent({
                  id: 0,
                  name: this.name,
                  bundle: this.bundle,
                  ability: this.ability,
                  module: this.moduleName,
                  dimension: this.dimension,
                  temporary: false
                })
                  .allowUpdate(this.allowUpate)
                  .visibility(this.isShowing ? Visibility.Visible : Visibility.Hidden)
                  .onAcquired((form:InfoType) => {
                    console.info(` ${this.TAG} formsystemhostp get normal form, form id:` + form.id);

                    this.normalFormIds.push(form.id.toString());
                    if (this.normalFormIds.length == this.normal - this.normaldel) {
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "normalForm",
                        bundleName: "com.ohos.st.formsystemhostp",
                        parameters: {
                          "formIds": this.normalFormIds
                        }
                      };
                      setTimeout(()=>{
                        console.log(`${this.TAG} publish formOnAcquiredEvent:` + this.formOnAcquiredEvent);
                        commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, this.publishOnAcquiredCallBack);
                      }, 500);
                    }
                  })
                  .onError((error:ErroType) => {
                    console.info(` ${this.TAG} formsystemhostp normal form error code:` + error.errcode);
                    console.info(` ${this.TAG} formsystemhostp normal form error msg:` + error.msg);
                    let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                      data: error.msg,
                      bundleName: "com.ohos.st.formsystemhostp"
                    };
                    commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishOnErrorCallBack);
                  })
                  .size({ width: 40, height: 40 })
              }
            }, (item:number) => JSON.stringify(item))
          }
          .columnsTemplate('1fr 1fr 1fr 1fr 1fr 1fr 1fr 1fr 1fr 1fr ')
            .columnsGap(2)
            .rowsGap(2)
            .width('100%')
          Text(' [FormComponent] formsystemhostp normal form component test end')

          Text(' [FormComponent] formsystemhostp temp form component test start')
          Grid() {
            ForEach(this.tempFormCounts, (item:number) => {
              GridItem() {
                FormComponent({
                  id: 0,
                  name: this.name,
                  bundle: this.bundle,
                  ability: this.ability,
                  module: this.moduleName,
                  dimension: this.dimension,
                  temporary: true
                })
                  .allowUpdate(this.allowUpate)
                  .visibility(this.isShowing ? Visibility.Visible : Visibility.Hidden)
                  .onAcquired((form:InfoType) => {
                    console.info(` ${this.TAG} formsystemhostp get temp form, form id:` + form.id);
                    this.tempFormIds.push(form.id.toString());
                    if (this.tempFormIds.length == this.temporary - this.temporarydel) {
                      let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                        data: "tempForm",
                        bundleName: "com.ohos.st.formsystemhostp",
                        parameters: {
                          "formIds": this.tempFormIds
                        }
                      };
                      setTimeout(()=>{
                        console.log(`${this.TAG} publish formOnAcquiredEvent:` + this.formOnAcquiredEvent);
                        commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, this.publishOnAcquiredCallBack);
                      }, 500);
                    }
                  })
                  .onError((error:ErroType) => {
                    console.info(` ${this.TAG} formsystemhostp temp form error code:` + error.errcode);
                    console.info(` ${this.TAG} formsystemhostp temp form error msg:` + error.msg);
                    let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                      data: error.msg,
                      bundleName: "com.ohos.st.formsystemhostp"
                    };
                    commonEvent.publish(this.formOnErrorEvent, commonEventPublishData, this.publishOnErrorCallBack);
                  })
                  .size({ width: 40, height: 40 })
              }
            }, (item:number) => JSON.stringify(item))
          }
          .columnsTemplate('1fr 1fr 1fr 1fr 1fr 1fr 1fr 1fr 1fr 1fr ')
            .columnsGap(2)
            .rowsGap(2)
            .width('100%')

          Text(' [FormComponent] formsystemhostp temp form component test end')
        }
      }
      .backgroundColor(Color.White)
    }
    .scrollable(ScrollDirection.Vertical)
      .width('100%')
      .height('100%')
  }
}
