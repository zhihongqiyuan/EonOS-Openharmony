/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#define HILOG_TAG "Dwarf"

#include "dwarf_encoding.h"

#include "utilities.h"
namespace OHOS {
namespace Developtools {
namespace HiPerf {
DwarfEncoding::DwarfEncoding(dw_encode_t dw, const unsigned char *&data, uint64_t vaddrBase,
                             uint64_t vaddrPC, uint64_t vaddrText)
    : dw_(dw), data_(data), vaddrBase_(vaddrBase), vaddrPC_(vaddrPC), vaddrText_(vaddrText)
{
    value_[0] = ReadValue(data);
}

const std::string DwarfEncoding::ToString() const
{
    std::string debugString = ApplicationName() + ":" + FormatName() + " format:" + ToHex(dw_) +
                              " value size:" + std::to_string(GetSize()) + " raw:";

    size_t size = GetSize();
    const unsigned char *data = data_;
    while (size-- > 0) {
        debugString.append(ToHex(data[0]) + " ");
        data++;
    }

    debugString.append(" | " + ToHex(GetValue(), GetSize(), true));
    debugString.append(" applied:" + ToHex(GetAppliedValue(), GetSize()));
    return debugString;
}

const unsigned char *DwarfEncoding::GetEnd() const
{
    return data_ + GetSize();
}

const unsigned char *DwarfEncoding::GetData() const
{
    return data_;
}

size_t DwarfEncoding::GetSize() const
{
    return DWFormatSizeMap.at((dw_encode_t)Format());
}

uint64_t DwarfEncoding::GetValue() const
{
    return value_[0];
}

uint64_t DwarfEncoding::GetAppliedValue() const
{
    if ((Application() & DW_EH_PE_datarel) == DW_EH_PE_datarel) {
        return value_[0] + vaddrBase_;
    } else if ((Application() & DW_EH_PE_textrel) == DW_EH_PE_textrel) {
        return value_[0] + vaddrText_;
    } else if ((Application() & DW_EH_PE_pcrel) == DW_EH_PE_pcrel) {
        return value_[0] + vaddrPC_;
    }
    HLOGM("Application is empty");

    return value_[0];
}

bool DwarfEncoding::IsOmit() const
{
    return (dw_ == DW_EH_PE_omit);
}

dw_encode_t DwarfEncoding::Format() const
{
    return (dw_ & 0x0F);
}
dw_encode_t DwarfEncoding::Application() const
{
    return (dw_ & 0xF0);
}
uint64_t DwarfEncoding::ReadValue(const unsigned char *&data) const
{
    switch (Format()) {
        case DW_EH_PE_udata2:
            return dwReadAnyTypeData(data, uint16_t());
        case DW_EH_PE_udata4:
            return dwReadAnyTypeData(data, uint32_t());
        case DW_EH_PE_udata8:
            return dwReadAnyTypeData(data, uint64_t());
        case DW_EH_PE_sdata2:
            return dwReadAnyTypeData(data, int16_t());
        case DW_EH_PE_sdata4:
            return dwReadAnyTypeData(data, int32_t());
        case DW_EH_PE_sdata8:
            return dwReadAnyTypeData(data, int64_t());
        default:
            return -1;
    }
}
const std::string DwarfEncoding::FormatName() const
{
    switch (Format()) {
        case DW_EH_PE_absptr:
            return "DW_EH_PE_absptr";
        case DW_EH_PE_uleb128:
            return "DW_EH_PE_uleb128";
        case DW_EH_PE_udata2:
            return "DW_EH_PE_udata2";
        case DW_EH_PE_udata4:
            return "DW_EH_PE_udata4";
        case DW_EH_PE_udata8:
            return "DW_EH_PE_udata8";
        case DW_EH_PE_sleb128:
            return "DW_EH_PE_sleb128";
        case DW_EH_PE_sdata2:
            return "DW_EH_PE_sdata2";
        case DW_EH_PE_sdata4:
            return "DW_EH_PE_sdata4";
        case DW_EH_PE_sdata8:
            return "DW_EH_PE_sdata8";
        case DW_EH_PE_omit:
            return "DW_EH_PE_omit";
        default:
            return "unknown format";
    }
}
const std::string DwarfEncoding::ApplicationName() const
{
    switch (Application()) {
        case DW_EH_PE_pcrel:
            return "DW_EH_PE_pcrel";
        case DW_EH_PE_textrel:
            return "DW_EH_PE_textrel";
        case DW_EH_PE_datarel:
            return "DW_EH_PE_datarel";
        case DW_EH_PE_funcrel:
            return "DW_EH_PE_funcrel";
        case DW_EH_PE_aligned:
            return "DW_EH_PE_aligned";
        case DW_EH_PE_omit:
            return "DW_EH_PE_omit";
        case DW_EH_PE_nothing:
            return "DW_EH_PE_empty";
        default:
            return "unknown format";
    }
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
