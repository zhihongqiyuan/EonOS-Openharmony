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

#include "ETSGen.h"

#include "generated/signatures.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/classDefinition.h"
#include "ir/statement.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/binaryExpression.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/memberExpression.h"
#include "ir/expressions/templateLiteral.h"
#include "ir/statements/breakStatement.h"
#include "ir/statements/continueStatement.h"
#include "ir/statements/tryStatement.h"
#include "ir/ts/tsInterfaceDeclaration.h"
#include "varbinder/variableFlags.h"
#include "compiler/base/lreference.h"
#include "compiler/base/catchTable.h"
#include "compiler/core/dynamicContext.h"
#include "varbinder/ETSBinder.h"
#include "varbinder/variable.h"
#include "checker/types/type.h"
#include "checker/types/typeFlag.h"
#include "checker/checker.h"
#include "checker/ETSchecker.h"
#include "checker/types/ets/etsObjectType.h"
#include "checker/types/ets/etsAsyncFuncReturnType.h"
#include "parser/program/program.h"
#include "checker/types/globalTypesHolder.h"
#include "public/public.h"

namespace ark::es2panda::compiler {

static inline bool IsWidePrimitiveType(checker::Type const *type)
{
    return type->IsLongType() || type->IsDoubleType();
}

ETSGen::ETSGen(ArenaAllocator *allocator, RegSpiller *spiller, public_lib::Context *context,
               std::tuple<varbinder::FunctionScope *, ProgramElement *, AstCompiler *> toCompile) noexcept
    : CodeGen(allocator, spiller, context, toCompile),
      containingObjectType_(util::Helpers::GetContainingObjectType(RootNode()))
{
    ETSFunction::Compile(this);
}

void ETSGen::SetAccumulatorType(const checker::Type *type)
{
    SetVRegType(acc_, type);
}

const checker::Type *ETSGen::GetAccumulatorType() const
{
    return GetVRegType(acc_);
}

void ETSGen::CompileAndCheck(const ir::Expression *expr)
{
    // NOTE: vpukhov. bad accumulator type leads to terrible bugs in codegen
    // make exact types match mandatory
    expr->Compile(this);

    auto const *const accType = GetAccumulatorType();
    auto const *const exprType = expr->TsType();

    if (Checker()->Relation()->IsIdenticalTo(accType, exprType) || exprType->IsETSTypeParameter() ||
        exprType->IsETSPartialTypeParameter() || exprType->IsETSNonNullishType()) {
        return;
    }

    if (accType->IsETSPrimitiveType() &&
        ((accType->TypeFlags() ^ exprType->TypeFlags()) & ~checker::TypeFlag::CONSTANT) == 0) {
        return;
    }

    ASSERT_PRINT(false, std::string("Type mismatch after Expression::Compile: ") + accType->ToString() +
                            " instead of " + exprType->ToString());
}

const checker::ETSChecker *ETSGen::Checker() const noexcept
{
    return Context()->checker->AsETSChecker();
}

const varbinder::ETSBinder *ETSGen::VarBinder() const noexcept
{
    return Context()->parserProgram->VarBinder()->AsETSBinder();
}

const checker::Type *ETSGen::ReturnType() const noexcept
{
    return RootNode()->AsScriptFunction()->Signature()->ReturnType();
}

const checker::ETSObjectType *ETSGen::ContainingObjectType() const noexcept
{
    return containingObjectType_;
}

VReg &ETSGen::Acc() noexcept
{
    return acc_;
}

VReg ETSGen::Acc() const noexcept
{
    return acc_;
}

void ETSGen::ApplyConversionAndStoreAccumulator(const ir::AstNode *const node, const VReg vreg,
                                                const checker::Type *const targetType)
{
    ApplyConversion(node, targetType);
    StoreAccumulator(node, vreg);
}

VReg ETSGen::StoreException(const ir::AstNode *node)
{
    VReg exception = AllocReg();
    Ra().Emit<StaObj>(node, exception);

    SetAccumulatorType(Checker()->GlobalBuiltinExceptionType());
    SetVRegType(exception, GetAccumulatorType());
    return exception;
}

void ETSGen::StoreAccumulator(const ir::AstNode *const node, const VReg vreg)
{
    const auto *const accType = GetAccumulatorType();

    ASSERT(accType != nullptr);
    if (accType->IsETSReferenceType()) {
        Ra().Emit<StaObj>(node, vreg);
    } else if (IsWidePrimitiveType(accType)) {
        Ra().Emit<StaWide>(node, vreg);
    } else {
        Ra().Emit<Sta>(node, vreg);
    }

    SetVRegType(vreg, accType);
}

void ETSGen::LoadAccumulator(const ir::AstNode *node, VReg vreg)
{
    const auto *const vregType = GetVRegType(vreg);

    ASSERT(vregType != nullptr);
    if (vregType->IsETSReferenceType()) {
        Ra().Emit<LdaObj>(node, vreg);
    } else if (IsWidePrimitiveType(vregType)) {
        Ra().Emit<LdaWide>(node, vreg);
    } else {
        Ra().Emit<Lda>(node, vreg);
    }

    SetAccumulatorType(vregType);
}

IRNode *ETSGen::AllocMov(const ir::AstNode *const node, const VReg vd, const VReg vs)
{
    const auto *const sourceType = GetVRegType(vs);
    // CC-OFFNXT(G.FMT.14-CPP) project code style
    auto *const mov = [this, sourceType, node, vd, vs]() -> IRNode * {
        if (sourceType->IsETSReferenceType()) {
            return Allocator()->New<MovObj>(node, vd, vs);
        }
        if (IsWidePrimitiveType(sourceType)) {
            return Allocator()->New<MovWide>(node, vd, vs);
        }
        return Allocator()->New<Mov>(node, vd, vs);
    }();

    SetVRegType(vd, sourceType);
    return mov;
}

IRNode *ETSGen::AllocMov(const ir::AstNode *const node, OutVReg vd, const VReg vs)
{
    ASSERT(vd.type != OperandType::ANY && vd.type != OperandType::NONE);

    switch (vd.type) {
        case OperandType::REF:
            return Allocator()->New<MovObj>(node, *vd.reg, vs);
        case OperandType::B64:
            return Allocator()->New<MovWide>(node, *vd.reg, vs);
        default:
            break;
    }

    return Allocator()->New<Mov>(node, *vd.reg, vs);
}

checker::Type const *ETSGen::TypeForVar(varbinder::Variable const *var) const noexcept
{
    return var->TsType();
}

void ETSGen::MoveVreg(const ir::AstNode *const node, const VReg vd, const VReg vs)
{
    const auto *const sourceType = GetVRegType(vs);

    if (sourceType->IsETSReferenceType()) {
        Ra().Emit<MovObj>(node, vd, vs);
    } else if (IsWidePrimitiveType(sourceType)) {
        Ra().Emit<MovWide>(node, vd, vs);
    } else {
        Ra().Emit<Mov>(node, vd, vs);
    }

    SetVRegType(vd, sourceType);
}

util::StringView ETSGen::FormDynamicModulePropReference(const varbinder::Variable *var)
{
    ASSERT(VarBinder()->IsDynamicModuleVariable(var) || VarBinder()->IsDynamicNamespaceVariable(var));

    auto *data = VarBinder()->DynamicImportDataForVar(var);
    ASSERT(data != nullptr);

    auto *import = data->import;

    return FormDynamicModulePropReference(import);
}

void ETSGen::LoadAccumulatorDynamicModule(const ir::AstNode *node, const ir::ETSImportDeclaration *import)
{
    ASSERT(import->Language().IsDynamic());
    LoadStaticProperty(node, Checker()->GlobalBuiltinDynamicType(import->Language()),
                       FormDynamicModulePropReference(import));
}

util::StringView ETSGen::FormDynamicModulePropReference(const ir::ETSImportDeclaration *import)
{
    std::stringstream ss;

    if (!VarBinder()->Program()->OmitModuleName()) {
        ss << VarBinder()->Program()->ModuleName() << compiler::Signatures::METHOD_SEPARATOR;
    }

    ss << compiler::Signatures::DYNAMIC_MODULE_CLASS << compiler::Signatures::METHOD_SEPARATOR
       << import->AssemblerName();

    return util::UString(ss.str(), Allocator()).View();
}

void ETSGen::LoadDynamicModuleVariable(const ir::AstNode *node, varbinder::Variable const *const var)
{
    RegScope rs(this);

    auto *data = VarBinder()->DynamicImportDataForVar(var);
    auto *import = data->import;

    LoadStaticProperty(node, var->TsType(), FormDynamicModulePropReference(var));

    auto objReg = AllocReg();
    StoreAccumulator(node, objReg);

    auto *id = data->specifier->AsImportSpecifier()->Imported();
    auto lang = import->Language();
    LoadPropertyDynamic(node, Checker()->GlobalBuiltinDynamicType(lang), objReg, id->Name());

    ApplyConversion(node);
}

void ETSGen::LoadDynamicNamespaceVariable(const ir::AstNode *node, varbinder::Variable const *const var)
{
    LoadStaticProperty(node, var->TsType(), FormDynamicModulePropReference(var));
}

void ETSGen::LoadVar(const ir::Identifier *node, varbinder::Variable const *const var)
{
    if (VarBinder()->IsDynamicModuleVariable(var)) {
        LoadDynamicModuleVariable(node, var);
        return;
    }

    if (VarBinder()->IsDynamicNamespaceVariable(var)) {
        LoadDynamicNamespaceVariable(node, var);
        return;
    }

    auto *local = var->AsLocalVariable();

    switch (ETSLReference::ResolveReferenceKind(var)) {
        case ReferenceKind::STATIC_FIELD: {
            auto fullName = FormClassPropReference(var);
            LoadStaticProperty(node, var->TsType(), fullName);
            break;
        }
        case ReferenceKind::FIELD: {
            const auto fullName = FormClassPropReference(GetVRegType(GetThisReg())->AsETSObjectType(), var->Name());
            LoadProperty(node, var->TsType(), GetThisReg(), fullName);
            break;
        }
        case ReferenceKind::METHOD:
        case ReferenceKind::STATIC_METHOD:
        case ReferenceKind::CLASS:
        case ReferenceKind::STATIC_CLASS: {
            SetAccumulatorType(var->TsType());
            break;
        }
        case ReferenceKind::LOCAL: {
            LoadAccumulator(node, local->Vreg());
            SetAccumulatorType(GetVRegType(local->Vreg()));
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void ETSGen::StoreVar(const ir::Identifier *node, const varbinder::ConstScopeFindResult &result)
{
    auto *local = result.variable->AsLocalVariable();
    ApplyConversion(node, local->TsType());

    switch (ETSLReference::ResolveReferenceKind(result.variable)) {
        case ReferenceKind::STATIC_FIELD: {
            auto fullName = FormClassPropReference(result.variable);
            StoreStaticProperty(node, result.variable->TsType(), fullName);
            break;
        }
        case ReferenceKind::FIELD: {
            StoreProperty(node, result.variable->TsType(), GetThisReg(), result.name);
            break;
        }
        case ReferenceKind::LOCAL: {
            StoreAccumulator(node, local->Vreg());
            SetVRegType(local->Vreg(), GetAccumulatorType());
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

util::StringView ETSGen::FormClassPropReference(const checker::ETSObjectType *classType, const util::StringView &name)
{
    std::stringstream ss;

    auto *iter = classType;
    std::string fullName = classType->AssemblerName().Mutf8();
    while (iter->EnclosingType() != nullptr) {
        auto enclosingName = iter->EnclosingType()->Name().Mutf8().append(".").append(fullName);
        if (iter->EnclosingType()->GetDeclNode()->Type() == ir::AstNodeType::IDENTIFIER) {
            fullName = enclosingName;
        }
        iter = iter->EnclosingType();
    }

    if (fullName != classType->AssemblerName().Mutf8()) {
        fullName.append(".").append(Signatures::ETS_GLOBAL);
    }
    ss << fullName << '.' << name;
    auto res = ProgElement()->Strings().emplace(ss.str());

    return util::StringView(*res.first);
}

util::StringView ETSGen::FormClassPropReference(varbinder::Variable const *const var)
{
    auto containingObjectType = util::Helpers::GetContainingObjectType(var->Declaration()->Node());
    return FormClassPropReference(containingObjectType, var->Name());
}

void ETSGen::StoreStaticOwnProperty(const ir::AstNode *node, const checker::Type *propType,
                                    const util::StringView &name)
{
    util::StringView fullName = FormClassPropReference(containingObjectType_, name);
    StoreStaticProperty(node, propType, fullName);
}

void ETSGen::StoreStaticProperty(const ir::AstNode *const node, const checker::Type *propType,
                                 const util::StringView &fullName)
{
    if (propType->IsETSReferenceType()) {
        Sa().Emit<StstaticObj>(node, fullName);
    } else if (IsWidePrimitiveType(propType)) {
        Sa().Emit<StstaticWide>(node, fullName);
    } else {
        Sa().Emit<Ststatic>(node, fullName);
    }
}

void ETSGen::LoadStaticProperty(const ir::AstNode *const node, const checker::Type *propType,
                                const util::StringView &fullName)
{
    if (propType->IsETSReferenceType()) {
        Sa().Emit<LdstaticObj>(node, fullName);
    } else if (IsWidePrimitiveType(propType)) {
        Sa().Emit<LdstaticWide>(node, fullName);
    } else {
        Sa().Emit<Ldstatic>(node, fullName);
    }

    SetAccumulatorType(propType);
}

void ETSGen::StoreProperty(const ir::AstNode *const node, const checker::Type *propType, const VReg objReg,
                           const util::StringView &name)
{
    auto *objType = Checker()->GetApparentType(GetVRegType(objReg))->AsETSObjectType();
    const auto fullName = FormClassPropReference(objType, name);

    if (propType->IsETSReferenceType()) {
        Ra().Emit<StobjObj>(node, objReg, fullName);
    } else if (IsWidePrimitiveType(propType)) {
        Ra().Emit<StobjWide>(node, objReg, fullName);
    } else {
        Ra().Emit<Stobj>(node, objReg, fullName);
    }
}

void ETSGen::LoadProperty(const ir::AstNode *const node, const checker::Type *propType, const VReg objReg,
                          const util::StringView &fullName)
{
    if (propType->IsETSReferenceType()) {
        Ra().Emit<LdobjObj>(node, objReg, fullName);
    } else if (IsWidePrimitiveType(propType)) {
        Ra().Emit<LdobjWide>(node, objReg, fullName);
    } else {
        Ra().Emit<Ldobj>(node, objReg, fullName);
    }

    SetAccumulatorType(propType);
}

void ETSGen::StoreUnionProperty([[maybe_unused]] const ir::AstNode *node,
                                [[maybe_unused]] const checker::Type *propType, [[maybe_unused]] VReg objReg,
                                [[maybe_unused]] const util::StringView &propName)
{
#ifdef PANDA_WITH_ETS
    if (propType->IsETSReferenceType()) {
        Ra().Emit<EtsStobjNameObj>(node, objReg, propName);
    } else if (IsWidePrimitiveType(propType)) {
        Ra().Emit<EtsStobjNameWide>(node, objReg, propName);
    } else {
        Ra().Emit<EtsStobjName>(node, objReg, propName);
    }
#else
    UNREACHABLE();
#endif  // PANDA_WITH_ETS
}

void ETSGen::LoadUnionProperty([[maybe_unused]] const ir::AstNode *const node,
                               [[maybe_unused]] const checker::Type *propType, [[maybe_unused]] const VReg objReg,
                               [[maybe_unused]] const util::StringView &propName)
{
#ifdef PANDA_WITH_ETS
    if (propType->IsETSReferenceType()) {
        Ra().Emit<EtsLdobjNameObj>(node, objReg, propName);
    } else if (IsWidePrimitiveType(propType)) {
        Ra().Emit<EtsLdobjNameWide>(node, objReg, propName);
    } else {
        Ra().Emit<EtsLdobjName>(node, objReg, propName);
    }
    SetAccumulatorType(propType);
#else
    UNREACHABLE();
#endif  // PANDA_WITH_ETS
}

void ETSGen::StorePropertyDynamic(const ir::AstNode *node, const checker::Type *propType, VReg objReg,
                                  const util::StringView &propName)
{
    auto const lang = GetVRegType(objReg)->AsETSDynamicType()->Language();
    std::string_view methodName {};
    if (propType->IsETSBooleanType()) {
        methodName = Signatures::Dynamic::SetPropertyBooleanBuiltin(lang);
    } else if (propType->IsByteType()) {
        methodName = Signatures::Dynamic::SetPropertyByteBuiltin(lang);
    } else if (propType->IsCharType()) {
        methodName = Signatures::Dynamic::SetPropertyCharBuiltin(lang);
    } else if (propType->IsShortType()) {
        methodName = Signatures::Dynamic::SetPropertyShortBuiltin(lang);
    } else if (propType->IsIntType()) {
        methodName = Signatures::Dynamic::SetPropertyIntBuiltin(lang);
    } else if (propType->IsLongType()) {
        methodName = Signatures::Dynamic::SetPropertyLongBuiltin(lang);
    } else if (propType->IsFloatType()) {
        methodName = Signatures::Dynamic::SetPropertyFloatBuiltin(lang);
    } else if (propType->IsDoubleType()) {
        methodName = Signatures::Dynamic::SetPropertyDoubleBuiltin(lang);
    } else if (propType->IsETSStringType()) {
        methodName = Signatures::Dynamic::SetPropertyStringBuiltin(lang);
    } else if (propType->IsETSObjectType() || propType->IsETSTypeParameter()) {
        methodName = Signatures::Dynamic::SetPropertyDynamicBuiltin(lang);
        // NOTE: vpukhov. add non-dynamic builtin
        if (!propType->IsETSDynamicType()) {
            CastToDynamic(node, Checker()->GlobalBuiltinDynamicType(lang)->AsETSDynamicType());
        }
    } else {
        ASSERT_PRINT(false, "Unsupported property type");
    }

    RegScope rs(this);
    VReg propValueReg = AllocReg();
    VReg propNameReg = AllocReg();

    StoreAccumulator(node, propValueReg);

    // Load property name
    LoadAccumulatorString(node, propName);
    StoreAccumulator(node, propNameReg);

    // Set property by name
    Ra().Emit<Call, 3U>(node, methodName, objReg, propNameReg, propValueReg, dummyReg_);
    SetAccumulatorType(Checker()->GlobalBuiltinJSValueType());
}

void ETSGen::LoadPropertyDynamic(const ir::AstNode *node, const checker::Type *propType, VReg objReg,
                                 const util::StringView &propName)
{
    auto const lang = GetVRegType(objReg)->AsETSDynamicType()->Language();
    auto *type = propType;
    std::string_view methodName {};
    if (propType->IsETSBooleanType()) {
        methodName = Signatures::Dynamic::GetPropertyBooleanBuiltin(lang);
    } else if (propType->IsByteType()) {
        methodName = Signatures::Dynamic::GetPropertyByteBuiltin(lang);
    } else if (propType->IsCharType()) {
        methodName = Signatures::Dynamic::GetPropertyCharBuiltin(lang);
    } else if (propType->IsShortType()) {
        methodName = Signatures::Dynamic::GetPropertyShortBuiltin(lang);
    } else if (propType->IsIntType()) {
        methodName = Signatures::Dynamic::GetPropertyIntBuiltin(lang);
    } else if (propType->IsLongType()) {
        methodName = Signatures::Dynamic::GetPropertyLongBuiltin(lang);
    } else if (propType->IsFloatType()) {
        methodName = Signatures::Dynamic::GetPropertyFloatBuiltin(lang);
    } else if (propType->IsDoubleType()) {
        methodName = Signatures::Dynamic::GetPropertyDoubleBuiltin(lang);
    } else if (propType->IsETSStringType()) {
        methodName = Signatures::Dynamic::GetPropertyStringBuiltin(lang);
    } else if (propType->IsETSObjectType() || propType->IsETSTypeParameter()) {
        methodName = Signatures::Dynamic::GetPropertyDynamicBuiltin(lang);
        type = Checker()->GlobalBuiltinDynamicType(lang);
    } else {
        ASSERT_PRINT(false, "Unsupported property type");
    }

    RegScope rs(this);

    // Load property name
    LoadAccumulatorString(node, propName);
    VReg propNameObject = AllocReg();
    StoreAccumulator(node, propNameObject);

    // Get property by name
    Ra().Emit<CallShort, 2U>(node, methodName, objReg, propNameObject);
    SetAccumulatorType(type);

    if (propType != type && !propType->IsETSDynamicType()) {
        CastDynamicToObject(node, propType);
    }
}

void ETSGen::StoreElementDynamic(const ir::AstNode *node, VReg objectReg, VReg index)
{
    auto const lang = GetVRegType(objectReg)->AsETSDynamicType()->Language();
    std::string_view methodName = Signatures::Dynamic::SetElementDynamicBuiltin(lang);

    RegScope rs(this);

    VReg valueReg = AllocReg();
    StoreAccumulator(node, valueReg);

    // Set property by index
    Ra().Emit<Call, 3U>(node, methodName, objectReg, index, valueReg, dummyReg_);
    SetAccumulatorType(Checker()->GlobalVoidType());
}

void ETSGen::LoadElementDynamic(const ir::AstNode *node, VReg objectReg)
{
    auto const lang = GetVRegType(objectReg)->AsETSDynamicType()->Language();
    std::string_view methodName = Signatures::Dynamic::GetElementDynamicBuiltin(lang);

    RegScope rs(this);

    VReg indexReg = AllocReg();
    StoreAccumulator(node, indexReg);

    // Get property by index
    Ra().Emit<CallShort, 2U>(node, methodName, objectReg, indexReg);
    SetAccumulatorType(Checker()->GlobalBuiltinDynamicType(lang));
}

void ETSGen::LoadUndefinedDynamic(const ir::AstNode *node, Language lang)
{
    RegScope rs(this);
    Ra().Emit<CallShort, 0>(node, Signatures::Dynamic::GetUndefinedBuiltin(lang), dummyReg_, dummyReg_);
    SetAccumulatorType(Checker()->GlobalBuiltinDynamicType(lang));
}

void ETSGen::LoadThis(const ir::AstNode *node)
{
    LoadAccumulator(node, GetThisReg());
}

void ETSGen::CreateBigIntObject(const ir::AstNode *node, VReg arg0, std::string_view signature)
{
    Ra().Emit<InitobjShort>(node, signature, arg0, dummyReg_);
}

VReg ETSGen::GetThisReg() const
{
    const auto res = Scope()->Find(varbinder::VarBinder::MANDATORY_PARAM_THIS);
    return res.variable->AsLocalVariable()->Vreg();
}

const checker::Type *ETSGen::LoadDefaultValue([[maybe_unused]] const ir::AstNode *node,
                                              [[maybe_unused]] const checker::Type *type)
{
    if (type->IsETSAsyncFuncReturnType()) {
        LoadDefaultValue(node, type->AsETSAsyncFuncReturnType()->GetPromiseTypeArg());
        return type;
    }

    if (type->IsETSUnionType()) {
        if (type->AsETSUnionType()->HasUndefinedType()) {
            type = Checker()->GetGlobalTypesHolder()->GlobalETSUndefinedType();
        } else {
            type = Checker()->GetGlobalTypesHolder()->GlobalETSObjectType();
        }
    }
    // NOTE(vpukhov): #19701 void refactoring
    if (type->IsUndefinedType() || type->IsETSUndefinedType() || type->IsETSVoidType()) {
        LoadAccumulatorUndefined(node);
    } else if (type->IsETSObjectType() || type->IsETSArrayType() || type->IsETSTypeParameter() ||
               type->IsETSNullType() || type->IsETSPartialTypeParameter() || type->IsETSNeverType()) {
        // NOTE: need rework about ETSNeverType #20340
        LoadAccumulatorNull(node, type);
    } else if (type->IsETSBooleanType()) {
        LoadAccumulatorBoolean(node, type->AsETSBooleanType()->GetValue());
    } else {
        const auto ttctx = TargetTypeContext(this, type);
        LoadAccumulatorInt(node, 0);
    }

    return type;
}

void ETSGen::EmitReturnVoid(const ir::AstNode *node)
{
    Sa().Emit<ReturnVoid>(node);
}

void ETSGen::ReturnAcc(const ir::AstNode *node)
{
    const auto *const accType = GetAccumulatorType();

    if (accType->IsETSReferenceType()) {
        Sa().Emit<ReturnObj>(node);
    } else if (IsWidePrimitiveType(accType)) {
        Sa().Emit<ReturnWide>(node);
    } else {
        Sa().Emit<Return>(node);
    }
}

static bool IsAnyReferenceSupertype(checker::Type const *type)
{
    if (!type->IsETSUnionType()) {
        return false;
    }
    auto const &constituent = type->AsETSUnionType()->ConstituentTypes();
    return constituent.size() == 3U && std::all_of(constituent.begin(), constituent.end(), [](checker::Type *t) {
               return t->IsETSNullType() || t->IsETSUndefinedType() ||
                      (t->IsETSObjectType() && t->AsETSObjectType()->IsGlobalETSObjectType());
           });  // CC-OFF(G.FMT.02) project code style
}

void ETSGen::IsInstanceDynamic(const ir::BinaryExpression *const node, const VReg srcReg,
                               [[maybe_unused]] const VReg tgtReg)
{
    ASSERT(node->OperatorType() == lexer::TokenType::KEYW_INSTANCEOF);
    const checker::Type *lhsType = node->Left()->TsType();
    const checker::Type *rhsType = node->Right()->TsType();
    ASSERT(rhsType->IsETSDynamicType() || lhsType->IsETSDynamicType());

    const RegScope rs(this);
    if (rhsType->IsETSDynamicType()) {
        ASSERT(node->Right()->TsType()->AsETSDynamicType()->HasDecl());
        if (lhsType->IsETSDynamicType()) {
            VReg dynTypeReg = MoveAccToReg(node);
            // Semantics:
            //      let dyn_val: JSValue = ...
            //      dyn_value instanceof DynamicDecl
            // Bytecode:
            //      call runtime intrinsic_dynamic
            CallExact(node, Signatures::BUILTIN_JSRUNTIME_INSTANCE_OF_DYNAMIC, srcReg, dynTypeReg);
        } else if (lhsType == Checker()->GlobalETSObjectType()) {
            // Semantics:
            //      let obj: Object = ...
            //      obj instanceof DynamicDecl
            // Bytecode:
            //      if isinstance <dynamic type name>:
            //          checkcast <dynamic type name>
            //          return call runtime intrinsic_dynamic
            //      return false
            Label *ifFalse = AllocLabel();
            Language lang = rhsType->AsETSDynamicType()->Language();
            VReg dynTypeReg = MoveAccToReg(node);
            LoadAccumulator(node, srcReg);
            Sa().Emit<Isinstance>(node, Checker()->GlobalBuiltinDynamicType(lang)->AssemblerName());
            BranchIfFalse(node, ifFalse);
            LoadAccumulator(node, srcReg);
            Sa().Emit<Checkcast>(node, Checker()->GlobalBuiltinDynamicType(lang)->AssemblerName());
            CallExact(node, Signatures::BUILTIN_JSRUNTIME_INSTANCE_OF_DYNAMIC, srcReg, dynTypeReg);
            SetLabel(node, ifFalse);
        } else {
            // Semantics:
            //      let obj: EtsType = ...
            //      obj instanceof DynamicDecl
            // Bytecode:
            //      False
            Sa().Emit<Ldai>(node, 0);
        }
    } else {
        if (lhsType->IsETSDynamicType()) {
            if (rhsType == Checker()->GlobalETSObjectType()) {
                // Semantics:
                //      let dyn_val: JSValue = ...
                //      dyn_val instanceof Object
                // Bytecode:
                //      True
                Sa().Emit<Ldai>(node, 1);
            } else {
                // Semantics:
                //      let dyn_val: JSValue = ...
                //      dyn_val instanceof EtsType
                // Bytecode:
                //      lda.type + call runtime instrinsic_static
                Sa().Emit<LdaType>(node, rhsType->AsETSObjectType()->AssemblerName());
                VReg typeReg = MoveAccToReg(node);
                CallExact(node, Signatures::BUILTIN_JSRUNTIME_INSTANCE_OF_STATIC, srcReg, typeReg);
            }
        } else {
            UNREACHABLE();
        }
    }
    SetAccumulatorType(Checker()->GlobalETSBooleanType());
}

void ETSGen::TestIsInstanceConstant(const ir::AstNode *node, Label *ifTrue, VReg srcReg, checker::Type const *target)
{
    if (!target->IsConstantType()) {
        return;
    }
    RegScope rs(this);
    VReg rhs = AllocReg();
    auto ifNotEquals = AllocLabel();

    LoadAccumulator(node, srcReg);
    LoadConstantObject(node->AsExpression(), target);
    StoreAccumulator(node, rhs);
    EmitEtsEquals(node, srcReg, rhs);
    BranchIfFalse(node, ifNotEquals);
    BranchIfTrue(node, ifTrue);
    SetLabel(node, ifNotEquals);
    SetAccumulatorType(nullptr);
}

// Implemented on top of the runtime type system, do not relax checks, do not introduce new types
void ETSGen::TestIsInstanceConstituent(const ir::AstNode *const node, std::tuple<Label *, Label *> label, VReg srcReg,
                                       checker::Type const *target, bool acceptUndefined)
{
    ASSERT(!target->IsETSDynamicType());
    auto [ifTrue, ifFalse] = label;

    if (target->IsConstantType()) {
        TestIsInstanceConstant(node, ifTrue, srcReg, target);
        return;
    }

    switch (checker::ETSChecker::ETSType(target)) {
        case checker::TypeFlag::ETS_NULL: {
            BranchIfNull(node, ifTrue);
            break;
        }
        case checker::TypeFlag::ETS_UNDEFINED: {
            EmitIsUndefined(node);
            BranchIfTrue(node, ifTrue);
            break;
        }
        case checker::TypeFlag::ETS_OBJECT: {
            if (!target->AsETSObjectType()->IsGlobalETSObjectType()) {
                Sa().Emit<Isinstance>(node, ToAssemblerType(target));
                BranchIfTrue(node, ifTrue);
                break;
            }
            if (!acceptUndefined) {
                EmitIsUndefined(node);
                BranchIfTrue(node, ifFalse);
            }
            JumpTo(node, ifTrue);
            break;
        }
        case checker::TypeFlag::ETS_ARRAY: {
            Sa().Emit<Isinstance>(node, ToAssemblerType(target));
            BranchIfTrue(node, ifTrue);
            break;
        }
        default:
            UNREACHABLE();  // other types must not appear here
    }
    SetAccumulatorType(nullptr);
}

// Implemented on top of the runtime type system, do not relax checks, do not introduce new types
void ETSGen::BranchIfIsInstance(const ir::AstNode *const node, const VReg srcReg, const checker::Type *target,
                                Label *ifTrue)
{
    ASSERT(target == Checker()->GetApparentType(target));
    auto ifFalse = AllocLabel();

    bool const allowUndefined = target->PossiblyETSUndefined();
    if (!target->PossiblyETSNull()) {
        LoadAccumulator(node, srcReg);
        BranchIfNull(node, ifFalse);
    }

    auto const checkType = [this, srcReg, ifTrue, ifFalse, allowUndefined](const ir::AstNode *const n,
                                                                           checker::Type const *t) {
        LoadAccumulator(n, srcReg);
        if (t->IsETSTypeAliasType()) {
            TestIsInstanceConstituent(n, std::tie(ifTrue, ifFalse), srcReg, t->AsETSTypeAliasType()->GetTargetType(),
                                      allowUndefined);
        } else {
            TestIsInstanceConstituent(n, std::tie(ifTrue, ifFalse), srcReg, t, allowUndefined);
        }
    };

    if (target->IsETSUnionType()) {
        for (auto *ct : target->AsETSUnionType()->ConstituentTypes()) {
            checkType(node, ct);
        }
    } else if (!target->IsETSNeverType()) {
        checkType(node, target);
    }

    SetLabel(node, ifFalse);
    SetAccumulatorType(nullptr);
}

// Implemented on top of the runtime type system, do not relax checks, do not introduce new types
void ETSGen::IsInstance(const ir::AstNode *const node, const VReg srcReg, const checker::Type *target)
{
    target = Checker()->GetApparentType(target);
    ASSERT(target->IsETSReferenceType());

    if (IsAnyReferenceSupertype(target)) {  // should be IsSupertypeOf(target, source)
        LoadAccumulatorBoolean(node, true);
        return;
    }
    if (target->IsETSArrayType() ||
        (target->IsETSObjectType() &&
         !(target->AsETSObjectType()->IsGlobalETSObjectType() && GetAccumulatorType()->PossiblyETSUndefined()))) {
        InternalIsInstance(node, target);
        return;
    }

    auto ifTrue = AllocLabel();
    auto end = AllocLabel();

    BranchIfIsInstance(node, srcReg, target, ifTrue);
    LoadAccumulatorBoolean(node, false);
    JumpTo(node, end);

    SetLabel(node, ifTrue);
    LoadAccumulatorBoolean(node, true);
    SetLabel(node, end);
}

// isinstance can only be used for Object and [] types, ensure source is not undefined!
void ETSGen::InternalIsInstance(const ir::AstNode *node, const es2panda::checker::Type *target)
{
    ASSERT(target->IsETSObjectType() || target->IsETSArrayType());
    if (!target->IsETSObjectType() || !target->AsETSObjectType()->IsGlobalETSObjectType()) {
        Sa().Emit<Isinstance>(node, ToAssemblerType(target));
        SetAccumulatorType(Checker()->GlobalETSBooleanType());
    } else {
        LoadAccumulatorBoolean(node, true);
    }
}

// checkcast can only be used for Object and [] types, ensure source is not nullish!
void ETSGen::InternalCheckCast(const ir::AstNode *node, const es2panda::checker::Type *target)
{
    ASSERT(target->IsETSObjectType() || target->IsETSArrayType());
    if (!target->IsETSObjectType() || !target->AsETSObjectType()->IsGlobalETSObjectType()) {
        Sa().Emit<Checkcast>(node, ToAssemblerType(target));
    }
    SetAccumulatorType(target);
}

// optimized specialization for object and [] targets
void ETSGen::CheckedReferenceNarrowingObject(const ir::AstNode *node, const checker::Type *target)
{
    ASSERT(target->IsETSObjectType() || target->IsETSArrayType());
    const RegScope rs(this);
    const auto srcReg = AllocReg();
    StoreAccumulator(node, srcReg);

    auto isNullish = AllocLabel();
    auto end = AllocLabel();
    bool nullishCheck = false;

    auto *source = GetAccumulatorType();
    if (source->PossiblyETSNull()) {
        nullishCheck = true;
        BranchIfNull(node, isNullish);
    }
    if (source->PossiblyETSUndefined() && target->IsETSObjectType() &&
        target->AsETSObjectType()->IsGlobalETSObjectType()) {
        nullishCheck = true;
        EmitIsUndefined(node);
        BranchIfTrue(node, isNullish);
    }

    if (!nullishCheck) {
        InternalCheckCast(node, target);
    } else {
        LoadAccumulator(node, srcReg);
        InternalCheckCast(node, target);
        JumpTo(node, end);

        SetLabel(node, isNullish);
        EmitFailedTypeCastException(node, srcReg, target);

        SetLabel(node, end);
        SetAccumulatorType(target);
    }
}

// Implemented on top of the runtime type system, do not relax checks, do not introduce new types
void ETSGen::CheckedReferenceNarrowing(const ir::AstNode *node, const checker::Type *target)
{
    // NOTE(vpukhov): #19701 void refactoring
    if (target->IsETSVoidType()) {
        SetAccumulatorType(target);
        return;
    }

    target = Checker()->GetApparentType(target);
    ASSERT(target->IsETSReferenceType());

    if (IsAnyReferenceSupertype(target)) {  // should be IsSupertypeOf(target, source)
        SetAccumulatorType(target);
        return;
    }
    if (target->HasTypeFlag(checker::TypeFlag::ETS_ARRAY_OR_OBJECT) && !target->IsConstantType()) {
        CheckedReferenceNarrowingObject(node, target);
        return;
    }

    const RegScope rs(this);
    const auto srcReg = AllocReg();
    auto ifTrue = AllocLabel();

    StoreAccumulator(node, srcReg);
    BranchIfIsInstance(node, srcReg, target, ifTrue);

    EmitFailedTypeCastException(node, srcReg, target);

    SetLabel(node, ifTrue);
    LoadAccumulator(node, srcReg);
    // Verifier can't infer type if isinstance met, help him
    Sa().Emit<Checkcast>(node, ToAssemblerType(target));
    SetAccumulatorType(target);
}

void ETSGen::GuardUncheckedType(const ir::AstNode *node, const checker::Type *unchecked, const checker::Type *target)
{
    if (unchecked != nullptr) {
        SetAccumulatorType(unchecked);
        // this check guards possible type violations, **do not relax it**
        CheckedReferenceNarrowing(node, Checker()->MaybeBoxType(target));
    }
    SetAccumulatorType(target);
}

void ETSGen::EmitFailedTypeCastException(const ir::AstNode *node, const VReg src, checker::Type const *target)
{
    const RegScope rs(this);
    const auto errorReg = AllocReg();

    LoadAccumulatorString(node, util::UString(target->ToString(), Allocator()).View());
    Ra().Emit<CallAccShort, 1>(node, Signatures::BUILTIN_RUNTIME_FAILED_TYPE_CAST_EXCEPTION, src, 1);
    StoreAccumulator(node, errorReg);
    EmitThrow(node, errorReg);
    SetAccumulatorType(nullptr);
}

void ETSGen::LoadConstantObject(const ir::Expression *node, const checker::Type *type)
{
    if (type->HasTypeFlag(checker::TypeFlag::BIGINT_LITERAL)) {
        LoadAccumulatorBigInt(node, type->AsETSObjectType()->AsETSBigIntType()->GetValue());
        const VReg value = AllocReg();
        StoreAccumulator(node, value);
        CreateBigIntObject(node, value);
    } else {
        LoadAccumulatorString(node, type->AsETSObjectType()->AsETSStringType()->GetValue());
        SetAccumulatorType(node->TsType());
    }
}

bool ETSGen::TryLoadConstantExpression(const ir::Expression *node)
{
    const auto *type = node->TsType();

    if (!type->HasTypeFlag(checker::TypeFlag::CONSTANT) || type->IsETSObjectType()) {
        return false;
    }
    // bug: this should be forbidden for most expression types!

    auto typeKind = checker::ETSChecker::TypeKind(type);

    switch (typeKind) {
        case checker::TypeFlag::CHAR: {
            LoadAccumulatorChar(node, type->AsCharType()->GetValue());
            break;
        }
        case checker::TypeFlag::ETS_BOOLEAN: {
            LoadAccumulatorBoolean(node, type->AsETSBooleanType()->GetValue());
            break;
        }
        case checker::TypeFlag::BYTE: {
            LoadAccumulatorByte(node, type->AsByteType()->GetValue());
            break;
        }
        case checker::TypeFlag::SHORT: {
            LoadAccumulatorShort(node, type->AsShortType()->GetValue());
            break;
        }
        case checker::TypeFlag::INT: {
            LoadAccumulatorInt(node, type->AsIntType()->GetValue());
            break;
        }
        case checker::TypeFlag::LONG: {
            LoadAccumulatorWideInt(node, type->AsLongType()->GetValue());
            break;
        }
        case checker::TypeFlag::FLOAT: {
            LoadAccumulatorFloat(node, type->AsFloatType()->GetValue());
            break;
        }
        case checker::TypeFlag::DOUBLE: {
            LoadAccumulatorDouble(node, type->AsDoubleType()->GetValue());
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    return true;
}

void ETSGen::ApplyConversionCast(const ir::AstNode *node, const checker::Type *targetType)
{
    switch (checker::ETSChecker::TypeKind(targetType)) {
        case checker::TypeFlag::DOUBLE: {
            CastToDouble(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            CastToFloat(node);
            break;
        }
        case checker::TypeFlag::LONG: {
            CastToLong(node);
            break;
        }
        case checker::TypeFlag::CHAR: {
            CastToChar(node);
            break;
        }
        case checker::TypeFlag::ETS_ARRAY:
        case checker::TypeFlag::ETS_OBJECT:
        case checker::TypeFlag::ETS_TYPE_PARAMETER: {
            if (GetAccumulatorType() != nullptr && GetAccumulatorType()->IsETSDynamicType()) {
                CastDynamicToObject(node, targetType);
            }
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            CastToDynamic(node, targetType->AsETSDynamicType());
            break;
        }
        default: {
            break;
        }
    }
}

void ETSGen::ApplyBoxingConversion(const ir::AstNode *node)
{
    EmitBoxingConversion(node);
    node->SetBoxingUnboxingFlags(
        static_cast<ir::BoxingUnboxingFlags>(node->GetBoxingUnboxingFlags() & ~(ir::BoxingUnboxingFlags::BOXING_FLAG)));
}

static checker::Type const *GetUnboxedTypeFromContext(checker::ETSChecker const *checker, const ir::AstNode *node)
{
    // A consequence of current enum type implementation, #20510
    checker::Type const *type;
    if (node->IsExpression()) {
        if (node->Parent()->IsTSAsExpression()) {
            type = node->Parent()->AsTSAsExpression()->TsType();
        } else {
            type = node->AsExpression()->TsType();
        }
    } else {
        UNREACHABLE();
    }
    type = checker->MaybeUnboxType(const_cast<checker::Type *>(type));
    ASSERT(!type->IsETSReferenceType());
    return type;
}

void ETSGen::ApplyUnboxingConversion(const ir::AstNode *node)
{
    auto const callUnbox = [this, node](std::string_view sig, checker::Type const *unboxedType) {
        auto boxedType = Checker()->MaybeBoxType(unboxedType)->AsETSObjectType();
        EmitUnboxedCall(node, sig, unboxedType, boxedType);
    };

    auto const unboxFlags =
        ir::BoxingUnboxingFlags(node->GetBoxingUnboxingFlags() & ir::BoxingUnboxingFlags::UNBOXING_FLAG);
    node->RemoveBoxingUnboxingFlags(ir::BoxingUnboxingFlags::UNBOXING_FLAG);

    switch (unboxFlags) {
        case ir::BoxingUnboxingFlags::UNBOX_TO_BOOLEAN:
            callUnbox(Signatures::BUILTIN_BOOLEAN_UNBOXED, Checker()->GlobalETSBooleanType());
            return;
        case ir::BoxingUnboxingFlags::UNBOX_TO_BYTE:
            callUnbox(Signatures::BUILTIN_BYTE_UNBOXED, Checker()->GlobalByteType());
            return;
        case ir::BoxingUnboxingFlags::UNBOX_TO_CHAR:
            callUnbox(Signatures::BUILTIN_CHAR_UNBOXED, Checker()->GlobalCharType());
            return;
        case ir::BoxingUnboxingFlags::UNBOX_TO_SHORT:
            callUnbox(Signatures::BUILTIN_SHORT_UNBOXED, Checker()->GlobalShortType());
            return;
        case ir::BoxingUnboxingFlags::UNBOX_TO_INT:
            callUnbox(Signatures::BUILTIN_INT_UNBOXED, Checker()->GlobalIntType());
            return;
        case ir::BoxingUnboxingFlags::UNBOX_TO_LONG:
            callUnbox(Signatures::BUILTIN_LONG_UNBOXED, Checker()->GlobalLongType());
            return;
        case ir::BoxingUnboxingFlags::UNBOX_TO_FLOAT:
            callUnbox(Signatures::BUILTIN_FLOAT_UNBOXED, Checker()->GlobalFloatType());
            return;
        case ir::BoxingUnboxingFlags::UNBOX_TO_DOUBLE:
            callUnbox(Signatures::BUILTIN_DOUBLE_UNBOXED, Checker()->GlobalDoubleType());
            return;
        case ir::BoxingUnboxingFlags::UNBOX_TO_ENUM: {
            auto enumType = GetUnboxedTypeFromContext(Checker(), node)->AsETSEnumType();
            callUnbox(enumType->UnboxMethod().globalSignature->InternalName().Utf8(), enumType);
            return;
        }
        default:
            UNREACHABLE();
    }
}

void ETSGen::ApplyConversion(const ir::AstNode *node, const checker::Type *targetType)
{
    auto ttctx = TargetTypeContext(this, targetType);

    if ((node->GetBoxingUnboxingFlags() & ir::BoxingUnboxingFlags::BOXING_FLAG) != 0U) {
        ApplyBoxingConversion(node);

        if (node->HasAstNodeFlags(ir::AstNodeFlags::CONVERT_TO_STRING)) {
            CastToString(node);
            node->RemoveAstNodeFlags(ir::AstNodeFlags::CONVERT_TO_STRING);
        }

        return;
    }

    if ((node->GetBoxingUnboxingFlags() & ir::BoxingUnboxingFlags::UNBOXING_FLAG) != 0U) {
        ApplyUnboxingConversion(node);
    }

    if (targetType == nullptr) {
        return;
    }

    ApplyConversionCast(node, targetType);
}

void ETSGen::ApplyCast(const ir::AstNode *node, const checker::Type *targetType)
{
    auto typeKind = checker::ETSChecker::TypeKind(targetType);

    switch (typeKind) {
        case checker::TypeFlag::DOUBLE: {
            CastToDouble(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            CastToFloat(node);
            break;
        }
        case checker::TypeFlag::LONG: {
            CastToLong(node);
            break;
        }
        case checker::TypeFlag::INT: {
            CastToInt(node);
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            CastToDynamic(node, targetType->AsETSDynamicType());
            break;
        }
        default: {
            break;
        }
    }
}

void ETSGen::ApplyCastToBoxingFlags(const ir::AstNode *node, const ir::BoxingUnboxingFlags targetType)
{
    switch (targetType) {
        case ir::BoxingUnboxingFlags::BOX_TO_DOUBLE: {
            CastToDouble(node);
            break;
        }
        case ir::BoxingUnboxingFlags::BOX_TO_FLOAT: {
            CastToFloat(node);
            break;
        }
        case ir::BoxingUnboxingFlags::BOX_TO_LONG: {
            CastToLong(node);
            break;
        }
        case ir::BoxingUnboxingFlags::BOX_TO_INT: {
            CastToInt(node);
            break;
        }
        default: {
            break;
        }
    }
}

void ETSGen::EmitUnboxedCall(const ir::AstNode *node, std::string_view signatureFlag,
                             const checker::Type *const targetType, const checker::Type *const boxedType)
{
    RegScope rs(this);
    // NOTE(vpukhov): #20510 lowering
    if (node->HasAstNodeFlags(ir::AstNodeFlags::CHECKCAST)) {
        CheckedReferenceNarrowing(node, boxedType);
    }

    // to cast to primitive types we probably have to cast to corresponding boxed built-in types first.
    auto *const checker = Checker()->AsETSChecker();
    auto const *accumulatorType = GetAccumulatorType();
    if (accumulatorType->IsETSObjectType() &&  //! accumulatorType->DefinitelyNotETSNullish() &&
        !checker->Relation()->IsIdenticalTo(const_cast<checker::Type *>(accumulatorType),
                                            const_cast<checker::Type *>(boxedType))) {
        CastToReftype(node, boxedType, false);
    }

    Ra().Emit<CallAccShort, 0>(node, signatureFlag, dummyReg_, 0);
    SetAccumulatorType(targetType);
    if (node->IsExpression()) {
        const_cast<ir::Expression *>(node->AsExpression())->SetTsType(const_cast<checker::Type *>(targetType));
    }
}

// NOTE(vpukhov): #20510 should be available only as a part of ApplyBoxingConversion
void ETSGen::EmitBoxingConversion(ir::BoxingUnboxingFlags boxingFlag, const ir::AstNode *node)
{
    auto const callBox = [this, node](std::string_view sig, checker::Type const *unboxedType) {
        Ra().Emit<CallAccShort, 0>(node, sig, dummyReg_, 0);
        SetAccumulatorType(Checker()->MaybeBoxType(unboxedType)->AsETSObjectType());
    };

    switch (boxingFlag) {
        case ir::BoxingUnboxingFlags::BOX_TO_BOOLEAN:
            callBox(Signatures::BUILTIN_BOOLEAN_VALUE_OF, Checker()->GlobalETSBooleanType());
            return;
        case ir::BoxingUnboxingFlags::BOX_TO_BYTE:
            callBox(Signatures::BUILTIN_BYTE_VALUE_OF, Checker()->GlobalByteType());
            return;
        case ir::BoxingUnboxingFlags::BOX_TO_CHAR:
            callBox(Signatures::BUILTIN_CHAR_VALUE_OF, Checker()->GlobalCharType());
            return;
        case ir::BoxingUnboxingFlags::BOX_TO_SHORT:
            callBox(Signatures::BUILTIN_SHORT_VALUE_OF, Checker()->GlobalShortType());
            return;
        case ir::BoxingUnboxingFlags::BOX_TO_INT:
            callBox(Signatures::BUILTIN_INT_VALUE_OF, Checker()->GlobalIntType());
            return;
        case ir::BoxingUnboxingFlags::BOX_TO_LONG:
            callBox(Signatures::BUILTIN_LONG_VALUE_OF, Checker()->GlobalLongType());
            return;
        case ir::BoxingUnboxingFlags::BOX_TO_FLOAT:
            callBox(Signatures::BUILTIN_FLOAT_VALUE_OF, Checker()->GlobalFloatType());
            return;
        case ir::BoxingUnboxingFlags::BOX_TO_DOUBLE:
            callBox(Signatures::BUILTIN_DOUBLE_VALUE_OF, Checker()->GlobalDoubleType());
            return;
        case ir::BoxingUnboxingFlags::BOX_TO_ENUM: {
            const auto *const enumType = node->AsExpression()->TsType()->AsETSEnumType();
            callBox(enumType->BoxedFromIntMethod().globalSignature->InternalName().Utf8(), enumType);
            return;
        }
        default:
            UNREACHABLE();
    }
}

// NOTE(vpukhov): #20510 should be available only as a part of ApplyBoxingConversion
void ETSGen::EmitBoxingConversion(const ir::AstNode *node)
{
    auto boxingFlag =
        static_cast<ir::BoxingUnboxingFlags>(ir::BoxingUnboxingFlags::BOXING_FLAG & node->GetBoxingUnboxingFlags());

    RegScope rs(this);

    ApplyCastToBoxingFlags(node, boxingFlag);

    EmitBoxingConversion(boxingFlag, node);

    if (node->IsExpression()) {
        auto boxedType = const_cast<checker::Type *>(GetAccumulatorType());
        const_cast<ir::Expression *>(node->AsExpression())->SetTsType(boxedType);
    }
}

void ETSGen::SwapBinaryOpArgs(const ir::AstNode *const node, const VReg lhs)
{
    const RegScope rs(this);
    const auto tmp = AllocReg();

    StoreAccumulator(node, tmp);
    LoadAccumulator(node, lhs);
    MoveVreg(node, lhs, tmp);
}

VReg ETSGen::MoveAccToReg(const ir::AstNode *const node)
{
    const auto newReg = AllocReg();
    StoreAccumulator(node, newReg);
    return newReg;
}

void ETSGen::CastToBoolean([[maybe_unused]] const ir::AstNode *node)
{
    auto typeKind = checker::ETSChecker::TypeKind(GetAccumulatorType());
    switch (typeKind) {
        case checker::TypeFlag::ETS_BOOLEAN: {
            return;
        }
        case checker::TypeFlag::CHAR: {
            Sa().Emit<U32tou1>(node);
            break;
        }
        case checker::TypeFlag::BYTE:
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::INT: {
            Sa().Emit<I32tou1>(node);
            return;
        }
        case checker::TypeFlag::LONG: {
            Sa().Emit<I64tou1>(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Sa().Emit<F32toi32>(node);
            Sa().Emit<I32tou1>(node);
            break;
        }
        case checker::TypeFlag::DOUBLE: {
            Sa().Emit<F64toi32>(node);
            Sa().Emit<I32tou1>(node);
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            CastDynamicTo(node, checker::TypeFlag::ETS_BOOLEAN);
            ASSERT(GetAccumulatorType() == Checker()->GlobalETSBooleanType());
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    SetAccumulatorType(Checker()->GlobalETSBooleanType());
}

void ETSGen::CastToByte([[maybe_unused]] const ir::AstNode *node)
{
    auto typeKind = checker::ETSChecker::TypeKind(GetAccumulatorType());
    switch (typeKind) {
        case checker::TypeFlag::BYTE: {
            return;
        }
        case checker::TypeFlag::ETS_BOOLEAN:
        case checker::TypeFlag::CHAR: {
            Sa().Emit<U32toi8>(node);
            break;
        }
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::INT: {
            Sa().Emit<I32toi8>(node);
            break;
        }
        case checker::TypeFlag::LONG: {
            Sa().Emit<I64toi32>(node);
            Sa().Emit<I32toi8>(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Sa().Emit<F32toi32>(node);
            Sa().Emit<I32toi8>(node);
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            CastDynamicTo(node, checker::TypeFlag::DOUBLE);
            ASSERT(GetAccumulatorType() == Checker()->GlobalDoubleType());
            [[fallthrough]];
        }
        case checker::TypeFlag::DOUBLE: {
            Sa().Emit<F64toi32>(node);
            Sa().Emit<I32toi8>(node);
            break;
        }
        case checker::TypeFlag::ETS_OBJECT: {
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    SetAccumulatorType(Checker()->GlobalByteType());
}

void ETSGen::CastToChar([[maybe_unused]] const ir::AstNode *node)
{
    auto typeKind = checker::ETSChecker::TypeKind(GetAccumulatorType());
    switch (typeKind) {
        case checker::TypeFlag::CHAR: {
            return;
        }
        case checker::TypeFlag::ETS_BOOLEAN: {
            break;
        }
        case checker::TypeFlag::BYTE:
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::INT: {
            Sa().Emit<I32tou16>(node);
            break;
        }
        case checker::TypeFlag::LONG: {
            Sa().Emit<I64toi32>(node);
            Sa().Emit<I32tou16>(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Sa().Emit<F32toi32>(node);
            Sa().Emit<I32tou16>(node);
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            CastDynamicTo(node, checker::TypeFlag::DOUBLE);
            ASSERT(GetAccumulatorType() == Checker()->GlobalDoubleType());
            [[fallthrough]];
        }
        case checker::TypeFlag::DOUBLE: {
            Sa().Emit<F64toi32>(node);
            Sa().Emit<I32tou16>(node);
            break;
        }
        case checker::TypeFlag::ETS_OBJECT: {
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    SetAccumulatorType(Checker()->GlobalCharType());
}

void ETSGen::CastToShort([[maybe_unused]] const ir::AstNode *node)
{
    auto typeKind = checker::ETSChecker::TypeKind(GetAccumulatorType());
    switch (typeKind) {
        case checker::TypeFlag::SHORT: {
            return;
        }
        case checker::TypeFlag::ETS_BOOLEAN:
        case checker::TypeFlag::CHAR: {
            Sa().Emit<U32toi16>(node);
            break;
        }
        case checker::TypeFlag::BYTE: {
            break;
        }
        case checker::TypeFlag::INT: {
            Sa().Emit<I32toi16>(node);
            break;
        }
        case checker::TypeFlag::LONG: {
            Sa().Emit<I64toi32>(node);
            Sa().Emit<I32toi16>(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Sa().Emit<F32toi32>(node);
            Sa().Emit<I32toi16>(node);
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            CastDynamicTo(node, checker::TypeFlag::DOUBLE);
            ASSERT(GetAccumulatorType() == Checker()->GlobalDoubleType());
            [[fallthrough]];
        }
        case checker::TypeFlag::DOUBLE: {
            Sa().Emit<F64toi32>(node);
            Sa().Emit<I32toi16>(node);
            break;
        }
        case checker::TypeFlag::ETS_OBJECT: {
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    SetAccumulatorType(Checker()->GlobalShortType());
}

void ETSGen::CastToDouble(const ir::AstNode *node)
{
    auto typeKind = checker::ETSChecker::TypeKind(GetAccumulatorType());
    switch (typeKind) {
        case checker::TypeFlag::DOUBLE: {
            return;
        }
        case checker::TypeFlag::ETS_BOOLEAN:
        case checker::TypeFlag::CHAR: {
            Sa().Emit<U32tof64>(node);
            break;
        }
        case checker::TypeFlag::BYTE:
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::INT: {
            Sa().Emit<I32tof64>(node);
            break;
        }
        case checker::TypeFlag::LONG: {
            Sa().Emit<I64tof64>(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Sa().Emit<F32tof64>(node);
            break;
        }
        case checker::TypeFlag::ETS_NEVER:
        case checker::TypeFlag::ETS_OBJECT: {
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            CastDynamicTo(node, checker::TypeFlag::DOUBLE);
            ASSERT(GetAccumulatorType() == Checker()->GlobalDoubleType());
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    SetAccumulatorType(Checker()->GlobalDoubleType());
}

void ETSGen::CastToFloat(const ir::AstNode *node)
{
    auto typeKind = checker::ETSChecker::TypeKind(GetAccumulatorType());
    switch (typeKind) {
        case checker::TypeFlag::FLOAT: {
            return;
        }
        case checker::TypeFlag::ETS_BOOLEAN:
        case checker::TypeFlag::CHAR: {
            Sa().Emit<U32tof32>(node);
            break;
        }
        case checker::TypeFlag::BYTE:
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::INT: {
            Sa().Emit<I32tof32>(node);
            break;
        }
        case checker::TypeFlag::LONG: {
            Sa().Emit<I64tof32>(node);
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            CastDynamicTo(node, checker::TypeFlag::DOUBLE);
            ASSERT(GetAccumulatorType() == Checker()->GlobalDoubleType());
            [[fallthrough]];
        }
        case checker::TypeFlag::DOUBLE: {
            Sa().Emit<F64tof32>(node);
            break;
        }
        case checker::TypeFlag::ETS_OBJECT: {
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    SetAccumulatorType(Checker()->GlobalFloatType());
}

void ETSGen::CastToLong(const ir::AstNode *node)
{
    auto typeKind = checker::ETSChecker::TypeKind(GetAccumulatorType());
    switch (typeKind) {
        case checker::TypeFlag::LONG: {
            return;
        }
        case checker::TypeFlag::ETS_BOOLEAN:
        case checker::TypeFlag::CHAR: {
            Sa().Emit<U32toi64>(node);
            break;
        }
        case checker::TypeFlag::BYTE:
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::INT: {
            Sa().Emit<I32toi64>(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Sa().Emit<F32toi64>(node);
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            CastDynamicTo(node, checker::TypeFlag::DOUBLE);
            ASSERT(GetAccumulatorType() == Checker()->GlobalDoubleType());
            [[fallthrough]];
        }
        case checker::TypeFlag::DOUBLE: {
            Sa().Emit<F64toi64>(node);
            break;
        }
        case checker::TypeFlag::ETS_OBJECT: {
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    SetAccumulatorType(Checker()->GlobalLongType());
}

void ETSGen::CastToInt(const ir::AstNode *node)
{
    auto typeKind = checker::ETSChecker::TypeKind(GetAccumulatorType());
    switch (typeKind) {
        case checker::TypeFlag::INT: {
            return;
        }
        case checker::TypeFlag::ETS_BOOLEAN:
        case checker::TypeFlag::CHAR:
        case checker::TypeFlag::ETS_INT_ENUM:
        case checker::TypeFlag::ETS_STRING_ENUM:
        case checker::TypeFlag::BYTE:
        case checker::TypeFlag::SHORT: {
            break;
        }
        case checker::TypeFlag::LONG: {
            Sa().Emit<I64toi32>(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Sa().Emit<F32toi32>(node);
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            CastDynamicTo(node, checker::TypeFlag::DOUBLE);
            ASSERT(GetAccumulatorType() == Checker()->GlobalDoubleType());
            [[fallthrough]];
        }
        case checker::TypeFlag::DOUBLE: {
            Sa().Emit<F64toi32>(node);
            break;
        }
        case checker::TypeFlag::ETS_OBJECT: {
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    SetAccumulatorType(Checker()->GlobalIntType());
}

void ETSGen::CastToReftype(const ir::AstNode *const node, const checker::Type *const targetType, const bool unchecked)
{
    ASSERT(GetAccumulatorType()->IsETSReferenceType());

    const auto *const sourceType = GetAccumulatorType();

    if (sourceType->IsETSDynamicType()) {
        CastDynamicToObject(node, targetType);
        return;
    }
    if (targetType->IsETSDynamicType()) {
        CastToDynamic(node, targetType->AsETSDynamicType());
        return;
    }

    if (targetType->IsETSStringType() && !sourceType->IsETSStringType()) {
        CastToString(node);
    }

    if (!unchecked) {
        CheckedReferenceNarrowing(node, targetType);
        return;
    }

    ASSERT(!targetType->IsETSTypeParameter() && !targetType->IsETSNonNullishType() &&
           !targetType->IsETSPartialTypeParameter());
    CheckedReferenceNarrowing(node, targetType);
    SetAccumulatorType(targetType);
}

void ETSGen::CastDynamicToObject(const ir::AstNode *node, const checker::Type *targetType)
{
    if (targetType->IsETSStringType()) {
        CastDynamicTo(node, checker::TypeFlag::STRING);
        return;
    }

    // NOTE(vpukhov): #14626 remove, replace targetType with interface
    if (targetType->IsLambdaObject()) {
        RegScope rs(this);
        VReg dynObjReg = AllocReg();
        StoreAccumulator(node, dynObjReg);
        Ra().Emit<InitobjShort>(node, targetType->AsETSObjectType()->ConstructSignatures()[0]->InternalName(),
                                dynObjReg, dummyReg_);
        SetAccumulatorType(targetType);
        return;
    }

    if (targetType == Checker()->GlobalETSObjectType()) {
        SetAccumulatorType(targetType);
        return;
    }

    if (targetType->IsETSDynamicType()) {
        SetAccumulatorType(targetType);
        return;
    }

    // should be valid only for Object and [] types, other are workarounds
    if (targetType->IsETSArrayType() || targetType->IsETSObjectType() || targetType->IsETSTypeParameter() ||
        targetType->IsETSUnionType()) {
        auto lang = GetAccumulatorType()->AsETSDynamicType()->Language();
        auto methodName = compiler::Signatures::Dynamic::GetObjectBuiltin(lang);

        RegScope rs(this);
        VReg dynObjReg = AllocReg();
        StoreAccumulator(node, dynObjReg);

        // try internal checkcast
        VReg typeReg = AllocReg();
        auto assemblerType = ToAssemblerType(targetType);
        Sa().Emit<LdaType>(node, assemblerType);
        StoreAccumulator(node, typeReg);

        Ra().Emit<CallShort, 2U>(node, methodName, dynObjReg, typeReg);
        Sa().Emit<Checkcast>(node, assemblerType);  // trick verifier
        SetAccumulatorType(targetType);
        return;
    }

    UNREACHABLE();
}

void ETSGen::CastToString(const ir::AstNode *const node)
{
    const auto *const sourceType = GetAccumulatorType();
    if (sourceType->IsETSPrimitiveType()) {
        EmitBoxingConversion(node);
    } else {
        ASSERT(sourceType->IsETSReferenceType());
    }
    // caller must ensure parameter is not null
    Ra().Emit<CallVirtAccShort, 0>(node, Signatures::BUILTIN_OBJECT_TO_STRING, dummyReg_, 0);
    SetAccumulatorType(Checker()->GetGlobalTypesHolder()->GlobalETSStringBuiltinType());
}

void ETSGen::CastToDynamic(const ir::AstNode *node, const checker::ETSDynamicType *type)
{
    std::string_view methodName {};
    auto typeKind = checker::ETSChecker::TypeKind(GetAccumulatorType());
    switch (typeKind) {
        case checker::TypeFlag::ETS_BOOLEAN: {
            methodName = compiler::Signatures::Dynamic::NewBooleanBuiltin(type->Language());
            break;
        }
        case checker::TypeFlag::CHAR:
        case checker::TypeFlag::BYTE:
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::INT:
        case checker::TypeFlag::LONG:
        case checker::TypeFlag::FLOAT:
        case checker::TypeFlag::DOUBLE: {
            CastToDouble(node);
            methodName = compiler::Signatures::Dynamic::NewDoubleBuiltin(type->Language());
            break;
        }
        case checker::TypeFlag::ETS_OBJECT:
        case checker::TypeFlag::ETS_TYPE_PARAMETER:
        case checker::TypeFlag::ETS_NONNULLISH:
        case checker::TypeFlag::ETS_PARTIAL_TYPE_PARAMETER:
        case checker::TypeFlag::ETS_UNION: {  // NOTE(vpukhov): refine dynamic type cast rules
            if (GetAccumulatorType()->IsETSStringType()) {
                methodName = compiler::Signatures::Dynamic::NewStringBuiltin(type->Language());
                break;
            }
            [[fallthrough]];
        }
        case checker::TypeFlag::ETS_ARRAY: {
            methodName = compiler::Signatures::Dynamic::NewObjectBuiltin(type->Language());
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            SetAccumulatorType(type);
            return;
        }
        default: {
            UNREACHABLE();
        }
    }

    ASSERT(!methodName.empty());

    RegScope rs(this);
    // Load value
    VReg valReg = AllocReg();
    StoreAccumulator(node, valReg);

    // Create new JSValue and initialize it
    Ra().Emit<CallShort, 1>(node, methodName, valReg, dummyReg_);
    SetAccumulatorType(Checker()->GlobalBuiltinDynamicType(type->Language()));
}

void ETSGen::CastDynamicTo(const ir::AstNode *node, enum checker::TypeFlag typeFlag)
{
    std::string_view methodName {};
    checker::Type *objectType {};
    auto type = GetAccumulatorType()->AsETSDynamicType();
    switch (typeFlag) {
        case checker::TypeFlag::ETS_BOOLEAN: {
            methodName = compiler::Signatures::Dynamic::GetBooleanBuiltin(type->Language());
            objectType = Checker()->GlobalETSBooleanType();
            break;
        }
        case checker::TypeFlag::DOUBLE: {
            methodName = compiler::Signatures::Dynamic::GetDoubleBuiltin(type->Language());
            objectType = Checker()->GlobalDoubleType();
            break;
        }
        case checker::TypeFlag::STRING: {
            methodName = compiler::Signatures::Dynamic::GetStringBuiltin(type->Language());
            objectType = Checker()->GlobalBuiltinETSStringType();
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    RegScope rs(this);
    // Load dynamic object
    VReg dynObjReg = AllocReg();
    StoreAccumulator(node, dynObjReg);

    // Get value from dynamic object
    Ra().Emit<CallShort, 1>(node, methodName, dynObjReg, dummyReg_);
    SetAccumulatorType(objectType);
}

void ETSGen::ToBinaryResult(const ir::AstNode *node, Label *ifFalse)
{
    Label *end = AllocLabel();
    Sa().Emit<Ldai>(node, 1);
    Sa().Emit<Jmp>(node, end);
    SetLabel(node, ifFalse);
    Sa().Emit<Ldai>(node, 0);
    SetLabel(node, end);
    SetAccumulatorType(Checker()->GlobalETSBooleanType());
}

void ETSGen::BinaryLogic(const ir::AstNode *node, lexer::TokenType op, VReg lhs)
{
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_MOD:
        case lexer::TokenType::PUNCTUATOR_MOD_EQUAL: {
            SwapBinaryOpArgs(node, lhs);
            BinaryArithmetic<Mod2, Mod2Wide, Fmod2, Fmod2Wide>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_LEFT_SHIFT:
        case lexer::TokenType::PUNCTUATOR_LEFT_SHIFT_EQUAL: {
            SwapBinaryOpArgs(node, lhs);
            BinaryBitwiseArithmetic<Shl2, Shl2Wide>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_RIGHT_SHIFT:
        case lexer::TokenType::PUNCTUATOR_RIGHT_SHIFT_EQUAL: {
            SwapBinaryOpArgs(node, lhs);
            BinaryBitwiseArithmetic<Ashr2, Ashr2Wide>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT:
        case lexer::TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT_EQUAL: {
            SwapBinaryOpArgs(node, lhs);
            BinaryBitwiseArithmetic<Shr2, Shr2Wide>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_BITWISE_AND:
        case lexer::TokenType::PUNCTUATOR_BITWISE_AND_EQUAL: {
            BinaryBitwiseArithmetic<And2, And2Wide>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_BITWISE_OR:
        case lexer::TokenType::PUNCTUATOR_BITWISE_OR_EQUAL: {
            BinaryBitwiseArithmetic<Or2, Or2Wide>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_BITWISE_XOR:
        case lexer::TokenType::PUNCTUATOR_BITWISE_XOR_EQUAL: {
            BinaryBitwiseArithmetic<Xor2, Xor2Wide>(node, lhs);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
    ASSERT(node->IsAssignmentExpression() || node->IsBinaryExpression());
    ASSERT(Checker()->Relation()->IsIdenticalTo(const_cast<checker::Type *>(GetAccumulatorType()),
                                                const_cast<checker::Type *>(node->AsExpression()->TsType())));
}

void ETSGen::BinaryArithmLogic(const ir::AstNode *node, lexer::TokenType op, VReg lhs)
{
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_PLUS:
        case lexer::TokenType::PUNCTUATOR_PLUS_EQUAL: {
            SwapBinaryOpArgs(node, lhs);
            BinaryArithmetic<Add2, Add2Wide, Fadd2, Fadd2Wide>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_MINUS:
        case lexer::TokenType::PUNCTUATOR_MINUS_EQUAL: {
            SwapBinaryOpArgs(node, lhs);
            BinaryArithmetic<Sub2, Sub2Wide, Fsub2, Fsub2Wide>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_MULTIPLY:
        case lexer::TokenType::PUNCTUATOR_MULTIPLY_EQUAL: {
            SwapBinaryOpArgs(node, lhs);
            BinaryArithmetic<Mul2, Mul2Wide, Fmul2, Fmul2Wide>(node, lhs);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_DIVIDE:
        case lexer::TokenType::PUNCTUATOR_DIVIDE_EQUAL: {
            SwapBinaryOpArgs(node, lhs);
            BinaryArithmetic<Div2, Div2Wide, Fdiv2, Fdiv2Wide>(node, lhs);
            break;
        }
        default: {
            BinaryLogic(node, op, lhs);
            break;
        }
    }
    ASSERT(node->IsAssignmentExpression() || node->IsBinaryExpression());
    ASSERT(Checker()->Relation()->IsIdenticalTo(const_cast<checker::Type *>(GetAccumulatorType()),
                                                const_cast<checker::Type *>(node->AsExpression()->TsType())));
}

void ETSGen::Binary(const ir::AstNode *node, lexer::TokenType op, VReg lhs)
{
    Label *ifFalse = AllocLabel();
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_EQUAL: {
            BinaryEquality<JneObj, Jne, Jnez, Jeqz>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_NOT_EQUAL: {
            BinaryEquality<JeqObj, Jeq, Jeqz, Jnez>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_STRICT_EQUAL: {
            RefEqualityStrict<JneObj, Jeqz>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_NOT_STRICT_EQUAL: {
            RefEqualityStrict<JeqObj, Jnez>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_LESS_THAN: {
            BinaryRelation<Jle, Jlez>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL: {
            BinaryRelation<Jlt, Jltz>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN: {
            BinaryRelation<Jge, Jgez>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL: {
            BinaryRelation<Jgt, Jgtz>(node, lhs, ifFalse);
            break;
        }
        default: {
            BinaryArithmLogic(node, op, lhs);
            break;
        }
    }
    ASSERT(node->IsAssignmentExpression() || node->IsBinaryExpression());
    ASSERT(Checker()->Relation()->IsIdenticalTo(const_cast<checker::Type *>(GetAccumulatorType()),
                                                const_cast<checker::Type *>(node->AsExpression()->TsType())));
}

void ETSGen::Condition(const ir::AstNode *node, lexer::TokenType op, VReg lhs, Label *ifFalse)
{
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_EQUAL: {
            BinaryEqualityCondition<JneObj, Jne, Jnez>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_NOT_EQUAL: {
            BinaryEqualityCondition<JeqObj, Jeq, Jeqz>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_LESS_THAN: {
            BinaryRelationCondition<Jle, Jlez>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL: {
            BinaryRelationCondition<Jlt, Jltz>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN: {
            BinaryRelationCondition<Jge, Jgez>(node, lhs, ifFalse);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL: {
            BinaryRelationCondition<Jgt, Jgtz>(node, lhs, ifFalse);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void ETSGen::BranchIfNullish([[maybe_unused]] const ir::AstNode *node, [[maybe_unused]] Label *ifNullish)
{
    auto *const type = GetAccumulatorType();

    if (type->DefinitelyNotETSNullish()) {
        // no action
    } else if (type->DefinitelyETSNullish()) {
        Sa().Emit<Jmp>(node, ifNullish);
    } else if (!type->PossiblyETSUndefined()) {
        Sa().Emit<JeqzObj>(node, ifNullish);
    } else {
        RegScope rs(this);
        auto tmpObj = AllocReg();
        auto notTaken = AllocLabel();

        if (type->PossiblyETSNull()) {
            Sa().Emit<JeqzObj>(node, ifNullish);
        }

        Sa().Emit<StaObj>(node, tmpObj);
        EmitIsUndefined(node);
        Sa().Emit<Jeqz>(node, notTaken);

        Sa().Emit<LdaObj>(node, tmpObj);
        Sa().Emit<Jmp>(node, ifNullish);

        SetLabel(node, notTaken);
        Sa().Emit<LdaObj>(node, tmpObj);
    }
}

void ETSGen::BranchIfNotNullish([[maybe_unused]] const ir::AstNode *node, [[maybe_unused]] Label *ifNotNullish)
{
    auto notTaken = AllocLabel();
    BranchIfNullish(node, notTaken);
    JumpTo(node, ifNotNullish);
    SetLabel(node, notTaken);
}

void ETSGen::AssumeNonNullish(const ir::AstNode *node, checker::Type const *targetType)
{
    auto const *nullishType = GetAccumulatorType();
    if (nullishType->PossiblyETSUndefined() &&
        ToAssemblerType(targetType) != ToAssemblerType(Checker()->GlobalETSObjectType())) {
        // clear 'undefined' union constituent
        Sa().Emit<Checkcast>(node, ToAssemblerType(targetType));
    }
    SetAccumulatorType(targetType);
}

void ETSGen::EmitNullishException(const ir::AstNode *node)
{
    RegScope ra(this);
    VReg exception = AllocReg();
    NewObject(node, Signatures::BUILTIN_NULLPOINTER_ERROR, exception);
    CallExact(node, Signatures::BUILTIN_NULLPOINTER_ERROR_CTOR, exception);
    EmitThrow(node, exception);
    SetAccumulatorType(nullptr);
}

void ETSGen::RefEqualityLooseDynamic(const ir::AstNode *node, VReg lhs, VReg rhs, Label *ifFalse)
{
    // NOTE(vpukhov): implement
    EmitEtsEquals(node, lhs, rhs);
    BranchIfFalse(node, ifFalse);
}

void ETSGen::HandleLooseNullishEquality(const ir::AstNode *node, VReg lhs, VReg rhs, Label *ifFalse, Label *ifTrue)
{
    Label *ifLhsNullish = AllocLabel();
    Label *out = AllocLabel();

    LoadAccumulator(node, lhs);
    BranchIfNullish(node, ifLhsNullish);

    LoadAccumulator(node, rhs);
    BranchIfNullish(node, ifFalse);
    JumpTo(node, out);

    SetLabel(node, ifLhsNullish);
    LoadAccumulator(node, rhs);
    BranchIfNotNullish(node, ifFalse);
    JumpTo(node, ifTrue);

    SetLabel(node, out);
    SetAccumulatorType(nullptr);
}

static std::optional<std::pair<checker::Type const *, util::StringView>> SelectLooseObjComparator(
    checker::ETSChecker *checker, checker::Type *lhs, checker::Type *rhs)
{
    auto alhs = checker->GetApparentType(checker->GetNonNullishType(lhs));
    auto arhs = checker->GetApparentType(checker->GetNonNullishType(rhs));
    alhs = alhs->IsETSStringType() ? checker->GlobalBuiltinETSStringType() : alhs;
    arhs = arhs->IsETSStringType() ? checker->GlobalBuiltinETSStringType() : arhs;
    if (!alhs->IsETSObjectType() || !arhs->IsETSObjectType()) {
        return std::nullopt;
    }
    if (!checker->Relation()->IsIdenticalTo(alhs, arhs)) {
        return std::nullopt;
    }
    auto obj = alhs->AsETSObjectType();
    if (!obj->HasObjectFlag(checker::ETSObjectFlags::VALUE_TYPED)) {
        return std::nullopt;
    }
    // NOTE(vpukhov): emit faster code
    auto methodSig =
        util::UString(std::string(obj->AssemblerName()) + ".equals:std.core.Object;u1;", checker->Allocator()).View();
    return std::make_pair(checker->GetNonConstantType(obj), methodSig);
}

void ETSGen::RefEqualityLoose(const ir::AstNode *node, VReg lhs, VReg rhs, Label *ifFalse)
{
    auto *checker = const_cast<checker::ETSChecker *>(Checker());
    auto ltype = checker->GetNonConstantType(const_cast<checker::Type *>(GetVRegType(lhs)));
    auto rtype = checker->GetNonConstantType(const_cast<checker::Type *>(GetVRegType(rhs)));
    if (ltype->IsETSDynamicType() || rtype->IsETSDynamicType()) {
        RefEqualityLooseDynamic(node, lhs, rhs, ifFalse);
        return;
    }

    if (ltype->DefinitelyETSNullish() || rtype->DefinitelyETSNullish()) {
        LoadAccumulator(node, ltype->DefinitelyETSNullish() ? rhs : lhs);
        BranchIfNotNullish(node, ifFalse);
    } else if (!ltype->PossiblyETSValueTypedExceptNullish() || !rtype->PossiblyETSValueTypedExceptNullish()) {
        auto ifTrue = AllocLabel();
        if ((ltype->PossiblyETSUndefined() && rtype->PossiblyETSNull()) ||
            (rtype->PossiblyETSUndefined() && ltype->PossiblyETSNull())) {
            HandleLooseNullishEquality(node, lhs, rhs, ifFalse, ifTrue);
        }
        LoadAccumulator(node, lhs);
        Ra().Emit<JneObj>(node, rhs, ifFalse);
        SetLabel(node, ifTrue);
    } else if (auto spec = SelectLooseObjComparator(  // try to select specific type
                                                      // CC-OFFNXT(G.FMT.06-CPP) project code style
                   const_cast<checker::ETSChecker *>(Checker()), const_cast<checker::Type *>(ltype),
                   const_cast<checker::Type *>(rtype));  // CC-OFF(G.FMT.02) project code style
               spec.has_value()) {                       // CC-OFF(G.FMT.02-CPP) project code style
        auto ifTrue = AllocLabel();
        if (ltype->PossiblyETSNullish() || rtype->PossiblyETSNullish()) {
            HandleLooseNullishEquality(node, lhs, rhs, ifFalse, ifTrue);
        }
        LoadAccumulator(node, rhs);
        AssumeNonNullish(node, spec->first);
        StoreAccumulator(node, rhs);
        LoadAccumulator(node, lhs);
        AssumeNonNullish(node, spec->first);
        CallExact(node, spec->second, lhs, rhs);
        BranchIfFalse(node, ifFalse);
        SetLabel(node, ifTrue);
    } else {
        EmitEtsEquals(node, lhs, rhs);
        BranchIfFalse(node, ifFalse);
    }
    SetAccumulatorType(nullptr);
}

void ETSGen::CompileStatements(const ArenaVector<ir::Statement *> &statements)
{
    for (const auto *stmt : statements) {
        stmt->Compile(this);
    }
}

void ETSGen::Negate(const ir::AstNode *node)
{
    auto typeKind = checker::ETSChecker::TypeKind(GetAccumulatorType());

    switch (typeKind) {
        case checker::TypeFlag::BYTE:
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::CHAR:
        case checker::TypeFlag::INT: {
            Sa().Emit<Neg>(node);
            return;
        }
        case checker::TypeFlag::LONG: {
            Sa().Emit<NegWide>(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Sa().Emit<Fneg>(node);
            break;
        }
        case checker::TypeFlag::DOUBLE: {
            Sa().Emit<FnegWide>(node);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void ETSGen::LogicalNot(const ir::AstNode *node)
{
    ASSERT(GetAccumulatorType()->IsConditionalExprType());
    ResolveConditionalResultIfFalse<true, false>(node);
    Sa().Emit<Xori>(node, 1);
    SetAccumulatorType(Checker()->GlobalETSBooleanType());
}

void ETSGen::Unary(const ir::AstNode *node, lexer::TokenType op)
{
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_PLUS:
            break;  // NOP -> Unary numeric promotion is performed
        case lexer::TokenType::PUNCTUATOR_MINUS:
            UnaryMinus(node);
            break;
        case lexer::TokenType::PUNCTUATOR_TILDE:
            UnaryTilde(node);
            break;
        case lexer::TokenType::PUNCTUATOR_EXCLAMATION_MARK:
            LogicalNot(node);
            break;
        default:
            UNREACHABLE();
    }
}

void ETSGen::UnaryMinus(const ir::AstNode *node)
{
    if (GetAccumulatorType()->IsETSBigIntType()) {
        const VReg value = AllocReg();
        StoreAccumulator(node, value);
        CallExact(node, Signatures::BUILTIN_BIGINT_NEGATE, value);
        return;
    }

    switch (checker::ETSChecker::ETSType(GetAccumulatorType())) {
        case checker::TypeFlag::LONG: {
            Sa().Emit<NegWide>(node);
            break;
        }
        case checker::TypeFlag::INT:
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::CHAR:
        case checker::TypeFlag::BYTE: {
            Sa().Emit<Neg>(node);
            break;
        }
        case checker::TypeFlag::DOUBLE: {
            Sa().Emit<FnegWide>(node);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Sa().Emit<Fneg>(node);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void ETSGen::UnaryTilde(const ir::AstNode *node)
{
    if (GetAccumulatorType()->IsETSBigIntType()) {
        const VReg value = AllocReg();
        StoreAccumulator(node, value);
        CallExact(node, Signatures::BUILTIN_BIGINT_OPERATOR_BITWISE_NOT, value);
        SetAccumulatorType(Checker()->GlobalETSBigIntType());
        return;
    }

    switch (checker::ETSChecker::ETSType(GetAccumulatorType())) {
        case checker::TypeFlag::LONG: {
            Sa().Emit<NotWide>(node);
            break;
        }
        case checker::TypeFlag::INT:
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::CHAR:
        case checker::TypeFlag::BYTE: {
            Sa().Emit<Not>(node);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void ETSGen::Update(const ir::AstNode *node, lexer::TokenType op)
{
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_PLUS_PLUS: {
            UpdateOperator<Add2Wide, Addi, Fadd2Wide, Fadd2>(node);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_MINUS_MINUS: {
            UpdateOperator<Sub2Wide, Subi, Fsub2Wide, Fsub2>(node);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void ETSGen::UpdateBigInt(const ir::Expression *node, VReg arg, lexer::TokenType op)
{
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_PLUS_PLUS: {
            CallBigIntUnaryOperator(node, arg, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_INCREMENT);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_MINUS_MINUS: {
            CallBigIntUnaryOperator(node, arg, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_DECREMENT);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void ETSGen::StringBuilderAppend(const ir::AstNode *node, VReg builder)
{
    RegScope rs(this);
    util::StringView signature {};

    node->Compile(this);

    std::unordered_map<checker::TypeFlag, std::string_view> typeFlagToSignaturesMap {
        {checker::TypeFlag::ETS_BOOLEAN, Signatures::BUILTIN_STRING_BUILDER_APPEND_BOOLEAN},
        {checker::TypeFlag::CHAR, Signatures::BUILTIN_STRING_BUILDER_APPEND_CHAR},
        {checker::TypeFlag::SHORT, Signatures::BUILTIN_STRING_BUILDER_APPEND_INT},
        {checker::TypeFlag::BYTE, Signatures::BUILTIN_STRING_BUILDER_APPEND_INT},
        {checker::TypeFlag::INT, Signatures::BUILTIN_STRING_BUILDER_APPEND_INT},
        {checker::TypeFlag::LONG, Signatures::BUILTIN_STRING_BUILDER_APPEND_LONG},
        {checker::TypeFlag::FLOAT, Signatures::BUILTIN_STRING_BUILDER_APPEND_FLOAT},
        {checker::TypeFlag::DOUBLE, Signatures::BUILTIN_STRING_BUILDER_APPEND_DOUBLE},
    };

    auto search = typeFlagToSignaturesMap.find(checker::ETSChecker::ETSType(GetAccumulatorType()));
    if (search != typeFlagToSignaturesMap.end()) {
        signature = search->second;
    } else {
        signature = Signatures::BUILTIN_STRING_BUILDER_APPEND_BUILTIN_STRING;
    }

    if (GetAccumulatorType()->IsETSReferenceType() && !GetAccumulatorType()->IsETSStringType()) {
        if (GetAccumulatorType()->PossiblyETSNull()) {
            Label *ifnull = AllocLabel();
            Label *end = AllocLabel();
            BranchIfNull(node, ifnull);
            Ra().Emit<CallVirtAccShort, 0>(node, Signatures::BUILTIN_OBJECT_TO_STRING, dummyReg_, 0);
            JumpTo(node, end);

            SetLabel(node, ifnull);
            LoadAccumulatorString(node, "null");

            SetLabel(node, end);
        } else {
            Ra().Emit<CallVirtAccShort, 0>(node, Signatures::BUILTIN_OBJECT_TO_STRING, dummyReg_, 0);
        }
    }

    VReg arg0 = AllocReg();
    StoreAccumulator(node, arg0);

    CallExact(node, signature, builder, arg0);
    SetAccumulatorType(Checker()->GetGlobalTypesHolder()->GlobalStringBuilderBuiltinType());
}

void ETSGen::AppendString(const ir::Expression *const expr, const VReg builder)
{
    ASSERT((expr->IsBinaryExpression() &&
            expr->AsBinaryExpression()->OperatorType() == lexer::TokenType::PUNCTUATOR_PLUS) ||
           (expr->IsAssignmentExpression() &&
            expr->AsAssignmentExpression()->OperatorType() == lexer::TokenType::PUNCTUATOR_PLUS_EQUAL));

    if (expr->IsBinaryExpression()) {
        StringBuilder(expr->AsBinaryExpression()->Left(), expr->AsBinaryExpression()->Right(), builder);
    } else {
        StringBuilder(expr->AsAssignmentExpression()->Left(), expr->AsAssignmentExpression()->Right(), builder);
    }
}

void ETSGen::StringBuilder(const ir::Expression *const left, const ir::Expression *const right, const VReg builder)
{
    if (left->IsBinaryExpression() && left->TsType()->IsETSStringType()) {
        AppendString(left->AsBinaryExpression(), builder);
    } else {
        StringBuilderAppend(left, builder);
    }

    StringBuilderAppend(right, builder);
}

void ETSGen::BuildString(const ir::Expression *node)
{
    RegScope rs(this);

    Ra().Emit<InitobjShort, 0>(node, Signatures::BUILTIN_STRING_BUILDER_CTOR, dummyReg_, dummyReg_);
    SetAccumulatorType(Checker()->GlobalStringBuilderBuiltinType());

    auto builder = AllocReg();
    StoreAccumulator(node, builder);

    AppendString(node, builder);
    CallExact(node, Signatures::BUILTIN_STRING_BUILDER_TO_STRING, builder);

    SetAccumulatorType(node->TsType());
}

void ETSGen::CallBigIntUnaryOperator(const ir::Expression *node, VReg arg, const util::StringView signature)
{
    LoadAccumulator(node, arg);
    CallExact(node, signature, arg);
    SetAccumulatorType(Checker()->GlobalETSBigIntType());
}

void ETSGen::CallBigIntBinaryOperator(const ir::Expression *node, VReg lhs, VReg rhs, const util::StringView signature)
{
    LoadAccumulator(node, lhs);
    CallExact(node, signature, lhs, rhs);
    SetAccumulatorType(Checker()->GlobalETSBigIntType());
}

void ETSGen::CallBigIntBinaryComparison(const ir::Expression *node, VReg lhs, VReg rhs,
                                        const util::StringView signature)
{
    LoadAccumulator(node, lhs);
    CallExact(node, signature, lhs, rhs);
    SetAccumulatorType(Checker()->GlobalETSBooleanType());
}

void ETSGen::BuildTemplateString(const ir::TemplateLiteral *node)
{
    RegScope rs(this);

    Ra().Emit<InitobjShort, 0>(node, Signatures::BUILTIN_STRING_BUILDER_CTOR, dummyReg_, dummyReg_);
    SetAccumulatorType(Checker()->GlobalStringBuilderBuiltinType());

    auto builder = AllocReg();
    StoreAccumulator(node, builder);

    // Just to reduce extra nested level(s):
    auto const appendExpressions = [this, &builder](ArenaVector<ir::Expression *> const &expressions,
                                                    ArenaVector<ir::TemplateElement *> const &quasis) -> void {
        auto const num = expressions.size();
        std::size_t i = 0U;

        while (i < num) {
            StringBuilderAppend(expressions[i], builder);
            if (!quasis[++i]->Raw().Empty()) {
                StringBuilderAppend(quasis[i], builder);
            }
        }
    };

    if (auto const &quasis = node->Quasis(); !quasis.empty()) {
        if (!quasis[0]->Raw().Empty()) {
            StringBuilderAppend(quasis[0], builder);
        }

        if (auto const &expressions = node->Expressions(); !expressions.empty()) {
            appendExpressions(expressions, quasis);
        }
    }

    CallExact(node, Signatures::BUILTIN_STRING_BUILDER_TO_STRING, builder);

    SetAccumulatorType(Checker()->GlobalBuiltinETSStringType());
}

void ETSGen::NewObject(const ir::AstNode *const node, const util::StringView name, VReg athis)
{
    Ra().Emit<Newobj>(node, athis, name);
    SetVRegType(athis, Checker()->GlobalETSObjectType());
}

void ETSGen::NewArray(const ir::AstNode *const node, const VReg arr, const VReg dim, const checker::Type *const arrType)
{
    std::stringstream ss;
    arrType->ToAssemblerTypeWithRank(ss);
    const auto res = ProgElement()->Strings().emplace(ss.str());

    Ra().Emit<Newarr>(node, arr, dim, util::StringView(*res.first));
    SetVRegType(arr, arrType);
}

void ETSGen::LoadArrayLength(const ir::AstNode *node, VReg arrayReg)
{
    Ra().Emit<Lenarr>(node, arrayReg);
    SetAccumulatorType(Checker()->GlobalIntType());
}

void ETSGen::LoadArrayElement(const ir::AstNode *node, VReg objectReg)
{
    auto *elementType = GetVRegType(objectReg)->AsETSArrayType()->ElementType();
    if (elementType->IsETSReferenceType()) {
        Ra().Emit<LdarrObj>(node, objectReg);
        SetAccumulatorType(elementType);
        return;
    }
    switch (checker::ETSChecker::ETSType(elementType)) {
        case checker::TypeFlag::ETS_BOOLEAN:
        case checker::TypeFlag::BYTE: {
            Ra().Emit<Ldarr8>(node, objectReg);
            break;
        }
        case checker::TypeFlag::CHAR: {
            Ra().Emit<Ldarru16>(node, objectReg);
            break;
        }
        case checker::TypeFlag::SHORT: {
            Ra().Emit<Ldarr16>(node, objectReg);
            break;
        }
        case checker::TypeFlag::ETS_STRING_ENUM:
            [[fallthrough]];
        case checker::TypeFlag::ETS_INT_ENUM:
        case checker::TypeFlag::INT: {
            Ra().Emit<Ldarr>(node, objectReg);
            break;
        }
        case checker::TypeFlag::LONG: {
            Ra().Emit<LdarrWide>(node, objectReg);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Ra().Emit<Fldarr32>(node, objectReg);
            break;
        }
        case checker::TypeFlag::DOUBLE: {
            Ra().Emit<FldarrWide>(node, objectReg);
            break;
        }

        default: {
            UNREACHABLE();
        }
    }

    SetAccumulatorType(elementType);
}

void ETSGen::StoreArrayElement(const ir::AstNode *node, VReg objectReg, VReg index, const checker::Type *elementType)
{
    if (elementType->IsETSReferenceType()) {
        Ra().Emit<StarrObj>(node, objectReg, index);
        SetAccumulatorType(elementType);
        return;
    }
    switch (checker::ETSChecker::ETSType(elementType)) {
        case checker::TypeFlag::ETS_BOOLEAN:
        case checker::TypeFlag::BYTE: {
            Ra().Emit<Starr8>(node, objectReg, index);
            break;
        }
        case checker::TypeFlag::CHAR:
        case checker::TypeFlag::SHORT: {
            Ra().Emit<Starr16>(node, objectReg, index);
            break;
        }
        case checker::TypeFlag::ETS_STRING_ENUM:
            [[fallthrough]];
        case checker::TypeFlag::ETS_INT_ENUM:
        case checker::TypeFlag::INT: {
            Ra().Emit<Starr>(node, objectReg, index);
            break;
        }
        case checker::TypeFlag::LONG: {
            Ra().Emit<StarrWide>(node, objectReg, index);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Ra().Emit<Fstarr32>(node, objectReg, index);
            break;
        }
        case checker::TypeFlag::DOUBLE: {
            Ra().Emit<FstarrWide>(node, objectReg, index);
            break;
        }

        default: {
            UNREACHABLE();
        }
    }

    SetAccumulatorType(elementType);
}

void ETSGen::LoadStringLength(const ir::AstNode *node)
{
    Ra().Emit<CallAccShort, 0>(node, Signatures::BUILTIN_STRING_LENGTH, dummyReg_, 0);
    SetAccumulatorType(Checker()->GlobalIntType());
}

void ETSGen::FloatIsNaN(const ir::AstNode *node)
{
    Ra().Emit<CallAccShort, 0>(node, Signatures::BUILTIN_FLOAT_IS_NAN, dummyReg_, 0);
    SetAccumulatorType(Checker()->GlobalETSBooleanType());
}

void ETSGen::DoubleIsNaN(const ir::AstNode *node)
{
    Ra().Emit<CallAccShort, 0>(node, Signatures::BUILTIN_DOUBLE_IS_NAN, dummyReg_, 0);
    SetAccumulatorType(Checker()->GlobalETSBooleanType());
}

void ETSGen::LoadStringChar(const ir::AstNode *node, const VReg stringObj, const VReg charIndex)
{
    Ra().Emit<CallShort>(node, Signatures::BUILTIN_STRING_CHAR_AT, stringObj, charIndex);
    SetAccumulatorType(Checker()->GlobalCharType());
}

void ETSGen::ThrowException(const ir::Expression *expr)
{
    RegScope rs(this);

    expr->Compile(this);
    VReg arg = AllocReg();
    StoreAccumulator(expr, arg);
    EmitThrow(expr, arg);
}

bool ETSGen::ExtendWithFinalizer(ir::AstNode const *node, const ir::AstNode *originalNode, Label *prevFinnaly)
{
    ASSERT(originalNode != nullptr);

    if (node == nullptr || !node->IsStatement()) {
        return false;
    }

    if ((originalNode->IsContinueStatement() && originalNode->AsContinueStatement()->Target() == node) ||
        (originalNode->IsBreakStatement() && originalNode->AsBreakStatement()->Target() == node)) {
        return false;
    }

    if (node->IsTryStatement() && node->AsTryStatement()->HasFinalizer()) {
        Label *beginLabel = nullptr;

        if (prevFinnaly == nullptr) {
            beginLabel = AllocLabel();
            Branch(originalNode, beginLabel);
        } else {
            beginLabel = prevFinnaly;
        }

        Label *endLabel = AllocLabel();

        if (node->Parent() != nullptr && node->Parent()->IsStatement()) {
            if (!ExtendWithFinalizer(node->Parent(), originalNode, endLabel)) {
                endLabel = nullptr;
            }
        } else {
            endLabel = nullptr;
        }

        LabelPair insertion = compiler::LabelPair(beginLabel, endLabel);

        auto *tryStatement = const_cast<ir::AstNode *>(node)->AsTryStatement();
        tryStatement->AddFinalizerInsertion(insertion, originalNode->AsStatement());

        return true;
    }

    auto *parent = node->Parent();

    if (parent == nullptr || !parent->IsStatement()) {
        return false;
    }

    if (parent->IsTryStatement() && node->IsBlockStatement() &&
        parent->AsTryStatement()->FinallyBlock() == node->AsBlockStatement()) {
        parent = parent->Parent();
    }

    return ExtendWithFinalizer(parent, originalNode, prevFinnaly);
}

util::StringView ETSGen::ToAssemblerType(const es2panda::checker::Type *type) const
{
    ASSERT(type->IsETSReferenceType());

    std::stringstream ss;
    type->ToAssemblerTypeWithRank(ss);
    return util::UString(ss.str(), Allocator()).View();
}
void ETSGen::CastUnionToFunctionType(const ir::AstNode *node, const checker::ETSUnionType *unionType,
                                     checker::Signature *signatureTarget)
{
    for (auto it : unionType->ConstituentTypes()) {
        for (auto prop : it->AsETSObjectType()->GetAllProperties()) {
            if (prop->TsType()->IsETSFunctionType() &&
                prop->TsType()->AsETSFunctionType()->CallSignatures().front() == signatureTarget) {
                InternalCheckCast(node, it);
                break;
            }
        }
    }
}

}  // namespace ark::es2panda::compiler
