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

#include "block_head.h"
#include "byte_buffer.h"
#include "sign_head.h"

namespace OHOS {
namespace SignatureTools {

int BlockHead::GetBlockLen()
{
    return BLOCK_LEN;
}

int BlockHead::GetElfBlockLen()
{
    return ELF_BLOCK_LEN;
}

std::string BlockHead::GetBlockHead(const char type, const char tag, const short length, const int offset)
{
    std::vector<int8_t> tmpVec;
    tmpVec.push_back(type);
    tmpVec.push_back(tag);
    tmpVec.push_back((length >> BIT_SIZE) & 0xff);
    tmpVec.push_back(length & 0xff);
    tmpVec.push_back((offset >> TRIPLE_BIT_SIZE) & 0xff);
    tmpVec.push_back((offset >> DOUBLE_BIT_SIZE) & 0xff);
    tmpVec.push_back((offset >> BIT_SIZE) & 0xff);
    tmpVec.push_back(offset & 0xff);

    return std::string(tmpVec.begin(), tmpVec.end());
}

std::vector<int8_t> BlockHead::GetBlockHeadLittleEndian(const char type, const char tag,
    const int length, const int offset)
{
    ByteBuffer bf = ByteBuffer(BlockHead::ELF_BLOCK_LEN);
    bf.PutByte(type);
    bf.PutByte(tag);
    bf.PutByte(0);
    bf.PutByte(0);
    bf.PutInt32(length);
    bf.PutInt32(offset);
    int8_t ret[BlockHead::ELF_BLOCK_LEN] = {0};
    bf.GetData(0, ret, BlockHead::ELF_BLOCK_LEN);
    std::vector<int8_t> byte(ret, ret + BlockHead::ELF_BLOCK_LEN);
    return byte;
}

} // namespace SignatureTools
} // namespace OHOS
