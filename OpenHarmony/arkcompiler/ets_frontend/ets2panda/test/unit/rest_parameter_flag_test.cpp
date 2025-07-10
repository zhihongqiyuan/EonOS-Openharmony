/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "assembler/assembly-program.h"
#include "es2panda.h"
#include "generated/signatures.h"
#include "libpandabase/mem/mem.h"
#include "macros.h"
#include "mem/pool_manager.h"
#include "util/options.h"

namespace ark::es2panda::compiler::test {

class RestParameterTest : public testing::Test {
public:
    RestParameterTest()
    {
        const auto compilerSize = 268435456;

        mem::MemConfig::Initialize(0, 0, compilerSize, 0, 0, 0);
        PoolManager::Initialize(PoolType::MMAP);
    }
    ~RestParameterTest() override
    {
        PoolManager::Finalize();
        mem::MemConfig::Finalize();
    }

    void SetCurrentProgram(std::string_view src)
    {
        int argc = 1;
        const char *argv = "../../../../bin/es2panda";  // NOLINT(modernize-avoid-c-arrays)
        static constexpr std::string_view FILE_NAME = "dummy.sts";

        program_ = GetProgram(argc, &argv, FILE_NAME, src);
        ASSERT_NE(program_.get(), nullptr);
    }

    void CheckRestParameterFlag(std::string_view functionName)
    {
        pandasm::Function *fn = GetFunction(functionName);
        ASSERT_TRUE(fn != nullptr) << "Function '" << functionName << "' not found";
        ASSERT_TRUE(HasRestParameterFlag(fn)) << "Function '" << fn->name << "' doesn't have ACC_VARARGS flag";
    }

    void CheckNoRestParameterFlag(std::string_view functionName)
    {
        pandasm::Function *fn = GetFunction(functionName);
        ASSERT_TRUE(fn != nullptr) << "Function '" << functionName << "' not found";
        ASSERT_FALSE(HasRestParameterFlag(fn)) << "Function '" << fn->name << "' has ACC_VARARGS flag";
    }

private:
    bool HasRestParameterFlag(pandasm::Function *fn)
    {
        return (fn->metadata->GetAccessFlags() & ACC_VARARGS) != 0;
    }

    NO_COPY_SEMANTIC(RestParameterTest);
    NO_MOVE_SEMANTIC(RestParameterTest);

    static std::unique_ptr<pandasm::Program> GetProgram(int argc, const char **argv, std::string_view fileName,
                                                        std::string_view src)
    {
        auto options = std::make_unique<es2panda::util::Options>();
        if (!options->Parse(argc, argv)) {
            std::cerr << options->ErrorMsg() << std::endl;
            return nullptr;
        }

        Logger::ComponentMask mask {};
        mask.set(Logger::Component::ES2PANDA);
        Logger::InitializeStdLogging(Logger::LevelFromString(options->LogLevel()), mask);

        es2panda::Compiler compiler(options->Extension(), options->ThreadCount());
        es2panda::SourceFile input(fileName, src, options->ParseModule());

        return std::unique_ptr<pandasm::Program>(compiler.Compile(input, *options));
    }

    pandasm::Function *GetFunction(std::string_view functionName)
    {
        auto it = program_->functionTable.find(functionName.data());
        if (it == program_->functionTable.end()) {
            return nullptr;
        }
        return &it->second;
    }

private:
    std::unique_ptr<pandasm::Program> program_ {};
};

// === Function ===
TEST_F(RestParameterTest, function_without_rest_parameters_0)
{
    SetCurrentProgram(R"(
        function fn(): void {
        }
    )");
    CheckNoRestParameterFlag("ETSGLOBAL.fn:void;");
}

TEST_F(RestParameterTest, function_without_rest_parameters_1)
{
    SetCurrentProgram(R"(
        function fn(args: int[]): void {
        }
    )");
    CheckNoRestParameterFlag("ETSGLOBAL.fn:i32[];void;");
}

TEST_F(RestParameterTest, function_without_rest_parameters_2)
{
    SetCurrentProgram(R"(
        function fn(arg0: int, args: String[]): void {
        }
    )");
    CheckNoRestParameterFlag("ETSGLOBAL.fn:i32;std.core.String[];void;");
}

TEST_F(RestParameterTest, function_with_rest_parameter_0)
{
    SetCurrentProgram(R"(
        function fn(...args: String[]): void {
        }
    )");
    CheckRestParameterFlag("ETSGLOBAL.fn:std.core.String[];void;");
}

TEST_F(RestParameterTest, function_with_rest_parameter_1)
{
    SetCurrentProgram(R"(
        function fn(o: Object, ...args: int[]): void {
        }
    )");
    CheckRestParameterFlag("ETSGLOBAL.fn:std.core.Object;i32[];void;");
}

// === Method of class ===
TEST_F(RestParameterTest, class_method_without_rest_parameters_0)
{
    SetCurrentProgram(R"(
        class A {
            fn() {};
        }
    )");
    CheckNoRestParameterFlag("A.fn:void;");
}

TEST_F(RestParameterTest, class_method_without_rest_parameters_1)
{
    SetCurrentProgram(R"(
        class A {
            fn(arg0: int) {};
        }
    )");
    CheckNoRestParameterFlag("A.fn:i32;void;");
}

TEST_F(RestParameterTest, class_method_with_rest_parameters_0)
{
    SetCurrentProgram(R"(
        class A {
            fn(...args: int[]) {};
        }
    )");
    CheckRestParameterFlag("A.fn:i32[];void;");
}

// === Static method of class ===
TEST_F(RestParameterTest, static_class_method_without_rest_parameters_0)
{
    SetCurrentProgram(R"(
        class A {
            static fn() {};
        }
    )");
    CheckNoRestParameterFlag("A.fn:void;");
}

TEST_F(RestParameterTest, static_class_method_without_rest_parameters_1)
{
    SetCurrentProgram(R"(
        class A {
            static fn(arg0: int) {};
        }
    )");
    CheckNoRestParameterFlag("A.fn:i32;void;");
}

TEST_F(RestParameterTest, static_class_method_with_rest_parameters_0)
{
    SetCurrentProgram(R"(
        class A {
            static fn(...args: int[]) {};
        }
    )");
    CheckRestParameterFlag("A.fn:i32[];void;");
}

TEST_F(RestParameterTest, static_class_method_with_rest_parameters_1)
{
    SetCurrentProgram(R"(
        class A {
            static fn(a: String[], ...args: int[]) {};
        }
    )");
    CheckRestParameterFlag("A.fn:std.core.String[];i32[];void;");
}

// === Constructor of class ===
TEST_F(RestParameterTest, class_constructor_without_rest_parameters_0)
{
    SetCurrentProgram(R"(
        class A {
            constructor() {};
        }
    )");
    CheckNoRestParameterFlag("A.<ctor>:void;");
}

TEST_F(RestParameterTest, class_constructor_without_rest_parameters_1)
{
    SetCurrentProgram(R"(
        class A {
            constructor(args: String[]) {};
        }
    )");
    CheckNoRestParameterFlag("A.<ctor>:std.core.String[];void;");
}

TEST_F(RestParameterTest, class_constructor_with_rest_parameters_0)
{
    SetCurrentProgram(R"(
        class A {
            constructor(...args: int[]) {};
        }
    )");
    CheckRestParameterFlag("A.<ctor>:i32[];void;");
}

TEST_F(RestParameterTest, class_constructor_with_rest_parameters_1)
{
    SetCurrentProgram(R"(
        class A {
            constructor(v0: long, ...args: String[]) {};
        }
    )");
    CheckRestParameterFlag("A.<ctor>:i64;std.core.String[];void;");
}

// === Method of interface ===
TEST_F(RestParameterTest, interface_without_rest_parameters_0)
{
    SetCurrentProgram(R"(
        interface A {
            fn() {};
        }
    )");
    CheckNoRestParameterFlag("A.fn:void;");
}

TEST_F(RestParameterTest, interface_without_rest_parameters_1)
{
    SetCurrentProgram(R"(
        interface A {
            fn(args: String[]) {};
        }
    )");
    CheckNoRestParameterFlag("A.fn:std.core.String[];void;");
}

TEST_F(RestParameterTest, interface_with_rest_parameters_0)
{
    SetCurrentProgram(R"(
        interface A {
            fn(...args: Object[]) {};
        }
    )");
    CheckRestParameterFlag("A.fn:std.core.Object[];void;");
}

TEST_F(RestParameterTest, interface_with_rest_parameters_1)
{
    SetCurrentProgram(R"(
        interface A {
            fn(o: Object, ...args: String[]) {};
        }
    )");
    CheckRestParameterFlag("A.fn:std.core.Object;std.core.String[];void;");
}

// === Lambda method ===
TEST_F(RestParameterTest, lambda_without_rest_parameters_0)
{
    SetCurrentProgram(R"(
        let fn: ()=>int = (): int => {
            return 1;
        }
    )");
    CheckNoRestParameterFlag("LambdaObject-ETSGLOBAL$lambda$invoke$0.invoke:i32;");
}

TEST_F(RestParameterTest, lambda_without_rest_parameters_1)
{
    SetCurrentProgram(R"(
        let fn: (args: long[])=>int = (args: long[]): int => {
            return 1;
        }
    )");
    CheckNoRestParameterFlag("LambdaObject-ETSGLOBAL$lambda$invoke$0.invoke:i64[];i32;");
}

// === Abstract method of abstract class ===
TEST_F(RestParameterTest, abstract_function_without_rest_parameter_0)
{
    SetCurrentProgram(R"(
        abstract class A {
            abstract fn(): void
        }
    )");
    CheckNoRestParameterFlag("A.fn:void;");
}

TEST_F(RestParameterTest, abstract_function_without_rest_parameter_1)
{
    SetCurrentProgram(R"(
        abstract class A {
            abstract fn(args: String[]): void
        }
    )");
    CheckNoRestParameterFlag("A.fn:std.core.String[];void;");
}

TEST_F(RestParameterTest, abstract_function_with_rest_parameter_0)
{
    SetCurrentProgram(R"(
        abstract class A {
            abstract fn(...args: String[]): void
        }
    )");
    CheckRestParameterFlag("A.fn:std.core.String[];void;");
}

TEST_F(RestParameterTest, abstract_function_with_rest_parameter_1)
{
    SetCurrentProgram(R"(
        abstract class A {
            abstract fn(v: int, ...args: String[]): void
        }
    )");
    CheckRestParameterFlag("A.fn:i32;std.core.String[];void;");
}

// === External methods ===
TEST_F(RestParameterTest, external_function_with_rest_parameter_0)
{
    SetCurrentProgram("");
    CheckRestParameterFlag("std.core.LambdaValue.invoke:std.core.Object[];std.core.Object;");
}

TEST_F(RestParameterTest, external_function_with_rest_parameter_1)
{
    SetCurrentProgram("");
    CheckRestParameterFlag("escompat.Math.max:f64[];f64;");
}

}  // namespace ark::es2panda::compiler::test
