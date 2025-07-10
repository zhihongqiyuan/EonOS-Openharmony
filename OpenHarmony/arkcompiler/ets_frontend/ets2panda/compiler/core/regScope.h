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

#ifndef ES2PANDA_COMPILER_CORE_REG_SCOPE_H
#define ES2PANDA_COMPILER_CORE_REG_SCOPE_H

#include "macros.h"
#include "varbinder/scope.h"

namespace ark::es2panda::ir {
class AstNode;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::compiler {
class EnvScope;
class PandaGen;
class CodeGen;

class RegScope {
public:
    explicit RegScope(CodeGen *cg);
    NO_COPY_SEMANTIC(RegScope);
    NO_MOVE_SEMANTIC(RegScope);
    ~RegScope();

    void *operator new(size_t) = delete;
    void *operator new[](size_t) = delete;

protected:
    void DebuggerCloseScope();

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    CodeGen *cg_;
    uint32_t regBase_;
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

class LocalRegScope : public RegScope {
public:
    explicit LocalRegScope(PandaGen *pg, varbinder::Scope *scope);
    explicit LocalRegScope(CodeGen *cg, varbinder::Scope *scope);
    explicit LocalRegScope(PandaGen *pg);
    NO_COPY_SEMANTIC(LocalRegScope);
    NO_MOVE_SEMANTIC(LocalRegScope);
    ~LocalRegScope();

    void *operator new(size_t) = delete;
    void *operator new[](size_t) = delete;

private:
    varbinder::Scope *prevScope_ {};
};

class LoopRegScope : public RegScope {
public:
    explicit LoopRegScope(PandaGen *pg, varbinder::LoopScope *scope);
    NO_COPY_SEMANTIC(LoopRegScope);
    NO_MOVE_SEMANTIC(LoopRegScope);
    ~LoopRegScope();

    void *operator new(size_t) = delete;
    void *operator new[](size_t) = delete;

private:
    varbinder::Scope *prevScope_ {};
};

class FunctionRegScope : public RegScope {
public:
    explicit FunctionRegScope(CodeGen *cg);
    explicit FunctionRegScope(PandaGen *pg);
    NO_COPY_SEMANTIC(FunctionRegScope);
    NO_MOVE_SEMANTIC(FunctionRegScope);
    ~FunctionRegScope();

    void *operator new(size_t) = delete;
    void *operator new[](size_t) = delete;

private:
    using StoreParamCb = std::function<void(varbinder::LocalVariable *, VReg)>;
    void InitializeParams(const StoreParamCb &cb);
    EnvScope *envScope_ {};
};
}  // namespace ark::es2panda::compiler

#endif
