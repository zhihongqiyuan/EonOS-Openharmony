/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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
import router from "@system.router";
import app from "@system.router";
import fetch from "@system.fetch";

import http from '@ohos.net.http';

import hello from 'libhello.so';

class A{
	pushPage() {
		router.push({
			uri: 'pages/routerpage2/routerpage2',
			params: {
				data1: 'message',
				data2: {
					data3: [123, 456, 789]
				}
			}
		});
	}
}

class Info {
  getInfo() {
    let info = app.getInfo()
    console.log(JSON.stringify(info))
  }
}

const json = {
  data: {
    responseData: 'NA',
    url: "test_url",
  },
  fetch: function () {
    var that = this;
    fetch.fetch({
      url: that.url,
      success: function(response) {
        console.info("fetch success");
        that.responseData = JSON.stringify(response);
      },
      fail: function() {
        console.info("fetch fail");
      }
    });
  }
}

let httpRequest = http.createHttp();