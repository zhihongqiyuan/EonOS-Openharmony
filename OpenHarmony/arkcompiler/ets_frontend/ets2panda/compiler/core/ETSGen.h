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

#ifndef ES2PANDA_COMPILER_CORE_ETSGEN_H
#define ES2PANDA_COMPILER_CORE_ETSGEN_H

#include "ir/astNode.h"
#include "varbinder/ETSBinder.h"
#include "compiler/core/codeGen.h"
#include "compiler/core/ETSfunction.h"
#include "compiler/core/targetTypeContext.h"
#include "checker/ETSchecker.h"
#include "util/helpers.h"

namespace ark::es2panda::compiler {

class ETSGen final : public CodeGen {
public:
    explicit ETSGen(ArenaAllocator *allocator, RegSpiller *spiller, public_lib::Context *context,
                    std::tuple<varbinder::FunctionScope *, ProgramElement *, AstCompiler *> toCompile) noexcept;

    [[nodiscard]] const checker::ETSChecker *Checker() const noexcept;
    [[nodiscard]] const varbinder::ETSBinder *VarBinder() const noexcept;
    [[nodiscard]] const checker::Type *ReturnType() const noexcept;
    [[nodiscard]] const checker::ETSObjectType *ContainingObjectType() const noexcept;

    [[nodiscard]] VReg &Acc() noexcept;
    [[nodiscard]] VReg Acc() const noexcept;

    void SetAccumulatorType(const checker::Type *type);
    [[nodiscard]] const checker::Type *GetAccumulatorType() const;
    void CompileAndCheck(const ir::Expression *expr);

    [[nodiscard]] VReg StoreException(const ir::AstNode *node);
    void ApplyConversionAndStoreAccumulator(const ir::AstNode *node, VReg vreg, const checker::Type *targetType);
    void StoreAccumulator(const ir::AstNode *node, VReg vreg);
    void LoadAccumulator(const ir::AstNode *node, VReg vreg);
    [[nodiscard]] IRNode *AllocMov(const ir::AstNode *node, VReg vd, VReg vs) override;
    [[nodiscard]] IRNode *AllocMov(const ir::AstNode *node, OutVReg vd, VReg vs) override;
    void MoveVreg(const ir::AstNode *node, VReg vd, VReg vs);

    [[nodiscard]] checker::Type const *TypeForVar(varbinder::Variable const *var) const noexcept override;

    void LoadVar(const ir::Identifier *node, varbinder::Variable const *var);
    void LoadDynamicModuleVariable(const ir::AstNode *node, varbinder::Variable const *var);
    void LoadDynamicNamespaceVariable(const ir::AstNode *node, varbinder::Variable const *var);
    void StoreVar(const ir::Identifier *node, const varbinder::ConstScopeFindResult &result);

    void LoadStaticProperty(const ir::AstNode *node, const checker::Type *propType, const util::StringView &fullName);
    void StoreStaticProperty(const ir::AstNode *node, const checker::Type *propType, const util::StringView &fullName);

    void StoreStaticOwnProperty(const ir::AstNode *node, const checker::Type *propType, const util::StringView &name);
    [[nodiscard]] util::StringView FormClassPropReference(const checker::ETSObjectType *classType,
                                                          const util::StringView &name);

    void StoreProperty(const ir::AstNode *node, const checker::Type *propType, VReg objReg,
                       const util::StringView &name);
    void LoadProperty(const ir::AstNode *node, const checker::Type *propType, VReg objReg,
                      const util::StringView &fullName);
    void StorePropertyDynamic(const ir::AstNode *node, const checker::Type *propType, VReg objReg,
                              const util::StringView &propName);
    void LoadPropertyDynamic(const ir::AstNode *node, const checker::Type *propType, VReg objReg,
                             const util::StringView &propName);

    void StoreElementDynamic(const ir::AstNode *node, VReg objectReg, VReg index);
    void LoadElementDynamic(const ir::AstNode *node, VReg objectReg);

    void StoreUnionProperty(const ir::AstNode *node, const checker::Type *propType, VReg objReg,
                            const util::StringView &propName);
    void LoadUnionProperty(const ir::AstNode *node, const checker::Type *propType, VReg objReg,
                           const util::StringView &propName);

    void LoadUndefinedDynamic(const ir::AstNode *node, Language lang);

    void LoadThis(const ir::AstNode *node);
    [[nodiscard]] VReg GetThisReg() const;

    const checker::Type *LoadDefaultValue(const ir::AstNode *node, const checker::Type *type);
    void EmitReturnVoid(const ir::AstNode *node);
    void ReturnAcc(const ir::AstNode *node);

    void BranchIfIsInstance(const ir::AstNode *node, VReg srcReg, const checker::Type *target, Label *ifTrue);
    void IsInstance(const ir::AstNode *node, VReg srcReg, checker::Type const *target);
    void IsInstanceDynamic(const ir::BinaryExpression *node, VReg srcReg, VReg tgtReg);
    void EmitFailedTypeCastException(const ir::AstNode *node, VReg src, checker::Type const *target);

    void BinaryLogic(const ir::AstNode *node, lexer::TokenType op, VReg lhs);
    void BinaryArithmLogic(const ir::AstNode *node, lexer::TokenType op, VReg lhs);
    void Binary(const ir::AstNode *node, lexer::TokenType op, VReg lhs);
    void Unary(const ir::AstNode *node, lexer::TokenType op);
    void Update(const ir::AstNode *node, lexer::TokenType op);
    void UpdateBigInt(const ir::Expression *node, VReg arg, lexer::TokenType op);

    bool TryLoadConstantExpression(const ir::Expression *node);
    void Condition(const ir::AstNode *node, lexer::TokenType op, VReg lhs, Label *ifFalse);

    template <typename CondCompare, bool BEFORE_LOGICAL_NOT>
    void ResolveConditionalResultFloat(const ir::AstNode *node, Label *realEndLabel)
    {
        auto type = GetAccumulatorType();
        VReg tmpReg = AllocReg();
        StoreAccumulator(node, tmpReg);
        if (type->IsFloatType()) {
            FloatIsNaN(node);
        } else {
            DoubleIsNaN(node);
        }
        Sa().Emit<Xori>(node, 1);

        BranchIfFalse(node, realEndLabel);
        LoadAccumulator(node, tmpReg);
        VReg zeroReg = AllocReg();

        if (type->IsFloatType()) {
            MoveImmediateToRegister(node, zeroReg, checker::TypeFlag::FLOAT, 0);
            BinaryNumberComparison<Fcmpl, Jeqz>(node, zeroReg, realEndLabel);
        } else {
            MoveImmediateToRegister(node, zeroReg, checker::TypeFlag::DOUBLE, 0);
            BinaryNumberComparison<FcmplWide, Jeqz>(node, zeroReg, realEndLabel);
        }
    }

    template <typename CondCompare, bool BEFORE_LOGICAL_NOT, bool USE_FALSE_LABEL>
    void ResolveConditionalResultNumeric(const ir::AstNode *node, [[maybe_unused]] Label *ifFalse, Label **end)
    {
        auto type = GetAccumulatorType();
        ASSERT(type != nullptr);
        auto realEndLabel = [end, ifFalse, this](bool useFalseLabel) {
            if (useFalseLabel) {
                return ifFalse;
            }
            if ((*end) == nullptr) {
                (*end) = AllocLabel();
            }
            return (*end);
        }(USE_FALSE_LABEL);
        if (type->IsDoubleType() || type->IsFloatType()) {
            ResolveConditionalResultFloat<CondCompare, BEFORE_LOGICAL_NOT>(node, realEndLabel);
        }
        if (type->IsLongType()) {
            VReg zeroReg = AllocReg();
            MoveImmediateToRegister(node, zeroReg, checker::TypeFlag::LONG, 0);
            BinaryNumberComparison<CmpWide, CondCompare>(node, zeroReg, realEndLabel);
        }
        if constexpr (BEFORE_LOGICAL_NOT) {
            Label *zeroPrimitive = AllocLabel();
            BranchIfFalse(node, zeroPrimitive);
            ToBinaryResult(node, zeroPrimitive);
        }
    }

    template <typename CondCompare, bool BEFORE_LOGICAL_NOT>
    void ResolveConditionalResultReference(const ir::AstNode *node)
    {
        auto const testString = [this, node]() {
            LoadStringLength(node);
            if constexpr (BEFORE_LOGICAL_NOT) {
                Label *zeroLenth = AllocLabel();
                BranchIfFalse(node, zeroLenth);
                ToBinaryResult(node, zeroLenth);
            }
        };

        auto type = GetAccumulatorType();
        if (!type->PossiblyETSString()) {
            Sa().Emit<Ldai>(node, 1);
            return;
        }
        if (type->IsETSStringType()) {  // should also be valid for string|null|undefined
            testString();
            return;
        }

        Label *isString = AllocLabel();
        Label *end = AllocLabel();
        compiler::VReg objReg = AllocReg();
        StoreAccumulator(node, objReg);

        Sa().Emit<Isinstance>(node, Checker()->GlobalBuiltinETSStringType()->AssemblerName());
        BranchIfTrue(node, isString);
        Sa().Emit<Ldai>(node, 1);
        Branch(node, end);
        SetLabel(node, isString);
        LoadAccumulator(node, objReg);
        InternalCheckCast(node, Checker()->GlobalBuiltinETSStringType());  // help verifier
        testString();
        SetLabel(node, end);
    }

    template <typename CondCompare, bool BEFORE_LOGICAL_NOT, bool USE_FALSE_LABEL>
    void ResolveConditionalResult(const ir::AstNode *node, [[maybe_unused]] Label *ifFalse)
    {
        auto type = GetAccumulatorType();
        if (type->IsETSBooleanType()) {
            return;
        }
        Label *ifNullish {nullptr};
        Label *end {nullptr};
        if (type->PossiblyETSNullish()) {
            if constexpr (USE_FALSE_LABEL) {
                BranchIfNullish(node, ifFalse);
            } else {
                ifNullish = AllocLabel();
                end = AllocLabel();
                BranchIfNullish(node, ifNullish);
            }
        }
        if (type->DefinitelyETSNullish()) {
            // skip
        } else if (type->IsETSReferenceType()) {
            ResolveConditionalResultReference<CondCompare, BEFORE_LOGICAL_NOT>(node);
        } else {
            ResolveConditionalResultNumeric<CondCompare, BEFORE_LOGICAL_NOT, USE_FALSE_LABEL>(node, ifFalse, &end);
        }
        if (ifNullish != nullptr) {
            Branch(node, end);
            SetLabel(node, ifNullish);
            Sa().Emit<Ldai>(node, 0);
        }
        if (end != nullptr) {
            SetLabel(node, end);
        }
    }

    template <bool BEFORE_LOGICAL_NOT = false, bool FALSE_LABEL_EXISTED = true>
    void ResolveConditionalResultIfFalse(const ir::AstNode *node, Label *ifFalse = nullptr)
    {
        ResolveConditionalResult<Jeqz, BEFORE_LOGICAL_NOT, FALSE_LABEL_EXISTED>(node, ifFalse);
    }

    template <bool BEFORE_LOGICAL_NOT = false, bool FALSE_LABEL_EXISTED = true>
    void ResolveConditionalResultIfTrue(const ir::AstNode *node, Label *ifFalse = nullptr)
    {
        ResolveConditionalResult<Jnez, BEFORE_LOGICAL_NOT, FALSE_LABEL_EXISTED>(node, ifFalse);
    }

    void BranchIfFalse(const ir::AstNode *node, Label *ifFalse)
    {
        Sa().Emit<Jeqz>(node, ifFalse);
    }

    void BranchIfTrue(const ir::AstNode *node, Label *ifTrue)
    {
        Sa().Emit<Jnez>(node, ifTrue);
    }

    void BranchIfNull(const ir::AstNode *node, Label *ifNull)
    {
        Sa().Emit<JeqzObj>(node, ifNull);
    }

    void BranchIfUndefined([[maybe_unused]] const ir::AstNode *node, [[maybe_unused]] Label *ifUndefined)
    {
#ifdef PANDA_WITH_ETS
        Sa().Emit<EtsIsundefined>(node);
        Sa().Emit<Jnez>(node, ifUndefined);
#else
        UNREACHABLE();
#endif  // PANDA_WITH_ETS
    }

    void BranchIfNotUndefined([[maybe_unused]] const ir::AstNode *node, [[maybe_unused]] Label *ifUndefined)
    {
#ifdef PANDA_WITH_ETS
        Sa().Emit<EtsIsundefined>(node);
        Sa().Emit<Jeqz>(node, ifUndefined);
#else
        UNREACHABLE();
#endif  // PANDA_WITH_ETS
    }

    void BranchIfNotNull(const ir::AstNode *node, Label *ifNotNull)
    {
        Sa().Emit<JnezObj>(node, ifNotNull);
    }

    void BranchIfNullish(const ir::AstNode *node, Label *ifNullish);
    void BranchIfNotNullish(const ir::AstNode *node, Label *ifNotNullish);
    void AssumeNonNullish(const ir::AstNode *node, checker::Type const *targetType);

    void JumpTo(const ir::AstNode *node, Label *labelTo)
    {
        Sa().Emit<Jmp>(node, labelTo);
    }

    void EmitThrow(const ir::AstNode *node, VReg err)
    {
        Ra().Emit<Throw>(node, err);
    }

    void EmitNullishException(const ir::AstNode *node);
    void ThrowException(const ir::Expression *expr);
    bool ExtendWithFinalizer(ir::AstNode const *node, const ir::AstNode *originalNode, Label *prevFinnaly = nullptr);

    void Negate(const ir::AstNode *node);
    void LogicalNot(const ir::AstNode *node);

    void LoadAccumulatorByte(const ir::AstNode *node, int8_t number)
    {
        LoadAccumulatorNumber<int8_t>(node, number, checker::TypeFlag::BYTE);
    }

    void LoadAccumulatorShort(const ir::AstNode *node, int16_t number)
    {
        LoadAccumulatorNumber<int16_t>(node, number, checker::TypeFlag::SHORT);
    }

    void LoadAccumulatorInt(const ir::AstNode *node, int32_t number)
    {
        LoadAccumulatorNumber<int32_t>(node, number, checker::TypeFlag::INT);
    }

    void LoadAccumulatorWideInt(const ir::AstNode *node, int64_t number)
    {
        LoadAccumulatorNumber<int64_t>(node, number, checker::TypeFlag::LONG);
    }

    void LoadAccumulatorFloat(const ir::AstNode *node, float number)
    {
        LoadAccumulatorNumber<float>(node, number, checker::TypeFlag::FLOAT);
    }

    void LoadAccumulatorDouble(const ir::AstNode *node, double number)
    {
        LoadAccumulatorNumber<double>(node, number, checker::TypeFlag::DOUBLE);
    }

    void LoadAccumulatorBoolean(const ir::AstNode *node, bool value)
    {
        Sa().Emit<Ldai>(node, value ? 1 : 0);
        SetAccumulatorType(Checker()->GlobalETSBooleanType());
        ApplyConversion(node, Checker()->GlobalETSBooleanType());
    }

    void LoadAccumulatorString(const ir::AstNode *node, util::StringView str)
    {
        Sa().Emit<LdaStr>(node, str);
        SetAccumulatorType(Checker()->GlobalETSStringLiteralType());
    }

    void LoadAccumulatorBigInt(const ir::AstNode *node, util::StringView str)
    {
        Sa().Emit<LdaStr>(node, str);
        SetAccumulatorType(Checker()->GlobalETSBigIntType());
    }

    void LoadAccumulatorNull(const ir::AstNode *node, const checker::Type *type)
    {
        Sa().Emit<LdaNull>(node);
        SetAccumulatorType(type);
    }

    void LoadAccumulatorUndefined([[maybe_unused]] const ir::AstNode *node)
    {
#ifdef PANDA_WITH_ETS
        Sa().Emit<EtsLdundefined>(node);
        SetAccumulatorType(Checker()->GlobalETSUndefinedType());
#else
        UNREACHABLE();
#endif  // PANDA_WITH_ETS
    }

    void LoadAccumulatorChar(const ir::AstNode *node, char16_t value)
    {
        Sa().Emit<Ldai>(node, value);
        SetAccumulatorType(Checker()->GlobalCharType());
        ApplyConversion(node, Checker()->GlobalCharType());
    }

    void LoadAccumulatorDynamicModule(const ir::AstNode *node, const ir::ETSImportDeclaration *import);

    void ApplyBoxingConversion(const ir::AstNode *node);
    void ApplyUnboxingConversion(const ir::AstNode *node);
    void ApplyConversion(const ir::AstNode *node)
    {
        if (targetType_ != nullptr) {
            ApplyConversion(node, targetType_);
        }
    }
    void ApplyConversionCast(const ir::AstNode *node, const checker::Type *targetType);
    void ApplyConversion(const ir::AstNode *node, const checker::Type *targetType);
    void ApplyCast(const ir::AstNode *node, const checker::Type *targetType);
    void ApplyCastToBoxingFlags(const ir::AstNode *node, const ir::BoxingUnboxingFlags targetType);
    void EmitBoxingConversion(ir::BoxingUnboxingFlags boxingFlag, const ir::AstNode *node);
    void EmitBoxingConversion(const ir::AstNode *node);
    void SwapBinaryOpArgs(const ir::AstNode *node, VReg lhs);
    VReg MoveAccToReg(const ir::AstNode *node);

    void LoadArrayLength(const ir::AstNode *node, VReg arrayReg);
    void LoadArrayElement(const ir::AstNode *node, VReg objectReg);
    void StoreArrayElement(const ir::AstNode *node, VReg objectReg, VReg index, const checker::Type *elementType);

    template <typename T>
    void MoveImmediateToRegister(const ir::AstNode *node, VReg reg, const checker::TypeFlag valueType, T const value)
    {
        switch (valueType) {
            case checker::TypeFlag::ETS_BOOLEAN:
                [[fallthrough]];
            case checker::TypeFlag::BYTE: {
                Ra().Emit<Movi>(node, reg, static_cast<checker::ByteType::UType>(value));
                SetVRegType(reg, Checker()->GlobalByteType());
                break;
            }
            case checker::TypeFlag::CHAR: {
                Ra().Emit<Movi>(node, reg, static_cast<checker::CharType::UType>(value));
                SetVRegType(reg, Checker()->GlobalCharType());
                break;
            }
            case checker::TypeFlag::SHORT: {
                Ra().Emit<Movi>(node, reg, static_cast<checker::ShortType::UType>(value));
                SetVRegType(reg, Checker()->GlobalShortType());
                break;
            }
            case checker::TypeFlag::INT: {
                Ra().Emit<Movi>(node, reg, static_cast<checker::IntType::UType>(value));
                SetVRegType(reg, Checker()->GlobalIntType());
                break;
            }
            case checker::TypeFlag::LONG: {
                Ra().Emit<MoviWide>(node, reg, static_cast<checker::LongType::UType>(value));
                SetVRegType(reg, Checker()->GlobalLongType());
                break;
            }
            case checker::TypeFlag::FLOAT: {
                Ra().Emit<Fmovi>(node, reg, static_cast<checker::FloatType::UType>(value));
                SetVRegType(reg, Checker()->GlobalFloatType());
                break;
            }
            case checker::TypeFlag::DOUBLE: {
                Ra().Emit<FmoviWide>(node, reg, static_cast<checker::DoubleType::UType>(value));
                SetVRegType(reg, Checker()->GlobalDoubleType());
                break;
            }
            default: {
                UNREACHABLE();
            }
        }
    }

    template <typename T>
    void IncrementImmediateRegister(const ir::AstNode *node, VReg reg, const checker::TypeFlag valueType, T const value)
    {
        switch (valueType) {
            // NOTE: operand of increment instruction (INCI) is defined in spec as 32-bit integer,
            // but its current implementation actually can work with 64-bit integers as well.
            case checker::TypeFlag::INT: {
                Ra().Emit<Inci>(node, reg, static_cast<checker::IntType::UType>(value));
                break;
            }
            case checker::TypeFlag::CHAR: {
                Ra().Emit<Inci>(node, reg, static_cast<checker::CharType::UType>(value));
                break;
            }
            case checker::TypeFlag::SHORT: {
                Ra().Emit<Inci>(node, reg, static_cast<checker::ShortType::UType>(value));
                break;
            }
            case checker::TypeFlag::ETS_BOOLEAN:
                [[fallthrough]];
            case checker::TypeFlag::BYTE: {
                Ra().Emit<Inci>(node, reg, static_cast<checker::ByteType::UType>(value));
                break;
            }
            default: {
                UNREACHABLE();
            }
        }
    }

    template <typename IntCompare>
    void JumpCompareRegister(const ir::AstNode *node, VReg lhs, Label *ifFalse)
    {
        Ra().Emit<IntCompare>(node, lhs, ifFalse);
    }

    void LoadStringLength(const ir::AstNode *node);
    void LoadStringChar(const ir::AstNode *node, VReg stringObj, VReg charIndex);

    void FloatIsNaN(const ir::AstNode *node);
    void DoubleIsNaN(const ir::AstNode *node);

    void CompileStatements(const ArenaVector<ir::Statement *> &statements);

    // Cast
    void CastToBoolean(const ir::AstNode *node);
    void CastToByte(const ir::AstNode *node);
    void CastToChar(const ir::AstNode *node);
    void CastToShort(const ir::AstNode *node);
    void CastToDouble(const ir::AstNode *node);
    void CastToFloat(const ir::AstNode *node);
    void CastToLong(const ir::AstNode *node);
    void CastToInt(const ir::AstNode *node);
    void CastToString(const ir::AstNode *node);
    void CastToDynamic(const ir::AstNode *node, const checker::ETSDynamicType *type);
    void CastDynamicTo(const ir::AstNode *node, enum checker::TypeFlag typeFlag);
    void CastToReftype(const ir::AstNode *node, const checker::Type *targetType, bool unchecked);
    void CastDynamicToObject(const ir::AstNode *node, const checker::Type *targetType);
    void CastUnionToFunctionType(const ir::AstNode *node, const checker::ETSUnionType *unionType,
                                 checker::Signature *signatureTarget);

    void InternalIsInstance(const ir::AstNode *node, const checker::Type *target);
    void InternalCheckCast(const ir::AstNode *node, const checker::Type *target);
    void CheckedReferenceNarrowing(const ir::AstNode *node, const checker::Type *target);
    void GuardUncheckedType(const ir::AstNode *node, const checker::Type *unchecked, const checker::Type *target);

    // Call, Construct
    void NewArray(const ir::AstNode *node, VReg arr, VReg dim, const checker::Type *arrType);
    void NewObject(const ir::AstNode *node, util::StringView name, VReg athis);
    void BuildString(const ir::Expression *node);
    void CallBigIntUnaryOperator(const ir::Expression *node, VReg arg, util::StringView signature);
    void CallBigIntBinaryOperator(const ir::Expression *node, VReg lhs, VReg rhs, util::StringView signature);
    void CallBigIntBinaryComparison(const ir::Expression *node, VReg lhs, VReg rhs, util::StringView signature);
    void BuildTemplateString(const ir::TemplateLiteral *node);
    void InitObject(const ir::AstNode *node, checker::Signature const *signature,
                    const ArenaVector<ir::Expression *> &arguments)
    {
        CallImpl<InitobjShort, Initobj, InitobjRange>(node, signature, arguments);
    }

    bool IsDevirtualizedSignature(const checker::Signature *signature)
    {
        ASSERT(!signature->HasSignatureFlag(checker::SignatureFlags::STATIC));
        return signature->HasSignatureFlag(checker::SignatureFlags::FINAL | checker::SignatureFlags::PRIVATE |
                                           checker::SignatureFlags::CONSTRUCTOR);
    }

    void CallExact(const ir::AstNode *node, checker::Signature *signature,
                   const ArenaVector<ir::Expression *> &arguments)
    {
        CallImpl<CallShort, Call, CallRange>(node, signature, arguments);
    }

    void CallExact(const ir::AstNode *const node, const checker::Signature *signature, const VReg arg0,
                   const ArenaVector<ir::Expression *> &arguments)
    {
        CallArgStart<CallShort, Call, CallRange>(node, signature, arg0, arguments);
    }

    void CallExact(const ir::AstNode *const node, const util::StringView name)
    {
        Ra().Emit<CallShort, 0>(node, name, dummyReg_, dummyReg_);
    }

    void CallExact(const ir::AstNode *const node, const util::StringView name, const VReg arg0)
    {
        Ra().Emit<CallShort, 1>(node, name, arg0, dummyReg_);
    }

    void CallExact(const ir::AstNode *const node, const util::StringView name, const VReg arg0, const VReg arg1)
    {
        Ra().Emit<CallShort>(node, name, arg0, arg1);
    }

    void CallExact(const ir::AstNode *const node, const util::StringView name, const VReg arg0, const VReg arg1,
                   const VReg arg2)
    {
        Ra().Emit<Call, 3U>(node, name, arg0, arg1, arg2, dummyReg_);
    }

    void CallVirtual(const ir::AstNode *const node, const checker::Signature *signature, const VReg athis,
                     const ArenaVector<ir::Expression *> &arguments)
    {
        ASSERT(!signature->HasSignatureFlag(checker::SignatureFlags::STATIC));
        ASSERT(!signature->Owner()->GetDeclNode()->IsFinal() || signature->IsFinal());
        if (IsDevirtualizedSignature(signature)) {
            CallArgStart<CallShort, Call, CallRange>(node, signature, athis, arguments);
        } else {
            CallArgStart<CallVirtShort, CallVirt, CallVirtRange>(node, signature, athis, arguments);
        }
    }

    void CallVirtual(const ir::AstNode *const node, const checker::Signature *signature, const VReg athis)
    {
        if (IsDevirtualizedSignature(signature)) {
            CallExact(node, signature->InternalName(), athis);
        } else {
            CallVirtual(node, signature->InternalName(), athis);
        }
    }

    void CallVirtual(const ir::AstNode *const node, const checker::Signature *signature, const VReg athis,
                     const VReg arg0)
    {
        if (IsDevirtualizedSignature(signature)) {
            CallExact(node, signature->InternalName(), athis, arg0);
        } else {
            CallVirtual(node, signature->InternalName(), athis, arg0);
        }
    }

    void CallVirtual(const ir::AstNode *const node, const util::StringView name, const VReg athis)
    {
        Ra().Emit<CallVirtShort, 1>(node, name, athis, dummyReg_);
    }

    void CallVirtual(const ir::AstNode *const node, const util::StringView name, const VReg athis, const VReg arg0)
    {
        Ra().Emit<CallVirtShort>(node, name, athis, arg0);
    }

    struct CallDynamicData {
        const ir::AstNode *node = nullptr;
        VReg obj;
        VReg param2;
    };

    void CallDynamic(CallDynamicData data, checker::Signature *signature,
                     const ArenaVector<ir::Expression *> &arguments)
    {
        CallDynamicImpl<CallShort, Call, CallRange>(data, signature, arguments);
    }

    void CallDynamic(CallDynamicData data, VReg param3, checker::Signature *signature,
                     const ArenaVector<ir::Expression *> &arguments)
    {
        CallDynamicImpl<CallShort, Call, CallRange>(data, param3, signature, arguments);
    }

#ifdef PANDA_WITH_ETS
    // The functions below use ETS specific instructions.
    // Compilation of es2panda fails if ETS plugin is disabled
    void LaunchExact(const ir::AstNode *node, checker::Signature *signature,
                     const ArenaVector<ir::Expression *> &arguments)
    {
        CallImpl<EtsLaunchShort, EtsLaunch, EtsLaunchRange>(node, signature, arguments);
    }

    void LaunchVirtual(const ir::AstNode *const node, checker::Signature *const signature, const VReg athis,
                       const ArenaVector<ir::Expression *> &arguments)
    {
        if (IsDevirtualizedSignature(signature)) {
            CallArgStart<EtsLaunchShort, EtsLaunch, EtsLaunchRange>(node, signature, athis, arguments);
        } else {
            CallArgStart<EtsLaunchVirtShort, EtsLaunchVirt, EtsLaunchVirtRange>(node, signature, athis, arguments);
        }
    }
#endif  // PANDA_WITH_ETS

    void CreateBigIntObject(const ir::AstNode *node, VReg arg0,
                            std::string_view signature = Signatures::BUILTIN_BIGINT_CTOR);

    void GetType(const ir::AstNode *node, bool isEtsPrimitive)
    {
        if (isEtsPrimitive) {
            // NOTE: SzD. LoadStaticProperty if ETS stdlib has static TYPE constants otherwise fallback to LdaType
        } else {
            auto classRef = GetAccumulatorType()->AsETSObjectType()->AssemblerName();
            Sa().Emit<LdaType>(node, classRef);
        }
    }

    ~ETSGen() override = default;
    NO_COPY_SEMANTIC(ETSGen);
    NO_MOVE_SEMANTIC(ETSGen);

private:
    const VReg dummyReg_ = VReg::RegStart();

    void EmitUnboxedCall(const ir::AstNode *node, std::string_view signatureFlag, const checker::Type *targetType,
                         const checker::Type *boxedType);

    void LoadConstantObject(const ir::Expression *node, const checker::Type *type);
    void StringBuilderAppend(const ir::AstNode *node, VReg builder);
    void AppendString(const ir::Expression *binExpr, VReg builder);
    void StringBuilder(const ir::Expression *left, const ir::Expression *right, VReg builder);
    util::StringView FormClassPropReference(varbinder::Variable const *var);
    void UnaryMinus(const ir::AstNode *node);
    void UnaryTilde(const ir::AstNode *node);

    util::StringView ToAssemblerType(const es2panda::checker::Type *type) const;
    void TestIsInstanceConstant(const ir::AstNode *node, Label *ifTrue, VReg srcReg, checker::Type const *target);
    void TestIsInstanceConstituent(const ir::AstNode *node, std::tuple<Label *, Label *> label, VReg srcReg,
                                   checker::Type const *target, bool acceptUndefined);
    void CheckedReferenceNarrowingObject(const ir::AstNode *node, const checker::Type *target);

    void HandleLooseNullishEquality(const ir::AstNode *node, VReg lhs, VReg rhs, Label *ifFalse, Label *ifTrue);

    void EmitIsUndefined([[maybe_unused]] const ir::AstNode *node)
    {
#ifdef PANDA_WITH_ETS
        Sa().Emit<EtsIsundefined>(node);
#else
        UNREACHABLE();
#endif  // PANDA_WITH_ETS
    }

    void EmitEtsEquals([[maybe_unused]] const ir::AstNode *node, [[maybe_unused]] const VReg lhs,
                       [[maybe_unused]] const VReg rhs)
    {
#ifdef PANDA_WITH_ETS
        Ra().Emit<EtsEquals>(node, lhs, rhs);
#else
        UNREACHABLE();
#endif  // PANDA_WITH_ETS
    }

    template <typename T>
    void StoreValueIntoArray(const ir::AstNode *const node, const VReg arr, const VReg index)
    {
        Ra().Emit<T>(node, arr, index);
    }

    template <typename LongOp, typename IntOp, typename DoubleOp, typename FloatOp>
    void UpdateOperator(const ir::AstNode *node)
    {
        switch (checker::ETSChecker::ETSType(GetAccumulatorType())) {
            case checker::TypeFlag::LONG: {
                RegScope scope(this);
                VReg reg = AllocReg();
                Ra().Emit<MoviWide>(node, reg, 1LL);
                Ra().Emit<LongOp>(node, reg);
                break;
            }
            case checker::TypeFlag::INT: {
                Sa().Emit<IntOp>(node, 1);
                break;
            }
            case checker::TypeFlag::CHAR: {
                Sa().Emit<IntOp>(node, 1);
                Sa().Emit<I32tou16>(node);
                break;
            }
            case checker::TypeFlag::SHORT: {
                Sa().Emit<IntOp>(node, 1);
                Sa().Emit<I32toi16>(node);
                break;
            }
            case checker::TypeFlag::BYTE: {
                Sa().Emit<IntOp>(node, 1);
                Sa().Emit<I32toi8>(node);
                break;
            }
            case checker::TypeFlag::DOUBLE: {
                RegScope scope(this);
                VReg reg = AllocReg();
                Ra().Emit<FmoviWide>(node, reg, 1.0);
                Ra().Emit<DoubleOp>(node, reg);
                break;
            }
            case checker::TypeFlag::FLOAT: {
                RegScope scope(this);
                VReg reg = AllocReg();
                Ra().Emit<Fmovi>(node, reg, 1.0F);
                Ra().Emit<FloatOp>(node, reg);
                break;
            }
            default: {
                UNREACHABLE();
            }
        }
    }

    template <typename Br>
    void InverseCondition(const ir::AstNode *node, Br const &br, Label *target, bool inverse = true)
    {
        if (!inverse) {
            br(target);
            return;
        }
        Label *loc = AllocLabel();
        br(loc);
        JumpTo(node, target);
        SetLabel(node, loc);
    }

    void RefEqualityLoose(const ir::AstNode *node, VReg lhs, VReg rhs, Label *ifFalse);
    void RefEqualityLooseDynamic(const ir::AstNode *node, VReg lhs, VReg rhs, Label *ifFalse);

    template <typename Compare, typename Cond>
    void BinaryNumberComparison(const ir::AstNode *node, VReg lhs, Label *ifFalse)
    {
        Ra().Emit<Compare>(node, lhs);
        Sa().Emit<Cond>(node, ifFalse);
    }

    template <typename DynCompare>
    void RefEqualityStrictDynamic(const ir::AstNode *node, VReg lhs, Label *ifFalse)
    {
        ASSERT(GetAccumulatorType()->IsETSDynamicType() && GetVRegType(lhs)->IsETSDynamicType());
        RegScope scope(this);
        Ra().Emit<CallShort, 2U>(node, Signatures::BUILTIN_JSRUNTIME_STRICT_EQUAL, lhs, MoveAccToReg(node));
        Ra().Emit<DynCompare>(node, ifFalse);
    }

    template <typename ObjCompare, typename IntCompare, typename CondCompare, typename DynCompare>
    void BinaryEquality(const ir::AstNode *node, VReg lhs, Label *ifFalse)
    {
        BinaryEqualityCondition<ObjCompare, IntCompare, CondCompare>(node, lhs, ifFalse);
        ToBinaryResult(node, ifFalse);
        SetAccumulatorType(Checker()->GlobalETSBooleanType());
    }

    template <typename ObjCompare, typename IntCompare, typename CondCompare>
    void BinaryEqualityCondition(const ir::AstNode *node, VReg lhs, Label *ifFalse)
    {
        if (targetType_->IsETSReferenceType()) {
            RegScope rs(this);
            VReg arg0 = AllocReg();
            StoreAccumulator(node, arg0);
            InverseCondition(
                node, [this, node, lhs, arg0](Label *tgt) { RefEqualityLoose(node, lhs, arg0, tgt); }, ifFalse,
                std::is_same_v<CondCompare, Jeqz>);
            SetAccumulatorType(Checker()->GlobalETSBooleanType());
            return;
        }

        auto typeKind = checker::ETSChecker::TypeKind(targetType_);

        switch (typeKind) {
            case checker::TypeFlag::DOUBLE: {
                BinaryFloatingPointComparison<FcmpgWide, FcmplWide, CondCompare>(node, lhs, ifFalse);
                break;
            }
            case checker::TypeFlag::FLOAT: {
                BinaryFloatingPointComparison<Fcmpg, Fcmpl, CondCompare>(node, lhs, ifFalse);
                break;
            }
            case checker::TypeFlag::LONG: {
                BinaryNumberComparison<CmpWide, CondCompare>(node, lhs, ifFalse);
                break;
            }
            case checker::TypeFlag::ETS_INT_ENUM:
            case checker::TypeFlag::ETS_STRING_ENUM:
            case checker::TypeFlag::ETS_BOOLEAN:
            case checker::TypeFlag::BYTE:
            case checker::TypeFlag::CHAR:
            case checker::TypeFlag::SHORT:
            case checker::TypeFlag::INT: {
                Ra().Emit<IntCompare>(node, lhs, ifFalse);
                break;
            }
            default: {
                UNREACHABLE();
            }
        }

        SetAccumulatorType(Checker()->GlobalETSBooleanType());
    }

    template <typename ObjCompare, typename DynCompare>
    void RefEqualityStrict(const ir::AstNode *node, VReg lhs, Label *ifFalse)
    {
        if (GetAccumulatorType()->IsETSDynamicType() || GetVRegType(lhs)->IsETSDynamicType()) {
            RefEqualityStrictDynamic<DynCompare>(node, lhs, ifFalse);
        } else {
            Ra().Emit<ObjCompare>(node, lhs, ifFalse);
        }

        ToBinaryResult(node, ifFalse);
        SetAccumulatorType(Checker()->GlobalETSBooleanType());
    }

    template <typename IntCompare, typename CondCompare>
    void BinaryRelation(const ir::AstNode *node, VReg lhs, Label *ifFalse)
    {
        BinaryRelationCondition<IntCompare, CondCompare>(node, lhs, ifFalse);
        ToBinaryResult(node, ifFalse);
        SetAccumulatorType(Checker()->GlobalETSBooleanType());
    }

    template <typename IntCompare, typename CondCompare>
    void BinaryRelationCondition(const ir::AstNode *node, VReg lhs, Label *ifFalse)
    {
        auto typeKind = checker::ETSChecker::TypeKind(targetType_);

        switch (typeKind) {
            case checker::TypeFlag::DOUBLE: {
                BinaryFloatingPointComparison<FcmpgWide, FcmplWide, CondCompare>(node, lhs, ifFalse);
                break;
            }
            case checker::TypeFlag::FLOAT: {
                BinaryFloatingPointComparison<Fcmpg, Fcmpl, CondCompare>(node, lhs, ifFalse);
                break;
            }
            case checker::TypeFlag::LONG: {
                BinaryNumberComparison<CmpWide, CondCompare>(node, lhs, ifFalse);
                break;
            }
            case checker::TypeFlag::ETS_BOOLEAN:
            case checker::TypeFlag::BYTE:
            case checker::TypeFlag::SHORT:
            case checker::TypeFlag::CHAR:
            case checker::TypeFlag::INT: {
                Ra().Emit<IntCompare>(node, lhs, ifFalse);
                break;
            }
            default: {
                UNREACHABLE();
            }
        }

        SetAccumulatorType(Checker()->GlobalETSBooleanType());
    }

    template <typename CompareGreater, typename CompareLess, typename CondCompare>
    void BinaryFloatingPointComparison(const ir::AstNode *node, VReg lhs, Label *ifFalse)
    {
        if constexpr (std::is_same_v<CondCompare, Jgez> || std::is_same_v<CondCompare, Jgtz>) {
            BinaryNumberComparison<CompareGreater, CondCompare>(node, lhs, ifFalse);
        } else {
            BinaryNumberComparison<CompareLess, CondCompare>(node, lhs, ifFalse);
        }
    }

    template <typename IntOp, typename LongOp, typename FloatOp, typename DoubleOp>
    void BinaryArithmetic(const ir::AstNode *node, VReg lhs)
    {
        auto typeKind = checker::ETSChecker::TypeKind(targetType_);

        switch (typeKind) {
            case checker::TypeFlag::DOUBLE: {
                Ra().Emit<DoubleOp>(node, lhs);
                SetAccumulatorType(Checker()->GlobalDoubleType());
                break;
            }
            case checker::TypeFlag::FLOAT: {
                Ra().Emit<FloatOp>(node, lhs);
                SetAccumulatorType(Checker()->GlobalFloatType());
                break;
            }
            default: {
                BinaryBitwiseArithmetic<IntOp, LongOp>(node, lhs);
            }
        }
    }

    template <typename IntOp, typename LongOp>
    void BinaryBitwiseArithmetic(const ir::AstNode *node, VReg lhs)
    {
        auto typeKind = checker::ETSChecker::TypeKind(targetType_);

        switch (typeKind) {
            case checker::TypeFlag::LONG: {
                Ra().Emit<LongOp>(node, lhs);
                SetAccumulatorType(Checker()->GlobalLongType());
                break;
            }
            case checker::TypeFlag::BYTE:
            case checker::TypeFlag::SHORT:
            case checker::TypeFlag::INT: {
                Ra().Emit<IntOp>(node, lhs);
                SetAccumulatorType(Checker()->GlobalIntType());
                break;
            }
            case checker::TypeFlag::ETS_BOOLEAN: {
                Ra().Emit<IntOp>(node, lhs);
                SetAccumulatorType(Checker()->GlobalETSBooleanType());
                break;
            }
            case checker::TypeFlag::CHAR: {
                Ra().Emit<IntOp>(node, lhs);
                SetAccumulatorType(Checker()->GlobalCharType());
                break;
            }
            default: {
                UNREACHABLE();
            }
        }
    }
// NOLINTBEGIN(cppcoreguidelines-macro-usage, readability-container-size-empty)
#define COMPILE_ARG(idx)                                                                                       \
    ASSERT((idx) < arguments.size());                                                                          \
    ASSERT((idx) < signature->Params().size() || signature->RestVar() != nullptr);                             \
    auto *param##idx = (idx) < signature->Params().size() ? signature->Params()[(idx)] : signature->RestVar(); \
    auto *paramType##idx = param##idx->TsType();                                                               \
    auto ttctx##idx = TargetTypeContext(this, paramType##idx);                                                 \
    arguments[idx]->Compile(this);                                                                             \
    VReg arg##idx = AllocReg();                                                                                \
    ApplyConversion(arguments[idx], nullptr);                                                                  \
    ApplyConversionAndStoreAccumulator(arguments[idx], arg##idx, paramType##idx)

    template <typename Short, typename General, typename Range>
    void CallArgStart(const ir::AstNode *const node, const checker::Signature *signature, const VReg argStart,
                      const ArenaVector<ir::Expression *> &arguments)
    {
        RegScope rs(this);
        const auto name = signature->InternalName();

        switch (arguments.size()) {
            case 0U: {
                Ra().Emit<Short, 1>(node, name, argStart, dummyReg_);
                break;
            }
            case 1U: {
                COMPILE_ARG(0);
                Ra().Emit<Short>(node, name, argStart, arg0);
                break;
            }
            case 2U: {
                COMPILE_ARG(0);
                COMPILE_ARG(1);
                Ra().Emit<General, 3U>(node, name, argStart, arg0, arg1, dummyReg_);
                break;
            }
            case 3U: {
                COMPILE_ARG(0);
                COMPILE_ARG(1);
                COMPILE_ARG(2);
                Ra().Emit<General>(node, name, argStart, arg0, arg1, arg2);
                break;
            }
            default: {
                for (size_t idx = 0; idx < arguments.size(); idx++) {
                    COMPILE_ARG(idx);
                }

                Rra().Emit<Range>(node, argStart, arguments.size() + 1, name, argStart);
                break;
            }
        }
    }

    template <typename Short, typename General, typename Range>
    void CallImpl(const ir::AstNode *node, checker::Signature const *signature,
                  const ArenaVector<ir::Expression *> &arguments)
    {
        ASSERT(signature != nullptr);
        RegScope rs(this);

        switch (arguments.size()) {
            case 0U: {
                Ra().Emit<Short, 0U>(node, signature->InternalName(), dummyReg_, dummyReg_);
                break;
            }
            case 1U: {
                COMPILE_ARG(0);
                Ra().Emit<Short, 1U>(node, signature->InternalName(), arg0, dummyReg_);
                break;
            }
            case 2U: {
                COMPILE_ARG(0);
                COMPILE_ARG(1);
                Ra().Emit<Short, 2U>(node, signature->InternalName(), arg0, arg1);
                break;
            }
            case 3U: {
                COMPILE_ARG(0);
                COMPILE_ARG(1);
                COMPILE_ARG(2);
                Ra().Emit<General, 3U>(node, signature->InternalName(), arg0, arg1, arg2, dummyReg_);
                break;
            }
            case 4U: {
                COMPILE_ARG(0);
                COMPILE_ARG(1);
                COMPILE_ARG(2);
                COMPILE_ARG(3);
                Ra().Emit<General, 4U>(node, signature->InternalName(), arg0, arg1, arg2, arg3);
                break;
            }
            default: {
                VReg argStart = NextReg();

                for (size_t idx = 0; idx < arguments.size(); idx++) {
                    COMPILE_ARG(idx);
                }

                Rra().Emit<Range>(node, argStart, arguments.size(), signature->InternalName(), argStart);
                break;
            }
        }
    }
#undef COMPILE_ARG

#define COMPILE_ARG(idx, shift)                                                              \
    ASSERT((idx) < arguments.size());                                                        \
    ASSERT((idx) + (shift) < signature->Params().size() || signature->RestVar() != nullptr); \
    auto *paramType##idx = (idx) + (shift) < signature->Params().size()                      \
                               ? signature->Params()[(idx) + (shift)]->TsType()              \
                               : signature->RestVar()->TsType();                             \
    auto ttctx##idx = TargetTypeContext(this, paramType##idx);                               \
    VReg arg##idx = AllocReg();                                                              \
    arguments[idx]->Compile(this);                                                           \
    ApplyConversionAndStoreAccumulator(arguments[idx], arg##idx, paramType##idx)

    template <typename Short, typename General, typename Range>
    void CallDynamicImpl(CallDynamicData data, checker::Signature *signature,
                         const ArenaVector<ir::Expression *> &arguments)
    {
        RegScope rs(this);
        const auto name = signature->InternalName();

        switch (arguments.size()) {
            case 0U: {
                Ra().Emit<Short>(data.node, name, data.obj, data.param2);
                break;
            }
            case 1U: {
                COMPILE_ARG(0, 2U);
                Ra().Emit<General, 3U>(data.node, name, data.obj, data.param2, arg0, dummyReg_);
                break;
            }
            case 2U: {
                COMPILE_ARG(0, 2U);
                COMPILE_ARG(1, 2U);
                Ra().Emit<General>(data.node, name, data.obj, data.param2, arg0, arg1);
                break;
            }
            default: {
                for (size_t idx = 0; idx < arguments.size(); idx++) {
                    COMPILE_ARG(idx, 2U);
                }

                Rra().Emit<Range>(data.node, data.obj, arguments.size() + 2U, name, data.obj);
                break;
            }
        }
    }

    template <typename Short, typename General, typename Range>
    void CallDynamicImpl(CallDynamicData data, VReg param3, checker::Signature *signature,
                         const ArenaVector<ir::Expression *> &arguments)
    {
        RegScope rs(this);
        const auto name = signature->InternalName();

        switch (arguments.size()) {
            case 0U: {
                Ra().Emit<General, 3U>(data.node, name, data.obj, data.param2, param3, dummyReg_);
                break;
            }
            case 1U: {
                COMPILE_ARG(0, 3U);
                Ra().Emit<General>(data.node, name, data.obj, data.param2, param3, arg0);
                break;
            }
            default: {
                for (size_t idx = 0; idx < arguments.size(); idx++) {
                    COMPILE_ARG(idx, 3U);
                }

                Rra().Emit<Range>(data.node, data.obj, arguments.size() + 3U, name, data.obj);
                break;
            }
        }
    }

#undef COMPILE_ARG
    // NOLINTEND(cppcoreguidelines-macro-usage, readability-container-size-empty)

    void ToBinaryResult(const ir::AstNode *node, Label *ifFalse);

    template <typename T>
    void LoadAccumulatorNumber(const ir::AstNode *node, T number, checker::TypeFlag targetType);
    template <typename T>
    void SetAccumulatorTargetType(const ir::AstNode *node, checker::TypeFlag typeKind, T number);
    void InitializeContainingClass();

    util::StringView FormDynamicModulePropReference(const varbinder::Variable *var);
    util::StringView FormDynamicModulePropReference(const ir::ETSImportDeclaration *import);

    friend class TargetTypeContext;

    VReg acc_ {};
    const checker::Type *targetType_ {};
    const checker::ETSObjectType *containingObjectType_ {};
};

template <typename T>
void ETSGen::SetAccumulatorTargetType(const ir::AstNode *node, checker::TypeFlag typeKind, T number)
{
    switch (typeKind) {
        case checker::TypeFlag::ETS_BOOLEAN:
        case checker::TypeFlag::BYTE: {
            Sa().Emit<Ldai>(node, static_cast<checker::ByteType::UType>(number));
            SetAccumulatorType(Checker()->GlobalByteType());
            break;
        }
        case checker::TypeFlag::CHAR: {
            Sa().Emit<Ldai>(node, static_cast<checker::CharType::UType>(number));
            SetAccumulatorType(Checker()->GlobalCharType());
            break;
        }
        case checker::TypeFlag::SHORT: {
            Sa().Emit<Ldai>(node, static_cast<checker::ShortType::UType>(number));
            SetAccumulatorType(Checker()->GlobalShortType());
            break;
        }
        case checker::TypeFlag::INT: {
            Sa().Emit<Ldai>(node, static_cast<checker::IntType::UType>(number));
            SetAccumulatorType(Checker()->GlobalIntType());
            break;
        }
        case checker::TypeFlag::LONG: {
            Sa().Emit<LdaiWide>(node, static_cast<checker::LongType::UType>(number));
            SetAccumulatorType(Checker()->GlobalLongType());
            break;
        }
        case checker::TypeFlag::FLOAT: {
            Sa().Emit<Fldai>(node, static_cast<checker::FloatType::UType>(number));
            SetAccumulatorType(Checker()->GlobalFloatType());
            break;
        }
        case checker::TypeFlag::DOUBLE: {
            Sa().Emit<FldaiWide>(node, static_cast<checker::DoubleType::UType>(number));
            SetAccumulatorType(Checker()->GlobalDoubleType());
            break;
        }
        case checker::TypeFlag::ETS_STRING_ENUM:
            [[fallthrough]];
        case checker::TypeFlag::ETS_INT_ENUM: {
            Sa().Emit<Ldai>(node, static_cast<checker::ETSEnumType::UType>(number));
            SetAccumulatorType(Checker()->GlobalIntType());
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

template <typename T>
void ETSGen::LoadAccumulatorNumber(const ir::AstNode *node, T number, checker::TypeFlag targetType)
{
    auto typeKind = targetType_ && (!targetType_->IsETSObjectType() && !targetType_->IsETSUnionType() &&
                                    !targetType_->IsETSArrayType())
                        ? checker::ETSChecker::TypeKind(targetType_)
                        : targetType;

    SetAccumulatorTargetType(node, typeKind, number);

    if (targetType_ && (targetType_->IsETSObjectType() || targetType_->IsETSUnionType())) {
        ApplyConversion(node, targetType_);
    }
}

}  // namespace ark::es2panda::compiler

#endif
