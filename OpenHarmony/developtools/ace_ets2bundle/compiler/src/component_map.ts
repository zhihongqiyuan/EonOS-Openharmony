/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

const fs = require('fs');
const path = require('path');
import ts from 'typescript';
import {
  WHITELISTNEW,
  readExternalComponents
} from './external_component_map';
const COMPONENTS = 'components';
const FORM_COMPONENTS = 'form_components';
export const COMPONENT_MAP: any = {};
export const FORM_MAP: any = {};

let EXTERNAL_COMPONENT_MAP: object = {};
export let EXT_WHITE_LIST: Array<string> = [];
export let COMMON_ATTRS: Set<string> = new Set([]);

(function readComponents(): void {
  EXTERNAL_COMPONENT_MAP = readExternalComponents();
  EXT_WHITE_LIST = WHITELISTNEW;
  const componentPath: Map<string, string> = new Map([
    [`${COMPONENTS}`, `../${COMPONENTS}`],
    [`${FORM_COMPONENTS}`, `../${FORM_COMPONENTS}`]
  ]);
  for (const [id, relPath] of componentPath.entries()) {
    const componentsFile: string = path.join(__dirname, relPath);
    const files: string[] = fs.readdirSync(componentsFile);
    files.forEach(function (item) {
      const fPath: string = path.join(componentsFile, item);
      const json: any = require(fPath);
      const stat: any = fs.statSync(fPath);
      if (stat.isFile()) {
        if (json.name) {
          const compName: string = json.name;
          delete json.name;
          if (id === COMPONENTS) {
            COMPONENT_MAP[compName] = json;
          } else if (id === FORM_COMPONENTS) {
            FORM_MAP[compName] = json;
          }
        } else {
          if (id === COMPONENTS) {
            COMMON_ATTRS = new Set(json.attrs);
          }
        }
      }
    });
  }
  addExternalComponents();
})();

function addExternalComponents(): void {
  const componentMapKeys = new Set(Object.keys(COMPONENT_MAP));
  const extComponentSet: Set<string> = new Set(EXTERNAL_COMPONENT_MAP ?
    Object.keys(EXTERNAL_COMPONENT_MAP) : []);
  const extComponentKeys: Array<string> = Array.from(extComponentSet);
  if (!extComponentKeys.length) {
    return;
  }
  extComponentKeys.forEach((extCompName: string) => {
    if (componentMapKeys.has(extCompName) && COMPONENT_MAP[extCompName].parents &&
      EXTERNAL_COMPONENT_MAP[extCompName].parents) {
      const newParents: Array<string> = Array.from(new Set([...COMPONENT_MAP[extCompName].parents,
      ...EXTERNAL_COMPONENT_MAP[extCompName].parents]));
      COMPONENT_MAP[extCompName].parents = newParents;
    } else {
      COMPONENT_MAP[extCompName] = EXTERNAL_COMPONENT_MAP[extCompName];
    }
  })
}

const TRANSITION_COMMON_ATTRS: Set<string> = new Set([
  'slide', 'translate', 'scale', 'opacity'
]);
export const GESTURE_ATTRS: Set<string> = new Set([
  'gesture', 'parallelGesture', 'priorityGesture'
]);

export const ID_ATTRS: Map<string, Map<string, string | number>> = new Map();

export const forbiddenUseStateType: Set<string> = new Set(['Scroller', 'SwiperScroller',
  'VideoController', 'WebController', 'CustomDialogController', 'SwiperController',
  'TabsController', 'CalendarController', 'AbilityController', 'XComponentController',
  'CanvasRenderingContext2D', 'CanvasGradient', 'ImageBitmap', 'ImageData', 'Path2D',
  'RenderingContextSettings', 'OffscreenCanvasRenderingContext2D', 'PatternLockController',
  'TextAreaController', 'TextInputController', 'TextTimerController', 'SearchController', 'RichEditorController',
  'ArcSwiperController'
]);

const FOREACH_ATTRIBUTE = ['onMove'];
export const INNER_COMPONENT_NAMES: Set<string> = new Set();
export const NO_DEBUG_LINE_COMPONENT: Set<string> = new Set();
export const BUILDIN_CONTAINER_COMPONENT: Set<string> = new Set();
export const COMPONENT_SYSTEMAPI_NAMES: Set<string> = new Set();
export const BUILDIN_STYLE_NAMES: Set<string> = new Set([
  ...COMMON_ATTRS, ...GESTURE_ATTRS, ...TRANSITION_COMMON_ATTRS, ...FOREACH_ATTRIBUTE
]);
export const AUTOMIC_COMPONENT: Set<string> = new Set();
export const SINGLE_CHILD_COMPONENT: Set<string> = new Set();
export const SPECIFIC_CHILD_COMPONENT: Map<string, Set<string>> = new Map();
export const SPECIFIC_PARENT_COMPONENT: Map<string, Set<string>> = new Map();
export const GESTURE_TYPE_NAMES: Set<string> = new Set([
  'TapGesture', 'LongPressGesture', 'PanGesture', 'PinchGesture', 'RotationGesture', 'GestureGroup',
  'SwipeGesture'
]);
export const CUSTOM_BUILDER_METHOD: Set<string> = new Set();
export const INNER_CUSTOM_LOCALBUILDER_METHOD: Set<string> = new Set();
export const INNER_STYLE_FUNCTION: Map<string, ts.Block> = new Map();
export const GLOBAL_STYLE_FUNCTION: Map<string, ts.Block> = new Map();

export interface ExtendParamterInterfance {
  attribute: string,
  parameterCount: number
}
export const EXTEND_ATTRIBUTE: Map<string, Set<string>> = new Map();
export const STYLES_ATTRIBUTE: Set<string> = new Set();

export const INTERFACE_NODE_SET: Set<ts.InterfaceDeclaration> = new Set();

export const INNER_CUSTOM_BUILDER_METHOD: Set<string> = new Set();
export const GLOBAL_CUSTOM_BUILDER_METHOD: Set<string> = new Set();

export const JS_BIND_COMPONENTS: Set<string> = new Set([
  'ForEach', 'LazyForEach', ...GESTURE_TYPE_NAMES, 'Gesture',
  'PanGestureOption', 'CustomDialogController', 'Storage', 'Scroller', 'SwiperController',
  'TabsController', 'CalendarController', 'AbilityController', 'VideoController', 'WebController',
  'XComponentController', 'CanvasRenderingContext2D', 'CanvasGradient', 'ImageBitmap', 'ImageData',
  'Path2D', 'RenderingContextSettings', 'OffscreenCanvasRenderingContext2D', 'DatePickerDialog',
  'TextPickerDialog', 'AlertDialog', 'ContextMenu', 'ActionSheet', 'PatternLockController',
  'TimePickerDialog', 'CalendarPickerDialog', 'ArcSwiperController'
]);

export const NEEDPOP_COMPONENT: Set<string> = new Set(['Blank', 'Search']);

export const CUSTOM_BUILDER_PROPERTIES: Set<string> = new Set(['background', 'bindPopup', 'bindMenu', 'bindContextMenu', 'title',
  'menus', 'toolBar', 'tabBar', 'toolbar', 'onDragStart', 'onItemDragStart', 'swipeAction', 'bindContentCover', 'bindSheet',
  'navDestination', 'overlay', 'toolbarConfiguration', 'customKeyboard', 'bindSelectionMenu', 'description',
  'showUnit', 'customKeyboard', 'create', 'addBuilderSpan', 'dragPreview', 'accessibilityVirtualNode']);
export const CUSTOM_BUILDER_PROPERTIES_WITHOUTKEY: Set<string> = new Set(['showUnit', 'create']);
export const CUSTOM_BUILDER_CONSTRUCTORS: Set<string> = new Set(['MenuItem', 'MenuItemGroup', 'Refresh', 'WaterFlow', 'Radio', 'Checkbox']);

(function initComponent() {
  Object.keys(COMPONENT_MAP).forEach((componentName) => {
    INNER_COMPONENT_NAMES.add(componentName);
    JS_BIND_COMPONENTS.add(componentName);
    if (!COMPONENT_MAP[componentName].atomic) {
      BUILDIN_CONTAINER_COMPONENT.add(componentName);
    } else {
      AUTOMIC_COMPONENT.add(componentName);
    }
    if (COMPONENT_MAP[componentName].single) {
      SINGLE_CHILD_COMPONENT.add(componentName);
    }
    if (COMPONENT_MAP[componentName].children) {
      SPECIFIC_CHILD_COMPONENT.set(componentName,
        new Set([...COMPONENT_MAP[componentName].children]));
    }
    if (COMPONENT_MAP[componentName].attrs && COMPONENT_MAP[componentName].attrs.length) {
      COMPONENT_MAP[componentName].attrs.forEach((item: string) => {
        BUILDIN_STYLE_NAMES.add(item);
      });
      if (COMPONENT_MAP[componentName].systemApi) {
        COMPONENT_MAP[componentName].attrs.forEach((item: string) => {
          COMPONENT_SYSTEMAPI_NAMES.add(item);
        });
      }
    }
    if (COMPONENT_MAP[componentName].noDebugLine) {
      NO_DEBUG_LINE_COMPONENT.add(componentName);
    }
    if (COMPONENT_MAP[componentName].parents) {
      SPECIFIC_PARENT_COMPONENT.set(componentName,
        new Set([...COMPONENT_MAP[componentName].parents]));
    }
  });
})();

export function resetComponentMap(): void {
  ID_ATTRS.clear();
  EXTEND_ATTRIBUTE.clear();
  STYLES_ATTRIBUTE.clear();
}

export function resetExtContent(): void {
  EXT_WHITE_LIST = [];
}
