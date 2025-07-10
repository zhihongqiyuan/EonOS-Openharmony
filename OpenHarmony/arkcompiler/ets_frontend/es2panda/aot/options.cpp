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

#include "options.h"

#include "bytecode_optimizer/bytecodeopt_options.h"
#include "compiler_options.h"
#include "utils/timers.h"

#include "mergeProgram.h"

namespace panda::es2panda::aot {
constexpr char PROCESS_AS_LIST_MARK = '@';
// item list: [filePath; recordName; moduleKind; sourceFile; pkgName; isSharedModule; sourceLang]
constexpr size_t ITEM_COUNT_MERGE = 7;
// item list: [filePath; recordName; moduleKind; sourceFile; outputfile; isSharedModule; sourceLang]
constexpr size_t ITEM_COUNT_NOT_MERGE = 7;
const std::string LIST_ITEM_SEPERATOR = ";";
const std::set<std::string> VALID_EXTENSIONS = { "js", "ts", "as", "abc" };

template <class T>
T RemoveExtension(T const &filename)
{
    typename T::size_type const P(filename.find_last_of('.'));
    return P > 0 && P != T::npos ? filename.substr(0, P) : filename;
}

static es2panda::ScriptExtension GetScriptExtensionFromStr(const std::string &extension)
{
    if (extension == "js") {
        return es2panda::ScriptExtension::JS;
    } else if (extension == "ts") {
        return es2panda::ScriptExtension::TS;
    } else if (extension == "as") {
        return es2panda::ScriptExtension::AS;
    } else if (extension == "abc") {
        return es2panda::ScriptExtension::ABC;
    } else {
        return es2panda::ScriptExtension::JS;
    }
}

static es2panda::ScriptExtension GetScriptExtension(const std::string &filename, const std::string &inputExtension)
{
    std::string fileExtension = "";
    std::string::size_type pos(filename.find_last_of('.'));
    if (pos > 0 && pos != std::string::npos) {
        fileExtension = filename.substr(pos + 1);
    }

    if (VALID_EXTENSIONS.find(fileExtension) != VALID_EXTENSIONS.end()) {
        return GetScriptExtensionFromStr(fileExtension);
    }

    return GetScriptExtensionFromStr(inputExtension);
}

static std::vector<std::string> GetStringItems(std::string &input, const std::string &delimiter)
{
    std::vector<std::string> items;
    size_t pos = 0;
    std::string token;
    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        if (!token.empty()) {
            items.push_back(token);
        }
        input.erase(0, pos + delimiter.length());
    }
    if (!input.empty()) {
        items.push_back(input);
    }
    return items;
}

void Options::CollectInputAbcFile(const std::vector<std::string> &itemList, const std::string &inputExtension)
{
    std::string fileName = itemList[0];
    // Split the fileInfo string to itemList by the delimiter ';'. If the element is empty, it will not be added to
    // the itemList. The fileInfo string of the abc file only contains the file path and pkgName, so the index
    // of pkgName is 1.
    constexpr uint32_t PKG_NAME_IDX = 1;
    es2panda::SourceFile src(fileName, "", parser::ScriptKind::SCRIPT, GetScriptExtension(fileName,
                             inputExtension));
    src.isSourceMode = false;
    if (itemList.size() > PKG_NAME_IDX && compilerOptions_.mergeAbc) {
        src.pkgName = itemList[PKG_NAME_IDX];
    }
    sourceFiles_.push_back(src);
}

void Options::CollectInputSourceFile(const std::vector<std::string> &itemList, const std::string &inputExtension)
{
    std::string fileName = itemList[0];
    std::string recordName = compilerOptions_.mergeAbc ? itemList[1] : "";
    constexpr uint32_t SCRIPT_KIND_IDX = 2;
    constexpr uint32_t SOURCE_FIEL_IDX = 3;
    constexpr uint32_t PKG_NAME_IDX = 4;
    constexpr uint32_t IS_SHARED_MODULE_IDX = 5;
    constexpr uint32_t ORIGIN_SOURCE_LANG_IDX = 6;
    parser::ScriptKind scriptKind;
    if (itemList[SCRIPT_KIND_IDX] == "script") {
        scriptKind = parser::ScriptKind::SCRIPT;
    } else if (itemList[SCRIPT_KIND_IDX] == "commonjs") {
        scriptKind = parser::ScriptKind::COMMONJS;
    } else {
        scriptKind = parser::ScriptKind::MODULE;
    }

    es2panda::SourceFile src(fileName, recordName, scriptKind, GetScriptExtension(fileName, inputExtension));
    src.sourcefile = itemList[SOURCE_FIEL_IDX];
    if (compilerOptions_.mergeAbc) {
        src.pkgName = itemList[PKG_NAME_IDX];
    }

    if (itemList.size() > IS_SHARED_MODULE_IDX) {
        src.isSharedModule = itemList[IS_SHARED_MODULE_IDX] == "true";
    }

    if (itemList.size() > ORIGIN_SOURCE_LANG_IDX) {
        src.sourceLang = itemList[ORIGIN_SOURCE_LANG_IDX];
    }

    sourceFiles_.push_back(src);
    if (!compilerOptions_.mergeAbc) {
        outputFiles_.insert({fileName, itemList[PKG_NAME_IDX]});
    }
}

bool Options::CheckFilesValidity(const std::string &input, const std::vector<std::string> &itemList,
                                 const std::string &line)
{
    // For compatibility, only throw error when item list's size is bigger than given size.
    if ((compilerOptions_.mergeAbc && itemList.size() > ITEM_COUNT_MERGE) ||
        (!compilerOptions_.mergeAbc && itemList.size() > ITEM_COUNT_NOT_MERGE) || itemList.empty()) {
        std::cerr << "Failed to parse line " << line << " of the input file: '"
            << input << "'." << std::endl
            << "Expected " << (compilerOptions_.mergeAbc ? ITEM_COUNT_MERGE : ITEM_COUNT_NOT_MERGE)
            << " items per line, but found " << itemList.size() << " items." << std::endl
            << "Please check the file format and content for correctness." << std::endl;
        return false;
    }
    return true;
}

bool Options::IsAbcFile(const std::string &fileName, const std::string &inputExtension)
{
    return (GetScriptExtension(fileName, inputExtension) == es2panda::ScriptExtension::ABC);
}

// Options
bool Options::CollectInputFilesFromFileList(const std::string &input, const std::string &inputExtension)
{
    std::ifstream ifs;
    std::string line;
    ifs.open(panda::os::file::File::GetExtendedFilePath(input));
    if (!ifs.is_open()) {
        std::cerr << "Failed to open source list file '" << input << "' during input file collection." << std::endl
                  << "Please check if the file exists or the path is correct, "
                  << "and you have the necessary permissions to access it." << std::endl;
        return false;
    }

    while (std::getline(ifs, line)) {
        std::vector<std::string> itemList = GetStringItems(line, LIST_ITEM_SEPERATOR);
        if (!CheckFilesValidity(input, itemList, line)) {
            return false;
        }
        if (IsAbcFile(itemList[0], inputExtension)) {
            CollectInputAbcFile(itemList, inputExtension);
        } else {
            CollectInputSourceFile(itemList, inputExtension);
        }
    }
    return true;
}

bool Options::CollectInputFilesFromFileDirectory(const std::string &input, const std::string &extension)
{
    std::vector<std::string> files;
    if (!proto::MergeProgram::GetProtoFiles(input, extension, files)) {
        return false;
    }

    for (const auto &f : files) {
        es2panda::SourceFile src(f, RemoveExtension(f.substr(input.length() + 1)),
                                 scriptKind_, GetScriptExtensionFromStr(extension));
        sourceFiles_.push_back(src);
    }

    return true;
}

void Options::ParseCacheFileOption(const std::string &cacheInput)
{
    if (cacheInput[0] != PROCESS_AS_LIST_MARK) {
        compilerOptions_.cacheFiles.insert({sourceFile_, cacheInput});
        return;
    }

    std::ifstream ifs;
    std::string line;
    ifs.open(panda::os::file::File::GetExtendedFilePath(cacheInput.substr(1)));
    if (!ifs.is_open()) {
        std::cerr << "Failed to open cache file list from the provided path: '" << cacheInput << "'." << std::endl
                  << "Please check if the file exists or the path is correct, "
                  << "and you have the necessary permissions to read the file." << std::endl;
        return;
    }

    constexpr int cacheListItemCount = 2;
    while (std::getline(ifs, line)) {
        std::vector<std::string> itemList = GetStringItems(line, LIST_ITEM_SEPERATOR);
        if (itemList.size() != cacheListItemCount) {
            continue;
        }
        compilerOptions_.cacheFiles.insert({itemList[0], itemList[1]});
    }
}

void Options::ParseUpdateVersionInfo(nlohmann::json &compileContextInfoJson)
{
    if (compileContextInfoJson.contains("updateVersionInfo") &&
        compileContextInfoJson["updateVersionInfo"].is_object()) {
        std::unordered_map<std::string, std::map<std::string, PkgInfo>> updateVersionInfo {};
        for (const auto& [abcName, versionInfo] : compileContextInfoJson["updateVersionInfo"].items()) {
            if (!versionInfo.is_object()) {
                std::cerr << "The input file '" << compilerOptions_.compileContextInfoPath
                          << "' is incomplete format of json" << std::endl;
            }
            std::map<std::string, PkgInfo> pkgContextMap {};
            for (const auto& [pkgName, version] : versionInfo.items()) {
                PkgInfo pkgInfo;
                pkgInfo.version = version;
                pkgInfo.packageName = pkgName;
                pkgContextMap[pkgName] = pkgInfo;
            }
            updateVersionInfo[abcName] = pkgContextMap;
        }
        compilerOptions_.compileContextInfo.updateVersionInfo = updateVersionInfo;
    } else if (compileContextInfoJson.contains("pkgContextInfo") &&
               compileContextInfoJson["pkgContextInfo"].is_object()) {
        std::map<std::string, PkgInfo> pkgContextMap {};
        for (const auto& [pkgName, pkgContextInfo] : compileContextInfoJson["pkgContextInfo"].items()) {
            PkgInfo pkgInfo;
            if (pkgContextInfo.contains("version") && pkgContextInfo["version"].is_string()) {
                pkgInfo.version = pkgContextInfo["version"];
            } else {
                std::cerr << "Failed to get version from pkgContextInfo."  << std::endl;
            }
            if (pkgContextInfo.contains("packageName") && pkgContextInfo["packageName"].is_string()) {
                pkgInfo.packageName = pkgContextInfo["packageName"];
            } else {
                std::cerr << "Failed to get package name from pkgContextInfo."  << std::endl;
            }
            pkgContextMap[pkgName] = pkgInfo;
        }
        compilerOptions_.compileContextInfo.pkgContextInfo = pkgContextMap;
    } else {
        UNREACHABLE();
    }
}

void Options::ParseCompileContextInfo(const std::string compileContextInfoPath)
{
    std::stringstream ss;
    std::string buffer;
    if (!util::Helpers::ReadFileToBuffer(compileContextInfoPath, ss)) {
        return;
    }

    buffer = ss.str();
    if (buffer.empty() || !nlohmann::json::accept(buffer)) {
        std::cerr << "The input file '" << compileContextInfoPath <<"' is incomplete format of json" << std::endl;
        return;
    }
    // Parser compile context info base on the input json file.
    nlohmann::json compileContextInfoJson = nlohmann::json::parse(buffer);
    if (!compileContextInfoJson.contains("compileEntries") || !compileContextInfoJson.contains("hspPkgNames")) {
        std::cerr << "The input json file '" << compileContextInfoPath << "' content format is incorrect" << std::endl;
        return;
    }
    if (!compileContextInfoJson["compileEntries"].is_array() || !compileContextInfoJson["hspPkgNames"].is_array()) {
        std::cerr << "The input json file '" << compileContextInfoPath << "' content type is incorrect" << std::endl;
        return;
    }
    if (compileContextInfoJson.contains("needModifyRecord") &&
        compileContextInfoJson["needModifyRecord"].is_boolean()) {
        compilerOptions_.compileContextInfo.needModifyRecord = compileContextInfoJson["needModifyRecord"];
    }
    if (compileContextInfoJson.contains("bundleName") &&
        compileContextInfoJson["bundleName"].is_string()) {
        compilerOptions_.compileContextInfo.bundleName = compileContextInfoJson["bundleName"];
    }
    std::set<std::string> externalPkgNames;
    for (const auto& elem : compileContextInfoJson["hspPkgNames"]) {
        if (elem.is_string()) {
            externalPkgNames.insert(elem.get<std::string>());
        }
    }
    compilerOptions_.compileContextInfo.externalPkgNames = externalPkgNames;
    compilerOptions_.compileContextInfo.compileEntries = compileContextInfoJson["compileEntries"];
    ParseUpdateVersionInfo(compileContextInfoJson);
}

// Collect dependencies based on the compile entries.
bool Options::NeedCollectDepsRelation()
{
    return compilerOptions_.enableAbcInput && !compilerOptions_.compileContextInfo.compileEntries.empty();
}

// Remove redundant content from the abc file and remove programs generated from redundant source files.
bool Options::NeedRemoveRedundantRecord()
{
    return compilerOptions_.removeRedundantFile && NeedCollectDepsRelation();
}

Options::Options() : argparser_(new panda::PandArgParser()) {}

Options::~Options()
{
    delete argparser_;
}

bool Options::Parse(int argc, const char **argv)
{
    panda::PandArg<bool> opHelp("help", false, "Print this message and exit");

    // parser
    panda::PandArg<std::string> inputExtension("extension", "js",
                                               "Parse the input as the given extension (options: js | ts | as | abc)");
    panda::PandArg<bool> opModule("module", false, "Parse the input as module");
    panda::PandArg<bool> opCommonjs("commonjs", false, "Parse the input as commonjs");
    panda::PandArg<bool> opParseOnly("parse-only", false, "Parse the input only");
    panda::PandArg<bool> opEnableTypeCheck("enable-type-check", false, "Check the type in ts after parse");
    panda::PandArg<bool> opDumpAst("dump-ast", false, "Dump the parsed AST");
    panda::PandArg<bool> opDumpTransformedAst("dump-transformed-ast", false, "Dump the parsed AST after transform");
    panda::PandArg<bool> opCheckTransformedAstStructure("check-transformed-ast-structure", false,
                                                        "Check the AST structure after transform");
    panda::PandArg<bool> opRecordDebugSource("record-debug-source", false, "Record source code to support "\
        "multi-platform debugger & detailed backtrace in debug mode");

    // compiler
    panda::PandArg<bool> opEnableAbcInput("enable-abc-input", false, "Allow abc file as input");
    panda::PandArg<bool> opDumpAsmProgram("dump-asm-program", false, "Dump program");
    std::string descOfDumpNormalizedProg =
        "Dump program in normalized form to ensure the output of source code compilation is consistent with that of "
        "abc file compilation.\n"
        "  The normalized form differs mainly as follows:\n"
        "  1. all instructions will be labled consecutively and all the labels will be dumped\n"
        "  2. the content of a literal array, rather than its id, will be dumped when the literal array appears in "
        "an opcode or is nested in another literal array\n"
        "  3. labels stored in catch blocks will be unified\n"
        "  4. strings won't be dumped\n"
        "  5. invalid opcodes won't be dumped, local variables' start and end offset will skip invalid opcodes";
    panda::PandArg<bool> opDumpNormalizedAsmProgram("dump-normalized-asm-program", false, descOfDumpNormalizedProg);
    panda::PandArg<bool> opDumpAssembly("dump-assembly", false, "Dump pandasm");
    panda::PandArg<bool> opDebugInfo("debug-info", false, "Compile with debug info");
    panda::PandArg<bool> opDumpDebugInfo("dump-debug-info", false, "Dump debug info");
    panda::PandArg<int> opOptLevel("opt-level", 2,
        "Compiler optimization level (options: 0 | 1 | 2). In debug and base64Input mode, optimizer is disabled");
    panda::PandArg<int> opFunctionThreadCount("function-threads", 0, "Number of worker threads to compile function");
    panda::PandArg<int> opFileThreadCount("file-threads", 0, "Number of worker threads to compile file");
    panda::PandArg<int> opAbcClassThreadCount("abc-class-threads", 4,
        "Number of worker threads to compile classes of abc file");
    panda::PandArg<bool> opSizeStat("dump-size-stat", false, "Dump size statistics");
    panda::PandArg<bool> opSizePctStat("dump-file-item-size", false, "Dump the size of each kind of file item "\
        "of the abc file");
    panda::PandArg<bool> opDumpLiteralBuffer("dump-literal-buffer", false, "Dump literal buffer");
    panda::PandArg<std::string> outputFile("output", "", "Compiler binary output (.abc)");
    panda::PandArg<std::string> recordName("record-name", "", "Specify the record name");
    panda::PandArg<bool> debuggerEvaluateExpression("debugger-evaluate-expression", false,
                                                    "evaluate expression in debugger mode");
    panda::PandArg<std::string> base64Input("base64Input", "", "base64 input of js content");
    panda::PandArg<bool> base64Output("base64Output", false, "output panda file content as base64 to std out");
    panda::PandArg<std::string> sourceFile("source-file", "",
                                           "specify the file path info recorded in generated abc");
    panda::PandArg<std::string> outputProto("outputProto", "",
                                            "specify the output name for serializd protobuf file (.protoBin)");
    panda::PandArg<std::string> opCacheFile("cache-file", "", "cache file for incremental compile");
    panda::PandArg<std::string> opNpmModuleEntryList("npm-module-entry-list", "", "entry list file for module compile");
    panda::PandArg<bool> opMergeAbc("merge-abc", false, "Compile as merge abc");
    panda::PandArg<std::string> opPerfFile("perf-file", "perf.txt", "Specify the file path to dump time consuming data"\
        " during compilation process, default to 'perf.txt' in the current directory");
    panda::PandArg<int> opPerfLevel("perf-level", 0, "Specify the performance data output level:"\
        "  0: Output compilation time data(default)");
    panda::PandArg<bool> opuseDefineSemantic("use-define-semantic", false, "Compile ts class fields "\
        "in accordance with ECMAScript2022");
    panda::PandArg<std::string> moduleRecordFieldName("module-record-field-name", "", "Specify the field name "\
        "of module record in unmerged abc");

    // optimizer
    panda::PandArg<bool> opBranchElimination("branch-elimination", false, "Enable branch elimination optimization");
    panda::PandArg<bool> opOptTryCatchFunc("opt-try-catch-func", true, "Enable optimizations for functions with "\
        "try-catch blocks");

    // patchfix && hotreload
    panda::PandArg<std::string> opDumpSymbolTable("dump-symbol-table", "", "dump symbol table to file");
    panda::PandArg<std::string> opInputSymbolTable("input-symbol-table", "", "input symbol table file");
    panda::PandArg<bool> opGeneratePatch("generate-patch", false, "generate patch abc, default as hotfix mode unless "\
        "the cold-fix argument is set");
    panda::PandArg<bool> opHotReload("hot-reload", false, "compile as hot-reload mode");
    panda::PandArg<bool> opColdReload("cold-reload", false, "compile as cold-reload mode");
    panda::PandArg<bool> opColdFix("cold-fix", false, "generate patch abc as cold-fix mode");

    // version
    panda::PandArg<bool> bcVersion("bc-version", false, "Print ark bytecode version. If both bc-version and"\
        "bc-min-version are enabled, only bc-version will take effects");
    panda::PandArg<bool> bcMinVersion("bc-min-version", false, "Print ark bytecode minimum supported version");
    // todo(huyunhui): change default api verion to 0 after refactoring
    // Current api version is 20
    panda::PandArg<int> targetApiVersion("target-api-version", 20,
        "Specify the targeting api version for es2abc to generated the corresponding version of bytecode");
    panda::PandArg<bool> targetBcVersion("target-bc-version", false, "Print the corresponding ark bytecode version"\
        "for target api version. If both target-bc-version and bc-version are enabled, only target-bc-version"\
        "will take effects");
    panda::PandArg<std::string> targetApiSubVersion("target-api-sub-version",
        std::string {util::Helpers::DEFAULT_SUB_API_VERSION},
        "Specify the targeting api sub version for es2abc to generated the corresponding version of bytecode");
    panda::PandArg<bool> enableAnnotations("enable-annotations", false, "Permits es2abc to compile annotations");

    // compile entries and pkg context info
    panda::PandArg<std::string> compileContextInfoPath("compile-context-info", "", "The path to compile context"\
        "info file");
    panda::PandArg<bool> opDumpDepsInfo("dump-deps-info", false, "Dump all dependency files and records "\
        "including source files and bytecode files");
    panda::PandArg<bool> opRemoveRedundantFile("remove-redundant-file", false, "Remove redundant info"\
        " from abc file and remove redundant source file, which is effective when the compile-context-info switch"\
        "  is turned on and there is abc input");
    panda::PandArg<bool> opDumpString("dump-string", false, "Dump program strings");
    panda::PandArg<std::string> srcPkgName("src-package-name", "", "This is for modify pacakge name in input abc"\
        " file and should aways be used with dstPkgName. srcPkgName is for finding the targeting package name to be"\
        " modified.");
    panda::PandArg<std::string> dstPkgName("dst-package-name", "", "This is for modify pacakge name in input abc"\
        " file, and should always be used with srcPkgName. dstPkgName what targeting package name will be"\
        " modified to.");

    // aop transform
    panda::PandArg<std::string> transformLib("transform-lib", "", "aop transform lib file path");

    // tail arguments
    panda::PandArg<std::string> inputFile("input", "", "input file");

    argparser_->Add(&opHelp);
    argparser_->Add(&opModule);
    argparser_->Add(&opCommonjs);
    argparser_->Add(&opDumpAst);
    argparser_->Add(&opDumpTransformedAst);
    argparser_->Add(&opCheckTransformedAstStructure);
    argparser_->Add(&opRecordDebugSource);
    argparser_->Add(&opParseOnly);
    argparser_->Add(&opEnableTypeCheck);
    argparser_->Add(&opEnableAbcInput);
    argparser_->Add(&opDumpAsmProgram);
    argparser_->Add(&opDumpNormalizedAsmProgram);
    argparser_->Add(&opDumpAssembly);
    argparser_->Add(&opDebugInfo);
    argparser_->Add(&opDumpDebugInfo);
    argparser_->Add(&debuggerEvaluateExpression);
    argparser_->Add(&base64Input);
    argparser_->Add(&base64Output);

    argparser_->Add(&opOptLevel);
    argparser_->Add(&opFunctionThreadCount);
    argparser_->Add(&opFileThreadCount);
    argparser_->Add(&opAbcClassThreadCount);
    argparser_->Add(&opSizeStat);
    argparser_->Add(&opSizePctStat);
    argparser_->Add(&opDumpLiteralBuffer);

    argparser_->Add(&inputExtension);
    argparser_->Add(&outputFile);
    argparser_->Add(&sourceFile);
    argparser_->Add(&recordName);
    argparser_->Add(&outputProto);
    argparser_->Add(&opCacheFile);
    argparser_->Add(&opNpmModuleEntryList);
    argparser_->Add(&opMergeAbc);
    argparser_->Add(&opPerfLevel);
    argparser_->Add(&opPerfFile);
    argparser_->Add(&opuseDefineSemantic);
    argparser_->Add(&moduleRecordFieldName);
    argparser_->Add(&opBranchElimination);
    argparser_->Add(&opOptTryCatchFunc);

    argparser_->Add(&opDumpSymbolTable);
    argparser_->Add(&opInputSymbolTable);
    argparser_->Add(&opGeneratePatch);
    argparser_->Add(&opHotReload);
    argparser_->Add(&opColdReload);
    argparser_->Add(&opColdFix);

    argparser_->Add(&bcVersion);
    argparser_->Add(&bcMinVersion);
    argparser_->Add(&targetApiVersion);
    argparser_->Add(&targetBcVersion);
    argparser_->Add(&targetApiSubVersion);
    argparser_->Add(&enableAnnotations);

    argparser_->Add(&compileContextInfoPath);
    argparser_->Add(&opDumpDepsInfo);
    argparser_->Add(&opRemoveRedundantFile);
    argparser_->Add(&opDumpString);

    argparser_->Add(&transformLib);

    argparser_->Add(&srcPkgName);
    argparser_->Add(&dstPkgName);

    argparser_->PushBackTail(&inputFile);
    argparser_->EnableTail();
    argparser_->EnableRemainder();

    bool parseStatus = argparser_->Parse(argc, argv);

    compilerOptions_.targetApiVersion = targetApiVersion.GetValue();
    compilerOptions_.targetApiSubVersion = targetApiSubVersion.GetValue();
    if (parseStatus && targetBcVersion.GetValue()) {
        compilerOptions_.targetBcVersion = targetBcVersion.GetValue();
        return true;
    }

    if (parseStatus && (bcVersion.GetValue() || bcMinVersion.GetValue())) {
        compilerOptions_.bcVersion = bcVersion.GetValue();
        compilerOptions_.bcMinVersion = bcMinVersion.GetValue();
        return true;
    }

    if (!parseStatus || opHelp.GetValue() || (inputFile.GetValue().empty() && base64Input.GetValue().empty())) {
        std::stringstream ss;

        ss << argparser_->GetErrorString() << std::endl;
        ss << "Usage: "
           << "es2panda"
           << " [OPTIONS] [input file] -- [arguments]" << std::endl;
        ss << std::endl;
        ss << "optional arguments:" << std::endl;
        ss << argparser_->GetHelpString() << std::endl;

        errorMsg_ = ss.str();
        return false;
    }

    bool inputIsEmpty = inputFile.GetValue().empty();
    bool base64InputIsEmpty = base64Input.GetValue().empty();
    bool outputIsEmpty = outputFile.GetValue().empty();

    if (!inputIsEmpty && !base64InputIsEmpty) {
        errorMsg_ = "--input and --base64Input can not be used simultaneously";
        return false;
    }

    if (!outputIsEmpty && base64Output.GetValue()) {
        errorMsg_ = "--output and --base64Output can not be used simultaneously";
        return false;
    }

    if (opModule.GetValue() && opCommonjs.GetValue()) {
        errorMsg_ = "[--module] and [--commonjs] can not be used simultaneously";
        return false;
    }

    if (opModule.GetValue()) {
        scriptKind_ = es2panda::parser::ScriptKind::MODULE;
    } else if (opCommonjs.GetValue()) {
        scriptKind_ = es2panda::parser::ScriptKind::COMMONJS;
    } else {
        scriptKind_ = es2panda::parser::ScriptKind::SCRIPT;
    }

    std::string extension = inputExtension.GetValue();
    if (!extension.empty()) {
        if (VALID_EXTENSIONS.find(extension) == VALID_EXTENSIONS.end()) {
            errorMsg_ = "Invalid extension (available options: js, ts, as, abc)";
            return false;
        }
    }

    bool isInputFileList = false;
    if (!inputIsEmpty) {
        std::string rawInput = inputFile.GetValue();
        isInputFileList = rawInput[0] == PROCESS_AS_LIST_MARK;
        std::string input = isInputFileList ? rawInput.substr(1) : rawInput;
        sourceFile_ = input;
    }

    if (base64Output.GetValue()) {
        compilerOutput_ = "";
    } else if (!outputIsEmpty) {
        compilerOutput_ = outputFile.GetValue();
    } else if (outputIsEmpty && !inputIsEmpty) {
        compilerOutput_ = RemoveExtension(util::Helpers::BaseName(sourceFile_)).append(".abc");
    }

    if (opMergeAbc.GetValue()) {
        recordName_ = recordName.GetValue();
        if (recordName_.empty()) {
            recordName_ = compilerOutput_.empty() ? "Base64Output" :
                RemoveExtension(util::Helpers::BaseName(compilerOutput_));
        }
        compilerOptions_.mergeAbc = opMergeAbc.GetValue();
    }

    if (opuseDefineSemantic.GetValue()) {
        compilerOptions_.useDefineSemantic = opuseDefineSemantic.GetValue();
    }

    if (!inputIsEmpty) {
        // common mode
        auto inputAbs = panda::os::file::File::GetAbsolutePath(sourceFile_);
        if (!inputAbs) {
            std::cerr << "Failed to find file '" << sourceFile_ << "' during input file resolution" << std::endl
                      << "Please check if the file name is correct, the file exists at the specified path, "
                      << "and your project has the necessary permissions to access it." << std::endl;
            return false;
        }

        auto fpath = inputAbs.Value();
        if (isInputFileList) {
            CollectInputFilesFromFileList(fpath, extension);
        } else if (panda::os::file::File::IsDirectory(fpath)) {
            CollectInputFilesFromFileDirectory(fpath, extension);
        } else {
            es2panda::SourceFile src(sourceFile_, recordName_, scriptKind_, GetScriptExtension(sourceFile_, extension));
            src.isSourceMode = !IsAbcFile(sourceFile_, extension);
            sourceFiles_.push_back(src);
        }
    } else if (!base64InputIsEmpty) {
        // input content is base64 string
        base64Input_ = ExtractContentFromBase64Input(base64Input.GetValue());
        if (base64Input_.empty()) {
            errorMsg_ = "The input string is not a valid base64 data";
            return false;
        }

        es2panda::SourceFile src("", recordName_, es2panda::parser::ScriptKind::SCRIPT,
                                 GetScriptExtensionFromStr(extension));
        src.source = base64Input_;
        sourceFiles_.push_back(src);
    }

    if (!outputProto.GetValue().empty()) {
        compilerProtoOutput_ = outputProto.GetValue();
    }

    optLevel_ = opOptLevel.GetValue();
    functionThreadCount_ = opFunctionThreadCount.GetValue();
    fileThreadCount_ = opFileThreadCount.GetValue();
    abcClassThreadCount_ = opAbcClassThreadCount.GetValue();
    npmModuleEntryList_ = opNpmModuleEntryList.GetValue();

    if (!opCacheFile.GetValue().empty()) {
        ParseCacheFileOption(opCacheFile.GetValue());
    }

    if (opParseOnly.GetValue()) {
        options_ |= OptionFlags::PARSE_ONLY;
    }

    if (opSizeStat.GetValue()) {
        options_ |= OptionFlags::SIZE_STAT;
    }

    if (opSizePctStat.GetValue()) {
        options_ |= OptionFlags::SIZE_PCT_STAT;
    }

    perfFile_ = "";
    perfLevel_ = opPerfLevel.GetValue();
    if (opPerfFile.WasSet() && (perfLevel_ == 0)) {
        perfFile_ = opPerfFile.GetValue().empty() ? opPerfFile.GetDefaultValue() : opPerfFile.GetValue();
    }
    panda::Timer::InitializeTimer(perfFile_);

    compilerOptions_.recordDebugSource = opRecordDebugSource.GetValue();
    compilerOptions_.enableAbcInput = opEnableAbcInput.GetValue();
    compilerOptions_.dumpAsmProgram = opDumpAsmProgram.GetValue();
    compilerOptions_.dumpNormalizedAsmProgram = opDumpNormalizedAsmProgram.GetValue();
    compilerOptions_.dumpAsm = opDumpAssembly.GetValue();
    compilerOptions_.dumpAst = opDumpAst.GetValue();
    compilerOptions_.dumpTransformedAst = opDumpTransformedAst.GetValue();
    compilerOptions_.checkTransformedAstStructure = opCheckTransformedAstStructure.GetValue();
    compilerOptions_.dumpDebugInfo = opDumpDebugInfo.GetValue();
    compilerOptions_.isDebug = opDebugInfo.GetValue();
    compilerOptions_.parseOnly = opParseOnly.GetValue();
    compilerOptions_.enableTypeCheck = opEnableTypeCheck.GetValue();
    compilerOptions_.dumpLiteralBuffer = opDumpLiteralBuffer.GetValue();
    compilerOptions_.isDebuggerEvaluateExpressionMode = debuggerEvaluateExpression.GetValue();

    compilerOptions_.functionThreadCount = functionThreadCount_;
    compilerOptions_.fileThreadCount = fileThreadCount_;
    compilerOptions_.abcClassThreadCount = abcClassThreadCount_;
    compilerOptions_.output = compilerOutput_;
    compilerOptions_.debugInfoSourceFile = sourceFile.GetValue();
    compilerOptions_.optLevel = (compilerOptions_.isDebug || !base64Input.GetValue().empty() ||
        base64Output.GetValue()) ? 0 : opOptLevel.GetValue();
    compilerOptions_.sourceFiles = sourceFiles_;
    compilerOptions_.mergeAbc = opMergeAbc.GetValue();
    compilerOptions_.compileContextInfoPath = compileContextInfoPath.GetValue();
    if (!compileContextInfoPath.GetValue().empty()) {
        ParseCompileContextInfo(compileContextInfoPath.GetValue());
    }
    compilerOptions_.dumpDepsInfo = opDumpDepsInfo.GetValue();
    compilerOptions_.updatePkgVersionForAbcInput = compilerOptions_.enableAbcInput &&
        (!compilerOptions_.compileContextInfo.pkgContextInfo.empty() ||
        !compilerOptions_.compileContextInfo.updateVersionInfo.empty());
    compilerOptions_.removeRedundantFile = opRemoveRedundantFile.GetValue();
    compilerOptions_.dumpString = opDumpString.GetValue();
    compilerOptions_.moduleRecordFieldName = moduleRecordFieldName.GetValue();

    compilerOptions_.patchFixOptions.dumpSymbolTable = opDumpSymbolTable.GetValue();
    compilerOptions_.patchFixOptions.symbolTable = opInputSymbolTable.GetValue();

    if (!srcPkgName.GetValue().empty() && !dstPkgName.GetValue().empty()) {
        compilerOptions_.modifiedPkgName = srcPkgName.GetValue() + util::COLON_SEPARATOR + dstPkgName.GetValue();
    }

    bool generatePatch = opGeneratePatch.GetValue();
    bool hotReload = opHotReload.GetValue();
    bool coldReload = opColdReload.GetValue();
    bool coldFix = opColdFix.GetValue();
    if (generatePatch && hotReload) {
        errorMsg_ = "--generate-patch and --hot-reload can not be used simultaneously";
        return false;
    }
    if (coldFix && !generatePatch) {
        errorMsg_ = "--cold-fix can not be used without --generate-patch";
        return false;
    }
    compilerOptions_.patchFixOptions.generatePatch = generatePatch;
    compilerOptions_.patchFixOptions.hotReload = hotReload;
    compilerOptions_.patchFixOptions.coldReload = coldReload;
    compilerOptions_.patchFixOptions.coldFix = coldFix;

    compilerOptions_.enableAnnotations = enableAnnotations.GetValue();

    bool transformLibIsEmpty = transformLib.GetValue().empty();
    if (!transformLibIsEmpty) {
        auto libName = transformLib.GetValue();
        // check file exist or not
        auto transformLibAbs = panda::os::file::File::GetAbsolutePath(libName);
        if (!transformLibAbs) {
            std::cerr << "Failed to find file '" << libName << "' during transformLib file resolution" << std::endl
                      << "Please check if the file name is correct, the file exists at the specified path, "
                      << "and your project has the necessary permissions to access it." << std::endl;
            return false;
        }
        compilerOptions_.transformLib = transformLibAbs.Value();
    }

    compilerOptions_.branchElimination = opBranchElimination.GetValue();
    compilerOptions_.requireGlobalOptimization = compilerOptions_.optLevel > 0 &&
                                                 compilerOptions_.branchElimination &&
                                                 compilerOptions_.mergeAbc;
    panda::compiler::options.SetCompilerBranchElimination(compilerOptions_.branchElimination);
    panda::bytecodeopt::options.SetSkipMethodsWithEh(!opOptTryCatchFunc.GetValue());

    return true;
}

std::string Options::ExtractContentFromBase64Input(const std::string &inputBase64String)
{
    std::string inputContent = util::Base64Decode(inputBase64String);
    if (inputContent == "") {
        return "";
    }
    bool validBase64Input = util::Base64Encode(inputContent) == inputBase64String;
    if (!validBase64Input) {
        return "";
    }
    return inputContent;
}
}  // namespace panda::es2panda::aot
