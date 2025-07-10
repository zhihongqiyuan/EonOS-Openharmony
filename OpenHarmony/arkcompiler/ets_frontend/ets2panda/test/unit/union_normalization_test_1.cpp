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
#include "test/unit/union_normalisation_test.h"

using ark::es2panda::gtests::UnionNormalizationTest;

namespace ark::es2panda {
TEST_F(UnionNormalizationTest, UnionWithObject)
{
    // Test normalization: int | Object | string ==> Object
    InitializeChecker("_.sts", "");

    auto checker = Checker();
    ASSERT(checker);

    ArenaVector<checker::Type *> unionConstituents(checker->Allocator()->Adapter());
    unionConstituents.emplace_back(checker->GlobalIntType());
    unionConstituents.emplace_back(checker->GetGlobalTypesHolder()->GlobalETSObjectType());
    unionConstituents.emplace_back(checker->GetGlobalTypesHolder()->GlobalETSStringBuiltinType());

    // Create union type, which will be normalized inside creation function
    auto *const normalizedType = checker->CreateETSUnionType(std::move(unionConstituents));
    ASSERT_NE(normalizedType, nullptr);
    ASSERT_TRUE(normalizedType->IsETSObjectType());
    ASSERT_EQ(normalizedType, checker->GlobalETSObjectType());
}

TEST_F(UnionNormalizationTest, UnionWithIdenticalTypes1)
{
    // Test normalization: number | Base | string | number ==> number | Base | string
    InitializeChecker("_.sts", "class Base {}");

    auto program = Program();
    ASSERT(program);

    auto *const baseType = FindClassType(program->VarBinder()->AsETSBinder(), "Base");
    ASSERT_NE(baseType, nullptr);

    auto checker = Checker();
    ASSERT(checker);

    ArenaVector<checker::Type *> unionConstituents(checker->Allocator()->Adapter());
    unionConstituents.emplace_back(checker->GlobalDoubleType());
    unionConstituents.emplace_back(baseType);
    unionConstituents.emplace_back(checker->GlobalBuiltinETSStringType());
    unionConstituents.emplace_back(checker->GlobalDoubleType());

    // Create union type, which will be normalized inside creation function
    auto *const normalizedType = checker->CreateETSUnionType(std::move(unionConstituents));
    ASSERT_NE(normalizedType, nullptr);
    ASSERT_TRUE(normalizedType->IsETSUnionType());
    auto *const unionType = normalizedType->AsETSUnionType();
    ASSERT_EQ(unionType->ConstituentTypes().size(), SIZE3);
    ASSERT_EQ(unionType->ConstituentTypes().at(IDX0), checker->GetGlobalTypesHolder()->GlobalDoubleBuiltinType());
    ASSERT_EQ(unionType->ConstituentTypes().at(IDX1), baseType);
    ASSERT_EQ(unionType->ConstituentTypes().at(IDX2), checker->GlobalBuiltinETSStringType());
}

TEST_F(UnionNormalizationTest, DISABLED_UnionWithIdenticalTypes2)
{
    // Test normalization: Base | int | Base | double | short | number ==> Base | number
    InitializeChecker("_.sts", "class Base {}");

    auto program = Program();
    ASSERT(program);

    auto *const baseType = FindClassType(program->VarBinder()->AsETSBinder(), "Base");
    ASSERT_NE(baseType, nullptr);

    auto checker = Checker();
    ASSERT(checker);

    ArenaVector<checker::Type *> unionConstituents(checker->Allocator()->Adapter());
    unionConstituents.emplace_back(baseType);
    unionConstituents.emplace_back(checker->GlobalIntType());
    unionConstituents.emplace_back(baseType);
    unionConstituents.emplace_back(checker->GlobalDoubleType());
    unionConstituents.emplace_back(checker->GlobalShortType());
    unionConstituents.emplace_back(checker->GlobalDoubleType());

    // Create union type, which will be normalized inside creation function
    auto *const normalizedType = checker->CreateETSUnionType(std::move(unionConstituents));
    ASSERT_NE(normalizedType, nullptr);
    ASSERT_TRUE(normalizedType->IsETSUnionType());
    auto *const unionType = normalizedType->AsETSUnionType();
    ASSERT_EQ(unionType->ConstituentTypes().size(), SIZE2);
    ASSERT_EQ(unionType->ConstituentTypes().at(IDX0), baseType);
    ASSERT_EQ(unionType->ConstituentTypes().at(IDX1), checker->GetGlobalTypesHolder()->GlobalDoubleBuiltinType());
}

TEST_F(UnionNormalizationTest, DISABLED_UnionWithNumeric1)
{
    // Test normalization: boolean | int | double | short ==> boolean | double
    InitializeChecker("_.sts", "");

    auto checker = Checker();
    ASSERT(checker);

    ArenaVector<checker::Type *> unionConstituents(checker->Allocator()->Adapter());
    unionConstituents.emplace_back(checker->GlobalETSBooleanType());
    unionConstituents.emplace_back(checker->GlobalIntType());
    unionConstituents.emplace_back(checker->GlobalDoubleType());
    unionConstituents.emplace_back(checker->GlobalShortType());

    // Create union type, which will be normalized inside creation function
    auto *const normalizedType = checker->CreateETSUnionType(std::move(unionConstituents));
    ASSERT_NE(normalizedType, nullptr);
    ASSERT_TRUE(normalizedType->IsETSUnionType());
    auto *const unionType = normalizedType->AsETSUnionType();
    ASSERT_EQ(unionType->ConstituentTypes().size(), SIZE2);
    ASSERT_EQ(unionType->ConstituentTypes().at(IDX0), checker->GetGlobalTypesHolder()->GlobalETSBooleanBuiltinType());
    ASSERT_EQ(unionType->ConstituentTypes().at(IDX1), checker->GetGlobalTypesHolder()->GlobalDoubleBuiltinType());
}

TEST_F(UnionNormalizationTest, DISABLED_UnionWithNumeric2)
{
    // Test normalization: string | int | Base | double | short ==> string | Base | double
    InitializeChecker("_.sts", "class Base {}");

    auto program = Program();
    ASSERT(program);

    auto *const baseType = FindClassType(program->VarBinder()->AsETSBinder(), "Base");
    ASSERT_NE(baseType, nullptr);

    auto checker = Checker();
    ASSERT(checker);

    ArenaVector<checker::Type *> unionConstituents(checker->Allocator()->Adapter());
    unionConstituents.emplace_back(checker->GlobalBuiltinETSStringType());
    unionConstituents.emplace_back(checker->GlobalIntType());
    unionConstituents.emplace_back(baseType);
    unionConstituents.emplace_back(checker->GlobalDoubleType());
    unionConstituents.emplace_back(checker->GlobalShortType());

    // Create union type, which will be normalized inside creation function
    auto *const normalizedType = checker->CreateETSUnionType(std::move(unionConstituents));
    ASSERT_NE(normalizedType, nullptr);
    ASSERT_TRUE(normalizedType->IsETSUnionType());
    auto *const unionType = normalizedType->AsETSUnionType();
    ASSERT_EQ(unionType->ConstituentTypes().size(), SIZE3);
    ASSERT_EQ(unionType->ConstituentTypes().at(IDX0), checker->GlobalBuiltinETSStringType());
    ASSERT_EQ(unionType->ConstituentTypes().at(IDX1), baseType);
    ASSERT_EQ(unionType->ConstituentTypes().at(IDX2), checker->GetGlobalTypesHolder()->GlobalDoubleBuiltinType());
}
}  // namespace ark::es2panda
