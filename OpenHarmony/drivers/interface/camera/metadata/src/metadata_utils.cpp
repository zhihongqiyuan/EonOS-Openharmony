/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include "metadata_utils.h"
#include <securec.h>
#include "metadata_log.h"
#include "camera_metadata_item_info.h"

#define IF_COND_PRINT_MSG_AND_RETURN(cond, msg) \
if (cond) { \
    METADATA_ERR_LOG(msg); \
    return {}; \
}

namespace OHOS::Camera {
void MetadataUtils::WriteMetadataDataToVec(const camera_metadata_item_t &entry, std::vector<uint8_t>& cameraAbility)
{
    if (entry.data_type == META_TYPE_BYTE) {
        for (size_t i = 0; i < entry.count; i++) {
            WriteData<int8_t>(*(entry.data.u8 + i), cameraAbility);
        }
    } else if (entry.data_type == META_TYPE_INT32) {
        for (size_t i = 0; i < entry.count; i++) {
            WriteData<int32_t>(*(entry.data.i32 + i), cameraAbility);
        }
    } else if (entry.data_type == META_TYPE_UINT32) {
        for (size_t i = 0; i < entry.count; i++) {
            WriteData<uint32_t>(*(entry.data.ui32 + i), cameraAbility);
        }
    } else if (entry.data_type == META_TYPE_FLOAT) {
        for (size_t i = 0; i < entry.count; i++) {
            WriteData<float>(*(entry.data.f + i), cameraAbility);
        }
    } else if (entry.data_type == META_TYPE_INT64) {
        for (size_t i = 0; i < entry.count; i++) {
            WriteData<int64_t>(*(entry.data.i64 + i), cameraAbility);
        }
    } else if (entry.data_type == META_TYPE_DOUBLE) {
        for (size_t i = 0; i < entry.count; i++) {
            WriteData<double>(*(entry.data.d + i), cameraAbility);
        }
    } else if (entry.data_type == META_TYPE_RATIONAL) {
        for (size_t i = 0; i < entry.count; i++) {
            WriteData<int32_t>((*(entry.data.r + i)).numerator, cameraAbility);
            WriteData<int32_t>((*(entry.data.r + i)).denominator, cameraAbility);
        }
    }
}

bool MetadataUtils::ConvertMetadataToVec(const std::shared_ptr<CameraMetadata> &metadata,
    std::vector<uint8_t>& cameraAbility)
{
    cameraAbility.clear();

    if (metadata == nullptr) {
        METADATA_ERR_LOG("ConvertMetadataToVec metadata is null!");
        return false;
    }

    common_metadata_header_t *meta = metadata->get();
    uint32_t tagCount = GetCameraMetadataItemCount(meta);
    if (tagCount > MAX_SUPPORTED_TAGS) {
        METADATA_ERR_LOG("ConvertMetadataToVec tagCount out of range:%{public}d", tagCount);
        return false;
    }

    uint32_t itemCapacity = GetCameraMetadataItemCapacity(meta);
    if (itemCapacity > MAX_ITEM_CAPACITY) {
        METADATA_ERR_LOG("ConvertMetadataToVec itemCapacity out of range:%{public}d", itemCapacity);
        return false;
    }

    uint32_t dataCapacity = GetCameraMetadataDataSize(meta);
    if (dataCapacity > MAX_DATA_CAPACITY) {
        METADATA_ERR_LOG("ConvertMetadataToVec dataCapacity out of range:%{public}d", dataCapacity);
        return false;
    }

    WriteData<uint32_t>(tagCount, cameraAbility);
    WriteData<uint32_t>(itemCapacity, cameraAbility);
    WriteData<uint32_t>(dataCapacity, cameraAbility);
    for (uint32_t i = 0; i < tagCount; i++) {
        camera_metadata_item_t item;
        int ret = GetCameraMetadataItem(meta, i, &item);
        if (ret != CAM_META_SUCCESS) {
            METADATA_ERR_LOG("ConvertMetadataToVec get meta item failed!");
            return false;
        }
        WriteData<uint32_t>(item.index, cameraAbility);
        WriteData<uint32_t>(item.item, cameraAbility);
        WriteData<uint32_t>(item.data_type, cameraAbility);
        WriteData<uint32_t>(item.count, cameraAbility);
        if (item.count > MAX_SUPPORTED_ITEMS) {
            METADATA_ERR_LOG("ConvertMetadataToVec item.count out of range:%{public}d", item.count);
            return false;
        }
        uint32_t dataPayloadBytes = item.count * OHOS_CAMERA_METADATA_TYPE_SIZE[item.data_type];
        if (item.count > ENTRY_DATA_SIZE &&
            ((item.data.u8 + dataPayloadBytes) > (GetMetadataData(meta) + dataCapacity))) {
            METADATA_ERR_LOG("ConvertMetadataToVec get meta item failed!");
            return false;
        }
        WriteMetadataDataToVec(item, cameraAbility);
    }
    return true;
}

bool MetadataUtils::EncodeCameraMetadata(const std::shared_ptr<CameraMetadata> &metadata,
                                         MessageParcel &data)
{
    if (metadata == nullptr) {
        return false;
    }

    bool bRet = true;
    uint32_t tagCount = 0;
    common_metadata_header_t *meta = metadata->get();
    if (meta != nullptr) {
        tagCount = GetCameraMetadataItemCount(meta);
        bRet = bRet && data.WriteUint32(tagCount);
        bRet = bRet && data.WriteUint32(GetCameraMetadataItemCapacity(meta));
        bRet = bRet && data.WriteUint32(GetCameraMetadataDataSize(meta));
        for (uint32_t i = 0; i < tagCount; i++) {
            camera_metadata_item_t item;
            int ret = GetCameraMetadataItem(meta, i, &item);
            if (ret != CAM_META_SUCCESS) {
                return false;
            }

            bRet = bRet && data.WriteUint32(item.index);
            bRet = bRet && data.WriteUint32(item.item);
            bRet = bRet && data.WriteUint32(item.data_type);
            bRet = bRet && data.WriteUint32(item.count);
            bRet = bRet && MetadataUtils::WriteMetadata(item, data);
        }
    } else {
        bRet = data.WriteUint32(tagCount);
    }
    return bRet;
}

static void ReadMetadataDataFromVecUInt8(int32_t &index, camera_metadata_item_t &entry,
    const std::vector<uint8_t>& cameraAbility)
{
    entry.data.u8 = new(std::nothrow) uint8_t[entry.count];
    if (entry.data.u8 != nullptr) {
        for (size_t i = 0; i < entry.count; i++) {
            MetadataUtils::ReadData<uint8_t>(entry.data.u8[i], index, cameraAbility);
        }
    }
}

static void ReadMetadataDataFromVecInt32(int32_t &index, camera_metadata_item_t &entry,
    const std::vector<uint8_t>& cameraAbility)
{
    entry.data.i32 = new(std::nothrow) int32_t[entry.count];
    if (entry.data.i32 != nullptr) {
        for (size_t i = 0; i < entry.count; i++) {
            MetadataUtils::ReadData<int32_t>(entry.data.i32[i], index, cameraAbility);
        }
    }
}

static void ReadMetadataDataFromVecUInt32(int32_t &index, camera_metadata_item_t &entry,
    const std::vector<uint8_t>& cameraAbility)
{
    entry.data.ui32 = new(std::nothrow) uint32_t[entry.count];
    if (entry.data.ui32 != nullptr) {
        for (size_t i = 0; i < entry.count; i++) {
            MetadataUtils::ReadData<uint32_t>(entry.data.ui32[i], index, cameraAbility);
        }
    }
}

static void ReadMetadataDataFromVecFloat(int32_t &index, camera_metadata_item_t &entry,
    const std::vector<uint8_t>& cameraAbility)
{
    entry.data.f = new(std::nothrow) float[entry.count];
    if (entry.data.f != nullptr) {
        for (size_t i = 0; i < entry.count; i++) {
            MetadataUtils::ReadData<float>(entry.data.f[i], index, cameraAbility);
        }
    }
}

static void ReadMetadataDataFromVecInt64(int32_t &index, camera_metadata_item_t &entry,
    const std::vector<uint8_t>& cameraAbility)
{
    entry.data.i64 = new(std::nothrow) int64_t[entry.count];
    if (entry.data.i64 != nullptr) {
        for (size_t i = 0; i < entry.count; i++) {
            MetadataUtils::ReadData<int64_t>(entry.data.i64[i], index, cameraAbility);
        }
    }
}

static void ReadMetadataDataFromVecDouble(int32_t &index, camera_metadata_item_t &entry,
    const std::vector<uint8_t>& cameraAbility)
{
    entry.data.d = new(std::nothrow) double[entry.count];
    if (entry.data.d != nullptr) {
        for (size_t i = 0; i < entry.count; i++) {
            MetadataUtils::ReadData<double>(entry.data.d[i], index, cameraAbility);
        }
    }
}

static void ReadMetadataDataFromVecRational(int32_t &index, camera_metadata_item_t &entry,
    const std::vector<uint8_t>& cameraAbility)
{
    entry.data.r = new(std::nothrow) camera_rational_t[entry.count];
    if (entry.data.r != nullptr) {
        for (size_t i = 0; i < entry.count; i++) {
            MetadataUtils::ReadData<int32_t>(entry.data.r[i].numerator, index, cameraAbility);
            MetadataUtils::ReadData<int32_t>(entry.data.r[i].denominator, index, cameraAbility);
        }
    }
}

void MetadataUtils::ReadMetadataDataFromVec(int32_t &index, camera_metadata_item_t &entry,
    const std::vector<uint8_t>& cameraAbility)
{
    switch (entry.data_type) {
        case META_TYPE_BYTE:
            ReadMetadataDataFromVecUInt8(index, entry, cameraAbility);
            break;
        case META_TYPE_INT32:
            ReadMetadataDataFromVecInt32(index, entry, cameraAbility);
            break;
        case META_TYPE_UINT32:
            ReadMetadataDataFromVecUInt32(index, entry, cameraAbility);
            break;
        case META_TYPE_FLOAT:
            ReadMetadataDataFromVecFloat(index, entry, cameraAbility);
            break;
        case META_TYPE_INT64:
            ReadMetadataDataFromVecInt64(index, entry, cameraAbility);
            break;
        case META_TYPE_DOUBLE:
            ReadMetadataDataFromVecDouble(index, entry, cameraAbility);
            break;
        case META_TYPE_RATIONAL:
            ReadMetadataDataFromVecRational(index, entry, cameraAbility);
            break;
    }
}

void MetadataUtils::ConvertVecToMetadata(const std::vector<uint8_t>& cameraAbility,
    std::shared_ptr<CameraMetadata> &metadata)
{
    int32_t index = 0;
    uint32_t tagCount = 0;
    uint32_t itemCapacity = 0;
    uint32_t dataCapacity = 0;

    if (cameraAbility.size() < MIN_VEC_SIZE) {
        METADATA_ERR_LOG("ConvertVecToMetadata cameraAbility size:%{public}d", static_cast<int>(cameraAbility.size()));
        return;
    }
    ReadData<uint32_t>(tagCount, index, cameraAbility);
    if (tagCount > MAX_SUPPORTED_TAGS) {
        METADATA_ERR_LOG("ConvertVecToMetadata tagCount out of range:%{public}d", tagCount);
        return;
    }
    ReadData<uint32_t>(itemCapacity, index, cameraAbility);
    if (itemCapacity > MAX_ITEM_CAPACITY) {
        METADATA_ERR_LOG("ConvertVecToMetadata itemCapacity out of range:%{public}d", itemCapacity);
        return;
    }
    ReadData<uint32_t>(dataCapacity, index, cameraAbility);
    if (dataCapacity > MAX_DATA_CAPACITY) {
        METADATA_ERR_LOG("ConvertVecToMetadata dataCapacity out of range:%{public}d", dataCapacity);
        return;
    }

    std::vector<camera_metadata_item_t> items;
    for (uint32_t i = 0; i < tagCount; i++) {
        camera_metadata_item_t item;
        ReadData<uint32_t>(item.index, index, cameraAbility);
        ReadData<uint32_t>(item.item, index, cameraAbility);
        ReadData<uint32_t>(item.data_type, index, cameraAbility);
        ReadData<uint32_t>(item.count, index, cameraAbility);
        if (item.count > MAX_SUPPORTED_ITEMS) {
            METADATA_ERR_LOG("ConvertVecToMetadata item.count out of range:%{public}d item:%{public}d",
                item.count, item.item);
            return;
        }
        ReadMetadataDataFromVec(index, item, cameraAbility);
        items.push_back(item);
    }

    metadata = std::make_shared<CameraMetadata>(itemCapacity, dataCapacity);
    common_metadata_header_t *meta = metadata->get();
    for (auto &item_ : items) {
        void *buffer = nullptr;
        MetadataUtils::ItemDataToBuffer(item_, &buffer);
        (void)AddCameraMetadataItem(meta, item_.item, buffer, item_.count);
        FreeMetadataBuffer(item_);
    }
}

void MetadataUtils::DecodeCameraMetadata(MessageParcel &data, std::shared_ptr<CameraMetadata> &metadata)
{
    uint32_t tagCount = data.ReadUint32();
    uint32_t itemCapacity = data.ReadUint32();
    uint32_t dataCapacity = data.ReadUint32();

    if (tagCount > MAX_SUPPORTED_TAGS) {
        tagCount = MAX_SUPPORTED_TAGS;
        METADATA_ERR_LOG("MetadataUtils::DecodeCameraMetadata tagCount is more than supported value");
    }

    if (itemCapacity > MAX_ITEM_CAPACITY) {
        itemCapacity = MAX_ITEM_CAPACITY;
        METADATA_ERR_LOG("MetadataUtils::DecodeCameraMetadata itemCapacity is more than supported value");
    }

    if (dataCapacity > MAX_DATA_CAPACITY) {
        dataCapacity = MAX_DATA_CAPACITY;
        METADATA_ERR_LOG("MetadataUtils::DecodeCameraMetadata dataCapacity is more than supported value");
    }

    std::vector<camera_metadata_item_t> items;
    for (uint32_t i = 0; i < tagCount; i++) {
        camera_metadata_item_t item;
        item.index = data.ReadUint32();
        item.item = data.ReadUint32();
        item.data_type = data.ReadUint32();
        item.count = data.ReadUint32();
        if (item.count > MAX_SUPPORTED_ITEMS) {
            item.count = MAX_SUPPORTED_ITEMS;
            METADATA_ERR_LOG("MetadataUtils::DecodeCameraMetadata item.count is more than supported value");
        }
        MetadataUtils::ReadMetadata(item, data);
        items.push_back(item);
    }

    metadata = std::make_shared<CameraMetadata>(itemCapacity, dataCapacity);
    common_metadata_header_t *meta = metadata->get();
    for (auto &item_ : items) {
        void *buffer = nullptr;
        MetadataUtils::ItemDataToBuffer(item_, &buffer);
        (void)AddCameraMetadataItem(meta, item_.item, buffer, item_.count);
        FreeMetadataBuffer(item_);
    }
}

bool MetadataUtils::WriteMetadata(const camera_metadata_item_t &item, MessageParcel &data)
{
    bool bRet = false;
    size_t i;
    if (item.data_type == META_TYPE_BYTE) {
        bRet = data.WriteUnpadBuffer(item.data.u8, item.count * sizeof(uint8_t));
    } else if (item.data_type == META_TYPE_INT32) {
        bRet = data.WriteUnpadBuffer(item.data.i32, item.count * sizeof(int32_t));
    } else if (item.data_type == META_TYPE_FLOAT) {
        bRet = data.WriteUnpadBuffer(item.data.f, item.count * sizeof(float));
    } else if (item.data_type == META_TYPE_UINT32) {
        bRet = data.WriteUnpadBuffer(item.data.ui32, item.count * sizeof(uint32_t));
    } else if (item.data_type == META_TYPE_INT64) {
        bRet = data.WriteUnpadBuffer(item.data.i64, item.count * sizeof(int64_t));
    } else if (item.data_type == META_TYPE_DOUBLE) {
        bRet = data.WriteUnpadBuffer(item.data.d, item.count * sizeof(double));
    } else if (item.data_type == META_TYPE_RATIONAL) {
        std::vector<int32_t> rationalBuffers;
        for (i = 0; i < item.count; i++) {
            rationalBuffers.push_back((*(item.data.r + i)).numerator);
            rationalBuffers.push_back((*(item.data.r + i)).denominator);
        }
        bRet = data.WriteInt32Vector(rationalBuffers);
    }

    return bRet;
}

std::string MetadataUtils::EncodeToString(std::shared_ptr<CameraMetadata> metadata)
{
    int32_t ret;
    const int32_t headerLength = sizeof(common_metadata_header_t);
    const int32_t itemLen = sizeof(camera_metadata_item_entry_t);
    const int32_t itemFixedLen = static_cast<int32_t>(offsetof(camera_metadata_item_entry_t, data));

    if (metadata == nullptr || metadata->get() == nullptr) {
        METADATA_ERR_LOG("MetadataUtils::EncodeToString Metadata is invalid");
        return {};
    }

    common_metadata_header_t *meta = metadata->get();
    int32_t encodeDataLen = headerLength + (itemLen * meta->item_count) + meta->data_count;
    std::string s(encodeDataLen, '\0');
    char *encodeData = &s[0];
    ret = memcpy_s(encodeData, encodeDataLen, meta, headerLength);
    if (ret != EOK) {
        METADATA_ERR_LOG("MetadataUtils::EncodeToString Failed to copy memory for metadata header");
        return {};
    }
    encodeData += headerLength;
    encodeDataLen -= headerLength;
    camera_metadata_item_entry_t *item = GetMetadataItems(meta);
    for (uint32_t index = 0; index < meta->item_count; index++, item++) {
        ret = memcpy_s(encodeData, encodeDataLen, item, itemFixedLen);
        METADATA_CHECK_ERROR_RETURN_RET_LOG(
            ret != EOK, {}, "MetadataUtils::EncodeToString Failed to copy memory for item fixed fields");
        encodeData += itemFixedLen;
        encodeDataLen -= itemFixedLen;
        int32_t dataLen = itemLen - itemFixedLen;
        METADATA_CHECK_ERROR_RETURN_RET_LOG(
            item == nullptr, {}, "MetadataUtils::EncodeToString Failed, item is nullptr");
        ret = memcpy_s(encodeData, encodeDataLen,  &(item->data), dataLen);
        if (ret != EOK) {
            METADATA_ERR_LOG("MetadataUtils::EncodeToString Failed to copy memory for item data field");
            return {};
        }
        encodeData += dataLen;
        encodeDataLen -= dataLen;
    }
            
    if (meta->data_count != 0) {
        ret = copyEncodeToStringMem(meta, encodeData, encodeDataLen);
        if (ret != CAM_META_SUCCESS) {
            return {};
        }
        encodeData += meta->data_count;
    }
    METADATA_DEBUG_LOG("MetadataUtils::EncodeToString Calculated length: %{public}zu, encoded length: %{public}zu",
                       s.capacity(), (encodeData - &s[0]));

    return s;
}

int MetadataUtils::copyEncodeToStringMem(common_metadata_header_t *meta, char *encodeData, int32_t encodeDataLen)
{
    uint8_t *metaMetadataData = GetMetadataData(meta);
    int32_t ret = CAM_META_SUCCESS;
    if (metaMetadataData == nullptr) {
        METADATA_ERR_LOG("MetadataUtils::EncodeToString GetMetadataData failed");
        return CAM_META_FAILURE;
    }
    ret = memcpy_s(encodeData, encodeDataLen, metaMetadataData, meta->data_count);
    if (ret != EOK) {
        METADATA_ERR_LOG("MetadataUtils::EncodeToString Failed to copy memory for data");
        return CAM_META_FAILURE;
    }
    return CAM_META_SUCCESS;
}

std::shared_ptr<CameraMetadata> MetadataUtils::DecodeFromString(std::string setting)
{
    int32_t ret;
    uint32_t totalLen = setting.capacity();
    const uint32_t headerLength = sizeof(common_metadata_header_t);
    const uint32_t itemLen = sizeof(camera_metadata_item_entry_t);
    const uint32_t itemFixedLen = offsetof(camera_metadata_item_entry_t, data);

    IF_COND_PRINT_MSG_AND_RETURN(totalLen < headerLength,
        "MetadataUtils::DecodeFromString Length is less than metadata header length")

    char *decodeData = &setting[0];
    common_metadata_header_t header;
    ret = memcpy_s(&header, headerLength, decodeData, headerLength);

    IF_COND_PRINT_MSG_AND_RETURN(ret != EOK,
        "MetadataUtils::DecodeFromString Failed to copy memory for metadata header")
    std::shared_ptr<CameraMetadata> metadata
        = std::make_shared<CameraMetadata>(header.item_capacity, header.data_capacity);
    common_metadata_header_t *meta = metadata->get();

    IF_COND_PRINT_MSG_AND_RETURN(!meta,
        "MetadataUtils::DecodeFromString Failed to get metadata header")
    uint32_t actualMemSize = meta->size;
    ret = memcpy_s(meta, headerLength, &header, headerLength);

    IF_COND_PRINT_MSG_AND_RETURN(ret != EOK,
        "MetadataUtils::DecodeFromString Failed to copy memory for metadata header")
    bool isItemsStartInvalid = meta->items_start >= actualMemSize || meta->items_start < headerLength;
    bool isDataStartInvalid = meta->data_start >= actualMemSize || meta->data_start < headerLength;
    bool isMetadataCountInvaild = (actualMemSize - meta->items_start) < (uint64_t)meta->item_count * itemLen ||
        (actualMemSize - meta->data_start) < meta->data_count;
    bool isFuzzErrData = (actualMemSize - meta->items_start) < (uint64_t)meta->item_capacity * itemLen;
    IF_COND_PRINT_MSG_AND_RETURN(isItemsStartInvalid || isDataStartInvalid || isMetadataCountInvaild || isFuzzErrData,
        "MetadataUtils::DecodeFromString invalid item_start")
    decodeData += headerLength;
    camera_metadata_item_entry_t *item = GetMetadataItems(meta);
    for (uint32_t index = 0; index < meta->item_count; index++, item++) {
        METADATA_CHECK_ERROR_RETURN_RET_LOG(totalLen < ((decodeData - &setting[0]) + itemLen), {},
            "MetadataUtils::DecodeFromString Failed at item index: %{public}u", index);
        ret = memcpy_s(item, itemFixedLen, decodeData, itemFixedLen);

        IF_COND_PRINT_MSG_AND_RETURN(ret != EOK,
            "MetadataUtils::DecodeFromString Failed to copy memory for item fixed fields")
        decodeData += itemFixedLen;
        uint32_t dataLen = itemLen - itemFixedLen;
        METADATA_CHECK_ERROR_RETURN_RET_LOG(
            item == nullptr, {}, "MetadataUtils::DecodeFromString Failed, item is nullptr");
        ret = memcpy_s(&(item->data), dataLen, decodeData, dataLen);
            if (item->data_type >= META_NUM_TYPES ||
                totalLen < (uint64_t)(item->count * OHOS_CAMERA_METADATA_TYPE_SIZE[item->data_type])) {
                METADATA_ERR_LOG("MetadataUtils::DecodeFromString Failed at item index: %{public}u, "
                    "totalLen :%{public}u, data type :%{public}u, count:%{public}u",
                    index, totalLen, item->data_type, item->count);
                return {};
            }
        IF_COND_PRINT_MSG_AND_RETURN(ret != EOK,
            "MetadataUtils::DecodeFromString Failed to copy memory for item data field")
        decodeData += dataLen;
    }

    char *decodeMetadataData = &setting[0];
    ret = copyDecodeFromStringMem(meta, decodeData, decodeMetadataData, totalLen);
    if (ret != CAM_META_SUCCESS) {
        return {};
    }

    METADATA_DEBUG_LOG("MetadataUtils::DecodeFromString String length: %{public}zu, Decoded length: %{public}zu",
                       setting.capacity(), (decodeData - &setting[0]));
    return metadata;
}

int MetadataUtils::copyDecodeFromStringMem(common_metadata_header_t *meta, char *decodeData,
    char *decodeMetadataData, uint32_t totalLen)
{
    if (meta->data_count != 0) {
        IF_COND_PRINT_MSG_AND_RETURN(
            totalLen < static_cast<uint32_t>(((decodeData - decodeMetadataData) + meta->data_count)),
            "MetadataUtils::DecodeFromString Failed at data copy")
        uint8_t *metaMetadataData = GetMetadataData(meta);
        int32_t ret;
        if (metaMetadataData == nullptr) {
            METADATA_ERR_LOG("MetadataUtils::DecodeFromString GetMetadataData failed");
            return CAM_META_FAILURE;
        }
        ret = memcpy_s(metaMetadataData, meta->data_count, decodeData, meta->data_count);

        IF_COND_PRINT_MSG_AND_RETURN(ret != EOK,
            "MetadataUtils::DecodeFromString Failed to copy memory for item data field")
        decodeData += meta->data_count;
        return CAM_META_SUCCESS;
    }
    return CAM_META_SUCCESS;
}

static void ReadMetadataUInt8(camera_metadata_item_t &entry, MessageParcel &data)
{
    entry.data.u8 = new(std::nothrow) uint8_t[entry.count];
    if (entry.data.u8 != nullptr) {
        size_t dataSize = entry.count * sizeof(uint8_t);
        const uint8_t* ptr = data.ReadUnpadBuffer(dataSize);
        memcpy_s(entry.data.u8, dataSize, ptr, dataSize);
    }
}

static void ReadMetadataInt32(camera_metadata_item_t &entry, MessageParcel &data)
{
    entry.data.i32 = new(std::nothrow) int32_t[entry.count];
    if (entry.data.i32 != nullptr) {
        size_t dataSize = entry.count * sizeof(int32_t);
        const uint8_t* ptr = data.ReadUnpadBuffer(dataSize);
        memcpy_s(entry.data.i32, dataSize, ptr, dataSize);
    }
}

static void ReadMetadataUInt32(camera_metadata_item_t &entry, MessageParcel &data)
{
    entry.data.ui32 = new(std::nothrow) uint32_t[entry.count];
    if (entry.data.ui32 != nullptr) {
        size_t dataSize = entry.count * sizeof(uint32_t);
        const uint8_t* ptr = data.ReadUnpadBuffer(dataSize);
        memcpy_s(entry.data.ui32, dataSize, ptr, dataSize);
    }
}

static void ReadMetadataFloat(camera_metadata_item_t &entry, MessageParcel &data)
{
    entry.data.f = new(std::nothrow) float[entry.count];
    if (entry.data.f != nullptr) {
        size_t dataSize = entry.count * sizeof(float);
        const uint8_t* ptr = data.ReadUnpadBuffer(dataSize);
        memcpy_s(entry.data.f, dataSize, ptr, dataSize);
    }
}

static void ReadMetadataInt64(camera_metadata_item_t &entry, MessageParcel &data)
{
    entry.data.i64 = new(std::nothrow) int64_t[entry.count];
    if (entry.data.i64 != nullptr) {
        size_t dataSize = entry.count * sizeof(int64_t);
        const uint8_t* ptr = data.ReadUnpadBuffer(dataSize);
        memcpy_s(entry.data.i64, dataSize, ptr, dataSize);
    }
}

static void ReadMetadataDouble(camera_metadata_item_t &entry, MessageParcel &data)
{
    entry.data.d = new(std::nothrow) double[entry.count];
    if (entry.data.d != nullptr) {
        size_t dataSize = entry.count * sizeof(double);
        const uint8_t* ptr = data.ReadUnpadBuffer(dataSize);
        memcpy_s(entry.data.d, dataSize, ptr, dataSize);
    }
}

static void ReadMetadataRational(camera_metadata_item_t &entry, MessageParcel &data)
{
    std::vector<int32_t> buffers;
    data.ReadInt32Vector(&buffers);
    if (buffers.size() < 1) {
        METADATA_ERR_LOG("MetadataUtils::ReadMetadataRational the buffers size < 1");
        entry.data.r = nullptr;
        return;
    }
    entry.data.r = new(std::nothrow) camera_rational_t[entry.count];
    if (entry.data.r != nullptr) {
        for (size_t i = 0, j = 0;
            i < entry.count && j < static_cast<size_t>(buffers.size() - 1);
            i++, j += 2) { // 2:Take two elements from the buffer vector container
            entry.data.r[i].numerator = buffers.at(j);
            entry.data.r[i].denominator = buffers.at(j + 1);
        }
    }
}
bool MetadataUtils::ReadMetadata(camera_metadata_item_t &entry, MessageParcel &data)
{
    if (entry.count > MAX_SUPPORTED_ITEMS) {
        entry.count = MAX_SUPPORTED_ITEMS;
        METADATA_ERR_LOG("MetadataUtils::ReadMetadata item.count is more than supported value");
    }
    switch (entry.data_type) {
        case META_TYPE_BYTE:
            ReadMetadataUInt8(entry, data);
            break;
        case META_TYPE_INT32:
            ReadMetadataInt32(entry, data);
            break;
        case META_TYPE_UINT32:
            ReadMetadataUInt32(entry, data);
            break;
        case META_TYPE_FLOAT:
            ReadMetadataFloat(entry, data);
            break;
        case META_TYPE_INT64:
            ReadMetadataInt64(entry, data);
            break;
        case META_TYPE_DOUBLE:
            ReadMetadataDouble(entry, data);
            break;
        case META_TYPE_RATIONAL:
            ReadMetadataRational(entry, data);
            break;
    }
    return true;
}

void MetadataUtils::ItemDataToBuffer(const camera_metadata_item_t &item, void **buffer)
{
    if (buffer == nullptr) {
        METADATA_ERR_LOG("MetadataUtils::ItemDataToBuffer buffer is null");
        return;
    }
    if (item.data_type == META_TYPE_BYTE) {
        *buffer = reinterpret_cast<void *>(item.data.u8);
    } else if (item.data_type == META_TYPE_INT32) {
        *buffer = reinterpret_cast<void *>(item.data.i32);
    } else if (item.data_type == META_TYPE_FLOAT) {
        *buffer = reinterpret_cast<void *>(item.data.f);
    } else if (item.data_type == META_TYPE_UINT32) {
        *buffer = reinterpret_cast<void *>(item.data.ui32);
    } else if (item.data_type == META_TYPE_INT64) {
        *buffer = reinterpret_cast<void *>(item.data.i64);
    } else if (item.data_type == META_TYPE_DOUBLE) {
        *buffer = reinterpret_cast<void *>(item.data.d);
    } else if (item.data_type == META_TYPE_RATIONAL) {
        *buffer = reinterpret_cast<void *>(item.data.r);
    }
}

void MetadataUtils::FreeMetadataBuffer(camera_metadata_item_t &entry)
{
    if (entry.data_type == META_TYPE_BYTE) {
        if (entry.data.u8 != nullptr) {
            delete[] entry.data.u8;
        }
    } else if (entry.data_type == META_TYPE_INT32) {
        if (entry.data.i32 != nullptr) {
            delete[] entry.data.i32;
        }
    } else if (entry.data_type == META_TYPE_FLOAT) {
        if (entry.data.f != nullptr) {
            delete[] entry.data.f;
        }
    } else if (entry.data_type == META_TYPE_INT64) {
        if (entry.data.i64 != nullptr) {
            delete[] entry.data.i64;
        }
    } else if (entry.data_type == META_TYPE_UINT32) {
        if (entry.data.ui32 != nullptr) {
            delete[] entry.data.ui32;
        }
    } else if (entry.data_type == META_TYPE_DOUBLE) {
        if (entry.data.d != nullptr) {
            delete[] entry.data.d;
        }
    } else if (entry.data_type == META_TYPE_RATIONAL) {
        if (entry.data.r != nullptr) {
            delete[] entry.data.r;
        }
    }
}
} // Camera
