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
 
#include <hdf_base.h>
#include <hdf_device_desc.h>
#include <hdf_log.h>
#include <hdf_sbuf_ipc.h>
#include "v1_0/hello_interface_stub.h"

#define HDF_LOG_TAG    hello_interface_driver

using namespace OHOS::HDI::Hello::V1_0;

struct HdfHelloInterfaceHost {
    struct IDeviceIoService ioService;
    OHOS::sptr<OHOS::IRemoteObject> stub;
};

/*
 * 处理客户端请求的Dispatch方法: 处理来自客户端的IO请求
 * client：指向HdfDeviceIoClient结构体的指针，表示发起请求的客户端。
 * cmdId：命令ID，标识了要执行的命令或操作。
 * data：指向HdfSBuf结构体的指针，包含了请求的数据。
 * reply：指向另一个HdfSBuf结构体的指针，用于存放响应的数据。
 */
static int32_t HelloInterfaceDriverDispatch(struct HdfDeviceIoClient *client, int cmdId, struct HdfSBuf *data,
    struct HdfSBuf *reply)
{
    auto *hdfHelloInterfaceHost = CONTAINER_OF(client->device->service, struct HdfHelloInterfaceHost, ioService);

    // 声明两个MessageParcel对象，用于序列化和反序列化IPC通信中的数据
    OHOS::MessageParcel *dataParcel = nullptr;
    OHOS::MessageParcel *replyParcel = nullptr;
    // 创建一个MessageOption对象，用于设置IPC通信的选项。
    OHOS::MessageOption option;

    // 响应序列化：将HdfSBuf中的数据转换为MessageParcel对象。如果转换失败，记录错误并返回错误代码。
    if (SbufToParcel(data, &dataParcel) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: invalid data sbuf object to dispatch", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    // 数据序列化：尝试将响应数据的HdfSBuf转换为MessageParcel对象。如果失败，也记录错误并返回错误代码。
    if (SbufToParcel(reply, &replyParcel) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: invalid reply sbuf object to dispatch", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    // 调用stub对象的SendRequest方法，发送请求。这个方法执行实际的IPC调用，将cmdId和序列化后的请求数据dataParcel发送给服务端，并将响应数据反序列化到replyParcel中。
    return hdfHelloInterfaceHost->stub->SendRequest(cmdId, *dataParcel, *replyParcel, option);
}

// 驱动自身业务初始化的接口
static int HdfHelloInterfaceDriverInit(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGI("%{public}s: driver init start", __func__);
    return HDF_SUCCESS;
}

// 将驱动对外提供的服务能力接口绑定到HDF框架
static int HdfHelloInterfaceDriverBind(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGI("%{public}s: driver bind start", __func__);
    // 创建对象：该对象是驱动服务的具体实现
    auto *hdfHelloInterfaceHost = new (std::nothrow) HdfHelloInterfaceHost;
    if (hdfHelloInterfaceHost == nullptr) {
        HDF_LOGE("%{public}s: failed to create create HdfHelloInterfaceHost object", __func__);
        return HDF_FAILURE;
    }

    // 为ioService结构体设置回调函数：设置的Dispatch函数用于处理IO请求
    hdfHelloInterfaceHost->ioService.Dispatch = HelloInterfaceDriverDispatch;
    hdfHelloInterfaceHost->ioService.Open = NULL;
    hdfHelloInterfaceHost->ioService.Release = NULL;

    auto serviceImpl = OHOS::HDI::Hello::V1_0::IHelloInterface::Get(true);
    if (serviceImpl == nullptr) {
        HDF_LOGE("%{public}s: failed to get of implement service", __func__);
        delete hdfHelloInterfaceHost;
        return HDF_FAILURE;
    }

    // 使用ObjectCollector的GetOrNewObject方法获取或创建一个Stub对象。Stub对象是服务接口的客户端代理，用于发起远程调用。
    hdfHelloInterfaceHost->stub = OHOS::HDI::ObjectCollector::GetInstance().GetOrNewObject(serviceImpl,
        OHOS::HDI::Hello::V1_0::IHelloInterface::GetDescriptor());
    if (hdfHelloInterfaceHost->stub == nullptr) {
        HDF_LOGE("%{public}s: failed to get stub object", __func__);
        delete hdfHelloInterfaceHost;
        return HDF_FAILURE;
    }

    // 将ioService绑定到deviceObject,这样HDF框架就可以通过deviceObject来访问服务
    deviceObject->service = &hdfHelloInterfaceHost->ioService;
    HDF_LOGI("%{public}s: driver bind end", __func__);
    return HDF_SUCCESS;
}

// 驱动释放资源的接口
static void HdfHelloInterfaceDriverRelease(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGI("%{public}s: driver release start", __func__);
    if (deviceObject->service == nullptr) {
        return;
    }

    auto *hdfHelloInterfaceHost = CONTAINER_OF(deviceObject->service, struct HdfHelloInterfaceHost, ioService);
    if (hdfHelloInterfaceHost != nullptr) {
        delete hdfHelloInterfaceHost;
    }
}

/*
 * 定义驱动入口的对象，必须为HdfDriverEntry（在hdf_device_desc.h中定义）类型的全局变量。
 */
struct HdfDriverEntry g_hellointerfaceDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "hello_service",
    .Bind = HdfHelloInterfaceDriverBind,
    .Init = HdfHelloInterfaceDriverInit,
    .Release = HdfHelloInterfaceDriverRelease,
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * 调用HDF_INIT将驱动入口注册到HDF框架中。
 * 在加载驱动时HDF框架会先调用Bind函数，再调用Init函数加载该驱动；当Init调用异常时，HDF框架会调用Release释放驱动资源并退出。
 */
HDF_INIT(g_hellointerfaceDriverEntry);
#ifdef __cplusplus
}
#endif /* __cplusplus */
