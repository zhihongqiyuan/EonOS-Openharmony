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

    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string rootCaCertFile = "/data/test/generateCA/root-ca1.cer";
    std::string subCaCertFile = "/data/test/generateCA/app-sign-srv-ca1.cer";
    std::string outFile = "/data/test/generateCA/app-release1.pem";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    int validity = 365;
    std::string outForm = "certChain";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::CA_CERT_FILE] = rootCaCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    params[Options::VALIDITY] = validity;
    return api.GenerateAppCert(&params);
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