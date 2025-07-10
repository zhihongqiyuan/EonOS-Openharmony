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

#include "result_convert.h"

#include "hilog_wrapper_api.h"

namespace OHOS {
namespace ContactsApi {
ResultConvert::ResultConvert(void)
{
}

ResultConvert::~ResultConvert()
{
}

/**
 * @brief Get object array by resultSet
 *
 * @param env Conditions for convert operation
 * @param resultSet Conditions for convert operation
 *
 * @return The result returned by convert operation
 */
napi_value ResultConvert::ResultSetToObject(
    napi_env env, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (resultSet == nullptr) {
        HILOG_ERROR("ResultConvert::ResultSetToObject resultSet is nullptr");
        napi_value array;
        napi_create_array(env, &array);
        return array;
    }
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    HILOG_INFO("ResultSetToObject GetRowCount is %{public}d", rowCount);
    if (rowCount == 0) {
        napi_value array;
        napi_create_array(env, &array);
        resultSet->Close();
        return array;
    }
    std::map<int, napi_value> resultSetMap;
    std::map<int, std::string> quickSearchMap;
    int resultSetNum = resultSet->GoToFirstRow();
    while (resultSetNum == 0) {
        int contactIdValue = 0;
        std::string contactIdKey = "contact_id";
        int contactIndex = 0;
        resultSet->GetColumnIndex(contactIdKey, contactIndex);
        resultSet->GetInt(contactIndex, contactIdValue);
        napi_value napiObject = GetResultMapValue(env, resultSetMap, contactIdValue);
        PutQuickSearchKey(env, resultSet, quickSearchMap, contactIdValue);
        ConvertContactObject(env, napiObject, resultSet);
        resultSetNum = resultSet->GoToNextRow();
    }
    resultSet->Close();
    napi_value array = ConvertContactArray(env, resultSetMap, quickSearchMap);
    return array;
}

void ResultConvert::PutQuickSearchKey(napi_env env, std::shared_ptr<DataShare::DataShareResultSet> &resultSet,
    std::map<int, std::string> &quickSearchMap, int contactIdValue)
{
    std::string quickSearchValue = "";
    std::string quickSearchKey = "quick_search_key";
    int columnIndex = 0;
    resultSet->GetColumnIndex(quickSearchKey, columnIndex);
    resultSet->GetString(columnIndex, quickSearchValue);
    if (quickSearchMap.count(contactIdValue) <= 0) {
        napi_value napiQuickSearchValue;
        napi_create_string_utf8(env, quickSearchValue.c_str(), NAPI_AUTO_LENGTH, &napiQuickSearchValue);
        quickSearchMap.insert(std::pair<int, std::string>(contactIdValue, quickSearchValue));
    }
}

napi_value ResultConvert::ConvertContactArray(
    napi_env env, std::map<int, napi_value> &resultSetMap, std::map<int, std::string> &quickSearchMap)
{
    napi_value array;
    napi_create_array(env, &array);
    int count = 0;
    std::map<int, napi_value>::iterator it;
    for (it = resultSetMap.begin(); it != resultSetMap.end(); ++it) {
        napi_value elementObject = it->second;
        if (quickSearchMap.count(it->first) > 0) {
            napi_value keyValue;
            napi_create_string_utf8(env, quickSearchMap[it->first].c_str(), NAPI_AUTO_LENGTH, &keyValue);
            napi_value napiQuickKey;
            napi_create_string_utf8(env, "key", NAPI_AUTO_LENGTH, &napiQuickKey);
            napi_set_property(env, elementObject, napiQuickKey, keyValue);
        }
        napi_value keyValue;
        napi_create_int64(env, it->first, &keyValue);
        napi_value napiIdKey;
        napi_create_string_utf8(env, "id", NAPI_AUTO_LENGTH, &napiIdKey);
        napi_set_property(env, elementObject, napiIdKey, keyValue);
        napi_set_element(env, array, count, elementObject);
        ++count;
    }
    return array;
}

/**
 * @brief Converting NaPi objects to C++ objects
 *
 * @param env Conditions for convert operation
 * @param napiObject Conditions for convert operation
 * @param resultSet Target of convert operation
 */
void ResultConvert::ConvertContactObject(
    napi_env env, napi_value napiObject, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    int typeIdValue = 0;
    std::string typeId = "type_id";
    int columnIndexType = 0;
    resultSet->GetColumnIndex(typeId, columnIndexType);
    resultSet->GetInt(columnIndexType, typeIdValue);
    ConvertEmail(env, napiObject, typeIdValue, resultSet);
    ConvertName(env, napiObject, typeIdValue, resultSet);
    ConvertUri(env, napiObject, typeIdValue, resultSet);
    ConvertEvent(env, napiObject, typeIdValue, resultSet);
    ConvertGroup(env, napiObject, typeIdValue, resultSet);
    ConvertImAddress(env, napiObject, typeIdValue, resultSet);
    ConvertPhoneNumber(env, napiObject, typeIdValue, resultSet);
    ConvertPostalAddress(env, napiObject, typeIdValue, resultSet);
    ConvertRelation(env, napiObject, typeIdValue, resultSet);
    ConvertSipAddress(env, napiObject, typeIdValue, resultSet);
    ConvertWebsite(env, napiObject, typeIdValue, resultSet);
    ConvertNickName(env, napiObject, typeIdValue, resultSet);
    ConvertNote(env, napiObject, typeIdValue, resultSet);
    ConvertOrganization(env, napiObject, typeIdValue, resultSet);
}

bool ResultConvert::IsEmpty(std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (resultSet == nullptr) {
        HILOG_ERROR("ResultSetToHolder resultSet is nullptr");
        return true;
    }
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    if (rowCount == 0) {
        resultSet->Close();
        return true;
    }
    return false;
}

napi_value ResultConvert::ResultSetToHolder(
    napi_env env, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    napi_value array;
    napi_create_array(env, &array);
    if (IsEmpty(resultSet)) {
        return array;
    }
    int resultSetNum = resultSet->GoToFirstRow();
    int count = 0;
    while (resultSetNum == 0) {
        napi_value elementObject;
        napi_create_object(env, &elementObject);
        int idValue;
        std::string id = "id";
        int idIndex = 0;
        resultSet->GetColumnIndex(id, idIndex);
        resultSet->GetInt(idIndex, idValue);
        napi_value napiIdKey;
        napi_create_string_utf8(env, "holderId", NAPI_AUTO_LENGTH, &napiIdKey);
        napi_value napiValue;
        napi_create_int64(env, idValue, &napiValue);
        napi_set_property(env, elementObject, napiIdKey, napiValue);
        std::string displayName = "account_name";
        std::string displayNameValue;
        int displayNameIndex = 0;
        resultSet->GetColumnIndex(displayName, displayNameIndex);
        resultSet->GetString(displayNameIndex, displayNameValue);
        napi_value napiDisplayNameKey;
        napi_create_string_utf8(env, "displayName", NAPI_AUTO_LENGTH, &napiDisplayNameKey);
        napi_value napiDisplayNameValue;
        napi_create_string_utf8(env, displayNameValue.c_str(), NAPI_AUTO_LENGTH, &napiDisplayNameValue);
        napi_set_property(env, elementObject, napiDisplayNameKey, napiDisplayNameValue);
        std::string bundleName = "account_type";
        std::string bundleNameValue;
        int bundleNameIndex = 0;
        resultSet->GetColumnIndex(bundleName, bundleNameIndex);
        resultSet->GetString(bundleNameIndex, bundleNameValue);
        napi_value napiBundleNameKey;
        napi_create_string_utf8(env, "bundleName", NAPI_AUTO_LENGTH, &napiBundleNameKey);
        napi_value napiBundleNameValue;
        napi_create_string_utf8(env, bundleNameValue.c_str(), NAPI_AUTO_LENGTH, &napiBundleNameValue);
        napi_set_property(env, elementObject, napiBundleNameKey, napiBundleNameValue);
        napi_set_element(env, array, count, elementObject);
        ++count;
        resultSetNum = resultSet->GoToNextRow();
    }
    resultSet->Close();
    return array;
}

napi_value ResultConvert::ResultSetToGroup(
    napi_env env, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    napi_value array;
    NAPI_CALL(env, napi_create_array(env, &array));
    if (resultSet == nullptr) {
        HILOG_ERROR("ResultSetToGroup resultSet is nullptr");
        return array;
    }
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    HILOG_INFO("ResultSetToGroup GetRowCount is %{public}d", rowCount);
    if (rowCount == 0) {
        resultSet->Close();
        return array;
    }
    int resultSetNum = resultSet->GoToFirstRow();
    HILOG_INFO("ResultSetToGroup resultSetNum is %{public}d", resultSetNum);
    int count = 0;
    while (resultSetNum == 0) {
        napi_value elementObject;
        napi_create_object(env, &elementObject);
        int idValue;
        std::string id = "id";
        int idIndex = 0;
        resultSet->GetColumnIndex(id, idIndex);
        resultSet->GetInt(idIndex, idValue);
        napi_value napiIdKey;
        napi_create_string_utf8(env, "id", NAPI_AUTO_LENGTH, &napiIdKey);
        napi_value napiValue;
        napi_create_int64(env, idValue, &napiValue);
        napi_set_property(env, elementObject, napiIdKey, napiValue);
        std::string title = "group_name";
        std::string titleValue;
        int titleIndex = 0;
        resultSet->GetColumnIndex(title, titleIndex);
        resultSet->GetString(titleIndex, titleValue);
        napi_value napiTitleKey;
        napi_create_string_utf8(env, "title", NAPI_AUTO_LENGTH, &napiTitleKey);
        napi_value napiTitleValue;
        napi_create_string_utf8(env, titleValue.c_str(), NAPI_AUTO_LENGTH, &napiTitleValue);
        napi_set_property(env, elementObject, napiTitleKey, napiTitleValue);
        napi_set_element(env, array, count, elementObject);
        ++count;
        resultSetNum = resultSet->GoToNextRow();
    }
    resultSet->Close();
    return array;
}

napi_value ResultConvert::GetResultMapValue(napi_env env, std::map<int, napi_value> &resultSetMap, int &contactId)
{
    napi_value napiObject;
    if (resultSetMap.count(contactId) > 0) {
        std::map<int, napi_value>::iterator it = resultSetMap.find(contactId);
        napiObject = it->second;
    } else {
        napi_create_object(env, &napiObject);
        resultSetMap.insert(std::pair<int, napi_value>(contactId, napiObject));
        std::map<int, napi_value>::iterator it = resultSetMap.find(contactId);
        napiObject = it->second;
    }
    return napiObject;
}

napi_value ResultConvert::GetNapiValue(napi_env env, const std::string keyChar, napi_value napiObject)
{
    if (napiObject == nullptr) {
        HILOG_ERROR("ResultConvert::GetNapiValue object is nullptr");
        return nullptr;
    }
    napi_value key;
    napi_create_string_utf8(env, keyChar.c_str(), NAPI_AUTO_LENGTH, &key);
    bool result = false;
    napi_has_property(env, napiObject, key, &result);
    if (result) {
        napi_value value = nullptr;
        napi_get_property(env, napiObject, key, &value);
        return value;
    }
    return nullptr;
}

napi_value ResultConvert::GetNapiElementObject(napi_env env, napi_value napiObject, const std::string valueChar)
{
    napi_value objectElement = GetNapiValue(env, valueChar, napiObject);
    if (objectElement == nullptr) {
        napi_create_object(env, &objectElement);
    }
    return objectElement;
}

napi_value ResultConvert::GetNapiElementArray(napi_env env, napi_value napiObject, const std::string valueChar)
{
    napi_value emailArray = GetNapiValue(env, valueChar, napiObject);
    if (emailArray == nullptr) {
        napi_create_array(env, &emailArray);
    }
    return emailArray;
}

napi_value ResultConvert::GetResultValue(
    napi_env env, std::string &contentKey, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    int columnIndex = ERROR;
    resultSet->GetColumnIndex(contentKey, columnIndex);
    OHOS::DataShare::DataType columnType;
    resultSet->GetDataType(columnIndex, columnType);
    napi_value napiValue = nullptr;
    if (columnType == OHOS::DataShare::DataType::TYPE_NULL) {
        return napiValue;
    } else if (columnType == OHOS::DataShare::DataType::TYPE_BLOB) {
        return napiValue;
    } else if (columnType == OHOS::DataShare::DataType::TYPE_INTEGER) {
        int intValue = 0;
        resultSet->GetInt(columnIndex, intValue);
        napi_create_int64(env, intValue, &napiValue);
    } else if (columnType == OHOS::DataShare::DataType::TYPE_FLOAT) {
        double doubleValue = 0;
        resultSet->GetDouble(columnIndex, doubleValue);
        napi_create_double(env, doubleValue, &napiValue);
    } else if (columnType == OHOS::DataShare::DataType::TYPE_STRING) {
        std::string stringValue;
        resultSet->GetString(columnIndex, stringValue);
        napi_create_string_utf8(env, stringValue.c_str(), NAPI_AUTO_LENGTH, &napiValue);
    }
    return napiValue;
}

napi_value ResultConvert::CreateNapiStringValue(napi_env env, const std::string key)
{
    napi_value keyValue;
    napi_create_string_utf8(env, key.c_str(), NAPI_AUTO_LENGTH, &keyValue);
    return keyValue;
}

void ResultConvert::ConvertEmail(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == EMAIL) {
        const std::string emails = "emails";
        napi_value emailArray = GetNapiElementArray(env, napiObject, emails);
        uint32_t count = 0;
        napi_get_array_length(env, emailArray, &count);
        napi_value objectElement;
        napi_create_object(env, &objectElement);
        std::string detailInfoKey = "detail_info";
        std::string customDataKey = "custom_data";
        std::string extend7Key = "extend7";
        std::string aliasDetailInfoKey = "alias_detail_info";
        napi_value detailInfoValue = GetResultValue(env, detailInfoKey, resultSet);
        napi_value customValue = GetResultValue(env, customDataKey, resultSet);
        napi_value extend7Value = GetResultValue(env, extend7Key, resultSet);
        napi_value aliasDetailInfoValue = GetResultValue(env, aliasDetailInfoKey, resultSet);
        napi_value napiDetailInfoKey = CreateNapiStringValue(env, "email");
        napi_set_property(env, objectElement, napiDetailInfoKey, detailInfoValue);
        napi_value napiLabelNameKey = CreateNapiStringValue(env, "labelName");
        napi_set_property(env, objectElement, napiLabelNameKey, customValue);
        napi_value napiLabelIdKey = CreateNapiStringValue(env, "labelId");
        napi_set_property(env, objectElement, napiLabelIdKey, extend7Value);
        napi_value napiDisplayNameIdKey = CreateNapiStringValue(env, "displayName");
        napi_set_property(env, objectElement, napiDisplayNameIdKey, aliasDetailInfoValue);
        napi_set_element(env, emailArray, count, objectElement);
        napi_value napiElementKey;
        napi_create_string_utf8(env, emails.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, emailArray);
    }
}

void ResultConvert::ConvertName(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == NAME) {
        const std::string name = "name";
        napi_value objectElement = GetNapiElementObject(env, napiObject, name);
        std::string fullNameKey = "detail_info";
        std::string familyNameKey = "family_name";
        std::string familyNamePhoneticKey = "phonetic_name";
        std::string givenNameKey = "given_name";
        std::string givenNamePhoneticKey = "given_name_phonetic";
        std::string middleNamePhoneticKey = "middle_name_phonetic";
        std::string middleNameKey = "other_lan_last_name";
        std::string namePrefixKey = "alpha_name";
        std::string nameSuffixKey = "other_lan_first_name";
        napi_value fullNameValue = GetResultValue(env, fullNameKey, resultSet);
        napi_value familyNameValue = GetResultValue(env, familyNameKey, resultSet);
        napi_value familyNamePhoneticValue = GetResultValue(env, familyNamePhoneticKey, resultSet);
        napi_value givenNameValue = GetResultValue(env, givenNameKey, resultSet);
        napi_value givenNamePhoneticValue = GetResultValue(env, givenNamePhoneticKey, resultSet);
        napi_value middleNamePhoneticValue = GetResultValue(env, middleNamePhoneticKey, resultSet);
        napi_value middleNameValue = GetResultValue(env, middleNameKey, resultSet);
        napi_value namePrefixValue = GetResultValue(env, namePrefixKey, resultSet);
        napi_value nameSuffixValue = GetResultValue(env, nameSuffixKey, resultSet);
        napi_value napiFullNameKey = CreateNapiStringValue(env, "fullName");
        napi_set_property(env, objectElement, napiFullNameKey, fullNameValue);
        napi_value napiFamilyNameKey = CreateNapiStringValue(env, "familyName");
        napi_set_property(env, objectElement, napiFamilyNameKey, familyNameValue);
        napi_value napiFamilyNamePhoneticKey = CreateNapiStringValue(env, "familyNamePhonetic");
        napi_set_property(env, objectElement, napiFamilyNamePhoneticKey, familyNamePhoneticValue);
        napi_value napiGivenNameKey = CreateNapiStringValue(env, "givenName");
        napi_set_property(env, objectElement, napiGivenNameKey, givenNameValue);
        napi_value napiGivenNamePhoneticKey = CreateNapiStringValue(env, "givenNamePhonetic");
        napi_set_property(env, objectElement, napiGivenNamePhoneticKey, givenNamePhoneticValue);
        napi_value napiMiddleNamePhoneticKey = CreateNapiStringValue(env, "middleNamePhonetic");
        napi_set_property(env, objectElement, napiMiddleNamePhoneticKey, middleNamePhoneticValue);
        napi_value napiMiddleNameKey = CreateNapiStringValue(env, "middleName");
        napi_set_property(env, objectElement, napiMiddleNameKey, middleNameValue);
        napi_value napiNamePrefixKey = CreateNapiStringValue(env, "namePrefix");
        napi_set_property(env, objectElement, napiNamePrefixKey, namePrefixValue);
        napi_value napiNameSuffixKey = CreateNapiStringValue(env, "nameSuffix");
        napi_set_property(env, objectElement, napiNameSuffixKey, nameSuffixValue);
        napi_value napiElementKey;
        napi_create_string_utf8(env, name.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, objectElement);
    }
}

void ResultConvert::ConvertUri(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == PHOTO) {
        const std::string portrait = "portrait";
        napi_value objectElement = GetNapiElementObject(env, napiObject, portrait);
        std::string uri = "detail_info";
        napi_value uriValue = GetResultValue(env, uri, resultSet);
        napi_value napiUri = CreateNapiStringValue(env, "uri");
        napi_set_property(env, objectElement, napiUri, uriValue);
        napi_value napiElementKey;
        napi_create_string_utf8(env, portrait.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, objectElement);
    }
}

void ResultConvert::ConvertEvent(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == CONTACT_EVENT) {
        const std::string events = "events";
        napi_value emailArray = GetNapiElementArray(env, napiObject, events);
        uint32_t count = 0;
        napi_get_array_length(env, emailArray, &count);
        napi_value objectElement;
        napi_create_object(env, &objectElement);
        std::string detailInfoKey = "detail_info";
        std::string customDataKey = "custom_data";
        std::string extend7Key = "extend7";
        napi_value detailInfoValue = GetResultValue(env, detailInfoKey, resultSet);
        napi_value customValue = GetResultValue(env, customDataKey, resultSet);
        napi_value extend7Value = GetResultValue(env, extend7Key, resultSet);
        napi_value napiDetailInfoKey = CreateNapiStringValue(env, "eventDate");
        napi_set_property(env, objectElement, napiDetailInfoKey, detailInfoValue);
        napi_value napiLabelNameKey = CreateNapiStringValue(env, "labelName");
        napi_set_property(env, objectElement, napiLabelNameKey, customValue);
        napi_value napiLabelIdKey = CreateNapiStringValue(env, "labelId");
        napi_set_property(env, objectElement, napiLabelIdKey, extend7Value);
        napi_set_element(env, emailArray, count, objectElement);
        napi_value napiElementKey;
        napi_create_string_utf8(env, events.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, emailArray);
    }
}

void ResultConvert::ConvertGroup(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == GROUP_MEMBERSHIP) {
        const std::string groups = "groups";
        napi_value emailArray = GetNapiElementArray(env, napiObject, groups);
        uint32_t count = 0;
        napi_get_array_length(env, emailArray, &count);
        napi_value objectElement;
        napi_create_object(env, &objectElement);
        std::string groupNameKey = "group_name";
        napi_value groupNameValue = GetResultValue(env, groupNameKey, resultSet);
        napi_value napiGroupNameKey = CreateNapiStringValue(env, "title");
        napi_set_property(env, objectElement, napiGroupNameKey, groupNameValue);
        std::string detailInfoKey = "detail_info";
        napi_value detailInfoValue = GetResultValue(env, detailInfoKey, resultSet);
        napi_value napiDetailInfoKey = CreateNapiStringValue(env, "groupId");
        napi_set_property(env, objectElement, napiDetailInfoKey, detailInfoValue);
        napi_value napiElementKey;
        napi_create_string_utf8(env, groups.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, emailArray);
    }
}

void ResultConvert::ConvertImAddress(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == IM) {
        const std::string imAddresses = "imAddresses";
        napi_value imAddressArray = GetNapiElementArray(env, napiObject, imAddresses);
        uint32_t count = 0;
        napi_get_array_length(env, imAddressArray, &count);
        napi_value objectElement;
        napi_create_object(env, &objectElement);
        std::string detailInfoKey = "detail_info";
        std::string customDataKey = "custom_data";
        std::string extend7Key = "extend7";
        napi_value detailInfoValue = GetResultValue(env, detailInfoKey, resultSet);
        napi_value customValue = GetResultValue(env, customDataKey, resultSet);
        napi_value extend7Value = GetResultValue(env, extend7Key, resultSet);
        napi_value napiDetailInfoKey = CreateNapiStringValue(env, "imAddress");
        napi_set_property(env, objectElement, napiDetailInfoKey, detailInfoValue);
        napi_value napiLabelNameKey = CreateNapiStringValue(env, "labelName");
        napi_set_property(env, objectElement, napiLabelNameKey, customValue);
        napi_value napiLabelIdKey = CreateNapiStringValue(env, "labelId");
        napi_set_property(env, objectElement, napiLabelIdKey, extend7Value);
        napi_set_element(env, imAddressArray, count, objectElement);
        napi_value napiElementKey;
        napi_create_string_utf8(env, imAddresses.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, imAddressArray);
    }
}

void ResultConvert::ConvertPhoneNumber(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == PHONE) {
        const std::string phoneNumbers = "phoneNumbers";
        napi_value phoneNumbersArray = GetNapiElementArray(env, napiObject, phoneNumbers);
        uint32_t count = 0;
        napi_get_array_length(env, phoneNumbersArray, &count);
        napi_value objectElement;
        napi_create_object(env, &objectElement);
        std::string detailInfoKey = "detail_info";
        std::string customDataKey = "custom_data";
        std::string extend7Key = "extend7";
        napi_value detailInfoValue = GetResultValue(env, detailInfoKey, resultSet);
        napi_value customValue = GetResultValue(env, customDataKey, resultSet);
        napi_value extend7Value = GetResultValue(env, extend7Key, resultSet);
        napi_value napiDetailInfoKey = CreateNapiStringValue(env, "phoneNumber");
        napi_set_property(env, objectElement, napiDetailInfoKey, detailInfoValue);
        napi_value napiLabelNameKey = CreateNapiStringValue(env, "labelName");
        napi_set_property(env, objectElement, napiLabelNameKey, customValue);
        napi_value napiLabelIdKey = CreateNapiStringValue(env, "labelId");
        napi_set_property(env, objectElement, napiLabelIdKey, extend7Value);
        napi_set_element(env, phoneNumbersArray, count, objectElement);
        napi_value napiElementKey;
        napi_create_string_utf8(env, phoneNumbers.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, phoneNumbersArray);
    }
}

void ResultConvert::ConvertPostalAddress(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == POSTAL_ADDRESS) {
        const std::string postalAddresses = "postalAddresses";
        napi_value postalAddressArray = GetNapiElementArray(env, napiObject, postalAddresses);
        uint32_t count = 0;
        napi_get_array_length(env, postalAddressArray, &count);
        napi_value objectElement;
        napi_create_object(env, &objectElement);
        std::string detailInfoKey = "detail_info";
        std::string customDataKey = "custom_data";
        std::string extend7Key = "extend7";
        napi_value detailInfoValue = GetResultValue(env, detailInfoKey, resultSet);
        napi_value customValue = GetResultValue(env, customDataKey, resultSet);
        napi_value extend7Value = GetResultValue(env, extend7Key, resultSet);
        std::string neighborhoodKey = "neighborhood";
        std::string poboxKey = "pobox";
        std::string postcodeKey = "postcode";
        std::string regionKey = "region";
        std::string streetKey = "street";
        std::string cityKey = "city";
        std::string countryKey = "country";
        napi_value neighborhoodKeyValue = GetResultValue(env, neighborhoodKey, resultSet);
        napi_value cityKeyValue = GetResultValue(env, cityKey, resultSet);
        napi_value countryKeyValue = GetResultValue(env, countryKey, resultSet);
        napi_value poboxKeyValue = GetResultValue(env, poboxKey, resultSet);
        napi_value postcodeKeyValue = GetResultValue(env, postcodeKey, resultSet);
        napi_value regionKeyValue = GetResultValue(env, regionKey, resultSet);
        napi_value streetKeyValue = GetResultValue(env, streetKey, resultSet);
        napi_value napiDetailInfoKey = CreateNapiStringValue(env, "postalAddress");
        napi_set_property(env, objectElement, napiDetailInfoKey, detailInfoValue);
        napi_value napiLabelNameKey = CreateNapiStringValue(env, "labelName");
        napi_set_property(env, objectElement, napiLabelNameKey, customValue);
        napi_value napiLabelIdKey = CreateNapiStringValue(env, "labelId");
        napi_set_property(env, objectElement, napiLabelIdKey, extend7Value);
        napi_set_property(
            env, objectElement, CreateNapiStringValue(env, neighborhoodKey.c_str()), neighborhoodKeyValue);
        napi_set_property(env, objectElement, CreateNapiStringValue(env, cityKey.c_str()), cityKeyValue);
        napi_set_property(env, objectElement, CreateNapiStringValue(env, countryKey.c_str()), countryKeyValue);
        napi_set_property(env, objectElement, CreateNapiStringValue(env, poboxKey.c_str()), poboxKeyValue);
        napi_set_property(env, objectElement, CreateNapiStringValue(env, postcodeKey.c_str()), postcodeKeyValue);
        napi_set_property(env, objectElement, CreateNapiStringValue(env, regionKey.c_str()), regionKeyValue);
        napi_set_property(env, objectElement, CreateNapiStringValue(env, streetKey.c_str()), streetKeyValue);
        napi_set_element(env, postalAddressArray, count, objectElement);
        napi_value napiElementKey = CreateNapiStringValue(env, postalAddresses);
        napi_set_property(env, napiObject, napiElementKey, postalAddressArray);
    }
}

void ResultConvert::ConvertRelation(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == RELATION) {
        const std::string relations = "relations";
        napi_value relationsArray = GetNapiElementArray(env, napiObject, relations);
        uint32_t count = 0;
        napi_get_array_length(env, relationsArray, &count);
        napi_value objectElement;
        napi_create_object(env, &objectElement);
        std::string detailInfoKey = "detail_info";
        std::string customDataKey = "custom_data";
        std::string extend7Key = "extend7";
        napi_value detailInfoValue = GetResultValue(env, detailInfoKey, resultSet);
        napi_value customValue = GetResultValue(env, customDataKey, resultSet);
        napi_value extend7Value = GetResultValue(env, extend7Key, resultSet);
        napi_value napiDetailInfoKey = CreateNapiStringValue(env, "sipAddress");
        napi_set_property(env, objectElement, napiDetailInfoKey, detailInfoValue);
        napi_value napiLabelNameKey = CreateNapiStringValue(env, "labelName");
        napi_set_property(env, objectElement, napiLabelNameKey, customValue);
        napi_value napiLabelIdKey = CreateNapiStringValue(env, "labelId");
        napi_set_property(env, objectElement, napiLabelIdKey, extend7Value);
        napi_set_element(env, relationsArray, count, objectElement);
        napi_value napiElementKey;
        napi_create_string_utf8(env, relations.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, relationsArray);
    }
}

void ResultConvert::ConvertSipAddress(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == SIP_ADDRESS) {
        const std::string sipAddresses = "sipAddresses";
        napi_value sipAddressArray = GetNapiElementArray(env, napiObject, sipAddresses);
        uint32_t count = 0;
        napi_get_array_length(env, sipAddressArray, &count);
        napi_value objectElement;
        napi_create_object(env, &objectElement);
        std::string detailInfoKey = "detail_info";
        std::string customDataKey = "custom_data";
        std::string extend7Key = "extend7";
        napi_value detailInfoValue = GetResultValue(env, detailInfoKey, resultSet);
        napi_value customValue = GetResultValue(env, customDataKey, resultSet);
        napi_value extend7Value = GetResultValue(env, extend7Key, resultSet);
        napi_value napiDetailInfoKey = CreateNapiStringValue(env, "relationName");
        napi_set_property(env, objectElement, napiDetailInfoKey, detailInfoValue);
        napi_value napiLabelNameKey = CreateNapiStringValue(env, "labelName");
        napi_set_property(env, objectElement, napiLabelNameKey, customValue);
        napi_value napiLabelIdKey = CreateNapiStringValue(env, "labelId");
        napi_set_property(env, objectElement, napiLabelIdKey, extend7Value);
        napi_set_element(env, sipAddressArray, count, objectElement);
        napi_value napiElementKey;
        napi_create_string_utf8(env, sipAddresses.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, sipAddressArray);
    }
}

void ResultConvert::ConvertWebsite(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == WEBSITE) {
        const std::string websites = "websites";
        napi_value websitesArray = GetNapiElementArray(env, napiObject, websites);
        uint32_t count = 0;
        napi_get_array_length(env, websitesArray, &count);
        napi_value objectElement;
        napi_create_object(env, &objectElement);
        std::string detailInfoKey = "detail_info";
        std::string customDataKey = "custom_data";
        std::string extend7Key = "extend7";
        napi_value detailInfoValue = GetResultValue(env, detailInfoKey, resultSet);
        napi_value customValue = GetResultValue(env, customDataKey, resultSet);
        napi_value extend7Value = GetResultValue(env, extend7Key, resultSet);
        napi_value napiDetailInfoKey = CreateNapiStringValue(env, "website");
        napi_set_property(env, objectElement, napiDetailInfoKey, detailInfoValue);
        napi_value napiLabelNameKey = CreateNapiStringValue(env, "labelName");
        napi_set_property(env, objectElement, napiLabelNameKey, customValue);
        napi_value napiLabelIdKey = CreateNapiStringValue(env, "labelId");
        napi_set_property(env, objectElement, napiLabelIdKey, extend7Value);
        napi_set_element(env, websitesArray, count, objectElement);
        napi_value napiElementKey;
        napi_create_string_utf8(env, websites.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, websitesArray);
    }
}

void ResultConvert::ConvertNickName(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == NICKNAME) {
        const std::string nickName = "nickName";
        napi_value objectElement = GetNapiElementObject(env, napiObject, nickName);
        std::string name = "detail_info";
        napi_value uriValue = GetResultValue(env, name, resultSet);
        napi_value napiUri = CreateNapiStringValue(env, "nickName");
        napi_set_property(env, objectElement, napiUri, uriValue);
        napi_value napiElementKey;
        napi_create_string_utf8(env, nickName.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, objectElement);
    }
}

void ResultConvert::ConvertNote(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == NOTE) {
        const std::string note = "note";
        napi_value objectElement = GetNapiElementObject(env, napiObject, note);
        std::string name = "detail_info";
        napi_value noteValue = GetResultValue(env, name, resultSet);
        napi_value noteKey = CreateNapiStringValue(env, "noteContent");
        napi_set_property(env, objectElement, noteKey, noteValue);
        napi_value napiElementKey;
        napi_create_string_utf8(env, note.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, objectElement);
    }
}

void ResultConvert::ConvertOrganization(
    napi_env env, napi_value napiObject, int &typeId, std::shared_ptr<DataShare::DataShareResultSet> &resultSet)
{
    if (typeId == ORGANIZATION) {
        const std::string organization = "organization";
        napi_value objectElement = GetNapiElementObject(env, napiObject, organization);
        std::string company = "detail_info";
        std::string position = "position";
        napi_value companyValue = GetResultValue(env, company, resultSet);
        napi_value positionValue = GetResultValue(env, position, resultSet);
        napi_value napiNameValueKey = CreateNapiStringValue(env, "name");
        napi_set_property(env, objectElement, napiNameValueKey, companyValue);
        napi_value napiTitleValueKey = CreateNapiStringValue(env, "title");
        napi_set_property(env, objectElement, napiTitleValueKey, positionValue);
        napi_value napiElementKey;
        napi_create_string_utf8(env, organization.c_str(), NAPI_AUTO_LENGTH, &napiElementKey);
        napi_set_property(env, napiObject, napiElementKey, objectElement);
    }
}
} // namespace ContactsApi
} // namespace OHOS