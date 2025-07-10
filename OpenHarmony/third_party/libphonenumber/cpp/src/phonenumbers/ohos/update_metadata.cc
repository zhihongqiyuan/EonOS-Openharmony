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
#include "update_metadata.h"

namespace i18n {
namespace phonenumbers {

const std::string UpdateMetadata::METADATA_NAME = "/system/etc/icu_tzdata/i18n/MetadataInfo";
const std::string UpdateMetadata::SHORT_METADATA_TYPE = "short";
const std::string UpdateMetadata::PHONE_METADATA_TYPE = "phone";
const std::string UpdateMetadata::ALTERNATE_METADATA_TYPE = "alter";
const std::string UpdateMetadata::REGION_CODE_NONGEO = "001";
PhoneMetadataCollection* UpdateMetadata::phoneMetadataCollection = nullptr;
std::map<std::string, PhoneMetadata>* UpdateMetadata::shortMetadata = new std::map<std::string, PhoneMetadata>();
std::map<std::string, PhoneMetadata>* UpdateMetadata::regionMetadata = new std::map<std::string, PhoneMetadata>();
std::map<int, PhoneMetadata>* UpdateMetadata::countryMetadata = new std::map<int, PhoneMetadata>();
std::map<int, PhoneMetadata>* UpdateMetadata::alterMetadata = new std::map<int, PhoneMetadata>();

void UpdateMetadata::UpdateShortNumber(scoped_ptr<std::map<std::string, PhoneMetadata>>& metadataMap)
{
    for (auto it = shortMetadata->begin(); it != shortMetadata->end(); it++) {
        std::string code = it->first;
        PhoneMetadata phoneMetadata = it->second;
        if (metadataMap->find(code) != metadataMap->end()) {
            metadataMap->at(code) = phoneMetadata;
        } else {
            metadataMap->insert(std::make_pair(code, phoneMetadata));
        }
    }
}

void UpdateMetadata::UpdatePhoneNumber(scoped_ptr<std::map<int, PhoneMetadata>>& countryMetadataMap,
    scoped_ptr<std::map<std::string, PhoneMetadata>>& regionMetadataMap)
{
    for (auto it = regionMetadata->begin(); it != regionMetadata->end(); it++) {
        std::string code = it->first;
        PhoneMetadata phoneMetadata = it->second;
        if (regionMetadataMap->find(code) != regionMetadataMap->end()) {
            regionMetadataMap->at(code) = phoneMetadata;
        } else {
            regionMetadataMap->insert(std::make_pair(code, phoneMetadata));
        }
    }
    for (auto it = countryMetadata->begin(); it != countryMetadata->end(); it++) {
        int code = it->first;
        PhoneMetadata phoneMetadata = it->second;
        if (countryMetadataMap->find(code) != countryMetadataMap->end()) {
            countryMetadataMap->at(code) = phoneMetadata;
        } else {
            countryMetadataMap->insert(std::make_pair(code, phoneMetadata));
        }
    }
}

void UpdateMetadata::UpdateAlternateFormat(std::map<int, const PhoneMetadata*>& metadataMap)
{
    for (auto it = alterMetadata->begin(); it != alterMetadata->end(); it++) {
        int code = it->first;
        if (metadataMap.find(code) != metadataMap.end()) {
            metadataMap.at(code) = &(it->second);
        } else {
            metadataMap.insert(std::make_pair(code, &(it->second)));
        }
        
    }
}

void UpdateMetadata::LoadUpdatedMetadata(int fd)
{
    if (phoneMetadataCollection != nullptr) {
        return;
    }
    if (fd == -1) {
        return;
    }
    phoneMetadataCollection = new PhoneMetadataCollection();
    phoneMetadataCollection->ParseFromFileDescriptor(fd);
    for (google::protobuf::RepeatedPtrField<PhoneMetadata>::const_iterator it = phoneMetadataCollection->metadata().begin();
        it != phoneMetadataCollection->metadata().end(); ++it) {
        
        std::string mdyRegionCode = it->id();
        std::string type = mdyRegionCode.substr(0, PHONE_METADATA_TYPE.length());
        std::string regionCode = mdyRegionCode.substr(PHONE_METADATA_TYPE.length());
        int countryCode = it->country_code();
        PhoneMetadata phoneMetadata = *it;
        phoneMetadata.set_id(regionCode);
        if (type == SHORT_METADATA_TYPE) {
            shortMetadata->insert(std::make_pair(regionCode, phoneMetadata));
        } else if (type == PHONE_METADATA_TYPE) {
            if (regionCode != REGION_CODE_NONGEO) {
                regionMetadata->insert(std::make_pair(regionCode, phoneMetadata));
            } else {
                countryMetadata->insert(std::make_pair(countryCode, phoneMetadata));
            }
        } else if (type == ALTERNATE_METADATA_TYPE) {
            alterMetadata->insert(std::make_pair(countryCode, phoneMetadata));
        }
    }
}
}  // namespace phonenumbers
}  // namespace i18n
