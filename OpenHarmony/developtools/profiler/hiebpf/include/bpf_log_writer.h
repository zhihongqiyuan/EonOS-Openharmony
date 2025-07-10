/*
* Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved.

* The bpf_log_writer.h is dual licensed: you can use it either under the terms of
* the GPL V2, or the 3-Clause BSD license, at your option.
* See the LICENSE file in the root of this repository for complete details.
*/
#ifndef BPF_LOG_WRITER_H
#define BPF_LOG_WRITER_H

#include "bpf_log.h"
#include "fstrace_types.h"

#define BPFLOG(level, expression, format, ...) {                                          \
    u32 bpf_log_level_index = BPF_LOG_LEVEL_INDEX;                                          \
    u32* bpf_log_level_ptr = bpf_map_lookup_elem(&config_var_map, &bpf_log_level_index);    \
    if (bpf_log_level_ptr) {                                                                \
        u32 bpf_log_level = BPF_LOG_NONE;                                                   \
        bpf_probe_read_kernel(&bpf_log_level, sizeof(u32), bpf_log_level_ptr);       \
        if ((expression) && (bpf_log_level <= level)) {                                     \
            char mesg[] = format;                                                           \
            bpf_trace_printk(mesg, sizeof(mesg), ##__VA_ARGS__);                            \
        }                                                                                   \
    } else {                                                                                \
        char mesg[] = "failed to get bpf log level";                                        \
        bpf_trace_printk(mesg, sizeof(mesg));                                               \
    }                                                                                       \
}

#ifdef BPF_LOGGER_DEBUG
#define BPFLOGD(expression, format, ...) BPFLOG(BPF_LOG_DEBUG, expression, format, ##__VA_ARGS__)
#define BPFLOGI(expression, format, ...) BPFLOG(BPF_LOG_INFO, expression, format, ##__VA_ARGS__)
#define BPFLOGW(expression, format, ...) BPFLOG(BPF_LOG_WARN, expression, format, ##__VA_ARGS__)
#define BPFLOGE(expression, format, ...) BPFLOG(BPF_LOG_ERROR, expression, format, ##__VA_ARGS__)
#define BPFLOGF(expression, format, ...) BPFLOG(BPF_LOG_FATAL, expression, format, ##__VA_ARGS__)

#elif defined(BPF_LOGGER_INFO)
#define BPFLOGD(expression, format, ...) {}
#define BPFLOGI(expression, format, ...) BPFLOG(BPF_LOG_INFO, expression, format, ##__VA_ARGS__)
#define BPFLOGW(expression, format, ...) BPFLOG(BPF_LOG_WARN, expression, format, ##__VA_ARGS__)
#define BPFLOGE(expression, format, ...) BPFLOG(BPF_LOG_ERROR, expression, format, ##__VA_ARGS__)
#define BPFLOGF(expression, format, ...) BPFLOG(BPF_LOG_FATAL, expression, format, ##__VA_ARGS__)

#elif defined(BPF_LOGGER_WARN)
#define BPFLOGD(expression, format, ...) {}
#define BPFLOGI(expression, format, ...) {}
#define BPFLOGW(expression, format, ...) BPFLOG(BPF_LOG_WARN, expression, format, ##__VA_ARGS__)
#define BPFLOGE(expression, format, ...) BPFLOG(BPF_LOG_ERROR, expression, format, ##__VA_ARGS__)
#define BPFLOGF(expression, format, ...) BPFLOG(BPF_LOG_FATAL, expression, format, ##__VA_ARGS__)

#elif defined(BPF_LOGGER_ERROR)
#define BPFLOGD(expression, format, ...) {}
#define BPFLOGI(expression, format, ...) {}
#define BPFLOGW(expression, format, ...) {}
#define BPFLOGE(expression, format, ...) BPFLOG(BPF_LOG_ERROR, expression, format, ##__VA_ARGS__)
#define BPFLOGF(expression, format, ...) BPFLOG(BPF_LOG_FATAL, expression, format, ##__VA_ARGS__)

#elif defined(BPF_LOGGER_FATAL)
#define BPFLOGD(expression, format, ...) {}
#define BPFLOGI(expression, format, ...) {}
#define BPFLOGW(expression, format, ...) {}
#define BPFLOGE(expression, format, ...) {}
#define BPFLOGF(expression, format, ...) BPFLOG(BPF_LOG_FATAL, expression, format, ##__VA_ARGS__)

#else
#define BPFLOGD(expression, format, ...) {}
#define BPFLOGI(expression, format, ...) {}
#define BPFLOGW(expression, format, ...) {}
#define BPFLOGE(expression, format, ...) {}
#define BPFLOGF(expression, format, ...) {}

#endif // BPF_LOGGER_LEVEL

#endif  // BPF_LOG_WRITER_H