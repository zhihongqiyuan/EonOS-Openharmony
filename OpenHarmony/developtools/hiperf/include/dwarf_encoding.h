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
#ifndef DWARF_ENCODING_H
#define DWARF_ENCODING_H

#include "utilities.h"

// now we only support 64 bit.
using uleb128_t = uint64_t;
using sleb128_t = int64_t;

namespace OHOS {
namespace Developtools {
namespace HiPerf {
constexpr const int LEB_BYTE_EFFECTIVE_LENGTH = 7;
constexpr const int SIGN_BIT_OF_BYTE = 0x40;
constexpr const int MAX_VALUE_OF_BYTE = 0x7f;
constexpr const int MORE_BIT_OF_BYTE = 0x80;

/*
10.5.1. DWARF Exception Header Encoding
The DWARF Exception Header Encoding is used to describe the type of data used in the .eh_frame and
.eh_frame_hdr section. The upper 4 bits indicate how the value is to be applied. The lower 4 bits
indicate the format of the data.
using dw_encode_t = unsigned char; // 4 bits + 4 bits
*/

using dw_encode_t = unsigned char; // 4 bits + 4 bits

// Table 10-5. DWARF Exception Header value format

enum DW_EH_PE_VF {
    DW_EH_PE_absptr = 0x00,  // a literal pointer whose size is determined by the architecture.
    DW_EH_PE_uleb128 = 0x01, // Unsigned value is encoded using the Little Endian Base 128 (LEB128)
    DW_EH_PE_udata2 = 0x02,  // A 2 bytes unsigned value.
    DW_EH_PE_udata4 = 0x03,  // A 4 bytes unsigned value.
    DW_EH_PE_udata8 = 0x04,  // An 8 bytes unsigned value.
    DW_EH_PE_sleb128 = 0x09, // Signed value is encoded using the Little Endian Base 128(LEB128)
    DW_EH_PE_sdata2 = 0x0A,  // A 2 bytes signed value.
    DW_EH_PE_sdata4 = 0x0B,  // A 4 bytes signed value.
    DW_EH_PE_sdata8 = 0x0C,  // An 8 bytes signed value.
};

// Table 10-6. DWARF Exception Header application
enum DW_EH_PE_A {
    DW_EH_PE_nothing = 0x00, // nothing to do
    DW_EH_PE_pcrel = 0x10,   // relative to the current program counter.
    DW_EH_PE_textrel = 0x20, // relative to the beginning of the .text section.
    DW_EH_PE_datarel = 0x30, // relative to the beginning of the .got or .eh_frame_hdr section.
    DW_EH_PE_funcrel = 0x40, // relative to the beginning of the function.
    DW_EH_PE_aligned = 0x50, // aligned to an address unit sized boundary.
    DW_EH_PE_omit = 0xff,    // indicate that no value is present.
};

const std::map<dw_encode_t, size_t> DWFormatSizeMap = {
#ifdef ARM
    {DW_EH_PE_absptr, 4},
#else
    {DW_EH_PE_absptr, 8},
#endif
#ifdef NOT_USE
    {DW_EH_PE_uleb128, sizeof(char) * 128},
#endif
    {DW_EH_PE_udata2, sizeof(char) * 2},
    {DW_EH_PE_udata4, sizeof(char) * 4},
    {DW_EH_PE_udata8, sizeof(char) * 8},
#ifdef NOT_USE
    {DW_EH_PE_sleb128, sizeof(char) * 128},
#endif
    {DW_EH_PE_sdata2, sizeof(char) * 2},
    {DW_EH_PE_sdata4, sizeof(char) * 4},
    {DW_EH_PE_sdata8, sizeof(char) * 8},
};

template<class T>
uint64_t dwReadAnyTypeData(const unsigned char *&buffer, T)
{
    T value;
    if (memcpy_s(&value, sizeof(T), buffer, sizeof(T)) != 0) {
        return 0;
    }
    buffer += sizeof(T);
    return static_cast<uint64_t>(value);
}

class DwarfEncoding {
public:
    DwarfEncoding(dw_encode_t dw, const unsigned char *&data, uint64_t vaddrBase = 0,
                  uint64_t vaddrPC = 0, uint64_t vaddrText = 0);

    const std::string ToString() const;

    const unsigned char *GetEnd() const;

    const unsigned char *GetData() const;

    size_t GetSize() const;

    uint64_t GetValue() const;

    uint64_t GetAppliedValue() const;

    bool IsOmit() const;

private:
    dw_encode_t dw_;
    const unsigned char *data_;
    uint64_t vaddrBase_ = 0;
    uint64_t vaddrPC_ = 0;
    uint64_t vaddrText_ = 0;
    uint64_t value_[2] = {0, 0};

    dw_encode_t Format() const;

    dw_encode_t Application() const;

    uint64_t ReadValue(const unsigned char *&data) const;

    const std::string FormatName() const;

    const std::string ApplicationName() const;
};

/*
Linux Standard Base Core Specification 4.1
Chapter 10. Object Format
10.6.2. The .eh_frame_hdr section

Table 10-11. .eh_frame_hdr Section Format

Encoding	Field
unsigned byte	version
unsigned byte	eh_frame_ptr_enc
unsigned byte	fde_count_enc
unsigned byte	table_enc
encoded	eh_frame_ptr
encoded	fde_count
    binary search table
*/

struct eh_frame_hdr {
    // Version of the .eh_frame_hdr format. This value shall be 1.
    dw_encode_t version;

    // The encoding format of the eh_frame_ptr field.
    dw_encode_t eh_frame_ptr_enc;

    // The encoding format of the fde_count field. A value of DW_EH_PE_omit indicates the binary
    // search table is not present.
    dw_encode_t fde_count_enc;

    // The encoding format of the entries in the binary search table. A value of DW_EH_PE_omit
    // indicates the binary search table is not present.
    dw_encode_t table_enc;

    // The encoded value of the pointer to the start of the .eh_frame section.
    /*
    dw_encode_t eh_frame_ptr
    dw_encode_t fde_count
    */
    // A binary search table containing fde_count entries. Each entry of the table consist of two
    // encoded values, the initial location, and the address. The entries are sorted in an
    // increasing order by the initial location value.

    unsigned char encode_data[0];
} PACKED;
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // DWARF_ENCODING_H
