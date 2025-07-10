/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include "params_run_tool.h"
#include "cmd_util.h"
#include "options.h"
#include "file_utils.h"

namespace OHOS {
namespace SignatureTools {
bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }
    X509* cert = X509_new();
    std::vector<X509*> certChain;
    std::string inputType = "clientAuthentication";
    CmdUtil::VerifyType(inputType);
    Options options;
    std::string key1 = "key1";
    std::string key2 = "key2";
    bool status = true;
    (options)["booltype"] = status;
    options.Equals(key1, key2);
    options.GetBool("booltype");
    std::shared_ptr<SignToolServiceImpl> serviceImplPtr = std::make_shared<SignToolServiceImpl>();
    serviceImplPtr->PrintX509CertFromMemory(cert);
    serviceImplPtr->PrintX509CertChainFromMemory(certChain);
    X509_free(cert);
    CertTools certTool;
    status = certTool.SetSubjectForCert(nullptr, nullptr);
    certTool.GenerateRootCertificate(nullptr, nullptr, &options);
    FileUtils::WriteInputToOutPut("", "");
    FileUtils::DelDir("");

    return true;
}
} // namespace SignatureTools
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::SignatureTools::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}
