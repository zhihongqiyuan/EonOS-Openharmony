/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

import { afterAll, afterEach, beforeAll, beforeEach, describe, expect, it } from 'deccjsunit/index';

const URI_CONTACTS = 'datashare:///com.ohos.contactsdataability';
const contactUri = 'datashare:///com.ohos.contactsdataability/contacts/contact';
const rawContactUri = 'datashare:///com.ohos.contactsdataability/contacts/raw_contact';
const contactDataUri = 'datashare:///com.ohos.contactsdataability/contacts/contact_data';
const groupUri = 'datashare:///com.ohos.contactsdataability/contacts/groups';
const contactBlocklistUri = 'datashare:///com.ohos.contactsdataability/contacts/contact_blocklist';
const deletedRawContactUri = 'datashare:///com.ohos.contactsdataability/contacts/deleted_raw_contact';
const profileRawContactUri = 'datashare:///com.ohos.contactsdataability/profile/raw_contact';
const profilecontactUri = 'datashare:///com.ohos.contactsdataability/profile/contact';
const profileContactDataUri = 'datashare:///com.ohos.contactsdataability/profile/contact_data';
const profileGroupUri = 'datashare:///com.ohos.contactsdataability/profile/groups';
const profileBlocklistUri = 'datashare:///com.ohos.contactsdataability/profile/contact_blocklist';
const profileDeletedContactsUri = 'datashare:///com.ohos.contactsdataability/profile/deleted_raw_contact';

const URI_CALLLOG = 'datashare:///com.ohos.calllogability';
const calllogUri = 'datashare:///com.ohos.calllogability/calls/calllog';
const INDEX_TWO = 2;
const INDEX_THREE = 3;
const INDEX_FOUR = 4;
const PHONE_NUM_LEN_SIX = 6;
const PHONE_NUM_LEN_EIGHT = 8;

export default {
  data: {},
  getCallLogInsertMap: function () {
    let phoneNumber = randomNum(PHONE_NUM_LEN_EIGHT);
    let calllogInsert100Map = new Map([
      ['phone_number', phoneNumber],
      ['display_name', 'xxw'],
      ['voicemail_uri', 'uri'],
      ['sim_type', '1'],
      ['is_hd', '2'],
      ['is_read', '1'],
      ['ring_duration', '900'],
      ['talk_duration', '200'],
      ['format_number', '123 456 789'],
      ['quicksearch_key', '2'],
      ['number_type', '2'],
      ['number_type_name', 'tian'],
      ['begin_time', '200'],
      ['end_time', '300'],
      ['answer_state', '1'],
      ['create_time', '400'],
      ['number_location', 'dizhi'],
      ['photo_id', '1'],
      ['photo_uri', '666'],
      ['country_iso_code', '2'],
      ['extra1', 'test1'],
      ['extra2', 'test2'],
      ['extra3', 'test3'],
      ['extra4', 'test4'],
      ['extra5', 'test5'],
      ['extra6', 'test6']
    ]);
    return calllogInsert100Map;
  },
  getCallLogInsert: function () {
    let phoneNumber = randomNum(PHONE_NUM_LEN_EIGHT);
    let calllogInsert100 = {
      'phone_number': phoneNumber,
      'display_name': 'xxw',
      'voicemail_uri': 'uri',
      'sim_type': 1,
      'is_hd': 2,
      'is_read': 1,
      'ring_duration': 900,
      'talk_duration': 200,
      'format_number': '123 456 789',
      'quicksearch_key': '2',
      'number_type': 2,
      'number_type_name': 'tian',
      'begin_time': 200,
      'end_time': 300,
      'answer_state': 1,
      'create_time': 400,
      'number_location': 'dizhi',
      'photo_id': 1,
      'photo_uri': '666',
      'country_iso_code': 2,
      'extra1': 'test1',
      'extra2': 'test2',
      'extra3': 'test3',
      'extra4': 'test4',
      'extra5': 'test5',
      'extra6': 'test6'
    };
    return calllogInsert100;
  },
  getCallLogResultColumns: function () {
    let resultColumns = [
      'id',
      'slot_id',
      'phone_number',
      'display_name',
      'call_direction',
      'voicemail_uri',
      'sim_type',
      'is_hd',
      'is_read',
      'ring_duration',
      'talk_duration',
      'format_number',
      'quicksearch_key',
      'number_type',
      'number_type_name',
      'begin_time',
      'end_time',
      'answer_state',
      'create_time',
      'number_location',
      'photo_id',
      'photo_uri',
      'country_iso_code',
      'extra1',
      'extra2',
      'extra3',
      'extra4',
      'extra5',
      'extra6'
    ];
    return resultColumns;
  },
  getCallLogUpdate: function () {
    let phoneNumber = randomNum(PHONE_NUM_LEN_EIGHT);
    let updateValues = {
      'phone_number': phoneNumber,
      'display_name': '6666',
      'voicemail_uri': '7777',
      'sim_type': 4,
      'is_hd': 4,
      'is_read': 4,
      'ring_duration': 400,
      'talk_duration': 400,
      'format_number': '987 654 321',
      'quicksearch_key': '4',
      'number_type': 4,
      'number_type_name': '4tian',
      'begin_time': 400,
      'end_time': 400,
      'answer_state': 4,
      'create_time': 40000,
      'number_location': '4dizhi',
      'photo_id': 4,
      'photo_uri': '4444',
      'country_iso_code': 4,
      'extra1': 'test7',
      'extra2': 'test7',
      'extra3': 'test7',
      'extra4': 'test7',
      'extra5': 'test7',
      'extra6': 'test7'
    };
    return updateValues;
  },
  getCallLogUpdateMap: function () {
    let phoneNumber = randomNum(PHONE_NUM_LEN_EIGHT);
    let calllogInsert100Map = new Map([
      ['phone_number', phoneNumber],
      ['display_name', '6666'],
      ['voicemail_uri', '7777'],
      ['sim_type', '4'],
      ['is_hd', '4'],
      ['is_read', '4'],
      ['ring_duration', '400'],
      ['talk_duration', '400'],
      ['format_number', '987 654 321'],
      ['quicksearch_key', '4'],
      ['number_type', '4'],
      ['number_type_name', '4tian'],
      ['begin_time', '400'],
      ['end_time', '400'],
      ['answer_state', '4'],
      ['create_time', '40000'],
      ['number_location', '4dizhi'],
      ['photo_id', '4'],
      ['photo_uri', '4444'],
      ['country_iso_code', '4'],
      ['extra1', 'test7'],
      ['extra2', 'test7'],
      ['extra3', 'test7'],
      ['extra4', 'test7'],
      ['extra5', 'test7'],
      ['extra6', 'test7']
    ]);
    return calllogInsert100Map;
  },
  getCallLogBatchInsert: function () {
    let phoneNumberLenThree = 3;
    let phoneNumber = randomNum(phoneNumberLenThree);
    let map = new Map([
      ['phone_number', phoneNumber],
      ['ring_duration', '333']
    ]);
    let map2 = new Map([
      ['phone_number', phoneNumber],
      ['ring_duration', '600']
    ]);
    let map3 = new Map([
      ['phone_number', phoneNumber],
      ['ring_duration', '600']
    ]);
    let map4 = new Map([
      ['phone_number', phoneNumber],
      ['ring_duration', '600']
    ]);
    let map5 = new Map([
      ['phone_number', phoneNumber],
      ['ring_duration', '600']
    ]);
    let array = [map, map2, map3, map4, map5];
    return array;
  },
  getProfileRawContact: function () {
    let rawContact = {
      'photo_id': 2,
      'photo_file_id': 3,
      'is_transfer_voicemail': 1,
      'personal_ringtone': 'test',
      'is_deleted': 0,
      'personal_notification_ringtone': '666',
      'photo_first_name': '7777',
      'account_id': 1,
      'version': 1,
      'sort': '6666',
      'contacted_count': 2,
      'lastest_contacted_time': 2,
      'favorite': 1,
      'favorite_order': 1,
      'phonetic_name': 'testname',
      'phonetic_name_type': 1,
      'display_name': 'licheng',
      'company': 'TTTTT',
      'position': 'Developer',
      'read_only': 2,
      'sort_first_letter': 'testsort',
      'sync_id': 8,
      'syn_1': 'syn_1',
      'syn_2': 'syn_2',
      'syn_3': 'syn_3'
    };
    return rawContact;
  },
  getProfileRawContactMap: function () {
    let rawContactMap = new Map([
      ['photo_id', '2'],
      ['photo_file_id', '3'],
      ['is_transfer_voicemail', '1'],
      ['personal_ringtone', 'test'],
      ['is_deleted', '0'],
      ['personal_notification_ringtone', '666'],
      ['photo_first_name', '7777'],
      ['account_id', '1'],
      ['version', '1'],
      ['sort', '6666'],
      ['contacted_count', '2'],
      ['lastest_contacted_time', '2'],
      ['favorite', '1'],
      ['favorite_order', '1'],
      ['phonetic_name', 'testname'],
      ['phonetic_name_type', '1'],
      ['display_name', 'licheng'],
      ['company', 'TTTTT'],
      ['position', 'Developer'],
      ['read_only', '2'],
      ['sort_first_letter', 'testsort'],
      ['merge_mode', '0'],
      ['is_need_merge', '1'],
      ['merge_status', '1'],
      ['is_merge_target', '0'],
      ['vibration_setting', '0'],
      ['sync_id', '8'],
      ['syn_1', 'syn_1'],
      ['syn_2', 'syn_2'],
      ['syn_3', 'syn_3']
    ]);
    return rawContactMap;
  },
  getProfileRawContactUpdate: function () {
    let rawContact = {
      'photo_id': 21,
      'photo_file_id': 31,
      'is_transfer_voicemail': 11,
      'personal_ringtone': 'test1',
      'is_deleted': 0,
      'personal_notification_ringtone': '6661',
      'photo_first_name': '77771',
      'account_id': 1,
      'sort': '66661',
      'contacted_count': 21,
      'lastest_contacted_time': 21,
      'favorite': 11,
      'favorite_order': 1,
      'phonetic_name': 'testname1',
      'phonetic_name_type': 11,
      'display_name': 'licheng1',
      'company': 'TTTTT1',
      'position': 'Developer1',
      'read_only': 21,
      'sort_first_letter': 'testsort1',
      'sync_id': 81,
      'syn_1': 'syn_11',
      'syn_2': 'syn_21',
      'syn_3': 'syn_31'
    };
    return rawContact;
  },
  getProfileRawContactUpdateMap: function () {
    let rawContactMap = new Map([
      ['photo_id', '21'],
      ['photo_file_id', '31'],
      ['is_transfer_voicemail', '11'],
      ['personal_ringtone', 'test1'],
      ['is_deleted', '0'],
      ['personal_notification_ringtone', '6661'],
      ['photo_first_name', '77771'],
      ['account_id', '1'],
      ['version', '1'],
      ['sort', '66661'],
      ['contacted_count', '21'],
      ['lastest_contacted_time', '21'],
      ['favorite', '11'],
      ['favorite_order', '1'],
      ['phonetic_name', 'testname1'],
      ['phonetic_name_type', '11'],
      ['display_name', 'licheng1'],
      ['company', 'TTTTT1'],
      ['position', 'Developer1'],
      ['read_only', '21'],
      ['sort_first_letter', 'testsort1'],
      ['merge_mode', '0'],
      ['is_need_merge', '1'],
      ['merge_status', '1'],
      ['is_merge_target', '0'],
      ['vibration_setting', '0'],
      ['sync_id', '81'],
      ['syn_1', 'syn_11'],
      ['syn_2', 'syn_21'],
      ['syn_3', 'syn_31']
    ]);
    return rawContactMap;
  },
  getProfileContactData: function () {
    let contactData = {
      'content_type': 'name',
      'detail_info': 'xiaocai',
      'family_name': 'family_name',
      'middle_name_phonetic': 'middle_name_phonetic',
      'given_name': 'given_name',
      'given_name_phonetic': 'given_name_phonetic',
      'alias_detail_info': 'alias_detail_info',
      'phonetic_name': 'phonetic_name',
      'position': 'Developer',
      'extend1': 'extend1',
      'extend2': 'extend2',
      'extend3': 'extend3',
      'extend4': 'extend4',
      'city': 'city',
      'country': 'country',
      'neighborhood': 'neighborhood',
      'pobox': 'pobox',
      'postcode': 'postcode',
      'region': 'region',
      'street': 'street',
      'alpha_name': 'alpha_name',
      'other_lan_last_name': 'other_lan_last_name',
      'other_lan_first_name': 'other_lan_first_name',
      'extend5': 'extend5',
      'lan_style': 'lan_style',
      'custom_data': 'custom_data',
      'extend6': 'extend6',
      'extend7': 'extend7',
      'blob_data': 'blob_data',
      'syn_1': 'syn_1',
      'syn_2': 'syn_2',
      'syn_3': 'syn_3'
    };
    return contactData;
  },
  getProfileContactDataMap: function () {
    let ContactDataMap = new Map([
      ['detail_info', 'xiaocai'],
      ['family_name', 'family_name'],
      ['middle_name_phonetic', 'middle_name_phonetic'],
      ['given_name', 'given_name'],
      ['given_name_phonetic', 'given_name_phonetic'],
      ['alias_detail_info', 'alias_detail_info'],
      ['phonetic_name', 'phonetic_name'],
      ['position', 'position'],
      ['extend1', 'extend1'],
      ['extend2', 'extend2'],
      ['pobox', 'pobox'],
      ['postcode', 'postcode'],
      ['city', 'city'],
      ['country', 'country'],
      ['neighborhood', 'neighborhood'],
      ['position', 'Developer'],
      ['region', 'region'],
      ['street', 'street'],
      ['alpha_name', 'alpha_name'],
      ['other_lan_last_name', 'other_lan_last_name'],
      ['other_lan_first_name', 'other_lan_first_name'],
      ['extend5', 'extend5'],
      ['lan_style', 'lan_style'],
      ['custom_data', 'custom_data'],
      ['extend6', 'extend6'],
      ['extend7', 'extend7'],
      ['blob_data', 'blob_data'],
      ['syn_1', 'syn_1'],
      ['syn_2', 'syn_2'],
      ['syn_3', 'syn_3'],
      ['read_only', '0'],
      ['version', '0'],
      ['is_preferred_number', '0']
    ]);
    return ContactDataMap;
  },
  getProfileContactDataUpdate: function () {
    let contactData = {
      'detail_info': 'xiaocai1',
      'family_name': 'family_name1',
      'middle_name_phonetic': 'middle_name_phonetic1',
      'given_name': 'given_name1',
      'given_name_phonetic': 'given_name_phonetic1',
      'alias_detail_info': 'alias_detail_info1',
      'phonetic_name': 'phonetic_name1',
      'position': 'Developer1',
      'extend1': 'extend11',
      'extend2': 'extend21',
      'extend3': 'extend31',
      'extend4': 'extend41',
      'city': 'city1',
      'country': 'country1',
      'neighborhood': 'neighborhood1',
      'pobox': 'pobox1',
      'postcode': 'postcode1',
      'region': 'region1',
      'street': 'street1',
      'alpha_name': 'alpha_name1',
      'other_lan_last_name': 'other_lan_last_name1',
      'other_lan_first_name': 'other_lan_first_name1',
      'extend5': 'extend51',
      'lan_style': 'lan_style1',
      'custom_data': 'custom_data1',
      'extend6': 'extend61',
      'extend7': 'extend71',
      'blob_data': 'blob_data1',
      'syn_1': 'syn_11',
      'syn_2': 'syn_21',
      'syn_3': 'syn_31'
    };
    return contactData;
  },
  getProfileContactDataUpdateMap: function () {
    let ContactDataMap = new Map([
      ['detail_info', 'xiaocai1'],
      ['family_name', 'family_name1'],
      ['middle_name_phonetic', 'middle_name_phonetic1'],
      ['given_name', 'given_name1'],
      ['given_name_phonetic', 'given_name_phonetic1'],
      ['alias_detail_info', 'alias_detail_info1'],
      ['phonetic_name', 'phonetic_name1'],
      ['position', 'position1'],
      ['extend1', 'extend11'],
      ['extend2', 'extend21'],
      ['pobox', 'pobox1'],
      ['postcode', 'postcode1'],
      ['city', 'city1'],
      ['country', 'country1'],
      ['neighborhood', 'neighborhood1'],
      ['position', 'Developer1'],
      ['region', 'region1'],
      ['street', 'street1'],
      ['alpha_name', 'alpha_name1'],
      ['other_lan_last_name', 'other_lan_last_name1'],
      ['other_lan_first_name', 'other_lan_first_name1'],
      ['extend5', 'extend51'],
      ['lan_style', 'lan_style1'],
      ['custom_data', 'custom_data1'],
      ['extend6', 'extend61'],
      ['extend7', 'extend71'],
      ['blob_data', 'blob_data1'],
      ['syn_1', 'syn_11'],
      ['syn_2', 'syn_21'],
      ['syn_3', 'syn_31'],
      ['read_only', '0'],
      ['version', '1'],
      ['is_preferred_number', '0']
    ]);
    return ContactDataMap;
  },
  getProfileGroup: function () {
    let group = {
      'account_id': 1,
      'group_name': 'DeveloperFirsthGroup',
      'group_notes': 'note',
      'group_ringtone': 'test'
    };
    return group;
  },
  getProfileGroupMap: function () {
    let groupMap = new Map([
      ['account_id', '1'],
      ['group_name', 'DeveloperFirsthGroup'],
      ['group_notes', 'note'],
      ['is_deleted', '0'],
      ['group_ringtone', 'test'],
      ['ringtone_modify_time', '0'],
      ['lastest_modify_time', '0']
    ]);
    return groupMap;
  },
  getProfileBlockList: function () {
    let phoneNumber = randomNum(PHONE_NUM_LEN_EIGHT);
    let blockList = { 'phone_number': phoneNumber, 'content': 'heimingdan' };
    return blockList;
  },
  getProfileBlockListMap: function () {
    let phoneNumber = randomNum(PHONE_NUM_LEN_EIGHT);
    let blockListMap = new Map([
      ['types', '0'],
      ['phone_number', phoneNumber],
      ['content', 'heimingdan'],
      ['time_stamp', '0']
    ]);
    return blockListMap;
  },
  getProfileUpdateRawContact: function () {
    let rawContact = {
      'photo_id': 8,
      'photo_file_id': 8,
      'is_transfer_voicemail': 8,
      'personal_ringtone': 'test8',
      'is_deleted': 1,
      'personal_notification_ringtone': '888',
      'photo_first_name': '9999',
      'account_id': 8,
      'version': 2,
      'sort': '8888',
      'contacted_count': 8,
      'lastest_contacted_time': 8,
      'favorite': 0,
      'favorite_order': 1,
      'phonetic_name': 'testname8',
      'phonetic_name_type': 8,
      'display_name': 'licheng8',
      'company': 'TTTTT8',
      'position': 'Developer8',
      'read_only': 8,
      'sort_first_letter': 'testsort8',
      'sync_id': 888,
      'syn_1': 'syn_8',
      'syn_2': 'syn_8',
      'syn_3': 'syn_8'
    };
    return rawContact;
  },
  getProfileUpdateRawContactMap: function () {
    let rawContactMap = new Map([
      ['photo_id', '8'],
      ['photo_file_id', '8'],
      ['is_transfer_voicemail', '8'],
      ['personal_ringtone', 'test8'],
      ['is_deleted', '1'],
      ['personal_notification_ringtone', '888'],
      ['photo_first_name', '9999'],
      ['account_id', '8'],
      ['version', '2'],
      ['sort', '8888'],
      ['contacted_count', '8'],
      ['lastest_contacted_time', '8'],
      ['favorite', '0'],
      ['favorite_order', '1'],
      ['phonetic_name', 'testname8'],
      ['phonetic_name_type', '8'],
      ['display_name', 'licheng8'],
      ['company', 'TTTTT8'],
      ['position', 'Developer8'],
      ['read_only', '8'],
      ['sort_first_letter', 'testsort8'],
      ['merge_mode', '0'],
      ['is_need_merge', '1'],
      ['merge_status', '1'],
      ['is_merge_target', '0'],
      ['vibration_setting', '0'],
      ['sync_id', '888'],
      ['syn_1', 'syn_8'],
      ['syn_2', 'syn_8'],
      ['syn_3', 'syn_8']
    ]);
    return rawContactMap;
  },
  getProfileUpdateContactData: function () {
    let contactData = {
      'detail_info': 'BossCai',
      'family_name': 'family_name8',
      'middle_name_phonetic': 'middle_name_phonetic8',
      'given_name': 'given_name8',
      'given_name_phonetic': 'given_name_phonetic8',
      'alias_detail_info': 'alias_detail_info8',
      'phonetic_name': 'phonetic_name8',
      'position': 'position8',
      'extend1': 'extend8',
      'extend2': 'extend8',
      'extend3': 'extend8',
      'extend4': 'extend8',
      'country': 'country8',
      'neighborhood': 'neighborhood8',
      'pobox': 'pobox8',
      'city': 'city8',
      'postcode': 'postcode8',
      'region': 'region8',
      'street': 'street8',
      'alpha_name': 'alpha_name8',
      'other_lan_last_name': 'other_lan_last_name8',
      'other_lan_first_name': 'other_lan_first_name8',
      'extend5': 'extend8',
      'lan_style': 'lan_style8',
      'custom_data': 'custom_data8',
      'extend6': 'extend8',
      'extend7': 'extend8',
      'blob_data': 'blob_data8',
      'syn_1': 'syn_8',
      'syn_2': 'syn_8',
      'syn_3': 'syn_8'
    };
    return contactData;
  },
  getProfileUpdateContactDataMap: function () {
    let ContactDataMap = new Map([
      ['detail_info', 'BossCai'],
      ['family_name', 'family_name8'],
      ['middle_name_phonetic', 'middle_name_phonetic8'],
      ['given_name', 'given_name8'],
      ['given_name_phonetic', 'given_name_phonetic8'],
      ['alias_detail_info', 'alias_detail_info8'],
      ['phonetic_name', 'phonetic_name8'],
      ['position', 'position8'],
      ['extend1', 'extend8'],
      ['extend2', 'extend8'],
      ['pobox', 'pobox8'],
      ['postcode', 'postcode8'],
      ['city', 'city8'],
      ['country', 'country8'],
      ['neighborhood', 'neighborhood8'],
      ['position', 'position8'],
      ['region', 'region8'],
      ['street', 'street8'],
      ['alpha_name', 'alpha_name8'],
      ['other_lan_last_name', 'other_lan_last_name8'],
      ['other_lan_first_name', 'other_lan_first_name8'],
      ['extend5', 'extend8'],
      ['lan_style', 'lan_style8'],
      ['custom_data', 'custom_data8'],
      ['extend6', 'extend8'],
      ['extend7', 'extend8'],
      ['blob_data', 'blob_data8'],
      ['syn_1', 'syn_8'],
      ['syn_2', 'syn_8'],
      ['syn_3', 'syn_8'],
      ['read_only', '0'],
      ['version', '1'],
      ['is_preferred_number', '0']
    ]);
    return ContactDataMap;
  },
  getProfileUpdateGroup: function () {
    let group = {
      'account_id': 8,
      'group_name': 'TestFifthGroup',
      'group_notes': 'note8',
      'group_ringtone': 'test8'
    };
    return group;
  },
  getProfileUpdateGroupMap: function () {
    let groupMap = new Map([
      ['account_id', '8'],
      ['group_name', 'TestFifthGroup'],
      ['group_notes', 'note8'],
      ['is_deleted', '0'],
      ['group_ringtone', 'test8'],
      ['ringtone_modify_time', '0'],
      ['lastest_modify_time', '0']
    ]);
    return groupMap;
  },
  getProfileUpdateBlockList: function () {
    let phoneNumber = randomNum(PHONE_NUM_LEN_EIGHT);
    let blockList = {
      'phone_number': phoneNumber,
      'content': 'danminghei'
    };
    return blockList;
  },
  getProfileUpdateBlockListMap: function () {
    let phoneNumber = randomNum(PHONE_NUM_LEN_EIGHT);
    let blockListMap = new Map([
      ['types', '0'],
      ['phone_number', phoneNumber],
      ['content', 'danminghei'],
      ['time_stamp', '0']
    ]);
    return blockListMap;
  },
  getVoiceMail: function () {
    let phoneNumberLenFour = 4;
    let phoneNumber = randomNum(phoneNumberLenFour);
    let voicemail = {
      'phone_number': phoneNumber,
      'quicksearch_key': '777',
      'display_name': 'xiaoli',
      'voicemail_uri': 'XXXX',
      'origin_type': 'xxxxx',
      'create_time': 0
    };
    return voicemail;
  },
  getVoiceMailMap: function () {
    let phoneNumberLenFour = 4;
    let phoneNumber = randomNum(phoneNumberLenFour);
    let map = new Map([
      ['phone_number', phoneNumber],
      ['quicksearch_key', '777'],
      ['display_name', 'xiaoli'],
      ['voicemail_uri', 'XXXX'],
      ['origin_type', 'xxxxx'],
      ['create_time', '0'],
      ['voicemail_type', '0'],
      ['voice_file_size', '0'],
      ['voice_duration', '0'],
      ['voice_status', '0']
    ]);
    return map;
  },
  getVoiceMailUpdate: function () {
    let phoneNumber = randomNum(PHONE_NUM_LEN_SIX);
    let voicemail = {
      'phone_number': phoneNumber,
      'quicksearch_key': '999',
      'display_name': 'xiaocai',
      'voicemail_uri': 'uuuu',
      'origin_type': 'iiiii',
      'create_time': '999'
    };
    return voicemail;
  },
  getVoiceMailUpdateMap: function () {
    let phoneNumber = randomNum(PHONE_NUM_LEN_SIX);
    let map = new Map([
      ['phone_number', phoneNumber],
      ['quicksearch_key', '999'],
      ['display_name', 'xiaocai'],
      ['voicemail_uri', 'uuuu'],
      ['origin_type', 'iiiii'],
      ['create_time', '999'],
      ['voicemail_type', '0'],
      ['voice_file_size', '0'],
      ['voice_duration', '0'],
      ['voice_status', '0']
    ]);
    return map;
  },
  getVoiceMailBatchInsert: function () {
    let phoneNumberLenThree = 3;
    let phoneNumber = randomNum(phoneNumberLenThree);
    let map = new Map([
      ['phone_number', phoneNumber],
      ['display_name', 'xiaoming'],
      ['voice_status', '0']
    ]);
    let map2 = new Map([
      ['phone_number', phoneNumber],
      ['display_name', 'xiaohei'],
      ['voice_status', '1']
    ]);
    let map3 = new Map([
      ['phone_number', phoneNumber],
      ['display_name', 'xiaohong'],
      ['voice_status', '0']
    ]);
    let map4 = new Map([
      ['phone_number', phoneNumber],
      ['display_name', 'xiaohei'],
      ['voice_status', '0']
    ]);
    let map5 = new Map([
      ['phone_number', phoneNumber],
      ['display_name', 'xiaofen'],
      ['voice_status', '0']
    ]);
    let array = [map, map2, map3, map4, map5];
    return array;
  },
  getContactBatchCompany: function () {
    let rawContactValuesOne = { 'display_name': 'fengyuanyuan', 'company': 'TT4900', 'position': 'Testers' };
    let rawContactValuesTwo = { 'display_name': 'caiboss', 'company': 'TT4900', 'position': 'developer' };
    let rawContactValuesThree = { 'display_name': 'zhangsan', 'company': 'TT4900', 'position': 'developer' };
    let rawContactValuesFour = { 'display_name': 'lisi', 'company': 'TT4900', 'position': 'developer' };
    let rawContactValuesFive = { 'display_name': 'wangwu', 'company': 'TT4900', 'position': 'developer' };
    let listAddBluk = [];
    listAddBluk[0] = rawContactValuesOne;
    listAddBluk[1] = rawContactValuesTwo;
    listAddBluk[INDEX_TWO] = rawContactValuesThree;
    listAddBluk[INDEX_THREE] = rawContactValuesFour;
    listAddBluk[INDEX_FOUR] = rawContactValuesFive;
    return listAddBluk;
  },
  getContactBatchCompanyTwo: function () {
    let contactDataValuesOne = {
      'raw_contact_id': rawContactId,
      'content_type': 'name',
      'detail_info': 'xiaozhi'
    };
    let contactDataValuesTwo = {
      'raw_contact_id': rawContactId,
      'content_type': 'organization',
      'detail_info': 'TT',
      'position': 'Testers'
    };
    let contactDataValuesThree = {
      'raw_contact_id': rawContactId,
      'content_type': 'email',
      'detail_info': '15844@163.com'
    };
    let contactDataValuesFour = {
      'raw_contact_id': rawContactId,
      'content_type': 'nickname',
      'detail_info': 'xiaozhicheng'
    };
    let contactDataValuesFive = {
      'raw_contact_id': rawContactId,
      'content_type': 'phone',
      'detail_info': '111885566'
    };
    let listAddBluk = [];
    listAddBluk[0] = contactDataValuesOne;
    listAddBluk[1] = contactDataValuesTwo;
    listAddBluk[INDEX_TWO] = contactDataValuesThree;
    listAddBluk[INDEX_THREE] = contactDataValuesFour;
    listAddBluk[INDEX_FOUR] = contactDataValuesFive;
    return listAddBluk;
  },
  getPhoneNumberBatch: function () {
    let phoneNumberLenThirteen = 13;
    let phoneNumber = randomNum(phoneNumberLenThirteen);
    let contactBlocklistValuesOne = { 'phone_number': phoneNumber };
    let contactBlocklistValuesTwo = { 'phone_number': phoneNumber };
    let contactBlocklistValuesThree = { 'phone_number': phoneNumber };
    let contactBlocklistValuesFour = { 'phone_number': phoneNumber };
    let contactBlocklistValuesFive = { 'phone_number': phoneNumber };
    let listAddBluk = [];
    listAddBluk[0] = contactBlocklistValuesOne;
    listAddBluk[1] = contactBlocklistValuesTwo;
    listAddBluk[INDEX_TWO] = contactBlocklistValuesThree;
    listAddBluk[INDEX_THREE] = contactBlocklistValuesFour;
    listAddBluk[INDEX_FOUR] = contactBlocklistValuesFive;
    return listAddBluk;
  },
  getSearchMap: function () {
    let map = new Map([
      ['account_id', '1'],
      ['search_name', '小黄||xiaohuang||xh'],
      ['display_name', '小黄'],
      ['phonetic_name', ''],
      ['photo_id', ''],
      ['photo_file_id', '']
    ]);
    return map;
  }
};

function randomNum(num) {
  let baseNumNine = 9;
  let baseNumTen = 10;
  let number = toString(Math.floor(Math.random() * (baseNumNine * Math.pow(baseNumTen, num))) + 1 *
    Math.pow(baseNumTen, num));
  return number;
}
