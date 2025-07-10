/*
 * Copyright (c) 2021 - 2024 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_COMPILER_CORE_CODEGEN_H
#define ES2PANDA_COMPILER_CORE_CODEGEN_H

#include "compiler/base/literals.h"
#include "compiler/core/ASTCompiler.h"
#include "compiler/core/regAllocator.h"
#include "compiler/core/regScope.h"
namespace ark::es2panda::compiler {
class CatchTable;
class DynamicContext;

enum class Constant {
    JS_NAN,
    JS_HOLE,
    JS_INFINITY,
    JS_UNDEFINED,
    JS_NULL,
    JS_TRUE,
    JS_FALSE,
    JS_SYMBOL,
    JS_GLOBAL,
};

class DebugInfo {
public:
    explicit DebugInfo(ArenaAllocator *allocator) : variableDebugInfo_(allocator->Adapter()) {};
    DEFAULT_COPY_SEMANTIC(DebugInfo);
    DEFAULT_MOVE_SEMANTIC(DebugInfo);
    ~DebugInfo() = default;

    ArenaVector<const varbinder::Scope *> &VariableDebugInfo()
    {
        return variableDebugInfo_;
    }

    const ArenaVector<const varbinder::Scope *> &VariableDebugInfo() const
    {
        return variableDebugInfo_;
    }

    const ir::Statement *FirstStatement() const
    {
        return firstStmt_;
    }

private:
    friend class CodeGen;

    ArenaVector<const varbinder::Scope *> variableDebugInfo_;
    const ir::Statement *firstStmt_ {};
};

class CodeGen {
public:
    using TypeMap = ArenaUnorderedMap<VReg, const checker::Type *>;

    explicit CodeGen(ArenaAllocator *allocator, RegSpiller *spiller, public_lib::Context *context,
                     std::tuple<varbinder::FunctionScope *, ProgramElement *, AstCompiler *> toCompile) noexcept
        : astCompiler_(std::get<AstCompiler *>(toCompile)),
          allocator_(allocator),
          context_(context),
          debugInfo_(allocator_),
          topScope_(std::get<varbinder::FunctionScope *>(toCompile)),
          scope_(topScope_),
          rootNode_(scope_->Node()),
          insns_(allocator_->Adapter()),
          catchList_(allocator_->Adapter()),
          typeMap_(allocator_->Adapter()),
          programElement_(std::get<ProgramElement *>(toCompile)),
          sa_(this),
          ra_(this, spiller),
          rra_(this, spiller)
    {
        astCompiler_->SetCodeGen(this);
    }
    virtual ~CodeGen() = default;
    NO_COPY_SEMANTIC(CodeGen);
    NO_MOVE_SEMANTIC(CodeGen);

    [[nodiscard]] virtual IRNode *AllocMov(const ir::AstNode *node, VReg vd, VReg vs) = 0;
    [[nodiscard]] virtual IRNode *AllocMov(const ir::AstNode *node, OutVReg vd, VReg vs) = 0;

    [[nodiscard]] ArenaAllocator *Allocator() const noexcept;
    [[nodiscard]] const ArenaVector<CatchTable *> &CatchList() const noexcept;
    [[nodiscard]] const varbinder::FunctionScope *TopScope() const noexcept;
    [[nodiscard]] const varbinder::Scope *Scope() const noexcept;
    [[nodiscard]] const ir::AstNode *RootNode() const noexcept;

    [[nodiscard]] ArenaVector<IRNode *> &Insns() noexcept;
    [[nodiscard]] const ArenaVector<IRNode *> &Insns() const noexcept;

    [[nodiscard]] VReg AllocReg();
    [[nodiscard]] VReg AllocRegWithType(const checker::Type *type);
    [[nodiscard]] VReg NextReg() const noexcept;

    [[nodiscard]] std::uint32_t TotalRegsNum() const noexcept;
    [[nodiscard]] std::size_t LabelCount() const noexcept;
    [[nodiscard]] const DebugInfo &Debuginfo() const noexcept;
    [[nodiscard]] constexpr std::uint32_t IcSize() const noexcept
    {
        return 0U;
    }

    [[nodiscard]] bool IsDebug() const noexcept;
    [[nodiscard]] std::uint32_t ParamCount() const noexcept;
    [[nodiscard]] std::uint32_t FormalParametersCount() const noexcept;
    [[nodiscard]] std::uint32_t InternalParamCount() const noexcept;
    [[nodiscard]] const util::StringView &InternalName() const noexcept;
    [[nodiscard]] const util::StringView &FunctionName() const noexcept;
    [[nodiscard]] varbinder::VarBinder *VarBinder() const noexcept;

    [[nodiscard]] Label *AllocLabel();
    [[nodiscard]] std::uint32_t AddLiteralBuffer(LiteralBuffer &&buf);

    void LoadAccumulatorString(const ir::AstNode *node, const util::StringView &str);

    void SetLabel(const ir::AstNode *node, Label *label);
    void Branch(const ir::AstNode *node, class Label *label);
    [[nodiscard]] bool CheckControlFlowChange() const;
    Label *ControlFlowChangeBreak(const ir::Identifier *label = nullptr);
    [[nodiscard]] Label *ControlFlowChangeContinue(const ir::Identifier *label);

    uint32_t TryDepth() const;
    [[nodiscard]] CatchTable *CreateCatchTable(util::StringView exceptionType = "");
    [[nodiscard]] CatchTable *CreateCatchTable(LabelPair tryLabelPair, util::StringView exceptionType = "");
    void SortCatchTables();

    void SetFirstStmt(const ir::Statement *stmt) noexcept;

    [[noreturn]] static void Unimplemented();

    void SetVRegType(VReg vreg, const checker::Type *type);

    [[nodiscard]] virtual const checker::Type *GetVRegType(VReg vreg) const;

    [[nodiscard]] public_lib::Context *Context() const noexcept;

    [[nodiscard]] virtual checker::Type const *TypeForVar(varbinder::Variable const *var) const noexcept;

    compiler::AstCompiler *GetAstCompiler() const;

protected:
    [[nodiscard]] SimpleAllocator &Sa() noexcept;
    [[nodiscard]] const SimpleAllocator &Sa() const noexcept;
    [[nodiscard]] RegAllocator &Ra() noexcept;
    [[nodiscard]] const RegAllocator &Ra() const noexcept;
    [[nodiscard]] RangeRegAllocator &Rra() noexcept;
    [[nodiscard]] const RangeRegAllocator &Rra() const noexcept;
    [[nodiscard]] ProgramElement *ProgElement() const noexcept;
    [[nodiscard]] TypeMap &GetTypeMap() noexcept;
    [[nodiscard]] const TypeMap &GetTypeMap() const noexcept;

private:
    AstCompiler *astCompiler_;
    ArenaAllocator *allocator_ {};
    public_lib::Context *context_ {};
    DebugInfo debugInfo_;
    varbinder::FunctionScope *topScope_ {};
    varbinder::Scope *scope_ {};
    const ir::AstNode *rootNode_ {};
    ArenaVector<IRNode *> insns_;
    ArenaVector<CatchTable *> catchList_;
    TypeMap typeMap_;
    ProgramElement *programElement_ {};
    DynamicContext *dynamicContext_ {};

    SimpleAllocator sa_;
    RegAllocator ra_;
    RangeRegAllocator rra_;
    std::size_t labelId_ {0};
    std::uint32_t literalBufferIdx_ {0};

    std::uint32_t usedRegs_ {VReg::REG_START};
    std::uint32_t totalRegs_ {VReg::REG_START};
    friend class ScopeContext;
    friend class RegScope;
    friend class LocalRegScope;
    friend class LoopRegScope;
    friend class ParamRegScope;
    friend class FunctionRegScope;
    friend class DynamicContext;
};
}  // namespace ark::es2panda::compiler

#endif
