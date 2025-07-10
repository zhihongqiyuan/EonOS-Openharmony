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

#ifndef I18N_PHONENUMBERS_UPDATE_METADATA_H_
#define I18N_PHONENUMBERS_UPDATE_METADATA_H_

#include <map>
#include <string>
#include "phonenumbers/base/memory/scoped_ptr.h"
#include "phonenumbers/phonemetadata.pb.h"

namespace i18n {
namespace phonenumbers {
class UpdateMetadata {
public:
    static void UpdateShortNumber(scoped_ptr<std::map<std::string, PhoneMetadata>>& metadataMap);
    static void UpdatePhoneNumber(scoped_ptr<std::map<int, PhoneMetadata>>& countryMetadataMap,
        scoped_ptr<std::map<std::string, PhoneMetadata>>& regionMetadataMap);
    static void UpdateAlternateFormat(std::map<int, const PhoneMetadata*>& metadataMap);
    static void LoadUpdatedMetadata(int fd);

private:
    static const std::string METADATA_NAME;
    static const std::string SHORT_METADATA_TYPE;
    static const std::string PHONE_METADATA_TYPE;
    static const std::string ALTERNATE_METADATA_TYPE;
    static const std::string REGION_CODE_NONGEO;
    static PhoneMetadataCollection* phoneMetadataCollection;
    static std::map<std::string, PhoneMetadata>* shortMetadata;
    static std::map<std::string, PhoneMetadata>* regionMetadata;
    static std::map<int, PhoneMetadata>* countryMetadata;
    static std::map<int, PhoneMetadata>* alterMetadata;
};
}  // namespace phonenumbers
}  // namespace i18n
#endif  // I18N_PHONENUMBERS_UPDATE_METADATA_H_
