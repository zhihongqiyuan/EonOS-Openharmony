/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef NAPI_CALENDAR_LOG_H
#define NAPI_CALENDAR_LOG_H

#ifndef LOG_TAG
#define LOG_TAG "CalendarApi"
#undef LOG_DOMAIN
#define LOG_DOMAIN 0x050a
#endif

#include "hilog/log.h"

namespace OHOS::CalendarApi {
#define LOG_DEBUG(fmt, ...) HILOG_DEBUG(LOG_APP, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) HILOG_INFO(LOG_APP, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) HILOG_WARN(LOG_APP, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) HILOG_ERROR(LOG_APP, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) HILOG_FATAL(LOG_APP, fmt, ##__VA_ARGS__)
} // namespace OHOS::CalendarApi
#endif

