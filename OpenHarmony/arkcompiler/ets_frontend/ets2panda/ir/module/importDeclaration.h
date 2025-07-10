/**
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

#ifndef ES2PANDA_IR_MODULE_IMPORT_DECLARATION_H
#define ES2PANDA_IR_MODULE_IMPORT_DECLARATION_H

#include "ir/statement.h"

namespace ark::es2panda::ir {
class StringLiteral;

enum class ImportKinds { VALUE, TYPE };

class ImportDeclaration : public Statement {
public:
    explicit ImportDeclaration(StringLiteral *source, ArenaVector<AstNode *> const &specifiers,
                               const ImportKinds importKind = ImportKinds::VALUE)
        : Statement(AstNodeType::IMPORT_DECLARATION), source_(source), specifiers_(specifiers), importKind_(importKind)
    {
    }

    const StringLiteral *Source() const
    {
        return source_;
    }

    StringLiteral *Source()
    {
        return source_;
    }

    const ArenaVector<AstNode *> &Specifiers() const
    {
        return specifiers_;
    }

    ArenaVector<AstNode *> &Specifiers()
    {
        return specifiers_;
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

    bool IsTypeKind() const
    {
        return importKind_ == ImportKinds::TYPE;
    }

private:
    StringLiteral *source_;
    ArenaVector<AstNode *> specifiers_;
    ImportKinds importKind_;
};
}  // namespace ark::es2panda::ir

#endif
