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

#ifndef ES2PANDA_COMPILER_CORE_EMITTER_H
#define ES2PANDA_COMPILER_CORE_EMITTER_H

#include "compiler/base/literals.h"

#include "util/ustring.h"

#include <list>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ark::pandasm {
struct Program;
struct Function;
struct Ins;
namespace debuginfo {
struct LocalVariable;
}  // namespace debuginfo
}  // namespace ark::pandasm

namespace ark::es2panda::varbinder {
class Scope;
class LocalVariable;
}  // namespace ark::es2panda::varbinder

namespace ark::es2panda::public_lib {
struct Context;
}  // namespace ark::es2panda::public_lib

namespace ark::es2panda::compiler {
class CodeGen;
class DebugInfo;
class Label;
class IRNode;
class ProgramElement;
class RegSpiller;

class FunctionEmitter {
public:
    explicit FunctionEmitter(const CodeGen *cg, ProgramElement *programElement)
        : cg_(cg), programElement_(programElement)
    {
    }

    virtual ~FunctionEmitter() = default;
    NO_COPY_SEMANTIC(FunctionEmitter);
    NO_MOVE_SEMANTIC(FunctionEmitter);

    void Generate();

protected:
    using VariablesStartsMap = std::unordered_map<const varbinder::Variable *, uint32_t>;

protected:
    virtual pandasm::Function *GenFunctionSignature() = 0;
    virtual void GenFunctionAnnotations(pandasm::Function *func) = 0;
    virtual void GenVariableSignature(pandasm::debuginfo::LocalVariable &variableDebug,
                                      varbinder::LocalVariable *variable) const = 0;

    void GenInstructionDebugInfo(const IRNode *ins, ark::pandasm::Ins *pandaIns);
    void GenFunctionInstructions(pandasm::Function *func);
    void GenScopeVariableInfo(pandasm::Function *func, const varbinder::Scope *scope) const;
    void GenScopeVariableInfoEnd(pandasm::Function *func, const varbinder::Scope *scope, uint32_t count,
                                 uint32_t scopeStart, const VariablesStartsMap &starts) const;
    void GenSourceFileDebugInfo(pandasm::Function *func);
    void GenFunctionCatchTables(ark::pandasm::Function *func);
    void GenVariablesDebugInfo(pandasm::Function *func);
    util::StringView SourceCode() const;

    const CodeGen *Cg() const
    {
        return cg_;
    }

    ProgramElement *GetProgramElement() const
    {
        return programElement_;
    }

private:
    const CodeGen *cg_;
    ProgramElement *programElement_;
    size_t offset_ {0};
};

class Emitter {
public:
    virtual ~Emitter();
    NO_COPY_SEMANTIC(Emitter);
    NO_MOVE_SEMANTIC(Emitter);

    void AddLiteralBuffer(const LiteralBuffer &literals, uint32_t index);
    void AddProgramElement(ProgramElement *programElement);
    static void DumpAsm(const pandasm::Program *prog);
    pandasm::Program *Finalize(bool dumpDebugInfo, std::string_view globalClass = "");

    uint32_t &LiteralBufferIndex()
    {
        return literalBufferIndex_;
    }

    virtual void GenAnnotation() = 0;

protected:
    explicit Emitter(const public_lib::Context *context);

    pandasm::Program *Program() const
    {
        return prog_;
    }

    const public_lib::Context *Context() const
    {
        return context_;
    }

private:
    pandasm::Program *prog_;
    const public_lib::Context *context_;
    uint32_t literalBufferIndex_ {};
};
}  // namespace ark::es2panda::compiler

#endif
