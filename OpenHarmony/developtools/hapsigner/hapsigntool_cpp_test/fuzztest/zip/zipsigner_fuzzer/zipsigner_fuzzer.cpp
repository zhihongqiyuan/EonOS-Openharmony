/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "zip_signer.h"

namespace OHOS {
namespace SignatureTools {
static constexpr int ALIGNMENT = 4;
const char* UNSIGNED_HAP_FILE_PATH = "./zip/unsigned.hap";
const char* SIGNED_HAP_FILE_PATH = "./zip/signed.hap";
const char* OUT_HAP_FILE_PATH = "./zip/output.hap";
const char* DATA_DESC_HAP_FILE_PATH = "./zip/data_descriptor_hap.hap";

void ZipSignerCompleteFlowFunc(const uint8_t* data, size_t size)
{
    std::ifstream inputFile(SIGNED_HAP_FILE_PATH, std::ios::binary);
    std::ofstream outputFile(OUT_HAP_FILE_PATH, std::ios::binary | std::ios::trunc);
    auto zip = std::make_shared<ZipSigner>();
    if (!zip->Init(inputFile)) {
        return;
    }
    zip->Alignment(ALIGNMENT);
    zip->RemoveSignBlock();
    zip->ToFile(inputFile, outputFile);
}

void ZipSignerInfoFunc(const uint8_t* data, size_t size)
{
    std::ifstream inputFile(SIGNED_HAP_FILE_PATH, std::ios::binary);
    auto zip = std::make_shared<ZipSigner>();
    if (!zip->Init(inputFile)) {
        return;
    }
    std::vector<ZipEntry*> zipEntries{nullptr};
    zip->SetZipEntries(zipEntries);
    zip->SetSigningOffset(size);
    std::string signingBlock(reinterpret_cast<const char*>(data), size);
    zip->SetSigningBlock(signingBlock);
    zip->SetCDOffset(size);
    zip->SetEOCDOffset(size);
    zip->SetEndOfCentralDirectory(nullptr);

    zip->GetZipEntries();
    zip->GetSigningOffset();
    zip->GetSigningBlock();
    zip->GetCDOffset();
    zip->GetEOCDOffset();
    zip->GetEndOfCentralDirectory();
}

void ZipEntryHeaderInfoFunc(const uint8_t* data, size_t size)
{
    std::ifstream inputFile(SIGNED_HAP_FILE_PATH, std::ios::binary);
    auto zip = std::make_shared<ZipSigner>();
    if (!zip->Init(inputFile)) {
        return;
    }
    auto zipEntries = zip->GetZipEntries();
    for (const auto& zipEntry : zipEntries) {
        auto zipEntryData = zipEntry->GetZipEntryData();
        auto zipEntryHeader = zipEntryData->GetZipEntryHeader();

        std::string fileName(reinterpret_cast<const char*>(data), size);
        zipEntryHeader->SetFileName(fileName);

        zipEntryHeader->GetCrc32();
        zipEntryHeader->GetLastTime();
        zipEntryHeader->GetLastDate();
        zipEntryHeader->GetCompressedSize();
        zipEntryHeader->GetUnCompressedSize();
        zipEntryHeader->GetHeaderLength();
        zipEntryHeader->GetSIGNATURE();
        zipEntryHeader->GetVersion();
    }
}

void CentralDirectoryInfoFunc(const uint8_t* data, size_t size)
{
    std::ifstream inputFile(UNSIGNED_HAP_FILE_PATH, std::ios::binary);
    auto zip = std::make_shared<ZipSigner>();
    if (!zip->Init(inputFile)) {
        return;
    }
    auto zipEntries = zip->GetZipEntries();
    for (const auto& zipEntry : zipEntries) {
        auto cd = zipEntry->GetCentralDirectory();
        cd->GetLength();
        cd->GetCdLength();
        cd->GetSIGNATURE();
        cd->GetVersion();
        cd->GetVersionExtra();
        cd->GetFlag();
        cd->GetLastTime();
        cd->GetLastDate();
        cd->GetCrc32();
        cd->GetDiskNumStart();
        cd->GetInternalFile();
        cd->GetExternalFile();
        cd->GetFileName();

        std::string comment(reinterpret_cast<const char*>(data), size);
        cd->SetComment(comment);

        cd->GetComment();
    }
}

void DataDescriptorInfoFunc(const uint8_t* data, size_t size)
{
    std::ifstream inputFile(DATA_DESC_HAP_FILE_PATH, std::ios::binary);
    auto zip = std::make_shared<ZipSigner>();
    if (!zip->Init(inputFile)) {
        return;
    }
    auto zipEntries = zip->GetZipEntries();
    for (const auto& zipEntry : zipEntries) {
        auto zipEntryData = zipEntry->GetZipEntryData();
        auto dataDescriptor = zipEntryData->GetDataDescriptor();
        if (!dataDescriptor) {
            continue;
        }
        dataDescriptor->GetDesLength();
        dataDescriptor->GetSIGNATURE();
        dataDescriptor->GetCrc32();
        dataDescriptor->GetCompressedSize();
        dataDescriptor->GetUnCompressedSize();
    }
}

void AlignmentFunc(const uint8_t* data, size_t size)
{
    std::ifstream inputFile(UNSIGNED_HAP_FILE_PATH, std::ios::binary);
    auto zip = std::make_shared<ZipSigner>();
    int aliBytes = 102400;
    if (!zip->Init(inputFile)) {
        return;
    }
    auto zipEntries = zip->GetZipEntries();
    for (const auto& zipEntry : zipEntries) {
        zipEntry->Alignment(aliBytes);
    }
}

void EndOfCentralDirectoryInfoFunc(const uint8_t* data, size_t size)
{
    std::ifstream inputFile(UNSIGNED_HAP_FILE_PATH, std::ios::binary);
    auto zip = std::make_shared<ZipSigner>();
    if (!zip->Init(inputFile)) {
        return;
    }
    auto eocd = zip->GetEndOfCentralDirectory();
    eocd->GetLength();
    eocd->GetEocdLength();
    eocd->GetSIGNATURE();
    eocd->GetDiskNum();
    eocd->GetcDStartDiskNum();
    eocd->GetThisDiskCDNum();

    std::string comment(reinterpret_cast<const char*>(data), size);
    eocd->SetComment(comment);

    eocd->GetComment();
}

void DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    if (data == nullptr || size == 0) {
        return;
    }

    ZipSignerCompleteFlowFunc(data, size);
    ZipSignerInfoFunc(data, size);
    ZipEntryHeaderInfoFunc(data, size);
    CentralDirectoryInfoFunc(data, size);
    DataDescriptorInfoFunc(data, size);
    AlignmentFunc(data, size);
    EndOfCentralDirectoryInfoFunc(data, size);
}
} // namespace SignatureTools
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    (void)rename("./zip/data_descriptor_hap.txt", OHOS::SignatureTools::DATA_DESC_HAP_FILE_PATH);
    (void)rename("./zip/unsigned.txt", OHOS::SignatureTools::UNSIGNED_HAP_FILE_PATH);
    (void)rename("./zip/signed.txt", OHOS::SignatureTools::SIGNED_HAP_FILE_PATH);
    sync();
    OHOS::SignatureTools::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}