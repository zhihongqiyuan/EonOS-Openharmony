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
#include "native_lib_info_segment.h"

namespace OHOS {
namespace SignatureTools {

NativeLibInfoSegment::NativeLibInfoSegment()
{
    magic = MAGIC_NUM;
    zeroPadding = std::vector<int8_t>(0);
    fileNameListBlockSize = 0;
    segmentSize = 0;
    sectionNum = 0;
    signInfoListBlockSize = 0;
}

NativeLibInfoSegment::NativeLibInfoSegment(int32_t magic,
                                           int32_t segmentSize,
                                           int32_t sectionNum,
                                           const std::vector<SignedFilePos>& signedFilePosList,
                                           const std::vector<std::string>& fileNameList,
                                           const std::vector<SignInfo>& signInfoList,
                                           const std::vector<int8_t>& zeroPadding)
{
    this->magic = magic;
    this->segmentSize = segmentSize;
    this->sectionNum = sectionNum;
    this->signedFilePosList = signedFilePosList;
    this->fileNameList = fileNameList;
    this->signInfoList = signInfoList;
    this->zeroPadding = zeroPadding;
    fileNameListBlockSize = 0;
    signInfoListBlockSize = 0;
}

void NativeLibInfoSegment::SetSoInfoList(const std::vector<std::pair<std::string, SignInfo>> &soInfoList)
{
    this->soInfoList = soInfoList;
    // Once map is set, update length, sectionNum as well
    sectionNum = soInfoList.size();
    // generate file name list and sign info list
    GenerateList();
}

int32_t NativeLibInfoSegment::GetSectionNum()
{
    return sectionNum;
}

std::vector<std::string>& NativeLibInfoSegment::GetFileNameList()
{
    return fileNameList;
}

std::vector<OHOS::SignatureTools::SignInfo>& NativeLibInfoSegment::GetSignInfoList()
{
    return signInfoList;
}

int32_t NativeLibInfoSegment::Size()
{
    int blockSize = MAGIC_LENGTH_SECNUM_BYTES;
    blockSize += signedFilePosList.size() * SIGNED_FILE_POS_SIZE;
    blockSize += fileNameListBlockSize + zeroPadding.size() + signInfoListBlockSize;
    return blockSize;
}

void NativeLibInfoSegment::ToByteArray(std::vector<int8_t> &ret)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(ByteBuffer(Size()));
    std::vector<int8_t> empt(Size());
    bf->PutData(empt.data(), empt.size());
    bf->Clear();
    bf->PutInt32(magic);
    bf->PutInt32(segmentSize);
    bf->PutInt32(sectionNum);
    for (SignedFilePos &offsetAndSize : signedFilePosList) {
        bf->PutInt32(offsetAndSize.GetFileNameOffset());
        bf->PutInt32(offsetAndSize.GetFileNameSize());
        bf->PutInt32(offsetAndSize.GetSignInfoOffset());
        bf->PutInt32(offsetAndSize.GetSignInfoSize());
    }
    for (std::string &fileName : fileNameList) {
        bf->PutData(fileName.c_str(), fileName.size() * sizeof(char));
    }
    bf->PutData(zeroPadding.data(), zeroPadding.size());
    for (SignInfo &signInfo : signInfoList) {
        std::vector<int8_t> signInfoArr;
        signInfo.ToByteArray(signInfoArr);
        bf->PutData(signInfoArr.data(), signInfoArr.size());
    }
    ret = std::vector<int8_t>(bf->GetBufferPtr(), bf->GetBufferPtr() + bf->GetPosition());
    return;
}

NativeLibInfoSegment NativeLibInfoSegment::FromByteArray(std::vector<int8_t> &bytes)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(ByteBuffer(bytes.size()));
    bf->PutData(bytes.data(), bytes.size());
    bf->Flip();
    int32_t inMagic = 0;
    int32_t inSegmentSize = 0;
    int32_t inSectionNum = 0;
    bool checkFlag = CheckBuffer(bf.get(), inMagic, inSegmentSize, inSectionNum);
    if (!checkFlag) {
        return NativeLibInfoSegment();
    }
    std::vector<SignedFilePos> inSignedFilePosList;
    for (int i = 0; i < inSectionNum; i++) {
        std::vector<int8_t> entry(SIGNED_FILE_POS_SIZE, 0);
        bf->GetByte(entry.data(), entry.size());
        inSignedFilePosList.push_back(SignedFilePos::FromByteArray(entry));
    }
    // parse file name list
    std::vector<std::string> inFileNameList;
    int fileNameListSize = 0;
    for (SignedFilePos &pos : inSignedFilePosList) {
        std::vector<char> fileNameBuffer(pos.GetFileNameSize(), 0);
        fileNameListSize += pos.GetFileNameSize();
        bf->SetPosition(pos.GetFileNameOffset());
        bf->GetData(fileNameBuffer.data(), fileNameBuffer.size());
        inFileNameList.push_back(std::string(fileNameBuffer.data(), fileNameBuffer.size()));
    }
    // parse zeroPadding
    std::vector<int8_t> inZeroPadding((ALIGNMENT_FOR_SIGNINFO - fileNameListSize
                                      % ALIGNMENT_FOR_SIGNINFO) % ALIGNMENT_FOR_SIGNINFO);
    bf->GetByte(inZeroPadding.data(), inZeroPadding.size());
    // parse sign info list
    std::vector<OHOS::SignatureTools::SignInfo> inSignInfoList;
    for (SignedFilePos &pos : inSignedFilePosList) {
        if (pos.GetSignInfoOffset() % ALIGNMENT_FOR_SIGNINFO != 0) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                                "The offset of signature information must be an integer multiple of 4");
            return NativeLibInfoSegment();
        }
        std::vector<int8_t> signInfoBuffer(pos.GetSignInfoSize());
        bf->SetPosition(pos.GetSignInfoOffset());
        bf->GetByte(signInfoBuffer.data(), signInfoBuffer.size());
        inSignInfoList.push_back(OHOS::SignatureTools::SignInfo::FromByteArray(signInfoBuffer));
    }
    return NativeLibInfoSegment(inMagic, inSegmentSize, inSectionNum, inSignedFilePosList,
                                inFileNameList, inSignInfoList, inZeroPadding);
}

bool NativeLibInfoSegment::CheckBuffer(ByteBuffer* bf, int32_t& inMagic, int32_t& inSegmentSize,
    int32_t& inSectionNum)
{
    bf->GetInt32(inMagic);
    if (inMagic != MAGIC_NUM) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The magic number of NativeLibInfoSegment is incorrect.");
        return false;
    }
    bf->GetInt32(inSegmentSize);
    if (inSegmentSize < 0) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The segment size of NativeLibInfoSegment is incorrect.");
        return false;
    }
    bf->GetInt32(inSectionNum);
    if (inSectionNum < 0) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The section number of NativeLibInfoSegment is incorrect.");
        return false;
    }
    return true;
}

void NativeLibInfoSegment::GenerateList()
{
    // empty all before generate list
    fileNameList.clear();
    signInfoList.clear();
    signedFilePosList.clear();
    int fileNameOffset = 0;
    int signInfoOffset = 0;
    for (std::pair<std::string, SignInfo> &soInfo :soInfoList) {
        std::string fileName = soInfo.first;
        SignInfo& signInfo = soInfo.second;
        int fileNameSizeInBytes = fileName.size() * sizeof(char);
        int signInfoSizeInBytes = signInfo.GetSize() * sizeof(char);
        fileNameList.push_back(fileName);
        signInfoList.push_back(signInfo);
        std::unique_ptr<SignedFilePos> posPtr = std::make_unique<SignedFilePos>(fileNameOffset,
            fileNameSizeInBytes, signInfoOffset, signInfoSizeInBytes);
        signedFilePosList.push_back(*posPtr.get());
        // increase fileNameOffset and signInfoOffset
        fileNameOffset += fileNameSizeInBytes;
        signInfoOffset += signInfoSizeInBytes;
    }
    fileNameListBlockSize = fileNameOffset;
    signInfoListBlockSize = signInfoOffset;
    // alignment for signInfo
    zeroPadding = std::vector<int8_t>((ALIGNMENT_FOR_SIGNINFO - fileNameListBlockSize
                                            % ALIGNMENT_FOR_SIGNINFO) % ALIGNMENT_FOR_SIGNINFO);
    // after fileNameList and signInfoList is generated, update segment size
    segmentSize = Size();
    // adjust file name and sign info offset base on segment start
    int fileNameOffsetBase = MAGIC_LENGTH_SECNUM_BYTES + signedFilePosList.size() * SIGNED_FILE_POS_SIZE;
    int signInfoOffsetBase = fileNameOffsetBase + fileNameListBlockSize;
    for (SignedFilePos &pos : signedFilePosList) {
        pos.IncreaseFileNameOffset(fileNameOffsetBase);
        pos.IncreaseSignInfoOffset(signInfoOffsetBase + zeroPadding.size());
    }
}

}
}
