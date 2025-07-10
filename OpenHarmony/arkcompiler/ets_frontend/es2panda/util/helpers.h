/*
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

#ifndef ES2PANDA_UTIL_HELPERS_H
#define ES2PANDA_UTIL_HELPERS_H

#include <cmath>

#include <mem/arena_allocator.h>
#include <os/file.h>
#include <os/library_loader.h>

#include <binder/variableFlags.h>
#include <programCache.h>
#include <util/ustring.h>

namespace panda::es2panda::ir {
class Expression;
class ScriptFunction;
class ClassDefinition;
class ClassProperty;
class Identifier;
class AstNode;
class ObjectExpression;
class StringLiteral;
class Statement;
}  // namespace panda::es2panda::ir

namespace panda::es2panda {
struct CompilerOptions;
enum class ErrorType;

struct PkgInfo {
    std::string packageName {};
    std::string version {};
};

struct CompileContextInfo {
    std::vector<std::string> compileEntries;
    std::set<std::string> externalPkgNames;
    std::map<std::string, PkgInfo> pkgContextInfo;
    // The key of updateVersionInfo is the package name for an abc file, and the value contains the name of its
    // dependent pacakge and corresponding package version which need to update version.
    std::unordered_map<std::string, std::map<std::string, PkgInfo>> updateVersionInfo;
    /**
     * When there is an abc file as input and needModifyRecord is true, it is necessary to modify the recordName
     * in abc2program and modify the ohmurl for dynamic and static imports.
     **/
    bool needModifyRecord {false};
    std::string bundleName {};
};
}  // namespace panda::es2panda

namespace panda::es2panda::binder {
class Scope;
}
namespace panda::pandasm {
struct Program;
}  // namespace panda::pandasm

namespace panda::es2panda::lexer {
class LineIndex;
class SourcePosition;
}

namespace panda::es2panda::parser {
class Program;
}

namespace panda::es2panda::util {

enum class SignedNumberLiteral {
    UNRECOGNIZED = 0,
    POSITIVE = 1,
    NEGATIVE = 2
};

class FileSuffix {
public:
    static constexpr std::string_view DLL = ".dll";
    static constexpr std::string_view SO = ".so";
    static constexpr std::string_view DYLIB = ".dylib";
};

template<typename T>
class SaveValue {
public:
    explicit SaveValue(T &value) : ptr_(&value), value_(value) {}

    ~SaveValue()
    {
        *ptr_ = value_;
    }
private:
    T *ptr_;
    T value_;
};

using AopTransformFuncDef = int (*)(const char *);

class Helpers {
public:
    Helpers() = delete;

    static bool IsGlobalIdentifier(const util::StringView &str);
    static bool ContainSpreadElement(const ArenaVector<ir::Expression *> &args);
    static util::StringView LiteralToPropName(ArenaAllocator *allocator, const ir::Expression *lit);

    template <typename T>
    static bool IsInteger(double number);
    static bool IsIndex(double number);
    static int64_t GetIndex(const util::StringView &str);

    static bool FileExtensionIs(std::string_view filePath, std::string_view extension);
    static bool EndsWith(std::string_view str, std::string_view suffix);
    static std::string DoubleToString(double number);
    static int32_t GetIntegerSignificandBitCount(double number, int32_t &numberBitCount, char *significandArray);
    static void GetScientificNotationForDouble(double number, uint32_t significandBitCount, int32_t &numberBitCount,
                                               char *significandArray, char *sciNotationArray, uint32_t size);
    static std::string ToString(double number);
    static util::StringView ToStringView(ArenaAllocator *allocator, double number);
    static util::StringView ToStringView(ArenaAllocator *allocator, int32_t number);
    static util::StringView ToStringView(ArenaAllocator *allocator, uint32_t number);

    static const ir::ScriptFunction *GetContainingConstructor(const ir::AstNode *node);
    static const ir::ScriptFunction *GetContainingConstructor(const ir::ClassProperty *node);
    static const ir::ScriptFunction *GetContainingFunction(const ir::AstNode *node);
    static const ir::ClassDefinition *GetClassDefiniton(const ir::ScriptFunction *node);
    static bool IsSpecialPropertyKey(const ir::Expression *expr);
    static bool IsConstantPropertyKey(const ir::Expression *expr, bool isComputed);
    static bool IsConstantExpr(const ir::Expression *expr);
    static bool IsBindingPattern(const ir::AstNode *node);
    static bool IsPattern(const ir::AstNode *node);
    static std::vector<const ir::Identifier *> CollectBindingNames(const ir::AstNode *node);
    static util::StringView FunctionName(ArenaAllocator *allocator, const ir::ScriptFunction *func);
    static util::StringView GetName(ArenaAllocator *allocator, const ir::AstNode *node);
    static std::tuple<util::StringView, bool> ParamName(ArenaAllocator *allocator, const ir::AstNode *param,
                                                        uint32_t index);
    static bool IsChild(const ir::AstNode *parent, const ir::AstNode *child);
    static bool IsChildScope(const binder::Scope *parent, const binder::Scope *child);
    static bool IsObjectPropertyValue(const ArenaVector<ir::Expression *> &properties, const ir::AstNode *ident);
    static SignedNumberLiteral GetSignedNumberLiteral(const ir::Expression *expr);

    static void SetConstantLocalExportSlots(const std::string &record, const std::unordered_set<uint32_t> &slots);
    static void AnalysisProgram(panda::pandasm::Program *prog, const std::string &inputFile);
    static void OptimizeProgram(panda::pandasm::Program *prog, const std::string &inputFile);
    static bool CheckAopTransformPath(const std::string &libPath);
    static AopTransformFuncDef LoadAopTransformLibFunc(const std::string &libPath,
        const std::string &funcName, os::library_loader::LibraryHandle &handler);
    static bool AopTransform(const std::string &inputFile, const std::string &libPath);
    template <typename T>
    static T BaseName(T const &path, T const &delims = std::string(panda::os::file::File::GetPathDelim()));
    static bool ReadFileToBuffer(const std::string &file, std::stringstream &ss);
    static void ScanDirectives(ir::ScriptFunction *func, const lexer::LineIndex &lineIndex, bool enableSendableClass,
                               bool enableSendableFunc);
    static std::string GetHashString(const std::string &str);
    static std::wstring Utf8ToUtf16(const std::string &utf8);
    template <typename T, typename... Args>
    static T FileStream(const std::string &str, Args &&...args);
    static void ThrowError(ErrorType type, const parser::Program *program, const lexer::SourcePosition &pos,
        const std::string_view &msg);
    static bool IsUseShared(const ir::Statement *statement);
    static const ir::ClassDefinition *GetContainingSendableClass(const ir::AstNode *node);
    static bool IsSpecialScopeName(const util::StringView &str);
    static bool BelongingToRecords(const std::string &name, const std::unordered_set<std::string> &retainRecordSet,
                                   const std::string &delimiter = std::string(DOT));
    static bool IsInnerAnnotationRecordName(const std::string &name);
    static std::string RemoveRecordSuffixAnnotationName(const std::string &name);
    static void RemoveProgramsRedundantData(std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo,
        const std::map<std::string, std::unordered_set<std::string>> &resolveDepsRelation);
    static bool IsDefaultApiVersion(int apiVersion, std::string subApiVersion);
    static bool IsSupportLazyImportVersion(int apiVersion, std::string subApiVersion);
    static bool IsEnableExpectedPropertyCountApiVersion(int apiVersion);
    static bool IsSupportAnnotationVersion(int apiVersion);

    static const uint32_t MAX_DOUBLE_DIGIT = 310;
    static const uint32_t MAX_DOUBLE_PRECISION_DIGIT = 17;
    static const int32_t MAX_DECIMAL_EXPONENT = 21;
    static const int32_t MIN_DECIMAL_EXPONENT = -6;
    static const int32_t FAIL_SNPRINTF_S  = -1;
    static const uint32_t INVALID_INDEX = 4294967295L;
    static const uint32_t MAX_INT32 = 2147483647;
    static const uint32_t MAX_INT16 = std::numeric_limits<int16_t>::max();
    static const uint32_t MAX_INT8 = std::numeric_limits<int8_t>::max();
    static constexpr std::string_view USE_CONCURRENT = "use concurrent";
    static constexpr std::string_view USE_SENDABLE = "use sendable";
    static constexpr std::string_view USE_SHARED = "use shared";
    static constexpr std::string_view STRING_EMPTY = ""; // Default tag value, or tag of GlobalScope and ModuleScope
    static constexpr std::string_view CLASS_SCOPE_TAG = "~";
    static constexpr std::string_view FUNCTION_TAG = "*";
    static constexpr std::string_view METHOD_TAG = ">";
    static constexpr std::string_view CTOR_TAG = "=";
    static constexpr std::string_view NAMESPACE_TAG = "&";
    static constexpr std::string_view ENUM_TAG = "%";
    static constexpr std::string_view STATIC_METHOD_TAG = "<";
    static constexpr std::string_view DUPLICATED_SEPERATOR = "^";
    static constexpr std::string_view FUNC_NAME_SEPARATOR = "#";
    static constexpr std::string_view INDEX_NAME_SPICIFIER = "@";
    static constexpr std::string_view DOT = ".";
    static constexpr std::string_view BACKSLASH = "\\";
    static const uint64_t FNV_PRIME = 1099511628211U;
    static const uint64_t FNV_OFFSET = 14695981039346656037U;
    static const uint8_t SENDABLE_CLASS_MIN_SUPPORTED_API_VERSION = 11;
    static const int32_t DEFAULT_TARGET_API_VERSION = 12;
    static const int32_t ABC_TO_PROGRAM_MIN_SUPPORTED_API_VERSION = 12;
    static constexpr std::array<uint8_t, panda_file::File::VERSION_SIZE>
        ABC_TO_PROGRAM_MIN_SUPPORTED_BYTECODE_VERSION {12, 0, 2, 0};
    static const int32_t SENDABLE_FUNCTION_MIN_SUPPORTED_API_VERSION = 12;
    static const int32_t LAZY_IMPORT_MIN_SUPPORTED_API_VERSION = 12;
    static const int32_t SENDABLE_LAZY_LOADING_MIN_SUPPORTED_API_VERSION = 12;
    static const int8_t SUPER_CALL_OPT_MIN_SUPPORTED_API_VERSION = 18;
    static const int8_t SENDABLE_CLASS_USING_LOCAL_MODULE_VAR_MIN_SUPPORTED_API_VERSION = 18;
    static const int8_t ENABLE_EXPECTED_PROPERTY_COUNT_MIN_SUPPORTED_API_VERSION = 18;
    static const int8_t ANNOTATION_SUPPORTED_API_VERSION = 20;
    static constexpr std::string_view SUB_API_VERSION_1 = "beta1";
    static constexpr std::string_view SUB_API_VERSION_2 = "beta2";
    static constexpr std::string_view DEFAULT_SUB_API_VERSION = SUB_API_VERSION_1;

private:
    static bool SetFuncFlagsForDirectives(const ir::StringLiteral *strLit, ir::ScriptFunction *func,
                                          const lexer::LineIndex &lineIndex, bool enableSendableClass,
                                          bool enableSendableFunc);
};

template <typename T>
bool Helpers::IsInteger(double number)
{
    if (std::fabs(number) <= static_cast<double>(std::numeric_limits<T>::max())) {
        T intNum = static_cast<T>(number);

        if (static_cast<double>(intNum) == number) {
            return true;
        }
    }

    return false;
}

template <class T>
T Helpers::BaseName(T const &path, T const &delims)
{
    return path.substr(path.find_last_of(delims) + 1);
}

template <typename T, typename... Args>
T Helpers::FileStream(const std::string &str, Args &&...args)
{
    T fileStream;
#ifdef PANDA_TARGET_WINDOWS
    std::wstring filename = Helpers::Utf8ToUtf16(str);
#else  //for linux and mac
    std::string filename = str;
#endif
    fileStream.open(filename.c_str(), args...);
    return fileStream;
}

}  // namespace panda::es2panda::util

#endif
