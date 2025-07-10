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

#include "application_context.h"
#include "contacts.h"
#include "contacts_ffi.h"
#include "datashare_values_bucket.h"

using namespace OHOS;
using namespace OHOS::ContactsFfi;

namespace OHOS {
namespace ContactsFfi {

extern "C" {
    int64_t FfiOHOSContactAddContact(int64_t contextId, ContactsFfi::RawContact* rawContact,
                                     ContactsFfi::ContactData* contactData, int32_t *errCode)
    {
        DataShare::DataShareValuesBucket convertedRawContact = convertToDataShareVB(rawContact[0]);
        std::vector<DataShare::DataShareValuesBucket> convertedContact;
        for (uint64_t i = 0; i < contactData->bucketCount; i++) {
            ContactsFfi::ValuesBucket vb = contactData->data[i];
            convertedContact.push_back(convertToDataShareVB(vb));
        }
        return Contacts::CJaddContact(contextId, convertedRawContact, convertedContact, errCode);
    }

    void FfiOHOSContactDeleteContact(int64_t contextId, int64_t predicatesId, int32_t *errCode)
    {
        Contacts::CJdeleteContact(contextId, predicatesId, errCode);
    }

    void FfiOHOSContactUpdateContact(int64_t contextId, int64_t contactId, ContactsFfi::ContactData* contactData,
                                     int64_t predicatesId, int32_t *errCode)
    {
        std::vector<DataShare::DataShareValuesBucket> convertedContact;
        for (uint64_t i = 0; i < contactData->bucketCount; i++) {
            ContactsFfi::ValuesBucket vb = contactData->data[i];
            convertedContact.push_back(convertToDataShareVB(vb));
        }
        Contacts::CJupdateContact(contextId, contactId, convertedContact, predicatesId, errCode);
    }

    bool FfiOHOSContactIsLocalContact(int64_t contextId, int64_t contactId, int32_t *errCode)
    {
        return Contacts::CJisLocalContact(contextId, contactId, errCode);
    }

    bool FfiOHOSContactIsMyCard(int64_t contextId, int64_t contactId, int32_t *errCode)
    {
        return Contacts::CJisMyCard(contextId, contactId, errCode);
    }

    ContactsFfi::ContactsData* FfiOHOSContactQueryMyCard(int64_t contextId, int64_t predicatesId, int32_t *errCode)
    {
        return Contacts::CJqueryMyCard(contextId, predicatesId, errCode);
    }

    ContactsFfi::HoldersData* FfiOHOSContactQueryHolders(int64_t contextId, int32_t *errCode)
    {
        return Contacts::CJqueryHolders(contextId, errCode);
    }

    ContactsFfi::ContactsData* FfiOHOSContactQueryContacts(int64_t contextId, int64_t predicatesId, int32_t *errCode)
    {
        return Contacts::CJqueryContacts(contextId, predicatesId, errCode);
    }

    ContactsFfi::GroupsData* FfiOHOSContactQueryGroups(int64_t contextId, int64_t predicatesId, int32_t *errCode)
    {
        return Contacts::CJqueryGroups(contextId, predicatesId, errCode);
    }
}

} // namespace ContactsFfi
} // namespace OHOS
