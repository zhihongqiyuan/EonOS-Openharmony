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
  private subscriberDel:CommonEventManager.CommonEventSubscriber|null = null;
  private subscriberFormDeleteEvent:CommonEventManager.CommonEventSubscribeInfo = {
    events: ["FMS_FormDelete_commonEvent"]
  };
  private TAG = `[FormComponent.host.l]`;
  private formOnAcquiredEvent = `FMS_FormOnAcquired_commonEvent`;
  private formOnRequestEvent = `FMS_FormOnRequest_commonEvent`;
  private formOnDeletedEvent = "FMS_FormOnDeleted_commonEvent";

  private onAcquiredCallback = () => {
    console.info(`====>${this.TAG} onAcquiredCallback====>`);
  }

  private onRequestCallback = () => {
    console.info(`====>${this.TAG} onRequestCallback====>`);
  }

  private unsubscribeDeleteCallback() {
    console.info(`${this.TAG} unSubscribeDeleteCallback====>`);
  }

  async aboutToAppear() {
    console.log(`${this.TAG} aboutToAppear start`);
    const onDeleteEventSubscriber = await commonEvent.createSubscriber(this.subscriberFormDeleteEvent);
    commonEvent.subscribe(onDeleteEventSubscriber, async (err, data) => {
      console.info(`${this.TAG} deleteCallBack start:====> ${JSON.stringify(data)}`);
      
      data.parameters?.formIds.forEach(async (formId:string) => {
        await formHost.deleteForm(formId)
          .then((res) => {
            console.info(`${this.TAG} deleteForm result: ${JSON.stringify(res)}`);
          });
      });

      commonEvent.unsubscribe(onDeleteEventSubscriber, () => this.unsubscribeDeleteCallback());
      console.info(`${this.TAG} deleteCallBack end ====>`);
    });

    console.info(`${this.TAG} aboutToAppear:getWant start`);
    featureAbility.getWant()
      .then((want: Want) => {
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
          this.requestForm = want.parameters.requestForm;
        }
        if (want.parameters) {
          this.requestId = want.parameters.requestId;
        }
        if (want.parameters) {
          this.canCreateForm = want.parameters.isCreate;
        }

        if (!this.canCreateForm && this.requestForm) {
          console.log(`${this.TAG} aboutToAppear: requestForm start`);
          let commonEventPublishData:CommonEventManager.CommonEventPublishData | null = null;
          formHost.requestForm(this.requestId)
            .then((data) => {
              console.info(`${this.TAG} requestForm result: ${JSON.stringify(data)}`);
              commonEventPublishData = {
                data: data + "",
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
          console.log(`${this.TAG} aboutToAppear: requestForm end`);
        }
      })
      .catch((error:BusinessError) => {
        console.error(`${this.TAG} Operation failed. Cause: ${JSON.stringify(error)}`);
      })
    console.log(`${this.TAG} aboutToAppear end`);
  }

  build() {
    Column() {
      Text(`form component test begin`);
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
            .onAcquired((form:InfoType) => {
              console.log(`${this.TAG} onAcquired start`);
              console.info(`${this.TAG} onAcquired, form id: ${form.id}`);
              this.formId = form.id;
              const commonEventPublishData:CommonEventManager.CommonEventPublishData = {
                data: this.formId.toString()
              };
              setTimeout(()=>{
                console.log(`${this.TAG} publish formOnAcquiredEvent:` + this.formOnAcquiredEvent);
                commonEvent.publish(this.formOnAcquiredEvent, commonEventPublishData, this.onAcquiredCallback);
              }, 500);
              console.log(`${this.TAG} onAcquired end`);
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
