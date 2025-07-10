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
#ifndef NONLINUX_MINGW64FIX_H
#define NONLINUX_MINGW64FIX_H
#if defined(is_mingw) && is_mingw
// in mingw 64 _pid_t define to int64
// but in linux it always int

#define _PID_T_
using _pid_t = int;
using pid_t = _pid_t;
#endif
#endif
