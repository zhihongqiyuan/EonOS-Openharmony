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
#include "key_store_helper.h"
#include "cert_tools.h"

namespace OHOS {
namespace SignatureTools {
bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    int keySize = 256;
    std::string algorithm = "ECC";
    std::string signAlgorithm = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    KeyStoreHelper keyStoreHelper;
    EVP_PKEY* keyPair = keyStoreHelper.GenerateKeyPair(algorithm, keySize);
    if (!keyPair) {
        return false;
    }

    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    std::string csrStr;
    if (csr) {
        csrStr = CertTools::CsrToString(csr);
        X509_REQ_free(csr);
    }
    if (keyPair) {
        EVP_PKEY_free(keyPair);
    }
    return csrStr.size() != 0;
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