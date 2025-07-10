/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef HIPERF_HILOG
#define HIPERF_HILOG

#include <securec.h>
#include <stdarg.h>

#ifndef CONFIG_NO_HILOG
#define HILOG_PUBLIC  "{public}"
#define HILOG_NEWLINE ""
#else
#define HILOG_PUBLIC  ""
#define HILOG_NEWLINE "\n"
#endif

#ifdef IS_RELEASE_VERSION
#define FORMATTED(fmt, ...) "[%" HILOG_PUBLIC "s]" fmt HILOG_NEWLINE, __FUNCTION__, ##__VA_ARGS__
#else
#define FORMATTED(fmt, ...)                                                                             \
    "[%" HILOG_PUBLIC "s:%" HILOG_PUBLIC "d] %" HILOG_PUBLIC "s# " fmt HILOG_NEWLINE, __FILE_NAME__,    \
        __LINE__, __FUNCTION__, ##__VA_ARGS__
#endif

#ifndef CONFIG_NO_HILOG
#include "hilog/log.h"

#ifdef HIPERF_HILOGF
#undef HIPERF_HILOGF
#endif

#ifdef HIPERF_HILOGE
#undef HIPERF_HILOGE
#endif

#ifdef HIPERF_HILOGW
#undef HIPERF_HILOGW
#endif

#ifdef HIPERF_HILOGI
#undef HIPERF_HILOGI
#endif

#ifdef HIPERF_HILOGD
#undef HIPERF_HILOGD
#endif

// param of log interface, such as HIPERF_HILOGF.
enum HiperfModule {
    MODULE_DEFAULT = 0,
    MODULE_JS_NAPI,
    MODULE_CPP_API,
};

static constexpr unsigned int BASE_HIPERF_DOMAIN_ID = 0xD002D0D;
static constexpr OHOS::HiviewDFX::HiLogLabel HIPERF_HILOG_LABLE[] = {
    {LOG_CORE, BASE_HIPERF_DOMAIN_ID, "hiperf"},
    {LOG_CORE, BASE_HIPERF_DOMAIN_ID, "HiperfJSNAPI"},
    {LOG_CORE, BASE_HIPERF_DOMAIN_ID, "HiperfCPPAPI"},
};

#ifdef LOG_DOMAIN
#undef LOG_DOMAIN
#endif
#define LOG_DOMAIN 0xD002D0D

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "hiperf"

// In order to improve performance, do not check the module range

#define HIPERF_HILOGF(module, ...)                                                                 \
    HILOG_FATAL(LOG_CORE, __VA_ARGS__)
#define HIPERF_HILOGE(module, ...)                                                                 \
    HILOG_ERROR(LOG_CORE, __VA_ARGS__)
#define HIPERF_HILOGW(module, ...)                                                                 \
    HILOG_WARN(LOG_CORE, __VA_ARGS__)
#define HIPERF_HILOGI(module, ...)                                                                 \
    HILOG_INFO(LOG_CORE, __VA_ARGS__)
#define HIPERF_HILOGD(module, ...)                                                                 \
    HILOG_DEBUG(LOG_CORE, __VA_ARGS__)
#else

#define HIPERF_HILOGF(module, ...) printf(FORMATTED(__VA_ARGS__))
#define HIPERF_HILOGE(module, ...) printf(FORMATTED(__VA_ARGS__))
#define HIPERF_HILOGW(module, ...) printf(FORMATTED(__VA_ARGS__))
#define HIPERF_HILOGI(module, ...) printf(FORMATTED(__VA_ARGS__))
#define HIPERF_HILOGD(module, ...) printf(FORMATTED(__VA_ARGS__))

#endif // CONFIG_NO_HILOG

static inline std::string StringFormat(const char* fmt, ...)
{
    va_list vargs;
    char buf[1024] = {0}; // 1024: buf size
    std::string format(fmt);
    va_start(vargs, fmt);
    if (vsnprintf_s(buf, sizeof(buf), sizeof(buf) - 1, format.c_str(), vargs) < 0) {
        va_end(vargs);
        return "";
    }

    va_end(vargs);
    return buf;
}

#define NO_RETVAL /* retval */
#define LOG_TYPE_PRINTF 2
#define LOG_TYPE_WITH_HILOG 3
#define CHECK_TRUE(expr, retval, log, fmt, ...)                                                    \
    do {                                                                                           \
        if (expr) {                                                                                \
            if (log == 1) {                                                                        \
                std::string str = StringFormat(fmt, ##__VA_ARGS__);                                \
                HLOGE("%s", str.c_str());                                                          \
            } else if (log == LOG_TYPE_PRINTF) {                                                   \
                printf("%s", StringFormat(fmt, ##__VA_ARGS__).c_str());                            \
            } else if (log == LOG_TYPE_WITH_HILOG) {                                               \
                std::string str = StringFormat(fmt, ##__VA_ARGS__);                                \
                HLOGE("%s", str.c_str());                                                          \
                HIPERF_HILOGE(MODULE_DEFAULT, "%{public}s", str.c_str());                          \
            }                                                                                      \
            return retval;                                                                         \
        }                                                                                          \
    } while (0)

#endif // HIPERF_HILOG
