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
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-root-ca-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Application Signature Service CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "./generateCA/OpenHarmony.p12";
    std::string outFile = "./generateCA/subca.cer";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char issuerKeyPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["issuerKeyPwd"] = issuerKeyPwd;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = adaptePtr->GetAliasKey(true);
    X509_REQ* rootcsr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    X509_REQ* subcsr = CertTools::GenerateCsr(keyPair, signAlg, issuer);
    X509* cert = X509_new();
    CertTools::SignForSubCert(cert, subcsr, rootcsr, keyPair, params.get());
    return true;
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