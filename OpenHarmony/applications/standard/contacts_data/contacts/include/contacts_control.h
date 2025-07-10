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

#ifndef CONTACT_CONTROL_H
#define CONTACT_CONTROL_H

#include "ability.h"
#include "ability_context.h"
#include "context.h"
#include "datashare_helper.h"
#include "datashare_predicates.h"
#include "rdb_errno.h"
#include "rdb_helper.h"
#include "rdb_open_callback.h"
#include "rdb_predicates.h"
#include "rdb_store.h"
#include "result_set.h"

#include "contacts_napi_object.h"

namespace OHOS {
namespace ContactsApi {
class ContactsControl {
public:
    ContactsControl();
    ~ContactsControl();
    int RawContactInsert(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
        DataShare::DataShareValuesBucket rawContactValues);
    int ContactDataInsert(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
        std::vector<DataShare::DataShareValuesBucket> contactsDataValues);
    int RawContactUpdate(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
        DataShare::DataShareValuesBucket updateValues, DataShare::DataSharePredicates predicates);
    int ContactDataUpdate(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
        DataShare::DataShareValuesBucket updateValues, DataShare::DataSharePredicates predicates);
    int ContactDelete(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
        DataShare::DataSharePredicates predicates);
    int ContactDataDelete(std::shared_ptr<DataShare::DataShareHelper> dataShareHelper,
        DataShare::DataSharePredicates predicates);
    std::shared_ptr<DataShare::DataShareResultSet> ContactQuery(
        std::shared_ptr<DataShare::DataShareHelper> dataShareHelper, std::vector<std::string> columns,
        DataShare::DataSharePredicates predicates);
    std::shared_ptr<DataShare::DataShareResultSet> ContactDataQuery(
        std::shared_ptr<DataShare::DataShareHelper> dataShareHelper, std::vector<std::string> columns,
        DataShare::DataSharePredicates predicates);
    std::shared_ptr<DataShare::DataShareResultSet> GroupsQuery(
        std::shared_ptr<DataShare::DataShareHelper> dataShareHelper, std::vector<std::string> columns,
        DataShare::DataSharePredicates predicates);
    std::shared_ptr<DataShare::DataShareResultSet> MyCardQuery(
        std::shared_ptr<DataShare::DataShareHelper> dataShareHelper, std::vector<std::string> columns,
        DataShare::DataSharePredicates predicates);
    std::shared_ptr<DataShare::DataShareResultSet> HolderQuery(
        std::shared_ptr<DataShare::DataShareHelper> dataShareHelper, std::vector<std::string> columns,
        DataShare::DataSharePredicates predicates);
};
} // namespace ContactsApi
} // namespace OHOS
#endif // CONTACT_CONTROL_H
