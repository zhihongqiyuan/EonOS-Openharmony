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

#include "pandagen.h"

#include "varbinder/varbinder.h"
#include "checker/checker.h"
#include "checker/types/globalTypesHolder.h"
#include "util/helpers.h"
#include "varbinder/scope.h"
#include "varbinder/variable.h"
#include "compiler/base/catchTable.h"
#include "compiler/base/lexenv.h"
#include "compiler/base/literals.h"
#include "public/public.h"
#include "compiler/core/labelTarget.h"
#include "compiler/core/regAllocator.h"
#include "compiler/function/asyncFunctionBuilder.h"
#include "compiler/function/asyncGeneratorFunctionBuilder.h"
#include "compiler/function/functionBuilder.h"
#include "compiler/function/generatorFunctionBuilder.h"
#include "es2panda.h"
#include "generated/isa.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/spreadElement.h"
#include "ir/statement.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/literals/numberLiteral.h"
#include "ir/expressions/literals/stringLiteral.h"

namespace ark::es2panda::compiler {

#ifndef PANDA_WITH_ECMASCRIPT
class EcmaDisabled : public IRNode {
public:
    template <typename... Args>
    explicit EcmaDisabled(const ir::AstNode *node, [[maybe_unused]] Args &&...args) : IRNode(node)
    {
        UNREACHABLE();
    }

    Formats GetFormats() const override
    {
        UNREACHABLE();
    }

    size_t Registers([[maybe_unused]] std::array<VReg *, MAX_REG_OPERAND> *regs) override
    {
        UNREACHABLE();
    }

    size_t Registers([[maybe_unused]] std::array<const VReg *, MAX_REG_OPERAND> *regs) const override
    {
        UNREACHABLE();
    }

    size_t OutRegisters([[maybe_unused]] std::array<OutVReg, MAX_REG_OPERAND> *regs) const override
    {
        UNREACHABLE();
    }

    void Transform([[maybe_unused]] pandasm::Ins *ins, [[maybe_unused]] ProgramElement *programElement,
                   [[maybe_unused]] uint32_t totalRegs) const override
    {
        UNREACHABLE();
    }
};

using EcmaLdhole = EcmaDisabled;
using EcmaLdnan = EcmaDisabled;
using EcmaLdinfinity = EcmaDisabled;
using EcmaLdglobal = EcmaDisabled;
using EcmaLdundefined = EcmaDisabled;
using EcmaLdsymbol = EcmaDisabled;
using EcmaLdnull = EcmaDisabled;
using EcmaLdtrue = EcmaDisabled;
using EcmaLdfalse = EcmaDisabled;
using EcmaTryldglobalbyname = EcmaDisabled;
using EcmaTrystglobalbyname = EcmaDisabled;
using EcmaLdobjbyname = EcmaDisabled;
using EcmaStobjbyname = EcmaDisabled;
using EcmaLdobjbyindex = EcmaDisabled;
using EcmaLdobjbyvalue = EcmaDisabled;
using EcmaStobjbyvalue = EcmaDisabled;
using EcmaStobjbyindex = EcmaDisabled;
using EcmaStownbyname = EcmaDisabled;
using EcmaStownbyvalue = EcmaDisabled;
using EcmaStownbyindex = EcmaDisabled;
using EcmaDelobjprop = EcmaDisabled;
using EcmaLdglobalvar = EcmaDisabled;
using EcmaStglobalvar = EcmaDisabled;
using EcmaLdbigint = EcmaDisabled;
using EcmaEqdyn = EcmaDisabled;
using EcmaNoteqdyn = EcmaDisabled;
using EcmaStricteqdyn = EcmaDisabled;
using EcmaStrictnoteqdyn = EcmaDisabled;
using EcmaLessdyn = EcmaDisabled;
using EcmaLesseqdyn = EcmaDisabled;
using EcmaGreaterdyn = EcmaDisabled;
using EcmaGreatereqdyn = EcmaDisabled;
using EcmaTonumber = EcmaDisabled;
using EcmaNegdyn = EcmaDisabled;
using EcmaNotdyn = EcmaDisabled;
using EcmaNegate = EcmaDisabled;
using EcmaIncdyn = EcmaDisabled;
using EcmaDecdyn = EcmaDisabled;
using EcmaEqdyn = EcmaDisabled;
using EcmaNoteqdyn = EcmaDisabled;
using EcmaStricteqdyn = EcmaDisabled;
using EcmaStrictnoteqdyn = EcmaDisabled;
using EcmaLessdyn = EcmaDisabled;
using EcmaLesseqdyn = EcmaDisabled;
using EcmaGreaterdyn = EcmaDisabled;
using EcmaGreatereqdyn = EcmaDisabled;
using EcmaAdd2dyn = EcmaDisabled;
using EcmaSub2dyn = EcmaDisabled;
using EcmaMul2dyn = EcmaDisabled;
using EcmaDiv2dyn = EcmaDisabled;
using EcmaMod2dyn = EcmaDisabled;
using EcmaExpdyn = EcmaDisabled;
using EcmaShl2dyn = EcmaDisabled;
using EcmaShr2dyn = EcmaDisabled;
using EcmaAshr2dyn = EcmaDisabled;
using EcmaAnd2dyn = EcmaDisabled;
using EcmaOr2dyn = EcmaDisabled;
using EcmaXor2dyn = EcmaDisabled;
using EcmaIsindyn = EcmaDisabled;
using EcmaInstanceofdyn = EcmaDisabled;
using EcmaIsundefined = EcmaDisabled;
using EcmaIsundefined = EcmaDisabled;
using EcmaJtrue = EcmaDisabled;
using EcmaIstrue = EcmaDisabled;
using EcmaJfalse = EcmaDisabled;
using EcmaIscoercible = EcmaDisabled;
using EcmaThrowdyn = EcmaDisabled;
using EcmaRethrowdyn = EcmaDisabled;
using EcmaReturnDyn = EcmaDisabled;
using EcmaReturnundefined = EcmaDisabled;
using EcmaCall0thisdyn = EcmaDisabled;
using EcmaCall1thisdyn = EcmaDisabled;
using EcmaCall0dyn = EcmaDisabled;
using EcmaCall1thisdyn = EcmaDisabled;
using EcmaCall1dyn = EcmaDisabled;
using EcmaCall2thisdyn = EcmaDisabled;
using EcmaCall2dyn = EcmaDisabled;
using EcmaCall3thisdyn = EcmaDisabled;
using EcmaCall3dyn = EcmaDisabled;
using EcmaCallithisrangedyn = EcmaDisabled;
using EcmaCallirangedyn = EcmaDisabled;
using EcmaCall1thisdyn = EcmaDisabled;
using EcmaCall1dyn = EcmaDisabled;
using EcmaCall2thisdyn = EcmaDisabled;
using EcmaCall2dyn = EcmaDisabled;
using EcmaCall3thisdyn = EcmaDisabled;
using EcmaCall3dyn = EcmaDisabled;
using EcmaCallithisrangedyn = EcmaDisabled;
using EcmaCallirangedyn = EcmaDisabled;
using EcmaSupercall = EcmaDisabled;
using EcmaSupercallspread = EcmaDisabled;
using EcmaNewobjdynrange = EcmaDisabled;
using EcmaLdhomeobject = EcmaDisabled;
using EcmaDefinemethod = EcmaDisabled;
using EcmaDefineasyncgeneratorfunc = EcmaDisabled;
using EcmaDefineasyncfunc = EcmaDisabled;
using EcmaDefinegeneratorfunc = EcmaDisabled;
using EcmaDefinencfuncdyn = EcmaDisabled;
using EcmaDefinefuncdyn = EcmaDisabled;
using EcmaTypeofdyn = EcmaDisabled;
using EcmaCallspreaddyn = EcmaDisabled;
using EcmaNewobjspreaddyn = EcmaDisabled;
using EcmaGetunmappedargs = EcmaDisabled;
using EcmaNegate = EcmaDisabled;
using EcmaToboolean = EcmaDisabled;
using EcmaTonumber = EcmaDisabled;
using EcmaGetmethod = EcmaDisabled;
using EcmaCreategeneratorobj = EcmaDisabled;
using EcmaCreateasyncgeneratorobj = EcmaDisabled;
using EcmaCreateiterresultobj = EcmaDisabled;
using EcmaSuspendgenerator = EcmaDisabled;
using EcmaSuspendasyncgenerator = EcmaDisabled;
using EcmaSetgeneratorstate = EcmaDisabled;
using EcmaSetgeneratorstate = EcmaDisabled;
using EcmaResumegenerator = EcmaDisabled;
using EcmaGetresumemode = EcmaDisabled;
using EcmaAsyncfunctionenter = EcmaDisabled;
using EcmaAsyncfunctionawait = EcmaDisabled;
using EcmaAsyncfunctionresolve = EcmaDisabled;
using EcmaAsyncfunctionreject = EcmaDisabled;
using EcmaAsyncgeneratorresolve = EcmaDisabled;
using EcmaAsyncgeneratorreject = EcmaDisabled;
using EcmaGettemplateobject = EcmaDisabled;
using EcmaCopyrestargs = EcmaDisabled;
using EcmaGetpropiterator = EcmaDisabled;
using EcmaGetnextpropname = EcmaDisabled;
using EcmaCreateemptyobject = EcmaDisabled;
using EcmaCreateobjectwithbuffer = EcmaDisabled;
using EcmaCreateobjecthavingmethod = EcmaDisabled;
using EcmaSetobjectwithproto = EcmaDisabled;
using EcmaCopydataproperties = EcmaDisabled;
using EcmaDefinegettersetterbyvalue = EcmaDisabled;
using EcmaCreateemptyarray = EcmaDisabled;
using EcmaCreatearraywithbuffer = EcmaDisabled;
using EcmaStarrayspread = EcmaDisabled;
using EcmaCreateregexpwithliteral = EcmaDisabled;
using EcmaThrowifnotobject = EcmaDisabled;
using EcmaThrowthrownotexists = EcmaDisabled;
using EcmaGetiterator = EcmaDisabled;
using EcmaGetasynciterator = EcmaDisabled;
using EcmaCreateobjectwithexcludedkeys = EcmaDisabled;
using EcmaThrowpatternnoncoercible = EcmaDisabled;
using EcmaCloseiterator = EcmaDisabled;
using EcmaImportmodule = EcmaDisabled;
using EcmaSetclasscomputedfields = EcmaDisabled;
using EcmaDefineclasswithbuffer = EcmaDisabled;
using EcmaLoadclasscomputedinstancefields = EcmaDisabled;
using EcmaDefineclassprivatefields = EcmaDisabled;
using EcmaClassfieldadd = EcmaDisabled;
using EcmaClassprivatefieldadd = EcmaDisabled;
using EcmaClassprivatemethodoraccessoradd = EcmaDisabled;
using EcmaClassprivatefieldget = EcmaDisabled;
using EcmaClassprivatefieldset = EcmaDisabled;
using EcmaClassprivatefieldin = EcmaDisabled;
using EcmaLdmodvarbyname = EcmaDisabled;
using EcmaStmodulevar = EcmaDisabled;
using EcmaCopymodule = EcmaDisabled;
using EcmaStsuperbyname = EcmaDisabled;
using EcmaLdsuperbyname = EcmaDisabled;
using EcmaStsuperbyvalue = EcmaDisabled;
using EcmaLdsuperbyvalue = EcmaDisabled;
using EcmaLdlexvardyn = EcmaDisabled;
using EcmaLdlexdyn = EcmaDisabled;
using EcmaStlexvardyn = EcmaDisabled;
using EcmaStlexdyn = EcmaDisabled;
using EcmaThrowifsupernotcorrectcall = EcmaDisabled;
using EcmaThrowtdz = EcmaDisabled;
using EcmaThrowconstassignment = EcmaDisabled;
using EcmaPoplexenvdyn = EcmaDisabled;
using EcmaCopylexenvdyn = EcmaDisabled;
using EcmaNewlexenvdyn = EcmaDisabled;
using EcmaLdlexenvdyn = EcmaDisabled;
using EcmaLdevalvar = EcmaDisabled;
using EcmaStevalvar = EcmaDisabled;
using EcmaLdevalbindings = EcmaDisabled;
using EcmaDirecteval = EcmaDisabled;
#endif

PandaGen::PandaGen(ArenaAllocator *const allocator, RegSpiller *const spiller, public_lib::Context *const context,
                   std::tuple<varbinder::FunctionScope *, ProgramElement *, AstCompiler *> toCompile)
    : CodeGen(allocator, spiller, context, toCompile)
{
    Function::Compile(this);
}

FunctionBuilder *PandaGen::FuncBuilder() const noexcept
{
    return builder_;
}

EnvScope *PandaGen::GetEnvScope() const noexcept
{
    return envScope_;
}

void PandaGen::OptionalChainCheck(const bool optional, const VReg obj) const
{
    if (optional && optionalChain_ != nullptr) {
        optionalChain_->Check(obj);
    }
}

void PandaGen::FunctionInit(CatchTable *catchTable)
{
    if (RootNode()->IsProgram()) {
        builder_ = Allocator()->New<FunctionBuilder>(this, catchTable);
        return;
    }

    const ir::ScriptFunction *func = RootNode()->AsScriptFunction();

    if (func->IsAsyncFunc()) {
        if (func->IsGenerator()) {
            builder_ = Allocator()->New<AsyncGeneratorFunctionBuilder>(this, catchTable);
            return;
        }

        builder_ = Allocator()->New<AsyncFunctionBuilder>(this, catchTable);
        return;
    }

    if (func->IsGenerator()) {
        builder_ = Allocator()->New<GeneratorFunctionBuilder>(this, catchTable);
        return;
    }

    builder_ = Allocator()->New<FunctionBuilder>(this, catchTable);
}

bool PandaGen::FunctionHasFinalizer() const
{
    if (RootNode()->IsProgram()) {
        return false;
    }

    const ir::ScriptFunction *func = RootNode()->AsScriptFunction();

    return func->IsAsyncFunc() || func->IsGenerator();
}

void PandaGen::FunctionEnter()
{
    builder_->Prepare(RootNode()->AsScriptFunction());
}

void PandaGen::FunctionExit()
{
    builder_->CleanUp(RootNode()->AsScriptFunction());
}

void PandaGen::StoreAccumulator(const ir::AstNode *node, VReg vreg)
{
    Ra().Emit<StaDyn>(node, vreg);
}

void PandaGen::LoadAccumulator(const ir::AstNode *node, VReg reg)
{
    Ra().Emit<LdaDyn>(node, reg);
}

IRNode *PandaGen::AllocMov(const ir::AstNode *node, const VReg vd, const VReg vs)
{
    return Allocator()->New<MovDyn>(node, vd, vs);
}

IRNode *PandaGen::AllocMov(const ir::AstNode *node, OutVReg vd, const VReg vs)
{
    ASSERT(vd.type == OperandType::ANY);
    return Allocator()->New<MovDyn>(node, *vd.reg, vs);
}

void PandaGen::MoveVreg(const ir::AstNode *node, VReg vd, VReg vs)
{
    Ra().Emit<MovDyn>(node, vd, vs);
}

void PandaGen::LoadAccumulatorDouble(const ir::AstNode *node, double num)
{
    Sa().Emit<FldaiDyn>(node, num);
}

void PandaGen::LoadAccumulatorInt(const ir::AstNode *node, size_t num)
{
    Sa().Emit<LdaiDyn>(node, static_cast<int64_t>(num));
}

void PandaGen::StoreConst(const ir::AstNode *node, VReg reg, Constant id)
{
    LoadConst(node, id);
    StoreAccumulator(node, reg);
}

void PandaGen::LoadConst(const ir::AstNode *node, Constant id)
{
    switch (id) {
        case Constant::JS_HOLE: {
            Sa().Emit<EcmaLdhole>(node);
            break;
        }
        case Constant::JS_NAN: {
            Sa().Emit<EcmaLdnan>(node);
            break;
        }
        case Constant::JS_INFINITY: {
            Sa().Emit<EcmaLdinfinity>(node);
            break;
        }
        case Constant::JS_GLOBAL: {
            Sa().Emit<EcmaLdglobal>(node);
            break;
        }
        case Constant::JS_UNDEFINED: {
            Sa().Emit<EcmaLdundefined>(node);
            break;
        }
        case Constant::JS_SYMBOL: {
            Sa().Emit<EcmaLdsymbol>(node);
            break;
        }
        case Constant::JS_NULL: {
            Sa().Emit<EcmaLdnull>(node);
            break;
        }
        case Constant::JS_TRUE: {
            Sa().Emit<EcmaLdtrue>(node);
            break;
        }
        case Constant::JS_FALSE: {
            Sa().Emit<EcmaLdfalse>(node);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void PandaGen::GetFunctionObject(const ir::AstNode *node)
{
    LoadAccFromLexEnv(node, Scope()->Find(varbinder::VarBinder::MANDATORY_PARAM_FUNC));
}

void PandaGen::GetNewTarget(const ir::AstNode *node)
{
    LoadAccFromLexEnv(node, Scope()->Find(varbinder::VarBinder::MANDATORY_PARAM_NEW_TARGET));
}

void PandaGen::GetThis(const ir::AstNode *node)
{
    LoadAccFromLexEnv(node, Scope()->Find(varbinder::VarBinder::MANDATORY_PARAM_THIS));
}

void PandaGen::SetThis(const ir::AstNode *node)
{
    StoreAccToLexEnv(node, Scope()->Find(varbinder::VarBinder::MANDATORY_PARAM_THIS), true);
}

void PandaGen::LoadVar(const ir::Identifier *node, const varbinder::ConstScopeFindResult &result)
{
    auto *var = result.variable;

    if (var == nullptr) {
        TryLoadGlobalByName(node, result.name);
        return;
    }

    if (var->IsGlobalVariable()) {
        LoadGlobalVar(node, var->Name());
        return;
    }

    if (var->IsModuleVariable()) {
        LoadModuleVariable(node, var->AsModuleVariable()->ModuleReg(), var->AsModuleVariable()->ExoticName());
        return;
    }

    ASSERT(var->IsLocalVariable());
    LoadAccFromLexEnv(node, result);
}

void PandaGen::StoreVar(const ir::AstNode *node, const varbinder::ConstScopeFindResult &result, bool isDeclaration)
{
    varbinder::Variable *var = result.variable;

    if (var == nullptr) {
        if (IsDirectEval()) {
            StoreEvalVariable(node, result.name);
        } else {
            TryStoreGlobalByName(node, result.name);
        }
        return;
    }

    if (var->IsGlobalVariable()) {
        StoreGlobalVar(node, var->Name());
        return;
    }

    if (var->IsModuleVariable()) {
        ThrowConstAssignment(node, var->Name());
        return;
    }

    ASSERT(var->IsLocalVariable());
    StoreAccToLexEnv(node, result, isDeclaration);
}

void PandaGen::LoadAccFromArgs(const ir::AstNode *node)
{
    if (!Scope()->HasFlag(varbinder::ScopeFlags::USE_ARGS)) {
        return;
    }

    auto res = Scope()->Find(varbinder::VarBinder::FUNCTION_ARGUMENTS);
    ASSERT(res.scope);

    GetUnmappedArgs(node);
    StoreAccToLexEnv(node, res, true);
}

void PandaGen::LoadObjProperty(const ir::AstNode *node, const Operand &prop)
{
    if (std::holds_alternative<VReg>(prop)) {
        LoadObjByValue(node, std::get<VReg>(prop));
        return;
    }

    if (std::holds_alternative<int64_t>(prop)) {
        LoadObjByIndex(node, std::get<int64_t>(prop));
        return;
    }

    ASSERT(std::holds_alternative<util::StringView>(prop));
    LoadObjByName(node, std::get<util::StringView>(prop));
}

void PandaGen::StoreObjProperty(const ir::AstNode *node, VReg obj, const Operand &prop)
{
    if (std::holds_alternative<VReg>(prop)) {
        StoreObjByValue(node, obj, std::get<VReg>(prop));
        return;
    }

    if (std::holds_alternative<int64_t>(prop)) {
        StoreObjByIndex(node, obj, std::get<int64_t>(prop));
        return;
    }

    ASSERT(std::holds_alternative<util::StringView>(prop));
    StoreObjByName(node, obj, std::get<util::StringView>(prop));
}

void PandaGen::StoreOwnProperty(const ir::AstNode *node, VReg obj, const Operand &prop)
{
    if (std::holds_alternative<VReg>(prop)) {
        StOwnByValue(node, obj, std::get<VReg>(prop));
        return;
    }

    if (std::holds_alternative<int64_t>(prop)) {
        StOwnByIndex(node, obj, std::get<int64_t>(prop));
        return;
    }

    ASSERT(std::holds_alternative<util::StringView>(prop));
    StOwnByName(node, obj, std::get<util::StringView>(prop));
}

void PandaGen::TryLoadGlobalByName(const ir::AstNode *node, const util::StringView &name)
{
    Sa().Emit<EcmaTryldglobalbyname>(node, name);
}

void PandaGen::TryStoreGlobalByName(const ir::AstNode *node, const util::StringView &name)
{
    Sa().Emit<EcmaTrystglobalbyname>(node, name);
}

void PandaGen::LoadObjByName(const ir::AstNode *node, const util::StringView &prop)
{
    Ra().Emit<EcmaLdobjbyname>(node, prop);
}

void PandaGen::StoreObjByName(const ir::AstNode *node, VReg obj, const util::StringView &prop)
{
    Ra().Emit<EcmaStobjbyname>(node, prop, obj);
}

void PandaGen::LoadObjByIndex(const ir::AstNode *node, int64_t index)
{
    Ra().Emit<EcmaLdobjbyindex>(node, index);
}

void PandaGen::LoadObjByValue(const ir::AstNode *node, VReg obj)
{
    Ra().Emit<EcmaLdobjbyvalue>(node, obj);
}

void PandaGen::StoreObjByValue(const ir::AstNode *node, VReg obj, VReg prop)
{
    Ra().Emit<EcmaStobjbyvalue>(node, obj, prop);
}

void PandaGen::StoreObjByIndex(const ir::AstNode *node, VReg obj, int64_t index)
{
    Ra().Emit<EcmaStobjbyindex>(node, index, obj);
}

void PandaGen::StOwnByName(const ir::AstNode *node, VReg obj, const util::StringView &prop)
{
    Ra().Emit<EcmaStownbyname>(node, prop, obj);
}

void PandaGen::StOwnByValue(const ir::AstNode *node, VReg obj, VReg prop)
{
    Ra().Emit<EcmaStownbyvalue>(node, obj, prop);
}

void PandaGen::StOwnByIndex(const ir::AstNode *node, VReg obj, int64_t index)
{
    Ra().Emit<EcmaStownbyindex>(node, index, obj);
}

void PandaGen::DeleteObjProperty(const ir::AstNode *node, VReg obj, VReg prop)
{
    Ra().Emit<EcmaDelobjprop>(node, obj, prop);
}

void PandaGen::LoadGlobalVar(const ir::AstNode *node, const util::StringView &name)
{
    Sa().Emit<EcmaLdglobalvar>(node, name);
}

void PandaGen::StoreGlobalVar(const ir::AstNode *node, const util::StringView &name)
{
    Sa().Emit<EcmaStglobalvar>(node, name);
}

VReg PandaGen::LexEnv() const noexcept
{
    return envScope_->LexEnv();
}

void PandaGen::LoadAccFromLexEnv(const ir::AstNode *node, const varbinder::ConstScopeFindResult &result)
{
    VirtualLoadVar::Expand(this, node, result);
}

void PandaGen::StoreAccToLexEnv(const ir::AstNode *node, const varbinder::ConstScopeFindResult &result,
                                bool isDeclaration)
{
    VirtualStoreVar::Expand(this, node, result, isDeclaration);
}

void PandaGen::LoadAccumulatorBigInt(const ir::AstNode *node, const util::StringView &bigInt)
{
    Sa().Emit<EcmaLdbigint>(node, bigInt);
}

void PandaGen::Condition(const ir::AstNode *node, lexer::TokenType op, VReg lhs, Label *ifFalse)
{
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_EQUAL: {
            Ra().Emit<EcmaEqdyn>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_NOT_EQUAL: {
            Ra().Emit<EcmaNoteqdyn>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_STRICT_EQUAL: {
            Ra().Emit<EcmaStricteqdyn>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_NOT_STRICT_EQUAL: {
            Ra().Emit<EcmaStrictnoteqdyn>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_LESS_THAN: {
            Ra().Emit<EcmaLessdyn>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL: {
            Ra().Emit<EcmaLesseqdyn>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN: {
            Ra().Emit<EcmaGreaterdyn>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL: {
            Ra().Emit<EcmaGreatereqdyn>(node, lhs);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    BranchIfFalse(node, ifFalse);
}

void PandaGen::Unary(const ir::AstNode *node, lexer::TokenType op, VReg operand)
{
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_PLUS: {
            Ra().Emit<EcmaTonumber>(node, operand);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_MINUS: {
            Ra().Emit<EcmaNegdyn>(node, operand);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_TILDE: {
            Ra().Emit<EcmaNotdyn>(node, operand);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_EXCLAMATION_MARK: {
            Sa().Emit<EcmaNegate>(node);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_PLUS_PLUS: {
            Ra().Emit<EcmaIncdyn>(node, operand);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_MINUS_MINUS: {
            Ra().Emit<EcmaDecdyn>(node, operand);
            break;
        }
        case lexer::TokenType::KEYW_VOID:
        case lexer::TokenType::KEYW_DELETE: {
            LoadConst(node, Constant::JS_UNDEFINED);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

// CC-OFFNXT(huge_method,huge_cyclomatic_complexity,G.FUN.01-CPP) big switch-case, solid logic
void PandaGen::Binary(const ir::AstNode *node, lexer::TokenType op, VReg lhs)
{
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_EQUAL:
            return Ra().Emit<EcmaEqdyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_NOT_EQUAL:
            return Ra().Emit<EcmaNoteqdyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_STRICT_EQUAL:
            return Ra().Emit<EcmaStricteqdyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_NOT_STRICT_EQUAL:
            return Ra().Emit<EcmaStrictnoteqdyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_LESS_THAN:
            return Ra().Emit<EcmaLessdyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL:
            return Ra().Emit<EcmaLesseqdyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN:
            return Ra().Emit<EcmaGreaterdyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL:
            return Ra().Emit<EcmaGreatereqdyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_PLUS:
        case lexer::TokenType::PUNCTUATOR_PLUS_EQUAL:
            return Ra().Emit<EcmaAdd2dyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_MINUS:
        case lexer::TokenType::PUNCTUATOR_MINUS_EQUAL:
            return Ra().Emit<EcmaSub2dyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_MULTIPLY:
        case lexer::TokenType::PUNCTUATOR_MULTIPLY_EQUAL:
            return Ra().Emit<EcmaMul2dyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_DIVIDE:
        case lexer::TokenType::PUNCTUATOR_DIVIDE_EQUAL:
            return Ra().Emit<EcmaDiv2dyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_MOD:
        case lexer::TokenType::PUNCTUATOR_MOD_EQUAL:
            return Ra().Emit<EcmaMod2dyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_EXPONENTIATION_EQUAL:
        case lexer::TokenType::PUNCTUATOR_EXPONENTIATION:
            return Ra().Emit<EcmaExpdyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_LEFT_SHIFT:
        case lexer::TokenType::PUNCTUATOR_LEFT_SHIFT_EQUAL:
            return Ra().Emit<EcmaShl2dyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_RIGHT_SHIFT:
        case lexer::TokenType::PUNCTUATOR_RIGHT_SHIFT_EQUAL:
            return Ra().Emit<EcmaShr2dyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT:
        case lexer::TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT_EQUAL:
            return Ra().Emit<EcmaAshr2dyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_BITWISE_AND:
        case lexer::TokenType::PUNCTUATOR_BITWISE_AND_EQUAL:
            return Ra().Emit<EcmaAnd2dyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_BITWISE_OR:
        case lexer::TokenType::PUNCTUATOR_BITWISE_OR_EQUAL:
            return Ra().Emit<EcmaOr2dyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_BITWISE_XOR:
        case lexer::TokenType::PUNCTUATOR_BITWISE_XOR_EQUAL:
            return Ra().Emit<EcmaXor2dyn>(node, lhs);
        case lexer::TokenType::KEYW_IN:
            return Ra().Emit<EcmaIsindyn>(node, lhs);
        case lexer::TokenType::KEYW_INSTANCEOF:
            return Ra().Emit<EcmaInstanceofdyn>(node, lhs);
        case lexer::TokenType::PUNCTUATOR_NULLISH_COALESCING:
        case lexer::TokenType::PUNCTUATOR_LOGICAL_NULLISH_EQUAL:
            return Unimplemented();
        default:
            UNREACHABLE();
    }
}

void PandaGen::BranchIfUndefined(const ir::AstNode *node, Label *target)
{
    Sa().Emit<EcmaIsundefined>(node);
    BranchIfTrue(node, target);
}

void PandaGen::BranchIfNotUndefined(const ir::AstNode *node, Label *target)
{
    Sa().Emit<EcmaIsundefined>(node);
    BranchIfFalse(node, target);
}

void PandaGen::BranchIfTrue(const ir::AstNode *node, Label *target)
{
    Sa().Emit<EcmaJtrue>(node, target);
}

void PandaGen::BranchIfNotTrue(const ir::AstNode *node, Label *target)
{
    Sa().Emit<EcmaIstrue>(node);
    BranchIfFalse(node, target);
}

void PandaGen::BranchIfFalse(const ir::AstNode *node, Label *target)
{
    Sa().Emit<EcmaJfalse>(node, target);
}

void PandaGen::BranchIfCoercible(const ir::AstNode *node, Label *target)
{
    Sa().Emit<EcmaIscoercible>(node);
    BranchIfTrue(node, target);
}

void PandaGen::EmitThrow(const ir::AstNode *node)
{
    Sa().Emit<EcmaThrowdyn>(node);
}

void PandaGen::EmitRethrow(const ir::AstNode *node)
{
    Sa().Emit<EcmaRethrowdyn>(node);
}

void PandaGen::EmitReturn(const ir::AstNode *node)
{
    Sa().Emit<EcmaReturnDyn>(node);
}

void PandaGen::EmitReturnUndefined(const ir::AstNode *node)
{
    Sa().Emit<EcmaReturnundefined>(node);
}

void PandaGen::ImplicitReturn(const ir::AstNode *node)
{
    builder_->ImplicitReturn(node);
}

void PandaGen::DirectReturn(const ir::AstNode *node)
{
    builder_->DirectReturn(node);
}

void PandaGen::ValidateClassDirectReturn(const ir::AstNode *node)
{
    const ir::ScriptFunction *func = util::Helpers::GetContainingFunction(node);

    if (func == nullptr || !func->IsConstructor()) {
        return;
    }

    RegScope rs(this);
    VReg value = AllocReg();
    StoreAccumulator(node, value);

    auto *notUndefined = AllocLabel();
    auto *condEnd = AllocLabel();

    BranchIfNotUndefined(node, notUndefined);
    GetThis(func);
    ThrowIfSuperNotCorrectCall(func, 0);
    Branch(node, condEnd);

    SetLabel(node, notUndefined);
    LoadAccumulator(node, value);

    SetLabel(node, condEnd);
}

void PandaGen::EmitAwait(const ir::AstNode *node)
{
    builder_->Await(node);
}

void PandaGen::Call0This(const ir::AstNode *node, VReg callee, VReg thisReg)
{
    LoadAccumulator(node, thisReg);
    Ra().Emit<EcmaCall0thisdyn>(node, callee);
}

void PandaGen::Call1This(const ir::AstNode *node, VReg callee, VReg thisReg, VReg arg0)
{
    LoadAccumulator(node, arg0);
    Ra().Emit<EcmaCall1thisdyn>(node, callee, thisReg);
}

void PandaGen::Call0Args(const ir::AstNode *n, VReg c, VReg thisR, bool hasThis)
{
    if (hasThis) {
        Call0This(n, c, thisR);
    } else {
        Sa().Emit<EcmaCall0dyn>(n);
    }
}

void PandaGen::Call1Arg(const ir::AstNode *n, VReg c, VReg thisR, const ArenaVector<ir::Expression *> &args,
                        bool hasThis)
{
    const auto *arg0 = args[0];
    arg0->Compile(this);

    if (hasThis) {
        Ra().Emit<EcmaCall1thisdyn>(n, c, thisR);
    } else {
        Ra().Emit<EcmaCall1dyn>(n, c);
    }
}

void PandaGen::Call2Args(const ir::AstNode *n, VReg c, VReg thisR, const ArenaVector<ir::Expression *> &args,
                         bool hasThis)
{
    const auto *arg0 = args[0];
    arg0->Compile(this);
    compiler::VReg arg0Reg = AllocReg();
    StoreAccumulator(arg0, arg0Reg);

    const auto *arg1 = args[1];
    arg1->Compile(this);

    if (hasThis) {
        Ra().Emit<EcmaCall2thisdyn>(n, c, thisR, arg0Reg);
    } else {
        Ra().Emit<EcmaCall2dyn>(n, c, arg0Reg);
    }
}

void PandaGen::Call3Args(const ir::AstNode *n, VReg c, VReg thisR, const ArenaVector<ir::Expression *> &args,
                         bool hasThis)
{
    const auto *arg0 = args[0];
    arg0->Compile(this);
    compiler::VReg arg0Reg = AllocReg();
    StoreAccumulator(arg0, arg0Reg);

    const auto *arg1 = args[1];
    arg1->Compile(this);
    compiler::VReg arg1Reg = AllocReg();
    StoreAccumulator(arg1, arg1Reg);

    const auto *arg2 = args[2];
    arg2->Compile(this);

    if (hasThis) {
        Ra().Emit<EcmaCall3thisdyn>(n, c, thisR, arg0Reg, arg1Reg);
    } else {
        Ra().Emit<EcmaCall3dyn>(n, c, arg0Reg, arg1Reg);
    }
}

void PandaGen::Call(const ir::AstNode *node, VReg callee, VReg thisReg, const ArenaVector<ir::Expression *> &arguments)
{
    bool hasThis = !thisReg.IsInvalid();

    switch (arguments.size()) {
        case 0: {  // 0 args
            Call0Args(node, callee, thisReg, hasThis);
            return;
        }
        case 1: {  // 1 arg
            Call1Arg(node, callee, thisReg, arguments, hasThis);
            return;
        }
        case 2: {  // 2 args
            Call2Args(node, callee, thisReg, arguments, hasThis);
            return;
        }
        case 3: {  // 3 args
            Call3Args(node, callee, thisReg, arguments, hasThis);
            return;
        }
        default: {
            break;
        }
    }

    for (const auto *it : arguments) {
        it->Compile(this);
        compiler::VReg arg = AllocReg();
        StoreAccumulator(it, arg);
    }

    if (hasThis) {
        size_t argCount = arguments.size() + 1;
        auto constexpr EXTRA_ARGS = 2;
        Rra().Emit<EcmaCallithisrangedyn>(node, callee, argCount + EXTRA_ARGS, static_cast<int64_t>(argCount), callee);
    } else {
        size_t argCount = arguments.size();
        Rra().Emit<EcmaCallirangedyn>(node, callee, argCount + 1, static_cast<int64_t>(argCount), callee);
    }
}

bool PandaGen::CallArgsTagged(const ir::AstNode *node, VReg callee, VReg thisReg,
                              const ArenaVector<ir::Expression *> &arguments, bool hasThis)
{
    VReg arg0Reg = AllocReg();
    StoreAccumulator(node, arg0Reg);

    const auto call1 = [this, hasThis, arg0Reg](const ir::AstNode *n, VReg c, VReg thisR,
                                                const ArenaVector<ir::Expression *> &args) {
        const auto *arg = args[0];
        arg->Compile(this);

        if (hasThis) {
            Ra().Emit<EcmaCall2thisdyn>(n, c, thisR, arg0Reg);
        } else {
            Ra().Emit<EcmaCall2dyn>(n, c, arg0Reg);
        }
    };
    const auto call2 = [this, hasThis, arg0Reg](const ir::AstNode *n, VReg c, VReg thisR,
                                                const ArenaVector<ir::Expression *> &args) {
        const auto *arg1 = args[0];
        arg1->Compile(this);
        compiler::VReg arg1Reg = AllocReg();
        StoreAccumulator(arg1, arg1Reg);

        const auto *arg2 = args[1];
        arg2->Compile(this);

        if (hasThis) {
            Ra().Emit<EcmaCall3thisdyn>(n, c, thisR, arg0Reg, arg1Reg);
        } else {
            Ra().Emit<EcmaCall3dyn>(n, c, arg0Reg, arg1Reg);
        }
    };

    switch (arguments.size()) {
        case 1: {
            call1(node, callee, thisReg, arguments);
            return true;
        }
        case 2: {  // 2:2 args
            call2(node, callee, thisReg, arguments);
            return true;
        }
        default: {
            break;
        }
    }
    return false;
}

void PandaGen::CallTagged(const ir::AstNode *node, VReg callee, VReg thisReg,
                          const ArenaVector<ir::Expression *> &arguments)
{
    bool hasThis = !thisReg.IsInvalid();

    StoreAccumulator(node, callee);
    Literals::GetTemplateObject(this, node->AsTaggedTemplateExpression());

    if (arguments.empty()) {
        if (hasThis) {
            Ra().Emit<EcmaCall1thisdyn>(node, callee, thisReg);
        } else {
            Sa().Emit<EcmaCall1dyn>(node, callee);
        }
        return;
    }

    if (CallArgsTagged(node, callee, thisReg, arguments, hasThis)) {
        return;
    }

    for (const auto *it : arguments) {
        it->Compile(this);
        compiler::VReg arg = AllocReg();
        StoreAccumulator(it, arg);
    }

    if (hasThis) {
        auto constexpr EXTRA_ARGS = 2;
        size_t argCount = arguments.size() + EXTRA_ARGS;
        Rra().Emit<EcmaCallithisrangedyn>(node, callee, argCount + EXTRA_ARGS, static_cast<int64_t>(argCount), callee);
    } else {
        size_t argCount = arguments.size() + 1;
        Rra().Emit<EcmaCallirangedyn>(node, callee, argCount + 1, static_cast<int64_t>(argCount), callee);
    }
}

void PandaGen::SuperCall(const ir::AstNode *node, VReg startReg, size_t argCount)
{
    Rra().Emit<EcmaSupercall>(node, startReg, argCount, static_cast<int64_t>(argCount), startReg);
}

void PandaGen::SuperCallSpread(const ir::AstNode *node, VReg vs)
{
    Ra().Emit<EcmaSupercallspread>(node, vs);
}

void PandaGen::NewObject(const ir::AstNode *node, VReg startReg, size_t argCount)
{
    Rra().Emit<EcmaNewobjdynrange>(node, startReg, argCount, static_cast<int64_t>(argCount), startReg);
}

void PandaGen::LoadHomeObject(const ir::AstNode *node)
{
    Sa().Emit<EcmaLdhomeobject>(node);
}

void PandaGen::DefineMethod(const ir::AstNode *node, const util::StringView &name)
{
    Ra().Emit<EcmaDefinemethod>(node, name, LexEnv());
}

void PandaGen::DefineFunction(const ir::AstNode *node, const ir::ScriptFunction *realNode, const util::StringView &name)
{
    if (realNode->IsAsyncFunc()) {
        if (realNode->IsGenerator()) {
            Ra().Emit<EcmaDefineasyncgeneratorfunc>(node, name, LexEnv());
        } else {
            Ra().Emit<EcmaDefineasyncfunc>(node, name, LexEnv());
        }
    } else if (realNode->IsGenerator()) {
        Ra().Emit<EcmaDefinegeneratorfunc>(node, name, LexEnv());
    } else if (realNode->IsArrow()) {
        LoadHomeObject(node);
        Ra().Emit<EcmaDefinencfuncdyn>(node, name, LexEnv());
    } else if (realNode->IsMethod()) {
        DefineMethod(node, name);
    } else {
        Ra().Emit<EcmaDefinefuncdyn>(node, name, LexEnv());
    }
}

void PandaGen::TypeOf(const ir::AstNode *node)
{
    Sa().Emit<EcmaTypeofdyn>(node);
}

void PandaGen::CallSpread(const ir::AstNode *node, VReg func, VReg thisReg, VReg args)
{
    Ra().Emit<EcmaCallspreaddyn>(node, func, thisReg, args);
}

void PandaGen::NewObjSpread(const ir::AstNode *node, VReg obj, VReg target)
{
    Ra().Emit<EcmaNewobjspreaddyn>(node, obj, target);
}

void PandaGen::GetUnmappedArgs(const ir::AstNode *node)
{
    Sa().Emit<EcmaGetunmappedargs>(node);
}

void PandaGen::Negate(const ir::AstNode *node)
{
    Sa().Emit<EcmaNegate>(node);
}

void PandaGen::ToBoolean(const ir::AstNode *node)
{
    Sa().Emit<EcmaToboolean>(node);
}

void PandaGen::ToNumber(const ir::AstNode *node, VReg arg)
{
    Ra().Emit<EcmaTonumber>(node, arg);
}

void PandaGen::GetMethod(const ir::AstNode *node, VReg obj, const util::StringView &name)
{
    Ra().Emit<EcmaGetmethod>(node, name, obj);
}

void PandaGen::CreateGeneratorObj(const ir::AstNode *node, VReg funcObj)
{
    Ra().Emit<EcmaCreategeneratorobj>(node, funcObj);
}

void PandaGen::CreateAsyncGeneratorObj(const ir::AstNode *node, VReg funcObj)
{
    Ra().Emit<EcmaCreateasyncgeneratorobj>(node, funcObj);
}

void PandaGen::CreateIterResultObject(const ir::AstNode *node, bool done)
{
    Ra().Emit<EcmaCreateiterresultobj>(node, static_cast<int32_t>(done));
}

void PandaGen::SuspendGenerator(const ir::AstNode *node, VReg genObj)
{
    Ra().Emit<EcmaSuspendgenerator>(node, genObj);
}

void PandaGen::SuspendAsyncGenerator(const ir::AstNode *node, VReg asyncGenObj)
{
    Ra().Emit<EcmaSuspendasyncgenerator>(node, asyncGenObj);
}

void PandaGen::GeneratorYield(const ir::AstNode *node, VReg genObj)
{
    Ra().Emit<EcmaSetgeneratorstate>(node, genObj, static_cast<int32_t>(GeneratorState::SUSPENDED_YIELD));
}

void PandaGen::GeneratorComplete(const ir::AstNode *node, VReg genObj)
{
    Ra().Emit<EcmaSetgeneratorstate>(node, genObj, static_cast<int32_t>(GeneratorState::COMPLETED));
}

void PandaGen::ResumeGenerator(const ir::AstNode *node, VReg genObj)
{
    Ra().Emit<EcmaResumegenerator>(node, genObj);
}

void PandaGen::GetResumeMode(const ir::AstNode *node, VReg genObj)
{
    Ra().Emit<EcmaGetresumemode>(node, genObj);
}

void PandaGen::AsyncFunctionEnter(const ir::AstNode *node)
{
    Sa().Emit<EcmaAsyncfunctionenter>(node);
}

void PandaGen::AsyncFunctionAwait(const ir::AstNode *node, VReg asyncFuncObj)
{
    Ra().Emit<EcmaAsyncfunctionawait>(node, asyncFuncObj);
}

void PandaGen::AsyncFunctionResolve(const ir::AstNode *node, VReg asyncFuncObj)
{
    Ra().Emit<EcmaAsyncfunctionresolve>(node, asyncFuncObj);
}

void PandaGen::AsyncFunctionReject(const ir::AstNode *node, VReg asyncFuncObj)
{
    Ra().Emit<EcmaAsyncfunctionreject>(node, asyncFuncObj);
}

void PandaGen::AsyncGeneratorResolve(const ir::AstNode *node, VReg asyncGenObj)
{
    Ra().Emit<EcmaAsyncgeneratorresolve>(node, asyncGenObj);
}

void PandaGen::AsyncGeneratorReject(const ir::AstNode *node, VReg asyncGenObj)
{
    Ra().Emit<EcmaAsyncgeneratorreject>(node, asyncGenObj);
}

void PandaGen::GetTemplateObject(const ir::AstNode *node, VReg value)
{
    Ra().Emit<EcmaGettemplateobject>(node, value);
}

void PandaGen::CopyRestArgs(const ir::AstNode *node, uint32_t index)
{
    Sa().Emit<EcmaCopyrestargs>(node, index);
}

void PandaGen::GetPropIterator(const ir::AstNode *node)
{
    Sa().Emit<EcmaGetpropiterator>(node);
}

void PandaGen::GetNextPropName(const ir::AstNode *node, VReg iter)
{
    Ra().Emit<EcmaGetnextpropname>(node, iter);
}

void PandaGen::CreateEmptyObject(const ir::AstNode *node)
{
    Sa().Emit<EcmaCreateemptyobject>(node);
}

void PandaGen::CreateObjectWithBuffer(const ir::AstNode *node, uint32_t idx)
{
    ASSERT(util::Helpers::IsInteger<uint32_t>(idx));
    Sa().Emit<EcmaCreateobjectwithbuffer>(node, util::Helpers::ToStringView(Allocator(), idx));
}

void PandaGen::CreateObjectHavingMethod(const ir::AstNode *node, uint32_t idx)
{
    ASSERT(util::Helpers::IsInteger<uint32_t>(idx));
    LoadAccumulator(node, LexEnv());
    Sa().Emit<EcmaCreateobjecthavingmethod>(node, util::Helpers::ToStringView(Allocator(), idx));
}

void PandaGen::SetObjectWithProto(const ir::AstNode *node, VReg proto, VReg obj)
{
    Ra().Emit<EcmaSetobjectwithproto>(node, proto, obj);
}

void PandaGen::CopyDataProperties(const ir::AstNode *node, VReg dst, VReg src)
{
    Ra().Emit<EcmaCopydataproperties>(node, dst, src);
}

void PandaGen::DefineGetterSetterByValue(const ir::AstNode *node, std::tuple<VReg, VReg, VReg, VReg> registers,
                                         bool setName)
{
    const auto [obj, name, getter, setter] = registers;
    LoadConst(node, setName ? Constant::JS_TRUE : Constant::JS_FALSE);
    Ra().Emit<EcmaDefinegettersetterbyvalue>(node, obj, name, getter, setter);
}

void PandaGen::CreateEmptyArray(const ir::AstNode *node)
{
    Sa().Emit<EcmaCreateemptyarray>(node);
}

void PandaGen::CreateArrayWithBuffer(const ir::AstNode *node, uint32_t idx)
{
    ASSERT(util::Helpers::IsInteger<uint32_t>(idx));
    Sa().Emit<EcmaCreatearraywithbuffer>(node, util::Helpers::ToStringView(Allocator(), idx));
}

size_t PandaGen::HandleArrayLiterals(const ir::AstNode *node, const ArenaVector<ir::Expression *> &elements)
{
    LiteralBuffer buf {};

    size_t i = 0;
    // This loop handles constant literal data by collecting it into a literal buffer
    // until a non-constant element is encountered.
    while (i < elements.size()) {
        Literal lit = util::Helpers::ToConstantLiteral(elements[i]);
        if (lit.IsInvalid()) {
            break;
        }

        buf.emplace_back(std::move(lit));
        i++;
    }

    if (buf.empty()) {
        CreateEmptyArray(node);
    } else {
        uint32_t bufIdx = AddLiteralBuffer(std::move(buf));
        CreateArrayWithBuffer(node, bufIdx);
    }
    return i;
}

void PandaGen::HandleArraySpread(const ir::AstNode *node, const ArenaVector<ir::Expression *> &elements, VReg obj)
{
    size_t i = 0;
    bool hasSpread = false;

    // This loop handles array elements until a spread element is encountered
    for (; i < elements.size(); i++) {
        const ir::Expression *elem = elements[i];

        if (elem->IsOmittedExpression()) {
            continue;
        }

        if (elem->IsSpreadElement()) {
            // The next loop will handle arrays that have a spread element
            hasSpread = true;
            break;
        }

        elem->Compile(this);
        StOwnByIndex(elem, obj, i);
    }

    RegScope rs(this);
    VReg idxReg {};

    if (hasSpread) {
        idxReg = AllocReg();
        LoadAccumulatorInt(node, i);
        StoreAccumulator(node, idxReg);
    }

    // This loop handles arrays that contain spread elements
    for (; i < elements.size(); i++) {
        const ir::Expression *elem = elements[i];

        if (elem->IsSpreadElement()) {
            elem->AsSpreadElement()->Argument()->Compile(this);

            StoreArraySpread(elem, obj, idxReg);
            StoreAccumulator(elem, idxReg);
            continue;
        }

        if (!elem->IsOmittedExpression()) {
            elem->Compile(this);
            StOwnByValue(elem, obj, idxReg);
        }

        Unary(elem, lexer::TokenType::PUNCTUATOR_PLUS_PLUS, idxReg);
        StoreAccumulator(elem, idxReg);
    }

    // If the last element is omitted, we also have to update the length property
    if (elements.back()->IsOmittedExpression()) {
        // if there was a spread value then acc already contains the length
        if (!hasSpread) {
            LoadAccumulatorInt(node, i);
        }

        StOwnByName(node, obj, "length");
    }
}

void PandaGen::CreateArray(const ir::AstNode *node, const ArenaVector<ir::Expression *> &elements, VReg obj)
{
    if (elements.empty()) {
        CreateEmptyArray(node);
        StoreAccumulator(node, obj);
        return;
    }

    const auto i = HandleArrayLiterals(node, elements);

    StoreAccumulator(node, obj);

    if (i == elements.size()) {
        return;
    }

    HandleArraySpread(node, elements, obj);

    LoadAccumulator(node, obj);
}

void PandaGen::StoreArraySpread(const ir::AstNode *node, VReg array, VReg index)
{
    Ra().Emit<EcmaStarrayspread>(node, array, index);
}

void PandaGen::CreateRegExpWithLiteral(const ir::AstNode *node, const util::StringView &pattern, uint8_t flags)
{
    Sa().Emit<EcmaCreateregexpwithliteral>(node, pattern, flags);
}

void PandaGen::ThrowIfNotObject(const ir::AstNode *node)
{
    Ra().Emit<EcmaThrowifnotobject>(node);
}

void PandaGen::ThrowThrowNotExist(const ir::AstNode *node)
{
    Sa().Emit<EcmaThrowthrownotexists>(node);
}

void PandaGen::GetIterator(const ir::AstNode *node)
{
    Sa().Emit<EcmaGetiterator>(node);
}

void PandaGen::GetAsyncIterator(const ir::AstNode *node)
{
    Sa().Emit<EcmaGetasynciterator>(node);
}

void PandaGen::CreateObjectWithExcludedKeys(const ir::AstNode *node, VReg obj, VReg argStart, size_t argCount)
{
    ASSERT(argStart.GetIndex() == obj.GetIndex() - 1);
    if (argCount == 0) {  // Do not emit undefined register
        argStart = obj;
    }

    Rra().Emit<EcmaCreateobjectwithexcludedkeys>(node, argStart, argCount, static_cast<int64_t>(argCount), obj,
                                                 argStart);
}

void PandaGen::ThrowObjectNonCoercible(const ir::AstNode *node)
{
    Sa().Emit<EcmaThrowpatternnoncoercible>(node);
}

void PandaGen::CloseIterator(const ir::AstNode *node, VReg iter)
{
    Ra().Emit<EcmaCloseiterator>(node, iter);
}

void PandaGen::ImportModule(const ir::AstNode *node, const util::StringView &name)
{
    Sa().Emit<EcmaImportmodule>(node, name);
}

void PandaGen::SetClassComputedFields(const ir::AstNode *node, VReg classReg, VReg computedInstanceFieldArray)
{
    Ra().Emit<EcmaSetclasscomputedfields>(node, classReg, computedInstanceFieldArray);
}

void PandaGen::DefineClassWithBuffer(const ir::AstNode *node, const util::StringView &ctorId, uint32_t litIdx,
                                     VReg lexenv, VReg base)
{
    Ra().Emit<EcmaDefineclasswithbuffer>(node, ctorId, litIdx, lexenv, base);
}

void PandaGen::LoadClassComputedInstanceFields(const ir::AstNode *node, VReg ctor)
{
    Sa().Emit<EcmaLoadclasscomputedinstancefields>(node, ctor);
}

void PandaGen::DefineClassPrivateFields(const ir::AstNode *node, uint32_t privateBufIdx)
{
    Sa().Emit<EcmaDefineclassprivatefields>(node, util::Helpers::ToStringView(Allocator(), privateBufIdx), LexEnv());
}

void PandaGen::ClassFieldAdd(const ir::AstNode *node, VReg obj, VReg prop)
{
    Ra().Emit<EcmaClassfieldadd>(node, obj, prop);
}

void PandaGen::ClassPrivateFieldAdd(const ir::AstNode *node, VReg ctor, VReg obj, const util::StringView &prop)
{
    Ra().Emit<EcmaClassprivatefieldadd>(node, prop, ctor, obj);
}

void PandaGen::ClassPrivateMethodOrAccessorAdd(const ir::AstNode *node, VReg ctor, VReg obj)
{
    Ra().Emit<EcmaClassprivatemethodoraccessoradd>(node, ctor, obj);
}

void PandaGen::ClassPrivateFieldGet(const ir::AstNode *node, VReg ctor, VReg obj, const util::StringView &prop)
{
    Ra().Emit<EcmaClassprivatefieldget>(node, prop, ctor, obj);
}

void PandaGen::ClassPrivateFieldSet(const ir::AstNode *node, VReg ctor, VReg obj, const util::StringView &prop)
{
    Ra().Emit<EcmaClassprivatefieldset>(node, prop, ctor, obj);
}

void PandaGen::ClassPrivateFieldIn(const ir::AstNode *node, VReg ctor, const util::StringView &prop)
{
    Ra().Emit<EcmaClassprivatefieldin>(node, prop, ctor);
}

void PandaGen::LoadModuleVariable(const ir::AstNode *node, VReg module, const util::StringView &name)
{
    Ra().Emit<EcmaLdmodvarbyname>(node, name, module);
}

void PandaGen::StoreModuleVar(const ir::AstNode *node, const util::StringView &name)
{
    Sa().Emit<EcmaStmodulevar>(node, name);
}

void PandaGen::CopyModule(const ir::AstNode *node, VReg module)
{
    Ra().Emit<EcmaCopymodule>(node, module);
}

void PandaGen::StSuperByName(const ir::AstNode *node, VReg obj, const util::StringView &key)
{
    Ra().Emit<EcmaStsuperbyname>(node, key, obj);
}

void PandaGen::LdSuperByName(const ir::AstNode *node, const util::StringView &key)
{
    Ra().Emit<EcmaLdsuperbyname>(node, key);
}

void PandaGen::StSuperByValue(const ir::AstNode *node, VReg obj, VReg prop)
{
    Ra().Emit<EcmaStsuperbyvalue>(node, obj, prop);
}

void PandaGen::LdSuperByValue(const ir::AstNode *node, VReg obj)
{
    Ra().Emit<EcmaLdsuperbyvalue>(node, obj);
}

void PandaGen::StoreSuperProperty(const ir::AstNode *node, VReg obj, const Operand &prop)
{
    if (std::holds_alternative<util::StringView>(prop)) {
        StSuperByName(node, obj, std::get<util::StringView>(prop));
        return;
    }

    ASSERT(std::holds_alternative<VReg>(prop));
    StSuperByValue(node, obj, std::get<VReg>(prop));
}

void PandaGen::LoadSuperProperty(const ir::AstNode *node, const Operand &prop)
{
    if (std::holds_alternative<util::StringView>(prop)) {
        LdSuperByName(node, std::get<util::StringView>(prop));
        return;
    }

    ASSERT(std::holds_alternative<VReg>(prop));
    LdSuperByValue(node, std::get<VReg>(prop));
}

void PandaGen::LoadLexicalVar(const ir::AstNode *node, uint32_t level, uint32_t slot)
{
    Sa().Emit<EcmaLdlexvardyn>(node, level, slot);
}

void PandaGen::LoadLexical(const ir::AstNode *node, const util::StringView &name, uint32_t level, uint32_t slot)
{
    Sa().Emit<EcmaLdlexdyn>(node, name, level, slot);
}

void PandaGen::StoreLexicalVar(const ir::AstNode *node, uint32_t level, uint32_t slot)
{
    Ra().Emit<EcmaStlexvardyn>(node, level, slot);
}

void PandaGen::StoreLexical(const ir::AstNode *node, const util::StringView &name, uint32_t level, uint32_t slot)
{
    Ra().Emit<EcmaStlexdyn>(node, name, level, slot);
}

void PandaGen::ThrowIfSuperNotCorrectCall(const ir::AstNode *node, int64_t num)
{
    Sa().Emit<EcmaThrowifsupernotcorrectcall>(node, num);
}

void PandaGen::ThrowTdz(const ir::AstNode *node, const util::StringView &name)
{
    Sa().Emit<EcmaThrowtdz>(node, name);
}

void PandaGen::ThrowConstAssignment(const ir::AstNode *node, const util::StringView &name)
{
    Ra().Emit<EcmaThrowconstassignment>(node, name);
}

void PandaGen::PopLexEnv(const ir::AstNode *node)
{
    Sa().Emit<EcmaPoplexenvdyn>(node);
}

void PandaGen::CopyLexEnv(const ir::AstNode *node)
{
    Sa().Emit<EcmaCopylexenvdyn>(node);
}

void PandaGen::NewLexEnv(const ir::AstNode *node, uint32_t num)
{
    Sa().Emit<EcmaNewlexenvdyn>(node, num);
}

void PandaGen::LdLexEnv(const ir::AstNode *node)
{
    Sa().Emit<EcmaLdlexenvdyn>(node);
}

Operand PandaGen::ToNamedPropertyKey(const ir::Expression *prop, bool isComputed)
{
    VReg res {VReg::REG_START};

    if (!isComputed) {
        if (prop->IsIdentifier()) {
            return prop->AsIdentifier()->Name();
        }
        return res;
    }

    if (prop->IsStringLiteral()) {
        const util::StringView &str = prop->AsStringLiteral()->Str();

        /* NOTE: dbatyai. remove this when runtime handles __proto__ as property name correctly */
        if (str.Is("__proto__")) {
            return res;
        }

        int64_t index = util::Helpers::GetIndex(str);
        if (index != util::Helpers::INVALID_INDEX) {
            return index;
        }

        return str;
    }

    if (prop->IsNumberLiteral()) {
        auto num = prop->AsNumberLiteral()->Number().GetDouble();
        if (util::Helpers::IsIndex(num)) {
            return static_cast<int64_t>(num);
        }

        return prop->AsNumberLiteral()->Str();
    }

    return res;
}

Operand PandaGen::ToPropertyKey(const ir::Expression *prop, bool isComputed, bool isSuperExpression)
{
    Operand op = ToNamedPropertyKey(prop, isComputed);
    if (std::holds_alternative<util::StringView>(op)) {
        return op;
    }

    if (std::holds_alternative<int64_t>(op) && !isSuperExpression) {
        return op;
    }

    VReg objReg = AllocReg();
    StoreAccumulator(prop, objReg);
    prop->Compile(this);

    return objReg;
}

Operand PandaGen::ToOwnPropertyKey(const ir::Expression *prop, bool isComputed)
{
    Operand op = ToNamedPropertyKey(prop, isComputed);
    if (std::holds_alternative<util::StringView>(op)) {
        ASSERT(std::holds_alternative<util::StringView>(op) || std::holds_alternative<int64_t>(op));
        return op;
    }

    if (std::holds_alternative<int64_t>(op)) {
        return op;
    }

    VReg propReg = AllocReg();
    prop->Compile(this);
    StoreAccumulator(prop, propReg);

    return propReg;
}

void PandaGen::LoadPropertyKeyAcc(const ir::Expression *prop, bool isComputed)
{
    Operand op = ToNamedPropertyKey(prop, isComputed);
    if (std::holds_alternative<util::StringView>(op)) {
        LoadAccumulatorString(prop, std::get<util::StringView>(op));
    } else if (std::holds_alternative<int64_t>(op)) {
        LoadAccumulatorInt(prop, static_cast<size_t>(std::get<int64_t>(op)));
    } else {
        prop->Compile(this);
    }
}

VReg PandaGen::LoadPropertyKey(const ir::Expression *prop, bool isComputed)
{
    LoadPropertyKeyAcc(prop, isComputed);

    VReg propReg = AllocReg();
    StoreAccumulator(prop, propReg);

    return propReg;
}

void PandaGen::LoadEvalVariable(const ir::AstNode *node, const util::StringView &name)
{
    RegScope rs(this);
    LoadLexicalContext(node);
    Ra().Emit<EcmaLdevalvar>(node, name);
}

void PandaGen::StoreEvalVariable(const ir::AstNode *node, const util::StringView &name)
{
    RegScope rs(this);
    VReg value = AllocReg();
    StoreAccumulator(node, value);
    LoadLexicalContext(node);
    Ra().Emit<EcmaStevalvar>(node, name, value);
}

void PandaGen::DirectEval(const ir::AstNode *node, uint32_t parserStatus)
{
    RegScope rs(this);

    uint32_t index = 0;
    uint32_t evalBindingsIndex = 0;
    VReg arg0 = AllocReg();
    StoreAccumulator(node, arg0);
    VReg bindings = AllocReg();
    CreateEmptyArray(node);
    StoreAccumulator(node, bindings);

    GetFunctionObject(node);
    StOwnByIndex(node, bindings, index++);
    GetNewTarget(node);
    StOwnByIndex(node, bindings, index++);
    GetThis(node);
    StOwnByIndex(node, bindings, index++);

    VReg evalBindings = AllocReg();
    CreateEmptyArray(node);
    StoreAccumulator(node, evalBindings);

    LoadAccumulator(node, LexEnv());
    StOwnByIndex(node, evalBindings, evalBindingsIndex++);

    const auto *iter = Scope()->EnclosingVariableScope();

    while (true) {
        uint32_t scopeBindingsBuf = iter->EvalBindings();
        if (scopeBindingsBuf != INVALID_LITERAL_BUFFER_ID) {
            Sa().Emit<EcmaLdevalbindings>(node, util::Helpers::ToStringView(Allocator(), scopeBindingsBuf));
            StOwnByIndex(node, evalBindings, evalBindingsIndex++);
        }

        if (iter->Parent() == nullptr) {
            break;
        }

        iter = iter->Parent()->EnclosingVariableScope();
    }

    LoadAccumulator(node, evalBindings);
    StOwnByIndex(node, bindings, index++);

    Sa().Emit<EcmaDirecteval>(node, static_cast<int64_t>(parserStatus), arg0, bindings);
}

void PandaGen::LoadLexicalContext(const ir::AstNode *node)
{
    auto result = Scope()->Find(varbinder::VarBinder::LEXICAL_CONTEXT_PARAM);
    LoadLexicalVar(node, result.lexLevel, result.variable->AsLocalVariable()->LexIdx());
}

bool PandaGen::IsDirectEval() const
{
    return Context()->config->options->CompilerOptions().isDirectEval;
}

bool PandaGen::IsEval() const
{
    return Context()->config->options->CompilerOptions().isEval;
}

const checker::Type *PandaGen::GetVRegType(VReg vreg) const
{
    // We assume that all used regs have any type
    if (vreg.GetIndex() > NextReg().GetIndex()) {
        return Context()->checker->GetGlobalTypesHolder()->GlobalAnyType();
    }

    return nullptr;
}

}  // namespace ark::es2panda::compiler
