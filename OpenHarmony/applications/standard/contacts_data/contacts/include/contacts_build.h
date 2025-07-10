/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef CONTACT_BUILD_H
#define CONTACT_BUILD_H

#include "ability.h"
#include "ability_context.h"
#include "context.h"
#include "datashare_helper.h"
#include "datashare_predicates.h"
#include "napi/native_common.h"
#include "napi/native_node_api.h"
#include "rdb_errno.h"
#include "rdb_helper.h"
#include "rdb_open_callback.h"
#include "rdb_predicates.h"
#include "rdb_store.h"
#include "result_set.h"

#include "contacts_napi_common.h"
#include "contacts_napi_object.h"
#include "hilog_wrapper_api.h"

namespace OHOS {
namespace ContactsApi {
class ContactsBuild {
public:
    ContactsBuild();
    ~ContactsBuild();
    void GetContactDataByObject(napi_env env, napi_value object, Contacts &contacts);
    int GetInt(napi_env env, napi_value id);
    void GetContactData(napi_env env, napi_value object, std::vector<DataShare::DataShareValuesBucket> &valueContact,
        std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    std::string NapiGetValueString(napi_env env, napi_value value);
    ContactAttributes GetContactAttributes(napi_env env, napi_value object);
    Holder GetHolder(napi_env env, napi_value object);
    void BuildValueContactDataByType(
        Contacts &contacts, int typeId, std::vector<DataShare::DataShareValuesBucket> &valueContactData);

private:
    void TypeSwitchSplit(int typeId, Contacts &contacts,
        std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    napi_value GetArrayByKey(napi_env env, napi_value valueObject, std::string key);
    napi_value GetObjectByKey(napi_env env, napi_value object, std::string key);
    std::string GetStringValueByKey(napi_env env, napi_value valueObject, std::string key);
    int GetIntValueByKey(napi_env env, napi_value valueObject, std::string key);
    Name GetName(napi_env env, napi_value object);
    Portrait GetUri(napi_env env, napi_value object);
    std::vector<Email> GetEmails(napi_env env, napi_value object);
    std::vector<Event> GetEvent(napi_env env, napi_value object);
    std::vector<Group> GetGroup(napi_env env, napi_value object);
    std::vector<ImAddress> GetImAddress(napi_env env, napi_value object);
    std::vector<PhoneNumber> GetPhoneNumbers(napi_env env, napi_value object);
    std::vector<PostalAddress> GetPostalAddress(napi_env env, napi_value object);
    std::vector<Relation> GetRelation(napi_env env, napi_value object);
    std::vector<SipAddress> GetSipAddress(napi_env env, napi_value object);
    std::vector<Website> GetWebsite(napi_env env, napi_value object);
    NickName GetNickName(napi_env env, napi_value object);
    Note GetNote(napi_env env, napi_value object);
    Organization GetOrganization(napi_env env, napi_value object);
    // structure parameter and ValuesBucket
    void BuildValueContact(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContact);
    void BuildValueContactData(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketEmail(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketEvent(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketGroup(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketImAddress(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketPortrait(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketPhoneNumber(Contacts &contacts,
        std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketPostalAddress(Contacts &contacts,
        std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketRelation(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketSipAddress(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketWebsite(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketName(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketNickName(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketNote(Contacts &contacts, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
    void GetValuesBucketOrganization(Contacts &contacts,
        std::vector<DataShare::DataShareValuesBucket> &valueContactData);
};
} // namespace ContactsApi
} // namespace OHOS
#endif // CONTACT_BUILD_H
