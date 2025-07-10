/*
 * Copyright (c) 2022-2023 Shenzhen Kaihong Digital Industry Development Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

const { NapiLog } = require('./NapiLog');
const re = require('./re');

const MAX_DECIMAL = 10;
const MAX_BINARY = 2;
const MAX_OCTAL = 8;
const MAX_HEXADECIMAL = 16;
const STAT_SIZE = 100;

const DataType = {
  INT8: 1,
  INT16: 2,
  INT32: 3,
  INT64: 4,
  STRING: 5,
  NODE: 6,
  ATTR: 7,
  ARRAY: 8,
  REFERENCE: 9,
  DELETE: 10,
  BOOL: 11,
};
const NodeType = {
  DATA: 0,
  COPY: 1,
  REFERENCE: 2,
  DELETE: 3,
  TEMPLETE: 4,
  INHERIT: 5,
};

class NodeTools {}

function getRoot(node) {
  while (node.parent_ !== undefined) {
    node = node.parent_;
  }
  return node;
}

NodeTools.isElders = function (node, elders) {
  while (node !== undefined) {
    if (node === elders) {
      return true;
    }
    node = node.parent_;
  }
  return false;
};

NodeTools.getPathByNode = function (node, expandRef = true) {
  if (node === null) {
    return '';
  }
  let ret = node.name_;
  while (node !== null && node.parent_ !== undefined) {
    node = node.parent_;
    if (expandRef && node.nodeType_ === NodeType.REFERENCE) {
      if (node.ref_.startsWith(getRoot(node).name_)) {
        node = NodeTools.getNodeByPath(getRoot(node), node.ref_);
      } else {
        node = NodeTools.getNodeByPath(node.parent_, node.ref_);
      }
    }
    if (node !== null) {
      ret = node.name_ + '.' + ret;
    }
  }
  return ret;
};
NodeTools.getNodeByPath = function (node, path) {
  let ps = path.split('.');
  if (ps[0] === 'root') {
    ps.shift();
  }
  for (let p in ps) {
    node = NodeTools.findChildByName(node, ps[p]);
    if (node === null) {
      return null;
    }
  }
  return node;
};

NodeTools.lookupInherit = function (node) {
  if (
    node.type_ === DataType.NODE &&
    node.nodeType_ === NodeType.INHERIT &&
    node.parent_.nodeType_ === NodeType.INHERIT
  ) {
    let p = NodeTools.lookupInherit(node.parent_);
    if (p === null) {
      return p;
    }
    return NodeTools.findChildByName(p, node.ref_);
  }
  return NodeTools.findChildByName(node.parent_, node.ref_);
};

NodeTools.lookup = function (node) {
  let refname;
  if (
    node.type_ === DataType.NODE &&
    (node.nodeType_ === NodeType.COPY ||
      node.nodeType_ === NodeType.REFERENCE ||
      node.nodeType_ === NodeType.INHERIT)
  ) {
    refname = node.ref_;
  }
  else if (
    node.type_ === DataType.ATTR &&
    node.value_.type_ === DataType.REFERENCE
  ) {
    refname = node.value_.value_;
  }
  else { return null; }
  if (refname.indexOf('.') >= 0) {
    return NodeTools.getNodeByPath(getRoot(node), refname);
  }

  let ret = NodeTools.findChildByName(node.parent_, refname);
  if (ret === null) {
    if (
      node.type_ === DataType.NODE &&
      node.nodeType_ === NodeType.INHERIT &&
      node.parent_.nodeType_ === NodeType.INHERIT
    ) {
      ret = NodeTools.lookupInherit(node);
    }
  }
  return ret;
};

NodeTools.recursionNode = function (node, callback) {
  if (node.type_ === DataType.NODE) {
    callback(node);
    for (let n in node.value_) {
      NodeTools.recursionNode(node.value_[n], callback);
    }
  }
};
NodeTools.recursionAll = function (node, callback, isForward) {
  let ret = false;
  if (isForward) {
    ret = callback(node);
  }
  if (node.type_ === DataType.NODE) {
    for (let i = 0; i < node.value_.length; i++) {
      if (NodeTools.recursionAll(node.value_[i], callback, isForward)) {
        i--;
      }
    }
  }
  if (!isForward) {
    ret = callback(node);
  }
  return ret;
};
NodeTools.redefineCheck = function (node) {
  let ret = true;
  NodeTools.recursionNode(node, (pn) => {
    let nameMap = [];
    for (let i in pn.value_) {
      if (nameMap.indexOf(pn.value_[i].name_) >= 0) {
        pn.value_[i].errMsg_ = '重名:' + pn.value_[i].lineno_;
        ret = false;
      } else {
        nameMap.push(pn.value_[i].name_);
        pn.value_[i].errMsg_ = null;
      }
    }
  });
  return ret;
};
function separate(node) {
  let pn = node.parent_;
  if (pn === null) {
    return;
  }
  for (let i in pn.value_) {
    if (pn.value_[i] === node) {
      pn.value_.splice(i, 1);
    }
  }
}
NodeTools.findChildByName = function (node, name) {
  for (let i in node.value_) {
    if (node.value_[i].name_ === name) {
      return node.value_[i];
    }
  }
  return null;
};

NodeTools.copyNode = function (node, parent) {
  let ret = {
    type_: node.type_,
    name_: node.name_,
    lineno_: node.lineno_,
    parent_: parent,
  };
  if (node.raw_ !== undefined) {
    ret.raw_ = node.raw_;
  }
  switch (node.type_) {
    case DataType.INT8:
    case DataType.INT16:
    case DataType.INT32:
    case DataType.INT64:
      ret.value_ = node.value_;
      ret.jinzhi_ = node.jinzhi_;
      break;
    case DataType.STRING:
      ret.value_ = node.value_;
      break;
    case DataType.NODE:
      ret.nodeType_ = node.nodeType_;
      if ([NodeType.INHERIT, NodeType.COPY, NodeType.REFERENCE].indexOf(node.nodeType_) > -1) {
        ret.ref_ = node.ref_;
      } else if (!([NodeType.DATA, NodeType.TEMPLETE, NodeType.DELETE].indexOf(node.nodeType_) > -1)) {
        NapiLog.logError('unknow node type:' + ret.lineno_);
      }
      ret.value_ = [];
      for (let i in node.value_) { ret.value_.push(NodeTools.copyNode(node.value_[i], ret)); }
      break;
    case DataType.ATTR:
      ret.value_ = NodeTools.copyNode(node.value_, ret);
      break;
    case DataType.ARRAY:
      ret.value_ = [];
      for (let i in node.value_) { ret.value_.push(NodeTools.copyNode(node.value_[i], ret)); }
      break;
    case DataType.REFERENCE:
      ret.value_ = node.value_;
      break;
    case DataType.DELETE:
      ret.value_ = null;
      break;
    case DataType.BOOL:
      ret.value_ = node.value_;
      break;
    default:
      NapiLog.logInfo('unknow', node.type_);
      break;
  }
  return ret;
};

function makeError(node, errStr) {
  if (node.raw_ !== undefined) {
    node.raw_.errMsg_ = errStr;
  }
}
NodeTools.nodeNestCheck = function (node) {
  NodeTools.recursionAll(
    node,
    (pn) => {
      if (pn.type_ === DataType.NODE) {
        if (pn.nodeType_ === NodeType.COPY && pn.raw_.errMsg_ === null) {
          pn.raw_.errMsg_ = '有Copy的嵌套' + pn.raw_.lineno_;
        }
      }
      return false;
    },
    false
  );
};
NodeTools.recursionCopyAndReferenceNodes = function (pn) {
  let ref = NodeTools.lookup(pn);
  if (ref === null) {
    NapiLog.logError(
      'reference not exist' + NodeTools.getPathByNode(pn) + ':' + pn.ref_
    );
    if (pn.nodeType_ === NodeType.COPY) {
      makeError(pn, pn.name_ + ' 复制目标没找到!');
    } else {
      makeError(pn, pn.name_ + ' 引用目标没找到!');
    }
    return false;
  } else if (ref.nodeType_ === NodeType.TEMPLETE) {
    if (pn.nodeType_ === NodeType.COPY) {
      makeError(pn, pn.name_ + ' 复制目标不能为模板节点!');
    } else {
      makeError(pn, pn.name_ + ' 引用目标不能为模板节点!');
    }
    return false;
  } else if (ref.nodeType_ === NodeType.DELETE) {
    if (pn.nodeType_ === NodeType.COPY) {
      makeError(pn, pn.name_ + ' 复制目标不能为删除节点!');
    } else {
      makeError(pn, pn.name_ + ' 引用目标不能为删除节点!');
    }
    return false;
  } else if (NodeTools.isElders(pn, ref)) {
    NapiLog.logError(
      'circular reference' + NodeTools.getPathByNode(pn) + ':' + pn.ref_
    );
    if (pn.nodeType_ === NodeType.COPY) {
      makeError(pn, pn.name_ + ' 循环复制!');
    } else {
      makeError(pn, pn.name_ + ' 循环引用!');
    }
    return false;
  } else if (pn.nodeType_ === NodeType.COPY) {
    if (ref.nodeType_ === NodeType.COPY) {
      pn.raw_.errMsg_ = '有Copy的嵌套:' + pn.raw_.lineno_;
    }
    pn.nodeType_ = NodeType.DATA;
    let tref = NodeTools.copyNode(ref, pn.parent_);
    tref.name_ = pn.name_;
    NodeTools.merge(tref, pn);
    pn.value_ = tref.value_;
    return false;
  } else if (pn.nodeType_ === NodeType.REFERENCE) {
    pn.nodeType_ = ref.nodeType_;
    pn.name_ = ref.name_;
    pn.ref_ = ref.ref_;
    NodeTools.merge(ref, pn);
    separate(pn);
    return true;
  }
  return false;
};

NodeTools.checkInheritNode = function (pn) {
  let ref = NodeTools.lookup(pn);
  if (ref === null) {
    makeError(pn, pn.name_ + ' 继承目标找不到!');
  } else if (ref.type_ !== DataType.NODE) {
    makeError(pn, pn.name_ + ' 不能继承属性!');
  } else if (ref.nodeType_ === NodeType.REFERENCE) {
    makeError(pn, pn.name_ + ' 不能继承引用类节点!');
  } else if (ref.nodeType_ === NodeType.DELETE) {
    makeError(pn, pn.name_ + ' 不能继承删除类节点!');
  } else if (ref.nodeType_ === NodeType.DATA) {
    makeError(pn, pn.name_ + ' 不能继承数据类节点!');
  } else if (ref.nodeType_ === NodeType.INHERIT) {
    makeError(pn, pn.name_ + ' 不能继承继承类节点!');
  } else if (ref.nodeType_ === NodeType.COPY) {
    makeError(pn, pn.name_ + ' 不能继承复制类节点!');
  }
};

NodeTools.nodeExpand = function (node) {
  NodeTools.recursionAll(node, (pn) => {
    if (pn.type_ === DataType.NODE) {
      if (pn.nodeType_ === NodeType.DELETE) {
        separate(pn);
        return true;
      }
      if (pn.nodeType_ === NodeType.COPY || pn.nodeType_ === NodeType.REFERENCE) {
        return NodeTools.recursionCopyAndReferenceNodes(pn);
      }
      if (pn.nodeType_ === NodeType.INHERIT) {
        NodeTools.checkInheritNode(pn);
      }
    } else if (pn.type_ === DataType.ATTR) {
      if (pn.value_.type_ === DataType.DELETE) {
        separate(pn);
        return true;
      }
      if (pn.value_.type_ === DataType.REFERENCE) {
        let ref = NodeTools.lookup(pn);
        if (ref === null || ref.type_ !== DataType.NODE || ref.nodeType_ === NodeType.REFERENCE ||
          ref.nodeType_ === NodeType.TEMPLETE || ref.nodeType_ === NodeType.DELETE
        ) {
          NapiLog.logError('reference invalid node' + NodeTools.getPathByNode(pn) + ':' + pn.value_.value_);
          if (ref === null) {
            makeError(pn, pn.name_ + ' 找不到引用目标!');
          } else if (ref.type_ !== DataType.NODE) {
            makeError(pn, pn.name_ + ' 不能引用属性!');
          } else if (ref.nodeType_ === NodeType.REFERENCE) {
            makeError(pn, pn.name_ + ' 不能引用引用类节点!');
          } else if (ref.nodeType_ === NodeType.TEMPLETE) {
            makeError(pn, pn.name_ + ' 不能引用模板类节点!');
          } else if (ref.nodeType_ === NodeType.DELETE) {
            makeError(pn, pn.name_ + ' 不能引用删除类节点!');
          }
        } else {
          pn.refNode_ = ref;
        }
      }
      }
    return false;
  }, false);
};

NodeTools.inheritExpand = function (node) {
  NodeTools.recursionAll(
    node,
    (pn) => {
      let tt = re.match('^[a-zA-Z_]{1}[a-zA-Z_0-9]*$', pn.name_);
      if (tt === null) {
        makeError(pn, pn.name_ + ' 名字不合规范!');
      }
      if (pn.type_ !== DataType.NODE) {
        return false;
      }
      if (pn.nodeType_ !== NodeType.INHERIT) {
        return false;
      }
      let inherit = NodeTools.lookup(pn);
      if (inherit === null) {
        NapiLog.logError(
          'inherit invalid node: ' + NodeTools.getPathByNode(pn) + ':' + pn.ref_
        );
        makeError(pn, '继承目标找不到!');
        return false;
      }
      pn.nodeType_ = NodeType.DATA;
      let tinherit = NodeTools.copyNode(inherit, pn.parent_);
      NodeTools.merge(tinherit, pn);
      pn.value_ = tinherit.value_;
      return false;
    },
    true
  );
};
NodeTools.merge = function (node1, node2) {
  if (node2 === null) {
    return true;
  }
  if (node2.raw_ === undefined) {
    node1.raw_ = node2;
  } else {
    node1.raw_ = node2.raw_;
  }
  if (node1.type_ === DataType.NODE) {
    if (node1.name_ !== node2.name_) {
      return false;
    }
    node1.nodeType_ = node2.nodeType_;
    if (node2.nodeType_ === NodeType.INHERIT || node2.nodeType_ === NodeType.REFERENCE || node2.nodeType_ === NodeType.COPY) {
      node1.ref_ = node2.ref_;
    }
    if (node1.value_ === undefined) {
      node1.value_ = [];
    }

    for (let i in node2.value_) {
      let child2 = node2.value_[i];
      let child1 = NodeTools.findChildByName(node1, child2.name_);
      if (child1 === null) {
        child1 = {
          type_: child2.type_,
          name_: child2.name_,
          lineno_: child2.lineno_,
          parent_: node1,
        };
        node1.value_.push(child1);
      } else if (child1.type_ !== child2.type_) {
        child2.raw_.errMsg_ =
          '所修改的子节的类型和父节点类型不同:' + child2.raw_.lineno_;
        return false;
      }
      NodeTools.merge(child1, child2);
    }
  } else if (node1.type_ === DataType.ATTR) {
    node1.value_ = NodeTools.copyNode(node2.value_, node1);
  }
  return true;
};
NodeTools.jinZhi10ToX = function (num, jinzhi) {
  let ret;
  switch (jinzhi) {
    case MAX_BINARY:
      ret = '0b';
      break;
    case MAX_OCTAL:
      ret = '0';
      break;
    case MAX_DECIMAL:
      ret = '';
      break;
    case MAX_HEXADECIMAL:
      ret = '0x';
      break;
    default:
      NapiLog.logError(jinzhi + '进制转换失败');
      break;
  }
  return ret + num.toString(jinzhi);
};
NodeTools.jinZhiXTo10 = function (s) {
  if (s === null || s.length === 0) {
    return [0, MAX_DECIMAL];
  }

  s = s.trim();
  if (!isFinite(s)) {
    return [undefined, MAX_DECIMAL];
  }

  try {
    if (s[0] === '0') {
      if (s.length === 1) {
        return [BigInt(s), MAX_DECIMAL];
      } else if (s[1] === 'b' || s[1] === 'B') {
        return [BigInt(s), MAX_BINARY];
      } else if (s[1] === 'x' || s[1] === 'X') {
        return [BigInt(s), MAX_HEXADECIMAL];
      } else {
        return [BigInt('0o' + s.substring(1)), MAX_OCTAL];
      }
    } else {
      return [BigInt(s), MAX_DECIMAL];
    }
  } catch (ex) {
    NapiLog.logError(ex.message);
    return [undefined, MAX_DECIMAL];
  }
};

NodeTools.createNewNode = function (type, name, value, nodetype) {
  let ret = new Object();
  ret.type_ = type;
  ret.name_ = name;
  ret.value_ = value;
  ret.isOpen_ = true;
  if (type < DataType.STRING) {
    ret.jinzhi_ = MAX_DECIMAL;
  }
  if (type === DataType.NODE) {
    ret.nodeType_ = nodetype;
  }
  return ret;
};
NodeTools.arrayToString = function (node, maxw) {
  let ret = '';
  let type = DataType.INT8;
  for (let d in node.value_) {
    if (type < node.value_[d].type_) {
      type = node.value_[d].type_;
    }
  }
  let line = '';
  for (let d in node.value_) {
    if (d > 0) {
      line += ',';
      if (maxw !== undefined) {
        if (line.length >= maxw) {
          ret += line + '\n';
          line = '';
        }
      }
    }
    if (type === DataType.STRING) {
      line += '"' + node.value_[d].value_ + '"';
    } else {
      line += NodeTools.jinZhi10ToX(node.value_[d].value_, node.value_[d].jinzhi_);
    }
  }
  ret += line;
  return ret;
};

NodeTools.stringToArray = function (s) {
  let type = DataType.INT8;
  let ret = [];
  s = s.replace(/\n/g, '');
  if (s.length <= 0) {
    return ret;
  }
  if (s.indexOf('"') >= 0) {
    type = DataType.STRING;
    let p = 0;
    let stat = 0;
    let v;
    while (p < s.length && stat < STAT_SIZE) {
      switch (stat) {
        case 0:
          if (s[p] === '"') {
            stat = 1;
            v = '';
          } else if (s[p] !== ' ') {
            stat = STAT_SIZE;
          }
          break;
        case 1:
          if (s[p] === '"') {
            stat = 2;
            ret.push(NodeTools.createNewNode(type, '', v));
          } else {
            v += s[p];
          }
          break;
        case 2:
          if (s[p] === ',') {
            stat = 0;
          }
          else if (s[p] !== ' ') {
            stat = STAT_SIZE;
          }
          break;
      }
      p += 1;
    }
  } else {
    let arr = s.split(',');
    stringToArrayWithQuote(ret, type, arr);
  }
  return ret;
};

function stringToArrayWithQuote(ret, type, arr) {
  for (let i in arr) {
    let num = NodeTools.jinZhiXTo10(arr[i]);
    if (num[0] === undefined) {
      num[0] = 0;
    }
    let attr = NodeTools.createNewNode(type, '', num[0]);
    attr.jinzhi_ = num[1];
    ret.push(attr);
    if (num[0] <= 0xff) {
      if (type < DataType.INT8) {
        type = DataType.INT8;
      }
    } else if (num[0] <= 0xffff) {
      if (type < DataType.INT16) {
        type = DataType.INT16;
      }
    } else if (num[0] <= 0xffffffff) {
      if (type < DataType.INT32) {
        type = DataType.INT32;
      }
    } else {
      type = DataType.INT64;
    }
  }
  if (type !== DataType.INT8) {
    for (let i in ret) {
      ret[i].type_ = type;
    }
  }
}

module.exports = {
  NodeTools,
  DataType,
  NodeType,
};
