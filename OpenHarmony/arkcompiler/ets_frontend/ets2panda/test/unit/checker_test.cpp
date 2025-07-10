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
#include "macros.h"
#include "public/es2panda_lib.h"

#include "test/utils/panda_executable_path_getter.h"

class CheckerTest : public testing::Test {
public:
    CheckerTest()
    {
        impl_ = es2panda_GetImpl(ES2PANDA_LIB_VERSION);
        auto es2pandaPath = test::utils::PandaExecutablePathGetter {}.Get();
        // NOLINTNEXTLINE(modernize-avoid-c-arrays)
        char const *argv[] = {es2pandaPath.c_str()};
        cfg_ = impl_->CreateConfig(1, argv);
    }

    ~CheckerTest() override
    {
        impl_->DestroyConfig(cfg_);
    }

    NO_COPY_SEMANTIC(CheckerTest);
    NO_MOVE_SEMANTIC(CheckerTest);

protected:
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    es2panda_Impl const *impl_;
    es2panda_Config *cfg_;
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

TEST_F(CheckerTest, ExtendedConditionalExpressionFunctor)
{
    char const *text = R"XXX(
class A {
    m() {}
    m2() { this.m ? "a": "b" }
}
)XXX";
    es2panda_Context *ctx = impl_->CreateContextFromString(cfg_, text, "dummy.sts");
    ctx = impl_->ProceedToState(ctx, ES2PANDA_STATE_CHECKED);
    ASSERT_EQ(std::string(impl_->ContextErrorMessage(ctx)),
              "TypeError: Condition must be of possible condition type[dummy.sts:4,12]");

    impl_->DestroyContext(ctx);
}
