/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef HIPERF_LIBREPORT_H
#define HIPERF_LIBREPORT_H

#if defined(is_mingw) && is_mingw
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif

#if is_ohos || is_double_framework
// error: '__cdecl' calling convention is not supported for this target
// [-Werror,-Wignored-attributes]
#define CDECL
#else
#define CDECL __cdecl
#endif

extern "C" {
/*
    demo usage in script/loadlib_test.py
*/

// this is a loop back test
// will return the const char with caller send to us
DLL_EXPORT const char *CDECL EchoLoopBack(const char *);

// set the hiperf in debug mode , it will print some detail log for debug
// log will save in current dir
// return 0 means suucessed
DLL_EXPORT int CDECL SetDebug(bool enable);

// same as "hiperf report", will create a report file with ascii code
// parameter:
// perfFile         the perf.data file path
// reportFile       the output file path
// reportOptions    options pass to report
// return 0 means suucessed
DLL_EXPORT int CDECL Report(const char *perfFile, const char *reportFile,
                            const char *reportOptions);

// same as "hiperf report --json"
// will output the report as json format
// our html template will read this for UI show
// parameter:
// perfFile     the perf.data file path
// reportFile   the output file path
// return 0 means suucessed
DLL_EXPORT int CDECL ReportJson(const char *perfFile, const char *reportFile);

// same as "hiperf report --json --symbol-dir <dir>"
// will output the report as json format and also unwind with --symbol-dir
// our html template will read this for UI show
// parameter:
// perfFile     the perf.data file path
// reportFile   the output file path
// return 0 means suucessed
DLL_EXPORT int CDECL ReportUnwindJson(const char *perfFile, const char *reportFile,
                                      const char *symbolsDir);

// same as "hiperf report dump"
// must be give the the perf.data file path
// return 0 means suucessed
DLL_EXPORT int CDECL Dump(const char *);

// when give the perf.data file path
// will return a const char with this format
// [filepath,buildid],[filepath,buildid],[filepath,buildid],[filepath,buildid],....
// python use this for build id match
DLL_EXPORT const char *CDECL ReportGetSymbolFiles(const char *perfFile);

// when give the elf file path
// will return the buildId in this elf
// return "" when no buildId found
DLL_EXPORT const char *CDECL ReportGetBuildId(const char *elfPath);

// when give the elf file path
// will return the arch in this elf
// now support "arm","arm64","x86","x86_64"
// return machine id(const char in ascii) when not support
// return "unknown" when failed happend
DLL_EXPORT const char *CDECL ReportGetElfArch(const char *elfPath);
}
#endif // HIPERF_LIBREPORT_H
