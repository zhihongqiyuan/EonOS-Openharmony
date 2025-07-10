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
  ScriptTarget,
  SymbolFlags,
  createCompilerHost,
  createProgram,
  createSourceFile
} from 'typescript';

import type {
  CompilerHost,
  CompilerOptions,
  Program,
  SourceFile,
  Symbol,
  TypeChecker
} from 'typescript';
import { Extension, PathAndExtension } from '../common/type';
import { FileUtils } from './FileUtils';
import { EventList, performancePrinter } from '../ArkObfuscator';
import { endSingleFileEvent, startSingleFileEvent } from './PrinterUtils';
import { exportSymbolAliasMap } from './ScopeAnalyzer';

export class TypeUtils {
  /**
   * Create .d.ets, .d.ts, .ts ast from .d.ets, .d.ts, .ts content.
   * Create .ts ast from .ets, .js content
   * @param {string} sourceFilePath 
   * @param {string} content - The content in sourceFilePath
   */
  public static createObfSourceFile(sourceFilePath: string, content: string, compilerOptions?: CompilerOptions): SourceFile {
    const pathOrExtension: PathAndExtension = FileUtils.getFileSuffix(sourceFilePath);
    const fileSuffix = pathOrExtension.ext;

    if (fileSuffix === Extension.JS) {
      sourceFilePath = pathOrExtension.path + Extension.TS;
    }

    if (compilerOptions && compilerOptions.etsAnnotationsEnable === true) {
      return createSourceFile(sourceFilePath, content, ScriptTarget.ES2015, true, undefined, compilerOptions, true);
    }

    return createSourceFile(sourceFilePath, content, ScriptTarget.ES2015, true);
  }

  public static tsToJs(ast: SourceFile): void {
    const pathOrExtension: PathAndExtension = FileUtils.getFileSuffix(ast.fileName);
    const fileSuffix = Extension.JS;
    const targetName: string = pathOrExtension.path + fileSuffix;
    ast.fileName = targetName;
  }

  public static createChecker(ast: SourceFile): TypeChecker {
    const host: CompilerHost = createCompilerHost({});

    const customHost: CompilerHost = {
      getSourceFile(name, languageVersion): SourceFile | undefined {
        if (name === ast.fileName) {
          return ast;
        } else {
          return undefined;
        }
      },
      // optional
      getDefaultLibLocation: () => '',
      getDefaultLibFileName: () => '',
      writeFile: (filename, data) => {
      },
      getCurrentDirectory: () => '',
      useCaseSensitiveFileNames: host.useCaseSensitiveFileNames,
      getCanonicalFileName: host.getCanonicalFileName,
      getNewLine: host.getNewLine,
      fileExists: () => true,
      readFile: (name): string => {
        return name === ast.fileName ? ast.text : undefined;
      },
      // must, read program.ts => createCompilerHost
      directoryExists: undefined,
      getEnvironmentVariable: undefined,
      getDirectories: undefined,
    };

    let option: CompilerOptions = {
      'alwaysStrict': true
    };
    if (ast.fileName.endsWith('.js')) {
      option.allowJs = true;
    }

    startSingleFileEvent(EventList.CREATE_PROGRAM, performancePrinter.timeSumPrinter);
    let program: Program = createProgram([ast.fileName], option, customHost);
    endSingleFileEvent(EventList.CREATE_PROGRAM, performancePrinter.timeSumPrinter);

    startSingleFileEvent(EventList.GET_CHECKER, performancePrinter.timeSumPrinter);
    let typeChecker: TypeChecker = program.getTypeChecker();
    endSingleFileEvent(EventList.GET_CHECKER, performancePrinter.timeSumPrinter);
    return typeChecker;
  }

  /**
   * Retrieves the symbol associated with the declaration site of the given symbol.
   *
   * This method resolves the symbol to its declaration site to ensure consistency
   * in obfuscated naming. Obfuscation names are bound to the symbol at the declaration
   * site. If the symbol at the declaration site differs from the symbol at the usage
   * site, discrepancies in obfuscated names may occur. By using this method, the
   * obfuscation name can be consistently retrieved from the declaration site symbol,
   * ensuring uniformity between the declaration and usage sites.
   */
  public static getOriginalSymbol(symbol: Symbol, checker: TypeChecker): Symbol {
    if (!(symbol.getFlags() & SymbolFlags.Alias)) {
      return symbol;
    }

    if (exportSymbolAliasMap.has(symbol)) {
      return exportSymbolAliasMap.get(symbol);
    }

    // This method helps determine if the original symbol obtained for a given symbol is valid and usable.
    // Sometimes the original symbol's name may differ from the current symbol's name due to aliasing.
    // For example:
    // `let A = 1; export { A as B };`
    // The `originalSymbol` for `B` refers to the symbol for `A`, but the names are different.
    // However, for obfuscation purposes, we want to ensure that the symbol used in the declaration (i.e., `A` in this case)
    // aligns with the current symbol (i.e., `B`), because this allows us to apply the declaration's obfuscation name to `B`.
    // If the names don't match, we should avoid applying the declaration's obfuscation name to the current symbol.
    const isValidOriginalSymbol = (originalSymbol: Symbol | undefined): boolean =>
      originalSymbol !== undefined && originalSymbol.name === symbol.name;

    const originalSymbol: Symbol | undefined = checker.getAliasedSymbol(symbol);
    if (isValidOriginalSymbol(originalSymbol)) {
      return originalSymbol!;
    }

    const immediateSymbol: Symbol | undefined = checker.getImmediateAliasedSymbol(symbol);
    if (isValidOriginalSymbol(immediateSymbol)) {
      return immediateSymbol!;
    }

    return symbol;
  }
}
