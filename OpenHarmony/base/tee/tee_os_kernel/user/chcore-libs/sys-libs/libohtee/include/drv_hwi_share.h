/*
 * Copyright (c) 2023 Institute of Parallel And Distributed Systems (IPADS), Shanghai Jiao Tong University (SJTU)
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 */

#ifndef DRV_HWI_SHARED_H
#define DRV_HWI_SHARED_H

#include <stdint.h>
typedef unsigned int UINT32;
#define VOID void
typedef UINT32 HWI_ARG_T;
typedef VOID (*HWI_PROC_FUNC)(HWI_ARG_T);

uint32_t sys_hwi_create(uint32_t hwi_num, uint16_t hwi_prio, uint16_t mode,
                        HWI_PROC_FUNC handler, uint32_t args);
uint32_t sys_hwi_resume(uint32_t hwi_num, uint16_t hwi_prio, uint16_t mode);
uint32_t sys_hwi_delete(uint32_t hwi_num);
uint32_t sys_hwi_disable(uint32_t hwi_num);
uint32_t sys_hwi_enable(uint32_t hwi_num);
int32_t sys_hwi_notify(uint32_t hwi_num);

#endif /* DRV_HWI_SHARED_H */
