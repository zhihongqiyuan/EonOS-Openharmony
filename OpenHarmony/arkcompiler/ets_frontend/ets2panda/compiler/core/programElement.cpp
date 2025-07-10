/*
 * Copyright (c) 2021 - 2023 Huawei Device Co., Ltd.
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

#include "programElement.h"

#include <assembly-program.h>

namespace ark::es2panda::compiler {
std::set<std::string> &ProgramElement::Strings()
{
    return strings_;
}

std::vector<pandasm::Ins *> &ProgramElement::LiteralBufferIns()
{
    return literalBufferIns_;
}

std::vector<LiteralBuffer> &ProgramElement::BuffStorage()
{
    return buffStorage_;
}

pandasm::Function *ProgramElement::Function()
{
    return func_;
}

void ProgramElement::SetFunction(pandasm::Function *func)
{
    func_ = func;
}

ProgramElement::~ProgramElement()
{
    delete func_;
}
}  // namespace ark::es2panda::compiler
