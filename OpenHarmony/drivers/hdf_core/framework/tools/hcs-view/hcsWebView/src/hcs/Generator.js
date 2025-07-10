/*
 * Copyright (c) 2022-2023 Shenzhen Kaihong Digital Industry Development Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http:// www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

const { NapiLog } = require('./NapiLog');
const { NodeTools } = require('./NodeTools');
const { Parser } = require('./parser');
const { DataType } = require('./NodeTools');

class Generator {
  constructor() {
    this.parser = new Parser();
  }

  hcsToAst(fn) {
    return this.parser.parse(fn);
  }

  DATA_NODES_NOT_INHERIT = 0;
  CLASS_NODES_COPY = 1;
  CLASS_NODES_REFERENCE = 2;
  CLASS_NODES_DELETION = 3;
  CLASS_NODES_TEMPLETE = 4;
  DATA_NODES_INHERIT = 5;

  astToObjConfigNodeType(nodeType, obj, node) {
    if (
      nodeType === this.DATA_NODES_INHERIT ||
      nodeType === this.CLASS_NODES_COPY ||
      nodeType === this.CLASS_NODES_REFERENCE
    ) {
      obj.ref_ = node.refNodePath_;
    } else if (
      nodeType !== this.DATA_NODES_NOT_INHERIT &&
      nodeType !== this.CLASS_NODES_DELETION &&
      nodeType !== this.CLASS_NODES_TEMPLETE
    ) {
      NapiLog.logError('unknow node type');
    }
  }

  astToObjConfigNode(ret, child, node) {
    ret.value_ = [];
    child = node.child_;
    while (child !== undefined) {
      ret.value_.push(this.astToObj(child, ret));
      child = child.next_;
    }
    ret.nodeType_ = node.nodeType_;
    ret.isOpen_ = true;
    ret.nodeWidth_ = 132;
    this.astToObjConfigNodeType(node.nodeType_, ret, node);
  }

  astToObj(node, parent) {
    let child;
    let ret = new Object();
    ret.type_ = node.type_;
    ret.name_ = node.name_;
    ret.lineno_ = node.lineno_;
    ret.parent_ = parent;
    ret.nodeWidth_ = 132;
    switch (node.type_) {
      case DataType.INT8:
      case DataType.INT16:
      case DataType.INT32:
      case DataType.INT64:
        ret.value_ = node.integerValue_;
        ret.jinzhi_ = node.jinzhi_;
        break;
      case DataType.STRING:
        ret.value_ = node.stringValue_;
        break;
      case DataType.NODE:
        this.astToObjConfigNode(ret, child, node);
        break;
      case DataType.ATTR:
        ret.value_ = this.astToObj(node.child_, ret);
        break;
      case DataType.ARRAY:
        ret.value_ = [];
        child = node.child_;
        while (child !== undefined) {
          ret.value_.push(this.astToObj(child, ret));
          child = child.next_;
        }
        ret.arraySize_ = node.arraySize_;
        ret.arrayType_ = node.arrayType_;
        break;
      case DataType.REFERENCE:
        ret.value_ = node.stringValue_;
        break;
      case DataType.DELETE:
        ret.value_ = null;
        break;
      case DataType.BOOL:
        if (node.integerValue_ === 0) {
          ret.value_ = false;
        }
        else {
          ret.value_ = true;
        }
        break;
      default:
        NapiLog.logError('unknow type');
        break;
    }
    return ret;
  }

  mergeObj(files) {
    let objList = [];
    for (let i in files) {
      objList.push(files[i]);
    }
    let err = true;
    for (let i in objList) {
      err = err && NodeTools.redefineCheck(objList[i]);
    }
    if (!err) {
      return false;
    }
    objList.push(objList.shift());
    let ret = {
      type_: DataType.NODE,
      name_: 'root',
      value_: [],
    };
    for (let i in objList) {
      NodeTools.merge(ret, objList[i]);
    }
    return ret;
  }

  expandObj(node) {
    NodeTools.nodeExpand(node);
    NodeTools.inheritExpand(node);
    NodeTools.nodeNestCheck(node);
    return node;
  }

  makeSpace(deep) {
    let ret = '';
    for (let i = 0; i < deep; i++) ret += '    ';
    return ret;
  }

  makeHcs(fn, node) {
    let body = this.objToHcs(node, 0);
    let head = '';

    let fpath = fn.substring(0, fn.lastIndexOf('\\') + 1);
    for (let i in this.parser.astList[fn].include) {
      head +=
        '#include "./' +
        this.parser.astList[fn].include[i].substring(fpath.length) +
        '"\n';
    }
    return head + body;
  }

  objToHcsConfigNode(node, deep) {
    let ret = '';
    if (node.nodeType_ === this.DATA_NODES_NOT_INHERIT) {
      ret = this.makeSpace(deep) + node.name_ + ' {\n';
    } else if (node.nodeType_ === this.CLASS_NODES_COPY) {
      ret = this.makeSpace(deep) + node.name_ + ' : ' + node.ref_ + ' {\n';
    } else if (node.nodeType_ === this.CLASS_NODES_REFERENCE) {
      ret = this.makeSpace(deep) + node.name_ + ' : &' + node.ref_ + ' {\n';
    } else if (node.nodeType_ === this.CLASS_NODES_DELETION) {
      ret = this.makeSpace(deep) + node.name_ + ' : delete {\n';
    } else if (node.nodeType_ === this.CLASS_NODES_TEMPLETE) {
      ret = this.makeSpace(deep) + 'template ' + node.name_ + ' {\n';
    } else if (node.nodeType_ === this.DATA_NODES_INHERIT) {
      ret = this.makeSpace(deep) + node.name_ + ' :: ' + node.ref_ + ' {\n';
    } else {
      NapiLog.logError('unknow node type');
    }

    for (let i in node.value_) {
      ret += this.objToHcs(node.value_[i], deep + 1);
    }
    ret += this.makeSpace(deep) + '}\n';
    return ret;
  }

  objToHcs(node, deep) {
    let ret = '';
    switch (node.type_) {
      case DataType.INT8:
      case DataType.INT16:
      case DataType.INT32:
      case DataType.INT64:
        ret = NodeTools.jinZhi10ToX(node.value_, node.jinzhi_);
        break;
      case DataType.STRING:
        ret = '"' + node.value_ + '"';
        break;
      case DataType.NODE:
        ret += this.objToHcsConfigNode(node, deep);
        break;
      case DataType.ATTR:
        ret = this.makeSpace(deep) + node.name_ + ' = ';
        ret += this.objToHcs(node.value_, 0);
        ret += ';\n';
        break;
      case DataType.ARRAY:
        ret = '[';
        for (let i in node.value_) {
          let ss = this.objToHcs(node.value_[i], 0);
          if (i > 0) {
            ret += ', ';
          }
          ret += ss;
        }
        ret += ']';
        break;
      case DataType.REFERENCE:
        ret = '&' + node.value_;
        break;
      case DataType.DELETE:
        ret = 'delete';
        break;
      case DataType.BOOL:
        if (node.value_) {
          ret = 'true';
        }
        else {
          ret = 'false';
        }
        break;
      default:
        NapiLog.logError('unknow' + node.type_);
        break;
    }
    return ret;
  }
}

Generator.pInstance_ = null;
Generator.gi = function () {
  if (Generator.pInstance_ === null) {
    Generator.pInstance_ = new Generator();
  }
  return Generator.pInstance_;
};

module.exports = {
  Generator,
};
