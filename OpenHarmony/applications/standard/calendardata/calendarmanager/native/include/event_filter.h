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

#ifndef CALENDAR_EVENT_FILTER_H
#define CALENDAR_EVENT_FILTER_H
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "datashare_predicates.h"

namespace OHOS::CalendarApi::Native {
using DataShare::DataSharePredicates;
class EventFilter {
public:
    EventFilter() = delete;
    ~EventFilter();
    EventFilter(std::shared_ptr<DataSharePredicates>);
    std::shared_ptr<DataSharePredicates> GetFilterPrediacates() const;
private:
    std::shared_ptr<DataSharePredicates> m_predicates;
};

std::shared_ptr<EventFilter> FilterById(const std::vector<int> &ids);
std::shared_ptr<EventFilter> FilterByTime(int64_t start, int64_t end);
std::shared_ptr<EventFilter> FilterByTitle(std::string_view title);
}
#endif