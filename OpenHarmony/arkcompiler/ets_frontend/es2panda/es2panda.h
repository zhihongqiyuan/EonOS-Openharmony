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

#ifndef ES2PANDA_PUBLIC_H
#define ES2PANDA_PUBLIC_H

#include <macros.h>
#include <mem/arena_allocator.h>
#include <util/patchFix.h>
#include <util/programCache.h>
#include <util/symbolTable.h>
#include <abc2program/abc2program_compiler.h>

#include <string>
#include <unordered_map>

namespace panda::pandasm {
struct Program;
}  // namespace panda::pandasm

namespace panda::es2panda {
struct CompileContextInfo;
struct PkgInfo;

namespace parser {
class ParserImpl;
class Program;
class Transformer;
enum class ScriptKind;
}  // namespace parser

namespace compiler {
class CompilerImpl;
}  // namespace compiler

enum class ScriptExtension {
    JS,
    TS,
    AS,
    ABC,
};

struct SourceFile {
    SourceFile(const std::string &fn, const std::string &recordName, parser::ScriptKind sk, ScriptExtension se)
        : fileName(fn), recordName(recordName), scriptKind(sk), scriptExtension(se)
    {
    }

    std::string fileName {};
    std::string recordName {};
    std::string_view source {};
    parser::ScriptKind scriptKind {};
    ScriptExtension scriptExtension {};
    std::string sourcefile {};
    std::string pkgName {};
    uint32_t hash {0};
    bool isSharedModule {false};
    bool isSourceMode {true};
    std::string sourceLang {};
};

struct PatchFixOptions {
    std::string dumpSymbolTable {};
    std::string symbolTable {};
    bool generatePatch {false};
    bool hotReload {false};
    bool coldReload {false};
    bool coldFix {false};
};

struct CompilerOptions {
    bool enableAbcInput {false};
    bool dumpAsmProgram {false};
    bool dumpNormalizedAsmProgram {false};
    bool isDebug {false};
    bool dumpAst {false};
    bool dumpTransformedAst {false};
    bool checkTransformedAstStructure {false};
    bool dumpAsm {false};
    bool dumpDebugInfo {false};
    bool parseOnly {false};
    bool enableTypeCheck {false};
    bool dumpLiteralBuffer {false};
    bool isDebuggerEvaluateExpressionMode {false};
    bool mergeAbc {false};
    bool useDefineSemantic {false};
    bool typeExtractor {false};
    bool typeDtsBuiltin {false};
    bool recordDebugSource {false};
    int fileThreadCount {0};
    int functionThreadCount {0};
    int abcClassThreadCount {0};
    int optLevel {0};
    std::string output {};
    std::string debugInfoSourceFile {};
    std::vector<es2panda::SourceFile> sourceFiles;
    PatchFixOptions patchFixOptions;
    bool bcVersion {false};
    bool bcMinVersion {false};
    int targetApiVersion {0};
    bool targetBcVersion {false};
    std::unordered_map<std::string, std::string> cacheFiles;
    std::string transformLib {};
    bool branchElimination {false};
    bool requireGlobalOptimization {false};
    std::string compileContextInfoPath {};
    CompileContextInfo compileContextInfo {};
    bool dumpDepsInfo {false};
    bool updatePkgVersionForAbcInput {false};
    bool removeRedundantFile {false};
    bool dumpString {false};
    std::string targetApiSubVersion;
    std::string moduleRecordFieldName;
    bool enableAnnotations;
    // Ability to modify package names using bytecode
    std::string modifiedPkgName {};
};

enum class ErrorType {
    GENERIC,
    SYNTAX,
    TYPE,
};

class Error : public std::exception {
public:
    Error() noexcept = default;
    explicit Error(ErrorType type, const std::string_view &message) noexcept : type_(type), message_(message) {}
    explicit Error(ErrorType type, const std::string_view &message, size_t line, size_t column) noexcept
        : type_(type), message_(message), line_(line), col_(column)
    {
    }
    ~Error() override = default;
    DEFAULT_COPY_SEMANTIC(Error);
    DEFAULT_MOVE_SEMANTIC(Error);

    ErrorType Type() const noexcept
    {
        return type_;
    }

    const char *TypeString() const noexcept
    {
        switch (type_) {
            case ErrorType::SYNTAX:
                return "SyntaxError";
            case ErrorType::TYPE:
                return "TypeError";
            default:
                break;
        }

        return "Error";
    }

    const char *what() const noexcept override
    {
        return message_.c_str();
    }

    int ErrorCode() const noexcept
    {
        return errorCode_;
    }

    const std::string &Message() const noexcept
    {
        return message_;
    }

    size_t Line() const
    {
        return line_;
    }

    size_t Col() const
    {
        return col_;
    }

    bool Reported() const
    {
        return reported_;
    }

    void SetReported(bool reported)
    {
        reported_ = reported;
    }

private:
    ErrorType type_ {ErrorType::GENERIC};
    std::string message_;
    size_t line_ {};
    size_t col_ {};
    int errorCode_ {1};
    bool reported_ {false};
};

class Compiler {
public:
    explicit Compiler(ScriptExtension ext);
    explicit Compiler(ScriptExtension ext, size_t threadCount);
    ~Compiler();
    NO_COPY_SEMANTIC(Compiler);
    NO_MOVE_SEMANTIC(Compiler);

    panda::pandasm::Program *Compile(const SourceFile &input, const CompilerOptions &options,
                                     util::SymbolTable *symbolTable = nullptr);
    panda::pandasm::Program *CompileFile(const CompilerOptions &options, SourceFile *src,
                                         util::SymbolTable *symbolTable);
    void ProcessAstForTS(parser::Program *ast, const es2panda::CompilerOptions &options);
    static int CompileFiles(CompilerOptions &options,
        std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo, panda::ArenaAllocator *allocator);

    inline panda::pandasm::Program *Compile(const SourceFile &input)
    {
        CompilerOptions options;

        return Compile(input, options);
    }

    static void DumpAsm(const panda::pandasm::Program *prog);

    Error &GetError() noexcept
    {
        return error_;
    }

    panda::pandasm::Program *CompileAbcFile(const std::string &fname, const CompilerOptions &options);

    void CompileAbcFileInParallel(SourceFile *src, const CompilerOptions &options,
                                  std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo,
                                  panda::ArenaAllocator *allocator);

    static size_t GetExpectedProgsCount()
    {
        return expectedProgsCount_;
    }

    static void SetExpectedProgsCount(size_t count)
    {
        expectedProgsCount_ = count;
    }

private:
    util::PatchFix *InitPatchFixHelper(const SourceFile &input, const CompilerOptions &options,
                                       util::SymbolTable *symbolTable);
    static void CleanPatchFixHelper(const util::PatchFix *patchFixHelper);
    void CheckOptionsAndFileForAbcInput(const std::string &fname, const CompilerOptions &options);

    static size_t expectedProgsCount_;
    parser::ParserImpl *parser_;
    compiler::CompilerImpl *compiler_;
    panda::abc2program::Abc2ProgramCompiler *abcToAsmCompiler_;
    std::unique_ptr<parser::Transformer> transformer_ {nullptr};
    Error error_;
};
}  // namespace panda::es2panda

#endif
