/**
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

#include <gtest/gtest.h>
#include <algorithm>
#include "macros.h"

#include "assembler/assembly-program.h"
#include "ir/astDump.h"
#include "ir/expressions/literals/stringLiteral.h"

#include "bytecode_optimizer/bytecodeopt_options.h"
#include "compiler/compiler_logger.h"
#include "mem/arena_allocator.h"
#include "mem/pool_manager.h"
#include "es2panda.h"
#include "util/arktsconfig.h"
#include "util/generateBin.h"
#include "util/options.h"
#include "libpandabase/mem/mem.h"
#include "test/utils/panda_executable_path_getter.h"

namespace {

struct TestParams {
    explicit TestParams(std::string_view testSrc, std::string testArgsList = std::string {},
                        int testArgsCount = ARGS_COUNT_DEFAULT, std::string_view testFileName = FILE_NAME_DEFAULT)
        : src {testSrc}, argsList {std::move(testArgsList)}, argsCount {testArgsCount}, fileName {testFileName}
    {
    }

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    std::string_view src;
    std::string argsList;
    int argsCount;
    std::string_view fileName;

    static constexpr int ARGS_COUNT_DEFAULT = 1;
    static constexpr std::string_view FILE_NAME_DEFAULT = "dummy.sts";
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

TestParams DumpJsonSimple()
{
    static constexpr std::string_view SRC =
        "\
        function main(args: String[]): int {\
            let a: int = 2;\
            let b: int = 3;\
            return a + b;\
        }";

    return TestParams {SRC};
}

TestParams DumpJsonUTF16Char()
{
    static constexpr std::string_view SRC =
        "\
        function main(args: String[]): int {\
            let a: char = c'\\uDBFF';\
            let b: char = c'\\uDC00';\
            console.log(a);\
            console.log(b);\
            return 0;\
        }";

    return TestParams {SRC};
}

TestParams DumpEtsSrcSimple()
{
    static constexpr std::string_view SRC =
        "\
        function main(args: String[]): int {\
            let a: int = 2;\
            let b: int = 3;\
            return a + b;\
        }";

    auto es2pandaPath = test::utils::PandaExecutablePathGetter {}.Get();
    auto argsList =
        es2pandaPath +
        "--extension=sts "
        "--dump-ets-src-before-phases=\"plugins-after-parse:lambda-lowering:checker:plugins-after-check:generate-"
        "ts-"
        "declarations:op-assignment:tuple-lowering:union-property-access:plugins-after-lowering\"";

    return TestParams {SRC, argsList};
}

}  // namespace

class ASTDumperTest : public testing::TestWithParam<TestParams> {
public:
    ASTDumperTest()
    {
        constexpr auto COMPILER_SIZE = 268435456;

        ark::mem::MemConfig::Initialize(0, 0, COMPILER_SIZE, 0, 0, 0);
        ark::PoolManager::Initialize(ark::PoolType::MMAP);
    }

    ~ASTDumperTest() override
    {
        ark::PoolManager::Finalize();
        ark::mem::MemConfig::Finalize();
    };

    static ark::pandasm::Program *GetProgram(std::string_view src, const char **argsList, int argsCount,
                                             std::string_view fileName)
    {
        auto options = std::make_unique<ark::es2panda::util::Options>();
        if (!options->Parse(argsCount, argsList)) {
            std::cerr << options->ErrorMsg() << std::endl;
            return nullptr;
        }

        ark::Logger::ComponentMask mask {};
        mask.set(ark::Logger::Component::ES2PANDA);
        ark::Logger::InitializeStdLogging(ark::Logger::LevelFromString(options->LogLevel()), mask);

        ark::es2panda::Compiler compiler(options->Extension(), options->ThreadCount());
        ark::es2panda::SourceFile input(fileName, src, options->ParseModule());

        return compiler.Compile(input, *options);
    }

    NO_COPY_SEMANTIC(ASTDumperTest);
    NO_MOVE_SEMANTIC(ASTDumperTest);
};

TEST_P(ASTDumperTest, CheckNoDump)
{
    auto param = GetParam();
    if (param.argsList.empty()) {
        param.argsList = test::utils::PandaExecutablePathGetter {}.Get();
    }

    auto argsListPtr = param.argsList.c_str();

    auto program =
        std::unique_ptr<ark::pandasm::Program> {GetProgram(param.src, &argsListPtr, param.argsCount, param.fileName)};
    ASSERT(program);

    auto dumpStr = program->JsonDump();
    ASSERT(!dumpStr.empty());
}

INSTANTIATE_TEST_SUITE_P(ASTDumperTestParamList, ASTDumperTest,
                         ::testing::Values(DumpJsonSimple(), DumpJsonUTF16Char(), DumpEtsSrcSimple()));
