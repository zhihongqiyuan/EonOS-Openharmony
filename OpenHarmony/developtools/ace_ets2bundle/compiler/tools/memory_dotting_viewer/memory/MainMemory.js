/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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
import {BaseElement} from '../base-ui/BaseElement.js';
import '../base-ui/menu/LitMainMenu.js';
import '../base-ui/icon/LitIcon.js';
import '../base-ui/progress-bar/LitProgressBar.js';
import './component/WelcomePage.js';
import {error, info} from '../log/Log.js';
import {appHtml,} from './MainMemoryFunc.js';
import './component/MemoryDotting.js';
import './component/MemoryTree.js';
import { MemoryDotting } from './component/MemoryDotting.js';

export class MainMemory extends BaseElement {
  static get observedAttributes() {
    return ['subsection'];
  }

  initHtml() {
    return appHtml;
  }

  initElements() {
    this.rootEL = this.shadowRoot.querySelector('.root');
    this.welcomePage = this.shadowRoot.querySelector('#memory-welcome');
    this.table = this.shadowRoot.querySelector('#tabpane-summary');
    this.tree = this.shadowRoot.querySelector('#tab-pane-tree');
    this.mainMenu = this.shadowRoot.querySelector('#main-menu');
    this.menu = this.mainMenu.shadowRoot.querySelector('.menu-button');
    this.initElementsAttr();
    this.resetMenus();
    this.initGlobalEvents();
    this.downloadFile();
  }

  initElementsAttr() {
    this.mainMenu.setAttribute('main_menu', '1');
    this.childComponent = [
      this.table,
      this.welcomePage,
      this.tree
    ];
  }



  async downloadFile() {
    let localUrl = `${window.location.origin}/get_filename`;
    const response = await fetch(localUrl);
    if (!response.ok) {
        throw new Error('Failed to get filename: ' + response.statusText);
    }
    const filename = await response.text();
    console.log(filename);
    const downloadRes = await fetch(`${window.location.origin}/download?filename=${encodeURIComponent(filename)}`);
    if (!downloadRes.ok) {
        throw new Error('Failed to download file: ' + downloadRes.statusText);
    }
    const blob = await downloadRes.blob();
    const file = new File([blob], filename, {type: blob.type});
    this.openTraceFile(file);
  }

  openTraceFile(file) {
    this.openFileInit();
    let fileName = file.name;
    let showFileName = fileName.lastIndexOf('.') === -1 ? fileName : fileName.substring(0, fileName.lastIndexOf('.'));
    this.parseLogFile(file, showFileName, file.size, fileName);
  }

  initGlobalDropEvents() {
    let body = document.querySelector('body');
    body.addEventListener('drop', (event) => {
      let fileItem;
      let dirItem;
      event.preventDefault();
      event.stopPropagation();
      if (this.rootEL.classList.contains('filedrag')) {
        this.rootEL.classList.remove('filedrag');
      }
      if (event && event.dataTransfer && event.dataTransfer.items !== undefined && event.dataTransfer.items.length > 0) {
        let item = event.dataTransfer.items[0];
        if ((fileItem = item.webkitGetAsEntry()) === null || fileItem === void 0 ? void 0 : fileItem.isFile) {
          this.openTraceFile(item.getAsFile());
        } else if ((dirItem = item.webkitGetAsEntry()) === null || dirItem === void 0 ? void 0 : dirItem.isDirectory) {
          this.freshMenuDisable(false);
        }
      }
    }, false);
  }

  initGlobalEvents() {
    let body = document.querySelector('body');
    body.addEventListener('dragover', (event) => {
      event.preventDefault();
      event.stopPropagation();
      if (event && event.dataTransfer && event.dataTransfer.items.length > 0 && event.dataTransfer.items[0].kind === 'file') {
        event.dataTransfer.dropEffect = 'copy';
        if (!this.rootEL.classList.contains('filedrag')) {
          this.rootEL.classList.add('filedrag');
        }
      }
    }, false);
    body.addEventListener('dragleave', (event) => {
      event.stopPropagation();
      event.preventDefault();
      if (this.rootEL.classList.contains('filedrag')) {
        this.rootEL.classList.remove('filedrag');
      }
    }, false);
    this.initGlobalDropEvents();
  }

  initDocumentListener() {
    document.addEventListener('file-error', () => {
    });
  }

  initNavigationMenu() {
    return [
      {
        collapsed: false,
        title: 'Navigation',
        second: false,
        icon: 'caret-down',
        describe: 'Import file',
        children: [
          {
            title: 'Import file',
            icon: '',
            fileChoose: true,
            fileHandler: (ev) => {
              this.openTraceFile(ev.detail);
            },
          },
          {
              title: 'Chart',
              icon: '',
              clickHandler: (item) => this.clickHandleChart(),
          },
          {
              title: 'Tree',
              icon: '',
              clickHandler: (item) => this.clickHandleTree(),
          }
        ],
      }
    ];
  }

  clickHandleChart() {
    this.showContent(this.table);
  }
  clickHandleTree() {
    this.showContent(this.tree);
  }

  parseLogFile(file, showFileName, fileSize, fileName) {
    let fileSizeStr = (fileSize / 1048576).toFixed(1);
    document.title = `${showFileName} (${fileSizeStr}M)`;
    console.log('fileName:' + fileName);
    if (fileName.endsWith('.log') || fileName.endsWith('.txt')) {
      let reader = new FileReader();
      reader.readAsArrayBuffer(file);
      reader.onloadend = () => {
        let arrayBuffer = reader.result;
        let decoder = new TextDecoder('utf-8');
        let text = decoder.decode(arrayBuffer);
        let modifiedText = `[${text.trim().replace(/,\s*$/, '')}]`;
        let datas = JSON.parse(modifiedText);
        this.freshMenuDisable(false);
        let data = MemoryDotting.filterKeyPoints(datas, 10240);
        this.table.data = data;
        this.tree.data = data;
      };
    } else {
      this.freshMenuDisable(false);
    }
  }

  resetMenus() {
    this.mainMenu.menus = [...this.initNavigationMenu()];
  }

  openFileInit() {
    info('open memory File');
    this.resetMenus();
    this.freshMenuDisable(true);
    this.showContent(this.table);
  }

  showContent(showNode) {
    if (showNode === this.table) {
      this.menu.style.pointerEvents = 'auto';
    } else {
      this.menu.style.pointerEvents = 'none';
    }
    this.childComponent.forEach((node) => {
      if (node === showNode) {
        showNode.style.visibility = 'visible';
      } else {
        node.style.visibility = 'hidden';
      }
    });
  }

  freshMenuDisable(disable) {
    let menu;
    this.mainMenu.menus[0].children[0].disabled = disable;
    if (this.mainMenu.menus.length > 2) {
      (menu = this.mainMenu.menus) === null || menu === void 0 ? void 0 : menu[1].children.map((it) => (it.disabled = disable));
    }
    this.mainMenu.menus = this.mainMenu.menus;
  }
}

if (!customElements.get('main-memory')) {
  customElements.define('main-memory', MainMemory);
}
