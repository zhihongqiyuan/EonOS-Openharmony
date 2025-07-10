/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

import {FunctionIntentDecoratorInfo,InsightIntentFunction, InsightIntentFunctionMethod} from "@ohos.app.ability.InsightIntentDecorator";

const validDataFunc: FunctionIntentDecoratorInfo = {
  intentName: 'ViewYourHisAccount',
  domain: "game",
  intentVersion:'1.0.1',
  displayName: '查看我的账户我是个天才',
  displayDescription: '查看账户信息，包括余额、消费明细等信息',
  schema: "ControlPlayback",
};
@Entry
@Component
@InsightIntentFunction()
struct Index {
  @InsightIntentFunctionMethod(validDataFunc)
  static function(playbackProgress:number, entityId?: number){
  }
  build() {
    Row() {
    }
  }
}