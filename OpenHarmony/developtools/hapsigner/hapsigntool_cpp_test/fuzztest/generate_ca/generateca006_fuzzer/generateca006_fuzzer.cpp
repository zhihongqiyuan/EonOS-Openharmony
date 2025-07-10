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
#include <cstring>

#include "sign_tool_service_impl.h"
#include <openssl/ssl.h>
#include "options.h"
#include "cert_tools.h"
#include "params_run_tool.h"
#include "localization_adapter.h"


namespace OHOS {
namespace SignatureTools {
bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }

    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string issuerKeyAlias = "oh-root-ca-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Application Signature Service CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keyAlg = "ECC";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    int validity = 365;
    int keySize = 384;
    char keyPwd[] = "123456";
    char ksPwd[] = "123456";
    char issuerkeyPwd[] = "123456";
    std::string outFile = "/data/test/generateCA/app-sign-srv-ca1.cer";
    (*params)["keyPwd"] = keyPwd;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyPwd"] = issuerkeyPwd;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["keyAlg"] = keyAlg;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    (*params)["validity"] = validity;
    return api->GenerateCA(params.get());
}
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::SignatureTools::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}