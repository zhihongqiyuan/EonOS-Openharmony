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

import dataShare from '@ohos.data.dataShare';
import {afterAll, afterEach, beforeAll, beforeEach, describe, expect, it} from 'deccjsunit/index'

import common from './common.js';

const URI_CONTACTS = "datashare:///com.ohos.contactsdataability";
const contactUri = "datashare:///com.ohos.contactsdataability/contacts/contact";
const rawContactUri = "datashare:///com.ohos.contactsdataability/contacts/raw_contact";
const contactDataUri = "datashare:///com.ohos.contactsdataability/contacts/contact_data";
const groupUri = "datashare:///com.ohos.contactsdataability/contacts/groups";
const contactBlocklistUri = "datashare:///com.ohos.contactsdataability/contacts/contact_blocklist";
const deletedRawContactUri = "datashare:///com.ohos.contactsdataability/contacts/deleted_raw_contact";
const searchContactUri = "datashare:///com.ohos.contactsdataability/contacts/search_contact";
const deletedUri = "datashare:///com.ohos.contactsdataability/contacts/deleted_raw_contact";
const deleted_raw_contact_record = "datashare:///com.ohos.contactsdataability/contacts/deleted_raw_contact_record";

describe('ContactsTest', function() {
    console.info("-------ContactsTest is starting!--------");

    var sleep_two = 2000;
    var sleep_one = 1000;
    var array_two = 2;
    var array_three = 3;
    var array_four = 4;

    function sleep(numberMillis)
    {
        var now = new Date();
        var exitTime = now.getTime() + numberMillis;
        while (true) {
            now = new Date();
            if (now.getTime() > exitTime)
                return;
        }
    }

    async function contactsQuery(map, tag, uri)
    {
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info(tag + ': contactsQuery start ! dataShareHelper = ' + dataShareHelper);
        var resultColumns = [];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", map.get("id"));
        try {
            var resultSet = await dataShareHelper.query(uri, resultColumns, condition);
            sleep(sleep_two);
            if (resultSet.rowCount > 0) {
                if (resultSet.goToFirstRow()) {
                    do {
                        for (var [key, value] of map) {
                            let dbresult = resultSet.getString(resultSet.getColumnIndex(key));
                            console.info(tag + ' :logMessage contactsQuery key = ' + key + ' dbresult :' + dbresult +
                                         ' value : ' + value);
                            expect(value == dbresult).assertEqual(true);
                        }
                    } while (resultSet.goToNextRow());
                }
            }
            resultSet.close();
        } catch (error) {
            console.info(tag + "logMessage contactsQuery: error = " + error);
        }
    }

    async function queryBatch(map, tag, size)
    {
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info(tag + ': queryBatch start ! dataShareHelper = ' + dataShareHelper);
        var resultColumns = [];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", map.get("raw_contact_id"));
        try {
            var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
            sleep(sleep_one);
            console.info(tag + ': queryBatch start ! resultSet.rowCount = ' + resultSet.rowCount);
            expect(resultSet.rowCount == size).assertEqual(true);
            resultSet.close();
        } catch (error) {
            console.info(tag + "logMessage resultSet.rowCount: error = " + error);
        }
    }

    async function queryIdForDelete(map, tag, uri)
    {
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info(tag + ': queryIdForDelete start ! dataShareHelper = ' + dataShareHelper);
        let resultColumns = common.getCallLogResultColumns();
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", map.get("id"));
        try {
            var resultSet = await dataShareHelper.query(uri, resultColumns, condition);
            sleep(sleep_one);
            expect(resultSet.goToFirstRow() == false).assertTrue();
            console.info(tag + " :logMessage queryIdForDelete: goToFirstRow " + resultSet.goToFirstRow());
            resultSet.close();
        } catch (error) {
            console.info(tag + " :logMessage queryIdForDelete: error = " + error);
        }
    }

    async function deleteAll(uri, tag)
    {
        try {
            let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
            let condition = new dataShare.DataSharePredicates();
            condition.greaterThan("id", "0");
            var deleteCode = await dataShareHelper.delete(uri, condition);
            console.info(tag + ': deleteAll deleteCode = ' + deleteCode);
            expect(deleteCode == 0).assertTrue();
        } catch (error) {
            console.info(tag + ': deleteAll error = ' + error);
        }
    }

    async function deleteRawContact(tag)
    {
        try {
            let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
            let condition = new dataShare.DataSharePredicates();
            condition.greaterThan("id", "0");
            condition.and();
            condition.equalTo("is_deleted", "0");
            var deleteCode = await dataShareHelper.delete(rawContactUri, condition);
            console.info(tag + ': deleteRawContact deleteCode = ' + deleteCode);
            expect(deleteCode == 0).assertTrue();
            sleep(sleep_one);
            var conditionAll = new dataShare.DataSharePredicates();
            conditionAll.greaterThan("id", "0");
            var code = await dataShareHelper.delete(deletedUri, conditionAll);
            console.info(tag + ': Completely delete code = ' + code);
            expect(code == 0).assertTrue();
        } catch (error) {
            console.info(tag + ': deleteRawContact error = ' + error);
        }
    }

    async function insertData(rawContactId, type, value, position)
    {
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        var contactDataValues1 =
            {"raw_contact_id" : rawContactId, "content_type" : type, "detail_info" : value, "position" : position};
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            console.info("logMessage : contactDataId1 " + contactDataId1);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info("logMessage : contact_data name insert error = " + error);
        }
        return contactDataId;
    }

    /**
     * @tc.number  contact_insert_test_100
     * @tc.name    Insert contact data
     * @tc.desc    Function test
     */
    it("contact_insert_test_100", 0, async function(done) {
        console.info("------logMessage contact_insert_test_100 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, common.getProfileRawContact());
            sleep(sleep_one);
            console.info('logMessage contact_insert_test_100: rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var map = common.getProfileRawContactMap();
            map.set("id", rawContactId.toString());
            await contactsQuery(map, "contact_insert_test_100", rawContactUri);
            await deleteRawContact("contact_insert_test_100");
            done();
        } catch (error) {
            console.info('logMessage contact_insert_test_100: insert error = ' + error);
            done();
        }
    });

    /**
     * @tc.number  contact_insert_test_400
     * @tc.name    Insert contact details (name, company, position, mobile phone number)
     * @tc.desc    Function test
     */
    it("contact_insert_test_400", 0, async function(done) {
        console.info("------logMessage contact_insert_test_400 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocheng",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insert_test_400: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await dataNameInsert(dataShareHelper, rawContactId);
            await dataCompanyInsert(dataShareHelper, rawContactId);
            await dataPhoneInsert(dataShareHelper, rawContactId);
            await deleteRawContact("contact_insert_test_400");
            await deleteAll(contactDataUri, "contact_insert_test_400");
            done();
        } catch (error) {
            console.info("logMessage contact_insert_test_400: raw_contact insert error = " + error);
            done();
        }
    });

    async function dataNameInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues1 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "name",
            "detail_info" : "xiaocheng"
        };
        try {
            var contactDataId1 = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            console.info("logMessage contact_insert_test_400: contactDataId1 " + contactDataId1);
            expect(contactDataId1 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId1.toString());
            map.set("detail_info", "xiaocheng");
            map.set("raw_contact_id", rawContactId.toString());
            await contactsQuery(map, "contact_insert_test_400", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_400: contact_data name insert error = " + error);
            done();
        }
    }

    async function dataCompanyInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues2 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "organization",
            "detail_info" : "TT",
            "position" : "developer"
        };
        try {
            var contactDataId2 = await dataShareHelper.insert(contactDataUri, contactDataValues2);
            console.info("logMessage contact_insert_test_400: contactDataId2 " + contactDataId2);
            expect(contactDataId2 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId2.toString());
            map.set("detail_info", "TT");
            map.set("position", "developer");
            contactsQuery(map, "contact_insert_test_400", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_400: contact_data company insert error = " + error);
            done();
        }
    }

    async function dataPhoneInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues3 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "phone",
            "detail_info" : "12345678"
        };
        try {
            var contactDataId3 = await dataShareHelper.insert(contactDataUri, contactDataValues3);
            sleep(sleep_one);
            console.info("logMessage contact_insert_test_400: contactDataId3 " + contactDataId3);
            expect(contactDataId3 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId3.toString());
            map.set("detail_info", "12345678");
            await contactsQuery(map, "contact_insert_test_400", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_400: contact_data phone insert error = " + error);
            done();
        }
    }

    /**
     * @tc.number  contact_insert_test_500
     * @tc.name    Insert contact details (nickname, email, home)
     * @tc.desc    Function test
     */
    it("contact_insert_test_500", 0, async function(done) {
        console.info("------logMessage contact_insert_test_500 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocheng",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insert_test_500: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await dataEmailInsert(dataShareHelper, rawContactId);
            await dataNickNameInsert(dataShareHelper, rawContactId);
            await dataPostalAddressInsert(dataShareHelper, rawContactId);
            await deleteRawContact("contact_insert_test_500");
            await deleteAll(contactDataUri, "contact_insert_test_500");
            done();
        } catch (error) {
            console.info("logMessage contact_insert_test_500: raw_contact insert error = " + error);
            done();
        }
    });

    async function dataEmailInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues1 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "email",
            "detail_info" : "166@163.com"
        };
        try {
            var contactDataId1 = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            sleep(sleep_one);
            console.info("logMessage contact_insert_test_500: contactDataId1 " + contactDataId1);
            expect(contactDataId1 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId1.toString());
            map.set("detail_info", "166@163.com");
            await contactsQuery(map, "contact_insert_test_500_1", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_500: contact_data email insert error = " + error);
        }
    }

    async function dataNickNameInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues2 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "nickname",
            "detail_info" : "xiaocheng"
        };
        try {
            var contactDataId2 = await dataShareHelper.insert(contactDataUri, contactDataValues2);
            console.info("logMessage contact_insert_test_500: contactDataId2 " + contactDataId2);
            expect(contactDataId2 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId2.toString());
            map.set("detail_info", "xiaocheng");
            await contactsQuery(map, "contact_insert_test_500_2", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_500: contact_data nickname insert error = " + error);
        }
    }

    async function dataPostalAddressInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues3 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "postal_address",
            "detail_info" : "NanJingSoftRoad"
        };
        try {
            var contactDataId3 = await dataShareHelper.insert(contactDataUri, contactDataValues3);
            console.info("logMessage contact_insert_test_500: contactDataId3 " + contactDataId3);
            expect(contactDataId3 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId3.toString());
            map.set("detail_info", "NanJingSoftRoad");
            await contactsQuery(map, "contact_insert_test_500_3", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_500: contact_data postal_address insert error = " + error);
        }
    }

    /**
     * @tc.number  contact_insert_test_600
     * @tc.name    Insert contact details (remarks, AIM, birthday)
     * @tc.desc    Function test
     */
    it("contact_insert_test_600", 0, async function(done) {
        console.info("------logMessage contact_insert_test_600 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocheng",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues)
            console.info("logMessage contact_insert_test_600: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await dataNoteInsert(dataShareHelper, rawContactId);
            await dataAimInsert(dataShareHelper, rawContactId);
            await dataEventInsert(dataShareHelper, rawContactId);
            await deleteRawContact("contact_insert_test_600");
            await deleteAll(contactDataUri, "contact_insert_test_600");
            done();
        } catch (error) {
            console.info("logMessage contact_insert_test_600: raw_contact insert error = " + error);
            done();
        }
    });

    async function dataNoteInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues1 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "note",
            "detail_info" : "javaGangster"
        };
        try {
            var contactDataId1 = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            console.info("logMessage contact_insert_test_600: contactDataId1 " + contactDataId1);
            expect(contactDataId1 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId1.toString());
            map.set("detail_info", "javaGangster");
            await contactsQuery(map, "contact_insert_test_600_1", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_600: contact_data note insert error = " + error);
            done();
        }
    }

    async function dataAimInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues2 = {"raw_contact_id" : rawContactId, "content_type" : "im", "detail_info" : "aaaaa"};
        try {
            var contactDataId2 = await dataShareHelper.insert(contactDataUri, contactDataValues2);
            console.info("logMessage contact_insert_test_600: contactDataId2 " + contactDataId2);
            expect(contactDataId2 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId1.toString());
            map.set("detail_info", "aaaaa");
            await contactsQuery(map, "contact_insert_test_600_2", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_600: contact_data aim insert error = " + error);
        }
    }

    async function dataEventInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues3 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "contact_event",
            "detail_info" : "19960229"
        };
        try {
            var contactDataId3 = await dataShareHelper.insert(contactDataUri, contactDataValues3);
            console.info("logMessage contact_insert_test_600: contactDataId3 " + contactDataId3);
            expect(contactDataId3 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId1.toString());
            map.set("detail_info", "19960229");
            await contactsQuery(map, "contact_insert_test_600_3", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_600: contact_data event insert error = " + error);
        }
    }

    /**
     * @tc.number  contact_insert_test_700
     * @tc.name    Insert contact details (URL, assistant, group)
     * @tc.desc    Function test
     */
    it("contact_insert_test_700", 0, async function(done) {
        console.info("------logMessage contact_insert_test_700 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocheng",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insert_test_700: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await dataWebsiteInsert(dataShareHelper, rawContactId);
            await dataRelationInsert(dataShareHelper, rawContactId);
            await dataWebsiteInsert(dataShareHelper, rawContactId);
            await deleteRawContact("contact_insert_test_700");
            await deleteAll(contactDataUri, "contact_insert_test_700");
            done();
        } catch (error) {
            console.info("logMessage contact_insert_test_700: raw_contact insert error = " + error);
            done();
        }
    });

    async function dataWebsiteInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues1 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "website",
            "detail_info" : "www.com"
        };
        try {
            var contactDataId1 = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            sleep(sleep_one);
            console.info("logMessage contact_insert_test_700: contactDataId1 " + contactDataId1);
            expect(contactDataId1 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId1.toString());
            map.set("detail_info", "www.com");
            await contactsQuery(map, "contact_insert_test_700_1", contactDataUri);
            await ContactDataRelationInsert();
        } catch (error) {
            console.info("logMessage contact_insert_test_700: contact_data website insert error = " + error);
        }
    }

    async function dataRelationInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues2 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "relation",
            "detail_info" : "Secretary"
        };
        try {
            var contactDataId2 = await dataShareHelper.insert(contactDataUri, contactDataValues2);
            sleep(sleep_one);
            console.info("logMessage contact_insert_test_700: contactDataId2 " + contactDataId2);
            expect(contactDataId2 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId2.toString());
            map.set("detail_info", "Secretary");
            await contactsQuery(map, "contact_insert_test_700_2", contactDataUri);
            await ContactDataGroupInsert();
        } catch (error) {
            console.info("logMessage contact_insert_test_700: contact_data relation insert error = " + error);
        }
    }

    async function dataGroupInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues3 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "group_membership",
            "detail_info" : "1"
        };
        try {
            var contactDataId3 = await dataShareHelper.insert(contactDataUri, contactDataValues3);
            sleep(sleep_one);
            console.info("logMessage contact_insert_test_700: contactDataId3 " + contactDataId3);
            expect(contactDataId3 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId3.toString());
            map.set("detail_info", "1");
            await contactsQuery(map, "contact_insert_test_700_3", contactDataUri);
            done();
        } catch (error) {
            console.info("logMessage contact_insert_test_700: contact_data group insert error = " + error);
        }
    }

    /**
     * @tc.number  contact_insert_test_800
     * @tc.name    Insert contact details (phone ringtone)
     * @tc.desc    Function test
     */
    it("contact_insert_test_800", 0, async function(done) {
        console.info("------logMessage contact_insert_test_800 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocheng",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insert_test_800: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await dataMiscInsert(dataShareHelper, rawContactId);
            await deleteRawContact("contact_insert_test_800");
            await deleteAll(contactDataUri, "contact_insert_test_800");
            done();
        } catch (error) {
            console.info("logMessage contact_insert_test_800: raw_contact insert error = " + error);
            done();
        }
    });

    async function dataMiscInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues1 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "contact_misc",
            "detail_info" : "1234"
        };
        try {
            var contactDataId1 = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            sleep(sleep_one);
            console.info("logMessage contact_insert_test_800: contactDataId1 " + contactDataId1);
            expect(contactDataId1 > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId1.toString());
            map.set("detail_info", "1234");
            await contactsQuery(map, "contact_insert_test_800_1", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_800: contact_data misc insert error = " + error);
        }
    }

    /**
     * @tc.number  contact_update_test_900
     * @tc.name    Update contact data
     * @tc.desc    Function test
     */
    it("contact_update_test_900", 0, async function(done) {
        console.info("------logMessage contact_update_test_900 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "licheng",
            "company" : "TT",
            "position" : "developer",
            "favorite" : 1,
            "phonetic_name" : "licheng||lc"
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_900: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await rawContactUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_900");
            await deleteAll(contactDataUri, "contact_update_test_900");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_900: raw_contact insert error = " + error);
            done();
        }
    });

    async function rawContactUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {
            "display_name" : "dacheng",
            "company" : "oo",
            "position" : "Testers",
            "favorite" : 0,
            "phonetic_name" : "dacheng||dc"
        };
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(rawContactUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_900: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", rawContactId.toString());
            map.set("display_name", "dacheng");
            map.set("company", "oo");
            map.set("position", "Testers");
            map.set("favorite", "0");
            map.set("phonetic_name", "dacheng||dc");
            await contactsQuery(map, "contact_update_test_900", rawContactUri);
        } catch (error) {
            console.info('logMessage contact_update_test_900: raw_contact update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_1200
     * @tc.name    Update contact details (name)
     * @tc.desc    Function test
     */
    it("contact_update_test_1200", 0, async function(done) {
        console.info("------logMessage contact_update_test_1200 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_1200: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforedataNameInsert(dataShareHelper, rawContactId);
            await dataNameUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_1200");
            await deleteAll(contactDataUri, "contact_update_test_1200");
            done();
        } catch (error) {
            console.info('logMessage contact_update_test_1200: raw_contact insert error = ' + error);
            done();
        }
    });

    async function beforedataNameInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {"raw_contact_id" : rawContactId, "content_type" : "name", "detail_info" : "xiaoyuan"};
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1200: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_1200: contact_data insert error = ' + error);
        }
    }

    async function dataNameUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "dayuan"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1200: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", rawContactId.toString());
            map.set("display_name", "dayuan");
            await contactsQuery(map, "contact_update_test_1200", rawContactUri);
        } catch (error) {
            console.info('logMessage contact_update_test_1200: contact_data update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_1300
     * @tc.name    Update contact details (company, position)
     * @tc.desc    Function test
     */
    it("contact_update_test_1300", 0, async function(done) {
        console.info("------logMessage contact_update_test_1300 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_1300: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforedataCompanyInsert(dataShareHelper, rawContactId);
            await dataCompanyUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_1300");
            await deleteAll(contactDataUri, "contact_update_test_1300");
            done();
        } catch (error) {
            console.info('logMessage contact_update_test_1300: raw_contact insert error = ' + error);
            done();
        }
    });

    async function beforedataCompanyInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "organization",
            "detail_info" : "TT",
            "position" : "Testers",
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1300: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_1300: contact_data insert error = ' + error);
        }
    }

    async function dataCompanyUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "OOOO", "position" : "developer"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1300: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", rawContactId.toString());
            map.set("company", "OOOO");
            map.set("position", "developer");
            await contactsQuery(map, "contact_update_test_1300", rawContactUri);
        } catch (error) {
            console.info('logMessage contact_update_test_1300: contact_data update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_1400
     * @tc.name    Update contact details (mobile phone number)
     * @tc.desc    Function test
     */
    it("contact_update_test_1400", 0, async function(done) {
        console.info("------logMessage contact_update_test_1400 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_1400: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforeDataPhoneInsert(dataShareHelper, rawContactId);
            await dataPhoneUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_1400");
            await deleteAll(contactDataUri, "contact_update_test_1400");
            done();
        } catch (error) {
            console.info('logMessage contact_update_test_1400: raw_contact insert error = ' + error);
            done();
        }
    });

    async function beforeDataPhoneInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {"raw_contact_id" : rawContactId, "content_type" : "phone", "detail_info" : "123456"};
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1400: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_1400: contact_data insert error = ' + error);
        }
    }

    async function dataPhoneUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "99663355"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1400: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "99663355");
            await contactsQuery(map, "contact_update_test_1400", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_1400: contact_data update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_1500
     * @tc.name    Update contact details (nickname)
     * @tc.desc    Function test
     */
    it("contact_update_test_1500", 0, async function(done) {
        console.info("------logMessage contact_update_test_1500 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_1500: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforedataNickNameInsert(dataShareHelper, rawContactId);
            await dataNickNameUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_1500");
            await deleteAll(contactDataUri, "contact_update_test_1500");
            done();
        } catch (error) {
            console.info('logMessage contact_update_test_1500: raw_contact insert error = ' + error);
            done();
        }
    });

    async function beforedataNickNameInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "nickname",
            "detail_info" : "xiaoyuan"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1500: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_1500: contact_data insert error = ' + error);
        }
    }

    async function dataNickNameUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "fengyuan"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1500: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "fengyuan");
            await contactsQuery(map, "contact_update_test_1500", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_1500: contact_data update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_1600
     * @tc.name    Update contact details (email)
     * @tc.desc    Function test
     */
    it("contact_update_test_1600", 0, async function(done) {
        console.info("------logMessage contact_update_test_1600 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_1600: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforeDataEmailInsert(dataShareHelper, rawContactId);
            await dataEmailUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_1600");
            await deleteAll(contactDataUri, "contact_update_test_1600");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_1600: raw_contact insert error = " + error);
            done();
        }
    });

    async function beforeDataEmailInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "email",
            "detail_info" : "166@163.com"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1600: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_1600: contact_data insert error = ' + error);
        }
    }

    async function dataEmailUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "199@163.com"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition)
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1600: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "199@163.com");
            await contactsQuery(map, "contact_update_test_1600", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_1600: update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_1700
     * @tc.name    Update Contact Details (Residential)
     * @tc.desc    Function test
     */
    it("contact_update_test_1700", 0, async function(done) {
        console.info("------logMessage contact_update_test_1700 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_1700: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforeDataPostalInsert(dataShareHelper, rawContactId);
            await dataPostalUpdate();
            await deleteRawContact("contact_update_test_1700");
            await deleteAll(contactDataUri, "contact_update_test_1700");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_1700: raw_contact insert error = " + error);
            done();
        }
    });

    async function beforeDataPostalInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "postal_address",
            "detail_info" : "NanJing"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1700: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_update_test_1700: contact_data insert error = " + error);
        }
    }

    async function dataPostalUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "BeiJing"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1700: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "BeiJing");
            await contactsQuery(map, "contact_update_test_1700", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_1700: update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_1800
     * @tc.name    Update contact details (remarks)
     * @tc.desc    Function test
     */
    it("contact_update_test_1800", 0, async function(done) {
        console.info("------logMessage contact_update_test_1800 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_1800: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforedataNoteInsert(dataShareHelper, rawContactId);
            await dataNoteUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_1800");
            await deleteAll(contactDataUri, "contact_update_test_1800");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_1800: raw_contact insert error = " + error);
            done();
        }
    });

    async function beforedataNoteInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {"raw_contact_id" : rawContactId, "content_type" : "note", "detail_info" : "Gangster"};
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1800: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_1800: contact_data insert error = ' + error);
        }
    }

    async function dataNoteUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "God"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1800: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "God");
            await contactsQuery(map, "contact_update_test_1800", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_1800: update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_1900
     * @tc.name    Update Contact Details (AIM)
     * @tc.desc    Function test
     */
    it("contact_update_test_1900", 0, async function(done) {
        console.info("------logMessage contact_update_test_1900 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues)
            console.info("logMessage contact_update_test_1900: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforeDataAimInsert(dataShareHelper, rawContactId);
            await dataAimUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_1900");
            await deleteAll(contactDataUri, "contact_update_test_1900");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_1900: raw_contact insert error = " + error);
            done();
        }
    });

    async function beforeDataAimInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {"raw_contact_id" : rawContactId, "content_type" : "im", "detail_info" : "aaaaa"};
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1900: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_update_test_1900: contact_data insert error = " + error);
        }
    }

    async function dataAimUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "bbbbb"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_1900: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "bbbbb");
            await contactsQuery(map, "contact_update_test_1900", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_1900: update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_2000
     * @tc.name    Update Contact Details (Birthday)
     * @tc.desc    Function test
     */
    it("contact_update_test_2000", 0, async function(done) {
        console.info("------logMessage contact_update_test_2000 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_2000: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforeDataEventInsert(dataShareHelper, rawContactId);
            await dataEventUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_2000");
            await deleteAll(contactDataUri, "contact_update_test_2000");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_2000: raw_contact insert error = " + error);
            done();
        }
    });

    async function beforeDataEventInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues1 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "contact_event",
            "detail_info" : "19960229"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_2000: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_2000: contact_data insert error = ' + error);
        }
    }

    async function dataEventUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "19971021"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_2000: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "19971021");
            await contactsQuery(map, "contact_update_test_2000", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_2000: update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_2100
     * @tc.name    Update contact details (website)
     * @tc.desc    Function test
     */
    it("contact_update_test_2100", 0, async function(done) {
        console.info("------logMessage contact_update_test_2100 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_2100: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforeDataWebsiteInsert(dataShareHelper, rawContactId);
            await dataWebsiteUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_2100");
            await deleteAll(contactDataUri, "contact_update_test_2100");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_2100: raw_contact insert error = " + error);
            done();
        }
    });

    async function beforeDataWebsiteInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues1 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "website",
            "detail_info" : "www.com"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_2100: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_2100: contact_data insert error = ' + error);
        }
    }

    async function dataWebsiteUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "www.123.com"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_2100: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "www.123.com");
            await contactsQuery(map, "contact_update_test_2100", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_2100: update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_2200
     * @tc.name    Update contact details (assistant)
     * @tc.desc    Function test
     */
    it("contact_update_test_2200", 0, async function(done) {
        console.info("------logMessage contact_update_test_2200 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_2200: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforeDataRelationInsert(dataShareHelper, rawContactId);
            await dataRelationUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_2200");
            await deleteAll(contactDataUri, "contact_update_test_2200");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_2200: raw_contact insert error = " + error);
            done();
        }
    });

    async function beforeDataRelationInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "relation",
            "detail_info" : "Secretary"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            console.info('logMessage contact_update_test_2200: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_2200: contact_data insert error = ' + error);
        }
    }

    async function dataRelationUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "spouse"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_2200: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "spouse");
            await contactsQuery(map, "contact_update_test_2200", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_2200: update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_2300
     * @tc.name    Update contact detail data table (group)
     * @tc.desc    Function test
     */
    it("contact_update_test_2300", 0, async function(done) {
        console.info("------logMessage contact_update_test_2300 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_update_test_2300: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforeDataGroupInsert(dataShareHelper, rawContactId);
            await dataGroupUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_2300");
            await deleteAll(contactDataUri, "contact_update_test_2300");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_2300: raw_contact insert error = " + error);
            done();
        }
    });

    async function beforeDataGroupInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "group_membership",
            "detail_info" : "2"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_2300: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_2300: contact_data insert error = ' + error);
        }
    }

    async function dataGroupUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "7"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_2300: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "3");
            await contactsQuery(map, "contact_update_test_2300", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_2300: update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_update_test_2400
     * @tc.name    Update contact detail data table (phone ringtone)
     * @tc.desc    Function test
     */
    it("contact_update_test_2400", 0, async function(done) {
        console.info("------logMessage contact_update_test_2400 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoyuan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_update_test_2400: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await beforeDataMiscInsert(dataShareHelper, rawContactId);
            await dataMiscUpdate(dataShareHelper, rawContactId);
            await deleteRawContact("contact_update_test_2400");
            await deleteAll(contactDataUri, "contact_update_test_2400");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_2400: raw_contact insert error = " + error);
            done();
        }
    });

    async function beforeDataMiscInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues1 = {
            "raw_contact_id" : rawContactId,
            "content_type" : "contact_misc",
            "detail_info" : "1234"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_2400: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info('logMessage contact_update_test_2400: contact_data insert error = ' + error);
        }
    }

    async function dataMiscUpdate(dataShareHelper, rawContactId)
    {
        var updateValues = {"detail_info" : "999"};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            sleep(sleep_one);
            console.info('logMessage contact_update_test_2400: updateCode = ' + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "999");
            await contactsQuery(map, "contact_update_test_2400", contactDataUri);
        } catch (error) {
            console.info('logMessage contact_update_test_2400: update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_favorite_test_2500
     * @tc.name    Favorite contacts
     * @tc.desc    Function test
     */
    it("contact_favorite_test_2500", 0, async function(done) {
        console.info("-------logMessage contact_favorite_test_2500 is starting!------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaocai", "company" : "TT", "position" : "Testers"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues)
            console.info('logMessage contact_favorite_test_2500: rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await updateFavorite(dataShareHelper, rawContactId);
            await deleteRawContact("contact_favorite_test_2500");
            done();
        } catch (error) {
            console.info('logMessage contact_favorite_test_2500: raw_contact insert error = ' + error);
            done();
        }
    });

    async function updateFavorite(dataShareHelper, rawContactId)
    {
        var updateValues = {"favorite" : 1};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(rawContactUri, updateValues, condition);
            sleep(sleep_one);
            console.info("logMessage contact_favorite_test_2500: updateCode = " + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", rawContactId.toString());
            map.set("display_name", "xiaocai");
            map.set("company", "TT");
            map.set("position", "Testers");
            map.set("favorite", "1");
            await contactsQuery(map, "contact_favorite_test_2500", rawContactUri);
        } catch (error) {
            console.info('logMessage contact_favorite_test_2500: update error = ' + error);
        }
    }

    /**
     * @tc.number  contact_unfavorite_test_2600
     * @tc.name    Unfavorite contacts
     * @tc.desc    Function test
     */
    it("contact_unfavorite_test_2600", 0, async function(done) {
        console.info("-------logMessage contact_unfavorite_test_2600 is starting!------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var rawContactValues = {"display_name" : "xiaohong", "company" : "TT", "position" : "Testers", "favorite" : 1};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info('logMessage contact_unfavorite_test_2600: rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await updateUnFavorite(dataShareHelper, rawContactId);
            await deleteRawContact("contact_unfavorite_test_2600");
            done();
        } catch (error) {
            console.info('logMessage contact_unfavorite_test_2600: raw_contact insert error = ' + error);
            done();
        }
    });

    async function updateUnFavorite(dataShareHelper, rawContactId)
    {
        var updateValues = {"favorite" : 0};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", rawContactId.toString());
        try {
            var updateCode = await dataShareHelper.update(rawContactUri, updateValues, condition);
            sleep(sleep_one);
            console.info("logMessage contact_unfavorite_test_2600: updateCode = " + updateCode);
            expect(updateCode == 0).assertTrue();
            var map = new Map();
            map.set("id", rawContactId.toString());
            map.set("display_name", "xiaohong");
            map.set("company", "TT");
            map.set("position", "Testers");
            map.set("favorite", "0");
            await contactsQuery(map, "contact_favorite_test_2500", rawContactUri);
        } catch (error) {
            console.info("logMessage contact_unfavorite_test_2600: update error = " + error);
        }
    }

    /**
     * @tc.number  contact_insert_test_2700
     * @tc.name    Insert contact blocklist
     * @tc.desc    Function test
     */
    it("contact_insert_test_2700", 0, async function(done) {
        console.info("-------logMessage contact_insert_test_2700 is starting!------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var blocklistId = await dataShareHelper.insert(contactBlocklistUri, common.getProfileBlockList());
            sleep(sleep_one);
            console.info('logMessage contact_insert_test_2700: blocklistId = ' + blocklistId);
            expect(blocklistId > 0).assertTrue();
            var map = common.getProfileBlockListMap();
            map.set("id", blocklistId.toString());
            await contactsQuery(map, "contact_insert_test_2700", contactBlocklistUri);
            await deleteAll(contactBlocklistUri, "contact_insert_test_2700");
            done();
        } catch (error) {
            console.info('logMessage contact_insert_test_2700: blocklist insert error = ' + error);
            done();
        }
    });

    /**
     * @tc.number  contact_delete_test_2800
     * @tc.name    Delete contact blocklist
     * @tc.desc    Function test
     */
    it("contact_delete_test_2800", 0, async function(done) {
        console.info("-------logMessage contact_delete_test_2800 is starting!------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var blocklistId = await dataShareHelper.insert(contactBlocklistUri, common.getProfileBlockList());
            sleep(sleep_one);
            console.info('logMessage contact_delete_test_2800: blocklistId = ' + blocklistId);
            expect(blocklistId > 0).assertTrue();
            await BlocklistDelete();
            done();
        } catch (error) {
            console.info('logMessage contact_delete_test_2800: blocklist insert error = ' + error);
            done();
        }

        async function BlocklistDelete()
        {
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", blocklistId.toString());
            try {
                var deleteCode = await dataShareHelper.delete(contactBlocklistUri, condition);
                sleep(sleep_one);
                console.info("logMessage contact_delete_test_2800: deleteCode = " + deleteCode);
                expect(deleteCode == 0).assertTrue();
                var map = new Map();
                map.set("id", blocklistId.toString());
                await queryIdForDelete(map, "contact_delete_test_2800", contactBlocklistUri);
            } catch (error) {
                console.info('logMessage contact_delete_test_2800: delete error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  contact_insert_test_2900
     * @tc.name    Insert contact group
     * @tc.desc    Function test
     */
    it("contact_insert_test_2900", 0, async function(done) {
        console.info("-------logMessage contact_insert_test_2900 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var groupValues = {"group_name" : "TestersThird"};
        try {
            var groupId = await dataShareHelper.insert(groupUri, groupValues);
            sleep(sleep_one);
            console.info("logMessage contact_insert_test_2900: groupId =" + groupId);
            expect(groupId > 0).assertTrue();
            await groupRawContactInsert(dataShareHelper);
            await deleteRawContact("contact_insert_test_2900");
            await deleteAll(groupUri, "contact_insert_test_2900");
            await deleteAll(contactDataUri, "contact_insert_test_2900");
            done();
        } catch (error) {
            console.info("logMessage contact_insert_test_2900: group insert error =" + error);
            done();
        }
    });

    async function groupRawContactInsert(dataShareHelper)
    {
        var rawContactValues = {"display_name" : "liyu"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_insert_test_2900: rawContactId =" + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await groupDataInsert(dataShareHelper, rawContactId);
        } catch (error) {
            console.info("logMessage contact_insert_test_2900: raw_contact insert error =" + error);
        }
    }

    async function groupDataInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "group_membership",
            "detail_info" : groupId.toString()
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues)
            sleep(sleep_one);
            console.info("logMessage contact_insert_test_2900: contactDataId =" + contactDataId);
            expect(contactDataId > 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId);
            map.set("raw_contact_id", rawContactId.toString());
            map.set("detail_info", groupId.toString());
            await contactsQuery(map, "contact_insert_test_2900", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_insert_test_2900: contact_data insert error =" + error);
        }
    }

    /**
     * @tc.number  contact_delete_test_3000
     * @tc.name    Delete contact group
     * @tc.desc    Function test
     */
    it("contact_delete_test_3000", 0, async function(done) {
        console.info("-------logMessage contact_delete_test_3000 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var groupValues = {"group_name" : "TestersFourth"};
        try {
            var groupId = await dataShareHelper.insert(groupUri, groupValues);
            sleep(sleep_one);
            console.info("logMessage contact_delete_test_3000: groupId =" + groupId);
            expect(groupId > 0).assertTrue();
            await groupAfterRawContactInsert(dataShareHelper);
            await deleteRawContact("contact_delete_test_3000");
            await deleteAll(groupUri, "contact_delete_test_3000");
            await deleteAll(contactDataUri, "contact_delete_test_3000");
            done();
        } catch (error) {
            console.info("logMessage contact_delete_test_3000: group insert error =" + error);
            done();
        }
    });

    async function groupAfterRawContactInsert(dataShareHelper)
    {
        var rawContactValues = {"display_name" : "liyuchen"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_delete_test_3000: rawContactId =" + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await deleteGroupContactDataInsert(dataShareHelper, rawContactId);
        } catch (error) {
            console.info("logMessage contact_delete_test_3000: raw_contact insert error =" + error);
        }
    }

    async function deleteGroupContactDataInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "group_membership",
            "detail_info" : groupId.toString()
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            console.info("logMessage contact_delete_test_3000: contactDataId =" + contactDataId);
            expect(contactDataId > 0).assertTrue();
            await deleteGroup(dataShareHelper, contactDataId);
        } catch (error) {
            console.info("logMessage contact_delete_test_3000: contact_data insert error =" + error);
        }
    }

    async function deleteGroup(dataShareHelper, contactDataId)
    {
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", contactDataId.toString());
        try {
            var deleteCode = await dataShareHelper.delete(contactDataUri, condition);
            sleep(sleep_one);
            console.info("logMessage contact_delete_test_3000: deleteCode =" + deleteCode);
            expect(deleteCode == 0).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            await queryIdForDelete(map, "contact_delete_test_3000", contactDataUri);
        } catch (error) {
            console.info("logMessage contact_delete_test_3000: delete error =" + error);
        }
    }

    /**
     * @tc.number  contact_query_test_3100
     * @tc.name    Query basic information of a single contact
     * @tc.desc    Function test
     */
    it("contact_query_test_3100", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_3100 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoyu"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3100: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await RawContactQuery(dataShareHelper, rawContactId);
            await deleteRawContact("contact_query_test_3100");
            done();
        } catch (error) {
            console.info("logMessage contact_query_test_3100: raw_contact insert error = " + error);
            done();
        }
    });

    async function RawContactQuery(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "id", "display_name" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
            sleep(sleep_two);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_query_test_3100: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == rawContactId).assertEqual(true);
                    console.info('logMessage contact_query_test_3100: display_name = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == "xiaoyu").assertEqual(true);
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info("logMessage contact_query_test_3100: query error = " + error);
        }
    }

    /**
     * @tc.number  contact_query_test_3200
     * @tc.name    Query basic information of multiple contacts
     * @tc.desc    Function test
     */
    it("contact_query_test_3200", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_3200 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues1 = {"display_name" : "xiaoyu"};
        try {
            var rawContactIdOne = await dataShareHelper.insert(rawContactUri, rawContactValues1);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3200: rawContactId = " + rawContactIdOne);
            expect(rawContactIdOne > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_query_test_3200: raw_contact_1 insert error = " + error);
            done();
        }

        var rawContactValues2 = {"display_name" : "xiaohong"};
        try {
            var rawContactIdTwo = await dataShareHelper.insert(rawContactUri, rawContactValues2);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3200: rawContactId = " + rawContactIdTwo);
            expect(rawContactIdTwo > 0).assertTrue();
            await queryTwoRaw(dataShareHelper, rawContactIdOne, rawContactIdTwo);
            await deleteRawContact("contact_query_test_3200");
            done();
        } catch (error) {
            console.info("logMessage contact_query_test_3200: raw_contact_2 insert error = " + error);
            done();
        }
    });

    async function queryTwoRaw(dataShareHelper, rawContactIdOne, rawContactIdTwo)
    {
        var resultColumns = [ "id", "display_name" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", rawContactIdOne.toString());
        condition.or();
        condition.equalTo("id", rawContactIdTwo.toString());
        try {
            var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
            sleep(sleep_one);
            console.info('logMessage contact_query_test_3200: goToFirstRow' + resultSet.goToFirstRow());
            expect(resultSet.goToFirstRow()).assertEqual(true);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info("logMessage contact_query_test_3200: columnNames :" + resultSet.columnNames);
                    console.info("logMessage contact_query_test_3200: columnCount:" + resultSet.columnCount);
                    console.info('logMessage contact_query_test_3200: id = ' + resultSet.getString(0));
                    console.info('logMessage contact_query_test_3200: display_name = ' + resultSet.getString(1));
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_query_test_3200: query error' + error);
        }
    }

    /**
     * @tc.number  contact_query_test_3300
     * @tc.name    Query basic information of all contacts
     * @tc.desc    Function test
     */
    it("contact_query_test_3300", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_3300 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues1 = {"display_name" : "xiaoyu"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues1);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3300: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_query_test_3300: raw_contact_1 insert error = " + error);
            done();
        }
        var resultColumns = [ "id", "display_name" ];
        let condition = new dataShare.DataSharePredicates();
        condition.greaterThan("id", "0");
        try {
            var resultSet = await dataShareHelper.query(profileRawContactUri, resultColumns, condition);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_query_test_3300: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == rawContactId.toString()).assertEqual(true);
                    console.info('logMessage contact_query_test_3300: display_name = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == "xiaoyu").assertEqual(true);
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
            await deleteRawContact("contact_query_test_3300");
            done();
        } catch (error) {
            console.info("logMessage contact_query_test_3300: query error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_query_test_3400
     * @tc.name    Query all information of a single contact
     * @tc.desc    Function test
     */
    it("contact_query_test_3400", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_3400 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {
            "display_name" : "xiaoyuzhou",
            "company" : "TT",
            "position" : "Testers",
            "favorite" : 1,
            "phonetic_name" : "xiaoyu||xy"
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3400: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await queryAllInformation(dataShareHelper, rawContactId);
            await deleteRawContact("contact_query_test_3400");
            done();
        } catch (error) {
            console.info("logMessage contact_query_test_3400: raw_contact insert error = " + error);
            done();
        }
    });

    async function queryAllInformation(dataShareHelper, rawContactId)
    {
        var resultColumns = [];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                do {
                    var id = resultSet.getString(resultSet.getColumnIndex("id"));
                    console.info('logMessage contact_query_test_3400: id = ' + id);
                    expect(id == rawContactId).assertEqual(true);
                    var favorite = resultSet.getString(resultSet.getColumnIndex("favorite"));
                    console.info('logMessage contact_query_test_3400: favorite = ' + favorite);
                    expect(favorite == "1").assertEqual(true);
                    var phonetic_name = resultSet.getString(resultSet.getColumnIndex("phonetic_name"));
                    console.info('logMessage contact_query_test_3400: phonetic_name = ' + phonetic_name);
                    expect(phonetic_name == "xiaoyu||xy").assertEqual(true);
                    var company = resultSet.getString(resultSet.getColumnIndex("company"));
                    console.info('logMessage contact_query_test_3400: company = ' + company);
                    expect(company == "TT").assertEqual(true);
                    var display_name = resultSet.getString(resultSet.getColumnIndex("display_name"));
                    console.info('logMessage contact_query_test_3400: display_name = ' + display_name);
                    expect(display_name == "xiaoyuzhou").assertEqual(true);
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_query_test_3400: query error = ' + error);
        }
    }

    /**
     * @tc.number  contact_query_test_3500
     * @tc.name    Query group member ability
     * @tc.desc    Function test
     */
    it("contact_query_test_3500", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_3500 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var rawContactValues = {"display_name" : "xiaoyuzhou"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3500: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_query_test_3500: raw_contact insert error = " + error);
            done();
        }
        var groupValues = {"group_name" : "BossBoard3500"};
        try {
            var groupId = await dataShareHelper.insert(groupUri, groupValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3500: groupId = " + groupId);
            expect(groupId > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_query_test_3500: group insert error = " + error);
            done();
        }
        await groupBossBoardInsert(dataShareHelper, rawContactId, groupId);
        await deleteRawContact("contact_query_test_3500");
        await deleteAll(groupUri, "contact_query_test_3500");
        await deleteAll(contactDataUri, "contact_query_test_3500");
        done();
    });

    async function groupBossBoardInsert(dataShareHelper, rawContactId, groupId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "group_membership",
            "detail_info" : groupId.toString()
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3500: contactDataId = " + contactDataId);
            expect(contactDataId > 0).assertTrue();
            await groupBossBoardQuery(dataShareHelper, rawContactId, groupId);
        } catch (error) {
            console.info("logMessage contact_query_test_3500: query error = " + error);
        }
    }

    async function groupBossBoardQuery(dataShareHelper, rawContactId, groupId)
    {
        var resultColumns = [ "raw_contact_id", "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        // type_id = 9 data is group
        condition.equalTo("type_id", "9");
        condition.and();
        condition.equalTo("detail_info", groupId.toString());
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_query_test_3500: raw_contact_id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == rawContactId).assertEqual(true);
                    console.info('logMessage contact_query_test_3500: detail_info = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == groupId.toString()).assertEqual(true);
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info("logMessage contact_query_test_3500: query error :" + error);
        }
    }

    /**
     * @tc.number  contact_query_test_3600
     * @tc.name    Query favorite contacts
     * @tc.desc    Function test
     */
    it("contact_query_test_3600", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_3600 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var rawContactValues = {"display_name" : "xiaoyuzhou", "favorite" : 1};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3600: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await queryFavorite();
            await deleteRawContact("contact_query_test_3600");
            done();
        } catch (error) {
            console.info("logMessage contact_query_test_3600: raw_contact insert error = " + error);
            done();
        }

        async function queryFavorite()
        {
            var resultColumns = [ "id", "display_name" ];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("favorite", "1");
            condition.and();
            condition.equalTo("is_deleted", "0");
            try {
                var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
                sleep(sleep_one);
                if (resultSet.goToFirstRow()) {
                    do {
                        console.info('logMessage contact_query_test_3600: id = ' + resultSet.getString(0));
                        expect(resultSet.getString(0) == rawContactId.toString()).assertTrue();
                        console.info('logMessage contact_query_test_3600: display_name = ' + resultSet.getString(1));
                        expect(resultSet.getString(1) == "xiaoyuzhou").assertTrue();
                    } while (resultSet.goToNextRow());
                }
                resultSet.close();
            } catch (error) {
                console.info("logMessage contact_query_test_3600: query error :" + error);
                done();
            }
        }
    });

    /**
     * @tc.number  contact_query_test_3700
     * @tc.name    Query recent contact ability
     * @tc.desc    Function test
     */
    it("contact_query_test_3700", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_3700 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var rawContactValues = {"display_name" : "xiaoyuzhou", "lastest_contacted_time" : 60};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3700: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await queryContacted();
            await deleteRawContact("contact_query_test_3700");
            done();
        } catch (error) {
            console.info("logMessage contact_query_test_3700: raw_contact insert error = " + error);
            done();
        }

        async function queryContacted()
        {
            var resultColumns = [ "id", "display_name" ];
            let condition = new dataShare.DataSharePredicates();
            condition.lessThan("lastest_contacted_time", "100");
            condition.and();
            condition.equalTo("is_deleted", "0");
            try {
                var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
                sleep(sleep_one);
                if (resultSet.goToFirstRow()) {
                    do {
                        console.info('logMessage contact_query_test_3700: id = ' + resultSet.getString(0));
                        expect(resultSet.getString(0) == rawContactId.toString()).assertTrue();
                        console.info('logMessage contact_query_test_3700: display_name = ' + resultSet.getString(1));
                        expect(resultSet.getString(1) == "xiaoyuzhou").assertTrue();
                    } while (resultSet.goToNextRow());
                }
                resultSet.close();
            } catch (error) {
                console.info('logMessage contact_query_test_3700: query error' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  contact_query_test_3800
     * @tc.name    Query recently deleted contacts
     * @tc.desc    Function test
     */
    it("contact_query_test_3800", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_3800 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var rawContactValues = {"display_name" : "xiaocai"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3800: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await rawContactDelete(dataShareHelper, rawContactId);
            await deletedRawContactQuery(dataShareHelper, rawContactId);
            await deleteAll(deletedUri, "contact_query_test_3800")
            done();
        } catch (error) {
            console.info("logMessage contact_query_test_3800: raw_contact insert error = " + error);
            done();
        }
    });

    async function rawContactDelete(dataShareHelper, rawContactId)
    {
        var condition1 = new dataShare.DataSharePredicates();
        condition1.equalTo("id", rawContactId.toString());
        try {
            var deleteCode = await dataShareHelper.delete(rawContactUri, condition1);
            sleep(sleep_one);
            console.info('logMessage contact_query_test_3800: deleteCode = ' + deleteCode);
            expect(deleteCode == 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_query_test_3800: raw_contact delete error = " + error);
            done();
        }
    }

    async function deletedRawContactQuery(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "id", "raw_contact_id", "display_name", "is_deleted" ];
        var condition2 = new dataShare.DataSharePredicates();
        condition2.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(deletedRawContactUri, resultColumns, condition2);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                console.info('logMessage contact_query_test_3800: id = ' + resultSet.getString(0));
                console.info('logMessage contact_query_test_3800: raw_contact_id = ' + resultSet.getString(1));
                expect(resultSet.getString(1) == rawContactId.toString()).assertTrue();
                console.info('logMessage contact_query_test_3800: display_name = ' + resultSet.getString(array_two));
                expect(resultSet.getString(array_two) == "xiaocai").assertTrue();
                console.info('logMessage contact_query_test_3800: is_deleted = ' + resultSet.getString(array_three));
                expect(resultSet.getString(array_three) == "1").assertTrue();
            }
            resultSet.close();
        } catch (error) {
            console.info("logMessage contact_query_test_3800: query error :" + error);
            done();
        }
    }

    /**
     * @tc.number  contact_query_test_3900
     * @tc.name    Query all mobile phone numbers
     * @tc.desc    Function test
     */
    it("contact_query_test_3900", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_3900 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var rawContactValues = {"display_name" : "xiaocai"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3900: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await queryPhoneInsert();
            await phoneQuery(rawContactId);
            await deleteRawContact("contact_query_test_3900");
            await deleteAll(contactDataUri, "contact_query_test_3900");
            done();
        } catch (error) {
            console.info("logMessage contact_query_test_3900: raw_contact insert error = " + error);
            done();
        }
    });

    async function queryPhoneInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "phone",
            "detail_info" : "1853696321"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_3900: contactDataId = " + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_query_test_3900: contact_data insert error = " + error);
        }
    }

    async function phoneQuery(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "raw_contact_id", "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        // type_id = 5 data is phone
        condition.equalTo("content_type", "phone");
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_query_test_3900: raw_contact_id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == rawContactId.toString()).assertTrue();
                    console.info('logMessage contact_query_test_3900: detail_info = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == "1853696321").assertTrue();
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_query_test_3900: query error' + error);
        }
    }

    /**
     * @tc.number  contact_query_test_4000
     * @tc.name    Query all mailbox capabilities
     * @tc.desc    Function test
     */
    it("contact_query_test_4000", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_4000 is starting!------------");

        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoyu"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_4000: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await queryEmailInsert(dataShareHelper, rawContactId);
            await emailQuery(dataShareHelper, contactDataId, rawContactId);
            await deleteRawContact("contact_query_test_4000");
            await deleteAll(contactDataUri, "contact_query_test_4000");
            done();
        } catch (error) {
            console.info("logMessage contact_query_test_4000: raw_contact insert error = " + error);
            done();
        }
    });

    async function queryEmailInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "email",
            "detail_info" : "16658@163.com"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_4000: contactDataId = " + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_query_test_4000: contact_data insert error = " + error);
        }
    }

    async function emailQuery(dataShareHelper, contactDataId, rawContactId)
    {
        var resultColumns = [ "id", "raw_contact_id", "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        // type_id = 1 data content_type is  email
        condition.equalTo("type_id", "1").limitAs(array_three).orderByDesc("id");
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_query_test_4000: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == contactDataId.toString()).assertTrue();
                    console.info('logMessage contact_query_test_4000: raw_contact_id = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == rawContactId.toString()).assertTrue();
                    console.info('logMessage contact_query_test_4000: detail_info = ' + resultSet.getString(array_two));
                    expect(resultSet.getString(array_two) == "16658@163.com").assertTrue();
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info("logMessage contact_query_test_4000: query error :" + error);
        }
    }

    /**
     * @tc.number  contact_query_test_4100
     * @tc.name    Query data information of a single contact
     * @tc.desc    Function test
     */
    it("contact_query_test_4100", 0, async function(done) {
        console.info("-----------logMessage contact_query_test_4100 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoyu"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_4100: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await queryTwoDataInsert(dataShareHelper, rawContactId);
            await queryTwoDataInsertCompany(dataShareHelper, rawContactId);
            await queryTwo(dataShareHelper, rawContactId);
            await deleteRawContact("contact_query_test_4100");
            await deleteAll(contactDataUri, "contact_query_test_4100");
            del
            done();
        } catch (error) {
            console.info("logMessage contact_query_test_4100: raw_contact insert error = " + error);
            done();
        }
    });

    async function queryTwoDataInsert(dataShareHelper, rawContactId)
    {
        var contactDataValues = {"raw_contact_id" : rawContactId, "content_type" : "name", "detail_info" : "xiaoyu"};
        try {
            var contactDataId1 = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info("logMessage contact_query_test_4100: contactDataId = " + contactDataId1);
            expect(contactDataId1 > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_query_test_4100: contact_data name insert error = " + error);
        }
    }

    async function queryTwoDataInsertCompany(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "organization",
            "detail_info" : "OOOO",
            "position" : "Testers"
        };
        try {
            var contactDataId2 = await dataShareHelper.insert(contactDataUri, contactDataValues);
            console.info("logMessage contact_query_test_4100: contactDataId = " + contactDataId2);
            expect(contactDataId2 > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_query_test_4100: contact_data company insert error = " + error);
        }
    }

    async function queryTwo(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "id", "raw_contact_id", "detail_info", "position" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            sleep(sleep_one);
            var array = [ {detail_info : "xiaoyu", position : ""}, {detail_info : "OOOO", position : "Testers"} ];
            var i = 0;
            if (resultSet.goToFirstRow()) {
                do {
                    console.info("logMessage contact_query_test_4100: columnNames :" + resultSet.columnNames);
                    console.info("logMessage contact_query_test_4100: columnCount:" + resultSet.columnCount);
                    console.info('logMessage contact_query_test_4100: id = ' + resultSet.getString(0));
                    console.info('logMessage contact_query_test_4100: raw_contact_id = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == rawContactId.toString()).assertEqual(true);
                    console.info('logMessage contact_query_test_4100: detail_info = ' + resultSet.getString(array_two));
                    expect(resultSet.getString(array_two) == array[i].detail_info).assertEqual(true);
                    console.info('logMessage contact_query_test_4100: position = ' + resultSet.getString(array_three));
                    expect(resultSet.getString(array_three) == array[i].position).assertEqual(true);
                    i++;
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info("logMessage contact_query_test_4100: query error :" + error);
        }
    }

    /**
     * @tc.number  contact_delete_test_4400
     * @tc.name    Delete the basic raw data of the contact and record the deleted basic raw data
     * @tc.desc    Function test
     */
    it("contact_delete_test_4400", 0, async function(done) {
        console.info("-------logMessage contact_delete_test_4400 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaoming"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_delete_test_4400: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await deleteRaw(dataShareHelper, rawContactId);
            await queryDelete(dataShareHelper, rawContactId);
            done();
        } catch (error) {
            console.info("logMessage contact_delete_test_4400: raw_contact insert error :" + error);
            done();
        }
    });

    async function deleteRaw(dataShareHelper, rawContactId)
    {
        var condition1 = new dataShare.DataSharePredicates();
        condition1.equalTo("id", rawContactId.toString());
        try {
            var deleteCode = await dataShareHelper.delete(rawContactUri, condition1);
            sleep(sleep_one);
            console.info("logMessage contact_delete_test_4400: deleteCode = " + deleteCode);
            expect(deleteCode == 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_delete_test_4400: delete error = " + error);
        }
    }

    async function queryDelete(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "id", "delete_time", "display_name" ];
        var condition2 = new dataShare.DataSharePredicates();
        condition2.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(deletedRawContactUri, resultColumns, condition2);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                console.info("logMessage contact_delete_test_4400: columnNames:" + resultSet.columnNames);
                console.info("logMessage contact_delete_test_4400: columnCount:" + resultSet.columnCount);
                console.info('logMessage contact_delete_test_4400: id = ' + resultSet.getString(0));
                console.info('logMessage contact_delete_test_4400: delete_time = ' + resultSet.getString(1));
                console.info('logMessage contact_delete_test_4400: display_name = ' + resultSet.getString(array_two));
                expect(resultSet.getString(array_two) == "xiaoming").assertTrue();
            }
            resultSet.close();
        } catch (error) {
            console.info("logMessage contact_delete_test_4400: query error:" + error);
        }
    }

    /**
     * @tc.number  contact_delete_test_4700
     * @tc.name    Delete detailed contact data and record detailed data
     * @tc.desc    Function test
     */
    it("contact_delete_test_4700", 0, async function(done) {
        console.info("-------logMessage contact_delete_test_4700 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaozhang", "company" : "OOOO", "position" : "TestManager"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_delete_test_4700: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await queryDeleteCompanyData(dataShareHelper, rawContactId);
            await rawContactDelete(dataShareHelper, rawContactId);
            await companyRawContactDeleteQuery(dataShareHelper, rawContactId);
            await deleteAll(contactDataUri, "contact_delete_test_4700");
            done();
        } catch (error) {
            console.info("logMessage contact_delete_test_4700: raw_contact insert error:" + error);
            done();
        }
    });

    async function queryDeleteCompanyData(dataShareHelper, rawContactId)
    {
        var contactDataValues = {
            "raw_contact_id" : rawContactId,
            "content_type" : "organization",
            "detail_info" : "OOOO",
            "position" : "TestManager"
        };
        try {
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            sleep(sleep_one);
            console.info("logMessage contact_delete_test_4700: contactDataId = " + contactDataId);
            expect(contactDataId > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_delete_test_4700: contact_data insert error:" + error);
        }
    }

    async function rawContactDelete(dataShareHelper, rawContactId)
    {
        var condition1 = new dataShare.DataSharePredicates();
        condition1.equalTo("id", rawContactId.toString());
        try {
            var deleteCode = await dataShareHelper.delete(rawContactUri, condition1);
            sleep(sleep_one);
            console.info("logMessage contact_delete_test_4700: deleteCode = " + deleteCode);
            expect(deleteCode == 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_delete_test_4700: delete error = " + error);
            done();
        }
    }

    async function companyRawContactDeleteQuery(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "id", "display_name", "backup_data" ];
        var condition2 = new dataShare.DataSharePredicates();
        condition2.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(deletedRawContactUri, resultColumns, condition2);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                console.info("logMessage contact_delete_test_4700: columnNames:" + resultSet.columnNames);
                console.info("logMessage contact_delete_test_4700: columnCount:" + resultSet.columnCount);
                console.info('logMessage contact_delete_test_4700: id = ' + resultSet.getString(0));
                console.info('logMessage contact_delete_test_4700: display_name = ' + resultSet.getString(1));
                expect(resultSet.getString(1) == "xiaozhang").assertEqual(true);
                console.info('logMessage contact_delete_test_4700: backup_data = ' + resultSet.getString(array_two));
            }
            resultSet.close();
        } catch (error) {
            console.info("logMessage contact_delete_test_4700: query error:" + error);
        }
    }

    /**
     * @tc.number  contact_delete_test_4800
     * @tc.name    Delete and record basic data of a single contact
     * @tc.desc    Function test
     */
    it("contact_delete_test_4800", 0, async function(done) {
        console.info("-------logMessage contact_delete_test_4800 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {"display_name" : "xiaopeng"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_delete_test_4800: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDelete(dataShareHelper, rawContactId);
            await DeletedRawContactQuery(dataShareHelper, rawContactId)
                await deleteAll(contactDataUri, "contact_delete_test_4800");
            done();
        } catch (error) {
            console.info("logMessage contact_delete_test_4800: raw_contact insert error:" + error);
            done();
        }
    });

    async function ContactDelete(dataShareHelper, rawContactId)
    {
        var condition1 = new dataShare.DataSharePredicates();
        condition1.equalTo("name_raw_contact_id", rawContactId.toString());
        try {
            var deleteCode = await dataShareHelper.delete(contactUri, condition1);
            sleep(sleep_one);
            console.info("logMessage contact_delete_test_4800: deleteCode = " + deleteCode);
            expect(deleteCode == 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_delete_test_4800: delete error = " + error);
        }
    }

    async function DeletedRawContactQuery(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "id", "delete_time", "display_name" ];
        var condition2 = new dataShare.DataSharePredicates();
        condition2.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(deletedRawContactUri, resultColumns, condition2);
            sleep(sleep_one)
            if (resultSet.goToFirstRow())
            {
                console.info("logMessage contact_delete_test_4800: columnNames:" + resultSet.columnNames);
                console.info("logMessage contact_delete_test_4800: columnCount:" + resultSet.columnCount);
                console.info('logMessage contact_delete_test_4800: id = ' + resultSet.getString(0));
                console.info('logMessage contact_delete_test_4800: delete_time = ' + resultSet.getString(1));
                console.info('logMessage contact_delete_test_4800: display_name = ' + resultSet.getString(array_two));
                expect(resultSet.getString(array_two) == "xiaopeng").assertTrue();
            }
            resultSet.close();
        } catch (error) {
            console.info("logMessage contact_delete_test_4800: query error:" + error);
        }
    }

    /**
     * @tc.number  contact_batchinsert_test_4900
     * @tc.name    Insert contacts in batches(raw_contact)
     * @tc.desc    Function test
     */
    it("contact_batchinsert_test_4900", 0, async function(done) {
        console.info("--------logMessage contact_batchinsert_test_4900 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, common.getContactBatchCompany());
            sleep(sleep_one);
            console.info("logMessage contact_batchinsert_test_4900: batchInsertCode = " + batchInsertCode);
            expect(batchInsertCode == 0).assertTrue();
            var resultColumns = [];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("company", "TT4900");
            var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
            console.info(
                'contact_batchinsert_test_4900 : queryBatch start ! resultSet.rowCount = ' + resultSet.rowCount);
            expect(resultSet.rowCount == 5).assertEqual(true);
            resultSet.close();
            await deleteRawContact("contact_batchinsert_test_4900");
            done();
        } catch (error) {
            console.info("logMessage contact_batchinsert_test_4900: batchInsert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_batchinsert_test_5000
     * @tc.name    Insert contact_data in batches of address book
     * @tc.desc    Function test
     */
    it("contact_batchinsert_test_5000", 0, async function(done) {
        console.info("--------logMessage contact_batchinsert_test_5000  start!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var rawContactValues = {"display_name" : "xiaozhi"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info("logMessage contact_batchinsert_test_5000: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataBatchInsert();
            done();
        } catch (error) {
            console.info("logMessage contact_batchinsert_test_5000: raw_contact insert error = " + error);
            done();
        }
        async function ContactDataBatchInsert()
        {
            try {
                var batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, common.getContactBatchCompanyTwo());
                sleep(sleep_one);
                console.info("logMessage contact_batchinsert_test_5000: batchInsertCode = " + batchInsertCode);
                expect(batchInsertCode == 0).assertTrue();
                var map = new Map();
                map.set("raw_contact_id", raw_contact_id.toString());
                await queryBatch(map, "contact_batchinsert_test_5000", 5);
                await deleteRawContact("contact_batchinsert_test_5000");
                done();
            } catch (error) {
                console.info("logMessage contact_batchinsert_test_5000: batchInsert error = " + error);
                done();
            }
        }
    });

    /**
     * @tc.number  contact_batchinsert_test_5400
     * @tc.name    Batch operation of contact blocklist
     * @tc.desc    Function test
     */
    it("contact_batchinsert_test_5400", 0, async function(done) {
        console.info("--------logMessage contact_batchinsert_test_5400 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var phoneNumber = randomNum(13);
        try {
            var batchInsertCode = await dataShareHelper.batchInsert(contactBlocklistUri, common.getPhoneNumberBatch());
            sleep(sleep_one);
            console.info("logMessage contact_batchinsert_test_5400: batchInsertCode = " + batchInsertCode);
            expect(batchInsertCode == 0).assertTrue();
            await query(5);
            await BlocklistDelete();
            done();
        } catch (error) {
            console.info("logMessage contact_batchinsert_test_5400: batchInsert error = " + error);
            done();
        }

        async function BlocklistDelete()
        {
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("phone_number", phoneNumber);
            try {
                var deleteCode = await dataShareHelper.delete(contactBlocklistUri, condition);
                console.info("logMessage contact_batchinsert_test_5400: deleteCode = " + deleteCode);
                expect(deleteCode == 0).assertTrue();
                await query(0);
                done();
            } catch (error) {
                console.info("logMessage contact_batchinsert_test_5400: delete error = " + error);
                done();
            }
        }

        async function query(size)
        {
            var resultColumns = [];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("phone_number", phoneNumber);
            try {
                var resultSet = await dataShareHelper.query(contactBlocklistUri, resultColumns, condition);
                sleep(sleep_one);
                console.info(
                    'contact_batchinsert_test_5400 : queryBatch start ! resultSet.rowCount = ' + resultSet.rowCount);
                expect(resultSet.rowCount == size).assertEqual(true);
                resultSet.close();
            } catch (error) {
                console.info(" contact_batchinsert_test_5400 logMessage resultSet.rowCount: error = " + error);
            }
        }
    });

    /**
     * @tc.number  contact_update_test_5300
     * @tc.name    Bulk favorites and unfavorites of contacts
     * @tc.desc    Function test
     */
    it("contact_update_test_5300", 0, async function(done) {
        console.info("--------logMessage contact_update_test_5300 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValuesOne = {"display_name" : "fengyuan", "favorite" : 1};
        var rawContactValuesTwo = {"display_name" : "xiaoli", "favorite" : 1};
        var rawContactValuesThree = {"display_name" : "xiaoma", "favorite" : 1};
        var listAddBluk = []
        listAddBluk[0] = rawContactValuesOne;
        listAddBluk[1] = rawContactValuesTwo;
        listAddBluk[array_two] = rawContactValuesThree;
        try {
            var batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
            sleep(sleep_one);
            console.info("logMessage contact_update_test_5300: batchInsertCode = " + batchInsertCode);
            expect(batchInsertCode == 0).assertTrue();
            await RawContactUpdateUnFavorites(dataShareHelper);
            await queryUnFavorites(dataShareHelper);
            await deleteRawContact("contact_update_test_5300");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_5300: batchInsert error = " + error);
            done();
        }
    });

    async function RawContactUpdateUnFavorites(dataShareHelper)
    {
        var updateValues = {"favorite" : 0};
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("display_name", "fengyuan");
        condition.or();
        condition.equalTo("display_name", "xiaoma");
        try {
            var updateCode = await dataShareHelper.update(rawContactUri, updateValues, condition);
            sleep(sleep_one);
            console.info("logMessage contact_update_test_5300: updateCode = " + updateCode);
            expect(updateCode == 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_update_test_5300: update error = " + error);
        }
    }

    async function queryUnFavorites(dataShareHelper)
    {
        var resultColumns = [];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("display_name", "fengyuan");
        condition.or();
        condition.equalTo("display_name", "xiaoma");
        try {
            var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
            sleep(sleep_one);
            console.info('contact_update_test_5300 : queryBatch start ! resultSet.rowCount = ' + resultSet.rowCount);
            expect(resultSet.rowCount == array_two).assertEqual(true);
            if (resultSet.rowCount > 0) {
                if (resultSet.goToFirstRow()) {
                    do {
                        var dbresult = resultSet.getString(resultSet.getColumnIndex("favorite"));
                        console.info(' contact_update_test_5300 :logMessage contactsQuery dbresult :' + dbresult +
                                     ' value : ' + value);
                        expect(dbresult == "0").assertEqual(true);
                    } while (resultSet.goToNextRow());
                }
            }
            resultSet.close();
        } catch (error) {
            console.info(" contact_update_test_5300 logMessage resultSet.rowCount: error = " + error);
        }
    }

    /**
     * @tc.number  contact_batchinsert_test_5500
     * @tc.name    Contacts are added to and removed from groups in batches
     * @tc.desc    Function test
     */
    it("contact_batchinsert_test_5500", 0, async function(done) {
        console.info("--------logMessage contact_batchinsert_test_5500 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var groupValues = {"group_name" : "ManagerGroup"};
        try {
            var groupId = await dataShareHelper.insert(groupUri, groupValues);
            console.info("logMessage contact_batchinsert_test_5500: groupId = " + groupId);
            expect(groupId > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_batchinsert_test_5500: group insert error = " + error);
            done();
        }

        try {
            var rawContactValues1 = {"display_name" : "xiaohuang"};
            var rawContactIdOne = await dataShareHelper.insert(rawContactUri, rawContactValues1);
            console.info("logMessage contact_batchinsert_test_5500: rawContactIdOne = " + rawContactIdOne);
            expect(rawContactIdOne > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_batchinsert_test_5500: raw_contact_1 insert error = " + error);
            done();
        }

        try {
            var rawContactValues2 = {"display_name" : "xiaolv"};
            var rawContactIdTwo = await dataShareHelper.insert(rawContactUri, rawContactValues2);
            console.info("logMessage contact_batchinsert_test_5500: rawContactIdTwo = " + rawContactIdTwo);
            expect(rawContactIdTwo > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_batchinsert_test_5500: raw_contact_2 insert error = " + error);
            done();
        }

        try {
            var rawContactValues3 = {"display_name" : "xiaobai"};
            var rawContactId3 = await dataShareHelper.insert(rawContactUri, rawContactValues3);
            console.info("logMessage contact_batchinsert_test_5500: rawContactId3 = " + rawContactId3);
            expect(rawContactId3 > 0).assertTrue();
        } catch (error) {
            console.info("logMessage contact_batchinsert_test_5500: raw_contact_3 insert error = " + error);
            done();
        }
        await threeDataInsert(dataShareHelper, rawContactIdOne, rawContactIdTwo, rawContactId3, groupId);
        await deleteRawContact("contact_batchinsert_test_5500");
        await deleteAll(groupUri, "contact_batchinsert_test_5500");
        await deleteAll(contactDataUri, "contact_batchinsert_test_5500");
        done();
    });

    async function threeDataInsert(dataShareHelper, rawContactIdOne, rawContactIdTwo, rawContactId3, groupId)
    {
        var contactDataValues1 = {
            "raw_contact_id" : rawContactIdOne,
            "content_type" : "group_membership",
            "detail_info" : groupId.toString()
        };
        var contactDataValues2 = {
            "raw_contact_id" : rawContactIdTwo,
            "content_type" : "group_membership",
            "detail_info" : groupId.toString()
        };
        var contactDataValues3 = {
            "raw_contact_id" : rawContactId3,
            "content_type" : "group_membership",
            "detail_info" : groupId.toString()
        };
        var listAddBluk = [];
        listAddBluk[0] = contactDataValues1;
        listAddBluk[1] = contactDataValues2;
        listAddBluk[array_two] = contactDataValues3;
        try {
            var batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
            sleep(sleep_one);
            console.info("logMessage contact_batchinsert_test_5500: batchInsertCode = " + batchInsertCode);
            expect(batchInsertCode == 0).assertTrue();
            await ContactDataDelete(dataShareHelper, rawContactIdOne, rawContactId3);
        } catch (error) {
            console.info("logMessage contact_batchinsert_test_5500: batchInsert error = " + error);
        }
    }

    async function ContactDataDelete(dataShareHelper, rawContactIdOne, rawContactId3)
    {
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactIdOne.toString());
        condition.or();
        condition.equalTo("raw_contact_id", rawContactId3.toString());
        try {
            var deleteCode = await dataShareHelper.delete(contactDataUri, condition);
            sleep(sleep_one);
            console.info("logMessage contact_batchinsert_test_5500: deleteCode = " + deleteCode);
            expect(deleteCode == 0).assertTrue();
            await queryTwoData(dataShareHelper, rawContactIdOne, rawContactId3);
        } catch (error) {
            console.info("logMessage contact_batchinsert_test_5500: delete error = " + error);
        }
    }

    async function queryTwoData(dataShareHelper, rawContactIdOne, rawContactId3)
    {
        var resultColumns = [];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactIdOne.toString());
        condition.or();
        condition.equalTo("raw_contact_id", rawContactId3.toString());
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            sleep(sleep_one);
            console.info(
                'contact_batchinsert_test_5500 : queryBatch start ! resultSet.rowCount = ' + resultSet.rowCount);
            expect(resultSet.rowCount == 0).assertEqual(true);
            resultSet.close();
        } catch (error) {
            console.info(" contact_batchinsert_test_5500 logMessage resultSet.rowCount: error = " + error);
        }
    }

    /**
     * @tc.number  contact_pinyin_query_test_200
     * @tc.name    Contact name to pinyin (Chinese Simplified)
     * @tc.desc    Function test
     */
    it("contact_pinyin_query_test_200", 0, async function(done) {
        console.info("--------logMessage contact_pinyin_query_test_200 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "小黄"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info('logMessage contact_pinyin_query_test_200 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataInsert();
            await deleteRawContact("contact_pinyin_query_test_200");
            await deleteAll(contactDataUri, "contact_pinyin_query_test_200");
            done();
        } catch (error) {
            console.info("logMessage contact_pinyin_query_test_200: raw_contact insert error = " + error);
            done();
        }

        async function ContactDataInsert()
        {
            var contactDataValues = {
                "raw_contact_id" : rawContactId,
                "content_type" : "name",
                "detail_info" : "xiaohuang"
            };
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                console.info('logMessage contact_pinyin_query_test_200 contactDataId = ' + contactDataId);
                expect(contactDataId > 0).assertTrue();
                await SearchQuery(dataShareHelper, rawContactId);
            } catch (error) {
                console.info('logMessage contact_pinyin_query_test_200 contact_data insert error = ' + error);
                done();
            }
        }
    });

    async function SearchQuery(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "id", "search_name" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(searchContactUri, resultColumns, condition);
            var map = common.getSearchMap();
            map.set("raw_contact_id", rawContactId.toString());
            if (resultSet.goToFirstRow()) {
                do {
                    for (var [key, value] of map) {
                        let dbresult = resultSet.getString(resultSet.getColumnIndex(key));
                        console.info(tag + ':contact_pinyin_query_test_200 key = ' + key + ' dbresult :' + dbresult +
                                     ' value : ' + value);
                        expect(value == dbresult).assertEqual(true);
                    }
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_pinyin_query_test_200 query error = ' + error);
        }
    }

    /**
     * @tc.number  contact_pinyin_query_test_100
     * @tc.name    Contact name to pinyin (Chinese Traditional)
     * @tc.desc    Function test
     */
    it("contact_pinyin_query_test_100", 0, async function(done) {
        console.info("--------logMessage contact_pinyin_query_test_100 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "鍵盤"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage contact_pinyin_query_test_100 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataInsert();
            await deleteRawContact("contact_pinyin_query_test_100");
            await deleteAll(contactDataUri, "contact_pinyin_query_test_100");
            done();
        } catch (error) {
            console.info('logMessage contact_pinyin_query_test_100 raw_contact insert error = ' + error);
            done();
        }

        async function ContactDataInsert()
        {
            var contactDataValues = {"raw_contact_id" : rawContactId, "content_type" : "name", "detail_info" : "鍵盤"};
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                sleep(sleep_one);
                console.info('logMessage contact_pinyin_query_test_100 contactDataId = ' + contactDataId);
                expect(contactDataId > 0).assertTrue();
                await SearchContactQueryChinese(dataShareHelper, rawContactId);
            } catch (error) {
                console.info('logMessage contact_pinyin_query_test_100 contact_data insert error = ' + error);
                done();
            }
        }
    });

    async function SearchContactQueryChinese(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "raw_contact_id", "search_name" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(searchContactUri, resultColumns, condition);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_pinyin_query_test_100: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == rawContactId.toString()).assertEqual(true);
                    console.info('logMessage contact_pinyin_query_test_100: search_name = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == "鍵盤||jianpan||jp").assertEqual(true);
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_pinyin_query_test_100 query error = ' + error);
        }
    }

    /**
     * @tc.number  contact_pinyin_query_test_300
     * @tc.name    Contact name to pinyin (English)
     * @tc.desc    Function test
     */
    it("contact_pinyin_query_test_300", 0, async function(done) {
        console.info("--------logMessage contact_pinyin_query_test_300 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "Tom"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage contact_pinyin_query_test_300 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataInsert();
            await deleteRawContact("contact_pinyin_query_test_300");
            done();
        } catch (error) {
            console.info('logMessage contact_pinyin_query_test_300 raw_contact insert error = ' + error);
            done();
        }

        async function ContactDataInsert()
        {
            var contactDataValues = {"raw_contact_id" : rawContactId, "content_type" : "name", "detail_info" : "Tom"};
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                console.info('logMessage contact_pinyin_query_test_300 contactDataId = ' + contactDataId);
                expect(contactDataId > 0).assertTrue();
                await SearchContactQuery(dataShareHelper, rawContactId);
            } catch (error) {
                console.info('logMessage contact_pinyin_query_test_300 contact_data insert error = ' + error);
                done();
            }
        }
    });

    async function SearchContactQuery(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "raw_contact_id", "search_name" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(searchContactUri, resultColumns, condition);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_pinyin_query_test_300: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == rawContactId.toString()).assertTrue();
                    console.info('logMessage contact_pinyin_query_test_300: search_name = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == "Tom||Tom||Tom").assertTrue();
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_pinyin_query_test_300 query error = ' + error);
        }
    }

    /**
     * @tc.number  contact_fuzzyquery_test_400
     * @tc.name    Contact fuzzy search query interface (name)
     * @tc.desc    Function test
     */
    it("contact_fuzzyquery_test_400", 0, async function(done) {
        console.info("--------logMessage contact_fuzzyquery_test_400 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "400xiaoming400"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage contact_fuzzyquery_test_400 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await RawContactNameQuery();
            await deleteRawContact("contact_fuzzyquery_test_400");
            done();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_400 raw_contact insert error = ' + error);
            done();
        }

        async function RawContactNameQuery()
        {
            var resultColumns = [ "id", "display_name" ];
            let condition = new dataShare.DataSharePredicates();
            condition.like("display_name", "%xiaoming40%");
            condition.and();
            condition.equalTo("is_deleted", "0");
            try {
                var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
                if (resultSet.goToFirstRow()) {
                    do {
                        console.info('logMessage contact_fuzzyquery_test_400: id = ' + resultSet.getString(0));
                        expect(resultSet.getString(0) == rawContactId.toString()).assertEqual(true);
                        console.info(
                            'logMessage contact_fuzzyquery_test_400: display_name = ' + resultSet.getString(1));
                        expect(resultSet.getString(1) == "400xiaoming400").assertEqual(true);
                    } while (resultSet.goToNextRow());
                }
                resultSet.close();
            } catch (error) {
                console.info('logMessage contact_fuzzyquery_test_400 query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  contact_fuzzyquery_test_500
     * @tc.name    Contact fuzzy search query interface (name pinyin, pinyin name)
     * @tc.desc    Function test
     */
    it("contact_fuzzyquery_test_500", 0, async function(done) {
        console.info("--------logMessage contact_fuzzyquery_test_500 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        var rawContactValues = {"display_name" : "500xiaoming500", "phonetic_name" : "500xiaoming500||xm"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage contact_fuzzyquery_test_500 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await RawContactPhoneticNameQuery();
            await deleteRawContact("contact_fuzzyquery_test_500");
            done();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_500 raw_contact insert error = ' + error);
            done();
        }

        async function RawContactPhoneticNameQuery()
        {
            var resultColumns = [ "id", "display_name", "phonetic_name" ];
            let condition = new dataShare.DataSharePredicates();
            condition.like("phonetic_name", "%xiaoming%");
            condition.and();
            condition.equalTo("is_deleted", "0");
            condition.or();
            condition.like("phonetic_name", "%xm%");
            condition.and();
            condition.equalTo("is_deleted", "0");
            try {
                var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
                if (resultSet.goToFirstRow()) {
                    do {
                        console.info('logMessage contact_fuzzyquery_test_500: id = ' + resultSet.getString(0));
                        expect(resultSet.getString(0) == rawContactId.toString()).assertTrue();
                        console.info(
                            'logMessage contact_fuzzyquery_test_500: display_name = ' + resultSet.getString(1));
                        expect(resultSet.getString(1) == "500xiaoming500").assertTrue();
                        console.info(
                            'logMessage contact_fuzzyquery_test_500: phonetic_name = ' + resultSet.getString(array_two));
                        expect(resultSet.getString(array_two) == "500xiaoming500||xm").assertTrue();
                    } while (resultSet.goToNextRow());
                }
                resultSet.close();
            } catch (error) {
                console.info('logMessage contact_fuzzyquery_test_500 query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  contact_fuzzyquery_test_600
     * @tc.name    Contact fuzzy search query interface (company, position)
     * @tc.desc    Function test
     */
    it("contact_fuzzyquery_test_600", 0, async function(done) {
        console.info("--------logMessage contact_fuzzyquery_test_600 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoming", "company" : "TT600", "position" : "Testers600"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage contact_fuzzyquery_test_600 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await RawContactCompanyQuery(dataShareHelper, rawContactId);
            await deleteRawContact("contact_fuzzyquery_test_600");
            done();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_600 raw_contact insert error = ' + error);
            done();
        }
    });

    async function RawContactCompanyQuery(dataShareHelper, rawContactId)
    {
        var resultColumns = [ "id", "display_name", "company", "position" ];
        let condition = new dataShare.DataSharePredicates();
        condition.like("company", "%TT6%");
        condition.and();
        condition.equalTo("is_deleted", "0");
        condition.or();
        condition.like("position", "%Testers6%");
        condition.and();
        condition.equalTo("is_deleted", "0");
        try {
            var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_fuzzyquery_test_600: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == rawContactId.toString()).assertEqual(true);
                    console.info('logMessage contact_fuzzyquery_test_600: display_name = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == "xiaoming").assertEqual(true);
                    console.info('logMessage contact_fuzzyquery_test_600: company = ' + resultSet.getString(array_two));
                    expect(resultSet.getString(array_two) == "TT600").assertEqual(true);
                    console.info('logMessage contact_fuzzyquery_test_600: position = ' + resultSet.getString(array_three));
                    expect(resultSet.getString(array_three) == "Testers600").assertEqual(true);
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_600 query error = ' + error);
        }
    }

    /**
     * @tc.number  contact_fuzzyquery_test_700
     * @tc.name    Contact fuzzy search query interface (mobile phone number)
     * @tc.desc    Function test
     */
    it("contact_fuzzyquery_test_700", 0, async function(done) {
        console.info("--------logMessage contact_fuzzyquery_test_700 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoming"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage contact_fuzzyquery_test_700 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataPhoneInsert();
            await deleteRawContact("contact_fuzzyquery_test_700");
            await deleteAll(contactDataUri, "contact_fuzzyquery_test_700");
            done();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_700 raw_contact insert error = ' + error);
            done();
        }

        async function ContactDataPhoneInsert()
        {
            var contactDataValues = {
                "raw_contact_id" : rawContactId,
                "content_type" : "phone",
                "detail_info" : "14528963"
            };
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                sleep(sleep_one);
                console.info('logMessage contact_fuzzyquery_test_700 contactDataId  = ' + contactDataId);
                expect(contactDataId > 0).assertTrue();
                await fuzzyQueryPhone(dataShareHelper, rawContactId, contactDataId);
            } catch (error) {
                console.info('logMessage contact_fuzzyquery_test_700 contact_data insert error = ' + contactDataId);
                done();
            }
        }
    });

    async function fuzzyQueryPhone(dataShareHelper, rawContactId, contactDataId)
    {
        var resultColumns = [ "id", "raw_contact_id", "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        // type_id = 5 data is phone
        condition.like("detail_info", "%52896%");
        condition.and();
        condition.equalTo("type_id", "5");
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_fuzzyquery_test_700: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == contactDataId.toString()).assertEqual(true);
                    console.info('logMessage contact_fuzzyquery_test_700: raw_contact_id = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == rawContactId.toString()).assertEqual(true);
                    console.info('logMessage contact_fuzzyquery_test_700: detail_info = ' + resultSet.getString(array_two));
                    expect(resultSet.getString(array_two) == "14528963").assertEqual(true);
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_700 query error = ' + error);
        }
    }

    /**
     * @tc.number  contact_fuzzyquery_test_800
     * @tc.name    Contact fuzzy search query interface (nickname)
     * @tc.desc    Function test
     */
    it("contact_fuzzyquery_test_800", 0, async function(done) {
        console.info("--------logMessage contact_fuzzyquery_test_800 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoming"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage contact_fuzzyquery_test_800 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactNickNameInsert();
            await deleteRawContact("contact_fuzzyquery_test_800");
            await deleteAll(contactDataUri, "contact_fuzzyquery_test_800");
            done();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_800 raw_contact insert error = ' + error);
            done();
        }

        async function ContactNickNameInsert()
        {
            var contactDataValues = {
                "raw_contact_id" : rawContactId,
                "content_type" : "nickname",
                "detail_info" : "daming"
            };
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                console.info('logMessage contact_fuzzyquery_test_800 contactDataId = ' + contactDataId);
                expect(contactDataId > 0).assertTrue();
                await fuzzyQueryPhoneNickName(dataShareHelper, rawContactId, contactDataId);
            } catch (error) {
                console.info('logMessage contact_fuzzyquery_test_800 contact_data insert error = ' + contactDataId);
                done();
            }
        }
    });

    async function fuzzyQueryPhoneNickName(dataShareHelper, rawContactId, contactDataId)
    {
        var resultColumns = [ "id", "raw_contact_id", "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        // type_id = 3 data is nickname
        condition.like("detail_info", "%daming%");
        condition.and();
        condition.equalTo("type_id", "3");
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_fuzzyquery_test_800: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == contactDataId.toString()).assertTrue();
                    console.info('logMessage contact_fuzzyquery_test_800: raw_contact_id = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == rawContactId.toString()).assertTrue();
                    console.info('logMessage contact_fuzzyquery_test_800: detail_info = ' + resultSet.getString(array_two));
                    expect(resultSet.getString(array_two) == "daming").assertTrue();
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_800 query error = ' + error);
        }
    }
    /**
     * @tc.number  contact_fuzzyquery_test_900
     * @tc.name    Contact fuzzy search query interface (mailbox)
     * @tc.desc    Function test
     */
    it("contact_fuzzyquery_test_900", 0, async function(done) {
        console.info("--------logMessage contact_fuzzyquery_test_900 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoming"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage contact_fuzzyquery_test_900 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataEmailInsert();
            await deleteRawContact("contact_fuzzyquery_test_900");
            await deleteAll(contactDataUri, "contact_fuzzyquery_test_900");
            done();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_900 raw_contact insert error = ' + error);
            done();
        }

        async function ContactDataEmailInsert()
        {
            var contactDataValues = {
                "raw_contact_id" : rawContactId,
                "content_type" : "email",
                "detail_info" : "1564@163.com"
            };
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                console.info('logMessage contact_fuzzyquery_test_900 ; = ' + contactDataId);
                expect(contactDataId > 0).assertTrue();
                await fuzzyQueryEmail(dataShareHelper, rawContactId, contactDataId);
            } catch (error) {
                console.info('logMessage contact_fuzzyquery_test_900 contact_data insert error = ' + error);
                done();
            }
        }
    });

    async function fuzzyQueryEmail(dataShareHelper, rawContactId, contactDataId)
    {
        var resultColumns = [ "id", "raw_contact_id", "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        // type_id = 1 data is email
        condition.like("detail_info", "%1564%");
        condition.and();
        condition.equalTo("type_id", "1");
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_fuzzyquery_test_900: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == contactDataId.toString()).assertTrue();
                    console.info('logMessage contact_fuzzyquery_test_900: raw_contact_id = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == rawContactId.toString()).assertTrue();
                    console.info('logMessage contact_fuzzyquery_test_900: detail_info = ' + resultSet.getString(array_two));
                    expect(resultSet.getString(array_two) == "1564@163.com").assertTrue();
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_900 query error = ' + error);
        }
    }

    /**
     * @tc.number  contact_fuzzyquery_test_1000
     * @tc.name    Contact fuzzy search query interface (residential)
     * @tc.desc    Function test
     */
    it("contact_fuzzyquery_test_1000", 0, async function(done) {
        console.info("--------logMessage contact_fuzzyquery_test_1000 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoming"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage contact_fuzzyquery_test_1000 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataPostalInsert();
            await deleteRawContact("contact_fuzzyquery_test_1000");
            await deleteAll(contactDataUri, "contact_fuzzyquery_test_1000");
            done();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_1000 raw_contact insert error = ' + error);
            done();
        }

        async function ContactDataPostalInsert()
        {
            var contactDataValues = {
                "raw_contact_id" : rawContactId,
                "content_type" : "postal_address",
                "detail_info" : "nanjing1000"
            };
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                sleep(sleep_one);
                console.info('logMessage contact_fuzzyquery_test_1000 contactDataId = ' + contactDataId);
                expect(contactDataId > 0).assertTrue();
                await fuzzyQueryAddress(dataShareHelper, rawContactId, contactDataId);
            } catch (error) {
                console.info('logMessage contact_fuzzyquery_test_1000 contact_data insert error = ' + error);
                done();
            }
        }
    });

    async function fuzzyQueryAddress(dataShareHelper, rawContactId, contactDataId)
    {
        var resultColumns = [ "id", "raw_contact_id", "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        // type_id = 1 data is postal_address
        condition.like("detail_info", "%nanjing10%");
        condition.and();
        condition.equalTo("type_id", "7");
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            sleep(sleep_one);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_fuzzyquery_test_1000: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == contactDataId.toString()).assertTrue();
                    console.info('logMessage contact_fuzzyquery_test_1000: raw_contact_id = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == rawContactId.toString()).assertTrue();
                    console.info('logMessage contact_fuzzyquery_test_1000: detail_info = ' + resultSet.getString(array_two));
                    expect(resultSet.getString(array_two) == "nanjing1000").assertTrue();
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_1000: query error' + error);
        }
    }

    /**
     * @tc.number  contact_fuzzyquery_test_1100
     * @tc.name    Contact fuzzy search query interface (remarks)
     * @tc.desc    Function test
     */
    it("contact_fuzzyquery_test_1100", 0, async function(done) {
        console.info("--------logMessage contact_fuzzyquery_test_1100 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoming"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage contact_fuzzyquery_test_1100 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataNoteInsert();
            await deleteRawContact("contact_fuzzyquery_test_1100");
            await deleteAll(contactDataUri, "contact_fuzzyquery_test_1100");
            done();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_1100 raw_contact insert error = ' + error);
            done();
        }

        async function ContactDataNoteInsert()
        {
            var contactDataValues = {
                "raw_contact_id" : rawContactId,
                "content_type" : "note",
                "detail_info" : "java1100"
            };
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                sleep(sleep_one);
                console.info('logMessage contact_fuzzyquery_test_1100 contactDataId = ' + contactDataId);
                expect(contactDataId > 0).assertTrue();
                await fuzzyQueryNote(dataShareHelper, rawContactId, contactDataId);
            } catch (error) {
                console.info('logMessage contact_fuzzyquery_test_1100 contact_data insert error = ' + error);
                done();
            }
        }
    });

    async function fuzzyQueryNote(dataShareHelper, rawContactId, contactDataId)
    {
        var resultColumns = [ "id", "raw_contact_id", "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        // type_id = 10 data is note
        condition.like("detail_info", "%java11%");
        condition.and();
        condition.equalTo("type_id", "10");
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_fuzzyquery_test_1100: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == contactDataId.toString()).assertEqual(true);
                    console.info('logMessage contact_fuzzyquery_test_1100: raw_contact_id = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == rawContactId.toString()).assertEqual(true);
                    console.info('logMessage contact_fuzzyquery_test_1100: detail_info = ' + resultSet.getString(array_two));
                    expect(resultSet.getString(array_two) == "java1100").assertEqual(true);
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_1100: query error' + error);
        }
    }

    /**
     * @tc.number  contact_fuzzyquery_test_1200
     * @tc.name    Contact fuzzy search query interface (AIM)
     * @tc.desc    Function test
     */
    it("contact_fuzzyquery_test_1200", 0, async function(done) {
        console.info("--------logMessage contact_fuzzyquery_test_1200 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoming"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info('logMessage contact_fuzzyquery_test_1200 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataAimInsert();
            await deleteRawContact("contact_fuzzyquery_test_1200");
            await deleteAll(contactDataUri, "contact_fuzzyquery_test_1200");
            done();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_1200 raw_contact insert error = ' + error);
            done();
        }

        async function ContactDataAimInsert()
        {
            var contactDataValues = {
                "raw_contact_id" : rawContactId,
                "content_type" : "im",
                "detail_info" : "aaaa1200"
            };
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                console.info('logMessage contact_fuzzyquery_test_1200 ; = ' + contactDataId);
                expect(contactDataId > 0).assertTrue();
                await fuzzyQueryIM(dataShareHelper, rawContactId, contactDataId);
            } catch (error) {
                console.info('logMessage contact_fuzzyquery_test_1200 contact_data insert error = ' + error);
                done();
            }
        }
    });

    async function fuzzyQueryIM(dataShareHelper, rawContactId, contactDataId)
    {
        var resultColumns = [ "id", "raw_contact_id", "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        // type_id = 10 data is im account number
        condition.like("detail_info", "%aa12%");
        condition.and();
        condition.equalTo("type_id", "2");
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            if (resultSet.goToFirstRow()) {
                do {
                    console.info('logMessage contact_fuzzyquery_test_1200: id = ' + resultSet.getString(0));
                    expect(resultSet.getString(0) == contactDataId.toString()).assertEqual(true);
                    console.info('logMessage contact_fuzzyquery_test_1200: raw_contact_id = ' + resultSet.getString(1));
                    expect(resultSet.getString(1) == rawContactId.toString()).assertEqual(true);
                    console.info('logMessage contact_fuzzyquery_test_1200: detail_info = ' + resultSet.getString(array_two));
                    expect(resultSet.getString(array_two) == "aaaa1200").assertEqual(true);
                } while (resultSet.goToNextRow());
            }
            resultSet.close();
        } catch (error) {
            console.info('logMessage contact_fuzzyquery_test_1200: query error' + error);
        }
    }

    /**
     * @tc.number  abnormal_contact_insert_test_200
     * @tc.name    Abnormal use case, an incorrect field is passed in when inserting
     * @tc.desc    Function test
     */
    it("abnormal_contact_insert_test_200", 0, async function(done) {
        console.info("------------abnormal_contact_insert_test_200 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_names" : "xiaoshan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_insert_test_200: rawContactId = " + rawContactId);
            expect(rawContactId).assertEqual(-1);
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_insert_test_200: raw_contact insert error' + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_insert_test_300
     * @tc.name    Abnormal use case, an incorrect table name is passed in when inserting
     * @tc.desc    Function test
     */
    it("abnormal_contact_insert_test_300", 0, async function(done) {
        console.info("------------abnormal_contact_insert_test_300 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var errorUri = "datashare:///com.ohos.calllogability/contacts/raw_contacts";

        var rawContactValues = {"display_name" : "xiaoshan"};
        try {
            var rawContactId = await dataShareHelper.insert(errorUri, rawContactValues);
            console.info("logMessage abnormal_contact_insert_test_300: rawContactId = " + rawContactId);
            expect(rawContactId == -1).assertTrue();
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_insert_test_300: raw_contact insert error' + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_update_test_1100
     * @tc.name    Abnormal use case, an incorrect field is passed in when updating
     * @tc.desc    Function test
     */
    it("abnormal_contact_update_test_1100", 0, async function(done) {
        console.info("------------abnormal_contact_update_test_1100 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaoshan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_update_test_1100: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await AbnormalUpdate();
            await deleteRawContact("abnormal_contact_update_test_1100");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_update_test_1100: raw_contact insert error' + error);
            done();
        }

        async function AbnormalUpdate()
        {
            var updateValues = {"display_names" : "xiaosan"};
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("ids", rawContactId.toString());
            try {
                var updataCode = await dataShareHelper.update(rawContactUri, updateValues, condition)
                console.info("logMessage abnormal_contact_update_test_1100: updataCode = " + updataCode);
                expect(updataCode == -1).assertTrue();
                var map = new Map();
                map.set("id", rawContactId.toString());
                map.set("display_name", "xiaoshan");
                await contactsQuery(map, "abnormal_contact_update_test_1100", rawContactUri);
            } catch (error) {
                console.info('logMessage abnormal_contact_update_test_1100: update error' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_update_test_1000
     * @tc.name    Abnormal use case, an incorrect table name is passed in when updating
     * @tc.desc    Function test
     */
    it("abnormal_contact_update_test_1000", 0, async function(done) {
        console.info("------------abnormal_contact_update_test_1000 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var errorUri = "datashare:///com.ohos.calllogability/contacts/raw_contacts";

        var rawContactValues = {"display_name" : "xiaoshan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_update_test_1000: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await AbnormalUpdate();
            await deleteRawContact("abnormal_contact_update_test_1000");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_update_test_1000: raw_contact insert error' + error);
            done();
        }

        async function AbnormalUpdate()
        {
            var updateValues = {"display_name" : "xiaosan"};
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", rawContactId.toString());
            try {
                var updataCode = await dataShareHelper.update(errorUri, updateValues, condition);
                console.info("logMessage abnormal_contact_update_test_1000: updataCode = " + updataCode);
                expect(updataCode == -1).assertTrue();
                var map = new Map();
                map.set("id", rawContactId.toString());
                map.set("display_name", "xiaoshan");
                await contactsQuery(map, "abnormal_contact_update_test_1000", rawContactUri);
            } catch (error) {
                console.info("logMessage abnormal_contact_update_test_1000: updata error = " + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_delete_test_4500
     * @tc.name    Abnormal use case, an incorrect field is passed in when deleting
     * @tc.desc    Function test
     */
    it("abnormal_contact_delete_test_4500", 0, async function(done) {
        console.info("------------abnormal_contact_delete_test_4500 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaozhi"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_delete_test_4500: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await AbnormalDelete();
            await deleteRawContact("abnormal_contact_delete_test_4500");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_delete_test_4500: raw_contact insert error' + error);
            done();
        }

        async function AbnormalDelete()
        {
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("ids", rawContactId.toString());
            try {
                var deleteCode = await dataShareHelper.delete(rawContactUri, condition);
                console.info("logMessage abnormal_contact_delete_test_4500: deleteCode = " + deleteCode);
                expect(deleteCode == -1).assertTrue();
                var map = new Map();
                map.set("id", rawContactId.toString());
                map.set("display_name", "xiaozhi");
                await contactsQuery(map, "abnormal_contact_delete_test_4500", rawContactUri);
                done();
            } catch (error) {
                console.info('logMessage abnormal_contact_delete_test_4500: delete error' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_delete_test_4600
     * @tc.name    Abnormal use case, an incorrect table name is passed in when deleting
     * @tc.desc    Function test
     */
    it("abnormal_contact_delete_test_4600", 0, async function(done) {
        console.info("------------abnormal_contact_delete_test_4600 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var errorUri = "datashare:///com.ohos.calllogability/contacts/raw_contacts";

        var rawContactValues = {"display_name" : "xiaoshan"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_delete_test_4600: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await AbnormalDelete();
            await deleteRawContact("abnormal_contact_delete_test_4600");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_delete_test_4600: raw_contact insert error' + error);
            done();
        }

        async function AbnormalDelete()
        {
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", rawContactId.toString());
            try {
                var deleteCode = await dataShareHelper.delete(errorUri, condition);
                console.info("logMessage abnormal_contact_delete_test_4600: deleteCode = " + deleteCode);
                expect(deleteCode == -1).assertTrue();
                var map = new Map();
                map.set("id", rawContactId.toString());
                map.set("display_name", "xiaoshan");
                await contactsQuery(map, "abnormal_contact_delete_test_4600", rawContactUri);
                done();
            } catch (error) {
                console.info('logMessage abnormal_contact_delete_test_4600: delete error' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_4200
     * @tc.name    Abnormal use case, an incorrect field is passed in when querying
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_4200", 0, async function(done) {
        console.info("------------abnormal_contact_query_test_4200 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaozhicheng"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_4200: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await AbnormalQuery();
            await deleteRawContact("abnormal_contact_query_test_4200");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_4200: raw_contact insert error' + error);
            done();
        }

        async function AbnormalQuery()
        {
            var resultColumns = [ "id", "display_name" ];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("ids", rawContactId.toString());
            try {
                var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
                expect(resultSet.goToFirstRow()).assertEqual(false);
                console.info('logMessage abnormal_contact_query_test_4200: goToFirstRow' + resultSet.goToFirstRow());
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_query_test_4200: query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_batchinsert_test_5100
     * @tc.name    Abnormal use cases, when adding data in batches, some of them failed, check the processing logic
     * @tc.desc    Function test
     */
    it("abnormal_contact_batchinsert_test_5100", 0, async function(done) {
        console.info("--------logMessage abnormal_contact_batchinsert_test_5100 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var addBulk_value1 = {"display_name" : "zhangsan"};
        var addBulk_value2 = {"display_names" : "lisi"};
        var addBulk_value3 = {"display_name" : "wangwu"};
        var addBulk_value4 = {"display_names" : "laoliu"};
        var addBulk_value5 = {"display_name" : "xiaozhang"};
        var listAddBluk = [];
        listAddBluk[0] = addBulk_value1;
        listAddBluk[1] = addBulk_value2;
        listAddBluk[array_two] = addBulk_value3;
        listAddBluk[array_three] = addBulk_value4;
        listAddBluk[array_four] = addBulk_value5;
        try {
            var batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
            console.info("logMessage abnormal_contact_batchinsert_test_5100: batchInsertCode = " + batchInsertCode);
            expect(batchInsertCode == -1).assertTrue();
            done();
        } catch (error) {
            console.info("logMessage abnormal_contact_batchinsert_test_5100: batch insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_insert_data_5300
     * @tc.name    insert_data error no rawContactId
     * @tc.desc    Function test
     */
    it("abnormal_contact_insert_data_5300", 0, async function(done) {
        console.info("--------logMessage abnormal_contact_insert_data_5300 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaotian"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage abnormal_contact_insert_data_5300 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataAimInsert();
            await deleteRawContact("abnormal_contact_insert_data_5300");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_insert_data_5300 raw_contact insert error = ' + error);
            done();
        }

        async function ContactDataAimInsert()
        {
            var contactDataValues = {"content_type" : "im", "detail_info" : "aaaa"};
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                console.info('logMessage abnormal_contact_insert_data_5300 ; = ' + contactDataId);
                expect(contactDataId == -1).assertTrue();
            } catch (error) {
                console.info('logMessage abnormal_contact_insert_data_5300 contact_data insert error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_insert_data_5400
     * @tc.name    insert_data error no content_type
     * @tc.desc    Function test
     */
    it("abnormal_contact_insert_data_5400", 0, async function(done) {
        console.info("--------logMessage abnormal_contact_insert_data_5400 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "xiaotian"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info('logMessage abnormal_contact_insert_data_5400 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await ContactDataAimInsert();
            await deleteRawContact("abnormal_contact_insert_data_5400");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_insert_data_5400 raw_contact insert error = ' + error);
            done();
        }

        async function ContactDataAimInsert()
        {
            var contactDataValues = {"raw_contact_id" : rawContactId, "detail_info" : "aaaa"};
            try {
                var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
                console.info('logMessage abnormal_contact_insert_data_5400 ; = ' + contactDataId);
                expect(contactDataId == -1).assertTrue();
            } catch (error) {
                console.info('logMessage abnormal_contact_insert_data_5400 contact_data insert error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  contact_delete_test_5500
     * @tc.name    batchDelete
     * @tc.desc    Function test
     */
    it("contact_delete_test_5500", 0, async function(done) {
        console.info("--------logMessage contact_delete_test_5500 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, common.getProfileRawContact());
            console.info("logMessage contact_delete_test_5500: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var map = common.getProfileRawContactMap();
            map.set("id", rawContactId.toString());
            await contactsQuery(map, "contact_delete_test_5500", rawContactUri);
            await executeBatch();
        } catch (error) {
            console.info("logMessage contact_delete_test_5500:  insert error = " + error);
            done();
        }
        async function executeBatch()
        {
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", rawContactId.toString());
            dataShareHelper.executeBatch(URI_CONTACTS, [ {
                uri : rawContactUri,
                type : featureAbility.DataAbilityOperationType.TYPE_DELETE,
                predicates : condition,
                expectedCount : 0,
                PredicatesBackReferences : {},
                interrupted : true,
            } ],
                (error, data) => {
                    console.info("logMessage contact_delete_test_5500: executeBatch data = " + JSON.stringify(data));
                    console.info("logMessage contact_delete_test_5500: data_3 = " + data);
                    console.info("logMessage contact_delete_test_5500:  data_1= " + data[0].count);
                    expect(data[0].count == 0).assertTrue();
                    done();
                });
        }
    });

    /**
     * @tc.number  contact_update_test_5600
     * @tc.name    batchUpdate
     * @tc.desc    Function test
     */
    it("contact_update_test_5600", 0, async function(done) {
        console.info("--------logMessage contact_update_test_5600 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, common.getProfileRawContact());
            console.info("logMessage contact_update_test_5600: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var map = common.getProfileRawContactMap();
            map.set("id", rawContactId.toString());
            await contactsQuery(map, "contact_update_test_5600", rawContactUri);
            await executeBatch();
        } catch (error) {
            console.info("logMessage contact_update_test_5600:  insert error = " + error);
            done();
        }

        async function executeBatch()
        {
            var updateValues = {"display_name" : "xiaoxiaoxiao"};
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", rawContactId.toString());
            dataShareHelper.executeBatch(URI_CONTACTS, [ {
                uri : rawContactUri,
                type : featureAbility.DataAbilityOperationType.TYPE_UPDATE,
                valuesBucket : updateValues,
                predicates : condition,
                expectedCount : 0,
                PredicatesBackReferences : {},
                interrupted : true,
            } ],
                (error, data) => {
                    console.info("logMessage contact_update_test_5600: executeBatch data = " + JSON.stringify(data));
                    console.info("logMessage contact_update_test_5600: data_3 = " + data);
                    console.info("logMessage contact_update_test_5600: data_1= " + data[0].count);
                    expect(data[0].count == 0).assertTrue();
                    done();
                });
        }
    });

    /**
     * @tc.number  contact_insert_test_5700
     * @tc.name    Insert Multiple raw_contact essential information
     * @tc.desc    Function test
     */
    it("contact_insert_test_5700", 0, async function(done) {
        console.info("------logMessage contact_insert_test_5700 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, common.getProfileRawContact());
            sleep(sleep_one);
            console.info('logMessage contact_insert_test_5700: rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var map = common.getProfileRawContactMap();
            map.set("id", rawContactId.toString());
            await contactsQuery(map, "contact_insert_test_5700", rawContactUri);
            var rawContactIdOne = await dataShareHelper.insert(rawContactUri, common.getProfileRawContact());
            sleep(sleep_one);
            console.info('logMessage contact_insert_test_5700: rawContactIdOne = ' + rawContactIdOne);
            expect(rawContactIdOne > 0).assertTrue();
            var map = common.getProfileRawContactMap();
            map.set("id", rawContactIdOne.toString());
            await contactsQuery(map, "contact_insert_test_5700", rawContactUri);
            var rawContactIdTwo = await dataShareHelper.insert(rawContactUri, common.getProfileRawContact());
            sleep(sleep_one);
            console.info('logMessage contact_insert_test_5700: rawContactIdTwo = ' + rawContactIdTwo);
            expect(rawContactIdTwo > 0).assertTrue();
            var map = common.getProfileRawContactMap();
            map.set("id", rawContactIdTwo.toString());
            await contactsQuery(map, "contact_insert_test_5700", rawContactUri);
            await deleteRawContact("contact_insert_test_5700");
            done();
        } catch (error) {
            console.info('logMessage contact_insert_test_5700: insert error = ' + error);
            done();
        }
    });

    /**
     * @tc.number  contact_insert_test_5800
     * @tc.name    Add 20000 bytes of basic contact information and verify whether the text insertion is successful
     * @tc.desc    Function test
     */
    it("contact_insert_test_5800", 0, async function(done) {
        console.info("------logMessage contact_insert_test_5800 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var name;
        for (var i = 0; i < 20000; i++) {
            name += "i";
        }
        var contcatvalues = {"display_name" : name}
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, contcatvalues);
            sleep(sleep_one);
            console.info('logMessage contact_insert_test_5800: rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var map = new Map();
            map.set("id", rawContactId.toString());
            map.set("display_name", name);
            await contactsQuery(map, "contact_insert_test_5800", rawContactUri);
            await deleteRawContact("contact_insert_test_5800");
            done();
        } catch (error) {
            console.info('logMessage contact_insert_test_5800: insert error = ' + error);
            done();
        }
    });

    /**
     * @tc.number  contact_insert_test_5900
     * @tc.name    Add multiple contact details and verify that the insertion was successful
     * @tc.desc    Function test
     */
    it("contact_insert_test_5900", 0, async function(done) {
        console.info("------logMessage contact_insert_test_5900 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var contcatvalues = {"display_name" : "xiao5900"}
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, contcatvalues);
            console.info('logMessage contact_insert_test_5900: rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var contactDataValues1 = {
                "raw_contact_id" : rawContactId,
                "content_type" : "phone",
                "detail_info" : "19960229"
            };
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            console.info('logMessage contact_insert_test_5900: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();

            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("detail_info", "19960229");
            await contactsQuery(map, "contact_insert_test_5900", contactDataUri);

            var rawContactIdOne = await dataShareHelper.insert(rawContactUri, contcatvalues);
            console.info('logMessage contact_insert_test_5900: rawContactId = ' + rawContactIdOne);
            expect(rawContactIdOne > 0).assertTrue();
            var contactDataValues2 = {
                "raw_contact_id" : rawContactIdOne,
                "content_type" : "phone",
                "detail_info" : "111111032"
            };
            var contactDataId1 = await dataShareHelper.insert(contactDataUri, contactDataValues2);
            console.info('logMessage contact_insert_test_5900: contactDataId = ' + contactDataId1);
            expect(contactDataId1 > 0).assertTrue();

            var map = new Map();
            map.set("id", contactDataId1.toString());
            map.set("detail_info", "111111032");
            await contactsQuery(map, "contact_insert_test_5900", contactDataUri);

            await deleteRawContact("contact_insert_test_5900");
            await deleteAll(contactDataUri, "contact_insert_test_5900");
            done();
        } catch (error) {
            console.info('logMessage contact_insert_test_5900: insert error = ' + error);
            done();
        }
    });

    /**
     * @tc.number  contact_insert_test_6000
     * @tc.name    Add the basic information of a single contact and verify whether the insertion is successful (name,
     * Pinyin, company, position, favorite)
     * @tc.desc    Function test
     */
    it("contact_insert_test_6000", 0, async function(done) {
        console.info("------logMessage contact_insert_test_6000 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "licheng",
            "company" : "TT",
            "position" : "developer",
            "favorite" : 1,
            "phonetic_name" : "licheng||lc"
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insert_test_6000: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();

            var map = new Map();
            map.set("id", rawContactId.toString());
            map.set("display_name", "licheng");
            map.set("company", "TT");
            map.set("position", "developer");
            map.set("favorite", "1");
            map.set("phonetic_name", "licheng||lc");
            await contactsQuery(map, "contact_insert_test_6000", rawContactUri);
            await deleteRawContact("contact_insert_test_6000");
            await deleteAll(contactDataUri, "contact_insert_test_6000");
            done();
        } catch (error) {
            console.info("logMessage contact_insert_test_6000: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_insertData_test_6100
     * @tc.name    To contact_data Add a full field data to the data table and verify whether the insertion is
     * successful
     * @tc.desc    Function test
     */
    it("contact_insertData_test_6100", 0, async function(done) {
        console.info("------logMessage contact_insertData_test_6100 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocai",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insertData_test_6100: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var contactDataValues = common.getProfileContactData();
            contactDataValues["raw_contact_id"] = rawContactId;
            console.info('logMessage contact_insertData_test_6100: contactDataValues["raw_contact_id"] = ' +
                         contactDataValues.raw_contact_id);
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            console.info('logMessage contact_insertData_test_6100: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
            var map = common.getProfileContactDataMap();
            map.set("id", contactDataId.toString());
            map.set("raw_contact_id", rawContactId.toString());
            await contactsQuery(map, "contact_insertData_test_6100", contactDataUri);
            await deleteRawContact("contact_insertData_test_6100");
            await deleteAll(contactDataUri, "contact_insertData_test_6100");
            done();
        } catch (error) {
            console.info("logMessage contact_insertData_test_6100: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_update_test_6200
     * @tc.name    Modify the basic information of multiple contacts and verify whether the modification is successful
     * @tc.desc    Function test
     */
    it("contact_update_test_6200", 0, async function(done) {
        console.info("--------logMessage contact_update_test_6200 is starting!------------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var insertRawContactValues = {
                "display_name" : "xiaocai",
            };
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            expect(rawContactId > 0).assertTrue();
            console.info("logMessage contact_update_test_6200: rawContactId = " + rawContactId);
            var insertRawContactValuesOne = {
                "display_name" : "xiaocai1",
            };
            var rawContactIdOne = await dataShareHelper.insert(rawContactUri, insertRawContactValuesOne);
            expect(rawContactIdOne > 0).assertTrue();
            console.info("logMessage contact_update_test_6200: rawContactIdOne = " + rawContactIdOne);
            var insertRawContactValuesTwo = {
                "display_name" : "xiaocai1",
            };
            var rawContactIdTwo = await dataShareHelper.insert(rawContactUri, insertRawContactValuesTwo);
            expect(rawContactIdTwo > 0).assertTrue();
            console.info("logMessage contact_update_test_6200: rawContactIdTwo = " + rawContactIdTwo);

            var updateValues = {"display_name" : "xiaosan"};
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", rawContactId.toString());
            condition.or();
            condition.equalTo("id", rawContactIdOne.toString());
            condition.or();
            condition.equalTo("id", rawContactIdTwo.toString());
            var updataCode = await dataShareHelper.update(rawContactUri, updateValues, condition);
            console.info("logMessage contact_update_test_6200: updataCode = " + updataCode);
            expect(updataCode == 0).assertTrue();
            sleep(sleep_one);
            await queryUpdateThree(
                "contact_update_test_6200", dataShareHelper, rawContactId, rawContactIdOne, rawContactIdTwo);
            await deleteRawContact("contact_update_test_6200");
            done();
        } catch (error) {
            console.info("logMessage contact_update_test_6200:  insert error = " + error);
            done();
        }
    });

    async function queryUpdateThree(tag, dataShareHelper, rawContactId, rawContactIdOne, rawContactIdTwo)
    {

        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info(tag + ': contactsQuery start ! dataShareHelper = ' + dataShareHelper);
        var resultColumns = [ "display_name" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", rawContactId.toString());
        condition.or();
        condition.equalTo("id", rawContactIdOne.toString());
        condition.or();
        condition.equalTo("id", rawContactIdTwo.toString());
        try {
            var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
            if (resultSet.rowCount > 0) {
                if (resultSet.goToFirstRow()) {
                    do {
                        var name = resultSet.getString(resultSet.getColumnIndex("display_name"));
                        expect(name == "xiaosan").assertEqual(true);
                    } while (resultSet.goToNextRow());
                }
            }
            resultSet.close();
        } catch (error) {
            console.info(tag + "logMessage contactsQuery: error = " + error);
        }
    }

    /**
     * @tc.number  contact_UpdateRawContcat_test_6300
     * @tc.name    Update raw_contact The full field data of the contact table and verify whether the modification is
     * successful
     * @tc.desc    Function test
     */
    it("contact_UpdateRawContcat_test_6300", 0, async function(done) {
        console.info("------logMessage contact_UpdateRawContcat_test_6300 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, common.getProfileRawContact());
            console.info("logMessage contact_UpdateRawContcat_test_6300: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            sleep(sleep_two);
            var updateValues = common.getProfileRawContactUpdate();
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", rawContactId.toString());
            var updataCode = await dataShareHelper.update(rawContactUri, updateValues, condition);
            console.info('logMessage contact_UpdateRawContcat_test_6300: updataCode = ' + updataCode);
            sleep(3000);
            var map = common.getProfileRawContactUpdateMap();
            map.set("id", rawContactId.toString());
            await contactsQuery(map, "contact_UpdateRawContcat_test_6300", rawContactUri);
            await deleteRawContact("contact_UpdateRawContcat_test_6300");
            await deleteAll(contactDataUri, "contact_UpdateRawContcat_test_6300");
            done();
        } catch (error) {
            console.info("logMessage contact_UpdateRawContcat_test_6300: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_insertDataUpdate_test_6400
     * @tc.name    Update contact_data table, and verify whether the modification is successful
     * @tc.desc    Function test
     */
    it("contact_insertDataUpdate_test_6400", 0, async function(done) {
        console.info("------logMessage contact_insertDataUpdate_test_6400 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocai",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insertDataUpdate_test_6400: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();

            var contactDataValues = common.getProfileContactData();
            contactDataValues["raw_contact_id"] = rawContactId;
            console.info('logMessage contact_insertDataUpdate_test_6400: contactDataValues.raw_contact_id = ' +
                         contactDataValues.raw_contact_id);
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            console.info('logMessage contact_insertDataUpdate_test_6400: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();

            var updateValues = common.getProfileContactDataUpdate();
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", contactDataId.toString());
            var updataCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            console.info('logMessage contact_insertDataUpdate_test_6400: updataCode = ' + updataCode);
            sleep(sleep_one);
            var map = common.getProfileContactDataUpdateMap();
            map.set("id", contactDataId.toString());
            map.set("raw_contact_id", rawContactId.toString());
            await contactsQuery(map, "contact_insertDataUpdate_test_6400", contactDataUri);
            await deleteRawContact("contact_insertDataUpdate_test_6400");
            await deleteAll(contactDataUri, "contact_insertDataUpdate_test_6400");
            done();
        } catch (error) {
            console.info("logMessage contact_insertDataUpdate_test_6400: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_insertDataUpdateAll_test_6500
     * @tc.name    Update all contact details and verify that the modification was successful
     * @tc.desc    Function test
     */
    it("contact_insertDataUpdateAll_test_6500", 0, async function(done) {
        console.info("------logMessage contact_insertDataUpdateAll_test_6500 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocai",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insertDataUpdateAll_test_6500: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await insertData(rawContactId, "name", "xiaocai", "");
            await insertData(rawContactId, "phone", "6500", "");
            await insertData(rawContactId, "organization", "TTTTT", "Deve");
            await insertData(rawContactId, "nickname", "xxxxcai", "");
            await insertData(rawContactId, "email", "111@fox.com", "");
            await insertData(rawContactId, "postal_address", "dddd", "");
            await insertData(rawContactId, "note", "caicai", "");
            await insertData(rawContactId, "im", "999999999", "");
            await insertData(rawContactId, "contact_event", "1125", "");
            await insertData(rawContactId, "website", "wwww.xxx.com", "");
            await insertData(rawContactId, "relation", "xiaobai", "");
            await insertData(rawContactId, "group_membership", 1, "");
            await insertData(rawContactId, "contact_misc", "5678", "");
            var updateValues = {"detail_info" : "xiaocai"};
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("raw_contact_id", rawContactId.toString());
            var updataCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            console.info('logMessage contact_insertDataUpdateAll_test_6500: updataCode = ' + updataCode);
            sleep(sleep_one);
            await queryUpdateAllData("contact_insertDataUpdateAll_test_6500", dataShareHelper, rawContactId);
            await deleteRawContact("contact_insertDataUpdateAll_test_6500");
            await deleteAll(contactDataUri, "contact_insertDataUpdateAll_test_6500");
            done();
        } catch (error) {
            console.info("logMessage contact_insertDataUpdateAll_test_6500: raw_contact insert error = " + error);
            done();
        }
    });

    async function queryUpdateAllData(tag, dataShareHelper, rawContactId)
    {

        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info(tag + ': contactsQuery start ! dataShareHelper = ' + dataShareHelper);
        var resultColumns = [ "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("row_contact_id", rawContactId.toString());
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            if (resultSet.rowCount > 0) {
                if (resultSet.goToFirstRow()) {
                    do {
                        var detail_info = resultSet.getString(resultSet.getColumnIndex("detail_info"));
                        expect(detail_info == "xiaocai").assertEqual(true);
                    } while (resultSet.goToNextRow());
                }
            }
            resultSet.close();
        } catch (error) {
            console.info(tag + "logMessage contactsQuery: error = " + error);
        }
    }

    /**
     * @tc.number  contact_insertFavorite_test_6600
     * @tc.name    Add multiple contacts to favorites and verify that the favorites field is changed
     * @tc.desc    Function test
     */
    it("contact_insertFavorite_test_6600", 0, async function(done) {
        console.info("------logMessage contact_insertFavorite_test_6600 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        try {
            var insertRawContactValues = {"display_name" : "xiaocai", "favorite" : 1};
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insertFavorite_test_6600: rawContactId = " + rawContactId);

            var insertRawContactValuesOne = {"display_name" : "xiaotian", "favorite" : 1};
            var rawContactIdOne = await dataShareHelper.insert(rawContactUri, insertRawContactValuesOne);
            console.info("logMessage contact_insertFavorite_test_6600: rawContactId = " + rawContactIdOne);

            var insertRawContactValuesTwo = {"display_name" : "xiaoli", "favorite" : 1};
            var rawContactIdTwo = await dataShareHelper.insert(rawContactUri, insertRawContactValuesTwo);
            console.info("logMessage contact_insertFavorite_test_6600: rawContactId = " + rawContactIdTwo);

            sleep(sleep_one);
            await queryThreeFavorite(
                "contact_insertFavorite_test_6600", dataShareHelper, rawContactId, rawContactIdOne, rawContactIdTwo, 1);
            await deleteRawContact("contact_insertFavorite_test_6600");
            done();
        } catch (error) {
            console.info("logMessage contact_insertFavorite_test_6600: raw_contact insert error = " + error);
            done();
        }
    });

    async function queryThreeFavorite(tag, dataShareHelper, rawContactId, rawContactIdOne, rawContactIdTwo, favoritevalue)
    {

        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info(tag + ': queryThreeFavorite start ! dataShareHelper = ' + dataShareHelper);
        var resultColumns = [ "favorite" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", rawContactId.toString());
        condition.or();
        condition.equalTo("id", rawContactIdOne.toString());
        condition.or();
        condition.equalTo("id", rawContactIdTwo.toString());
        try {
            var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
            if (resultSet.rowCount > 0) {
                if (resultSet.goToFirstRow()) {
                    do {
                        var favorite = resultSet.getInt(resultSet.getColumnIndex("favorite"));
                        expect(favorite == favoritevalue).assertEqual(true);
                    } while (resultSet.goToNextRow());
                }
            }
            resultSet.close();
        } catch (error) {
            console.info(tag + "logMessage queryThreeFavorite: error = " + error);
        }
    }

    /**
     * @tc.number  contact_unFavorite_test_6700
     * @tc.name    Remove favorites from multiple contacts and verify that the favorites field has changed
     * @tc.desc    Function test
     */
    it("contact_unFavorite_test_6700", 0, async function(done) {
        console.info("------logMessage contact_unFavorite_test_6700 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        try {
            var insertRawContactValues = {"display_name" : "xiaocai", "favorite" : 1};
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_unFavorite_test_6700: rawContactId = " + rawContactId);

            var insertRawContactValuesOne = {"display_name" : "xiaotian", "favorite" : 1};
            var rawContactIdOne = await dataShareHelper.insert(rawContactUri, insertRawContactValuesOne);
            console.info("logMessage contact_unFavorite_test_6700: rawContactId = " + rawContactIdOne);

            var insertRawContactValuesTwo = {"display_name" : "xiaoli", "favorite" : 1};
            var rawContactIdTwo = await dataShareHelper.insert(rawContactUri, insertRawContactValuesTwo);
            console.info("logMessage contact_unFavorite_test_6700: rawContactId = " + rawContactIdTwo);

            var updateValues = {"favorite" : 0};
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", rawContactId.toString());
            condition.or();
            condition.equalTo("id", rawContactIdOne.toString());
            condition.or();
            condition.equalTo("id", rawContactIdTwo.toString());
            var updataCode = await dataShareHelper.update(rawContactUri, updateValues, condition);
            console.info("logMessage contact_unFavorite_test_6700: updataCode = " + updataCode);
            sleep(sleep_one);
            await queryThreeFavorite(
                "contact_unFavorite_test_6700", dataShareHelper, rawContactId, rawContactIdOne, rawContactIdTwo, 0);
            await deleteRawContact("contact_unFavorite_test_6700");
            done();
        } catch (error) {
            console.info("logMessage contact_unFavorite_test_6700: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_insertContactBlocklist_test_6800
     * @tc.name    Add multiple records to the blocklist and verify whether the insertion is successful
     * @tc.desc    Function test
     */
    it("contact_insertContactBlocklist_test_6800", 0, async function(done) {
        console.info("--------logMessage contact_insertContactBlocklist_test_6800 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var batchInsertCode = await dataShareHelper.insert(contactBlocklistUri, common.getProfileBlockList());
            console.info("logMessage contact_insertContactBlocklist_test_6800: insert = " + batchInsertCode);
            var batchInsertCodeOne = await dataShareHelper.insert(contactBlocklistUri, common.getProfileBlockList());
            console.info("logMessage contact_insertContactBlocklist_test_6800: insert = " + batchInsertCodeOne);
            var batchInsertCodeTwo = await dataShareHelper.insert(contactBlocklistUri, common.getProfileBlockList());
            console.info("logMessage contact_insertContactBlocklist_test_6800: insert = " + batchInsertCodeTwo);
            sleep(sleep_one);
            expect(batchInsertCode > 0).assertTrue();
            await query(array_three);
            await deleteAll(contactBlocklistUri, "contact_insertContactBlocklist_test_6800");
            done();
        } catch (error) {
            console.info("logMessage contact_insertContactBlocklist_test_6800: insert error = " + error);
            done();
        }

        async function query(size)
        {
            var resultColumns = [];
            let condition = new dataShare.DataSharePredicates();
            var phoneNumber = randomNum(8);
            condition.equalTo("phone_number", phoneNumber);
            try {
                var resultSet = await dataShareHelper.query(contactBlocklistUri, resultColumns, condition);
                sleep(sleep_one);
                console.info('contact_insertContactBlocklist_test_6800 :resultSet.goToFirstRow()  = ' +
                             resultSet.goToFirstRow());
                console.info(
                    'contact_insertContactBlocklist_test_6800 : query result is  = ' + resultSet.rowCount == size);
                expect(resultSet.rowCount == size).assertEqual(true);
                resultSet.close();
            } catch (error) {
                console.info(" contact_insertContactBlocklist_test_6800  error = " + error);
            }
        }
    });

    /**
     * @tc.number  contact_removeContactBlocklist_test_6900
     * @tc.name    Delete multiple records from the blocklist and verify whether the deletion is successful
     * @tc.desc    Function test
     */
    it("contact_removeContactBlocklist_test_6900", 0, async function(done) {
        console.info("--------logMessage contact_removeContactBlocklist_test_6900 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var batchInsertCode = await dataShareHelper.insert(contactBlocklistUri, common.getProfileBlockList());
            console.info("logMessage contact_removeContactBlocklist_test_6900: batchInsertCode = " + batchInsertCode);
            var batchInsertCodeOne = await dataShareHelper.insert(contactBlocklistUri, common.getProfileBlockList());
            console.info("logMessage contact_removeContactBlocklist_test_6900: insert = " + batchInsertCodeOne);
            var batchInsertCodeTwo = await dataShareHelper.insert(contactBlocklistUri, common.getProfileBlockList());
            console.info("logMessage contact_removeContactBlocklist_test_6900: insert = " + batchInsertCodeTwo);
            sleep(sleep_one);
            expect(batchInsertCode > 0).assertTrue();
            await deleteAll(contactBlocklistUri, "contact_removeContactBlocklist_test_6900");
            await query(0);
            done();
        } catch (error) {
            console.info("logMessage contact_removeContactBlocklist_test_6900: batchInsert error = " + error);
            done();
        }
        async function query(size)
        {
            var resultColumns = [];
            let condition = new dataShare.DataSharePredicates();
            var phoneNumber = randomNum(8);
            condition.equalTo("phone_number", phoneNumber);
            try {
                var resultSet = await dataShareHelper.query(contactBlocklistUri, resultColumns, condition);
                console.info('contact_removeContactBlocklist_test_6900 : resultSet  = ' + resultSet);
                expect(resultSet.rowCount == size).assertEqual(true);
                resultSet.close();
            } catch (error) {
                console.info(" contact_removeContactBlocklist_test_6900  error = " + error);
            }
        }
    });

    /**
     * @tc.number  contact_insertGroup_test_7000
     * @tc.name    Add multiple contacts to favorites and verify that the favorites field is changed
     * @tc.desc    Function test
     */
    it("contact_insertGroup_test_7000", 0, async function(done) {
        console.info("------logMessage contact_insertGroup_test_7000 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        try {
            var insertRawContactValues = {
                "display_name" : "xiaocai",
            };
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insertGroup_test_7000: rawContactId = " + rawContactId);
            await insertData(rawContactId, "group_membership", 1, "");

            var insertRawContactValuesOne = {
                "display_name" : "xiaotian",
            };
            var rawContactIdOne = await dataShareHelper.insert(rawContactUri, insertRawContactValuesOne);
            console.info("logMessage contact_insertGroup_test_7000: rawContactId = " + rawContactIdOne);
            await insertData(rawContactIdOne, "group_membership", 1, "");
            var insertRawContactValuesTwo = {
                "display_name" : "xiaoli",
            };
            var rawContactIdTwo = await dataShareHelper.insert(rawContactUri, insertRawContactValuesTwo);
            console.info("logMessage contact_insertGroup_test_7000: rawContactId = " + rawContactIdTwo);
            await insertData(rawContactIdTwo, "group_membership", 1, "");
            sleep(sleep_one);
            await queryThreeGroup(
                "contact_insertGroup_test_7000", dataShareHelper, rawContactId, rawContactIdOne, rawContactIdTwo, 1);
            await deleteRawContact("contact_insertGroup_test_7000");
            done();
        } catch (error) {
            console.info("logMessage contact_insertGroup_test_7000: raw_contact insert error = " + error);
            done();
        }
    });

    async function queryThreeGroup(tag, dataShareHelper, rawContactId, rawContactIdOne, rawContactIdTwo, groupId)
    {
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info(tag + ': queryThreeGroup start ! dataShareHelper = ' + dataShareHelper);
        var resultColumns = [ "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", rawContactId.toString());
        condition.or();
        condition.equalTo("id", rawContactIdOne.toString());
        condition.or();
        condition.equalTo("id", rawContactIdTwo.toString());
        try {
            var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
            if (resultSet.rowCount > 0) {
                if (resultSet.goToFirstRow()) {
                    do {
                        var group = resultSet.getInt(resultSet.getColumnIndex("detail_info"));
                        expect(group == groupId).assertEqual(true);
                    } while (resultSet.goToNextRow());
                }
            }
            resultSet.close();
        } catch (error) {
            console.info(tag + "logMessage queryThreeGroup: error = " + error);
        }
    }

    /**
     * @tc.number  contact_insert_test_7100
     * @tc.name    Add a full field data to the groups table and verify whether the insertion is successful
     * @tc.desc    Function test
     */
    it("contact_insert_test_7100", 0, async function(done) {
        console.info("---------logMessage contact_insert_test_7100 is starting!----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var groupId = await dataShareHelper.insert(groupUri, common.getProfileGroup());
            console.info("logMessage contact_insert_test_7100: groupId = " + groupId);
            expect(groupId > 0).assertTrue();
            var map = common.getProfileGroupMap();
            map.set("id", groupId.toString());
            await contactsQuery(map, "contact_insert_test_7100", groupUri);
            await deleteAll(groupUri, "contact_insert_test_7100");
            done();
        } catch (error) {
            console.info("logMessage contact_insert_test_7100: group insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_delete_test_7200
     * @tc.name    Add a full field data to the groups table and verify whether the insertion is successful
     * @tc.desc    Function test
     */
    it("contact_delete_test_7200", 0, async function(done) {
        console.info("------logMessage contact_delete_test_7200 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        try {
            var insertRawContactValues = {
                "display_name" : "xiaocai",
            };
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_delete_test_7200: rawContactId = " + rawContactId);
            await insertData(rawContactId, "group_membership", 1, "");

            var insertRawContactValuesOne = {
                "display_name" : "xiaotian",
            };
            var rawContactIdOne = await dataShareHelper.insert(rawContactUri, insertRawContactValuesOne);
            console.info("logMessage contact_delete_test_7200: rawContactId = " + rawContactIdOne);
            await insertData(rawContactIdOne, "group_membership", 1, "");
            var insertRawContactValuesTwo = {
                "display_name" : "xiaoli",
            };
            var rawContactIdTwo = await dataShareHelper.insert(rawContactUri, insertRawContactValuesTwo);
            console.info("logMessage contact_delete_test_7200: rawContactId = " + rawContactIdTwo);
            await insertData(rawContactIdTwo, "group_membership", 1, "");
            sleep(sleep_one);
            await deleteAll(contactDataUri, "contact_delete_test_7200");
            sleep(sleep_one);
            await queryZeroGroup("contact_delete_test_7200", dataShareHelper, rawContactId, rawContactIdOne, rawContactIdTwo);
            await deleteRawContact("contact_delete_test_7200");
            done();
        } catch (error) {
            console.info("logMessage contact_delete_test_7200: raw_contact insert error = " + error);
            done();
        }
    });

    async function queryZeroGroup(tag, dataShareHelper, rawContactId, rawContactIdOne, rawContactIdTwo)
    {

        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info(tag + ': queryZeroGroup start ! dataShareHelper = ' + dataShareHelper);
        var resultColumns = [ "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        condition.or();
        condition.equalTo("raw_contact_id", rawContactIdOne.toString());
        condition.or();
        condition.equalTo("raw_contact_id", rawContactIdTwo.toString());
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            console.info(tag + ': queryZeroGroup! resultSet.rowCount  = ' + resultSet.rowCount);
            expect(resultSet.rowCount == 0).assertEqual(true);
            resultSet.close();
        } catch (error) {
            console.info(tag + "logMessage queryZeroGroup: error = " + error);
        }
    }

    /**
     * @tc.number  contact_delete_test_7300
     * @tc.name    Delete the full field data of a groups table and verify whether the deletion is successful
     * @tc.desc    Function test
     */
    it("contact_delete_test_7300", 0, async function(done) {
        console.info("---------logMessage contact_delete_test_7300 is starting!----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var groupId = await dataShareHelper.insert(groupUri, common.getProfileGroup());
            console.info("logMessage contact_delete_test_7300: groupId = " + groupId);
            expect(groupId > 0).assertTrue();
            var map = common.getProfileGroupMap();
            map.set("id", groupId.toString());
            await contactsQuery(map, "contact_delete_test_7300", groupUri);
            await deleteAll(groupUri, "contact_delete_test_7300");
            done();
        } catch (error) {
            console.info("logMessage contact_delete_test_7300: group insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_delete_test_7400
     * @tc.name    Add a raw_contact all fields, delete raw_contact
     * @tc.desc    Function test
     */
    it("contact_delete_test_7400", 0, async function(done) {
        console.info("------logMessage contact_Delete_test_7400 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, common.getProfileRawContact());
            sleep(sleep_one);
            console.info('logMessage contact_Delete_test_7400: rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var map = common.getProfileRawContactMap();
            map.set("id", rawContactId.toString());
            map.set("version", "2");
            map.set("is_deleted", "1");
            await deleteAll(rawContactUri, "contact_Delete_test_7400");
            sleep(sleep_one);
            await contactsQuery(map, "contact_Delete_test_7400", rawContactUri);
            var conditionAll = new dataShare.DataSharePredicates();
            conditionAll.greaterThan("id", "0");
            var code = await dataShareHelper.delete(deletedUri, conditionAll);
            console.info('contact_Delete_test_7400 : Completely delete code = ' + code);
            expect(code == 0).assertTrue();
            done();
        } catch (error) {
            console.info('logMessage contact_Delete_test_7400: insert error = ' + error);
            done();
        }
    });

    /**
     * @tc.number  contact_delete_test_7500
     * @tc.name    Delete contact_data Single full field data in data table
     * @tc.desc    Function test
     */
    it("contact_delete_test_7500", 0, async function(done) {
        console.info("------logMessage contact_delete_test_7500 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocai",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_delete_test_7500: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var contactDataValues = common.getProfileContactData();
            contactDataValues["raw_contact_id"] = rawContactId;
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues);
            console.info('logMessage contact_delete_test_7500: contactDataId = ' + contactDataId);
            expect(contactDataId > 0).assertTrue();
            await deleteAll(contactDataUri, "contact_delete_test_7500");
            var map = new Map();
            map.set("id", contactDataId.toString());
            await queryIdForDelete(map, "contact_delete_test_7500", contactDataUri);
            await deleteRawContact("contact_delete_test_7500");
            done();
        } catch (error) {
            console.info("logMessage contact_delete_test_7500: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_insertDataAll_test_7600
     * @tc.name    Add all details of a single contact and verify whether the insertion is successful (name, mobile
     * number, company, position, nickname, email, home, remarks, aim)
     * @tc.desc    Function test
     */
    it("contact_insertDataAll_test_7600", 0, async function(done) {
        console.info("------logMessage contact_insertDataAll_test_7600 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocai",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insertDataAll_test_7600: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var contactdata1 = await insertData(rawContactId, "name", "xiaocai", "");
            await queryAlldata("contact_insertDataAll_test_7600", dataShareHelper, rawContactId, contactdata1, "xiaocai");
            var contactdata2 = await insertData(rawContactId, "phone", "6500", "");
            await queryAlldata("contact_insertDataAll_test_7600", dataShareHelper, rawContactId, contactdata2, "6500");
            var contactdatd3 = await insertData(rawContactId, "organization", "TTTTT", "Deve");
            await queryAlldata("contact_insertDataAll_test_7600", dataShareHelper, rawContactId, contactdatd3, "TTTTT");
            var contactdatd4 = await insertData(rawContactId, "nickname", "xxxxcai", "");
            await queryAlldata("contact_insertDataAll_test_7600", dataShareHelper, rawContactId, contactdatd4, "xxxxcai");
            var contactdatd5 = await insertData(rawContactId, "email", "111@fox.com", "");
            await queryAlldata("contact_insertDataAll_test_7600", dataShareHelper, rawContactId, contactdatd5, "111@fox.com");
            var contactdatd6 = await insertData(rawContactId, "postal_address", "dddd", "");
            await queryAlldata("contact_insertDataAll_test_7600", dataShareHelper, rawContactId, contactdatd6, "dddd");
            var contactdatd7 = await insertData(rawContactId, "note", "caicai", "");
            await queryAlldata("contact_insertDataAll_test_7600", dataShareHelper, rawContactId, contactdatd7, "caicai");
            var contactdatd8 = await insertData(rawContactId, "im", "999999999", "");
            await queryAlldata("contact_insertDataAll_test_7600", dataShareHelper, rawContactId, contactdatd8, "999999999");
            sleep(sleep_one);
            await deleteRawContact("contact_insertDataAll_test_7600");
            await deleteAll(contactDataUri, "contact_insertDataAll_test_7600");
            done();
        } catch (error) {
            console.info("logMessage contact_insertDataAll_test_7600: raw_contact insert error = " + error);
            done();
        }
    });

    async function queryAlldata(tag, dataShareHelper, rawContactId, contactdata, values)
    {
        var resultColumns = [ "raw_contact_id", "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("id", contactdata.toString());
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            if (resultSet.rowCount > 0) {
                if (resultSet.goToFirstRow()) {
                    do {
                        var id = resultSet.getString(resultSet.getColumnIndex("raw_contact_id"));
                        console.info(tag + " result raw_contact_id = " + id);
                        expect(id == rawContactId).assertEqual(true);
                        var info = resultSet.getString(resultSet.getColumnIndex("detail_info"));
                        console.info(tag + " result info = " + info + " value = " + values);
                        expect(info == values).assertEqual(true);
                    } while (resultSet.goToNextRow());
                }
            }
            resultSet.close();
        } catch (error) {
            console.info(tag + " :logMessage queryIdForDelete: error = " + error);
        }
    }

    /**
     * @tc.number  contact_insertDataAll_test_7700
     * @tc.name    Add all details of a single contact and verify whether the insertion is successful (name, mobile
     * number, company, position, nickname, email, home, remarks, aim, birthday, website, assistant, group, phone ring)
     * @tc.desc    Function test
     */
    it("contact_insertDataAll_test_7700", 0, async function(done) {
        console.info("------logMessage contact_insertDataAll_test_7700 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var insertRawContactValues = {
            "display_name" : "xiaocai",
        };
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insertDataAll_test_7700: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var contactdata1 = await insertData(rawContactId, "name", "xiaocai", "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdata1, "xiaocai");
            var contactdata2 = await insertData(rawContactId, "phone", "6500", "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdata2, "6500");
            var contactdatd3 = await insertData(rawContactId, "organization", "TTTTT", "Deve");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd3, "TTTTT");
            var contactdatd4 = await insertData(rawContactId, "nickname", "xxxxcai", "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd4, "xxxxcai");
            var contactdatd5 = await insertData(rawContactId, "email", "111@fox.com", "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd5, "111@fox.com");
            var contactdatd6 = await insertData(rawContactId, "postal_address", "dddd", "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd6, "dddd");
            var contactdatd7 = await insertData(rawContactId, "note", "caicai", "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd7, "caicai");
            var contactdatd8 = await insertData(rawContactId, "im", "999999999", "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd8, "999999999");
            var contactdatd9 = await insertData(rawContactId, "contact_event", "1125", "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd9, "1125");
            var contactdatd10 = await insertData(rawContactId, "website", "wwww.xxx.com", "");
            await queryAlldata(
                "contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd10, "wwww.xxx.com");
            var contactdatd11 = await insertData(rawContactId, "relation", "xiaobai", "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd11, "xiaobai");
            var contactdatd12 = await insertData(rawContactId, "group_membership", 1, "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd12, 1);
            var contactdatd13 = await insertData(rawContactId, "contact_misc", "5678", "");
            await queryAlldata("contact_insertDataAll_test_7700", dataShareHelper, rawContactId, contactdatd13, "5678");
            sleep(sleep_one);
            await deleteRawContact("contact_insertDataAll_test_7700");
            await deleteAll(contactDataUri, "contact_insertDataAll_test_7700");
            done();
        } catch (error) {
            console.info("logMessage contact_insertDataAll_test_7700: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  contact_insertDataUpdateMore_test_7800
     * @tc.name    Modify multiple contact details and verify whether the modification is successful
     * @tc.desc    Function test
     */
    it("contact_insertDataUpdateMore_test_7800", 0, async function(done) {
        console.info("------logMessage contact_insertDataUpdateMore_test_7800 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var insertRawContactValues = {"display_name" : "xiaocai"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_insertDataUpdateMore_test_7800: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await insertData(rawContactId, "name", "xiaotian", "");

            var insertRawContactValues1 = {"display_name" : "xiaotian"};
            var rawContactId1 = await dataShareHelper.insert(rawContactUri, insertRawContactValues1);
            console.info("logMessage contact_insertDataUpdateMore_test_7800: rawContactId1 = " + rawContactId1);
            expect(rawContactId1 > 0).assertTrue();
            await insertData(rawContactId, "name", "xiaotian", "");

            var updateValues = {"detail_info" : "xiaocai7800"};
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("raw_contact_id", rawContactId.toString());
            condition.or();
            condition.equalTo("raw_contact_id", rawContactId1.toString())
            var updataCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
            console.info('logMessage contact_insertDataUpdateMore_test_7800: updataCode = ' + updataCode);
            sleep(sleep_one);
            await insertDataUpdateMoreQuery(
                "contact_insertDataUpdateMore_test_7800", dataShareHelper, rawContactId, rawContactId1);
            await deleteRawContact("contact_insertDataUpdateMore_test_7800");
            await deleteAll(contactDataUri, "contact_insertDataUpdateMore_test_7800");
            done();
        } catch (error) {
            console.info("logMessage contact_insertDataUpdateMore_test_7800: raw_contact insert error = " + error);
            done();
        }
    });

    async function insertDataUpdateMoreQuery(tag, dataShareHelper, rawContactId, rawContactId1)
    {
        var resultColumns = [ "detail_info" ];
        let condition = new dataShare.DataSharePredicates();
        condition.equalTo("raw_contact_id", rawContactId.toString());
        condition.or();
        condition.equalTo("raw_contact_id", rawContactId1.toString())
        try {
            var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
            if (resultSet.rowCount > 0) {
                if (resultSet.goToFirstRow()) {
                    do {
                        var info = resultSet.getString(resultSet.getColumnIndex("detail_info"));
                        console.info(tag + " result info = " + info);
                        expect(info == "xiaocai7800").assertEqual(true);
                    } while (resultSet.goToNextRow());
                }
            }
            resultSet.close();
        } catch (error) {
            console.info(tag + " :logMessage : error = " + error);
        }
    }

    /**
     * @tc.number  contact_deleterestore_test_7900
     * @tc.name    Delete contacts and restore
     * @tc.desc    Function test
     */
    it("contact_deleterestore_test_7900", 0, async function(done) {
        console.info("------logMessage contact_deleterestore_test_7900 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var insertRawContactValues = {"display_name" : "xaioli7900"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage contact_deleterestore_test_7900: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            sleep(sleep_one);
            var condition1 = new dataShare.DataSharePredicates();
            condition1.equalTo("id", rawContactId.toString());
            var deleteCode = await dataShareHelper.delete(rawContactUri, condition1);
            console.info("contact_deleterestore_test_7900 " +
                         ': deleteCode = ' + deleteCode);
            expect(deleteCode == 0).assertTrue();
            sleep(sleep_one);

            var condition2 = new dataShare.DataSharePredicates();
            condition2.equalTo("id", rawContactId.toString());
            var updatevalue = {"is_deleted" : 0};
            var updateCode = await dataShareHelper.update(rawContactUri, updatevalue, condition2);
            console.info('contact_deleterestore_test_7900 : update = ' + updateCode);
            sleep(sleep_one);

            var condition3 = new dataShare.DataSharePredicates();
            condition3.equalTo("id", rawContactId.toString());
            var code = await dataShareHelper.delete(deleted_raw_contact_record, condition3);
            console.info('contact_deleterestore_test_7900 : record code = ' + code);
            var map = new Map();
            map.set("id", rawContactId.toString());
            map.set("is_deleted", "0");
            await contactsQuery(map, "contact_deleterestore_test_7900", rawContactUri);
            await deleteRawContact("contact_deleterestore_test_7900");
            done();
        } catch (error) {
            console.info("logMessage contact_deleterestore_test_7900: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_updateData_test_8000
     * @tc.name    When modifying contact_data table, incorrect field or table name or nonexistent value is passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_updateData_test_8000", 0, async function(done) {
        console.info("------logMessage abnormal_contact_updateData_test_8000 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var insertRawContactValues = {"display_name" : "xaioli8000"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage abnormal_contact_updateData_test_8000: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            sleep(sleep_one);
            var contactDataId = await insertData(rawContactId, "name", "xaioli8000", "");
            console.info("logMessage abnormal_contact_updateData_test_8000: contactDataId = " + contactDataId);
            expect(contactDataId > 0).assertTrue();

            let condition = new dataShare.DataSharePredicates();
            var updateValues = {"display_namess" : "xaioli80000000"};
            condition.equalTo("id", contactDataId.toString());
            var code = await dataShareHelper.update(contactDataUri, updateValues, condition);
            console.info('abnormal_contact_updateData_test_8000 : update code = ' + code);
            expect(code == -1).assertTrue();
            var map = new Map();
            map.set("id", contactDataId.toString());
            map.set("display_name", "xaioli8000");
            await contactsQuery(map, "abnormal_contact_updateData_test_8000", contactDataUri);
            await deleteRawContact("abnormal_contact_updateData_test_8000");
            await deleteAll(contactDataUri, "abnormal_contact_updateData_test_8000");
            done();
        } catch (error) {
            console.info("logMessage abnormal_contact_updateData_test_8000: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_favorite_test_8100
     * @tc.name    When collecting contacts, an incorrect field was passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_favorite_test_8100", 0, async function(done) {
        console.info("------logMessage abnormal_contact_favorite_test_8100 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var insertRawContactValues = {"display_name" : "xaioli8100", "favoriteeee" : 1};
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage abnormal_contact_favorite_test_8100: rawContactId = " + rawContactId);
            expect(rawContactId == -1).assertTrue();
            sleep(sleep_one);
            done();
        } catch (error) {
            console.info("logMessage abnormal_contact_favorite_test_8100: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_blocklist_test_8200
     * @tc.name    When adding to the blocklist, an error field is passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_blocklist_test_8200", 0, async function(done) {
        console.info("------logMessage abnormal_contact_blocklist_test_8200 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        var phoneNumber = randomNum(6);
        try {
            var insertValues = {
                "phone_numberss" : phoneNumber,
            };
            var id = await dataShareHelper.insert(contactBlocklistUri, insertValues);
            console.info("logMessage abnormal_contact_blocklist_test_8200: id = " + id);
            expect(id == -1).assertTrue();
            sleep(sleep_one);
            done();
        } catch (error) {
            console.info("logMessage abnormal_contact_blocklist_test_8200: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_group_test_8300
     * @tc.name    When joining a group, an incorrect field is passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_group_test_8300", 0, async function(done) {
        console.info("------logMessage abnormal_contact_group_test_8300 is starting!-----");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var insertRawContactValues = {"display_name" : "xaioli8100"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, insertRawContactValues);
            console.info("logMessage abnormal_contact_group_test_8300: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var contactDataValues1 = {
                "raw_contact_id" : rawContactId,
                "content_typess" : "group_membership",
                "detail_info" : 1
            };
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            console.info("abnormal_contact_group_test_8300 logMessage : contactDataId " + contactDataId);
            expect(contactDataId == -1).assertTrue();
            done();
        } catch (error) {
            console.info("logMessage abnormal_contact_group_test_8300: raw_contact insert error = " + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_8400
     * @tc.name    When querying the basic information of multiple contacts, an error field is passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_8400", 0, async function(done) {
        console.info("------------abnormal_contact_query_test_8400 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian8400"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_8400: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var rawContactValues = {"display_name" : "xiaoli8400"};
            var rawContactId1 = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_8400: rawContactId1 = " + rawContactId1);
            expect(rawContactId1 > 0).assertTrue();
            await AbnormalQuery();
            await deleteRawContact("abnormal_contact_query_test_8400");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_8400: raw_contact insert error' + error);
            done();
        }

        async function AbnormalQuery()
        {
            var resultColumns = [ "id", "display_name" ];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("ids", rawContactId.toString());
            condition.or();
            condition.equalTo("ids", rawContactId1.toString());
            try {
                var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
                expect(resultSet.goToFirstRow()).assertEqual(false);
                console.info('logMessage abnormal_contact_query_test_8400: goToFirstRow' + resultSet.goToFirstRow());
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_query_test_8400: query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_8500
     * @tc.name    When querying the basic information of all contacts, an error field is passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_8500", 0, async function(done) {
        console.info("------------abnormal_contact_query_test_8500 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian8500"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_8500: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var rawContactValues = {"display_name" : "xiaoli8500"};
            var rawContactId1 = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_8500: rawContactId1 = " + rawContactId1);
            expect(rawContactId1 > 0).assertTrue();
            await AbnormalQuery();
            await deleteRawContact("abnormal_contact_query_test_8500");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_8500: raw_contact insert error' + error);
            done();
        }

        async function AbnormalQuery()
        {
            var resultColumns = [ "id", "display_name" ];
            let condition = new dataShare.DataSharePredicates();
            condition.greaterThan("ids", "0");
            try {
                var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
                expect(resultSet.goToFirstRow()).assertEqual(false);
                console.info('logMessage abnormal_contact_query_test_8500: goToFirstRow' + resultSet.goToFirstRow());
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_query_test_8500: query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_8600
     * @tc.name    When querying all the information of a single contact, an error field is passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_8600", 0, async function(done) {
        console.info("------------abnormal_contact_query_test_8600 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian8600"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_8600: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await AbnormalQuery();
            await deleteRawContact("abnormal_contact_query_test_8600");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_8600: raw_contact insert error' + error);
            done();
        }

        async function AbnormalQuery()
        {
            var resultColumns = [ "id", "display_names" ];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", rawContactId.toString());
            try {
                var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
                expect(resultSet.goToFirstRow()).assertEqual(false);
                console.info('logMessage abnormal_contact_query_test_8600: goToFirstRow' + resultSet.goToFirstRow());
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_query_test_8600: query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_8700
     * @tc.name    An incorrect field was passed in when querying group members
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_8700", 0, async function(done) {
        console.info("------------abnormal_contact_query_test_8700 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian8700"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_8700: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var dataId = await insertData(rawContactId, "group_membership", 1, "");
            console.info("logMessage abnormal_contact_query_test_8700: dataId = " + dataId);
            expect(dataId > 0).assertTrue();
            await AbnormalQuery();
            await deleteRawContact("abnormal_contact_query_test_8700");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_8700: raw_contact insert error' + error);
            done();
        }

        async function AbnormalQuery()
        {
            var resultColumns = [ "raw_contact_id" ];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("content_typess", "group_membership");
            try {
                var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
                expect(resultSet.goToFirstRow()).assertEqual(false);
                console.info('logMessage abnormal_contact_query_test_8700: goToFirstRow' + resultSet.goToFirstRow());
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_query_test_8700: query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_8800
     * @tc.name    An incorrect field was passed in when querying favorite contacts
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_8800", 0, async function(done) {
        console.info("------------abnormal_contact_query_test_8800 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian8700", "favorite" : 1};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_8800: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await AbnormalQuery();
            await deleteRawContact("abnormal_contact_query_test_8800");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_8800: raw_contact insert error' + error);
            done();
        }

        async function AbnormalQuery()
        {
            var resultColumns = [];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("ids", rawContactId.toString());
            try {
                var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
                expect(resultSet.goToFirstRow()).assertEqual(false);
                console.info('logMessage abnormal_contact_query_test_8800: goToFirstRow' + resultSet.goToFirstRow());
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_query_test_8800: query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_8900
     * @tc.name    An incorrect field was passed in when querying recent contacts
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_8900", 0, async function(done) {
        console.info("------------abnormal_contact_query_test_8900 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaoyuzhou", "lastest_contacted_time" : 60};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_8900: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            await AbnormalQuery();
            await deleteRawContact("abnormal_contact_query_test_8900");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_8900: raw_contact insert error' + error);
            done();
        }

        async function AbnormalQuery()
        {
            var resultColumns = [];
            let condition = new dataShare.DataSharePredicates();
            condition.greaterThan("lastest_contacted_timess", "50");
            try {
                var resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
                expect(resultSet.goToFirstRow()).assertEqual(false);
                console.info('logMessage abnormal_contact_query_test_8900: goToFirstRow' + resultSet.goToFirstRow());
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_query_test_8900: query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_9000
     * @tc.name    An error field was passed in when querying the recently deleted contact
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_9000", 0, async function(done) {
        console.info("------------abnormal_contact_query_test_9000 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_9000: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", rawContactId.toString());
            var deleteCode = await dataShareHelper.delete(rawContactUri, condition);
            console.info('abnormal_contact_query_test_9000 : deleteRawContact deleteCode = ' + deleteCode);
            expect(deleteCode == 0).assertTrue();
            await AbnormalQuery();
            var conditionAll = new dataShare.DataSharePredicates();
            conditionAll.greaterThan("id", "0");
            var code = await dataShareHelper.delete(deletedUri, conditionAll);
            console.info('abnormal_contact_query_test_9000 : Completely delete code = ' + code);
            expect(code == 0).assertTrue();
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_9000: raw_contact insert error' + error);
            done();
        }

        async function AbnormalQuery()
        {
            var resultColumns = [];
            let condition = new dataShare.DataSharePredicates();
            condition.orderByDesc("delete_timess");
            try {
                var resultSet = await dataShareHelper.query(deletedRawContactUri, resultColumns, condition);
                expect(resultSet.goToFirstRow()).assertEqual(false);
                console.info('logMessage abnormal_contact_query_test_9000: goToFirstRow' + resultSet.goToFirstRow());
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_query_test_9000: query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_9100
     * @tc.name    When querying all mobile phone numbers, an error field is passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_9100", 0, async function(done) {
        console.info("------------abnormal_contact_query_test_9100 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian9100"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_9100: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var dataId = await insertData(rawContactId, "phone", "11159100", "");
            console.info("logMessage abnormal_contact_query_test_9100: dataId = " + dataId);
            var dataId1 = await insertData(rawContactId, "phone", "111591009100", "");
            console.info("logMessage abnormal_contact_query_test_9100: dataId1 = " + dataId1);
            await AbnormalQuery();
            await deleteRawContact("abnormal_contact_query_test_9100");
            await deleteAll(contactDataUri, "abnormal_contact_query_test_9100");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_9100: raw_contact insert error' + error);
            done();
        }

        async function AbnormalQuery()
        {
            var resultColumns = [ "detail_info" ];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("content_typess", "phone");
            try {
                var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
                expect(resultSet.goToFirstRow()).assertEqual(false);
                console.info('logMessage abnormal_contact_query_test_9100: goToFirstRow' + resultSet.goToFirstRow());
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_query_test_9100: query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_9200
     * @tc.name    When querying all mailboxes, an error field was passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_9200", 0, async function(done) {
        console.info("------------abnormal_contact_query_test_9200 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian9100"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_query_test_9200: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var dataId = await insertData(rawContactId, "email", "fox@1.com", "");
            console.info("logMessage abnormal_contact_query_test_9200: dataId = " + dataId);
            var dataId1 = await insertData(rawContactId, "phone", "fox@2.com", "");
            console.info("logMessage abnormal_contact_query_test_9200: dataId1 = " + dataId1);
            await AbnormalQuery();
            await deleteRawContact("abnormal_contact_query_test_9200");
            await deleteAll(contactDataUri, "abnormal_contact_query_test_9200");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_9200: raw_contact insert error' + error);
            done();
        }

        async function AbnormalQuery()
        {
            var resultColumns = [ "detail_info" ];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("content_typess", "email");
            try {
                var resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
                expect(resultSet.goToFirstRow()).assertEqual(false);
                console.info('logMessage abnormal_contact_query_test_9200: goToFirstRow' + resultSet.goToFirstRow());
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_query_test_9200: query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_delete_test_10000
     * @tc.name    When querying all mailboxes, an error field was passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_delete_test_10000", 0, async function(done) {
        console.info("------------abnormal_contact_delete_test_10000 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian9300"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_delete_test_10000: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var dataId = await insertData(rawContactId, "email", "fox@1.com", "");
            console.info("logMessage abnormal_contact_delete_test_10000: dataId = " + dataId);
            var dataId1 = await insertData(rawContactId, "phone", "fox@2.com", "");
            console.info("logMessage abnormal_contact_delete_test_10000: dataId1 = " + dataId1);

            var errorUri = "datashare:///com.ohos.contactsdataability/contacts/contact_datasss";
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", dataId.toString());
            var deleteCode = await dataShareHelper.delete(errorUri, condition);
            console.info(' abnormal_contact_delete_test_10000 : deleteAll deleteCode = ' + deleteCode);
            expect(deleteCode == -1).assertTrue();
            var map = new Map();
            map.set("id", dataId.toString());
            map.set("detail_info", "fox@1.com");
            await contactsQuery(map, "abnormal_contact_delete_test_10000", contactDataUri);
            await deleteRawContact("abnormal_contact_delete_test_10000");
            await deleteAll(contactDataUri, "abnormal_contact_delete_test_10000");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_delete_test_10000: raw_contact insert error' + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_delete_test_9300
     * @tc.name    When deleting, an incorrect URI was passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_delete_test_9300", 0, async function(done) {
        console.info("------------abnormal_contact_delete_test_9300 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian9300"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_delete_test_9300: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var dataId = await insertData(rawContactId, "email", "fox@1.com", "");
            console.info("logMessage abnormal_contact_delete_test_9300: dataId = " + dataId);
            var dataId1 = await insertData(rawContactId, "phone", "fox@2.com", "");
            console.info("logMessage abnormal_contact_delete_test_9300: dataId1 = " + dataId1);

            var errorUri = "datashare:///com.ohos.contactsdataability/contacts/raw_contactsss";
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("id", rawContactId.toString());
            var deleteCode = await dataShareHelper.delete(errorUri, condition);
            console.info(' abnormal_contact_delete_test_9300 : deleteAll deleteCode = ' + deleteCode);
            expect(deleteCode == -1).assertTrue();
            var map = new Map();
            map.set("id", rawContactId.toString());
            map.set("display_name", "xiaotian9300");
            await contactsQuery(map, "abnormal_contact_delete_test_9300", rawContactUri);
            await deleteRawContact("abnormal_contact_delete_test_9300");
            await deleteAll(contactDataUri, "abnormal_contact_delete_test_9300");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_delete_test_9300: raw_contact insert error' + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_delete_test_9400
     * @tc.name    When deleting contacts in batch, an error field is passed in
     * @tc.desc    Function test
     */
    it("abnormal_contact_delete_test_9400", 0, async function(done) {
        console.info("------------abnormal_contact_delete_test_9400 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian9400"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_delete_test_9400: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var rawContactValues = {"display_name" : "xiaoli9400"};
            var rawContactId1 = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_delete_test_9400: rawContactId1 = " + rawContactId1);
            expect(rawContactId1 > 0).assertTrue();

            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("ids", rawContactId.toString());
            condition.or();
            condition.equalTo("ids", rawContactId1.toString())
            var deleteCode = await dataShareHelper.delete(rawContactUri, condition);
            console.info('abnormal_contact_delete_test_9400 :  deleteCode = ' + deleteCode);
            expect(deleteCode == -1).assertTrue();
            await deleteRawContact("abnormal_contact_delete_test_9400");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_delete_test_9400: raw_contact insert error' + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_favorite_test_9500
     * @tc.name    When collecting contacts in batch, the error field is transferred
     * @tc.desc    Function test
     */
    it("abnormal_contact_favorite_test_9500", 0, async function(done) {
        console.info("------------abnormal_contact_favorite_test_9500 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian9500", "favorite" : 0};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_favorite_test_9500: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var rawContactValues = {"display_name" : "xiaoli9500", "favorite" : 0};
            var rawContactId1 = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_favorite_test_9500: rawContactId1 = " + rawContactId1);
            expect(rawContactId1 > 0).assertTrue();

            let condition = new dataShare.DataSharePredicates();
            var updateValues = {"favorites" : 1};
            condition.equalTo("id", rawContactId.toString());
            condition.or();
            condition.equalTo("id", rawContactId1.toString())
            var code = await dataShareHelper.update(rawContactUri, updateValues, condition);
            console.info(' abnormal_contact_favorite_test_9500 : update code = ' + code);
            expect(code == -1).assertTrue();
            await deleteRawContact("abnormal_contact_favorite_test_9500");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_favorite_test_9500: raw_contact insert error' + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_insertblocklist_test_9600
     * @tc.name    When collecting contacts in batch, the error field is transferred
     * @tc.desc    Function test
     */
    it("abnormal_contact_insertblocklist_test_9600", 0, async function(done) {
        console.info("------------abnormal_contact_insertblocklist_test_9600 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {
                "display_name" : "xiaotian9600",
            };
            var phoneNumber = randomNum(12);
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_insertblocklist_test_9600: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var dataId = await insertData(rawContactId, "phone", phoneNumber, "");
            console.info("logMessage abnormal_contact_insertblocklist_test_9600: dataId = " + dataId);
            var rawContactValues = {
                "display_name" : "xiaoli9600",
            };
            var rawContactId1 = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_insertblocklist_test_9600: rawContactId1 = " + rawContactId1);
            expect(rawContactId1 > 0).assertTrue();
            var dataId1 = await insertData(rawContactId1, "phone", phoneNumber, "");
            console.info("logMessage abnormal_contact_insertblocklist_test_9600: dataId1 = " + dataId1);

            var blocklist = {"phone_numbers" : phoneNumber};
            var code = await dataShareHelper.insert(contactBlocklistUri, blocklist);
            expect(code == -1).assertTrue();
            await deleteRawContact("abnormal_contact_insertblocklist_test_9600");
            await deleteAll(contactDataUri, "abnormal_contact_insertblocklist_test_9600");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_insertblocklist_test_9600: raw_contact insert error' + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_insertGroup_test_9700
     * @tc.name    When a contact joins a group in batch, an error field is passed
     * @tc.desc    Function test
     */
    it("abnormal_contact_insertGroup_test_9700", 0, async function(done) {
        console.info("------------abnormal_contact_insertGroup_test_9700 is starting!-----------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
        try {
            var rawContactValues = {"display_name" : "xiaotian9700"};
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_insertGroup_test_9700: rawContactId = " + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var contactDataValues1 = {
                "raw_contact_id" : rawContactId,
                "content_typess" : "group_membership",
                "detail_info" : 1
            };
            var contactDataId = await dataShareHelper.insert(contactDataUri, contactDataValues1);
            console.info("abnormal_contact_insertGroup_test_9700 logMessage : contactDataId " + contactDataId);
            expect(contactDataId == -1).assertTrue();
            var rawContactValues = {"display_name" : "xiaoli9700"};
            var rawContactId1 = await dataShareHelper.insert(rawContactUri, rawContactValues);
            console.info("logMessage abnormal_contact_insertGroup_test_9700: rawContactId1 = " + rawContactId1);
            expect(rawContactId1 > 0).assertTrue();
            var contactDataValues2 = {
                "raw_contact_id" : rawContactId1,
                "content_typess" : "group_membership",
                "detail_info" : 1
            };
            var contactDataId1 = await dataShareHelper.insert(contactDataUri, contactDataValues2);
            console.info("abnormal_contact_insertGroup_test_9700 logMessage : contactDataId1 " + contactDataId1);
            expect(contactDataId1 == -1).assertTrue();
            await deleteRawContact("abnormal_contact_insertGroup_test_9700");
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_insertGroup_test_9700: raw_contact insert error' + error);
            done();
        }
    });

    /**
     * @tc.number  abnormal_contact_pinyinquery_test_9800
     * @tc.name    Insert a name with special characters to view the converted Pinyin
     * @tc.desc    Function test
     */
    it("abnormal_contact_pinyinquery_test_9800", 0, async function(done) {
        console.info("--------logMessage abnormal_contact_pinyinquery_test_9800 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "李bp玉成욱"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage abnormal_contact_pinyinquery_test_9800 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var dataId = await insertData(rawContactId, "name", "李bp玉成욱", "");
            console.info('logMessage abnormal_contact_pinyinquery_test_9800 dataId = ' + dataId);
            expect(dataId > 0).assertTrue();
            await query();
            await deleteRawContact("abnormal_contact_pinyinquery_test_9800");
            await deleteAll(contactDataUri, "abnormal_contact_pinyinquery_test_9800")
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_pinyinquery_test_9800 raw_contact insert error = ' + error);
            done();
        }

        async function query()
        {
            var resultColumns = [ "search_name" ];
            let condition = new dataShare.DataSharePredicates();
            condition.equalTo("raw_contact_id", rawContactId.toString());
            try {
                var resultSet = await dataShareHelper.query(searchContactUri, resultColumns, condition);
                if (resultSet.rowCount > 0) {
                    if (resultSet.goToFirstRow()) {
                        do {
                            var name = resultSet.getString(resultSet.getColumnIndex("search_name"));
                            console.info(
                                ' abnormal_contact_pinyinquery_test_9800 :logMessage contactsQuery name = ' + name);
                            expect(name == "李bp玉成욱||libpyucheng욱||lbpyc욱").assertEqual(true);
                        } while (resultSet.goToNextRow());
                    }
                }
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_pinyinquery_test_9800 query error = ' + error);
                done();
            }
        }
    });

    /**
     * @tc.number  abnormal_contact_query_test_9900
     * @tc.name    An incorrect field name was passed in during retrieval
     * @tc.desc    Function test
     */
    it("abnormal_contact_query_test_9900", 0, async function(done) {
        console.info("--------logMessage abnormal_contact_query_test_9900 is starting!-------");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

        var rawContactValues = {"display_name" : "键盘"};
        try {
            var rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
            sleep(sleep_one);
            console.info('logMessage abnormal_contact_query_test_9900 rawContactId = ' + rawContactId);
            expect(rawContactId > 0).assertTrue();
            var dataId = await insertData(rawContactId, "name", "键盘", "");
            console.info('logMessage abnormal_contact_query_test_9900 dataId = ' + dataId);
            expect(dataId > 0).assertTrue();
            await query();
            await deleteRawContact("abnormal_contact_query_test_9900");
            await deleteAll(contactDataUri, "abnormal_contact_query_test_9900")
            done();
        } catch (error) {
            console.info('logMessage abnormal_contact_query_test_9900 raw_contact insert error = ' + error);
            done();
        }

        async function query()
        {
            var resultColumns = [];
            let condition = new dataShare.DataSharePredicates();
            condition.like("search_namesss", "%jp%");
            try {
                var resultSet = await dataShareHelper.query(searchContactUri, resultColumns, condition);
                console.info(
                    ' abnormal_contact_query_test_9900 :resultSet.goToFirstRow() = ' + resultSet.goToFirstRow());
                expect(resultSet.goToFirstRow() == false).assertTrue();
                resultSet.close();
            } catch (error) {
                console.info('logMessage abnormal_contact_pinyinquery_test_9800 query error = ' + error);
                done();
            }
        }
    });

    afterAll(async function() {
        var tag = "Contacts_test_start_deleted";
        let condition = new dataShare.DataSharePredicates();
        condition.greaterThan("id", "0");
        let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
        console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
        var deleteBlockList = await dataShareHelper.delete(contactBlocklistUri, condition);
        console.info(tag + " : logMessage : deleteBlockList = " + deleteBlockList);
        var deletedGroup = await dataShareHelper.delete(groupUri, condition);
        console.info(tag + " : logMessage : deletedGroup = " + deletedGroup);
        var dataDeletedCode = await dataShareHelper.delete(contactDataUri, condition);
        sleep(sleep_two);
        console.info(tag + " : logMessage : dataDeletedCode = " + dataDeletedCode);
        var rawContactDeleted = await dataShareHelper.delete(rawContactUri, condition);
        sleep(sleep_two);
        console.info(tag + " : logMessage : rawContactDeleted = " + rawContactDeleted);
        var deleted = await dataShareHelper.delete(deletedUri, condition);
        sleep(sleep_two);
        console.info(tag + " : logMessage : deleted = " + deleted);
    });
});

function randomNum (num) {
    let number = toString(Math.floor(Math.random() * (9 * Math.pow(10, num))) + (1 * Math.pow(10, num)));
    return number ;
}
