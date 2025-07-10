/*
 * Copyright (C) 2010-2017 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU
 * licence.
 *
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef H_MALI_UTGARD_IOCTL_H
#define H_MALI_UTGARD_IOCTL_H

#include <linux/ioctl.h>
#include <linux/fs.h> /* file system operations */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file mali_kernel_ioctl.h
 * Interface to the Linux device driver.
 * This file describes the interface needed to use the Linux device driver.
 * Its interface is designed to used by the HAL implementation through a thin arch layer.
 */

/**
 * ioctl commands
 */

#define MALI_IOC_BASE 0x82
#define MALI_IOC_CORE_BASE (MALI_UK_CORE_SUBSYSTEM + MALI_IOC_BASE)
#define MALI_IOC_MEMORY_BASE (MALI_UK_MEMORY_SUBSYSTEM + MALI_IOC_BASE)
#define MALI_IOC_PP_BASE (MALI_UK_PP_SUBSYSTEM + MALI_IOC_BASE)
#define MALI_IOC_GP_BASE (MALI_UK_GP_SUBSYSTEM + MALI_IOC_BASE)
#define MALI_IOC_PROFILING_BASE (MALI_UK_PROFILING_SUBSYSTEM + MALI_IOC_BASE)
#define MALI_IOC_VSYNC_BASE (MALI_UK_VSYNC_SUBSYSTEM + MALI_IOC_BASE)

#define MALI_IOC_WAIT_FOR_NOTIFICATION                                                                                 \
    _IOWR(MALI_IOC_CORE_BASE, MALI_UK_WAIT_FOR_NOTIFICATION, mali_uk_wait_for_notification_s)
#define MALI_IOC_GET_API_VERSION _IOWR(MALI_IOC_CORE_BASE, MALI_UK_GET_API_VERSION, u32)
#define MALI_IOC_GET_API_VERSION_V2 _IOWR(MALI_IOC_CORE_BASE, MALI_UK_GET_API_VERSION, mali_uk_get_api_version_v2_s)
/* rk_ext. */
#define MALI_IOC_GET_RK_KO_VERSION _IOWR(MALI_IOC_CORE_BASE, MALI_GET_RK_KO_VERSION, mali_rk_ko_version_s)
#define MALI_IOC_POST_NOTIFICATION _IOWR(MALI_IOC_CORE_BASE, MALI_UK_POST_NOTIFICATION, mali_uk_post_notification_s)
#define MALI_IOC_GET_USER_SETTING _IOWR(MALI_IOC_CORE_BASE, MALI_UK_GET_USER_SETTING, mali_uk_get_user_setting_s)
#define MALI_IOC_GET_USER_SETTINGS _IOWR(MALI_IOC_CORE_BASE, MALI_UK_GET_USER_SETTINGS, mali_uk_get_user_settings_s)
#define MALI_IOC_REQUEST_HIGH_PRIORITY                                                                                 \
    _IOW(MALI_IOC_CORE_BASE, MALI_UK_REQUEST_HIGH_PRIORITY, mali_uk_request_high_priority_s)
#define MALI_IOC_TIMELINE_GET_LATEST_POINT                                                                             \
    _IOWR(MALI_IOC_CORE_BASE, MALI_UK_TIMELINE_GET_LATEST_POINT, mali_uk_timeline_get_latest_point_s)
#define MALI_IOC_TIMELINE_WAIT _IOWR(MALI_IOC_CORE_BASE, MALI_UK_TIMELINE_WAIT, mali_uk_timeline_wait_s)
#define MALI_IOC_TIMELINE_CREATE_SYNC_FENCE                                                                            \
    _IOWR(MALI_IOC_CORE_BASE, MALI_UK_TIMELINE_CREATE_SYNC_FENCE, mali_uk_timeline_create_sync_fence_s)
#define MALI_IOC_SOFT_JOB_START _IOWR(MALI_IOC_CORE_BASE, MALI_UK_SOFT_JOB_START, mali_uk_soft_job_start_s)
#define MALI_IOC_SOFT_JOB_SIGNAL _IOWR(MALI_IOC_CORE_BASE, MALI_UK_SOFT_JOB_SIGNAL, mali_uk_soft_job_signal_s)
#define MALI_IOC_PENDING_SUBMIT _IOWR(MALI_IOC_CORE_BASE, MALI_UK_PENDING_SUBMIT, mali_uk_pending_submit_s)

#define MALI_IOC_MEM_ALLOC _IOWR(MALI_IOC_MEMORY_BASE, MALI_UK_ALLOC_MEM, _ali_uk_alloc_mem_s)
#define MALI_IOC_MEM_FREE _IOWR(MALI_IOC_MEMORY_BASE, MALI_UK_FREE_MEM, mali_uk_free_mem_s)
#define MALI_IOC_MEM_BIND _IOWR(MALI_IOC_MEMORY_BASE, MALI_UK_BIND_MEM, mali_uk_bind_mem_s)
#define MALI_IOC_MEM_UNBIND _IOWR(MALI_IOC_MEMORY_BASE, MALI_UK_UNBIND_MEM, mali_uk_unbind_mem_s)
#define MALI_IOC_MEM_COW _IOWR(MALI_IOC_MEMORY_BASE, MALI_UK_COW_MEM, mali_uk_cow_mem_s)
#define MALI_IOC_MEM_COW_MODIFY_RANGE _IOWR(MALI_IOC_MEMORY_BASE, MALI_UK_COW_MODIFY_RANGE, mali_uk_cow_modify_range_s)
#define MALI_IOC_MEM_RESIZE _IOWR(MALI_IOC_MEMORY_BASE, MALI_UK_RESIZE_MEM, mali_uk_mem_resize_s)
#define MALI_IOC_MEM_DMA_BUF_GET_SIZE _IOR(MALI_IOC_MEMORY_BASE, MALI_UK_DMA_BUF_GET_SIZE, mali_uk_dma_buf_get_size_s)
#define MALI_IOC_MEM_QUERY_MMU_PAGE_TABLE_DUMP_SIZE                                                                    \
    _IOR(MALI_IOC_MEMORY_BASE, MALI_UK_QUERY_MMU_PAGE_TABLE_DUMP_SIZE, mali_uk_query_mmu_page_table_dump_size_s)
#define MALI_IOC_MEM_DUMP_MMU_PAGE_TABLE                                                                               \
    _IOWR(MALI_IOC_MEMORY_BASE, MALI_UK_DUMP_MMU_PAGE_TABLE, mali_uk_dump_mmu_page_table_s)
#define MALI_IOC_MEM_WRITE_SAFE _IOWR(MALI_IOC_MEMORY_BASE, MALI_UK_MEM_WRITE_SAFE, mali_uk_mem_write_safe_s)

#define MALI_IOC_PP_START_JOB _IOWR(MALI_IOC_PP_BASE, MALI_UK_PP_START_JOB, mali_uk_pp_start_job_s)
#define MALI_IOC_PP_AND_GP_START_JOB _IOWR(MALI_IOC_PP_BASE, MALI_UK_PP_AND_GP_START_JOB, mali_uk_pp_and_gp_start_job_s)
#define MALI_IOC_PP_NUMBER_OF_CORES_GET                                                                                \
    _IOR(MALI_IOC_PP_BASE, MALI_UK_GET_PP_NUMBER_OF_CORES, mali_uk_get_pp_number_of_cores_s)
#define MALI_IOC_PP_CORE_VERSION_GET _IOR(MALI_IOC_PP_BASE, MALI_UK_GET_PP_CORE_VERSION, mali_uk_get_pp_core_version_s)
#define MALI_IOC_PP_DISABLE_WB _IOW(MALI_IOC_PP_BASE, MALI_UK_PP_DISABLE_WB, mali_uk_pp_disable_wb_s)

#define MALI_IOC_GP2_START_JOB _IOWR(MALI_IOC_GP_BASE, MALI_UK_GP_START_JOB, mali_uk_gp_start_job_s)
#define MALI_IOC_GP2_NUMBER_OF_CORES_GET                                                                               \
    _IOR(MALI_IOC_GP_BASE, MALI_UK_GET_GP_NUMBER_OF_CORES, mali_uk_get_gp_number_of_cores_s)
#define MALI_IOC_GP2_CORE_VERSION_GET _IOR(MALI_IOC_GP_BASE, MALI_UK_GET_GP_CORE_VERSION, mali_uk_get_gp_core_version_s)
#define MALI_IOC_GP2_SUSPEND_RESPONSE _IOW(MALI_IOC_GP_BASE, MALI_UK_GP_SUSPEND_RESPONSE, mali_uk_gp_suspend_response_s)

#define MALI_IOC_PROFILING_ADD_EVENT                                                                                   \
    _IOWR(MALI_IOC_PROFILING_BASE, MALI_UK_PROFILING_ADD_EVENT, mali_uk_profiling_add_event_s)
#define MALI_IOC_PROFILING_REPORT_SW_COUNTERS                                                                          \
    _IOW(MALI_IOC_PROFILING_BASE, MALI_UK_PROFILING_REPORT_SW_COUNTERS, mali_uk_sw_counters_report_s)
#define MALI_IOC_PROFILING_MEMORY_USAGE_GET                                                                            \
    _IOR(MALI_IOC_PROFILING_BASE, MALI_UK_PROFILING_MEMORY_USAGE_GET, mali_uk_profiling_memory_usage_get_s)
#define MALI_IOC_PROFILING_STREAM_FD_GET                                                                               \
    _IOR(MALI_IOC_PROFILING_BASE, MALI_UK_PROFILING_STREAM_FD_GET, mali_uk_profiling_stream_fd_get_s)
#define MALI_IOC_PROILING_CONTROL_SET                                                                                  \
    _IOR(MALI_IOC_PROFILING_BASE, MALI_UK_PROFILING_CONTROL_SET, mali_uk_profiling_control_set_s)

#define MALI_IOC_VSYNC_EVENT_REPORT _IOW(MALI_IOC_VSYNC_BASE, MALI_UK_VSYNC_EVENT_REPORT, mali_uk_vsync_event_report_s)

#ifdef __cplusplus
}
#endif

#endif /* H_MALI_UTGARD_IOCTL_H */
