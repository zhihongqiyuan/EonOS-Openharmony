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

#ifndef ES2PANDA_COMPILER_CORE_PANDAGEN_H
#define ES2PANDA_COMPILER_CORE_PANDAGEN_H

#include "compiler/core/codeGen.h"
#include "compiler/base/optionalChain.h"
#include "compiler/core/envScope.h"
#include "compiler/core/function.h"

#include "compiler/core/regAllocator.h"
#include "compiler/core/regScope.h"
#include "ir/irnode.h"
#include "lexer/token/tokenType.h"
#include "macros.h"

#include <unordered_map>

namespace ark::es2panda::varbinder {
class FunctionScope;
class Scope;
}  // namespace ark::es2panda::varbinder

namespace ark::es2panda::ir {
class AstNode;
class ScriptFunction;
class Statement;
class Expression;
class Identifier;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::compiler {
class FunctionBuilder;
class DynamicContext;

class PandaGen final : public CodeGen {
public:
    explicit PandaGen(ArenaAllocator *allocator, RegSpiller *spiller, public_lib::Context *context,
                      std::tuple<varbinder::FunctionScope *, ProgramElement *, AstCompiler *> toCompile);

    ~PandaGen() override = default;
    NO_COPY_SEMANTIC(PandaGen);
    NO_MOVE_SEMANTIC(PandaGen);

    [[nodiscard]] FunctionBuilder *FuncBuilder() const noexcept;
    [[nodiscard]] EnvScope *GetEnvScope() const noexcept;

    void OptionalChainCheck(bool optional, VReg obj) const;

    [[nodiscard]] VReg LexEnv() const noexcept;

    bool FunctionHasFinalizer() const;
    void FunctionInit(CatchTable *catchTable);
    void FunctionEnter();
    void FunctionExit();

    void StoreAccumulator(const ir::AstNode *node, VReg vreg);
    void LoadAccumulator(const ir::AstNode *node, VReg reg);

    [[nodiscard]] IRNode *AllocMov(const ir::AstNode *node, VReg vd, VReg vs) override;
    [[nodiscard]] IRNode *AllocMov(const ir::AstNode *node, OutVReg vd, VReg vs) override;
    void MoveVreg(const ir::AstNode *node, VReg vd, VReg vs);

    void LoadAccumulatorDouble(const ir::AstNode *node, double num);
    void LoadAccumulatorInt(const ir::AstNode *node, size_t num);

    void LoadConst(const ir::AstNode *node, Constant id);
    void StoreConst(const ir::AstNode *node, VReg reg, Constant id);

    void GetFunctionObject(const ir::AstNode *node);
    void GetNewTarget(const ir::AstNode *node);
    void GetThis(const ir::AstNode *node);
    void SetThis(const ir::AstNode *node);
    void LoadVar(const ir::Identifier *node, const varbinder::ConstScopeFindResult &result);
    void StoreVar(const ir::AstNode *node, const varbinder::ConstScopeFindResult &result, bool isDeclaration);

    void LoadAccFromArgs(const ir::AstNode *node);
    void LoadObjProperty(const ir::AstNode *node, const Operand &prop);

    void LoadObjByName(const ir::AstNode *node, const util::StringView &prop);

    void StoreObjProperty(const ir::AstNode *node, VReg obj, const Operand &prop);
    void StoreOwnProperty(const ir::AstNode *node, VReg obj, const Operand &prop);
    void DeleteObjProperty(const ir::AstNode *node, VReg obj, VReg prop);
    void LoadGlobalVar(const ir::AstNode *node, const util::StringView &name);
    void StoreGlobalVar(const ir::AstNode *node, const util::StringView &name);
    void StoreGlobalLet(const ir::AstNode *node, const util::StringView &name);

    void TryLoadGlobalByName(const ir::AstNode *node, const util::StringView &name);
    void TryStoreGlobalByName(const ir::AstNode *node, const util::StringView &name);

    void LoadAccFromLexEnv(const ir::AstNode *node, const varbinder::ConstScopeFindResult &result);
    void StoreAccToLexEnv(const ir::AstNode *node, const varbinder::ConstScopeFindResult &result, bool isDeclaration);

    void LoadAccumulatorBigInt(const ir::AstNode *node, const util::StringView &bigInt);

    void Condition(const ir::AstNode *node, lexer::TokenType op, VReg lhs, class Label *ifFalse);
    void Unary(const ir::AstNode *node, lexer::TokenType op, VReg operand);
    void Binary(const ir::AstNode *node, lexer::TokenType op, VReg lhs);

    void BranchIfUndefined(const ir::AstNode *node, class Label *target);
    void BranchIfNotUndefined(const ir::AstNode *node, class Label *target);
    void BranchIfHole(const ir::AstNode *node, class Label *target);

    void BranchIfTrue(const ir::AstNode *node, class Label *target);
    void BranchIfNotTrue(const ir::AstNode *node, class Label *target);
    void BranchIfFalse(const ir::AstNode *node, class Label *target);
    void BranchIfCoercible(const ir::AstNode *node, class Label *target);

    void EmitThrow(const ir::AstNode *node);
    void EmitRethrow(const ir::AstNode *node);
    void EmitReturn(const ir::AstNode *node);
    void EmitReturnUndefined(const ir::AstNode *node);
    void ValidateClassDirectReturn(const ir::AstNode *node);
    void DirectReturn(const ir::AstNode *node);
    void ImplicitReturn(const ir::AstNode *node);
    void EmitAwait(const ir::AstNode *node);

    static constexpr auto MAX_RANGE_CALL_ARG = 128;
    void CallTagged(const ir::AstNode *node, VReg callee, VReg thisReg, const ArenaVector<ir::Expression *> &arguments);
    void Call(const ir::AstNode *node, VReg callee, VReg thisReg, const ArenaVector<ir::Expression *> &arguments);
    void Call0This(const ir::AstNode *node, VReg callee, VReg thisReg);
    void Call1This(const ir::AstNode *node, VReg callee, VReg thisReg, VReg arg0);

    void CallSpread(const ir::AstNode *node, VReg func, VReg thisReg, VReg args);
    void SuperCall(const ir::AstNode *node, VReg startReg, size_t argCount);
    void SuperCallSpread(const ir::AstNode *node, VReg vs);

    void LoadHomeObject(const ir::AstNode *node);
    void NewObject(const ir::AstNode *node, VReg startReg, size_t argCount);
    void DefineMethod(const ir::AstNode *node, const util::StringView &name);
    void DefineFunction(const ir::AstNode *node, const ir::ScriptFunction *realNode, const util::StringView &name);

    void TypeOf(const ir::AstNode *node);
    void NewObjSpread(const ir::AstNode *node, VReg obj, VReg target);
    void GetUnmappedArgs(const ir::AstNode *node);

    void Negate(const ir::AstNode *node);
    void ToBoolean(const ir::AstNode *node);
    void ToNumber(const ir::AstNode *node, VReg arg);

    void CreateGeneratorObj(const ir::AstNode *node, VReg funcObj);
    void ResumeGenerator(const ir::AstNode *node, VReg genObj);
    void GetResumeMode(const ir::AstNode *node, VReg genObj);

    void AsyncFunctionEnter(const ir::AstNode *node);
    void AsyncFunctionAwait(const ir::AstNode *node, VReg asyncFuncObj);
    void AsyncFunctionResolve(const ir::AstNode *node, VReg asyncFuncObj);
    void AsyncFunctionReject(const ir::AstNode *node, VReg asyncFuncObj);

    void GetMethod(const ir::AstNode *node, VReg obj, const util::StringView &name);
    void GeneratorYield(const ir::AstNode *node, VReg genObj);
    void GeneratorComplete(const ir::AstNode *node, VReg genObj);
    void CreateAsyncGeneratorObj(const ir::AstNode *node, VReg funcObj);
    void CreateIterResultObject(const ir::AstNode *node, bool done);
    void SuspendGenerator(const ir::AstNode *node, VReg genObj);
    void SuspendAsyncGenerator(const ir::AstNode *node, VReg asyncGenObj);

    void AsyncGeneratorResolve(const ir::AstNode *node, VReg asyncGenObj);
    void AsyncGeneratorReject(const ir::AstNode *node, VReg asyncGenObj);

    void GetTemplateObject(const ir::AstNode *node, VReg value);
    void CopyRestArgs(const ir::AstNode *node, uint32_t index);

    void GetPropIterator(const ir::AstNode *node);
    void GetNextPropName(const ir::AstNode *node, VReg iter);
    void CreateEmptyObject(const ir::AstNode *node);
    void CreateObjectWithBuffer(const ir::AstNode *node, uint32_t idx);
    void CreateObjectHavingMethod(const ir::AstNode *node, uint32_t idx);
    void SetObjectWithProto(const ir::AstNode *node, VReg proto, VReg obj);
    void CopyDataProperties(const ir::AstNode *node, VReg dst, VReg src);
    void DefineGetterSetterByValue(const ir::AstNode *node, std::tuple<VReg, VReg, VReg, VReg> registers, bool setName);
    void CreateEmptyArray(const ir::AstNode *node);
    size_t HandleArrayLiterals(const ir::AstNode *node, const ArenaVector<ir::Expression *> &elements);
    void HandleArraySpread(const ir::AstNode *node, const ArenaVector<ir::Expression *> &elements, VReg obj);
    void CreateArray(const ir::AstNode *node, const ArenaVector<ir::Expression *> &elements, VReg obj);
    void CreateArrayWithBuffer(const ir::AstNode *node, uint32_t idx);
    void StoreArraySpread(const ir::AstNode *node, VReg array, VReg index);

    void CreateRegExpWithLiteral(const ir::AstNode *node, const util::StringView &pattern, uint8_t flags);

    void ThrowIfNotObject(const ir::AstNode *node);
    void ThrowThrowNotExist(const ir::AstNode *node);
    void GetIterator(const ir::AstNode *node);
    void GetAsyncIterator(const ir::AstNode *node);

    void CreateObjectWithExcludedKeys(const ir::AstNode *node, VReg obj, VReg argStart, size_t argCount);
    void ThrowObjectNonCoercible(const ir::AstNode *node);
    void CloseIterator(const ir::AstNode *node, VReg iter);
    void SetClassComputedFields(const ir::AstNode *node, VReg classReg, VReg computedInstanceFieldArray);
    void DefineClassWithBuffer(const ir::AstNode *node, const util::StringView &ctorId, uint32_t litIdx, VReg lexenv,
                               VReg base);
    void LoadClassComputedInstanceFields(const ir::AstNode *node, VReg ctor);
    void DefineClassPrivateFields(const ir::AstNode *node, uint32_t privateBufIdx);
    void ClassFieldAdd(const ir::AstNode *node, VReg obj, VReg prop);
    void ClassPrivateFieldAdd(const ir::AstNode *node, VReg ctor, VReg obj, const util::StringView &prop);
    void ClassPrivateMethodOrAccessorAdd(const ir::AstNode *node, VReg ctor, VReg obj);
    void ClassPrivateFieldGet(const ir::AstNode *node, VReg ctor, VReg obj, const util::StringView &prop);
    void ClassPrivateFieldSet(const ir::AstNode *node, VReg ctor, VReg obj, const util::StringView &prop);
    void ClassPrivateFieldIn(const ir::AstNode *node, VReg ctor, const util::StringView &prop);

    void ImportModule(const ir::AstNode *node, const util::StringView &name);
    void LoadModuleVariable(const ir::AstNode *node, VReg module, const util::StringView &name);
    void StoreModuleVar(const ir::AstNode *node, const util::StringView &name);
    void CopyModule(const ir::AstNode *node, VReg module);

    void StSuperByName(const ir::AstNode *node, VReg obj, const util::StringView &key);
    void LdSuperByName(const ir::AstNode *node, const util::StringView &key);
    void StSuperByValue(const ir::AstNode *node, VReg obj, VReg prop);
    void LdSuperByValue(const ir::AstNode *node, VReg obj);
    void StoreSuperProperty(const ir::AstNode *node, VReg obj, const Operand &prop);
    void LoadSuperProperty(const ir::AstNode *node, const Operand &prop);

    void LdLexEnv(const ir::AstNode *node);
    void PopLexEnv(const ir::AstNode *node);
    void CopyLexEnv(const ir::AstNode *node);
    void NewLexEnv(const ir::AstNode *node, uint32_t num);
    void LoadLexicalVar(const ir::AstNode *node, uint32_t level, uint32_t slot);
    void LoadLexical(const ir::AstNode *node, const util::StringView &name, uint32_t level, uint32_t slot);
    void StoreLexicalVar(const ir::AstNode *node, uint32_t level, uint32_t slot);
    void StoreLexical(const ir::AstNode *node, const util::StringView &name, uint32_t level, uint32_t slot);

    void ThrowIfSuperNotCorrectCall(const ir::AstNode *node, int64_t num);
    void ThrowTdz(const ir::AstNode *node, const util::StringView &name);
    void ThrowConstAssignment(const ir::AstNode *node, const util::StringView &name);

    void LoadObjByIndex(const ir::AstNode *node, int64_t index);
    void LoadObjByValue(const ir::AstNode *node, VReg obj);

    void StoreObjByName(const ir::AstNode *node, VReg obj, const util::StringView &prop);
    void StoreObjByIndex(const ir::AstNode *node, VReg obj, int64_t index);
    void StoreObjByValue(const ir::AstNode *node, VReg obj, VReg prop);

    void StOwnByName(const ir::AstNode *node, VReg obj, const util::StringView &prop);
    void StOwnByValue(const ir::AstNode *node, VReg obj, VReg prop);
    void StOwnByIndex(const ir::AstNode *node, VReg obj, int64_t index);

    static Operand ToNamedPropertyKey(const ir::Expression *prop, bool isComputed);
    void LoadPropertyKeyAcc(const ir::Expression *prop, bool isComputed);
    Operand ToPropertyKey(const ir::Expression *prop, bool isComputed, bool isSuperExpression = false);
    Operand ToOwnPropertyKey(const ir::Expression *prop, bool isComputed);
    VReg LoadPropertyKey(const ir::Expression *prop, bool isComputed);

    void LoadEvalVariable(const ir::AstNode *node, const util::StringView &name);
    void StoreEvalVariable(const ir::AstNode *node, const util::StringView &name);
    void DirectEval(const ir::AstNode *node, uint32_t parserStatus);
    void LoadLexicalContext(const ir::AstNode *node);
    bool IsDirectEval() const;
    bool IsEval() const;

    const checker::Type *GetVRegType(VReg vreg) const override;

private:
    void LoadEvalBindings(const ir::AstNode *node);
    void Call0Args(const ir::AstNode *n, VReg c, VReg thisR, bool hasThis);
    void Call1Arg(const ir::AstNode *n, VReg c, VReg thisR, const ArenaVector<ir::Expression *> &args, bool hasThis);
    void Call2Args(const ir::AstNode *n, VReg c, VReg thisR, const ArenaVector<ir::Expression *> &args, bool hasThis);
    void Call3Args(const ir::AstNode *n, VReg c, VReg thisR, const ArenaVector<ir::Expression *> &args, bool hasThis);
    bool CallArgsTagged(const ir::AstNode *node, VReg callee, VReg thisReg,
                        const ArenaVector<ir::Expression *> &arguments, bool hasThis);
    FunctionBuilder *builder_ {};
    EnvScope *envScope_ {};
    OptionalChain *optionalChain_ {};

    friend class EnvScope;
    friend class LoopEnvScope;
    friend class DynamicContext;
    friend class OptionalChain;
};
}  // namespace ark::es2panda::compiler

#endif
