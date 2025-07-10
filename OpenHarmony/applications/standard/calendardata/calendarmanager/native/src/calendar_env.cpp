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

#include "calendar_env.h"


namespace OHOS::CalendarApi {

void CalendarEnv::Init(std::string bundleName, uint64_t tokenId)
{
    m_bundleName = std::move(bundleName);
    m_tokenId = tokenId;
}

std::string &CalendarEnv::GetBundleName()
{
    return m_bundleName;
}

uint64_t CalendarEnv::GetTokenId()
{
    return m_tokenId;
}
}