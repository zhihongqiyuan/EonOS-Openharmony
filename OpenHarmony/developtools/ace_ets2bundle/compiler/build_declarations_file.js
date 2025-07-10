/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

const addTSInterfaceSet = ['ForEach', 'LazyForEach', 'TapGesture', 'LongPressGesture', 'LongPressGesture',
  'PanGesture', 'SwipeGesture', 'PinchGesture', 'RotationGesture', 'GestureGroup','PageTransitionEnter',
  'PageTransitionExit'];
const addTSAttributeSet = ['AlphabetIndexer', 'Animator', 'Badge', 'Blank', 'Button', 'Calendar', 'CalendarPicker',
  'Canvas', 'Checkbox', 'CheckboxGroup', 'Circle', 'Column', 'ColumnSplit', 'Counter', 'DataPanel', 'DatePicker',
  'Divider', 'Ellipse', 'Flex', 'FormComponent', 'Gauge', 'Grid', 'GridItem', 'GridContainer', 'Image',
  'ImageAnimator', 'IndicatorComponent', 'LazyVGridLayout', 'Line',
  'List', 'ListItem', 'ListItemGroup', 'LoadingProgress', 'Marquee', 'Navigation',
  'Navigator', 'Panel', 'Path', 'PatternLock', 'Piece', 'PluginComponent', 'Polygon', 'Polyline', 'Progress',
  'QRCode', 'Radio', 'Rating', 'Rect', 'Refresh', 'Row', 'RowSplit', 'Scroll', 'ScrollBar', 'Search',
  'Select', 'Shape', 'Sheet', 'Slider', 'Span', 'Stack', 'Stepper', 'StepperItem', 'Swiper',
  'TabContent', 'Tabs', 'Text', 'TextArea', 'TextClock', 'TextInput', 'TextPicker', 'TextTimer',
  'Toggle', 'Video', 'Web', 'XComponent', 'RichText', 'RemoteWindow', 'WaterFlow', 'FlowItem', 'ImageSpan',
  'RootScene', 'Screen', 'WindowScene', 'EffectComponent', 'RichEditor', 'ArcSwiper' , 'ArcScrollBar',
  'ArcList', 'ArcListItem', 'ArcAlphabetIndexer'];

generateTargetFile(process.argv[2], process.argv[3]);
function generateTargetFile(filePath, output) {
  const files = [];
  const globalTsFile = path.resolve(filePath, '../../ets/global.d.ts');
  const featureAbilityPath = path.resolve(filePath, '../../../common/full/featureability.d.ts');
  const middleTsFile = path.resolve(filePath, 'middle_class.d.ts');
  if (fs.existsSync(globalTsFile)) {
    files.push(globalTsFile);
  }
  if (fs.existsSync(featureAbilityPath)) {
    files.push(featureAbilityPath);
  }
  readFile(filePath, files);
  if (!fs.existsSync(output)) {
    mkDir(output);
  }
  const license = `/*
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
  */`;
  files.forEach((item) => {
    let content = fs.readFileSync(item, 'utf8');
    const fileName = path.resolve(output, path.basename(item));
    if (item === featureAbilityPath) {
      content = processsFile(content, fileName, true);
    } else if (item === globalTsFile) {
      content = license + '\n\n' + processsFile(content, fileName, true);
    } else {
      content = processImportType(content);
    }
    fs.writeFile(fileName, content, err => {
      if (err) {
        console.error(err);
        return;
      }
    });
  });
}

function processImportType(content) {
  return content.replace(/(import\s*\(("|'))(\.\.\/api\/[^("|')]*("|')\)\.)/g, (item, item1, item2, item3) => {
    return item1 + '../../' + item3;
  });
}

function readFile(dir, fileDir) {
  const files = fs.readdirSync(dir);
  files.forEach((element) => {
    const filePath = path.join(dir, element);
    const status = fs.statSync(filePath);
    if (status.isDirectory()) {
      readFile(filePath, fileDir);
    } else {
      fileDir.push(filePath);
    }
  });
}

function mkDir(filePath) {
  const parent = path.join(filePath, '..');
  if (!(fs.existsSync(parent) && !fs.statSync(parent).isFile())) {
    mkDir(parent);
  }
  fs.mkdirSync(filePath);
}

function processsFile(content, fileName, isGlobal) {
  let sourceFile = ts.createSourceFile(fileName, content, ts.ScriptTarget.Latest, true, ts.ScriptKind.TS);
  const newStatements = [];
  if (sourceFile.statements && sourceFile.statements.length) {
    if (isGlobal) {
      sourceFile.statements.forEach((node) => {
        if (!ts.isImportDeclaration(node)) {
          let modifiers = ts.canHaveModifiers(node) ? ts.getModifiers(node) : undefined;
          if (modifiers && modifiers.length && modifiers[0].kind === ts.SyntaxKind.ExportKeyword) {
            modifiers.splice(0, 1);
          }
          if (isVariable(node)) {
            const name = node.declarationList.declarations[0].name.getText();
            const type = node.declarationList.declarations[0].type.getText();
            if (name.indexOf(type) !== -1) {
              const declarationNode = ts.factory.updateVariableDeclaration(node.declarationList.declarations[0],
                ts.factory.createIdentifier(type), node.declarationList.declarations[0].exclamationToken,
                node.declarationList.declarations[0].type, node.declarationList.declarations[0].initializer);
              node.declarationList = ts.factory.updateVariableDeclarationList(node.declarationList, [declarationNode]);
            }
          }
          newStatements.push(node);
        }
      });
    } else {
      sourceFile.statements.forEach((node) => {
        processComponent(node, newStatements);
      });
    }
  }
  sourceFile = ts.factory.updateSourceFile(sourceFile, newStatements);
  const printer = ts.createPrinter({ removeComments: false, newLine: ts.NewLineKind.LineFeed });
  const result = printer.printNode(ts.EmitHint.Unspecified, sourceFile, sourceFile);
  return result;
}

function processComponent(node, newStatements) {
  let extendNode = null;
  if (isInterface(node)) {
    const componentName = node.name.getText().replace(/Interface$/, '');
    const result = validateComponentMembers(node, componentName);
    const modifiers = ts.canHaveModifiers(node) ? ts.getModifiers(node) : undefined;
    if (result.isComponentName) {
      const heritageClause = ts.factory.createHeritageClause(ts.SyntaxKind.ExtendsKeyword,
        [ts.factory.createExpressionWithTypeArguments(result.extendNode, undefined)]);
      extendNode = null;
      node = ts.factory.updateInterfaceDeclaration(node, modifiers,
        node.name, node.typeParameters, [heritageClause], node.members);
    }
    if (addTSInterfaceSet.includes(componentName)) {
      node = ts.factory.updateInterfaceDeclaration(node, modifiers, node.name,
        node.typeParameters, [ts.factory.createHeritageClause(ts.SyntaxKind.ExtendsKeyword,
          [ts.factory.createExpressionWithTypeArguments(ts.factory.createIdentifier('TS' + componentName + 'Interface'),
          undefined)])], node.members);
    }
  }
  if (isClass(node) && addTSAttributeSet.includes(node.name.getText().replace(/Attribute$/, ''))) {
    const modifiers = ts.canHaveModifiers(node) ? ts.getModifiers(node) : undefined;
    node = ts.factory.updateClassDeclaration(node, modifiers, node.name,
      node.typeParameters, [ts.factory.createHeritageClause(ts.SyntaxKind.ExtendsKeyword,
        [ts.factory.createExpressionWithTypeArguments(ts.factory.createIdentifier('TS' + node.name.getText()),
          undefined)])], node.members);
  }
  newStatements.push(node);
}

function validateComponentMembers(node, componentName) {
  let extendNode = null;
  let isComponentName = false;
  if (node.members) {
    for (let i = 0; i < node.members.length; i++) {
      const callSignNode = node.members[i];
      if (isSignNode(callSignNode)) {
        const callSignName = callSignNode.type.typeName.getText().replace(/Attribute$/, '');
        if (componentName === callSignName) {
          extendNode = callSignNode.type.typeName;
          isComponentName = true;
          break;
        }
      }
    }
  }
  return { isComponentName, extendNode };
}

function isVariable(node) {
  if (ts.isVariableStatement(node) && node.declarationList && node.declarationList.declarations &&
    node.declarationList.declarations.length && ts.isVariableDeclaration(node.declarationList.declarations[0]) &&
    node.declarationList.declarations[0].name && node.declarationList.declarations[0].type) {
    return true;
  }
  return false;
}

function isInterface(node) {
  return ts.isInterfaceDeclaration(node) && node.name && ts.isIdentifier(node.name) &&
    /Interface$/.test(node.name.getText());
}

function isClass(node) {
  return ts.isClassDeclaration(node) && node.name && ts.isIdentifier(node.name) &&
    /Attribute$/.test(node.name.getText());
}

function isSignNode(node) {
  return (ts.isCallSignatureDeclaration(node) || ts.isConstructSignatureDeclaration(node)) &&
    node.type && ts.isTypeReferenceNode(node.type) && node.type.typeName && ts.isIdentifier(node.type.typeName) &&
    /Attribute$/.test(node.type.typeName.getText());
}

generateComponentConfig(process.argv[4], process.argv[5]);
function generateComponentConfig(dir, buildPublicSDK) {
  const configFile = path.resolve(dir, 'component_map.js');
  if (fs.existsSync(configFile)) {
    const { COMPONENT_MAP, FORM_MAP, COMMON_ATTRS, forbiddenUseStateType } = require(configFile);
    const buildConfig = {
      forbiddenUseStateTypeForDecorators: [
        '@State', '@Prop', '@Link', '@Provide', '@Consume', '@ObjectLink', '@BuilderParam',
        '@LocalStorageLink', '@LocalStorageProp', '@StorageLink', '@StorageProp'
      ],
      forbiddenUseStateType: [...forbiddenUseStateType]
    };
    try {
      removeSystemApiComp(buildPublicSDK, COMPONENT_MAP, FORM_MAP);
      let commonMap = new Map();
      commonMap.attrs = [...COMMON_ATTRS];
      COMPONENT_MAP.Common = commonMap;
      fs.writeFileSync(path.resolve(dir, '../component_config.json'), JSON.stringify(COMPONENT_MAP));
      fs.writeFileSync(path.resolve(dir, '../form_config.json'), JSON.stringify(FORM_MAP));
      fs.writeFileSync(path.resolve(dir, '../build_config.json'), JSON.stringify(buildConfig));
    } catch (error) {
      console.error(error);
    }
  }
}

function removeSystemApiComp(buildPublicSDK, COMPONENT_MAP, FORM_MAP) {
  const systemApiComp = [];
  if (buildPublicSDK === 'true') {
    for (const comp in COMPONENT_MAP) {
      if (COMPONENT_MAP[comp] && COMPONENT_MAP[comp].systemApi) {
        systemApiComp.push(comp);
      }
    }
  }
  systemApiComp.forEach(comp => {
    if (COMPONENT_MAP[comp]) {
      delete COMPONENT_MAP[comp];
    }
    if (FORM_MAP[comp]) {
      delete FORM_MAP[comp];
    }
  });
}
