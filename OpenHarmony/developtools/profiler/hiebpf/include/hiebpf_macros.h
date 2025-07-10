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

#ifndef HIEBPF_MACROS_H
#define HIEBPF_MACROS_H

#ifndef __user
#define __user // user space address
#endif

/*
 * The following constants are used in both user and kernel space program,
 * since global variables are unavailable at present, these constants must
 * be defined as macros
*/
#define MAX_STACK_LIMIT       64
#define MAX_STACK_DEPTH       64

#ifndef MAX_COMM_LEN
#define MAX_COMM_LEN                16
#endif

#define MAX_START_EVENTS_NUM        256
#define MAX_TARGET_PIDS             32
#define MAX_TARGET_EVENTS           256
#define MAX_FILENAME_LEN            512
#define MAX_STACK_TRACE_ENTRIES     10000

#define KERN_STACKID_FLAGS          (0 | BPF_F_FAST_STACK_CMP)
#define USER_STACKID_FLAGS          (0 | BPF_F_FAST_STACK_CMP | BPF_F_USER_STACK | BPF_F_REUSE_STACKID)

// BPF_RINGBUF_SIZE = 16M
#define BPF_RINGBUF_SIZE            (1 << 24)
#define MAX_DENTRY_NAME_LEN         16

#define TRACER_PID_INDEX            0
#define BPF_LOG_LEVEL_INDEX         1
#define MAX_STACK_LIMIT_INDEX       2
#define MAX_STACK_DEPTH_INDEX       3
#define UNWIND_FLAG_INDEX           4
#define NR_CONFIG_VARIABLES         5

#define NUM_STACK_TRACE_MAPS        1
#define FSTRACE_STACK_TRACE_INDEX   0
#define PFTRACE_STACK_TRACE_INDEX   0
#define BIOTRACE_STACK_TRACE_INDEX  0

#ifndef PAGE_SHIFT
#define PAGE_SHIFT                    12
#endif

#define FILE_TYPE_BITS              20
#define DCACHE_REGULAR_TYPE         (0x00400000 >> FILE_TYPE_BITS) /* Regular file type (or fallthru to such) */
#define DCACHE_DIRECTORY_TYPE       (0x00200000 >> FILE_TYPE_BITS) /* Normal directory */

// the following definitions have to be stay consistent with their
// counterparts in linux 5.10.0

/*
 * Operations and flags common to the bio and request structures.
 * We use 8 bits for encoding the operation, and the remaining 24 for flags.
 *
 * The least significant bit of the operation number indicates the data
 * transfer direction:
 *
 *   - if the least significant bit is set transfers are TO the device
 *   - if the least significant bit is not set transfers are FROM the device
 *
 * If a operation does not transfer data the least significant bit has no
 * meaning.
 */
#define REQ_OP_BITS    8
#define REQ_OP_MASK    ((1 << REQ_OP_BITS) - 1)
#define REQ_FLAG_BITS    24

#define REQ_FAILFAST_DEV    (1ULL << __REQ_FAILFAST_DEV)
#define REQ_FAILFAST_TRANSPORT    (1ULL << __REQ_FAILFAST_TRANSPORT)
#define REQ_FAILFAST_DRIVER    (1ULL << __REQ_FAILFAST_DRIVER)
#define REQ_SYNC        (1ULL << __REQ_SYNC)
#define REQ_META        (1ULL << __REQ_META)
#define REQ_PRIO        (1ULL << __REQ_PRIO)
#define REQ_NOMERGE        (1ULL << __REQ_NOMERGE)
#define REQ_IDLE        (1ULL << __REQ_IDLE)
#define REQ_INTEGRITY        (1ULL << __REQ_INTEGRITY)
#define REQ_FUA            (1ULL << __REQ_FUA)
#define REQ_PREFLUSH        (1ULL << __REQ_PREFLUSH)
#define REQ_RAHEAD        (1ULL << __REQ_RAHEAD)
#define REQ_BACKGROUND        (1ULL << __REQ_BACKGROUND)
#define REQ_NOWAIT        (1ULL << __REQ_NOWAIT)
#define REQ_CGROUP_PUNT        (1ULL << __REQ_CGROUP_PUNT)

#define REQ_NOUNMAP        (1ULL << __REQ_NOUNMAP)
#define REQ_HIPRI        (1ULL << __REQ_HIPRI)

#define REQ_DRV            (1ULL << __REQ_DRV)
#define REQ_SWAP        (1ULL << __REQ_SWAP)

#define REQ_FAILFAST_MASK \
    (REQ_FAILFAST_DEV | REQ_FAILFAST_TRANSPORT | REQ_FAILFAST_DRIVER)

#define REQ_NOMERGE_FLAGS \
    (REQ_NOMERGE | REQ_PREFLUSH | REQ_FUA)

/*
 * Helper macro to manipulate data structures
 */
#ifndef offsetof
#define offsetof(TYPE, MEMBER)    ((unsigned long)&((TYPE *)0)->MEMBER)
#endif
#ifndef container_of
#define container_of(ptr, type, member)                    \
    ({                                                    \
        void *__mptr = (void *)(ptr);                    \
        ((type *)(__mptr - offsetof(type, member)));    \
    })
#endif

/**
 * list_for_each    -    iterate over a list
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:    the head for your list.
 */
#ifndef list_for_each
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)
#endif

/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_head within the struct.
 */
#ifndef list_entry
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)
#endif


#define hlist_entry(ptr, type, member) container_of(ptr, type, member)

#define hlist_for_each(pos, head) \
    for (pos = (head)->first; pos ; pos = pos->next)

#endif