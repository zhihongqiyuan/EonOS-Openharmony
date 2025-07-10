/*
 * Copyright (c) 2021 - 2023 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_COMPILER_BASE_JSLREFERENCE_H
#define ES2PANDA_COMPILER_BASE_JSLREFERENCE_H

#include "varbinder/scope.h"
#include "ir/irnode.h"

namespace ark::es2panda::ir {
class AstNode;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::checker {
class ETSObjectType;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::compiler {
enum class ReferenceKind {
    MEMBER,
    PRIVATE,
    SUPER,
    VAR_OR_GLOBAL,
    DESTRUCTURING,
    LOCAL,
    STATIC_FIELD,
    FIELD,
    CLASS,
    STATIC_CLASS,
    METHOD,
    STATIC_METHOD,
};

class CodeGen;
class ETSGen;
class PandaGen;

class LReference {
public:
    ~LReference() = default;
    NO_COPY_SEMANTIC(LReference);
    DEFAULT_MOVE_SEMANTIC(LReference);

    ReferenceKind Kind() const
    {
        return refKind_;
    }

    void SetKind(ReferenceKind refKind)
    {
        refKind_ = refKind;
    }

    varbinder::Variable *Variable() const
    {
        return res_.variable;
    }

    const ir::AstNode *Node() const
    {
        return node_;
    }

    varbinder::ConstScopeFindResult &Result()
    {
        return res_;
    }

    const varbinder::ConstScopeFindResult &Result() const
    {
        return res_;
    }

    bool IsDeclaration() const
    {
        return isDeclaration_;
    }

protected:
    using LReferenceBase =
        std::tuple<CodeGen *, const ir::AstNode *, ReferenceKind, varbinder::ConstScopeFindResult, bool>;
    static LReferenceBase CreateBase(CodeGen *cg, const ir::AstNode *node, bool isDeclaration);

    explicit LReference(const ir::AstNode *node, ReferenceKind refKind, varbinder::ConstScopeFindResult res,
                        bool isDeclaration)
        : node_(node), refKind_(refKind), res_(res), isDeclaration_(isDeclaration)
    {
    }

private:
    const ir::AstNode *node_;
    ReferenceKind refKind_;
    varbinder::ConstScopeFindResult res_;
    bool isDeclaration_;
};

class JSLReference : public LReference {
public:
    JSLReference(CodeGen *cg, const ir::AstNode *node, ReferenceKind refKind, varbinder::ConstScopeFindResult res,
                 bool isDeclaration);
    ~JSLReference() = default;
    NO_COPY_SEMANTIC(JSLReference);
    NO_MOVE_SEMANTIC(JSLReference);

    void GetValue() const;
    void SetValue() const;

    static JSLReference Create(CodeGen *cg, const ir::AstNode *node, bool isDeclaration)
    {
        return std::make_from_tuple<JSLReference>(CreateBase(cg, node, isDeclaration));
    }

private:
    PandaGen *pg_;
    VReg obj_;
    VReg privateCtor_ {};
    Operand prop_;
};

class ETSLReference : public LReference {
public:
    ETSLReference(CodeGen *cg, const ir::AstNode *node, ReferenceKind refKind, varbinder::ConstScopeFindResult res,
                  bool isDeclaration);
    ~ETSLReference() = default;
    NO_COPY_SEMANTIC(ETSLReference);
    NO_MOVE_SEMANTIC(ETSLReference);

    void GetValue() const;
    void SetValue() const;

    static ETSLReference Create(CodeGen *const cg, const ir::AstNode *const node, const bool isDeclaration);
    static ReferenceKind ResolveReferenceKind(const varbinder::Variable *variable);

private:
    void SetValueComputed(const ir::MemberExpression *memberExpr) const;
    void SetValueGetterSetter(const ir::MemberExpression *memberExpr) const;

    ETSGen *etsg_;
    const checker::Type *staticObjRef_ {};
    VReg baseReg_ {};
    VReg propReg_ {};
};
}  // namespace ark::es2panda::compiler

#endif
