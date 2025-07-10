/**
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_IR_ETS_RE_EXPORT_DECLARATION_H
#define ES2PANDA_IR_ETS_RE_EXPORT_DECLARATION_H

#include "ir/ets/etsImportDeclaration.h"
#include "ir/ets/etsImportSource.h"
#include "ir/module/importDeclaration.h"
#include "varbinder/varbinder.h"

namespace ark::es2panda::ir {

class ETSReExportDeclaration : public Statement {
public:
    explicit ETSReExportDeclaration(ETSImportDeclaration *etsImportDeclarations,
                                    const std::vector<std::string> &userPaths, util::StringView programPath,
                                    ArenaAllocator *allocator);

    ETSImportDeclaration *GetETSImportDeclarations() const
    {
        return etsImportDeclarations_;
    }

    ETSImportDeclaration *GetETSImportDeclarations()
    {
        return etsImportDeclarations_;
    }

    const ArenaVector<util::StringView> &GetUserPaths() const
    {
        return userPaths_;
    }

    util::StringView const &GetProgramPath() const
    {
        return programPath_;
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;

    void Iterate(const NodeTraverser &cb) const override;

    void Dump(ir::AstDumper *dumper) const override;
    void Dump([[maybe_unused]] ir::SrcDumper *dumper) const override {};

    void Compile(compiler::PandaGen * /*pg*/) const override {}

    void Compile(compiler::ETSGen * /*gen*/) const override {}

    checker::Type *Check(checker::TSChecker * /*checker*/) override
    {
        return nullptr;
    }

    checker::Type *Check(checker::ETSChecker * /*checker*/) override
    {
        return nullptr;
    }

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    // NOTE(rsipka): this should use a singular name
    ETSImportDeclaration *etsImportDeclarations_;
    ArenaVector<util::StringView> userPaths_;
    util::StringView programPath_;
};
}  // namespace ark::es2panda::ir

#endif
