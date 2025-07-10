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

#ifndef ES2PANDA_COMPILER_CORE_JSCOMPILER_H
#define ES2PANDA_COMPILER_CORE_JSCOMPILER_H

#include "compiler/core/ASTCompiler.h"
#include "util/bitset.h"

namespace ark::es2panda::ir {
class AstNode;
}  // namespace ark::es2panda::ir
namespace ark::es2panda::compiler {

class JSCompiler final : public AstCompiler {
public:
    JSCompiler() = default;
/* CC-OFFNXT(G.PRE.02,G.PRE.09) name part*/
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DECLARE_JSCOMPILER_COMPILE_METHOD(_, nodeType) void Compile(const ir::nodeType *node) const override;
    AST_NODE_MAPPING(DECLARE_JSCOMPILER_COMPILE_METHOD)
#undef DECLARE_JSCOMPILER_COMPILE_METHOD
/* CC-OFFNXT(G.PRE.02,G.PRE.09) name part*/
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DECLARE_JSCOMPILER_COMPILE_METHOD(_, __, nodeType, ___) void Compile(const ir::nodeType *node) const override;
    AST_NODE_REINTERPRET_MAPPING(DECLARE_JSCOMPILER_COMPILE_METHOD)
#undef DECLARE_JSCOMPILER_COMPILE_METHOD
    void CompileStaticProperties(compiler::PandaGen *pg, util::BitSet *compiled,
                                 const ir::ObjectExpression *expr) const;
    void CompileRemainingProperties(compiler::PandaGen *pg, const util::BitSet *compiled,
                                    const ir::ObjectExpression *expr) const;

private:
    PandaGen *GetPandaGen() const;
};

}  // namespace ark::es2panda::compiler

#endif  // ES2PANDA_COMPILER_CORE_JSCOMPILER_H
