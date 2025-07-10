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

#ifndef CONTACTS_UTILS_H
#define CONTACTS_UTILS_H

#include <cstdint>
#include "errors.h"

#include "hilog_wrapper_api.h"
#include "data_share_predicates_utils.h"
#include "datashare_result_set.h"
#include "datashare_values_bucket.h"

namespace OHOS {
namespace ContactsFfi {

constexpr int MAX_CONTACTS = 1024 * 1024;

const std::string CONTACTS_DATA_URI = "datashare:///com.ohos.contactsdataability";

// contactsData type
constexpr int EMAIL = 1;
constexpr int IM = 2;
constexpr int NICKNAME = 3;
constexpr int ORGANIZATION = 4;
constexpr int PHONE = 5;
constexpr int NAME = 6;
constexpr int POSTAL_ADDRESS = 7;
constexpr int PHOTO = 8;
constexpr int GROUP_MEMBERSHIP = 9;
constexpr int NOTE = 10;
constexpr int CONTACT_EVENT = 11;
constexpr int WEBSITE = 12;
constexpr int RELATION = 13;
constexpr int CONTACT_MISC = 14;
constexpr int HICALL_DEVICE = 15;
constexpr int CAMCARD = 16;
constexpr int SIP_ADDRESS = 17;

constexpr int ERROR = -1;
constexpr int SUCCESS = 0;

constexpr int PERMISSION_ERROR = 201;
constexpr int PARAMETER_ERROR = 401;

struct ValuesBucket {
    char** key = nullptr;
    DataShare::CValueType* value = nullptr;
    uint64_t size = 0;

    void freeContent()
    {
        bool hasKey = key != nullptr;
        bool hasValue = value != nullptr;
        for (uint64_t i = 0; i < size; i++) {
            if (hasKey) {
                free(key[i]);
                key[i] = nullptr;
            }
            if (hasValue &&
                value[i].tag == OHOS::DataShare::DataShareValueObjectType::TYPE_STRING) {
                free(value[i].string);
                value[i].string = nullptr;
            }
        }

        free(key);
        key = nullptr;

        free(value);
        value = nullptr;

        size = 0;
    }
};

using RawContact = ValuesBucket;

struct Buckets {
    ValuesBucket* data = nullptr;
    uint64_t bucketCount = 0;

    void freeContent()
    {
        if (data != nullptr) {
            for (uint64_t b = 0; b < bucketCount; b++) {
                ValuesBucket bucket = data[b];
                bucket.freeContent();
            }
            free(data);
            data = nullptr;
            bucketCount = 0;
        }
    }
};
using ContactData = Buckets;
using GroupsData = Buckets;
using HoldersData = Buckets;


struct ContactsData {
    ContactData* contactsData = nullptr;
    uint64_t contactsCount = 0;

    void freeContent()
    {
        if (contactsData != nullptr) {
            for (uint64_t i = 0; i < contactsCount; i++) {
                ContactData contactData = contactsData[i];
                contactData.freeContent();
            }
            free(contactsData);
            contactsData = nullptr;
            contactsCount = 0;
        }
    }
};

char* TransformFromString(std::string &str, int32_t* errCode);

// this is a pair of key and a kind of DataShare::CValueType but with std::string string
struct KeyWithValueType {
    std::string key;
    int64_t integer = 0;
    double dou = 0.0;
    std::string string;
    uint8_t tag = static_cast<int>(DataShare::DataType::TYPE_NULL);

    KeyWithValueType(std::string k, int64_t v)
    {
        this->key = k;
        this->integer = v;
        this->tag = static_cast<int>(DataShare::DataType::TYPE_INTEGER);
    }

    KeyWithValueType(std::string k, double v)
    {
        this->key = k;
        this->dou = v;
        this->tag = static_cast<int>(DataShare::DataType::TYPE_FLOAT);
    }

    KeyWithValueType(std::string k, std::string v)
    {
        this->key = k;
        this->string = v;
        this->tag = static_cast<int>(DataShare::DataType::TYPE_STRING);
    }

    int32_t allocToBucket(ValuesBucket* dst, int dstIdx, int idx, int32_t *errCode)
    {
        dst[dstIdx].key[idx] = TransformFromString(this->key, errCode);
        if (*errCode != SUCCESS) {
            return *errCode;
        }
        dst[dstIdx].value[idx].tag = this->tag;
        if (this->tag == static_cast<int>(DataShare::DataType::TYPE_INTEGER)) {
            dst[dstIdx].value[idx].integer = this->integer;
        } else if (this->tag == static_cast<int>(DataShare::DataType::TYPE_FLOAT)) {
            dst[dstIdx].value[idx].dou = this->dou;
        } else {
            std::string str = "";
            if (this->tag == static_cast<int>(DataShare::DataType::TYPE_STRING)) {
                str = this->string;
            } else { // put empty string for unsupported tag
                HILOG_ERROR("allocToBucket: for key %{public}s has tag %{public}d", this->key.c_str(), this->tag);
            }
            dst[dstIdx].value[idx].string = TransformFromString(str, errCode);
            if (*errCode != SUCCESS) {
                free(dst[dstIdx].key[idx]);
                return *errCode;
            }
        }
        return SUCCESS;
    }
};

OHOS::DataShare::DataShareValuesBucket convertToDataShareVB(OHOS::ContactsFfi::ValuesBucket vb);

ContactsData* parseResultSetForContacts(std::shared_ptr<OHOS::DataShare::DataShareResultSet> &resultSet,
                                        int32_t *errCode);

GroupsData* parseResultSetForGroups(std::shared_ptr<OHOS::DataShare::DataShareResultSet> &resultSet, int32_t *errCode);

HoldersData* parseResultSetForHolders(std::shared_ptr<OHOS::DataShare::DataShareResultSet> &resultSet,
                                      int32_t *errCode);

} // namespace ContactsFfi
} // namespace OHOS

#endif // CONTACTS_UTILS_H
