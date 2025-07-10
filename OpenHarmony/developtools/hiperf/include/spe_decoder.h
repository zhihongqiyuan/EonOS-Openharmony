/*
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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
#ifndef SPE_DECODER_H
#define SPE_DECODER_H

#include <linux/const.h>
#include <stddef.h>
#include <stdint.h>

#include "utilities.h"

#define BIT_ULL(x)    (_BITULL(x))
#define ULL(x)        (_ULL(x))
#define UL(x)         (_UL(x))
#define BIT(x)        (UL(1) << (x))
#define BITS_PER_LONG __BITS_PER_LONG
#define BITS_PER_LONG_LONG 64

#define GENMASK_INPUT_CHECK(h, l) 0

#define PR_GENMASK(h, l) \
        (((~UL(0)) - (UL(1) << (l)) + 1) & \
         (~UL(0) >> (BITS_PER_LONG - 1 - (h))))
#define GENMASK(h, l) \
        (GENMASK_INPUT_CHECK(h, l) + PR_GENMASK(h, l))

#define PR_GENMASK_ULL(h, l) \
        (((~ULL(0)) - (ULL(1) << (l)) + 1) & \
         (~ULL(0) >> (BITS_PER_LONG_LONG - 1 - (h))))
#define GENMASK_ULL(h, l) \
        (GENMASK_INPUT_CHECK(h, l) + PR_GENMASK_ULL(h, l))

#define PERF_SPE_PKT_DESC_MAX      256
#define PERF_SPE_NEED_MORE_BYTES   (-1)
#define PERF_SPE_BAD_PACKET        (-2)
#define PERF_SPE_PKT_MAX_SZ        16

namespace OHOS {
namespace Developtools {
namespace HiPerf {
enum SpePktType {
    PERF_SPE_BAD,
    PERF_SPE_PAD,
    PERF_SPE_END,
    PERF_SPE_TIMESTAMP,
    PERF_SPE_ADDRESS,
    PERF_SPE_COUNTER,
    PERF_SPE_CONTEXT,
    PERF_SPE_OP_TYPE,
    PERF_SPE_EVENTS,
    PERF_SPE_DATA_SOURCE,
};

struct SpePkt {
    SpePktType      type;
    unsigned char   index;
    uint64_t payload;
};

/* Short header (HEADER0) and extended header (HEADER1) */
#define PERF_SPE_HEADER0_PAD                 0x0
#define PERF_SPE_HEADER0_END                 0x1
#define PERF_SPE_HEADER0_TIMESTAMP           0x71
/* Mask for event & data source */
#define PERF_SPE_HEADER0_MASK1               (GENMASK_ULL(7, 6) | GENMASK_ULL(3, 0))
#define PERF_SPE_HEADER0_EVENTS              0x42
#define PERF_SPE_HEADER0_SOURCE              0x43
/* Mask for context & operation */
#define PERF_SPE_HEADER0_MASK2               GENMASK_ULL(7, 2)
#define PERF_SPE_HEADER0_CONTEXT             0x64
#define PERF_SPE_HEADER0_OP_TYPE             0x48
/* Mask for extended format */
#define PERF_SPE_HEADER0_EXTENDED            0x20
/* Mask for address & counter */
#define PERF_SPE_HEADER0_MASK3               GENMASK_ULL(7, 3)
#define PERF_SPE_HEADER0_ADDRESS             0xb0
#define PERF_SPE_HEADER0_COUNTER             0x98
#define PERF_SPE_HEADER1_ALIGNMENT           0x0

#define PERF_SPE_HDR_SHORT_INDEX(h)         ((h) & GENMASK_ULL(2, 0))
#define PERF_SPE_HDR_EXTENDED_INDEX(h0, h1)     (((h0) & GENMASK_ULL(1, 0)) << 3 | \
                                                 PERF_SPE_HDR_SHORT_INDEX(h1))

/* Address packet header */
#define PERF_SPE_ADDR_PKT_HDR_INDEX_INS         0x0
#define PERF_SPE_ADDR_PKT_HDR_INDEX_BRANCH      0x1
#define PERF_SPE_ADDR_PKT_HDR_INDEX_DATA_VIRT   0x2
#define PERF_SPE_ADDR_PKT_HDR_INDEX_DATA_PHYS   0x3
#define PERF_SPE_ADDR_PKT_HDR_INDEX_PREV_BRANCH 0x4

/* Address packet payload */
#define PERF_SPE_ADDR_PKT_ADDR_BYTE7_SHIFT      56
#define PERF_SPE_ADDR_PKT_ADDR_GET_BYTES_0_6(v) ((v) & GENMASK_ULL(55, 0))
#define PERF_SPE_ADDR_PKT_ADDR_GET_BYTE_6(v)        (((v) & GENMASK_ULL(55, 48)) >> 48)

#define PERF_SPE_ADDR_PKT_GET_NS(v)         (((v) & BIT_ULL(63)) >> 63)
#define PERF_SPE_ADDR_PKT_GET_EL(v)         (((v) & GENMASK_ULL(62, 61)) >> 61)
#define PERF_SPE_ADDR_PKT_GET_CH(v)         (((v) & BIT_ULL(62)) >> 62)
#define PERF_SPE_ADDR_PKT_GET_PAT(v)            (((v) & GENMASK_ULL(59, 56)) >> 56)

#define PERF_SPE_ADDR_PKT_EL0                  0
#define PERF_SPE_ADDR_PKT_EL1                  1
#define PERF_SPE_ADDR_PKT_EL2                  2
#define PERF_SPE_ADDR_PKT_EL3                  3

/* Context packet header */
#define PERF_SPE_CTX_PKT_HDR_INDEX(h)        ((h) & GENMASK_ULL(1, 0))

/* Counter packet header */
#define PERF_SPE_CNT_PKT_HDR_INDEX_TOTAL_LAT     0x0
#define PERF_SPE_CNT_PKT_HDR_INDEX_ISSUE_LAT     0x1
#define PERF_SPE_CNT_PKT_HDR_INDEX_TRANS_LAT     0x2

/* Get bit 5th and 4th of the header */
#define PERF_SPE_HDR_GET_BYTES_5_4(h)       (((h) & (GENMASK_ULL(5, 4))) >> 4)

/* Event packet payload */
enum SpeEvents {
    EVENT_EXCEPTION_GEN        = 0,
    EVENT_RETIRED              = 1,
    EVENT_L1D_ACCESS           = 2,
    EVENT_L1D_REFILL           = 3,
    EVENT_TLB_ACCESS           = 4,
    EVENT_TLB_WALK             = 5,
    EVENT_NOT_TAKEN            = 6,
    EVENT_MISPRED              = 7,
    EVENT_LLC_ACCESS           = 8,
    EVENT_LLC_MISS             = 9,
    EVENT_REMOTE_ACCESS        = 10,
    EVENT_ALIGNMENT            = 11,
    EVENT_PARTIAL_PREDICATE    = 17,
    EVENT_EMPTY_PREDICATE      = 18,
};


/* Operation packet header */
#define PERF_SPE_OP_PKT_HDR_CLASS(h)             ((h) & GENMASK_ULL(1, 0))
#define PERF_SPE_OP_PKT_HDR_CLASS_OTHER          0x0
#define PERF_SPE_OP_PKT_HDR_CLASS_LD_ST_ATOMIC   0x1
#define PERF_SPE_OP_PKT_HDR_CLASS_BR_ERET        0x2

#define PERF_SPE_OP_PKT_IS_OTHER_SVE_OP(v)       (((v) & (BIT(7) | BIT(3) | BIT(0))) == 0x8)

#define PERF_SPE_OP_PKT_COND                     BIT(0)

#define PERF_SPE_OP_PKT_LDST_SUBCLASS_GET(v)     ((v) & GENMASK_ULL(7, 1))
#define PERF_SPE_OP_PKT_LDST_SUBCLASS_GP_REG     0x0
#define PERF_SPE_OP_PKT_LDST_SUBCLASS_SIMD_FP    0x4
#define PERF_SPE_OP_PKT_LDST_SUBCLASS_UNSPEC_REG 0x10
#define PERF_SPE_OP_PKT_LDST_SUBCLASS_NV_SYSREG  0x30
#define PERF_SPE_OP_PKT_LDST_SUBCLASS_MTE_TAG    0x14
#define PERF_SPE_OP_PKT_LDST_SUBCLASS_MEMCPY     0x20
#define PERF_SPE_OP_PKT_LDST_SUBCLASS_MEMSET     0x25

#define PERF_SPE_OP_PKT_IS_LDST_ATOMIC(v)        (((v) & (GENMASK_ULL(7, 5) | BIT(1))) == 0x2)

#define PERF_SPE_OP_PKT_AR                       BIT(4)
#define PERF_SPE_OP_PKT_EXCL                     BIT(3)
#define PERF_SPE_OP_PKT_AT                       BIT(2)
#define PERF_SPE_OP_PKT_ST                       BIT(0)

#define PERF_SPE_OP_PKT_IS_LDST_SVE(v)           (((v) & (BIT(3) | BIT(1))) == 0x8)

#define PERF_SPE_OP_PKT_SVE_SG                   BIT(7)
/*
 * SVE effective vector length (EVL) is stored in byte 0 bits [6:4];
 * the length is rounded up to a power of two and use 32 as one step,
 * so EVL calculation is:
 *
 *   32 * (2 ^ bits [6:4]) = 32 << (bits [6:4])
 */
#define PERF_SPE_OP_PKG_SVE_EVL(v)              (32 << (((v) & GENMASK_ULL(6, 4)) >> 4))
#define PERF_SPE_OP_PKT_SVE_PRED                BIT(2)
#define PERF_SPE_OP_PKT_SVE_FP                  BIT(1)

#define PERF_SPE_OP_PKT_IS_INDIRECT_BRANCH(v)   (((v) & GENMASK_ULL(7, 1)) == 0x2)

const int LEN_TYPE_BYTE    = 1;
const int LEN_TYPE_HLFWRD  = 2;
const int LEN_TYPE_WORD    = 4;
const int LEN_TYPE_DBLEWRD = 8;
/*16 bytes of width*/
const int BYTE_WIDTH = 16;

enum SpeSampleType {
    PERF_SPE_L1D_ACCESS          = 1 << 0,
    PERF_SPE_L1D_MISS            = 1 << 1,
    PERF_SPE_LLC_ACCESS          = 1 << 2,
    PERF_SPE_LLC_MISS            = 1 << 3,
    PERF_SPE_TLB_ACCESS          = 1 << 4,
    PERF_SPE_TLB_MISS            = 1 << 5,
    PERF_SPE_BRANCH_MISS         = 1 << 6,
    PERF_SPE_REMOTE_ACCESS       = 1 << 7,
    PERF_SPE_SVE_PARTIAL_PRED    = 1 << 8,
    PERF_SPE_SVE_EMPTY_PRED      = 1 << 9,
};

enum SpeOpType {
    /* First level operation type */
    PERF_SPE_OP_OTHER            = 1 << 0,
    PERF_SPE_OP_LDST             = 1 << 1,
    PERF_SPE_OP_BRANCH_ERET      = 1 << 2,

    /* Second level operation type for OTHER */
    PERF_SPE_OP_SVE_OTHER        = 1 << 16,
    PERF_SPE_OP_SVE_FP           = 1 << 17,
    PERF_SPE_OP_SVE_PRED_OTHER   = 1 << 18,

    /* Second level operation type for LDST */
    PERF_SPE_OP_LD               = 1 << 16,
    PERF_SPE_OP_ST               = 1 << 17,
    PERF_SPE_OP_ATOMIC           = 1 << 18,
    PERF_SPE_OP_EXCL             = 1 << 19,
    PERF_SPE_OP_AR               = 1 << 20,
    PERF_SPE_OP_SIMD_FP          = 1 << 21,
    PERF_SPE_OP_GP_REG           = 1 << 22,
    PERF_SPE_OP_UNSPEC_REG       = 1 << 23,
    PERF_SPE_OP_NV_SYSREG        = 1 << 24,
    PERF_SPE_OP_SVE_LDST         = 1 << 25,
    PERF_SPE_OP_SVE_PRED_LDST    = 1 << 26,
    PERF_SPE_OP_SVE_SG           = 1 << 27,

    /* Second level operation type for BRANCH_ERET */
    PERF_SPE_OP_BR_COND          = 1 << 16,
    PERF_SPE_OP_BR_INDIRECT      = 1 << 17,
};

enum SpeNeoverseDataSource {
    PERF_SPE_NV_L1D           = 0x0,
    PERF_SPE_NV_L2            = 0x8,
    PERF_SPE_NV_PEER_CORE     = 0x9,
    PERF_SPE_NV_LOCAL_CLUSTER = 0xa,
    PERF_SPE_NV_SYS_CACHE     = 0xb,
    PERF_SPE_NV_PEER_CLUSTER  = 0xc,
    PERF_SPE_NV_REMOTE        = 0xd,
    PERF_SPE_NV_DRAM          = 0xe,
};

struct SpeRecord {
    u32 type;
    int err;
    u32 op;
    u32 latency;
    u64 from_ip;
    u64 to_ip;
    u64 timestamp;
    u64 virt_addr;
    u64 phys_addr;
    u64 context_id;
    u16 source;
};

struct SpeInsn;

struct SpeBuffer {
    const unsigned char *buf;
    size_t len;
    u64 offset;
    u64 trace_nr;
};

struct SpeParams {
    void *data;
};

struct SpeDecoder {
    void *data;
    struct SpeRecord record;
    const unsigned char *buf;
    size_t len;

    struct SpePkt packet;
};

struct SpeDecoder *SpeDecoderDataNew(const unsigned char *speBuf, size_t speLen);
void SpeDecoderFree(struct SpeDecoder *decoder);

int SpeDecode(struct SpeDecoder *decoder);

int SpePktDesc(const struct SpePkt *packet, char *buf, size_t len);
bool SpeDumpRawData(unsigned char *buf, size_t len, int indent, FILE *outputDump);

struct ReportItemAuxRawData {
    u32 type;
    float heating;
    u64 count;
    std::string comm;
    u64 pc;
    std::string SharedObject;
    std::string Symbol;
    u64 offset;
};

void AddReportItems(const std::vector<ReportItemAuxRawData>& auxRawData);
void UpdateHeating();
void DumpSpeReportData(int indent, FILE *outputDump);
void DumpSpeReportHead(int indent, uint32_t type, uint64_t count);
void GetSpeEventNameByType(uint32_t type, std::string& eventName);
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // SPE_DECODER_H
