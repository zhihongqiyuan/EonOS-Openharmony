/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#ifndef HIPERF_OPTION_H_
#define HIPERF_OPTION_H_

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "debug_logger.h"
#include "utilities.h"

using argsVector = std::vector<std::string>;
namespace OHOS {
namespace Developtools {
namespace HiPerf {
namespace Option {
struct MainOption {
    std::string help;
    std::function<bool(std::vector<std::string> &)> callBackFunction;
};

// called from main
bool RegisterMainOption(const std::string &optionName, const std::string &help,
                        std::function<bool(std::vector<std::string> &)> callBackFunction);

void ClearMainOptions();

bool CheckOptionFormat(const std::string &optionName);

argsVector::iterator FindOption(argsVector &args, const std::string &optionName);

// some option function
bool GetValueFromString(const std::string &optionValue, const std::string &optionName, bool &value);
bool GetValueFromString(const std::string &optionValue, const std::string &optionName, int &value);
bool GetValueFromString(const std::string &optionValue, const std::string &optionName, float &value);
bool GetValueFromString(const std::string &optionValue, const std::string &optionName, uint64_t &value);
bool GetValueFromString(const std::string &optionValue, const std::string &optionName, std::string &value);
bool GetValueFromString(const std::string &optionValue, const std::string &optionName, std::vector<int> &values);
bool GetValueFromString(const std::string &optionValue, const std::string &optionName,
                        std::vector<std::string> &values);

bool GetOptionTrackedCommand(argsVector &args, std::vector<std::string> &trackedCommand);

/*
Return false to indicate that the parameter is illegal
The program should exit with an error.

Return true, indicating that the parameter is legal (but the user does not necessarily enter the
parameter)
*/
template<class T>
bool GetOptionValue(argsVector &args, std::string optionName, T &value)
{
    // we need keep the ref if we got failed
    // so we use a local value first.
    T localValues = {};
    if constexpr (std::is_same<T, std::vector<std::vector<std::string>>>::value) {
        // try unitl failed.
        while (true) {
            if (!GetOptionValue(args, optionName, localValues.emplace_back())) {
                printf("incorrect option %s\n", optionName.c_str());
                return false; // format error
            } else if (localValues.back().size() == 0) {
                // if the last one we request is empty , we remove it
                localValues.pop_back();
                // nothing more needed
                // we don't allow empty value
                break;
            }
        }
        if (localValues.size() > 0) {
            value = localValues;
        }
        return true;
    } else {
        if (!CheckOptionFormat(optionName)) {
            if (optionName.empty()) {
                printf("unable to use empty option name!\n");
            } else {
                printf("format error. must use '-' at the begin of option '%s'!\n",
                       optionName.c_str());
            }
            return false; // something wrong
        }
        auto it = FindOption(args, optionName);
        if (it == args.end()) {
            HLOGV("not found option, return default value");
            return true; // not found but also not error
        } else {
            it = args.erase(it);
            // some special case
            if constexpr (std::is_same<T, bool>::value) {
                // for bool we don't need get value.
                // this always return true
                GetValueFromString(optionName, optionName, value);
                return true;
            } else if (it == args.end()) {
                // no value means failed
                printf("option %s value missed\n", optionName.c_str());
                return false;
            } else if (GetValueFromString(*it, optionName, localValues)) {
                // got some value
                value = localValues;
                args.erase(it);
                return true;
            } else {
                // have value but convert failed.
                printf("incorrect option value '%s' for option '%s'. View the usage with the --help option.\n",
                       (*it).c_str(), optionName.c_str());
                return false;
            }
        }
    }
}

const MainOption *FindMainOption(const std::string &argName);

const std::map<std::string, std::unique_ptr<MainOption>> &GetMainOptions();
} // namespace Option
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_OPTION_H_
