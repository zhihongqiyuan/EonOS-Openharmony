/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
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

/* This line is generated to truncate the original file */

#ifndef FSTRACE_TYPES_H
#define FSTRACE_TYPES_H

#include "hiebpf_macros.h"

struct sys_openat2_args_t {
    int dfd;
    const char __user* filename;
    struct open_how* how;
};

struct sys_read_args_t {
    unsigned int fd;
    char __user* buf;
    size_t count;
};

struct sys_write_args_t {
    unsigned int fd;
    const char __user* buf;
    size_t count;
};

struct sys_pread64_args_t {
    unsigned int fd;
    char __user* buf;
    size_t count;
    loff_t pos;
};

struct sys_pwrite64_args_t {
    unsigned int fd;
    const char __user* buf;
    size_t count;
    loff_t pos;
};

struct sys_readv_args_t {
    unsigned long fd;
    const struct iovec __user* vec;
    unsigned long vlen;
    rwf_t flags;
};

struct sys_writev_args_t {
    unsigned long fd;
    const struct iovec __user* vec;
    unsigned long vlen;
    rwf_t flags;
};

struct sys_preadv_args_t {
    unsigned long fd;
    const struct iovec __user* vec;
    unsigned long vlen;
    loff_t pos;
    rwf_t flags;
};

struct sys_pwritev_args_t {
    unsigned long fd;
    const struct iovec __user* vec;
    unsigned long vlen;
    loff_t pos;
    rwf_t flags;
};

struct sys_close_args_t {
    struct files_struct* files;
    unsigned fd;
};

struct fstrace_start_event_t {
    __u32 type;
    __u64 stime;
    union {
        struct sys_openat2_args_t openat2_args;
        struct sys_read_args_t read_args;
        struct sys_write_args_t write_args;
        struct sys_pread64_args_t pread64_args;
        struct sys_pwrite64_args_t pwrite64_args;
        struct sys_readv_args_t readv_args;
        struct sys_writev_args_t writev_args;
        struct sys_preadv_args_t preadv_args;
        struct sys_pwritev_args_t pwritev_args;
        struct sys_close_args_t close_args;
    };
};

enum FSTraceEventType:__u32 {
    SYS_OPENAT2 = 1,
    SYS_READ,
    SYS_WRITE,
    SYS_PREAD64,
    SYS_PWRITE64,
    SYS_READV,
    SYS_WRITEV,
    SYS_PREADV,
    SYS_PWRITEV,
    SYS_CLOSE,
};
#endif