/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef CALENDAR_DATA_SHARE_HELPER_MANAGER_H
#define CALENDAR_DATA_SHARE_HELPER_MANAGER_H

#include <memory>
#include "singleton.h"
#include "datashare_helper.h"

namespace OHOS::CalendarApi {
class DataShareHelperManager : public OHOS::Singleton<DataShareHelperManager> {
public:
    void SetDataShareHelper(std::shared_ptr<DataShare::DataShareHelper> helper);
     /**
     * @brief Get the dataShareHelper instance.
     *
     * @return Returns DataShareHelper instance or nullptr when failed.
     */
    std::shared_ptr<DataShare::DataShareHelper> GetDataShareHelper();
    /**
     * @brief Inserts a single data record into the database.
     *
     * @param uri Indicates the path of the data to operate.
     * @param value  Indicates the data record to insert. If this parameter is null, a blank row will be inserted.
     *
     * @return Returns the index of the inserted data record.
     */
    int Insert(const Uri &uri, const DataShare::DataShareValuesBucket &value);
    
     /**
     * @brief batch insert data records into the database.
     *
     * @param uri Indicates the path of the data to operate.
     * @param values  Indicates the data records to insert. If this parameter is null, a blank row will be inserted.
     *
     * @return Returns the index of the inserted data count.
     */
    int BatchInsert(const Uri &uri, const std::vector<DataShare::DataShareValuesBucket> &values);
    
    /**
     * @brief Updates data records in the database.
     *
     * @param uri Indicates the path of data to update.
     * @param predicates Indicates filter criteria. You should define the processing logic when this parameter is null.
     * @param value Indicates the data to update. This parameter can be null.
     *
     * @return Returns the number of data records updated.
     */
    int Update(const Uri &uri, const DataShare::DataSharePredicates &predicates,
        const DataShare::DataShareValuesBucket &value);

    /**
     * @brief Deletes one or more data records from the database.
     *
     * @param uri Indicates the path of the data to operate.
     * @param predicates Indicates filter criteria. You should define the processing logic when this parameter is null.
     *
     * @return Returns the number of data records deleted.
     */
    int Delete(const Uri &uri, const DataShare::DataSharePredicates &predicates);

    /**
     * @brief Query records from the database.
     *
     * @param uri Indicates the path of data to query.
     * @param predicates Indicates filter criteria. You should define the processing logic when this parameter is null.
     * @param columns Indicates the columns to query. If this parameter is null, all columns are queried.
     * @param businessError Indicates the error by query.
     *
     * @return Returns the query result.
     */
    std::shared_ptr<DataShare::DataShareResultSet> Query(const Uri &uri,
        const DataShare::DataSharePredicates &predicates, std::vector<std::string> &columns,
        DataShare::DatashareBusinessError *businessError = nullptr);
private:
     /**
     * @brief Create a dataShareHelper instance.
     *
     * @return Returns DataShareHelper instance or nullptr when failed.
     */
    std::shared_ptr<DataShare::DataShareHelper> CreateDataShareHelper();
     /**
     * @brief Destroy the dataShareHelper instance when there is no dataShare operations for a period of time,
     * otherwise no destruction is performed.
     *
     * @return Returns a boolean whether the dataShare helper is destroyed.
     */
    bool DestroyDataShareHelper();
     /**
     * @brief set the timer when DataShareHelper should be destoried when there is no operations
     * during a period of time, default to 3000 milliseconds
     *
     * @param milliseconds set the timer in milliseconds to destory dataShareHeloer when there is no
     * operation performed, default to 3000 millis.
     *
     * @return void.
     */
    void SetDataShareHelperTimer(int milliseconds = 3000);
    std::shared_ptr<DataShare::DataShareHelper> m_dataShareHelper;
    std::atomic<long long> expire = 0;
    std::atomic<uint32_t> useCount = 0;
    std::recursive_mutex dataShareLock;
};
} // namespace OHOS::CalendarApi

#endif // CALENDAR_DATA_SHARE_HELPER_MANAGER_H