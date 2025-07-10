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

#include "ETSemitter.h"

#include "compiler/core/ETSGen.h"
#include "varbinder/varbinder.h"
#include "varbinder/ETSBinder.h"
#include "ir/astNode.h"
#include "ir/expressions/identifier.h"
#include "ir/base/decorator.h"
#include "ir/base/methodDefinition.h"
#include "ir/base/classDefinition.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/classProperty.h"
#include "ir/ts/tsEnumDeclaration.h"
#include "ir/ts/tsEnumMember.h"
#include "ir/ts/tsInterfaceDeclaration.h"
#include "ir/ts/tsInterfaceBody.h"
#include "ir/ts/tsTypeParameterDeclaration.h"
#include "ir/ts/tsTypeParameter.h"
#include "ir/typeNode.h"
#include "parser/program/program.h"
#include "checker/checker.h"
#include "checker/types/signature.h"
#include "checker/ETSchecker.h"
#include "checker/types/type.h"
#include "checker/types/ets/types.h"
#include "checker/types/ets/etsPartialTypeParameter.h"
#include "public/public.h"

#include "assembly-program.h"

namespace {
uint32_t g_litArrayValueCount = 0;
}  // namespace

namespace ark::es2panda::compiler {

#ifdef PANDA_WITH_ETS
static constexpr auto EXTENSION = panda_file::SourceLang::ETS;
#else
// NOTE: temporary dummy gn buildfix until ETS plugin has gn build support
static constexpr auto EXTENSION = panda_file::SourceLang::PANDA_ASSEMBLY;
#endif

static uint32_t TranslateModifierFlags(ir::ModifierFlags modifierFlags)
{
    uint32_t accessFlags = 0;

    if ((modifierFlags & ir::ModifierFlags::PRIVATE) != 0) {
        accessFlags = ACC_PRIVATE;
    } else if ((modifierFlags & ir::ModifierFlags::INTERNAL) != 0) {
        if ((modifierFlags & ir::ModifierFlags::PROTECTED) != 0) {
            accessFlags = ACC_PROTECTED;
        }
        // NOTE: torokg. Add ACC_INTERNAL access flag to libpandabase
    } else if ((modifierFlags & ir::ModifierFlags::PROTECTED) != 0) {
        accessFlags = ACC_PROTECTED;
    } else {
        accessFlags = ACC_PUBLIC;
    }

    if ((modifierFlags & ir::ModifierFlags::STATIC) != 0) {
        accessFlags |= ACC_STATIC;
    }
    if ((modifierFlags & ir::ModifierFlags::FINAL) != 0) {
        accessFlags |= ACC_FINAL;
    }
    // NOTE: should be ModifierFlags::READONLY
    if ((modifierFlags & ir::ModifierFlags::READONLY) != 0) {
        accessFlags |= ACC_READONLY;
    }
    if ((modifierFlags & ir::ModifierFlags::ABSTRACT) != 0) {
        accessFlags |= ACC_ABSTRACT;
    }
    if ((modifierFlags & ir::ModifierFlags::NATIVE) != 0) {
        accessFlags |= ACC_NATIVE;
    }

    return accessFlags;
}

static pandasm::Type PandasmTypeWithRank(checker::Type const *type, uint32_t rank = 0)
{
    if (type->IsETSTypeParameter()) {
        return PandasmTypeWithRank(type->AsETSTypeParameter()->GetConstraintType());
    }
    if (type->IsETSNonNullishType()) {
        return PandasmTypeWithRank(type->AsETSNonNullishType()->GetUnderlying());
    }
    if (type->IsETSPartialTypeParameter()) {
        return PandasmTypeWithRank(type->AsETSPartialTypeParameter()->GetUnderlying());
    }
    if (type->IsETSUnionType()) {
        return PandasmTypeWithRank(type->AsETSUnionType()->GetAssemblerLUB());
    }

    std::stringstream ss;
    type->ToAssemblerType(ss);
    return pandasm::Type(ss.str(), rank == 0 ? type->Rank() : rank);
}

static pandasm::Function GenScriptFunction(const ir::ScriptFunction *scriptFunc, ETSEmitter *emitter)
{
    auto *funcScope = scriptFunc->Scope();
    auto *paramScope = funcScope->ParamScope();

    auto func = pandasm::Function(funcScope->InternalName().Mutf8(), EXTENSION);
    func.params.reserve(paramScope->Params().size());

    for (const auto *var : paramScope->Params()) {
        func.params.emplace_back(PandasmTypeWithRank(var->TsType()), EXTENSION);
    }

    if (scriptFunc->IsConstructor() || scriptFunc->IsStaticBlock()) {
        func.returnType = pandasm::Type(Signatures::PRIMITIVE_VOID, 0);
    } else {
        func.returnType = PandasmTypeWithRank(scriptFunc->Signature()->ReturnType());
    }

    uint32_t accessFlags = 0;
    if (!scriptFunc->IsStaticBlock()) {
        const auto *methodDef = util::Helpers::GetContainingClassMethodDefinition(scriptFunc);
        accessFlags |= TranslateModifierFlags(methodDef->Modifiers());
    }
    if (scriptFunc->HasRestParameter()) {
        accessFlags |= ACC_VARARGS;
    }
    func.metadata->SetAccessFlags(accessFlags);

    if (!scriptFunc->Annotations().empty()) {
        auto baseName = scriptFunc->Id()->Name().Mutf8();
        func.metadata->SetAnnotations(emitter->GenCustomAnnotations(scriptFunc->Annotations(), baseName));
    }
    return func;
}

pandasm::Function *ETSFunctionEmitter::GenFunctionSignature()
{
    auto *scriptFunc = Cg()->RootNode()->AsScriptFunction();
    auto *emitter = static_cast<ETSEmitter *>(Cg()->Context()->emitter);
    auto func = GenScriptFunction(scriptFunc, emitter);

    if (Cg()->RootNode()->AsScriptFunction()->IsExternal()) {
        func.metadata->SetAttribute(Signatures::EXTERNAL);
    }

    auto *funcElement = new pandasm::Function(func.name, func.language);
    *funcElement = std::move(func);
    GetProgramElement()->SetFunction(funcElement);
    funcElement->regsNum = VReg::REG_START - Cg()->TotalRegsNum();

    return funcElement;
}

void ETSFunctionEmitter::GenVariableSignature(pandasm::debuginfo::LocalVariable &variableDebug,
                                              [[maybe_unused]] varbinder::LocalVariable *variable) const
{
    variableDebug.signature = Signatures::ANY;
    variableDebug.signatureType = Signatures::ANY;
}

void ETSFunctionEmitter::GenFunctionAnnotations([[maybe_unused]] pandasm::Function *func) {}

static pandasm::Function GenExternalFunction(checker::Signature *signature, bool isCtor)
{
    auto func = pandasm::Function(signature->InternalName().Mutf8(), EXTENSION);

    for (auto param : signature->Params()) {
        func.params.emplace_back(PandasmTypeWithRank(param->TsType()), EXTENSION);
    }
    func.returnType = PandasmTypeWithRank(signature->ReturnType());

    if (isCtor) {
        func.metadata->SetAttribute(Signatures::CONSTRUCTOR);
    }
    func.metadata->SetAttribute(Signatures::EXTERNAL);

    return func;
}

static std::string GenerateMangledName(const std::string &baseName, const std::string &propName)
{
    return baseName + "$" + propName;
}

void ETSEmitter::GenAnnotation()
{
    Program()->lang = EXTENSION;
    const auto *varbinder = static_cast<varbinder::ETSBinder *>(Context()->parserProgram->VarBinder());

    auto *globalRecordTable = varbinder->GetGlobalRecordTable();
    auto baseName = varbinder->GetRecordTable()->RecordName().Mutf8();
    for (auto *annoDecl : globalRecordTable->AnnotationDeclarations()) {
        auto newBaseName = GenerateMangledName(baseName, annoDecl->GetBaseName()->Name().Mutf8());
        GenCustomAnnotationRecord(annoDecl, newBaseName, annoDecl->IsDeclare());
    }

    for (auto *classDecl : globalRecordTable->ClassDefinitions()) {
        GenClassRecord(classDecl, classDecl->IsDeclare());
    }

    for (auto *interfaceDecl : globalRecordTable->InterfaceDeclarations()) {
        GenInterfaceRecord(interfaceDecl, interfaceDecl->IsDeclare());
    }

    for (auto *signature : globalRecordTable->Signatures()) {
        auto *scriptFunc = signature->Node()->AsScriptFunction();
        auto func = GenScriptFunction(scriptFunc, this);
        if (scriptFunc->IsDeclare()) {
            func.metadata->SetAttribute(Signatures::EXTERNAL);
        }
        if (scriptFunc->IsAsyncFunc()) {
            std::vector<pandasm::AnnotationData> annotations;
            annotations.push_back(GenAnnotationAsync(scriptFunc));
            func.metadata->SetAnnotations(std::move(annotations));
        }
        Program()->functionTable.emplace(func.name, std::move(func));
    }

    for (auto [extProg, recordTable] : varbinder->GetExternalRecordTable()) {
        (void)extProg;
        if (recordTable == varbinder->GetRecordTable()) {
            continue;
        }
        GenExternalRecord(recordTable);
    }

    const auto *checker = static_cast<checker::ETSChecker *>(Context()->checker);

    for (auto [arrType, signature] : checker->GlobalArrayTypes()) {
        GenGlobalArrayRecord(arrType, signature);
    }
}

void ETSEmitter::GenExternalRecord(varbinder::RecordTable *recordTable)
{
    bool isGenStdLib = recordTable->Program()->VarBinder()->IsGenStdLib();
    const auto *varbinder = static_cast<const varbinder::ETSBinder *>(Context()->parserProgram->VarBinder());
    auto baseName = varbinder->GetRecordTable()->RecordName().Mutf8();
    for (auto *annoDecl : recordTable->AnnotationDeclarations()) {
        auto newBaseName = GenerateMangledName(baseName, annoDecl->GetBaseName()->Name().Mutf8());
        GenCustomAnnotationRecord(annoDecl, newBaseName, !isGenStdLib);
    }

    for (auto *classDecl : recordTable->ClassDefinitions()) {
        GenClassRecord(classDecl, !isGenStdLib);
    }

    for (auto *interfaceDecl : recordTable->InterfaceDeclarations()) {
        GenInterfaceRecord(interfaceDecl, !isGenStdLib);
    }

    for (auto *signature : recordTable->Signatures()) {
        auto func = GenScriptFunction(signature->Node()->AsScriptFunction(), this);

        if (!isGenStdLib) {
            func.metadata->SetAttribute(Signatures::EXTERNAL);
        }

        Program()->functionTable.emplace(func.name, std::move(func));
    }
}

// Helper function to reduce EmitDefaultFieldValue size and pass code check
static pandasm::ScalarValue CreateScalarValue(const checker::Type *type, checker::TypeFlag typeKind)
{
    switch (typeKind) {
        case checker::TypeFlag::ETS_BOOLEAN: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::U1>(
                static_cast<uint8_t>(type->AsETSBooleanType()->GetValue()));
        }
        case checker::TypeFlag::BYTE: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::I8>(type->AsByteType()->GetValue());
        }
        case checker::TypeFlag::SHORT: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::I16>(type->AsShortType()->GetValue());
        }
        case checker::TypeFlag::INT: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::I32>(type->AsIntType()->GetValue());
        }
        case checker::TypeFlag::LONG: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::I64>(type->AsLongType()->GetValue());
        }
        case checker::TypeFlag::FLOAT: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::F32>(type->AsFloatType()->GetValue());
        }
        case checker::TypeFlag::DOUBLE: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::F64>(type->AsDoubleType()->GetValue());
        }
        case checker::TypeFlag::CHAR: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::U16>(type->AsCharType()->GetValue());
        }
        case checker::TypeFlag::ETS_OBJECT: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(
                type->AsETSObjectType()->AsETSStringType()->GetValue().Mutf8());
        }
        default: {
            UNREACHABLE();
        }
    }
}

void ETSEmitter::EmitDefaultFieldValue(pandasm::Field &classField, const ir::Expression *init)
{
    if (init == nullptr) {
        return;
    }

    const auto *type = init->TsType();

    if (!type->HasTypeFlag(checker::TypeFlag::CONSTANT)) {
        return;
    }

    auto typeKind = checker::ETSChecker::TypeKind(type);
    classField.metadata->SetFieldType(classField.type);
    classField.metadata->SetValue(CreateScalarValue(type, typeKind));
}

void ETSEmitter::GenInterfaceMethodDefinition(const ir::MethodDefinition *methodDef, bool external)
{
    auto *scriptFunc = methodDef->Function();
    auto func = GenScriptFunction(scriptFunc, this);

    if (external) {
        func.metadata->SetAttribute(Signatures::EXTERNAL);
    }

    if (scriptFunc->Body() != nullptr) {
        return;
    }

    func.metadata->SetAccessFlags(func.metadata->GetAccessFlags() | ACC_ABSTRACT);
    Program()->functionTable.emplace(func.name, std::move(func));
}

void ETSEmitter::GenClassField(const ir::ClassProperty *prop, pandasm::Record &classRecord, bool external)
{
    auto field = pandasm::Field(Program()->lang);
    field.name = prop->Id()->Name().Mutf8();
    field.type = PandasmTypeWithRank(prop->TsType());
    field.metadata->SetAccessFlags(TranslateModifierFlags(prop->Modifiers()));

    if (external || prop->IsDeclare()) {
        field.metadata->SetAttribute(Signatures::EXTERNAL);
    } else if (prop->TsType()->IsETSPrimitiveType() || prop->TsType()->IsETSStringType()) {
        EmitDefaultFieldValue(field, prop->Value());
    } else if (prop->TsType()->IsETSEnumType()) {
        CreateEnumProp(prop, field);
    }

    classRecord.fieldList.emplace_back(std::move(field));
}

void ETSEmitter::GenClassInheritedFields(const checker::ETSObjectType *baseType, pandasm::Record &classRecord)
{
    std::vector<const varbinder::LocalVariable *> foreignProps = baseType->ForeignProperties();

    for (const auto *foreignProp : foreignProps) {
        auto *declNode = foreignProp->Declaration()->Node();
        if (!declNode->IsClassProperty()) {
            continue;
        }

        GenClassField(declNode->AsClassProperty(), classRecord, true);
    }
}

void ETSEmitter::GenGlobalArrayRecord(checker::ETSArrayType *arrayType, checker::Signature *signature)
{
    std::stringstream ss;
    arrayType->ToAssemblerTypeWithRank(ss);

    auto arrayRecord = pandasm::Record(ss.str(), Program()->lang);

    auto func = GenExternalFunction(signature, true);
    func.params.emplace(func.params.begin(), pandasm::Type(ss.str(), 0), EXTENSION);

    Program()->functionTable.emplace(func.name, std::move(func));

    arrayRecord.metadata->SetAttribute(Signatures::EXTERNAL);
    Program()->recordTable.emplace(arrayRecord.name, std::move(arrayRecord));
    Program()->arrayTypes.emplace(PandasmTypeWithRank(arrayType));
}

void ETSEmitter::GenInterfaceRecord(const ir::TSInterfaceDeclaration *interfaceDecl, bool external)
{
    auto *baseType = interfaceDecl->TsType()->AsETSObjectType();

    auto interfaceRecord = pandasm::Record(interfaceDecl->InternalName().Mutf8(), Program()->lang);
    if (external) {
        interfaceRecord.metadata->SetAttribute(Signatures::EXTERNAL);
    }

    uint32_t accessFlags = ACC_PUBLIC | ACC_ABSTRACT | ACC_INTERFACE;

    if (interfaceDecl->IsStatic()) {
        accessFlags |= ACC_STATIC;
    }

    interfaceRecord.metadata->SetAccessFlags(accessFlags);
    interfaceRecord.sourceFile =
        Context()->parserProgram->VarBinder()->Program()->SourceFile().GetAbsolutePath().Mutf8();
    interfaceRecord.metadata->SetAttributeValue(Signatures::EXTENDS_ATTRIBUTE, Signatures::BUILTIN_OBJECT);

    for (auto *it : baseType->Interfaces()) {
        auto *declNode = it->GetDeclNode();
        ASSERT(declNode->IsTSInterfaceDeclaration());
        std::string name = declNode->AsTSInterfaceDeclaration()->InternalName().Mutf8();
        interfaceRecord.metadata->SetAttributeValue(Signatures::IMPLEMENTS_ATTRIBUTE, name);
    }

    GenClassInheritedFields(baseType, interfaceRecord);

    for (const auto *prop : interfaceDecl->Body()->Body()) {
        if (prop->IsClassProperty()) {
            GenClassField(prop->AsClassProperty(), interfaceRecord, external);
        } else if (prop->IsMethodDefinition()) {
            GenInterfaceMethodDefinition(prop->AsMethodDefinition(), external);
        }
    }

    Program()->recordTable.emplace(interfaceRecord.name, std::move(interfaceRecord));
}

std::vector<pandasm::AnnotationData> ETSEmitter::GenAnnotations(const ir::ClassDefinition *classDef)
{
    std::vector<pandasm::AnnotationData> annotations;
    const ir::AstNode *parent = classDef->Parent();
    while (parent != nullptr) {
        if (parent->IsMethodDefinition()) {
            annotations.emplace_back(GenAnnotationEnclosingMethod(parent->AsMethodDefinition()));
            annotations.emplace_back(GenAnnotationInnerClass(classDef, parent));
            break;
        }
        if (parent->IsClassDefinition()) {
            annotations.emplace_back(GenAnnotationEnclosingClass(
                parent->AsClassDefinition()->TsType()->AsETSObjectType()->AssemblerName().Utf8()));
            annotations.emplace_back(GenAnnotationInnerClass(classDef, parent));
            break;
        }
        parent = parent->Parent();
    }

    auto classIdent = classDef->Ident()->Name().Mutf8();
    bool isConstruct = classIdent == Signatures::JSNEW_CLASS;
    if (isConstruct || classIdent == Signatures::JSCALL_CLASS) {
        auto *callNames = Context()->checker->AsETSChecker()->DynamicCallNames(isConstruct);
        annotations.push_back(GenAnnotationDynamicCall(*callNames));
    }

    return annotations;
}

static uint32_t GetAccessFlags(const ir::ClassDefinition *classDef)
{
    uint32_t accessFlags = ACC_PUBLIC;
    if (classDef->IsAbstract()) {
        accessFlags |= ACC_ABSTRACT;
    } else if (classDef->IsFinal()) {
        accessFlags |= ACC_FINAL;
    }

    if (classDef->IsStatic()) {
        accessFlags |= ACC_STATIC;
    }

    return accessFlags;
}

void ETSEmitter::GenClassRecord(const ir::ClassDefinition *classDef, bool external)
{
    auto classRecord = pandasm::Record(classDef->InternalName().Mutf8(), Program()->lang);
    auto baseName = classDef->Ident()->Name().Mutf8();
    if (external) {
        classRecord.metadata->SetAttribute(Signatures::EXTERNAL);
    }

    if (!classDef->Annotations().empty()) {
        classRecord.metadata->SetAnnotations(GenCustomAnnotations(classDef->Annotations(), baseName));
    }

    uint32_t accessFlags = GetAccessFlags(classDef);
    classRecord.metadata->SetAccessFlags(accessFlags);
    classRecord.sourceFile = Context()->parserProgram->VarBinder()->Program()->SourceFile().GetAbsolutePath().Mutf8();

    auto *baseType = classDef->TsType()->AsETSObjectType();
    if (baseType->SuperType() != nullptr) {
        classRecord.metadata->SetAttributeValue(Signatures::EXTENDS_ATTRIBUTE,
                                                baseType->SuperType()->AssemblerName().Mutf8());
    } else {
        // NOTE: rtakacs. Replace the whole if block (below) with assert when lambda objects have super class.
        if (baseType->AssemblerName().Mutf8() != Signatures::BUILTIN_OBJECT) {
            classRecord.metadata->SetAttributeValue(Signatures::EXTENDS_ATTRIBUTE, Signatures::BUILTIN_OBJECT);
        }
    }

    for (auto *it : baseType->Interfaces()) {
        // We do not need to add dynamic interfaces
        if (it->IsETSDynamicType()) {
            continue;
        }

        auto *declNode = it->GetDeclNode();
        // NOTE: itrubachev. replace it with ASSERT(decl_node->IsTSInterfaceDeclaration())
        // after adding proper creation of lambda object in ETSFunctionType::AssignmentSource
        if (!declNode->IsTSInterfaceDeclaration()) {
            continue;
        }
        std::string name = declNode->AsTSInterfaceDeclaration()->InternalName().Mutf8();
        classRecord.metadata->SetAttributeValue(Signatures::IMPLEMENTS_ATTRIBUTE, name);
    }

    GenClassInheritedFields(baseType, classRecord);
    for (const auto *prop : classDef->Body()) {
        if (!prop->IsClassProperty()) {
            continue;
        }

        GenClassField(prop->AsClassProperty(), classRecord, external);
    }

    std::vector<pandasm::AnnotationData> annotations = GenAnnotations(classDef);
    if (!annotations.empty()) {
        classRecord.metadata->SetAnnotations(std::move(annotations));
    }

    Program()->recordTable.emplace(classRecord.name, std::move(classRecord));
}

// Helper function to check if the unary expression is a numeric literal with negation.
// This expression should be handled during lowering with the associated issue number.
static bool IsNegativeLiteralNode(const ir::UnaryExpression *expr)
{
    return expr->OperatorType() == lexer::TokenType::PUNCTUATOR_MINUS && expr->Argument()->IsNumberLiteral();
}

void ETSEmitter::CreateEnumProp(const ir::ClassProperty *prop, pandasm::Field &field)
{
    if (prop->Value() == nullptr) {
        return;
    }
    auto *init = prop->Value()->AsMemberExpression()->PropVar()->Declaration()->Node()->AsTSEnumMember()->Init();
    field.metadata->SetFieldType(field.type);
    if (init->IsNumberLiteral()) {
        auto value = init->AsNumberLiteral()->Number().GetInt();
        field.metadata->SetValue(pandasm::ScalarValue::Create<pandasm::Value::Type::I32>(value));
    } else if (init->IsStringLiteral()) {
        auto value = init->AsStringLiteral()->Str().Mutf8();
        field.metadata->SetValue(pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(value));
    } else if (init->IsUnaryExpression() && IsNegativeLiteralNode(init->AsUnaryExpression())) {
        double doubleValue = (-1) * init->AsUnaryExpression()->Argument()->AsNumberLiteral()->Number().GetDouble();
        field.metadata->SetValue(pandasm::ScalarValue::Create<pandasm::Value::Type::F64>(doubleValue));
    } else {
        UNREACHABLE();
    }
}

void ETSEmitter::ProcessArrayExpression(
    std::string &baseName, std::vector<std::pair<std::string, std::vector<pandasm::LiteralArray::Literal>>> &result,
    std::vector<pandasm::LiteralArray::Literal> &literals, const ir::Expression *elem)
{
    auto litArrays = CreateLiteralArray(baseName, elem);
    auto emplaceLiteral = [&literals](panda_file::LiteralTag tag, const auto &value) {
        literals.emplace_back(pandasm::LiteralArray::Literal {tag, value});
    };

    emplaceLiteral(panda_file::LiteralTag::TAGVALUE, static_cast<uint8_t>(panda_file::LiteralTag::LITERALARRAY));
    emplaceLiteral(panda_file::LiteralTag::LITERALARRAY, litArrays.back().first);
    for (const auto &item : litArrays) {
        result.push_back(item);
    }
}

static void ProcessEnumExpression(std::vector<pandasm::LiteralArray::Literal> &literals, const ir::Expression *elem)
{
    auto *memberExpr = elem->IsCallExpression() ? elem->AsCallExpression()->Arguments()[0]->AsMemberExpression()
                                                : elem->AsMemberExpression();
    auto *init = memberExpr->PropVar()->Declaration()->Node()->AsTSEnumMember()->Init();
    if (init->IsNumberLiteral()) {
        auto enumValue = static_cast<uint32_t>(init->AsNumberLiteral()->Number().GetInt());
        literals.emplace_back(pandasm::LiteralArray::Literal {panda_file::LiteralTag::TAGVALUE,
                                                              static_cast<uint8_t>(panda_file::LiteralTag::INTEGER)});
        literals.emplace_back(pandasm::LiteralArray::Literal {panda_file::LiteralTag::INTEGER, enumValue});
    } else {
        auto enumValue = init->AsStringLiteral()->Str().Mutf8();
        literals.emplace_back(pandasm::LiteralArray::Literal {panda_file::LiteralTag::TAGVALUE,
                                                              static_cast<uint8_t>(panda_file::LiteralTag::STRING)});
        literals.emplace_back(pandasm::LiteralArray::Literal {panda_file::LiteralTag::STRING, enumValue});
    }
}

void ETSEmitter::ProcessArrayElement(const ir::Expression *elem, std::vector<pandasm::LiteralArray::Literal> &literals,
                                     std::string &baseName, LiteralArrayVector &result)
{
    switch (elem->Type()) {
        case ir::AstNodeType::NUMBER_LITERAL: {
            auto doubleValue = elem->AsNumberLiteral()->Number().GetDouble();
            literals.emplace_back(pandasm::LiteralArray::Literal {
                panda_file::LiteralTag::TAGVALUE, static_cast<uint8_t>(panda_file::LiteralTag::DOUBLE)});
            literals.emplace_back(pandasm::LiteralArray::Literal {panda_file::LiteralTag::DOUBLE, doubleValue});
            break;
        }
        case ir::AstNodeType::BOOLEAN_LITERAL: {
            bool boolValue = elem->AsBooleanLiteral()->Value();
            literals.emplace_back(pandasm::LiteralArray::Literal {panda_file::LiteralTag::TAGVALUE,
                                                                  static_cast<uint8_t>(panda_file::LiteralTag::BOOL)});
            literals.emplace_back(pandasm::LiteralArray::Literal {panda_file::LiteralTag::BOOL, boolValue});
            break;
        }
        case ir::AstNodeType::STRING_LITERAL: {
            std::string stringValue {elem->AsStringLiteral()->Str().Utf8()};
            literals.emplace_back(pandasm::LiteralArray::Literal {
                panda_file::LiteralTag::TAGVALUE, static_cast<uint8_t>(panda_file::LiteralTag::STRING)});
            literals.emplace_back(pandasm::LiteralArray::Literal {panda_file::LiteralTag::STRING, stringValue});
            break;
        }
        case ir::AstNodeType::ARRAY_EXPRESSION: {
            ProcessArrayExpression(baseName, result, literals, elem);
            break;
        }
        case ir::AstNodeType::MEMBER_EXPRESSION:
        case ir::AstNodeType::CALL_EXPRESSION: {
            ProcessEnumExpression(literals, elem);
            break;
        }
        case ir::AstNodeType::UNARY_EXPRESSION: {
            double doubleValue = (-1) * elem->AsUnaryExpression()->Argument()->AsNumberLiteral()->Number().GetDouble();
            literals.emplace_back(pandasm::LiteralArray::Literal {
                panda_file::LiteralTag::TAGVALUE, static_cast<uint8_t>(panda_file::LiteralTag::DOUBLE)});
            literals.emplace_back(pandasm::LiteralArray::Literal {panda_file::LiteralTag::DOUBLE, doubleValue});
            break;
        }
        default:
            UNREACHABLE();
            break;
    }
}

LiteralArrayVector ETSEmitter::CreateLiteralArray(std::string &baseName, const ir::Expression *array)
{
    LiteralArrayVector result;
    std::vector<pandasm::LiteralArray::Literal> literals;
    ArenaVector<ir::Expression *> elements {array->AsArrayExpression()->Elements()};

    for (const auto *elem : elements) {
        ProcessArrayElement(elem, literals, baseName, result);
    }

    std::string litArrayName = GenerateMangledName(baseName, std::to_string(g_litArrayValueCount));
    ++g_litArrayValueCount;
    result.emplace_back(litArrayName, literals);
    return result;
}

void ETSEmitter::CreateLiteralArrayProp(const ir::ClassProperty *prop, std::string &baseName, pandasm::Field &field)
{
    uint8_t rank = 1;
    auto *elemType = prop->TsType()->AsETSArrayType()->ElementType();
    while (elemType->IsETSArrayType()) {
        ++rank;
        elemType = elemType->AsETSArrayType()->ElementType();
    }
    if (elemType->IsETSEnumType()) {
        field.type = PandasmTypeWithRank(elemType, rank);
    } else {
        std::stringstream ss;
        elemType->ToAssemblerType(ss);
        field.type = pandasm::Type(ss.str(), rank);
    }

    auto value = prop->Value();
    if (value != nullptr) {
        std::string newBaseName = GenerateMangledName(baseName, field.name);
        auto litArray = CreateLiteralArray(newBaseName, value);
        for (const auto &item : litArray) {
            Program()->literalarrayTable.emplace(item.first, item.second);
        }
        field.metadata->SetValue(
            pandasm::ScalarValue::Create<pandasm::Value::Type::LITERALARRAY>(std::string_view {litArray.back().first}));
    }
}

void ETSEmitter::GenCustomAnnotationProp(const ir::ClassProperty *prop, std::string &baseName, pandasm::Record &record,
                                         bool external)
{
    auto field = pandasm::Field(Program()->lang);
    auto *type = prop->TsType();
    field.name = prop->Id()->Name().Mutf8();
    field.type = PandasmTypeWithRank(type);
    field.metadata->SetAccessFlags(TranslateModifierFlags(prop->Modifiers()));

    if (external) {
        field.metadata->SetAttribute(Signatures::EXTERNAL);
    } else if (type->IsETSEnumType()) {
        CreateEnumProp(prop, field);
    } else if (type->IsETSPrimitiveType() || type->IsETSStringType()) {
        EmitDefaultFieldValue(field, prop->Value());
    } else if (type->IsETSArrayType()) {
        CreateLiteralArrayProp(prop, baseName, field);
    } else {
        UNREACHABLE();
    }
    record.fieldList.emplace_back(std::move(field));
}

void ETSEmitter::GenCustomAnnotationRecord(const ir::AnnotationDeclaration *annoDecl, std::string &baseName,
                                           bool external)
{
    auto annoRecord = pandasm::Record(annoDecl->InternalName().Mutf8(), Program()->lang);
    if (Program()->recordTable.find(annoRecord.name) != Program()->recordTable.end()) {
        return;
    }

    if (external) {
        annoRecord.metadata->SetAttribute(Signatures::EXTERNAL);
    }

    uint32_t accessFlags = ACC_PUBLIC | ACC_ABSTRACT | ACC_ANNOTATION;
    annoRecord.metadata->SetAccessFlags(accessFlags);
    annoRecord.sourceFile = Context()->parserProgram->VarBinder()->Program()->SourceFile().GetAbsolutePath().Mutf8();
    for (auto *it : annoDecl->Properties()) {
        GenCustomAnnotationProp(it->AsClassProperty(), baseName, annoRecord, external);
    }

    Program()->recordTable.emplace(annoRecord.name, std::move(annoRecord));
}

static pandasm::ScalarValue CreateAnnotationScalarValue(checker::TypeFlag typeKind, const ir::Literal *init)
{
    switch (typeKind) {
        case checker::TypeFlag::ETS_BOOLEAN: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::U1>(
                static_cast<uint8_t>(init->AsBooleanLiteral()->Value()));
        }
        case checker::TypeFlag::BYTE: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::I8>(init->AsNumberLiteral()->Number().GetInt());
        }
        case checker::TypeFlag::SHORT: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::I16>(init->AsNumberLiteral()->Number().GetInt());
        }
        case checker::TypeFlag::INT: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::I32>(init->AsNumberLiteral()->Number().GetInt());
        }
        case checker::TypeFlag::LONG: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::I64>(init->AsNumberLiteral()->Number().GetLong());
        }
        case checker::TypeFlag::FLOAT: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::F32>(
                init->AsNumberLiteral()->Number().GetFloat());
        }
        case checker::TypeFlag::DOUBLE: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::F64>(
                init->AsNumberLiteral()->Number().GetDouble());
        }
        case checker::TypeFlag::ETS_OBJECT: {
            return pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(init->AsStringLiteral()->Str().Mutf8());
        }
        default: {
            UNREACHABLE();
        }
    }
}

pandasm::AnnotationElement ETSEmitter::ProcessArrayType(const ir::ClassProperty *prop, std::string &baseName,
                                                        const ir::Expression *init)
{
    auto propName = prop->Id()->Name().Mutf8();
    std::string newBaseName = GenerateMangledName(baseName, propName);
    auto litArrays = CreateLiteralArray(newBaseName, init);

    for (const auto &item : litArrays) {
        Program()->literalarrayTable.emplace(item.first, item.second);
    }

    return pandasm::AnnotationElement {
        propName, std::make_unique<pandasm::ScalarValue>(pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(
                      std::string_view {litArrays.back().first}))};
}

pandasm::AnnotationElement ETSEmitter::GenCustomAnnotationElement(const ir::ClassProperty *prop, std::string &baseName)
{
    const auto *init = prop->Value();
    const auto *type = init->TsType();
    auto typeKind = checker::ETSChecker::TypeKind(type);
    auto propName = prop->Id()->Name().Mutf8();
    if (type->IsETSArrayType()) {
        return ProcessArrayType(prop, baseName, init);
    }
    switch (checker::ETSChecker::TypeKind(
        Context()->checker->AsETSChecker()->MaybeUnboxType(const_cast<checker::Type *>(type)))) {
        case checker::TypeFlag::BYTE:
        case checker::TypeFlag::SHORT:
        case checker::TypeFlag::INT:
        case checker::TypeFlag::LONG:
        case checker::TypeFlag::FLOAT:
        case checker::TypeFlag::DOUBLE:
        case checker::TypeFlag::ETS_BOOLEAN:
        case checker::TypeFlag::ETS_OBJECT: {
            if (init->IsUnaryExpression() && IsNegativeLiteralNode(init->AsUnaryExpression())) {
                double negNumberValue =
                    (-1) * init->AsUnaryExpression()->Argument()->AsNumberLiteral()->Number().GetDouble();
                return pandasm::AnnotationElement {
                    propName, std::make_unique<pandasm::ScalarValue>(
                                  pandasm::ScalarValue::Create<pandasm::Value::Type::F64>(negNumberValue))};
            }
            return pandasm::AnnotationElement {propName, std::make_unique<pandasm::ScalarValue>(
                                                             CreateAnnotationScalarValue(typeKind, init->AsLiteral()))};
        }
        case checker::TypeFlag::ETS_INT_ENUM: {
            auto *initValue = init->AsMemberExpression()->PropVar()->Declaration()->Node()->AsTSEnumMember()->Init();
            auto enumValue = static_cast<uint32_t>(initValue->AsNumberLiteral()->Number().GetInt());
            auto intEnumValue = pandasm::ScalarValue::Create<pandasm::Value::Type::I32>(enumValue);
            return pandasm::AnnotationElement {propName, std::make_unique<pandasm::ScalarValue>(intEnumValue)};
        }
        case checker::TypeFlag::ETS_STRING_ENUM: {
            auto *initValue = init->AsMemberExpression()->PropVar()->Declaration()->Node()->AsTSEnumMember()->Init();
            auto enumValue = initValue->AsStringLiteral()->Str().Mutf8();
            auto stringValue = pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(enumValue);
            return pandasm::AnnotationElement {propName, std::make_unique<pandasm::ScalarValue>(stringValue)};
        }
        default:
            UNREACHABLE();
    }
}

pandasm::AnnotationData ETSEmitter::GenCustomAnnotation(ir::AnnotationUsage *anno, std::string &baseName)
{
    auto *annoDecl = anno->GetBaseName()->Variable()->Declaration()->Node()->AsAnnotationDeclaration();
    pandasm::AnnotationData annotation(annoDecl->InternalName().Mutf8());
    if (annoDecl->IsImportDeclaration()) {
        auto annoRecord = pandasm::Record(annoDecl->InternalName().Mutf8(), Program()->lang);
        annoRecord.metadata->SetAttribute(Signatures::EXTERNAL);
        uint32_t accessFlags = ACC_PUBLIC | ACC_ABSTRACT | ACC_ANNOTATION;
        annoRecord.metadata->SetAccessFlags(accessFlags);
        Program()->recordTable.emplace(annoRecord.name, std::move(annoRecord));
    }

    for (auto *prop : anno->Properties()) {
        annotation.AddElement(GenCustomAnnotationElement(prop->AsClassProperty(), baseName));
    }
    return annotation;
}

std::vector<pandasm::AnnotationData> ETSEmitter::GenCustomAnnotations(
    const ArenaVector<ir::AnnotationUsage *> &annotationUsages, std::string &baseName)
{
    std::vector<pandasm::AnnotationData> annotations;
    for (auto *anno : annotationUsages) {
        auto newBaseName = GenerateMangledName(baseName, anno->GetBaseName()->Name().Mutf8());
        annotations.emplace_back(GenCustomAnnotation(anno, newBaseName));
    }
    return annotations;
}

pandasm::AnnotationData ETSEmitter::GenAnnotationSignature(const ir::ClassDefinition *classDef)
{
    static constexpr std::string_view OBJECT = "Lstd/core/Object";
    std::vector<pandasm::ScalarValue> parts {};
    std::stringstream ss {};
    const auto &params = classDef->TypeParams()->Params();

    bool firstIteration = true;
    for (const auto *param : params) {
        if (firstIteration) {
            ss << Signatures::GENERIC_BEGIN;
            firstIteration = false;
        }
        ss << param->Name()->Name() << Signatures::MANGLE_BEGIN;
        parts.emplace_back(pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(ss.str()));

        std::stringstream {}.swap(ss);
        if (param->Constraint() == nullptr) {
            ss << OBJECT;
        } else {
            param->Constraint()->TsType()->ToAssemblerTypeWithRank(ss);
            auto str = ss.str();
            std::replace(str.begin(), str.end(), *Signatures::METHOD_SEPARATOR.begin(),
                         *Signatures::NAMESPACE_SEPARATOR.begin());
            std::stringstream {}.swap(ss);
            ss << Signatures::CLASS_REF_BEGIN << str << Signatures::MANGLE_SEPARATOR;
        }

        parts.emplace_back(pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(ss.str()));
        std::stringstream {}.swap(ss);  // cleanup
    }

    ss << Signatures::GENERIC_END;
    parts.emplace_back(pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(ss.str()));

    std::stringstream {}.swap(ss);
    if (classDef->TsType()->AsETSObjectType()->SuperType() == nullptr) {
        ss << OBJECT;
    } else {
        ss << Signatures::CLASS_REF_BEGIN;
        auto superType = classDef->TsType()->AsETSObjectType()->SuperType()->AssemblerName().Mutf8();
        std::replace(superType.begin(), superType.end(), *Signatures::METHOD_SEPARATOR.begin(),
                     *Signatures::NAMESPACE_SEPARATOR.begin());
        ss << superType << Signatures::MANGLE_SEPARATOR;
    }
    parts.emplace_back(pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(ss.str()));

    GenAnnotationRecord(Signatures::ETS_ANNOTATION_SIGNATURE);
    pandasm::AnnotationData signature(Signatures::ETS_ANNOTATION_SIGNATURE);
    pandasm::AnnotationElement value(
        Signatures::ANNOTATION_KEY_VALUE,
        std::make_unique<pandasm::ArrayValue>(pandasm::Value::Type::STRING, std::move(parts)));
    signature.AddElement(std::move(value));
    return signature;
}

pandasm::AnnotationData ETSEmitter::GenAnnotationEnclosingMethod(const ir::MethodDefinition *methodDef)
{
    GenAnnotationRecord(Signatures::ETS_ANNOTATION_ENCLOSING_METHOD);
    pandasm::AnnotationData enclosingMethod(Signatures::ETS_ANNOTATION_ENCLOSING_METHOD);
    pandasm::AnnotationElement value(
        Signatures::ANNOTATION_KEY_VALUE,
        std::make_unique<pandasm::ScalarValue>(pandasm::ScalarValue::Create<pandasm::Value::Type::METHOD>(
            methodDef->Function()->Scope()->InternalName().Mutf8())));
    enclosingMethod.AddElement(std::move(value));
    return enclosingMethod;
}

pandasm::AnnotationData ETSEmitter::GenAnnotationEnclosingClass(std::string_view className)
{
    GenAnnotationRecord(Signatures::ETS_ANNOTATION_ENCLOSING_CLASS);
    pandasm::AnnotationData enclosingClass(Signatures::ETS_ANNOTATION_ENCLOSING_CLASS);
    pandasm::AnnotationElement value(
        Signatures::ANNOTATION_KEY_VALUE,
        std::make_unique<pandasm::ScalarValue>(
            pandasm::ScalarValue::Create<pandasm::Value::Type::RECORD>(pandasm::Type::FromName(className, true))));
    enclosingClass.AddElement(std::move(value));
    return enclosingClass;
}

pandasm::AnnotationData ETSEmitter::GenAnnotationInnerClass(const ir::ClassDefinition *classDef,
                                                            const ir::AstNode *parent)
{
    GenAnnotationRecord(Signatures::ETS_ANNOTATION_INNER_CLASS);
    pandasm::AnnotationData innerClass(Signatures::ETS_ANNOTATION_INNER_CLASS);
    const bool isAnonymous = (classDef->Modifiers() & ir::ClassDefinitionModifiers::ANONYMOUS) != 0;
    pandasm::AnnotationElement name(Signatures::ANNOTATION_KEY_NAME,
                                    std::make_unique<pandasm::ScalarValue>(
                                        isAnonymous
                                            ? pandasm::ScalarValue::Create<pandasm::Value::Type::STRING_NULLPTR>(0)
                                            : pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(
                                                  classDef->TsType()->AsETSObjectType()->AssemblerName().Mutf8())));
    innerClass.AddElement(std::move(name));

    pandasm::AnnotationElement accessFlags(
        Signatures::ANNOTATION_KEY_ACCESS_FLAGS,
        std::make_unique<pandasm::ScalarValue>(
            pandasm::ScalarValue::Create<pandasm::Value::Type::I32>(TranslateModifierFlags(parent->Modifiers()))));
    innerClass.AddElement(std::move(accessFlags));
    return innerClass;
}

ir::MethodDefinition *ETSEmitter::FindAsyncImpl(ir::ScriptFunction *asyncFunc)
{
    std::string implName = checker::ETSChecker::GetAsyncImplName(asyncFunc->Id()->Name());
    ir::AstNode *ownerNode = asyncFunc->Signature()->Owner()->GetDeclNode();
    ASSERT(ownerNode != nullptr && ownerNode->IsClassDefinition());
    const ir::ClassDefinition *classDef = ownerNode->AsClassDefinition();
    ASSERT(classDef != nullptr);

    auto it = std::find_if(classDef->Body().rbegin(), classDef->Body().rend(), [&implName](ir::AstNode *node) {
        return node->IsMethodDefinition() && node->AsMethodDefinition()->Id()->Name().Utf8() == implName;
    });
    if (it == classDef->Body().rend()) {
        return nullptr;
    }

    ir::MethodDefinition *method = (*it)->AsMethodDefinition();
    auto *checker = static_cast<checker::ETSChecker *>(Context()->checker);
    checker::TypeRelation *typeRel = checker->Relation();
    checker::SavedTypeRelationFlagsContext savedFlagsCtx(typeRel, checker::TypeRelationFlag::NO_RETURN_TYPE_CHECK);
    method->Function()->Signature()->Compatible(typeRel, asyncFunc->Signature());
    auto overloadIt = method->Overloads().begin();
    while (overloadIt != method->Overloads().end() && !typeRel->IsTrue()) {
        method = *overloadIt;
        method->Function()->Signature()->Compatible(typeRel, asyncFunc->Signature());
        ++overloadIt;
    }
    return typeRel->IsTrue() ? method : nullptr;
}

pandasm::AnnotationData ETSEmitter::GenAnnotationAsync(ir::ScriptFunction *scriptFunc)
{
    GenAnnotationRecord(Signatures::ETS_COROUTINE_ASYNC);
    const ir::MethodDefinition *impl = FindAsyncImpl(scriptFunc);
    ASSERT(impl != nullptr);
    pandasm::AnnotationData ann(Signatures::ETS_COROUTINE_ASYNC);
    pandasm::AnnotationElement value(
        Signatures::ANNOTATION_KEY_VALUE,
        std::make_unique<pandasm::ScalarValue>(pandasm::ScalarValue::Create<pandasm::Value::Type::METHOD>(
            impl->Function()->Scope()->InternalName().Mutf8())));
    ann.AddElement(std::move(value));
    return ann;
}

pandasm::AnnotationData ETSEmitter::GenAnnotationDynamicCall(DynamicCallNamesMap &callNames)
{
    GenAnnotationRecord(Signatures::ETS_ANNOTATION_DYNAMIC_CALL);
    pandasm::AnnotationData dynamicCallSig(Signatures::ETS_ANNOTATION_DYNAMIC_CALL);
    std::vector<pandasm::ScalarValue> allParts {};
    for (auto &[parts, startIdx] : callNames) {
        startIdx = allParts.size();
        for (const auto &str : parts) {
            allParts.emplace_back(pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(str.Utf8()));
        }
    }
    pandasm::AnnotationElement value(
        Signatures::ANNOTATION_KEY_VALUE,
        std::make_unique<pandasm::ArrayValue>(pandasm::Value::Type::STRING, std::move(allParts)));
    dynamicCallSig.AddElement(std::move(value));
    return dynamicCallSig;
}

void ETSEmitter::GenAnnotationRecord(std::string_view recordNameView, bool isRuntime, bool isType)
{
    const std::string recordName(recordNameView);
    const auto recordIt = Program()->recordTable.find(recordName);
    if (recordIt == Program()->recordTable.end()) {
        pandasm::Record record(recordName, EXTENSION);
        record.metadata->SetAttribute(Signatures::EXTERNAL);
        record.metadata->SetAttribute(Signatures::ANNOTATION_ATTRIBUTE);
        if (isRuntime && isType) {
            record.metadata->SetAttributeValue(Signatures::ANNOTATION_ATTRIBUTE_TYPE,
                                               Signatures::RUNTIME_TYPE_ANNOTATION);
        } else if (isRuntime && !isType) {
            record.metadata->SetAttributeValue(Signatures::ANNOTATION_ATTRIBUTE_TYPE, Signatures::RUNTIME_ANNOTATION);
        } else if (!isRuntime && isType) {
            record.metadata->SetAttributeValue(Signatures::ANNOTATION_ATTRIBUTE_TYPE, Signatures::TYPE_ANNOTATION);
        }
        Program()->recordTable.emplace(record.name, std::move(record));
    }
}
}  // namespace ark::es2panda::compiler
