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
#include <fstream>

#include "signature_algorithm_helper.h"
#include "verify_hap_openssl_utils.h"
#include "hash_utils.h"
#include "params.h"

namespace OHOS {
namespace SignatureTools {
bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }

    Params param;
    std::string str;
    std::string  algName;
    int algId = 256;
    std::vector<std::string> paramFields;
    std::vector<int8_t> fileBytes;
    int64_t length = 0;
    SignatureAlgorithmHelper out;
    CertChain certsChain;
    Pkcs7Context pkcs7Context;

    param.SetMethod(str);
    param.GetMethod();
    param.GetOptions();
    param.InitParamField(paramFields);
    param.GetSignatureAlgorithm(str, out);
    algName = HashUtils::GetHashAlgName(algId);
    HashUtils::GetDigestFromBytes(fileBytes, length, algName);
    VerifyCertOpensslUtils::VerifyCrl(certsChain, nullptr, pkcs7Context);
    VerifyCertOpensslUtils::GetCrlBySignedCertIssuer(nullptr, nullptr);
    VerifyCertOpensslUtils::GetIssuerFromX509(nullptr, str);

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