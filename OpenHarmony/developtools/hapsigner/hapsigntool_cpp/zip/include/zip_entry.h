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

#ifndef SIGNATRUETOOLS_ZIP_ENTRY_H
#define SIGNATRUETOOLS_ZIP_ENTRY_H

#include "central_directory.h"
#include "zip_entry_data.h"

namespace OHOS {
namespace SignatureTools {
class ZipEntry {
public:
    ZipEntry()
    {
        m_zipEntryData = nullptr;
        m_fileEntryInCentralDirectory = nullptr;
    }

    ~ZipEntry()
    {
        delete m_zipEntryData;
        delete m_fileEntryInCentralDirectory;
    }

    ZipEntryData* GetZipEntryData();

    void SetZipEntryData(ZipEntryData* zipEntryData);

    CentralDirectory* GetCentralDirectory();

    void SetCentralDirectory(CentralDirectory* centralDirectory);

    int Alignment(int alignNum);

private:
    bool CalZeroPaddingLengthForEntryExtra(uint16_t& padding);

    bool SetCenterDirectoryNewExtraLength(uint16_t newLength);

    bool SetEntryHeaderNewExtraLength(uint16_t newLength);

    bool GetAlignmentNewExtra(uint16_t newLength, const std::string& old, std::string& res);

    ZipEntryData* m_zipEntryData;

    CentralDirectory* m_fileEntryInCentralDirectory;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_ZIP_ENTRY_H