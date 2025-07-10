/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CJ_DECLARE_MACRO_H
#define CJ_DECLARE_MACRO_H

#include <stdint.h>

#define FFI_DECLARE_INSTANCE(CLASS, ...) \
    int32_t FfiCert##CLASS##NewInstance(__VA_ARGS__, CLASS *returnObj); \
    void FfiCert##CLASS##DeleteInstance(CLASS self)

#define FFI_DECLARE_API(CLASS, FUNC, RETURN_TYPE, ...) \
    RETURN_TYPE FfiCert##CLASS##FUNC(const CLASS self, ##__VA_ARGS__)

#endif //CJ_DECLARE_MACRO_H
