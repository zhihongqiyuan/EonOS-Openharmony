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

#include "cj_x500_distinguished_name.h"

int32_t FfiCertCjX500DistinguishedNameNewInstance(const CfBlob *blob, bool bString, CjX500DistinguishedName *returnObj)
{
    auto name = static_cast<HcfX509DistinguishedName *>(malloc(sizeof(HcfX509DistinguishedName)));
    if (name == nullptr) {
        return CF_ERR_MALLOC;
    }
    const auto errCode = HcfX509DistinguishedNameCreate(blob, bString, &name);
    if (errCode != CF_SUCCESS) {
        free(name);
        return errCode;
    }
    returnObj->name = name;
    return CF_SUCCESS;
}

void FfiCertCjX500DistinguishedNameDeleteInstance(CjX500DistinguishedName self)
{
    CfObjDestroy(self.name);
}

CfResult FfiCertCjX500DistinguishedNameGetEncoded(const CjX500DistinguishedName self, CfEncodingBlob *out)
{
    return self.name->getEncode(self.name, out);
}

CfResult FfiCertCjX500DistinguishedNameGetName(const CjX500DistinguishedName self, CfBlob *type, CfBlob *out,
                                               CfArray *outArr)
{
    return self.name->getName(self.name, type, out, outArr);
}
