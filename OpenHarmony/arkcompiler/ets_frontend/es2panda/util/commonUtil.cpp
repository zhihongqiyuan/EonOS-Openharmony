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

#include "commonUtil.h"

#include "util/helpers.h"

namespace panda::es2panda::util {

std::vector<std::string> Split(const std::string &str, const char delimiter)
{
    std::vector<std::string> items;

    size_t start = 0;
    size_t pos = str.find(delimiter);
    while (pos != std::string::npos) {
        std::string item = str.substr(start, pos - start);
        items.emplace_back(item);
        start = pos + 1;
        pos = str.find(delimiter, start);
    }
    std::string tail = str.substr(start);
    items.emplace_back(tail);

    return items;
}

std::string GetStringByVectorElementsWithDelimiter(const std::vector<std::string> &vec, const char delimiter)
{
    std::ostringstream oss;
    auto it = vec.begin();
    if (it != vec.end()) {
        oss << *it;
        ++it;
    }
    for (; it != vec.end(); ++it) {
        oss << delimiter << *it;
    }
    return oss.str();
}

std::string GetPkgNameFromNormalizedImport(const std::string &normalizedImport)
{
    std::string pkgName {};
    size_t pos = normalizedImport.find(SLASH_TAG);
    if (pos != std::string::npos) {
        pkgName = normalizedImport.substr(0, pos);
    }
    if (normalizedImport[0] == NORMALIZED_OHMURL_PREFIX) {
        pos = normalizedImport.find(SLASH_TAG, pos + 1);
        if (pos != std::string::npos) {
            pkgName = normalizedImport.substr(0, pos);
        }
    }
    return pkgName;
}

std::string GetPkgNameFromNormalizedOhmurl(const std::string &ohmurl)
{
    std::string normalizedImport {};
    std::string pkgName {};
    auto items = Split(ohmurl, NORMALIZED_OHMURL_SEPARATOR);
    if (items.size() <= NORMALIZED_IMPORT_POS) {
        return pkgName;
    }
    normalizedImport = items[NORMALIZED_IMPORT_POS];
    return GetPkgNameFromNormalizedImport(normalizedImport);
}

std::string GetRecordNameFromNormalizedOhmurl(const std::string &ohmurl)
{
    // format of recordName: "<bundleName>&normalizedImport&<version>"
    std::string recordName {};
    auto items = Split(ohmurl, NORMALIZED_OHMURL_SEPARATOR);

    recordName += items[BUNDLE_NAME_POS] + NORMALIZED_OHMURL_SEPARATOR + items[NORMALIZED_IMPORT_POS] +
        NORMALIZED_OHMURL_SEPARATOR + items[VERSION_POS];
    return recordName;
}

bool IsExternalPkgNames(const std::string &ohmurl, const std::set<std::string> &externalPkgNames)
{
    auto pkgName = GetPkgNameFromNormalizedOhmurl(ohmurl);
    if (std::find(externalPkgNames.begin(), externalPkgNames.end(), pkgName) != externalPkgNames.end()) {
        return true;
    }
    return false;
}

std::string UpdatePackageVersionIfNeeded(const std::string &ohmurl,
                                         const std::map<std::string, PkgInfo> &pkgContextInfo)
{
    // Input ohmurl format:
    // @normalized:{N|Y}&[module name]&[bundle name]&{<package name>|<@package/name>}/{import_path}&[version]
    // Update the version for ohmurls and return the updated ohmurl when:
    // 1. The package name and version are specified in the CompileContextInfo file.
    // 2. The ohmurl is an imported non-native ohmurl (starts with @normalized:N).
    // 3. The version in the ohmurl differs from the version in the CompileContextInfo file.
    // Return the original ohmurl otherwise.
    if (ohmurl.find(util::NORMALIZED_OHMURL_NOT_SO) != 0) {
        return ohmurl;
    }
    std::string packageName = util::GetPkgNameFromNormalizedOhmurl(ohmurl);
    // Incorrect ohmurl format: no package name, skip version update
    if (packageName.empty()) {
        return ohmurl;
    }
    auto iter = pkgContextInfo.find(packageName);
    if (iter == pkgContextInfo.end()) {
        return ohmurl;
    }
    auto versionStart = ohmurl.rfind(util::NORMALIZED_OHMURL_SEPARATOR);
    // Incorrect ohmurl format: no version, skip version update
    if (versionStart == std::string::npos) {
        return ohmurl;
    }
    return ohmurl.substr(0, versionStart + 1) + iter->second.version;
}

/**
 * Modify the package name in the bytecode. The modifiedPkgName format is originalPkgName:targetPkgName.
 */
std::string UpdatePackageNameIfNeeded(const std::string &ohmurl, const std::string &modifiedPkgName)
{
    if (ohmurl.find(util::NORMALIZED_OHMURL_NOT_SO) != 0) {
        return ohmurl;
    }
    std::string packageName = util::GetPkgNameFromNormalizedOhmurl(ohmurl);
    std::vector<std::string> pkgNames = Split(modifiedPkgName, COLON_SEPARATOR);
    if (packageName == pkgNames[ORIGINAL_PKG_NAME_POS]) {
        size_t pos = ohmurl.find(NORMALIZED_OHMURL_SEPARATOR + packageName) + 1;
        std::string modified = ohmurl;
        std::string target = pkgNames[TARGET_PKG_NAME_POS];
        modified.replace(pos, packageName.length(), target);
        return modified;
    }
    return ohmurl;
}

/**
 * If a Har package is dependent of a cross-app Hsp, its ohmurl need to contain the bundleName of this cross-app Hsp.
 * Since Har's ohmurl doesn't contain bundleName during its own compilation, the bundleName need to be added during
 * the compilation of cross-app Hsp.
 */
std::string UpdateBundleNameIfNeeded(std::string &ohmurl, const std::string &bundleName,
                                     const std::set<std::string> &externalPkgNames)
{
    // Input ohmurl format:
    // @normalized:{N|Y}&[module name]&[bundle name]&{<package name>|<@package/name>}/{import_path}&[version]
    if (ohmurl.find(util::NORMALIZED) != 0) {
        return ohmurl;
    }

    std::vector<std::string> items = Split(ohmurl, NORMALIZED_OHMURL_SEPARATOR);
    // Incorrect ohmurl format: The quantity of '&' is incorrect
    if (items.size() <= VERSION_POS) {
        return ohmurl;
    }
    /**
     * If an ohmurl already contains [bundle name], it means its not from a Har, so there's no need to updated its
     * [bundle name].
     */
    if (!items[BUNDLE_NAME_POS].empty()) {
        return ohmurl;
    }
    /**
     * The hsp package don't compile into the current abc file.
     * Ohmurl of both Har and in-app Hsp don't contain [bundle name], need to further screen out Hsp.
     */
    if (IsExternalPkgNames(ohmurl, externalPkgNames)) {
        return ohmurl;
    }

    items[BUNDLE_NAME_POS] = bundleName;
    return GetStringByVectorElementsWithDelimiter(items, NORMALIZED_OHMURL_SEPARATOR);
}

bool RecordNotGeneratedFromBytecode(std::string recordName)
{
    return recordName.find(util::CHAR_VERTICAL_LINE) == std::string::npos;
}

} // namespace panda::es2panda::util