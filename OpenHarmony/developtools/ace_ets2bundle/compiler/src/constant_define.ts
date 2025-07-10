/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

const NAVIGATION_BUILDER_REGISTER: string = 'NavigationBuilderRegister';

const MONITOR: string = 'Monitor';
const COMPUTED: string = 'Computed';
const REQUIRE: string = 'Require';
const BUILDER_PARAM: string = 'BuilderParam';
const DECORATOR_BUILDER_PARAM: string = '@BuilderParam';
const MONITOR_DECORATOR: string = '@Monitor';
const COMPUTED_DECORATOR: string = '@Computed';
const EVENT_DECORATOR: string = '@Event';

const COMPONENT_MEMBER_DECORATOR_V1: string[] = [
  '@State', '@Prop', '@Link', '@Provide', '@Consume', '@Watch', '@StorageLink', '@StorageProp',
  '@LocalStorageLink', '@LocalStorageProp', '@ObjectLink'
];

const COMPONENT_MEMBER_DECORATOR_V2: string[] = [
  '@Local', '@Param', '@Once', '@Event', '@Provider', '@Consumer'
];
const STRUCT_CLASS_MEMBER_DECORATOR_V2: string[] = [
  '@Monitor', '@Computed'
];
const DECORATOR_V2: string[] = [
  ...COMPONENT_MEMBER_DECORATOR_V2, ...STRUCT_CLASS_MEMBER_DECORATOR_V2
];

const STRUCT_PARENT: string = 'ViewV2';
const INIT_PARAM: string = 'initParam';
const UPDATE_PARAM: string = 'updateParam';
const RESET_PARAM: string = 'resetParam';
const UPDATE_STATE_VARS: string = 'updateStateVars';
const RESET_STATE_VARS_METHOD: string = 'resetStateVarsOnReuse';
const RESET_CONSUMER: string = 'resetConsumer';
const RESET_COMPUTED: string = 'resetComputed';
const RESET_MONITORS_ON_REUSE: string = 'resetMonitorsOnReuse';
const OBJECT_TYPE: string = 'Object';
const REUSE_OR_CREATE_METHOD: string = 'reuseOrCreateNewComponent';
const COMPONENT_CLASS: string = 'componentClass';
const GET_PARAMS: string = 'getParams';
const GET_REUSE_ID: string = 'getReuseId';
const EXTRA_INFO: string = 'extraInfo';

const BYTE_CODE_HAR: string = 'byteCodeHar';
const CLOSED_SOURCE_HAR: string = 'closedSourceHar';
const SHARED_HSP: string = 'sharedHsp';
const MODULE_TYPE: string = 'moduleType';
const HAR_DEFAULT_PAGE_PATH: string = '__harDefaultPagePath__';
const HAR_DEFAULT_INTEGRATED_HSP_TYPE: string = '__harDefaultIntegratedHspType__';
const FOLLOW_WITH_HAP: string = 'followWithHap';

export default {
  NAVIGATION_BUILDER_REGISTER,
  MONITOR,
  COMPUTED,
  STRUCT_PARENT,
  COMPONENT_MEMBER_DECORATOR_V1,
  COMPONENT_MEMBER_DECORATOR_V2,
  INIT_PARAM,
  UPDATE_PARAM,
  RESET_PARAM,
  UPDATE_STATE_VARS,
  REQUIRE,
  MONITOR_DECORATOR,
  COMPUTED_DECORATOR,
  DECORATOR_V2,
  BUILDER_PARAM,
  DECORATOR_BUILDER_PARAM,
  BYTE_CODE_HAR,
  CLOSED_SOURCE_HAR,
  SHARED_HSP,
  MODULE_TYPE,
  HAR_DEFAULT_PAGE_PATH,
  HAR_DEFAULT_INTEGRATED_HSP_TYPE,
  FOLLOW_WITH_HAP,
  EVENT_DECORATOR,
  RESET_STATE_VARS_METHOD,
  RESET_CONSUMER,
  RESET_COMPUTED,
  RESET_MONITORS_ON_REUSE,
  OBJECT_TYPE,
  REUSE_OR_CREATE_METHOD,
  COMPONENT_CLASS,
  GET_PARAMS,
  GET_REUSE_ID,
  EXTRA_INFO
};
