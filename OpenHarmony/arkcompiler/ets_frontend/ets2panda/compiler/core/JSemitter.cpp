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

#include "JSemitter.h"

#include "compiler/core/pandagen.h"
#include "varbinder/varbinder.h"
#include "parser/program/program.h"
#include "assembly-program.h"
#include "public/public.h"

namespace ark::es2panda::compiler {
pandasm::Function *JSFunctionEmitter::GenFunctionSignature()
{
#ifdef PANDA_WITH_ECMASCRIPT
    auto *func = new pandasm::Function(Cg()->InternalName().Mutf8(), panda_file::SourceLang::ECMASCRIPT);
    GetProgramElement()->SetFunction(func);

    size_t paramCount = Cg()->InternalParamCount();
    func->params.reserve(paramCount);

    for (uint32_t i = 0; i < paramCount; ++i) {
        func->params.emplace_back(pandasm::Type("any", 0), panda_file::SourceLang::ECMASCRIPT);
    }

    func->regsNum = VReg::REG_START - Cg()->TotalRegsNum();
    func->returnType = pandasm::Type("any", 0);

    return func;
#else
    UNREACHABLE();
#endif
}

void JSFunctionEmitter::GenVariableSignature(pandasm::debuginfo::LocalVariable &variableDebug,
                                             [[maybe_unused]] varbinder::LocalVariable *variable) const
{
    variableDebug.signature = "any";
    variableDebug.signatureType = "any";
}

void JSFunctionEmitter::GenFunctionAnnotations(pandasm::Function *func)
{
    pandasm::AnnotationData funcAnnotationData("_ESAnnotation");
    pandasm::AnnotationElement icSizeAnnotationElement(
        "icSize", std::make_unique<pandasm::ScalarValue>(
                      pandasm::ScalarValue::Create<pandasm::Value::Type::U32>(Pg()->IcSize())));
    funcAnnotationData.AddElement(std::move(icSizeAnnotationElement));

    pandasm::AnnotationElement parameterLengthAnnotationElement(
        "parameterLength", std::make_unique<pandasm::ScalarValue>(
                               pandasm::ScalarValue::Create<pandasm::Value::Type::U32>(Pg()->FormalParametersCount())));
    funcAnnotationData.AddElement(std::move(parameterLengthAnnotationElement));

    pandasm::AnnotationElement funcNameAnnotationElement(
        "funcName", std::make_unique<pandasm::ScalarValue>(
                        pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(Pg()->FunctionName().Mutf8())));
    funcAnnotationData.AddElement(std::move(funcNameAnnotationElement));

    func->metadata->AddAnnotations({funcAnnotationData});
}

void JSEmitter::GenAnnotation()
{
#ifdef PANDA_WITH_ECMASCRIPT
    Program()->lang = panda_file::SourceLang::ECMASCRIPT;
    GenESAnnotationRecord();
    GenESModuleModeRecord(Context()->parserProgram->Kind() == parser::ScriptKind::MODULE);
#else
    UNREACHABLE();
#endif
}

void JSEmitter::GenESAnnotationRecord()
{
    auto annotationRecord = pandasm::Record("_ESAnnotation", Program()->lang);
    annotationRecord.metadata->SetAttribute("external");
    annotationRecord.metadata->SetAccessFlags(ACC_ANNOTATION);
    Program()->recordTable.emplace(annotationRecord.name, std::move(annotationRecord));
}

void JSEmitter::GenESModuleModeRecord(bool isModule)
{
    auto modeRecord = pandasm::Record("_ESModuleMode", Program()->lang);
    modeRecord.metadata->SetAccessFlags(ACC_PUBLIC);

    auto modeField = pandasm::Field(Program()->lang);
    modeField.name = "isModule";
    modeField.type = pandasm::Type("u8", 0);
    modeField.metadata->SetValue(
        pandasm::ScalarValue::Create<pandasm::Value::Type::U8>(static_cast<uint8_t>(isModule)));

    modeRecord.fieldList.emplace_back(std::move(modeField));

    Program()->recordTable.emplace(modeRecord.name, std::move(modeRecord));
}
}  // namespace ark::es2panda::compiler
