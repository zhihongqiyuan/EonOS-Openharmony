/*
 * Copyright (c) 2024 Shenzhen Kaihong Digital Industry Development Co., Ltd.
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
 
#ifndef OHOS_HDI_HELLO_V1_0_HELLOINTERFACESERVICE_H
#define OHOS_HDI_HELLO_V1_0_HELLOINTERFACESERVICE_H

#include "v1_0/ihello_interface.h"

namespace OHOS {
namespace HDI {
namespace Hello {
namespace V1_0 {
class HelloInterfaceService : public OHOS::HDI::Hello::V1_0::IHelloInterface {
public:
    HelloInterfaceService() = default;
    virtual ~HelloInterfaceService() = default;

    int32_t Helloworld(const std::string& sendMsg, std::string& recvMsg) override;

};
} // V1_0
} // Hello
} // HDI
} // OHOS

#endif // OHOS_HDI_HELLO_V1_0_HELLOINTERFACESERVICE_H