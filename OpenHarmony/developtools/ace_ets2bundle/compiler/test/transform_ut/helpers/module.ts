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

  constructor(entryModuleName: string, modulePath: string) {
    this.belongModulePath = '';
    this.hostModulesInfo = [];
    this.moduleName = entryModuleName;
    this.pkgName = '';
    this.isLocalDependency = true;
    this.isNodeEntryFile = false;
    this.pkgPath = modulePath;
    this.dependencyPkgInfo = undefined;
    this.belongProjectPath = '';
  }
};

class ModuleInfo {
  meta: Meta;
  id: string;
  importedIdMaps: object = {};
  importCache = [];
  isEntry: boolean = false;

  constructor(id: string, entryModuleName: string, modulePath: string) {
    this.meta = new Meta(entryModuleName, modulePath);
    this.id = id;

    if (entryModuleName === 'entry') {
      this.isEntry = true;
    }
  }

  setIsLocalDependency(value: boolean) {
    this.meta.isLocalDependency = value;
  }
  setIsNodeEntryFile(value: boolean) {
    this.meta.isNodeEntryFile = value;
  }

  setImportedIdMaps(path?: string) { }

  setNodeImportDeclaration() { }

  setNodeImportExpression() { }

  getNodeByType(IMPORT_NODE: string, EXPORTNAME_NODE: string, EXPORTALL_NODE: string, DYNAMICIMPORT_NODE: string) { }
}

export { 
	ModuleInfo 
};