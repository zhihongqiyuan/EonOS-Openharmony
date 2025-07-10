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
#ifndef SIGNATURETOOLS_UNZIP_HANDLE_PARAM_H
#define SIGNATURETOOLS_UNZIP_HANDLE_PARAM_H

#include <vector>
#include <string>

#include "code_sign_block.h"

namespace OHOS {
namespace SignatureTools {
class UnzipHandleParam {
public:
    UnzipHandleParam(const CodeSignBlock& csb, const std::pair<std::string, std::string>& pairResult, bool isSign);
    UnzipHandleParam(std::vector<std::pair<std::string, SignInfo>>& ret, const std::string& ownerID, bool isSign);
    CodeSignBlock& GetCodeSignBlock();
    std::pair<std::string, std::string>& GetPairResult();
    std::vector<std::pair<std::string, SignInfo>>& GetRet();
    std::string& GetOwnerID();
    bool IsSign();

private:
    CodeSignBlock csb;
    std::pair<std::string, std::string> pairResult;
    std::vector<std::pair<std::string, SignInfo>> ret;
    std::string ownerID;
    bool isSign;
};
} // namespace SignatureTools
} // namespace OHOS

#endif