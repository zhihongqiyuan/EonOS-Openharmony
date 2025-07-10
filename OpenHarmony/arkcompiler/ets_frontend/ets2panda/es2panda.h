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

#ifndef ES2PANDA_PUBLIC_H
#define ES2PANDA_PUBLIC_H

#include "macros.h"
#include "util/arktsconfig.h"
#include "util/plugin.h"
#include "util/ustring.h"

#include <string>
#include <functional>
#include <memory>
#include <unordered_set>

namespace ark::pandasm {
struct Program;
}  // namespace ark::pandasm

namespace ark::es2panda {

constexpr std::string_view ES2PANDA_VERSION = "0.1";
namespace util {
class Options;
}  // namespace util
namespace parser {
class ParserImpl;
}  // namespace parser

namespace compiler {
class CompilerImpl;
}  // namespace compiler

namespace varbinder {
class VarBinder;
}  // namespace varbinder

enum class ScriptExtension {
    JS,
    TS,
    AS,
    ETS,
    INVALID,
};

enum class CompilationMode {
    GEN_STD_LIB,
    PROJECT,
    SINGLE_FILE,
};
// CC-OFFNXT(G.FUD.06) switch-case, ODR
inline Language ToLanguage(ScriptExtension ext)
{
    switch (ext) {
        case ScriptExtension::JS:
            return Language(Language::Id::JS);
        case ScriptExtension::TS:
            return Language(Language::Id::TS);
        case ScriptExtension::AS:
            return Language(Language::Id::AS);
        case ScriptExtension::ETS:
            return Language(Language::Id::ETS);
        default:
            UNREACHABLE();
    }
    UNREACHABLE();
}

struct SourceFile {
    SourceFile(std::string_view fn, std::string_view s);
    SourceFile(std::string_view fn, std::string_view s, bool m);
    SourceFile(std::string_view fn, std::string_view s, std::string_view rp, bool m);

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    std::string_view filePath {};
    std::string_view fileFolder {};
    std::string_view source {};
    std::string_view resolvedPath {};
    bool isModule {};
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

enum ETSWarnings {
    NONE,
    IMPLICIT_BOXING_UNBOXING,
    PROHIBIT_TOP_LEVEL_STATEMENTS,
    BOOST_EQUALITY_STATEMENT,
    REMOVE_LAMBDA,
    SUGGEST_FINAL,
    REMOVE_ASYNC_FUNCTIONS,
};

struct CompilerOptions {
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    bool isDebug {};
    bool isEtsModule {};
    bool isEval {};
    bool isDirectEval {};
    bool isFunctionEval {};
    bool dumpAst {};
    bool opDumpAstOnlySilent {};
    bool dumpCheckedAst {};
    bool dumpAsm {};
    bool dumpDebugInfo {};
    bool parseOnly {};
    bool verifierAllChecks {};
    bool verifierFullProgram {};
    bool debuggerEvalMode {};
    uint64_t debuggerEvalLine {};
    std::string debuggerEvalSource {};
    std::string stdLib {};
    std::vector<std::string> plugins {};
    std::vector<std::string> debuggerEvalPandaFiles {};
    std::unordered_set<std::string> skipPhases {};
    std::unordered_set<std::string> verifierWarnings {};
    std::unordered_set<std::string> verifierErrors {};
    std::unordered_set<std::string> dumpBeforePhases {};
    std::unordered_set<std::string> dumpEtsSrcBeforePhases {};
    std::string exitBeforePhase {};
    std::unordered_set<std::string> dumpAfterPhases {};
    std::unordered_set<std::string> dumpEtsSrcAfterPhases {};
    std::string exitAfterPhase {};
    std::shared_ptr<ArkTsConfig> arktsConfig {};
    CompilationMode compilationMode {};
    // NOLINTEND(misc-non-private-member-variables-in-classes)

    // ETS Warning Groups
    bool etsEnableAll {};          // Enable all ETS-warnings for System ArkTS
    bool etsSubsetWarnings {};     // Enable only ETS-warnings that keep you in subset
    bool etsNonsubsetWarnings {};  // Enable only ETS-warnings that do not keep you in subset
    bool etsHasWarnings = false;

    // Subset ETS-Warnings
    bool etsProhibitTopLevelStatements {};
    bool etsBoostEqualityStatement {};
    bool etsRemoveLambda {};
    bool etsImplicitBoxingUnboxing {};

    // Non-subset ETS-Warnings
    bool etsSuggestFinal {};
    bool etsRemoveAsync {};

    bool etsWerror {};  // Treat all enabled ETS-warnings as errors
    std::vector<ETSWarnings> etsWarningCollection = {};
};

enum class ErrorType {
    INVALID,
    GENERIC,
    SYNTAX,
    TYPE,
    ETS_WARNING,
};

class Error : public std::exception {
public:
    Error() noexcept = default;
    explicit Error(ErrorType type, std::string_view file, std::string_view message) noexcept
        : type_(type), file_(file), message_(message)
    {
    }
    explicit Error(ErrorType type, std::string_view file, std::string_view message, size_t line, size_t column) noexcept
        : type_(type), file_(file), message_(message), line_(line), col_(column)
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
            case ErrorType::ETS_WARNING:
                return "System ArkTS: warning treated as error.";
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

    const std::string &File() const noexcept
    {
        return file_;
    }

    size_t Line() const
    {
        return line_;
    }

    size_t Col() const
    {
        return col_;
    }

private:
    ErrorType type_ {ErrorType::INVALID};
    std::string file_;
    std::string message_;
    size_t line_ {};
    size_t col_ {};
    int errorCode_ {1};
};

class Compiler {
public:
    explicit Compiler(ScriptExtension ext);
    explicit Compiler(ScriptExtension ext, size_t threadCount);
    explicit Compiler(ScriptExtension ext, size_t threadCount, std::vector<util::Plugin> &&plugins);
    ~Compiler();
    NO_COPY_SEMANTIC(Compiler);
    NO_MOVE_SEMANTIC(Compiler);

    pandasm::Program *Compile(const SourceFile &input, const util::Options &options, uint32_t parseStatus = 0);

    static void DumpAsm(const pandasm::Program *prog);

    // This is used as a _different_ channel of error reporting than GetError().
    // If this is true, the errors in question have already been reported to the user.
    bool IsAnyError() const noexcept;

    const Error &GetError() const noexcept
    {
        return error_;
    }

    std::string GetPhasesList() const;

    std::vector<util::Plugin> const &Plugins()
    {
        return plugins_;
    }

private:
    std::vector<util::Plugin> const plugins_;
    compiler::CompilerImpl *compiler_;
    Error error_;
    ScriptExtension ext_;
};
}  // namespace ark::es2panda

#endif
