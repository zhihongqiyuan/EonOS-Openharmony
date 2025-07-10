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

#include "contacts_control.h"

#include "hilog_wrapper_api.h"

namespace OHOS {
namespace ContactsApi {
ContactsControl::ContactsControl(void)
{
}

ContactsControl::~ContactsControl()
{
}

int ContactsControl::RawContactInsert(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
    DataShare::DataShareValuesBucket rawContactValues)
{
    int code = 0;
    OHOS::Uri uriRawContact("datashare:///com.ohos.contactsdataability/contacts/raw_contact");
    code = dataShareHelper->Insert(uriRawContact, rawContactValues);
    HILOG_INFO(" ContactsControl::RawContactInsert insert code %{public}d", code);
    rawContactValues.Clear();
    return code;
}

int ContactsControl::ContactDataInsert(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
    std::vector<DataShare::DataShareValuesBucket> contactDataValues)
{
    int code = 0;
    OHOS::Uri uriContactData("datashare:///com.ohos.contactsdataability/contacts/contact_data");
    code = dataShareHelper->BatchInsert(uriContactData, contactDataValues);
    HILOG_INFO(" ContactsControl::ContactDataInsert insert code %{public}d", code);
    contactDataValues.clear();
    return code;
}
int ContactsControl::ContactDataDelete(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
    DataShare::DataSharePredicates predicates)
{
    int code = 0;
    OHOS::Uri uriContactData("datashare:///com.ohos.contactsdataability/contacts/contact_data");
    code = dataShareHelper->Delete(uriContactData, predicates);
    HILOG_INFO(" ContactsControl::ContactDataDelete delete code %{public}d", code);
    return code;
}

int ContactsControl::RawContactUpdate(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
    DataShare::DataShareValuesBucket updateValues, DataShare::DataSharePredicates predicates)
{
    int code = 0;
    OHOS::Uri uriRawContact("datashare:///com.ohos.contactsdataability/contacts/raw_contact");
    code = dataShareHelper->Update(uriRawContact, predicates, updateValues);
    return code;
}

int ContactsControl::ContactDataUpdate(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
    DataShare::DataShareValuesBucket updateValues, DataShare::DataSharePredicates predicates)
{
    int code = 0;
    OHOS::Uri uriContactData("datashare:///com.ohos.contactsdataability/contacts/contact_data");
    code = dataShareHelper->Update(uriContactData, predicates, updateValues);
    return code;
}

int ContactsControl::ContactDelete(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
    DataShare::DataSharePredicates predicates)
{
    int code = 0;
    OHOS::Uri uriContact("datashare:///com.ohos.contactsdataability/contacts/contact");
    code = dataShareHelper->Delete(uriContact, predicates);
    return code;
}

std::shared_ptr<DataShare::DataShareResultSet> ContactsControl::HolderQuery(
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper, std::vector<std::string> columns,
    DataShare::DataSharePredicates predicates)
{
    std::shared_ptr<DataShare::DataShareResultSet> resultSet;
    OHOS::Uri uriContact("datashare:///com.ohos.contactsdataability/contacts/account");
    resultSet = dataShareHelper->Query(uriContact, predicates, columns);
    return resultSet;
}

std::shared_ptr<DataShare::DataShareResultSet> ContactsControl::ContactQuery(
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper, std::vector<std::string> columns,
    DataShare::DataSharePredicates predicates)
{
    HILOG_INFO("ContactsControl::ContactQuery is start");
    std::shared_ptr<DataShare::DataShareResultSet> resultSet;
    OHOS::Uri uriContact("datashare:///com.ohos.contactsdataability/contacts/contact_data");
    resultSet = dataShareHelper->Query(uriContact, predicates, columns);
    return resultSet;
}

std::shared_ptr<DataShare::DataShareResultSet> ContactsControl::ContactDataQuery(
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper, std::vector<std::string> columns,
    DataShare::DataSharePredicates predicates)
{
    std::shared_ptr<DataShare::DataShareResultSet> resultSet;
    OHOS::Uri uriContactData("datashare:///com.ohos.contactsdataability/contacts/contact_data");
    resultSet = dataShareHelper->Query(uriContactData, predicates, columns);
    return resultSet;
}

std::shared_ptr<DataShare::DataShareResultSet> ContactsControl::GroupsQuery(
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper, std::vector<std::string> columns,
    DataShare::DataSharePredicates predicates)
{
    std::shared_ptr<DataShare::DataShareResultSet> resultSet;
    OHOS::Uri uriGroups("datashare:///com.ohos.contactsdataability/contacts/groups");
    resultSet = dataShareHelper->Query(uriGroups, predicates, columns);
    return resultSet;
}

std::shared_ptr<DataShare::DataShareResultSet> ContactsControl::MyCardQuery(
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper, std::vector<std::string> columns,
    DataShare::DataSharePredicates predicates)
{
    std::shared_ptr<DataShare::DataShareResultSet> resultSet;
    OHOS::Uri uriProfileContact("datashare:///com.ohos.contactsdataability/profile/contact_data");
    resultSet = dataShareHelper->Query(uriProfileContact, predicates, columns);
    return resultSet;
}
} // namespace ContactsApi
} // namespace OHOS