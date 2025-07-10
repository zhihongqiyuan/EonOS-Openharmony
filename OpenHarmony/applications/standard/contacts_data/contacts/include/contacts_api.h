/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#ifndef CONTACT_API_H
#define CONTACT_API_H

#include "contacts_build.h"
#include "contacts_napi_object.h"
#include "napi_base_context.h"

namespace OHOS {
namespace ContactsApi {
void Init(napi_env env, napi_value exports);
napi_value DeclareContactConst(napi_env env, napi_value exports);
napi_value DeclareEmailConst(napi_env env, napi_value exports);
napi_value DeclareEventConst(napi_env env, napi_value exports);
napi_value DeclareImAddressConst(napi_env env, napi_value exports);
napi_value DeclarePhoneNumberConst(napi_env env, napi_value exports);
napi_value DeclarePostalAddressConst(napi_env env, napi_value exports);
napi_value DeclareRelationConst(napi_env env, napi_value exports);
napi_value DeclareSipAddressConst(napi_env env, napi_value exports);
napi_value DeclareAttributeConst(napi_env env, napi_value exports);
napi_value AddContact(napi_env env, napi_callback_info info);
napi_value DeleteContact(napi_env env, napi_callback_info info);
napi_value UpdateContact(napi_env env, napi_callback_info info);
napi_value QueryContact(napi_env env, napi_callback_info info);
napi_value QueryContacts(napi_env env, napi_callback_info info);
napi_value QueryContactsByEmail(napi_env env, napi_callback_info info);
napi_value QueryContactsByPhoneNumber(napi_env env, napi_callback_info info);
napi_value QueryGroups(napi_env env, napi_callback_info info);
napi_value QueryHolders(napi_env env, napi_callback_info info);
napi_value QueryKey(napi_env env, napi_callback_info info);
napi_value QueryMyCard(napi_env env, napi_callback_info info);
napi_value IsMyCard(napi_env env, napi_callback_info info);
napi_value IsLocalContact(napi_env env, napi_callback_info info);
void Execute(napi_env env, void *data);
void ExecuteAsync(napi_env env, void *data);
void ExecuteDone(napi_env env, napi_status status, void *data);
void ExecuteSyncDone(napi_env env, napi_status status, void *data);
void HandleExecuteErrorCode(napi_env env, ExecuteHelper *executeHelper, napi_value &errorCode);
void HandleExecuteResult(napi_env env, ExecuteHelper *executeHelper, napi_value &result);
int GetRawIdByResultSet(const std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
napi_value CreateAsyncWork(napi_env env, ExecuteHelper *executeHelper);
void LocalExecute(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteSplit(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteAddContact(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteDeleteContact(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteQueryContact(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteQueryContactsOrKey(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteQueryContactsByData(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteQueryGroup(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteQueryHolders(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteQueryMyCard(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteUpdateContact(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteIsLocalContact(napi_env env, ExecuteHelper *executeHelper);
void LocalExecuteIsMyCard(napi_env env, ExecuteHelper *executeHelper);
napi_value Scheduling(napi_env env, napi_callback_info info, ExecuteHelper *executeHelper, int actionCode);
void VerificationParameterId(napi_env env, napi_value id, ExecuteHelper *executeHelper, napi_value hold);
void VerificationParameterHolderId(napi_env env, ExecuteHelper *executeHelper, napi_value hold);
void SetChildActionCodeAndConvertParams(napi_env env, ExecuteHelper *executeHelper);
DataShare::DataSharePredicates ConvertParamsSwitchSplit(int code, napi_env env, const napi_value &key,
    const napi_value &hold, const napi_value &attr, ExecuteHelper *executeHelper);
bool GetDataShareHelper(napi_env env, napi_callback_info info, ExecuteHelper *executeHelper);
void HolderPredicates(const Holder &holder, DataShare::DataSharePredicates &predicates);
void HoldersStructure(std::map<std::string, std::string> &holders, Holder &holder);
void AttributesPredicates(bool isBegin, ContactAttributes &attrs, DataShare::DataSharePredicates &predicates);
void CheckAttributes(ContactAttributes &attrs);
DataShare::DataSharePredicates BuildQueryContactPredicates(
    napi_env env, std::string key, napi_value hold, napi_value attr);
DataShare::DataSharePredicates BuildUpdateContactPredicates(napi_env env, napi_value contacts, napi_value attr);
DataShare::DataSharePredicates BuildQueryContactData(napi_env env, napi_value &contactsObject,
    napi_value &attrObject, std::vector<DataShare::DataShareValuesBucket> &valueContactData);
std::vector<std::string> BuildUpdateContactColumns();
DataShare::DataSharePredicates BuildUpdateContactConvertParams(
    napi_env env, napi_value &contacts, napi_value &attr, ExecuteHelper &executeHelper);
DataShare::DataSharePredicates BuildQueryContactsPredicates(napi_env env, napi_value hold, napi_value attr);
DataShare::DataSharePredicates BuildQueryContactsByEmailPredicates(
    napi_env env, std::string email, napi_value hold, napi_value attr);
DataShare::DataSharePredicates BuildQueryContactsByPhoneNumberPredicates(
    napi_env env, std::string phoneNumber, napi_value hold, napi_value attr);
DataShare::DataSharePredicates BuildQueryGroupsPredicates(napi_env env, napi_value hold);
DataShare::DataSharePredicates BuildQueryKeyPredicates(napi_env env, int id, napi_value hold);
DataShare::DataSharePredicates BuildQueryMyCardPredicates(napi_env env, napi_value attr);
DataShare::DataSharePredicates BuildIsLocalContactPredicates(napi_env env, napi_value id);
DataShare::DataSharePredicates BuildIsMyCardPredicates(napi_env env, napi_value id);
DataShare::DataSharePredicates BuildDeleteContactDataPredicates(napi_env env, napi_value attr);
DataShare::DataSharePredicates BuildDeleteContactPredicates(napi_env env, ExecuteHelper &executeHelper);
void ObjectInit(napi_env env, napi_value object, napi_value &hold, napi_value &attr, napi_value &contacts);
void ObjectInitId(napi_env env, napi_value object, napi_value &id);
void ObjectInitString(napi_env env, napi_value object, napi_value &key);
int GetType(napi_env env, napi_value value);
} // namespace ContactsApi
} // namespace OHOS

#endif // CONTACT_API_H
