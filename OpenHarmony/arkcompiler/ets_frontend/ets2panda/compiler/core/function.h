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

#ifndef ES2PANDA_COMPILER_CORE_FUNCTION_H
#define ES2PANDA_COMPILER_CORE_FUNCTION_H

#include "ir/irnode.h"

namespace ark::es2panda::ir {
class ScriptFunction;
class AstNode;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::compiler {
class PandaGen;

class Function {
public:
    Function() = delete;

    static void IterateOverElements(const ArenaVector<ir::AstNode *> &elements, PandaGen *pg, VReg &ctor, VReg &thisReg,
                                    VReg &computedInstanceFieldsArray);
    static void Compile(PandaGen *pg);
    static void CompileInstanceFields(PandaGen *pg, const ir::ScriptFunction *decl);

    static void LoadClassContexts(const ir::AstNode *node, PandaGen *pg, VReg ctor, const util::StringView &name);
};
}  // namespace ark::es2panda::compiler

#endif
