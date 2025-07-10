/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "mock_ans_manager_stub.h"

#include "ans_inner_errors.h"

namespace OHOS {
namespace Notification {
std::string MockAnsManagerStub::GetCmd()
{
    return cmd_;
}

std::string MockAnsManagerStub::GetBundle()
{
    return bundle_;
}

int32_t MockAnsManagerStub::GetUserId()
{
    return userId_;
}

ErrCode MockAnsManagerStub::ShellDump(const std::string &cmd, const std::string &bundle, int32_t userId,
    int32_t recvUserId, std::vector<std::string> &dumpInfo)
{
    ANS_LOGE("enter");
    ErrCode result = ERR_ANS_NOT_ALLOWED;
    cmd_ = cmd;
    bundle_ = bundle;
    userId_ = userId;
    return result;
}
} // namespace Notification
} // namespace OHOSc
