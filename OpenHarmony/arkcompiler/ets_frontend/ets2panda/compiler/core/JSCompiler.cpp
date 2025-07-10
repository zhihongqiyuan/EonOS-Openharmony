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

#include "JSCompiler.h"

#include "varbinder/varbinder.h"
#include "compiler/base/catchTable.h"
#include "compiler/base/condition.h"
#include "compiler/base/lreference.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/switchBuilder.h"
#include "compiler/function/functionBuilder.h"
#include "util/bitset.h"
#include "util/helpers.h"
namespace ark::es2panda::compiler {

PandaGen *JSCompiler::GetPandaGen() const
{
    return static_cast<PandaGen *>(GetCodeGen());
}

// from base folder
void JSCompiler::Compile(const ir::CatchClause *st) const
{
    PandaGen *pg = GetPandaGen();
    compiler::LocalRegScope lrs(pg, st->Scope()->ParamScope());

    if (st->Param() != nullptr) {
        auto lref = compiler::JSLReference::Create(pg, st->Param(), true);
        lref.SetValue();
    }

    ASSERT(st->Scope() == st->Body()->Scope());
    st->Body()->Compile(pg);
}

static compiler::VReg CompileHeritageClause(compiler::PandaGen *pg, const ir::ClassDefinition *node)
{
    compiler::VReg baseReg = pg->AllocReg();

    if (node->Super() != nullptr) {
        node->Super()->Compile(pg);
    } else {
        pg->LoadConst(node, compiler::Constant::JS_HOLE);
    }

    pg->StoreAccumulator(node, baseReg);
    return baseReg;
}

static void CreatePrivateElement(const ir::ClassElement *prop, const ir::MethodDefinition *propMethod,
                                 compiler::LiteralBuffer &privateBuf, util::StringView name)
{
    privateBuf.emplace_back(static_cast<uint32_t>(prop->ToPrivateFieldKind(propMethod->IsStatic())));
    privateBuf.emplace_back(name);

    const ir::ScriptFunction *func = propMethod->Value()->AsFunctionExpression()->Function();
    compiler::LiteralTag tag = compiler::LiteralTag::METHOD;
    bool isAsyncFunc = func->IsAsyncFunc();
    if (isAsyncFunc && func->IsGenerator()) {
        tag = compiler::LiteralTag::ASYNC_GENERATOR_METHOD;
    } else if (isAsyncFunc && !func->IsGenerator()) {
        tag = compiler::LiteralTag::ASYNC_METHOD;
    } else if (!isAsyncFunc && func->IsGenerator()) {
        tag = compiler::LiteralTag::GENERATOR_METHOD;
    }

    privateBuf.emplace_back(tag, func->Scope()->InternalName());
}

compiler::Literal PropertyMethodKind(const ir::MethodDefinition *propMethod, util::BitSet &compiled, size_t i)
{
    compiler::Literal value {};
    switch (propMethod->Kind()) {
        case ir::MethodDefinitionKind::METHOD: {
            const ir::FunctionExpression *func = propMethod->Value()->AsFunctionExpression();
            const util::StringView &internalName = func->Function()->Scope()->InternalName();

            value = compiler::Literal(compiler::LiteralTag::METHOD, internalName);
            compiled.Set(i);
            break;
        }
        case ir::MethodDefinitionKind::GET:
        case ir::MethodDefinitionKind::SET: {
            value = compiler::Literal::NullLiteral();
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
    return value;
}

static std::tuple<int32_t, compiler::LiteralBuffer> CreateClassStaticPropertiesBuf(compiler::LiteralBuffer &buf,
                                                                                   compiler::LiteralBuffer &privateBuf,
                                                                                   compiler::LiteralBuffer &staticBuf,
                                                                                   compiler::PandaGen *pg)
{
    uint32_t litPairs = buf.size() / 2;

    /* Static items are stored at the end of the buffer */
    buf.insert(buf.end(), staticBuf.begin(), staticBuf.end());

    /* The last literal item represents the offset of the first static property. The regular property literal count
     * is divided by 2 as key/value pairs count as one. */
    buf.emplace_back(litPairs);

    return {pg->AddLiteralBuffer(std::move(buf)), privateBuf};
}

// NOLINTNEXTLINE(google-runtime-references)
static std::tuple<size_t, compiler::LiteralBuffer, bool> CreateClassStaticPropertiesHelper(
    compiler::LiteralBuffer &literalBuf, std::unordered_map<util::StringView, size_t> &nameMap, util::StringView name,
    bool seenComputed)
{
    size_t bufferPos = literalBuf.size();
    auto res = nameMap.insert({name, bufferPos});
    if (res.second) {
        if (seenComputed) {
            return {bufferPos, literalBuf, true};
        }

        literalBuf.emplace_back(name);
        literalBuf.emplace_back();
    } else {
        bufferPos = res.first->second;
    }
    return {bufferPos, literalBuf, false};
}

// NOLINTNEXTLINE(google-runtime-references)
static std::tuple<int32_t, compiler::LiteralBuffer> CreateClassStaticProperties(
    compiler::PandaGen *pg, util::BitSet &compiled, const ArenaVector<ir::AstNode *> &properties)
{
    compiler::LiteralBuffer buf {};
    compiler::LiteralBuffer privateBuf {};
    compiler::LiteralBuffer staticBuf {};
    bool seenComputed = false;
    std::unordered_map<util::StringView, size_t> propNameMap;
    std::unordered_map<util::StringView, size_t> staticPropNameMap;

    for (size_t i = 0; i < properties.size(); i++) {
        const ir::ClassElement *prop = properties[i]->AsClassElement();

        if (prop->IsClassStaticBlock()) {
            continue;
        }

        if (prop->IsClassProperty() && prop->IsPrivateElement()) {
            bool isStatic = prop->IsStatic();
            privateBuf.emplace_back(static_cast<uint32_t>(prop->ToPrivateFieldKind(isStatic)));
            privateBuf.emplace_back(prop->Id()->Name());
            continue;
        }
        if (prop->IsClassProperty() && !prop->IsPrivateElement()) {
            continue;
        }

        ASSERT(prop->IsMethodDefinition());
        const ir::MethodDefinition *propMethod = prop->AsMethodDefinition();

        if (!util::Helpers::IsConstantPropertyKey(propMethod->Key(), propMethod->IsComputed()) ||
            (propMethod->IsComputed() && util::Helpers::IsSpecialPropertyKey(propMethod->Key()))) {
            seenComputed = true;
            continue;
        }

        util::StringView name = util::Helpers::LiteralToPropName(prop->Key());
        compiler::LiteralBuffer &literalBuf = prop->IsStatic() ? staticBuf : buf;
        auto &nameMap = prop->IsStatic() ? staticPropNameMap : propNameMap;

        if (prop->IsPrivateElement()) {
            CreatePrivateElement(prop, propMethod, privateBuf, name);
            compiled.Set(i);
            continue;
        }

        size_t bufferPos = literalBuf.size();
        bool stop = false;
        std::tie(bufferPos, literalBuf, stop) =
            CreateClassStaticPropertiesHelper(literalBuf, nameMap, name, seenComputed);
        if (stop) {
            break;
        }

        compiler::Literal value = PropertyMethodKind(propMethod, compiled, i);

        literalBuf[bufferPos + 1] = std::move(value);
    }

    return CreateClassStaticPropertiesBuf(buf, privateBuf, staticBuf, pg);
}

static void CompileStaticFieldInitializers(compiler::PandaGen *pg, compiler::VReg classReg,
                                           const std::vector<compiler::VReg> &staticComputedFieldKeys,
                                           const ir::ClassDefinition *node)
{
    const auto &properties = node->Body();
    auto iter = staticComputedFieldKeys.begin();

    if (node->HasPrivateMethod()) {
        pg->ClassPrivateMethodOrAccessorAdd(node, classReg, classReg);
    }

    for (const auto *it : properties) {
        compiler::RegScope rs(pg);

        if (it->IsClassStaticBlock()) {
            const auto *func = it->AsClassStaticBlock()->Value()->AsFunctionExpression()->Function();

            compiler::VReg funcReg = pg->AllocReg();
            compiler::VReg thisReg = pg->AllocReg();

            pg->LoadAccumulator(it, classReg);
            pg->StoreAccumulator(it, thisReg);
            pg->DefineMethod(it, func->Scope()->InternalName());
            pg->StoreAccumulator(it, funcReg);

            pg->Call0This(node, funcReg, thisReg);
            continue;
        }

        if (it->IsMethodDefinition()) {
            continue;
        }

        ASSERT(it->IsClassProperty());
        const ir::ClassProperty *prop = it->AsClassProperty();

        if (!prop->IsStatic()) {
            continue;
        }

        compiler::VReg keyReg {};

        if (prop->IsComputed()) {
            ASSERT(iter != staticComputedFieldKeys.end());
            keyReg = *iter++;
        } else if (!prop->IsPrivateElement()) {
            keyReg = pg->LoadPropertyKey(prop->Key(), false);
        }

        if (prop->Value() == nullptr) {
            pg->LoadConst(prop, compiler::Constant::JS_UNDEFINED);
        } else {
            compiler::RegScope vrs(pg);
            prop->Value()->Compile(pg);
        }

        if (prop->IsPrivateElement()) {
            pg->ClassPrivateFieldAdd(prop, classReg, classReg, prop->Id()->Name());
            continue;
        }

        pg->ClassFieldAdd(prop, classReg, keyReg);
    }
}

static void CompilePropertyKind(const ir::MethodDefinition *prop, compiler::VReg dest, compiler::PandaGen *pg,
                                const ir::ClassDefinition *node)
{
    switch (prop->Kind()) {
        case ir::MethodDefinitionKind::METHOD: {
            compiler::Operand key = pg->ToOwnPropertyKey(prop->Key(), prop->IsComputed());

            pg->LoadAccumulator(node, dest);
            const ir::FunctionExpression *func = prop->Value()->AsFunctionExpression();
            func->Compile(pg);

            pg->StoreOwnProperty(prop->Value()->Parent(), dest, key);
            break;
        }
        case ir::MethodDefinitionKind::GET:
        case ir::MethodDefinitionKind::SET: {
            compiler::VReg keyReg = pg->LoadPropertyKey(prop->Key(), prop->IsComputed());

            compiler::VReg undef = pg->AllocReg();
            pg->LoadConst(node, compiler::Constant::JS_UNDEFINED);
            pg->StoreAccumulator(node, undef);

            compiler::VReg getter = undef;
            compiler::VReg setter = undef;

            pg->LoadAccumulator(node, dest);

            compiler::VReg accessor = pg->AllocReg();
            prop->Value()->Compile(pg);
            pg->StoreAccumulator(prop->Value(), accessor);

            if (prop->Kind() == ir::MethodDefinitionKind::GET) {
                getter = accessor;
            } else {
                setter = accessor;
            }

            pg->DefineGetterSetterByValue(node, std::make_tuple(dest, keyReg, getter, setter), prop->IsComputed());
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

static void CompileMissingProperties(compiler::PandaGen *pg, const util::BitSet &compiled, compiler::VReg classReg,
                                     const ir::ClassDefinition *node)
{
    const auto &properties = node->Body();
    std::vector<compiler::VReg> staticComputedFieldKeys;
    compiler::VReg protoReg = pg->AllocReg();
    compiler::VReg computedInstanceFieldsArray {};
    uint32_t computedInstanceFieldsIndex = 0;

    pg->LoadObjByName(node, "prototype");
    pg->StoreAccumulator(node, protoReg);

    if (node->HasComputedInstanceField()) {
        pg->CreateEmptyArray(node);
        computedInstanceFieldsArray = pg->AllocReg();
        pg->StoreAccumulator(node, computedInstanceFieldsArray);
    }

    for (size_t i = 0; i < properties.size(); i++) {
        if (compiled.Test(i)) {
            continue;
        }

        if (properties[i]->IsClassStaticBlock()) {
            continue;
        }

        if (properties[i]->IsMethodDefinition()) {
            const ir::MethodDefinition *prop = properties[i]->AsMethodDefinition();
            compiler::VReg dest = prop->IsStatic() ? classReg : protoReg;
            compiler::RegScope rs(pg);
            CompilePropertyKind(prop, dest, pg, node);

            continue;
        }

        ASSERT(properties[i]->IsClassProperty());
        const ir::ClassProperty *prop = properties[i]->AsClassProperty();

        if (!prop->IsComputed()) {
            continue;
        }

        if (prop->IsStatic()) {
            compiler::VReg keyReg = pg->LoadPropertyKey(prop->Key(), prop->IsComputed());
            staticComputedFieldKeys.push_back(keyReg);
            continue;
        }

        pg->LoadPropertyKeyAcc(prop->Key(), prop->IsComputed());
        pg->StOwnByIndex(node, computedInstanceFieldsArray, computedInstanceFieldsIndex++);
    }

    if (computedInstanceFieldsIndex != 0) {
        pg->SetClassComputedFields(node, classReg, computedInstanceFieldsArray);
    }

    CompileStaticFieldInitializers(pg, classReg, staticComputedFieldKeys, node);
}

static void InitializeClassName(compiler::PandaGen *pg, const ir::ClassDefinition *node)
{
    if (node->Ident() == nullptr) {
        return;
    }

    auto lref = compiler::JSLReference::Create(pg, node->Ident(), true);
    lref.SetValue();
}

void JSCompiler::Compile(const ir::ClassDefinition *node) const
{
    PandaGen *pg = GetPandaGen();
    compiler::RegScope rs(pg);
    compiler::VReg classReg = pg->AllocReg();
    compiler::VReg lexenv = pg->LexEnv();

    compiler::LocalRegScope lrs(pg, node->Scope());

    compiler::VReg baseReg = CompileHeritageClause(pg, node);
    util::StringView ctorId = node->Ctor()->Function()->Scope()->InternalName();
    util::BitSet compiled(node->Body().size());

    auto [bufIdx, privateBuf] = CreateClassStaticProperties(pg, compiled, node->Body());

    pg->DefineClassWithBuffer(node, ctorId, bufIdx, lexenv, baseReg);
    pg->StoreAccumulator(node, classReg);

    if (!privateBuf.empty()) {
        pg->DefineClassPrivateFields(node, pg->AddLiteralBuffer(std::move(privateBuf)));
    }

    auto res = pg->Scope()->Find(node->PrivateId());
    ASSERT(res.variable);

    if (res.variable->AsLocalVariable()->LexicalBound()) {
        pg->StoreLexicalVar(node, res.lexLevel, res.variable->AsLocalVariable()->LexIdx());
    }

    InitializeClassName(pg, node);

    CompileMissingProperties(pg, compiled, classReg, node);

    pg->LoadAccumulator(node, classReg);
}

void JSCompiler::Compile(const ir::MetaProperty *expr) const
{
    PandaGen *pg = GetPandaGen();
    if (expr->Kind() == ir::MetaProperty::MetaPropertyKind::NEW_TARGET) {
        pg->GetNewTarget(expr);
        return;
    }

    if (expr->Kind() == ir::MetaProperty::MetaPropertyKind::IMPORT_META) {
        // NOTE
        pg->Unimplemented();
    }
}

// JSCompiler::compile methods for EXPRESSIONS in alphabetical order
void JSCompiler::Compile(const ir::ArrayExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    compiler::RegScope rs(pg);
    compiler::VReg arrayObj = pg->AllocReg();

    pg->CreateArray(expr, expr->Elements(), arrayObj);
}

void JSCompiler::Compile(const ir::ArrowFunctionExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    pg->DefineFunction(expr->Function(), expr->Function(), expr->Function()->Scope()->InternalName());
}

void JSCompiler::Compile(const ir::AssignmentExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    compiler::RegScope rs(pg);
    auto lref = compiler::JSLReference::Create(pg, expr->Left(), false);

    if (expr->OperatorType() == lexer::TokenType::PUNCTUATOR_LOGICAL_AND_EQUAL ||
        expr->OperatorType() == lexer::TokenType::PUNCTUATOR_LOGICAL_OR_EQUAL) {
        compiler::PandaGen::Unimplemented();
    }

    if (expr->OperatorType() == lexer::TokenType::PUNCTUATOR_SUBSTITUTION) {
        expr->Right()->Compile(pg);
        lref.SetValue();
        return;
    }

    compiler::VReg lhsReg = pg->AllocReg();

    lref.GetValue();
    pg->StoreAccumulator(expr->Left(), lhsReg);
    expr->Right()->Compile(pg);
    pg->Binary(expr, expr->OperatorType(), lhsReg);

    lref.SetValue();
}

void JSCompiler::Compile(const ir::AwaitExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    compiler::RegScope rs(pg);

    if (expr->Argument() != nullptr) {
        expr->Argument()->Compile(pg);
    } else {
        pg->LoadConst(expr, compiler::Constant::JS_UNDEFINED);
    }

    pg->EmitAwait(expr);
}

static void CompileLogical(compiler::PandaGen *pg, const ir::BinaryExpression *expr)
{
    compiler::RegScope rs(pg);
    compiler::VReg lhs = pg->AllocReg();

    ASSERT(expr->OperatorType() == lexer::TokenType::PUNCTUATOR_LOGICAL_AND ||
           expr->OperatorType() == lexer::TokenType::PUNCTUATOR_LOGICAL_OR ||
           expr->OperatorType() == lexer::TokenType::PUNCTUATOR_NULLISH_COALESCING);

    auto *skipRight = pg->AllocLabel();
    auto *endLabel = pg->AllocLabel();

    // left -> acc -> lhs -> toboolean -> acc -> bool_lhs
    expr->Left()->Compile(pg);
    pg->StoreAccumulator(expr, lhs);

    if (expr->OperatorType() == lexer::TokenType::PUNCTUATOR_LOGICAL_AND) {
        pg->ToBoolean(expr);
        pg->BranchIfFalse(expr, skipRight);
    } else if (expr->OperatorType() == lexer::TokenType::PUNCTUATOR_LOGICAL_OR) {
        pg->ToBoolean(expr);
        pg->BranchIfTrue(expr, skipRight);
    } else if (expr->OperatorType() == lexer::TokenType::PUNCTUATOR_NULLISH_COALESCING) {
        pg->BranchIfCoercible(expr, skipRight);
    }

    // left is true/false(and/or) then right -> acc
    expr->Right()->Compile(pg);
    pg->Branch(expr, endLabel);

    // left is false/true(and/or) then lhs -> acc
    pg->SetLabel(expr, skipRight);
    pg->LoadAccumulator(expr, lhs);
    pg->SetLabel(expr, endLabel);
}

void JSCompiler::Compile(const ir::BinaryExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    if (expr->IsLogical()) {
        CompileLogical(pg, expr);
        return;
    }

    if (expr->OperatorType() == lexer::TokenType::KEYW_IN && expr->Left()->IsIdentifier() &&
        expr->Left()->AsIdentifier()->IsPrivateIdent()) {
        compiler::RegScope rs(pg);
        compiler::VReg ctor = pg->AllocReg();
        const auto &name = expr->Left()->AsIdentifier()->Name();
        compiler::Function::LoadClassContexts(expr, pg, ctor, name);
        expr->Right()->Compile(pg);
        pg->ClassPrivateFieldIn(expr, ctor, name);
        return;
    }

    compiler::RegScope rs(pg);
    compiler::VReg lhs = pg->AllocReg();

    expr->Left()->Compile(pg);
    pg->StoreAccumulator(expr, lhs);
    expr->Right()->Compile(pg);

    pg->Binary(expr, expr->OperatorType(), lhs);
}

static compiler::VReg CreateSpreadArguments(compiler::PandaGen *pg, const ir::CallExpression *expr)
{
    compiler::VReg argsObj = pg->AllocReg();
    pg->CreateArray(expr, expr->Arguments(), argsObj);

    return argsObj;
}

void CompileSuperExprWithoutSpread(PandaGen *pg, const ir::CallExpression *expr)
{
    compiler::RegScope paramScope(pg);
    compiler::VReg argStart {};

    if (expr->Arguments().empty()) {
        argStart = pg->AllocReg();
        pg->StoreConst(expr, argStart, compiler::Constant::JS_UNDEFINED);
    } else {
        argStart = pg->NextReg();
    }

    for (const auto *it : expr->Arguments()) {
        compiler::VReg arg = pg->AllocReg();
        it->Compile(pg);
        pg->StoreAccumulator(it, arg);
    }

    pg->GetFunctionObject(expr);
    pg->SuperCall(expr, argStart, expr->Arguments().size());
}

void JSCompiler::Compile(const ir::CallExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    compiler::RegScope rs(pg);
    bool containsSpread = util::Helpers::ContainSpreadElement(expr->Arguments());

    if (expr->Callee()->IsSuperExpression()) {
        if (containsSpread) {
            compiler::RegScope paramScope(pg);
            compiler::VReg argsObj = CreateSpreadArguments(pg, expr);

            pg->GetFunctionObject(expr);
            pg->SuperCallSpread(expr, argsObj);
        } else {
            CompileSuperExprWithoutSpread(pg, expr);
        }

        compiler::VReg newThis = pg->AllocReg();
        pg->StoreAccumulator(expr, newThis);

        pg->GetThis(expr);
        pg->ThrowIfSuperNotCorrectCall(expr, 1);

        pg->LoadAccumulator(expr, newThis);
        pg->SetThis(expr);

        compiler::Function::CompileInstanceFields(pg, pg->RootNode()->AsScriptFunction());
        return;
    }

    compiler::VReg callee = pg->AllocReg();
    compiler::VReg thisReg = compiler::VReg::Invalid();

    if (expr->Callee()->IsMemberExpression()) {
        thisReg = pg->AllocReg();

        compiler::RegScope mrs(pg);
        expr->Callee()->AsMemberExpression()->CompileToReg(pg, thisReg);
    } else if (expr->Callee()->IsChainExpression()) {
        thisReg = pg->AllocReg();

        compiler::RegScope mrs(pg);
        expr->Callee()->AsChainExpression()->CompileToReg(pg, thisReg);
    } else {
        expr->Callee()->Compile(pg);
    }

    pg->StoreAccumulator(expr, callee);
    pg->OptionalChainCheck(expr->IsOptional(), callee);

    if (containsSpread || expr->Arguments().size() >= compiler::PandaGen::MAX_RANGE_CALL_ARG) {
        if (thisReg.IsInvalid()) {
            thisReg = pg->AllocReg();
            pg->StoreConst(expr, thisReg, compiler::Constant::JS_UNDEFINED);
        }

        compiler::VReg argsObj = CreateSpreadArguments(pg, expr);
        pg->CallSpread(expr, callee, thisReg, argsObj);
    } else {
        pg->Call(expr, callee, thisReg, expr->Arguments());
    }
}

void JSCompiler::Compile(const ir::ChainExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    compiler::OptionalChain chain(pg, expr);
    expr->GetExpression()->Compile(pg);
}

void JSCompiler::Compile(const ir::ClassExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    expr->Definition()->Compile(pg);
}

template <typename CodeGen>
static void CompileImpl(const ir::ConditionalExpression *self, CodeGen *cg)
{
    auto *falseLabel = cg->AllocLabel();
    auto *endLabel = cg->AllocLabel();

    compiler::Condition::Compile(cg, self->Test(), falseLabel);
    self->Consequent()->Compile(cg);
    cg->Branch(self, endLabel);
    cg->SetLabel(self, falseLabel);
    self->Alternate()->Compile(cg);
    cg->SetLabel(self, endLabel);
}

void JSCompiler::Compile(const ir::ConditionalExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    CompileImpl(expr, pg);
}

void JSCompiler::Compile(const ir::DirectEvalExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    if (expr->Arguments().empty()) {
        pg->LoadConst(expr, compiler::Constant::JS_UNDEFINED);
        return;
    }

    compiler::RegScope rs(pg);
    bool containsSpread = util::Helpers::ContainSpreadElement(expr->Arguments());
    if (containsSpread) {
        [[maybe_unused]] compiler::VReg argsObj = CreateSpreadArguments(pg, expr);
        pg->LoadObjByIndex(expr, 0);
    } else {
        compiler::VReg arg0 = pg->AllocReg();
        auto iter = expr->Arguments().cbegin();
        (*iter++)->Compile(pg);
        pg->StoreAccumulator(expr, arg0);

        while (iter != expr->Arguments().cend()) {
            (*iter++)->Compile(pg);
        }

        pg->LoadAccumulator(expr, arg0);
    }

    pg->DirectEval(expr, expr->parserStatus_);
}

void JSCompiler::Compile(const ir::FunctionExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    pg->DefineFunction(expr->Function(), expr->Function(), expr->Function()->Scope()->InternalName());
}

void JSCompiler::Compile(const ir::Identifier *expr) const
{
    PandaGen *pg = GetPandaGen();
    auto res = pg->Scope()->Find(expr->Name());
    if (res.variable != nullptr) {
        pg->LoadVar(expr, res);
        return;
    }

    if (pg->IsDirectEval()) {
        pg->LoadEvalVariable(expr, expr->Name());
        return;
    }

    if (expr->Name().Is("NaN")) {
        pg->LoadConst(expr, compiler::Constant::JS_NAN);
        return;
    }

    if (expr->Name().Is("Infinity")) {
        pg->LoadConst(expr, compiler::Constant::JS_INFINITY);
        return;
    }

    if (expr->Name().Is("globalThis")) {
        pg->LoadConst(expr, compiler::Constant::JS_GLOBAL);
        return;
    }

    if (expr->Name().Is("undefined")) {
        pg->LoadConst(expr, compiler::Constant::JS_UNDEFINED);
        return;
    }

    pg->TryLoadGlobalByName(expr, expr->Name());
}

void JSCompiler::Compile([[maybe_unused]] const ir::ImportExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    pg->Unimplemented();
}

void JSCompiler::Compile(const ir::MemberExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    expr->Object()->Compile(pg);
    pg->OptionalChainCheck(expr->IsOptional(), compiler::VReg::Invalid());
    expr->LoadRhs(pg);
}

void JSCompiler::Compile(const ir::NewExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    compiler::RegScope rs(pg);
    compiler::VReg ctor = pg->AllocReg();
    compiler::VReg newTarget = pg->AllocReg();

    expr->Callee()->Compile(pg);
    pg->StoreAccumulator(expr, ctor);

    // new.Target will be the same as ctor
    pg->StoreAccumulator(expr, newTarget);

    if (!util::Helpers::ContainSpreadElement(expr->Arguments()) &&
        expr->Arguments().size() < compiler::PandaGen::MAX_RANGE_CALL_ARG) {
        for (const auto *it : expr->Arguments()) {
            compiler::VReg arg = pg->AllocReg();
            it->Compile(pg);
            pg->StoreAccumulator(expr, arg);
        }

        pg->NewObject(expr, ctor, expr->Arguments().size() + 2U);
    } else {
        compiler::VReg argsObj = pg->AllocReg();

        pg->CreateArray(expr, expr->Arguments(), argsObj);
        pg->NewObjSpread(expr, ctor, newTarget);
    }
}

void JSCompiler::Compile(const ir::ObjectExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    if (expr->Properties().empty()) {
        pg->CreateEmptyObject(expr);
        return;
    }

    util::BitSet compiled(expr->Properties().size());
    CompileStaticProperties(pg, &compiled, expr);

    if (compiled.Any(false)) {
        CompileRemainingProperties(pg, &compiled, expr);
    }
}

static compiler::Literal CreateLiteral(const ir::Property *prop, util::BitSet *compiled, size_t propIndex)
{
    compiler::Literal lit = util::Helpers::ToConstantLiteral(prop->Value());
    if (!lit.IsInvalid()) {
        compiled->Set(propIndex);
        return lit;
    }

    if (prop->Kind() != ir::PropertyKind::INIT) {
        ASSERT(prop->IsAccessor());
        return compiler::Literal::AccessorLiteral();
    }

    if (!prop->Value()->IsFunctionExpression()) {
        return compiler::Literal::NullLiteral();
    }

    const ir::ScriptFunction *method = prop->Value()->AsFunctionExpression()->Function();

    compiler::LiteralTag tag = compiler::LiteralTag::METHOD;

    if (method->IsGenerator()) {
        tag = compiler::LiteralTag::GENERATOR_METHOD;

        if (method->IsAsyncFunc()) {
            tag = compiler::LiteralTag::ASYNC_GENERATOR_METHOD;
        }
    }

    compiled->Set(propIndex);
    return compiler::Literal(tag, method->Scope()->InternalName());
}

static bool IsLiteralBufferCompatible(const ir::Expression *expr)
{
    if (expr->IsSpreadElement()) {
        return false;
    }

    const ir::Property *prop = expr->AsProperty();
    if (prop->Value()->IsFunctionExpression() && !prop->Value()->AsFunctionExpression()->Function()->IsMethod()) {
        return false;
    }

    return util::Helpers::IsConstantPropertyKey(prop->Key(), prop->IsComputed()) &&
           prop->Kind() != ir::PropertyKind::PROTO;
}

void JSCompiler::CompileStaticProperties(compiler::PandaGen *pg, util::BitSet *compiled,
                                         const ir::ObjectExpression *expr) const
{
    bool hasMethod = false;
    bool seenComputed = false;
    compiler::LiteralBuffer buf;
    std::unordered_map<util::StringView, size_t> propNameMap;

    for (size_t i = 0; i < expr->Properties().size(); i++) {
        if (!IsLiteralBufferCompatible(expr->Properties()[i])) {
            seenComputed = true;
            continue;
        }

        const ir::Property *prop = expr->Properties()[i]->AsProperty();

        util::StringView name = util::Helpers::LiteralToPropName(prop->Key());
        size_t bufferPos = buf.size();
        auto res = propNameMap.insert({name, bufferPos});
        if (res.second) {
            if (seenComputed) {
                break;
            }

            buf.emplace_back(name);
            buf.emplace_back();
        } else {
            bufferPos = res.first->second;
        }

        compiler::Literal lit = CreateLiteral(prop, compiled, i);
        if (lit.IsTagMethod()) {
            hasMethod = true;
        }

        buf[bufferPos + 1] = std::move(lit);
    }

    if (buf.empty()) {
        pg->CreateEmptyObject(expr);
        return;
    }

    uint32_t bufIdx = pg->AddLiteralBuffer(std::move(buf));

    if (hasMethod) {
        pg->CreateObjectHavingMethod(expr, bufIdx);
    } else {
        pg->CreateObjectWithBuffer(expr, bufIdx);
    }
}

void CompileRemainingPropertyKind(const ir::Property *prop, compiler::VReg objReg, compiler::PandaGen *pg,
                                  const ir::ObjectExpression *expr)
{
    switch (prop->Kind()) {
        case ir::PropertyKind::GET:
        case ir::PropertyKind::SET: {
            compiler::VReg key = pg->LoadPropertyKey(prop->Key(), prop->IsComputed());

            compiler::VReg undef = pg->AllocReg();
            pg->LoadConst(expr, compiler::Constant::JS_UNDEFINED);
            pg->StoreAccumulator(expr, undef);

            compiler::VReg getter = undef;
            compiler::VReg setter = undef;

            compiler::VReg accessor = pg->AllocReg();
            pg->LoadAccumulator(prop->Value(), objReg);
            prop->Value()->Compile(pg);
            pg->StoreAccumulator(prop->Value(), accessor);

            if (prop->Kind() == ir::PropertyKind::GET) {
                getter = accessor;
            } else {
                setter = accessor;
            }

            pg->DefineGetterSetterByValue(expr, std::make_tuple(objReg, key, getter, setter), prop->IsComputed());
            break;
        }
        case ir::PropertyKind::INIT: {
            compiler::Operand key = pg->ToOwnPropertyKey(prop->Key(), prop->IsComputed());

            if (prop->IsMethod()) {
                pg->LoadAccumulator(prop->Value(), objReg);
            }

            prop->Value()->Compile(pg);
            pg->StoreOwnProperty(expr, objReg, key);
            break;
        }
        case ir::PropertyKind::PROTO: {
            prop->Value()->Compile(pg);
            compiler::VReg proto = pg->AllocReg();
            pg->StoreAccumulator(expr, proto);

            pg->SetObjectWithProto(expr, proto, objReg);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void JSCompiler::CompileRemainingProperties(compiler::PandaGen *pg, const util::BitSet *compiled,
                                            const ir::ObjectExpression *expr) const
{
    compiler::RegScope rs(pg);
    compiler::VReg objReg = pg->AllocReg();

    pg->StoreAccumulator(expr, objReg);

    for (size_t i = 0; i < expr->Properties().size(); i++) {
        if (compiled->Test(i)) {
            continue;
        }

        compiler::RegScope prs(pg);

        if (expr->Properties()[i]->IsSpreadElement()) {
            compiler::VReg srcObj = pg->AllocReg();
            auto const *const spread = expr->Properties()[i]->AsSpreadElement();

            spread->Argument()->Compile(pg);
            pg->StoreAccumulator(spread, srcObj);

            pg->CopyDataProperties(spread, objReg, srcObj);
            continue;
        }

        const ir::Property *prop = expr->Properties()[i]->AsProperty();
        CompileRemainingPropertyKind(prop, objReg, pg, expr);
    }

    pg->LoadAccumulator(expr, objReg);
}

void JSCompiler::Compile(const ir::SequenceExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    for (const auto *it : expr->Sequence()) {
        it->Compile(pg);
    }
}

void JSCompiler::Compile(const ir::SuperExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    pg->GetThis(expr);

    const ir::ScriptFunction *func = util::Helpers::GetContainingConstructor(expr);

    if (func != nullptr) {
        pg->ThrowIfSuperNotCorrectCall(expr, 0);
    }
}

void JSCompiler::Compile(const ir::TaggedTemplateExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    compiler::RegScope rs(pg);
    compiler::VReg callee = pg->AllocReg();
    compiler::VReg thisReg = compiler::VReg::Invalid();

    if (expr->Tag()->IsMemberExpression()) {
        thisReg = pg->AllocReg();
        compiler::RegScope mrs(pg);
        expr->Tag()->AsMemberExpression()->CompileToReg(pg, thisReg);
    } else {
        expr->Tag()->Compile(pg);
    }

    pg->CallTagged(expr, callee, thisReg, expr->Quasi()->Expressions());
}

void JSCompiler::Compile(const ir::TemplateLiteral *expr) const
{
    PandaGen *pg = GetPandaGen();
    auto quasisIt = expr->Quasis().begin();
    auto expressionIt = expr->Expressions().begin();

    pg->LoadAccumulatorString(expr, (*quasisIt)->Raw());

    quasisIt++;

    bool isQuais = false;
    size_t total = expr->Quasis().size() + expr->Expressions().size();

    compiler::RegScope rs(pg);
    compiler::VReg lhs = pg->AllocReg();

    while (total != 1) {
        const ir::AstNode *node = nullptr;

        if (isQuais) {
            pg->StoreAccumulator(*quasisIt, lhs);
            pg->LoadAccumulatorString(expr, (*quasisIt)->Raw());

            node = *quasisIt;
            quasisIt++;
        } else {
            const ir::Expression *element = *expressionIt;
            pg->StoreAccumulator(element, lhs);

            element->Compile(pg);

            node = element;
            expressionIt++;
        }

        pg->Binary(node, lexer::TokenType::PUNCTUATOR_PLUS, lhs);

        isQuais = !isQuais;
        total--;
    }
}

void JSCompiler::Compile(const ir::ThisExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    auto res = pg->Scope()->Find(varbinder::VarBinder::MANDATORY_PARAM_THIS);

    ASSERT(res.variable && res.variable->IsLocalVariable());
    pg->LoadAccFromLexEnv(expr, res);

    const ir::ScriptFunction *func = util::Helpers::GetContainingConstructor(expr);

    if (func != nullptr) {
        pg->ThrowIfSuperNotCorrectCall(expr, 0);
    }
}

void JSCompiler::Compile([[maybe_unused]] const ir::TypeofExpression *expr) const
{
    PandaGen *pg = GetPandaGen();

    if (expr->Argument()->IsIdentifier()) {
        const auto *ident = expr->Argument()->AsIdentifier();

        auto res = pg->Scope()->Find(ident->Name());
        if (res.variable == nullptr) {
            pg->LoadConst(expr, compiler::Constant::JS_GLOBAL);
            pg->LoadObjByName(expr, ident->Name());
        } else {
            pg->LoadVar(ident, res);
        }
    } else {
        expr->Argument()->Compile(pg);
    }

    pg->TypeOf(expr);
}

void JSCompiler::Compile(const ir::UnaryExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    switch (expr->OperatorType()) {
        case lexer::TokenType::KEYW_DELETE: {
            if (expr->Argument()->IsIdentifier()) {
                auto result = pg->Scope()->Find(expr->Argument()->AsIdentifier()->Name());
                if (result.variable == nullptr ||
                    (result.scope->IsGlobalScope() && result.variable->IsGlobalVariable())) {
                    compiler::RegScope rs(pg);
                    compiler::VReg variable = pg->AllocReg();
                    compiler::VReg global = pg->AllocReg();

                    pg->LoadConst(expr, compiler::Constant::JS_GLOBAL);
                    pg->StoreAccumulator(expr, global);

                    pg->LoadAccumulatorString(expr, expr->Argument()->AsIdentifier()->Name());
                    pg->StoreAccumulator(expr, variable);

                    pg->DeleteObjProperty(expr, global, variable);
                } else {
                    // Otherwise it is a local variable which can't be deleted and we just
                    // return false.
                    pg->LoadConst(expr, compiler::Constant::JS_FALSE);
                }
            } else if (expr->Argument()->IsMemberExpression()) {
                compiler::RegScope rs(pg);
                compiler::VReg object = pg->AllocReg();
                compiler::VReg property = pg->AllocReg();

                expr->Argument()->AsMemberExpression()->CompileToRegs(pg, object, property);
                pg->DeleteObjProperty(expr, object, property);
            } else {
                // compile the delete operand.
                expr->Argument()->Compile(pg);
                // Deleting any value or a result of an expression returns True.
                pg->LoadConst(expr, compiler::Constant::JS_TRUE);
            }
            break;
        }
        case lexer::TokenType::KEYW_VOID: {
            expr->Argument()->Compile(pg);
            pg->LoadConst(expr, compiler::Constant::JS_UNDEFINED);
            break;
        }
        default: {
            expr->Argument()->Compile(pg);

            compiler::RegScope rs(pg);
            compiler::VReg operandReg = pg->AllocReg();
            pg->StoreAccumulator(expr, operandReg);
            pg->Unary(expr, expr->OperatorType(), operandReg);
            break;
        }
    }
}

void JSCompiler::Compile(const ir::UpdateExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    compiler::RegScope rs(pg);
    compiler::VReg operandReg = pg->AllocReg();

    auto lref = compiler::JSLReference::Create(pg, expr->Argument(), false);
    lref.GetValue();

    pg->StoreAccumulator(expr, operandReg);
    pg->Unary(expr, expr->OperatorType(), operandReg);

    lref.SetValue();

    if (!expr->IsPrefix()) {
        pg->ToNumber(expr, operandReg);
    }
}

void JSCompiler::Compile(const ir::YieldExpression *expr) const
{
    PandaGen *pg = GetPandaGen();
    compiler::RegScope rs(pg);

    if (expr->Argument() != nullptr) {
        expr->Argument()->Compile(pg);
    } else {
        pg->LoadConst(expr, compiler::Constant::JS_UNDEFINED);
    }

    if (expr->HasDelegate()) {
        ASSERT(expr->Argument());
        pg->FuncBuilder()->YieldStar(expr);
    } else {
        pg->FuncBuilder()->Yield(expr);
    }
}

// Compile methods for LITERAL EXPRESSIONS in alphabetical order
void JSCompiler::Compile(const ir::BigIntLiteral *expr) const
{
    PandaGen *pg = GetPandaGen();
    pg->LoadAccumulatorBigInt(expr, expr->Str());
}

void JSCompiler::Compile(const ir::BooleanLiteral *expr) const
{
    PandaGen *pg = GetPandaGen();
    pg->LoadConst(expr, expr->Value() ? compiler::Constant::JS_TRUE : compiler::Constant::JS_FALSE);
}

void JSCompiler::Compile(const ir::NullLiteral *expr) const
{
    PandaGen *pg = GetPandaGen();
    pg->LoadConst(expr, compiler::Constant::JS_NULL);
}

void JSCompiler::Compile(const ir::NumberLiteral *expr) const
{
    PandaGen *pg = GetPandaGen();
    if (std::isnan(expr->Number().GetDouble())) {
        pg->LoadConst(expr, compiler::Constant::JS_NAN);
    } else if (!std::isfinite(expr->Number().GetDouble())) {
        pg->LoadConst(expr, compiler::Constant::JS_INFINITY);
    } else if (util::Helpers::IsInteger<int32_t>(expr->Number().GetDouble())) {
        pg->LoadAccumulatorInt(expr, static_cast<int32_t>(expr->Number().GetDouble()));
    } else {
        pg->LoadAccumulatorDouble(expr, expr->Number().GetDouble());
    }
}

void JSCompiler::Compile(const ir::RegExpLiteral *expr) const
{
    PandaGen *pg = GetPandaGen();
    pg->CreateRegExpWithLiteral(expr, expr->Pattern(), static_cast<uint8_t>(expr->Flags()));
}

void JSCompiler::Compile(const ir::StringLiteral *expr) const
{
    PandaGen *pg = GetPandaGen();
    pg->LoadAccumulatorString(expr, expr->Str());
}

// Compile methods for MODULE-related nodes in alphabetical order
void JSCompiler::Compile([[maybe_unused]] const ir::ExportAllDeclaration *st) const {}

void JSCompiler::Compile(const ir::ExportDefaultDeclaration *st) const
{
    PandaGen *pg = GetPandaGen();
    st->Decl()->Compile(pg);
    pg->StoreModuleVar(st, "default");
}

void JSCompiler::Compile(const ir::ExportNamedDeclaration *st) const
{
    PandaGen *pg = GetPandaGen();
    if (st->Decl() == nullptr) {
        return;
    }

    st->Decl()->Compile(pg);
}

void JSCompiler::Compile([[maybe_unused]] const ir::ImportDeclaration *st) const {}

void JSCompiler::Compile(const ir::BlockStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    compiler::LocalRegScope lrs(pg, st->Scope());

    for (const auto *it : st->Statements()) {
        it->Compile(pg);
    }
}

template <typename CodeGen>
static void CompileImpl(const ir::BreakStatement *self, [[maybe_unused]] CodeGen *cg)
{
    compiler::Label *target = cg->ControlFlowChangeBreak(self->Ident());
    cg->Branch(self, target);
}
void JSCompiler::Compile(const ir::BreakStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    CompileImpl(st, pg);
}

void JSCompiler::Compile(const ir::ClassDeclaration *st) const
{
    PandaGen *pg = GetPandaGen();
    auto lref = compiler::JSLReference::Create(pg, st->Definition()->Ident(), true);
    st->Definition()->Compile(pg);
    lref.SetValue();
}

static void CompileImpl(const ir::ContinueStatement *self, PandaGen *cg)
{
    compiler::Label *target = cg->ControlFlowChangeContinue(self->Ident());
    cg->Branch(self, target);
}

void JSCompiler::Compile(const ir::ContinueStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    CompileImpl(st, pg);
}

void JSCompiler::Compile([[maybe_unused]] const ir::DebuggerStatement *st) const {}

static void CompileImpl(const ir::DoWhileStatement *self, PandaGen *cg)
{
    auto *startLabel = cg->AllocLabel();
    compiler::LabelTarget labelTarget(cg);

    cg->SetLabel(self, startLabel);

    {
        compiler::LocalRegScope regScope(cg, self->Scope());
        compiler::LabelContext labelCtx(cg, labelTarget);
        self->Body()->Compile(cg);
    }

    cg->SetLabel(self, labelTarget.ContinueTarget());
    compiler::Condition::Compile(cg, self->Test(), labelTarget.BreakTarget());

    cg->Branch(self, startLabel);
    cg->SetLabel(self, labelTarget.BreakTarget());
}

void JSCompiler::Compile(const ir::DoWhileStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    CompileImpl(st, pg);
}

void JSCompiler::Compile([[maybe_unused]] const ir::EmptyStatement *st) const {}

void JSCompiler::Compile(const ir::ExpressionStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    st->GetExpression()->Compile(pg);
}

void JSCompiler::Compile(const ir::ForInStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    compiler::LabelTarget labelTarget(pg);

    compiler::RegScope rs(pg);
    compiler::VReg iter = pg->AllocReg();
    compiler::VReg propName = pg->AllocReg();

    // create enumerator
    st->Right()->Compile(pg);
    pg->GetPropIterator(st);
    pg->StoreAccumulator(st, iter);

    pg->SetLabel(st, labelTarget.ContinueTarget());

    // get next prop of enumerator
    pg->GetNextPropName(st, iter);
    pg->StoreAccumulator(st, propName);
    pg->BranchIfUndefined(st, labelTarget.BreakTarget());

    compiler::LocalRegScope declRegScope(pg, st->Scope()->DeclScope()->InitScope());
    auto lref = compiler::JSLReference::Create(pg, st->Left(), false);
    pg->LoadAccumulator(st, propName);
    lref.SetValue();

    compiler::LoopEnvScope declEnvScope(pg, st->Scope()->DeclScope());

    {
        compiler::LoopEnvScope envScope(pg, st->Scope(), labelTarget);
        st->Body()->Compile(pg);
    }

    pg->Branch(st, labelTarget.ContinueTarget());
    pg->SetLabel(st, labelTarget.BreakTarget());
}

void JSCompiler::Compile(const ir::ForOfStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    compiler::LocalRegScope declRegScope(pg, st->Scope()->DeclScope()->InitScope());

    st->Right()->Compile(pg);

    compiler::LabelTarget labelTarget(pg);
    auto iteratorType = st->IsAwait() ? compiler::IteratorType::ASYNC : compiler::IteratorType::SYNC;
    compiler::Iterator iterator(pg, st, iteratorType);

    pg->SetLabel(st, labelTarget.ContinueTarget());

    iterator.Next();
    iterator.Complete();
    pg->BranchIfTrue(st, labelTarget.BreakTarget());

    iterator.Value();
    pg->StoreAccumulator(st, iterator.NextResult());

    auto lref = compiler::JSLReference::Create(pg, st->Left(), false);

    {
        compiler::IteratorContext forOfCtx(pg, iterator, labelTarget);
        pg->LoadAccumulator(st, iterator.NextResult());
        lref.SetValue();

        compiler::LoopEnvScope declEnvScope(pg, st->Scope()->DeclScope());
        compiler::LoopEnvScope envScope(pg, st->Scope(), {});
        st->Body()->Compile(pg);
    }

    pg->Branch(st, labelTarget.ContinueTarget());
    pg->SetLabel(st, labelTarget.BreakTarget());
}

void JSCompiler::Compile(const ir::ForUpdateStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    compiler::LocalRegScope declRegScope(pg, st->Scope()->DeclScope()->InitScope());

    if (st->Init() != nullptr) {
        ASSERT(st->Init()->IsVariableDeclaration() || st->Init()->IsExpression());
        st->Init()->Compile(pg);
    }

    auto *startLabel = pg->AllocLabel();
    compiler::LabelTarget labelTarget(pg);

    compiler::LoopEnvScope declEnvScope(pg, st->Scope()->DeclScope());
    compiler::LoopEnvScope envScope(pg, labelTarget, st->Scope());
    pg->SetLabel(st, startLabel);

    {
        compiler::LocalRegScope regScope(pg, st->Scope());

        if (st->Test() != nullptr) {
            compiler::Condition::Compile(pg, st->Test(), labelTarget.BreakTarget());
        }

        st->Body()->Compile(pg);
        pg->SetLabel(st, labelTarget.ContinueTarget());
        envScope.CopyPetIterationCtx();
    }

    if (st->Update() != nullptr) {
        st->Update()->Compile(pg);
    }

    pg->Branch(st, startLabel);
    pg->SetLabel(st, labelTarget.BreakTarget());
}

void JSCompiler::Compile([[maybe_unused]] const ir::FunctionDeclaration *st) const {}
void JSCompiler::Compile([[maybe_unused]] const ir::AnnotationDeclaration *st) const {}
void JSCompiler::Compile([[maybe_unused]] const ir::AnnotationUsage *st) const {}

void JSCompiler::Compile(const ir::IfStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    auto *consequentEnd = pg->AllocLabel();
    compiler::Label *statementEnd = consequentEnd;

    compiler::Condition::Compile(pg, st->Test(), consequentEnd);
    st->Consequent()->Compile(pg);

    if (st->Alternate() != nullptr) {
        statementEnd = pg->AllocLabel();
        pg->Branch(pg->Insns().back()->Node(), statementEnd);

        pg->SetLabel(st, consequentEnd);
        st->Alternate()->Compile(pg);
    }

    pg->SetLabel(st, statementEnd);
}

void CompileImpl(const ir::LabelledStatement *self, PandaGen *cg)
{
    compiler::LabelContext labelCtx(cg, self);
    self->Body()->Compile(cg);
}

void JSCompiler::Compile(const ir::LabelledStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    CompileImpl(st, pg);
}

void JSCompiler::Compile(const ir::ReturnStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    if (st->Argument() != nullptr) {
        st->Argument()->Compile(pg);
    } else {
        pg->LoadConst(st, compiler::Constant::JS_UNDEFINED);
    }

    if (pg->CheckControlFlowChange()) {
        compiler::RegScope rs(pg);
        compiler::VReg res = pg->AllocReg();

        pg->StoreAccumulator(st, res);
        pg->ControlFlowChangeBreak();
        pg->LoadAccumulator(st, res);
    }

    if (st->Argument() != nullptr) {
        pg->ValidateClassDirectReturn(st);
        pg->DirectReturn(st);
    } else {
        pg->ImplicitReturn(st);
    }
}

static void CompileImpl(const ir::SwitchStatement *self, PandaGen *cg)
{
    compiler::LocalRegScope lrs(cg, self->Scope());
    compiler::SwitchBuilder builder(cg, self);
    compiler::VReg tag = cg->AllocReg();

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

void JSCompiler::Compile(const ir::SwitchStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    CompileImpl(st, pg);
}

void JSCompiler::Compile(const ir::ThrowStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    st->Argument()->Compile(pg);
    pg->EmitThrow(st);
}

static void CompileTryCatch(compiler::PandaGen *pg, const ir::TryStatement *st)
{
    ASSERT(st->CatchClauses().size() == 1);
    ASSERT(st->CatchClauses().front() && !st->FinallyBlock());

    compiler::TryContext tryCtx(pg, st);
    const auto &labelSet = tryCtx.LabelSet();

    pg->SetLabel(st, labelSet.TryBegin());
    st->Block()->Compile(pg);
    pg->SetLabel(st, labelSet.TryEnd());

    pg->Branch(st, labelSet.CatchEnd());

    pg->SetLabel(st, labelSet.CatchBegin());
    st->CatchClauses().front()->Compile(pg);
    pg->SetLabel(st, labelSet.CatchEnd());
}

static void CompileFinally(compiler::PandaGen *pg, compiler::TryContext *tryCtx, const compiler::TryLabelSet &labelSet,
                           const ir::TryStatement *st)
{
    compiler::RegScope rs(pg);
    compiler::VReg exception = pg->AllocReg();
    pg->StoreConst(st, exception, compiler::Constant::JS_HOLE);
    pg->Branch(st, labelSet.CatchEnd());

    pg->SetLabel(st, labelSet.CatchBegin());
    pg->StoreAccumulator(st, exception);

    pg->SetLabel(st, labelSet.CatchEnd());

    compiler::Label *label = pg->AllocLabel();
    pg->LoadAccumulator(st, tryCtx->FinalizerRun());

    pg->BranchIfNotUndefined(st, label);
    pg->StoreAccumulator(st, tryCtx->FinalizerRun());
    tryCtx->EmitFinalizer();
    pg->SetLabel(st, label);

    pg->LoadAccumulator(st, exception);
    pg->EmitRethrow(st);
}

static void CompileTryCatchFinally(compiler::PandaGen *pg, const ir::TryStatement *st)
{
    ASSERT(st->CatchClauses().size() == 1);
    ASSERT(st->CatchClauses().front() && st->FinallyBlock());

    compiler::TryContext tryCtx(pg, st);
    const auto &labelSet = tryCtx.LabelSet();

    pg->SetLabel(st, labelSet.TryBegin());
    {
        compiler::TryContext innerTryCtx(pg, st, false);
        const auto &innerLabelSet = innerTryCtx.LabelSet();

        pg->SetLabel(st, innerLabelSet.TryBegin());
        st->Block()->Compile(pg);
        pg->SetLabel(st, innerLabelSet.TryEnd());

        pg->Branch(st, innerLabelSet.CatchEnd());

        pg->SetLabel(st, innerLabelSet.CatchBegin());
        st->CatchClauses().front()->Compile(pg);
        pg->SetLabel(st, innerLabelSet.CatchEnd());
    }
    pg->SetLabel(st, labelSet.TryEnd());

    CompileFinally(pg, &tryCtx, labelSet, st);
}

static void CompileTryFinally(compiler::PandaGen *pg, const ir::TryStatement *st)
{
    ASSERT(st->CatchClauses().empty() && st->FinallyBlock());

    compiler::TryContext tryCtx(pg, st);
    const auto &labelSet = tryCtx.LabelSet();

    pg->SetLabel(st, labelSet.TryBegin());
    {
        compiler::TryContext innerTryCtx(pg, st, false);
        const auto &innerLabelSet = innerTryCtx.LabelSet();

        pg->SetLabel(st, innerLabelSet.TryBegin());
        st->Block()->Compile(pg);
        pg->SetLabel(st, innerLabelSet.TryEnd());

        pg->Branch(st, innerLabelSet.CatchEnd());

        pg->SetLabel(st, innerLabelSet.CatchBegin());
        pg->EmitThrow(st);
        pg->SetLabel(st, innerLabelSet.CatchEnd());
    }
    pg->SetLabel(st, labelSet.TryEnd());

    CompileFinally(pg, &tryCtx, labelSet, st);
}

void JSCompiler::Compile(const ir::TryStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    if (st->finalizer_ != nullptr) {
        if (!st->CatchClauses().empty()) {
            CompileTryCatchFinally(pg, st);
        } else {
            CompileTryFinally(pg, st);
        }
    } else {
        CompileTryCatch(pg, st);
    }
}

void JSCompiler::Compile(const ir::VariableDeclarator *st) const
{
    PandaGen *pg = GetPandaGen();
    auto lref = compiler::JSLReference::Create(pg, st->Id(), true);
    const ir::VariableDeclaration *decl = st->Parent()->AsVariableDeclaration();

    if (st->Init() != nullptr) {
        st->Init()->Compile(pg);
    } else {
        if (decl->Kind() == ir::VariableDeclaration::VariableDeclarationKind::VAR) {
            return;
        }
        if (decl->Kind() == ir::VariableDeclaration::VariableDeclarationKind::LET && !decl->Parent()->IsCatchClause()) {
            pg->LoadConst(st, compiler::Constant::JS_UNDEFINED);
        }
    }

    lref.SetValue();
}

void JSCompiler::Compile(const ir::VariableDeclaration *st) const
{
    PandaGen *pg = GetPandaGen();
    for (const auto *it : st->Declarators()) {
        it->Compile(pg);
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

void JSCompiler::Compile(const ir::WhileStatement *st) const
{
    PandaGen *pg = GetPandaGen();
    CompileImpl(st, pg);
}
}  // namespace ark::es2panda::compiler
