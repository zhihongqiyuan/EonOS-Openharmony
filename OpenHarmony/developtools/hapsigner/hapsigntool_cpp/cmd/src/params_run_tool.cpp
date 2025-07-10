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

#include "params_run_tool.h"
#include <unistd.h>
#include <memory>
#include <filesystem>

#include "constant.h"
#include "help.h"

namespace OHOS {
namespace SignatureTools {
const std::string ParamsRunTool::VERSION = "1.0.0";

std::vector<std::string> ParamsRunTool::InformList = {
    "bin",
    "elf",
    "zip"
};

static std::unordered_map <std::string,
                           std::function<bool(Options* params, SignToolServiceImpl& api)>> DISPATCH_RUN_METHOD {
    {SIGN_APP, ParamsRunTool::RunSignApp},
    {SIGN_PROFILE, ParamsRunTool::RunSignProfile},
    {VERIFY_APP, ParamsRunTool::RunVerifyApp},
    {VERIFY_PROFILE, ParamsRunTool::RunVerifyProfile}
};

static std::unordered_map <std::string,
                           std::function<bool(Options* params, SignToolServiceImpl& api)>> GENERATOR_RUN_METHOD {
    {GENERATE_KEYPAIR, ParamsRunTool::RunKeypair},
    {GENERATE_CSR, ParamsRunTool::RunCsr},
    {GENERATE_CA, ParamsRunTool::RunCa},
    {GENERATE_APP_CERT, ParamsRunTool::RunAppCert},
    {GENERATE_PROFILE_CERT, ParamsRunTool::RunProfileCert},
    {GENERATE_CERT, ParamsRunTool::RunCert},
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

bool ParamsRunTool::CallGenerators(const ParamsSharedPtr& params, SignToolServiceImpl& api)
{
    bool isSuccess = false;
    std::string method = params->GetMethod();
    auto it = GENERATOR_RUN_METHOD.find(method);
    if (it == GENERATOR_RUN_METHOD.end()) {
        PrintErrorNumberMsg("COMMAND_ERROR", COMMAND_ERROR, "Unsupported method: " + method);
        return false;
    } else {
        isSuccess = it->second(params->GetOptions(), api);
    }
    return isSuccess;
}

bool ParamsRunTool::RunSignApp(Options* params, SignToolServiceImpl& api)
{
    if (!params->Required({Options::MODE, Options::IN_FILE, Options::OUT_FILE, Options::SIGN_ALG})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckInFile(params, {Options::IN_FILE, Options::APP_CERT_FILE,
                                            Options::PROFILE_FILE, Options::KEY_STORE_FILE,
                                            ParamConstants::PARAM_REMOTE_SIGNERPLUGIN})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::OUT_FILE})) {
        return false;
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
    std::string inForm = params->GetString(Options::INFORM, ZIP);
    if (!StringUtils::IsEmpty(inForm) && !StringUtils::ContainsCase(InformList, inForm)) {
        PrintErrorNumberMsg("NOT_SUPPORT_ERROR", NOT_SUPPORT_ERROR, "parameter '" + inForm
                            + "' format error, Inform only support zip/elf/bin");
        return false;
    }
    std::string signAlg = params->GetString(Options::SIGN_ALG);
    if (!CmdUtil::JudgeSignAlgType(signAlg)) {
        return false;
    }
    return api.SignHap(params);
}

bool ParamsRunTool::CheckProfile(Options& params)
{
    std::string inForm = params.GetString(Options::INFORM);
    std::string profileFile = params.GetString(Options::PROFILE_FILE);

    std::string profileSigned = params.GetString(Options::PROFILE_SIGNED);
    if (StringUtils::CaseCompare(inForm, ELF) && FileUtils::IsEmpty(profileFile)) {
        return true;
    }

    if (profileSigned == "1") {
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
        isSuccess = ParamsRunTool::CallGenerators(params, api);
    } else {
        isSuccess = it->second(params->GetOptions(), api);
    }
    return isSuccess;
}

bool ParamsRunTool::RunCa(Options* params, SignToolServiceImpl& api)
{
    if (!params->Required({Options::KEY_ALIAS, Options::KEY_ALG,
        Options::KEY_SIZE, Options::SUBJECT, Options::SIGN_ALG, Options::KEY_STORE_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::OUT_FILE, Options::KEY_STORE_FILE,
                                             Options::ISSUER_KEY_STORE_FILE})) {
        return false;
    }

    std::string keyAlg = params->GetString(Options::KEY_ALG);
    if (!CmdUtil::JudgeAlgType(keyAlg)) {
        return false;
    }
    int keySize = params->GetInt(Options::KEY_SIZE);
    if (!CmdUtil::JudgeSize(keySize)) {
        return false;
    }
    std::string signAlg = params->GetString(Options::SIGN_ALG);
    if (!CmdUtil::JudgeSignAlgType(signAlg)) {
        return false;
    }
    if (!FileUtils::ValidFileType(params->GetString(Options::KEY_STORE_FILE), {"p12", "jks"})) {
        return false;
    }

    return api.GenerateCA(params);
}

bool ParamsRunTool::RunCert(Options* params, SignToolServiceImpl& api)
{
    if (!params->Required({Options::KEY_ALIAS, Options::ISSUER,
        Options::ISSUER_KEY_ALIAS, Options::SUBJECT, Options::KEY_USAGE,
        Options::SIGN_ALG, Options::KEY_STORE_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckInFile(params, {Options::KEY_STORE_FILE, Options::ISSUER_KEY_STORE_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::OUT_FILE})) {
        return false;
    }

    std::string keyusage = params->GetString(Options::KEY_USAGE);
    if (!CmdUtil::VerifyTypes(keyusage)) {
        return false;
    }
    std::string extkeyusage = params->GetString(Options::EXT_KEY_USAGE);
    if (!extkeyusage.empty()) {
        if (!CmdUtil::VerifyType(extkeyusage)) {
            return false;
        }
    }
    std::string signAlg = params->GetString(Options::SIGN_ALG);
    if (!CmdUtil::JudgeSignAlgType(signAlg)) {
        return false;
    }
    if (!FileUtils::ValidFileType(params->GetString(Options::KEY_STORE_FILE), {"p12", "jks"})) {
        return false;
    }
    return api.GenerateCert(params);
}

bool ParamsRunTool::CheckEndCertArguments(Options& params)
{
    if (!params.Required({Options::KEY_ALIAS, Options::ISSUER, Options::ISSUER_KEY_ALIAS,
                        Options::SUBJECT, Options::SIGN_ALG, Options::KEY_STORE_FILE})) {
        return false;
    }
    std::string signAlg = params.GetString(Options::SIGN_ALG);
    if (!CmdUtil::JudgeSignAlgType(signAlg)) {
        return false;
    }
    std::string outForm = params.GetString(Options::OUT_FORM);
    if (!outForm.empty()) {
        if (!CmdUtil::VerifyType(outForm, Options::OUT_FORM_SCOPE)) {
            return false;
        }
    }
    if (!outForm.empty() && "certChain" == outForm) {
        if (!params.Required({Options::SUB_CA_CERT_FILE, Options::CA_CERT_FILE})) {
            return false;
        }
        if (!FileUtils::ValidFileType(params.GetString(Options::SUB_CA_CERT_FILE), {"cer"}) ||
            !FileUtils::ValidFileType(params.GetString(Options::CA_CERT_FILE), {"cer"})) {
            return false;
        }
    }
    std::string keyStoreFile = params.GetString(Options::KEY_STORE_FILE);
    if (!FileUtils::ValidFileType(keyStoreFile, {"p12", "jks"})) {
        return false;
    }
    if (params.find(Options::ISSUER_KEY_STORE_FILE) != params.end()) {
        std::string issuerKeyStoreFile = params.GetString(Options::ISSUER_KEY_STORE_FILE);
        if (!FileUtils::ValidFileType(issuerKeyStoreFile, {"p12", "jks"})) {
            return false;
        }
    }
    std::string outFile = params.GetString(Options::OUT_FILE);
    if (!outFile.empty()) {
        if (!FileUtils::ValidFileType(outFile, {"cer", "pem"})) {
            return false;
        }
    }
    return true;
}

bool ParamsRunTool::RunAppCert(Options* params, SignToolServiceImpl& api)
{
    if (!CmdUtil::UpdateParamForCheckInFile(params, {Options::KEY_STORE_FILE, Options::ISSUER_KEY_STORE_FILE,
                                            Options::CA_CERT_FILE, Options::SUB_CA_CERT_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::OUT_FILE})) {
        return false;
    }

    if (!ParamsRunTool::CheckEndCertArguments(*params)) {
        return false;
    }
    return api.GenerateAppCert(params);
}

bool ParamsRunTool::RunProfileCert(Options* params, SignToolServiceImpl& api)
{
    if (!CmdUtil::UpdateParamForCheckInFile(params, {Options::KEY_STORE_FILE, Options::ISSUER_KEY_STORE_FILE,
                                            Options::CA_CERT_FILE, Options::SUB_CA_CERT_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::OUT_FILE})) {
        return false;
    }

    if (!ParamsRunTool::CheckEndCertArguments(*params)) {
        return false;
    }
    return api.GenerateProfileCert(params);
}

bool ParamsRunTool::RunKeypair(Options* params, SignToolServiceImpl& api)
{
    if (!params->Required({Options::KEY_ALIAS, Options::KEY_ALG, Options::KEY_SIZE, Options::KEY_STORE_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::KEY_STORE_FILE})) {
        return false;
    }

    std::string keyAlgorithm = params->GetString(Options::KEY_ALG);
    if (!CmdUtil::JudgeAlgType(keyAlgorithm)) {
        return false;
    }
    int keyAlgorithmSize = params->GetInt(Options::KEY_SIZE);
    if (!CmdUtil::JudgeSize(keyAlgorithmSize)) {
        return false;
    }
    if (!FileUtils::ValidFileType(params->GetString(Options::KEY_STORE_FILE), {"p12", "jks"})) {
        return false;
    }
    return api.GenerateKeyStore(params);
}

bool ParamsRunTool::RunCsr(Options* params, SignToolServiceImpl& api)
{
    if (!params->Required({Options::KEY_ALIAS, Options::SUBJECT, Options::SIGN_ALG, Options::KEY_STORE_FILE})) {
        return false;
    }
    if (!CmdUtil::UpdateParamForCheckInFile(params, {Options::KEY_STORE_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::OUT_FILE})) {
        return false;
    }

    std::string signAlg = params->GetString(Options::SIGN_ALG);
    if (!CmdUtil::JudgeSignAlgType(signAlg)) {
        return false;
    }
    if (!FileUtils::ValidFileType(params->GetString(Options::KEY_STORE_FILE), {"p12", "jks"})) {
        return false;
    }
    if (params->count(Options::OUT_FILE)) {
        if (!FileUtils::ValidFileType(params->GetString(Options::OUT_FILE), {"csr"})) {
            return false;
        }
    }
    return api.GenerateCsr(params);
}

bool ParamsRunTool::RunSignProfile(Options* params, SignToolServiceImpl& api)
{
    if (!params->Required({params->MODE, params->SIGN_ALG, params->OUT_FILE, params->IN_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckInFile(params, {Options::KEY_STORE_FILE, Options::PROFILE_CERT_FILE,
                                            Options::IN_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::OUT_FILE})) {
        return false;
    }

    std::string mode = params->GetString(Options::MODE);
    if (!StringUtils::CaseCompare(mode, LOCAL_SIGN) &&
        !StringUtils::CaseCompare(mode, REMOTE_SIGN)) {
        PrintErrorNumberMsg("COMMAND_ERROR", COMMAND_ERROR, "not support command param '" + mode + "'");
        return false;
    }
    if (StringUtils::CaseCompare(mode, LOCAL_SIGN)) {
        if (!params->Required({params->KEY_STORE_FILE, params->KEY_ALIAS, params->PROFILE_CERT_FILE})) {
            return false;
        }

        if (!FileUtils::ValidFileType(params->GetString(Options::KEY_STORE_FILE), {"p12", "jks"})) {
            return false;
        }
    }
    std::string signAlg = params->GetString(Options::SIGN_ALG);
    if (!CmdUtil::JudgeSignAlgType(signAlg)) {
        return false;
    }
    std::string outFile = params->GetString(Options::OUT_FILE);
    if (FileUtils::ValidFileType(outFile, {"p7b"}) == false) {
        return false;
    }
    return api.SignProfile(params);
}

bool ParamsRunTool::RunVerifyProfile(Options* params, SignToolServiceImpl& api)
{
    if (!params->Required({Options::IN_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckInFile(params, {Options::IN_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::OUT_FILE})) {
        return false;
    }

    if (!FileUtils::ValidFileType(params->GetString(Options::IN_FILE), {"p7b"})) {
        return false;
    }

    std::string outFile = params->GetString(Options::OUT_FILE);
    if (!outFile.empty()) {
        if (!FileUtils::ValidFileType(outFile, {"json"})) {
            return false;
        }
    }
    return api.VerifyProfile(params);
}

void ParamsRunTool::PrintHelp()
{
    PrintMsg(HELP_TXT);
}

void  ParamsRunTool::Version()
{
    PrintMsg(ParamsRunTool::VERSION);
}

bool ParamsRunTool::RunVerifyApp(Options* params, SignToolServiceImpl& api)
{
    if (!params->Required({Options::IN_FILE, Options::OUT_CERT_CHAIN, Options::OUT_PROFILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckInFile(params, {Options::IN_FILE})) {
        return false;
    }

    if (!CmdUtil::UpdateParamForCheckOutFile(params, {Options::OUT_CERT_CHAIN, Options::OUT_PROFILE})) {
        return false;
    }

    std::string inForm = params->GetString(Options::INFORM, ZIP);
    if (!StringUtils::ContainsCase(InformList, inForm)) {
        PrintErrorNumberMsg("NOT_SUPPORT_ERROR", NOT_SUPPORT_ERROR, "parameter '" + inForm
                            + "' format error, Inform only support zip/elf/bin");
        return false;
    }
    if (!FileUtils::ValidFileType(params->GetString(Options::OUT_CERT_CHAIN), {"cer"})) {
        return false;
    }
    if (!FileUtils::ValidFileType(params->GetString(Options::OUT_PROFILE), {"p7b"})) {
        return false;
    }
    return api.VerifyHapSigner(params);
}
} // namespace SignatureTools
} // namespace OHOS