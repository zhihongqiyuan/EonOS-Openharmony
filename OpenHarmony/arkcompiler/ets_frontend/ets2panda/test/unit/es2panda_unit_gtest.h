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

#ifndef PANDA_ES2PANDA_UNIT_GTEST_H
#define PANDA_ES2PANDA_UNIT_GTEST_H

#include <gtest/gtest.h>
#include <memory>
#include <libpandabase/mem/arena_allocator.h>
#include <libpandabase/mem/pool_manager.h>

namespace ark::es2panda::testing {

class Es2pandaUnitGtest : public ::testing::Test {
public:
    Es2pandaUnitGtest() : allocator_(std::make_unique<ArenaAllocator>(SpaceType::SPACE_TYPE_COMPILER)) {}

    static void SetUpTestCase()
    {
        constexpr auto COMPILER_SIZE = operator""_MB(256ULL);
        mem::MemConfig::Initialize(0, 0, COMPILER_SIZE, 0, 0, 0);
        PoolManager::Initialize();
    }

    ArenaAllocator *Allocator()
    {
        return allocator_.get();
    }

private:
    std::unique_ptr<ArenaAllocator> allocator_;
};

}  // namespace ark::es2panda::testing
#endif  // PANDA_ES2PANDA_UNIT_GTEST_H
