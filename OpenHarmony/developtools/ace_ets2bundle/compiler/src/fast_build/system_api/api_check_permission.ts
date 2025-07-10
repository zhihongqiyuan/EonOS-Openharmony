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

import ts from 'typescript';

enum PermissionVaildTokenState {
  Init,
  LeftParenthesis,
  RightParenthesis,
  PermissionChar,
  And,
  Or,
}

interface PermissionVaildCalcInfo {
  valid: boolean;
  currentToken: PermissionVaildTokenState;
  finish: boolean;
  currentPermissionMatch: boolean;
}

interface PermissionVaildCalcGroup {
  subQueue: string[];
  includeParenthesis: boolean;
}

export interface PermissionModule {
  modulePath: string;
  testPermissions: string[];
  permissions: string[];
}

export class JsDocCheckService {
  /**
   * STER1. Parse the permission information configured on the API
   * STEP2. Recursive queue to obtain whether the current permission configuration supports it
   */
  static validPermission(comment: string, permissionsArray: string[]): boolean {
    const permissionsItem: string[] = JsDocCheckService.getSplitsArrayWithDesignatedCharAndStr(comment ?? '', ' ')
      .filter((item) => {
        return item !== '';
      });
    const permissionsQueue: string[] = [];
    permissionsItem.forEach((item: string) => {
      //STEP1.1 Parse'('
      const leftParenthesisItem: string[] = JsDocCheckService.getSplitsArrayWithDesignatedCharAndArrayStr([item], '(');
      //STEP1.2 Parse')'
      const rightParenthesisItem: string[] = JsDocCheckService.getSplitsArrayWithDesignatedCharAndArrayStr(leftParenthesisItem, ')');
      permissionsQueue.push(...rightParenthesisItem);
    });
    //STEP2
    const calcValidResult: PermissionVaildCalcInfo = {
      valid: false,
      currentToken: PermissionVaildTokenState.Init,
      finish: false,
      currentPermissionMatch: true,
    };
    JsDocCheckService.validPermissionRecursion(permissionsQueue, permissionsArray, calcValidResult);
    return calcValidResult.valid;
  }

  private static validPermissionRecursion(permissionsQueue: string[], permissions: string[], calcValidResult: PermissionVaildCalcInfo): void {
    if (permissionsQueue.some(item => ['(', ')'].includes(item))) {
      const groups: PermissionVaildCalcGroup[] = JsDocCheckService.groupWithParenthesis(permissionsQueue);
      const groupJoin: string[] = JsDocCheckService.getGroupItemPermission(groups, calcValidResult, permissions);
      JsDocCheckService.getPermissionVaildAtoms(groupJoin, calcValidResult, permissions ?? []);
    } else {
      JsDocCheckService.getPermissionVaildAtoms(permissionsQueue, calcValidResult, permissions ?? []);
    }
  }

  private static getSplitsArrayWithDesignatedCharAndStr(permission: string, designatedChar: string): string[] {
    return permission.split(designatedChar).map(item => item.trim());
  }

  private static getGroupItemPermission(
    groups: PermissionVaildCalcGroup[],
    calcValidResult: PermissionVaildCalcInfo,
    permissions: string[]): string[] {
    const groupJoin: string[] = [];
    groups.forEach((groupItem: PermissionVaildCalcGroup) => {
      if (groupItem.includeParenthesis) {
        const calcValidResultItem: PermissionVaildCalcInfo = {
          ...calcValidResult,
        };
        const subStack: string[] = groupItem.subQueue.slice(1, groupItem.subQueue.length - 1);
        JsDocCheckService.validPermissionRecursion(subStack, permissions, calcValidResultItem);
        if (calcValidResultItem.valid) {
          groupJoin.push('');
        } else {
          groupJoin.push('NA');
        }
      } else {
        groupJoin.push(...groupItem.subQueue);
      }
    });
    return groupJoin;
  }

  private static groupWithParenthesis(stack: string[]): PermissionVaildCalcGroup[] {
    let currentLeftParenthesisCount: number = 0;
    const groups: PermissionVaildCalcGroup[] = [];
    let currentGroupItem: PermissionVaildCalcGroup = {
      subQueue: [],
      includeParenthesis: false,
    };
    stack.forEach((item: string, index: number) => {
      if (item === '(') {
        if (currentLeftParenthesisCount === 0) {
          groups.push(currentGroupItem);
          currentGroupItem = {
            subQueue: [item],
            includeParenthesis: true
          };
        } else {
          currentGroupItem.subQueue.push(item);
        }
        currentLeftParenthesisCount++;
      } else if (item === ')') {
        currentLeftParenthesisCount--;
        currentGroupItem.subQueue.push(item);
        if (currentLeftParenthesisCount === 0) {
          groups.push(currentGroupItem);
          currentGroupItem = {
            subQueue: [],
            includeParenthesis: false,
          };
        }
      } else {
        currentGroupItem.subQueue.push(item);
        if (index === stack.length - 1) {
          groups.push(currentGroupItem);
        }
      }
    });
    return groups;
  }

  private static getPermissionVaildAtoms(atomStacks: string[], calcValidResult: PermissionVaildCalcInfo, configPermissions: string[]): void {
    if (calcValidResult.finish) {
      return;
    }
    if (atomStacks[0] === 'and') {
      calcValidResult.currentToken = PermissionVaildTokenState.And;
    } else if (atomStacks[0] === 'or') {
      calcValidResult.currentToken = PermissionVaildTokenState.Or;
    } else {
      if (calcValidResult.currentToken === PermissionVaildTokenState.Or) {
        if (JsDocCheckService.inValidOrExpression(
          atomStacks,
          calcValidResult,
          configPermissions
        )) {
          calcValidResult.currentPermissionMatch = false;
        }
      } else if (calcValidResult.currentToken === PermissionVaildTokenState.And) {
        if (JsDocCheckService.inValidAndExpression(
          atomStacks,
          calcValidResult,
          configPermissions
        )) {
          calcValidResult.currentPermissionMatch = false;
        }
      } else {
        calcValidResult.currentPermissionMatch =
          JsDocCheckService.validPermissionItem(atomStacks[0], configPermissions);
      }
    }
    if (atomStacks.length > 1) {
      JsDocCheckService.getPermissionVaildAtoms(
        atomStacks.slice(1),
        calcValidResult,
        configPermissions
      );
    } else {
      calcValidResult.valid = calcValidResult.currentPermissionMatch;
      calcValidResult.finish = true;
    }
  }

  private static inValidOrExpression(
    atomStacks: string[],
    calcValidResult: PermissionVaildCalcInfo,
    configPermissions: string[]): boolean {
    if (
      !calcValidResult.currentPermissionMatch &&
      !JsDocCheckService.validPermissionItem(atomStacks[0], configPermissions)
    ) {
      calcValidResult.valid = false;
      return true;
    }
    calcValidResult.currentPermissionMatch = true;
    return false;
  }
  private static inValidAndExpression(
    atomStacks: string[],
    calcValidResult: PermissionVaildCalcInfo,
    configPermissions: string[]): boolean {
    if (
      !calcValidResult.currentPermissionMatch ||
      !JsDocCheckService.validPermissionItem(atomStacks[0], configPermissions)
    ) {
      calcValidResult.valid = false;
      return true;
    }
    calcValidResult.currentPermissionMatch =
      JsDocCheckService.validPermissionItem(atomStacks[0], configPermissions);
    return false;
  }
  private static validPermissionItem(atomStackItem: string, configPermissions: string[]): boolean {
    return atomStackItem === '' || configPermissions.includes(atomStackItem);
  }

  private static getSplitsArrayWithDesignatedCharAndArrayStr(
    leftParenthesisItems: string[],
    designatedChar: string
  ): string[] {
    const rightParenthesisItems: string[] = [];
    leftParenthesisItems.forEach((leftParenthesisItem: string) => {
      if (leftParenthesisItem.includes(designatedChar)) {
        const rightParenthesis: string[] =
          JsDocCheckService.getSplitsArrayWithDesignatedCharAndStr(
            leftParenthesisItem,
            designatedChar
          );
        rightParenthesis.forEach((item: string) => {
          if (item === '') {
            rightParenthesisItems.push(designatedChar);
          } else {
            rightParenthesisItems.push(item);
          }
        });
      } else {
        rightParenthesisItems.push(leftParenthesisItem);
      }
    });
    return rightParenthesisItems;
  }
}
