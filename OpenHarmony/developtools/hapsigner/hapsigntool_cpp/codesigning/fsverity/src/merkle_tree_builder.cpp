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
#include <cmath>

#include "merkle_tree_builder.h"

using namespace OHOS::SignatureTools::Uscript;
namespace OHOS {
namespace SignatureTools {
    
const int MerkleTreeBuilder::FSVERITY_HASH_PAGE_SIZE = 4096;
const int64_t MerkleTreeBuilder::INPUTSTREAM_MAX_SIZE = 4503599627370496L;
const int MerkleTreeBuilder::CHUNK_SIZE = 4096;
const long MerkleTreeBuilder::MAX_READ_SIZE = 4194304L;
const int MerkleTreeBuilder::MAX_PROCESSORS = 32;
const int MerkleTreeBuilder::BLOCKINGQUEUE = 4;

void MerkleTreeBuilder::SetAlgorithm(const std::string& algorithm)
{
    mAlgorithm = algorithm;
}

void MerkleTreeBuilder::TransInputStreamToHashData(std::istream& inputStream,
                                                   long size, ByteBuffer* outputBuffer, int bufStartIdx)
{
    if (size == 0 || static_cast<int64_t>(size) > INPUTSTREAM_MAX_SIZE) {
        SIGNATURE_TOOLS_LOGE("invalid input stream size");
        CheckCalculateHashResult = false;
        return;
    }
    std::vector<std::vector<int8_t>> hashes = GetDataHashes(inputStream, size);
    int32_t writeSize = 0;
    for (const auto& hash : hashes) {
        outputBuffer->PutData(writeSize + bufStartIdx, hash.data(), hash.size());
        writeSize += hash.size();
    }
    outputBuffer->SetLimit(outputBuffer->GetCapacity() - bufStartIdx);
    outputBuffer->SetCapacity(outputBuffer->GetCapacity() - bufStartIdx);
    outputBuffer->SetPosition(writeSize);
}

std::vector<std::vector<int8_t>> MerkleTreeBuilder::GetDataHashes(std::istream& inputStream, long size)
{
    int count = (int)(GetChunkCount(size, MAX_READ_SIZE));
    int chunks = (int)(GetChunkCount(size, CHUNK_SIZE));
    std::vector<std::vector<int8_t>> hashes(chunks);
    std::vector<std::future<void>> thread_results;
    long readOffset = 0L;
    for (int i = 0; i < count; i++) {
        long readLimit = std::min(readOffset + MAX_READ_SIZE, size);
        int readSize = (int)((readLimit - readOffset));
        int fullChunkSize = (int)(GetFullChunkSize(readSize, CHUNK_SIZE, CHUNK_SIZE));
        ByteBuffer* byteBuffer(new ByteBuffer(fullChunkSize));
        std::vector<char> buffer(CHUNK_SIZE);
        int num = 0;
        int offset = 0;
        int flag = 0;
        int len = CHUNK_SIZE;
        while (num > 0 || flag == 0) {
            inputStream.read((buffer.data()), len);
            if (inputStream.fail() && !inputStream.eof()) {
                PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "Error occurred while reading data");
                CheckCalculateHashResult = false;
                return std::vector<std::vector<int8_t>>();
            }
            num = inputStream.gcount();
            byteBuffer->PutData(buffer.data(), num);
            offset += num;
            len = std::min(CHUNK_SIZE, readSize - offset);
            if (len <= 0 || offset == readSize) {
                break;
            }
            flag = 1;
        }
        if (offset != readSize) {
            PrintErrorNumberMsg("READ_FILE_ERROR", IO_ERROR, "Error reading buffer from input");
            CheckCalculateHashResult = false;
            return std::vector<std::vector<int8_t>>();
        }
        byteBuffer->Flip();
        int readChunkIndex = (int)(GetFullChunkSize(MAX_READ_SIZE, CHUNK_SIZE, i));
        thread_results.push_back(mPools->Enqueue(&MerkleTreeBuilder::RunHashTask, this, std::ref(hashes),
                                                 byteBuffer, readChunkIndex, 0));
        readOffset += readSize;
    }
    for (auto& thread_result : thread_results) {
        thread_result.wait();
    }
    return hashes;
}

ByteBuffer* MerkleTreeBuilder::Slice(ByteBuffer* buffer, int begin, int end)
{
    ByteBuffer* tmpBuffer = buffer->Duplicate();
    tmpBuffer->SetPosition(0);
    tmpBuffer->SetLimit(end);
    tmpBuffer->SetPosition(begin);
    return &tmpBuffer->slice_for_codesigning();
}

std::vector<int64_t> MerkleTreeBuilder::GetOffsetArrays(long dataSize, int digestSize)
{
    std::vector<long> levelSize = GetLevelSize(dataSize, digestSize);
    std::vector<int64_t> levelOffset(levelSize.size() + 1);
    levelOffset[0] = 0;
    for (int i = 0; i < levelSize.size(); i++) {
        levelOffset[i + 1] = levelOffset[i] + levelSize[levelSize.size() - i - 1];
    }
    return levelOffset;
}

std::vector<long> MerkleTreeBuilder::GetLevelSize(long dataSize, int digestSize)
{
    std::vector<long> levelSize;

    long fullChunkSize = 0L;
    long originalDataSize = dataSize;
    do {
        fullChunkSize = GetFullChunkSize(originalDataSize, CHUNK_SIZE, digestSize);
        int size = GetFullChunkSize(fullChunkSize, CHUNK_SIZE, CHUNK_SIZE);
        levelSize.push_back(size);
        originalDataSize = fullChunkSize;
    } while (fullChunkSize > CHUNK_SIZE);
    return levelSize;
}

void MerkleTreeBuilder::RunHashTask(std::vector<std::vector<int8_t>>& hashes,
                                    ByteBuffer* buffer, int readChunkIndex, int bufStartIdx)
{
    int offset = 0;

    std::shared_ptr<ByteBuffer> bufferPtr(buffer);
    int bufferSize = bufferPtr->GetCapacity();
    int index = readChunkIndex;
    while (offset < bufferSize) {
        ByteBuffer* chunk = Slice(bufferPtr.get(), offset, offset + CHUNK_SIZE);
        std::vector<int8_t> tmpByte(CHUNK_SIZE);
        chunk->GetData(offset + bufStartIdx, tmpByte.data(), CHUNK_SIZE);
        DigestUtils digestUtils(HASH_SHA256);
        std::string tmpByteStr(tmpByte.begin(), tmpByte.end());
        digestUtils.AddData(tmpByteStr);
        std::string result = digestUtils.Result(DigestUtils::Type::BINARY);
        std::vector<int8_t> hashEle;
        for (int i = 0; i < result.size(); i++) {
            hashEle.push_back(result[i]);
        }
        hashes[index++] = hashEle;
        offset += CHUNK_SIZE;
        delete chunk;
    }
}

void MerkleTreeBuilder::TransInputDataToHashData(ByteBuffer* inputBuffer, ByteBuffer* outputBuffer,
                                                 int64_t inputStartIdx, int64_t outputStartIdx)
{
    long size = inputBuffer->GetCapacity();
    int chunks = (int)GetChunkCount(size, CHUNK_SIZE);
    std::vector<std::vector<int8_t>> hashes(chunks);
    long readOffset = 0L;
    int startChunkIndex = 0;
    while (readOffset < size) {
        long readLimit = std::min(readOffset + MAX_READ_SIZE, size);
        ByteBuffer* buffer = Slice(inputBuffer, (int)readOffset, (int)readLimit);
        buffer->SetPosition(0);
        int readChunkIndex = startChunkIndex;
        RunHashTask(hashes, buffer, readChunkIndex, inputStartIdx);
        int readSize = (int)(readLimit - readOffset);
        startChunkIndex += (int)GetChunkCount(readSize, CHUNK_SIZE);
        readOffset += readSize;
        inputStartIdx += readSize;
    }
    int32_t writeSize = 0;
    for (const auto& hash : hashes) {
        outputBuffer->PutData(writeSize + outputStartIdx, hash.data(), hash.size());
        writeSize += hash.size();
    }
}

OHOS::SignatureTools::MerkleTreeBuilder::MerkleTreeBuilder() :mPools(new Uscript::ThreadPool(POOL_SIZE))
{
    CheckCalculateHashResult = true;
}

MerkleTree* MerkleTreeBuilder::GenerateMerkleTree(std::istream& inputStream, long size,
                                                  const FsVerityHashAlgorithm& fsVerityHashAlgorithm)
{
    SetAlgorithm(fsVerityHashAlgorithm.GetHashAlgorithm());
    int digestSize = fsVerityHashAlgorithm.GetOutputByteSize();
    std::vector<int64_t> offsetArrays = GetOffsetArrays(size, digestSize);
    std::unique_ptr<ByteBuffer> allHashBuffer = std::make_unique<ByteBuffer>
        (ByteBuffer(offsetArrays[offsetArrays.size() - 1]));
    GenerateHashDataByInputData(inputStream, size, allHashBuffer.get(), offsetArrays, digestSize);
    GenerateHashDataByHashData(allHashBuffer.get(), offsetArrays, digestSize);

    if (CheckCalculateHashResult) {
        return GetMerkleTree(allHashBuffer.get(), size, fsVerityHashAlgorithm);
    }
    return nullptr;
}

void MerkleTreeBuilder::GenerateHashDataByInputData(std::istream& inputStream, long size, ByteBuffer* outputBuffer,
                                                    std::vector<int64_t>& offsetArrays, int digestSize)
{
    int64_t inputDataOffsetBegin = offsetArrays[offsetArrays.size() - 2];
    int64_t inputDataOffsetEnd = offsetArrays[offsetArrays.size() - 1];
    ByteBuffer* hashBuffer = Slice(outputBuffer, 0, inputDataOffsetEnd);
    TransInputStreamToHashData(inputStream, size, hashBuffer, inputDataOffsetBegin);
    DataRoundupChunkSize(hashBuffer, size, digestSize);
    delete hashBuffer;
}

void MerkleTreeBuilder::GenerateHashDataByHashData(ByteBuffer* buffer,
                                                   std::vector<int64_t>& offsetArrays, int digestSize)
{
    for (int i = offsetArrays.size() - 3; i >= 0; i--) {
        int64_t generateOffset = offsetArrays[i];
        int64_t originalOffset = offsetArrays[i + 1];
        ByteBuffer* generateHashBuffer = Slice(buffer, offsetArrays[i], offsetArrays[i + 1] + offsetArrays[i]);
        ByteBuffer* readOnlyBuffer = buffer->Duplicate();
        ByteBuffer* originalHashBuffer = Slice(readOnlyBuffer, offsetArrays[i + 1], offsetArrays[i + 2]);
        TransInputDataToHashData(originalHashBuffer, generateHashBuffer, originalOffset, generateOffset);
        DataRoundupChunkSize(generateHashBuffer, originalHashBuffer->GetCapacity(), digestSize);
        delete originalHashBuffer;
        delete readOnlyBuffer;
        delete generateHashBuffer;
    }
}

MerkleTree* MerkleTreeBuilder::GetMerkleTree(ByteBuffer* dataBuffer, long inputDataSize,
                                             FsVerityHashAlgorithm fsVerityHashAlgorithm)
{
    int digestSize = fsVerityHashAlgorithm.GetOutputByteSize();
    dataBuffer->Flip();
    std::vector<int8_t> rootHash;
    std::vector<int8_t> tree;
    if (inputDataSize < FSVERITY_HASH_PAGE_SIZE) {
        ByteBuffer* fsVerityHashPageBuffer = Slice(dataBuffer, 0, digestSize);
        if (fsVerityHashPageBuffer != nullptr) {
            rootHash = std::vector<int8_t>(digestSize);
            fsVerityHashPageBuffer->GetByte(rootHash.data(), digestSize);
            delete fsVerityHashPageBuffer;
            fsVerityHashPageBuffer = nullptr;
        }
    } else {
        tree = std::vector<int8_t>(dataBuffer->GetBufferPtr(), dataBuffer->GetBufferPtr() + dataBuffer->GetCapacity());
        ByteBuffer* fsVerityHashPageBuffer = Slice(dataBuffer, 0, FSVERITY_HASH_PAGE_SIZE);
        if (fsVerityHashPageBuffer != nullptr) {
            std::vector<int8_t> fsVerityHashPage(FSVERITY_HASH_PAGE_SIZE);
            fsVerityHashPageBuffer->GetData(0, fsVerityHashPage.data(), FSVERITY_HASH_PAGE_SIZE);
            DigestUtils digestUtils(HASH_SHA256);
            std::string fsVerityHashPageStr(fsVerityHashPage.begin(), fsVerityHashPage.end());
            digestUtils.AddData(fsVerityHashPageStr);
            std::string result = digestUtils.Result(DigestUtils::Type::BINARY);
            for (int i = 0; i < static_cast<int>(result.size()); i++) {
                rootHash.push_back(result[i]);
            }
            delete fsVerityHashPageBuffer;
            fsVerityHashPageBuffer = nullptr;
        }
    }

    return new MerkleTree(rootHash, tree, fsVerityHashAlgorithm);
}

void MerkleTreeBuilder::DataRoundupChunkSize(ByteBuffer* data, long originalDataSize, int digestSize)
{
    long fullChunkSize = GetFullChunkSize(originalDataSize, CHUNK_SIZE, digestSize);
    int diffValue = (int)(fullChunkSize % CHUNK_SIZE);
    if (diffValue > 0) {
        data->SetPosition(data->GetPosition() + (CHUNK_SIZE - diffValue));
    }
}

long MerkleTreeBuilder::GetChunkCount(long dataSize, long divisor)
{
    return (long)std::ceil((double)dataSize / (double)divisor);
}

long MerkleTreeBuilder::GetFullChunkSize(long dataSize, long divisor, long multiplier)
{
    return GetChunkCount(dataSize, divisor) * multiplier;
}
} // namespace SignatureTools
} // namespace OHOS