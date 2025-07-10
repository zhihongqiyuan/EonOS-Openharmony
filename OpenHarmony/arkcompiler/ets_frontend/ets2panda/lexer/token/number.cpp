/*
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "number.h"
#include "lexer/lexer.h"

namespace ark::es2panda::lexer {
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,bugprone-exception-escape)
Number::Number(util::StringView str, NumberFlags flags) noexcept : str_(str), flags_(flags)
{
    Lexer::ConversionResult res {};

    if ((flags & (NumberFlags::DECIMAL_POINT | NumberFlags::EXPONENT)) == std::underlying_type_t<TokenFlags>(0)) {
        const int64_t temp = Lexer::StrToNumeric(&std::strtoll, str.Utf8().data(), res, 10);

        if (res == Lexer::ConversionResult::SUCCESS) {
            if (temp <= std::numeric_limits<int32_t>::max() && temp >= std::numeric_limits<int32_t>::min()) {
                num_ = static_cast<int32_t>(temp);
            } else {
                num_ = temp;
            }
        } else {
            flags_ |= NumberFlags::ERROR;
        }
    } else {
        const double temp = Lexer::StrToNumeric(&std::strtod, str.Utf8().data(), res);
        if (res == Lexer::ConversionResult::SUCCESS) {
            if (str.Utf8().back() != 'f') {
                num_ = temp;
            } else {
                num_ = static_cast<float>(temp);
            }
        } else {
            flags_ |= NumberFlags::ERROR;
        }
    }
}
}  // namespace ark::es2panda::lexer
