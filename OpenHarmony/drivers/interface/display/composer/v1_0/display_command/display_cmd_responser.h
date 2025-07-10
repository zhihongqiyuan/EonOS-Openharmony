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

#include <fstream>
#include <poll.h>
#include <securec.h>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <unordered_map>
#include <queue>

#include "base/hdi_smq.h"
#include "buffer_handle_utils.h"
#include "command_pack/command_data_packer.h"
#include "command_pack/command_data_unpacker.h"
#include "display_cmd_utils.h"
#include "hdf_base.h"
#include "hdf_trace.h"
#include "hdifd_parcelable.h"
#include "hilog/log.h"
#include "idisplay_composer_vdi.h"
#include "parameter.h"
#include "v1_0/display_composer_type.h"
#include "v1_0/mapper_stub.h"
#include "common/include/display_vdi_adapter_interface.h"

#define DISPLAY_TRACE HdfTrace trace(__func__, "HDI:DISP:")

namespace OHOS {
namespace HDI {
namespace Display {
namespace Composer {
namespace V1_0 {
using namespace OHOS::HDI::Base;
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Display::Buffer::V1_0;
using HdifdSet = std::vector<std::shared_ptr<HdifdParcelable>>;

static constexpr uint32_t TIME_BUFFER_MAX_LEN = 15;
static constexpr uint32_t BUFFER_QUEUE_MAX_SIZE = 6;
static constexpr unsigned int REDUCE_COUNT = 50;
static constexpr int32_t ERROR_FENCE_COUNT = 500;
static sptr<IMapper> g_bufferServiceImpl = nullptr;

static constexpr uint32_t COMMIT_PRINT_INTERVAL = 1200;

template <typename Transfer, typename VdiImpl>
class DisplayCmdResponser {
public:
    static std::unique_ptr<DisplayCmdResponser> Create(VdiImpl* impl, std::shared_ptr<DeviceCacheManager> cacheMgr)
    {
        DISPLAY_CHK_RETURN(impl == nullptr, nullptr,
            HDF_LOGE("%{public}s: error, VdiImpl is nullptr", __func__));
        DISPLAY_CHK_RETURN(cacheMgr == nullptr, nullptr,
            HDF_LOGE("%{public}s: error, VdiImpl is nullptr", __func__));
        return std::make_unique<DisplayCmdResponser>(impl, cacheMgr);
    }

    DisplayCmdResponser(VdiImpl* impl, std::shared_ptr<DeviceCacheManager> cacheMgr)
        : impl_(impl),
        cacheMgr_(cacheMgr),
        request_(nullptr),
        isReplyUpdated_(false),
        reply_(nullptr),
        replyCommandCnt_(0) {}

    virtual ~DisplayCmdResponser()
    {
        while (delayFreeQueue_.size() > 0) {
            BufferHandle *temp = delayFreeQueue_.front();
            delayFreeQueue_.pop();
            FreeBufferHandle(temp);
            temp = nullptr;
        }
    }

    int32_t InitCmdRequest(const std::shared_ptr<Transfer>& request)
    {
        DISPLAY_CHK_RETURN(request == nullptr, HDF_FAILURE,
            HDF_LOGE("%{public}s: error, request is nullptr", __func__));
        std::lock_guard<std::mutex> lock(requestMutex_);
        if (request_ != nullptr) {
            request_.reset();
        }
        request_ = request;
        requestData_.reset(new char[CmdUtils::INIT_ELEMENT_COUNT], std::default_delete<char[]>());
        if (requestData_ == nullptr) {
            HDF_LOGE("requestData alloc failed.");
            return HDF_FAILURE;
        }

        return HDF_SUCCESS;
    }

    int32_t GetCmdReply(std::shared_ptr<Transfer>& reply)
    {
        std::lock_guard<std::mutex> lock(replyMutex_);
        int32_t ret = HDF_SUCCESS;
        if (isReplyUpdated_ == false) {
            ret = InitReply(CmdUtils::INIT_ELEMENT_COUNT);
        }
        if (ret == HDF_SUCCESS) {
            if (reply_ != nullptr) {
                reply = reply_;
            } else {
                ret = HDF_FAILURE;
            }
        }
        isReplyUpdated_ = false;
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("error: GetCmdReply failure");
        }

        return ret;
    }

    int32_t ProcessRequestCmd(CommandDataUnpacker& unpacker, int32_t cmd,
        const std::vector<HdifdInfo>& inFds, std::vector<HdifdInfo>& outFds)
    {
        int32_t ret = HDF_SUCCESS;
        switch (cmd) {
            case REQUEST_CMD_PREPARE_DISPLAY_LAYERS: OnPrepareDisplayLayers(unpacker); break;
            case REQUEST_CMD_SET_DISPLAY_CLIENT_BUFFER: OnSetDisplayClientBuffer(unpacker, inFds); break;
            case REQUEST_CMD_SET_DISPLAY_CLIENT_DAMAGE: OnSetDisplayClientDamage(unpacker); break;
            case REQUEST_CMD_COMMIT: OnCommit(unpacker, outFds); break;
            case REQUEST_CMD_SET_LAYER_ALPHA: OnSetLayerAlpha(unpacker); break;
            case REQUEST_CMD_SET_LAYER_REGION: OnSetLayerRegion(unpacker); break;
            case REQUEST_CMD_SET_LAYER_CROP: OnSetLayerCrop(unpacker); break;
            case REQUEST_CMD_SET_LAYER_ZORDER: OnSetLayerZorder(unpacker); break;
            case REQUEST_CMD_SET_LAYER_PREMULTI: OnSetLayerPreMulti(unpacker); break;
            case REQUEST_CMD_SET_LAYER_TRANSFORM_MODE: OnSetLayerTransformMode(unpacker); break;
            case REQUEST_CMD_SET_LAYER_DIRTY_REGION: OnSetLayerDirtyRegion(unpacker); break;
            case REQUEST_CMD_SET_LAYER_VISIBLE_REGION: OnSetLayerVisibleRegion(unpacker); break;
            case REQUEST_CMD_SET_LAYER_BUFFER: OnSetLayerBuffer(unpacker, inFds); break;
            case REQUEST_CMD_SET_LAYER_COMPOSITION_TYPE: OnSetLayerCompositionType(unpacker); break;
            case REQUEST_CMD_SET_LAYER_BLEND_TYPE: OnSetLayerBlendType(unpacker); break;
            case REQUEST_CMD_SET_LAYER_MASK_INFO: OnSetLayerMaskInfo(unpacker); break;
            case CONTROL_CMD_REQUEST_END: ret = OnRequestEnd(unpacker); break;
            case REQUEST_CMD_SET_LAYER_COLOR: OnSetLayerColor(unpacker); break;
            default:
                HDF_LOGE("%{public}s: not support this cmd, unpacked cmd = %{public}d", __func__, cmd);
                ret = HDF_FAILURE;
                break;
        }
        return ret;
    }

    int32_t CmdRequest(uint32_t inEleCnt, const std::vector<HdifdInfo>& inFds, uint32_t& outEleCnt,
        std::vector<HdifdInfo>& outFds)
    {
        if (inEleCnt > CmdUtils::MAX_ELE_COUNT) {
            HDF_LOGE("%{public}s: inEleCnt:%{public}u is too large", __func__, inEleCnt);
            return HDF_FAILURE;
        }

        int32_t ret = HDF_SUCCESS;
        {
            std::lock_guard<std::mutex> lock(requestMutex_);
            ret = request_->Read(reinterpret_cast<int32_t *>(requestData_.get()), inEleCnt,
                CmdUtils::TRANSFER_WAIT_TIME);
        }
        CommandDataUnpacker unpacker;
        unpacker.Init(requestData_.get(), inEleCnt << CmdUtils::MOVE_SIZE);
#ifdef DEBUG_DISPLAY_CMD_RAW_DATA
        unpacker.Dump();
#endif // DEBUG_DISPLAY_CMD_RAW_DATA

        int32_t unpackCmd = -1;
        bool retBool = unpacker.PackBegin(unpackCmd);
        DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE,
            HDF_LOGE("%{public}s: error: Check RequestBegin failed", __func__));
        DISPLAY_CHK_RETURN(unpackCmd != CONTROL_CMD_REQUEST_BEGIN, HDF_FAILURE,
            HDF_LOGI("error: unpacker PackBegin cmd not match, cmd(%{public}d)=%{public}s.", unpackCmd,
            CmdUtils::CommandToString(unpackCmd)));

        while (ret == HDF_SUCCESS && unpacker.NextSection()) {
            if (!unpacker.BeginSection(unpackCmd)) {
                HDF_LOGE("error: PackSection failed, unpackCmd=%{public}s.", CmdUtils::CommandToString(unpackCmd));
                ret = HDF_FAILURE;
                break;
            }
            ret = ProcessRequestCmd(unpacker, unpackCmd, inFds, outFds);
        }

        DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret, HDF_LOGE("%{public}s:ProcessRequestCmd failed", __func__));
        /* pack request end commands */
        replyPacker_.PackEnd(CONTROL_CMD_REPLY_END);

#ifdef DEBUG_DISPLAY_CMD_RAW_DATA
        /* just for debug */
        replyPacker_.Dump();
        HDF_LOGI("CmdReply command cnt=%{public}d", replyCommandCnt_);
#endif // DEBUG_DISPLAY_CMD_RAW_DATA

        /*  Write reply pack */
        outEleCnt = replyPacker_.ValidSize() >> CmdUtils::MOVE_SIZE;
        ret = CmdRequestDataWrite(outEleCnt);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("Reply write failure, ret=%{public}d", ret);
            outEleCnt = 0;
        }
        int32_t ec = PeriodDataReset();
        return (ret == HDF_SUCCESS ? ec : ret);
    }

protected:
    int32_t CmdRequestDataRead(std::shared_ptr<char> requestData, uint32_t inEleCnt)
    {
        std::lock_guard<std::mutex> lock(requestMutex_);
        if (request_ == nullptr || requestData == nullptr) {
            HDF_LOGE("%{public}s: inEleCnt: %{public}u request_ is nullptr", __func__, inEleCnt);
            return HDF_FAILURE;
        }
        return request_->Read(reinterpret_cast<int32_t *>(requestData.get()), inEleCnt,
            CmdUtils::TRANSFER_WAIT_TIME);
    }
	
    int32_t CmdRequestDataWrite(uint32_t outEleCnt)
    {
        std::lock_guard<std::mutex> lock(replyMutex_);
        if (reply_ == nullptr) {
            HDF_LOGE("%{public}s: reply_ is nullptr", __func__);
            return HDF_FAILURE;
        }
        return reply_->Write(reinterpret_cast<int32_t *>(replyPacker_.GetDataPtr()), outEleCnt,
            CmdUtils::TRANSFER_WAIT_TIME);
    }

    int32_t InitReply(uint32_t size)
    {
        if (size > CmdUtils::MAX_MEMORY) {
            HDF_LOGE("%{public}s: size:%{public}u is too large", __func__, size);
            return HDF_FAILURE;
        }
        reply_ = std::make_shared<Transfer>(size, SmqType::SYNCED_SMQ);
        DISPLAY_CHK_RETURN(reply_ == nullptr, HDF_FAILURE,
            HDF_LOGE("%{public}s: reply_ construct failed", __func__));

        bool retBool = replyPacker_.Init(reply_->GetSize() << CmdUtils::MOVE_SIZE);
        DISPLAY_CHK_RETURN(retBool == false, HDF_FAILURE,
            HDF_LOGE("%{public}s: replyPacker_ init failed", __func__));

        return CmdUtils::StartPack(CONTROL_CMD_REPLY_BEGIN, replyPacker_);
    }

    int32_t OnRequestEnd(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        size_t errCnt = errMaps_.size();
        if (errCnt >= 0) {
            int32_t ret = CmdUtils::StartSection(REPLY_CMD_SET_ERROR, replyPacker_);
            DISPLAY_CHK_RETURN(ret != HDF_SUCCESS, ret,
                HDF_LOGE("%{public}s: StartSection failed", __func__));

            bool result = replyPacker_.WriteUint32(errCnt);
            DISPLAY_CHK_RETURN(result == false, HDF_FAILURE,
                HDF_LOGE("%{public}s: write errCnt failed", __func__));
            for (auto it = errMaps_.begin(); it != errMaps_.end(); ++it) {
                result = replyPacker_.WriteInt32(it->first);
                DISPLAY_CHK_RETURN(result == false, HDF_FAILURE,
                    HDF_LOGE("%{public}s: write err-cmd failed, cmdId:%{public}s",
                    __func__, CmdUtils::CommandToString(it->first)));

                result = replyPacker_.WriteInt32(it->second);
                DISPLAY_CHK_RETURN(result == false, HDF_FAILURE,
                    HDF_LOGE("%{public}s: write errNo failed, errNo:%{public}d", __func__, it->second));
            }
            result = CmdUtils::EndSection(replyPacker_);
            DISPLAY_CHK_RETURN(result == false, HDF_FAILURE,
                HDF_LOGE("%{public}s: write replyPacker_ EndSection failed", __func__));
            replyCommandCnt_++;
        }
        return HDF_SUCCESS;
    }

    void OnPrepareDisplayLayers(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        bool needFlush = false;
        uint32_t vectSize = 0;
        std::vector<uint32_t> layers;
        std::vector<int32_t> types;

        int32_t ret = unpacker.ReadUint32(devId) ? HDF_SUCCESS : HDF_FAILURE;
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);
        {
            HdfTrace traceVdi("PrepareDisplayLayers", "HDI:DISP:HARDWARE");
            ret = impl_->PrepareDisplayLayers(devId, needFlush);
        }

        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);
        {
            HdfTrace traceVdi("GetDisplayCompChange", "HDI:DISP:HARDWARE");
            ret = impl_->GetDisplayCompChange(devId, layers, types);
        }

        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = CmdUtils::StartSection(REPLY_CMD_PREPARE_DISPLAY_LAYERS, replyPacker_);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        DISPLAY_CHECK(replyPacker_.WriteUint32(devId) == false, goto EXIT);

        DISPLAY_CHECK(replyPacker_.WriteBool(needFlush) == false, goto EXIT);
        // Write layers vector
        vectSize = static_cast<uint32_t>(layers.size());
        DISPLAY_CHECK(replyPacker_.WriteUint32(vectSize) == false, goto EXIT);

        for (uint32_t i = 0; i < vectSize; i++) {
            DISPLAY_CHECK(replyPacker_.WriteUint32(layers[i]) == false, goto EXIT);
        }
        // Write composer types vector
        vectSize = static_cast<uint32_t>(types.size());
        DISPLAY_CHECK(replyPacker_.WriteUint32(vectSize) == false, goto EXIT);

        for (uint32_t i = 0; i < vectSize; i++) {
            DISPLAY_CHECK(replyPacker_.WriteUint32(types[i]) == false, goto EXIT);
        }
        // End this cmd section
        ret = CmdUtils::EndSection(replyPacker_);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);
        replyCommandCnt_++;
EXIT:
        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_PREPARE_DISPLAY_LAYERS, ret);
        }
        return;
    }

    typedef struct ClientBufferData {
        uint32_t devId;
        uint32_t seqNo;
        int32_t fence;
        BufferHandle *buffer;
        bool isValidBuffer;
    } ClientBufferData;

    int32_t UnpackDisplayClientBufferInfo(CommandDataUnpacker& unpacker,
        const std::vector<HdifdInfo>& inFds, ClientBufferData& data)
    {
        if (!unpacker.ReadUint32(data.devId)) {
            return HDF_FAILURE;
        }

        if (CmdUtils::BufferHandleUnpack(unpacker, inFds, data.buffer) != HDF_SUCCESS) {
            data.isValidBuffer = false;
            HDF_LOGE("%{public}s, read buffer handle error", __func__);
            return HDF_FAILURE;
        }
        data.isValidBuffer = true;

        if (!unpacker.ReadUint32(data.seqNo)) {
            HDF_LOGE("%{public}s, read seqNo error", __func__);
            return HDF_FAILURE;
        }

        if (CmdUtils::FileDescriptorUnpack(unpacker, inFds, data.fence) != HDF_SUCCESS) {
            HDF_LOGE("%{public}s, FileDescriptorUnpack error", __func__);
            return HDF_FAILURE;
        }

        return HDF_SUCCESS;
    }

    int32_t SetDisplayClientBuffer(ClientBufferData& data, bool &needFreeBuffer, bool &needMoveFd, int fd)
    {
        if (cacheMgr_ == nullptr) {
            HDF_LOGE("%{public}s, get cache manager error", __func__);
            return HDF_FAILURE;
        }
        std::lock_guard<std::mutex> lock(cacheMgr_->GetCacheMgrMutex());

        DeviceCache* devCache = cacheMgr_->DeviceCacheInstance(data.devId);
        if (devCache == nullptr) {
            HDF_LOGE("%{public}s, get device cache error", __func__);
            return HDF_FAILURE;
        }

        int32_t ret = devCache->SetDisplayClientBuffer(data.buffer, data.seqNo, needFreeBuffer,
            [&](const BufferHandle& handle)->int32_t {
#ifdef DISPLAY_COMSPOER_DEBUG_DUMP
            DumpLayerBuffer(data.devId, data.seqNo, data.fence, handle, "client_");
#endif
            std::string traceMsg = "";
            if (data.buffer == nullptr) {
                traceMsg = "data.buffer is nullptr! seqNo:" + std::to_string(data.seqNo);
            } else {
                traceMsg = "HDI:DISP:HARDWARE "
                           "height:" + std::to_string(data.buffer->height) +
                           " width:" + std::to_string(data.buffer->width) +
                           " data.buffer->fd:" + std::to_string(data.buffer->fd) +
                           " seqNo:" + std::to_string(data.seqNo);
            }
            traceMsg += " fd:" + std::to_string(fd);
            if (data.fence > ERROR_FENCE_COUNT) {
                HDF_LOGI("SetDisplayClientBuffer: %{public}s", traceMsg.c_str());
            }
            HdfTrace traceVdi("SetDisplayClientBuffer", traceMsg);
            needMoveFd = true;
            int rc = impl_->SetDisplayClientBuffer(data.devId, handle, fd);
            DISPLAY_CHK_RETURN(rc != HDF_SUCCESS, HDF_FAILURE, HDF_LOGE(" fail"));
            return HDF_SUCCESS;
        });
        return ret;
    }

    void OnSetDisplayClientBuffer(CommandDataUnpacker& unpacker, const std::vector<HdifdInfo>& inFds)
    {
        DISPLAY_TRACE;

        ClientBufferData data = {0};
        data.fence = -1;
        bool needFreeBuffer = false;
        bool needMoveFd = false;
        int32_t ret = UnpackDisplayClientBufferInfo(unpacker, inFds, data);
        HdifdParcelable fdParcel(data.fence);
        if (ret == HDF_SUCCESS) {
            ret = SetDisplayClientBuffer(data, needFreeBuffer, needMoveFd, fdParcel.GetFd());
        }
#ifndef DISPLAY_COMMUNITY
        // fix fd leak
        if (data.buffer != nullptr && needFreeBuffer) {
            FreeBufferWithDelay(data.buffer);
            data.buffer = nullptr;
            data.isValidBuffer = false;
        }
        if (needMoveFd) {
            fdParcel.Move();
        }
#endif // DISPLAY_COMMUNITY
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("%{public}s, SetDisplayClientBuffer error", __func__);
            errMaps_.emplace(REQUEST_CMD_SET_DISPLAY_CLIENT_BUFFER, ret);
        }
    }

    void OnSetDisplayClientDamage(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t vectSize = 0;
        bool retBool = true;
        DISPLAY_CHK_CONDITION(retBool, true, unpacker.ReadUint32(devId),
            HDF_LOGE("%{public}s, read devId error", __func__));

        DISPLAY_CHK_CONDITION(retBool, true, unpacker.ReadUint32(vectSize),
            HDF_LOGE("%{public}s, read vectSize error", __func__));

        int32_t ret = (retBool ? HDF_SUCCESS : HDF_FAILURE);
        std::vector<IRect> rects(vectSize);
        if (ret == HDF_SUCCESS) {
            for (uint32_t i = 0; i < vectSize; i++) {
                DISPLAY_CHK_CONDITION(ret, HDF_SUCCESS, CmdUtils::RectUnpack(unpacker, rects[i]),
                    HDF_LOGE("%{public}s, read vect error at i = %{public}d", __func__, i));
                if (ret != HDF_SUCCESS) {
                    break;
                }
            }
        }
        if (ret == HDF_SUCCESS) {
            HdfTrace traceVdi("SetDisplayClientDamage", "HDI:DISP:HARDWARE");
            impl_->SetDisplayClientDamage(devId, rects);
        } else {
            HDF_LOGE("%{public}s, SetDisplayClientDamage error", __func__);
            errMaps_.emplace(REQUEST_CMD_SET_DISPLAY_CLIENT_DAMAGE, ret);
        }
        return;
    }

    void OnCommit(CommandDataUnpacker& unpacker, std::vector<HdifdInfo>& outFds)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        int32_t fence = -1;
#ifdef DISPLAY_COMSPOER_DEBUG_DUMP
        const std::string SWITCH_ON = "on";
        const uint32_t DUMP_CACHE_SWITCH_LEN = 4;
        char dumpSwitch[DUMP_CACHE_SWITCH_LEN] = {0};
        GetParameter("hdi.composer.dumpcache", "off", dumpSwitch, DUMP_CACHE_SWITCH_LEN);

        if (SWITCH_ON.compare(dumpSwitch) == 0) {
            cacheMgr_->Dump();
        }
#endif
        int32_t ret = HDF_SUCCESS;
        if (!unpacker.ReadUint32(devId)) {
            HDF_LOGE("%{public}s, read devId error", __func__);
            ret = HDF_FAILURE;
            goto REPLY;
        }

        {
            HdfTrace traceVdi("Commit", "HDI:DISP:HARDWARE");
            ret = impl_->Commit(devId, fence);
        }
        static unsigned int count = 0;
        if (ret == HDF_SUCCESS) {
            count = 0;
        } else {
            if (++count > REDUCE_COUNT) {
                HDF_LOGE("%{public}s, commit failed with ret = %{public}d", __func__, ret);
                count = 0;
            }
        }

REPLY:
        HdifdParcelable fdParcel(fence);
        DISPLAY_CHK_CONDITION(ret, HDF_SUCCESS, CmdUtils::StartSection(REPLY_CMD_COMMIT, replyPacker_),
            HDF_LOGE("%{public}s, StartSection error", __func__));

        DISPLAY_CHK_CONDITION(ret, HDF_SUCCESS, CmdUtils::FileDescriptorPack(fdParcel.GetFd(), replyPacker_, outFds),
            HDF_LOGE("%{public}s, FileDescriptorPack error", __func__));

        DISPLAY_CHK_CONDITION(ret, HDF_SUCCESS, CmdUtils::EndSection(replyPacker_),
            HDF_LOGE("%{public}s, EndSection error", __func__));

        replyCommandCnt_++;

#ifndef DISPLAY_COMMUNITY
        fdParcel.Move();
#endif // DISPLAY_COMMUNITY

        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_COMMIT, ret);
        }

        return;
    }

    void OnSetLayerAlpha(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        LayerAlpha alpha = {0};
        bool retBool = true;

        int32_t ret = CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        retBool = unpacker.ReadBool(alpha.enGlobalAlpha);
        DISPLAY_CHECK(retBool == false, goto EXIT);

        retBool = unpacker.ReadBool(alpha.enPixelAlpha);
        DISPLAY_CHECK(retBool == false, goto EXIT);

        retBool = unpacker.ReadUint8(alpha.alpha0);
        DISPLAY_CHECK(retBool == false, goto EXIT);

        retBool = unpacker.ReadUint8(alpha.alpha1);
        DISPLAY_CHECK(retBool == false, goto EXIT);

        retBool = unpacker.ReadUint8(alpha.gAlpha);
        DISPLAY_CHECK(retBool == false, goto EXIT);

        {
            HdfTrace traceVdi("SetLayerAlpha", "HDI:DISP:HARDWARE");
            ret = impl_->SetLayerAlpha(devId, layerId, alpha);
        }
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

EXIT:
        if (ret != HDF_SUCCESS || retBool == false) {
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_ALPHA, ret);
        }
        return;
    }

    void OnSetLayerRegion(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        IRect rect = {0};

        int32_t ret = CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = CmdUtils::RectUnpack(unpacker, rect);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        {
            HdfTrace traceVdi("SetLayerRegion", "HDI:DISP:HARDWARE");
            ret = impl_->SetLayerRegion(devId, layerId, rect);
        }
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);
EXIT:
        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_REGION, ret);
        }
        return;
    }

    void OnSetLayerCrop(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        IRect rect = {0};

        int32_t ret = CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = CmdUtils::RectUnpack(unpacker, rect);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        {
            HdfTrace traceVdi("SetLayerCrop", "HDI:DISP:HARDWARE");
            ret = impl_->SetLayerCrop(devId, layerId, rect);
        }
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);
EXIT:
        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_CROP, ret);
        }
        return;
    }

    int32_t CheckBufferNotExist(uint32_t devId, uint32_t layerId)
    {
        if (cacheMgr_ == nullptr) {
            HDF_LOGE("%{public}s, cacheMgr_ is null, devId:%{public}d, layerId:%{public}d",
                __func__, devId, layerId);
            return HDF_FAILURE;
        }

        std::lock_guard<std::mutex> lock(cacheMgr_->GetCacheMgrMutex());
        DeviceCache* devCache = cacheMgr_->DeviceCacheInstance(devId);
        if (devCache == nullptr) {
            HDF_LOGE("%{public}s, devCache is null, devId:%{public}d, layerId:%{public}d",
                __func__, devId, layerId);
            return HDF_FAILURE;
        }

        LayerCache* layerCache = devCache->LayerCacheInstance(layerId);
        if (layerCache == nullptr) {
            HDF_LOGE("%{public}s, layerCache is null, devId:%{public}d, layerId:%{public}d",
                __func__, devId, layerId);
            return HDF_FAILURE;
        }

        if (layerCache->IsBufferCacheNotExist()) {
            HDF_LOGE("%{public}s, no buffer in devId:%{public}d, layerId:%{public}d",
                __func__, devId, layerId);
            return HDF_FAILURE;
        }
        return HDF_SUCCESS;
    }

    void OnSetLayerZorder(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        uint32_t zorder = 0;

        int32_t ret = CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = unpacker.ReadUint32(zorder) ? HDF_SUCCESS : HDF_FAILURE;
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        {
            HdfTrace traceVdi("SetLayerZorder", "HDI:DISP:HARDWARE");
            ret = impl_->SetLayerZorder(devId, layerId, zorder);
        }
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);
EXIT:
        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_ZORDER, ret);
        }
        return;
    }

    void OnSetLayerPreMulti(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        bool preMulti = false;

        int32_t ret = CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = unpacker.ReadBool(preMulti) ? HDF_SUCCESS : HDF_FAILURE;
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        {
            HdfTrace traceVdi("SetLayerPreMulti", "HDI:DISP:HARDWARE");
            ret = impl_->SetLayerPreMulti(devId, layerId, preMulti);
        }
        DISPLAY_CHECK(ret != HDF_SUCCESS && ret != DISPLAY_NOT_SUPPORT && ret != HDF_ERR_NOT_SUPPORT, goto EXIT);
EXIT:
        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_PREMULTI, ret);
        }
        return;
    }

    void OnSetLayerTransformMode(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        int32_t type = 0;

        int32_t ret = CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = unpacker.ReadInt32(type) ? HDF_SUCCESS : HDF_FAILURE;
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        {
            HdfTrace traceVdi("SetLayerTransformMode", "HDI:DISP:HARDWARE");
            ret = impl_->SetLayerTransformMode(devId, layerId, static_cast<TransformType>(type));
        }
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);
EXIT:
        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_TRANSFORM_MODE, ret);
        }

        return;
    }

    void OnSetLayerDirtyRegion(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        uint32_t vectSize = 0;
        int32_t ret = HDF_SUCCESS;

        DISPLAY_CHK_CONDITION(ret, HDF_SUCCESS, CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId),
            HDF_LOGE("%{public}s, read devId error", __func__));

        DISPLAY_CHK_CONDITION(ret, HDF_SUCCESS, unpacker.ReadUint32(vectSize) ? HDF_SUCCESS : HDF_FAILURE,
            HDF_LOGE("%{public}s, read vectSize error", __func__));

        std::vector<IRect> rects(vectSize);
        if (ret == HDF_SUCCESS) {
            for (uint32_t i = 0; i < vectSize; i++) {
                DISPLAY_CHK_CONDITION(ret, HDF_SUCCESS, CmdUtils::RectUnpack(unpacker, rects[i]),
                    HDF_LOGE("%{public}s, read vect error, at i = %{public}d", __func__, i));
                if (ret != HDF_SUCCESS) {
                    break;
                }
            }
        }
        if (ret == HDF_SUCCESS) {
            HdfTrace traceVdi("SetLayerDirtyRegion", "HDI:DISP:HARDWARE");
            impl_->SetLayerDirtyRegion(devId, layerId, rects);
        } else {
            HDF_LOGE("%{public}s, SetLayerDirtyRegion error", __func__);
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_DIRTY_REGION, ret);
        }
        return;
    }

    void OnSetLayerVisibleRegion(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        uint32_t vectSize = 0;
        int32_t ret = HDF_SUCCESS;

        DISPLAY_CHK_CONDITION(ret, HDF_SUCCESS, CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId),
            HDF_LOGE("%{public}s, read devId error", __func__));

        DISPLAY_CHK_CONDITION(ret, HDF_SUCCESS, unpacker.ReadUint32(vectSize) ? HDF_SUCCESS : HDF_FAILURE,
            HDF_LOGE("%{public}s, read vectSize error", __func__));

        std::vector<IRect> rects(vectSize);
        if (ret == HDF_SUCCESS) {
            for (uint32_t i = 0; i < vectSize; i++) {
                DISPLAY_CHK_CONDITION(ret, HDF_SUCCESS, CmdUtils::RectUnpack(unpacker, rects[i]),
                    HDF_LOGE("%{public}s, read vect error, at i = %{public}d", __func__, i));
                if (ret != HDF_SUCCESS) {
                    break;
                }
            }
        }
        if (ret == HDF_SUCCESS) {
            HdfTrace traceVdi("SetLayerVisibleRegion", "HDI:DISP:HARDWARE");
            impl_->SetLayerVisibleRegion(devId, layerId, rects);
        } else {
            HDF_LOGE("%{public}s, SetLayerDirtyRegion error", __func__);
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_VISIBLE_REGION, ret);
        }
        return;
    }

    typedef struct LayerBufferData {
        bool isValidBuffer;
        uint32_t devId;
        uint32_t layerId;
        uint32_t seqNo;
        int32_t fence;
        BufferHandle *buffer;
    } LayerBufferData;

    int32_t UnPackLayerBufferInfo(CommandDataUnpacker& unpacker, const std::vector<HdifdInfo>& inFds,
        struct LayerBufferData *data, std::vector<uint32_t> &deletingList)
    {
        DISPLAY_CHK_RETURN(HDF_SUCCESS != CmdUtils::SetupDeviceUnpack(unpacker, data->devId, data->layerId),
            HDF_FAILURE, HDF_LOGE("%{public}s, read devId error", __func__));

        DISPLAY_CHK_RETURN(HDF_SUCCESS != CmdUtils::BufferHandleUnpack(unpacker, inFds, data->buffer), HDF_FAILURE,
            HDF_LOGE("%{public}s, read BufferHandleUnpack error", __func__));

        data->isValidBuffer = true;

        DISPLAY_CHK_RETURN(true != unpacker.ReadUint32(data->seqNo), HDF_FAILURE,
            HDF_LOGE("%{public}s, read seqNo error", __func__));

        DISPLAY_CHK_RETURN(HDF_SUCCESS != CmdUtils::FileDescriptorUnpack(unpacker, inFds, data->fence), HDF_FAILURE,
            HDF_LOGE("%{public}s, FileDescriptorUnpack error", __func__));

        // unpack deletingList
        uint32_t vectSize = 0;
        DISPLAY_CHK_RETURN(true != unpacker.ReadUint32(vectSize), HDF_FAILURE,
            HDF_LOGE("%{public}s, read vectSize error", __func__));
        if (vectSize > CmdUtils::MAX_MEMORY) {
            HDF_LOGE("%{public}s: vectSize:%{public}u is too large", __func__, vectSize);
            return HDF_FAILURE;
        }

        deletingList.resize(vectSize);
        for (uint32_t i = 0; i < vectSize; i++) {
            DISPLAY_CHK_RETURN(true != unpacker.ReadUint32(deletingList[i]), HDF_FAILURE,
                HDF_LOGE("%{public}s, read seqNo error, at i = %{public}d", __func__, i));
        }
        return HDF_SUCCESS;
    }

    int32_t SetLayerBuffer(LayerBufferData& data, std::vector<uint32_t> &deletingList,
        bool &needFreeBuffer, bool &needMoveFd, int fd)
    {
        DISPLAY_CHECK(cacheMgr_ == nullptr, return HDF_FAILURE);
        std::lock_guard<std::mutex> lock(cacheMgr_->GetCacheMgrMutex());
        DeviceCache* devCache = nullptr;
        LayerCache* layerCache = nullptr;
        devCache = cacheMgr_->DeviceCacheInstance(data.devId);
        DISPLAY_CHECK(devCache == nullptr, return HDF_FAILURE);
        layerCache = devCache->LayerCacheInstance(data.layerId);
        DISPLAY_CHECK(layerCache == nullptr, return HDF_FAILURE);

        int32_t ret = layerCache->SetLayerBuffer(data.buffer, data.seqNo, needFreeBuffer, deletingList,
            [&](const BufferHandle& handle)->int32_t {
#ifdef DISPLAY_COMSPOER_DEBUG_DUMP
            DumpLayerBuffer(data.devId, data.layerId, data.fence, handle, "layer_");
#endif
            std::string traceMsg = "";
            if (data.buffer == nullptr) {
                traceMsg = "data.buffer is nullptr! seqNo:" + std::to_string(data.seqNo);
            } else {
                traceMsg = "HDI:DISP:HARDWARE"
                           "height:" + std::to_string(data.buffer->height) +
                           " width:" + std::to_string(data.buffer->width) +
                           " data.buffer->fd:" + std::to_string(data.buffer->fd) +
                           " seqNo:" + std::to_string(data.seqNo);
            }
            traceMsg += " fd:" + std::to_string(fd);
            if (data.fence > ERROR_FENCE_COUNT || fd > ERROR_FENCE_COUNT || handle.fd > ERROR_FENCE_COUNT) {
                HDF_LOGI("SetLayerBuffer: %{public}s data.devId: %{public}d data.layerId: %{public}d, "
                    "data.buffer->fd:%{public}d, data.seqNo:%{public}d handle.fd:%{public}d, fd:%{public}d",
                    data.buffer == nullptr ? "data.buffer is nullptr!" : "",
                    data.devId, data.layerId, data.buffer == nullptr ? -1 : data.buffer->fd, data.seqNo, handle.fd, fd);
                cacheMgr_->Dump();
            }
            HdfTrace traceVdi("SetLayerBuffer", traceMsg);
            needMoveFd = true;
            int rc = impl_->SetLayerBuffer(data.devId, data.layerId, handle, fd);
            DISPLAY_CHK_RETURN(rc != HDF_SUCCESS, HDF_FAILURE, HDF_LOGE(" fail"));
            return HDF_SUCCESS;
        });
        return ret;
    }

    void OnSetLayerBuffer(CommandDataUnpacker& unpacker, const std::vector<HdifdInfo>& inFds)
    {
        DISPLAY_TRACE;

        struct LayerBufferData data;
        std::vector<uint32_t> deletingList;

        int32_t ret = UnPackLayerBufferInfo(unpacker, inFds, &data, deletingList);
        HdifdParcelable fdParcel(data.fence);
        bool needFreeBuffer = false;
        bool needMoveFd = false;

        if (ret == HDF_SUCCESS) {
            ret = SetLayerBuffer(data, deletingList, needFreeBuffer, needMoveFd, fdParcel.GetFd());
        }
#ifndef DISPLAY_COMMUNITY
        // fix fd leak
        if (data.buffer != nullptr && needFreeBuffer) {
            FreeBufferWithDelay(data.buffer);
            data.buffer = nullptr;
            data.isValidBuffer = false;
        }
        if (needMoveFd) {
            fdParcel.Move();
        }
#endif // DISPLAY_COMMUNITY
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("%{public}s, SetLayerBuffer error", __func__);
            errMaps_.emplace(REQUEST_CMD_SET_DISPLAY_CLIENT_BUFFER, ret);
        }

        return;
    }

    void OnSetLayerCompositionType(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        int32_t type;
        int32_t ret = CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = unpacker.ReadInt32(type) ? HDF_SUCCESS : HDF_FAILURE;
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        {
            HdfTrace traceVdi("SetLayerCompositionType", "HDI:DISP:HARDWARE");
            ret = impl_->SetLayerCompositionType(devId, layerId, static_cast<CompositionType>(type));
        }
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);
EXIT:
        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_COMPOSITION_TYPE, ret);
        }
        return;
    }

    void OnSetLayerBlendType(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        int32_t type;
        int32_t ret = CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = unpacker.ReadInt32(type) ? HDF_SUCCESS : HDF_FAILURE;
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        {
            HdfTrace traceVdi("SetLayerBlendType", "HDI:DISP:HARDWARE");
            ret = impl_->SetLayerBlendType(devId, layerId, static_cast<BlendType>(type));
        }
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);
EXIT:
        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_BLEND_TYPE, ret);
        }
        return;
    }

    void OnSetLayerMaskInfo(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        uint32_t maskInfo = 0;

        int32_t ret = CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = unpacker.ReadUint32(maskInfo) ? HDF_SUCCESS : HDF_FAILURE;
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        {
            HdfTrace traceVdi("SetLayerMaskInfo", "HDI:DISP:HARDWARE");
            ret = impl_->SetLayerMaskInfo(devId, layerId, static_cast<MaskInfo>(maskInfo));
        }
        DISPLAY_CHECK(ret != HDF_SUCCESS && ret != DISPLAY_NOT_SUPPORT && ret != HDF_ERR_NOT_SUPPORT, goto EXIT);
EXIT:
        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_MASK_INFO, ret);
        }
        return;
    }

    void OnSetLayerColor(CommandDataUnpacker& unpacker)
    {
        DISPLAY_TRACE;

        uint32_t devId = 0;
        uint32_t layerId = 0;
        LayerColor layerColor = {0};

        int32_t ret = CmdUtils::SetupDeviceUnpack(unpacker, devId, layerId);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        ret = CmdUtils::LayerColorUnpack(unpacker, layerColor);
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);

        {
            HdfTrace traceVdi("SetLayerColor", "HDI:DISP:HARDWARE");
            ret = impl_->SetLayerColor(devId, layerId, layerColor);
        }
        DISPLAY_CHECK(ret != HDF_SUCCESS, goto EXIT);
EXIT:
        if (ret != HDF_SUCCESS) {
            errMaps_.emplace(REQUEST_CMD_SET_LAYER_COLOR, ret);
        }
        return;
    }

    int32_t PeriodDataReset()
    {
        replyCommandCnt_ = 0;
        errMaps_.clear();

        int32_t ret = CmdUtils::StartPack(CONTROL_CMD_REPLY_BEGIN, replyPacker_);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("PackBegin failure, ret=%{public}d", ret);
        }
        return ret;
    }

    static std::string GetFileName(uint32_t devId, uint32_t layerId, const BufferHandle& buffer)
    {
        struct timeval tv;
        char nowStr[TIME_BUFFER_MAX_LEN] = {0};

        gettimeofday(&tv, nullptr);
        if (strftime(nowStr, sizeof(nowStr), "%m-%d-%H-%M-%S", localtime(&tv.tv_sec)) == 0) {
            HDF_LOGE("strftime failed");
            return "";
        };

        std::ostringstream strStream;
        const int32_t PIXEL_BYTES = 4;
        strStream << "hdi_layer_" << devId << "_" << layerId << "_" << buffer.stride / PIXEL_BYTES << "x" <<
            buffer.height << "_" << nowStr << "-" << tv.tv_usec;
        return strStream.str();
    }
#ifdef DISPLAY_COMSPOER_DEBUG_DUMP
    static void DumpLayerBuffer(uint32_t devId, uint32_t layerId, int32_t fence, const BufferHandle& buffer,
        std::string tag)
    {
        const std::string SWITCH_ON = "on";
        const uint32_t DUMP_BUFFER_SWITCH_LEN = 4;
        char dumpSwitch[DUMP_BUFFER_SWITCH_LEN] = {0};
        GetParameter("hdi.composer.dumpbuffer", "off", dumpSwitch, DUMP_BUFFER_SWITCH_LEN);

        if (SWITCH_ON.compare(dumpSwitch) != 0) {
            return;
        }

        const uint32_t FENCE_TIMEOUT = 3000;
        int32_t retCode = WaitFence(fence, FENCE_TIMEOUT);
        if (retCode != HDF_SUCCESS) {
            return;
        }

        if (g_bufferServiceImpl == nullptr) {
            g_bufferServiceImpl = IMapper::Get(true);
            DISPLAY_CHECK((g_bufferServiceImpl == nullptr), HDF_LOGE("get IMapper failed"));
        }

        std::string fileName = GetFileName(devId, layerId, buffer);
        DISPLAY_CHECK((fileName == ""), HDF_LOGE("GetFileName failed"));
        HDF_LOGI("fileName = %{public}s", fileName.c_str());

        const std::string PATH_PREFIX = "/data/local/traces/";
        std::stringstream filePath;
        filePath << PATH_PREFIX << tag << fileName;
        std::ofstream rawDataFile(filePath.str(), std::ofstream::binary);
        DISPLAY_CHECK((!rawDataFile.good()), HDF_LOGE("open file failed, %{public}s",
            std::strerror(errno)));

        sptr<NativeBuffer> hdiBuffer = new NativeBuffer();
        hdiBuffer->SetBufferHandle(const_cast<BufferHandle*>(&buffer));

        int32_t ret = 0;
        ret = g_bufferServiceImpl->Mmap(hdiBuffer);
        DISPLAY_CHECK((ret != HDF_SUCCESS), HDF_LOGE("Mmap buffer failed"));

        std::chrono::milliseconds time_before = std::chrono::duration_cast<std::chrono::milliseconds> (
            std::chrono::system_clock::now().time_since_epoch()
        );
        rawDataFile.write(static_cast<const char *>(buffer.virAddr), buffer.size);
        std::chrono::milliseconds time_after = std::chrono::duration_cast<std::chrono::milliseconds> (
            std::chrono::system_clock::now().time_since_epoch()
        );
        HDF_LOGI("wirte file take time %{public}lld", time_after.count() - time_before.count());
        rawDataFile.close();

        ret = g_bufferServiceImpl->Unmap(hdiBuffer);
        DISPLAY_CHECK((ret != HDF_SUCCESS), HDF_LOGE("Unmap buffer failed"));
    }
#endif

    static int32_t WaitFence(int32_t fence, uint32_t timeout)
    {
        int retCode = -1;
        if (fence < 0) {
            HDF_LOGE("The fence id is invalid.");
            return retCode;
        }

        struct pollfd pollfds = {0};
        pollfds.fd = fence;
        pollfds.events = POLLIN;

        do {
            retCode = poll(&pollfds, 1, timeout);
        } while (retCode == -1 && (errno == EINTR || errno == EAGAIN));

        if (retCode == 0) {
            retCode = -1;
            errno = ETIME;
        } else if (retCode > 0) {
            retCode = 0;
            if (pollfds.revents & (POLLERR | POLLNVAL)) {
                retCode = -1;
                errno = EINVAL;
            }
        }

        return retCode < 0 ? -errno : HDF_SUCCESS;
    }

    void FreeBufferWithDelay(BufferHandle *handle)
    {
        delayFreeQueue_.push(handle);
        if (delayFreeQueue_.size() >= BUFFER_QUEUE_MAX_SIZE) {
            BufferHandle *temp = delayFreeQueue_.front();
            delayFreeQueue_.pop();
            FreeBufferHandle(temp);
            temp = nullptr;
        }
    }

protected:
    VdiImpl* impl_ = nullptr;
    std::shared_ptr<DeviceCacheManager> cacheMgr_;
    std::shared_ptr<Transfer> request_;
    bool isReplyUpdated_;
    std::shared_ptr<Transfer> reply_;
    /* period data */
    uint32_t replyCommandCnt_;
    CommandDataPacker replyPacker_;
    std::unordered_map<int32_t, int32_t> errMaps_;
    /* fix fd leak */
    std::queue<BufferHandle *> delayFreeQueue_;
    std::mutex requestMutex_;
    std::mutex replyMutex_;
    std::shared_ptr<char> requestData_;
};
using HdiDisplayCmdResponser = DisplayCmdResponser<SharedMemQueue<int32_t>, DisplayComposerVdiAdapter>;
} // namespace V1_0
} // namespace Composer
} // namespace Display
} // namespace HDI
} // namespace OHOS
#endif // OHOS_HDI_DISPLAY_V1_0_DISPLAY_CMD_REQUESTER_H