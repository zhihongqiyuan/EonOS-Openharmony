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

#ifndef ES2PANDA_COMPILER_CORE_REG_ALLOCATOR_H
#define ES2PANDA_COMPILER_CORE_REG_ALLOCATOR_H

#include "generated/isa.h"
#include "compiler/core/regSpiller.h"
#include "macros.h"

namespace ark::es2panda::ir {
class AstNode;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::compiler {
class CodeGen;

class AllocatorBase {
public:
    explicit AllocatorBase(CodeGen *const cg) noexcept;
    NO_COPY_SEMANTIC(AllocatorBase);
    NO_MOVE_SEMANTIC(AllocatorBase);
    ~AllocatorBase() = default;

protected:
    void PushBack(IRNode *ins) const;
    [[nodiscard]] CodeGen &GetCodeGen() noexcept;
    [[nodiscard]] const CodeGen &GetCodeGen() const noexcept;

    template <typename T, typename... Args>
    [[nodiscard]] T *Alloc(const ir::AstNode *const node, Args &&...args)
    {
        return Allocator().New<T>(node, std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    void Add(const ir::AstNode *const node, Args &&...args)
    {
        return PushBack(Alloc<T>(node, std::forward<Args>(args)...));
    }

private:
    [[nodiscard]] ArenaAllocator &Allocator() noexcept;
    [[nodiscard]] const ArenaAllocator &Allocator() const noexcept;

    CodeGen *cg_;
};

class SimpleAllocator final : public AllocatorBase {
public:
    explicit SimpleAllocator(CodeGen *const cg) noexcept;
    NO_COPY_SEMANTIC(SimpleAllocator);
    NO_MOVE_SEMANTIC(SimpleAllocator);
    ~SimpleAllocator() = default;

    Label *AllocLabel(std::string &&id);
    void AddLabel(Label *label) const;

    template <typename T, typename... Args>
    void Emit(const ir::AstNode *const node, Args &&...args)
    {
        Add<T>(node, std::forward<Args>(args)...);
    }
};

class RegAllocatorBase : public AllocatorBase {
public:
    explicit RegAllocatorBase(CodeGen *cg, RegSpiller *spiller) noexcept;
    NO_COPY_SEMANTIC(RegAllocatorBase);
    NO_MOVE_SEMANTIC(RegAllocatorBase);
    ~RegAllocatorBase() = default;

protected:
    [[nodiscard]] RegSpiller &Spiller() noexcept;
    [[nodiscard]] const RegSpiller &Spiller() const noexcept;
    VReg Spill(IRNode *ins, VReg reg) const;
    void Restore(const IRNode *ins) const;

    [[nodiscard]] static std::pair<bool, std::size_t> RegIndicesValid(const IRNode *ins, const Span<VReg *> &registers);

private:
    RegSpiller *spiller_;
};

class RegAllocator final : public RegAllocatorBase {
public:
    explicit RegAllocator(CodeGen *cg, RegSpiller *spiller) noexcept;
    NO_COPY_SEMANTIC(RegAllocator);
    NO_MOVE_SEMANTIC(RegAllocator);
    ~RegAllocator() = default;

    template <typename T, int32_t VALID_VREGS = std::numeric_limits<int32_t>::max(), typename... Args>
    void Emit(const ir::AstNode *const node, Args &&...args)
    {
        auto *const ins = Alloc<T>(node, std::forward<Args>(args)...);
        Run(ins, VALID_VREGS);
    }

private:
    void Run(IRNode *ins, int32_t spillMax);
};

class RangeRegAllocator final : public RegAllocatorBase {
public:
    explicit RangeRegAllocator(CodeGen *cg, RegSpiller *spiller) noexcept;
    NO_COPY_SEMANTIC(RangeRegAllocator);
    NO_MOVE_SEMANTIC(RangeRegAllocator);
    ~RangeRegAllocator() = default;

    template <typename T, typename... Args>
    void Emit(const ir::AstNode *const node, const VReg rangeStart, const std::size_t argCount, Args &&...args)
    {
        auto *const ins = Alloc<T>(node, std::forward<Args>(args)...);
        Run(ins, rangeStart, argCount);
    }

private:
    void Run(IRNode *ins, VReg rangeStart, size_t argCount);
};
}  // namespace ark::es2panda::compiler

#endif
