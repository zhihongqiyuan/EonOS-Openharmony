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
 
#include "v1_0/hello_interface_service.h"
#include <hdf_base.h>
#include "hello_log.h"
#include "devhost_dump_reg.h"
#include "hello_dump.h"

#define HDF_LOG_TAG    hello_interface_service

namespace OHOS {
namespace HDI {
namespace Hello {
namespace V1_0 {
extern "C" IHelloInterface *HelloInterfaceImplGetInstance(void)
{
    // עᨩdumper
    DevHostRegisterDumpHost(GetHelloDump);
    // [hdf-gen] Todo
    HDF_LOGI("%{public}s: IHelloInterface init", __func__);
    return new (std::nothrow) HelloInterfaceService();
}

int32_t HelloInterfaceService::Helloworld(const std::string& sendMsg, std::string& recvMsg)
{
    // [hdf-gen] Todo
    HDF_LOGI("%{public}s: HelloService::Helloworld print", __func__);
    return HDF_SUCCESS;
}

} // V1_0
} // Hello
} // HDI
} // OHOS
