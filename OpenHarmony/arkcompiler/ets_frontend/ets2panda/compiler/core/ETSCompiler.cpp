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

#include "ETSCompiler.h"

#include "compiler/base/catchTable.h"
#include "checker/ets/dynamic/dynamicCall.h"
#include "compiler/base/condition.h"
#include "compiler/base/lreference.h"
#include "compiler/core/switchBuilder.h"
#include "compiler/function/functionBuilder.h"
#include "checker/ETSchecker.h"
#include "checker/types/ets/etsDynamicFunctionType.h"
#include "parser/ETSparser.h"

namespace ark::es2panda::compiler {

ETSGen *ETSCompiler::GetETSGen() const
{
    return static_cast<ETSGen *>(GetCodeGen());
}

void ETSCompiler::Compile(const ir::CatchClause *st) const
{
    ETSGen *etsg = GetETSGen();
    compiler::LocalRegScope lrs(etsg, st->Scope()->ParamScope());
    etsg->SetAccumulatorType(st->TsType());
    auto lref = compiler::ETSLReference::Create(etsg, st->Param(), true);
    lref.SetValue();
    st->Body()->Compile(etsg);
}

void ETSCompiler::Compile(const ir::ClassProperty *st) const
{
    ETSGen *etsg = GetETSGen();
    if (st->Value() == nullptr && st->TsType()->IsETSPrimitiveType()) {
        return;
    }

    auto ttctx = compiler::TargetTypeContext(etsg, st->TsType());
    compiler::RegScope rs(etsg);

    if (st->Value() == nullptr) {
        etsg->LoadDefaultValue(st, st->TsType());
    } else if (!etsg->TryLoadConstantExpression(st->Value())) {
        st->Value()->Compile(etsg);
        etsg->ApplyConversion(st->Value(), st->TsType());
    }

    if (st->IsStatic()) {
        etsg->StoreStaticOwnProperty(st, st->TsType(), st->Key()->AsIdentifier()->Name());
    } else {
        etsg->StoreProperty(st, st->TsType(), etsg->GetThisReg(), st->Key()->AsIdentifier()->Name());
    }
}

void ETSCompiler::Compile(const ir::TemplateElement *expr) const
{
    ETSGen *etsg = GetETSGen();
    etsg->LoadAccumulatorString(expr, expr->Cooked());
    etsg->SetAccumulatorType(expr->TsType());
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::ETSClassLiteral *expr) const
{
    ETSGen *etsg = GetETSGen();

    auto *literal = expr->Expr();
    auto *literalType = literal->TsType();

    bool const isPrimitive = !literalType->IsETSReferenceType();
    if (!isPrimitive) {
        literal->Compile(etsg);
    } else {
        ASSERT(literalType->IsETSPrimitiveType());
        etsg->SetAccumulatorType(literalType);
    }

    etsg->GetType(expr, isPrimitive);
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::ETSFunctionType *node) const
{
    ETSGen *etsg = GetETSGen();
    etsg->LoadAccumulatorNull(node, node->TsType());
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), node->TsType()));
}

void ETSCompiler::Compile([[maybe_unused]] const ir::ETSLaunchExpression *expr) const
{
#ifdef PANDA_WITH_ETS
    ETSGen *etsg = GetETSGen();
    compiler::RegScope rs(etsg);
    compiler::VReg calleeReg = etsg->AllocReg();
    checker::Signature *signature = expr->expr_->Signature();
    bool isStatic = signature->HasSignatureFlag(checker::SignatureFlags::STATIC);
    if (expr->expr_->Callee()->IsIdentifier()) {
        if (!isStatic) {
            etsg->LoadThis(expr->expr_);
            etsg->StoreAccumulator(expr, calleeReg);
        }
    } else if (expr->expr_->Callee()->IsMemberExpression()) {
        if (!isStatic) {
            expr->expr_->Callee()->AsMemberExpression()->Object()->Compile(etsg);
            etsg->StoreAccumulator(expr, calleeReg);
        }
    } else {
        expr->expr_->Callee()->Compile(etsg);
        etsg->StoreAccumulator(expr, calleeReg);
    }

    if (isStatic) {
        etsg->LaunchExact(expr, signature, expr->expr_->Arguments());
    } else {
        etsg->LaunchVirtual(expr, signature, calleeReg, expr->expr_->Arguments());
    }

    etsg->SetAccumulatorType(expr->TsType());
#endif  // PANDA_WITH_ETS
}

void ETSCompiler::Compile(const ir::ETSNewArrayInstanceExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    compiler::RegScope rs(etsg);
    compiler::TargetTypeContext ttctx(etsg, etsg->Checker()->GlobalIntType());

    expr->Dimension()->Compile(etsg);

    compiler::VReg arr = etsg->AllocReg();
    compiler::VReg dim = etsg->AllocReg();
    etsg->ApplyConversionAndStoreAccumulator(expr, dim, expr->Dimension()->TsType());
    etsg->NewArray(expr, arr, dim, expr->TsType());

    const auto *exprType = expr->TypeReference()->TsType();

    const bool isUnionTypeContainsUndefined =
        expr->TypeReference()->IsETSTypeReference() && exprType->IsETSUnionType() &&
        exprType->AsETSUnionType()->HasType(etsg->Checker()->GlobalETSUndefinedType());
    if (expr->Signature() != nullptr || isUnionTypeContainsUndefined) {
        compiler::VReg countReg = etsg->AllocReg();
        auto *startLabel = etsg->AllocLabel();
        auto *endLabel = etsg->AllocLabel();
        etsg->MoveImmediateToRegister(expr, countReg, checker::TypeFlag::INT, static_cast<std::int32_t>(0));
        const auto indexReg = etsg->AllocReg();

        etsg->SetLabel(expr, startLabel);
        etsg->LoadAccumulator(expr, dim);
        etsg->JumpCompareRegister<compiler::Jle>(expr, countReg, endLabel);

        etsg->LoadAccumulator(expr, countReg);
        etsg->StoreAccumulator(expr, indexReg);

        const compiler::TargetTypeContext ttctx2(etsg, exprType);
        ArenaVector<ir::Expression *> arguments(GetCodeGen()->Allocator()->Adapter());
        if (isUnionTypeContainsUndefined) {
            exprType = etsg->LoadDefaultValue(expr, exprType);
        } else {
            etsg->InitObject(expr, expr->Signature(), arguments);
        }
        etsg->StoreArrayElement(expr, arr, indexReg, exprType);

        etsg->IncrementImmediateRegister(expr, countReg, checker::TypeFlag::INT, static_cast<std::int32_t>(1));
        etsg->JumpTo(expr, startLabel);
        etsg->SetLabel(expr, endLabel);
    }

    etsg->SetVRegType(arr, expr->TsType());
    etsg->LoadAccumulator(expr, arr);

    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

static std::pair<VReg, VReg> LoadDynamicName(compiler::ETSGen *etsg, const ir::AstNode *node,
                                             const ArenaVector<util::StringView> &dynName, bool isConstructor)
{
    auto *checker = const_cast<checker::ETSChecker *>(etsg->Checker()->AsETSChecker());
    auto *callNames = checker->DynamicCallNames(isConstructor);

    auto qnameStart = etsg->AllocReg();
    auto qnameLen = etsg->AllocReg();

    TargetTypeContext ttctx(etsg, nullptr);  // without this ints will be cast to JSValue
    etsg->LoadAccumulatorInt(node, callNames->at(dynName));
    etsg->StoreAccumulator(node, qnameStart);
    etsg->LoadAccumulatorInt(node, dynName.size());
    etsg->StoreAccumulator(node, qnameLen);
    return {qnameStart, qnameLen};
}

static void CreateDynamicObject(const ir::AstNode *node, compiler::ETSGen *etsg, const ir::Expression *typeRef,
                                checker::Signature *signature, const ArenaVector<ir::Expression *> &arguments)
{
    auto objReg = etsg->AllocReg();

    auto callInfo = checker::DynamicCall::ResolveCall(etsg->VarBinder(), typeRef);
    if (callInfo.obj->IsETSImportDeclaration()) {
        etsg->LoadAccumulatorDynamicModule(node, callInfo.obj->AsETSImportDeclaration());
    } else {
        callInfo.obj->Compile(etsg);
    }

    etsg->StoreAccumulator(node, objReg);

    auto [qnameStart, qnameLen] = LoadDynamicName(etsg, node, callInfo.name, true);
    etsg->CallDynamic(ETSGen::CallDynamicData {node, objReg, qnameStart}, qnameLen, signature, arguments);
}

static void ConvertRestArguments(checker::ETSChecker *const checker, const ir::ETSNewClassInstanceExpression *expr)
{
    if (expr->GetSignature()->RestVar() != nullptr) {
        std::size_t const argumentCount = expr->GetArguments().size();
        std::size_t const parameterCount = expr->GetSignature()->MinArgCount();
        ASSERT(argumentCount >= parameterCount);

        auto &arguments = const_cast<ArenaVector<ir::Expression *> &>(expr->GetArguments());
        std::size_t i = parameterCount;

        if (i < argumentCount && expr->GetArguments()[i]->IsSpreadElement()) {
            arguments[i] = expr->GetArguments()[i]->AsSpreadElement()->Argument();
        } else {
            ArenaVector<ir::Expression *> elements(checker->Allocator()->Adapter());
            for (; i < argumentCount; ++i) {
                elements.emplace_back(expr->GetArguments()[i]);
            }
            auto *arrayExpression = checker->AllocNode<ir::ArrayExpression>(std::move(elements), checker->Allocator());
            arrayExpression->SetParent(const_cast<ir::ETSNewClassInstanceExpression *>(expr));
            auto restType = expr->GetSignature()->RestVar()->TsType()->AsETSArrayType();
            arrayExpression->SetTsType(restType);
            arrayExpression->SetPreferredType(restType->ElementType());
            arguments.erase(expr->GetArguments().begin() + parameterCount, expr->GetArguments().end());
            arguments.emplace_back(arrayExpression);
        }
    }
}

static void HandleUnionTypeInForOf(compiler::ETSGen *etsg, checker::Type const *const exprType,
                                   const ir::ForOfStatement *st, VReg objReg, VReg *countReg)
{
    ArenaVector<Label *> labels(etsg->Allocator()->Adapter());

    for (auto it : exprType->AsETSUnionType()->ConstituentTypes()) {
        labels.push_back(etsg->AllocLabel());
        etsg->LoadAccumulator(st->Right(), objReg);
        etsg->IsInstance(st->Right(), objReg, it);
        etsg->BranchIfTrue(st, labels.back());
    }

    labels.push_back(etsg->AllocLabel());

    for (size_t i = 0; i < exprType->AsETSUnionType()->ConstituentTypes().size(); i++) {
        compiler::VReg unionReg = etsg->AllocReg();
        auto currentType = exprType->AsETSUnionType()->ConstituentTypes()[i];
        etsg->SetLabel(st->Right(), labels[i]);
        etsg->LoadAccumulator(st, objReg);
        etsg->CastToReftype(st->Right(), currentType, false);
        etsg->StoreAccumulator(st, unionReg);
        etsg->LoadAccumulator(st, unionReg);
        if (countReg == nullptr) {
            if (currentType->IsETSArrayType()) {
                etsg->LoadArrayLength(st, unionReg);
            } else {
                etsg->LoadStringLength(st);
            }
        } else {
            if (currentType->IsETSArrayType()) {
                etsg->LoadAccumulator(st, *countReg);
                etsg->LoadArrayElement(st, unionReg);
            } else {
                etsg->LoadStringChar(st, unionReg, *countReg);
                // NOTE(vpukhov): #20510 use a single unboxing convertor
                etsg->ApplyCastToBoxingFlags(st, ir::BoxingUnboxingFlags::BOX_TO_CHAR);
                etsg->EmitBoxingConversion(ir::BoxingUnboxingFlags::BOX_TO_CHAR, st);
                etsg->CastToChar(st);
            }
        }

        if (i + 1 != exprType->AsETSUnionType()->ConstituentTypes().size()) {
            etsg->Branch(st, labels.back());
        }
    }

    etsg->SetLabel(st->Right(), labels.back());
}

static void GetSizeInForOf(compiler::ETSGen *etsg, checker::Type const *const exprType, const ir::ForOfStatement *st,
                           VReg objReg)
{
    if (exprType->IsETSArrayType()) {
        etsg->LoadArrayLength(st, objReg);
    } else if (exprType->IsETSUnionType()) {
        HandleUnionTypeInForOf(etsg, exprType, st, objReg, nullptr);
    } else {
        etsg->LoadStringLength(st);
    }
}

static void MaybeCastUnionTypeToFunctionType(compiler::ETSGen *etsg, const ir::CallExpression *expr,
                                             checker::Signature *signature)
{
    expr->Callee()->AsMemberExpression()->Object()->Compile(etsg);
    auto objType = expr->Callee()->AsMemberExpression()->Object()->TsType();
    if (auto propType = expr->Callee()->AsMemberExpression()->Property()->TsType();
        propType != nullptr && propType->IsETSFunctionType() && objType->IsETSUnionType()) {
        etsg->CastUnionToFunctionType(expr, objType->AsETSUnionType(), signature);
    }
}

void ETSCompiler::Compile(const ir::ETSNewClassInstanceExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    if (expr->TsType()->IsETSDynamicType()) {
        compiler::RegScope rs(etsg);
        auto *name = expr->GetTypeRef();
        CreateDynamicObject(expr, etsg, name, expr->signature_, expr->GetArguments());
    } else {
        ConvertRestArguments(const_cast<checker::ETSChecker *>(etsg->Checker()->AsETSChecker()), expr);
        etsg->InitObject(expr, expr->signature_, expr->GetArguments());
    }

    etsg->SetAccumulatorType(expr->TsType());
}

void ETSCompiler::Compile(const ir::ETSNewMultiDimArrayInstanceExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    etsg->InitObject(expr, expr->Signature(), expr->Dimensions());
    etsg->SetAccumulatorType(expr->TsType());
}

void ETSCompiler::Compile(const ir::ETSParameterExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    expr->Ident()->Compile(etsg);

    if (auto *const paramType = expr->TsType();
        !etsg->Checker()->AsETSChecker()->Relation()->IsIdenticalTo(paramType, etsg->GetAccumulatorType())) {
        etsg->SetAccumulatorType(paramType);
    }

    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::ETSTypeReference *node) const
{
    ETSGen *etsg = GetETSGen();
    node->Part()->Compile(etsg);
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), node->TsType()));
}

void ETSCompiler::Compile(const ir::ETSTypeReferencePart *node) const
{
    ETSGen *etsg = GetETSGen();
    node->Name()->Compile(etsg);
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), node->TsType()));
}

void ETSCompiler::Compile([[maybe_unused]] const ir::ETSWildcardType *node) const
{
    ETSGen *etsg = GetETSGen();
    etsg->Unimplemented();
}

void ETSCompiler::Compile(const ir::ArrayExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    const compiler::RegScope rs(etsg);

    const auto arr = etsg->AllocReg();
    const auto dim = etsg->AllocReg();

    const compiler::TargetTypeContext ttctx(etsg, etsg->Checker()->GlobalIntType());
    etsg->LoadAccumulatorInt(expr, static_cast<std::int32_t>(expr->Elements().size()));
    etsg->StoreAccumulator(expr, dim);
    etsg->NewArray(expr, arr, dim, expr->TsType());

    const auto indexReg = etsg->AllocReg();
    for (std::uint32_t i = 0; i < expr->Elements().size(); ++i) {
        const auto *const expression = expr->Elements()[i];
        etsg->LoadAccumulatorInt(expr, i);
        etsg->StoreAccumulator(expr, indexReg);

        const compiler::TargetTypeContext ttctx2(etsg, expr->TsType()->AsETSArrayType()->ElementType());
        if (!etsg->TryLoadConstantExpression(expression)) {
            expression->Compile(etsg);
        }

        etsg->ApplyConversion(expression, nullptr);
        etsg->ApplyConversion(expression);

        if (expression->TsType()->IsETSArrayType()) {
            etsg->StoreArrayElement(expr, arr, indexReg, expression->TsType());
        } else {
            etsg->StoreArrayElement(expr, arr, indexReg, expr->TsType()->AsETSArrayType()->ElementType());
        }
    }

    etsg->LoadAccumulator(expr, arr);
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::AssignmentExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    // All other operations are handled in OpAssignmentLowering
    ASSERT(expr->OperatorType() == lexer::TokenType::PUNCTUATOR_SUBSTITUTION);
    auto *const exprType = expr->TsType();

    compiler::RegScope rs(etsg);
    auto lref = compiler::ETSLReference::Create(etsg, expr->Left(), false);
    auto ttctx = compiler::TargetTypeContext(etsg, exprType);

    if (expr->Right()->IsNullLiteral()) {
        etsg->LoadAccumulatorNull(expr, exprType);
    } else if (expr->Right()->IsUndefinedLiteral()) {
        etsg->LoadAccumulatorUndefined(expr);
        etsg->SetAccumulatorType(exprType);
    } else {
        expr->Right()->Compile(etsg);
        etsg->ApplyConversion(expr->Right(), exprType);
        etsg->SetAccumulatorType(exprType);
    }

    if (expr->Right()->TsType()->IsETSBigIntType()) {
        // For bigints we have to copy the bigint object when performing an assignment operation
        const VReg value = etsg->AllocReg();
        etsg->StoreAccumulator(expr, value);
        etsg->CreateBigIntObject(expr, value, Signatures::BUILTIN_BIGINT_CTOR_BIGINT);
    }

    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), exprType) ||
           etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(),
                                                      etsg->Checker()->GlobalBuiltinJSValueType()));
    lref.SetValue();
}

void ETSCompiler::Compile(const ir::AwaitExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    static constexpr bool IS_UNCHECKED_CAST = false;
    compiler::RegScope rs(etsg);
    compiler::VReg argumentReg = etsg->AllocReg();
    expr->Argument()->Compile(etsg);
    etsg->StoreAccumulator(expr, argumentReg);
    etsg->CallVirtual(expr->Argument(), compiler::Signatures::BUILTIN_PROMISE_AWAIT_RESOLUTION, argumentReg);
    etsg->CastToReftype(expr->Argument(), expr->TsType(), IS_UNCHECKED_CAST);
    etsg->SetAccumulatorType(expr->TsType());
}

void ETSCompiler::UnimplementedPathError(const ir::AstNode *node, util::StringView message) const
{
    ETSGen *etsg = GetETSGen();

    compiler::RegScope rs(etsg);
    const auto errorReg = etsg->AllocReg();
    const auto msgReg = etsg->AllocReg();
    etsg->LoadAccumulatorString(node, message);
    etsg->StoreAccumulator(node, msgReg);
    etsg->NewObject(node, Signatures::BUILTIN_ERROR, errorReg);
    etsg->CallExact(node, Signatures::BUILTIN_ERROR_CTOR, errorReg, msgReg);
    etsg->EmitThrow(node, errorReg);
}

void ETSCompiler::Compile(const ir::ImportExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    expr->Source()->Compile(etsg);

    UnimplementedPathError(expr, "Dynamic import is not supported");
    etsg->SetAccumulatorType(expr->TsType());  // dead code
}

static void CompileNullishCoalescing(compiler::ETSGen *etsg, ir::BinaryExpression const *const node)
{
    auto const compileOperand = [etsg, optype = node->OperationType()](ir::Expression const *expr) {
        etsg->CompileAndCheck(expr);
        etsg->ApplyConversion(expr, nullptr);
    };

    compileOperand(node->Left());

    if (node->Left()->TsType()->DefinitelyNotETSNullish()) {
        // fallthrough
    } else if (node->Left()->TsType()->DefinitelyETSNullish()) {
        compileOperand(node->Right());
    } else {
        auto *ifLeftNullish = etsg->AllocLabel();
        auto *endLabel = etsg->AllocLabel();

        etsg->BranchIfNullish(node, ifLeftNullish);

        etsg->AssumeNonNullish(node, node->OperationType());
        etsg->ApplyConversion(node->Left(), node->OperationType());
        etsg->JumpTo(node, endLabel);

        etsg->SetLabel(node, ifLeftNullish);
        compileOperand(node->Right());

        etsg->SetLabel(node, endLabel);
    }
    etsg->SetAccumulatorType(node->TsType());
}

static void CompileLogical(compiler::ETSGen *etsg, const ir::BinaryExpression *expr)
{
    if (expr->OperatorType() == lexer::TokenType::PUNCTUATOR_NULLISH_COALESCING) {
        CompileNullishCoalescing(etsg, expr);
        return;
    }

    ASSERT(expr->IsLogicalExtended());
    auto ttctx = compiler::TargetTypeContext(etsg, expr->OperationType());
    compiler::RegScope rs(etsg);
    auto lhs = etsg->AllocReg();

    expr->Left()->Compile(etsg);
    etsg->ApplyConversionAndStoreAccumulator(expr->Left(), lhs, expr->OperationType());

    auto *endLabel = etsg->AllocLabel();

    auto returnLeftLabel = etsg->AllocLabel();
    if (expr->OperatorType() == lexer::TokenType::PUNCTUATOR_LOGICAL_AND) {
        etsg->ResolveConditionalResultIfFalse(expr->Left(), returnLeftLabel);
        etsg->BranchIfFalse(expr, returnLeftLabel);

        expr->Right()->Compile(etsg);
        etsg->ApplyConversion(expr->Right(), expr->OperationType());
        etsg->Branch(expr, endLabel);

        etsg->SetLabel(expr, returnLeftLabel);
        etsg->LoadAccumulator(expr, lhs);
    } else {
        etsg->ResolveConditionalResultIfTrue(expr->Left(), returnLeftLabel);
        etsg->BranchIfTrue(expr, returnLeftLabel);

        expr->Right()->Compile(etsg);
        etsg->ApplyConversion(expr->Right(), expr->OperationType());
        etsg->Branch(expr, endLabel);

        etsg->SetLabel(expr, returnLeftLabel);
        etsg->LoadAccumulator(expr, lhs);
    }

    etsg->SetLabel(expr, endLabel);
    etsg->SetAccumulatorType(expr->TsType());
    etsg->ApplyConversion(expr, expr->OperationType());

    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

static void CompileInstanceof(compiler::ETSGen *etsg, const ir::BinaryExpression *expr)
{
    ASSERT(expr->OperatorType() == lexer::TokenType::KEYW_INSTANCEOF);
    auto ttctx = compiler::TargetTypeContext(etsg, expr->OperationType());
    compiler::RegScope rs(etsg);
    auto lhs = etsg->AllocReg();

    expr->Left()->Compile(etsg);
    etsg->ApplyConversionAndStoreAccumulator(expr->Left(), lhs, expr->OperationType());

    if (expr->Left()->TsType()->IsETSDynamicType() || expr->Right()->TsType()->IsETSDynamicType()) {
        auto rhs = etsg->AllocReg();
        expr->Right()->Compile(etsg);
        etsg->StoreAccumulator(expr, rhs);
        etsg->IsInstanceDynamic(expr, lhs, rhs);
    } else {
        auto target = expr->Right()->TsType();
        // NOTE(vpukhov): #20510 lowering
        target = target->IsETSEnumType() ? etsg->Checker()->MaybeBoxType(target) : target;
        etsg->IsInstance(expr, lhs, target);
    }
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

std::map<lexer::TokenType, std::string_view> &GetBigintSignatures()
{
    static std::map<lexer::TokenType, std::string_view> bigintSignatures = {
        {lexer::TokenType::PUNCTUATOR_PLUS, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_ADD},
        {lexer::TokenType::PUNCTUATOR_MINUS, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_SUBTRACT},
        {lexer::TokenType::PUNCTUATOR_MULTIPLY, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_MULTIPLY},
        {lexer::TokenType::PUNCTUATOR_DIVIDE, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_DIVIDE},
        {lexer::TokenType::PUNCTUATOR_MOD, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_MODULE},
        {lexer::TokenType::PUNCTUATOR_BITWISE_OR, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_BITWISE_OR},
        {lexer::TokenType::PUNCTUATOR_BITWISE_AND, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_BITWISE_AND},
        {lexer::TokenType::PUNCTUATOR_BITWISE_XOR, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_BITWISE_XOR},
        {lexer::TokenType::PUNCTUATOR_LEFT_SHIFT, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_LEFT_SHIFT},
        {lexer::TokenType::PUNCTUATOR_RIGHT_SHIFT, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_RIGHT_SHIFT},
        {lexer::TokenType::PUNCTUATOR_GREATER_THAN, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_GREATER_THAN},
        {lexer::TokenType::PUNCTUATOR_LESS_THAN, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_LESS_THAN},
        {lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL,
         compiler::Signatures::BUILTIN_BIGINT_OPERATOR_GREATER_THAN_EQUAL},
        {lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL, compiler::Signatures::BUILTIN_BIGINT_OPERATOR_LESS_THAN_EQUAL},
    };

    return bigintSignatures;
}

static bool CompileBigInt(compiler::ETSGen *etsg, const ir::BinaryExpression *expr)
{
    if ((expr->Left()->TsType() == nullptr) || (expr->Right()->TsType() == nullptr)) {
        return false;
    }

    if (!expr->Left()->TsType()->IsETSBigIntType()) {
        return false;
    }

    if (!expr->Right()->TsType()->IsETSBigIntType()) {
        return false;
    }

    auto map = GetBigintSignatures();
    if (map.find(expr->OperatorType()) == map.end()) {
        return false;
    }

    const checker::Type *operationType = expr->OperationType();
    auto ttctx = compiler::TargetTypeContext(etsg, operationType);
    compiler::RegScope rs(etsg);
    compiler::VReg lhs = etsg->AllocReg();
    expr->Left()->Compile(etsg);
    etsg->ApplyConversionAndStoreAccumulator(expr->Left(), lhs, operationType);
    expr->Right()->Compile(etsg);
    etsg->ApplyConversion(expr->Right(), operationType);
    compiler::VReg rhs = etsg->AllocReg();
    etsg->StoreAccumulator(expr, rhs);

    std::string_view signature = map.at(expr->OperatorType());
    switch (expr->OperatorType()) {
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN:
        case lexer::TokenType::PUNCTUATOR_LESS_THAN:
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL:
        case lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL:
            etsg->CallBigIntBinaryComparison(expr, lhs, rhs, signature);
            break;
        default:
            etsg->CallBigIntBinaryOperator(expr, lhs, rhs, signature);
            break;
    }

    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
    return true;
}

void ETSCompiler::Compile(const ir::BinaryExpression *expr) const
{
    ETSGen *etsg = GetETSGen();

    if (CompileBigInt(etsg, expr)) {
        return;
    }

    if (etsg->TryLoadConstantExpression(expr)) {
        return;
    }

    if (expr->IsLogical()) {
        CompileLogical(etsg, expr);
        return;
    }
    if (expr->OperatorType() == lexer::TokenType::KEYW_INSTANCEOF) {
        CompileInstanceof(etsg, expr);
        return;
    }

    auto ttctx = compiler::TargetTypeContext(etsg, expr->OperationType());
    compiler::RegScope rs(etsg);
    compiler::VReg lhs = etsg->AllocReg();

    if (expr->OperatorType() == lexer::TokenType::PUNCTUATOR_PLUS && expr->OperationType()->IsETSStringType()) {
        etsg->BuildString(expr);
        return;
    }

    expr->CompileOperands(etsg, lhs);
    if (expr->OperationType()->IsIntType()) {
        etsg->ApplyCast(expr->Right(), expr->OperationType());
    }

    etsg->Binary(expr, expr->OperatorType(), lhs);
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

static void ConvertRestArguments(checker::ETSChecker *const checker, const ir::CallExpression *expr,
                                 checker::Signature *signature)
{
    if (signature->RestVar() != nullptr) {
        std::size_t const argumentCount = expr->Arguments().size();
        std::size_t const parameterCount = signature->MinArgCount();
        ASSERT(argumentCount >= parameterCount);

        auto &arguments = const_cast<ArenaVector<ir::Expression *> &>(expr->Arguments());
        std::size_t i = parameterCount;

        if (i < argumentCount && expr->Arguments()[i]->IsSpreadElement()) {
            arguments[i] = expr->Arguments()[i]->AsSpreadElement()->Argument();
        } else if (i < argumentCount && expr->Arguments()[i]->IsTSAsExpression() &&
                   expr->Arguments()[i]->AsTSAsExpression()->Expr()->Type() == ir::AstNodeType::SPREAD_ELEMENT) {
            arguments[i] = expr->Arguments()[i]->AsTSAsExpression()->Expr()->AsSpreadElement()->Argument();
        } else {
            ArenaVector<ir::Expression *> elements(checker->Allocator()->Adapter());
            for (; i < argumentCount; ++i) {
                elements.emplace_back(expr->Arguments()[i]);
            }
            auto *arrayExpression = checker->AllocNode<ir::ArrayExpression>(std::move(elements), checker->Allocator());
            arrayExpression->SetParent(const_cast<ir::CallExpression *>(expr));
            auto restType = signature->RestVar()->TsType()->AsETSArrayType();
            arrayExpression->SetTsType(restType);
            arrayExpression->SetPreferredType(restType->ElementType());
            arguments.erase(expr->Arguments().begin() + parameterCount, expr->Arguments().end());
            arguments.emplace_back(arrayExpression);
        }
    }
}

void ConvertArgumentsForFunctionalCall(checker::ETSChecker *const checker, const ir::CallExpression *expr)
{
    std::size_t const argumentCount = expr->Arguments().size();
    auto &arguments = const_cast<ArenaVector<ir::Expression *> &>(expr->Arguments());
    auto *signature = expr->Signature();

    for (size_t i = 0; i < argumentCount; i++) {
        checker::Type *paramType;
        if (i < signature->Params().size()) {
            paramType = signature->Params()[i]->TsType();
        } else {
            ASSERT(signature->RestVar() != nullptr);
            auto *restType = signature->RestVar()->TsType();
            ASSERT(restType->IsETSArrayType());
            paramType = restType->AsETSArrayType()->ElementType();
        }

        auto *arg = arguments[i];
        auto *cast = checker->Allocator()->New<ir::TSAsExpression>(arg, nullptr, false);
        arguments[i]->SetParent(cast);
        cast->SetParent(const_cast<ir::CallExpression *>(expr));
        cast->SetTsType(paramType);

        if (paramType->IsETSPrimitiveType()) {
            cast->AddBoxingUnboxingFlags(checker->GetBoxingFlag(paramType));
        }

        arguments[i] = cast;
    }
}

void ETSCompiler::Compile(const ir::BlockExpression *expr) const
{
    ETSGen *etsg = GetETSGen();

    // Nasty hack: current sccope may not be expr's parent scope.
    // For example. when expr is a field initializer, the current scope will
    // be a constructor's scope, not the class scope where the field definition resides.
    auto *oldParent = expr->Scope()->Parent();
    expr->Scope()->SetParent(const_cast<varbinder::Scope *>(etsg->Scope()));

    compiler::LocalRegScope lrs(etsg, expr->Scope());

    etsg->CompileStatements(expr->Statements());

    expr->Scope()->SetParent(oldParent);
}

bool ETSCompiler::IsSucceedCompilationProxyMemberExpr(const ir::CallExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    auto *const calleeObject = expr->callee_->AsMemberExpression()->Object();
    auto const *const enumInterface = [calleeType = calleeObject->TsType()]() -> checker::ETSEnumType const * {
        if (calleeType == nullptr) {
            return nullptr;
        }
        return calleeType->IsETSEnumType() ? calleeType->AsETSEnumType() : nullptr;
    }();

    if (enumInterface != nullptr) {
        ArenaVector<ir::Expression *> arguments(etsg->Allocator()->Adapter());

        checker::Signature *const signature = [expr, calleeObject, enumInterface, &arguments]() {
            const auto &memberProxyMethodName = expr->Signature()->InternalName();

            if (memberProxyMethodName == checker::ETSIntEnumType::TO_STRING_METHOD_NAME) {
                arguments.push_back(calleeObject);
                return enumInterface->ToStringMethod().globalSignature;
            }
            if (memberProxyMethodName == checker::ETSIntEnumType::VALUE_OF_METHOD_NAME) {
                arguments.push_back(calleeObject);
                return enumInterface->ValueOfMethod().globalSignature;
            }
            if (memberProxyMethodName == checker::ETSIntEnumType::GET_NAME_METHOD_NAME) {
                arguments.push_back(calleeObject);
                return enumInterface->GetNameMethod().globalSignature;
            }
            if (memberProxyMethodName == checker::ETSIntEnumType::VALUES_METHOD_NAME) {
                return enumInterface->ValuesMethod().globalSignature;
            }
            if (memberProxyMethodName == checker::ETSIntEnumType::GET_VALUE_OF_METHOD_NAME) {
                arguments.push_back(expr->Arguments().front());
                return enumInterface->GetValueOfMethod().globalSignature;
            }
            UNREACHABLE();
        }();

        ASSERT(signature->ReturnType() == expr->Signature()->ReturnType());
        etsg->CallExact(expr, signature, arguments);
        etsg->SetAccumulatorType(expr->TsType());
    }

    return enumInterface != nullptr;
}

void ETSCompiler::CompileDynamic(const ir::CallExpression *expr, compiler::VReg &calleeReg) const
{
    ETSGen *etsg = GetETSGen();
    auto callInfo = checker::DynamicCall::ResolveCall(etsg->VarBinder(), expr->Callee());
    if (callInfo.obj->IsETSImportDeclaration()) {
        etsg->LoadAccumulatorDynamicModule(expr, callInfo.obj->AsETSImportDeclaration());
    } else {
        callInfo.obj->Compile(etsg);
    }
    etsg->StoreAccumulator(expr, calleeReg);

    if (!callInfo.name.empty()) {
        auto [qnameStart, qnameLen] = LoadDynamicName(etsg, expr, callInfo.name, false);
        etsg->CallDynamic(ETSGen::CallDynamicData {expr, calleeReg, qnameStart}, qnameLen, expr->Signature(),
                          expr->Arguments());
    } else {
        compiler::VReg dynParam2 = etsg->AllocReg();

        auto lang = expr->Callee()->TsType()->IsETSDynamicFunctionType()
                        ? expr->Callee()->TsType()->AsETSDynamicFunctionType()->Language()
                        : expr->Callee()->TsType()->AsETSDynamicType()->Language();
        etsg->LoadUndefinedDynamic(expr, lang);
        etsg->StoreAccumulator(expr, dynParam2);
        etsg->CallDynamic(ETSGen::CallDynamicData {expr, calleeReg, dynParam2}, expr->Signature(), expr->Arguments());
    }
    etsg->SetAccumulatorType(expr->Signature()->ReturnType());

    if (etsg->GetAccumulatorType() != expr->TsType()) {
        etsg->ApplyConversion(expr, expr->TsType());
    }
}

void ETSCompiler::EmitCall(const ir::CallExpression *expr, compiler::VReg &calleeReg,
                           checker::Signature *signature) const
{
    ETSGen *etsg = GetETSGen();
    if (expr->Callee()->GetBoxingUnboxingFlags() != ir::BoxingUnboxingFlags::NONE) {
        etsg->ApplyConversionAndStoreAccumulator(expr->Callee(), calleeReg, nullptr);
    }
    if (signature->HasSignatureFlag(checker::SignatureFlags::STATIC)) {
        etsg->CallExact(expr, expr->Signature(), expr->Arguments());
    } else if ((expr->Callee()->IsMemberExpression() &&
                expr->Callee()->AsMemberExpression()->Object()->IsSuperExpression())) {
        etsg->CallExact(expr, signature, calleeReg, expr->Arguments());
    } else {
        etsg->CallVirtual(expr, signature, calleeReg, expr->Arguments());
    }

    etsg->GuardUncheckedType(expr, expr->UncheckedType(), expr->TsType());
}

static checker::Signature *ConvertArgumentsForFunctionReference(ETSGen *etsg, const ir::CallExpression *expr)
{
    checker::Signature *origSignature = expr->Signature();

    auto *funcType =
        origSignature->Owner()
            ->GetOwnProperty<checker::PropertyType::INSTANCE_METHOD>(checker::FUNCTIONAL_INTERFACE_INVOKE_METHOD_NAME)
            ->TsType()
            ->AsETSFunctionType();
    ASSERT(funcType->IsETSArrowType());
    checker::Signature *signature = funcType->CallSignatures()[0];

    if (signature->ReturnType()->IsETSPrimitiveType()) {
        expr->AddBoxingUnboxingFlags(const_cast<checker::ETSChecker *>(etsg->Checker()->AsETSChecker())
                                         ->GetUnboxingFlag(signature->ReturnType()));
    }

    ConvertArgumentsForFunctionalCall(const_cast<checker::ETSChecker *>(etsg->Checker()->AsETSChecker()), expr);

    return signature;
}

void ETSCompiler::Compile(const ir::CallExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    compiler::RegScope rs(etsg);
    compiler::VReg calleeReg = etsg->AllocReg();

    checker::Signature *signature = expr->Signature();

    if (signature->HasSignatureFlag(checker::SignatureFlags::PROXY) && expr->Callee()->IsMemberExpression()) {
        if (IsSucceedCompilationProxyMemberExpr(expr)) {
            return;
        }
    }

    bool isStatic = signature->HasSignatureFlag(checker::SignatureFlags::STATIC);
    bool isReference = false;  // expr->Signature()->HasSignatureFlag(checker::SignatureFlags::TYPE);
    bool isDynamic = expr->Callee()->TsType()->HasTypeFlag(checker::TypeFlag::ETS_DYNAMIC_FLAG);

    if (isReference) {
        signature = ConvertArgumentsForFunctionReference(etsg, expr);
    }

    ConvertRestArguments(const_cast<checker::ETSChecker *>(etsg->Checker()->AsETSChecker()), expr, signature);

    if (isDynamic) {
        CompileDynamic(expr, calleeReg);
    } else if (!isReference && expr->Callee()->IsIdentifier()) {
        if (!isStatic) {
            etsg->LoadThis(expr);
            etsg->StoreAccumulator(expr, calleeReg);
        }
        EmitCall(expr, calleeReg, signature);
    } else if (!isReference && expr->Callee()->IsMemberExpression()) {
        if (!isStatic) {
            MaybeCastUnionTypeToFunctionType(etsg, expr, signature);
            etsg->StoreAccumulator(expr, calleeReg);
        }
        EmitCall(expr, calleeReg, signature);
    } else if (expr->Callee()->IsSuperExpression() || expr->Callee()->IsThisExpression()) {
        ASSERT(!isReference && expr->IsETSConstructorCall());
        expr->Callee()->Compile(etsg);  // ctor is not a value!
        etsg->StoreAccumulator(expr, calleeReg);
        EmitCall(expr, calleeReg, signature);
    } else {
        ASSERT(isReference);
        etsg->CompileAndCheck(expr->Callee());
        etsg->StoreAccumulator(expr, calleeReg);
        EmitCall(expr, calleeReg, signature);
    }

    if (expr->HasBoxingUnboxingFlags(ir::BoxingUnboxingFlags::UNBOXING_FLAG | ir::BoxingUnboxingFlags::BOXING_FLAG)) {
        etsg->ApplyConversion(expr, expr->TsType());
    }
}

void ETSCompiler::Compile(const ir::ConditionalExpression *expr) const
{
    ETSGen *etsg = GetETSGen();

    auto *falseLabel = etsg->AllocLabel();
    auto *endLabel = etsg->AllocLabel();

    compiler::Condition::Compile(etsg, expr->Test(), falseLabel);

    auto ttctx = compiler::TargetTypeContext(etsg, expr->TsType());

    expr->Consequent()->Compile(etsg);
    etsg->ApplyConversion(expr->Consequent());
    etsg->Branch(expr, endLabel);
    etsg->SetLabel(expr, falseLabel);
    expr->Alternate()->Compile(etsg);
    etsg->ApplyConversion(expr->Alternate());
    etsg->SetLabel(expr, endLabel);
    etsg->SetAccumulatorType(expr->TsType());
}

void ETSCompiler::Compile(const ir::Identifier *expr) const
{
    ETSGen *etsg = GetETSGen();

    auto const *smartType = expr->TsType();
    if (smartType->IsETSTypeParameter() || smartType->IsETSPartialTypeParameter() || smartType->IsETSNonNullishType()) {
        smartType = etsg->Checker()->GetApparentType(smartType);
    }
    auto ttctx = compiler::TargetTypeContext(etsg, smartType);

    ASSERT(expr->Variable() != nullptr);
    if (!expr->Variable()->HasFlag(varbinder::VariableFlags::TYPE_ALIAS)) {
        etsg->LoadVar(expr, expr->Variable());
    }

    if (smartType->IsETSReferenceType()) {
        //  In case when smart cast type of identifier differs from initial variable type perform cast if required
        if (!etsg->Checker()->AsETSChecker()->Relation()->IsSupertypeOf(smartType, etsg->GetAccumulatorType())) {
            etsg->CastToReftype(expr, smartType, false);
        }
    }

    etsg->SetAccumulatorType(smartType);
}

bool ETSCompiler::CompileComputed(compiler::ETSGen *etsg, const ir::MemberExpression *expr)
{
    if (!expr->IsComputed()) {
        return false;
    }
    auto *const objectType = expr->Object()->TsType();

    auto ottctx = compiler::TargetTypeContext(etsg, expr->Object()->TsType());
    etsg->CompileAndCheck(expr->Object());

    compiler::VReg objReg = etsg->AllocReg();
    etsg->StoreAccumulator(expr, objReg);

    auto pttctx = compiler::TargetTypeContext(etsg, expr->Property()->TsType());

    etsg->CompileAndCheck(expr->Property());
    etsg->ApplyConversion(expr->Property(), expr->Property()->TsType());

    auto ttctx = compiler::TargetTypeContext(etsg, expr->TsType());

    if (objectType->IsETSDynamicType()) {
        etsg->LoadElementDynamic(expr, objReg);
    } else {
        etsg->LoadArrayElement(expr, objReg);
    }

    etsg->GuardUncheckedType(expr, expr->UncheckedType(), expr->TsType());
    etsg->ApplyConversion(expr);

    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
    return true;
}

void ETSCompiler::Compile(const ir::MemberExpression *expr) const
{
    ETSGen *etsg = GetETSGen();

    compiler::RegScope rs(etsg);

    if (CompileComputed(etsg, expr)) {
        return;
    }

    if (HandleArrayTypeLengthProperty(expr, etsg)) {
        return;
    }

    if (HandleEnumTypes(expr, etsg)) {
        return;
    }

    if (HandleStaticProperties(expr, etsg)) {
        return;
    }

    auto *const objectType = etsg->Checker()->GetApparentType(expr->Object()->TsType());
    auto &propName = expr->Property()->AsIdentifier()->Name();

    auto ottctx = compiler::TargetTypeContext(etsg, expr->Object()->TsType());
    etsg->CompileAndCheck(expr->Object());

    etsg->ApplyConversion(expr->Object());
    compiler::VReg objReg = etsg->AllocReg();
    etsg->StoreAccumulator(expr, objReg);

    auto ttctx = compiler::TargetTypeContext(etsg, expr->TsType());
    ASSERT(expr->PropVar()->TsType() != nullptr);
    const checker::Type *const variableType = expr->PropVar()->TsType();
    if (variableType->HasTypeFlag(checker::TypeFlag::GETTER_SETTER)) {
        if (expr->Object()->IsSuperExpression()) {
            etsg->CallExact(expr, variableType->AsETSFunctionType()->FindGetter()->InternalName(), objReg);
        } else {
            etsg->CallVirtual(expr, variableType->AsETSFunctionType()->FindGetter(), objReg);
        }
    } else if (objectType->IsETSDynamicType()) {
        etsg->LoadPropertyDynamic(expr, expr->TsType(), objReg, propName);
    } else if (objectType->IsETSUnionType()) {
        etsg->LoadUnionProperty(expr, expr->TsType(), objReg, propName);
    } else {
        const auto fullName = etsg->FormClassPropReference(objectType->AsETSObjectType(), propName);
        etsg->LoadProperty(expr, variableType, objReg, fullName);
    }
    etsg->GuardUncheckedType(expr, expr->UncheckedType(), expr->TsType());

    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

bool ETSCompiler::HandleArrayTypeLengthProperty(const ir::MemberExpression *expr, ETSGen *etsg) const
{
    auto *const objectType = etsg->Checker()->GetApparentType(expr->Object()->TsType());
    auto &propName = expr->Property()->AsIdentifier()->Name();
    if (objectType->IsETSArrayType() && propName.Is("length")) {
        auto ottctx = compiler::TargetTypeContext(etsg, objectType);
        etsg->CompileAndCheck(expr->Object());

        compiler::VReg objReg = etsg->AllocReg();
        etsg->StoreAccumulator(expr, objReg);

        auto ttctx = compiler::TargetTypeContext(etsg, expr->TsType());
        etsg->LoadArrayLength(expr, objReg);
        etsg->ApplyConversion(expr, expr->TsType());
        return true;
    }
    return false;
}

// NOTE(vpukhov): #20510 lowering
bool ETSCompiler::HandleEnumTypes(const ir::MemberExpression *expr, ETSGen *etsg) const
{
    auto *const exprType = etsg->Checker()->GetApparentType(expr->TsType());
    if (exprType->IsETSEnumType() && exprType->AsETSEnumType()->IsLiteralType()) {
        auto ttctx = compiler::TargetTypeContext(etsg, expr->TsType());
        etsg->LoadAccumulatorInt(expr, exprType->AsETSEnumType()->GetOrdinal());
        return true;
    }
    return false;
}

bool ETSCompiler::HandleStaticProperties(const ir::MemberExpression *expr, ETSGen *etsg) const
{
    auto &propName = expr->Property()->AsIdentifier()->Name();
    auto const *const variable = expr->PropVar();
    if (etsg->Checker()->IsVariableStatic(variable)) {
        auto ttctx = compiler::TargetTypeContext(etsg, expr->TsType());

        if (expr->PropVar()->TsType()->HasTypeFlag(checker::TypeFlag::GETTER_SETTER)) {
            checker::Signature *sig = variable->TsType()->AsETSFunctionType()->FindGetter();
            etsg->CallExact(expr, sig->InternalName());
            etsg->SetAccumulatorType(expr->TsType());
            return true;
        }

        util::StringView fullName = etsg->FormClassPropReference(expr->Object()->TsType()->AsETSObjectType(), propName);
        etsg->LoadStaticProperty(expr, expr->TsType(), fullName);

        ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
        return true;
    }
    return false;
}

void ETSCompiler::Compile(const ir::ObjectExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    compiler::RegScope rs {etsg};
    compiler::VReg objReg = etsg->AllocReg();

    // NOTE: object expressions of dynamic type are not handled in objectLiteralLowering phase
    ASSERT(expr->TsType()->IsETSDynamicType());

    auto *signatureInfo = etsg->Allocator()->New<checker::SignatureInfo>(etsg->Allocator());
    auto *createObjSig = etsg->Allocator()->New<checker::Signature>(
        signatureInfo, nullptr, compiler::Signatures::BUILTIN_JSRUNTIME_CREATE_OBJECT);
    compiler::VReg dummyReg = compiler::VReg::RegStart();
    etsg->CallDynamic(ETSGen::CallDynamicData {expr, dummyReg, dummyReg}, createObjSig,
                      ArenaVector<ir::Expression *>(etsg->Allocator()->Adapter()));

    etsg->SetAccumulatorType(expr->TsType());
    etsg->StoreAccumulator(expr, objReg);

    for (ir::Expression *propExpr : expr->Properties()) {
        ASSERT(propExpr->IsProperty());
        ir::Property *prop = propExpr->AsProperty();
        ir::Expression *key = prop->Key();
        ir::Expression *value = prop->Value();

        util::StringView pname;
        if (key->IsStringLiteral()) {
            pname = key->AsStringLiteral()->Str();
        } else if (key->IsIdentifier()) {
            pname = key->AsIdentifier()->Name();
        } else {
            UNREACHABLE();
        }

        value->Compile(etsg);
        etsg->ApplyConversion(value, key->TsType());
        if (expr->TsType()->IsETSDynamicType()) {
            etsg->StorePropertyDynamic(expr, value->TsType(), objReg, pname);
        } else {
            etsg->StoreProperty(expr, key->TsType(), objReg, pname);
        }
    }

    etsg->LoadAccumulator(expr, objReg);
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::SequenceExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    for (const auto *it : expr->Sequence()) {
        it->Compile(etsg);
    }
}

void ETSCompiler::Compile(const ir::SuperExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    etsg->LoadThis(expr);
    etsg->SetAccumulatorType(etsg->GetAccumulatorType()->AsETSObjectType()->SuperType());
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::TemplateLiteral *expr) const
{
    ETSGen *etsg = GetETSGen();
    etsg->BuildTemplateString(expr);
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::ThisExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    etsg->LoadThis(expr);
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile([[maybe_unused]] const ir::TypeofExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    ir::Expression *arg = expr->Argument();
    arg->Compile(etsg);
    if (expr->TsType()->IsETSStringType() && expr->TsType()->HasTypeFlag(checker::TypeFlag::CONSTANT)) {
        etsg->LoadAccumulatorString(expr, expr->TsType()->AsETSStringType()->GetValue());
        return;
    }
    auto argReg = etsg->AllocReg();
    etsg->StoreAccumulator(expr, argReg);
    etsg->CallExact(expr, Signatures::BUILTIN_RUNTIME_TYPEOF, argReg);
    etsg->SetAccumulatorType(expr->TsType());
}

void ETSCompiler::Compile(const ir::UnaryExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    auto ttctx = compiler::TargetTypeContext(etsg, expr->TsType());

    if (!etsg->TryLoadConstantExpression(expr->Argument())) {
        expr->Argument()->Compile(etsg);
    }

    etsg->ApplyConversion(expr->Argument(), nullptr);
    etsg->ApplyCast(expr->Argument(), expr->TsType());

    if (expr->OperatorType() == lexer::TokenType::PUNCTUATOR_DOLLAR_DOLLAR) {
        UnimplementedPathError(expr, "$$ operator can only be used with ARKUI plugin");
        etsg->SetAccumulatorType(expr->TsType());  // dead code
    } else {
        etsg->Unary(expr, expr->OperatorType());
    }

    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile([[maybe_unused]] const ir::BigIntLiteral *expr) const
{
    ETSGen *etsg = GetETSGen();
    compiler::TargetTypeContext ttctx = compiler::TargetTypeContext(etsg, expr->TsType());
    compiler::RegScope rs {etsg};
    etsg->LoadAccumulatorBigInt(expr, expr->Str());
    const compiler::VReg value = etsg->AllocReg();
    etsg->StoreAccumulator(expr, value);
    etsg->CreateBigIntObject(expr, value);
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::BooleanLiteral *expr) const
{
    ETSGen *etsg = GetETSGen();
    etsg->LoadAccumulatorBoolean(expr, expr->Value());
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::CharLiteral *expr) const
{
    ETSGen *etsg = GetETSGen();
    etsg->LoadAccumulatorChar(expr, expr->Char());
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::NullLiteral *expr) const
{
    ETSGen *etsg = GetETSGen();
    etsg->LoadAccumulatorNull(expr, expr->TsType());
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::NumberLiteral *expr) const
{
    ETSGen *etsg = GetETSGen();
    auto ttctx = compiler::TargetTypeContext(etsg, expr->TsType());

    if (expr->Number().IsInt()) {
        if (util::Helpers::IsTargetFitInSourceRange<checker::ByteType::UType, checker::IntType::UType>(
                expr->Number().GetInt())) {  // CC-OFF(G.FMT.06-CPP) project code style
            etsg->LoadAccumulatorByte(expr, static_cast<int8_t>(expr->Number().GetInt()));
        } else if (util::Helpers::IsTargetFitInSourceRange<checker::ShortType::UType, checker::IntType::UType>(
                       expr->Number().GetInt())) {  // CC-OFF(G.FMT.06-CPP) project code style
            etsg->LoadAccumulatorShort(expr, static_cast<int16_t>(expr->Number().GetInt()));
        } else {
            etsg->LoadAccumulatorInt(expr, static_cast<int32_t>(expr->Number().GetInt()));
        }
    } else if (expr->Number().IsLong()) {
        etsg->LoadAccumulatorWideInt(expr, expr->Number().GetLong());
    } else if (expr->Number().IsFloat()) {
        etsg->LoadAccumulatorFloat(expr, expr->Number().GetFloat());
    } else {
        etsg->LoadAccumulatorDouble(expr, expr->Number().GetDouble());
    }

    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->TsType()));
}

void ETSCompiler::Compile(const ir::StringLiteral *expr) const
{
    ETSGen *etsg = GetETSGen();
    etsg->LoadAccumulatorString(expr, expr->Str());
    etsg->SetAccumulatorType(expr->TsType());
}

static void ThrowError(compiler::ETSGen *const etsg, const ir::AssertStatement *st)
{
    const compiler::RegScope rs(etsg);

    if (st->Second() != nullptr) {
        st->Second()->Compile(etsg);
    } else {
        etsg->LoadAccumulatorString(st, "Assertion failed.");
    }

    const auto message = etsg->AllocReg();
    etsg->StoreAccumulator(st, message);

    const auto assertionError = etsg->AllocReg();
    etsg->NewObject(st, compiler::Signatures::BUILTIN_ASSERTION_ERROR, assertionError);
    etsg->CallExact(st, compiler::Signatures::BUILTIN_ASSERTION_ERROR_CTOR, assertionError, message);
    etsg->EmitThrow(st, assertionError);
}

void ETSCompiler::Compile(const ir::AssertStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    auto res = compiler::Condition::CheckConstantExpr(etsg, st->Test());
    if (res == compiler::Condition::Result::CONST_TRUE) {
        return;
    }

    if (res == compiler::Condition::Result::CONST_FALSE) {
        ThrowError(etsg, st);
        return;
    }

    compiler::Label *trueLabel = etsg->AllocLabel();
    compiler::Label *falseLabel = etsg->AllocLabel();

    compiler::Condition::Compile(etsg, st->Test(), falseLabel);
    etsg->JumpTo(st, trueLabel);

    etsg->SetLabel(st, falseLabel);
    ThrowError(etsg, st);

    etsg->SetLabel(st, trueLabel);
}

void ETSCompiler::Compile(const ir::BlockStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    compiler::LocalRegScope lrs(etsg, st->Scope());

    etsg->CompileStatements(st->Statements());
}

template <typename CodeGen>
static void CompileImpl(const ir::BreakStatement *self, [[maybe_unused]] CodeGen *cg)
{
    compiler::Label *target = cg->ControlFlowChangeBreak(self->Ident());
    cg->Branch(self, target);
}

void ETSCompiler::Compile(const ir::BreakStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    if (etsg->ExtendWithFinalizer(st->Parent(), st)) {
        return;
    }
    CompileImpl(st, etsg);
}

void ETSCompiler::Compile([[maybe_unused]] const ir::ClassDeclaration *st) const {}
void ETSCompiler::Compile([[maybe_unused]] const ir::AnnotationDeclaration *st) const {}
void ETSCompiler::Compile([[maybe_unused]] const ir::AnnotationUsage *st) const {}

static void CompileImpl(const ir::ContinueStatement *self, ETSGen *etsg)
{
    compiler::Label *target = etsg->ControlFlowChangeContinue(self->Ident());
    etsg->Branch(self, target);
}

void ETSCompiler::Compile(const ir::ContinueStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    if (etsg->ExtendWithFinalizer(st->Parent(), st)) {
        return;
    }
    CompileImpl(st, etsg);
}

void CompileImpl(const ir::DoWhileStatement *self, ETSGen *etsg)
{
    auto *startLabel = etsg->AllocLabel();
    compiler::LabelTarget labelTarget(etsg);

    etsg->SetLabel(self, startLabel);

    {
        compiler::LocalRegScope regScope(etsg, self->Scope());
        compiler::LabelContext labelCtx(etsg, labelTarget);
        self->Body()->Compile(etsg);
    }

    etsg->SetLabel(self, labelTarget.ContinueTarget());
    compiler::Condition::Compile(etsg, self->Test(), labelTarget.BreakTarget());

    etsg->Branch(self, startLabel);
    etsg->SetLabel(self, labelTarget.BreakTarget());
}

void ETSCompiler::Compile(const ir::DoWhileStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    CompileImpl(st, etsg);
}

void ETSCompiler::Compile([[maybe_unused]] const ir::EmptyStatement *st) const {}

void ETSCompiler::Compile(const ir::ExpressionStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    st->GetExpression()->Compile(etsg);
}

void ETSCompiler::Compile(const ir::ForOfStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    compiler::LocalRegScope declRegScope(etsg, st->Scope()->DeclScope()->InitScope());

    checker::Type const *const exprType = st->Right()->TsType();
    ASSERT(exprType->IsETSArrayType() || exprType->IsETSStringType() || exprType->IsETSUnionType());

    st->Right()->Compile(etsg);
    compiler::VReg objReg = etsg->AllocReg();
    etsg->StoreAccumulator(st, objReg);

    GetSizeInForOf(etsg, exprType, st, objReg);

    compiler::VReg sizeReg = etsg->AllocReg();
    etsg->StoreAccumulator(st, sizeReg);

    compiler::LabelTarget labelTarget(etsg);
    auto labelCtx = compiler::LabelContext(etsg, labelTarget);

    etsg->BranchIfFalse(st, labelTarget.BreakTarget());

    compiler::VReg countReg = etsg->AllocReg();
    etsg->MoveImmediateToRegister(st, countReg, checker::TypeFlag::INT, static_cast<std::int32_t>(0));
    etsg->LoadAccumulatorInt(st, static_cast<std::int32_t>(0));

    auto *const startLabel = etsg->AllocLabel();
    etsg->SetLabel(st, startLabel);

    auto lref = compiler::ETSLReference::Create(etsg, st->Left(), false);

    if (exprType->IsETSArrayType()) {
        etsg->LoadArrayElement(st, objReg);
    } else if (exprType->IsETSUnionType()) {
        HandleUnionTypeInForOf(etsg, exprType, st, objReg, &countReg);
    } else {
        etsg->LoadStringChar(st, objReg, countReg);
    }

    lref.SetValue();
    st->Body()->Compile(etsg);

    etsg->SetLabel(st, labelTarget.ContinueTarget());

    etsg->IncrementImmediateRegister(st, countReg, checker::TypeFlag::INT, static_cast<std::int32_t>(1));
    etsg->LoadAccumulator(st, countReg);

    etsg->JumpCompareRegister<compiler::Jlt>(st, sizeReg, startLabel);
    etsg->SetLabel(st, labelTarget.BreakTarget());
}

void ETSCompiler::Compile(const ir::ForUpdateStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    compiler::LocalRegScope declRegScope(etsg, st->Scope()->DeclScope()->InitScope());

    if (st->Init() != nullptr) {
        ASSERT(st->Init()->IsVariableDeclaration() || st->Init()->IsExpression());
        st->Init()->Compile(etsg);
    }

    auto *startLabel = etsg->AllocLabel();
    compiler::LabelTarget labelTarget(etsg);
    auto labelCtx = compiler::LabelContext(etsg, labelTarget);
    etsg->SetLabel(st, startLabel);

    {
        compiler::LocalRegScope regScope(etsg, st->Scope());

        if (st->Test() != nullptr) {
            compiler::Condition::Compile(etsg, st->Test(), labelTarget.BreakTarget());
        }

        st->Body()->Compile(etsg);
        etsg->SetLabel(st, labelTarget.ContinueTarget());
    }

    if (st->Update() != nullptr) {
        st->Update()->Compile(etsg);
    }

    etsg->Branch(st, startLabel);
    etsg->SetLabel(st, labelTarget.BreakTarget());
}

void ETSCompiler::Compile(const ir::IfStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    auto res = compiler::Condition::CheckConstantExpr(etsg, st->Test());
    if (res == compiler::Condition::Result::CONST_TRUE) {
        st->Test()->Compile(etsg);
        st->Consequent()->Compile(etsg);
        return;
    }

    if (res == compiler::Condition::Result::CONST_FALSE) {
        st->Test()->Compile(etsg);
        if (st->Alternate() != nullptr) {
            st->Alternate()->Compile(etsg);
        }
        return;
    }

    auto *consequentEnd = etsg->AllocLabel();
    compiler::Label *statementEnd = consequentEnd;

    compiler::Condition::Compile(etsg, st->Test(), consequentEnd);

    st->Consequent()->Compile(etsg);

    if (st->Alternate() != nullptr) {
        statementEnd = etsg->AllocLabel();
        etsg->Branch(etsg->Insns().back()->Node(), statementEnd);

        etsg->SetLabel(st, consequentEnd);
        st->Alternate()->Compile(etsg);
    }

    etsg->SetLabel(st, statementEnd);
}

void CompileImpl(const ir::LabelledStatement *self, ETSGen *cg)
{
    compiler::LabelContext labelCtx(cg, self);
    self->Body()->Compile(cg);
}

void ETSCompiler::Compile(const ir::LabelledStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    CompileImpl(st, etsg);
}

void ETSCompiler::Compile(const ir::ReturnStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    if (st->Argument() == nullptr) {
        if (etsg->ExtendWithFinalizer(st->Parent(), st)) {
            return;
        }

        if (etsg->CheckControlFlowChange()) {
            etsg->ControlFlowChangeBreak();
        }

        etsg->EmitReturnVoid(st);

        return;
    }

    if (st->Argument()->IsCallExpression() &&
        st->Argument()->AsCallExpression()->Signature()->ReturnType()->IsETSVoidType()) {
        st->Argument()->Compile(etsg);
        etsg->EmitReturnVoid(st);
        return;
    }

    auto ttctx = compiler::TargetTypeContext(etsg, etsg->ReturnType());

    if (!etsg->TryLoadConstantExpression(st->Argument())) {
        st->Argument()->Compile(etsg);
    }

    etsg->ApplyConversion(st->Argument(), st->ReturnType());

    if (etsg->ExtendWithFinalizer(st->Parent(), st)) {
        return;
    }

    if (etsg->CheckControlFlowChange()) {
        compiler::RegScope rs(etsg);
        compiler::VReg res = etsg->AllocReg();

        etsg->StoreAccumulator(st, res);
        etsg->ControlFlowChangeBreak();
        etsg->LoadAccumulator(st, res);
    }

    etsg->ReturnAcc(st);
}

static void CompileImpl(const ir::SwitchStatement *self, ETSGen *etsg)
{
    compiler::LocalRegScope lrs(etsg, self->Scope());
    compiler::SwitchBuilder builder(etsg, self);
    compiler::VReg tag = etsg->AllocReg();

    builder.CompileTagOfSwitch(tag);
    uint32_t defaultIndex = 0;

    for (size_t i = 0; i < self->Cases().size(); i++) {
        const auto *clause = self->Cases()[i];

        if (clause->Test() == nullptr) {
            defaultIndex = i;
            continue;
        }

        builder.JumpIfCase(tag, i);
    }

    if (defaultIndex > 0) {
        builder.JumpToDefault(defaultIndex);
    } else {
        builder.Break();
    }

    for (size_t i = 0; i < self->Cases().size(); i++) {
        builder.SetCaseTarget(i);
        builder.CompileCaseStatements(i);
    }
}

void ETSCompiler::Compile(const ir::SwitchStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    CompileImpl(st, etsg);
}

void ETSCompiler::Compile(const ir::ThrowStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    etsg->ThrowException(st->Argument());
}

void ETSCompiler::Compile(const ir::TryStatement *st) const
{
    ETSGen *etsg = GetETSGen();

    compiler::ETSTryContext tryCtx(etsg, etsg->Allocator(), st, st->FinallyBlock() != nullptr);

    compiler::LabelPair tryLabelPair(etsg->AllocLabel(), etsg->AllocLabel());

    for (ir::CatchClause *clause : st->CatchClauses()) {
        tryCtx.AddNewCathTable(clause->TsType()->AsETSObjectType()->AssemblerName(), tryLabelPair);
    }

    compiler::Label *statementEnd = etsg->AllocLabel();
    auto catchTables = tryCtx.GetETSCatchTable();

    etsg->SetLabel(st, tryLabelPair.Begin());
    st->Block()->Compile(etsg);
    etsg->Branch(st, statementEnd);
    etsg->SetLabel(st, tryLabelPair.End());

    ASSERT(st->CatchClauses().size() == catchTables.size());

    for (uint32_t i = 0; i < st->CatchClauses().size(); i++) {
        etsg->SetLabel(st, catchTables.at(i)->LabelSet().CatchBegin());

        st->CatchClauses().at(i)->Compile(etsg);

        etsg->Branch(st, statementEnd);
    }

    etsg->SetLabel(st, statementEnd);

    auto trycatchLabelPair = compiler::LabelPair(tryLabelPair.Begin(), statementEnd);

    tryCtx.EmitFinalizer(trycatchLabelPair, st->finalizerInsertions_);
}

void ETSCompiler::Compile(const ir::VariableDeclarator *st) const
{
    ETSGen *etsg = GetETSGen();
    auto lref = compiler::ETSLReference::Create(etsg, st->Id(), true);
    auto ttctx = compiler::TargetTypeContext(etsg, st->TsType());

    if (st->Init() != nullptr) {
        if (!etsg->TryLoadConstantExpression(st->Init())) {
            st->Init()->Compile(etsg);
            etsg->ApplyConversion(st->Init(), nullptr);
        }
    } else {
        etsg->LoadDefaultValue(st, st->Id()->AsIdentifier()->Variable()->TsType());
    }

    etsg->ApplyConversion(st, st->TsType());
    lref.SetValue();
}

void ETSCompiler::Compile(const ir::VariableDeclaration *st) const
{
    ETSGen *etsg = GetETSGen();
    for (const auto *it : st->Declarators()) {
        it->Compile(etsg);
    }
}

template <typename CodeGen>
void CompileImpl(const ir::WhileStatement *whileStmt, [[maybe_unused]] CodeGen *cg)
{
    compiler::LabelTarget labelTarget(cg);

    cg->SetLabel(whileStmt, labelTarget.ContinueTarget());
    compiler::Condition::Compile(cg, whileStmt->Test(), labelTarget.BreakTarget());

    {
        compiler::LocalRegScope regScope(cg, whileStmt->Scope());
        compiler::LabelContext labelCtx(cg, labelTarget);
        whileStmt->Body()->Compile(cg);
    }

    cg->Branch(whileStmt, labelTarget.ContinueTarget());
    cg->SetLabel(whileStmt, labelTarget.BreakTarget());
}

void ETSCompiler::Compile(const ir::WhileStatement *st) const
{
    ETSGen *etsg = GetETSGen();
    CompileImpl(st, etsg);
}

void ETSCompiler::Compile(const ir::TSArrayType *node) const
{
    ETSGen *etsg = GetETSGen();
    etsg->LoadAccumulatorNull(node, node->TsType());
}

void ETSCompiler::CompileCastUnboxable(const ir::TSAsExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    auto *targetType = etsg->Checker()->GetApparentType(expr->TsType());
    ASSERT(targetType->IsETSObjectType());

    switch (targetType->AsETSObjectType()->UnboxableKind()) {
        case checker::ETSObjectFlags::BUILTIN_BOOLEAN:
            etsg->CastToBoolean(expr);
            break;
        case checker::ETSObjectFlags::BUILTIN_BYTE:
            etsg->CastToByte(expr);
            break;
        case checker::ETSObjectFlags::BUILTIN_CHAR:
            etsg->CastToChar(expr);
            break;
        case checker::ETSObjectFlags::BUILTIN_SHORT:
            etsg->CastToShort(expr);
            break;
        case checker::ETSObjectFlags::BUILTIN_INT:
            etsg->CastToInt(expr);
            break;
        case checker::ETSObjectFlags::BUILTIN_LONG:
            etsg->CastToLong(expr);
            break;
        case checker::ETSObjectFlags::BUILTIN_FLOAT:
            etsg->CastToFloat(expr);
            break;
        case checker::ETSObjectFlags::BUILTIN_DOUBLE:
            etsg->CastToDouble(expr);
            break;
        default:
            UNREACHABLE();
    }
}

void ETSCompiler::CompileCastPrimitives(const ir::TSAsExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    auto *targetType = etsg->Checker()->GetApparentType(expr->TsType());

    switch (checker::ETSChecker::TypeKind(targetType)) {
        case checker::TypeFlag::ETS_BOOLEAN: {
            etsg->CastToBoolean(expr);
            break;
        }
        case checker::TypeFlag::CHAR: {
            etsg->CastToChar(expr);
            break;
        }
        case checker::TypeFlag::BYTE: {
            etsg->CastToByte(expr);
            break;
        }
        case checker::TypeFlag::SHORT: {
            etsg->CastToShort(expr);
            break;
        }
        case checker::TypeFlag::INT: {
            etsg->CastToInt(expr);
            break;
        }
        case checker::TypeFlag::LONG: {
            etsg->CastToLong(expr);
            break;
        }
        case checker::TypeFlag::FLOAT: {
            etsg->CastToFloat(expr);
            break;
        }
        case checker::TypeFlag::DOUBLE: {
            etsg->CastToDouble(expr);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void ETSCompiler::CompileCast(const ir::TSAsExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    auto *targetType = etsg->Checker()->GetApparentType(expr->TsType());

    switch (checker::ETSChecker::TypeKind(targetType)) {
        case checker::TypeFlag::ETS_ARRAY:
        case checker::TypeFlag::ETS_OBJECT:
        case checker::TypeFlag::ETS_TYPE_PARAMETER:
        case checker::TypeFlag::ETS_NONNULLISH:
        case checker::TypeFlag::ETS_PARTIAL_TYPE_PARAMETER:
        case checker::TypeFlag::ETS_UNION:
        case checker::TypeFlag::ETS_NULL:
        case checker::TypeFlag::ETS_UNDEFINED: {
            etsg->CastToReftype(expr, targetType, expr->isUncheckedCast_);
            break;
        }
        case checker::TypeFlag::ETS_DYNAMIC_TYPE: {
            etsg->CastToDynamic(expr, targetType->AsETSDynamicType());
            break;
        }
        // NOTE(vpukhov): #20510 lowering
        case checker::TypeFlag::ETS_STRING_ENUM:
            [[fallthrough]];
        case checker::TypeFlag::ETS_INT_ENUM: {
            auto *const acuType = etsg->GetAccumulatorType();
            if (acuType->IsETSEnumType()) {
                break;
            }
            ASSERT(acuType->IsIntType());
            auto *const signature = expr->TsType()->AsETSEnumType()->FromIntMethod().globalSignature;
            compiler::RegScope rs(etsg);
            auto arg = etsg->AllocReg();
            etsg->StoreAccumulator(expr, arg);
            etsg->CallExact(expr, signature->InternalName(), arg);
            etsg->SetAccumulatorType(signature->ReturnType());
            break;
        }
        default: {
            return CompileCastPrimitives(expr);
        }
    }
}

void ETSCompiler::Compile(const ir::TSAsExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    auto ttctx = compiler::TargetTypeContext(etsg, nullptr);
    if (!etsg->TryLoadConstantExpression(expr->Expr())) {
        expr->Expr()->Compile(etsg);
    }

    auto *targetType = etsg->Checker()->GetApparentType(expr->TsType());

    if ((expr->Expr()->GetBoxingUnboxingFlags() & ir::BoxingUnboxingFlags::UNBOXING_FLAG) != 0U) {
        etsg->ApplyUnboxingConversion(expr->Expr());
    }

    if (targetType->IsETSObjectType() &&
        ((expr->Expr()->GetBoxingUnboxingFlags() & ir::BoxingUnboxingFlags::UNBOXING_FLAG) != 0U ||
         (expr->Expr()->GetBoxingUnboxingFlags() & ir::BoxingUnboxingFlags::BOXING_FLAG) != 0U) &&
        checker::ETSChecker::TypeKind(etsg->GetAccumulatorType()) != checker::TypeFlag::ETS_OBJECT) {
        if (targetType->IsETSUnboxableObject()) {
            CompileCastUnboxable(expr);
        }
    }

    if ((expr->Expr()->GetBoxingUnboxingFlags() & ir::BoxingUnboxingFlags::BOXING_FLAG) != 0U) {
        etsg->ApplyBoxingConversion(expr->Expr());
    }

    CompileCast(expr);
    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), targetType));
}

void ETSCompiler::Compile([[maybe_unused]] const ir::TSInterfaceDeclaration *st) const {}

void ETSCompiler::Compile(const ir::TSNonNullExpression *expr) const
{
    ETSGen *etsg = GetETSGen();
    compiler::RegScope rs(etsg);

    expr->Expr()->Compile(etsg);

    if (etsg->GetAccumulatorType()->PossiblyETSNullish()) {
        auto arg = etsg->AllocReg();
        etsg->StoreAccumulator(expr, arg);
        etsg->LoadAccumulator(expr, arg);

        auto endLabel = etsg->AllocLabel();

        etsg->BranchIfNotNullish(expr, endLabel);
        etsg->EmitNullishException(expr);

        etsg->SetLabel(expr, endLabel);
        etsg->LoadAccumulator(expr, arg);
        etsg->AssumeNonNullish(expr, expr->OriginalType());
    }

    ASSERT(etsg->Checker()->Relation()->IsIdenticalTo(etsg->GetAccumulatorType(), expr->OriginalType()));
}

void ETSCompiler::Compile([[maybe_unused]] const ir::TSTypeAliasDeclaration *st) const {}
void ETSCompiler::Compile([[maybe_unused]] const ir::TSEnumDeclaration *st) const {}

}  // namespace ark::es2panda::compiler
