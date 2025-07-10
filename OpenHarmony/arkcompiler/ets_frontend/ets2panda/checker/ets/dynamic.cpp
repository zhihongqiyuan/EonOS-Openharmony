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

#include <utility>
#include "checker/ETSchecker.h"

#include "compiler/lowering/util.h"
#include "varbinder/declaration.h"
#include "varbinder/varbinder.h"
#include "varbinder/ETSBinder.h"
#include "checker/types/ets/etsDynamicFunctionType.h"
#include "checker/ets/dynamic/dynamicCall.h"
#include "compiler/lowering/scopesInit/scopesInitPhase.h"
#include "ir/base/classProperty.h"
#include "ir/base/classStaticBlock.h"
#include "ir/base/methodDefinition.h"
#include "ir/base/scriptFunction.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/functionExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/thisExpression.h"
#include "ir/expressions/memberExpression.h"
#include "ir/ets/etsPrimitiveType.h"
#include "ir/ts/tsAsExpression.h"
#include "ir/statements/blockStatement.h"
#include "ir/statements/classDeclaration.h"
#include "ir/statements/expressionStatement.h"
#include "ir/statements/returnStatement.h"
#include "parser/program/program.h"
#include "util/helpers.h"
#include "util/language.h"
#include "generated/signatures.h"
#include "ir/ets/etsParameterExpression.h"

namespace ark::es2panda::checker {

void ProcessCheckerNode(ETSChecker *checker, ir::AstNode *node)
{
    auto scope = compiler::NearestScope(node);
    if (scope->IsGlobalScope()) {
        // NOTE(aleksisch): All classes are contained in ETSGlobal class scope (not just Global scope),
        // however it's parent is ETSScript. It should be fixed
        scope = checker->VarBinder()->Program()->GlobalClassScope();
    }

    auto expressionCtx = varbinder::LexicalScope<varbinder::Scope>::Enter(checker->VarBinder(), scope);
    checker->VarBinder()->AsETSBinder()->ResolveReference(node);

    if (node->IsMethodDefinition()) {
        // NOTE(aleksisch): This should be done in varbinder,
        // however right now checker do it when called on ClassDefinition
        auto method = node->AsMethodDefinition();
        auto func = method->Value()->AsFunctionExpression()->Function();
        func->Id()->SetVariable(method->Id()->Variable());
    }
    ScopeContext checkerScope(checker, scope);
    node->Check(checker);
}

void ProcessScopesNode(ETSChecker *checker, ir::AstNode *node)
{
    auto *scope = compiler::NearestScope(node);
    if (scope->IsGlobalScope()) {
        // NOTE(aleksisch): All classes are contained in ETSGlobal scope,
        // however it's parent is ETSScript (not ETSGlobal). It should be fixed
        scope = checker->VarBinder()->Program()->GlobalClassScope();
    }
    auto expressionCtx = varbinder::LexicalScope<varbinder::Scope>::Enter(checker->VarBinder(), scope);
    compiler::InitScopesPhaseETS::RunExternalNode(node, checker->VarBinder());
}

ir::ETSParameterExpression *ETSChecker::AddParam(util::StringView name, ir::TypeNode *type)
{
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *paramIdent = AllocNode<ir::Identifier>(name, Allocator());
    if (type != nullptr) {
        paramIdent->SetTsTypeAnnotation(type);
        type->SetParent(paramIdent);
    }
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    return AllocNode<ir::ETSParameterExpression>(paramIdent, nullptr);
}

template <typename T>
ir::MethodDefinition *ETSChecker::CreateDynamicCallIntrinsic(ir::Expression *callee, const ArenaVector<T *> &arguments,
                                                             Language lang)
{
    ArenaVector<ir::Expression *> params(Allocator()->Adapter());

    auto dynamicTypeNode = AllocNode<ir::OpaqueTypeNode>(GlobalBuiltinDynamicType(lang));
    auto intTypeNode = AllocNode<ir::ETSPrimitiveType>(ir::PrimitiveType::INT);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *objParam = AddParam("obj", dynamicTypeNode);
    params.push_back(objParam);

    ir::ETSParameterExpression *param2;
    if (!DynamicCall::IsByValue(VarBinder()->AsETSBinder(), callee)) {
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        param2 = AddParam("qname_start", intTypeNode);
        params.push_back(param2);
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        param2 = AddParam("qname_len", intTypeNode->Clone(Allocator(), nullptr));
    } else {
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        param2 = AddParam("this", dynamicTypeNode->Clone(Allocator(), nullptr));
    }

    params.push_back(param2);

    for (size_t i = 0; i < arguments.size(); i++) {
        util::UString paramName("p" + std::to_string(i), Allocator());
        auto paramType = arguments[i]->TsType()->IsLambdaObject()
                             ? dynamicTypeNode->Clone(Allocator(), nullptr)
                             : AllocNode<ir::OpaqueTypeNode>(arguments[i]->TsType());
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        params.emplace_back(AddParam(paramName.View(), paramType));
    }

    auto funcSignature =
        ir::FunctionSignature(nullptr, std::move(params), dynamicTypeNode->Clone(Allocator(), nullptr));
    auto *func = AllocNode<ir::ScriptFunction>(
        Allocator(), ir::ScriptFunction::ScriptFunctionData {nullptr, std::move(funcSignature),
                                                             ir::ScriptFunctionFlags::METHOD, ir::ModifierFlags::NONE});

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *name = AllocNode<ir::Identifier>("invoke", Allocator());
    func->SetIdent(name);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *funcExpr = AllocNode<ir::FunctionExpression>(func);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *method = AllocNode<ir::MethodDefinition>(
        ir::MethodDefinitionKind::METHOD, func->Id()->Clone(Allocator(), nullptr), funcExpr,
        ir::ModifierFlags::PUBLIC | ir::ModifierFlags::NATIVE | ir::ModifierFlags::STATIC, Allocator(), false);
    return method;
}

static void ToString(ETSChecker *checker, const ArenaVector<ir::Expression *> &arguments, std::stringstream &ss)
{
    for (auto *arg : arguments) {
        auto *type = arg->Check(checker);
        ss << "-";
        type->ToString(ss);
    }
}

static void ToString([[maybe_unused]] ETSChecker *checker, const ArenaVector<varbinder::LocalVariable *> &arguments,
                     std::stringstream &ss)
{
    for (auto *arg : arguments) {
        auto *type = arg->TsType();
        ss << "-";
        type->ToString(ss);
    }
}

template <typename T>
Signature *ETSChecker::ResolveDynamicCallExpression(ir::Expression *callee, const ArenaVector<T *> &arguments,
                                                    Language lang, bool isConstruct)
{
    auto &dynamicIntrinsics = *DynamicCallIntrinsics(isConstruct);

    auto mapIt = dynamicIntrinsics.find(lang);
    if (mapIt == dynamicIntrinsics.cend()) {
        std::tie(mapIt, std::ignore) = dynamicIntrinsics.emplace(lang, Allocator()->Adapter());
    }

    auto &map = mapIt->second;

    std::stringstream ss;
    ss << "dyncall";
    if (DynamicCall::IsByValue(VarBinder()->AsETSBinder(), callee)) {
        ss << "-byvalue";
    } else {
        const auto callNames = DynamicCall::ResolveCall(VarBinder()->AsETSBinder(), callee);
        DynamicCallNames(isConstruct)->try_emplace(callNames.name, 0);
    }

    ToString(this, arguments, ss);

    auto key = ss.str();
    auto it = map.find(util::StringView(key));
    if (it == map.end()) {
        auto klass = GetDynamicClass(lang, isConstruct);
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        auto *method = CreateDynamicCallIntrinsic(callee, arguments, lang);
        auto props = ArenaVector<ir::AstNode *>(Allocator()->Adapter());
        props.emplace_back(method);
        klass->Definition()->AddProperties(std::move(props));

        {
            auto prevClass = VarBinder()->AsETSBinder()->GetGlobalRecordTable()->ClassDefinition();
            VarBinder()->AsETSBinder()->GetGlobalRecordTable()->SetClassDefinition(klass->Definition());
            ProcessScopesNode(this, method);
            ProcessCheckerNode(this, method);
            VarBinder()->AsETSBinder()->GetGlobalRecordTable()->SetClassDefinition(prevClass);
        }
        method->Function()->Signature()->SetReturnType(GlobalBuiltinDynamicType(lang));

        map.emplace(util::UString(key, Allocator()).View(), method->Function());
        return method->Function()->Signature();
    }

    return it->second->Signature();
}

template Signature *ETSChecker::ResolveDynamicCallExpression<ir::Expression>(
    ir::Expression *callee, const ArenaVector<ir::Expression *> &arguments, Language lang, bool isConstruct);

template Signature *ETSChecker::ResolveDynamicCallExpression<varbinder::LocalVariable>(
    ir::Expression *callee, const ArenaVector<varbinder::LocalVariable *> &arguments, Language lang, bool isConstruct);

std::pair<ir::ScriptFunction *, ir::Identifier *> ETSChecker::CreateStaticScriptFunction(
    ClassInitializerBuilder const &builder)
{
    ArenaVector<ir::Statement *> statements(Allocator()->Adapter());
    ArenaVector<ir::Expression *> params(Allocator()->Adapter());

    ir::ScriptFunction *func;
    ir::Identifier *id;

    builder(&statements, nullptr);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *body = AllocNode<ir::BlockStatement>(Allocator(), std::move(statements));
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    id = AllocNode<ir::Identifier>(compiler::Signatures::CCTOR, Allocator());
    auto signature = ir::FunctionSignature(nullptr, std::move(params), nullptr);
    // clang-format off
    func = AllocNode<ir::ScriptFunction>(
        Allocator(), ir::ScriptFunction::ScriptFunctionData {
                        body,
                        std::move(signature),
                        ir::ScriptFunctionFlags::STATIC_BLOCK | ir::ScriptFunctionFlags::EXPRESSION,
                        ir::ModifierFlags::STATIC,
                     });
    // clang-format on
    func->SetIdent(id);

    return std::make_pair(func, id);
}

std::pair<ir::ScriptFunction *, ir::Identifier *> ETSChecker::CreateScriptFunction(
    ClassInitializerBuilder const &builder)
{
    ArenaVector<ir::Statement *> statements(Allocator()->Adapter());
    ArenaVector<ir::Expression *> params(Allocator()->Adapter());

    ir::ScriptFunction *func;
    ir::Identifier *id;

    builder(&statements, &params);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *body = AllocNode<ir::BlockStatement>(Allocator(), std::move(statements));
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    id = AllocNode<ir::Identifier>(compiler::Signatures::CTOR, Allocator());
    auto funcSignature = ir::FunctionSignature(nullptr, std::move(params), nullptr);
    func = AllocNode<ir::ScriptFunction>(Allocator(),
                                         ir::ScriptFunction::ScriptFunctionData {
                                             body, std::move(funcSignature),
                                             ir::ScriptFunctionFlags::CONSTRUCTOR | ir::ScriptFunctionFlags::EXPRESSION,
                                             ir::ModifierFlags::PUBLIC});
    func->SetIdent(id);

    return std::make_pair(func, id);
}

ir::ClassStaticBlock *ETSChecker::CreateClassStaticInitializer(const ClassInitializerBuilder &builder,
                                                               [[maybe_unused]] ETSObjectType *type)
{
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto [func, id] = CreateStaticScriptFunction(builder);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *funcExpr = AllocNode<ir::FunctionExpression>(func);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *staticBlock = AllocNode<ir::ClassStaticBlock>(funcExpr, Allocator());
    staticBlock->AddModifier(ir::ModifierFlags::STATIC);

    return staticBlock;
}

ir::MethodDefinition *ETSChecker::CreateClassInstanceInitializer(const ClassInitializerBuilder &builder,
                                                                 [[maybe_unused]] ETSObjectType *type)
{
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto [func, id] = CreateScriptFunction(builder);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *funcExpr = AllocNode<ir::FunctionExpression>(func);

    auto *ctor =
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        AllocNode<ir::MethodDefinition>(ir::MethodDefinitionKind::CONSTRUCTOR, id->Clone(Allocator(), nullptr),
                                        funcExpr, ir::ModifierFlags::NONE, Allocator(), false);
    return ctor;
}

ir::ClassStaticBlock *ETSChecker::CreateDynamicCallClassInitializer(Language lang, bool isConstruct)
{
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    return CreateClassStaticInitializer([this, lang,
                                         isConstruct](ArenaVector<ir::Statement *> *statements,
                                                      [[maybe_unused]] ArenaVector<ir::Expression *> *params) {
        auto [builtin_class_name, builtin_method_name] =
            util::Helpers::SplitSignature(isConstruct ? compiler::Signatures::Dynamic::InitNewClassBuiltin(lang)
                                                      : compiler::Signatures::Dynamic::InitCallClassBuiltin(lang));
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        auto *classId = AllocNode<ir::Identifier>(builtin_class_name, Allocator());
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        auto *methodId = AllocNode<ir::Identifier>(builtin_method_name, Allocator());
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        auto *callee =
            AllocNode<ir::MemberExpression>(classId, methodId, ir::MemberExpressionKind::PROPERTY_ACCESS, false, false);

        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        auto *initCall = AllocNode<ir::CallExpression>(callee, ArenaVector<ir::Expression *>(Allocator()->Adapter()),
                                                       nullptr, false);

        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        statements->push_back(AllocNode<ir::ExpressionStatement>(initCall));
    });
}

ir::ClassDeclaration *ETSChecker::BuildClass(util::StringView name, const ClassBuilder &builder)
{
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *classId = AllocNode<ir::Identifier>(name, Allocator());

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *classDef = AllocNode<ir::ClassDefinition>(Allocator(), classId, ir::ClassDefinitionModifiers::CLASS_DECL,
                                                    ir::ModifierFlags::NONE, Language(Language::Id::ETS));

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *classDecl = AllocNode<ir::ClassDeclaration>(classDef, Allocator());

    VarBinder()->Program()->Ast()->Statements().push_back(classDecl);
    classDecl->SetParent(VarBinder()->Program()->Ast());

    varbinder::BoundContext boundCtx(VarBinder()->AsETSBinder()->GetGlobalRecordTable(), classDef);

    ArenaVector<ir::AstNode *> classBody(Allocator()->Adapter());

    builder(&classBody);

    classDef->AddProperties(std::move(classBody));

    ProcessScopesNode(this, classDecl);
    ProcessCheckerNode(this, classDecl);
    return classDecl;
}

ir::ClassProperty *ETSChecker::CreateStaticReadonlyField(const char *name)
{
    auto *fieldIdent = AllocNode<ir::Identifier>(name, Allocator());
    auto flags = ir::ModifierFlags::STATIC | ir::ModifierFlags::PRIVATE | ir::ModifierFlags::READONLY;
    auto *field = AllocNode<ir::ClassProperty>(
        fieldIdent, nullptr, AllocNode<ir::ETSPrimitiveType>(ir::PrimitiveType::INT), flags, Allocator(), false);

    return field;
}

ir::ClassDeclaration *ETSChecker::GetDynamicClass(Language lang, bool isConstruct)
{
    auto &klasses = dynamicClasses_[static_cast<size_t>(isConstruct)];
    if (klasses.count(lang) != 0U) {
        return klasses[lang];
    }
    auto className =
        isConstruct ? compiler::Signatures::Dynamic::NewClass(lang) : compiler::Signatures::Dynamic::CallClass(lang);
    auto klass = BuildClass(className, [this, lang, isConstruct](ArenaVector<ir::AstNode *> *classBody) {
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        classBody->push_back(CreateStaticReadonlyField("qname_start_from"));
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        classBody->push_back(CreateDynamicCallClassInitializer(lang, isConstruct));
    });
    klasses.emplace(lang, klass);
    return klass;
}

void ETSChecker::ClassInitializerFromImport(ir::ETSImportDeclaration *import, ArenaVector<ir::Statement *> *statements)
{
    auto builtin = compiler::Signatures::Dynamic::LoadModuleBuiltin(import->Language());
    auto [builtin_class_name, builtin_method_name] = util::Helpers::SplitSignature(builtin);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *classId = AllocNode<ir::Identifier>(builtin_class_name, Allocator());
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *methodId = AllocNode<ir::Identifier>(builtin_method_name, Allocator());
    auto *callee =
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        AllocNode<ir::MemberExpression>(classId, methodId, ir::MemberExpressionKind::PROPERTY_ACCESS, false, false);

    // Note(rsipka): this check could be avoided with appropriate language extensions
    ArenaVector<ir::Expression *> callParams(Allocator()->Adapter());
    if (ark::os::file::File::IsRegularFile(import->ResolvedSource()->Str().Mutf8())) {
        callParams.push_back(AllocNode<ir::StringLiteral>(
            util::UString(ark::os::RemoveExtension(import->ResolvedSource()->Str().Mutf8()), Allocator()).View()));
    } else {
        callParams.push_back(import->ResolvedSource());
    }
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *loadCall = AllocNode<ir::CallExpression>(callee, std::move(callParams), nullptr, false);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *moduleClassId = AllocNode<ir::Identifier>(compiler::Signatures::DYNAMIC_MODULE_CLASS, Allocator());
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *fieldId = AllocNode<ir::Identifier>(import->AssemblerName(), Allocator());
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *property = AllocNode<ir::MemberExpression>(moduleClassId, fieldId, ir::MemberExpressionKind::PROPERTY_ACCESS,
                                                     false, false);

    auto *initializer =
        AllocNode<ir::AssignmentExpression>(property, loadCall, lexer::TokenType::PUNCTUATOR_SUBSTITUTION);
    statements->push_back(AllocNode<ir::ExpressionStatement>(initializer));
}

ir::ClassStaticBlock *ETSChecker::CreateDynamicModuleClassInitializer(
    const std::vector<ir::ETSImportDeclaration *> &imports)
{
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    return CreateClassStaticInitializer([this, imports](ArenaVector<ir::Statement *> *statements,
                                                        [[maybe_unused]] ArenaVector<ir::Expression *> *params) {
        for (auto *import : imports) {
            ClassInitializerFromImport(import, statements);
        }
    });
}

ir::MethodDefinition *ETSChecker::CreateClassMethod(const std::string_view name, ir::ScriptFunctionFlags funcFlags,
                                                    ir::ModifierFlags modifierFlags, const MethodBuilder &builder)
{
    ArenaVector<ir::Expression *> params(Allocator()->Adapter());
    auto *id = AllocNode<ir::Identifier>(name, Allocator());

    ArenaVector<ir::Statement *> statements(Allocator()->Adapter());
    Type *returnType = nullptr;

    builder(&statements, &params, &returnType);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *body = AllocNode<ir::BlockStatement>(Allocator(), std::move(statements));
    auto funcSignature = ir::FunctionSignature(
        nullptr, std::move(params), returnType == nullptr ? nullptr : AllocNode<ir::OpaqueTypeNode>(returnType));
    auto *func = AllocNode<ir::ScriptFunction>(
        Allocator(), ir::ScriptFunction::ScriptFunctionData {body, std::move(funcSignature), funcFlags, modifierFlags});

    func->SetIdent(id);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *funcExpr = AllocNode<ir::FunctionExpression>(func);
    auto *method =
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHt resetint)
        AllocNode<ir::MethodDefinition>(ir::MethodDefinitionKind::METHOD, func->Id()->Clone(Allocator(), nullptr),
                                        funcExpr, modifierFlags, Allocator(), false);

    return method;
}

ir::MethodDefinition *ETSChecker::CreateDynamicModuleClassInitMethod()
{
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    return CreateClassMethod(compiler::Signatures::DYNAMIC_MODULE_CLASS_INIT, ir::ScriptFunctionFlags::METHOD,
                             ir::ModifierFlags::PUBLIC | ir::ModifierFlags::STATIC,
                             [this]([[maybe_unused]] ArenaVector<ir::Statement *> *statements,
                                    [[maybe_unused]] ArenaVector<ir::Expression *> *params,
                                    Type **returnType) { *returnType = GlobalVoidType(); });
}

ir::MethodDefinition *ETSChecker::CreateLambdaObjectClassInvokeMethod(Signature *invokeSignature,
                                                                      ir::TypeNode *retTypeAnnotation)
{
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    return CreateClassMethod(
        compiler::Signatures::LAMBDA_OBJECT_INVOKE, ir::ScriptFunctionFlags::METHOD, ir::ModifierFlags::PUBLIC,
        [this, invokeSignature, retTypeAnnotation](ArenaVector<ir::Statement *> *statements,
                                                   ArenaVector<ir::Expression *> *params, Type **returnType) {
            util::UString thisParamName(std::string("this"), Allocator());
            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            ir::ETSParameterExpression *thisParam = AddParam(thisParamName.View(), nullptr);
            params->push_back(thisParam);

            ArenaVector<ir::Expression *> callParams(Allocator()->Adapter());
            for (auto *invokeParam : invokeSignature->Params()) {
                // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
                auto paramName =
                    util::UString(std::string("p") + std::to_string(callParams.size()), Allocator()).View();
                auto *param = AddParam(paramName, AllocNode<ir::OpaqueTypeNode>(invokeParam->TsType()));
                params->push_back(param);
                callParams.push_back(param->Clone(Allocator(), nullptr));
            }

            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            auto *properyId = AllocNode<ir::Identifier>("jsvalue_lambda", Allocator());
            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            auto *callee = AllocNode<ir::MemberExpression>(thisParam->Clone(Allocator(), nullptr), properyId,
                                                           ir::MemberExpressionKind::PROPERTY_ACCESS, false, false);
            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            auto *callLambda = AllocNode<ir::CallExpression>(callee, std::move(callParams), nullptr, false);

            auto *castToRetTypeExpr =
                // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
                AllocNode<ir::TSAsExpression>(callLambda, retTypeAnnotation->Clone(Allocator(), nullptr), false);
            castToRetTypeExpr->SetTsType(invokeSignature->ReturnType());
            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            auto *retStatement = AllocNode<ir::ReturnStatement>(castToRetTypeExpr);
            statements->push_back(retStatement);

            *returnType = invokeSignature->ReturnType();
        });
}

void ETSChecker::EmitDynamicModuleClassInitCall()
{
    auto *globalClass = VarBinder()->Program()->GlobalClass();
    auto &body = globalClass->Body();
    auto it = std::find_if(body.begin(), body.end(), [](ir::AstNode *node) { return node->IsClassStaticBlock(); });

    ASSERT(it != body.end());

    auto *staticBlock = (*it)->AsClassStaticBlock();
    auto *cctorBody = staticBlock->Function()->Body()->AsBlockStatement();

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *classId = AllocNode<ir::Identifier>(compiler::Signatures::DYNAMIC_MODULE_CLASS, Allocator());
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *methodId = AllocNode<ir::Identifier>(compiler::Signatures::DYNAMIC_MODULE_CLASS_INIT, Allocator());
    auto *callee =
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        AllocNode<ir::MemberExpression>(classId, methodId, ir::MemberExpressionKind::PROPERTY_ACCESS, false, false);

    ArenaVector<ir::Expression *> callParams(Allocator()->Adapter());
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *initCall = AllocNode<ir::CallExpression>(callee, std::move(callParams), nullptr, false);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *const node = AllocNode<ir::ExpressionStatement>(initCall);
    node->SetParent(cctorBody);
    cctorBody->Statements().push_back(node);

    ProcessScopesNode(this, node);
    ProcessCheckerNode(this, node);
}

void ETSChecker::BuildDynamicImportClass()
{
    auto dynamicImports = VarBinder()->AsETSBinder()->DynamicImports();
    if (dynamicImports.empty()) {
        return;
    }

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    BuildClass(
        compiler::Signatures::DYNAMIC_MODULE_CLASS, [this, dynamicImports](ArenaVector<ir::AstNode *> *classBody) {
            std::unordered_set<util::StringView> fields;
            std::vector<ir::ETSImportDeclaration *> imports;

            for (auto *import : dynamicImports) {
                auto source = import->Source()->Str();
                if (fields.find(source) != fields.cend()) {
                    continue;
                }

                auto assemblyName = std::string(source);
                std::replace_if(
                    assemblyName.begin(), assemblyName.end(), [](char c) { return std::isalnum(c) == 0; }, '_');
                assemblyName.append(std::to_string(fields.size()));

                import->AssemblerName() = util::UString(assemblyName, Allocator()).View();
                fields.insert(import->AssemblerName());
                imports.push_back(import);

                auto *fieldIdent = AllocNode<ir::Identifier>(import->AssemblerName(), Allocator());
                auto flags = ir::ModifierFlags::STATIC | ir::ModifierFlags::PUBLIC | ir::ModifierFlags::READONLY;
                auto *field = AllocNode<ir::ClassProperty>(
                    fieldIdent, nullptr, AllocNode<ir::OpaqueTypeNode>(GlobalBuiltinDynamicType(import->Language())),
                    flags, Allocator(), false);

                classBody->push_back(field);
            }

            classBody->push_back(CreateDynamicModuleClassInitializer(imports));
            classBody->push_back(CreateDynamicModuleClassInitMethod());
        });
    EmitDynamicModuleClassInitCall();
}

ir::MethodDefinition *ETSChecker::CreateLambdaObjectClassInitializer(ETSObjectType *functionalInterface)
{
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    return CreateClassInstanceInitializer(
        [this](ArenaVector<ir::Statement *> *statements, ArenaVector<ir::Expression *> *params) {
            ir::ETSParameterExpression *thisParam = AddParam(varbinder::VarBinder::MANDATORY_PARAM_THIS, nullptr);
            params->push_back(thisParam);

            util::UString jsvalueParamName(std::string("jsvalue_param"), Allocator());
            ir::ETSParameterExpression *jsvalueParam =
                AddParam(jsvalueParamName.View(), AllocNode<ir::OpaqueTypeNode>(GlobalBuiltinJSValueType()));
            params->push_back(jsvalueParam);
            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            auto *moduleClassId = AllocNode<ir::Identifier>(varbinder::VarBinder::MANDATORY_PARAM_THIS, Allocator());
            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            auto *fieldId = AllocNode<ir::Identifier>("jsvalue_lambda", Allocator());
            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            auto *property = AllocNode<ir::MemberExpression>(moduleClassId, fieldId,
                                                             ir::MemberExpressionKind::PROPERTY_ACCESS, false, false);
            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            auto *initializer = AllocNode<ir::AssignmentExpression>(property, jsvalueParam->Clone(Allocator(), nullptr),
                                                                    lexer::TokenType::PUNCTUATOR_SUBSTITUTION);
            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            statements->push_back(AllocNode<ir::ExpressionStatement>(initializer));
        },
        functionalInterface);
}

void ETSChecker::BuildLambdaObjectClass(ETSObjectType *functionalInterface, ir::TypeNode *retTypeAnnotation)
{
    auto *invokeMethod = functionalInterface->GetOwnProperty<checker::PropertyType::INSTANCE_METHOD>("invoke");
    auto *invokeSignature = invokeMethod->TsType()->AsETSFunctionType()->CallSignatures()[0];

    std::stringstream ss;
    ss << compiler::Signatures::LAMBDA_OBJECT;
    ToString(this, invokeSignature->Params(), ss);
    auto syntheticLambdaObjName = ss.str();
    if (dynamicLambdaSignatureCache_.count(syntheticLambdaObjName) != 0) {
        functionalInterface->AddConstructSignature(dynamicLambdaSignatureCache_[syntheticLambdaObjName]);
        return;
    }

    auto buildBody = [this, invokeSignature, retTypeAnnotation,
                      functionalInterface](ArenaVector<ir::AstNode *> *classBody) {
        auto assemblyName = "jsvalue_lambda";
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        auto *fieldIdent = AllocNode<ir::Identifier>(assemblyName, Allocator());
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        auto *field =
            AllocNode<ir::ClassProperty>(fieldIdent, nullptr, AllocNode<ir::OpaqueTypeNode>(GlobalBuiltinJSValueType()),
                                         ir::ModifierFlags::PRIVATE, Allocator(), false);
        classBody->push_back(field);
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        classBody->push_back(CreateLambdaObjectClassInitializer(functionalInterface));
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        classBody->push_back(CreateLambdaObjectClassInvokeMethod(invokeSignature, retTypeAnnotation));
    };

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    BuildClass(util::StringView(syntheticLambdaObjName), buildBody);

    dynamicLambdaSignatureCache_[syntheticLambdaObjName] = functionalInterface->ConstructSignatures()[0];
}

}  // namespace ark::es2panda::checker
