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

import { BaseElement } from '../../base-ui/BaseElement.js';
import { TabPaneTreeHtml } from './MemoryTree.html.js';
export class MemoryTree extends BaseElement {

  constructor() {
    super();
    this.treeDataSource = [];
    this.maxTreeDataSource = [];
    this.treeNodes = [];
    this.expandedNodeList = new Set();
    this.selectTreeDepth = 0;
    this.expansionClickEvent = () => {
      this.expandedNodeList.clear();
      if (this.expansionUpIcon.name === 'down') {
        this.selectTreeDepth = 0;
        this.expansionUpIcon.name = 'up';
        this.expansionDownIcon.name = 'down';
      } else {
        this.selectTreeDepth = 4;
        this.expansionUpIcon.name = 'down';
        this.expansionDownIcon.name = 'up';
      }
      this.refreshSelectDepth(this.treeNodes);
      this.refreshRowNodeTable(true);
    };
  }

  set data(treeDatas) {
    this.treeDataSource = [];
    this.expandedNodeList.clear();
    this.expansionUpIcon.name = 'up';
    this.expansionDownIcon.name = 'down';
    this.apSummaryTable.innerHTML = '';
    let firstTimestamp = treeDatas[0].timestamp;
    let data = treeDatas.filter(item => item.stage !== '')
     .map(item => (Object.assign(Object.assign({}, item), 
     { timestamp: item.timestamp - firstTimestamp, 
      parentStage: item.parentStage === undefined ? '' : item.parentStage }))); 
    this.filterMaxData(data);
    this.treeDataSource = data;
    if (this.treeDataSource.length !== 0) {
      this.refreshRowNodeTable();
    }
  }

  initElements() {
    this.apSummaryTable = this.shadowRoot.querySelector('#tab-summary');
    this.expansionDiv = this.shadowRoot.querySelector('.expansion-div');
    this.expansionUpIcon = this.shadowRoot.querySelector('.expansion-up-icon');
    this.expansionDownIcon = this.shadowRoot.querySelector('.expansion-down-icon');
    let summaryTreeLevel = ['ParentStage', '->Stage', '->Timestamp'];
    this.shadowRoot.querySelectorAll('.head-label').forEach((summaryTreeHead) => {
      summaryTreeHead.addEventListener('click', () => {
        this.selectTreeDepth = summaryTreeLevel.indexOf(summaryTreeHead.textContent);
        this.expandedNodeList.clear();
        this.refreshSelectDepth(this.treeNodes);
        this.refreshRowNodeTable(true);
      });
    });
    this.apSummaryTable.onscroll = () => {
      let treeTableEl = this.shadowRoot.querySelector('.log-tree-table');
      if (treeTableEl) {
        treeTableEl.scrollTop = this.apSummaryTable.scrollTop || 0;
      }
    };
  }

  filterMaxData(data) {
    const groupedData = data.reduce((groups, item) => {
    const key = `${item.stage}-${item.parentStage}`;
    if (!Object.prototype.hasOwnProperty.call(groups, key)) {
        groups[key] = [];
    }
    groups[key].push(item);
    return groups;
    }, {});
    const maxData = [];
    for (const key of Object.keys(groupedData)) {
        const group = groupedData[key];
        const maxRss = Math.max(...group.map(item => item.rss));
        const maxRssItem = group.find(item => item.rss === maxRss);
        if (maxRssItem) {
          maxData.push(Object.assign(Object.assign({}, maxRssItem)));
        }
     }
     this.maxTreeDataSource = maxData;
  }

  initHtml() {
    return TabPaneTreeHtml;
  }

  connectedCallback() {
    super.connectedCallback();
    new ResizeObserver(() => {
      this.parentElement.style.overflow = 'hidden';
      this.refreshRowNodeTable();
    }).observe(this.parentElement);
    this.expansionDiv.addEventListener('click', this.expansionClickEvent);
  }

  disconnectedCallback() {
    super.disconnectedCallback();
    this.expansionDiv.removeEventListener('click', this.expansionClickEvent);
  }

  refreshSelectDepth(treeNodes) {
    treeNodes.forEach((item) => {
      if (item.depth < this.selectTreeDepth) {
        this.expandedNodeList.add(item.id);
        if (item.children.length > 0) {
          this.refreshSelectDepth(item.children);
        }
      }
    });
  }

  getStageMax(map) {
   this.maxTreeDataSource.forEach(item => {
   let stage = item.stage + item.parentStage;
   if (map && map.get(stage)) {
     if (item.rss > 0) {
       map.get(stage).rss = item.rss;
     }
    } else {
      map.set(stage, { rss: item.rss });
    }
    return map;
    });
  }

  createTd(rowNode, rowNodeTextEL, type) {
    let td = document.createElement('td');
    if (type === 'rss') {
      td.textContent = rowNode.rss;
      td.className = 'rss-column-td';
      if (rowNode.depth === 0) {
        rowNodeTextEL.style.color = '#00000';
        td.style.color = '#00000';
      }
    }
    return td;
  }

  createRowNodeTableEL(rowNodeList, tableTreeEl, tableRssEl, rowColor = '') {
    let unitPadding = 20;
    let leftPadding = 5;
    let map = new Map();
    this.getStageMax(map);
    rowNodeList.forEach((rowNode) => {
    let tableTreeRowEl = document.createElement('tr');
    tableTreeRowEl.className = 'tree-row-tr';
    tableTreeRowEl.title = rowNode.parentStageName + '';
    let leftSpacingEl = document.createElement('td');
    leftSpacingEl.style.paddingLeft = `${rowNode.depth * unitPadding + leftPadding}px`;
    tableTreeRowEl.appendChild(leftSpacingEl);
    this.addToggleIconEl(rowNode, tableTreeRowEl);
    let rowNodeTextEL = document.createElement('td');
    rowNodeTextEL.textContent = rowNode.parentStageName + '';
    rowNodeTextEL.className = 'row-name-td';
    tableTreeRowEl.appendChild(rowNodeTextEL);
    tableTreeEl.appendChild(tableTreeRowEl);
    let rssRow = document.createElement('tr');
    rssRow.title = rowNode.rss;
    let rssTd = this.createTd(rowNode, rowNodeTextEL, 'rss');
    rssRow.appendChild(rssTd);
    tableRssEl.appendChild(rssRow);
    if (map.get(rowNode.stage + rowNode.parentStage)) {
      if (this.convertToMB(map.get(rowNode.stage + rowNode.parentStage).rss) === rowNode.rss) {
        rssTd.style.color = '#FF4040';
      }
    }
    if (rowNode.children && this.expandedNodeList.has(rowNode.id)) {
      this.createRowNodeTableEL(rowNode.children, tableTreeEl, tableRssEl, rssTd.style.color);
    }
   });
  }

  addToggleIconEl(rowNode, tableRowEl) {
    let toggleIconEl = document.createElement('td');
    let expandIcon = document.createElement('lit-icon');
    expandIcon.classList.add('tree-icon');
    if (rowNode.children && rowNode.children.length > 0) {
      toggleIconEl.appendChild(expandIcon);
      expandIcon.name = this.expandedNodeList.has(rowNode.id) ? 'minus-square' : 'plus-square';
      toggleIconEl.classList.add('expand-icon');
      toggleIconEl.addEventListener('click', () => {
        let scrollTop = this.apSummaryTable.scrollTop ?? 0;
        this.changeNode(rowNode.id);
        this.apSummaryTable.scrollTop = scrollTop;
      });
    }
    tableRowEl.appendChild(toggleIconEl);
  }

  changeNode(currentNode) {
    if (this.expandedNodeList.has(currentNode)) {
      this.expandedNodeList.delete(currentNode);
    } else {
      this.expandedNodeList.add(currentNode);
    }
    this.refreshRowNodeTable();
  }

  refreshRowNodeTable(useCacheRefresh = false) {
    this.apSummaryTable.innerHTML = '';
    if (this.apSummaryTable && this.parentElement) {
      this.apSummaryTable.style.height = `${this.parentElement.clientHeight - 100}px`;
    }
    if (!useCacheRefresh) {
      this.treeNodes = this.buildTreeTblNodes(this.treeDataSource);
    }
    let tableFragmentEl = document.createDocumentFragment();
    let tableTreeEl = document.createElement('div');
    tableTreeEl.className = 'log-tree-table';
    let tableRssEl = document.createElement('div');
    if (this.parentElement) {
      tableTreeEl.style.height = `${this.parentElement.clientHeight - 40}px`;
    }
    this.createRowNodeTableEL(this.treeNodes, tableTreeEl, tableRssEl, '');
    let emptyTr = document.createElement('tr');
    emptyTr.className = 'tree-row-tr';
    tableTreeEl === null || tableTreeEl === void 0 ? void 0 : tableTreeEl.appendChild(emptyTr);
    let emptyRssTr = document.createElement('tr');
    emptyRssTr.className = 'tree-row-tr';
    tableRssEl === null || tableRssEl === void 0 ? void 0 : tableRssEl.appendChild(emptyRssTr);
    tableFragmentEl.appendChild(tableTreeEl);
    tableFragmentEl.appendChild(tableRssEl);
    this.apSummaryTable.appendChild(tableFragmentEl);
	this.apSummaryTable.scrollTop = this.apSummaryTable.scrollTop + 1;
  }

  convertToMB(value) {
    return (value / (1024 * 1024)).toFixed(8) + 'MB';
  }

  buildTreeTblNodes(apTreeNodes) {
    let id = 0;
    let root = { id: id, depth: 0, children: [], parentStageName: 'All', parentStage: '',
      stage: '', rss: ''};
    apTreeNodes.forEach((item) => {
      id++;
      let parentNode = root.children.find((node) => node.parentStageName === item.parentStage);
      if (!parentNode) {
        id++;
        parentNode = {id: id, depth: 0, children: [], parentStageName: item.parentStage, parentStage: '-',
          stage: '-', rss: '-'
        };
        root.children.push(parentNode);
      }
      let childrenNode = parentNode.children.find((node) => node.parentStageName === item.stage);
      if (!childrenNode) {
        id++;
        childrenNode = {id: id, depth: 1, children: [], parentStageName: item.stage, parentStage: '-',
          stage: '-', rss: '-'
        };
        parentNode.children.push(childrenNode);
      }
      let timeNode = childrenNode.children.find((node) => node.parentStageName === item.timestamp);
      if (!timeNode) {
        id++;
        timeNode = { id: id, depth: 2, children: [], parentStageName: item.timestamp + '(ms)',
          parentStage: item.parentStage, stage: item.stage,
          rss: this.convertToMB(item.rss)
        };
        childrenNode.children.push(timeNode);
      }
    });
    return root.children.sort((leftData, rightData) => {
      return leftData.stage.localeCompare(rightData.stage);
    });
  }
}

if (!customElements.get('memory-dotting-tree')) {
   customElements.define('memory-dotting-tree', MemoryTree);
}
export class TreeBean {
  constructor() {
    this.rss = 0;
  }
}
