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

#ifndef I18N_PHONENUMBERS_UPDATE_GEOCODING_H_
#define I18N_PHONENUMBERS_UPDATE_GEOCODING_H_

#include <map>
#include <string>
#include "geocoding_data.pb.h"
#include "phonenumbers/geocoding/geocoding_data.h"

namespace i18n {
namespace phonenumbers {
class UpdateGeocoding {
public:
    static void LoadGeocodingData(int fd);
    static const PrefixDescriptions* UpdatePrefixDescriptions(const PrefixDescriptions** prePrefixDescriptionsArray,
        int index);
    static const char** UpdateLanguageCodes(const char** languageCodes, int languageCodesSize);
    static int UpdateLanguageCodesSize();
    static const CountryLanguages* UpdateCountryLanguages(const CountryLanguages** preCountryLanguagesArray,
        int index);
    static const int* UpdateCountryCodes(const int* countryCodes, int countryCodesSize);
    static int UpdateCountryCodesSize();

private:
    static void AddPrefixDescriptions(const std::string& languageCode, int index);
    static void ModifyPrefixDescriptions(int32_t* prefixes, const char** descriptions,
        PrefixesInfo& prefixesInfo, int index);
    static void ModifyPossibleLengths(int32_t* possibleLengths, PrefixesInfo& prefixesInfo, int index);
    static void AddCountryLanguages(int countryCode);

    static int FindLanguageCode(const std::string& languageCode);
    static int FindCountryCode(int countryCode);
    static bool IsLowerThan(const char* s1, const char* s2);
    static char* WriteStr(const std::string& str);
    static std::map<std::string, PrefixDescriptions>* prefixDescriptionsMap;
    static std::map<std::string, PrefixesInfo>* prefixesInfoMap;
    static const PrefixDescriptions** prefixDescriptionsArray;
    static LanguageCodeInfo* languageCodeInfo;
    static bool isupdatedLanguageCodes;
    static const char** preLanguageCodes;
    static const char** curLanguageCodes;
    static int curLanguageCodesSize;
    static int preLanguageCodesSize;

    static std::map<int, CountryLanguages>* countryLanguagesMap;
    static std::map<int, CountriesInfo>* countriesInfoMap;
    static const CountryLanguages** countryLanguagesArray;
    static CountryCodeInfo* countryCodeInfo;

    static bool isupdatedCountryCodes;
    static const int* preCountryCodes;
    static int* curCountryCodes;
    static int curCountryCodesSize;
    static int preCountryCodesSize;

    static GeocodingInfo* geocodingInfo;
};
}  // namespace phonenumbers
}  // namespace i18n
#endif  // I18N_PHONENUMBERS_UPDATE_GEOCODING_H_