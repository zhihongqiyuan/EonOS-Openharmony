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

#ifndef SIGNATRUETOOLS_ZIP_UTILS_H
#define SIGNATRUETOOLS_ZIP_UTILS_H

#include <limits>
#include <memory>

#include "signature_tools_log.h"
#include "zip_data_input.h"

namespace OHOS {
namespace SignatureTools {
class ZipUtils {
public:
    ZipUtils() = delete;
    /**
     * set offset value of Central Directory to End of Central Directory Record.
     *
     * @param eocd buffer of End of Central Directory Record.
     * @param offset offset value of Central Directory.
     */
    static bool SetCentralDirectoryOffset(ByteBuffer& eocd, int64_t offset);

private:
    static constexpr int ZIP_CENTRAL_DIR_OFFSET_IN_EOCD = 16;

    static constexpr int64_t UINT32_MAX_VALUE = 0xffffffffLL;

    static bool SetUInt32ToBuffer(ByteBuffer& buffer, int offset, int64_t value);
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_ZIP_UTILS_H
