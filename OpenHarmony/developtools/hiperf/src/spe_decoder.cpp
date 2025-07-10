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

#include "spe_decoder.h"
#include "hiperf_hilog.h"

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define LE16_TO_CPU bswap_16
#define LE32_TO_CPU bswap_32
#define LE64_TO_CPU bswap_64
#else
#define LE16_TO_CPU
#define LE32_TO_CPU
#define LE64_TO_CPU
#endif

namespace OHOS {
namespace Developtools {
namespace HiPerf {
constexpr const int UN_PRMT = -1;

const std::string SpePktName(enum SpePktType type)
{
    std::string spePacketName = "";
    switch (type) {
        case PERF_SPE_PAD:         spePacketName = "PAD"; break;
        case PERF_SPE_END:         spePacketName = "END"; break;
        case PERF_SPE_TIMESTAMP:   spePacketName = "TS"; break;
        case PERF_SPE_ADDRESS:     spePacketName = "ADDR"; break;
        case PERF_SPE_COUNTER:     spePacketName = "LAT"; break;
        case PERF_SPE_CONTEXT:     spePacketName = "CONTEXT"; break;
        case PERF_SPE_OP_TYPE:     spePacketName = "OP-TYPE"; break;
        case PERF_SPE_EVENTS:      spePacketName = "EVENTS"; break;
        case PERF_SPE_DATA_SOURCE: spePacketName = "DATA-SOURCE"; break;
        default:                   spePacketName = "INVALID"; break;
    }
    return spePacketName;
}

static unsigned int SpePayloadLen(unsigned char hdr)
{
    return 1U << PERF_SPE_HDR_GET_BYTES_5_4(hdr);
}

static int SpeGetPayload(const unsigned char *buf, size_t len,
                         unsigned char extHdr, struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    size_t payloadLen = SpePayloadLen(buf[extHdr]);
    if (len < 1 + extHdr + payloadLen) {
        return PERF_SPE_NEED_MORE_BYTES;
    }
    buf += 1 + extHdr;

    switch (payloadLen) {
        case LEN_TYPE_BYTE: packet->payload = *(reinterpret_cast<const uint8_t *>(buf)); break;
        case LEN_TYPE_HLFWRD: packet->payload = LE16_TO_CPU(*reinterpret_cast<const uint16_t *>(buf)); break;
        case LEN_TYPE_WORD: packet->payload = LE32_TO_CPU(*reinterpret_cast<const uint32_t *>(buf)); break;
        case LEN_TYPE_DBLEWRD: packet->payload = LE64_TO_CPU(*reinterpret_cast<const uint64_t *>(buf)); break;
        default: return PERF_SPE_BAD_PACKET;
    }

    return 1 + extHdr + payloadLen;
}

static int SpeGetPad(struct SpePkt *packet)
{
    CHECK_TRUE(packet == nullptr, -1, 1, "Invalid pointer!");
    packet->type = PERF_SPE_PAD;
    return 1;
}

static int SpeGetAlignment(const unsigned char *buf, size_t len,
                           struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    unsigned int alignment = 1 << ((buf[0] & 0xf) + 1);

    if (len < alignment)
        return PERF_SPE_NEED_MORE_BYTES;

    packet->type = PERF_SPE_PAD;
    return alignment - (((uintptr_t)buf) & (alignment - 1));
}

static int SpeGetEnd(struct SpePkt *packet)
{
    CHECK_TRUE(packet == nullptr, -1, 1, "Invalid pointer!");
    packet->type = PERF_SPE_END;
    return 1;
}

static int SpeGetTimestamp(const unsigned char *buf, size_t len,
                           struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    packet->type = PERF_SPE_TIMESTAMP;
    return SpeGetPayload(buf, len, 0, packet);
}

static int SpeGetEvents(const unsigned char *buf, size_t len,
                        struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    packet->type = PERF_SPE_EVENTS;
    packet->index = SpePayloadLen(buf[0]);
    return SpeGetPayload(buf, len, 0, packet);
}

static int SpeGetDataSource(const unsigned char *buf, size_t len,
                            struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    packet->type = PERF_SPE_DATA_SOURCE;
    return SpeGetPayload(buf, len, 0, packet);
}

static int SpeGetContext(const unsigned char *buf, size_t len,
                         struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    packet->type = PERF_SPE_CONTEXT;
    packet->index = PERF_SPE_CTX_PKT_HDR_INDEX(buf[0]);
    return SpeGetPayload(buf, len, 0, packet);
}

static int SpeGetOpType(const unsigned char *buf, size_t len,
                        struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    packet->type = PERF_SPE_OP_TYPE;
    packet->index = PERF_SPE_OP_PKT_HDR_CLASS(buf[0]);
    return SpeGetPayload(buf, len, 0, packet);
}

static int SpeGetCounter(const unsigned char *buf, size_t len,
                         const unsigned char extHdr, struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    packet->type = PERF_SPE_COUNTER;
    if (extHdr) {
        packet->index = PERF_SPE_HDR_EXTENDED_INDEX(buf[0], buf[1]);
    } else {
        packet->index = PERF_SPE_HDR_SHORT_INDEX(buf[0]);
    }

    return SpeGetPayload(buf, len, extHdr, packet);
}

static int SpeGetAddr(const unsigned char *buf, size_t len,
                      const unsigned char extHdr, struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    packet->type = PERF_SPE_ADDRESS;
    if (extHdr) {
        packet->index = PERF_SPE_HDR_EXTENDED_INDEX(buf[0], buf[1]);
    } else {
        packet->index = PERF_SPE_HDR_SHORT_INDEX(buf[0]);
    }

    return SpeGetPayload(buf, len, extHdr, packet);
}

static int SpeDoGetPacket(const unsigned char *buf, size_t len,
                          struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    unsigned int hdr;
    unsigned char extHdr = 0;

    if (memset_s(packet, sizeof(struct SpePkt), 0, sizeof(struct SpePkt)) != EOK) {
        HLOGE("memset_s failed in SpeDoGetPacket");
        return -1;
    }

    if (!len) {
        return PERF_SPE_NEED_MORE_BYTES;
    }

    hdr = buf[0];
    if (hdr == PERF_SPE_HEADER0_PAD) {
        return SpeGetPad(packet);
    } else if (hdr == PERF_SPE_HEADER0_END) {      /* no timestamp at end of record */
        return SpeGetEnd(packet);
    }
    if (hdr == PERF_SPE_HEADER0_TIMESTAMP) {
        return SpeGetTimestamp(buf, len, packet);
    } else if ((hdr & PERF_SPE_HEADER0_MASK1) == PERF_SPE_HEADER0_EVENTS) {
        return SpeGetEvents(buf, len, packet);
    } else if ((hdr & PERF_SPE_HEADER0_MASK1) == PERF_SPE_HEADER0_SOURCE) {
        return SpeGetDataSource(buf, len, packet);
    } else if ((hdr & PERF_SPE_HEADER0_MASK2) == PERF_SPE_HEADER0_CONTEXT) {
        return SpeGetContext(buf, len, packet);
    } else if ((hdr & PERF_SPE_HEADER0_MASK2) == PERF_SPE_HEADER0_OP_TYPE) {
        return SpeGetOpType(buf, len, packet);
    }
    if ((hdr & PERF_SPE_HEADER0_MASK2) == PERF_SPE_HEADER0_EXTENDED) {
        /* 16-bit extended format header */
        if (len == 1) {
            return PERF_SPE_BAD_PACKET;
        }

        extHdr = 1;
        hdr = buf[1];
        if (hdr == PERF_SPE_HEADER1_ALIGNMENT) {
            return SpeGetAlignment(buf, len, packet);
        }
    }

    /*
     * The short format header's byte 0 or the extended format header's
     * byte 1 has been assigned to 'hdr', which uses the same encoding for
     * address packet and counter packet, so don't need to distinguish if
     * it's short format or extended format and handle in once.
     */
    if ((hdr & PERF_SPE_HEADER0_MASK3) == PERF_SPE_HEADER0_ADDRESS) {
        return SpeGetAddr(buf, len, extHdr, packet);
    }

    if ((hdr & PERF_SPE_HEADER0_MASK3) == PERF_SPE_HEADER0_COUNTER) {
        return SpeGetCounter(buf, len, extHdr, packet);
    }
    return PERF_SPE_BAD_PACKET;
}

int SpeGetPacket(const unsigned char *buf, size_t len,
                 struct SpePkt *packet)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    int ret = SpeDoGetPacket(buf, len, packet);
    /* put multiple consecutive PADs on the same line, up to
     * the fixed-width output format of 16 bytes per line.
     */
    if (ret > 0 && packet->type == PERF_SPE_PAD) {
        while (ret < BYTE_WIDTH && len > (size_t)ret && !buf[ret]) {
            ret += 1;
        }
    }
    return ret;
}

static int SpePktOutString(int *err, char **bufPtr, size_t *bufLen,
                           const char *fmt, ...)
{
    CHECK_TRUE(*bufPtr == nullptr || bufLen == nullptr || fmt == nullptr, -1, 1, "Invalid pointer!");
    va_list args;
    int ret = 0;

    /* If any errors occur, exit */
    if (err && *err) {
        return *err;
    }
    if (*bufLen - 1 < 0) {
        HLOGW("SpePktOutString failed, bufLen: %d", static_cast<int>(*bufLen));
    }
    va_start(args, fmt);
    ret = vsnprintf_s(*bufPtr, *bufLen, *bufLen - 1, fmt, args);
    va_end(args);

    if (ret < 0) {
        if (err && !*err) {
            *err = ret;
        }
        HLOGW("vsnprintf_s failed: %d\n", ret);

    /*
     * If the return value is *bufLen or greater, the output was
     * truncated and the buffer overflowed.
     */
    } else if ((size_t)ret >= *bufLen) {
        (*bufPtr)[*bufLen - 1] = '\0';

        /*
         * Set *err to 'ret' to avoid overflow if tries to
         * fill this buffer sequentially.
         */
        if (err && !*err) {
            *err = ret;
        }
    } else {
        *bufPtr += ret;
        *bufLen -= ret;
    }

    return ret;
}

static int SpePktDescEvent(const struct SpePkt *packet,
                           char *buf, size_t bufLen)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    u64 payload = packet->payload;
    int err = 0;

    SpePktOutString(&err, &buf, &bufLen, "EV");
    if (payload & BIT(EVENT_EXCEPTION_GEN)) {
        SpePktOutString(&err, &buf, &bufLen, " EXCEPTION-GEN");
    }
    if (payload & BIT(EVENT_RETIRED)) {
        SpePktOutString(&err, &buf, &bufLen, " RETIRED");
    }
    if (payload & BIT(EVENT_L1D_ACCESS)) {
        SpePktOutString(&err, &buf, &bufLen, " L1D-ACCESS");
    }
    if (payload & BIT(EVENT_L1D_REFILL)) {
        SpePktOutString(&err, &buf, &bufLen, " L1D-REFILL");
    }
    if (payload & BIT(EVENT_TLB_ACCESS)) {
        SpePktOutString(&err, &buf, &bufLen, " TLB-ACCESS");
    }
    if (payload & BIT(EVENT_TLB_WALK)) {
        SpePktOutString(&err, &buf, &bufLen, " TLB-REFILL");
    }
    if (payload & BIT(EVENT_NOT_TAKEN)) {
        SpePktOutString(&err, &buf, &bufLen, " NOT-TAKEN");
    }
    if (payload & BIT(EVENT_MISPRED)) {
        SpePktOutString(&err, &buf, &bufLen, " MISPRED");
    }
    if (payload & BIT(EVENT_LLC_ACCESS)) {
        SpePktOutString(&err, &buf, &bufLen, " LLC-ACCESS");
    }
    if (payload & BIT(EVENT_LLC_MISS)) {
        SpePktOutString(&err, &buf, &bufLen, " LLC-REFILL");
    }
    if (payload & BIT(EVENT_REMOTE_ACCESS)) {
        SpePktOutString(&err, &buf, &bufLen, " REMOTE-ACCESS");
    }
    if (payload & BIT(EVENT_ALIGNMENT)) {
        SpePktOutString(&err, &buf, &bufLen, " ALIGNMENT");
    }
    if (payload & BIT(EVENT_PARTIAL_PREDICATE)) {
        SpePktOutString(&err, &buf, &bufLen, " SVE-PARTIAL-PRED");
    }
    if (payload & BIT(EVENT_EMPTY_PREDICATE)) {
        SpePktOutString(&err, &buf, &bufLen, " SVE-EMPTY-PRED");
    }

    return err;
}

static int SpePktDescOpType(const struct SpePkt *packet,
                            char *buf, size_t bufLen)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    u64 payload = packet->payload;
    int err = 0;

    switch (packet->index) {
        case PERF_SPE_OP_PKT_HDR_CLASS_OTHER:
            if (PERF_SPE_OP_PKT_IS_OTHER_SVE_OP(payload)) {
                SpePktOutString(&err, &buf, &bufLen, "SVE-OTHER");

                /* SVE effective vector length */
                SpePktOutString(&err, &buf, &bufLen, " EVLEN %d",
                                PERF_SPE_OP_PKG_SVE_EVL(payload));

                if (payload & PERF_SPE_OP_PKT_SVE_FP)
                    SpePktOutString(&err, &buf, &bufLen, " FP");
                if (payload & PERF_SPE_OP_PKT_SVE_PRED)
                    SpePktOutString(&err, &buf, &bufLen, " PRED");
            } else {
                SpePktOutString(&err, &buf, &bufLen, "OTHER");
                SpePktOutString(&err, &buf, &bufLen, " %s",
                                payload & PERF_SPE_OP_PKT_COND ?
                                "COND-SELECT" : "INSN-OTHER");
            }
            break;
        case PERF_SPE_OP_PKT_HDR_CLASS_LD_ST_ATOMIC:
            SpePktOutString(&err, &buf, &bufLen,
                            payload & 0x1 ? "ST" : "LD");

            if (PERF_SPE_OP_PKT_IS_LDST_ATOMIC(payload)) {
                if (payload & PERF_SPE_OP_PKT_AT)
                    SpePktOutString(&err, &buf, &bufLen, " AT");
                if (payload & PERF_SPE_OP_PKT_EXCL)
                    SpePktOutString(&err, &buf, &bufLen, " EXCL");
                if (payload & PERF_SPE_OP_PKT_AR)
                    SpePktOutString(&err, &buf, &bufLen, " AR");
            }

            switch (PERF_SPE_OP_PKT_LDST_SUBCLASS_GET(payload)) {
                case PERF_SPE_OP_PKT_LDST_SUBCLASS_SIMD_FP:
                    SpePktOutString(&err, &buf, &bufLen, " SIMD-FP");
                    break;
                case PERF_SPE_OP_PKT_LDST_SUBCLASS_GP_REG:
                    SpePktOutString(&err, &buf, &bufLen, " GP-REG");
                    break;
                case PERF_SPE_OP_PKT_LDST_SUBCLASS_UNSPEC_REG:
                    SpePktOutString(&err, &buf, &bufLen, " UNSPEC-REG");
                    break;
                case PERF_SPE_OP_PKT_LDST_SUBCLASS_NV_SYSREG:
                    SpePktOutString(&err, &buf, &bufLen, " NV-SYSREG");
                    break;
                case PERF_SPE_OP_PKT_LDST_SUBCLASS_MTE_TAG:
                    SpePktOutString(&err, &buf, &bufLen, " MTE-TAG");
                    break;
                case PERF_SPE_OP_PKT_LDST_SUBCLASS_MEMCPY:
                    SpePktOutString(&err, &buf, &bufLen, " MEMCPY");
                    break;
                case PERF_SPE_OP_PKT_LDST_SUBCLASS_MEMSET:
                    SpePktOutString(&err, &buf, &bufLen, " MEMSET");
                    break;
                default:
                    break;
            }

            if (PERF_SPE_OP_PKT_IS_LDST_SVE(payload)) {
                /* SVE effective vector length */
                SpePktOutString(&err, &buf, &bufLen, " EVLEN %d",
                                PERF_SPE_OP_PKG_SVE_EVL(payload));

                if (payload & PERF_SPE_OP_PKT_SVE_PRED)
                    SpePktOutString(&err, &buf, &bufLen, " PRED");
                if (payload & PERF_SPE_OP_PKT_SVE_SG)
                    SpePktOutString(&err, &buf, &bufLen, " SG");
            }
            break;
        case PERF_SPE_OP_PKT_HDR_CLASS_BR_ERET:
            SpePktOutString(&err, &buf, &bufLen, "B");

            if (payload & PERF_SPE_OP_PKT_COND)
                SpePktOutString(&err, &buf, &bufLen, " COND");

            if (PERF_SPE_OP_PKT_IS_INDIRECT_BRANCH(payload))
                SpePktOutString(&err, &buf, &bufLen, " IND");

            break;
        default:
            /* Unknown packet index */
            err = UN_PRMT;
            break;
    }

    return err;
}

static int SpePktDescAddr(const struct SpePkt *packet,
                          char *buf, size_t bufLen)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    int ns;
    int el;
    int idx = packet->index;
    int ch;
    int pat;
    u64 payload = packet->payload;
    int err = 0;
    static const char *idxName[] = {"PC", "TGT", "VA", "PA", "PBT"};

    switch (idx) {
        case PERF_SPE_ADDR_PKT_HDR_INDEX_INS:
        case PERF_SPE_ADDR_PKT_HDR_INDEX_BRANCH:
        case PERF_SPE_ADDR_PKT_HDR_INDEX_PREV_BRANCH:
            ns = !!PERF_SPE_ADDR_PKT_GET_NS(payload);
            el = PERF_SPE_ADDR_PKT_GET_EL(payload);
            payload = PERF_SPE_ADDR_PKT_ADDR_GET_BYTES_0_6(payload);
            SpePktOutString(&err, &buf, &bufLen,
                            "%s 0x%llx el%d ns=%d",
                            idxName[idx], payload, el, ns);
            break;
        case PERF_SPE_ADDR_PKT_HDR_INDEX_DATA_VIRT:
            SpePktOutString(&err, &buf, &bufLen,
                            "VA 0x%llx", payload);
            break;
        case PERF_SPE_ADDR_PKT_HDR_INDEX_DATA_PHYS:
            ns = !!PERF_SPE_ADDR_PKT_GET_NS(payload);
            ch = !!PERF_SPE_ADDR_PKT_GET_CH(payload);
            pat = PERF_SPE_ADDR_PKT_GET_PAT(payload);
            payload = PERF_SPE_ADDR_PKT_ADDR_GET_BYTES_0_6(payload);
            SpePktOutString(&err, &buf, &bufLen,
                            "PA 0x%llx ns=%d ch=%d pat=%x",
                            payload, ns, ch, pat);
            break;
        default:
            /* Unknown packet index */
            err = UN_PRMT;
            break;
    }

    return err;
}

static int SpePktDesCont(const struct SpePkt *packet,
                         char *buf, size_t bufLen)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    u64 payload = packet->payload;
    const std::string name = SpePktName(packet->type);
    int err = 0;

    SpePktOutString(&err, &buf, &bufLen, "%s %d ", name.c_str(),
                    (unsigned short)payload);

    switch (packet->index) {
        case PERF_SPE_CNT_PKT_HDR_INDEX_TOTAL_LAT:
            SpePktOutString(&err, &buf, &bufLen, "TOT");
            break;
        case PERF_SPE_CNT_PKT_HDR_INDEX_ISSUE_LAT:
            SpePktOutString(&err, &buf, &bufLen, "ISSUE");
            break;
        case PERF_SPE_CNT_PKT_HDR_INDEX_TRANS_LAT:
            SpePktOutString(&err, &buf, &bufLen, "XLAT");
            break;
        default:
            break;
    }

    return err;
}

int SpePktDesc(const struct SpePkt *packet, char *buf,
               size_t bufLen)
{
    CHECK_TRUE(buf == nullptr || packet == nullptr, -1, 1, "Invalid pointer!");
    int idx = packet->index;
    unsigned long long payload = packet->payload;
    const std::string name = SpePktName(packet->type);
    char *bufOrig = buf;
    size_t blen = bufLen;
    int err = 0;

    switch (packet->type) {
        case PERF_SPE_BAD:
        case PERF_SPE_PAD:
        case PERF_SPE_END:
            SpePktOutString(&err, &buf, &blen, "%s", name.c_str());
            break;
        case PERF_SPE_EVENTS:
            err = SpePktDescEvent(packet, buf, bufLen);
            break;
        case PERF_SPE_OP_TYPE:
            err = SpePktDescOpType(packet, buf, bufLen);
            break;
        case PERF_SPE_DATA_SOURCE:
        case PERF_SPE_TIMESTAMP:
            SpePktOutString(&err, &buf, &blen, "%s %lld", name.c_str(), payload);
            break;
        case PERF_SPE_ADDRESS:
            err = SpePktDescAddr(packet, buf, bufLen);
            break;
        case PERF_SPE_CONTEXT:
            SpePktOutString(&err, &buf, &blen, "%s 0x%lx el%d",
                            name.c_str(), (unsigned long)payload, idx + 1);
            break;
        case PERF_SPE_COUNTER:
            err = SpePktDesCont(packet, buf, bufLen);
            break;
        default:
            /* Unknown packet type */
            err = UN_PRMT;
            break;
    }

    /* If any errors are detected, the raw data is output*/
    if (err) {
        err = 0;
        SpePktOutString(&err, &bufOrig, &bufLen, "%s 0x%llx (%d)",
                        name.c_str(), payload, packet->index);
    }

    return err;
}

static u64 SpeCalcIp(int index, u64 payload)
{
    u64 ns;
    u64 el;
    u64 val;

    /* Instruction virtual address or Branch target address */
    if (index == PERF_SPE_ADDR_PKT_HDR_INDEX_INS ||
        index == PERF_SPE_ADDR_PKT_HDR_INDEX_BRANCH) {
        ns = PERF_SPE_ADDR_PKT_GET_NS(payload);
        el = PERF_SPE_ADDR_PKT_GET_EL(payload);

        /* Clean highest byte */
        payload = PERF_SPE_ADDR_PKT_ADDR_GET_BYTES_0_6(payload);

        /* Fill highest byte for EL1 or EL2 (VHE) mode */
        if (ns && (el == PERF_SPE_ADDR_PKT_EL1 || el == PERF_SPE_ADDR_PKT_EL2)) {
            payload |= 0xffULL << PERF_SPE_ADDR_PKT_ADDR_BYTE7_SHIFT;
        }

    /* Data access virtual address */
    } else if (index == PERF_SPE_ADDR_PKT_HDR_INDEX_DATA_VIRT) {
        /* Clean tags */
        payload = PERF_SPE_ADDR_PKT_ADDR_GET_BYTES_0_6(payload);

        /*
         * Armv8 ARM (ARM DDI 0487F.c), chapter "D10.2.1 Address packet"
         * defines the data virtual address payload format, the top byte
         * (bits [63:56]) is assigned as top-byte tag; so we only can
         * retrieve address value from bits [55:0].
         *
         * According to Documentation/arch/arm64/memory.rst, if detects the
         * specific pattern in bits [55:52] of payload which falls in
         * the kernel space, should fixup the top byte and this allows
         * perf tool to parse DSO symbol for data address correctly.
         *
         * For this reason, if detects the bits [55:52] is 0xf, will
         * fill 0xff into the top byte.
         */
        val = PERF_SPE_ADDR_PKT_ADDR_GET_BYTE_6(payload);
        if ((val & 0xf0ULL) == 0xf0ULL) {
            payload |= 0xffULL << PERF_SPE_ADDR_PKT_ADDR_BYTE7_SHIFT;
        }

        /* Data access physical address */
    } else if (index == PERF_SPE_ADDR_PKT_HDR_INDEX_DATA_PHYS) {
        /* Clean highest byte */
        payload = PERF_SPE_ADDR_PKT_ADDR_GET_BYTES_0_6(payload);
    } else {
        static u32 seen_idx = 0;
        if (!(seen_idx & BIT(index))) {
            seen_idx |= BIT(index);
            HLOGV("ignoring address packet index: 0x%x\n", index);
        }
    }

    return payload;
}

void SpeDecoderFree(struct SpeDecoder *decoder)
{
    CHECK_TRUE(decoder == nullptr, NO_RETVAL, 1, "Invalid pointer!");
    free(decoder);
}

static int SpeGetNextPacket(struct SpeDecoder *decoder)
{
    CHECK_TRUE(decoder == nullptr, -1, 1, "Invalid pointer!");
    int ret = 1;

    do {
        if (!decoder->len) {
            /* Failed to read out trace data */
            if (ret <= 0) {
                return ret;
            }
        }

        ret = SpeGetPacket(decoder->buf, decoder->len,
                           &decoder->packet);
        if (ret <= 0) {
            /* Move forward for 1 byte */
            decoder->buf += 1;
            decoder->len -= 1;
            return -EBADMSG;
        }

        decoder->buf += ret;
        decoder->len -= static_cast<size_t>(ret);
    } while (decoder->packet.type == PERF_SPE_PAD);
    return 1;
}

static int SpeReadRecord(struct SpeDecoder *decoder)
{
    u64 payload;
    u64 ip;
    CHECK_TRUE(decoder == nullptr, -1, 1, "Invalid pointer!");
    if (memset_s(&decoder->record, sizeof(decoder->record), 0, sizeof(decoder->record)) != EOK) {
        HLOGE("memset_s failed in SpeReadRecord.");
        return -1;
    }
    decoder->record.context_id = (u64)-1;

    while (true) {
        int err = SpeGetNextPacket(decoder);
        if (err <= 0) {
            return err;
        }

        int idx = decoder->packet.index;
        payload = decoder->packet.payload;

        switch (decoder->packet.type) {
            case PERF_SPE_TIMESTAMP:
                decoder->record.timestamp = payload;
                return 1;
            case PERF_SPE_END:
                return 1;
            case PERF_SPE_ADDRESS:
                ip = SpeCalcIp(idx, payload);
                if (idx == PERF_SPE_ADDR_PKT_HDR_INDEX_INS) {
                    decoder->record.from_ip = ip;
                } else if (idx == PERF_SPE_ADDR_PKT_HDR_INDEX_BRANCH) {
                    decoder->record.to_ip = ip;
                } else if (idx == PERF_SPE_ADDR_PKT_HDR_INDEX_DATA_VIRT) {
                    decoder->record.virt_addr = ip;
                } else if (idx == PERF_SPE_ADDR_PKT_HDR_INDEX_DATA_PHYS) {
                    decoder->record.phys_addr = ip;
                }
                break;
            case PERF_SPE_COUNTER:
                if (idx == PERF_SPE_CNT_PKT_HDR_INDEX_TOTAL_LAT) {
                    decoder->record.latency = payload;
                }
                break;
            case PERF_SPE_CONTEXT:
                decoder->record.context_id = payload;
                break;
            case PERF_SPE_OP_TYPE:
                switch (idx) {
                    case PERF_SPE_OP_PKT_HDR_CLASS_LD_ST_ATOMIC:
                        decoder->record.op |= PERF_SPE_OP_LDST;
                        if (payload & PERF_SPE_OP_PKT_ST) {
                            decoder->record.op |= PERF_SPE_OP_ST;
                        } else {
                            decoder->record.op |= PERF_SPE_OP_LD;
                        }

                        if (PERF_SPE_OP_PKT_IS_LDST_SVE(payload)) {
                            decoder->record.op |= PERF_SPE_OP_SVE_LDST;
                        }
                        break;
                    case PERF_SPE_OP_PKT_HDR_CLASS_OTHER:
                        decoder->record.op |= PERF_SPE_OP_OTHER;
                        if (PERF_SPE_OP_PKT_IS_OTHER_SVE_OP(payload)) {
                            decoder->record.op |= PERF_SPE_OP_SVE_OTHER;
                        }
                        break;
                    case PERF_SPE_OP_PKT_HDR_CLASS_BR_ERET:
                        decoder->record.op |= PERF_SPE_OP_BRANCH_ERET;
                        break;
                    default:
                        HLOGV("Get packet error!");
                }
                break;
            case PERF_SPE_EVENTS:
                if (payload & BIT(EVENT_L1D_REFILL)) {
                    decoder->record.type |= PERF_SPE_L1D_MISS;
                }

                if (payload & BIT(EVENT_L1D_ACCESS)) {
                    decoder->record.type |= PERF_SPE_L1D_ACCESS;
                }

                if (payload & BIT(EVENT_TLB_WALK)) {
                    decoder->record.type |= PERF_SPE_TLB_MISS;
                }

                if (payload & BIT(EVENT_TLB_ACCESS)) {
                    decoder->record.type |= PERF_SPE_TLB_ACCESS;
                }

                if (payload & BIT(EVENT_LLC_MISS)) {
                    decoder->record.type |= PERF_SPE_LLC_MISS;
                }

                if (payload & BIT(EVENT_LLC_ACCESS)) {
                    decoder->record.type |= PERF_SPE_LLC_ACCESS;
                }

                if (payload & BIT(EVENT_REMOTE_ACCESS)) {
                    decoder->record.type |= PERF_SPE_REMOTE_ACCESS;
                }

                if (payload & BIT(EVENT_MISPRED)) {
                    decoder->record.type |= PERF_SPE_BRANCH_MISS;
                }

                if (payload & BIT(EVENT_PARTIAL_PREDICATE)) {
                    decoder->record.type |= PERF_SPE_SVE_PARTIAL_PRED;
                }

                if (payload & BIT(EVENT_EMPTY_PREDICATE)) {
                    decoder->record.type |= PERF_SPE_SVE_EMPTY_PRED;
                }
                break;
            case PERF_SPE_DATA_SOURCE:
                decoder->record.source = payload;
                break;
            case PERF_SPE_BAD:
                break;
            case PERF_SPE_PAD:
                break;
            default:
                printf("Get packet error!\n");
                return -1;
        }
    }
    return 0;
}

int SpeDecode(struct SpeDecoder *decoder)
{
    CHECK_TRUE(decoder == nullptr, -1, 1, "Invalid pointer!");
    return SpeReadRecord(decoder);
}

struct SpeDecoder *SpeDecoderDataNew(const unsigned char *speBuf, size_t speLen)
{
    CHECK_TRUE(speBuf == nullptr, nullptr, 1, "Invalid pointer!");
    struct SpeDecoder *decoder;

    decoder = reinterpret_cast<SpeDecoder *>(malloc(sizeof(struct SpeDecoder)));
    if (!decoder) {
        return nullptr;
    }
    if (memset_s(decoder, sizeof(struct SpeDecoder), 0, sizeof(struct SpeDecoder)) != EOK) {
        HLOGE("memset_s failed in SpeDecoderDataNew.");
        free(decoder);
        return nullptr;
    }

    decoder->buf = speBuf;
    decoder->len = speLen;

    return decoder;
}

bool SpeDumpRawData(unsigned char *buf, size_t len, int indent, FILE *outputDump)
{
    CHECK_TRUE(buf == nullptr, false, 1, "Invalid pointer!");
    if (outputDump != nullptr) {
        g_outputDump = outputDump;
    }
    struct SpePkt packet;
    size_t pos = 0;
    int pktLen;
    int i;
    char desc[PERF_SPE_PKT_DESC_MAX];

    PRINT_INDENT(indent, ". ... ARM SPE data: size %#zx bytes\n", len);
    while (len) {
        int ret = SpeGetPacket(buf, len, &packet);
        if (ret > 0) {
            pktLen = ret;
        } else {
            pktLen = 1;
        }
        PRINT_INDENT(indent, ".");
        PRINT_INDENT(indent, "  %08zx: ", pos);
        for (i = 0; i < pktLen; i++) {
            PRINT_INDENT(indent, " %02x", buf[i]);
        }
        for (; i < 16; i++) { // 16 : space
            PRINT_INDENT(indent, "   ");
        }
        if (ret > 0) {
            ret = SpePktDesc(&packet, desc,
                             PERF_SPE_PKT_DESC_MAX);
            if (!ret) {
                PRINT_INDENT(indent, " %s\n", desc);
            }
        } else {
            PRINT_INDENT(indent, " Bad packet!\n");
        }
        pos += static_cast<size_t>(pktLen);
        buf += pktLen;
        if (len >= static_cast<size_t>(pktLen)) {
            len -= static_cast<size_t>(pktLen);
        } else {
            break;
        }
    }
    return true;
}

std::map<u32, std::map<u64, ReportItemAuxRawData>> AuxRawDataMap_;
std::map<u32, u64> typeCount;

const std::vector<u32> DEFAULT_SPE_EVENT_TYPE = {
    PERF_SPE_L1D_ACCESS,
    PERF_SPE_L1D_MISS,
    PERF_SPE_LLC_ACCESS,
    PERF_SPE_LLC_MISS,
    PERF_SPE_TLB_ACCESS,
    PERF_SPE_TLB_MISS,
    PERF_SPE_BRANCH_MISS,
    PERF_SPE_REMOTE_ACCESS,
    PERF_SPE_SVE_PARTIAL_PRED,
    PERF_SPE_SVE_EMPTY_PRED,
};

constexpr const int SPE_PERCENTAGE_COMM_LEN = 40;
constexpr const int SPE_PERCENTAGE_PC_LEN = 18;
constexpr const int SPE_PERCENTAGE_DSO_LEN = 50;
constexpr const int SPE_PERCENTAGE_FUNC_LEN = 60;
constexpr const int SPE_PERCENTAGE_OFFSET_LEN = 20;

void AddReportItems(const std::vector<ReportItemAuxRawData>& auxRawData)
{
    for (const auto& data : auxRawData) {
        for (auto type : DEFAULT_SPE_EVENT_TYPE) {
            if (data.type & type) {
                if (typeCount.count(type) == 0) {
                    typeCount[type] = 1;
                } else {
                    typeCount[type]++;
                }
                std::map<u64, ReportItemAuxRawData>& map = AuxRawDataMap_[type];

                auto data1 = map.find(data.pc);
                if (data1 == map.end()) {
                    HLOGV("add %llx", data.pc);
                    map[data.pc] = {type, 0.0f, 1, data.comm, data.pc, data.SharedObject, data.Symbol, data.offset};
                } else {
                    HLOGV("add pc: %llx", data.pc);
                    data1->second.count++;
                    HLOGV("add pc: %llx count: %llu", data.pc, data1->second.count);
                }
            }
        }
    }
}

void UpdateHeating()
{
    for (auto it = AuxRawDataMap_.begin(); it != AuxRawDataMap_.end(); it++) {
        u64 cc = typeCount[it->first];
        for (auto& it2 : it->second) {
            float heating = (float)it2.second.count / cc * FULL_PERCENTAGE;
            HLOGV("heating %llu/%llu %f", it2.second.count, cc, heating);
            it2.second.heating = heating;
        }
    }
}

void GetSpeEventNameByType(uint32_t type, std::string& eventName)
{
    switch (type) {
        case PERF_SPE_L1D_ACCESS:
            eventName = "l1d-access";
            break;
        case PERF_SPE_L1D_MISS:
            eventName = "l1d-miss";
            break;
        case PERF_SPE_LLC_ACCESS:
            eventName = "llc-access";
            break;
        case PERF_SPE_LLC_MISS:
            eventName = "llc-miss";
            break;
        case PERF_SPE_TLB_ACCESS:
            eventName = "tlb-access";
            break;
        case PERF_SPE_TLB_MISS:
            eventName = "tlb-miss";
            break;
        case PERF_SPE_BRANCH_MISS:
            eventName = "branch-miss";
            break;
        case PERF_SPE_REMOTE_ACCESS:
            eventName = "remote-access";
            break;
        case PERF_SPE_SVE_PARTIAL_PRED:
            eventName = "paritial_read";
            break;
        case PERF_SPE_SVE_EMPTY_PRED:
            eventName = "empty_read";
            break;
        default:
            eventName = "unknow";
            return;
        }
}

void DumpSpeReportHead(int indent, uint32_t type, uint64_t count)
{
    std::string eventName = "";
    GetSpeEventNameByType(type, eventName);
    PRINT_INDENT(indent, "\nEvent :%s\n", eventName.c_str());
    PRINT_INDENT(indent, "Samples Count: %" PRIu64 "\n", count);

    // head print
    const std::string head = "Heating";
    PRINT_INDENT(indent, "%-*s ", FULL_PERCENTAGE_LEN, head.c_str());
    const std::string eventCount = "   count";
    PRINT_INDENT(indent, "%-*s ", FULL_PERCENTAGE_LEN, eventCount.c_str());
    const std::string comm = "    comm";
    PRINT_INDENT(indent, "%-*s ", SPE_PERCENTAGE_COMM_LEN, comm.c_str());
    const std::string pc = "      PC";
    PRINT_INDENT(indent, "%-*s ", SPE_PERCENTAGE_PC_LEN, pc.c_str());
    const std::string dso = "          dso";
    PRINT_INDENT(indent, "%-*s ", SPE_PERCENTAGE_DSO_LEN, dso.c_str());
    const std::string func = "            func";
    PRINT_INDENT(indent, "%-*s", SPE_PERCENTAGE_FUNC_LEN, func.c_str());
    const std::string offset = "              offset";
    PRINT_INDENT(indent, "%-*s\n", SPE_PERCENTAGE_OFFSET_LEN, offset.c_str());
    return;
}

void DumpSpeReportData(int indent, FILE *outputDump)
{
    if (outputDump != nullptr) {
        g_outputDump = outputDump;
    }
    if (AuxRawDataMap_.empty()) {
        return;
    }
    PRINT_INDENT(indent, "\n ==== Spe Report Data ====\n");
    for (auto it = AuxRawDataMap_.begin(); it != AuxRawDataMap_.end(); it++) {
        u64 count = typeCount[it->first];
        DumpSpeReportHead(indent, it->first, count);
        std::vector<ReportItemAuxRawData> auxRawData;
        for (auto& it2 : it->second) {
            struct ReportItemAuxRawData reportItem = {
                it2.second.type,
                it2.second.heating,
                it2.second.count,
                it2.second.comm,
                it2.second.pc,
                it2.second.SharedObject,
                it2.second.Symbol,
                it2.second.offset
            };
            auxRawData.emplace_back(reportItem);
        }
        std::sort(auxRawData.begin(), auxRawData.end(), [](const ReportItemAuxRawData& lhs,
                                                           const ReportItemAuxRawData& rhs) {
            return lhs.count > rhs.count;
        });
        for (auto& it3 : auxRawData) {
            PRINT_INDENT(indent + 1, "%*.2f%% ", FULL_PERCENTAGE_LEN, it3.heating);
            PRINT_INDENT(indent + 1, "%-*llu ", FULL_PERCENTAGE_LEN, it3.count);
            PRINT_INDENT(indent + 1, "%-*s ", SPE_PERCENTAGE_COMM_LEN, it3.comm.c_str());
            PRINT_INDENT(indent + 1, "0x%-*llx ", SPE_PERCENTAGE_PC_LEN, it3.pc);
            PRINT_INDENT(indent + 1, "%-*s ", SPE_PERCENTAGE_DSO_LEN, it3.SharedObject.c_str());
            PRINT_INDENT(indent + 1, "%-*s", SPE_PERCENTAGE_FUNC_LEN, it3.Symbol.c_str());
            PRINT_INDENT(indent + 1, "0x%llx\n", it3.offset);
        }
    }
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
