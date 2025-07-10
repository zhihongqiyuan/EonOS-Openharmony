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

#include "securec.h"
#include "contacts_utils.h"

using namespace OHOS;
using namespace OHOS::ContactsFfi;
using namespace OHOS::DataShare;

namespace OHOS {
namespace ContactsFfi {

// return true if succeeded; after it b is fully allocated or completely empty with errCode set
bool allocBucket(ValuesBucket* b, size_t total, int32_t *errCode)
{
    if (*errCode != SUCCESS) {
        return false;
    }
    if (total > 0) {
        b->size = total;
        b->key = (char**) malloc(total * sizeof(char*));
        if (b->key == nullptr) {
            *errCode = ERROR;
            b->freeContent(); // actually. there is nothing to free, just set size to 0
            return false;
        }
        b->value = (struct CValueType*) malloc(total * sizeof(struct CValueType));
        if (b->value == nullptr) {
            *errCode = ERROR;
            b->freeContent();
            return false;
        } else {
            b->value->tag = static_cast<int>(DataType::TYPE_NULL);
            b->value->string = nullptr;
        }
    }
    return true;
}

DataShareValuesBucket convertToDataShareVB(ValuesBucket vb)
{
    uint64_t size = vb.size;
    DataShareValuesBucket dsvb = DataShareValuesBucket();
    for (uint64_t i = 0; i < size; i++) {
        switch (vb.value[i].tag) {
            case DataShareValueObjectType::TYPE_STRING: {
                DataShareValueObject valueObject = DataShareValueObject(std::string(vb.value[i].string));
                std::string keyStr = vb.key[i];
                dsvb.Put(keyStr, valueObject);
                break;
            }
            case DataShareValueObjectType::TYPE_INT: {
                DataShareValueObject valueObject = DataShareValueObject(vb.value[i].integer);
                std::string keyStr = vb.key[i];
                dsvb.Put(keyStr, valueObject);
                break;
            }
            default: { // should not reach here
                std::string keyStr = vb.key[i];
                HILOG_ERROR("VB unsupported value type is %{public}d for key = %{public}s",
                    vb.value[i].tag, keyStr.c_str());
                dsvb.Put(vb.key[i], DataShareValueObject());
                break;
            }
        }
    }
    return dsvb;
}

void PutQuickSearchKey(std::shared_ptr<DataShareResultSet> &resultSet,
    std::map<int, std::string> &quickSearchMap, int contactIdValue)
{
    std::string quickSearchValue = "";
    std::string quickSearchKey = "quick_search_key";
    int columnIndex = 0;
    resultSet->GetColumnIndex(quickSearchKey, columnIndex);
    resultSet->GetString(columnIndex, quickSearchValue);
    if (quickSearchMap.count(contactIdValue) <= 0) {
        quickSearchMap.insert(std::pair<int, std::string>(contactIdValue, quickSearchValue));
    }
}

std::vector<ValuesBucket>& GetResultMapValue(std::map<int, std::vector<ValuesBucket>> &resultSetMap, int contactId)
{
    std::map<int, std::vector<ValuesBucket>>::iterator it = resultSetMap.find(contactId);
    if (it == resultSetMap.end()) {
        std::vector<ValuesBucket> contactData = std::vector<ValuesBucket>();
        resultSetMap.insert(std::pair<int, std::vector<ValuesBucket>>(contactId, contactData));
        it = resultSetMap.find(contactId);
    }
    return it->second;
}

void copyBucket(ValuesBucket* dst, int dstIdx, ValuesBucket &src)
{
    dst[dstIdx].key = src.key;
    dst[dstIdx].value = src.value;
    dst[dstIdx].size = src.size;
}

char* TransformFromString(std::string &str, int32_t* errCode)
{
    size_t len = str.size() + 1;
    char* retValue = static_cast<char *>(malloc(len));
    if (retValue == nullptr) {
        *errCode = ERROR;
        return nullptr;
    }
    *errCode = memcpy_s(retValue, len, str.c_str(), len);
    if (*errCode != SUCCESS) {
        free(retValue);
        retValue = nullptr;
    }
    return retValue;
}

ValuesBucket allocBucketData(std::vector<KeyWithValueType> &bucketData, int32_t *errCode)
{
    struct ValuesBucket b;
    size_t total = bucketData.size();
    if (total > 0) {
        if (!allocBucket(&b, total, errCode)) {
            return b;
        }
        // Let's increment b.size one by one for b.freeContent() call in case of error
        b.size = 0;
        for (size_t i = 0; i < total; i++) {
            bucketData[i].allocToBucket(&b, 0, i, errCode);
            if (*errCode != SUCCESS) {
                b.freeContent();
                return b;
            } else {
                b.size = b.size + 1; // b.size is incremented one by one for b.freeContent() call in case of error
            }
        }
    }
    return b;
}

ValuesBucket singleStringAsValueBucket(std::string contentType, std::string value, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", contentType));
    bucketData.push_back(KeyWithValueType("detail_info", value));
    return allocBucketData(bucketData, errCode);
}

void PutResultValue(std::vector<KeyWithValueType> &bucket, std::string contentStoreKey,
                    std::shared_ptr<DataShareResultSet> &resultSet, std::string contentLoadKey)
{
    int columnIndex = 0;
    resultSet->GetColumnIndex(contentLoadKey, columnIndex);
    DataType columnType;
    resultSet->GetDataType(columnIndex, columnType);

    // NULL and BLOB are ignored here
    if (columnType == DataType::TYPE_STRING) {
        std::string stringValue;
        resultSet->GetString(columnIndex, stringValue);
        bucket.push_back(KeyWithValueType(contentStoreKey, stringValue));
    } else if (columnType == DataType::TYPE_INTEGER) {
        int intValue = 0;
        resultSet->GetInt(columnIndex, intValue);
        bucket.push_back(KeyWithValueType(contentStoreKey, (int64_t) intValue));
    } else if (columnType == DataType::TYPE_FLOAT) {
        double doubleValue = 0;
        resultSet->GetDouble(columnIndex, doubleValue);
        bucket.push_back(KeyWithValueType(contentStoreKey, doubleValue));
    } else if (columnType != DataType::TYPE_NULL) { // TYPE_NULL is just ignored
        HILOG_ERROR("PutResultValue unsupported columnType for key %{public}s is %{public}d",
            contentLoadKey.c_str(), columnType);
    }
}

void PutResultValue(std::vector<KeyWithValueType> &bucket,
                    std::shared_ptr<DataShareResultSet> &resultSet, std::string contentKey)
{
    PutResultValue(bucket, contentKey, resultSet, contentKey);
}

/**
 * @brief Converting resultset of a query to Email data ValuesBucket
 */
ValuesBucket resultSetAsEmail(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "email"));
    PutResultValue(bucketData, resultSet, "detail_info");
    PutResultValue(bucketData, resultSet, "alias_detail_info");
    PutResultValue(bucketData, resultSet, "custom_data");
    PutResultValue(bucketData, resultSet, "extend7");
    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to Name data ValuesBucket
 */
ValuesBucket resultSetAsName(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "name"));
    PutResultValue(bucketData, resultSet, "detail_info");
    PutResultValue(bucketData, resultSet, "alpha_name");
    PutResultValue(bucketData, resultSet, "other_lan_last_name");
    PutResultValue(bucketData, resultSet, "other_lan_first_name");
    PutResultValue(bucketData, resultSet, "family_name");
    PutResultValue(bucketData, resultSet, "middle_name_phonetic");
    PutResultValue(bucketData, resultSet, "given_name");
    PutResultValue(bucketData, resultSet, "given_name_phonetic");
    PutResultValue(bucketData, resultSet, "phonetic_name");
    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to Portrait data ValuesBucket
 */
ValuesBucket resultSetAsPortrait(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "photo"));
    PutResultValue(bucketData, resultSet, "detail_info");
    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to Event data ValuesBucket
 */
ValuesBucket resultSetAsEvent(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "contact_event"));

    PutResultValue(bucketData, resultSet, "detail_info");
    PutResultValue(bucketData, resultSet, "custom_data");
    PutResultValue(bucketData, resultSet, "extend7");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to Group data ValuesBucket
 */
ValuesBucket resultSetAsGroup(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "group_membership"));
    PutResultValue(bucketData, resultSet, "detail_info");
    PutResultValue(bucketData, resultSet, "group_name");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to ImAddress data ValuesBucket
 */
ValuesBucket resultSetAsImAddress(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "im"));
    PutResultValue(bucketData, resultSet, "detail_info");
    PutResultValue(bucketData, resultSet, "custom_data");
    PutResultValue(bucketData, resultSet, "extend7");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to PhoneNumber data ValuesBucket
 */
ValuesBucket resultSetAsPhone(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "phone"));
    PutResultValue(bucketData, resultSet, "detail_info");
    PutResultValue(bucketData, resultSet, "custom_data");
    PutResultValue(bucketData, resultSet, "extend7");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to PostalAddress data ValuesBucket
 */
ValuesBucket resultSetAsPostAddress(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "postal_address"));
    PutResultValue(bucketData, resultSet, "detail_info");
    PutResultValue(bucketData, resultSet, "neighborhood");
    PutResultValue(bucketData, resultSet, "pobox");
    PutResultValue(bucketData, resultSet, "postcode");
    PutResultValue(bucketData, resultSet, "region");
    PutResultValue(bucketData, resultSet, "street");
    PutResultValue(bucketData, resultSet, "city");
    PutResultValue(bucketData, resultSet, "country");
    PutResultValue(bucketData, resultSet, "custom_data");
    PutResultValue(bucketData, resultSet, "extend7");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to Relation data ValuesBucket
 */
ValuesBucket resultSetAsRelation(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "relation"));
    PutResultValue(bucketData, resultSet, "detail_info");
    PutResultValue(bucketData, resultSet, "custom_data");
    PutResultValue(bucketData, resultSet, "extend7");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to SipAddress data ValuesBucket
 */
ValuesBucket resultSetAsSipAddress(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "sip_address"));
    PutResultValue(bucketData, resultSet, "detail_info");
    PutResultValue(bucketData, resultSet, "custom_data");
    PutResultValue(bucketData, resultSet, "extend7");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to Website data ValuesBucket
 */
ValuesBucket resultSetAsWebsite(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "website"));
    PutResultValue(bucketData, resultSet, "detail_info");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to Nickname data ValuesBucket
 */
ValuesBucket resultSetAsNickname(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "nickname"));
    PutResultValue(bucketData, resultSet, "detail_info");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to Note data ValuesBucket
 */
ValuesBucket resultSetAsNote(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "note"));
    PutResultValue(bucketData, resultSet, "detail_info");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to Organization data ValuesBucket
 */
ValuesBucket resultSetAsOrganization(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    bucketData.push_back(KeyWithValueType("content_type", "organization"));
    PutResultValue(bucketData, resultSet, "detail_info");
    PutResultValue(bucketData, resultSet, "position");

    return allocBucketData(bucketData, errCode);
}

/**
 * @brief Converting resultset of a query to one of contact's ValuesBucket
 */
void addResultSetAsValuesBucket(std::vector<ValuesBucket> &contactData,
                                std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    int typeIdValue = 0;
    std::string typeId = "type_id";
    int columnIndexType = 0;
    resultSet->GetColumnIndex(typeId, columnIndexType);
    resultSet->GetInt(columnIndexType, typeIdValue);
    switch (typeIdValue) {
        case EMAIL:            contactData.push_back(resultSetAsEmail(resultSet, errCode)); return;
        case NAME:             contactData.push_back(resultSetAsName(resultSet, errCode)); return;
        case PHOTO:            contactData.push_back(resultSetAsPortrait(resultSet, errCode)); return;
        case CONTACT_EVENT:    contactData.push_back(resultSetAsEvent(resultSet, errCode)); return;
        case GROUP_MEMBERSHIP: contactData.push_back(resultSetAsGroup(resultSet, errCode)); return;
        case IM:               contactData.push_back(resultSetAsImAddress(resultSet, errCode)); return;
        case PHONE:            contactData.push_back(resultSetAsPhone(resultSet, errCode)); return;
        case POSTAL_ADDRESS:   contactData.push_back(resultSetAsPostAddress(resultSet, errCode)); return;
        case RELATION:         contactData.push_back(resultSetAsRelation(resultSet, errCode)); return;
        case SIP_ADDRESS:      contactData.push_back(resultSetAsSipAddress(resultSet, errCode)); return;
        case WEBSITE:          contactData.push_back(resultSetAsWebsite(resultSet, errCode)); return;
        case NICKNAME:         contactData.push_back(resultSetAsNickname(resultSet, errCode)); return;
        case NOTE:             contactData.push_back(resultSetAsNote(resultSet, errCode)); return;
        case ORGANIZATION:     contactData.push_back(resultSetAsOrganization(resultSet, errCode)); return;
        default:               return;
    }
}

// returns false when mem allocation failed
bool allocateDataForContact(ContactsData* allContacts, int contactIndex, int contactId, std::string searchKey,
                            std::vector<ValuesBucket> contactDataVector, int32_t *errCode)
{
    int bucketIndex = 0;
    // consider passing contactId as integer value
    ValuesBucket idBucket = singleStringAsValueBucket("id", std::to_string(contactId), errCode);
    if (*errCode != SUCCESS) {
        free(allContacts->contactsData[contactIndex].data);
        allContacts->contactsData[contactIndex].bucketCount = 0;
        allContacts->contactsData[contactIndex].data = nullptr;
        return false;
    }
    ValuesBucket searchKeyBucket = singleStringAsValueBucket("key", searchKey, errCode);
    if (*errCode != SUCCESS) {
        idBucket.freeContent();
        free(allContacts->contactsData[contactIndex].data);
        allContacts->contactsData[contactIndex].bucketCount = 0;
        allContacts->contactsData[contactIndex].data = nullptr;
        return false;
    }

    copyBucket(allContacts->contactsData[contactIndex].data, bucketIndex, idBucket);
    bucketIndex++;
    copyBucket(allContacts->contactsData[contactIndex].data, bucketIndex, searchKeyBucket);
    bucketIndex++;

    // copy bucket pointers from vector
    for (std::vector<ValuesBucket>::size_type i = 0; i < contactDataVector.size(); i++, bucketIndex++) {
        copyBucket(allContacts->contactsData[contactIndex].data, bucketIndex, contactDataVector[i]);
    }

    return true;
}

void releaseRresultSetMapBuckets(std::map<int, std::vector<ValuesBucket>> resultSetMap)
{
    std::map<int, std::vector<ValuesBucket>>::iterator it;
    for (it = resultSetMap.begin(); it != resultSetMap.end(); it++) {
        std::vector<ValuesBucket> contactDataVector = it->second;
        for (std::vector<ValuesBucket>::size_type i = 0; i < contactDataVector.size(); i++) {
            ValuesBucket bucket = contactDataVector[i];
            bucket.freeContent();
        }
    }
}

ContactsData* allocCollectedContacts(std::map<int, std::vector<ValuesBucket>> &resultSetMap,
                                     std::map<int, std::string> &quickSearchMap, int32_t *errCode)
{
    ContactsData* allContacts = (struct ContactsData*) malloc(sizeof(struct ContactsData));
    if (allContacts == nullptr) {
        HILOG_ERROR("ContactUtils::allocCollectedContacts fail to mem alloc");
        *errCode = ERROR;
        return nullptr;
    }

    size_t totalContacts = resultSetMap.size();
    if (totalContacts == 0 || totalContacts > MAX_CONTACTS) {
        free(allContacts);
        return nullptr;
    }

    allContacts->contactsData = (ContactData*) malloc(totalContacts * sizeof(ContactData));
    if (allContacts->contactsData == nullptr) {
        free(allContacts);
        HILOG_ERROR("ContactUtils::allocCollectedContacts fail to mem alloc");
        *errCode = ERROR;
        return nullptr;
    }
    allContacts->contactsCount = totalContacts;

    int contactIndex = 0;
    std::map<int, std::vector<ValuesBucket>>::iterator it;
    for (it = resultSetMap.begin(); it != resultSetMap.end(); it++, contactIndex++) {
        int contactId = it->first;
        std::vector<ValuesBucket> contactDataVector = it->second;
        std::string searchKey = quickSearchMap.find(contactId)->second;

        if (*errCode != SUCCESS) {
            allContacts->contactsData[contactIndex].bucketCount = 0;
            allContacts->contactsData[contactIndex].data = nullptr;
            continue;
        }
        size_t totalBuckets = 2 + contactDataVector.size(); // 2 more for contactId and searchKey buckets
        allContacts->contactsData[contactIndex].bucketCount = totalBuckets;
        allContacts->contactsData[contactIndex].data =
            (struct ValuesBucket*) malloc(totalBuckets * sizeof(struct ValuesBucket));
        if (allContacts->contactsData[contactIndex].data == nullptr) {
            *errCode = ERROR;
            continue;
        }

        allocateDataForContact(allContacts, contactIndex, contactId, searchKey, contactDataVector, errCode);
    }

    if (*errCode == SUCCESS) {
        return allContacts;
    }

    allContacts->freeContent();
    free(allContacts);
    allContacts = nullptr;
    releaseRresultSetMapBuckets(resultSetMap);

    return nullptr;
}

// it closes resultSet after parse
ContactsData* parseResultSetForContacts(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    if (resultSet == nullptr) {
        HILOG_ERROR("ContactUtils::parseResultSetForContacts resultSet is nullptr");
        return nullptr;
    }
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    HILOG_INFO("parseResultSetForContacts GetRowCount is %{public}d", rowCount);
    if (rowCount == 0) {
        resultSet->Close();
        return nullptr;
    }
    std::map<int, std::vector<ValuesBucket>> resultSetMap; // factored by contactId
    std::map<int, std::string> quickSearchMap;
    int resultSetNum = resultSet->GoToFirstRow();
    while (resultSetNum == 0) {
        int contactIdValue = 0;
        std::string contactIdKey = "contact_id";
        int contactIndex = 0;
        resultSet->GetColumnIndex(contactIdKey, contactIndex);
        resultSet->GetInt(contactIndex, contactIdValue);
        std::vector<ValuesBucket>& contactData = GetResultMapValue(resultSetMap, contactIdValue);
        PutQuickSearchKey(resultSet, quickSearchMap, contactIdValue);
        addResultSetAsValuesBucket(contactData, resultSet, errCode);
        if (*errCode != SUCCESS) {
            HILOG_ERROR("ContactUtils::parseResultSetForContacts fail to mem alloc");
            break;
        }
        resultSetNum = resultSet->GoToNextRow();
    }
    resultSet->Close();

    if (*errCode != SUCCESS) {
        std::map<int, std::vector<ValuesBucket>>::iterator it;
        for (it = resultSetMap.begin(); it != resultSetMap.end(); it++) {
            std::vector<ValuesBucket> contactDataVector = it->second;
            for (std::vector<ValuesBucket>::size_type i = 0; i < contactDataVector.size(); i++) {
                ValuesBucket bucket = contactDataVector[i];
                bucket.freeContent();
            }
        }
        return nullptr;
    }
    return allocCollectedContacts(resultSetMap, quickSearchMap, errCode);
}

/**
 * @brief Converting resultset of a query to Group data ValuesBucket
 */
void resultSetAsGroup(ValuesBucket* groups, int idx, std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    // content_type for group is redundant
    PutResultValue(bucketData, "detail_info", resultSet, "id");
    PutResultValue(bucketData, "group_name", resultSet, "group_name");

    ValuesBucket bucket = allocBucketData(bucketData, errCode);
    if (*errCode != SUCCESS) {
        HILOG_ERROR("ContactUtils::resultSetAsGroup fail to mem alloc");
    } else {
        copyBucket(groups, idx, bucket);
    }
}

/**
 * @brief Converting resultset of a query to Holder data ValuesBucket
 */
void resultSetAsHolder(ValuesBucket* holders, int idx, std::shared_ptr<DataShareResultSet> &resultSet,
                       int32_t *errCode)
{
    std::vector<KeyWithValueType> bucketData;
    // content_type for holder is redundant
    PutResultValue(bucketData, "detail_info", resultSet, "account_name");
    PutResultValue(bucketData, "custom_data", resultSet, "account_type");
    PutResultValue(bucketData, "extend7", resultSet, "id");

    ValuesBucket bucket = allocBucketData(bucketData, errCode);
    if (*errCode != SUCCESS) {
        HILOG_ERROR("ContactUtils::resultSetAsHolder fail to mem alloc");
    } else {
        copyBucket(holders, idx, bucket);
    }
}

// it closes resultSet after parse
GroupsData* parseResultSetForGroups(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    if (resultSet == nullptr) {
        HILOG_ERROR("ContactUtils::parseResultSetForGroups resultSet is nullptr");
        return nullptr;
    }
    int totalGroups = 0;
    resultSet->GetRowCount(totalGroups);
    HILOG_INFO("parseResultSetForGroups GetRowCount is %{public}d", totalGroups);
    if (totalGroups <= 0) {
        return nullptr;
    }

    GroupsData* allGroups = (GroupsData*) malloc(sizeof(GroupsData));
    if (allGroups == nullptr) {
        HILOG_ERROR("ContactUtils::parseResultSetForGroups fail to mem alloc");
        *errCode = ERROR;
        return nullptr;
    }
    allGroups->data = (struct ValuesBucket*) malloc(totalGroups * sizeof(struct ValuesBucket));
    if (allGroups->data == nullptr) {
        free(allGroups);
        HILOG_ERROR("ContactUtils::parseResultSetForGroups fail to mem alloc");
        *errCode = ERROR;
        return NULL;
    }
    allGroups->bucketCount = static_cast<uint64_t>(totalGroups);

    int resultSetNum = resultSet->GoToFirstRow();
    int count = 0;
    while (resultSetNum == 0) {
        resultSetAsGroup(allGroups->data, count, resultSet, errCode);
        if (*errCode != SUCCESS) {
            break;
        }
        resultSetNum = resultSet->GoToNextRow();
        count++;
    }
    resultSet->Close();

    if (*errCode != SUCCESS) {
        allGroups->freeContent();
        free(allGroups);
        allGroups = nullptr;
    }

    return allGroups;
}

// it closes resultSet after parse
HoldersData* parseResultSetForHolders(std::shared_ptr<DataShareResultSet> &resultSet, int32_t *errCode)
{
    if (resultSet == nullptr) {
        HILOG_ERROR("ContactUtils::parseResultSetForHolders resultSet is nullptr");
        return nullptr;
    }
    int totalHolders = 0;
    resultSet->GetRowCount(totalHolders);
    HILOG_INFO("parseResultSetForHolders GetRowCount is %{public}d", totalHolders);
    if (totalHolders <= 0) {
        return nullptr;
    }

    HoldersData* allHolders = (HoldersData*) malloc(sizeof(HoldersData));
    if (allHolders == nullptr) {
        HILOG_ERROR("ContactUtils::parseResultSetForHolders fail to mem alloc");
        *errCode = ERROR;
        return nullptr;
    }
    allHolders->data = (struct ValuesBucket*) malloc(totalHolders * sizeof(struct ValuesBucket));
    if (allHolders->data == nullptr) {
        free(allHolders);
        HILOG_ERROR("ContactUtils::parseResultSetForHolders fail to mem alloc");
        *errCode = ERROR;
        return nullptr;
    }
    allHolders->bucketCount = static_cast<uint64_t>(totalHolders);

    int resultSetNum = resultSet->GoToFirstRow();
    int count = 0;
    while (resultSetNum == 0) {
        resultSetAsHolder(allHolders->data, count, resultSet, errCode);
        if (*errCode != SUCCESS) {
            break;
        }
        resultSetNum = resultSet->GoToNextRow();
        count++;
    }
    resultSet->Close();

    if (*errCode != SUCCESS) {
        allHolders->freeContent();
        free(allHolders);
        allHolders = nullptr;
    }

    return allHolders;
}

} // namespace ContactsFfi
} // namespace OHOS
