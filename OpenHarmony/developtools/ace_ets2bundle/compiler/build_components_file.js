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

const ts = require('typescript');
const path = require('path');
const fs = require('fs');

/**
 * Directory location to API .d.ts files.
 */
const SOURCE_DTS_DIR = process.argv[2];

/**
 * Directory location to component JSON files.
 */
const COMPONENT_OUTPUT_DIR = process.argv[3];

/**
 * Directory location to form component JSON files.
 */
const FORM_COMPONENT_OUTPUT_DIR = process.argv[4];

/**
 * Map _special_ extend class name to method names under the class.
 * 
 * @type {Map<string, string[]>}
 */
const SPECIAL_EXTEND_ATTRS = new Map();

/**
 * _special_ extend class names
 */
const SPECIAL_EXTEND_ATTR_NAMES = [
  'DynamicNode', 
  'BaseSpan', 
  'ScrollableCommonMethod',
  'CommonShapeMethod',
  'SecurityComponentMethod'
];

/**
 * _special_ component data array. Each data contains:
 * 
 * - componentName: the component JSON file name.
 * - apiName: the API .d.ts file name.
 * - className: the class name in the API .d.ts file.
 * - includeClassName: whether needs to include the class name in the component JSON file.
 */
const SPECIAL_COMPONENTS = [
  {
    componentName: 'common_attrs',
    apiName: 'common',
    className: 'CommonMethod',
    includeClassName: false,
  }
];

/**
 * API .d.ts file names to skip finding components.
 */
const COMPONENT_WHITE_LIST = ['common'];

/**
 * API .d.ts file names to skip finding form components.
 */
const FORM_COMPONENT_WHITE_LIST = ['common'];

registerSpecialExtends(path.join(SOURCE_DTS_DIR, 'common.d.ts'));
registerSpecialExtends(path.join(SOURCE_DTS_DIR, 'span.d.ts'));
registerSpecialExtends(path.join(SOURCE_DTS_DIR, 'security_component.d.ts'));
generateSpecialTargetFiles();
generateTargetFile(SOURCE_DTS_DIR);

/**
 * Register _special_ extend classes (i.e. classes except CommonMethod that Component attributes' extends from).
 * 
 * Registered classes will be recognized later when an Component attribute extends any of them, 
 * then parse those methods in the extend class as the methods of the Component attribute.
 * 
 * @param {string} filePath File location of the special extend classes.
 */
function registerSpecialExtends(filePath) {
  const content = fs.readFileSync(filePath, 'utf8');
  const sourceFile = ts.createSourceFile(filePath, content, ts.ScriptTarget.Latest, true, ts.ScriptKind.TS);

  ts.forEachChild(sourceFile, (node) => {
    if (isSpecialExtendClass(node)) {
      const specialExtendClassName = node.name.getText();
      const attrs = getAttrs(node, false);
      SPECIAL_EXTEND_ATTRS.set(specialExtendClassName, attrs);
    }
  });
}

/**
 * Generate or update component JSON files for the special extend classes (e.g. CommonMethod).
 */
function generateSpecialTargetFiles() {
  SPECIAL_COMPONENTS.forEach((special) => {
    const { componentName, apiName, className, includeClassName } = special;

    const apiFilePath = path.join(SOURCE_DTS_DIR, `${apiName}.d.ts`);

    const content = fs.readFileSync(apiFilePath, 'utf8');
    const sourceFile = ts.createSourceFile(apiFilePath, content, ts.ScriptTarget.Latest, true, ts.ScriptKind.TS);

    ts.forEachChild(sourceFile, (node) => {
      if (!(ts.isClassDeclaration(node) && node.name && ts.isIdentifier(node.name) &&
        node.name.getText() === className)) {
        return;
      }

      const [flags, isForm] = getFlags(node);
      const component = { attrs: Array.from(new Set(getAttrs(node, false))), ...flags };
      const formComponent = { attrs: Array.from(new Set(getAttrs(node, isForm))), ...flags };

      if (includeClassName) {
        component.name = className;
        formComponent.name = className;
      }

      generateFormAndComponents(componentName, component, formComponent, isForm);
    });
  });
}

/**
 * Generate or update component JSON files for the Component attributes.
 * 
 * @param {string} filePath Directory location of API .d.ts files.
 */
function generateTargetFile(filePath) {
  const files = [];
  readFile(filePath, files);

  const program = ts.createProgram(files, {});
  const checker = program.getTypeChecker();
  const sourceFiles = program.getSourceFiles()
    .filter((f) => files.includes(toUnixPath(f.fileName)));

  sourceFiles.forEach((sourceFile) => {
    const sourceFilePath = path.parse(toUnixPath(sourceFile.fileName));
    const baseName = path.basename(sourceFilePath.name, path.extname(sourceFilePath.name));

    const [component, formComponent, isForm] = findComponent(sourceFile, checker, program);

    Object.keys(component).length > 0 && 
      generateFormAndComponents(baseName, component, formComponent, isForm);
  });
}

/**
 * Generate or update component JSON files for form and non-form components.
 * 
 * @param {string} componentName Component name.
 * @param {object} component Component JSON data.
 * @param {object} formComponent Form component JSON data.
 * @param {boolean} isForm Whether the component can be a form component.
 */
function generateFormAndComponents(componentName, component, formComponent, isForm) {
  if (!COMPONENT_WHITE_LIST.includes(componentName)) {
    if (isForm && !FORM_COMPONENT_WHITE_LIST.includes(componentName)) {
      const formComponentFileName = path.join(FORM_COMPONENT_OUTPUT_DIR, `${componentName}.json`);
      generateComponentJSONFile(formComponentFileName, formComponent);
    }
    const componentFileName = path.join(COMPONENT_OUTPUT_DIR, `${componentName}.json`);
    generateComponentJSONFile(componentFileName, component);
  }
}

/**
 * Write file utility for generating or updating component JSON files.
 * 
 * This only updates the `attrs` attribute in the JSON file.
 * 
 * @param {string} filePath File location to write the component JSON file.
 * @param {object} component component JSON object.
 */
function generateComponentJSONFile(filePath, component) {
  if (!fs.existsSync(filePath)) {
    fs.writeFileSync(path.resolve(filePath), JSON.stringify(component, null, 2));
    return;
  }
  const source = JSON.parse(fs.readFileSync(filePath, 'utf8'));
  const updateComponent = { ...source, ...component };
  fs.writeFileSync(path.resolve(filePath), JSON.stringify(updateComponent, null, 2));
}

/**
 * Read file utility for reading all files in the directory `dir`.
 * 
 * Add all file pathes to the array `fileDir`.
 * 
 * @param {string} dir Directory location for reading all files. 
 * @param {string[]} fileDir All file pathes array. 
 */
function readFile(dir, fileDir) {
  const files = fs.readdirSync(dir);
  files.forEach((element) => {
    const filePath = path.join(dir, element);
    const status = fs.statSync(filePath);
    if (status.isDirectory()) {
      readFile(filePath, fileDir);
    } else {
      fileDir.push(path.resolve(filePath));
    }
  });
}

/**
 * Find any Component attribute in the API .d.ts file.
 * 
 * @param {ts.SourceFile} sourceFile .d.ts file.
 * @param {ts.TypeChecker} checker type checker.
 * @param {ts.Program} program program.
 * @returns {[object, object, boolean]} A component JSON object, a form component JSON object, 
 * and a boolean to indicate whether the Component attribute can be a form component.
 */
function findComponent(sourceFile, checker, program) {
  let component = {};
  let formComponent = {};
  let isForm = false;

  ts.forEachChild(sourceFile, (node) => {
    if (isClass(node)) {
      const [flags, _isForm] = getFlags(node);
      component = {
        name: node.name.getText().replace(/Attribute$/, ''),
        attrs: Array.from(new Set([
          ...getAttrs(node, false),
          ...getCommonAttrs(node)
        ])),
        ...flags
      };
      formComponent = {
        name: node.name.getText().replace(/Attribute$/, ''),
        attrs: Array.from(new Set([
          ...getAttrs(node, _isForm),
          ...getCommonAttrs(node)
        ])),
        ...flags
      };
      isForm = _isForm;
    }
  });

  return [component, formComponent, isForm];
}

/**
 * Check whether a AST Node is a class.
 * 
 * @param {ts.Node} node 
 * @returns {boolean}
 */
function isClass(node) {
  return ts.isClassDeclaration(node) && node.name && ts.isIdentifier(node.name) &&
    /Attribute$/.test(node.name.getText());
}

/**
 * Check whether a AST Node is a _special_ extend class.
 * 
 * @param {ts.Node} node 
 * @returns {boolean}
 */
function isSpecialExtendClass(node) {
  return ts.isClassDeclaration(node) && node.name && ts.isIdentifier(node.name) &&
    SPECIAL_EXTEND_ATTR_NAMES.includes(node.name.getText());
}

/**
 * Check whether a AST Node is a method.
 * 
 * @param {ts.Node} node 
 * @returns {boolean}
 */
function isMethod(node) {
  return ts.isMethodDeclaration(node) && node.name && ts.isIdentifier(node.name) &&
    node.name.escapedText;
}

/**
 * Check whether a AST Node is a class and extends any _special_ extend class.
 * 
 * @param {ts.Node} node 
 * @returns {boolean}
 */
function isExtendSpecialDeclaration(node) {
  return ts.isClassDeclaration(node) && node.heritageClauses && node.heritageClauses.length > 0;
}

/**
 * Check whether a method belongs to the form component.
 * 
 * @param {ts.Node} node 
 * @returns {boolean}
 */
function isFormComponent(node) {
  const flags = getFlags(node);
  return flags && flags[1];
}

/**
 * Get all identifiers that this class extends.
 * 
 * @param {ts.Node} node 
 * @returns {ts.Expression[]}
 */
function getExtendIdentifiers(node) {
  if (!ts.isHeritageClause(node)) {
    return [];
  }

  const identifiers = [];
  node.types.forEach((type) => {
    if (ts.isExpressionWithTypeArguments(type) && type.expression) {
      identifiers.push(type.expression);
    }
  });

  return identifiers;
}

/**
 * Get all method names from _special_ extend class.
 * 
 * @param {ts.ClassDeclaration} node 
 * @returns {string[]}
 */
function getCommonAttrs(node) {
  if (!isExtendSpecialDeclaration(node)) {
    return [];
  }

  const attrs = [];
  const heritageClause = node.heritageClauses[0];
  const identifiers = getExtendIdentifiers(heritageClause);
  identifiers.forEach((identifier) => {
    if (SPECIAL_EXTEND_ATTRS.has(identifier.escapedText)) {
      attrs.push(...SPECIAL_EXTEND_ATTRS.get(identifier.escapedText));
    }
  });
  return attrs;
}

/**
 * Get all method names from the class (without any method names from extend class).
 * 
 * @param {ts.ClassDeclaration} node 
 * @param {boolean} shouldFilterForm Whether needs to filter any method names that belongs to form component.
 * @returns {string[]}
 */
function getAttrs(node, shouldFilterForm) {
  const attrs = [];

  ts.forEachChild(node, (child) => {
    if (isMethod(child) && (!shouldFilterForm || isFormComponent(child))) {
      const attrName = child.name.escapedText;
      attrs.push(attrName);
    }
  });

  return attrs;
}

/**
 * Get flags and whether is form component boolean.
 * 
 * @param {ts.Node} node 
 * @returns {[string[], boolean]} Flag names, whether is form component.
 */
function getFlags(node) {
  const tags = parseTags(node);
  const flags = filterFlags(tags);

  if (flags.form) {
    delete flags.form;
    return [flags, true];
  }
  return [flags, false];
}

/**
 * Parse all JSDoc tags from a AST Node.
 * 
 * @param {ts.Node} node 
 * @returns {string[]}
 */
function parseTags(node) {
  const tags = [];
  const jsTags = ts.getJSDocTags(node);

  jsTags.forEach((jsTag) => {
    tags.push(jsTag.tagName.getText());
  });
  return tags;
}

/**
 * Filter useful tags from a tag array.
 * 
 * Currently, we only parse `@form`.
 * 
 * @param {string} tags 
 * @returns {object}
 */
function filterFlags(tags) {
  let form;

  tags.forEach((tag) => {
    const name = typeof tag === 'string' ? tag : tag?.title;

    if (name) {
      if (name === 'form') {
        form = true;
      }
    }
  });

  return {
    ...(form && { form }),
  };
}

/**
 * Convert file path format to linux-style.
 * 
 * @param {string} data file location. 
 * @returns {string}
 */
function toUnixPath(data) {
  if (/^win/.test(require('os').platform())) {
    const fileTmps = data.split(path.sep);
    const newData = path.posix.join(...fileTmps);
    return newData;
  }
  return data;
}
