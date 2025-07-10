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

#include "contacts_build.h"

#include "result_convert.h"

namespace OHOS {
namespace ContactsApi {
ContactsBuild::ContactsBuild(void)
{
}

ContactsBuild::~ContactsBuild()
{
}

void ContactsBuild::GetContactDataByObject(napi_env env, napi_value object, Contacts &contact)
{
    contact.id = GetIntValueByKey(env, object, "id");
    contact.key = GetStringValueByKey(env, object, "key");
    contact.emails = GetEmails(env, object);
    contact.events = GetEvent(env, object);
    contact.groups = GetGroup(env, object);
    contact.imAddresses = GetImAddress(env, object);
    contact.phoneNumbers = GetPhoneNumbers(env, object);
    contact.portrait = GetUri(env, object);
    contact.relations = GetRelation(env, object);
    contact.sipAddresses = GetSipAddress(env, object);
    contact.websites = GetWebsite(env, object);
    contact.name = GetName(env, object);
    contact.nickName = GetNickName(env, object);
    contact.note = GetNote(env, object);
    contact.organization = GetOrganization(env, object);
    contact.postalAddresses = GetPostalAddress(env, object);
}

void ContactsBuild::GetContactData(napi_env env, napi_value object,
    std::vector<DataShare::DataShareValuesBucket> &valueContact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    Contacts contact;
    GetContactDataByObject(env, object, contact);
    BuildValueContact(contact, valueContact);
    BuildValueContactData(contact, valueContactData);
}

void ContactsBuild::BuildValueContact(Contacts &contact, std::vector<DataShare::DataShareValuesBucket> &valueContact)
{
    DataShare::DataShareValuesBucket valuesBucketContact;
    if (!contact.name.fullName.empty() || contact.name.fullName != "") {
        valuesBucketContact.Put("display_name", contact.name.fullName);
    }
    if (!contact.organization.name.empty() || contact.organization.name != "") {
        valuesBucketContact.Put("company", contact.organization.name);
    }
    if (!contact.organization.title.empty() || contact.organization.title != "") {
        valuesBucketContact.Put("position", contact.organization.title);
    }
    valueContact.push_back(valuesBucketContact);
}

void ContactsBuild::BuildValueContactData(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    GetValuesBucketEmail(contact, valueContactData);
    GetValuesBucketEvent(contact, valueContactData);
    GetValuesBucketGroup(contact, valueContactData);
    GetValuesBucketImAddress(contact, valueContactData);
    GetValuesBucketPortrait(contact, valueContactData);
    GetValuesBucketPhoneNumber(contact, valueContactData);
    GetValuesBucketPostalAddress(contact, valueContactData);
    GetValuesBucketRelation(contact, valueContactData);
    GetValuesBucketSipAddress(contact, valueContactData);
    GetValuesBucketWebsite(contact, valueContactData);
    GetValuesBucketName(contact, valueContactData);
    GetValuesBucketNickName(contact, valueContactData);
    GetValuesBucketNote(contact, valueContactData);
    GetValuesBucketOrganization(contact, valueContactData);
}

void ContactsBuild::BuildValueContactDataByType(
    Contacts &contact, int typeId, std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    switch (typeId) {
        case EMAIL:
            GetValuesBucketEmail(contact, valueContactData);
            break;
        case IM:
            GetValuesBucketImAddress(contact, valueContactData);
            break;
        case NICKNAME:
            GetValuesBucketNickName(contact, valueContactData);
            break;
        case ORGANIZATION:
            GetValuesBucketOrganization(contact, valueContactData);
            break;
        case PHONE:
            GetValuesBucketPhoneNumber(contact, valueContactData);
            break;
        case NAME:
            GetValuesBucketName(contact, valueContactData);
            break;
        case POSTAL_ADDRESS:
            GetValuesBucketPostalAddress(contact, valueContactData);
            break;
        default:
            TypeSwitchSplit(typeId, contact, valueContactData);
            break;
    }
}

void ContactsBuild::TypeSwitchSplit(
    int typeId, Contacts &contact, std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    switch (typeId) {
        case PHOTO:
            GetValuesBucketPortrait(contact, valueContactData);
            break;
        case GROUP_MEMBERSHIP:
            GetValuesBucketGroup(contact, valueContactData);
            break;
        case NOTE:
            GetValuesBucketNote(contact, valueContactData);
            break;
        case CONTACT_EVENT:
            GetValuesBucketEvent(contact, valueContactData);
            break;
        case WEBSITE:
            GetValuesBucketWebsite(contact, valueContactData);
            break;
        case RELATION:
            GetValuesBucketRelation(contact, valueContactData);
            break;
        case CONTACT_MISC:
            HILOG_INFO("TypeSwitchSplit is CONTACT_MISC ");
            break;
        case HICALL_DEVICE:
            HILOG_INFO("TypeSwitchSplit is HICALL_DEVICE ");
            break;
        case CAMCARD:
        case SIP_ADDRESS:
            GetValuesBucketSipAddress(contact, valueContactData);
            break;
        default:
            HILOG_ERROR("TypeSwitchSplit type is error ");
            break;
    }
}

/**
 * @brief Establish ValuesBucket condition in EMAIL case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketEmail(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    unsigned int emailsSize = contact.emails.size();
    for (unsigned int i = 0; i < emailsSize; i++) {
        DataShare::DataShareValuesBucket valuesBucketEmail;
        valuesBucketEmail.Put("detail_info", contact.emails[i].email);
        if (contact.emails[i].labelId != 0) {
            valuesBucketEmail.Put("extend7", std::to_string(contact.emails[i].labelId));
        }
        if (!contact.emails[i].labelName.empty()) {
            valuesBucketEmail.Put("custom_data", contact.emails[i].labelName);
            valuesBucketEmail.Put("extend7", std::to_string(Email::CUSTOM_LABEL));
        }
        if (!contact.emails[i].displayName.empty()) {
            valuesBucketEmail.Put("alias_detail_info", contact.emails[i].displayName);
        }
        valuesBucketEmail.Put("content_type", "email");
        valueContactData.push_back(valuesBucketEmail);
    }
}

/**
 * @brief Establish ValuesBucket condition in CONTACT_EVENT case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketEvent(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    unsigned int eventsSize = contact.events.size();
    for (unsigned int i = 0; i < eventsSize; i++) {
        DataShare::DataShareValuesBucket valuesBucketEvent;
        valuesBucketEvent.Put("detail_info", contact.events[i].eventDate);
        if (contact.events[i].labelId != 0) {
            valuesBucketEvent.Put("extend7", std::to_string(contact.events[i].labelId));
        }
        if (!contact.events[i].labelName.empty()) {
            valuesBucketEvent.Put("custom_data", contact.events[i].labelName);
            valuesBucketEvent.Put("extend7", std::to_string(Event::CUSTOM_LABEL));
        }
        valuesBucketEvent.Put("content_type", "contact_event");
        valueContactData.push_back(valuesBucketEvent);
    }
}

/**
 * @brief Establish ValuesBucket condition in GROUP_MEMBERSHIP case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketGroup(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    unsigned int groupsSize = contact.groups.size();
    for (unsigned int i = 0; i < groupsSize; i++) {
        DataShare::DataShareValuesBucket valuesBucketGroup;
        if (contact.groups[i].groupId != ERROR) {
            valuesBucketGroup.Put("detail_info", std::to_string(contact.groups[i].groupId));
        }
        valuesBucketGroup.Put("alias_detail_info", contact.groups[i].title);
        valuesBucketGroup.Put("content_type", "group_membership");
        valueContactData.push_back(valuesBucketGroup);
    }
}

/**
 * @brief Establish ValuesBucket condition in Im case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketImAddress(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    unsigned int imAddressSize = contact.imAddresses.size();
    for (unsigned int i = 0; i < imAddressSize; i++) {
        DataShare::DataShareValuesBucket valuesBucketImAddress;
        valuesBucketImAddress.Put("detail_info", contact.imAddresses[i].imAddress);
        if (contact.imAddresses[i].labelId != 0) {
            valuesBucketImAddress.Put("extend7", std::to_string(contact.imAddresses[i].labelId));
        }
        if (!contact.imAddresses[i].labelName.empty()) {
            valuesBucketImAddress.Put("custom_data", contact.imAddresses[i].labelName);
            valuesBucketImAddress.Put("extend7", std::to_string(ImAddress::CUSTOM_LABEL));
        }
        valuesBucketImAddress.Put("content_type", "im");
        valueContactData.push_back(valuesBucketImAddress);
    }
}

/**
 * @brief Establish ValuesBucket condition in PHOTO case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketPortrait(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    if (!contact.portrait.uri.empty() || contact.portrait.uri != "") {
        DataShare::DataShareValuesBucket valuesBucketPortrait;
        valuesBucketPortrait.Put("detail_info", contact.portrait.uri);
        valuesBucketPortrait.Put("content_type", "photo");
        valueContactData.push_back(valuesBucketPortrait);
    }
}

/**
 * @brief Establish ValuesBucket condition in PHONE case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketPhoneNumber(
    Contacts &contact, std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    unsigned int phoneNumbersSize = contact.phoneNumbers.size();
    for (unsigned int i = 0; i < phoneNumbersSize; i++) {
        DataShare::DataShareValuesBucket valuesBucketPhoneNumber;
        valuesBucketPhoneNumber.Put("detail_info", contact.phoneNumbers[i].phoneNumber);
        if (contact.phoneNumbers[i].labelId != 0) {
            valuesBucketPhoneNumber.Put("extend7", std::to_string(contact.phoneNumbers[i].labelId));
        }
        if (!contact.phoneNumbers[i].labelName.empty()) {
            valuesBucketPhoneNumber.Put("custom_data", contact.phoneNumbers[i].labelName);
            valuesBucketPhoneNumber.Put("extend7", std::to_string(PhoneNumber::CUSTOM_LABEL));
        }
        valuesBucketPhoneNumber.Put("content_type", "phone");
        valueContactData.push_back(valuesBucketPhoneNumber);
    }
}

/**
 * @brief Establish ValuesBucket condition in POSTAL_ADDRESS case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketPostalAddress(
    Contacts &contact, std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    unsigned int postalAddressesSize = contact.postalAddresses.size();
    for (unsigned int i = 0; i < postalAddressesSize; i++) {
        DataShare::DataShareValuesBucket valuesBucketPostalAddress;
        valuesBucketPostalAddress.Put("detail_info", contact.postalAddresses[i].postalAddress);
        if (contact.postalAddresses[i].labelId != 0) {
            valuesBucketPostalAddress.Put("extend7", std::to_string(contact.postalAddresses[i].labelId));
        }
        if (!contact.postalAddresses[i].labelName.empty()) {
            valuesBucketPostalAddress.Put("custom_data", contact.postalAddresses[i].labelName);
            valuesBucketPostalAddress.Put("extend7", std::to_string(PostalAddress::CUSTOM_LABEL));
        }
        if (!contact.postalAddresses[i].neighborhood.empty()) {
            valuesBucketPostalAddress.Put("neighborhood", contact.postalAddresses[i].neighborhood);
        }
        if (!contact.postalAddresses[i].pobox.empty()) {
            valuesBucketPostalAddress.Put("pobox", contact.postalAddresses[i].pobox);
        }
        if (!contact.postalAddresses[i].postcode.empty()) {
            valuesBucketPostalAddress.Put("postcode", contact.postalAddresses[i].postcode);
        }
        if (!contact.postalAddresses[i].region.empty()) {
            valuesBucketPostalAddress.Put("region", contact.postalAddresses[i].region);
        }
        if (!contact.postalAddresses[i].street.empty()) {
            valuesBucketPostalAddress.Put("street", contact.postalAddresses[i].street);
        }
        if (!contact.postalAddresses[i].city.empty()) {
            valuesBucketPostalAddress.Put("city", contact.postalAddresses[i].city);
        }
        if (!contact.postalAddresses[i].country.empty()) {
            valuesBucketPostalAddress.Put("country", contact.postalAddresses[i].country);
        }
        valuesBucketPostalAddress.Put("content_type", "postal_address");
        valueContactData.push_back(valuesBucketPostalAddress);
    }
}

/**
 * @brief Establish ValuesBucket condition in RELATION case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketRelation(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    unsigned int relationsSize = contact.relations.size();
    for (unsigned int i = 0; i < relationsSize; i++) {
        DataShare::DataShareValuesBucket valuesBucketRelation;
        valuesBucketRelation.Put("detail_info", contact.relations[i].relationName);
        if (contact.relations[i].labelId != 0) {
            valuesBucketRelation.Put("extend7", std::to_string(contact.relations[i].labelId));
        }
        if (!contact.relations[i].labelName.empty()) {
            valuesBucketRelation.Put("custom_data", contact.relations[i].labelName);
            valuesBucketRelation.Put("extend7", std::to_string(Relation::CUSTOM_LABEL));
        }
        valuesBucketRelation.Put("content_type", "relation");
        valueContactData.push_back(valuesBucketRelation);
    }
}

/**
 * @brief Establish ValuesBucket condition in SIP_ADDRESS case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketSipAddress(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    unsigned int sipAddressesSize = contact.sipAddresses.size();
    for (unsigned int i = 0; i < sipAddressesSize; i++) {
        DataShare::DataShareValuesBucket valuesBucketSipAddress;
        valuesBucketSipAddress.Put("detail_info", contact.sipAddresses[i].sipAddress);
        if (contact.sipAddresses[i].labelId != 0) {
            valuesBucketSipAddress.Put("extend7", std::to_string(contact.sipAddresses[i].labelId));
        }
        if (!contact.sipAddresses[i].labelName.empty()) {
            valuesBucketSipAddress.Put("custom_data", contact.sipAddresses[i].labelName);
            valuesBucketSipAddress.Put("extend7", std::to_string(SipAddress::CUSTOM_LABEL));
        }
        valuesBucketSipAddress.Put("content_type", "sip_address");
        valueContactData.push_back(valuesBucketSipAddress);
    }
}

/**
 * @brief Establish ValuesBucket condition in WEBSITE case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketWebsite(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    unsigned int websitesSize = contact.websites.size();
    for (unsigned int i = 0; i < websitesSize; i++) {
        DataShare::DataShareValuesBucket valuesBucketWebsite;
        valuesBucketWebsite.Put("detail_info", contact.websites[i].website);
        valuesBucketWebsite.Put("content_type", "website");
        valueContactData.push_back(valuesBucketWebsite);
    }
}

/**
 * @brief Establish ValuesBucket condition in NAME case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketName(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    if (!contact.name.fullName.empty() || contact.name.fullName != "") {
        DataShare::DataShareValuesBucket valuesBucketName;
        valuesBucketName.Put("detail_info", contact.name.fullName);
        if (!contact.name.namePrefix.empty()) {
            valuesBucketName.Put("alpha_name", contact.name.namePrefix);
        }
        if (!contact.name.middleName.empty()) {
            valuesBucketName.Put("other_lan_last_name", contact.name.middleName);
        }
        if (!contact.name.nameSuffix.empty()) {
            valuesBucketName.Put("other_lan_first_name", contact.name.nameSuffix);
        }
        if (!contact.name.familyName.empty()) {
            valuesBucketName.Put("family_name", contact.name.familyName);
        }
        if (!contact.name.middleNamePhonetic.empty()) {
            valuesBucketName.Put("middle_name_phonetic", contact.name.middleNamePhonetic);
        }
        if (!contact.name.givenName.empty()) {
            valuesBucketName.Put("given_name", contact.name.givenName);
        }
        if (!contact.name.givenNamePhonetic.empty()) {
            valuesBucketName.Put("given_name_phonetic", contact.name.givenNamePhonetic);
        }
        if (!contact.name.familyNamePhonetic.empty()) {
            valuesBucketName.Put("phonetic_name", contact.name.familyNamePhonetic);
        }
        valuesBucketName.Put("content_type", "name");
        valueContactData.push_back(valuesBucketName);
    }
}

/**
 * @brief Establish ValuesBucket condition in NICKNAME case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketNickName(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    if (!contact.nickName.nickName.empty() || contact.nickName.nickName != "") {
        DataShare::DataShareValuesBucket valuesBucketNickName;
        valuesBucketNickName.Put("detail_info", contact.nickName.nickName);
        valuesBucketNickName.Put("content_type", "nickname");
        valueContactData.push_back(valuesBucketNickName);
    }
}

/**
 * @brief Establish ValuesBucket condition in NOTE case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketNote(Contacts &contact,
    std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    if (!contact.note.noteContent.empty() || contact.note.noteContent != "") {
        DataShare::DataShareValuesBucket valuesBucketNote;
        valuesBucketNote.Put("detail_info", contact.note.noteContent);
        valuesBucketNote.Put("content_type", "note");
        valueContactData.push_back(valuesBucketNote);
    }
}

/**
 * @brief Establish ValuesBucket condition in ORGANIZATION case
 *
 * @param contact Conditions for establish ValuesBucket operation
 * @param valueContactData Conditions for establish ValuesBucket operation
 */
void ContactsBuild::GetValuesBucketOrganization(
    Contacts &contact, std::vector<DataShare::DataShareValuesBucket> &valueContactData)
{
    if (!contact.organization.name.empty()) {
        DataShare::DataShareValuesBucket valuesBucketData;
        valuesBucketData.Put("detail_info", contact.organization.name);
        if (!contact.organization.title.empty()) {
            valuesBucketData.Put("position", contact.organization.title);
        }
        valuesBucketData.Put("content_type", "organization");
        valueContactData.push_back(valuesBucketData);
    }
}

napi_value ContactsBuild::GetObjectByKey(napi_env env, napi_value object, std::string key)
{
    ResultConvert resultConvert;
    return resultConvert.GetNapiValue(env, key.c_str(), object);
}

napi_value ContactsBuild::GetArrayByKey(napi_env env, napi_value valueObject, std::string key)
{
    ResultConvert resultConvert;
    napi_value array = resultConvert.GetNapiValue(env, key.c_str(), valueObject);
    bool isArray = false;
    napi_is_array(env, array, &isArray);
    if (!isArray) {
        HILOG_INFO("ContactsBuild GetArrayByKey value is not array");
        return nullptr;
    }
    return array;
}

std::string ContactsBuild::GetStringValueByKey(napi_env env, napi_value valueObject, std::string key)
{
    ResultConvert resultConvert;
    napi_value value = resultConvert.GetNapiValue(env, key.c_str(), valueObject);
    std::string result = NapiGetValueString(env, value);
    return result;
}

int ContactsBuild::GetIntValueByKey(napi_env env, napi_value valueObject, std::string key)
{
    ResultConvert resultConvert;
    napi_value value = resultConvert.GetNapiValue(env, key.c_str(), valueObject);
    if (value == nullptr) {
        return ERROR;
    }
    int64_t result;
    napi_get_value_int64(env, value, &result);
    int code = result;
    return code;
}

Name ContactsBuild::GetName(napi_env env, napi_value object)
{
    HILOG_INFO("ContactsBuild into GetName");
    Name name;
    if (object == nullptr) {
        HILOG_ERROR("ContactsBuild GetName  nameKey is null ");
        return name;
    }
    napi_value nameObj = GetObjectByKey(env, object, "name");
    napi_valuetype valueType;
    napi_typeof(env, nameObj, &valueType);
    if (nameObj == nullptr || valueType != napi_object) {
        HILOG_ERROR("ContactsBuild GetName nameObj is null or object type is not object");
        return name;
    }
    name.familyName = GetStringValueByKey(env, nameObj, "familyName");
    name.middleNamePhonetic = GetStringValueByKey(env, nameObj, "middleNamePhonetic");
    name.givenName = GetStringValueByKey(env, nameObj, "givenName");
    name.givenNamePhonetic = GetStringValueByKey(env, nameObj, "givenNamePhonetic");
    name.familyNamePhonetic = GetStringValueByKey(env, nameObj, "familyNamePhonetic");
    name.fullName = GetStringValueByKey(env, nameObj, "fullName");
    name.middleName = GetStringValueByKey(env, nameObj, "middleName");
    name.namePrefix = GetStringValueByKey(env, nameObj, "namePrefix");
    name.nameSuffix = GetStringValueByKey(env, nameObj, "nameSuffix");
    return name;
}

Portrait ContactsBuild::GetUri(napi_env env, napi_value object)
{
    Portrait portrait;
    napi_value portraitObj = GetObjectByKey(env, object, "portrait");
    napi_valuetype valueType;
    napi_typeof(env, portraitObj, &valueType);
    if (portraitObj == nullptr || valueType != napi_object) {
        HILOG_ERROR("ContactsBuild GetUri portraitObj is null or object type is not object");
        return portrait;
    }
    portrait.uri = GetStringValueByKey(env, portraitObj, "uri");
    return portrait;
}

std::vector<Email> ContactsBuild::GetEmails(napi_env env, napi_value object)
{
    std::vector<Email> emailVec;
    napi_value EmailArray = GetArrayByKey(env, object, "emails");
    if (EmailArray == nullptr) {
        HILOG_ERROR("ContactsBuild GetEmail napiValueEmail is null ");
        return emailVec;
    }
    uint32_t size = 0;
    napi_get_array_length(env, EmailArray, &size);
    for (uint32_t i = 0; i < size; i++) {
        Email email;
        napi_value object;
        napi_get_element(env, EmailArray, i, &object);
        email.email = GetStringValueByKey(env, object, "email");
        email.labelName = GetStringValueByKey(env, object, "labelName");
        email.displayName = GetStringValueByKey(env, object, "displayName");
        email.labelId = GetIntValueByKey(env, object, "labelId");
        emailVec.push_back(email);
    }
    return emailVec;
}

std::vector<Event> ContactsBuild::GetEvent(napi_env env, napi_value object)
{
    napi_value eventArray = GetArrayByKey(env, object, "events");
    if (eventArray == nullptr) {
        HILOG_ERROR("ContactsBuild GetEvent arrKey is null ");
        return std::vector<Event>();
    }
    uint32_t size = 0;
    napi_get_array_length(env, eventArray, &size);
    std::vector<Event> resultVec;
    for (uint32_t i = 0; i < size; i++) {
        napi_value obj;
        napi_get_element(env, eventArray, i, &obj);
        Event eventObj;
        eventObj.eventDate = GetStringValueByKey(env, obj, "eventDate");
        eventObj.labelName = GetStringValueByKey(env, obj, "labelName");
        eventObj.labelId = GetIntValueByKey(env, obj, "labelId");
        resultVec.push_back(eventObj);
    }
    return resultVec;
}

std::vector<Group> ContactsBuild::GetGroup(napi_env env, napi_value object)
{
    napi_value groupArray = GetArrayByKey(env, object, "groups");
    std::vector<Group> resultVec;
    if (groupArray == nullptr) {
        HILOG_ERROR("ContactsBuild GetGroup arrKey is null ");
        return resultVec;
    }
    uint32_t size = 0;
    napi_get_array_length(env, groupArray, &size);
    for (uint32_t i = 0; i < size; i++) {
        napi_value obj;
        napi_get_element(env, groupArray, i, &obj);
        Group groupObj;
        groupObj.groupId = GetIntValueByKey(env, obj, "groupId");
        groupObj.title = GetStringValueByKey(env, obj, "title");
        resultVec.push_back(groupObj);
    }
    return resultVec;
}

std::vector<ImAddress> ContactsBuild::GetImAddress(napi_env env, napi_value object)
{
    napi_value imAddressArray = GetArrayByKey(env, object, "imAddresses");
    std::vector<ImAddress> resultVec;
    if (imAddressArray == nullptr) {
        HILOG_ERROR("ContactsBuild GetImAddress arrKey is null ");
        return resultVec;
    }
    uint32_t size = 0;
    napi_get_array_length(env, imAddressArray, &size);
    for (uint32_t i = 0; i < size; i++) {
        napi_value obj;
        napi_get_element(env, imAddressArray, i, &obj);
        ImAddress targetObj;
        targetObj.imAddress = GetStringValueByKey(env, obj, "imAddress");
        targetObj.labelName = GetStringValueByKey(env, obj, "labelName");
        targetObj.labelId = GetIntValueByKey(env, obj, "labelId");
        resultVec.push_back(targetObj);
    }
    return resultVec;
}

std::vector<PhoneNumber> ContactsBuild::GetPhoneNumbers(napi_env env, napi_value object)
{
    std::vector<PhoneNumber> resultVec;
    napi_value phoneNumberArray = GetArrayByKey(env, object, "phoneNumbers");
    if (phoneNumberArray == nullptr) {
        HILOG_ERROR("ContactsBuild GetPhoneNumber napiValuePhoneNumber is null ");
        return resultVec;
    }
    uint32_t size = 0;
    napi_get_array_length(env, phoneNumberArray, &size);
    for (uint32_t i = 0; i < size; i++) {
        PhoneNumber phones;
        napi_value obj;
        napi_get_element(env, phoneNumberArray, i, &obj);
        phones.phoneNumber = GetStringValueByKey(env, obj, "phoneNumber");
        phones.labelName = GetStringValueByKey(env, obj, "labelName");
        phones.labelId = GetIntValueByKey(env, obj, "labelId");
        resultVec.push_back(phones);
    }
    return resultVec;
}

std::vector<PostalAddress> ContactsBuild::GetPostalAddress(napi_env env, napi_value object)
{
    napi_value postalAddressArray = GetArrayByKey(env, object, "postalAddresses");
    std::vector<PostalAddress> resultVec;
    if (postalAddressArray == nullptr) {
        HILOG_ERROR("GetPostalAddress arrKey is null ");
        return resultVec;
    }
    uint32_t size = 0;
    napi_get_array_length(env, postalAddressArray, &size);
    for (uint32_t i = 0; i < size; i++) {
        napi_value obj;
        napi_get_element(env, postalAddressArray, i, &obj);
        PostalAddress targetObj;
        targetObj.city = GetStringValueByKey(env, obj, "city");
        targetObj.country = GetStringValueByKey(env, obj, "country");
        targetObj.labelName = GetStringValueByKey(env, obj, "labelName");
        targetObj.neighborhood = GetStringValueByKey(env, obj, "neighborhood");
        targetObj.pobox = GetStringValueByKey(env, obj, "pobox");
        targetObj.postalAddress = GetStringValueByKey(env, obj, "postalAddress");
        targetObj.postcode = GetStringValueByKey(env, obj, "postcode");
        targetObj.region = GetStringValueByKey(env, obj, "region");
        targetObj.street = GetStringValueByKey(env, obj, "street");
        targetObj.labelId = GetIntValueByKey(env, obj, "labelId");
        resultVec.push_back(targetObj);
    }
    return resultVec;
}

std::vector<Relation> ContactsBuild::GetRelation(napi_env env, napi_value object)
{
    napi_value relationArray = GetArrayByKey(env, object, "relations");
    std::vector<Relation> resultVec;
    if (relationArray == nullptr) {
        HILOG_ERROR("GetRelation arrKey is null ");
        return resultVec;
    }
    uint32_t size = 0;
    napi_get_array_length(env, relationArray, &size);
    for (uint32_t i = 0; i < size; i++) {
        napi_value obj;
        napi_get_element(env, relationArray, i, &obj);
        Relation targetObj;
        targetObj.relationName = GetStringValueByKey(env, obj, "relationName");
        targetObj.labelName = GetStringValueByKey(env, obj, "labelName");
        targetObj.labelId = GetIntValueByKey(env, obj, "labelId");
        resultVec.push_back(targetObj);
    }
    return resultVec;
}

std::vector<SipAddress> ContactsBuild::GetSipAddress(napi_env env, napi_value object)
{
    napi_value sipAddressArray = GetArrayByKey(env, object, "sipAddresses");
    std::vector<SipAddress> resultVec;
    if (sipAddressArray == nullptr) {
        HILOG_ERROR("GetSipAddress arrKey is null ");
        return resultVec;
    }
    uint32_t size = 0;
    napi_get_array_length(env, sipAddressArray, &size);
    for (uint32_t i = 0; i < size; i++) {
        napi_value obj;
        napi_get_element(env, sipAddressArray, i, &obj);
        SipAddress targetObj;
        targetObj.sipAddress = GetStringValueByKey(env, obj, "sipAddress");
        targetObj.labelName = GetStringValueByKey(env, obj, "labelName");
        targetObj.labelId = GetIntValueByKey(env, obj, "labelId");
        resultVec.push_back(targetObj);
    }
    return resultVec;
}

std::vector<Website> ContactsBuild::GetWebsite(napi_env env, napi_value object)
{
    napi_value websiteArray = GetArrayByKey(env, object, "websites");
    std::vector<Website> resultVec;
    if (websiteArray == nullptr) {
        HILOG_ERROR("ContactsBuild GetWebsite arrKey is null ");
        return resultVec;
    }
    uint32_t size = 0;
    napi_get_array_length(env, websiteArray, &size);
    for (uint32_t i = 0; i < size; i++) {
        napi_value obj;
        napi_get_element(env, websiteArray, i, &obj);
        Website targetObj;
        targetObj.website = GetStringValueByKey(env, obj, "website");
        resultVec.push_back(targetObj);
    }
    return resultVec;
}

NickName ContactsBuild::GetNickName(napi_env env, napi_value object)
{
    HILOG_INFO("ContactsBuild into GetNickName");
    napi_value nickNameObj = GetObjectByKey(env, object, "nickName");
    NickName resultObj;
    if (nickNameObj == nullptr) {
        HILOG_ERROR("ContactsBuild GetNickName nickNameKey is null ");
        return resultObj;
    }
    resultObj.nickName = GetStringValueByKey(env, nickNameObj, "nickName");
    return resultObj;
}

Note ContactsBuild::GetNote(napi_env env, napi_value object)
{
    Note resultObj;
    napi_value noteObj = GetObjectByKey(env, object, "note");
    if (noteObj == nullptr) {
        HILOG_ERROR("GetNote noteKey is null ");
        return resultObj;
    }
    resultObj.noteContent = GetStringValueByKey(env, noteObj, "noteContent");
    return resultObj;
}

Organization ContactsBuild::GetOrganization(napi_env env, napi_value object)
{
    napi_value organizationObject = GetObjectByKey(env, object, "organization");
    Organization resultObj;
    if (organizationObject == nullptr) {
        HILOG_ERROR("GetOrganization organizationKey is null ");
        return resultObj;
    }
    resultObj.name = GetStringValueByKey(env, organizationObject, "name");
    resultObj.title = GetStringValueByKey(env, organizationObject, "title");
    return resultObj;
}

int ContactsBuild::GetInt(napi_env env, napi_value id)
{
    int64_t value = 0;
    if (id == nullptr) {
        HILOG_ERROR("GetInt id is 0");
        return value;
    }
    napi_get_value_int64(env, id, &value);
    return value;
}

Holder ContactsBuild::GetHolder(napi_env env, napi_value object)
{
    Holder holder;
    if (object == nullptr) {
        HILOG_ERROR("GetHolder Holder is null ");
        return holder;
    }
    holder.bundleName = GetStringValueByKey(env, object, "bundleName");
    holder.displayName = GetStringValueByKey(env, object, "displayName");
    holder.holderId = GetIntValueByKey(env, object, "holderId");
    HILOG_INFO(" ContactsBuild::GetHolder int id = %{public}d", holder.holderId);
    return holder;
}

ContactAttributes ContactsBuild::GetContactAttributes(napi_env env, napi_value object)
{
    ContactAttributes contactAttributes;
    if (object == nullptr) {
        HILOG_ERROR("GetContactAttributes object is null ");
        return contactAttributes;
    }
    napi_value napiValueAttr = GetArrayByKey(env, object, "attributes");
    std::vector<int> attrVector;
    uint32_t size = 0;
    napi_get_array_length(env, napiValueAttr, &size);
    for (uint32_t i = 0; i < size; i++) {
        napi_value intValue;
        napi_get_element(env, napiValueAttr, i, &intValue);
        int64_t intNapiValue;
        napi_get_value_int64(env, intValue, &intNapiValue);
        if (intNapiValue != 0) {
            attrVector.push_back(intNapiValue);
        }
    }
    contactAttributes.attributes = attrVector;
    return contactAttributes;
}

std::string ContactsBuild::NapiGetValueString(napi_env env, napi_value value)
{
    if (value == nullptr) {
        return "";
    }
    char valueString[NAPI_GET_STRING_SIZE];
    size_t valueSize = NAPI_GET_STRING_SIZE;
    size_t resultSize = 0;
    napi_get_value_string_utf8(env, value, valueString, valueSize, &resultSize);
    std::string resultValue = valueString;
    if (resultValue == "") {
        HILOG_ERROR("ContactsBuild NapiGetValueString Data error");
        return "";
    }
    return resultValue;
}
} // namespace ContactsApi
} // namespace OHOS