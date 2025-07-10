/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <stdbool.h>
#include "endian_internal.h"

#define B_L_SWAP16(A)  ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))
#define B_L_SWAP32(A)  ((((uint32_t)(A) & 0xff000000) >> 24) | (((uint32_t)(A) & 0x00ff0000) >> 8) | \
    (((uint32_t)(A) & 0x0000ff00) << 8) | (((uint32_t)(A) & 0x000000ff) << 24))

static bool CheckEndian(void);

static bool CheckEndian(void)
{
    union {
        int32_t i;
        uint8_t s[4];
    } c;
    c.i = 0x12345678;

    return (c.s[0] == 0x12);
}

uint32_t HtonlInter(uint32_t h)
{
    return CheckEndian() ? h : B_L_SWAP32(h);
}

uint32_t NtohlInter(uint32_t n)
{
    return CheckEndian() ? n : B_L_SWAP32(n);
}

uint16_t HtonsInter(uint16_t h)
{
    return CheckEndian() ? h : B_L_SWAP16(h);
}

uint16_t NtohsInter(uint16_t n)
{
    return CheckEndian() ? n : B_L_SWAP16(n);
}