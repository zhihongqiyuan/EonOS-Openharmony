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

#ifndef ES2PANDA_IR_ETS_IMPORT_DECLARATION_H
#define ES2PANDA_IR_ETS_IMPORT_DECLARATION_H

#include "ir/ets/etsImportSource.h"
#include "ir/module/importDeclaration.h"
#include "util/language.h"
#include "util/ustring.h"

namespace ark::es2panda::ir {
class StringLiteral;

class ETSImportDeclaration : public ImportDeclaration {
public:
    explicit ETSImportDeclaration(ImportSource *source, const ArenaVector<AstNode *> &specifiers,
                                  const ImportKinds importKind = ImportKinds::VALUE)
        : ImportDeclaration(source->Source(), specifiers, importKind), source_(source)
    {
        SetType(AstNodeType::ETS_IMPORT_DECLARATION);
    }

    es2panda::Language Language() const
    {
        return source_->Language();
    }

    bool HasDecl() const
    {
        return source_->HasDecl();
    }

    bool IsPureDynamic() const
    {
        return !HasDecl() && Language().IsDynamic();
    }

    util::StringView &AssemblerName()
    {
        return assemblerName_;
    }

    const util::StringView &AssemblerName() const
    {
        return assemblerName_;
    }

    StringLiteral *Source() const
    {
        return source_->Source();
    }

    StringLiteral *ResolvedSource()
    {
        return source_->ResolvedSource();
    }

    const StringLiteral *ResolvedSource() const
    {
        return source_->ResolvedSource();
    }

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    ImportSource *source_;
    util::StringView assemblerName_ {};
};
}  // namespace ark::es2panda::ir

#endif
