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

#ifndef DISPLAY_INTERFACE_UTILS_H
#define DISPLAY_INTERFACE_UTILS_H

#include "hilog/log.h"

#define DISPLAY_CHECK(val, ...)           \
    do {                                  \
        if (val) {                        \
            HDF_LOGE("%{public}d@%{public}s: check failed", __LINE__, __func__); \
            __VA_ARGS__;                  \
        }                                 \
    } while (0)

#define DISPLAY_CHK_RETURN(val, ret, ...) \
    do {                                  \
        if (val) {                        \
            __VA_ARGS__;                  \
            return (ret);                 \
        }                                 \
    } while (0)

#define DISPLAY_CHK_CONDITION(ret, cond, func, ...)         \
    do {                                                    \
        if (cond == ret) {                                  \
            ret = func;                                     \
            if (cond != ret) {                              \
                __VA_ARGS__;                                \
            }                                               \
        }                                                   \
    } while (0)

#define DISPLAY_CHK_BREAK(val, ...)   \
    if (val) {                        \
        __VA_ARGS__;                  \
        break;                        \
    }                                 \

#endif // DISPLAY_INTERFACE_UTILS_H
