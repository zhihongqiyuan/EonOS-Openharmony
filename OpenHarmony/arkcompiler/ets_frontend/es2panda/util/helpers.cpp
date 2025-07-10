/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.Apache.Org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "helpers.h"

#include <abc2program/common/abc_file_utils.h>
#include <ir/base/classDefinition.h>
#include <ir/base/property.h>
#include <ir/base/scriptFunction.h>
#include <ir/base/spreadElement.h>
#include <ir/expressions/arrayExpression.h>
#include <ir/expressions/assignmentExpression.h>
#include <ir/expressions/functionExpression.h>
#include <ir/expressions/literals/booleanLiteral.h>
#include <ir/expressions/literals/numberLiteral.h>
#include <ir/expressions/literals/stringLiteral.h>
#include <ir/expressions/objectExpression.h>
#include <ir/expressions/unaryExpression.h>
#include <ir/statements/blockStatement.h>
#include <ir/statements/expressionStatement.h>
#include <ir/statements/variableDeclarator.h>
#include <ir/ts/tsParameterProperty.h>
#include <util/commonUtil.h>
#include <util/concurrent.h>

#ifdef ENABLE_BYTECODE_OPT
#include <bytecode_optimizer/bytecode_analysis_results.h>
#include <bytecode_optimizer/optimize_bytecode.h>
#else
#include <assembly-type.h>
#include <assembly-program.h>
#include <assembly-emitter.h>
#endif

#ifdef PANDA_TARGET_WINDOWS
#include <windows.h>
#undef ERROR
#else
#include <unistd.h>
#endif

namespace panda::es2panda::util {

// Helpers

bool Helpers::IsGlobalIdentifier(const util::StringView &str)
{
    return (str.Is("NaN") || str.Is("undefined") || str.Is("Infinity"));
}

bool Helpers::ContainSpreadElement(const ArenaVector<ir::Expression *> &args)
{
    return std::any_of(args.begin(), args.end(), [](const auto *it) { return it->IsSpreadElement(); });
}

util::StringView Helpers::LiteralToPropName(ArenaAllocator *allocator, const ir::Expression *lit)
{
    switch (lit->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            return lit->AsIdentifier()->Name();
        }
        case ir::AstNodeType::STRING_LITERAL: {
            return lit->AsStringLiteral()->Str();
        }
        case ir::AstNodeType::NUMBER_LITERAL: {
            return Helpers::ToStringView(allocator, lit->AsNumberLiteral()->Number());
        }
        case ir::AstNodeType::NULL_LITERAL: {
            return "null";
        }
        case ir::AstNodeType::BOOLEAN_LITERAL: {
            if (lit->AsBooleanLiteral()->Value()) {
                return "true";
            }
            return "false";
        }
        default: {
            UNREACHABLE();
        }
    }
}

bool Helpers::IsIndex(double number)
{
    if (number >= 0 && number < static_cast<double>(INVALID_INDEX)) {
        auto intNum = static_cast<uint32_t>(number);

        if (static_cast<double>(intNum) == number) {
            return true;
        }
    }

    return false;
}

static bool IsDigit(char c)
{
    return (c >= '0' && c <= '9');
}

int64_t Helpers::GetIndex(const util::StringView &str)
{
    const auto &s = str.Utf8();

    if (s.empty() || (*s.begin() == '0' && s.length() > 1)) {
        return INVALID_INDEX;
    }

    int64_t value = 0;
    for (const auto c : s) {
        if (!IsDigit(c)) {
            return INVALID_INDEX;
        }

        constexpr auto MULTIPLIER = 10;
        value *= MULTIPLIER;
        value += (c - '0');

        if (value >= INVALID_INDEX) {
            return INVALID_INDEX;
        }
    }

    return value;
}

bool Helpers::FileExtensionIs(std::string_view filePath, std::string_view extension)
{
    return filePath.length() > extension.length() && Helpers::EndsWith(filePath, extension);
}

bool Helpers::EndsWith(std::string_view str, std::string_view suffix)
{
    if (str.length() < suffix.length()) {
        return false;
    }
    size_t expectPos = str.length() - suffix.length();
    return str.find(suffix, expectPos) == expectPos;
}

void Helpers::GetScientificNotationForDouble(double number, uint32_t significandBitCount, int32_t &numberBitCount,
                                             char *significandArray, char *sciNotationArray, uint32_t size)
{
    if (size < MAX_DOUBLE_DIGIT) {
        std::cerr << "Failed to set the size of buffer, the buffer size provided (" << size
                  << ") is less than the required minimum size (" << MAX_DOUBLE_DIGIT
                  << ") for formatting the number in scientific notation." << std::endl;
        return;
    }
    if (snprintf_s(sciNotationArray, size, size - 1, "%.*e", significandBitCount - 1, number) == FAIL_SNPRINTF_S) {
        std::cerr << "Failed to format the number " << number
                  << " into scientific notation using snprintf_s. Please check if the buffer size (" << size
                  << ") and significand bit count (" << significandBitCount << ") are appropriate." << std::endl;
        return;
    }

    // sciNotationArray includes significand, '.' and 'e'
    // If significandBitCount == 1, sciNotationArray does not contain '.'
    int32_t exponent = atoi(sciNotationArray + significandBitCount + 1 + (significandBitCount > 1));
    numberBitCount = exponent + 1;

    // Get the significand of the current sciNotationArray
    if (significandBitCount > 1) {
        for (uint32_t i = 0; i < significandBitCount + 1; i++) {
            significandArray[i] = sciNotationArray[i];
        }
    }

    significandArray[significandBitCount + 1] = '\0';
}

int32_t Helpers::GetIntegerSignificandBitCount(double number, int32_t &numberBitCount, char *significandArray)
{
    uint32_t bitPos = 0;
    uint32_t minBitPos = 1;
    uint32_t maxBitPos = MAX_DOUBLE_PRECISION_DIGIT;
    uint32_t integerAndPointBitCount = 2;
    char sciNotationArray[MAX_DOUBLE_DIGIT] = {0};

    while (minBitPos < maxBitPos) {
        bitPos = (minBitPos + maxBitPos) / 2; // 2: binary search
        GetScientificNotationForDouble(number, bitPos, numberBitCount, significandArray,
                                       sciNotationArray, sizeof(sciNotationArray));

        // Update bitPos
        if (std::strtod(sciNotationArray, nullptr) == number) {
            while (bitPos >= integerAndPointBitCount && significandArray[bitPos] == '0') {
                bitPos--;
            }
            maxBitPos = bitPos;
        } else {
            minBitPos = bitPos + 1;
        }
    }

    // minBitPos == maxBitPos
    bitPos = maxBitPos;
    GetScientificNotationForDouble(number, bitPos, numberBitCount, significandArray,
                                   sciNotationArray, sizeof(sciNotationArray));

    return bitPos;
}

std::string Helpers::DoubleToString(double number)
{
    // In Scientific notation, number is expressed in the form of significand multiplied by exponent-th power of 10.
    // The range of significand is: 1 <= |significand| < 10
    // Scientific notation of number: sciNotationArray = significand * (10 ** exponent)
    // number 1.23e25 => sciNotationArray: 1.23e+25, significand: 1.23, exponent: 25,

    // In the ECMAScript, integerSignificand, integerSignificandBitCount and numberBitCount are defined as an integer:
    // 1. integerSignificand is an integer in the Decimal representation of Scientific notation.
    // 2. integerSignificandBitCount is the number of bits in the Decimal representation of significand.
    // 3. numberBitCount is the number of bits in the Decimal representation of number.
    // Scientific notation of number in the ECMAScript of Number::toString (number):
    // integerSciNotationArray = integerSignificand * (10 ** (numberBitCount - integerSignificandBitCount))
    // number 1.23e25 => integerSciNotationArray: 123e+23, integerSignificand: 123, integerExponent: 23,
    //                   integerSignificandBitCount: 3, numberBitCount: 26
    std::string result;
    int32_t numberBitCount = 0;
    char significandArray[MAX_DOUBLE_DIGIT] = {0};

    if (number < 0) {
        result += "-";
        number = -number;
    }

    // The number of bits of significand in integer form
    int32_t integerSignificandBitCount = GetIntegerSignificandBitCount(number, numberBitCount, significandArray);

    std::string significand = significandArray;
    std::string integerSignificand;
    if (numberBitCount > 0 && numberBitCount <= MAX_DECIMAL_EXPONENT) {
        integerSignificand = significand.erase(1, 1);
        if (numberBitCount >= integerSignificandBitCount) {
            // If integerSignificandBitCount ≤ numberBitCount ≤ 21, return the string represented by Decimal,
            // integerSignificand followed by (numberBitCount - integerSignificandBitCount) digit zeros.
            integerSignificand += std::string(numberBitCount - integerSignificandBitCount, '0');
        } else {
            // If 0 < numberBitCount < integerSignificandBitCount, return the string represented by Decimal,
            // integerSignificand followed by point on the (numberBitCount + 1) digit.
            integerSignificand.insert(numberBitCount, 1, '.');
        }
    } else if (numberBitCount <= 0 && numberBitCount > MIN_DECIMAL_EXPONENT) {
        // If -6 < numberBitCount ≤ 0, return the string consisting of "0." and digit zeros represented by Decimal,
        // string followed by integerSignificand.
        integerSignificand = significand.erase(1, 1);
        integerSignificand = std::string("0.") + std::string(-numberBitCount, '0') + integerSignificand;
    } else {
        // If integerSignificandBitCount == 1, return the string consisting of the single digit of significand.
        if (integerSignificandBitCount == 1) {
            significand = significand.erase(1, 1);
        }
        // If numberBitCount ≤ -6 or numberBitCount > 21, return the string represented by Scientific notation,
        // integerSignificand followed by "e", symbol and (numberBitCount - 1) digit zeros.
        significand += 'e' + (numberBitCount >= 1 ? std::string("+") : "") + std::to_string(numberBitCount - 1);

        result += significand;
        return result;
    }

    result += integerSignificand;
    return result;
}

std::string Helpers::ToString(double number)
{
    if (std::isnan(number)) {
        return "NaN";
    }
    if (number == 0.0) {
        return "0";
    }
    if (std::isinf(number)) {
        return "Infinity";
    }

    std::string str;
    if (Helpers::IsInteger<int32_t>(number)) {
        str = std::to_string(static_cast<int32_t>(number));
    } else {
        str = DoubleToString(number);
    }

    return str;
}

util::StringView Helpers::ToStringView(ArenaAllocator *allocator, double number)
{
    util::UString str(ToString(number), allocator);
    return str.View();
}

util::StringView Helpers::ToStringView(ArenaAllocator *allocator, uint32_t number)
{
    ASSERT(number <= static_cast<uint32_t>(std::numeric_limits<int32_t>::max()));
    return ToStringView(allocator, static_cast<int32_t>(number));
}

util::StringView Helpers::ToStringView(ArenaAllocator *allocator, int32_t number)
{
    util::UString str(ToString(number), allocator);
    return str.View();
}

const ir::ScriptFunction *Helpers::GetContainingConstructor(const ir::AstNode *node)
{
    const ir::ScriptFunction *iter = GetContainingFunction(node);

    while (iter != nullptr) {
        if (iter->IsConstructor()) {
            return iter;
        }

        if (!iter->IsArrow()) {
            return nullptr;
        }

        iter = GetContainingFunction(iter);
    }

    return iter;
}

const ir::ScriptFunction *Helpers::GetContainingConstructor(const ir::ClassProperty *node)
{
    for (const auto *parent = node->Parent(); parent != nullptr; parent = parent->Parent()) {
        if (parent->IsClassDefinition()) {
            return parent->AsClassDefinition()->Ctor()->Function();
        }
    }

    return nullptr;
}

const ir::ScriptFunction *Helpers::GetContainingFunction(const ir::AstNode *node)
{
    for (const auto *parent = node->Parent(); parent != nullptr; parent = parent->Parent()) {
        if (parent->IsScriptFunction()) {
            return parent->AsScriptFunction();
        }
    }

    return nullptr;
}

const ir::ClassDefinition *Helpers::GetClassDefiniton(const ir::ScriptFunction *node)
{
    CHECK_NOT_NULL(node);
    ASSERT(node->IsConstructor() || node->IsMethod());
    ASSERT(node->Parent()->IsFunctionExpression());
    ASSERT(node->Parent()->Parent()->IsMethodDefinition());
    ASSERT(node->Parent()->Parent()->Parent()->IsClassDefinition());

    return node->Parent()->Parent()->Parent()->AsClassDefinition();
}

bool Helpers::IsSpecialPropertyKey(const ir::Expression *expr)
{
    if (!expr->IsStringLiteral()) {
        return false;
    }

    auto *lit = expr->AsStringLiteral();
    return lit->Str().Is("prototype") || lit->Str().Is("constructor");
}

bool Helpers::IsConstantPropertyKey(const ir::Expression *expr, bool isComputed)
{
    switch (expr->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            return !isComputed;
        }
        case ir::AstNodeType::NUMBER_LITERAL:
        case ir::AstNodeType::STRING_LITERAL:
        case ir::AstNodeType::BOOLEAN_LITERAL:
        case ir::AstNodeType::NULL_LITERAL: {
            return true;
        }
        default:
            break;
    }

    return false;
}

bool Helpers::IsConstantExpr(const ir::Expression *expr)
{
    switch (expr->Type()) {
        case ir::AstNodeType::NUMBER_LITERAL:
        case ir::AstNodeType::STRING_LITERAL:
        case ir::AstNodeType::BOOLEAN_LITERAL:
        case ir::AstNodeType::NULL_LITERAL: {
            return true;
        }
        default:
            break;
    }

    return false;
}

bool Helpers::IsBindingPattern(const ir::AstNode *node)
{
    return node->IsArrayPattern() || node->IsObjectPattern();
}

bool Helpers::IsPattern(const ir::AstNode *node)
{
    return node->IsArrayPattern() || node->IsObjectPattern() || node->IsAssignmentPattern();
}

static void CollectBindingName(const ir::AstNode *node, std::vector<const ir::Identifier *> *bindings)
{
    switch (node->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            if (!Helpers::IsGlobalIdentifier(node->AsIdentifier()->Name())) {
                bindings->push_back(node->AsIdentifier());
            }

            break;
        }
        case ir::AstNodeType::OBJECT_PATTERN: {
            for (const auto *prop : node->AsObjectPattern()->Properties()) {
                CollectBindingName(prop, bindings);
            }
            break;
        }
        case ir::AstNodeType::ARRAY_PATTERN: {
            for (const auto *element : node->AsArrayPattern()->Elements()) {
                CollectBindingName(element, bindings);
            }
            break;
        }
        case ir::AstNodeType::ASSIGNMENT_PATTERN: {
            CollectBindingName(node->AsAssignmentPattern()->Left(), bindings);
            break;
        }
        case ir::AstNodeType::PROPERTY: {
            CollectBindingName(node->AsProperty()->Value(), bindings);
            break;
        }
        case ir::AstNodeType::REST_ELEMENT: {
            CollectBindingName(node->AsRestElement()->Argument(), bindings);
            break;
        }
        default:
            break;
    }
}

std::vector<const ir::Identifier *> Helpers::CollectBindingNames(const ir::AstNode *node)
{
    std::vector<const ir::Identifier *> bindings;
    CollectBindingName(node, &bindings);
    return bindings;
}

util::StringView Helpers::FunctionName(ArenaAllocator *allocator, const ir::ScriptFunction *func)
{
    if (func->Id()) {
        return func->Id()->Name();
    }

    if (func->Parent()->IsFunctionDeclaration()) {
        return parser::SourceTextModuleRecord::DEFAULT_EXTERNAL_NAME;
    }

    const ir::AstNode *parent = func->Parent()->Parent();

    if (func->IsConstructor()) {
        parent = parent->Parent();
        if (parent->AsClassDefinition()->Ident()) {
            return parent->AsClassDefinition()->Ident()->Name();
        }

        parent = parent->Parent()->Parent();
    }
    return GetName(allocator, parent);
}

util::StringView Helpers::GetName(ArenaAllocator *allocator, const ir::AstNode *node)
{
    switch (node->Type()) {
        case ir::AstNodeType::VARIABLE_DECLARATOR: {
            const ir::VariableDeclarator *varDecl = node->AsVariableDeclarator();

            if (varDecl->Id()->IsIdentifier()) {
                return varDecl->Id()->AsIdentifier()->Name();
            }

            break;
        }
        case ir::AstNodeType::METHOD_DEFINITION: {
            const ir::MethodDefinition *methodDef = node->AsMethodDefinition();

            if (methodDef->Key()->IsIdentifier()) {
                return methodDef->Key()->AsIdentifier()->Name();
            }

            break;
        }
        case ir::AstNodeType::ASSIGNMENT_EXPRESSION: {
            const ir::AssignmentExpression *assignment = node->AsAssignmentExpression();

            if (assignment->Left()->IsIdentifier()) {
                return assignment->Left()->AsIdentifier()->Name();
            }

            break;
        }
        case ir::AstNodeType::ASSIGNMENT_PATTERN: {
            const ir::AssignmentExpression *assignment = node->AsAssignmentPattern();

            if (assignment->Left()->IsIdentifier()) {
                return assignment->Left()->AsIdentifier()->Name();
            }

            break;
        }
        case ir::AstNodeType::PROPERTY: {
            const ir::Property *prop = node->AsProperty();

            if (prop->Kind() != ir::PropertyKind::PROTO &&
                Helpers::IsConstantPropertyKey(prop->Key(), prop->IsComputed())) {
                return Helpers::LiteralToPropName(allocator, prop->Key());
            }

            break;
        }
        case ir::AstNodeType::CLASS_PROPERTY: {
            const ir::ClassProperty *prop = node->AsClassProperty();
            if (Helpers::IsConstantPropertyKey(prop->Key(), prop->IsComputed())) {
                return Helpers::LiteralToPropName(allocator, prop->Key());
            }

            break;
        }
        case ir::AstNodeType::EXPORT_DEFAULT_DECLARATION: {
            return parser::SourceTextModuleRecord::DEFAULT_EXTERNAL_NAME;
        }
        default:
            break;
    }

    return util::StringView();
}

std::tuple<util::StringView, bool> Helpers::ParamName(ArenaAllocator *allocator, const ir::AstNode *param,
                                                      uint32_t index)
{
    switch (param->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            return {param->AsIdentifier()->Name(), false};
        }
        case ir::AstNodeType::ASSIGNMENT_PATTERN: {
            const auto *lhs = param->AsAssignmentPattern()->Left();
            if (lhs->IsIdentifier()) {
                return {param->AsAssignmentPattern()->Left()->AsIdentifier()->Name(), false};
            }
            break;
        }
        case ir::AstNodeType::REST_ELEMENT: {
            if (param->AsRestElement()->Argument()->IsIdentifier()) {
                return {param->AsRestElement()->Argument()->AsIdentifier()->Name(), false};
            }
            break;
        }
        case ir::AstNodeType::TS_PARAMETER_PROPERTY: {
            return ParamName(allocator, param->AsTSParameterProperty()->Parameter(), index);
        }
        default:
            break;
    }

    return {Helpers::ToStringView(allocator, index), true};
}

bool Helpers::IsChild(const ir::AstNode *parent, const ir::AstNode *child)
{
    while (child) {
        if (child == parent) {
            return true;
        }

        child = child->Parent();
    }

    return false;
}

bool Helpers::IsChildScope(const binder::Scope *parent, const binder::Scope *child)
{
    while (child) {
        if (child == parent) {
            return true;
        }

        child = child->Parent();
    }

    return false;
}

bool Helpers::IsObjectPropertyValue(const ArenaVector<ir::Expression *> &properties, const ir::AstNode *ident)
{
    for (const auto *prop : properties) {
        ASSERT(prop->IsProperty() || prop->IsSpreadElement());
        if (prop->IsProperty() && (prop->AsProperty()->Value() == ident)) {
            return true;
        }

        if (prop->IsSpreadElement() && (prop->AsSpreadElement()->Argument() == ident)) {
            return true;
        }
    }

    return false;
}

SignedNumberLiteral Helpers::GetSignedNumberLiteral(const ir::Expression *expr)
{
    if (!expr->IsUnaryExpression()) {
        return SignedNumberLiteral::UNRECOGNIZED;
    }

    auto unaryExpression = expr->AsUnaryExpression();
    if (!unaryExpression->Argument()->IsNumberLiteral()) {
        return SignedNumberLiteral::UNRECOGNIZED;
    }

    // TODO(hxw): Here we return different value for positive and nagative number literal in UnaryExpression.
    // Because when we access a computed property by MemberExpression, the compiler should emit different instruction.
    // Now es2abc always emits the instruction `loadObjByValue` whether the computed property is literal or not.
    // It can be optimized. For positive integer literal, the instruction should be `loadObjByIndex`.
    // While for negative number literal, the instruction should be `loadObjByName`.
    // So I add this util api and return different value for future use.
    if (unaryExpression->OperatorType() == lexer::TokenType::PUNCTUATOR_PLUS) {
        return SignedNumberLiteral::POSITIVE;
    } else if (unaryExpression->OperatorType() == lexer::TokenType::PUNCTUATOR_MINUS) {
        return SignedNumberLiteral::NEGATIVE;
    }

    return SignedNumberLiteral::UNRECOGNIZED;
}

void Helpers::SetConstantLocalExportSlots(const std::string &record, const std::unordered_set<uint32_t> &slots)
{
    bool ignored;
    auto &result = panda::bytecodeopt::BytecodeAnalysisResults::GetOrCreateBytecodeAnalysisResult(record, ignored);
    result.SetConstantLocalExportSlots(slots);
}

static std::string GetTempOutputName(const std::string &inputFile)
{
    std::string pid;
#ifdef PANDA_TARGET_WINDOWS
    pid = std::to_string(GetCurrentProcessId());
#else
    pid = std::to_string(getpid());
#endif
    const std::string outputSuffix = ".unopt.abc";
    return panda::os::file::File::GetExtendedFilePath(inputFile + pid + outputSuffix);
}

void Helpers::AnalysisProgram(panda::pandasm::Program *prog, const std::string &inputFile)
{
    std::map<std::string, size_t> stat;
    std::map<std::string, size_t> *statp = &stat;

#ifdef PANDA_WITH_BYTECODE_OPTIMIZER
    auto tempOutput = GetTempOutputName(inputFile);
    bool exists = false;
    auto mapsp = &panda::bytecodeopt::BytecodeAnalysisResults::GetOrCreateBytecodeMaps(tempOutput, exists);
    ASSERT(!exists);

    const uint32_t COMPONENT_MASK = panda::Logger::Component::ASSEMBLER |
                                    panda::Logger::Component::BYTECODE_OPTIMIZER |
                                    panda::Logger::Component::COMPILER;
    panda::Logger::InitializeStdLogging(panda::Logger::Level::ERROR, COMPONENT_MASK);

    if (panda::pandasm::AsmEmitter::Emit(tempOutput, *prog, statp, mapsp, true)) {
        panda::bytecodeopt::AnalysisBytecode(prog, mapsp, tempOutput, true, true);
    } else {
        panda::bytecodeopt::BytecodeAnalysisResults::DeleteBytecodeMaps(tempOutput);
    }
#endif
}

void Helpers::OptimizeProgram(panda::pandasm::Program *prog, const std::string &inputFile)
{
    std::map<std::string, size_t> stat;
    std::map<std::string, size_t> *statp = &stat;
    auto tempOutput = GetTempOutputName(inputFile);

#ifdef PANDA_WITH_BYTECODE_OPTIMIZER
    const uint32_t COMPONENT_MASK = panda::Logger::Component::ASSEMBLER |
                                    panda::Logger::Component::BYTECODE_OPTIMIZER |
                                    panda::Logger::Component::COMPILER;
    panda::Logger::InitializeStdLogging(panda::Logger::Level::ERROR, COMPONENT_MASK);

    bool exists = false;
    auto mapsp = &panda::bytecodeopt::BytecodeAnalysisResults::GetOrCreateBytecodeMaps(tempOutput, exists);
    if (!exists) {
        exists = panda::pandasm::AsmEmitter::Emit(tempOutput, *prog, statp, mapsp, true);
    }
    if (exists) {
        panda::bytecodeopt::OptimizeBytecode(prog, mapsp, tempOutput, true, true);
        std::remove(tempOutput.c_str());
    }
    panda::bytecodeopt::BytecodeAnalysisResults::DeleteBytecodeMaps(tempOutput);

#endif
}

bool Helpers::CheckAopTransformPath(const std::string &libPath)
{
    std::string lowerLibPath = libPath;
    std::transform(libPath.begin(), libPath.end(), lowerLibPath.begin(), ::tolower);
    bool isValidSuffix = false;
#ifdef PANDA_TARGET_WINDOWS
    isValidSuffix = FileExtensionIs(lowerLibPath, FileSuffix::DLL);
    std::string supportSuffix = std::string(FileSuffix::DLL);
#else
    isValidSuffix = FileExtensionIs(lowerLibPath, FileSuffix::SO)
        || FileExtensionIs(lowerLibPath, FileSuffix::DYLIB);
    std::string supportSuffix = std::string(FileSuffix::SO) + "|" + std::string(FileSuffix::DYLIB);
#endif
    //check file suffix(.so|.dll)
    if (!isValidSuffix) {
        std::string msg = "aop transform file suffix support " + supportSuffix + ", error file: " + libPath;
        std::cout << msg << std::endl;
        return false;
    }
    return true;
}

AopTransformFuncDef Helpers::LoadAopTransformLibFunc(const std::string &libPath,
    const std::string &funcName, os::library_loader::LibraryHandle &handler)
{
    auto loadRes = os::library_loader::Load(libPath);
    if (!loadRes.HasValue()) {
        std::string msg = "os::library_loader::Load error: " + loadRes.Error().ToString();
        std::cout << msg << std::endl;
        return nullptr;
    }
    handler = std::move(loadRes.Value());

    auto initRes = os::library_loader::ResolveSymbol(handler, funcName);
    if (!initRes.HasValue()) {
        std::string msg = "os::library_loader::ResolveSymbol get func Transform error: " + initRes.Error().ToString();
        std::cout << msg << std::endl;
        return nullptr;
    }

    return reinterpret_cast<AopTransformFuncDef>(initRes.Value());
}

bool Helpers::AopTransform(const std::string &inputFile, const std::string &libPath)
{
    if (!CheckAopTransformPath(libPath)) {
        return false;
    }

    os::library_loader::LibraryHandle handler(nullptr);
    AopTransformFuncDef transform = LoadAopTransformLibFunc(libPath, "Transform", handler);
    if (transform == nullptr) {
        return false;
    }

    //invoke Transform, untransformed ABC to transformed ABC, result define: 0:success, other:fail
    int res = transform(inputFile.c_str());
    if (res != 0) {
        std::string msg = "Transform exec fail: " + libPath;
        std::cout << msg << std::endl;
        return false;
    }
    return true;
}

bool Helpers::ReadFileToBuffer(const std::string &file, std::stringstream &ss)
{
    std::ifstream inputStream = Helpers::FileStream<std::ifstream>(
        panda::os::file::File::GetExtendedFilePath(file), std::ios::binary);
    if (inputStream.fail()) {
        std::cerr << "Failed to read file to buffer: " << file << std::endl <<
                     "Solutions: > Check whether the above file exists." <<
                     "> Check whether you have the correct access permissions for the file.";
        return false;
    }
    ss << inputStream.rdbuf();
    return true;
}

void Helpers::ScanDirectives(ir::ScriptFunction *func, const lexer::LineIndex &lineIndex, bool enableSendableClass,
                             bool enableSendableFunc)
{
    auto *body = func->Body();
    if (!body || body->IsExpression()) {
        return;
    }

    auto &statements = body->AsBlockStatement()->Statements();
    if (statements.empty()) {
        return;
    }

    bool keepScan = true;
    auto iter = statements.begin();
    while (keepScan && (iter != statements.end())) {
        auto *stmt = *iter++;
        if (!stmt->IsExpressionStatement()) {
            return;
        }

        auto *expr = stmt->AsExpressionStatement()->GetExpression();
        if (!expr->IsStringLiteral()) {
            return;
        }

        keepScan = SetFuncFlagsForDirectives(expr->AsStringLiteral(), func, lineIndex, enableSendableClass,
                                             enableSendableFunc);
    }

    return;
}

bool Helpers::SetFuncFlagsForDirectives(const ir::StringLiteral *strLit, ir::ScriptFunction *func,
                                        const lexer::LineIndex &lineIndex, bool enableSendableClass,
                                        bool enableSendableFunc)
{
    if (strLit->Str().Is(USE_CONCURRENT)) {
        util::Concurrent::SetConcurrent(func, strLit, lineIndex);
        return true;
    }

    if (strLit->Str().Is(USE_SENDABLE)) {
        if (func->IsConstructor()) {
            if (enableSendableClass) {
                auto *classDef = const_cast<ir::ClassDefinition*>(GetClassDefiniton(func));
                classDef->SetSendable();
            }
        } else if (enableSendableFunc) {
            func->AddFlag(ir::ScriptFunctionFlags::SENDABLE);
        }
        return true;
    }

    return false;
}

std::string Helpers::GetHashString(const std::string &str)
{
    uint64_t result = FNV_OFFSET;

    const uint8_t *input = reinterpret_cast<const uint8_t *>(str.c_str());
    // FNV-1a 64-bit Algorithm
    for (size_t i = 0; i < str.size(); i++) {
        result ^= input[i];
        result *= FNV_PRIME;
    }

    return std::to_string(result);
}

#ifdef PANDA_TARGET_WINDOWS
std::wstring Helpers::Utf8ToUtf16(const std::string &utf8)
{
    std::wstring utf16;
    if (utf8.empty()) {
        return utf16;
    }

    if (utf8.length() > static_cast<size_t>(std::numeric_limits<int>::max())) {
        std::cerr << "Length of filename: " << utf8 << " is too long" << std::endl;
        return utf16;
    }

    const int utf8Length = static_cast<int>(utf8.length());
    constexpr DWORD kFlags = MB_ERR_INVALID_CHARS;
    const int utf16Length = MultiByteToWideChar(CP_UTF8, kFlags, utf8.data(), utf8Length, nullptr, 0);
    if (utf16Length == 0) {
        std::cerr << "The filename: " << utf8 << " is not a valid utf8 encoding string" << std::endl;
        return utf16;
    }

    utf16.resize(utf16Length);
    MultiByteToWideChar(CP_UTF8, kFlags, utf8.data(), utf8Length, &utf16[0], utf16Length);
    return utf16;
}
#endif

void Helpers::ThrowError(ErrorType type, const parser::Program *program, const lexer::SourcePosition &pos,
    const std::string_view &msg)
{
    lexer::LineIndex index(program->SourceCode());
    lexer::SourceLocation loc = index.GetLocation(pos);

    throw Error {type, msg, loc.line, loc.col};
}

bool Helpers::IsUseShared(const ir::Statement *statement)
{
    if (!statement->IsExpressionStatement()) {
        return false;
    }

    if (!statement->AsExpressionStatement()->GetExpression()->IsStringLiteral()) {
        return false;
    }

    return statement->AsExpressionStatement()->GetExpression()->AsStringLiteral()->Str().Is(USE_SHARED);
}

const ir::ClassDefinition *Helpers::GetContainingSendableClass(const ir::AstNode *node)
{
    while (node != nullptr) {
        if (node->IsClassDefinition() && node->AsClassDefinition()->IsSendable()) {
            return node->AsClassDefinition();
        }

        node = node->Parent();
    }

    return nullptr;
}

bool Helpers::IsSpecialScopeName(const util::StringView &name)
{
    return name.Find(Helpers::DOT.data()) != std::string::npos ||
           name.Find(Helpers::BACKSLASH.data()) != std::string::npos;
}

bool Helpers::BelongingToRecords(const std::string &name, const std::unordered_set<std::string> &retainRecordSet,
                                 const std::string &delimiter)
{
    size_t pos = name.rfind(delimiter);
    if (pos == std::string::npos) {
        std::cerr << "The input name: " << name << " is illegal, it should contain the delimiter character '" <<
                     delimiter << "'" << std::endl;
        return false;
    }

    auto recordName = name.substr(0, pos);
    return retainRecordSet.find(recordName) != retainRecordSet.end();
}

bool Helpers::IsInnerAnnotationRecordName(const std::string &name)
{
    return name == panda::abc2program::CONCURRENT_MODULE_REQUEST_RECORD_NAME ||
           name == panda::abc2program::SLOT_NUMBER_RECORD_NAME ||
           name == panda::abc2program::EXPECTED_PROPERTY_COUNT_RECORD_NAME;
}

std::string Helpers::RemoveRecordSuffixAnnotationName(const std::string &name)
{
    auto pos = name.rfind(util::DOT_SEPARATOR);
    if (pos == std::string::npos) {
        return name;
    }
    return name.substr(0, pos);
}

void Helpers::RemoveProgramsRedundantData(std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo,
    const std::map<std::string, std::unordered_set<std::string>> &resolvedDepsRelation)
{
    auto progInfoIter = progsInfo.begin();
    while (progInfoIter != progsInfo.end()) {
        auto name = progInfoIter->first;
        auto &program = progInfoIter->second->program;
        /**
         * The record name of a user-defined annotation is the annotation name concatenated with the record name of the
         * file where it is declared. In order to preserve the annotated record, it is necessary to remove the
         * concatenated annotation name from the record name before perform dependency matching.
         */
        if (!util::RecordNotGeneratedFromBytecode(name) &&
            (program.record_table.begin()->second.metadata->GetAccessFlags() & ACC_ANNOTATION) != 0 &&
            !IsInnerAnnotationRecordName(program.record_table.begin()->first)) {
            name = RemoveRecordSuffixAnnotationName(name);
        }
        // remove redundant sourcefiles and bytecodefile data which are not dependant in compilation
        if (resolvedDepsRelation.find(name) == resolvedDepsRelation.end()) {
            progInfoIter = progsInfo.erase(progInfoIter);
            continue;
        }

        progInfoIter++;
    }
}

bool Helpers::IsDefaultApiVersion(int apiVersion, std::string subApiVersion)
{
    return apiVersion < DEFAULT_TARGET_API_VERSION || ((apiVersion == DEFAULT_TARGET_API_VERSION) &&
        (subApiVersion == SUB_API_VERSION_1 || subApiVersion == SUB_API_VERSION_2));
}

bool Helpers::IsSupportLazyImportVersion(int apiVersion, std::string subApiVersion)
{
    return !(apiVersion < LAZY_IMPORT_MIN_SUPPORTED_API_VERSION ||
           ((apiVersion == LAZY_IMPORT_MIN_SUPPORTED_API_VERSION) &&
           (subApiVersion == SUB_API_VERSION_1 || subApiVersion == SUB_API_VERSION_2)));
}

bool Helpers::IsEnableExpectedPropertyCountApiVersion(int apiVersion)
{
    return !(apiVersion < ENABLE_EXPECTED_PROPERTY_COUNT_MIN_SUPPORTED_API_VERSION);
}

bool Helpers::IsSupportAnnotationVersion(int apiVersion)
{
    return !(apiVersion < ANNOTATION_SUPPORTED_API_VERSION);
}

}  // namespace panda::es2panda::util
