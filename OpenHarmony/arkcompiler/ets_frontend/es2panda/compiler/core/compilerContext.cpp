/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "compilerContext.h"

#include <compiler/core/emitter/emitter.h>

namespace panda::es2panda::compiler {

CompilerContext::CompilerContext(binder::Binder *binder, bool isDebug, bool isDebuggerEvaluateExpressionMode,
                                 bool isMergeAbc, bool isJsonInputFile,
                                 bool isRecordDebugSource, const std::string &sourceFile, const std::string &pkgName,
                                 util::StringView recordName, util::PatchFix *patchFixHelper)
    : binder_(binder), isDebug_(isDebug), isDebuggerEvaluateExpressionMode_(isDebuggerEvaluateExpressionMode),
      isMergeAbc_(isMergeAbc), isJsonInputFile_(isJsonInputFile),
      isRecordDebugSource_(isRecordDebugSource), sourceFile_(sourceFile), pkgName_(pkgName), recordName_(recordName),
      patchFixHelper_(patchFixHelper), emitter_(std::make_unique<class Emitter>(this))
{
}

}  // namespace panda::es2panda::compiler
