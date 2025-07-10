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

#include "cj_x509_crl_entry.h"

int32_t FfiCertCjX509CrlEntryNewInstance(HcfX509CrlEntry *crlEntry, CjX509CrlEntry *returnObj)
{
    returnObj->crlEntry = crlEntry;
    return CF_SUCCESS;
}

void FfiCertCjX509CrlEntryDeleteInstance(CjX509CrlEntry self)
{
    CfObjDestroy(self.crlEntry);
}

CfResult FfiCertCjX509CrlEntryGetEncoded(const CjX509CrlEntry self, CfEncodingBlob *encodedOut)
{
    return self.crlEntry->getEncoded(self.crlEntry, encodedOut);
}

CfResult FfiCertCjX509CrlEntryGetSerialNumber(const CjX509CrlEntry self, CfBlob *out)
{
    return self.crlEntry->getSerialNumber(self.crlEntry, out);
}

CfResult FfiCertCjX509CrlEntryGetCertIssuer(const CjX509CrlEntry self, CfBlob *encodedOut)
{
    return self.crlEntry->getCertIssuer(self.crlEntry, encodedOut);
}

CfResult FfiCertCjX509CrlEntryGetRevocationDate(const CjX509CrlEntry self, CfBlob *out)
{
    return self.crlEntry->getRevocationDate(self.crlEntry, out);
}

CfResult FfiCertCjX509CrlEntryGetExtensions(const CjX509CrlEntry self, CfBlob *out)
{
    return self.crlEntry->getExtensions(self.crlEntry, out);
}

CfResult FfiCertCjX509CrlEntryHasExtensions(const CjX509CrlEntry self, bool *out)
{
    return self.crlEntry->hasExtensions(self.crlEntry, out);
}

CfResult FfiCertCjX509CrlEntryToString(const CjX509CrlEntry self, CfBlob *out)
{
    return self.crlEntry->toString(self.crlEntry, out);
}

CfResult FfiCertCjX509CrlEntryHashCode(const CjX509CrlEntry self, CfBlob *out)
{
    return self.crlEntry->hashCode(self.crlEntry, out);
}

CfResult FfiCertCjX509CrlEntryGetExtensionsObject(const CjX509CrlEntry self, CfBlob *out)
{
    return self.crlEntry->getExtensionsObject(self.crlEntry, out);
}
