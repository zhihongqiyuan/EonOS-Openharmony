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

#ifndef OHOS_HDI_DISPLAY_V1_0_DISPLAY_CMD_REQUESTER_H
#define OHOS_HDI_DISPLAY_V1_0_DISPLAY_CMD_REQUESTER_H

#include <unordered_map>
#include "base/hdi_smq.h"
#include "command_pack/command_data_packer.h"
#include "command_pack/command_data_unpacker.h"
#include "common/include/display_interface_utils.h"
#include "display_cmd_utils.h"
#include "hdifd_parcelable.h"
#include "hdf_log.h"
#include "v1_0/display_composer_type.h"
#include "v1_0/idisplay_composer.h"

namespace OHOS {
namespace HDI {
namespace Display {
namespace Composer {
namespace V1_0 {
using namespace OHOS::HDI::Base;
using namespace OHOS::HDI::Display::Composer::V1_0;

template <typename Transfer, typename CompHdi>
class DisplayCmdRequester {
public:
    DisplayCmdRequester(sptr<CompHdi> hdi)
        : initFlag_(false),
        hdi_(hdi),
        request_(nullptr),
        reply_(nullptr)
    {
    }

    static std::unique_ptr<DisplayCmdRequester> Create(sptr<CompHdi> hdi)
    {
        DISPLAY_CHK_RETURN(hdi == nullptr, nullptr, HDF_LOGE("%{public}s: hdi is nullptr", __func__));
        auto requester = std::make_unique<DisplayCmdRequester>(hdi);
        DISPLAY_CHK_RETURN(requester == nullptr, nullptr,
            HDF_LOGE("%{public}s: CmdRequester is nullptr", __func__));
        auto ret = requester->Init(CmdUtils::INIT_ELEMENT_COUNT);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("DisplayCmdRequester init failed");
            return nullptr;
        }
        return requester;
    }

    int32_t Init(uint32_t eleCnt)
    {
        request_ = std::make_shared<Transfer>(eleCnt, SmqType::SYNCED_SMQ);
        DISPLAY_CHK_RETURN(request_ == nullptr, HDF_FAILURE,
            HDF_LOGE("%{public}s: request_ is nullptr", __func__));

        DISPLAY_CHK_RETURN(hdi_ == nullptr, HDF_FAILURE,
            HDF_LOGE("%{public}s: hdi_ is nullptr", __func__));

        int32_t ret = hdi_->InitCmdRequest(request_);
        DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
            HDF_LOGE("%{public}s: InitCmdRequest failure, ret=%{public}d", __func__, ret));

        if (requestPacker_.Init(request_->GetSize() << CmdUtils::MOVE_SIZE) == false) {
            HDF_LOGE("%{public}s: requestPacker init failed", __func__);
            return HDF_FAILURE;
        }

        replyData_.reset(new char[CmdUtils::INIT_ELEMENT_COUNT], std::default_delete<char[]>());
        if (replyData_ == nullptr) {
            HDF_LOGE("replyData alloc failed.");
            return HDF_FAILURE;
        }

        ret = hdi_->GetCmdReply(reply_);
        DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
            HDF_LOGE("%{public}s: GetCmdReply failure, ret=%{public}d", __func__, ret));
        initFlag_ = true;

        ret = CmdUtils::StartPack(CONTROL_CMD_REQUEST_BEGIN, requestPacker_);
        DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
            HDF_LOGE("%{public}s: StartPack failed", __func__));

        return HDF_SUCCESS;
    }

    int32_t PrepareDisplayLayers(uint32_t devId, bool &needFlushFb)
    {
        uint32_t replyEleCnt;
        std::vector<HdifdInfo> outFds;

        int32_t ret = CmdUtils::StartSection(REQUEST_CMD_PREPARE_DISPLAY_LAYERS, requestPacker_);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = requestPacker_.WriteUint32(devId) ? HDF_SUCCESS : HDF_FAILURE;
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = CmdUtils::EndSection(requestPacker_);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = CmdUtils::EndPack(requestPacker_);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = DoRequest(replyEleCnt, outFds);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = DoReplyResults(replyEleCnt, outFds, [&](void *data) -> int32_t {
            needFlushFb = *(reinterpret_cast<bool *>(data));
            return HDF_SUCCESS;
        });
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("DoReplyResults failure, ret=%{public}d", ret);
        }

EXIT:
        return PeriodDataReset() == HDF_SUCCESS ? ret : HDF_FAILURE;
    }

    int32_t SetDisplayClientBuffer(uint32_t devId, const BufferHandle* buffer, uint32_t seqNo,
        int32_t fence)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_DISPLAY_CLIENT_BUFFER, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            retBool = requestPacker_.WriteUint32(devId);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write devId failed", __func__));

            ret = CmdUtils::BufferHandlePack(buffer, requestPacker_, requestHdiFds_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: BufferHandlePack failed", __func__));

            retBool = requestPacker_.WriteUint32(seqNo);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write seqNo failed", __func__));

            ret = CmdUtils::FileDescriptorPack(fence, requestPacker_, requestHdiFds_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: FileDescriptorPack failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetDisplayClientDamage(uint32_t devId, std::vector<IRect> &rects)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_DISPLAY_CLIENT_DAMAGE, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = requestPacker_.WriteUint32(devId) ? HDF_SUCCESS : HDF_FAILURE;
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: write devId failed", __func__));

            uint32_t vectSize = static_cast<uint32_t>(rects.size());
            retBool = requestPacker_.WriteUint32(vectSize);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write damage vector size failed", __func__));

            for (uint32_t i = 0; i < vectSize; i++) {
                ret = CmdUtils::RectPack(rects[i], requestPacker_);
                DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                    HDF_LOGE("%{public}s: RectPack failed", __func__));
            }
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: RectPack failed, break!", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t Commit(uint32_t devId, int32_t &fence)
    {
        uint32_t replyEleCnt = 0;
        std::vector<HdifdInfo> outFds;

        int32_t ret = CmdUtils::StartSection(REQUEST_CMD_COMMIT, requestPacker_);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = requestPacker_.WriteUint32(devId) ? HDF_SUCCESS : HDF_FAILURE;
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = CmdUtils::EndSection(requestPacker_);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = CmdUtils::EndPack(requestPacker_);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = DoRequest(replyEleCnt, outFds);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = DoReplyResults(replyEleCnt, outFds, [&](void *data) -> int32_t {
            fence = *(reinterpret_cast<int32_t *>(data));
            return HDF_SUCCESS;
        });
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("DoReplyResults failure, ret=%{public}d", ret);
        }

EXIT:
        return PeriodDataReset() == HDF_SUCCESS ? ret : HDF_FAILURE;
    }

    int32_t SetLayerAlpha(uint32_t devId, uint32_t layerId, const LayerAlpha &alpha)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_ALPHA, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: write devId failed", __func__));

            retBool = requestPacker_.WriteBool(alpha.enGlobalAlpha);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write enGlobalAlpha failed", __func__));

            retBool = requestPacker_.WriteBool(alpha.enPixelAlpha);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write enPixelAlpha failed", __func__));

            retBool = requestPacker_.WriteUint8(alpha.alpha0);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write alpha0 failed", __func__));

            retBool = requestPacker_.WriteUint8(alpha.alpha1);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write alpha1 failed", __func__));

            retBool = requestPacker_.WriteUint8(alpha.gAlpha);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write gAlpha failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerRegion(uint32_t devId, uint32_t layerId, const IRect &rect)
    {
        int32_t ret = 0;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_REGION, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            ret = CmdUtils::RectPack(rect, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: RectPack failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerCrop(uint32_t devId, uint32_t layerId, const IRect &rect)
    {
        int32_t ret = 0;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_CROP, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            ret = CmdUtils::RectPack(rect, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: RectPack failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerZorder(uint32_t devId, uint32_t layerId, uint32_t zorder)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_ZORDER, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            retBool = requestPacker_.WriteUint32(zorder);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write zorder failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerPreMulti(uint32_t devId, uint32_t layerId, bool preMul)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_PREMULTI, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            retBool = requestPacker_.WriteBool(preMul);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write preMul failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }

        return HDF_SUCCESS;
    }

    int32_t SetLayerTransformMode(uint32_t devId, uint32_t layerId, TransformType type)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_TRANSFORM_MODE, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            retBool = requestPacker_.WriteInt32(type);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write transform-type failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerDirtyRegion(uint32_t devId, uint32_t layerId, const std::vector<IRect> &rects)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_DIRTY_REGION, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            uint32_t vSize = rects.size();
            retBool = requestPacker_.WriteUint32(vSize);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write vSize failed", __func__));
            for (uint32_t i = 0; i < vSize; i++) {
                ret = CmdUtils::RectPack(rects[i], requestPacker_);
                DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                    HDF_LOGE("%{public}s: RectPack failed", __func__));
            }
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: RectPack failed, break", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerVisibleRegion(uint32_t devId, uint32_t layerId, std::vector<IRect> &rects)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_VISIBLE_REGION, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            uint32_t vSize = rects.size();
            retBool = requestPacker_.WriteUint32(vSize);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write vSize failed", __func__));
            for (uint32_t i = 0; i < vSize; i++) {
                ret = CmdUtils::RectPack(rects[i], requestPacker_);
                DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                    HDF_LOGE("%{public}s: RectPack failed", __func__));
            }
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: RectPack failed, break", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerBuffer(uint32_t devId, uint32_t layerId, const BufferHandle* buffer, uint32_t seqNo,
        int32_t fence, const std::vector<uint32_t>& deletingList)
    {
        int32_t ret = 0;
        bool retBool = false;
        bool result = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_BUFFER, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            ret = CmdUtils::BufferHandlePack(buffer, requestPacker_, requestHdiFds_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: BufferHandlePack failed", __func__));

            result = requestPacker_.WriteUint32(seqNo);
            DISPLAY_CHK_BREAK(result == false,
                HDF_LOGE("%{public}s: write seqNo failed", __func__));

            ret = CmdUtils::FileDescriptorPack(fence, requestPacker_, requestHdiFds_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: FileDescriptorPack failed", __func__));
            // write deletingList
            uint32_t vectSize = static_cast<uint32_t>(deletingList.size());
            retBool = requestPacker_.WriteUint32(vectSize);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write vector size failed", __func__));

            for (uint32_t i = 0; i < vectSize; i++) {
                bool result = requestPacker_.WriteUint32(deletingList[i]);
                DISPLAY_CHK_BREAK(result == false,
                    HDF_LOGE("%{public}s: write deletingList failed", __func__));
            }
            DISPLAY_CHK_BREAK(result == false,
                HDF_LOGE("%{public}s: write deletingList failed, break!", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || result == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerCompositionType(uint32_t devId, uint32_t layerId, CompositionType type)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_COMPOSITION_TYPE, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            retBool = requestPacker_.WriteInt32(type);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write composition type failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerBlendType(uint32_t devId, uint32_t layerId, BlendType type)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_BLEND_TYPE, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            retBool = requestPacker_.WriteInt32(type);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write blend type failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerMaskInfo(uint32_t devId, uint32_t layerId, const MaskInfo maskInfo)
    {
        int32_t ret = 0;
        bool retBool = false;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_MASK_INFO, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            retBool = requestPacker_.WriteUint32(maskInfo);
            DISPLAY_CHK_BREAK(retBool == false,
                HDF_LOGE("%{public}s: write maskInfo failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (retBool == false || ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerColor(uint32_t devId, uint32_t layerId, const LayerColor& layerColor)
    {
        int32_t ret = 0;
        size_t writePos = requestPacker_.ValidSize();

        do {
            ret = CmdUtils::StartSection(REQUEST_CMD_SET_LAYER_COLOR, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            ret = CmdUtils::SetupDevice(devId, layerId, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: SetupDevice failed", __func__));

            ret = CmdUtils::LayerColorPack(layerColor, requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: RectPack failed", __func__));

            ret = CmdUtils::EndSection(requestPacker_);
            DISPLAY_CHK_BREAK(ret != HDF_SUCCESS,
                HDF_LOGE("%{public}s: EndSection failed", __func__));
        } while (0);

        if (ret != HDF_SUCCESS) {
            requestPacker_.RollBack(writePos);
            HDF_LOGE("%{public}s: writePos_ rollback", __func__);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    int32_t GetDisplayCompChange(uint32_t devId, std::vector<uint32_t>& layers, std::vector<int32_t>& types)
    {
        layers = compChangeLayers_[devId];
        types = compChangeTypes_[devId];
        compChangeLayers_.erase(devId);
        compChangeTypes_.erase(devId);
        return HDF_SUCCESS;
    }
protected:
    int32_t OnReplySetError(CommandDataUnpacker& replyUnpacker, std::unordered_map<int32_t, int32_t> &errMaps)
    {
        uint32_t errCnt = 0;
        bool retBool = replyUnpacker.ReadUint32(errCnt);
        DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE,
            HDF_LOGE("%{public}s: read err cnt failed", __func__));

        int32_t errCmd = -1;
        int32_t errCode = -1;
        for (; errCnt > 0; errCnt--) {
            retBool = replyUnpacker.ReadInt32(errCmd);
            DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE,
                HDF_LOGE("%{public}s: read err cmd failed", __func__));
            retBool = replyUnpacker.ReadInt32(errCode);
            DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE,
                HDF_LOGE("%{public}s: read err code failed", __func__));
            errMaps.emplace(errCmd, errCode);
        }

        return HDF_SUCCESS;
    }

    int32_t OnReplyPrepareDisplayLayers(CommandDataUnpacker& replyUnpacker, bool &needFlushFb)
    {
        uint32_t devId = 0;
        int32_t retBool = replyUnpacker.ReadUint32(devId);
        DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE, HDF_LOGE("%{public}s: read devId failed", __func__));

        retBool = replyUnpacker.ReadBool(needFlushFb);
        DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE, HDF_LOGE("%{public}s: read needFlushFb failed", __func__));
        // unpack layers vector
        uint32_t vectSize = 0;
        retBool = replyUnpacker.ReadUint32(vectSize);
        DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE, HDF_LOGE("%{public}s: read vect size failed", __func__));
        if (vectSize > CmdUtils::MAX_MEMORY) {
            HDF_LOGE("%{public}s: layers vectSize:%{public}u is too large", __func__, vectSize);
            return HDF_FAILURE;
        }

        compChangeLayers_[devId].resize(vectSize);
        for (uint32_t i = 0; i < vectSize; i++) {
            DISPLAY_CHK_RETURN(replyUnpacker.ReadUint32(compChangeLayers_[devId][i]) == false, HDF_FAILURE,
                HDF_LOGE("%{public}s: read layer vector failed", __func__));
        }
        // unpack types vector
        vectSize = 0;
        retBool = replyUnpacker.ReadUint32(vectSize);
        DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE, HDF_LOGE("%{public}s: read vect size failed", __func__));
        if (vectSize > CmdUtils::MAX_MEMORY) {
            HDF_LOGE("%{public}s: types vectSize:%{public}u is too large", __func__, vectSize);
            return HDF_FAILURE;
        }

        compChangeTypes_[devId].resize(vectSize);
        for (uint32_t i = 0; i < vectSize; i++) {
            DISPLAY_CHK_RETURN(replyUnpacker.ReadInt32(compChangeTypes_[devId][i]) == false, HDF_FAILURE,
                HDF_LOGE("%{public}s: read composition type vector failed", __func__));
        }

        return HDF_SUCCESS;
    }

    int32_t OnReplyCommit(
        CommandDataUnpacker& replyUnpacker, std::vector<HdifdInfo>& replyFds, int32_t &fenceFd)
    {
        int32_t ret = CmdUtils::FileDescriptorUnpack(replyUnpacker, replyFds, fenceFd);
        DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
            HDF_LOGE("%{public}s: FileDescriptorUnpack failed", __func__));
        return HDF_SUCCESS;
    }

    int32_t ProcessUnpackCmd(CommandDataUnpacker& replyUnpacker, int32_t unpackCmd,
        std::vector<HdifdInfo>& replyFds, std::function<int32_t(void *)> fn)
    {
        int32_t ret = HDF_SUCCESS;
        while (replyUnpacker.NextSection()) {
            bool retBool = replyUnpacker.BeginSection(unpackCmd);
            DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE,
                HDF_LOGE("%{public}s: BeginSection failed", __func__));

            bool needFlushFb;
            int32_t fenceFd = -1;
            std::unordered_map<int32_t, int32_t> errMaps;
            switch (unpackCmd) {
                case REPLY_CMD_PREPARE_DISPLAY_LAYERS:
                    ret = OnReplyPrepareDisplayLayers(replyUnpacker, needFlushFb);
                    DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
                        HDF_LOGE("%{public}s: OnReplyPrepareDisplayLayers failed", __func__));

                    ret = fn(&needFlushFb);
                    DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
                        HDF_LOGE("%{public}s: ReadBool failed, unpackCmd=%{public}s",
                        __func__, CmdUtils::CommandToString(unpackCmd)));
                    break;
                case REPLY_CMD_COMMIT:
                    ret = OnReplyCommit(replyUnpacker, replyFds, fenceFd);
                    DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
                        HDF_LOGE("%{public}s: OnReplyCommit failed unpackCmd=%{public}s",
                        __func__, CmdUtils::CommandToString(unpackCmd)));

                    ret = fn(&fenceFd);
                    DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
                        HDF_LOGE("%{public}s: return fence fd error, unpackCmd=%{public}s",
                        __func__, CmdUtils::CommandToString(unpackCmd)));
                    break;
                case REPLY_CMD_SET_ERROR:
                    ret = OnReplySetError(replyUnpacker, errMaps);
                    DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
                        HDF_LOGE("%{public}s: OnReplySetError failed", __func__));
                    DISPLAY_CHK_RETURN(errMaps.size() > 0, HDF_FAILURE,
                        HDF_LOGE("error: server return errs, size=%{public}zu", errMaps.size()));
                    break;
                default:
                    HDF_LOGE("Unpack command failure");
                    return HDF_FAILURE;
            }
        }
        return HDF_SUCCESS;
    }

    int32_t DoReplyResults(uint32_t replyEleCnt, std::vector<HdifdInfo>& replyFds, std::function<int32_t(void *)> fn)
    {
        CommandDataUnpacker replyUnpacker;
        replyUnpacker.Init(replyData_.get(), replyEleCnt << CmdUtils::MOVE_SIZE);
#ifdef DEBUG_DISPLAY_CMD_RAW_DATA
        replyUnpacker.Dump();
#endif // DEBUG_DISPLAY_CMD_RAW_DATA
        int32_t unpackCmd = -1;
        bool retBool = replyUnpacker.PackBegin(unpackCmd);
        DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE,
            HDF_LOGE("%{public}s: PackBegin failed", __func__));
        DISPLAY_CHK_RETURN(unpackCmd != CONTROL_CMD_REPLY_BEGIN, HDF_FAILURE,
            HDF_LOGE("%{public}s: PackBegin cmd not match, unpackCmd=%{public}d", __func__, unpackCmd));
        if (ProcessUnpackCmd(replyUnpacker, unpackCmd, replyFds, fn) != HDF_SUCCESS) {
            return HDF_FAILURE;
        }

        retBool = replyUnpacker.PackEnd(unpackCmd);
        DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE,
            HDF_LOGE("%{public}s: PackEnd failed", __func__));

        DISPLAY_CHK_RETURN(unpackCmd != CONTROL_CMD_REPLY_END, HDF_FAILURE,
            HDF_LOGE("%{public}s: PackEnd failed, endCmd = %{public}s",
            __func__, CmdUtils::CommandToString(unpackCmd)));

        return HDF_SUCCESS;
    }

    int32_t DoRequest(uint32_t &replyEleCnt, std::vector<HdifdInfo> &outFds)
    {
#ifdef DEBUG_DISPLAY_CMD_RAW_DATA
        requestPacker_.Dump();
#endif // DEBUG_DISPLAY_CMD_RAW_DATA
        uint32_t eleCnt = requestPacker_.ValidSize() >> CmdUtils::MOVE_SIZE;
        int32_t ret = request_->Write(
            reinterpret_cast<int32_t *>(requestPacker_.GetDataPtr()), eleCnt, CmdUtils::TRANSFER_WAIT_TIME);
        DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
            HDF_LOGE("%{public}s: CmdRequest write failed", __func__));

        ret = hdi_->CmdRequest(eleCnt, requestHdiFds_, replyEleCnt, outFds);
        DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
            HDF_LOGE("%{public}s: CmdRequest failed", __func__));

        if (replyEleCnt != 0) {
            ret = reply_->Read(reinterpret_cast<int32_t *>(replyData_.get()),
                               replyEleCnt, CmdUtils::TRANSFER_WAIT_TIME);
            if (ret != HDF_SUCCESS) {
                HDF_LOGE("reply read data failure, ret=%{public}d", ret);
            }
        }

        return ret;
    }

    int32_t PeriodDataReset()
    {
        for (uint32_t i = 0; i < requestHdiFds_.size(); ++i) {
            int32_t fd = requestHdiFds_[i].hdiFd->Move();
            if (fd != -1) {
                close(fd);
            }
        }
        requestHdiFds_.clear();
        int32_t ret = CmdUtils::StartPack(CONTROL_CMD_REQUEST_BEGIN, requestPacker_);
        DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
            HDF_LOGE("%{public}s: StartPack failed", __func__));

        return HDF_SUCCESS;
    }

protected:
    bool initFlag_;
    sptr<CompHdi> hdi_;
    std::shared_ptr<Transfer> request_;
    std::shared_ptr<Transfer> reply_;
    std::shared_ptr<char> replyData_;
    // Period data
    CommandDataPacker requestPacker_;
    std::vector<HdifdInfo> requestHdiFds_;
    // Composition layers/types changed
    std::unordered_map<uint32_t, std::vector<uint32_t>> compChangeLayers_;
    std::unordered_map<uint32_t, std::vector<int32_t>> compChangeTypes_;
};
using HdiDisplayCmdRequester = DisplayCmdRequester<SharedMemQueue<int32_t>, IDisplayComposer>;
} // namespace V1_0
} // namespace Composer
} // namespace Display
} // namespace HDI
} // namespace OHOS
#endif // OHOS_HDI_DISPLAY_V1_0_DISPLAY_CMD_REQUESTER_H
