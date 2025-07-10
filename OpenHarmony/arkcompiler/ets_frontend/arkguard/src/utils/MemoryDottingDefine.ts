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

export enum MemoryDottingDefine {
  CREATE_AST = 'ArkObfuscator(createAst: Create AST)',
  OBFUSCATE_AST = 'ArkObfuscator(obfuscateAst: Obfuscate AST)',
  CREATE_PRINTER = 'ArkObfuscator(writeObfuscationResult: Create Printer)',
  SCAN_SYS_API = 'ConfigResolver(resolveObfuscationConfigs: Scan system api)',
  REMOVE_CONSOLE = 'DisableConsoleTransformer(disableConsoleFactory: Remove console)',
  FILENAME_OBFUSCATION = 'RenameFileNameTransformer(renameFileNameTransformer: Filename obfuscation)',
  CREATE_CHECKER = 'RenameIdentifierTransformer(renameTransformer: Create checker)',
  SCOPE_ANALYZE = 'RenameIdentifierTransformer(renameTransformer: Scope analyze)',
  OBFUSCATE_NAMES = 'RenameIdentifierTransformer(renameTransformer: Create obfuscated names)',
  OBFUSCATE_NODES = 'RenameIdentifierTransformer(renameTransformer: Obfuscate nodes)',
  PROPERTY_OBFUSCATION = 'RenamePropertiesTransformer(renamePropertiesFactory: Property obfuscation)',
  SHORTHAND_OBFUSCATION = 'ShorthandPropertyTransformer(shorthandPropertyTransformFactory: Shorthand obfuscation)',
  VIRTUAL_OBFUSCATION = 'VirtualConstructorTransformer(virtualConstructorTransformer: Virtual constructor obfuscation)'
}

