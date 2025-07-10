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

#include "contacts_api.h"

#include <mutex>

#include "datashare_predicates.h"
#include "rdb_errno.h"
#include "rdb_helper.h"
#include "rdb_open_callback.h"
#include "rdb_predicates.h"
#include "rdb_store.h"
#include "result_set.h"
#include "securec.h"

#include "contacts_control.h"
#include "contacts_napi_common.h"
#include "contacts_napi_utils.h"
#include "hilog_wrapper_api.h"
#include "result_convert.h"
#include "contacts_telephony_permission.h"

namespace OHOS {
namespace ContactsApi {
constexpr int32_t MAXCOUNT = 10;
namespace {
std::mutex g_mutex;
}
/**
 * @brief Initialize NAPI object
 *
 * @param env Conditions for initialize operation
 * @param object Conditions for initialize operation
 * @param hold Attribute of object
 * @param attr Attribute of object
 * @param contact Attribute of object
 */
void ObjectInit(napi_env env, napi_value object, napi_value &hold, napi_value &attr, napi_value &contact)
{
    int type = GetType(env, object);
    switch (type) {
        case TYPE_HOLDER:
            hold = object;
            break;
        case TYPE_ATTR:
            attr = object;
            break;
        case TYPE_CONTACT:
            contact = object;
            break;
        default:
            break;
    }
}

/**
 * @brief Initialize NAPI number object
 *
 * @param env Conditions for initialize operation
 * @param object Conditions for initialize operation
 * @param id Number object
 */
void ObjectInitId(napi_env env, napi_value object, napi_value &id)
{
    int type = GetType(env, object);
    switch (type) {
        case TYPE_NAPI_NUMBER:
            id = object;
            break;
        default:
            break;
    }
}

/**
 * @brief Initialize NAPI string object
 *
 * @param env Conditions for initialize operation
 * @param object Conditions for initialize operation
 * @param key String object
 */
void ObjectInitString(napi_env env, napi_value object, napi_value &key)
{
    int type = GetType(env, object);
    switch (type) {
        case TYPE_NAPI_STRING:
            key = object;
            break;
        default:
            break;
    }
}

/**
 * @brief Get NAPI object type
 *
 * @param env Conditions for get type operation
 * @param value Conditions for get type operation
 *
 * @return The result returned by get type operation
 */
int GetType(napi_env env, napi_value value)
{
    napi_valuetype valueType;
    napi_typeof(env, value, &valueType);
    bool result = false;
    switch (valueType) {
        case napi_number:
            return TYPE_NAPI_NUMBER;
            break;
        case napi_string:
            return TYPE_NAPI_STRING;
            break;
        case napi_object:
            napi_value key;
            napi_create_string_utf8(env, "bundleName", NAPI_AUTO_LENGTH, &key);
            napi_has_property(env, value, key, &result);
            if (result) {
                return TYPE_HOLDER;
            }
            napi_create_string_utf8(env, "attributes", NAPI_AUTO_LENGTH, &key);
            napi_has_property(env, value, key, &result);
            if (result) {
                return TYPE_ATTR;
            }
            return TYPE_CONTACT;
            break;
        default:
            return TYPE_NAPI_ERROR;
            break;
    }
}

/**
 * @brief Get dataShareHelper
 *
 * @param env Conditions for get dataShareHelper operation
 *
 * @return The result returned by get dataShareHelper
 */
bool GetDataShareHelper(napi_env env, napi_callback_info info, ExecuteHelper *executeHelper)
{
    napi_value global;
    bool isStageMode = false;
    napi_value abilityContext = nullptr;
    napi_status status = napi_get_global(env, &global);
    if (executeHelper->abilityContext != nullptr) {
        isStageMode = true;
        status = napi_ok;
        abilityContext = executeHelper->abilityContext;
    } else {
        if (status != napi_ok) {
            HILOG_ERROR("GetDataShareHelper napi_get_global != napi_ok");
        }
        napi_value globalThis;
        status = napi_get_named_property(env, global, "globalThis", &globalThis);
        if (status != napi_ok) {
            HILOG_ERROR("GetDataShareHelper napi_get_globalThis != napi_ok");
        }
        status = napi_get_named_property(env, globalThis, "abilityContext", &abilityContext);
        if (status != napi_ok) {
            HILOG_ERROR("GetDataShareHelper napi_get_abilityContext != napi_ok");
        }
        status = OHOS::AbilityRuntime::IsStageContext(env, abilityContext, isStageMode);
    }

    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = nullptr;    
    if (status != napi_ok || !isStageMode) {
        HILOG_INFO("GetFAModeContext");
        auto ability = OHOS::AbilityRuntime::GetCurrentAbility(env);
        if (ability == nullptr) {
            HILOG_ERROR("Failed to get native ability instance");
            return false;
        }
        auto context = ability->GetContext();
        if (context == nullptr) {
            HILOG_ERROR("Failed to get native context instance");
            return false;
        }
        executeHelper->dataShareHelper = DataShare::DataShareHelper::Creator(context->GetToken(), CONTACTS_DATA_URI);
    } else {
        HILOG_INFO("GetStageModeContext");
        auto context = OHOS::AbilityRuntime::GetStageModeContext(env, abilityContext);
        if (context == nullptr) {
            HILOG_ERROR("Failed to get native stage context instance");
            return false;
        }
        executeHelper->dataShareHelper = DataShare::DataShareHelper::Creator(context->GetToken(), CONTACTS_DATA_URI,
            "", MAXCOUNT);
    }
    return false;
}

/**
 * @brief Establish predicates condition by holder object
 *
 * @param holder Conditions for establish predicates operation
 * @param predicates Conditions for establish predicates operation
 */
void HolderPredicates(Holder &holder, DataShare::DataSharePredicates &predicates)
{
    if (!holder.bundleName.empty()) {
        predicates.And();
        predicates.EqualTo("account_type", holder.bundleName);
    }
    if (!holder.displayName.empty()) {
        predicates.And();
        predicates.EqualTo("account_name", holder.displayName);
    }
    if (holder.holderId > 0) {
        predicates.And();
        predicates.EqualTo("account_id", std::to_string(holder.holderId));
    }
}

/**
 * @brief Establish predicates condition by attributes object
 *
 * @param attrs Conditions for establish predicates operation
 * @param predicates Conditions for establish predicates operation
 */
void AttributesPredicates(bool isBegin, ContactAttributes &attrs, DataShare::DataSharePredicates &predicates)
{
    unsigned int size = attrs.attributes.size();
    if (size > 0) {
        if (!isBegin) {
            predicates.And();
        }
        predicates.BeginWrap();
    }
    for (unsigned int i = 0; i < size; ++i) {
        predicates.EqualTo("type_id", std::to_string(attrs.attributes[i]));
        if (i != size - 1) {
            predicates.Or();
        }
    }
    if (size > 0) {
        predicates.EndWrap();
    }
}

void CheckAttributes(ContactAttributes &attrs)
{
    unsigned int size = attrs.attributes.size();
    if (size == 0) {
        HILOG_INFO("attributes not exist, it means all attribute");
        attrs.attributes.push_back(EMAIL);
        attrs.attributes.push_back(IM);
        attrs.attributes.push_back(NICKNAME);
        attrs.attributes.push_back(ORGANIZATION);
        attrs.attributes.push_back(PHONE);
        attrs.attributes.push_back(NAME);
        attrs.attributes.push_back(POSTAL_ADDRESS);
        attrs.attributes.push_back(PHOTO);
        attrs.attributes.push_back(GROUP_MEMBERSHIP);
        attrs.attributes.push_back(NOTE);
        attrs.attributes.push_back(CONTACT_EVENT);
        attrs.attributes.push_back(WEBSITE);
        attrs.attributes.push_back(RELATION);
        attrs.attributes.push_back(SIP_ADDRESS);
    }
}

/**
 * @brief Resolve object interface in DELETE_CONTACT case
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 */
DataShare::DataSharePredicates BuildDeleteContactPredicates(napi_env env, ExecuteHelper *executeHelper)
{
    DataShare::DataSharePredicates predicates;
    ContactsBuild contactsBuild;
    std::string keyValue = contactsBuild.NapiGetValueString(env, executeHelper->argv[0]);
    if (!keyValue.empty()) {
        predicates.EqualTo("is_deleted", "0");
        predicates.And();
        predicates.EqualTo("quick_search_key", keyValue);
    }
    return predicates;
}

/**
 * @brief Resolve object interface in QUERY_CONTACT case
 *
 * @param env Conditions for resolve object interface operation
 * @param key Conditions for resolve object interface operation
 * @param hold Conditions for resolve object interface operation
 * @param attr Conditions for resolve object interface operation
 */
DataShare::DataSharePredicates BuildQueryContactPredicates(
    napi_env env, napi_value key, napi_value hold, napi_value attr)
{
    ContactsBuild contactsBuild;
    std::string keyValue = contactsBuild.NapiGetValueString(env, key);
    Holder holder = contactsBuild.GetHolder(env, hold);
    DataShare::DataSharePredicates predicates;
    if (!keyValue.empty()) {
        predicates.EqualTo("is_deleted", "0");
        predicates.And();
        predicates.EqualTo("quick_search_key", keyValue);
        HolderPredicates(holder, predicates);
    }
    return predicates;
}

void HoldersStructure(std::map<std::string, std::string> &holders, Holder &holder)
{
    if (!holder.bundleName.empty()) {
        holders["account_type"] = holder.bundleName;
    }
    if (!holder.displayName.empty()) {
        holders["account_name"] = holder.displayName;
    }
    if (holder.holderId > 0) {
        holders["account_id"] = std::to_string(holder.holderId);
    }
}

/**
 * @brief Resolve object interface in QUERY_CONTACTS case
 *
 * @param env Conditions for resolve object interface operation
 * @param hold Conditions for resolve object interface operation
 * @param attr Conditions for resolve object interface operation
 */
DataShare::DataSharePredicates BuildQueryContactsPredicates(napi_env env, napi_value hold, napi_value attr)
{
    ContactsBuild contactsBuild;
    Holder holder = contactsBuild.GetHolder(env, hold);
    ContactAttributes attrs = contactsBuild.GetContactAttributes(env, attr);
    DataShare::DataSharePredicates predicates;
    std::map<std::string, std::string> holders;
    HoldersStructure(holders, holder);
    unsigned int size = attrs.attributes.size();
    unsigned int mapSize = holders.size();
    std::map<std::string, std::string>::iterator it;
    for (it = holders.begin(); it != holders.end(); ++it) {
        predicates.EqualTo(it->first, it->second);
        if (it != --holders.end()) {
            predicates.And();
        }
    }
    if (mapSize > 0) {
        predicates.And();
    }
    if (size > 0) {
        predicates.BeginWrap();
    }
    for (unsigned int i = 0; i < size; ++i) {
        predicates.EqualTo("type_id", std::to_string(attrs.attributes[i]));
        if (i != size - 1) {
            predicates.Or();
        }
    }
    if (size > 0) {
        predicates.EndWrap();
    }
    if (size > 0 || mapSize > 0) {
        predicates.And();
    }
    predicates.EqualTo("is_deleted", "0");
    return predicates;
}

/**
 * @brief Resolve object interface in QUERY_CONTACTS_BY_EMAIL case
 *
 * @param env Conditions for resolve object interface operation
 * @param emailobject Conditions for resolve object interface operation
 * @param hold Conditions for resolve object interface operation
 * @param attr Conditions for resolve object interface operation
 */
DataShare::DataSharePredicates BuildQueryContactsByEmailPredicates(
    napi_env env, napi_value emailobject, napi_value hold, napi_value attr)
{
    ContactsBuild contactsBuild;
    std::string email = contactsBuild.NapiGetValueString(env, emailobject);
    Holder holder = contactsBuild.GetHolder(env, hold);
    DataShare::DataSharePredicates predicates;
    if (!email.empty() || email != "") {
        predicates.EqualTo("is_deleted", "0");
        predicates.And();
        predicates.EqualTo("detail_info", email);
        predicates.And();
        predicates.EqualTo("content_type", "email");
        HolderPredicates(holder, predicates);
    }
    return predicates;
}

/**
 * @brief Resolve object interface in QUERY_CONTACTS_BY_PHONE_NUMBER case
 *
 * @param env Conditions for resolve object interface operation
 * @param number Conditions for resolve object interface operation
 * @param hold Conditions for resolve object interface operation
 * @param attr Conditions for resolve object interface operation
 */
DataShare::DataSharePredicates BuildQueryContactsByPhoneNumberPredicates(
    napi_env env, napi_value number, napi_value hold, napi_value attr)
{
    ContactsBuild contactsBuild;
    std::string phoneNumber = contactsBuild.NapiGetValueString(env, number);
    Holder holder = contactsBuild.GetHolder(env, hold);
    DataShare::DataSharePredicates predicates;
    if (!phoneNumber.empty() || phoneNumber != "") {
        predicates.EqualTo("is_deleted", "0");
        predicates.And();
        predicates.EqualTo("detail_info", phoneNumber);
        predicates.And();
        predicates.EqualTo("content_type", "phone");
        HolderPredicates(holder, predicates);
    }
    return predicates;
}

/**
 * @brief Resolve object interface in QUERY_GROUPS case
 *
 * @param env Conditions for resolve object interface operation
 * @param hold Conditions for resolve object interface operation
 */
DataShare::DataSharePredicates BuildQueryGroupsPredicates(napi_env env, napi_value hold)
{
    ContactsBuild contactsBuild;
    Holder holder = contactsBuild.GetHolder(env, hold);
    DataShare::DataSharePredicates predicates;
    std::map<std::string, std::string> holders;
    HoldersStructure(holders, holder);
    predicates.EqualTo("is_deleted", "0");
    unsigned int size = holders.size();
    if (size > 0) {
        predicates.And();
    }
    std::map<std::string, std::string>::iterator it;
    for (it = holders.begin(); it != holders.end(); ++it) {
        predicates.EqualTo(it->first, it->second);
        if (it != --holders.end()) {
            predicates.And();
        }
    }
    return predicates;
}

/**
 * @brief Resolve object interface in QUERY_KEY case
 *
 * @param env Conditions for resolve object interface operation
 * @param id Conditions for resolve object interface operation
 * @param hold Conditions for resolve object interface operation
 */
DataShare::DataSharePredicates BuildQueryKeyPredicates(napi_env env, napi_value id, napi_value hold)
{
    ContactsBuild contactsBuild;
    int value = contactsBuild.GetInt(env, id);
    Holder holder = contactsBuild.GetHolder(env, hold);
    DataShare::DataSharePredicates predicates;
    if (value != 0) {
        predicates.EqualTo("is_deleted", "0");
        predicates.And();
        predicates.EqualTo("contact_id", std::to_string(value));
        HolderPredicates(holder, predicates);
    }
    return predicates;
}

/**
 * @brief Resolve object interface in QUERY_MY_CARD case
 *
 * @param env Conditions for resolve object interface operation
 * @param attr Conditions for resolve object interface operation
 */
DataShare::DataSharePredicates BuildQueryMyCardPredicates(napi_env env, napi_value attr)
{
    ContactsBuild contactsBuild;
    ContactAttributes attrs = contactsBuild.GetContactAttributes(env, attr);
    DataShare::DataSharePredicates predicates;
    unsigned int size = attrs.attributes.size();
    predicates.EqualTo("is_deleted", "0");
    if (size > 0) {
        predicates.And();
    }
    for (unsigned int i = 0; i < size; ++i) {
        predicates.EqualTo("type_id", std::to_string(attrs.attributes[i]));
        if (i != size - 1) {
            predicates.Or();
        }
    }
    return predicates;
}

DataShare::DataSharePredicates BuildQueryContactData(napi_env env, napi_value &contactObject, napi_value &attrObject,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    ContactsBuild contactsBuild;
    Contacts contact;
    contactsBuild.GetContactDataByObject(env, contactObject, contact);
    ContactAttributes attrs = contactsBuild.GetContactAttributes(env, attrObject);
    CheckAttributes(attrs);
    DataShare::DataSharePredicates predicates;
    std::vector<std::string> fields;
    fields.push_back("raw_contact_id");
    if (contact.id != 0) {
        predicates.EqualTo("is_deleted", "0");
        predicates.And();
        predicates.EqualTo("contact_id", std::to_string(contact.id));
        predicates.GroupBy(fields);
    }
    unsigned int size = attrs.attributes.size();
    for (unsigned int i = 0; i < size; i++) {
        contactsBuild.BuildValueContactDataByType(contact, attrs.attributes[i], valueContactData);
    }
    return predicates;
}

std::vector<std::string> BuildUpdateContactColumns()
{
    std::vector<std::string> columns;
    columns.push_back("raw_contact_id");
    return columns;
}

int GetRawIdByResultSet(const std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (resultSet == nullptr) {
        return -1;
    }
    int resultSetNum = resultSet->GoToFirstRow();
    int intValue = 0;
    while (resultSetNum == OHOS::NativeRdb::E_OK) {
        resultSet->GetInt(0, intValue);
        resultSetNum = resultSet->GoToNextRow();
    }
    resultSet->Close();
    return intValue;
}

DataShare::DataSharePredicates BuildUpdateContactConvertParams(napi_env env, napi_value &contact,
    napi_value &attr, ExecuteHelper *executeHelper)
{
    executeHelper->valueContactData.clear();
    DataShare::DataSharePredicates predicates =
        BuildQueryContactData(env, contact, attr, executeHelper->valueContactData);
    executeHelper->columns = BuildUpdateContactColumns();
    executeHelper->deletePredicates = BuildDeleteContactDataPredicates(env, attr);
    return predicates;
}

DataShare::DataSharePredicates BuildDeleteContactDataPredicates(napi_env env, napi_value attr)
{
    ContactsBuild contactsBuild;
    ContactAttributes attrs = contactsBuild.GetContactAttributes(env, attr);
    CheckAttributes(attrs);
    DataShare::DataSharePredicates predicates;
    AttributesPredicates(true, attrs, predicates);
    return predicates;
}

/**
 * @brief Resolve object interface in IS_LOCAL_CONTACT case
 *
 * @param env Conditions for resolve object interface operation
 * @param id Conditions for resolve object interface operation
 */
DataShare::DataSharePredicates BuildIsLocalContactPredicates(napi_env env, napi_value id)
{
    ContactsBuild contactsBuild;
    int value = contactsBuild.GetInt(env, id);
    DataShare::DataSharePredicates predicates;
    if (value != 0) {
        predicates.EqualTo("is_deleted", "0");
        predicates.And();
        predicates.EqualTo("contact_id", std::to_string(value));
        predicates.And();
        predicates.EqualTo("account_type", "com.ohos.contacts");
        predicates.And();
        predicates.EqualTo("account_name", "phone");
    }
    return predicates;
}

/**
 * @brief Resolve object interface in IS_MY_CARD case
 *
 * @param env Conditions for resolve object interface operation
 * @param id Conditions for resolve object interface operation
 */
DataShare::DataSharePredicates BuildIsMyCardPredicates(napi_env env, napi_value id)
{
    ContactsBuild contactsBuild;
    int value = contactsBuild.GetInt(env, id);
    DataShare::DataSharePredicates predicates;
    if (value != 0) {
        predicates.EqualTo("is_deleted", "0");
        predicates.And();
        predicates.EqualTo("contact_id", std::to_string(value));
    }
    return predicates;
}

void ExecuteDone(napi_env env, napi_status status, void *data)
{
    HILOG_INFO("contactApi ExecuteDone start===>");
    ExecuteHelper *executeHelper = reinterpret_cast<ExecuteHelper *>(data);
    HILOG_INFO("ExecuteDone workName: %{public}d", executeHelper->actionCode);
    napi_value result = nullptr;
    napi_deferred deferred = executeHelper->deferred;
    HandleExecuteResult(env, executeHelper, result);
    if (executeHelper->abilityContext != nullptr) {
        HILOG_INFO("executeHelper->abilityContext != nullptr");
        napi_value errorCode = nullptr;
        HandleExecuteErrorCode(env, executeHelper, errorCode);
        if (errorCode != nullptr) {
            NAPI_CALL_RETURN_VOID(env, napi_reject_deferred(env, deferred, errorCode));
        } else {
            NAPI_CALL_RETURN_VOID(env, napi_resolve_deferred(env, deferred, result));
        }
    } else {
        HILOG_INFO("executeHelper->abilityContext = nullptr");
        NAPI_CALL_RETURN_VOID(env, napi_resolve_deferred(env, deferred, result));
    }
    executeHelper->deferred = nullptr;
    if (executeHelper->valueUpdateContact.capacity() != 0) {
        std::vector<DataShare::DataShareValuesBucket>().swap(executeHelper->valueUpdateContact);
    }
    if (executeHelper->valueContact.capacity() != 0) {
        std::vector<DataShare::DataShareValuesBucket>().swap(executeHelper->valueUpdateContact);
    }
    if (executeHelper->valueContactData.capacity() != 0) {
        std::vector<DataShare::DataShareValuesBucket>().swap(executeHelper->valueUpdateContact);
    }
    
    NAPI_CALL_RETURN_VOID(env, napi_delete_async_work(env, executeHelper->work));
    if (executeHelper->dataShareHelper != nullptr) {
        executeHelper->dataShareHelper->Release();
        executeHelper->dataShareHelper = nullptr;
    }
    delete executeHelper;
    executeHelper = nullptr;
}

void ExecuteSyncDone(napi_env env, napi_status status, void *data)
{
    HILOG_INFO("contactApi ExecuteSyncDone start===>");
    if (status != napi_ok) {
        HILOG_ERROR("ExecuteSyncDone status is not ok===>");
        return;
    }
    if (data != nullptr) {
        ExecuteHelper *executeHelper = reinterpret_cast<ExecuteHelper *>(data);
        HILOG_INFO("ExecuteSyncDone workName: %{public}d", executeHelper->actionCode);
        napi_value global;
        napi_get_global(env, &global);
        napi_value resultData[RESULT_DATA_SIZE];
        if (executeHelper->abilityContext != nullptr) {
            HandleExecuteErrorCode(env, executeHelper, resultData[0]);
            HandleExecuteResult(env, executeHelper, resultData[1]);
        } else {
            if (executeHelper->resultData < 0) {
                HandleExecuteErrorCode(env, executeHelper, resultData[0]);
                HandleExecuteResult(env, executeHelper, resultData[1]);
            } else {
                napi_get_undefined(env, &resultData[0]);
                HandleExecuteResult(env, executeHelper, resultData[1]);
            }
        }
        
        napi_value result;
        napi_value callBack;
        napi_get_reference_value(env, executeHelper->callBack, &callBack);
        napi_valuetype valuetype = napi_undefined;
        napi_typeof(env, callBack, &valuetype);
        if (valuetype != napi_function) {
            HILOG_ERROR("contactApi params not is function");
            return;
        }
        napi_call_function(env, global, callBack, RESULT_DATA_SIZE, resultData, &result);
        if (executeHelper->work != nullptr) {
            napi_delete_async_work(env, executeHelper->work);
        }
        executeHelper->work = nullptr;
        executeHelper->deferred = nullptr;
        if (executeHelper->valueUpdateContact.capacity() != 0) {
            std::vector<DataShare::DataShareValuesBucket>().swap(executeHelper->valueUpdateContact);
        }
        if (executeHelper->valueContact.capacity() != 0) {
            std::vector<DataShare::DataShareValuesBucket>().swap(executeHelper->valueUpdateContact);
        }
        if (executeHelper->valueContactData.capacity() != 0) {
            std::vector<DataShare::DataShareValuesBucket>().swap(executeHelper->valueUpdateContact);
        }
        if (executeHelper->dataShareHelper != nullptr) {
            executeHelper->dataShareHelper->Release();
            executeHelper->dataShareHelper = nullptr;
        }
        delete executeHelper;
        executeHelper = nullptr;
    }
    HILOG_INFO("contactApi ExecuteSyncDone done===>");
}

void HandleExecuteErrorCode(napi_env env, ExecuteHelper *executeHelper, napi_value &result)
{
    HILOG_INFO("HandleExecuteErrorCode");
    ResultConvert resultConvert;
    switch (executeHelper->actionCode) {
        case ADD_CONTACT:
        case DELETE_CONTACT:
        case UPDATE_CONTACT:
        case SELECT_CONTACT:
        case IS_LOCAL_CONTACT:
        case IS_MY_CARD:
        case QUERY_CONTACT:
        case QUERY_MY_CARD:
        case QUERY_KEY:
        case QUERY_CONTACTS:
        case QUERY_CONTACTS_BY_EMAIL:
        case QUERY_CONTACTS_BY_PHONE_NUMBER:
        case QUERY_GROUPS:
        case QUERY_HOLDERS:
            HILOG_INFO("HandleExecuteErrorCode resultData");
            if (executeHelper->resultData == RDB_PARAMETER_ERROR || executeHelper->resultData == ERROR) {
                HILOG_ERROR("handleExecuteErrorCode handle parm error");
                result = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
            } else if (executeHelper->resultData == VERIFICATION_PERMISSION_ERROR) {
                HILOG_ERROR("parameter verification failed error");
                result = ContactsNapiUtils::CreateErrorByVerification(env, PARAMETER_ERROR);
            } else if (executeHelper->resultData == RDB_PERMISSION_ERROR) {
                HILOG_ERROR("permission error");
                result = ContactsNapiUtils::CreateError(env, PERMISSION_ERROR);
            }
            break;
        default:
            break;
    }
}

void HandleExecuteResult(napi_env env, ExecuteHelper *executeHelper, napi_value &result)
{
    ResultConvert resultConvert;
    napi_value results = nullptr;
    switch (executeHelper->actionCode) {
        case ADD_CONTACT:
        case DELETE_CONTACT:
        case UPDATE_CONTACT:
        case SELECT_CONTACT:
            if (executeHelper->resultData == RDB_PERMISSION_ERROR) {
                napi_create_int64(env, ERROR, &result);
            } else {
                napi_create_int64(env, executeHelper->resultData, &result);
            }
            break;
        case IS_LOCAL_CONTACT:
        case IS_MY_CARD:
            napi_get_boolean(env, executeHelper->resultData != 0, &result);
            break;
        case QUERY_CONTACT:
        case QUERY_MY_CARD:
            results = resultConvert.ResultSetToObject(env, executeHelper->resultSet);
            if (results != nullptr) {
                napi_get_element(env, results, 0, &result);
            }
            break;
        case QUERY_KEY:
            results = resultConvert.ResultSetToObject(env, executeHelper->resultSet);
            if (results != nullptr) {
                napi_get_element(env, results, 0, &result);
            }
            napi_get_named_property(env, result, "key", &result);
            break;
        case QUERY_CONTACTS:
        case QUERY_CONTACTS_BY_EMAIL:
        case QUERY_CONTACTS_BY_PHONE_NUMBER:
            result = resultConvert.ResultSetToObject(env, executeHelper->resultSet);
            break;
        case QUERY_GROUPS:
            result = resultConvert.ResultSetToGroup(env, executeHelper->resultSet);
            break;
        case QUERY_HOLDERS:
            result = resultConvert.ResultSetToHolder(env, executeHelper->resultSet);
            break;
        default:
            break;
    }
}

void LocalExecuteAddContact(napi_env env, ExecuteHelper *executeHelper)
{
    if (executeHelper->valueContactData.empty()) {
        HILOG_ERROR("addContact contact_data can not be empty");
        executeHelper->resultData = ERROR;
        return;
    }
    ContactsControl contactsControl;
    int rawId = contactsControl.RawContactInsert(
        executeHelper->dataShareHelper, (executeHelper->valueContact)[0]);
    std::vector<DataShare::DataShareValuesBucket> value = executeHelper->valueContactData;
    unsigned int size = value.size();
    for (unsigned int i = 0; i < size; ++i) {
        (executeHelper->valueContactData)[i].Put("raw_contact_id", rawId);
    }
    int code = contactsControl.ContactDataInsert(executeHelper->dataShareHelper, executeHelper->valueContactData);
    if (code == 0) {
        executeHelper->resultData = rawId;
    } else {
        executeHelper->resultData = code;
    }
}

void LocalExecuteDeleteContact(napi_env env, ExecuteHelper *executeHelper)
{
    ContactsControl contactsControl;
    int ret = contactsControl.ContactDelete(executeHelper->dataShareHelper, executeHelper->predicates);
    HILOG_INFO("LocalExecuteDeleteContact contact ret = %{public}d", ret);
    executeHelper->resultData = ret;
}

void LocalExecuteQueryContact(napi_env env, ExecuteHelper *executeHelper)
{
    ContactsControl contactsControl;
    executeHelper->resultSet = contactsControl.ContactQuery(
        executeHelper->dataShareHelper, executeHelper->columns, executeHelper->predicates);
    executeHelper->resultData = SUCCESS;
}

void LocalExecuteQueryContactsOrKey(napi_env env, ExecuteHelper *executeHelper)
{
    ContactsControl contactsControl;
    executeHelper->resultSet = contactsControl.ContactQuery(
        executeHelper->dataShareHelper, executeHelper->columns, executeHelper->predicates);
    executeHelper->resultData = SUCCESS;
}

void LocalExecuteQueryContactsByData(napi_env env, ExecuteHelper *executeHelper)
{
    ContactsControl contactsControl;
    executeHelper->resultSet = contactsControl.ContactDataQuery(
        executeHelper->dataShareHelper, executeHelper->columns, executeHelper->predicates);
    std::shared_ptr<DataShare::DataShareResultSet> resultSet = executeHelper->resultSet;
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    if (rowCount == 0) {
        executeHelper->resultData = RDB_PARAMETER_ERROR;
        resultSet->Close();
    } else {
        executeHelper->resultData = SUCCESS;
    }
}

void LocalExecuteQueryGroup(napi_env env, ExecuteHelper *executeHelper)
{
    ContactsControl contactsControl;
    executeHelper->resultSet = contactsControl.GroupsQuery(
        executeHelper->dataShareHelper, executeHelper->columns, executeHelper->predicates);
    executeHelper->resultData = SUCCESS;
}

void LocalExecuteQueryHolders(napi_env env, ExecuteHelper *executeHelper)
{
    ContactsControl contactsControl;
    executeHelper->resultSet = contactsControl.HolderQuery(
        executeHelper->dataShareHelper, executeHelper->columns, executeHelper->predicates);
    executeHelper->resultData = SUCCESS;
}

void LocalExecuteQueryMyCard(napi_env env, ExecuteHelper *executeHelper)
{
    ContactsControl contactsControl;
    executeHelper->resultSet = contactsControl.MyCardQuery(
        executeHelper->dataShareHelper, executeHelper->columns, executeHelper->predicates);
    executeHelper->resultData = SUCCESS;
}

void LocalExecuteUpdateContact(napi_env env, ExecuteHelper *executeHelper)
{
    ContactsControl contactsControl;
    // query raw_contact_id
    std::shared_ptr<DataShare::DataShareResultSet> resultSet = contactsControl.ContactDataQuery(
        executeHelper->dataShareHelper, executeHelper->columns, executeHelper->predicates);
    int rawId = GetRawIdByResultSet(resultSet);
    if (rawId == 0) {
        HILOG_ERROR("LocalExecuteUpdateContact contact rawId equals 0");
        executeHelper->resultData = ERROR;
        return;
    }
    std::vector<DataShare::DataShareValuesBucket> value = executeHelper->valueContactData;
    unsigned int size = value.size();
    for (unsigned int i = 0; i < size; ++i) {
        (executeHelper->valueContactData)[i].Put("raw_contact_id", rawId);
    }
    executeHelper->deletePredicates.And();
    executeHelper->deletePredicates.EqualTo("raw_contact_id", std::to_string(rawId));
    int resultCode = contactsControl.ContactDataDelete(
        executeHelper->dataShareHelper, executeHelper->deletePredicates);
    if (resultCode >= 0) {
        resultCode = contactsControl.ContactDataInsert(
            executeHelper->dataShareHelper, executeHelper->valueContactData);
    }
    executeHelper->resultData = resultCode;
}

void LocalExecuteIsLocalContact(napi_env env, ExecuteHelper *executeHelper)
{
    int64_t isLocal = 0;
    ContactsControl contactsControl;
    std::shared_ptr<DataShare::DataShareResultSet> resultSet = contactsControl.ContactQuery(
        executeHelper->dataShareHelper, executeHelper->columns, executeHelper->predicates);
    if (resultSet == nullptr) {
        executeHelper->resultData = isLocal;
        return;
    }
    int resultSetNum = resultSet->GoToFirstRow();
    if (resultSetNum == OHOS::NativeRdb::E_OK) {
        isLocal = 1;
    }
    executeHelper->resultData = isLocal;
    resultSet->Close();
}

void LocalExecuteIsMyCard(napi_env env, ExecuteHelper *executeHelper)
{
    int64_t isMyCard = 0;
    ContactsControl contactsControl;
    std::shared_ptr<DataShare::DataShareResultSet> resultSet = contactsControl.MyCardQuery(
        executeHelper->dataShareHelper, executeHelper->columns, executeHelper->predicates);
    if (resultSet == nullptr) {
        executeHelper->resultData = isMyCard;
        return;
    }
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    int resultSetNum = resultSet->GoToFirstRow();
    if (resultSetNum == OHOS::NativeRdb::E_OK) {
        isMyCard = 1;
    }
    executeHelper->resultData = isMyCard;
    resultSet->Close();
}

void LocalExecute(napi_env env, ExecuteHelper *executeHelper)
{
    if (executeHelper->dataShareHelper == nullptr) {
        HILOG_ERROR("create dataShareHelper is null, please check your permission");
        executeHelper->resultData = RDB_PERMISSION_ERROR;
        return;
    }
    switch (executeHelper->actionCode) {
        case ADD_CONTACT:
            LocalExecuteAddContact(env, executeHelper);
            break;
        case DELETE_CONTACT:
            LocalExecuteDeleteContact(env, executeHelper);
            break;
        case UPDATE_CONTACT:
            LocalExecuteUpdateContact(env, executeHelper);
            break;
        default:
            LocalExecuteSplit(env, executeHelper);
            HILOG_INFO("LocalExecute case error===>");
            break;
    }
}

void LocalExecuteSplit(napi_env env, ExecuteHelper *executeHelper)
{
    ContactsTelephonyPermission permission;
    if (!permission.CheckPermission(ContactsApi::Permission::READ_CONTACTS)) {
        HILOG_ERROR("LocalExecuteQueryContactsByData Permission denied!");
        executeHelper->resultData = RDB_PERMISSION_ERROR;
        return;
    } else if (executeHelper->resultData == VERIFICATION_PERMISSION_ERROR) {
        HILOG_ERROR("PARAMETER_ERROR, please check your PARAMETER");
        return;
    }
    switch (executeHelper->actionCode) {
        case QUERY_CONTACT:
            LocalExecuteQueryContact(env, executeHelper);
            break;
        case QUERY_CONTACTS:
            LocalExecuteQueryContactsOrKey(env, executeHelper);
            break;
        case QUERY_CONTACTS_BY_EMAIL:
        case QUERY_CONTACTS_BY_PHONE_NUMBER:
            LocalExecuteQueryContactsByData(env, executeHelper);
            break;
        case QUERY_GROUPS:
            LocalExecuteQueryGroup(env, executeHelper);
            break;
        case QUERY_HOLDERS:
            LocalExecuteQueryHolders(env, executeHelper);
            break;
        case QUERY_KEY:
            LocalExecuteQueryContactsOrKey(env, executeHelper);
            break;
        case QUERY_MY_CARD:
            LocalExecuteQueryMyCard(env, executeHelper);
            break;
        case IS_LOCAL_CONTACT:
            LocalExecuteIsLocalContact(env, executeHelper);
            break;
        case IS_MY_CARD:
            LocalExecuteIsMyCard(env, executeHelper);
            break;
        default:
            HILOG_INFO("LocalExecute case error===>");
            break;
    }
}

void Execute(napi_env env, void *data)
{
    ExecuteHelper *executeHelper = static_cast<ExecuteHelper *>(data);
    HILOG_INFO("Execute start workName: %{public}d", executeHelper->actionCode);
    LocalExecute(env, executeHelper);
}

napi_value CreateAsyncWork(napi_env env, ExecuteHelper *executeHelper)
{
    napi_value workName;
    napi_value result = nullptr;
    if (executeHelper->sync == NAPI_CALL_TYPE_CALLBACK) {
        napi_create_string_latin1(env, __func__, NAPI_AUTO_LENGTH, &workName);
        napi_create_reference(env, executeHelper->argv[executeHelper->argc - 1], 1, &executeHelper->callBack);
        napi_create_async_work(env, nullptr, workName, Execute, ExecuteSyncDone,
            reinterpret_cast<void *>(executeHelper), &(executeHelper->work));
        napi_get_null(env, &result);
    } else {
        napi_create_string_latin1(env, __func__, NAPI_AUTO_LENGTH, &workName);
        napi_create_promise(env, &(executeHelper->deferred), &result);
        napi_create_async_work(env, nullptr, workName, Execute, ExecuteDone,
            reinterpret_cast<void *>(executeHelper), &(executeHelper->work));
    }
    napi_queue_async_work(env, executeHelper->work);
    executeHelper->promise = result;
    return result;
}

DataShare::DataSharePredicates ConvertParamsSwitchSplit(int code, napi_env env, const napi_value &key,
    const napi_value &hold, const napi_value &attr, ExecuteHelper *executeHelper)
{
    DataShare::DataSharePredicates predicates;
    switch (code) {
        case QUERY_CONTACT:
            VerificationParameterHolderId(env, executeHelper, hold);
            predicates = BuildQueryContactPredicates(env, key, hold, attr);
            break;
        case QUERY_CONTACTS:
            VerificationParameterHolderId(env, executeHelper, hold);
            predicates = BuildQueryContactsPredicates(env, hold, attr);
            break;
        case QUERY_CONTACTS_BY_EMAIL:
            VerificationParameterHolderId(env, executeHelper, hold);
            predicates = BuildQueryContactsByEmailPredicates(env, key, hold, attr);
            break;
        case QUERY_CONTACTS_BY_PHONE_NUMBER:
            VerificationParameterHolderId(env, executeHelper, hold);
            predicates = BuildQueryContactsByPhoneNumberPredicates(env, key, hold, attr);
            break;
        case QUERY_GROUPS:
            VerificationParameterHolderId(env, executeHelper, hold);
            predicates = BuildQueryGroupsPredicates(env, hold);
            break;
        case QUERY_HOLDERS:
            break;
        case QUERY_MY_CARD:
            predicates = BuildQueryMyCardPredicates(env, attr);
            break;
        default:
            HILOG_ERROR("ConvertParamsSwitchSplit code is no match");
            break;
    }
    return predicates;
}

void SetChildActionCodeAndConvertParams(napi_env env, ExecuteHelper *executeHelper)
{
    napi_value id = nullptr;
    napi_value key = nullptr;
    napi_value hold = nullptr;
    napi_value attr = nullptr;
    napi_value contact = nullptr;
    unsigned int size = executeHelper->argc;
    for (unsigned int i = 0; i < size; i++) {
        ObjectInitId(env, executeHelper->argv[i], id);
        ObjectInitString(env, executeHelper->argv[i], key);
        ObjectInit(env, executeHelper->argv[i], hold, attr, contact);
    }
    ContactsBuild contactsBuild;
    switch (executeHelper->actionCode) {
        case ADD_CONTACT:
            contactsBuild.GetContactData(
                env, executeHelper->argv[0], executeHelper->valueContact, executeHelper->valueContactData);
            break;
        case DELETE_CONTACT:
            executeHelper->predicates = BuildDeleteContactPredicates(env, executeHelper);
            break;
        case UPDATE_CONTACT:
            executeHelper->predicates = BuildUpdateContactConvertParams(env, contact, attr, executeHelper);
            break;
        case IS_LOCAL_CONTACT:
            VerificationParameterId(env, id, executeHelper, hold);
            executeHelper->predicates = BuildIsLocalContactPredicates(env, id);
            break;
        case IS_MY_CARD:
            VerificationParameterId(env, id, executeHelper, hold);
            executeHelper->predicates = BuildIsMyCardPredicates(env, id);
            break;
        case QUERY_KEY:
            VerificationParameterId(env, id, executeHelper, hold);
            executeHelper->predicates = BuildQueryKeyPredicates(env, id, hold);
            break;
        default:
            executeHelper->predicates = ConvertParamsSwitchSplit(executeHelper->actionCode, env, key, hold,
                attr, executeHelper);
            break;
    }
}

void VerificationParameterId(napi_env env, napi_value id, ExecuteHelper *executeHelper, napi_value hold)
{
    ContactsBuild contactsBuild;
    Holder holder = contactsBuild.GetHolder(env, hold);
    int holderId = holder.holderId;
    int valueId = contactsBuild.GetInt(env, id);
    if (valueId <= 0 || isinf(valueId)) {
        executeHelper->resultData = VERIFICATION_PERMISSION_ERROR;
        HILOG_ERROR("PARAMETER_ERROR valueId: %{public}d", valueId);
    } else if (hold != nullptr && holderId != 1) {
        executeHelper->resultData = VERIFICATION_PERMISSION_ERROR;
        HILOG_ERROR("PARAMETER_ERROR holderId: %{public}d", holderId);
    }
}

void VerificationParameterHolderId(napi_env env, ExecuteHelper *executeHelper, napi_value hold)
{
    ContactsBuild contactsBuild;
    Holder holder = contactsBuild.GetHolder(env, hold);
    int holderId = holder.holderId;
    if (hold != nullptr && holderId != 1) {
        executeHelper->resultData = VERIFICATION_PERMISSION_ERROR;
        HILOG_ERROR("PARAMETER_ERROR holderId: %{public}d", holderId);
    }
}

napi_value Scheduling(napi_env env, napi_callback_info info, ExecuteHelper *executeHelper, int actionCode)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    executeHelper->argc = argc;
    executeHelper->actionCode = actionCode;

    if (argc > 0) {
        napi_valuetype valuetype = napi_undefined;
        napi_typeof(env, argv[argc - 1], &valuetype);
        // last params is function as callback
        if (valuetype == napi_function) {
            executeHelper->sync = NAPI_CALL_TYPE_CALLBACK;
        } else {
            executeHelper->sync = NAPI_CALL_TYPE_PROMISE;
        }
    }
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        HILOG_INFO("use API 10 interface");
        for (int i = 1; i < MAX_PARAMS; i++) {
            executeHelper->argv[i - 1] = argv[i];
        }
        executeHelper->abilityContext = argv[0];
        executeHelper->argc -= 1;
    } else {
        HILOG_INFO("use API 7 interface");
        for (int i = 0; i < MAX_PARAMS; i++) {
            executeHelper->argv[i] = argv[i];
        }
    }

    SetChildActionCodeAndConvertParams(env, executeHelper);
    GetDataShareHelper(env, info, executeHelper);

    napi_value result = CreateAsyncWork(env, executeHelper);
    return result;
}

/**
 * @brief Test interface ADD_CONTACT
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value AddContact(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, ADD_CONTACT);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface DELETE_CONTACT
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value DeleteContact(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_string })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_string, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, DELETE_CONTACT);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface UPDATE_CONTACT
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value UpdateContact(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object, napi_function })
                    && !ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object, napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_FOUR:
                if (!ContactsNapiUtils::MatchParameters(env, argv,
                    { napi_object, napi_object, napi_object, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, UPDATE_CONTACT);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface QUERY_CONTACT
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value QueryContact(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_string })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_string, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_FOUR:
                if (!ContactsNapiUtils::MatchParameters(env, argv,
                    { napi_object, napi_string, napi_object, napi_function }) &&
                    !ContactsNapiUtils::MatchParameters(env, argv,
                    { napi_object, napi_string, napi_object, napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_FIVE:
                if (!ContactsNapiUtils::MatchParameters(env, argv,
                    {
                        napi_object, napi_string, napi_object, napi_object, napi_function
                    })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, QUERY_CONTACT);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface QUERY_CONTACTS
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value QueryContacts(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_ONE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object, napi_function })
                && !ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object, napi_object })
                ) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_FOUR:
                if (!ContactsNapiUtils::MatchParameters(env, argv,
                    { napi_object, napi_object, napi_object, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, QUERY_CONTACTS);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface QUERY_CONTACTS_BY_EMAIL
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value QueryContactsByEmail(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_string })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_string, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_FOUR:
                if (!ContactsNapiUtils::MatchParameters(env, argv,
                    { napi_object, napi_string, napi_object, napi_function }) &&
                    !ContactsNapiUtils::MatchParameters(env, argv,
                    { napi_object, napi_string, napi_object, napi_function }) &&
                    !ContactsNapiUtils::MatchParameters(env, argv,
                    { napi_object, napi_string, napi_object, napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_FIVE:
                if (!ContactsNapiUtils::MatchParameters(env, argv,
                    {
                        napi_object, napi_string, napi_object, napi_object, napi_function
                    })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, QUERY_CONTACTS_BY_EMAIL);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface QUERY_CONTACTS_BY_PHONE_NUMBER
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value QueryContactsByPhoneNumber(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_string })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_string, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_FOUR:
                if (!ContactsNapiUtils::MatchParameters(env, argv,
                    { napi_object, napi_string, napi_object, napi_function }) &&
                    !ContactsNapiUtils::MatchParameters(env, argv,
                    { napi_object, napi_string, napi_object, napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_FIVE:
                if (!ContactsNapiUtils::MatchParameters(env, argv,
                    {
                        napi_object, napi_string, napi_object, napi_object, napi_function
                    })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, QUERY_CONTACTS_BY_PHONE_NUMBER);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface QUERY_CONTACTS_BY_PHONE_NUMBER
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value QueryGroups(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_ONE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_function })
                    && !ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, QUERY_GROUPS);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface QUERY_HOLDERS
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value QueryHolders(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_ONE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, QUERY_HOLDERS);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface QUERY_KEY
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value QueryKey(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_ONE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_function })
                && !ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_number })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_number, napi_function })
                && !ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_number, napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_FOUR:
                if (!ContactsNapiUtils::MatchParameters(env, argv,
                    { napi_object, napi_number, napi_object, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, QUERY_KEY);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface QUERY_MY_CARD
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value QueryMyCard(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_ONE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object })
                    && !ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_object, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, QUERY_MY_CARD);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface IS_MY_CARD
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value IsMyCard(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_number })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_number, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, IS_MY_CARD);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

/**
 * @brief Test interface IS_LOCAL_CONTACT
 *
 * @param env Conditions for resolve object interface operation
 * @param info Conditions for resolve object interface operation
 *
 * @return The result returned by test
 */
napi_value IsLocalContact(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_PARAMS;
    napi_value argv[MAX_PARAMS] = {0};
    napi_value thisVar = nullptr;
    void *data;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    bool isStageMode = false;
    OHOS::AbilityRuntime::IsStageContext(env, argv[0], isStageMode);
    if (isStageMode) {
        napi_value errorCode = ContactsNapiUtils::CreateError(env, PARAMETER_ERROR);
        switch (argc) {
            case ARGS_TWO:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_number })) {
                    napi_throw(env, errorCode);
                }
                break;
            case ARGS_THREE:
                if (!ContactsNapiUtils::MatchParameters(env, argv, { napi_object, napi_number, napi_function })) {
                    napi_throw(env, errorCode);
                }
                break;
            default:
                napi_throw(env, errorCode);
                break;
        }
    }
    ExecuteHelper *executeHelper = new (std::nothrow) ExecuteHelper();
    napi_value result = nullptr;
    if (executeHelper != nullptr) {
        result = Scheduling(env, info, executeHelper, IS_LOCAL_CONTACT);
        return result;
    }
    napi_create_int64(env, ERROR, &result);
    return result;
}

napi_value DeclareContactConst(napi_env env, napi_value exports)
{
    // Contact
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("INVALID_CONTACT_ID",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Contacts::INVALID_CONTACT_ID))),
    };
    napi_value result = nullptr;
    napi_define_class(env, "Contact", NAPI_AUTO_LENGTH, ContactsNapiUtils::CreateClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result);
    napi_set_named_property(env, exports, "Contact", result);
    return exports;
}

napi_value DeclareEmailConst(napi_env env, napi_value exports)
{
    // Email
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("CUSTOM_LABEL",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Email::CUSTOM_LABEL))),
        DECLARE_NAPI_STATIC_PROPERTY("EMAIL_HOME",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Email::EMAIL_HOME))),
        DECLARE_NAPI_STATIC_PROPERTY("EMAIL_WORK",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Email::EMAIL_WORK))),
        DECLARE_NAPI_STATIC_PROPERTY("EMAIL_OTHER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Email::EMAIL_OTHER))),
        DECLARE_NAPI_STATIC_PROPERTY("INVALID_LABEL_ID",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Email::INVALID_LABEL_ID))),
    };
    napi_value result = nullptr;
    napi_define_class(env, "Email", NAPI_AUTO_LENGTH, ContactsNapiUtils::CreateClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result);
    napi_set_named_property(env, exports, "Email", result);
    return exports;
}

napi_value DeclareEventConst(napi_env env, napi_value exports)
{
    // Event
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("CUSTOM_LABEL",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Event::CUSTOM_LABEL))),
        DECLARE_NAPI_STATIC_PROPERTY("EVENT_ANNIVERSARY",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Event::EVENT_ANNIVERSARY))),
        DECLARE_NAPI_STATIC_PROPERTY("EVENT_OTHER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Event::EVENT_OTHER))),
        DECLARE_NAPI_STATIC_PROPERTY("EVENT_BIRTHDAY",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Event::EVENT_BIRTHDAY))),
        DECLARE_NAPI_STATIC_PROPERTY("INVALID_LABEL_ID",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Event::INVALID_LABEL_ID))),
    };
    napi_value result = nullptr;
    napi_define_class(env, "Event", NAPI_AUTO_LENGTH, ContactsNapiUtils::CreateClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result);
    napi_set_named_property(env, exports, "Event", result);
    return exports;
}

napi_value DeclareImAddressConst(napi_env env, napi_value exports)
{
    // ImAddress
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("CUSTOM_LABEL",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(ImAddress::CUSTOM_LABEL))),
        DECLARE_NAPI_STATIC_PROPERTY("IM_AIM",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(ImAddress::IM_AIM))),
        DECLARE_NAPI_STATIC_PROPERTY("IM_MSN",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(ImAddress::IM_MSN))),
        DECLARE_NAPI_STATIC_PROPERTY("IM_YAHOO",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(ImAddress::IM_YAHOO))),
        DECLARE_NAPI_STATIC_PROPERTY("IM_SKYPE",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(ImAddress::IM_SKYPE))),
        DECLARE_NAPI_STATIC_PROPERTY("IM_QQ",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(ImAddress::IM_QQ))),
        DECLARE_NAPI_STATIC_PROPERTY("IM_ICQ",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(ImAddress::IM_ICQ))),
        DECLARE_NAPI_STATIC_PROPERTY("IM_JABBER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(ImAddress::IM_JABBER))),
        DECLARE_NAPI_STATIC_PROPERTY("INVALID_LABEL_ID",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(ImAddress::INVALID_LABEL_ID))),
    };
    napi_value result = nullptr;
    napi_define_class(env, "ImAddress", NAPI_AUTO_LENGTH, ContactsNapiUtils::CreateClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result);
    napi_set_named_property(env, exports, "ImAddress", result);
    return exports;
}

napi_value DeclarePhoneNumberConst(napi_env env, napi_value exports)
{
    // PhoneNumber
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("CUSTOM_LABEL",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::CUSTOM_LABEL))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_HOME",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_HOME))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_MOBILE",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_MOBILE))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_WORK",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_WORK))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_FAX_WORK",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_FAX_WORK))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_FAX_HOME",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_FAX_HOME))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_PAGER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_PAGER))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_OTHER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_OTHER))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_CALLBACK",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_CALLBACK))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_CAR",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_CAR))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_COMPANY_MAIN",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_COMPANY_MAIN))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_ISDN",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_ISDN))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_MAIN",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_MAIN))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_OTHER_FAX",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_OTHER_FAX))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_RADIO",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_RADIO))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_TELEX",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_TELEX))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_TTY_TDD",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_TTY_TDD))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_WORK_MOBILE",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_WORK_MOBILE))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_WORK_PAGER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_WORK_PAGER))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_ASSISTANT",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_ASSISTANT))),
        DECLARE_NAPI_STATIC_PROPERTY("NUM_MMS",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::NUM_MMS))),
        DECLARE_NAPI_STATIC_PROPERTY("INVALID_LABEL_ID",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PhoneNumber::INVALID_LABEL_ID))),
    };
    napi_value result = nullptr;
    napi_define_class(env, "PhoneNumber", NAPI_AUTO_LENGTH, ContactsNapiUtils::CreateClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result);
    napi_set_named_property(env, exports, "PhoneNumber", result);
    return exports;
}

napi_value DeclarePostalAddressConst(napi_env env, napi_value exports)
{
    // PostalAddress
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("CUSTOM_LABEL",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PostalAddress::CUSTOM_LABEL))),
        DECLARE_NAPI_STATIC_PROPERTY("ADDR_HOME",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PostalAddress::ADDR_HOME))),
        DECLARE_NAPI_STATIC_PROPERTY("ADDR_WORK",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PostalAddress::ADDR_WORK))),
        DECLARE_NAPI_STATIC_PROPERTY("ADDR_OTHER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PostalAddress::ADDR_OTHER))),
        DECLARE_NAPI_STATIC_PROPERTY("INVALID_LABEL_ID",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(PostalAddress::INVALID_LABEL_ID))),
    };
    napi_value result = nullptr;
    napi_define_class(env, "PostalAddress", NAPI_AUTO_LENGTH, ContactsNapiUtils::CreateClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result);
    napi_set_named_property(env, exports, "PostalAddress", result);
    return exports;
}

napi_value DeclareRelationConst(napi_env env, napi_value exports)
{
    // Relation
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("CUSTOM_LABEL",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::CUSTOM_LABEL))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_ASSISTANT",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_ASSISTANT))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_BROTHER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_BROTHER))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_CHILD",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_CHILD))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_DOMESTIC_PARTNER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_DOMESTIC_PARTNER))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_FATHER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_FATHER))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_FRIEND",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_FRIEND))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_MANAGER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_MANAGER))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_MOTHER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_MOTHER))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_PARENT",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_PARENT))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_PARTNER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_PARTNER))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_REFERRED_BY",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_REFERRED_BY))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_RELATIVE",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_RELATIVE))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_SISTER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_SISTER))),
        DECLARE_NAPI_STATIC_PROPERTY("RELATION_SPOUSE",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::RELATION_SPOUSE))),
        DECLARE_NAPI_STATIC_PROPERTY("INVALID_LABEL_ID",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Relation::INVALID_LABEL_ID))),
    };
    napi_value result = nullptr;
    napi_define_class(env, "Relation", NAPI_AUTO_LENGTH, ContactsNapiUtils::CreateClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result);
    napi_set_named_property(env, exports, "Relation", result);
    return exports;
}

napi_value DeclareSipAddressConst(napi_env env, napi_value exports)
{
    // SipAddress
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("CUSTOM_LABEL",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(SipAddress::CUSTOM_LABEL))),
        DECLARE_NAPI_STATIC_PROPERTY("SIP_HOME",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(SipAddress::SIP_HOME))),
        DECLARE_NAPI_STATIC_PROPERTY("SIP_WORK",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(SipAddress::SIP_WORK))),
        DECLARE_NAPI_STATIC_PROPERTY("SIP_OTHER",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(SipAddress::SIP_OTHER))),
        DECLARE_NAPI_STATIC_PROPERTY("INVALID_LABEL_ID",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(SipAddress::INVALID_LABEL_ID))),
    };
    napi_value result = nullptr;
    napi_define_class(env, "SipAddress", NAPI_AUTO_LENGTH, ContactsNapiUtils::CreateClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result);
    napi_set_named_property(env, exports, "SipAddress", result);
    return exports;
}

napi_value DeclareAttributeConst(napi_env env, napi_value exports)
{
    // Attribute
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_CONTACT_EVENT",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_CONTACT_EVENT))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_EMAIL",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_EMAIL))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_GROUP_MEMBERSHIP",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_GROUP_MEMBERSHIP))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_IM",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_IM))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_NAME",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_NAME))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_NICKNAME",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_NICKNAME))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_NOTE",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_NOTE))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_ORGANIZATION",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_ORGANIZATION))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_PHONE",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_PHONE))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_PORTRAIT",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_PORTRAIT))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_POSTAL_ADDRESS",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_POSTAL_ADDRESS))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_RELATION",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_RELATION))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_SIP_ADDRESS",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_SIP_ADDRESS))),
        DECLARE_NAPI_STATIC_PROPERTY("ATTR_WEBSITE",
            ContactsNapiUtils::ToInt32Value(env, static_cast<int32_t>(Attribute::ATTR_WEBSITE))),
    };
    napi_value result = nullptr;
    napi_define_class(env, "Attribute", NAPI_AUTO_LENGTH, ContactsNapiUtils::CreateClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result);
    napi_set_named_property(env, exports, "Attribute", result);
    return exports;
}

void Init(napi_env env, napi_value exports)
{
    napi_property_descriptor exportFuncs[] = {
        DECLARE_NAPI_FUNCTION("addContact", OHOS::ContactsApi::AddContact),
        DECLARE_NAPI_FUNCTION("deleteContact", OHOS::ContactsApi::DeleteContact),
        DECLARE_NAPI_FUNCTION("updateContact", OHOS::ContactsApi::UpdateContact),
        DECLARE_NAPI_FUNCTION("queryContact", OHOS::ContactsApi::QueryContact),
        DECLARE_NAPI_FUNCTION("queryContacts", OHOS::ContactsApi::QueryContacts),
        DECLARE_NAPI_FUNCTION("queryContactsByEmail", OHOS::ContactsApi::QueryContactsByEmail),
        DECLARE_NAPI_FUNCTION("queryContactsByPhoneNumber", OHOS::ContactsApi::QueryContactsByPhoneNumber),
        DECLARE_NAPI_FUNCTION("queryGroups", OHOS::ContactsApi::QueryGroups),
        DECLARE_NAPI_FUNCTION("queryHolders", OHOS::ContactsApi::QueryHolders),
        DECLARE_NAPI_FUNCTION("queryKey", OHOS::ContactsApi::QueryKey),
        DECLARE_NAPI_FUNCTION("queryMyCard", OHOS::ContactsApi::QueryMyCard),
        DECLARE_NAPI_FUNCTION("isMyCard", OHOS::ContactsApi::IsMyCard),
        DECLARE_NAPI_FUNCTION("isLocalContact", OHOS::ContactsApi::IsLocalContact),
    };
    napi_define_properties(env, exports, sizeof(exportFuncs) / sizeof(*exportFuncs), exportFuncs);
    // Declare class const initialization
    DeclareContactConst(env, exports);
    DeclareEmailConst(env, exports);
    DeclareEventConst(env, exports);
    DeclareImAddressConst(env, exports);
    DeclarePhoneNumberConst(env, exports);
    DeclarePostalAddressConst(env, exports);
    DeclareRelationConst(env, exports);
    DeclareSipAddressConst(env, exports);
    DeclareAttributeConst(env, exports);
}
} // namespace ContactsApi
} // namespace OHOS
