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

#include <algorithm>

#include "params_trust_list.h"
#include "constant.h"
#include "params.h"
#include "string_utils.h"
#include "help.h"

namespace OHOS {
namespace SignatureTools {
const std::string options = "[options]:";

const std::vector<std::string> commands = {
    GENERATE_KEYPAIR + options,
    GENERATE_CSR + options,
    GENERATE_CERT + options,
    GENERATE_CA + options,
    GENERATE_APP_CERT + options,
    GENERATE_PROFILE_CERT + options,
    SIGN_PROFILE + options,
    VERIFY_PROFILE + options,
    SIGN_APP + options,
    VERIFY_APP + options
};

ParamsTrustList ParamsTrustList::GetInstance()
{
    static ParamsTrustList instance;
    return instance;
}

void ParamsTrustList::PutTrustMap(const std::string& cmdStandBy, const std::string& param)
{
    if (param.at(0) == '-') {
        size_t pos = param.find(':');
        std::string subParam = param.substr(0, pos);
        subParam = StringUtils::Trim(subParam);
        bool isExists = false;
        if (trustMap.find(cmdStandBy) != trustMap.end()) {
            isExists = true;
        }
        std::vector<std::string> trustList = isExists ? trustMap[cmdStandBy] : std::vector<std::string>();
        trustList.push_back(subParam);
        trustMap[cmdStandBy] = trustList;
    }
}

void ParamsTrustList::ReadHelpParam(std::istringstream& fd)
{
    std::string str;
    std::string cmdStandBy;
    while (std::getline(fd, str)) {
        bool isExists = false;
        std::string params = StringUtils::Trim(str);
        if (params.empty()) {
            continue;
        }
        isExists = std::any_of(commands.begin(), commands.end(),
                               [params](const std::string& cmd) {return cmd == params; });
        if (isExists) {
            cmdStandBy = params;
        } else {
            PutTrustMap(cmdStandBy, params);
        }
    }
}

void ParamsTrustList::GenerateTrustList()
{
    std::istringstream iss(HELP_TXT);
    ReadHelpParam(iss);
}

std::vector<std::string> ParamsTrustList::GetTrustList(const std::string& command)
{
    std::string keyParam = command + options;
    GenerateTrustList();
    if (trustMap.find(keyParam) != trustMap.end()) {
        return trustMap[keyParam];
    } else {
        PrintErrorNumberMsg("COMMAND_ERROR", COMMAND_ERROR, "'" + command + "is not trust command");
        trustMap[keyParam].clear();
        return trustMap[keyParam];
    }
}
} // namespace SignatureTools
} // namespace OHOS