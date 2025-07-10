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

#ifndef RESULT_CONVERT_H
#define RESULT_CONVERT_H

#include <map>
#include <string>

#include "contacts_napi_common.h"
#include "napi/native_common.h"
#include "napi/native_node_api.h"
#include "result_set.h"

namespace OHOS {
namespace ContactsApi {
class ResultConvert {
public:
    ResultConvert();
    ~ResultConvert();
    napi_value GetNapiValue(napi_env env, const std::string keyChar, napi_value object);
    napi_value GetNapiElementArray(napi_env env, napi_value object, const std::string keyChar);
    napi_value GetNapiElementObject(napi_env env, napi_value object, const std::string keyChar);
    napi_value GetResultValue(
        napi_env env, std::string &contentKey, std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    napi_value ResultSetToHolder(napi_env env, std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    napi_value ResultSetToGroup(napi_env env, std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    napi_value ResultSetToObject(napi_env env, std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    napi_value ConvertContactArray(
        napi_env env, std::map<int, napi_value> &resultSetMap, std::map<int, std::string> &quickSearchMap);
    void PutQuickSearchKey(napi_env env, std::shared_ptr<DataShare::DataShareResultSet> &resultSet,
        std::map<int, std::string> &quickSearchMap, int contactsIdValue);
    void ConvertContactObject(
        napi_env env, napi_value napiObject, std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    napi_value GetResultMapValue(napi_env env, std::map<int, napi_value> &resultSetMap, int &contactsId);
    void ConvertEmail(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertName(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertUri(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertEvent(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertGroup(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertImAddress(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertPhoneNumber(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertPostalAddress(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertRelation(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertSipAddress(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertWebsite(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertNickName(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertNote(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    void ConvertOrganization(napi_env env, napi_value napiObject, int &typeId,
        std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
    napi_value CreateNapiStringValue(napi_env env, const std::string key);
    bool IsEmpty(std::shared_ptr<DataShare::DataShareResultSet> &resultSet);
};
} // namespace ContactsApi
} // namespace OHOS
#endif // RESULT_CONVERT_H
