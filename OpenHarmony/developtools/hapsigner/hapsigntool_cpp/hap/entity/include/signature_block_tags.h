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

#ifndef SIGNATRUETOOLS_SIGNATURE_BLOCK_TAGS_H
#define SIGNATRUETOOLS_SIGNATURE_BLOCK_TAGS_H

namespace OHOS {
namespace SignatureTools {
class SignatureBlockTags {
public:
    static const char DEFAULT = 0;
    static const char HASH_ALL = 1;
    static const char HASH_ROOT_1M = 0x80;
    static const char HASH_ROOT_512K = 0x81;
    static const char HASH_ROOT_256K = 0x82;
    static const char HASH_ROOT_128K = 0x83;
    static const char HASH_ROOT_64K = 0x84;
    static const char HASH_ROOT_32K = 0x85;
    static const char HASH_ROOT_16K = 0x86;
    static const char HASH_ROOT_8K = 0x87;
    static const char HASH_ROOT_4K = 0x88;
    static const char HASH_BLOCK_1M = 0x90;
    static const char HASH_BLOCK_512K = 0x91;
    static const char HASH_BLOCK_256K = 0x92;
    static const char HASH_BLOCK_128K = 0x93;
    static const char HASH_BLOCK_64K = 0x94;
    static const char HASH_BLOCK_32K = 0x95;
    static const char HASH_BLOCK_16K = 0x96;
    static const char HASH_BLOCK_8K = 0x97;
    static const char HASH_BLOCK_4K = 0x98;
};
} // namespace SignatureTools
} // namespace OHOS
#endif