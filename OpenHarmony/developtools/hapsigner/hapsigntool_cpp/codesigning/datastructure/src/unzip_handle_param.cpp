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

#include "unzip_handle_param.h"

namespace OHOS {
namespace SignatureTools {

UnzipHandleParam::UnzipHandleParam(const CodeSignBlock& csb, const std::pair<std::string, std::string>& pairResult,
    bool isSign)
{
    this->csb = csb;
    this->pairResult = pairResult;
    this->isSign = isSign;
}

UnzipHandleParam::UnzipHandleParam(std::vector<std::pair<std::string, SignInfo>>& ret,
    const std::string& ownerID, bool isSign)
{
    this->ret = ret;
    this->ownerID = ownerID;
    this->isSign = isSign;
}

CodeSignBlock& UnzipHandleParam::GetCodeSignBlock()
{
    return csb;
}

std::pair<std::string, std::string>& UnzipHandleParam::GetPairResult()
{
    return pairResult;
}

std::vector<std::pair<std::string, SignInfo>>& UnzipHandleParam::GetRet()
{
    return ret;
}

std::string& UnzipHandleParam::GetOwnerID()
{
    return ownerID;
}

bool UnzipHandleParam::IsSign()
{
    return isSign;
}

} // namespace SignatureTools
} // namespace OHOS