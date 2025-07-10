/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_AOT_DEPEND_RELATION_H
#define ES2PANDA_AOT_DEPEND_RELATION_H
#include <map>
#include <string>
#include <queue>
#include <unordered_set>
#include <vector>

#include <aot/options.h>
#include <es2panda.h>
#include <util/workerQueue.h>

namespace panda::es2panda::aot {

class DepsRelationResolver {
public:
    explicit DepsRelationResolver(const std::map<std::string, panda::es2panda::util::ProgramCache *> &progsInfo,
                                  const std::unique_ptr<panda::es2panda::aot::Options> &options,
                                  std::map<std::string, std::unordered_set<std::string>> &resolvedDepsRelation)
        : progsInfo_(progsInfo), resolvedDepsRelation_(resolvedDepsRelation),
        compileContextInfo_(options->CompilerOptions().compileContextInfo),
        dumpDepsInfo_(options->CompilerOptions().dumpDepsInfo)
    {
    }

    ~DepsRelationResolver() = default;
    void CollectStaticImportDepsRelation(const panda::pandasm::Program &program, const std::string &recordName);
    void CollectDynamicImportDepsRelation(const panda::pandasm::Program &program, const std::string &recordName);
    bool Resolve();

private:
    void FillRecord2ProgramMap(std::unordered_map<std::string, std::string> &record2ProgramMap);
    bool CollectCommonjsAndJsonRecords(const std::vector<panda::pandasm::Field> &fieldList,
                                       const std::string &progKey, const std::string &recordName);
    void CollectDepsIfNeeded(const std::string &ohmurl);
    void DumpDepsRelations();

    const std::map<std::string, panda::es2panda::util::ProgramCache *> &progsInfo_;
    std::map<std::string, std::unordered_set<std::string>> &resolvedDepsRelation_;
    CompileContextInfo &compileContextInfo_;
    std::queue<std::string> depsToBeResolved_ {};
    std::unordered_set<std::string> resolvedRecords_ {};
    bool dumpDepsInfo_ {false};
};
} // namespace panda::es2panda::aot

#endif

