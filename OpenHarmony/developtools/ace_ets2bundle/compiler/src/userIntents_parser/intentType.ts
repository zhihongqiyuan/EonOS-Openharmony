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

import ts from 'typescript';
import parseIntent from './parseUserIntents';
import Ajv from 'ajv';

const ajv = new Ajv({allErrors: true});

export interface IntentInfo {
  intentName: string;
  domain: string;
  intentVersion: string;
  displayName: string;
  displayDescription: string;
  schema: string;
  icon: Function;
  llmDescription: string;
  keywords: string[];
  parameters: object;
}

export interface LinkIntentParamMapping {
  paramName: string;
  paramMappingName: string;
  paramCategory: string;
}

export interface IntentLinkInfo extends IntentInfo {
  uri: string;
  paramMappings: LinkIntentParamMapping[]
}

export interface IntentEntryInfo extends IntentInfo {
  abilityName: string;
  executeMode: number[];
}

export interface IntentFunctionInfo extends IntentInfo {
}

export interface IntentPageInfo extends IntentInfo {
  uiAbility: string;
  pagePath: string;
  navigationId: string;
  navDestinationName: string;
}

export class ParamChecker<T> {
  private _requiredFields: (keyof T)[];
  private _allowFields: Set<keyof T>;
  private _paramValidators: Record<keyof T, (v: ts.Expression) => boolean>;
  private _nestedCheckers: Map<string, ParamChecker<LinkIntentParamMapping>>;

  get requiredFields(): (keyof T)[] {
    return this._requiredFields;
  }

  get allowFields(): Set<keyof T> {
    return this._allowFields;
  }

  get paramValidators(): Record<keyof T, (v: ts.Expression) => boolean> {
    return this._paramValidators;
  }

  get nestedCheckers(): Map<string, ParamChecker<LinkIntentParamMapping>> {
    return this._nestedCheckers;
  }

  set nestedCheckers(value: Map<string, ParamChecker<LinkIntentParamMapping>>) {
    this._nestedCheckers = value;
  }

  set requiredFields(value: (keyof T)[]) {
    this._requiredFields = value;
  }

  set allowFields(value: Set<keyof T>) {
    this._allowFields = value;
  }

  set paramValidators(value: Record<keyof T, (v: ts.Expression) => boolean>) {
    this._paramValidators = value;
  }
}

function validateRequiredString(v: ts.Expression): boolean {
  return v !== undefined && v !== null && ts.isStringLiteral(v) && v.text.trim() !== '';
}

function validateOptionalString(v: ts.Expression): boolean {
  return v !== undefined && v !== null && ts.isStringLiteral(v);
}

function validateIcon(v: ts.Expression): boolean {
  return v !== undefined && v !== null && ((ts.isCallExpression(v) && v.expression.getText() === '$r') || ts.isStringLiteral(v));
}

function validateParameters(v: ts.Expression): boolean {
  try {
    const initializer = ts.isIdentifier(v) ?
      parseIntent.checker.getSymbolAtLocation(v)?.valueDeclaration?.initializer :
      v;
    return ts.isObjectLiteralExpression(initializer) && ajv.compile(JSON.parse(initializer.getText()));
  } catch {
    return false;
  }
}

function validateKeywords(v: ts.Expression): boolean {
  if (v === undefined || v === null) {
    return false;
  }
  if (!ts.isArrayLiteralExpression(v)) {
    return false;
  }
  return v.elements.every(ele => {
    const element = ts.isIdentifier(ele) ?
      parseIntent.checker.getSymbolAtLocation(ele)?.valueDeclaration?.initializer :
      ele;
    return element !== undefined && (ts.isStringLiteral(element) || ts.isNoSubstitutionTemplateLiteral(element));
  });
}

const BASE_REQUIRED = ['intentName', 'domain', 'intentVersion', 'displayName'];
const BASE_ALLOW = ['displayDescription', 'schema', 'icon', 'llmDescription', 'keywords', 'parameters', ...BASE_REQUIRED];

const IntentLinkParamsChecker: ParamChecker<LinkIntentParamMapping> = new ParamChecker<LinkIntentParamMapping>();
IntentLinkParamsChecker.requiredFields = ['paramName'];
IntentLinkParamsChecker.allowFields = new Set<keyof LinkIntentParamMapping>([
  'paramName', 'paramMappingName', 'paramCategory'
]);
IntentLinkParamsChecker.paramValidators = {
  paramCategory: validateOptionalString,
  paramMappingName: validateOptionalString,
  paramName: validateRequiredString
};

export const IntentLinkInfoChecker: ParamChecker<IntentLinkInfo> = new ParamChecker<IntentLinkInfo>();
IntentLinkInfoChecker.requiredFields = [...BASE_REQUIRED as Array<keyof IntentLinkInfo>, 'uri'];
IntentLinkInfoChecker.allowFields = new Set<keyof IntentLinkInfo>([
  ...BASE_ALLOW as Array<keyof IntentLinkInfo>,
  'uri',
  'paramMappings'
]);
IntentLinkInfoChecker.paramValidators = {
  parameters: validateParameters,
  paramMappings: (v: ts.Expression): boolean => {
    if (v === null || v === undefined) {
      return false;
    }
    if (!ts.isArrayLiteralExpression(v)) {
      return false;
    }
    return v.elements.every(element => {
      if (element.kind === ts.SyntaxKind.NullKeyword) {
        return false;
      }
      if (element.kind === ts.SyntaxKind.UndefinedKeyword) {
        return false;
      }
      if (ts.isIdentifier(element) && element.text === 'undefined') {
        return false;
      }
      return true;
    }
    );
  },
  keywords: validateKeywords,
  intentName: validateRequiredString,
  domain: validateRequiredString,
  intentVersion: validateRequiredString,
  displayName: validateRequiredString,
  displayDescription: validateOptionalString,
  schema: validateRequiredString,
  icon: validateIcon,
  llmDescription: validateOptionalString,
  uri: validateRequiredString
};
IntentLinkInfoChecker.nestedCheckers = new Map([['paramMappings', IntentLinkParamsChecker]]);

export const intentEntryInfoChecker: ParamChecker<IntentEntryInfo> = new ParamChecker<IntentEntryInfo>();
intentEntryInfoChecker.requiredFields = [...BASE_REQUIRED as Array<keyof IntentEntryInfo>, 'abilityName'];
intentEntryInfoChecker.allowFields = new Set<keyof IntentEntryInfo>([...BASE_ALLOW as Array<keyof IntentEntryInfo>, 'abilityName', 'executeMode']);
intentEntryInfoChecker.paramValidators = {
  parameters: validateParameters,
  icon: validateIcon,
  keywords: validateKeywords,
  schema: validateRequiredString,
  abilityName: validateRequiredString,
  displayDescription: validateRequiredString,
  displayName: validateRequiredString,
  domain: validateRequiredString,
  executeMode(v: ts.Expression): boolean {
    return v !== undefined && v !== null && ts.isArrayLiteralExpression(v) &&
      v.elements.every(e => {
        const validModes = [
          'insightIntent.ExecuteMode.UI_ABILITY_FOREGROUND',
          'insightIntent.ExecuteMode.UI_ABILITY_BACKGROUND',
          'insightIntent.ExecuteMode.UI_EXTENSION_ABILITY',
          'insightIntent.ExecuteMode.SERVICE_EXTENSION_ABILITY'
        ];
        return (ts.isNumericLiteral(e) && [0, 1, 2, 3].includes(Number(e.text))) || validModes.includes(e.getText());
      });
  },
  intentName: validateRequiredString,
  intentVersion: validateRequiredString,
  llmDescription: validateOptionalString
};

export const intentMethodInfoChecker: ParamChecker<IntentFunctionInfo> = new ParamChecker<IntentFunctionInfo>();
intentMethodInfoChecker.requiredFields = [...BASE_REQUIRED as Array<keyof IntentFunctionInfo>];
intentMethodInfoChecker.allowFields = new Set<keyof IntentFunctionInfo>([
  ...BASE_ALLOW as Array<keyof IntentFunctionInfo>
]);

intentMethodInfoChecker.paramValidators = {
  parameters: validateParameters,
  icon: validateIcon,
  keywords: validateKeywords,
  schema: validateRequiredString,
  intentName: validateRequiredString,
  domain: validateRequiredString,
  intentVersion: validateRequiredString,
  displayName: validateRequiredString,
  displayDescription: validateOptionalString,
  llmDescription: validateOptionalString
};

export const IntentPageInfoChecker: ParamChecker<IntentPageInfo> = new ParamChecker<IntentPageInfo>();
IntentPageInfoChecker.requiredFields = [...BASE_REQUIRED as Array<keyof IntentPageInfo>, 'pagePath'];
IntentPageInfoChecker.allowFields = new Set<keyof IntentPageInfo>([
  ...BASE_ALLOW as Array<keyof IntentPageInfo>,
  'uiAbility',
  'pagePath',
  'navigationId',
  'navDestinationName'
]);

IntentPageInfoChecker.paramValidators = {
  parameters: validateParameters,
  icon: validateIcon,
  keywords: validateKeywords,
  schema: validateRequiredString,
  intentName: validateRequiredString,
  domain: validateRequiredString,
  intentVersion: validateRequiredString,
  displayName: validateRequiredString,
  displayDescription: validateOptionalString,
  llmDescription: validateOptionalString,
  uiAbility: validateOptionalString,
  pagePath: validateRequiredString,
  navigationId: validateOptionalString,
  navDestinationName: validateOptionalString
};
