/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "update_geocoding.h"

namespace i18n {
namespace phonenumbers {
GeocodingInfo* UpdateGeocoding::geocodingInfo = nullptr;

std::map<std::string, PrefixDescriptions>* UpdateGeocoding::prefixDescriptionsMap = new std::map<std::string, PrefixDescriptions>();
std::map<std::string, PrefixesInfo>* UpdateGeocoding::prefixesInfoMap = new std::map<std::string, PrefixesInfo>();
const PrefixDescriptions** UpdateGeocoding::prefixDescriptionsArray = nullptr;
LanguageCodeInfo* UpdateGeocoding::languageCodeInfo = nullptr;
bool UpdateGeocoding::isupdatedLanguageCodes = false;
const char** UpdateGeocoding::preLanguageCodes = nullptr;
const char** UpdateGeocoding::curLanguageCodes = nullptr;
int UpdateGeocoding::curLanguageCodesSize = 0;
int UpdateGeocoding::preLanguageCodesSize = 0;

std::map<int, CountryLanguages>* UpdateGeocoding::countryLanguagesMap = new std::map<int, CountryLanguages>();
std::map<int, CountriesInfo>* UpdateGeocoding::countriesInfoMap = new std::map<int, CountriesInfo>();
const CountryLanguages** UpdateGeocoding::countryLanguagesArray = nullptr;
CountryCodeInfo* UpdateGeocoding::countryCodeInfo = nullptr;
bool UpdateGeocoding::isupdatedCountryCodes = false;
const int* UpdateGeocoding::preCountryCodes = nullptr;
int* UpdateGeocoding::curCountryCodes = nullptr;
int UpdateGeocoding::curCountryCodesSize = 0;
int UpdateGeocoding::preCountryCodesSize = 0;

void UpdateGeocoding::LoadGeocodingData(int fd)
{
    if (geocodingInfo != nullptr) {
        return;
    }
    if (fd == -1) {
        return;
    }
    geocodingInfo = new GeocodingInfo();
    geocodingInfo->ParseFromFileDescriptor(fd);
    languageCodeInfo = new  LanguageCodeInfo(geocodingInfo->language_code_info());
    countryCodeInfo = new CountryCodeInfo(geocodingInfo->country_code_info());
    for (int i = 0; i < geocodingInfo->languages_size(); i++) {
        prefixesInfoMap->insert(std::make_pair(geocodingInfo->languages(i), geocodingInfo->prefixes_info(i)));
    }
    for (int i = 0; i < geocodingInfo->countries_size(); i++) {
        countriesInfoMap->insert(std::make_pair(geocodingInfo->countries(i), geocodingInfo->countries_info(i)));
    }
}

const PrefixDescriptions* UpdateGeocoding::UpdatePrefixDescriptions(
    const PrefixDescriptions** prePrefixDescriptionsArray, int index)
{
    prefixDescriptionsArray = prePrefixDescriptionsArray;
    std::string languageCode = isupdatedLanguageCodes ? curLanguageCodes[index] : preLanguageCodes[index];
    if (prefixDescriptionsMap->find(languageCode) != prefixDescriptionsMap->end()) {
        return &prefixDescriptionsMap->at(languageCode);
    }
    int preIndex = isupdatedLanguageCodes ? FindLanguageCode(languageCode) : index;
    AddPrefixDescriptions(languageCode, preIndex);

    if (prefixDescriptionsMap->find(languageCode) == prefixDescriptionsMap->end()) {
        prefixDescriptionsMap->insert(std::make_pair(languageCode, *(prefixDescriptionsArray[preIndex])));
    }
    return &prefixDescriptionsMap->at(languageCode);
}

const char** UpdateGeocoding::UpdateLanguageCodes(const char** languageCodes, int languageCodesSize)
{
    preLanguageCodes = languageCodes;
    preLanguageCodesSize = languageCodesSize;
    if (languageCodeInfo == nullptr || languageCodeInfo->language_codes_size() == 0) {
        return preLanguageCodes;
    }
    int curLength = languageCodeInfo->language_codes_num();
    curLanguageCodesSize = curLength;
    curLanguageCodes = (const char **)malloc(sizeof(char*) * curLength);
    if (!curLanguageCodes) {
        return preLanguageCodes;
    }
    int preIndex = 0;
    int curIndex = 0;
    int mdyIndex = 0;
    while (preIndex < preLanguageCodesSize && mdyIndex < languageCodeInfo->language_codes_size()) {
        if (strcmp(languageCodeInfo->language_codes(mdyIndex).c_str(), preLanguageCodes[preIndex]) < 0) {
            curLanguageCodes[curIndex] = WriteStr(languageCodeInfo->language_codes(mdyIndex));
            curIndex++;
            mdyIndex++;
        } else if (strcmp(languageCodeInfo->language_codes(mdyIndex).c_str(), preLanguageCodes[preIndex]) == 0) {
            preIndex++;
            mdyIndex++;
        } else {
            curLanguageCodes[curIndex] = preLanguageCodes[preIndex];
            curIndex++;
            preIndex++;
        }
    }

    while (preIndex < preLanguageCodesSize) {
        curLanguageCodes[curIndex] = preLanguageCodes[preIndex];
        curIndex++;
        preIndex++;
    }

    while (mdyIndex < languageCodeInfo->language_codes_size()) {
        curLanguageCodes[curIndex] = WriteStr(languageCodeInfo->language_codes(mdyIndex));
        curIndex++;
        mdyIndex++;
    }
    isupdatedLanguageCodes = true;
    return curLanguageCodes;
}

int UpdateGeocoding::UpdateLanguageCodesSize()
{
    if (isupdatedLanguageCodes) {
        return curLanguageCodesSize;
    }
    return preLanguageCodesSize;
}

void UpdateGeocoding::AddPrefixDescriptions(const std::string& languageCode, int index)
{
    if (prefixesInfoMap->find(languageCode) == prefixesInfoMap->end()) {
        return;
    }
    PrefixesInfo prefixesInfo = prefixesInfoMap->at(languageCode);
    int prefixesSize = static_cast<int>(prefixesInfo.prefixes_num());
    int32_t* prefixes = (int32_t*)malloc(sizeof(int32_t) * prefixesSize);
    if (!prefixes) {
        return;
    }
    memset(prefixes, 0, sizeof(int32_t) * prefixesSize);
    const char** descriptions = (const char **)malloc(sizeof(char*) * prefixesSize);
    if (!descriptions) {
        return;
    }
    ModifyPrefixDescriptions(prefixes, descriptions, prefixesInfo, index);

    int lengthsSize = prefixesInfo.lengths_num();
    int32_t* possibleLengths = (int32_t*)malloc(sizeof(int32_t) * lengthsSize);
    if (!possibleLengths) {
        return;
    }
    ModifyPossibleLengths(possibleLengths, prefixesInfo, index);
 
    PrefixDescriptions prefixDescriptions = {
        prefixes,
        prefixesSize,
        descriptions,
        possibleLengths,
        lengthsSize
    };
    prefixDescriptionsMap->insert(std::make_pair(languageCode, prefixDescriptions));
}

void UpdateGeocoding::ModifyPrefixDescriptions(int32_t* prefixes, const char** descriptions,
    PrefixesInfo& prefixesInfo, int index)
{
    int curIndex = 0;
    int preIndex = 0;
    int mdyIndex = 0;
    const PrefixDescriptions* prefixDescriptions = nullptr;
    std::string description;
    int prefixesSize = 0;
    if (index != -1) {
        prefixDescriptions = prefixDescriptionsArray[index];
        prefixesSize = prefixDescriptions->prefixes_size;
    }
    while (preIndex < prefixesSize && mdyIndex < prefixesInfo.prefixes_size()) {
        int prePrefix = prefixDescriptions->prefixes[preIndex];
        int mdyPrefix = static_cast<int>(prefixesInfo.prefixes(mdyIndex));
        if (prePrefix == mdyPrefix) {
            if (prefixesInfo.descriptions(mdyIndex) == "NULL") {
                preIndex++;
                mdyIndex++;
                continue;
            } else {
                prefixes[curIndex] = mdyPrefix;
                descriptions[curIndex] = WriteStr(prefixesInfo.descriptions(mdyIndex));
                preIndex++;
                mdyIndex++;
            }
        } else if (prePrefix > mdyPrefix) {
            prefixes[curIndex] = mdyPrefix;
            descriptions[curIndex] = WriteStr(prefixesInfo.descriptions(mdyIndex));
            mdyIndex++;
        } else {
            prefixes[curIndex] = prePrefix;
            descriptions[curIndex] = prefixDescriptions->descriptions[preIndex];
            preIndex++;
        }
        curIndex++;
    }
    while (preIndex < prefixesSize) {
        prefixes[curIndex] = prefixDescriptions->prefixes[preIndex];
        descriptions[curIndex] = prefixDescriptions->descriptions[preIndex];
        preIndex++;
        curIndex++;
    }
    while (mdyIndex < prefixesInfo.prefixes_size()) {
        prefixes[curIndex] = static_cast<int>(prefixesInfo.prefixes(mdyIndex));
        descriptions[curIndex] = WriteStr(prefixesInfo.descriptions(mdyIndex));
        mdyIndex++;
        curIndex++;
    }
}

void UpdateGeocoding::ModifyPossibleLengths(int32_t* possibleLengths, PrefixesInfo& prefixesInfo, int index)
{
    if (index == -1) {
        for (int i = 0; i < prefixesInfo.lengths_size(); i++) {
            possibleLengths[i] = prefixesInfo.lengths(i);
        }
    } else {
        for (int i = 0; i < prefixDescriptionsArray[index]->possible_lengths_size; i++) {
            possibleLengths[i] = prefixDescriptionsArray[index]->possible_lengths[i];
        }
    }
}

const CountryLanguages* UpdateGeocoding::UpdateCountryLanguages(const CountryLanguages** preCountryLanguagesArray, int index)
{
    countryLanguagesArray = preCountryLanguagesArray;
    int countryCode = isupdatedCountryCodes ? curCountryCodes[index] : preCountryCodes[index];
    if (countryLanguagesMap->find(countryCode) != countryLanguagesMap->end()) {
        return &countryLanguagesMap->at(countryCode);
    }
    int preIndex = isupdatedCountryCodes ? FindCountryCode(countryCode) : index;
    AddCountryLanguages(countryCode);
    if (countryLanguagesMap->find(countryCode) == countryLanguagesMap->end() && preIndex != -1) {
        countryLanguagesMap->insert(std::make_pair(countryCode, *(countryLanguagesArray[preIndex])));
    }
    return &countryLanguagesMap->at(countryCode);
}

const int* UpdateGeocoding::UpdateCountryCodes(const int* countryCodes, int countryCodesSize)
{
    preCountryCodes = countryCodes;
    preCountryCodesSize = countryCodesSize;
    if (countryCodeInfo == nullptr || countryCodeInfo->country_codes_size() == 0) {
        return preCountryCodes;
    }
    int curLength = countryCodeInfo->country_codes_num();
    curCountryCodesSize = curLength;
    curCountryCodes = (int *)malloc(sizeof(int) * curLength);
    if (!curCountryCodes) {
        return preCountryCodes;
    }
    int preIndex = 0;
    int curIndex = 0;
    int mdyIndex = 0;
    while (preIndex < preCountryCodesSize && mdyIndex < countryCodeInfo->country_codes_size()) {
        int preCountryCode = preCountryCodes[preIndex];
        int mdyCountryCode = static_cast<int>(countryCodeInfo->country_codes(mdyIndex));
        if (preCountryCode > mdyCountryCode) {
            curCountryCodes[curIndex] = mdyCountryCode;
            curIndex++;
            mdyIndex++;
        } else if (mdyCountryCode == preCountryCode) {
            preIndex++;
            mdyIndex++;
        } else {
            curCountryCodes[curIndex] = preCountryCode;
            curIndex++;
            preIndex++;
        }
    }
    while (preIndex < preCountryCodesSize) {
        curCountryCodes[curIndex] = preCountryCodes[preIndex];
        curIndex++;
        preIndex++;
    }
    while (mdyIndex < countryCodeInfo->country_codes_size()) {
        curCountryCodes[curIndex] = static_cast<int>(countryCodeInfo->country_codes(mdyIndex));
        curIndex++;
        mdyIndex++;
    }
    isupdatedCountryCodes = true;
    return curCountryCodes;
}

int UpdateGeocoding::UpdateCountryCodesSize()
{
    if (isupdatedCountryCodes) {
        return curCountryCodesSize;
    }
    return preCountryCodesSize;
}

void UpdateGeocoding::AddCountryLanguages(int countryCode)
{
    if (countriesInfoMap->find(countryCode) == countriesInfoMap->end()) {
        return;
    }
    CountriesInfo countriesInfo = countriesInfoMap->at(countryCode);

    int availableLanguagesSize = static_cast<int>(countriesInfo.country_languages_num());
    const char** availableLanguages = (const char **)malloc(sizeof(char*) * availableLanguagesSize);
    if (!availableLanguages) {
        return;
    }
    for (int i = 0; i < countriesInfo.country_languages_size(); i++) {
        std::string language = countriesInfo.country_languages(i);
        int len = sizeof(char) * (language.length() + 1);
        char* temp = (char *)malloc(len);
        if (!temp) {
            return;
        }
        memset(temp, '\0', len);
        strncpy(temp, language.c_str(), len);
        availableLanguages[i] = temp;
    }
    CountryLanguages countryLanguages = {
        availableLanguages,
        availableLanguagesSize
    };
    countryLanguagesMap->insert(std::make_pair(countryCode, countryLanguages));
}

int UpdateGeocoding::FindLanguageCode(const std::string& languageCode)
{
    const char** const preLanguageCodesEnd = preLanguageCodes + preLanguageCodesSize;
    const char** const preIndex = std::lower_bound(preLanguageCodes,
                                                   preLanguageCodesEnd,
                                                   languageCode.c_str(),
                                                   IsLowerThan);
    if (preIndex == preLanguageCodesEnd || languageCode.compare(*preIndex) != 0) {
        return -1;
    }
    return preIndex - preLanguageCodes;
}

int UpdateGeocoding::FindCountryCode(int countryCode)
{
    const int* const preCountryCodesEnd = preCountryCodes + preCountryCodesSize;
    const int* const preIndex = std::lower_bound(preCountryCodes,
                                                 preCountryCodesEnd,
                                                 countryCode);
    if (preIndex == preCountryCodesEnd || *preIndex != countryCode) {
        return -1;
    }
    return preIndex - preCountryCodes;
}

bool UpdateGeocoding::IsLowerThan(const char* s1, const char* s2)
{
    return strcmp(s1, s2) < 0;
}

char* UpdateGeocoding::WriteStr(const std::string& str)
{
    int len = sizeof(char) * (str.length() + 1);
    char* result = (char*)malloc(len);
    if (!result) {
        return nullptr;
    }
    memset(result, '\0', len);
    strncpy(result, str.c_str(), len);
    return result;
}
}
}