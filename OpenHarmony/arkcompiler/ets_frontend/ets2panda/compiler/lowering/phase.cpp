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

#include "phase.h"
#include "checker/checker.h"
#include "ets/ambientLowering.h"
#include "lexer/token/sourceLocation.h"
#include "compiler/lowering/resolveIdentifiers.h"
#include "compiler/lowering/checkerPhase.h"
#include "compiler/lowering/ets/stringConstantsLowering.h"
#include "compiler/lowering/ets/constStringToCharLowering.h"
#include "compiler/lowering/ets/defaultParameterLowering.h"
#include "compiler/lowering/ets/expandBrackets.h"
#include "compiler/lowering/ets/recordLowering.h"
#include "compiler/lowering/ets/topLevelStmts/topLevelStmts.h"
#include "compiler/lowering/ets/expressionLambdaLowering.h"
#include "compiler/lowering/ets/boxingForLocals.h"
#include "compiler/lowering/ets/capturedVariables.h"
#include "compiler/lowering/ets/lambdaLowering.h"
#include "compiler/lowering/ets/spreadLowering.h"
#include "compiler/lowering/ets/interfacePropertyDeclarations.h"
#include "compiler/lowering/ets/objectIndexAccess.h"
#include "compiler/lowering/ets/objectIterator.h"
#include "compiler/lowering/ets/localClassLowering.h"
#include "compiler/lowering/ets/opAssignment.h"
#include "compiler/lowering/ets/objectLiteralLowering.h"
#include "compiler/lowering/ets/interfaceObjectLiteralLowering.h"
#include "compiler/lowering/ets/optionalLowering.h"
#include "compiler/lowering/ets/packageImplicitImport.h"
#include "compiler/lowering/ets/partialExportClassGen.h"
#include "compiler/lowering/ets/promiseVoid.h"
#include "compiler/lowering/ets/stringComparison.h"
#include "compiler/lowering/ets/structLowering.h"
#include "compiler/lowering/ets/tupleLowering.h"
#include "compiler/lowering/ets/bigintLowering.h"
#include "compiler/lowering/ets/unionLowering.h"
#include "compiler/lowering/ets/stringConstructorLowering.h"
#include "compiler/lowering/ets/enumLowering.h"
#include "compiler/lowering/ets/enumPostCheckLowering.h"
#include "compiler/lowering/ets/genericBridgesLowering.h"
#include "compiler/lowering/plugin_phase.h"
#include "compiler/lowering/scopesInit/scopesInitPhase.h"
#include "public/es2panda_lib.h"

namespace ark::es2panda::compiler {

static CheckerPhase g_checkerPhase;
static ResolveIdentifiers g_resolveIdentifiers {};
static AmbientLowering g_ambientLowering;
static BigIntLowering g_bigintLowering;
static StringConstructorLowering g_stringConstructorLowering;
static ConstStringToCharLowering g_constStringToCharLowering;
static InterfacePropertyDeclarationsPhase g_interfacePropDeclPhase;
static EnumLoweringPhase g_enumLoweringPhase;
static EnumPostCheckLoweringPhase g_enumPostCheckLoweringPhase;
static SpreadConstructionPhase g_spreadConstructionPhase;
static ExpressionLambdaConstructionPhase g_expressionLambdaConstructionPhase;
static OpAssignmentLowering g_opAssignmentLowering;
static BoxingForLocals g_boxingForLocals;
static CapturedVariables g_capturedVariables {};
static LambdaConversionPhase g_lambdaConversionPhase;
static ObjectIndexLowering g_objectIndexLowering;
static ObjectIteratorLowering g_objectIteratorLowering;
static ObjectLiteralLowering g_objectLiteralLowering;
static InterfaceObjectLiteralLowering g_interfaceObjectLiteralLowering;
static TupleLowering g_tupleLowering;  // Can be only applied after checking phase, and OP_ASSIGNMENT_LOWERING phase
static UnionLowering g_unionLowering;
static OptionalLowering g_optionalLowering;
static ExpandBracketsPhase g_expandBracketsPhase;
static PromiseVoidInferencePhase g_promiseVoidInferencePhase;
static RecordLowering g_recordLowering;
static StructLowering g_structLowering;
static DefaultParameterLowering g_defaultParameterLowering;
static TopLevelStatements g_topLevelStatements;
static LocalClassConstructionPhase g_localClassLowering;
static StringComparisonLowering g_stringComparisonLowering;
static StringConstantsLowering g_stringConstantsLowering;
static PartialExportClassGen g_partialExportClassGen;
static PackageImplicitImport g_packageImplicitImport;
static GenericBridgesPhase g_genericBridgesLowering;
static PluginPhase g_pluginsAfterParse {"plugins-after-parse", ES2PANDA_STATE_PARSED, &util::Plugin::AfterParse};
static PluginPhase g_pluginsAfterCheck {"plugins-after-check", ES2PANDA_STATE_CHECKED, &util::Plugin::AfterCheck};
static PluginPhase g_pluginsAfterLowerings {"plugins-after-lowering", ES2PANDA_STATE_LOWERED,
                                            &util::Plugin::AfterLowerings};
// NOLINTBEGIN(fuchsia-statically-constructed-objects)
static InitScopesPhaseETS g_initScopesPhaseEts;
static InitScopesPhaseAS g_initScopesPhaseAs;
static InitScopesPhaseTs g_initScopesPhaseTs;
static InitScopesPhaseJs g_initScopesPhaseJs;
// NOLINTEND(fuchsia-statically-constructed-objects)

enum class ActionAfterCheckPhase {
    NONE,
    EXIT,
};

static ActionAfterCheckPhase CheckOptionsBeforePhase(const CompilerOptions &options, const parser::Program *program,
                                                     const std::string &name);
static ActionAfterCheckPhase CheckOptionsAfterPhase(const CompilerOptions &options, const parser::Program *program,
                                                    const std::string &name);

std::vector<Phase *> GetETSPhaseList()
{
    // clang-format off
    return {
        &g_pluginsAfterParse,
        &g_stringConstantsLowering,
        &g_packageImplicitImport,
        &g_topLevelStatements,
        &g_defaultParameterLowering,
        &g_ambientLowering,
        &g_initScopesPhaseEts,
        &g_optionalLowering,
        &g_promiseVoidInferencePhase,
        &g_structLowering,
        &g_expressionLambdaConstructionPhase,
        &g_interfacePropDeclPhase,
        &g_enumLoweringPhase,
        &g_resolveIdentifiers,
        &g_capturedVariables,
        &g_checkerPhase,        // please DO NOT change order of these two phases: checkerPhase and pluginsAfterCheck
        &g_pluginsAfterCheck,   // pluginsAfterCheck has to go right after checkerPhase, nothing should be between them
        &g_enumPostCheckLoweringPhase,
        &g_spreadConstructionPhase,
        &g_bigintLowering,
        &g_opAssignmentLowering,
        &g_constStringToCharLowering,
        &g_boxingForLocals,
        &g_recordLowering,
        &g_lambdaConversionPhase,
        &g_objectIndexLowering,
        &g_objectIteratorLowering,
        &g_tupleLowering,
        &g_unionLowering,
        &g_expandBracketsPhase,
        &g_localClassLowering,
        &g_interfaceObjectLiteralLowering,
        &g_objectLiteralLowering,
        &g_stringConstructorLowering,
        &g_stringComparisonLowering,
        &g_partialExportClassGen,
        &g_genericBridgesLowering,
        &g_pluginsAfterLowerings,  // pluginsAfterLowerings has to come at the very end, nothing should go after it
    };
    // clang-format on
}

std::vector<Phase *> GetASPhaseList()
{
    return {
        &g_initScopesPhaseAs,
        &g_checkerPhase,
    };
}

std::vector<Phase *> GetTSPhaseList()
{
    return {
        &g_initScopesPhaseTs,
        &g_checkerPhase,
    };
}

std::vector<Phase *> GetJSPhaseList()
{
    return {
        &g_initScopesPhaseJs,
        &g_checkerPhase,
    };
}

std::vector<Phase *> GetPhaseList(ScriptExtension ext)
{
    switch (ext) {
        case ScriptExtension::ETS:
            return GetETSPhaseList();
        case ScriptExtension::AS:
            return GetASPhaseList();
        case ScriptExtension::TS:
            return GetTSPhaseList();
        case ScriptExtension::JS:
            return GetJSPhaseList();
        default:
            UNREACHABLE();
    }
}

bool Phase::Apply(public_lib::Context *ctx, parser::Program *program)
{
    const auto &options = ctx->config->options->CompilerOptions();
    const auto name = std::string {Name()};
    if (options.skipPhases.count(name) > 0) {
        return true;
    }

    if (CheckOptionsBeforePhase(options, program, name) == ActionAfterCheckPhase::EXIT) {
        return false;
    }

#ifndef NDEBUG
    if (!Precondition(ctx, program)) {
        ctx->checker->LogTypeError({"Precondition check failed for ", util::StringView {Name()}},
                                   lexer::SourcePosition {});
        return false;
    }
#endif

    if (!Perform(ctx, program)) {
        return false;
    }

    if (CheckOptionsAfterPhase(options, program, name) == ActionAfterCheckPhase::EXIT) {
        return false;
    }

#ifndef NDEBUG
    if (!Postcondition(ctx, program)) {
        ctx->checker->LogTypeError({"Postcondition check failed for ", util::StringView {Name()}},
                                   lexer::SourcePosition {});
        return false;
    }
#endif

    return !ctx->checker->ErrorLogger()->IsAnyError() && !ctx->parser->ErrorLogger()->IsAnyError();
}

static ActionAfterCheckPhase CheckOptionsBeforePhase(const CompilerOptions &options, const parser::Program *program,
                                                     const std::string &name)
{
    if (options.dumpBeforePhases.count(name) > 0) {
        std::cout << "Before phase " << name << ":" << std::endl;
        std::cout << program->Dump() << std::endl;
    }

    if (options.dumpEtsSrcBeforePhases.count(name) > 0) {
        std::cout << "Before phase " << name << " ets source"
                  << ":" << std::endl;
        std::cout << program->Ast()->DumpEtsSrc() << std::endl;
    }

    if (options.exitBeforePhase == name) {
        return ActionAfterCheckPhase::EXIT;
    }

    return ActionAfterCheckPhase::NONE;
}

static ActionAfterCheckPhase CheckOptionsAfterPhase(const CompilerOptions &options, const parser::Program *program,
                                                    const std::string &name)
{
    if (options.dumpAfterPhases.count(name) > 0) {
        std::cout << "After phase " << name << ":" << std::endl;
        std::cout << program->Dump() << std::endl;
    }

    if (options.dumpEtsSrcAfterPhases.count(name) > 0) {
        std::cout << "After phase " << name << " ets source"
                  << ":" << std::endl;
        std::cout << program->Ast()->DumpEtsSrc() << std::endl;
    }

    if (options.exitAfterPhase == name) {
        return ActionAfterCheckPhase::EXIT;
    }

    return ActionAfterCheckPhase::NONE;
}

}  // namespace ark::es2panda::compiler
