/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdlib>

#include "cj_certchain_validator.h"

int32_t FfiCertCjCertChainValidatorNewInstance(const char *algorithm, CjCertChainValidator *returnObj)
{
    auto validator = static_cast<HcfCertChainValidator *>(malloc(sizeof(HcfCertChainValidator)));
    if (validator == nullptr) {
        return CF_ERR_MALLOC;
    }
    const auto errCode = HcfCertChainValidatorCreate(algorithm, &validator);
    if (errCode != CF_SUCCESS) {
        free(validator);
        return errCode;
    }
    returnObj->validator = validator;
    return CF_SUCCESS;
}

void FfiCertCjCertChainValidatorDeleteInstance(CjCertChainValidator self)
{
    CfObjDestroy(self.validator);
}

CfResult FfiCertCjCertChainValidatorValidate(const CjCertChainValidator self,
                                             const HcfCertChainData *certChainData)
{
    return self.validator->validate(self.validator, certChainData);
}
