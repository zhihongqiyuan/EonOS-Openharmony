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

#include "etsPrimitiveType.h"

#include "checker/TSchecker.h"
#include "checker/ETSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "macros.h"

namespace ark::es2panda::ir {
void ETSPrimitiveType::TransformChildren([[maybe_unused]] const NodeTransformer &cb,
                                         [[maybe_unused]] std::string_view const transformationName)
{
}

void ETSPrimitiveType::Iterate([[maybe_unused]] const NodeTraverser &cb) const {}

void ETSPrimitiveType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSPrimitiveType"}});
}

void ETSPrimitiveType::Dump(ir::SrcDumper *dumper) const
{
    switch (GetPrimitiveType()) {
        case PrimitiveType::BYTE:
            dumper->Add("byte");
            break;
        case PrimitiveType::INT:
            dumper->Add("int");
            break;
        case PrimitiveType::LONG:
            dumper->Add("long");
            break;
        case PrimitiveType::SHORT:
            dumper->Add("short");
            break;
        case PrimitiveType::FLOAT:
            dumper->Add("float");
            break;
        case PrimitiveType::DOUBLE:
            dumper->Add("double");
            break;
        case PrimitiveType::BOOLEAN:
            dumper->Add("boolean");
            break;
        case PrimitiveType::CHAR:
            dumper->Add("char");
            break;
        case PrimitiveType::VOID:
            dumper->Add("void");
            break;
        default:
            UNREACHABLE();
    }
}

void ETSPrimitiveType::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ETSPrimitiveType::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ETSPrimitiveType::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSPrimitiveType::GetType([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GlobalAnyType();
}

checker::Type *ETSPrimitiveType::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSPrimitiveType::GetType([[maybe_unused]] checker::ETSChecker *checker)
{
    switch (type_) {
        case PrimitiveType::BYTE: {
            SetTsType(checker->GlobalByteType());
            return TsType();
        }
        case PrimitiveType::SHORT: {
            SetTsType(checker->GlobalShortType());
            return TsType();
        }
        case PrimitiveType::INT: {
            SetTsType(checker->GlobalIntType());
            return TsType();
        }
        case PrimitiveType::LONG: {
            SetTsType(checker->GlobalLongType());
            return TsType();
        }
        case PrimitiveType::FLOAT: {
            SetTsType(checker->GlobalFloatType());
            return TsType();
        }
        case PrimitiveType::DOUBLE: {
            SetTsType(checker->GlobalDoubleType());
            return TsType();
        }
        case PrimitiveType::BOOLEAN: {
            SetTsType(checker->GlobalETSBooleanType());
            return TsType();
        }
        case PrimitiveType::CHAR: {
            SetTsType(checker->GlobalCharType());
            return TsType();
        }
        case PrimitiveType::VOID: {
            if (LIKELY(checker->CheckVoidAnnotation(this))) {
                SetTsType(checker->GlobalVoidType());
                return TsType();
            }

            return checker->InvalidateType(this);
        }
        default: {
            UNREACHABLE();
        }
    }
}

ETSPrimitiveType *ETSPrimitiveType::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    if (auto *const clone = allocator->New<ETSPrimitiveType>(type_); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRange(Range());
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

}  // namespace ark::es2panda::ir
