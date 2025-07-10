/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "cj_ffi/cj_common_ffi.h"

extern "C" {
FFI_EXPORT int FfiOHOSSysDateTimeSetTime = 0;
FFI_EXPORT int FfiOHOSSysDateTimegetCurrentTime = 0;
FFI_EXPORT int FfiOHOSSysDateTimegetRealActiveTime = 0;
FFI_EXPORT int FfiOHOSSysDateTimegetRealTime = 0;
FFI_EXPORT int FfiOHOSSysDateTimeGetTime = 0;
FFI_EXPORT int FfiOHOSSysDateTimeGetUptime = 0;
FFI_EXPORT int FfiOHOSSysSetTimezone = 0;
FFI_EXPORT int FfiOHOSSysGetTimezone = 0;
}