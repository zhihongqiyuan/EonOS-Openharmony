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

#ifndef ES2PANDA_PARSER_CORE_MODULERECORD_H
#define ES2PANDA_PARSER_CORE_MODULERECORD_H

#include <util/ustring.h>

namespace panda::es2panda::binder {
class ModuleScope;
class Variable;
}

namespace panda::es2panda::ir {
class Identifier;
}  // namespace panda::es2panda::ir

namespace panda::es2panda::parser {
class SourceTextModuleRecord {
public:
    explicit SourceTextModuleRecord(ArenaAllocator *allocator)
        : allocator_(allocator),
          moduleRequestsMap_(allocator_->Adapter()),
          moduleRequests_(allocator_->Adapter()),
          localExportEntries_(allocator_->Adapter()),
          regularImportEntries_(allocator_->Adapter()),
          namespaceImportEntries_(allocator_->Adapter()),
          starExportEntries_(allocator_->Adapter()),
          indirectExportEntries_(allocator_->Adapter())
    {
    }

    ~SourceTextModuleRecord() = default;
    NO_COPY_SEMANTIC(SourceTextModuleRecord);
    NO_MOVE_SEMANTIC(SourceTextModuleRecord);

    struct ImportEntry {
        int moduleRequestIdx_;
        util::StringView localName_;
        util::StringView importName_;
        const ir::Identifier *localId_ {nullptr};
        const ir::Identifier *importId_ {nullptr};

        ImportEntry(const util::StringView localName, const util::StringView importName, int moduleRequestIdx,
                    const ir::Identifier *localId, const ir::Identifier *importId)
            : moduleRequestIdx_(moduleRequestIdx), localName_(localName), importName_(importName),
              localId_(localId), importId_(importId) {}
        ImportEntry(const util::StringView localName, int moduleRequestIdx, const ir::Identifier *localId)
            : moduleRequestIdx_(moduleRequestIdx), localName_(localName), localId_(localId) {}
    };

    struct ExportEntry {
        int moduleRequestIdx_;
        util::StringView exportName_;
        util::StringView localName_;
        util::StringView importName_;
        const ir::Identifier *exportId_ {nullptr};
        const ir::Identifier *localId_ {nullptr};
        const ir::Identifier *importId_ {nullptr};
        bool isConstant_ {false};

        explicit ExportEntry(int moduleRequest) : moduleRequestIdx_(moduleRequest) {}
        ExportEntry(const util::StringView exportName, const util::StringView localName,
                    const ir::Identifier *exportId, const ir::Identifier *localId)
            : moduleRequestIdx_(-1), exportName_(exportName), localName_(localName),
              exportId_(exportId), localId_(localId) {}
        ExportEntry(const util::StringView exportName, const util::StringView importName, int moduleRequest,
                    const ir::Identifier *exportId, const ir::Identifier *importId)
            : moduleRequestIdx_(moduleRequest), exportName_(exportName), importName_(importName),
              exportId_(exportId), importId_(importId) {}

        void SetAsConstant()
        {
            isConstant_ = true;
        }
    };

    struct ModuleRequestRecord {
        util::StringView source_;
        bool isLazy_ {false};

        explicit ModuleRequestRecord(util::StringView source) : source_{source} {}
        ModuleRequestRecord(util::StringView source, bool isLazy) : source_{source}, isLazy_{isLazy} {}

        bool operator<(const ModuleRequestRecord &mrr) const
        {
            return source_ < mrr.source_ || (source_ == mrr.source_ && isLazy_ && !mrr.isLazy_);
        }
    };

    template <typename T, typename... Args>
    T *NewEntry(Args &&... args)
    {
        return allocator_->New<T>(std::forward<Args>(args)...);
    }

    template <typename... Args>
    int AddModuleRequest(Args &&... args)
    {
        const auto *record = allocator_->New<ModuleRequestRecord>(std::forward<Args>(args)...);
        CHECK_NOT_NULL(record);
        return AddModuleRequest(*record);
    }
    int AddModuleRequest(const ModuleRequestRecord record);
    void AddImportEntry(ImportEntry *entry);
    void AddStarImportEntry(ImportEntry *entry);
    bool AddLocalExportEntry(ExportEntry *entry);
    void RemoveDefaultLocalExportEntry();
    bool AddIndirectExportEntry(ExportEntry *entry);
    void AddStarExportEntry(ExportEntry *entry);

    bool CheckImplicitIndirectExport(ExportEntry *exportEntry);
    void CheckImplicitIndirectExport(ImportEntry *importEntry);

    void AssignIndexToModuleVariable(binder::ModuleScope *moduleScope);
    int GetModuleRequestIdx(const util::StringView localName);

    using ModuleRequestList = ArenaVector<ModuleRequestRecord>;
    using ModuleRequestMap = ArenaMap<const ModuleRequestRecord, uint32_t>;
    using LocalExportEntryMap = ArenaMultiMap<const util::StringView, ExportEntry *>;
    using RegularImportEntryMap = ArenaMap<const util::StringView, ImportEntry *>;
    using NamespaceImportEntryList = ArenaVector<ImportEntry *>;
    using SpecialExportEntryList = ArenaVector<ExportEntry *>;

    const ModuleRequestList &GetModuleRequests() const
    {
        return moduleRequests_;
    }

    const LocalExportEntryMap &GetLocalExportEntries() const
    {
        return localExportEntries_;
    }

    const RegularImportEntryMap &GetRegularImportEntries() const
    {
        return regularImportEntries_;
    }

    const NamespaceImportEntryList &GetNamespaceImportEntries() const
    {
        return namespaceImportEntries_;
    }

    const SpecialExportEntryList &GetStarExportEntries() const
    {
        return starExportEntries_;
    }

    const SpecialExportEntryList &GetIndirectExportEntries() const
    {
        return indirectExportEntries_;
    }

    bool HasLazyImport() const
    {
        return hasLazyImport_;
    }

    static constexpr std::string_view DEFAULT_LOCAL_NAME = "*default*";
    static constexpr std::string_view DEFAULT_EXTERNAL_NAME = "default";
    static constexpr std::string_view ANONY_NAMESPACE_NAME = "=ens";
    static constexpr int INVALID_MODULEREQUEST_ID = -1;

private:
    bool HasDuplicateExport(util::StringView exportName) const;
    void ConvertLocalExportToIndirect(ImportEntry *importEntry, ExportEntry *exportEntry);
    binder::Variable *CheckAndAssignIndex(binder::ModuleScope *moduleScope, util::StringView name, uint32_t *idx) const;

    ArenaAllocator *allocator_;
    ModuleRequestMap moduleRequestsMap_;
    ModuleRequestList moduleRequests_;
    LocalExportEntryMap localExportEntries_;
    RegularImportEntryMap regularImportEntries_;
    NamespaceImportEntryList namespaceImportEntries_;
    SpecialExportEntryList starExportEntries_;
    SpecialExportEntryList indirectExportEntries_;
    bool hasLazyImport_ { false };
};
} // namespace panda::es2panda::parser

#endif