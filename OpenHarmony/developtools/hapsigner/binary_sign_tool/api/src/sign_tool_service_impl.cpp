/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
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

#include "sign_tool_service_impl.h"
#include "pkcs7_data.h"
#include "profile_sign_tool.h"
#include "nlohmann/json.hpp"
#include "profile_info.h"
#include "profile_verify.h"
#include "signature_tools_errno.h"
#include "local_sign_provider.h"
#include "signature_tools_log.h"
#include "param_constants.h"
#include "constant.h"
#include "remote_sign_provider.h"

namespace OHOS {
namespace SignatureTools {
bool SignToolServiceImpl::Sign(Options* options)
{
    std::string inFile = options->GetString(Options::IN_FILE);
    if (!FileUtils::isElfFile(inFile)) {
        SIGNATURE_TOOLS_LOGE("inFile is not a elf file");
        return false;
    }
    if (options->GetString(Options::AD_HOC) == ParamConstants::AD_HOC_TYPE_1) {
        return SignAdHoc(options);
    }
    std::string mode = options->GetString(Options::MODE);
    std::shared_ptr<SignProvider> signProvider;
    if (LOCAL_SIGN == mode) {
        signProvider = std::make_shared<LocalSignProvider>();
    } else if (REMOTE_SIGN == mode) {
        signProvider = std::make_shared<RemoteSignProvider>();
    } else {
        SIGNATURE_TOOLS_LOGE("Resign mode. But not implemented yet");
        return false;
    }
    return signProvider->SignElf(options);
}

int SignToolServiceImpl::GetProvisionContent(const std::string& input, std::string& ret)
{
    std::string bytes;
    if (FileUtils::ReadFile(input, bytes) < 0) {
        SIGNATURE_TOOLS_LOGE("provision read faild!");
        return IO_ERROR;
    }
    nlohmann::json obj = nlohmann::json::parse(bytes);
    if (obj.is_discarded() || (!obj.is_structured())) {
        PrintErrorNumberMsg("PARSE ERROR", PARSE_ERROR, "Parsing provision failed!");
        return PARSE_ERROR;
    }
    ret = obj.dump();
    ProfileInfo provision;
    AppProvisionVerifyResult result = ParseProvision(ret, provision);
    if (result != PROVISION_OK) {
        SIGNATURE_TOOLS_LOGE("invalid provision");
        return INVALIDPARAM_ERROR;
    }
    return 0;
}

bool SignToolServiceImpl::SignAdHoc(Options* options)
{
    std::string inFile = options->GetString(Options::IN_FILE);
    std::string outFile = options->GetString(Options::OUT_FILE);
    std::string tmpOutFile = outFile;
    if (outFile == inFile) {
        tmpOutFile = "tmp-signed-elf";
    }
    bool ret = std::filesystem::copy_file(inFile, tmpOutFile, std::filesystem::copy_options::overwrite_existing);
    if (!ret) {
        SIGNATURE_TOOLS_LOGE("Error: SignAdHoc failed");
        return false;
    }
    return FileUtils::CopyTmpFileAndDel(tmpOutFile, outFile);
}
} // namespace SignatureTools
} // namespace OHOS
