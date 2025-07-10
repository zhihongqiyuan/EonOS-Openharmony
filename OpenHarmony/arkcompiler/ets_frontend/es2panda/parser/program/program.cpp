/**
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "program.h"

#include <ir/astDump.h>

namespace panda::es2panda::parser {

Program::Program(ScriptExtension extension)
    : allocator_(std::make_unique<ArenaAllocator>(SpaceType::SPACE_TYPE_COMPILER, nullptr, true)),
      binder_(allocator_->New<binder::Binder>(this, extension)),
      sourceCode_(Allocator()),
      sourceFile_(Allocator()),
      extension_(extension)
{
}

Program::Program(Program &&other)
    : allocator_(std::move(other.allocator_)),
      binder_(other.binder_),
      ast_(other.ast_),
      sourceCode_(other.sourceCode_),
      sourceFile_(other.sourceFile_),
      recordName_(other.recordName_),
      formatedRecordName_(other.formatedRecordName_),
      kind_(other.kind_),
      extension_(other.extension_),
      lineIndex_(std::move(other.lineIndex_)),
      moduleRecord_(other.moduleRecord_),
      typeModuleRecord_(other.typeModuleRecord_),
      patchFixHelper_(other.patchFixHelper_),
      isDtsFile_(other.isDtsFile_),
      hasTLA_(other.hasTLA_),
      isDebug_(other.isDebug_),
      targetApiVersion_(other.targetApiVersion_),
      useDefineSemantic_(other.useDefineSemantic_),
      isShared_(other.isShared_),
      enableAnnotations_(other.enableAnnotations_),
      targetApiSubVersion_(other.targetApiSubVersion_),
      moduleRecordFieldName_(other.moduleRecordFieldName_),
      sourceLang_(other.sourceLang_)
{
    other.binder_ = nullptr;
    other.ast_ = nullptr;
    other.moduleRecord_ = nullptr;
    other.patchFixHelper_ = nullptr;
    other.typeModuleRecord_ = nullptr;
}

Program &Program::operator=(Program &&other)
{
    if (this == &other) {
        return *this;
    }
    allocator_ = std::move(other.allocator_);
    binder_ = other.binder_;
    ast_ = other.ast_;
    sourceCode_ = other.sourceCode_;
    sourceFile_ = other.sourceFile_;
    kind_ = other.kind_;
    extension_ = other.extension_;
    lineIndex_ = other.lineIndex_;
    isDtsFile_ = other.isDtsFile_;
    hasTLA_ = other.hasTLA_;
    isDebug_ = other.isDebug_;
    targetApiVersion_ = other.targetApiVersion_;
    useDefineSemantic_ = other.useDefineSemantic_;
    isShared_ = other.isShared_;
    enableAnnotations_ = other.enableAnnotations_;
    targetApiSubVersion_ = other.targetApiSubVersion_;
    moduleRecordFieldName_ = other.moduleRecordFieldName_;
    sourceLang_ = other.sourceLang_;

    other.ast_ = nullptr;
    other.binder_ = nullptr;
    other.moduleRecord_ = nullptr;
    other.patchFixHelper_ = nullptr;
    other.typeModuleRecord_ = nullptr;

    return *this;
}

void Program::SetKind(ScriptKind kind)
{
    kind_ = kind;
    binder_->InitTopScope();

    if (kind == ScriptKind::MODULE) {
        moduleRecord_ = allocator_->New<SourceTextModuleRecord>(Allocator());
        typeModuleRecord_ = allocator_->New<SourceTextModuleRecord>(Allocator());
    }
}

std::string Program::Dump() const
{
    ir::AstDumper dumper {ast_, SourceCode()};
    return dumper.Str();
}

}  // namespace panda::es2panda::parser
