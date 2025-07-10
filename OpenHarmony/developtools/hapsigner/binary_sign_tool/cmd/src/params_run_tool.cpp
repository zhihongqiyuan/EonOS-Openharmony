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

#include "params_run_tool.h"
#include <unistd.h>
#include <memory>
#include <filesystem>

#include "constant.h"
#include "help.h"

namespace OHOS {
namespace SignatureTools {
const std::string ParamsRunTool::VERSION = "1.0.0";

static std::unordered_map <std::string,
                           std::function<bool(Options* params, SignToolServiceImpl& api)>> DISPATCH_RUN_METHOD {
    {SIGN_ELF, ParamsRunTool::RunSignApp}
};

bool ParamsRunTool::ProcessCmd(char** args, size_t size)
{
    if (size < CmdUtil::ARGS_MIN_LEN) {
        PrintHelp();
        return true;
    }
    if (args == nullptr || strcmp(args[1], "") == 0) {
        PrintHelp();
        return true;
    } else if (strcmp(args[1], "-h") == 0 || strcmp(args[1], "-help") == 0) {
        PrintHelp();
        return true;
    } else if (strcmp(args[1], "-v") == 0 || strcmp(args[1], "-version") == 0) {
        Version();
        return true;
    } else {
        std::shared_ptr<SignToolServiceImpl> serviceApi = std::make_shared<SignToolServiceImpl>();
        ParamsSharedPtr param = std::make_shared<Params>();
        if (!CmdUtil::Convert2Params(args, size, param)) {
            PrintMsg(param->GetMethod() + " failed");
            return false;
        }
        PrintMsg("Start " + param->GetMethod());
        SIGNATURE_TOOLS_LOGD("%s run start time  ", param->GetMethod().c_str());
        if (!DispatchParams(param, *serviceApi)) {
            SIGNATURE_TOOLS_LOGD("%s run end time  ", param->GetMethod().c_str());
            PrintMsg(param->GetMethod() + " failed");
            return false;
        }
        PrintMsg(param->GetMethod() + " success");
        SIGNATURE_TOOLS_LOGD("%s run end time  ", param->GetMethod().c_str());
    }
    return true;
}

bool ParamsRunTool::RunSignApp(Options* params, SignToolServiceImpl& api)
{
    if (!params->Required({Options::MODE, Options::IN_FILE, Options::OUT_FILE})) {
        return false;
    }
    if (!CmdUtil::UpdateParamForCheckInFile(params, {Options::IN_FILE, Options::APP_CERT_FILE,
                                            Options::PROFILE_FILE, Options::KEY_STORE_FILE, Options::MODULE_FILE,
                                            ParamConstants::PARAM_REMOTE_SIGNERPLUGIN})) {
        return false;
    }
    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::OUT_FILE})) {
        return false;
    }
    if (params->GetString(Options::AD_HOC) == ParamConstants::AD_HOC_TYPE_1) {
        return api.Sign(params);
    }
    std::string mode = params->GetString(Options::MODE);
    if (!StringUtils::CaseCompare(mode, LOCAL_SIGN) &&
        !StringUtils::CaseCompare(mode, REMOTE_SIGN)) {
        PrintErrorNumberMsg("COMMAND_ERROR", COMMAND_ERROR, "not support command param '" + mode + "'");
        return false;
    }
    if (StringUtils::CaseCompare(mode, LOCAL_SIGN)) {
        if (!params->Required({Options::KEY_STORE_FILE, Options::KEY_ALIAS, Options::APP_CERT_FILE})) {
            return false;
        }
        if (!FileUtils::ValidFileType(params->GetString(Options::KEY_STORE_FILE), {"p12", "jks"})) {
            return false;
        }
    }
    if (!CheckProfile(*params)) {
        return false;
    }
    std::string signAlg = params->GetString(Options::SIGN_ALG);
    if (!CmdUtil::JudgeSignAlgType(signAlg)) {
        return false;
    }
    return api.Sign(params);
}

bool ParamsRunTool::CheckProfile(Options& params)
{
    std::string profileFile = params.GetString(Options::PROFILE_FILE);
    std::string profileSigned = params.GetString(Options::PROFILE_SIGNED);
    if (profileSigned == DEFAULT_PROFILE_SIGNED_1) {
        if (!FileUtils::ValidFileType(profileFile, {"p7b"})) {
            return false;
        }
    } else {
        if (!FileUtils::ValidFileType(profileFile, {"json"})) {
            return false;
        }
    }
    return true;
}

bool ParamsRunTool::DispatchParams(const ParamsSharedPtr& params, SignToolServiceImpl& api)
{
    bool isSuccess = false;
    std::string method = params->GetMethod();
    auto it = DISPATCH_RUN_METHOD.find(method);
    if (it == DISPATCH_RUN_METHOD.end()) {
        PrintErrorNumberMsg("COMMAND_ERROR", COMMAND_ERROR, "Unsupported method: " + method);
        return false;
    } else {
        isSuccess = it->second(params->GetOptions(), api);
    }
    return isSuccess;
}

void ParamsRunTool::PrintHelp()
{
    PrintMsg(HELP_TXT);
}

void  ParamsRunTool::Version()
{
    PrintMsg(ParamsRunTool::VERSION);
}
} // namespace SignatureTools
} // namespace OHOS