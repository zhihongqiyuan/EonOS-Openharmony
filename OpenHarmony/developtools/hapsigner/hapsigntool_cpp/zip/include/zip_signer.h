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

#ifndef SIGNATRUETOOLS_ZIP_SIGNER_H
#define SIGNATRUETOOLS_ZIP_SIGNER_H

#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "endof_central_directory.h"
#include "signature_tools_log.h"
#include "zip_entry.h"

namespace OHOS {
namespace SignatureTools {
class ZipSigner {
public:
    /* file is uncompress file flag */
    static constexpr int FILE_UNCOMPRESS_METHOD_FLAG = 0;

    /* max comment length */
    static constexpr int MAX_COMMENT_LENGTH = 65535;

    ZipSigner()
    {
        m_endOfCentralDirectory = nullptr;
    }

    ~ZipSigner()
    {
        delete m_endOfCentralDirectory;
        for (auto& zipEntry : m_zipEntries) {
            delete zipEntry;
        }
    }

    bool Init(std::ifstream& inputFile);

    /**
     * output zip to zip file
     *
     * @param outFile file path
     */
    bool ToFile(std::ifstream& input, std::ofstream& output);

    /**
     * alignment uncompress entry
     *
     * @param alignment int alignment
     */
    void Alignment(int alignment);

    void RemoveSignBlock();

    std::vector<ZipEntry*>& GetZipEntries();

    void SetZipEntries(const std::vector<ZipEntry*>& zipEntries);

    uint32_t GetSigningOffset();

    void SetSigningOffset(uint32_t signingOffset);

    std::string GetSigningBlock();

    void SetSigningBlock(const std::string& signingBlock);

    uint32_t GetCDOffset();

    void SetCDOffset(uint32_t cDOffset);

    uint32_t GetEOCDOffset();

    void SetEOCDOffset(uint32_t eOCDOffset);

    EndOfCentralDirectory* GetEndOfCentralDirectory();

    void SetEndOfCentralDirectory(EndOfCentralDirectory* endOfCentralDirectory);

private:
    EndOfCentralDirectory* GetZipEndOfCentralDirectory(std::ifstream& input);

    bool GetZipCentralDirectory(std::ifstream& input);

    std::string GetSigningBlock(std::ifstream& input);

    bool GetZipEntries(std::ifstream& input);

    /* sort uncompress entry in the front. */
    void Sort();

    void ResetOffset();

    std::vector<ZipEntry*> m_zipEntries;

    uint32_t m_signingOffset = 0;

    std::string m_signingBlock;

    uint32_t m_cDOffset = 0;

    uint32_t m_eOCDOffset = 0;

    EndOfCentralDirectory* m_endOfCentralDirectory;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_ZIP_SIGNER_H
