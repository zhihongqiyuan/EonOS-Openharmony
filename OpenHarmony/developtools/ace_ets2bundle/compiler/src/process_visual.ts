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

import ts from 'typescript';
import fs from 'fs';
import path from 'path';
import { SourceMapGenerator } from 'source-map';

import {
  validateUISyntax,
  componentCollection,
  ReplaceResult,
  sourceReplace
} from './validate_ui_syntax';
import {
  LogType,
  LogInfo,
  mkDir,
  emitLogInfo
} from './utils';
import {
  MODULE_ETS_PATH,
  MODULE_VISUAL_PATH,
  SUPERVISUAL,
  SUPERVISUAL_SOURCEMAP_EXT
} from './pre_define';

import { projectConfig } from '../main.js';
import { genETS } from '../codegen/codegen_ets.js';
import { concatenateEtsOptions, getExternalComponentPaths } from './external_component_map';

const visualMap: Map<number, number> = new Map();
const slotMap: Map<number, number> = new Map();

const red: string = '\u001b[31m';
const reset: string = '\u001b[39m';

let compilerOptions = ts.readConfigFile(
  path.resolve(__dirname, '../tsconfig.json'), ts.sys.readFile).config.compilerOptions;
const componentPaths: string[] | undefined = getExternalComponentPaths();
if (componentPaths) {
  for (const componentPath of componentPaths) {
    if (!fs.existsSync(componentPath)) {
      continue;
    }
    const externalCompilerOptions: ts.CompilerOptions = ts.readConfigFile(
      path.resolve(componentPath, 'externalconfig.json'), ts.sys.readFile
    ).config.compilerOptions;
    concatenateEtsOptions(compilerOptions, externalCompilerOptions);
  }
}
compilerOptions.sourceMap = false;

export function visualTransform(code: string, id: string, logger: any) {
  const log: LogInfo[] = [];
  const content: string | null = getParsedContent(code, path.normalize(id), log);
  if (!content) {
    return code;
  }
  if (log.length) {
    emitLogInfo(logger, log, true, id);
  }
  generateSourceMapForNewAndOriEtsFile(path.normalize(id), code);
  return content;
}

export function parseVisual(resourcePath: string, resourceQuery: string, content: string,
  log: LogInfo[], source: string): string {
  let code: string | null = getParsedContent(content, resourcePath, log);
  if (!code) {
    return content;
  }
  const result: ReplaceResult = sourceReplace(code, resourcePath);
  code = result.content;
  log.concat(result.log);
  const resultLog: LogInfo[] = validateUISyntax(source, code, resourcePath, resourceQuery);
  log.concat(resultLog);
  if (!log.length) {
    generateSourceMapForNewAndOriEtsFile(resourcePath, source);
  }
  return code;
}

function parseStatement(statement: ts.Statement, content: string, log: LogInfo[],
  visualContent: any): string {
  if (statement.kind === ts.SyntaxKind.StructDeclaration && statement.name) {
    if (statement.members) {
      statement.members.forEach(member => {
        if (member.kind && member.kind === ts.SyntaxKind.MethodDeclaration) {
          content = parseMember(statement, member, content, log, visualContent);
        }
      });
    }
  }
  return content;
}

function parseMember(statement: ts.Statement, member: ts.MethodDeclaration, content: string,
  log: LogInfo[], visualContent: any): string {
  let newContent: string = content;
  if (member.name && member.name.getText() === 'build') {
    const buildBody: string = member.getText();
    if (buildBody.replace(/\ +/g, '').replace(/[\r\n]/g, '') === 'build(){}') {
      newContent = insertVisualCode(statement, member, visualContent, newContent);
    } else {
      log.push({
        type: LogType.ERROR,
        message: `when the corresponding visual file exists,` +
          ` the build function of the entry component must be empty.`,
        pos: member.pos
      });
    }
  }
  return newContent;
}

function insertVisualCode(statement: ts.Statement, member: ts.MethodDeclaration,
  visualContent: any, content: string): string {
  let newContent: string = content;
  newContent = insertImport(visualContent, newContent);
  newContent = insertVarAndFunc(member, visualContent, newContent, content);
  newContent = insertBuild(member, visualContent, newContent, content);
  newContent = insertAboutToAppear(statement, member, visualContent, newContent, content);
  return newContent;
}

function insertImport(visualContent: any, content: string): string {
  if (!visualContent.etsImport) {
    return content;
  }
  const mediaQueryImport: string = visualContent.etsImport + '\n';
  const newContent: string = mediaQueryImport + content;
  slotMap.set(0, mediaQueryImport.length);
  visualMap.set(0, mediaQueryImport.split('\n').length - 1);
  return newContent;
}

function insertVarAndFunc(build: ts.MethodDeclaration, visualContent: any,
  content: string, oriContent: string): string {
  const visualVarAndFunc: string = (visualContent.etsVariable ? visualContent.etsVariable : '') +
    (visualContent.etsFunction ? visualContent.etsFunction : '');
  return visualVarAndFunc ? insertVisualCodeBeforePos(build, '\n' + visualVarAndFunc, content,
    oriContent) : content;
}

function insertBuild(build: ts.MethodDeclaration, visualContent: any, content: string,
  oriContent: string): string {
  return visualContent.build ? insertVisualCodeAfterPos(build.body,
    '\n' + visualContent.build + '\n', content, oriContent) : content;
}

function insertAboutToAppear(statement: ts.Statement, build: ts.MethodDeclaration,
  visualContent: any, content: string, oriContent: string): string {
  if (!visualContent.aboutToAppear) {
    return content;
  }
  for (const member of statement.members) {
    const hasAboutToAppear: boolean = member.kind && member.kind === ts.SyntaxKind.MethodDeclaration &&
      member.name && member.name.getText() === 'aboutToAppear';
    if (hasAboutToAppear) {
      return insertVisualCodeAfterPos(member.body, '\n' + visualContent.aboutToAppear, content,
        oriContent);
    }
  }

  const aboutToAppearFunc: string = '\n  aboutToAppear() {\n' + visualContent.aboutToAppear +
    '  }\n';
  return insertVisualCodeBeforePos(build, aboutToAppearFunc, content, oriContent);
}

function insertVisualCodeAfterPos(member: ts.Block, visualContent: string, content: string,
  oriContent: string): string {
  const contentBeforePos: string = oriContent.substring(0, member.getStart() + 1);
  const originEtsFileLineNumber: number = contentBeforePos.split('\n').length;
  const visualLines: number = visualContent.split('\n').length - 1;
  const insertedLineNumbers: number = visualMap.get(originEtsFileLineNumber);
  visualMap.set(originEtsFileLineNumber, insertedLineNumbers ? insertedLineNumbers + visualLines :
    visualLines);

  let newPos: number = member.getStart() + 1;
  for (const [key, value] of slotMap) {
    if (member.getStart() >= key) {
      newPos += value;
    }
  }

  const newContent: string = content.substring(0, newPos) + visualContent +
    content.substring(newPos);
  slotMap.set(member.getStart(), visualContent.length);
  return newContent;
}

function insertVisualCodeBeforePos(member: ts.MethodDeclaration, visualContent: string,
  content: string, oriContent: string): string {
  const contentBeforePos: string = oriContent.substring(0, member.pos);
  const originEtsFileLineNumber: number = contentBeforePos.split('\n').length;
  const visualLines: number = visualContent.split('\n').length - 1;
  const insertedLineNumbers: number = visualMap.get(originEtsFileLineNumber);
  visualMap.set(originEtsFileLineNumber, insertedLineNumbers ? insertedLineNumbers + visualLines :
    visualLines);
  let newPos: number = member.pos;
  for (const [key, value] of slotMap) {
    if (member.pos >= key) {
      newPos += value;
    }
  }
  const newContent: string = content.substring(0, newPos) + visualContent +
    content.substring(newPos);
  slotMap.set(member.pos, visualContent.length);
  return newContent;
}

function generateSourceMapForNewAndOriEtsFile(resourcePath: string, content: string) {
  if (!process.env.cachePath) {
    return;
  }
  const sourcemap: SourceMapGenerator = new SourceMapGenerator({
    file: resourcePath
  });
  const lines: Array<string> = content.split('\n');
  const originEtsFileLines: number = lines.length;
  for (let l: number = 1; l <= originEtsFileLines; l++) {
    let newEtsFileLineNumber: number = l;
    for (const [originEtsFileLineNumber, visualLines] of visualMap) {
      if (l > originEtsFileLineNumber) {
        newEtsFileLineNumber += visualLines;
      }
    }
    sourcemap.addMapping({
      generated: {
        line: newEtsFileLineNumber,
        column: 0
      },
      source: resourcePath,
      original: {
        line: l,
        column: 0
      }
    });
  }
  const visualMapName: string = path.parse(resourcePath).name + SUPERVISUAL_SOURCEMAP_EXT;
  const visualDirPath: string = path.parse(resourcePath).dir;
  const etsDirPath: string = path.parse(projectConfig.projectPath).dir;
  let visualMapDirPath: string = path.resolve(process.env.cachePath, SUPERVISUAL +
    visualDirPath.replace(etsDirPath, ''));
  if (!visualDirPath.includes(etsDirPath)) {
    const projectRootPath = getProjectRootPath();
    visualMapDirPath = path.resolve(process.env.cachePath, SUPERVISUAL +
      visualDirPath.replace(projectRootPath, ''));
  }
  if (!(fs.existsSync(visualMapDirPath) && fs.statSync(visualMapDirPath).isDirectory())) {
    mkDir(visualMapDirPath);
  }
  fs.writeFile(path.resolve(visualMapDirPath, visualMapName), sourcemap.toString(), (err) => {
    if (err) {
      console.error(red, 'ERROR: Failed to write visual.js.map', reset);
    }
  });
}

function getProjectRootPath(): string {
  let projectRootPath = projectConfig.projectRootPath;
  if (!projectRootPath) {
    if (!projectConfig.aceModuleJsonPath) {
      projectRootPath = path.resolve(projectConfig.projectPath, '../../../../../');
    } else {
      projectRootPath = path.resolve(projectConfig.projectPath, '../../../../');
    }
  }
  return projectRootPath;
}

export function findVisualFile(filePath: string): string {
  if (!/\.ets$/.test(filePath)) {
    return '';
  }
  let etsDirPath: string = path.parse(projectConfig.projectPath).dir;
  let visualDirPath: string = path.parse(projectConfig.aceSuperVisualPath).dir;
  let resolvePath = filePath.replace(projectConfig.projectPath, projectConfig.aceSuperVisualPath)
    .replace(etsDirPath, visualDirPath).replace(/\.ets$/, '.visual');
  if (fs.existsSync(resolvePath)) {
    return resolvePath;
  }
  try {
    const projectRootPath = getProjectRootPath();
    let moduleName = '';
    const relativePath = filePath.replace(projectRootPath, '');
    const moduleNames = relativePath.split(path.sep);
    for (let i = 0; i < moduleNames.length; ++i) {
      if (moduleNames[i] === 'src') {
        if (i >= moduleNames.length - 2) {
          break;
        }
        const modulePath = path.join(moduleNames[i], moduleNames[i + 1], moduleNames[i + 2]);
        if (modulePath === MODULE_ETS_PATH) {
          break;
        }
      }
      moduleName = path.join(moduleName, moduleNames[i]);
    }
    etsDirPath = path.join(projectRootPath, moduleName, MODULE_ETS_PATH);
    visualDirPath = path.join(projectRootPath, moduleName, MODULE_VISUAL_PATH);
    resolvePath = filePath.replace(etsDirPath, visualDirPath).replace(/\.ets$/, '.visual');
    return resolvePath;
  } catch (e) {
    // avoid projectConfig attributes has undefined value
    return '';
  }
}

function getVisualContent(visualPath: string, log: LogInfo[]): any {
  const parseContent: any = genETS(fs.readFileSync(visualPath, 'utf-8'));
  if (parseContent && parseContent.errorType && parseContent.errorType !== '') {
    log.push({
      type: LogType.ERROR,
      message: parseContent.errorMessage
    });
  }
  return parseContent ? parseContent.ets : null;
}

function getParsedContent(code: string, id: string, log: LogInfo[]): string | null {
  if (!projectConfig.aceSuperVisualPath ||
    !(componentCollection.entryComponent || componentCollection.customComponents)) {
      return null;
  }
  const visualPath: string = findVisualFile(id);
  if (!visualPath || !fs.existsSync(visualPath)) {
    return null;
  }
  const visualContent: any = getVisualContent(visualPath, log);
  if (!visualContent) {
    return null;
  }
  clearVisualSlotMap();
  const sourceFile: ts.SourceFile = ts.createSourceFile(
    id,
    code,
    ts.ScriptTarget.Latest,
    true,
    ts.ScriptKind.ETS,
    compilerOptions
  );
  let content: string = code;
  if (sourceFile.statements) {
    sourceFile.statements.forEach(statement => {
      content = parseStatement(statement, content, log, visualContent);
    });
  }
  return content;
}

function clearVisualSlotMap(): void {
  visualMap.clear();
  slotMap.clear();
}