/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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

#include "fs_verity_descriptor_with_sign.h"

namespace OHOS {
namespace SignatureTools {

FsVerityDescriptorWithSign::FsVerityDescriptorWithSign()
{
}

FsVerityDescriptorWithSign::FsVerityDescriptorWithSign(FsVerityDescriptor fsVerityDescriptor,
    const std::vector<int8_t> &signature)
{
    this->fsVerityDescriptor = fsVerityDescriptor;
    if (!signature.empty()) {
        this->signature = signature;
    }
    length = FsVerityDescriptor::DESCRIPTOR_SIZE + this->signature.size();
}

FsVerityDescriptorWithSign::FsVerityDescriptorWithSign(int32_t type, int32_t length,
    FsVerityDescriptor fsVerityDescriptor, const std::vector<int8_t> &signature)
{
    this->type = type;
    this->length = length;
    this->fsVerityDescriptor = fsVerityDescriptor;
    this->signature = signature;
}

int32_t FsVerityDescriptorWithSign::Size()
{
    int tmpVariable = 2;
    return INTEGER_BYTES * tmpVariable + FsVerityDescriptor::DESCRIPTOR_SIZE + signature.size();
}

void FsVerityDescriptorWithSign::ToByteArray(std::vector<int8_t>& ret)
{
    std::unique_ptr<ByteBuffer> buffer = std::make_unique<ByteBuffer>(Size());
    buffer->PutInt32(type);
    buffer->PutInt32(length);
    std::vector<int8_t> fsArr;
    fsVerityDescriptor.ToByteArray(fsArr);
    buffer->PutData(fsArr.data(), fsArr.size());
    buffer->PutData(signature.data(), signature.size());
    buffer->Flip();
    ret = std::vector<int8_t>(buffer->GetBufferPtr(), buffer->GetBufferPtr() + buffer->GetLimit());
    return;
}

FsVerityDescriptor FsVerityDescriptorWithSign::GetFsVerityDescriptor()
{
    return fsVerityDescriptor;
}

std::vector<int8_t>& FsVerityDescriptorWithSign::GetSignature()
{
    return signature;
}
} // namespace SignatureTools
} // namespace OHOS