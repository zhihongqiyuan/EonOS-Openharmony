/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_HDI_DISPLAY_V1_0_DISPLAY_CMD_UTILS_H
#define OHOS_HDI_DISPLAY_V1_0_DISPLAY_CMD_UTILS_H

#include "buffer_handle_utils.h"
#include "command_pack/command_data_packer.h"
#include "command_pack/command_data_unpacker.h"
#include "common/include/display_interface_utils.h"
#include "v1_0/display_composer_type.h"

#undef LOG_TAG
#define LOG_TAG "DISP_CMD"
#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD002515

#define INVALID_FD (-1)

namespace OHOS {
namespace HDI {
namespace Display {
namespace Composer {
namespace V1_0 {
using namespace OHOS::HDI::Display::Composer::V1_0;

class DisplayCmdUtils {
public:
    static constexpr int32_t MAX_INT = 0x7fffffff;
    static constexpr uint32_t ELEMENT_SIZE = sizeof(int32_t);
    static constexpr uint32_t MOVE_SIZE = sizeof(int) / 2;
    static constexpr uint32_t TRANSFER_WAIT_TIME = 100000000; // ms
    static constexpr uint32_t INIT_ELEMENT_COUNT = 32 * 1024;
    static constexpr uint32_t MAX_MEMORY = 10485760; // 10M
    static constexpr uint32_t MAX_ELE_COUNT = 100000;

    #define SWITCHCASE(x) case (x): {return #x;}
    static const char *CommandToString(int32_t cmdId)
    {
        switch (cmdId) {
            /* request cmd */
            SWITCHCASE(REQUEST_CMD_PREPARE_DISPLAY_LAYERS);
            SWITCHCASE(REQUEST_CMD_SET_DISPLAY_CLIENT_BUFFER);
            SWITCHCASE(REQUEST_CMD_SET_DISPLAY_CLIENT_DAMAGE);
            SWITCHCASE(REQUEST_CMD_COMMIT);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_ALPHA);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_REGION);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_CROP);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_ZORDER);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_PREMULTI);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_TRANSFORM_MODE);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_DIRTY_REGION);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_VISIBLE_REGION);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_BUFFER);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_COMPOSITION_TYPE);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_BLEND_TYPE);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_VISIBLE);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_MASK_INFO);
            SWITCHCASE(REQUEST_CMD_SET_LAYER_COLOR);
            /* reply cmd */
            SWITCHCASE(REPLY_CMD_SET_ERROR);
            SWITCHCASE(REPLY_CMD_PREPARE_DISPLAY_LAYERS);
            SWITCHCASE(REPLY_CMD_COMMIT);
            /* pack control cmd */
            SWITCHCASE(CONTROL_CMD_REQUEST_BEGIN);
            SWITCHCASE(CONTROL_CMD_REPLY_BEGIN);
            SWITCHCASE(CONTROL_CMD_REQUEST_END);
            SWITCHCASE(CONTROL_CMD_REPLY_END);
            default:
                return "unknow command id.";
        }
    }

    static int32_t StartPack(int32_t cmdId, CommandDataPacker& packer)
    {
        return packer.PackBegin(cmdId) ? HDF_SUCCESS : HDF_FAILURE;
    }

    static int32_t EndPack(CommandDataPacker& packer)
    {
        return packer.PackEnd(CONTROL_CMD_REQUEST_END) ? HDF_SUCCESS : HDF_FAILURE;
    }

    static int32_t StartSection(int32_t cmdId, CommandDataPacker& packer)
    {
        return packer.BeginSection(cmdId) ? HDF_SUCCESS : HDF_FAILURE;
    }

    static int32_t SetupDevice(uint32_t devId, uint32_t layerId, CommandDataPacker& packer)
    {
        DISPLAY_CHK_RETURN(packer.WriteUint32(devId) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write devId error", __func__));
        DISPLAY_CHK_RETURN(packer.WriteUint32(layerId) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write layerId error", __func__));
        return HDF_SUCCESS;
    }

    static int32_t EndSection(CommandDataPacker& packer)
    {
        return packer.EndSection() ? HDF_SUCCESS : HDF_FAILURE;
    }

    static int32_t GenerateHdifdSeqid()
    {
        static int32_t HdifdSeqidCursor = 0;

        if (HdifdSeqidCursor <= MAX_INT) {
            ++HdifdSeqidCursor;
            return HdifdSeqidCursor;
        } else {
            return 0;
        }
    }

    static bool MatchHdiFd(int32_t id, std::vector<HdifdInfo> hdiFds, int32_t& fd)
    {
        for (uint32_t i = 0; i < hdiFds.size(); ++i) {
            if (hdiFds[i].hdiFd == nullptr) {
                HDF_LOGE("%{public}s, hdiFd is nullptr", __func__);
                continue;
            }
            if (hdiFds[i].id == id) {
                fd = hdiFds[i].hdiFd->Move();
                return true;
            }
        }
        return false;
    }

    static int32_t RectPack(const IRect& rect, CommandDataPacker& packer)
    {
        DISPLAY_CHK_RETURN(packer.WriteInt32(rect.x) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write rect.x error", __func__));
        DISPLAY_CHK_RETURN(packer.WriteInt32(rect.y) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write rect.y error", __func__));
        DISPLAY_CHK_RETURN(packer.WriteInt32(rect.w) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write rect.w error", __func__));
        DISPLAY_CHK_RETURN(packer.WriteInt32(rect.h) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write rect.h error", __func__));
        return HDF_SUCCESS;
    }

    static int32_t LayerColorPack(const LayerColor& layerColor, CommandDataPacker& packer)
    {
        DISPLAY_CHK_RETURN(packer.WriteUint8(layerColor.r) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write layerColor.r error", __func__));
        DISPLAY_CHK_RETURN(packer.WriteUint8(layerColor.g) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write layerColor.g error", __func__));
        DISPLAY_CHK_RETURN(packer.WriteUint8(layerColor.b) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write layerColor.b error", __func__));
        DISPLAY_CHK_RETURN(packer.WriteUint8(layerColor.a) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write layerColor.a error", __func__));
        return HDF_SUCCESS;
    }

    static int32_t FileDescriptorPack(
        const int32_t fd, CommandDataPacker& packer, std::vector<HdifdInfo>& hdiFds, bool dupFd = true)
    {
        if (fd < 0) {
            DISPLAY_CHK_RETURN(packer.WriteInt32(fd) == false, HDF_FAILURE,
                HDF_LOGE("%{public}s, write fd error", __func__));
            return HDF_SUCCESS;
        }

        HdifdInfo hdifdInfo;
        hdifdInfo.id = GenerateHdifdSeqid();
        if (dupFd) {
            hdifdInfo.hdiFd = new HdifdParcelable();
            DISPLAY_CHK_RETURN(hdifdInfo.hdiFd == nullptr, HDF_FAILURE,
                HDF_LOGE("%{public}s, new HdifdParcelable failed", __func__));
            // A normal fd is transfered by binder, here just write id for unpacking to match fd.
            DISPLAY_CHK_RETURN(hdifdInfo.hdiFd->Init(fd) == false, HDF_FAILURE,
                HDF_LOGE("%{public}s, hdiFd init failed, fd:%{public}d", __func__, fd));
        } else {
            hdifdInfo.hdiFd = new HdifdParcelable(fd);
            DISPLAY_CHK_RETURN(hdifdInfo.hdiFd == nullptr, HDF_FAILURE,
                HDF_LOGE("%{public}s, new HdifdParcelable failed", __func__));
            hdifdInfo.hdiFd->Move();
        }

        hdiFds.push_back(hdifdInfo);
        DISPLAY_CHK_RETURN(packer.WriteInt32(hdifdInfo.id) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write hdifdInfo.id failed", __func__));

        return HDF_SUCCESS;
    }

    static int32_t BufferHandlePack(const BufferHandle* buffer, CommandDataPacker& packer,
        std::vector<HdifdInfo>& hdiFds)
    {
        if (buffer == nullptr) {
            DISPLAY_CHK_RETURN(packer.WriteUint32(UINT32_MAX) == false, HDF_FAILURE,
                HDF_LOGE("%{public}s, write null buffer reserveFds error", __func__));
            DISPLAY_CHK_RETURN(packer.WriteUint32(UINT32_MAX) == false, HDF_FAILURE,
                HDF_LOGE("%{public}s, write null buffer reservceInts error", __func__));
            return HDF_SUCCESS;
        } else {
            DISPLAY_CHK_RETURN(packer.WriteUint32(buffer->reserveFds) == false, HDF_FAILURE,
                HDF_LOGE("%{public}s, write buffer->reserveFds error", __func__));
            DISPLAY_CHK_RETURN(packer.WriteUint32(buffer->reserveInts) == false, HDF_FAILURE,
                HDF_LOGE("%{public}s, write buffer->reserveInts error", __func__));
        }
        int32_t ret = FileDescriptorPack(buffer->fd, packer, hdiFds);
        if (ret != HDF_SUCCESS) {
            return ret;
        }
        DISPLAY_CHK_RETURN(packer.WriteInt32(buffer->width) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write buffer->width failed", __func__));
        DISPLAY_CHK_RETURN(packer.WriteInt32(buffer->stride) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write buffer->stride failed", __func__));
        DISPLAY_CHK_RETURN(packer.WriteInt32(buffer->height) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write buffer->height failed", __func__));
        DISPLAY_CHK_RETURN(packer.WriteInt32(buffer->size) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write buffer->size failed", __func__));
        DISPLAY_CHK_RETURN(packer.WriteInt32(buffer->format) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write buffer->format failed", __func__));
        DISPLAY_CHK_RETURN(packer.WriteUint64(buffer->usage) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write buffer->usage failed", __func__));
        DISPLAY_CHK_RETURN(packer.WriteUint64(buffer->phyAddr) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, write buffer->phyAddr failed", __func__));
        uint32_t i = 0;
        for (i = 0; i < buffer->reserveFds; i++) {
            ret = FileDescriptorPack(buffer->reserve[i], packer, hdiFds);
            if (ret != HDF_SUCCESS) {
                return ret;
            }
        }
        for (uint32_t j = 0; j < buffer->reserveInts; j++) {
            DISPLAY_CHK_RETURN(packer.WriteInt32(buffer->reserve[i++]) == false, HDF_FAILURE,
                HDF_LOGE("%{public}s, write buffer->reserve failed", __func__));
        }
        return HDF_SUCCESS;
    }

    static int32_t SetupDeviceUnpack(CommandDataUnpacker& unpacker, uint32_t& devId, uint32_t& layerId)
    {
        DISPLAY_CHK_RETURN(unpacker.ReadUint32(devId) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read devId failed", __func__));
        DISPLAY_CHK_RETURN(unpacker.ReadUint32(layerId) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read layerId failed", __func__));
        return HDF_SUCCESS;
    }

    static int32_t RectUnpack(CommandDataUnpacker& unpacker, IRect& rect)
    {
        DISPLAY_CHK_RETURN(unpacker.ReadInt32(rect.x) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read rect.x failed", __func__));
        DISPLAY_CHK_RETURN(unpacker.ReadInt32(rect.y) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read rect.y failed", __func__));
        DISPLAY_CHK_RETURN(unpacker.ReadInt32(rect.w) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read rect.w failed", __func__));
        DISPLAY_CHK_RETURN(unpacker.ReadInt32(rect.h) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read rect.h failed", __func__));
        return HDF_SUCCESS;
    }

    static int32_t FileDescriptorUnpack(
        CommandDataUnpacker& unpacker, const std::vector<HdifdInfo>& hdiFds, int32_t& fd)
    {
        int32_t fdId = -1;
        DISPLAY_CHK_RETURN(unpacker.ReadInt32(fdId) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read fdId failed", __func__));
        if (fdId < 0) {
            fd = INVALID_FD;
            return HDF_SUCCESS;
        }
        if (!(MatchHdiFd(fdId, hdiFds, fd))) {
            // If matching failure, the illegal fd is transfered by smq directly, not by binder IPC.
            HDF_LOGE("%{public}s, matching failure, fd %{public}d, fdId %{public}d, hdiFds.size %{public}zu",
                     __func__, fd, fdId, hdiFds.size());
            fd = INVALID_FD;
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    static bool UnpackBasicInfo(CommandDataUnpacker& unpacker, const std::vector<HdifdInfo>& hdiFds,
        BufferHandle *handle)
    {
        int32_t ret = FileDescriptorUnpack(unpacker, hdiFds, handle->fd);
        bool retVal = (ret == HDF_SUCCESS ? true : false);
        DISPLAY_CHK_CONDITION(retVal, true, unpacker.ReadInt32(handle->width),
            HDF_LOGE("%{public}s, read handle->width error", __func__));
        DISPLAY_CHK_CONDITION(retVal, true, unpacker.ReadInt32(handle->stride),
            HDF_LOGE("%{public}s, read handle->stride error", __func__));
        DISPLAY_CHK_CONDITION(retVal, true, unpacker.ReadInt32(handle->height),
            HDF_LOGE("%{public}s, read handle->height error", __func__));
        DISPLAY_CHK_CONDITION(retVal, true, unpacker.ReadInt32(handle->size),
            HDF_LOGE("%{public}s, read handle->size error", __func__));
        DISPLAY_CHK_CONDITION(retVal, true, unpacker.ReadInt32(handle->format),
            HDF_LOGE("%{public}s, read handle->format error", __func__));
        DISPLAY_CHK_CONDITION(retVal, true, unpacker.ReadUint64(handle->usage),
            HDF_LOGE("%{public}s, read handle->usage error", __func__));
        DISPLAY_CHK_CONDITION(retVal, true, unpacker.ReadUint64(handle->phyAddr),
            HDF_LOGE("%{public}s, read handle->phyAddr error", __func__));
        return retVal;
    }

    static bool UnpackExtraInfo(CommandDataUnpacker& unpacker, const std::vector<HdifdInfo>& hdiFds,
        BufferHandle *handle)
    {
        bool retVal = true;
        uint32_t i = 0;
        for (i = 0; i < handle->reserveFds; i++) {
            int32_t ret = FileDescriptorUnpack(unpacker, hdiFds, handle->reserve[i]);
            if (ret != HDF_SUCCESS) {
                retVal = false;
                break;
            }
        }
        for (uint32_t j = 0; j < handle->reserveInts; j++) {
            retVal = unpacker.ReadInt32(handle->reserve[i++]);
            if (!retVal) {
                HDF_LOGE("%{public}s, get reserve data error, i:%{public}u, j:%{public}u",
                    __func__, i, j);
                break;
            }
        }
        return retVal;
    }

    static int32_t BufferHandleUnpack(CommandDataUnpacker& unpacker,
        const std::vector<HdifdInfo>& hdiFds, BufferHandle*& buffer)
    {
        uint32_t fdsNum = 0;
        uint32_t intsNum = 0;
        DISPLAY_CHK_RETURN(unpacker.ReadUint32(fdsNum) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read fdsNum error", __func__));
        DISPLAY_CHK_RETURN(unpacker.ReadUint32(intsNum) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read intsNum error", __func__));
        if (fdsNum == UINT32_MAX && intsNum == UINT32_MAX) {
            buffer = nullptr;
            return HDF_SUCCESS;
        }
        BufferHandle *handle = AllocateBufferHandle(fdsNum, intsNum);
        if (handle == nullptr) {
            return HDF_FAILURE;
        }
        bool retVal = UnpackBasicInfo(unpacker, hdiFds, handle);
        if (retVal) {
            retVal = UnpackExtraInfo(unpacker, hdiFds, handle);
        }
        if (!retVal) {
            if (handle != nullptr) {
                FreeBufferHandle(handle);
                handle = nullptr;
            }
            HDF_LOGE("%{public}s: buffer handle unpack failed", __func__);
        }
        buffer = handle;
        return retVal ? HDF_SUCCESS : HDF_FAILURE;
    }

    static int32_t LayerColorUnpack(CommandDataUnpacker& unpacker, LayerColor& layerColor)
    {
        DISPLAY_CHK_RETURN(unpacker.ReadUint8(layerColor.r) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read layerColor.r failed", __func__));
        DISPLAY_CHK_RETURN(unpacker.ReadUint8(layerColor.g) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read layerColor.g failed", __func__));
        DISPLAY_CHK_RETURN(unpacker.ReadUint8(layerColor.b) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read layerColor.b failed", __func__));
        DISPLAY_CHK_RETURN(unpacker.ReadUint8(layerColor.a) == false, HDF_FAILURE,
            HDF_LOGE("%{public}s, read layerColor.a failed", __func__));
        return HDF_SUCCESS;
    }
};
using CmdUtils = DisplayCmdUtils;
} // namespace V1_0
} // namespace Composer
} // namespace Display
} // namespace HDI
} // namespace OHOS
#endif // OHOS_HDI_DISPLAY_V1_0_DISPLAY_CMD_UTILS_H