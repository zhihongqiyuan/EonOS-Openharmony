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

const MAX_LABEL_LENGTH = 40;

class AttributeArea {
  constructor() {
    document.attrCallback = this;
    this.area = document.getElementById('attribute_area');
    this.htmlStr = '';
    this.callbackFunc = null;
    this.freshInputValue_ = [];
  }
  clear() {
    this.htmlStr = '';
  }
  flush() {
    this.area.innerHTML = this.htmlStr;
    while (this.freshInputValue_.length > 0) {
      let v = this.freshInputValue_.shift();
      let e = document.getElementById(v[0]);
      e.value = v[1];
    }
  }
  addTitle(name) {
    this.htmlStr +=
      '<div class="div_title"><span class="att_title" style="font-size:16px;">' +
      name +
      '</span></div>';
  }
  addImage(path) {
    this.htmlStr += '<img src="' + path + '">' + '</img>';
  }
  addDotLine() {}
  addTopInput(searchId, label, default_, disable) {
    let ret =
      '<label class="input_text_readonly_top">' + label + '</label></br>';
    ret += '<input id="' + searchId + '"';
    ret += ' class="input_text"';
    ret = this.addInputFunc(default_, ret, searchId, disable);
    ret += ' oninput="document.attrCallback.Event(' + "'input', " +
      "'" + searchId + "'" + ')" /><br>';
    this.htmlStr += ret;
  }

  addInputFunc(default_, ret, searchId, disable) {
    if (default_.indexOf('"') >= 0) {
      ret += ' value=""';
      this.freshInputValue_.push([searchId, default_]);
    } else { 
      ret += ' value="' + default_ + '"';
    }
    if (disable) {
      ret += ' disabled="disabled"';
    }
    return ret;
  }

  addInput(searchId, label, default_, disable) {
    let ret = '<label class="input_text_readonly">' + label + '</label></br>';
    ret += '<input id="' + searchId + '"';
    ret += ' class="input_text"';
    ret = this.addInputFunc(default_, ret, searchId, disable);
    ret += ' oninput="document.attrCallback.Event(' + "'input', " + "'" +
      searchId + "'" + ')" /><br>';
    this.htmlStr += ret;
  }

  addValidatorInput(
    searchId,
    label,
    default_,
    disable = false,
    defaultTxt = ''
  ) {
    let validatorId = 'valid_' + searchId;
    let ret = '<label class="input_text_readonly">' + label + '</label></br>';
    ret += '<input id="' + searchId + '"';
    ret += ' class="input_text" maxlength="40" placeholder="' + defaultTxt + '"';
    ret = this.addInputFunc(default_, ret, searchId, disable);
    ret +=
      ' autofocus="autofocus" onFocus="document.attrCallback.Event(' +
      "'input', " +
      "'" +
      searchId +
      "'" +
      ');"';
    ret +=
      ' oninput="document.attrCallback.Event(' +
      "'input', " +
      "'" +
      searchId +
      "'" +
      ')" /><br>';
    ret +=
      '<label id="' +
      validatorId +
      '" class="validator_label" display="none"></label></br>';
    this.htmlStr += ret;
  }

  addTextArea(searchId, label, default_) {
    let ret = '<label class="input_text_readonly">' + label + '</label><br>';
    ret += '<textarea id="' + searchId + '"';
    ret += ' class="text_area"';
    ret +=
      ' oninput="document.attrCallback.Event(' +
      "'input', " +
      "'" +
      searchId +
      "'" +
      ')">';
    ret += default_;
    ret += '</textarea><br>';
    this.htmlStr += ret;
  }
  addButton(searchId, label) {
    if (label.length > MAX_LABEL_LENGTH) {
      label = label.substring(0, MAX_LABEL_LENGTH) + '...';
    }
    let text =
      '" class="button_click" type="button" onclick="document.attrCallback.Event(';
    let htmlString = this.getStrText(searchId, text, label);
    this.htmlStr += '<button id="' + htmlString;
  }
  getStrText(searchId, text, label) {
    return searchId + text + "'button', " + "'" + searchId + "'" + ')">' + label + '</button><br>';
  }

  addLabelButton(searchId, label, title) {
    if (label.length > MAX_LABEL_LENGTH) {
      label = label.substring(0, MAX_LABEL_LENGTH) + '...';
    }
    let text =
      '" class="label_button_click" type="button" onclick="document.attrCallback.Event(';
    let htmlString = this.getStrText(searchId, text, label);
    this.htmlStr += '<label class="input_text_readonly">' + title + '</label></br><button id="' + htmlString;
  }

  addButtonDelete(searchId, label) {
    if (label.length > MAX_LABEL_LENGTH) {
      label = label.substring(0, MAX_LABEL_LENGTH) + '...';
    }
    let text = '" class="button_click_delete" type="button" onclick="document.attrCallback.Event(';
    this.htmlStr += '<button id="' + searchId + text + "'button', " + "'" +
      searchId + "'" + ')">' + label + '</button><br>';
  }
  addSelect(searchId, label, selectList, default_, disable) {
    let ret = '<label class="input_text_readonly">' + label + '</label></br>';
    ret += '<div class="ul-select"><span></span>';
    ret +=
      '<input type="text" id="' +
      searchId +
      '" value="' +
      default_ +
      '"  class="input_text" readonly="readonly" ';
    if (disable) {
      ret += ' disabled="true" />';
    } else {
      ret += 'onfocus="selectFocus(this);"';
      ret += 'onblur="selectBlur(this);" />';
      ret += '<ul>';
      for (let i in selectList) {
        ret += '<li onclick="liClick(this, \'' + searchId + '\');">';
        ret += '<a href="javascript:;">' + selectList[i] + '</a>';
        ret += '</li>';
      }
      ret += '</ul>';
    }

    ret += '</div>';
    this.htmlStr += ret;
  }
  addGap(type) {
    if (type === 0) {
      this.htmlStr += '<br>';
    }
  }
  Event(type, value) {
    let cbv = '';
    if (type === 'input') {
      cbv = document.getElementById(value).value;
    } else if (type === 'select') {
      cbv = document.getElementById(value).value;
    }
    if (this.callbackFunc !== null) {
      this.callbackFunc(value, type, cbv);
    }
  }
  registRecvCallback(func) {
    this.callbackFunc = func;
  }
}
AttributeArea.pInstance_ = null;
AttributeArea.gi = function () {
  if (AttributeArea.pInstance_ === null) {
    AttributeArea.pInstance_ = new AttributeArea();
  }
  return AttributeArea.pInstance_;
};

module.exports = {
  AttributeArea,
};
