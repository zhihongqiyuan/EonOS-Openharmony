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

#ifndef SIGNATRUETOOLS_PARAMSTRUSTLIST_H
#define SIGNATRUETOOLS_PARAMSTRUSTLIST_H

#include <string>
#include <vector>
#include <fstream>
#include <mutex>
#include <unordered_map>
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
class ParamsTrustList final {
public:
    static ParamsTrustList GetInstance();
    std::vector<std::string> GetTrustList(const std::string& command);
    
private:
    void ReadHelpParam(std::istringstream& fd);
    void PutTrustMap(const std::string& cmd_stand_by, const std::string& param);
    void GenerateTrustList();
    
private:
    std::unordered_map<std::string, std::vector<std::string>> trustMap;
};
} // namespace SignatureTools
} // namespace OHOS
#endif
