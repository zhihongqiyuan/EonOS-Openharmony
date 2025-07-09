/*
* Copyright (c) 2023 Shenzhen Kaihong Digital Industry Development Co., Ltd.
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
#include "napitest.h"
#include "napitest_middle.h"
#include "hilog/log.h"
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, 0XD002E00, "NAPITESTNAPILayer"};
#define NAPITEST_LOGI(fmt, ...) OHOS::HiviewDFX::HiLog::Info(LABEL, \
    	"%{public}s:%{public}d " fmt, __func__, __LINE__, ##__VA_ARGS__)

namespace napitest {
namespace napitest_interface {
NodeISayHelloListener NodeISayHello::listener_ = {};
bool NodeISayHello::addSayHelloListener(NodeISayHelloListener& listener)
{
    NodeISayHello::listener_ = listener;
    return true;
}

bool NodeISayHello::removeSayHelloListener(NodeISayHelloListener& listener)
{
    return true;
}

bool NodeISayHello::registerCallbackfunc()
{
    return true;
}

// 供业务调用的回调接口
void NodeISayHello::CallbackfuncCallback(NUMBER_TYPE_2& wid)
{
    std::string eventName = "Callbackfunc";
    NodeISayHello_middle *ptr = new NodeISayHello_middle();
    ptr->CallbackfuncCallbackMiddle(eventName,  wid);
    delete ptr;
}

bool NodeISayHello::unRegisterCallbackfunc()
{
    return true;
}

bool NodeISayHello::sayHello(std::string& from, std::string& to, NUMBER_TYPE_9& sayType)
{
    NAPITEST_LOGI("NAPITEST_LOGI sayHello from = %s\r\n", from.c_str());
    NAPITEST_LOGI("NAPITEST_LOGI sayHello to = %s\r\n", to.c_str());
    NAPITEST_LOGI("NAPITEST_LOGI sayHello sayType = %d\r\n", sayType);
    SayInfo info1;
    info1.from = "js1";
    uint32_t a = 992;
    info1.fromId.emplace(a);
    uint32_t b = 1014;
    info1.toId.emplace(b);
    info1.to = "native1";
    info1.content = "hello1";
    info1.saidTime = "123456789";
    info1.isEnd = false;
    SayInfo info2;
    info2.from = "native";
    uint32_t c = 101;
    info2.fromId.emplace(c);
    uint32_t d = 99;
    info2.toId.emplace(d);
    info2.to = "js";
    info2.content = "hello";
    info2.saidTime = "987654321";
    info2.isEnd = true;
    // 业务代码调用 onSayHelloStart callback
    listener_.NodeISayHelloListener_onSayHelloStartCallback(info1);
    // 业务代码调用 onSayHelloEnd callback
    listener_.NodeISayHelloListener_onSayHelloEndCallback(info2);
    return true;
}

bool NodeISayHello::sayHi(std::string& from, std::string& to, NUMBER_TYPE_10& sayType)
{
    NAPITEST_LOGI("NAPITEST_LOGI sayHi from = %s\r\n", from.c_str());
    NAPITEST_LOGI("NAPITEST_LOGI sayHi to = %s\r\n", to.c_str());
    NAPITEST_LOGI("NAPITEST_LOGI sayHi sayType = %d\r\n", sayType);
    NodeISayHello *ptr = new NodeISayHello();
    uint32_t callbackNum = 50;
    ptr->CallbackfuncCallback(callbackNum);
    delete ptr;
    return true;
}

bool NodeISayHello::sayHelloWithResponse(std::string& from, std::string& to, NUMBER_TYPE_11& sayType,
    uint32_t& outErrCode, AUTO_INTERFACE_5& out)
{
    NAPITEST_LOGI("NAPITEST_LOGI sayHelloWithResponse from = %s\r\n", from.c_str());
    NAPITEST_LOGI("NAPITEST_LOGI sayHelloWithResponse to = %s\r\n", to.c_str());
    NAPITEST_LOGI("NAPITEST_LOGI sayHelloWithResponse sayType = %d\r\n", sayType);
    out.errMsg = "";
    out.response = "rec hello.";
    out.result = 0;
    return true;
}

AUTO_INTERFACE_5 NodeISayHello::auto_interface_5OutRes = {};
void NodeISayHello::auto_interface_5SetCbValue(NUMBER_TYPE_6 result, std::string errMsg, std::string response)
{
    NodeISayHello::auto_interface_5OutRes.result = result;
    NodeISayHello::auto_interface_5OutRes.errMsg = errMsg;
    NodeISayHello::auto_interface_5OutRes.response = response;
    return;
}

bool NodeISayHelloListener::onSayHelloStart()
{
    return true;
}

// 供业务调用的回调接口
void NodeISayHelloListener::NodeISayHelloListener_onSayHelloStartCallback(SayInfo& info)
{
    std::string eventName = "NodeISayHelloListener_onSayHelloStart";
    NodeISayHelloListener_middle *ptr = new NodeISayHelloListener_middle();
    ptr->NodeISayHelloListener_onSayHelloStartCallbackMiddle(eventName,  info);
    delete ptr;
}

bool NodeISayHelloListener::onSayHelloEnd()
{
    return true;
}

// 供业务调用的回调接口
void NodeISayHelloListener::NodeISayHelloListener_onSayHelloEndCallback(SayInfo& info)
{
    std::string eventName = "NodeISayHelloListener_onSayHelloEnd";
    NodeISayHelloListener_middle *ptr = new NodeISayHelloListener_middle();
    ptr->NodeISayHelloListener_onSayHelloEndCallbackMiddle(eventName,  info);
    delete ptr;
}

bool funcTest(bool& v, std::string& out)
{
    if (v) {
        out = "ret is true";
    } else {
        out = "ret is false";
    }
    return true;
}
}
}
