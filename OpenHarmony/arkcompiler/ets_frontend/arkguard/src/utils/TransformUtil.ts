/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import {
  forEachChild,
  getLeadingCommentRangesOfNode,
  isCallExpression,
  isExpressionStatement,
  isIdentifier,
  isStructDeclaration,
  SyntaxKind,
  visitEachChild
} from 'typescript';

import type {
  CommentRange,
  Identifier,
  Node,
  SourceFile,
  StructDeclaration,
  TransformationContext
} from 'typescript';
import type { IOptions } from '../configs/IOptions';
import { LocalVariableCollections, PropCollections, UnobfuscationCollections } from './CommonCollections';
import { historyUnobfuscatedNamesMap } from '../transformers/rename/RenameIdentifierTransformer';

export interface ReservedNameInfo {
  universalReservedArray: RegExp[]; // items contain wildcards
  specificReservedArray: string[]; // items do not contain wildcards
}

/**
 * collect exist identifier names in current source file
 * @param sourceFile
 */
export function collectExistNames(sourceFile: SourceFile): Set<string> {
  const identifiers: Set<string> = new Set<string>();

  let visit = (node: Node): void => {
    if (isIdentifier(node)) {
      identifiers.add(node.text);
    }

    forEachChild(node, visit);
  };

  forEachChild(sourceFile, visit);
  return identifiers;
}

type IdentifiersAndStructs = {shadowIdentifiers: Identifier[], shadowStructs: StructDeclaration[]};

/**
 * separate wildcards from specific items.
 */
export function separateUniversalReservedItem(originalArray: string[] | undefined,
  shouldPrintKeptName: boolean): ReservedNameInfo {
  if (!originalArray) {
    throw new Error('Unable to handle the empty array.');
  }
  const reservedInfo: ReservedNameInfo = {
    universalReservedArray: [],
    specificReservedArray: []
  };

  originalArray.forEach(reservedItem => {
    if (containWildcards(reservedItem)) {
      const regexPattern = wildcardTransformer(reservedItem);
      const regexOperator = new RegExp(`^${regexPattern}$`);
      reservedInfo.universalReservedArray.push(regexOperator);
      recordWildcardMapping(reservedItem, regexOperator, shouldPrintKeptName);
    } else {
      reservedInfo.specificReservedArray.push(reservedItem);
    }
  });
  return reservedInfo;
}

function recordWildcardMapping(originString: string, regExpression: RegExp,
  shouldPrintKeptName: boolean): void {
  if (shouldPrintKeptName) {
    UnobfuscationCollections.reservedWildcardMap.set(regExpression, originString);
  }
}

/**
 * check if the item contains '*', '?'.
 */
export function containWildcards(item: string): boolean {
  return /[\*\?]/.test(item);
}

/**
 * Convert specific characters into regular expressions.
 */
export function wildcardTransformer(wildcard: string, isPath?: boolean): string {
  // Add an escape character in front of special characters
  // special characters: '\', '^', '$', '.', '+', '|', '[', ']', '{', '}', '(', ')'
  let escapedItem = wildcard.replace(/[\\+^${}()|\[\]\.]/g, '\\$&');

  // isPath: containing '**', and '*', '?' can not be matched with '/'.
  if (isPath) {
    // before: ../**/a/b/c*/?.ets
    // after: ../.*/a/b/c[^/]*/[^/].ets
    return escapedItem.replace(/\*\*/g, '.*').replace(/(?<!\.)\*/g, '[^/]*').replace(/\?/g, '[^/]');
  }
  // before: *a?
  // after: .*a.
  return escapedItem.replace(/\*/g, '.*').replace(/\?/g, '.');
}

/**
 * Determine whether the original name needs to be preserved.
 */
export function needToBeReserved(reservedSet: Set<string>, universalArray: RegExp[], originalName: string): boolean {
  return reservedSet.has(originalName) || isMatchWildcard(universalArray, originalName);
}

/**
 * Determine whether it can match the wildcard character in the array.
 */
export function isMatchWildcard(wildcardArray: RegExp[], item: string): boolean {
  for (const wildcard of wildcardArray) {
    if (wildcard.test(item)) {
      return true;
    }
  }
  return false;
}

/**
 * Separate parts of an array that contain wildcard characters.
 */
export function handleReservedConfig(config: IOptions, optionName: string, reservedListName: string,
  universalLisName: string, enableRemove?: string): void {
  const reservedConfig = config?.[optionName];
  let needSeparate: boolean = !!(reservedConfig?.[reservedListName]);
  if (enableRemove) {
    needSeparate &&= reservedConfig[enableRemove];
  }
  if (needSeparate) {
    // separate items which contain wildcards from others
    const reservedInfo: ReservedNameInfo =
      separateUniversalReservedItem(reservedConfig[reservedListName], !!(config.mUnobfuscationOption?.mPrintKeptNames));
    reservedConfig[reservedListName] = reservedInfo.specificReservedArray;
    reservedConfig[universalLisName] = reservedInfo.universalReservedArray;
  }
}

export function isReservedLocalVariable(mangledName: string): boolean {
  return LocalVariableCollections.reservedLangForLocal.has(mangledName) ||
    LocalVariableCollections.reservedConfig?.has(mangledName) ||
    UnobfuscationCollections.reservedSdkApiForLocal?.has(mangledName) ||
    UnobfuscationCollections.reservedExportName?.has(mangledName);
}

export function isReservedTopLevel(originalName: string, enablePropertyObf: boolean): boolean {
  if (enablePropertyObf) {
    return isReservedProperty(originalName);
  }

  // The 'mReservedToplevelNames' has already been added to 'PropCollections.reservedProperties'.
  return UnobfuscationCollections.reservedLangForTopLevel.has(originalName) ||
    UnobfuscationCollections.reservedSdkApiForGlobal?.has(originalName) ||
    UnobfuscationCollections.reservedExportName?.has(originalName) ||
    PropCollections.reservedProperties?.has(originalName) ||
    isMatchWildcard(PropCollections.universalReservedProperties, originalName);
}

export function isReservedProperty(originalName: string): boolean {
  return UnobfuscationCollections.reservedSdkApiForProp?.has(originalName) ||
    UnobfuscationCollections.reservedLangForProperty?.has(originalName) ||
    UnobfuscationCollections.reservedStruct?.has(originalName) ||
    UnobfuscationCollections.reservedExportNameAndProp?.has(originalName) ||
    UnobfuscationCollections.reservedStrProp?.has(originalName) ||
    UnobfuscationCollections.reservedEnum?.has(originalName) ||
    PropCollections.reservedProperties?.has(originalName) ||
    isMatchWildcard(PropCollections.universalReservedProperties, originalName);
}

  /**
   * Reasons for not being obfuscated.
   */
export enum WhitelistType {
  SDK = 'sdk',
  LANG = 'lang',
  CONF = 'conf',
  STRUCT = 'struct',
  EXPORT = 'exported',
  STRPROP = 'strProp',
  ENUM = 'enum'
}

function needToRecordTopLevel(originalName: string, recordMap: Map<string, Set<string>>,
  nameWithScope: string, enablePropertyObf: boolean): boolean {
  if (enablePropertyObf) {
    return needToRecordProperty(originalName, recordMap, nameWithScope);
  }

  let reservedFlag = false;
  if (UnobfuscationCollections.reservedLangForTopLevel.has(originalName)) {
    recordReservedName(nameWithScope, WhitelistType.LANG, recordMap);
    reservedFlag = true;
  }

  if (UnobfuscationCollections.reservedSdkApiForGlobal?.has(originalName)) {
    recordReservedName(nameWithScope, WhitelistType.SDK, recordMap);
    reservedFlag = true;
  }

  if (UnobfuscationCollections.reservedExportName?.has(originalName)) {
    recordReservedName(nameWithScope, WhitelistType.EXPORT, recordMap);
    reservedFlag = true;
  }

  // The 'mReservedToplevelNames' has already been added to 'PropCollections.reservedProperties'.
  if (PropCollections.reservedProperties?.has(originalName) ||
    isMatchWildcard(PropCollections.universalReservedProperties, originalName)) {
    recordReservedName(nameWithScope, WhitelistType.CONF, recordMap);
    reservedFlag = true;
  }

  return reservedFlag;
}

function needToReservedLocal(originalName: string, recordMap: Map<string, Set<string>>, nameWithScope: string): boolean {
  let reservedFlag = false;

  if (LocalVariableCollections.reservedLangForLocal.has(originalName)) {
    recordReservedName(nameWithScope, WhitelistType.LANG, recordMap);
    reservedFlag = true;
  }

  if (UnobfuscationCollections.reservedSdkApiForLocal?.has(originalName)) {
    recordReservedName(nameWithScope, WhitelistType.SDK, recordMap);
    reservedFlag = true;
  }

  if (UnobfuscationCollections.reservedExportName?.has(originalName)) {
    recordReservedName(nameWithScope, WhitelistType.EXPORT, recordMap);
    reservedFlag = true;
  }

  if (LocalVariableCollections.reservedConfig?.has(originalName)) {
    recordReservedName(nameWithScope, WhitelistType.CONF, recordMap);
    reservedFlag = true;
  }

  return reservedFlag;
}

/**
 * If the property name is in the whitelist, record the reason for not being obfuscated.
 * @param nameWithScope: If both property obfuscation and top-level obfuscation or export obfuscation are enabled,
 * this interface is also used to record the reasons why the top-level names or export names were not obfuscated,
 * and the top-level names or export names include the scope.
 */
export function needToRecordProperty(originalName: string, recordMap?: Map<string, Set<string>>, nameWithScope?: string): boolean {
  let reservedFlag = false;
  let recordName = nameWithScope ? nameWithScope : originalName;
  if (UnobfuscationCollections.reservedSdkApiForProp?.has(originalName)) {
    recordReservedName(recordName, WhitelistType.SDK, recordMap);
    reservedFlag = true;
  }

  if (UnobfuscationCollections.reservedLangForProperty?.has(originalName)) {
    recordReservedName(recordName, WhitelistType.LANG, recordMap);
    reservedFlag = true;
  }

  if (UnobfuscationCollections.reservedStruct?.has(originalName)) {
    recordReservedName(recordName, WhitelistType.STRUCT, recordMap);
    reservedFlag = true;
  }

  if (UnobfuscationCollections.reservedExportNameAndProp?.has(originalName)) {
    recordReservedName(recordName, WhitelistType.EXPORT, recordMap);
    reservedFlag = true;
  }

  if (UnobfuscationCollections.reservedStrProp?.has(originalName)) {
    recordReservedName(recordName, WhitelistType.STRPROP, recordMap);
    reservedFlag = true;
  }

  if (UnobfuscationCollections.reservedEnum?.has(originalName)) {
    recordReservedName(recordName, WhitelistType.ENUM, recordMap);
    reservedFlag = true;
  }

  if (PropCollections.reservedProperties?.has(originalName) ||
    isMatchWildcard(PropCollections.universalReservedProperties, originalName)) {
    recordReservedName(recordName, WhitelistType.CONF, recordMap);
    reservedFlag = true;
  }

  return reservedFlag;
}

export function isInTopLevelWhitelist(originalName: string, recordMap: Map<string, Set<string>>,
  nameWithScope: string, enablePropertyObf: boolean, shouldPrintKeptNames: boolean): boolean {
  if (shouldPrintKeptNames) {
    return needToRecordTopLevel(originalName, recordMap, nameWithScope, enablePropertyObf);
  }

  return isReservedTopLevel(originalName, enablePropertyObf);
}

export function isInPropertyWhitelist(originalName: string, recordMap: Map<string, Set<string>>,
  shouldPrintKeptNames: boolean): boolean {
  if (shouldPrintKeptNames) {
    return needToRecordProperty(originalName, recordMap);
  }

  return isReservedProperty(originalName);
}

export function isInLocalWhitelist(originalName: string, recordMap: Map<string, Set<string>>,
  nameWithScope: string, shouldPrintKeptNames: boolean): boolean {
  if (shouldPrintKeptNames) {
    return needToReservedLocal(originalName, recordMap, nameWithScope);
  }

  return isReservedLocalVariable(originalName);
}

function recordReservedName(originalName: string, type: string, recordObj: Map<string, Set<string>>): void {
  if (!recordObj.has(originalName)) {
    recordObj.set(originalName, new Set());
  }
  recordObj.get(originalName).add(type);
}

export function recordHistoryUnobfuscatedNames(nameWithScope: string): void {
  if (historyUnobfuscatedNamesMap?.has(nameWithScope)) {
    UnobfuscationCollections.unobfuscatedNamesMap.set(nameWithScope,
      new Set(historyUnobfuscatedNamesMap.get(nameWithScope)));
  }
}