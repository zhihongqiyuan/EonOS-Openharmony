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

import featureAbility from '@ohos.ability.featureAbility';
import formHost from '@ohos.application.formHost';
import commonEvent from '@ohos.commonEvent';
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
  @State bundle: string = `com.acts.form.formsystemtestservicea.hmservice`;
  @State ability: string = `com.acts.form.formsystemtestservicea.hmservice.MainAbility`;
  @State moduleName: string = `entry`;
  @State name: string = `Form_Js001`;
  @State canCreateForm: boolean = false;
  @State allowUpdate: boolean = true;
  @State isShowing: boolean = true;
  @State dimension: FormDimension = FormDimension.Dimension_1_2;
  @State temporary: boolean = false;
  private requestForm = false;
  private requestId = `-1`;
  private temporaryId = `-1`;
  private deleteForm = false;
  private deleteId = `-1`;
  private castTempForm = false;
  private notifyVisibility = false;
  private notifyVisibleForms = false;
  private notifyInvisibleForms = false;
  private releaseForm = false;
  private releaseId = "-1";
  private update = false;
  private enableUpdate = false;
  private disableUpdate = false;
  private isVisible = false;
  private castForm = false;
  private tempFormId = 0;
  private createTempAndNormal = false;
  //private tempParam = {};
  private createSecondForm = false;
  //private secondParam = {};
  private subscriberDel:CommonEventManager.CommonEventSubscriber|null = null;
  private subscriberFormDeleteEvent:CommonEventManager.CommonEventSubscribeInfo = {
    events: ["FMS_FormDelete_commonEvent"]
  };
  private subscriberFormTerminateEvent:CommonEventManager.CommonEventSubscribeInfo = {
    events: ["FMS_FormTerminate_commonEvent"]
  };
  private TAG = `[FormComponent.host.k]`;

  private formOnAcquiredEvent = "FMS_FormOnAcquired_commonEvent";
  private formOnRequestEvent = "FMS_FormOnRequest_commonEvent";
  private formOnDeletedEvent = "FMS_FormOnDeleted_commonEvent";
  private formOnReleasedEvent = "FMS_FormOnReleased_commonEvent";

  private onAcquiredCallback = () => {
    console.info(`${this.TAG} onAcquiredCallback`);
  }

  private onRequestCallback = () => {
    console.info(`====>${this.TAG} onRequestCallback====>`);
  }

  private onDeletedCallback = () => {
    console.info(`${this.TAG} onDeletedCallback`);
  }

  private onReleasedCallback = () => {
    console.info(`====>${this.TAG} onReleasedCallback====>`);
  }

  private unsubscribeDeleteCallback() {
    console.info(`${this.TAG} unSubscribeDeleteCallback`);
  }

  async aboutToAppear() {
    const onDeleteEventSubscriber = await commonEvent.createSubscriber(this.subscriberFormDeleteEvent);
    const onTerminateEventSubscriber = await commonEvent.createSubscriber(this.subscriberFormTerminateEvent);
    commonEvent.subscribe(onDeleteEventSubscriber,
      async (err:BusinessError, data:CommonEventManager.CommonEventData) => {
      const formIds: string[] = data.parameters?.formIds ?? [];
      console.info(`${this.TAG} deleteCallback data: ${JSON.stringify(data)}, error: ${JSON.stringify(err)}`);
      console.info(`${this.TAG} deleteCallback formIds: ${JSON.stringify(formIds)}`);

      commonEvent.unsubscribe(onDeleteEventSubscriber, () => this.unsubscribeDeleteCallback());
      let deletedFormAmount = 0;
      if (formIds.length == 0) {
        return;
      }
      formIds.forEach(async (formId:string, index:number) => {
        try {
          const res = await formHost.deleteForm(formId);
          ++deletedFormAmount;
          console.info(`${this.TAG} deleteForm${index} result: ${JSON.stringify(res)}`);
        } catch (error) {
          console.info(`${this.TAG} deleteForm${index} error: ${JSON.stringify(error)}`);
        }

        if (index >= formIds.length - 1) {
          const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
            parameters: {
              deletedFormAmount: deletedFormAmount,
              formIds: [].concat(data.parameters?.formIds ?? 0)
            }
          };
          commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, () => this.onDeletedCallback());
          console.info(`${this.TAG} deleteCallback end, deletedFormAmount ${deletedFormAmount}`);
        }
      });
    });

    featureAbility.getWant()
      .then(async (want: Want) => {
        console.info(`${this.TAG} getWant data: ${JSON.stringify(want)}`);
        this.formId = parseInt(want.parameters?.formId ?? 0);
        this.name = want.parameters?.name ?? "";
        this.bundle = want.parameters?.bundle ?? "";
        this.ability = want.parameters?.ability ?? "";
        if (want.parameters) {
          this.moduleName = want.parameters.moduleName;
        }
        if (want.parameters) {
          this.temporary = want.parameters.temporary;
        }
        if (want.parameters) {
          this.dimension = want.parameters.dimension;
        }
        if (want.parameters) {
          this.castForm = want.parameters.castForm;
        }
        if (want.parameters) {
          this.temporaryId = want.parameters.temporaryId;
        }
        if (want.parameters) {
          this.requestForm = want.parameters.requestForm;
        }
        if (want.parameters) {
          this.requestId = want.parameters.requestId;
        }
        if (want.parameters) {
          this.createSecondForm = want.parameters.createSecondForm;
          console.info(`====${this.TAG} onRequestCallback123====>` + JSON.stringify(want.parameters.secondParam));
        }
        if (want.parameters) {
          this.createTempAndNormal = want.parameters.createTempAndNormal;
        }
        if (want.parameters) {
          this.deleteForm = want.parameters.deleteForm;
        }
        if (want.parameters) {
          this.deleteId = want.parameters.deleteId;
        }
        if (want.parameters) {
          this.castTempForm = want.parameters.castTempForm;
        }
        if (want.parameters) {
          this.notifyVisibility = want.parameters.notifyVisibility;
        }
        if (want.parameters) {
          this.notifyVisibleForms = want.parameters.notifyVisibleForms;
        }
        if (want.parameters) {
          this.notifyInvisibleForms = want.parameters.notifyInvisibleForms;
        }
        if (want.parameters) {
          this.releaseForm = want.parameters.releaseForm;
        }
        if (want.parameters) {
          this.releaseId = want.parameters.releaseId;
        }
        if (want.parameters) {
          this.update = want.parameters.update;
        }
        if (want.parameters) {
          this.enableUpdate = want.parameters.enableUpdate;
        }
        if (want.parameters) {
          this.disableUpdate = want.parameters.disableUpdate;
        }
        if (want.parameters) {
          commonEvent.subscribe(onTerminateEventSubscriber, async (err, data) => {
            console.info(`${this.TAG} onTerminateCallback data: ${JSON.stringify(data)}, error: ${JSON.stringify(err)}`);
            commonEvent.unsubscribe(onTerminateEventSubscriber, () => { });
            console.info(`${this.TAG} onTerminateCallback end`);
            setTimeout(async () => {
              // terminate self
              featureAbility.terminateSelf();
              console.info(`${this.TAG} featureAbility.terminateSelf`);
            }, 1000);
          });
        }
        if (want.parameters) {
          this.canCreateForm = want.parameters.isCreate;
        }

        if (!(want.parameters?.isCreate ?? false) && this.deleteForm) {
          console.log(`${this.TAG} deleteForm start`);
          formHost.deleteForm(this.deleteId)
            .then((data) => {
              console.info(`${this.TAG} deleteForm result: ${JSON.stringify(data)}`);
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: (data + "") || "0",
                parameters: {
                  "formId": this.deleteId
                }
              };
              commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.onDeletedCallback);
            })
            .catch((error:BusinessError) => {
              console.info(`${this.TAG} deleteForm error: ${JSON.stringify(error)}`);
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: error.code.toString(),
                parameters: {
                  "formId": this.deleteId
                }
              };
              commonEvent.publish(this.formOnDeletedEvent, commonEventPublishData, this.onDeletedCallback);
            });
        }

        if (!(want.parameters?.isCreate ?? false) && this.releaseForm) {
          console.log(`${this.TAG} releaseForm start`);
          formHost.releaseForm(this.releaseId, true)
            .then((data) => {
              console.info(`${this.TAG} releaseForm result: ${JSON.stringify(data)}`);
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: (data + "") || "0",
                parameters: {
                  "formId": this.releaseId
                }
              };
              commonEvent.publish(this.formOnReleasedEvent, commonEventPublishData, this.onReleasedCallback);
            })
            .catch((error:BusinessError) => {
              console.info(`${this.TAG} releaseForm error: ${JSON.stringify(error)}`);
              let commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: error.code.toString(),
                parameters: {
                  "formId": this.releaseId
                }
              };
              commonEvent.publish(this.formOnReleasedEvent, commonEventPublishData, this.onReleasedCallback);
            });
        }

        if (!this.canCreateForm && this.requestForm) {
          let commonEventPublishData:CommonEventManager.CommonEventPublishData;
          formHost.requestForm(this.requestId)
            .then((data) => {
              console.info(`${this.TAG} requestForm result: ${JSON.stringify(data)}`);
              commonEventPublishData = {
                data: (data + "") || "0",
                parameters: {
                  "formId": this.requestId
                }
              };
              commonEvent.publish(this.formOnRequestEvent, commonEventPublishData, this.onRequestCallback);
            })
            .catch((error:BusinessError) => {
              console.info(`${this.TAG} requestForm error: ${JSON.stringify(error)}`);
              commonEventPublishData = {
                parameters: {
                  "error": error
                }
              };
              commonEvent.publish(this.formOnRequestEvent, commonEventPublishData, this.onRequestCallback);
            });
        }
      })
      .catch((error:BusinessError) => {
        console.error(`${this.TAG} Operation failed. Cause: ${JSON.stringify(error)}`);
      })
  }

  build() {
    Column() {
      Text(`form component test begin`)
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
            .allowUpdate(this.allowUpdate)
            .visibility(this.isShowing ? Visibility.Visible : Visibility.Hidden)
            .onAcquired(async (form:InfoType) => {
              const createdTime = await systemTime.getCurrentTime();
              this.formId = form.id;
              console.info(`${this.TAG} get form, form id: ${form.id}`);
              if (this.deleteForm) {
                try {
                  const startTime = await systemTime.getCurrentTime();
                  let res = await formHost.deleteForm(this.formId.toString());
                  const endTime = await systemTime.getCurrentTime();
                  console.info(`${this.TAG} deleteForm result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `deleteForm`,
                      formId: this.formId,
                      startTime: startTime,
                      hostTime: endTime - startTime
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} deleteForm error: ${JSON.stringify(error)}`);
                }
              } else if (this.castTempForm) {
                try {
                  const startTime = await systemTime.getCurrentTime();
                  let res = await formHost.castTempForm(this.formId.toString());
                  const endTime = await systemTime.getCurrentTime();
                  console.info(`${this.TAG} castTempForm result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `castTempForm`,
                      formId: this.formId,
                      startTime: startTime,
                      hostTime: endTime - startTime
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} castTempForm error: ${JSON.stringify(error)}`);
                }
              } else if (this.notifyVisibility) {
                try {
                  let res = await formHost.notifyVisibleForms([this.formId.toString()]);
                  console.info(`${this.TAG} notifyVisibleForms result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `notifyVisibleForms`,
                      formId: this.formId,
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} notifyVisibleForms error: ${JSON.stringify(error)}`);
                }
                try {
                  let res = await formHost.notifyInvisibleForms([this.formId.toString()]);
                  console.info(`${this.TAG} notifyInvisibleForms result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `notifyInvisibleForms`,
                      formId: this.formId,
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} notifyInvisibleForms error: ${JSON.stringify(error)}`);
                }
              } else if (this.notifyVisibleForms) {
                try {
                  const startTime = await systemTime.getCurrentTime();
                  let res = await formHost.notifyVisibleForms([this.formId.toString()]);
                  const endTime = await systemTime.getCurrentTime();
                  console.info(`${this.TAG} notifyVisibleForms result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `notifyVisibleForms`,
                      formId: this.formId,
                      startTime: startTime,
                      hostTime: endTime - startTime
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} notifyVisibleForms error: ${JSON.stringify(error)}`);
                }
              } else if (this.notifyInvisibleForms) {
                try {
                  const startTime = await systemTime.getCurrentTime();
                  let res = await formHost.notifyInvisibleForms([this.formId.toString()]);
                  const endTime = await systemTime.getCurrentTime();
                  console.info(`${this.TAG} notifyInvisibleForms result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `notifyInvisibleForms`,
                      formId: this.formId,
                      startTime: startTime,
                      hostTime: endTime - startTime
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} notifyInvisibleForms error: ${JSON.stringify(error)}`);
                }
              } else if (this.requestForm) {
                let sleep = async (delay: number = 1000): Promise<void> => {
                  return new Promise((resolve, _) => {
                    setTimeout(async () => {resolve();
                    }, delay);
                  });
                };
                await sleep(500);

                try {
                  const startTime = await systemTime.getCurrentTime();
                  let res = await formHost.requestForm(this.formId.toString());
                  const endTime = await systemTime.getCurrentTime();
                  console.info(`${this.TAG} requestForm result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `request`,
                      formId: this.formId,
                      startTime: startTime,
                      hostTime: endTime - startTime
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} requestForm error: ${JSON.stringify(error)}`);
                }
              } else if (this.releaseForm) {
                try {
                  const startTime = await systemTime.getCurrentTime();
                  let res = await formHost.releaseForm(this.formId.toString());
                  const endTime = await systemTime.getCurrentTime();
                  console.info(`${this.TAG} releaseForm result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `release`,
                      formId: this.formId,
                      hostTime: endTime - startTime
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} releaseForm error: ${JSON.stringify(error)}`);
                }
              } else if (this.update) {
                try {
                  let res = await formHost.enableFormsUpdate([this.formId.toString()]);
                  console.info(`${this.TAG} enableUpdate result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `enableUpdate`,
                      formId: this.formId,
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} enableUpdate error: ${JSON.stringify(error)}`);
                }
                try {
                  let res = await formHost.disableFormsUpdate([this.formId.toString()]);
                  console.info(`${this.TAG} disableUpdate result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `disableUpdate`,
                      formId: this.formId,
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} disableUpdate error: ${JSON.stringify(error)}`);
                }
              } else if (this.enableUpdate) {
                try {
                  const startTime = await systemTime.getCurrentTime();
                  let res = await formHost.enableFormsUpdate([this.formId.toString()]);
                  const endTime = await systemTime.getCurrentTime();
                  console.info(`${this.TAG} enableUpdate result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                    data: (res + "") || "0",
                    parameters: {
                      kind: `enableUpdate`,
                      formId: this.formId,
                      hostTime: endTime - startTime
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} enableUpdate error: ${JSON.stringify(error)}`);
                }
              } else if (this.disableUpdate) {
                try {
                  const startTime = await systemTime.getCurrentTime();
                  const res = await formHost.disableFormsUpdate([this.formId.toString()]);
                  const endTime = await systemTime.getCurrentTime();
                  console.info(`${this.TAG} disableUpdate result: ${JSON.stringify(res)}`);
                  const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                   data: (res + "") || "0",
                    parameters: {
                      kind: `disableUpdate`,
                      formId: this.formId,
                      hostTime: endTime - startTime
                    }
                  };
                  commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
                } catch (error) {
                  console.info(`${this.TAG} disableUpdate error: ${JSON.stringify(error)}`);
                }
              } else {
                const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                  data: this.formId.toString(),
                  parameters: {
                    formId: this.formId.toString(),
                    time: createdTime
                  }
                };
                commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
              }
            })
            .onUninstall((info:InfoType) => {
              console.info(`${this.TAG} onUninstall: ${JSON.stringify(info)}`);
              const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                parameters: {
                  formId: info.id.toString(),
                  kind: "onUninstall"
                }
              };
              commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
            })
            .onError((error:ErroType) => {
              console.error(`${this.TAG} onError: ${JSON.stringify(error)}`);
            })
        }
        if (this.createSecondForm) {
          FormComponent({
            id: this.formId,
            name: this.name,
            bundle: this.bundle,
            ability: this.ability,
            module: this.moduleName,
            dimension: this.dimension,
            temporary: this.temporary,
          })
            .allowUpdate(this.allowUpdate)
            .visibility(this.isShowing ? Visibility.Visible : Visibility.Hidden)
            .onAcquired((form:InfoType) => {
              console.info(`${this.TAG} secondForm onAcquired, form id: ${form.id}`);
              this.formId = form.id;
              const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: this.formId.toString(),
                parameters: {
                  "formId": this.formId.toString()
                }
              };
              commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
            })
            .onUninstall((info:InfoType) => {
              console.info(`${this.TAG} onUninstall: ${JSON.stringify(info)}`);
            })
            .onError((error:ErroType) => {
              console.error(`${this.TAG} onError: ${JSON.stringify(error)}`);
            });
        }
        if (this.createTempAndNormal) {
          FormComponent({
            id: this.formId,
            name: this.name,
            bundle: this.bundle,
            ability: this.ability,
            module: this.moduleName,
            dimension: this.dimension,
            temporary: this.temporary,
          })
            .allowUpdate(this.allowUpdate)
            .visibility(this.isShowing ? Visibility.Visible : Visibility.Hidden)
            .onAcquired((form:InfoType) => {
              console.info(`${this.TAG} castForm onAcquired, form id: ${form.id}`);
              this.formId = form.id;
              const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: this.formId.toString(),
                parameters: {
                  "formId": this.formId.toString(),
                  "cast": true,
                }
              };
              commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, () => this.onAcquiredCallback());
            })
            .onUninstall((info:InfoType) => {
              console.info(`${this.TAG} onUninstall: ${JSON.stringify(info)}`);
            })
            .onError((error:ErroType) => {
              console.error(`${this.TAG} onError: ${JSON.stringify(error)}`);
            });
        }
      }
      .backgroundColor(Color.White)

      Text(`form component test end`);
    }
    .backgroundColor(Color.White)
  }
}
