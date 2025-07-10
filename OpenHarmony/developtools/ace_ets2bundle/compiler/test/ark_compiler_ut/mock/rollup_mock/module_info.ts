/*
* Copyright (c) 2023 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use rollupObject file except in compliance with the License.
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

import {
  MODULE_ID_ROLLUP_PLACEHOLDER,
  PROJECT_ROOT,
  DEFAULT_PROJECT,
  DEFAULT_ENTRY
} from '../rollup_mock/path_config';

class Meta {
  belongModulePath: string;
  hostModulesInfo: Array<object>;
  moduleName: string;
  pkgName: string;
  isLocalDependency: boolean;
  isNodeEntryFile: boolean;
  pkgPath: string;
  dependencyPkgInfo: Object;
  belongProjectPath: string;
  commonjs: Object;

  constructor(entryModuleName: string, modulePath: string) {
    this.belongModulePath = `${PROJECT_ROOT}/${DEFAULT_PROJECT}/${DEFAULT_ENTRY}`;
    this.hostModulesInfo = [];
    this.moduleName = entryModuleName;
    this.pkgName = '';
    this.isLocalDependency = true;
    this.isNodeEntryFile = false;
    this.pkgPath = modulePath;
    this.dependencyPkgInfo = undefined;
    this.belongProjectPath = `${PROJECT_ROOT}/${DEFAULT_PROJECT}`;
    this.commonjs = {
      isCommonJS: false
    };
  }
};

export class ModuleInfo {
  meta: Meta;
  id: string;
  importedIdMaps: object = {};
  importCache = [];

  constructor(id: string, entryModuleName: string, modulePath: string) {
    this.meta = new Meta(entryModuleName, modulePath);
    this.id = id;
  }

  setIsLocalDependency(value: boolean) {
    this.meta.isLocalDependency = value;
  }
  setIsNodeEntryFile(value: boolean) {
    this.meta.isNodeEntryFile = value;
  }

  setImportedIdMaps(path?: string) {
    if (path) {
      this.importedIdMaps = {
        'requestFile': path
      };
    } else {
      this.importedIdMaps = {
        '@ohos.app.ability.UIAbility': MODULE_ID_ROLLUP_PLACEHOLDER,
        '@ohos.hilog': MODULE_ID_ROLLUP_PLACEHOLDER
      };
    };

  }

  setNodeImportDeclaration() {
    this.importCache.push(
      {
        "type": "ImportDeclaration",
        "start": 0,
        "end": 52,
        "specifiers": [
          {
            "type": "ImportDefaultSpecifier",
            "start": 7,
            "end": 16,
            "local": {
              "type": "Identifier",
              "start": 7,
              "end": 16,
              "name": "UIAbility"
            }
          }
        ],
        "source": {
          "type": "Literal",
          "start": 22,
          "end": 51,
          "value": "@ohos.app.ability.UIAbility",
          "raw": "'@ohos.app.ability.UIAbility'"
        }
      },
      {
        "type": "ImportDeclaration",
        "start": 54,
        "end": 86,
        "specifiers": [
          {
            "type": "ImportDefaultSpecifier",
            "start": 61,
            "end": 66,
            "local": {
              "type": "Identifier",
              "start": 61,
              "end": 66,
              "name": "hilog"
            }
          }
        ],
        "source": {
          "type": "Literal",
          "start": 72,
          "end": 85,
          "value": "@ohos.hilog",
          "raw": "'@ohos.hilog'"
        }
      }
    );
  }

  setNodeImportExpression() {
    this.importCache.push(
      {
        "type": "ImportExpression",
        "start": 0,
        "end": 52,
        "specifiers": [
          {
            "type": "ImportDefaultSpecifier",
            "start": 7,
            "end": 16,
            "local": {
              "type": "Identifier",
              "start": 7,
              "end": 16,
              "name": "UIAbility"
            }
          }
        ],
        "source": {
          "type": "Literal",
          "start": 22,
          "end": 51,
          "value": "@ohos/sharedLibrary",
          "raw": "@ohos/sharedLibrary"
        }
      }
    )
  }

  getNodeByType(IMPORT_NODE: string, EXPORTNAME_NODE: string, EXPORTALL_NODE: string, DYNAMICIMPORT_NODE: string) {
    const nodeByType = new Map();
    this.importCache.forEach(node => {
      if (node.type === IMPORT_NODE) {
        if (!nodeByType.has(node.type)) {
          nodeByType.set(node.type, []);
        }
        nodeByType.get(node.type).push(node);
      }

      if (node.type === DYNAMICIMPORT_NODE) {
        if (!nodeByType.has(node.type)) {
          nodeByType.set(node.type, []);
        }
        nodeByType.get(node.type).push(node);
      }
    });
    return nodeByType;
  }
}

export default ModuleInfo;