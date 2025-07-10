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


#include "event_filter.h"
#include "calendar_log.h"

namespace OHOS::CalendarApi::Native {

EventFilter::EventFilter(std::shared_ptr<DataShare::DataSharePredicates> predicates)
    : m_predicates(std::move(predicates))
{
}

EventFilter::~EventFilter()
{
}

std::shared_ptr<DataShare::DataSharePredicates> EventFilter::GetFilterPrediacates() const
{
    return m_predicates;
}

std::shared_ptr<EventFilter> FilterById(const std::vector<int> &ids)
{
    LOG_DEBUG("FilterById");
    auto predicates = std::make_shared<DataShare::DataSharePredicates>();
    if (!predicates) {
        LOG_ERROR("DataSharePredicates make failed");
        return nullptr;
    }
    if (ids.empty()) {
        return nullptr;
    }
    predicates->BeginWrap();
    predicates->EqualTo("_id", ids.at(0));
    for (size_t index = 1; index < ids.size(); ++index) {
        predicates->Or();
        predicates->EqualTo("_id", ids.at(index));
    }
    predicates->EndWrap();
    return std::make_shared<EventFilter>(predicates);
}


std::shared_ptr<EventFilter> FilterByTime(int64_t start, int64_t end)
{
    LOG_DEBUG("FilterByTime start:%{private}s end:%{private}s",
        std::to_string(start).c_str(), std::to_string(end).c_str());
    auto predicates = std::make_shared<DataShare::DataSharePredicates>();
    if (!predicates) {
        LOG_ERROR("DataSharePredicates make failed");
        return nullptr;
    }
    // Obtain intersections based on time.
    predicates->BeginWrap();
    predicates->LessThanOrEqualTo("dtstart", end);
    predicates->And();
    predicates->GreaterThanOrEqualTo("dtstart", start);
    predicates->EndWrap();
    predicates->And();
    predicates->BeginWrap();
    predicates->LessThanOrEqualTo("dtend", end);
    predicates->And();
    predicates->GreaterThanOrEqualTo("dtend", start);
    predicates->EndWrap();
    return std::make_shared<EventFilter>(predicates);
}

std::shared_ptr<EventFilter> FilterByTitle(std::string_view title)
{
    LOG_DEBUG("FilterByTitle %{private}s", std::string(title).c_str());
    auto predicates = std::make_shared<DataShare::DataSharePredicates>();
    predicates->Like("title", "%" + std::string(title) + "%");
    return std::make_shared<EventFilter>(predicates);
}
}