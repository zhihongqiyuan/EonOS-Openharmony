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
#ifndef SIGNATRUETOOLS_CODE_SIGN_BLOCK_HEADER_H
#define SIGNATRUETOOLS_CODE_SIGN_BLOCK_HEADER_H

#include <vector>
#include <string>

#include "byte_buffer.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {

class CodeSignBlockHeader {
public:
    class Builder {
    public:
        virtual CodeSignBlockHeader* Build();
        virtual ~Builder();
        
        virtual Builder* SetMagic(int64_t magic);
        virtual Builder* SetVersion(int version);
        virtual Builder* SetBlockSize(int blockSize);
        virtual Builder* SetSegmentNum(int segmentNum);
        virtual Builder* SetFlags(int flags);
        virtual Builder* SetReserved(const std::vector<int8_t>& reserved);

        int64_t magic = MAGIC_NUM;
        int version = CODE_SIGNING_VERSION;
        int blockSize = 0;
        int segmentNum = 0;
        int flags = 0;
        std::vector<int8_t> reserved = std::vector<int8_t>(RESERVED_BYTE_ARRAY_LENGTH);
    };

public:
    static const int FLAG_MERKLE_TREE_INLINED = 0x1;
    static const int FLAG_NATIVE_LIB_INCLUDED = 0x2;
    CodeSignBlockHeader();
    CodeSignBlockHeader(Builder* builder);
    virtual ~CodeSignBlockHeader();
    static CodeSignBlockHeader* FromByteArray(const std::vector<int8_t>& bytes);
    static int Size();
    virtual void SetSegmentNum(int num);
    virtual int GetSegmentNum();
    virtual void SetBlockSize(int64_t size);
    virtual int GetBlockSize();
    virtual void SetFlags(int flags);
    virtual void ToByteArray(std::vector<int8_t>& ret);

private:
    static const signed int  MAGIC_BYTE_LENGTH = 4;
    static constexpr int CODE_SIGNING_VERSION = 1;
    // byte size of magic number
    static const int8_t MAGIC_BYTE_ARRAY_LENGTH = 8;
    // lower 8 bytes of MD5 result of string "hap code sign block" (E046 C8C6 5389 FCCD)
    static const int64_t MAGIC_NUM = ((0xE046C8C6LL << 32) + 0x5389FCCDLL);
    // size of byte[8] reserved
    static const int8_t RESERVED_BYTE_ARRAY_LENGTH = 8;
    // At all times three segment are always included in code sign block
    // update this if new segments are created.
    static const int SEGMENT_NUM = 3;
    int64_t magic = 0;
    int version = 0;
    int blockSize = 0;
    int segmentNum = 0;
    // FLAG_MERKLE_TREE_INLINED + FLAG_NATIVE_LIB_INCLUDED
    int flags = 0;
    std::vector<int8_t> reserved;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_CODE_SIGN_BLOCK_HEADER_H
