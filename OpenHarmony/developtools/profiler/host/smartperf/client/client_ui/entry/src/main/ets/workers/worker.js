/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

import worker from '@ohos.worker'; // 导入worker模块
import netSocket from '@ohos.net.socket';

const workTag = 'SmartPerf::Work:: ';
let parentPort = worker.parentPort; // 获取parentPort属性

export let IPv4 = 1;

export let IPv4BindAddr = {
  address: '127.0.0.1',
  family: IPv4,
  port: 8282,
};

export let UdpSendAddress = {
  address: '127.0.0.1',
  family: IPv4,
  port: 8283,
};

export let flagPackageNum = 0;

export let udp = netSocket.constructUDPSocketInstance();
udp.bind(IPv4BindAddr, (err) => {
  if (err) {
    console.log(workTag + 'Worker socket bind fail');
    return;
  }
  console.log(workTag + 'Worker socket bind success');
  udp.getState((err, data) => {
    if (err) {
      console.log(workTag + 'Worker socket getState fail');
      return;
    }
    console.log(workTag + 'Worker socket getState success:' + JSON.stringify(data));
  });
});

parentPort.onmessage = function (e) {
  let socketCollectItems = e.data;
  console.log(workTag + 'sub worker recv:' + JSON.stringify(e.data));

  let messageSetPkg = 'set_pkgName::' + socketCollectItems.pkg;
  udp.getState((err, data) => {
    if (err) {
      parentPort.postMessage('UdpStatus$-1');
      console.log(workTag + 'Worker socket getState error', err);
    }
    console.log(workTag + 'Worker socket getState success:' + JSON.stringify(data));

    if (socketCollectItems.testConnection) {
      for (let i = 0; i < 10; i++) {
        udp.send({
          address: UdpSendAddress,
          data: 'set_pkgName::com.ohos.smartperf',
        });
        console.log(workTag + 'Worker socket test connection send');
      }
    }

    if (socketCollectItems.setDuBaiDb) {
      udp.send({
        address: UdpSendAddress,
        data: 'set_dubai_db',
      });
      console.log(workTag + 'Worker socket ‘set_dubai_db’ send');
    }

    if (flagPackageNum < 2) {
      if (socketCollectItems.pkg !== undefined) {
        udp.send({
          address: UdpSendAddress,
          data: messageSetPkg,
        });
        flagPackageNum++;
      }
    }
    if (socketCollectItems.fps) {
      let messageFps = 'get_fps_and_jitters';
      udp.send({
        address: UdpSendAddress,
        data: messageFps,
      });
      console.log(workTag + 'sub worker messageFps :' + messageFps);
    }
    if (socketCollectItems.ram) {
      let messageRam = 'get_ram_info::' + socketCollectItems.pkg;
      udp.send({
        address: UdpSendAddress,
        data: messageRam,
      });
      console.log(workTag + 'sub worker messageRam :' + messageRam);
    }
    if (socketCollectItems.screenCapture) {
      udp.send({
        address: UdpSendAddress,
        data: 'get_capture',
      });
      console.log(workTag + 'sub worker screen_capture :' + 'get_capture');
    }
    if (socketCollectItems.catch_trace_start) {
      let messageTrace = 'catch_trace_start';
      udp.send({
        address: UdpSendAddress,
        data: messageTrace,
      });
      console.log(workTag + 'sub worker catch_trace_start :' + 'catch_trace_start');
    }
    if (socketCollectItems.catch_trace_end) {
      let messageTrace = 'catch_trace_end';
      udp.send({
        address: UdpSendAddress,
        data: messageTrace,
      });
      console.log(workTag + 'sub worker catch_trace_end :' + 'catch_trace_end');
    }
  });
};

udp.on('message', function (data) {
  let buffer = data.message;
  let dataView = new DataView(buffer);
  let str = '';
  for (let i = 0; i < dataView.byteLength; ++i) {
    str += String.fromCharCode(dataView.getUint8(i));
  }
  console.log(workTag + 'sub worker SocketRecv:' + str);
  if (str.length > 0) {
    parentPort.postMessage('UdpStatus$1');
  }
  try {
    if (includes(str, 'pss')) {
      parentPort.postMessage('RAM$' + str);
    } else if (includes(str, 'fps')) {
      if (str.indexOf('$$') !== -1) {
        let arrStr = str.split('$$');
        if (arrStr.length > 0) {
          if (arrStr[0].indexOf('||') !== -1 && arrStr[1].indexOf('||') !== -1) {
            let fps = arrStr[0].split('||');
            let fpsJitter = arrStr[1].split('||');
            parentPort.postMessage('FPS$' + fps[1].toString() + '$' + fpsJitter[1].toString());
          }
        }
      }
    }
  } catch (e) {
    console.log(workTag + 'SockOnMessage recv callback err:' + e);
  }
});

function includes(all, sub) {
  all = all.toLowerCase();
  sub = sub.toLowerCase();

  let firstChar = sub.substring(0, 1);
  let subLength = sub.length;

  for (let i = 0; i < all.length - subLength + 1; i++) {
    if (all.charAt(i) === firstChar) {
      if (all.substring(i, i + subLength) === sub) {
        return true;
      }
    }
  }
  return false;
}