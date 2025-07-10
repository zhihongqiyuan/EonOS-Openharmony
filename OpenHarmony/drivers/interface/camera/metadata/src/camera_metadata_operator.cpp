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

#include "camera_metadata_info.h"

namespace OHOS::Camera {
uint8_t *GetMetadataData(const common_metadata_header_t *metadataHeader)
{
    return CameraMetadata::GetMetadataData(metadataHeader);
}

camera_metadata_item_entry_t *GetMetadataItems(const common_metadata_header_t *metadataHeader)
{
    return CameraMetadata::GetMetadataItems(metadataHeader);
}

common_metadata_header_t *FillCameraMetadata(common_metadata_header_t *buffer, size_t memoryRequired,
                                             uint32_t itemCapacity, uint32_t dataCapacity)
{
    return CameraMetadata::FillCameraMetadata(buffer, memoryRequired, itemCapacity, dataCapacity);
}

common_metadata_header_t *AllocateCameraMetadataBuffer(uint32_t item_capacity, uint32_t data_capacity)
{
    return CameraMetadata::AllocateCameraMetadataBuffer(item_capacity, data_capacity);
}

int32_t GetMetadataSection(uint32_t itemSection, uint32_t *section)
{
    return CameraMetadata::GetMetadataSection(itemSection, section);
}

int32_t GetCameraMetadataItemType(uint32_t item, uint32_t *dataType)
{
    return CameraMetadata::GetCameraMetadataItemType(item, dataType);
}

const char *GetCameraMetadataItemName(uint32_t item)
{
    return CameraMetadata::GetCameraMetadataItemName(item);
}

int32_t CalculateCameraMetadataItemDataSize(uint32_t type, size_t dataCount)
{
    return CameraMetadata::CalculateCameraMetadataItemDataSize(type, dataCount);
}

int AddCameraMetadataItem(common_metadata_header_t *dst, uint32_t item, const void *data, size_t dataCount)
{
    return CameraMetadata::AddCameraMetadataItem(dst, item, data, dataCount);
}

int GetCameraMetadataItem(const common_metadata_header_t *src, uint32_t index, camera_metadata_item_t *item)
{
    return CameraMetadata::GetCameraMetadataItem(src, index, item);
}

int FindCameraMetadataItemIndex(const common_metadata_header_t *src, uint32_t item, uint32_t *idx)
{
    return CameraMetadata::FindCameraMetadataItemIndex(src, item, idx);
}

int FindCameraMetadataItem(const common_metadata_header_t *src, uint32_t item, camera_metadata_item_t *metadataItem)
{
    return CameraMetadata::FindCameraMetadataItem(src, item, metadataItem);
}

bool IsCameraMetadataItemExist(const common_metadata_header_t *src, uint32_t item)
{
    return CameraMetadata::IsCameraMetadataItemExist(src, item);
}

int MetadataExpandItemMem(common_metadata_header_t *dst, camera_metadata_item_entry_t *item,
    size_t oldItemSize)
{
    return CameraMetadata::MetadataExpandItemMem(dst, item, oldItemSize);
}

int UpdateCameraMetadataItemByIndex(common_metadata_header_t *dst, uint32_t index, const void *data, uint32_t dataCount,
                                    camera_metadata_item_t *updatedItem)
{
    return CameraMetadata::UpdateCameraMetadataItemByIndex(dst, index, data, dataCount, updatedItem);
}

int UpdateCameraMetadataItem(common_metadata_header_t *dst, uint32_t item, const void *data,
                             uint32_t dataCount, camera_metadata_item_t *updatedItem)
{
    return CameraMetadata::UpdateCameraMetadataItem(dst, item, data, dataCount, updatedItem);
}

int DeleteCameraMetadataItemByIndex(common_metadata_header_t *dst, uint32_t index)
{
    return CameraMetadata::DeleteCameraMetadataItemByIndex(dst, index);
}

int DeleteCameraMetadataItem(common_metadata_header_t *dst, uint32_t item)
{
    return CameraMetadata::DeleteCameraMetadataItem(dst, item);
}

void FreeCameraMetadataBuffer(common_metadata_header_t *dst)
{
    CameraMetadata::FreeCameraMetadataBuffer(dst);
}

uint32_t GetCameraMetadataItemCount(const common_metadata_header_t *metadataHeader)
{
    return CameraMetadata::GetCameraMetadataItemCount(metadataHeader);
}

uint32_t GetCameraMetadataItemCapacity(const common_metadata_header_t *metadataHeader)
{
    return CameraMetadata::GetCameraMetadataItemCapacity(metadataHeader);
}

uint32_t GetCameraMetadataDataSize(const common_metadata_header_t *metadataHeader)
{
    return CameraMetadata::GetCameraMetadataDataSize(metadataHeader);
}

int32_t CopyCameraMetadataItems(common_metadata_header_t *newMetadata, const common_metadata_header_t *oldMetadata)
{
    return CameraMetadata::CopyCameraMetadataItems(newMetadata, oldMetadata);
}

std::string MetadataItemDump(const common_metadata_header_t *metadataHeader, uint32_t item)
{
    return CameraMetadata::MetadataItemDump(metadataHeader, item);
}

std::string FormatCameraMetadataToString(const common_metadata_header_t *metadataHeader)
{
    return CameraMetadata::FormatCameraMetadataToString(metadataHeader);
}

int32_t GetAllVendorTags(std::vector<vendorTag_t>& tagVec)
{
    return CameraMetadata::GetAllVendorTags(tagVec);
}
} // Camera
