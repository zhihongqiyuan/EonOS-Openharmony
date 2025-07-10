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
#ifndef SIGNATURETOOLS_MERKLE_TREE_BUILDER_H
#define SIGNATURETOOLS_MERKLE_TREE_BUILDER_H

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>

#include "thread_pool.h"
#include "byte_buffer.h"
#include "merkle_tree.h"
#include "fs_digest_utils.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
class MerkleTreeBuilder {
public:
    MerkleTreeBuilder();
    MerkleTree* GenerateMerkleTree(std::istream& inputStream, long size,
                                   const FsVerityHashAlgorithm& fsVerityHashAlgorithm);

private:
    static const int FSVERITY_HASH_PAGE_SIZE;
    static const int64_t INPUTSTREAM_MAX_SIZE;
    static const int CHUNK_SIZE;
    static const long MAX_READ_SIZE;
    static const int MAX_PROCESSORS;
    static const int BLOCKINGQUEUE;
    static ByteBuffer* Slice(ByteBuffer* buffer, int begin, int end);
    static std::vector<int64_t> GetOffsetArrays(long dataSize, int digestSize);
    static std::vector<long> GetLevelSize(long dataSize, int digestSize);
    static long GetChunkCount(long dataSize, long divisor);
    static long GetFullChunkSize(long dataSize, long divisor, long multiplier);
    void SetAlgorithm(const std::string& algorithm);
    void TransInputStreamToHashData(std::istream& inputStream, long size,
                                    ByteBuffer* outputBuffer, int bufStartIdx);
    std::vector<std::vector<int8_t>> GetDataHashes(std::istream& inputStream, long size);

    void RunHashTask(std::vector<std::vector<int8_t>>& hashes, ByteBuffer* buffer,
                     int readChunkIndex, int bufStartIdx);
    void TransInputDataToHashData(ByteBuffer* inputBuffer, ByteBuffer* outputBuffer,
                                  int64_t bufStartIdx, int64_t outputStartIdx);
    void GenerateHashDataByInputData(std::istream& inputStream, long size, ByteBuffer* outputBuffer,
                                     std::vector<int64_t>& offsetArrays, int digestSize);
    void GenerateHashDataByHashData(ByteBuffer* buffer, std::vector<int64_t>& offsetArrays, int digestSize);
    MerkleTree* GetMerkleTree(ByteBuffer* dataBuffer, long inputDataSize,
                              FsVerityHashAlgorithm fsVerityHashAlgorithm);
    void DataRoundupChunkSize(ByteBuffer* data, long originalDataSize, int digestSize);
    bool CheckCalculateHashResult;
    const int POOL_SIZE = std::min(MAX_PROCESSORS, static_cast<int>(std::thread::hardware_concurrency()));
    std::string mAlgorithm = "SHA-256";
    std::shared_ptr<Uscript::ThreadPool> mPools;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_MERKLE_TREE_BUILDER_H