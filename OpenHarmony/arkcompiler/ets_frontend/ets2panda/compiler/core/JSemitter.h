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

#ifndef ES2PANDA_COMPILER_CORE_JS_EMITTER_H
#define ES2PANDA_COMPILER_CORE_JS_EMITTER_H

#include "emitter.h"

namespace ark::es2panda::compiler {

class JSFunctionEmitter : public FunctionEmitter {
public:
    JSFunctionEmitter(const CodeGen *cg, ProgramElement *programElement) : FunctionEmitter(cg, programElement) {}
    ~JSFunctionEmitter() override = default;
    NO_COPY_SEMANTIC(JSFunctionEmitter);
    NO_MOVE_SEMANTIC(JSFunctionEmitter);

protected:
    const PandaGen *Pg() const
    {
        return reinterpret_cast<const PandaGen *>(Cg());
    }

    pandasm::Function *GenFunctionSignature() override;

    void GenFunctionAnnotations(pandasm::Function *func) override;
    void GenVariableSignature(pandasm::debuginfo::LocalVariable &variableDebug,
                              varbinder::LocalVariable *variable) const override;
};

class JSEmitter : public Emitter {
public:
    explicit JSEmitter(const public_lib::Context *context) : Emitter(context) {}
    ~JSEmitter() override = default;
    NO_COPY_SEMANTIC(JSEmitter);
    NO_MOVE_SEMANTIC(JSEmitter);

    void GenAnnotation() override;

private:
    void GenESAnnotationRecord();
    void GenESModuleModeRecord(bool isModule);
};
}  // namespace ark::es2panda::compiler

#endif
