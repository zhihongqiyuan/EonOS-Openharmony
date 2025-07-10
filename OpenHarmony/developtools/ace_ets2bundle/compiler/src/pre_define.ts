/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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

import path from 'path';
import { EXT_WHITE_LIST } from './component_map';

export const NATIVE_MODULE: Set<string> = new Set(
  ['system.app', 'ohos.app', 'system.router', 'system.curves', 'ohos.curves', 'system.matrix4', 'ohos.matrix4']);
export const VALIDATE_MODULE: string[] = ['application', 'util', 'screen', 'mediaquery'];
export const SYSTEM_PLUGIN: string = 'system';
export const OHOS_PLUGIN: string = 'ohos';
// 'arkui-x' represents cross platform related APIs, processed as 'ohos'
export const ARKUI_X_PLUGIN: string = 'arkui-x';

export const COMPONENT_DECORATOR_ENTRY: string = '@Entry';
export const COMPONENT_DECORATOR_PREVIEW: string = '@Preview';
export const COMPONENT_DECORATOR_COMPONENT: string = '@Component';
export const COMPONENT_DECORATOR_CUSTOM_DIALOG: string = '@CustomDialog';
export const COMPONENT_DECORATOR_REUSEABLE: string = '@Reusable';
export const DECORATOR_REUSEABLE: string = 'Reusable';

export const COMPONENT_RECYCLE: string = '__Recycle__';
export const RECYCLE_REUSE_ID: string = 'reuseId';

export const COMPONENT_NON_DECORATOR: string = 'regular';
export const COMPONENT_STATE_DECORATOR: string = '@State';
export const COMPONENT_PROP_DECORATOR: string = '@Prop';
export const COMPONENT_LINK_DECORATOR: string = '@Link';
export const COMPONENT_STORAGE_PROP_DECORATOR: string = '@StorageProp';
export const COMPONENT_STORAGE_LINK_DECORATOR: string = '@StorageLink';
export const COMPONENT_PROVIDE_DECORATOR: string = '@Provide';
export const COMPONENT_CONSUME_DECORATOR: string = '@Consume';
export const COMPONENT_OBJECT_LINK_DECORATOR: string = '@ObjectLink';
export const COMPONENT_WATCH_DECORATOR: string = '@Watch';
export const COMPONENT_BUILDERPARAM_DECORATOR: string = '@BuilderParam';
export const COMPONENT_LOCAL_STORAGE_LINK_DECORATOR: string = '@LocalStorageLink';
export const COMPONENT_LOCAL_STORAGE_PROP_DECORATOR: string = '@LocalStorageProp';
export const COMPONENT_CUSTOM_DECORATOR: string = 'COMPONENT_CUSTOM_DECORATOR';
export const COMPONENT_REQUIRE_DECORATOR: string = '@Require';

export const COMPONENTV2_LOCAL_DECORATOR: string = '@Local';
export const COMPONENTV2_PARAM_DECORATOR: string = '@Param';
export const COMPONENTV2_ONCE_DECORATOR: string = '@Once';
export const COMPONENTV2_EVENT_DECORATOR: string = '@Event';
export const COMPONENTV2_CONSUMER_DECORATOR: string = '@Consumer';
export const COMPONENTV2_PROVIDER_DECORATOR: string = '@Provider';

export const CLASS_TRACK_DECORATOR: string = 'Track';
export const CLASS_MIN_TRACK_DECORATOR: string = 'Trace';
export const COMPONENT_DECORATOR_COMPONENT_V2: string = '@ComponentV2';
export const COMPONENT_DECORATOR_REUSABLE_V2: string = '@ReusableV2';
export const DECORATOR_REUSABLE_V2: string = 'ReusableV2';
export const REUSABLE_V2_INNER_DECORATOR: string = '__ReusableV2_Inner_Decorator__';
export const REUSE_ATTRIBUTE: string = 'reuse';

export const COMPONENT_DECORATORS_PARAMS: Set<string> = new Set([COMPONENT_CONSUME_DECORATOR,
  COMPONENT_STORAGE_PROP_DECORATOR, COMPONENT_STORAGE_LINK_DECORATOR, COMPONENT_PROVIDE_DECORATOR,
  COMPONENT_WATCH_DECORATOR]);
export const INNER_COMPONENT_DECORATORS: Set<string> = new Set([COMPONENT_DECORATOR_ENTRY,
  COMPONENT_DECORATOR_PREVIEW, COMPONENT_DECORATOR_COMPONENT, COMPONENT_DECORATOR_CUSTOM_DIALOG,
  COMPONENT_DECORATOR_REUSEABLE, COMPONENT_DECORATOR_COMPONENT_V2, COMPONENT_DECORATOR_REUSABLE_V2]);
export const INNER_COMPONENT_MEMBER_DECORATORS: Set<string> = new Set([COMPONENT_STATE_DECORATOR,
  COMPONENT_PROP_DECORATOR, COMPONENT_LINK_DECORATOR, COMPONENT_STORAGE_PROP_DECORATOR,
  COMPONENT_STORAGE_LINK_DECORATOR, COMPONENT_PROVIDE_DECORATOR, COMPONENT_CONSUME_DECORATOR,
  COMPONENT_OBJECT_LINK_DECORATOR, COMPONENT_WATCH_DECORATOR, COMPONENT_BUILDERPARAM_DECORATOR,
  COMPONENT_LOCAL_STORAGE_LINK_DECORATOR, COMPONENT_LOCAL_STORAGE_PROP_DECORATOR,
  COMPONENT_REQUIRE_DECORATOR]);
export const STRUCT_DECORATORS: Set<string> = new Set([...INNER_COMPONENT_DECORATORS,
  ...INNER_COMPONENT_MEMBER_DECORATORS]);

export const COMPONENT_OBSERVED_DECORATOR: string = '@Observed';
export const COMPONENT_OBSERVEDV2_DECORATOR: string = '@ObservedV2';
export const OBSERVED: string = 'Observed';
export const MIN_OBSERVED: string = 'ObservedV2';
export const SENDABLE: string = 'Sendable';
export const TYPE: string = 'Type';
export const COMPONENT_BUILDER_DECORATOR: string = '@Builder';
export const COMPONENT_LOCAL_BUILDER_DECORATOR: string = '@LocalBuilder';
export const COMPONENT_EXTEND_DECORATOR: string = '@Extend';
export const COMPONENT_STYLES_DECORATOR: string = '@Styles';
export const COMPONENT_ANIMATABLE_EXTEND_DECORATOR: string = '@AnimatableExtend';
export const COMPONENT_CONCURRENT_DECORATOR: string = '@Concurrent';
export const COMPONENT_SENDABLE_DECORATOR: string = '@Sendable';
export const COMPONENT_USER_INTENTS_DECORATOR: string = '@InsightIntentLink';
export const COMPONENT_USER_INTENTS_DECORATOR_ENTRY: string = '@InsightIntentEntry';
export const COMPONENT_USER_INTENTS_DECORATOR_FUNCTION: string = '@InsightIntentFunction';
export const COMPONENT_USER_INTENTS_DECORATOR_METHOD: string = '@InsightIntentFunctionMethod';
export const COMPONENT_USER_INTENTS_DECORATOR_PAGE: string = '@InsightIntentPage';
export const CHECK_COMPONENT_EXTEND_DECORATOR: string = 'Extend';
export const STRUCT_CONTEXT_METHOD_DECORATORS: Set<string> = new Set([COMPONENT_BUILDER_DECORATOR,
  COMPONENT_STYLES_DECORATOR, COMPONENT_LOCAL_BUILDER_DECORATOR]);
export const CHECK_COMPONENT_ANIMATABLE_EXTEND_DECORATOR: string = 'AnimatableExtend';
export const EXTEND_DECORATORS: string[] = [
  COMPONENT_EXTEND_DECORATOR,
  COMPONENT_ANIMATABLE_EXTEND_DECORATOR
];
export const CHECK_EXTEND_DECORATORS: string[] = [
  CHECK_COMPONENT_EXTEND_DECORATOR,
  CHECK_COMPONENT_ANIMATABLE_EXTEND_DECORATOR
];

export const CREATE_ANIMATABLE_PROPERTY: string = 'createAnimatableProperty';
export const UPDATE_ANIMATABLE_PROPERTY: string = 'updateAnimatableProperty';
export const GET_AND_PUSH_FRAME_NODE: string = 'GetAndPushFrameNode';
export const FINISH_UPDATE_FUNC: string = 'finishUpdateFunc';

export const OBSERVED_PROPERTY_SIMPLE: string = 'ObservedPropertySimple';
export const OBSERVED_PROPERTY_OBJECT: string = 'ObservedPropertyObject';
export const SYNCHED_PROPERTY_SIMPLE_ONE_WAY: string = 'SynchedPropertySimpleOneWay';
export const SYNCHED_PROPERTY_SIMPLE_TWO_WAY: string = 'SynchedPropertySimpleTwoWay';
export const SYNCHED_PROPERTY_OBJECT_TWO_WAY: string = 'SynchedPropertyObjectTwoWay';
export const SYNCHED_PROPERTY_NESED_OBJECT: string = 'SynchedPropertyNesedObject';
export const DECORATOR_TYPE_ANY: string = 'any';

export const INITIALIZE_CONSUME_FUNCTION: string = 'initializeConsume';
export const ADD_PROVIDED_VAR: string = 'addProvidedVar';

export const APP_STORAGE: string = 'AppStorage';
export const APP_STORAGE_SET_AND_PROP: string = 'SetAndProp';
export const APP_STORAGE_SET_AND_LINK: string = 'SetAndLink';

export const PAGE_ENTRY_FUNCTION_NAME: string = 'loadDocument';
export const STORE_PREVIEW_COMPONENTS: string = 'storePreviewComponents';
export const PREVIEW_COMPONENT_FUNCTION_NAME: string = 'previewComponent';
export const GET_PREVIEW_FLAG_FUNCTION_NAME: string = 'getPreviewComponentFlag';

export const COMPONENT_DECORATOR_NAME_COMPONENT: string = 'Component';
export const XCOMPONENT_SINGLE_QUOTATION: string = `'component'`;
export const XCOMPONENT_DOUBLE_QUOTATION: string = `"component"`;
export const XCOMPONENTTYPE: string = 'XComponentType';
export const XCOMPONENTTYPE_CONTAINER: string = 'COMPONENT';
export const COMPONENT_DECORATOR_NAME_CUSTOMDIALOG: string = 'CustomDialog';
export const CUSTOM_DECORATOR_NAME: Set<string> = new Set([
  COMPONENT_DECORATOR_NAME_COMPONENT, COMPONENT_DECORATOR_NAME_CUSTOMDIALOG,
  DECORATOR_REUSEABLE, 'Entry', 'Preview'
]);

export const EXTNAME_ETS: string = '.ets';
export const NODE_MODULES: string = 'node_modules';
export const PACKAGES: string = 'pkg_modules';
export const INDEX_ETS: string = 'index.ets';
export const INDEX_TS: string = 'index.ts';
export const PACKAGE_JSON: string = 'package.json';
export const CUSTOM_COMPONENT_DEFAULT: string = 'default';
export const TS_WATCH_END_MSG: string = 'TS Watch End';

export const BASE_COMPONENT_NAME: string = 'View';
export const STRUCT: string = 'struct';
export const CLASS: string = 'class';
export const COMPONENT_BUILD_FUNCTION: string = 'build';
export const COMPONENT_RENDER_FUNCTION: string = 'render';
export const COMPONENT_TRANSITION_FUNCTION: string = 'pageTransition';
export const COMPONENT_TRANSITION_NAME: string = 'PageTransition';
export const CUSTOM_COMPONENT: string = 'CustomComponent';
export const GLOBAL_THIS_REQUIRE_NATIVE_MODULE: string = 'globalThis.requireNativeModule';
export const GLOBAL_THIS_REQUIRE_NAPI: string = 'globalThis.requireNapi';

export const COMPONENT_BUTTON: string = 'Button';
export const COMPONENT_FOREACH: string = 'ForEach';
export const COMPONENT_LAZYFOREACH: string = 'LazyForEach';
export const COMPONENT_REPEAT: string = 'Repeat';
export const REPEAT_EACH: string = 'each';
export const REPEAT_TEMPLATE: string = 'template';
export const IS_RENDERING_IN_PROGRESS: string = 'isRenderingInProgress';
export const FOREACH_OBSERVED_OBJECT: string = 'ObservedObject';
export const FOREACH_GET_RAW_OBJECT: string = 'GetRawObject';
export const COMPONENT_IF: string = 'If';
export const COMPONENT_IF_BRANCH_ID_FUNCTION: string = 'branchId';
export const COMPONENT_IF_UNDEFINED: string = 'undefined';
export const GLOBAL_CONTEXT: string = 'Context';
export const ATTRIBUTE_ANIMATION: string = 'animation';
export const ATTRIBUTE_ANIMATETO_SET: Set<string> = new Set(['animateTo', 'animateToImmediately']);
export const ATTRIBUTE_STATESTYLES: string = 'stateStyles';
export const ATTRIBUTE_ID: string = 'id';
export const ATTRIBUTE_ATTRIBUTE_MODIFIER: string = 'attributeModifier';
export const ATTRIBUTE_CONTENT_MODIFIER: string = 'contentModifier';
export const ATTRIBUTE_MENUITEM_CONTENT_MODIFIER: string = 'menuItemContentModifier';
export const TRUE: string = 'true';
export const FALSE: string = 'false';
export const NULL: string = 'null';
export const FOREACH_LAZYFOREACH: Set<string> = new Set([
  COMPONENT_FOREACH, COMPONENT_LAZYFOREACH
]);

export const COMPONENT_CONSTRUCTOR_ID: string = 'compilerAssignedUniqueChildId';
export const COMPONENT_CONSTRUCTOR_PARENT: string = 'parent';
export const COMPONENT_CONSTRUCTOR_PARAMS: string = 'params';
export const COMPONENT_PARAMS_FUNCTION: string = 'paramsGenerator_';
export const COMPONENT_PARAMS_LAMBDA_FUNCTION: string = 'paramsLambda';
export const COMPONENT_CONSTRUCTOR_UNDEFINED: string = 'undefined';
export const COMPONENT_CONSTRUCTOR_LOCALSTORAGE: string = 'localStorage';
export const COMPONENT_ABOUTTOREUSEINTERNAL_FUNCTION: string = 'aboutToReuseInternal';
export const COMPONENT_SET_AND_LINK: string = 'setAndLink';
export const COMPONENT_SET_AND_PROP: string = 'setAndProp';

export const BUILD_ON: string = 'on';
export const BUILD_OFF: string = 'off';

export const START: string = 'start';
export const END: string = 'end';

export const COMPONENT_CREATE_FUNCTION: string = 'create';
export const COMPONENT_CREATE_LABEL_FUNCTION: string = 'createWithLabel';
export const COMPONENT_CREATE_CHILD_FUNCTION: string = 'createWithChild';
export const COMPONENT_POP_FUNCTION: string = 'pop';
export const COMPONENT_DEBUGLINE_FUNCTION: string = 'debugLine';
export const COMPONENT_COMMON: string = '__Common__';

export const OBSERVE_RECYCLE_COMPONENT_CREATION: string = 'observeRecycleComponentCreation';
export const COMPONENT_CREATE_RECYCLE: string = 'createRecycle';
export const RECYCLE_NODE: string = 'recycleNode';
export const ABOUT_TO_REUSE: string = 'aboutToReuse';
export const COMPONENT_UPDATE_ELMT_ID: string = 'updateElmtId';
export const UPDATE_RECYCLE_ELMT_ID: string = 'updateRecycleElmtId';
export const OLD_ELMT_ID: string = 'oldElmtId';
export const NEW_ELMT_ID: string = 'newElmtId';
export const NAVIGATION: string = 'Navigation';
export const NAV_DESTINATION: string = 'NavDestination';
export const NAV_PATH_STACK: string = 'NavPathStack';
export const CREATE_ROUTER_COMPONENT_COLLECT: Set<string> = new Set([NAVIGATION, NAV_DESTINATION]);

export const COMPONENT_CONSTRUCTOR_UPDATE_PARAMS: string = 'updateWithValueParams';
export const COMPONENT_CONSTRUCTOR_DELETE_PARAMS: string = 'aboutToBeDeleted';

export const CREATE_GET_METHOD: string = 'get';
export const CREATE_SET_METHOD: string = 'set';
export const CREATE_NEWVALUE_IDENTIFIER: string = 'newValue';
export const CREATE_CONSTRUCTOR_PARAMS: string = 'params';
export const CREATE_CONSTRUCTOR_SUBSCRIBER_MANAGER: string = 'SubscriberManager';
export const CREATE_CONSTRUCTOR_GET_FUNCTION: string = 'Get';
export const CREATE_CONSTRUCTOR_DELETE_FUNCTION: string = 'delete';
export const ABOUT_TO_BE_DELETE_FUNCTION_ID: string = 'id';
export const COMPONENT_WATCH_FUNCTION: string = 'declareWatch';
export const CONTEXT_STACK: string = 'contextStack';

export const CREATE_STATE_METHOD: string = 'createState';
export const CREATE_PROP_METHOD: string = 'createProp';
export const CREATE_LINK_METHOD: string = 'createLink';
export const CREATE_OBSERVABLE_OBJECT_METHOD: string = 'createObservableObject';

export const CUSTOM_COMPONENT_EARLIER_CREATE_CHILD: string = 'earlierCreatedChild_';
export const CUSTOM_COMPONENT_FUNCTION_FIND_CHILD_BY_ID: string = 'findChildById';
export const CUSTOM_COMPONENT_NEEDS_UPDATE_FUNCTION: string = 'needsUpdate';
export const CUSTOM_COMPONENT_MARK_STATIC_FUNCTION: string = 'markStatic';
export const CUSTOM_COMPONENT_EXTRAINFO: string = 'extraInfo';

export const COMPONENT_GESTURE: string = 'Gesture';
export const COMPONENT_GESTURE_GROUP: string = 'GestureGroup';
export const GESTURE_ATTRIBUTE: string = 'gesture';
export const PARALLEL_GESTURE_ATTRIBUTE: string = 'parallelGesture';
export const PRIORITY_GESTURE_ATTRIBUTE: string = 'priorityGesture';
export const GESTURE_ENUM_KEY: string = 'GesturePriority';
export const GESTURE_ENUM_VALUE_HIGH: string = 'High';
export const GESTURE_ENUM_VALUE_LOW: string = 'Low';
export const GESTURE_ENUM_VALUE_PARALLEL: string = 'Parallel';

export const RESOURCE: string = '$r';
export const RESOURCE_RAWFILE: string = '$rawfile';
export const RESOURCE_NAME_ID: string = 'id';
export const RESOURCE_NAME_TYPE: string = 'type';
export const RESOURCE_NAME_PARAMS: string = 'params';
export const RESOURCE_NAME_BUNDLE: string = 'bundleName';
export const RESOURCE_NAME_MODULE: string = 'moduleName';
export const RESOURCE_TYPE = {
  color: 10001,
  float: 10002,
  string: 10003,
  plural: 10004,
  boolean: 10005,
  intarray: 10006,
  integer: 10007,
  pattern: 10008,
  strarray: 10009,
  media: 20000,
  rawfile: 30000,
  symbol: 40000
};

export const WORKERS_DIR: string = 'workers';
export const WORKER_OBJECT: string = 'Worker';

export const TEST_RUNNER_DIR_SET: Set<string> = new Set(['TestRunner', 'testrunner']);

export const SET_CONTROLLER_METHOD: string = 'setController';
export const SET_CONTROLLER_CTR: string = 'ctr';
export const SET_CONTROLLER_CTR_TYPE: string = 'CustomDialogController';
export const JS_DIALOG: string = 'jsDialog';
export const CUSTOM_DIALOG_CONTROLLER_BUILDER: string = 'builder';

export const BUILDER_ATTR_NAME: string = 'builder';
export const BUILDER_ATTR_BIND: string = 'bind';

export const GEOMETRY_VIEW: string = 'GeometryView';

export const MODULE_SHARE_PATH: string = 'src' + path.sep + 'main' + path.sep + 'ets' + path.sep + 'share';
export const BUILD_SHARE_PATH: string = '../share';
export const MODULE_ETS_PATH: string = 'src' + path.sep + 'main' + path.sep + 'ets';
export const MODULE_VISUAL_PATH: string = 'src' + path.sep + 'main' + path.sep + 'supervisual';

export const THIS: string = 'this';
export const STYLES: string = 'Styles';
export const VISUAL_STATE: string = 'visualState';
export const VIEW_STACK_PROCESSOR: string = 'ViewStackProcessor';

export const BIND_POPUP: string = 'bindPopup';
export const BIND_POPUP_SET: Set<string> = new Set(['bindPopup']);
export const BIND_DRAG_SET: Set<string> = new Set(['onDragStart', 'onItemDragStart']);
export const ALL_COMPONENTS: string = 'AllComponents';
export const BIND_OBJECT_PROPERTY: Map<string, Set<string>> = new Map([
  ['Navigation', new Set(['title'])],
  ['NavDestination', new Set(['title'])],
  ['ListItem', new Set(['swipeAction'])],
  ['MenuItem', new Set([COMPONENT_CREATE_FUNCTION])],
  ['MenuItemGroup', new Set([COMPONENT_CREATE_FUNCTION])],
  ['Refresh', new Set([COMPONENT_CREATE_FUNCTION])],
  ['WaterFlow', new Set([COMPONENT_CREATE_FUNCTION])],
  ['Radio', new Set([COMPONENT_CREATE_FUNCTION])],
  ['Checkbox', new Set([COMPONENT_CREATE_FUNCTION])],
  ['Web', new Set(['bindSelectionMenu'])],
  [ALL_COMPONENTS, new Set(['bindMenu', 'bindContextMenu', 'bindSheet', 'dragPreview'])]
]);

export const CHECKED: string = 'checked';
export const RADIO: string = 'Radio';
export const $$_VALUE: string = 'value';
export const $$_CHANGE_EVENT: string = 'changeEvent';
export const $_VALUE: string = '$value';
export const TEXT_TIMER: string = 'TextTimer';
export const REFRESH: string = 'Refresh';
export const REFRESHING: string = 'refreshing';
export const FORMAT: string = 'format';
export const IS_COUNT_DOWN: string = 'isCountDown';
export const COUNT: string = 'count';
export const $$_THIS: string = '$$this';
export const $$_NEW_VALUE: string = 'newValue';
export const $$: string = '$$';
export const $$_VISIBILITY: string = 'visibility';
export const BIND_CONTENT_COVER: string = 'bindContentCover';
export const BIND_SHEET: string = 'bindSheet';
export const DATE_PICKER: string = 'DatePicker';
export const TIME_PICKER: string = 'TimePicker';
export const RATING: string = 'Rating';
export const SEAECH: string = 'Search';
export const CALENDAR: string = 'Calendar';
export const MODE: string = 'mode';
export const SHOW_SIDE_BAR: string = 'showSideBar';
export const SIDE_BAR_WIDTH: string = 'sideBarWidth';
export const CHECK_BOX: string = 'Checkbox';
export const SELECT_LOW: string = 'select';
export const CHECKBOX_GROUP: string = 'CheckboxGroup';
export const SELECT_ALL: string = 'selectAll';
export const SELECTED: string = 'selected';
export const MENU_ITEM: string = 'MenuItem';
export const PANEL: string = 'Panel';
export const RATING_LOW: string = 'rating';
export const VALUE: string = 'value';
export const SIDE_BAR_CONTAINER: string = 'SideBarContainer';
export const SLIDER: string = 'Slider';
export const STEPPER: string = 'Stepper';
export const INDEX: string = 'index';
export const SWIPER: string = 'Swiper';
export const TABS: string = 'Tabs';
export const TEXT_AREA: string = 'TextArea';
export const TEXT: string = 'text';
export const SELECT: string = 'Select';
export const TEXT_INPUT: string = 'TextInput';
export const TEXT_PICKER: string = 'TextPicker';
export const TOGGLE: string = 'Toggle';
export const ALPHABET_INDEXER: string = 'AlphabetIndexer';
export const ARC_ALPHABET_INDEXER: string = 'ArcAlphabetIndexer';
export const IS_ON: string = 'isOn';
export const DATE: string = 'date';
export const GRID_ITEM: string = 'GridItem';
export const LIST_ITEM: string = 'ListItem';
export const UPDATE_FUNC_BY_ELMT_ID: string = 'updateFuncByElmtId';
export const BIND_MENU: string = 'bindMenu';
export const BIND_CONTEXT_MENU: string = 'bindContextMenu';
export const NAV_BAR_WIDTH: string = 'navBarWidth';
export const ARC_LIST_ITEM: string = 'ArcListItem';

export const DOLLAR_BLOCK_INTERFACE: Set<string> = new Set([
  CHECK_BOX, CHECKBOX_GROUP, DATE_PICKER, TIME_PICKER, MENU_ITEM, PANEL, RATING, SIDE_BAR_CONTAINER, STEPPER, SWIPER, TABS, TEXT_PICKER, TOGGLE, SELECT,
  REFRESH, CALENDAR, GRID_ITEM, LIST_ITEM, TEXT_TIMER, SEAECH, TEXT_INPUT, SLIDER, TEXT_AREA, ALPHABET_INDEXER, ARC_ALPHABET_INDEXER]);
export const STYLE_ADD_DOUBLE_DOLLAR: Set<string> = new Set([
  BIND_POPUP, $$_VISIBILITY, BIND_CONTENT_COVER, BIND_SHEET]);
export const STYLE_ADD_DOUBLE_EXCLAMATION: Set<string> = new Set([
  BIND_MENU, BIND_CONTEXT_MENU, BIND_POPUP, BIND_CONTENT_COVER, BIND_SHEET]);
export const PROPERTIES_ADD_DOUBLE_DOLLAR: Map<string, Set<string>> = new Map([
  [RADIO, new Set([CHECKED])],
  [TEXT_TIMER, new Set([FORMAT, COUNT, IS_COUNT_DOWN])],
  [REFRESH, new Set([REFRESHING])],
  [CHECK_BOX, new Set([SELECT_LOW])],
  [CHECKBOX_GROUP, new Set([SELECT_ALL])],
  [DATE_PICKER, new Set([SELECTED])],
  [TIME_PICKER, new Set([SELECTED])],
  [MENU_ITEM, new Set([SELECTED])],
  [PANEL, new Set([MODE])],
  [RATING, new Set([RATING_LOW])],
  [SEAECH, new Set([VALUE])],
  [SIDE_BAR_CONTAINER, new Set([SHOW_SIDE_BAR])],
  [SLIDER, new Set([VALUE])],
  [STEPPER, new Set([INDEX])],
  [SWIPER, new Set([INDEX])],
  [TABS, new Set([INDEX])],
  [TEXT_AREA, new Set([TEXT])],
  [TEXT_INPUT, new Set([TEXT])],
  [TEXT_PICKER, new Set([VALUE, SELECTED])],
  [TOGGLE, new Set([IS_ON])],
  [ALPHABET_INDEXER, new Set([SELECTED])],
  [SELECT, new Set([SELECTED, VALUE])],
  [CALENDAR, new Set([DATE])],
  [GRID_ITEM, new Set([SELECTED])],
  [LIST_ITEM, new Set([SELECTED])]
]);
export const PROPERTIES_ADD_DOUBLE_EXCLAMATION: Map<string, Set<string>> = new Map([
  [RADIO, new Set([CHECKED])],
  [TEXT_TIMER, new Set([FORMAT, COUNT, IS_COUNT_DOWN])],
  [REFRESH, new Set([REFRESHING])],
  [CHECK_BOX, new Set([SELECT_LOW])],
  [CHECKBOX_GROUP, new Set([SELECT_ALL])],
  [DATE_PICKER, new Set([SELECTED])],
  [TIME_PICKER, new Set([SELECTED])],
  [MENU_ITEM, new Set([SELECTED])],
  [NAVIGATION, new Set([NAV_BAR_WIDTH])],
  [PANEL, new Set([MODE])],
  [RATING, new Set([RATING_LOW])],
  [SEAECH, new Set([VALUE])],
  [SIDE_BAR_CONTAINER, new Set([SHOW_SIDE_BAR, SIDE_BAR_WIDTH])],
  [SLIDER, new Set([VALUE])],
  [STEPPER, new Set([INDEX])],
  [SWIPER, new Set([INDEX])],
  [TABS, new Set([INDEX])],
  [TEXT_AREA, new Set([TEXT])],
  [TEXT_INPUT, new Set([TEXT])],
  [TEXT_PICKER, new Set([VALUE, SELECTED])],
  [TOGGLE, new Set([IS_ON])],
  [ALPHABET_INDEXER, new Set([SELECTED])],
  [ARC_ALPHABET_INDEXER, new Set([SELECTED])],
  [SELECT, new Set([SELECTED, VALUE])],
  [CALENDAR, new Set([DATE])],
  [GRID_ITEM, new Set([SELECTED])],
  [LIST_ITEM, new Set([SELECTED])]
]);

export const CREATE_BIND_COMPONENT: Set<string> = new Set(['ListItemGroup', REFRESH]);
export const HEADER: string = 'header';
export const INDICATORBUILDER: string = 'indicatorBuilder';
export const FOOTER: string = 'footer';

export const INTERFACE_NAME_SUFFIX: string = '_Params';
export const OBSERVED_PROPERTY_ABSTRACT: string = 'ObservedPropertyAbstract';

export const SUPERVISUAL: string = './supervisual';
export const SUPERVISUAL_SOURCEMAP_EXT: string = '.visual.js.map';

export const INSTANCE: string = 'Instance';

export const COMPONENT_TOGGLE: string = 'Toggle';
export const TTOGGLE_CHECKBOX: string = 'Checkbox';
export const TOGGLE_SWITCH: string = 'Switch';

export const FILESINFO_TXT: string = 'filesInfo.txt';
export const NPMENTRIES_TXT: string = 'npmEntries.txt';
export const MODULES_CACHE: string = 'modules.cache';
export const MODULES_ABC: string = 'modules.abc';
export const MODULELIST_JSON: string = 'moduleList.json';
export const PREBUILDINFO_JSON: string = 'preBuildInfo.json';
export const SOURCEMAPS_JSON: string = 'sourceMaps.json';
export const SOURCEMAPS: string = 'sourceMaps.map';
export const PROTO_FILESINFO_TXT: string = 'protoFilesInfo.txt';
export const AOT_FULL: string = 'full';
export const AOT_TYPE: string = 'type';
export const AOT_PARTIAL: string = 'partial';
export const AOT_PROFILE_SUFFIX: string = '.ap';

export const ESMODULE: string = 'esmodule';
export const JSBUNDLE: string = 'jsbundle';
export const ARK: string = 'ark';
export const TEMPORARY: string = 'temporary';
export const MAIN: string = 'main';
export const AUXILIARY: string = 'auxiliary';
export const ZERO: string = '0';
export const ONE: string = '1';
export const EXTNAME_JS: string = '.js';
export const EXTNAME_TS: string = '.ts';
export const EXTNAME_JS_MAP: string = '.js.map';
export const EXTNAME_TS_MAP: string = '.ts.map';
export const EXTNAME_MJS: string = '.mjs';
export const EXTNAME_CJS: string = '.cjs';
export const EXTNAME_D_TS: string = '.d.ts';
export const EXTNAME_D_ETS: string = '.d.ets';
export const EXTNAME_JSON: string = '.json';
export const EXTNAME_ABC: string = '.abc';
export const EXTNAME_PROTO_BIN: string = '.protoBin';
export const PATCH_SYMBOL_TABLE: string = 'symbol.txt';
export const MANAGE_WORKERS_SCRIPT: string = 'manage_workers.js';
export const GEN_ABC_SCRIPT: string = 'gen_abc.js';
export const GEN_MODULE_ABC_SCRIPT: string = 'gen_module_abc.js';

export const SUCCESS: number = 0;
export const FAIL: number = 1;

export const TS2ABC: string = 'ts2abc';
export const ES2ABC: string = 'es2abc';

export const MAX_WORKER_NUMBER: number = 3;

export const GENERATE_ID = 'generateId';
export const _GENERATE_ID = '__generate__Id';

export const COMPONENT_CONSTRUCTOR_INITIAL_PARAMS: string = 'setInitiallyProvidedValue';
export const COMPONENT_UPDATE_STATE_VARS: string = 'updateStateVars';
export const COMPONENT_RERENDER_FUNCTION: string = 'rerender';
export const COMPONENT_CONSTRUCTOR_PURGE_VARIABLE_DEP: string = 'purgeVariableDependenciesOnElmtId';
export const MARKDEPENDENTELEMENTSDIRTY: string = 'markDependentElementsDirty';
export const ABOUT_TO_BE_DELETE_FUNCTION_ID__: string = 'id__';
export const RMELMTID: string = 'rmElmtId';
export const PURGEDEPENDENCYONELMTID: string = 'purgeDependencyOnElmtId';
export const SETPROPERTYUNCHANGED: string = 'SetPropertyUnchanged';
export const ABOUTTOBEDELETEDINTERNAL: string = 'aboutToBeDeletedInternal';
export const UPDATEDIRTYELEMENTS: string = 'updateDirtyElements';
export const BASICDECORATORS: Set<string> = new Set([COMPONENT_STATE_DECORATOR, COMPONENT_PROP_DECORATOR,
  COMPONENT_LINK_DECORATOR, COMPONENT_OBJECT_LINK_DECORATOR, COMPONENT_PROVIDE_DECORATOR, COMPONENT_CONSUME_DECORATOR,
  COMPONENT_STORAGE_LINK_DECORATOR, COMPONENT_STORAGE_PROP_DECORATOR, COMPONENT_LOCAL_STORAGE_LINK_DECORATOR,
  COMPONENT_LOCAL_STORAGE_PROP_DECORATOR]);
export const LINKS_DECORATORS: Set<string> = new Set([COMPONENT_LINK_DECORATOR, COMPONENT_OBJECT_LINK_DECORATOR]);
export const ISINITIALRENDER: string = 'isInitialRender';
export const ELMTID: string = 'elmtId';
export const STARTGETACCESSRECORDINGFOR: string = 'StartGetAccessRecordingFor';
export const STOPGETACCESSRECORDING: string = 'StopGetAccessRecording';
export const UPDATE_STATE_VARS_OF_CHIND_BY_ELMTID: string = 'updateStateVarsOfChildByElmtId';
export const VIEWSTACKPROCESSOR: string = 'ViewStackProcessor';
export const OBSERVECOMPONENTCREATION: string = 'observeComponentCreation';
export const OBSERVECOMPONENTCREATION2: string = 'observeComponentCreation2';
export const ISLAZYCREATE: string = 'isLazyCreate';
export const DEEPRENDERFUNCTION: string = 'deepRenderFunction';
export const ITEMCREATION: string = 'itemCreation';
export const ITEMCREATION2: string = 'itemCreation2';
export const OBSERVEDSHALLOWRENDER: string = 'observedShallowRender';
export const OBSERVEDDEEPRENDER: string = 'observedDeepRender';
export const ItemComponents: string[] = ['ListItem', 'GridItem', 'ArcListItem'];
export const FOREACHITEMGENFUNCTION: string = 'forEachItemGenFunction';
export const __LAZYFOREACHITEMGENFUNCTION: string = '__lazyForEachItemGenFunction';
export const _ITEM: string = '_item';
export const FOREACHITEMIDFUNC: string = 'forEachItemIdFunc';
export const __LAZYFOREACHITEMIDFUNC: string = '__lazyForEachItemIdFunc';
export const FOREACHUPDATEFUNCTION: string = 'forEachUpdateFunction';
export const ALLOCATENEWELMETIDFORNEXTCOMPONENT: string = 'AllocateNewElmetIdForNextComponent';
export const STATE_OBJECTLINK_DECORATORS: string[] = [COMPONENT_STATE_DECORATOR, COMPONENT_OBJECT_LINK_DECORATOR];
export const COMPONENT_INITIAL_RENDER_FUNCTION: string = 'initialRender';
export const DECORATOR_COMPONENT_FREEZEWHENINACTIVE: string = 'freezeWhenInactive';
export const INIT_ALLOW_COMPONENT_FREEZE: string = 'initAllowComponentFreeze';
export const GRID_COMPONENT: string = 'Grid';
export const GRIDITEM_COMPONENT: string = 'GridItem';
export const WILLUSEPROXY: string = 'willUseProxy';
export const BASE_COMPONENT_NAME_PU: string = 'ViewPU';
export const PUV2_VIEW_BASE: string = 'PUV2ViewBase';
export const GLOBAL_THIS: string = 'globalThis';
export const OBSERVED_PROPERTY_SIMPLE_PU: string = 'ObservedPropertySimplePU';
export const OBSERVED_PROPERTY_OBJECT_PU: string = 'ObservedPropertyObjectPU';
export const SYNCHED_PROPERTY_SIMPLE_ONE_WAY_PU: string = 'SynchedPropertySimpleOneWayPU';
export const SYNCHED_PROPERTY_OBJECT_ONE_WAY_PU: string = 'SynchedPropertyObjectOneWayPU';
export const SYNCHED_PROPERTY_SIMPLE_TWO_WAY_PU: string = 'SynchedPropertySimpleTwoWayPU';
export const SYNCHED_PROPERTY_OBJECT_TWO_WAY_PU: string = 'SynchedPropertyObjectTwoWayPU';
export const SYNCHED_PROPERTY_NESED_OBJECT_PU: string = 'SynchedPropertyNesedObjectPU';
export const OBSERVED_PROPERTY_ABSTRACT_PU: string = 'ObservedPropertyAbstractPU';
export const COMPONENT_CONSTRUCTOR_LOCALSTORAGE_PU: string = '__localStorage';
export const COMPONENT_CONSTRUCTOR_LOCALSTORAGE_TYPE_PU: string = 'LocalStorage';
export const IFELSEBRANCHUPDATEFUNCTION = 'ifElseBranchUpdateFunction';
export const CREATE_STORAGE_LINK = 'createStorageLink';
export const CREATE_STORAGE_PROP = 'createStorageProp';
export const CREATE_LOCAL_STORAGE_LINK = 'createLocalStorageLink';
export const CREATE_LOCAL_STORAGE_PROP = 'createLocalStorageProp';
export const GET_ENTRYNAME: string = 'getEntryName';
export const SUPER_ARGS: string = 'args';
export const FINALIZE_CONSTRUCTION: string = 'finalizeConstruction';
export const PROTOTYPE: string = 'prototype';
export const REFLECT: string = 'Reflect';
export const DEFINE_PROPERTY: string = 'defineProperty';
export const BASE_CLASS: string = 'BaseClass';
export const IS_REUSABLE_: string = 'isReusable_';
export const GET_ATTRIBUTE: string = 'get';

export const CARD_ENTRY_FUNCTION_NAME: string = 'loadEtsCard';
export const CARD_ENABLE_DECORATORS: Set<string> = new Set([
  '@StorageLink', '@StorageProp', '@LocalStorageLink', '@LocalStorageProp'
]);
export const CARD_ENABLE_COMPONENTS: Set<string> = new Set([
  'AbilityComponent', 'PluginComponent', 'FormComponent', 'RemoteWindow',
  'XComponent', 'Web', 'RichText'
]);
export const TabContentAndNavDestination: Set<string> = new Set(['TabContent',
  'NavDestination']);
if (EXT_WHITE_LIST.length) {
  for (const compName of EXT_WHITE_LIST) {
    CREATE_ROUTER_COMPONENT_COLLECT.add(compName);
  }
}
if (EXT_WHITE_LIST.length >= 2) {
  TabContentAndNavDestination.add(EXT_WHITE_LIST[1]);
}
export const CARD_LOG_TYPE_DECORATORS = 1;
export const CARD_LOG_TYPE_COMPONENTS = 2;
export const CARD_LOG_TYPE_IMPORT = 3;

export const CALL = 'call';
export const RESERT = 'reset';

export const TS_NOCHECK: string = '// @ts-nocheck';

export const BUILDER_PARAM_PROXY: string = 'makeBuilderParameterProxy';
export const BUILDER_TYPE: string = 'BuilderType';

export const FUNCTION: string = 'function';
export const NAME: string = 'name';

export const ROUTENAME_NODE: string = 'routeNameNode';
export const STORAGE_NODE: string = 'storageNode';
export const STORAGE: string = 'storage';
export const REGISTER_NAMED_ROUTE: string = 'registerNamedRoute';
export const ROUTE_NAME: string = 'routeName';
export const PAGE_PATH: string = 'pagePath';
export const PAGE_FULL_PATH: string = 'pageFullPath';
export const IS_USER_CREATE_STACK: string = 'isUserCreateStack';

export const CAN_RETAKE: string = 'canRetake';
export const DECORATOR_SUFFIX: string = '@';
export const TRANSFORMED_MOCK_CONFIG: string = 'mock-config.json';
export const USER_DEFINE_MOCK_CONFIG: string = 'mock-config.json5';

export const WRAPBUILDER_FUNCTION: string = 'wrapBuilder';
export const WRAPPEDBUILDER_CLASS: string = 'WrappedBuilder';
export const WRAPBUILDER_BUILDERPROP: string = 'builder';
export const LENGTH: string = 'length';

export const PREVIEW: string = 'preview';
export const TITLE: string = 'title';

export const IDS: string = 'ids';
export const PUSH: string = 'push';
export const UPDATE_LAZY_FOREACH_ELEMENTS = 'UpdateLazyForEachElements';
export const IS_INITIAL_ITEM = 'isInitialItem';
export const MY_IDS = 'myIds';
export const COMPONENT_CALL: string = 'componentCall';

export const TS_BUILD_INFO_SUFFIX = '.tsbuildinfo';
export const ARKTS_LINTER_BUILD_INFO_SUFFIX = 'inversedArkTsLinter.tsbuildinfo';
export const HOT_RELOAD_BUILD_INFO_SUFFIX = 'hotReload.tsbuildinfo';
export const WATCH_COMPILER_BUILD_INFO_SUFFIX = 'watchCompiler.tsbuildinfo';

export const GET_SHARED: string = 'getShared';
export const USE_SHARED_STORAGE: string = 'useSharedStorage';
export const ARKTS_MODULE_PREFIX: string = '@arkts';
export const ARKTS_MODULE_NAME: string = 'arkts';
export const COLD_RELOAD_MODE: string = 'coldReload';
export const INTEGRATED_HSP: string = 'integratedHsp';