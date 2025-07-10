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

#ifndef ES2PANDA_PARSER_INCLUDE_PROGRAM_H
#define ES2PANDA_PARSER_INCLUDE_PROGRAM_H

#include <lexer/token/sourceLocation.h>
#include <macros.h>
#include <mem/arena_allocator.h>
#include <parser/module/sourceTextModuleRecord.h>
#include <util/patchFix.h>
#include <util/ustring.h>

#include "es2panda.h"

namespace panda::es2panda::ir {
class BlockStatement;
}  // namespace panda::es2panda::ir

namespace panda::es2panda::binder {
class Binder;
}  // namespace panda::es2panda::binder

namespace panda::es2panda::parser {

enum class ScriptKind { SCRIPT, MODULE, COMMONJS };

class Program {
public:
    explicit Program(es2panda::ScriptExtension extension);
    NO_COPY_SEMANTIC(Program);
    Program(Program &&other);
    Program &operator=(Program &&other);
    ~Program() = default;

    ArenaAllocator *Allocator() const
    {
        return allocator_.get();
    }

    const binder::Binder *Binder() const
    {
        return binder_;
    }

    binder::Binder *Binder()
    {
        return binder_;
    }

    ScriptExtension Extension() const
    {
        return extension_;
    }

    ScriptKind Kind() const
    {
        return kind_;
    }

    bool IsCommonjs() const
    {
        return kind_ == ScriptKind::COMMONJS;
    }

    SourceTextModuleRecord *ModuleRecord() const
    {
        return moduleRecord_;
    }

    SourceTextModuleRecord *TypeModuleRecord() const
    {
        return typeModuleRecord_;
    }

    util::StringView SourceCode() const
    {
        return sourceCode_.View();
    }

    util::StringView SourceFile() const
    {
        return sourceFile_.View();
    }

    util::StringView RecordName() const
    {
        return recordName_.View();
    }

    util::StringView FormatedRecordName() const
    {
        return formatedRecordName_.View();
    }

    const lexer::LineIndex &GetLineIndex() const
    {
        return lineIndex_;
    }

    ir::BlockStatement *Ast()
    {
        return ast_;
    }

    const ir::BlockStatement *Ast() const
    {
        return ast_;
    }

    void SetAst(ir::BlockStatement *ast)
    {
        ast_ = ast;
    }

    void SetSource(const std::string &sourceCode, const std::string &sourceFile, bool isDtsFile)
    {
        sourceCode_ = util::UString(sourceCode, Allocator());
        sourceFile_ = util::UString(sourceFile, Allocator());
        lineIndex_ = lexer::LineIndex(SourceCode());
        isDtsFile_ = isDtsFile;
    }

    void SetRecordName(const std::string &recordName)
    {
        recordName_ = util::UString(recordName, Allocator());
        std::string formatedRecordName = recordName + ".";
        formatedRecordName_ = util::UString(formatedRecordName, Allocator());
    }

    void AddPatchFixHelper(util::PatchFix *patchFixHelper)
    {
        patchFixHelper_ = patchFixHelper;
    }

    util::PatchFix *PatchFixHelper()
    {
        return patchFixHelper_;
    }

    bool IsDtsFile() const
    {
        return isDtsFile_;
    }

    bool HasTLA() const
    {
        return hasTLA_;
    }

    void SetHasTLA(bool hasTLA)
    {
        hasTLA_ = hasTLA;
    }
 
    bool IsDebug() const
    {
        return isDebug_;
    }

    void SetDebug(bool isDebug)
    {
        isDebug_ = isDebug;
    }

    int TargetApiVersion() const
    {
        return targetApiVersion_;
    }

    void SetTargetApiVersion(int targetApiVersion)
    {
        targetApiVersion_ = targetApiVersion;
    }

    void SetTargetApiSubVersion(std::string targetApiSubVersion)
    {
        targetApiSubVersion_ = targetApiSubVersion;
    }

    std::string GetTargetApiSubVersion() const
    {
        return targetApiSubVersion_;
    }

    bool UseDefineSemantic() const
    {
        return useDefineSemantic_;
    }

    void SetDefineSemantic(bool useDefineSemantic)
    {
        useDefineSemantic_ = useDefineSemantic;
    }

    void SetShared(bool isShared)
    {
        isShared_ = isShared;
    }

    bool IsShared() const
    {
        return isShared_;
    }

    void SetModuleRecordFieldName(std::string moduleRecordFieldName)
    {
        moduleRecordFieldName_ = moduleRecordFieldName;
    }

    std::string ModuleRecordFieldName() const
    {
        return moduleRecordFieldName_;
    }
    
    void SetEnableAnnotations(bool enableAnnotations)
    {
        enableAnnotations_ = enableAnnotations;
    }

    bool IsEnableAnnotations() const
    {
        return enableAnnotations_;
    }

    void SetSourceLang(const std::string &sourceLang)
    {
        if (sourceLang == "ets") {
            sourceLang_ = panda::pandasm::extensions::Language::ARKTS;
        } else if (sourceLang == "ts") {
            sourceLang_ = panda::pandasm::extensions::Language::TYPESCRIPT;
        } else if (sourceLang == "js") {
            sourceLang_ = panda::pandasm::extensions::Language::JAVASCRIPT;
        } else {
            sourceLang_ = panda::pandasm::extensions::Language::ECMASCRIPT;
        }
    }

    panda::pandasm::extensions::Language SourceLang() const
    {
        return sourceLang_;
    }

    std::string Dump() const;
    void SetKind(ScriptKind kind);

private:
    std::unique_ptr<ArenaAllocator> allocator_ {};
    binder::Binder *binder_ {};
    ir::BlockStatement *ast_ {};
    util::UString sourceCode_ {};
    util::UString sourceFile_ {};
    util::UString recordName_ {};
    util::UString formatedRecordName_ {};
    ScriptKind kind_ {};
    ScriptExtension extension_ {};
    lexer::LineIndex lineIndex_ {};
    SourceTextModuleRecord *moduleRecord_ {nullptr};
    SourceTextModuleRecord *typeModuleRecord_ {nullptr};
    util::PatchFix *patchFixHelper_ {nullptr};
    bool isDtsFile_ {false};
    bool hasTLA_ {false};
    bool isDebug_ {false};
    int targetApiVersion_ {0};
    bool useDefineSemantic_ {true};
    bool isShared_ {false};
    bool enableAnnotations_ {false};
    std::string targetApiSubVersion_ { util::Helpers::DEFAULT_SUB_API_VERSION };
    std::string moduleRecordFieldName_;
    panda::pandasm::extensions::Language sourceLang_ {panda::pandasm::extensions::DEFAULT_LANGUAGE};
};

}  // namespace panda::es2panda::parser

#endif
