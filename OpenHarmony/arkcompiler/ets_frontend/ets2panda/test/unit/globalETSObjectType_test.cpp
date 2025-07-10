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

#include "checker/ETSAnalyzer.h"
#include "checker/ETSchecker.h"
#include "compiler/core/compilerImpl.h"
#include "compiler/core/ETSCompiler.h"
#include "compiler/core/ETSemitter.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/regSpiller.h"
#include "compiler/lowering/phase.h"
#include "es2panda.h"
#include "mem/arena_allocator.h"
#include "mem/pool_manager.h"
#include "public/public.h"
#include "util/arktsconfig.h"
#include "util/generateBin.h"
#include "varbinder/ETSBinder.h"
#include "test/utils/panda_executable_path_getter.h"
#include "checker/types/globalTypesHolder.h"
#include "test/unit/globalETSObjectType_test.h"

using ark::es2panda::gtests::GlobalETSObjectTypeTest;

namespace ark::es2panda {
TEST_F(GlobalETSObjectTypeTest, TypeDeclNodeTest)
{
    // The DeclNode of GlobalETSObjectType will be modified accidentally
    // when we don't use explicit type declaration in for-statments (issue20054)
    // Test case added to check the DeclNode of GlobalETSObjectType
    std::stringstream src;
    src << "class A {\n"
        << "    testObj: Object = {};\n"
        << "    testBool: boolean = false;\n"
        << "    prop: int = 1;\n}\n"
        << "function main() {"
        << "     let arr = new A[10]\n;"
        << "     for (let a of arr) {\n"
        << "         if (a.testBool) {\n"
        << "}\n}\n}" << std::endl;

    InitializeChecker("_.sts", src.str());
    auto checker = Checker();
    ASSERT(checker);
    auto *globalETSObjectType = checker->GlobalETSObjectType();
    [[maybe_unused]] auto *declNode = globalETSObjectType->Variable()->Declaration()->Node();
    ASSERT(declNode->IsClassDefinition());
}
}  // namespace ark::es2panda
