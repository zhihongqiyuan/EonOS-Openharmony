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

#include "unboxingConverter.h"
#include "checker/types/ets/types.h"
#include "checker/ETSchecker.h"
#include "util/helpers.h"

namespace ark::es2panda::checker {

checker::Type *UnboxingConverter::Convert(checker::ETSChecker const *checker, ETSObjectType *type)
{
    switch (type->UnboxableKind()) {
        case ETSObjectFlags::BUILTIN_BOOLEAN:
            return checker->GlobalETSBooleanType();
        case ETSObjectFlags::BUILTIN_BYTE:
            return checker->GlobalByteType();
        case ETSObjectFlags::BUILTIN_SHORT:
            return checker->GlobalShortType();
        case ETSObjectFlags::BUILTIN_CHAR:
            return checker->GlobalCharType();
        case ETSObjectFlags::BUILTIN_INT:
            return checker->GlobalIntType();
        case ETSObjectFlags::BUILTIN_LONG:
            return checker->GlobalLongType();
        case ETSObjectFlags::BUILTIN_FLOAT:
            return checker->GlobalFloatType();
        case ETSObjectFlags::BUILTIN_DOUBLE:
            return checker->GlobalDoubleType();
        case ETSObjectFlags::BOXED_ENUM:
            return type->GetDeclNode()->AsClassDefinition()->OrigEnumDecl()->TsType();
        default:
            UNREACHABLE();
    }
}

}  // namespace ark::es2panda::checker
