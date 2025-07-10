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

import dataShare from '@ohos.data.dataShare';
import { afterAll, afterEach, beforeAll, beforeEach, describe, expect, it } from 'deccjsunit/index';

const URI_CALLLOG = 'datashare:///com.ohos.calllogability';
const calllogUri = 'datashare:///com.ohos.calllogability/calls/calllog';

const URI_VOICEMAIL = 'datashare:///com.ohos.voicemailability';
const voicemailUri = 'datashare:///com.ohos.voicemailability/calls/voicemail';

const URI_CONTACTS = 'datashare:///com.ohos.contactsdataability';
const rawContactUri = 'datashare:///com.ohos.contactsdataability/contacts/raw_contact';
const contactDataUri = 'datashare:///com.ohos.contactsdataability/contacts/contact_data';
const deletedUri = 'datashare:///com.ohos.contactsdataability/contacts/deleted_raw_contact';
const SLEEP_ONE_SECOND = 1000;
const SLEEP_TWO_SECOND = 2000;
const SLEEP_FIVE_SECOND = 5000;
const ROW_COUNT = 10000;

describe('PerformanceTest', function () {
  console.log(' PerformanceTest is start');
  function sleep(numberMillis) {
    let now = new Date();
    let exitTime = now.getTime() + numberMillis;
    while (true) {
      now = new Date();
      if (now.getTime() > exitTime) {
        return;
      }
    }
  }

  /**
   * @tc.number  raw_contact_insert_stability_test_900
   * @tc.name    Add 10000 pieces of data to the raw_contact table to see if they can be successfully inserted
   * @tc.desc    Function test
   */
  it('raw_contact_insert_stability_test_900', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let cycleIndex = 1000;
    let listAddBluk = [];
    for (let i = 0; i < cycleIndex; i++) {
      let add = { 'display_name': 'xiaoli' + i, 'company': 'testCompany' + i, 'position': 'testPosition' + i };
      listAddBluk[i] = add;
    }
    try {
      let batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(rawContactUri, listAddBluk);
      sleep(SLEEP_ONE_SECOND);
      console.info('logMessage raw_contact_insert_stability_test_900: contactDataId1 = ' + batchInsertCode);
      expect(batchInsertCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage raw_contact_insert_stability_test_900:  insert error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  raw_contact_update_stability_test_1000
   * @tc.name    The raw_contact table updates the data whose ID is not equal to 0 to see whether they can be updated
   * successfully
   * @tc.desc    Function test
   */
  it('raw_contact_update_stability_test_1000', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let updateValues = { 'favorite': 1 };
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    condition.and();
    condition.equalTo('is_deleted', '0');
    try {
      let updateCode = await dataShareHelper.update(rawContactUri, updateValues, condition);
      sleep(SLEEP_ONE_SECOND);
      console.info('logMessage raw_contact_update_stability_test_1000: updateCode = ' + updateCode);
      expect(updateCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage raw_contact_update_stability_test_1000: update error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  raw_contact_query_stability_test_1100
   * @tc.name    The raw_contact table queries 10000 pieces of data to see whether they can be queried successfully
   * @tc.desc    Function test
   */
  it('raw_contact_query_stability_test_1100', 0, async function (done) {
    let tag = 'raw_contact_query_stability_test_1100';
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
    let resultColumns = ['id'];
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    condition.and();
    condition.equalTo('is_deleted', '0');
    try {
      let resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
      sleep(SLEEP_ONE_SECOND);
      console.info(tag + ' : logMessage : rowCount' + resultSet.rowCount);
      expect(resultSet.rowCount === ROW_COUNT).assertTrue();
      resultSet.close();
      done();
    } catch (error) {
      console.info(tag + ' :logMessage : error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  raw_contact_delete_stability_test_1200
   * @tc.name    Delete 10000 pieces of data in raw_contact table to see if they can be deleted successfully
   * @tc.desc    Function test
   */
  it('raw_contact_delete_stability_test_1200', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('raw_contact_delete_stability_test_1200 : start ! dataShareHelper = ' + dataShareHelper);
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    condition.and();
    condition.equalTo('is_deleted', '0');
    try {
      let deleteCode = await dataShareHelper.delete(rawContactUri, condition);
      sleep(SLEEP_ONE_SECOND);
      console.info('logMessage raw_contact_delete_stability_test_1200: deleteCode = ' + deleteCode);
      expect(deleteCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage raw_contact_delete_stability_test_1200: delete error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  contact_data_insert_stability_test_1300
   * @tc.name    Add 10000 pieces of data to the contact_data table to see if they can be successfully inserted
   * @tc.desc    Function test
   */
  it('contact_data_insert_stability_test_1300', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

    let rawContactValues = {
      'display_name': 'xiaoli'
    };
    try {
      let rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
      sleep(SLEEP_ONE_SECOND);
      console.info('logMessage contact_data_insert_stability_test_1300: rawContactId = ' + rawContactId);
    } catch (error) {
      console.info('logMessage contact_data_insert_stability_test_1300: raw_contact insert error = ' + error);
    }

    let condition = new dataShare.DataSharePredicates();
    condition.equalTo('id', rawContactId.toString());
    let deleteCode = await dataShareHelper.delete(rawContactUri, condition);
    sleep(SLEEP_ONE_SECOND);
    console.info('logMessage contact_data_insert_stability_test_1300: deleteCode = ' + deleteCode);
    let cycleIndex = 1000;
    let listAddBluk = [];
    for (let i = 0; i < cycleIndex; i++) {
      let add = { 'raw_contact_id': rawContactId, 'detail_info': 'xxx' + i, 'content_type': 'name' };
      listAddBluk[i] = add;
    }
    try {
      let batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(contactDataUri, listAddBluk);
      sleep(SLEEP_TWO_SECOND);
      console.info('logMessage contact_data_insert_stability_test_1300: batchInsertCode = ' + batchInsertCode);
      expect(batchInsertCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage contact_data_insert_stability_test_1300:  insert error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  contact_data_update_stability_test_1400
   * @tc.name    The contact_data table updates the data whose ID is not equal to 0 to see whether they can be updated
   * successfully
   * @tc.desc    Function test
   */
  it('contact_data_update_stability_test_1400', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info(
      'logMessage contact_data_update_stability_test_1400 dataShareHelper success! dataShareHelper = ' + dataShareHelper
    );
    let updateValues = { 'syn_1': 'test' };
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    try {
      let updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
      sleep(SLEEP_TWO_SECOND);
      console.info('logMessage contact_data_update_stability_test_1400: updateCode = ' + updateCode);
      expect(updateCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage contact_data_update_stability_test_1400: update error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  contact_data_query_stability_test_1500
   * @tc.name    The contact_data table queries 10000 pieces of data to see whether they can be queried successfully
   * @tc.desc    Function test
   */
  it('contact_data_query_stability_test_1500', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('contact_data_query_stability_test_1500 start ! dataShareHelper = ' + dataShareHelper);
    let resultColumns = ['id'];
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    try {
      let resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
      sleep(SLEEP_TWO_SECOND);
      console.info(' contact_data_query_stability_test_1500 :  resultSet.rowCount = ' + resultSet.rowCount);
      expect(resultSet.rowCount === ROW_COUNT).assertEqual(true);
      resultSet.close();
      done();
    } catch (error) {
      console.info('logMessage contact_data_query_stability_test_1500: error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  contact_data_delete_stability_test_1600
   * @tc.name    Delete 10000 pieces of data in contact_data table to see if they can be deleted successfully
   * @tc.desc    Function test
   */
  it('contact_data_delete_stability_test_1600', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('contact_data_delete_stability_test_1600 : start ! dataShareHelper = ' + dataShareHelper);
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    try {
      let deleteCode = await dataShareHelper.delete(contactDataUri, condition);
      sleep(SLEEP_TWO_SECOND);
      console.info('logMessage contact_data_delete_stability_test_1600: deleteCode = ' + deleteCode);
      expect(deleteCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage contact_data_delete_stability_test_1600: delete error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  calllog_insert_stability_test_100
   * @tc.name    Add 10000 pieces of data to the callog table to see if they can be successfully inserted
   * @tc.desc    Function test
   */
  it('calllog_insert_stability_test_100', 0, async function (done) {
    console.info('---------logMessage calllog_insert_stability_test_100 is starting!----------');
    let dataShareHelper = dataShare.createDataShareHelper(URI_CALLLOG);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let phoneNumberLenSix = 6;
    let cycleIndex = 1000;
    let listAddBluk = [];
    let phoneNumber = randomNum(phoneNumberLenSix);
    for (let i = 0; i < cycleIndex; i++) {
      let add = { 'phone_number': phoneNumber + i };
      listAddBluk[i] = add;
    }
    try {
      let batchInsertCode = await dataShareHelper.batchInsert(calllogUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(calllogUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(calllogUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(calllogUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(calllogUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(calllogUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(calllogUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(calllogUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(calllogUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(calllogUri, listAddBluk);
      sleep(SLEEP_ONE_SECOND);
      console.info('logMessage calllog_insert_stability_test_100: batchInsertCode = ' + batchInsertCode);
      expect(batchInsertCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage calllog_insert_stability_test_100: batchInsert error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  calllog_update_stability_test_200
   * @tc.name    The callog table updates the data whose ID is not equal to 0 to see whether they can be updated
   * successfully
   * @tc.desc    Function test
   */
  it('calllog_update_stability_test_200', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CALLLOG);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let updateValues = { 'answer_state': '1' };
    try {
      let condition = new dataShare.DataSharePredicates();
      condition.greaterThan('id', '0');
      let updateCode = await dataShareHelper.update(calllogUri, updateValues, condition);
      sleep(SLEEP_ONE_SECOND);
      console.info('logMessage calllog_update_stability_test_200: updateCode = ' + updateCode);
      expect(updateCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage calllog_update_stability_test_200: update error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  calllog_query_stability_test_300
   * @tc.name    The callog table queries 10000 pieces of data to see whether they can be queried successfully
   * @tc.desc    Function test
   */
  it('calllog_query_stability_test_300', 0, async function (done) {
    let tag = 'calllog_query_stability_test_300';
    let dataShareHelper = dataShare.createDataShareHelper(URI_CALLLOG);
    console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
    let resultColumns = ['id'];
    let condition = new dataShare.DataSharePredicates();
    condition.notEqualTo('id', 0);

    try {
      let resultSet = await dataShareHelper.query(calllogUri, resultColumns, condition);
      sleep(SLEEP_FIVE_SECOND);
      console.info(tag + ' : logMessage : rowCount' + resultSet.rowCount);
      expect(resultSet.rowCount === ROW_COUNT).assertTrue();
      resultSet.close();
      done();
    } catch (error) {
      console.info('logMessage calllog_query_stability_test_300: error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  calllog_delete_stability_test_400
   * @tc.name    Delete 10000 pieces of data in callog table to see if they can be deleted successfully
   * @tc.desc    Function test
   */
  it('calllog_delete_stability_test_400', 0, async function (done) {
    let tag = 'calllog_delete_stability_test_400';
    let dataShareHelper = dataShare.createDataShareHelper(URI_CALLLOG);
    console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    let deleteCode = await dataShareHelper.delete(calllogUri, condition);
    sleep(SLEEP_FIVE_SECOND);
    console.info(tag + ' : logMessage : deleteCode = ' + deleteCode);
    expect(deleteCode === 0).assertTrue();
    done();
  });

  /**
   * @tc.number  voicemail_insert_stability_test_500
   * @tc.name    Add 10000 pieces of data to the voicemail table to see if they can be successfully inserted
   * @tc.desc    Function test
   */
  it('voicemail_insert_stability_test_500', 0, async function (done) {
    console.info('---------logMessage voicemail_insert_stability_test_500 is starting!----------');
    let dataShareHelper = dataShare.createDataShareHelper(URI_VOICEMAIL);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let cycleIndex = 1000;
    let phoneNumberLenFour = 4;
    let listAddBluk = [];
    let phoneNumber = randomNum(phoneNumberLenFour);
    for (let i = 0; i < cycleIndex; i++) {
      let add = { 'phone_number': phoneNumber + i };
      listAddBluk[i] = add;
    }
    try {
      let batchInsertCode = await dataShareHelper.batchInsert(voicemailUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(voicemailUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(voicemailUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(voicemailUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(voicemailUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(voicemailUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(voicemailUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(voicemailUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(voicemailUri, listAddBluk);
      batchInsertCode = await dataShareHelper.batchInsert(voicemailUri, listAddBluk);
      sleep(SLEEP_ONE_SECOND);
      console.info('logMessage voicemail_insert_stability_test_500: batchInsertCode = ' + batchInsertCode);
      expect(batchInsertCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage voicemail_insert_stability_test_500: batchInsert error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  voicemail_update_stability_test_600
   * @tc.name    The voicemail table updates the data whose ID is not equal to 0 to see whether they can be updated
   * successfully
   * @tc.desc    Function test
   */
  it('voicemail_update_stability_test_600', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_VOICEMAIL);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let updateValues = { 'origin_type': 'test' };
    try {
      let condition = new dataShare.DataSharePredicates();
      condition.notEqualTo('id', 0);
      let updateCode = await dataShareHelper.update(voicemailUri, updateValues, condition);
      sleep(SLEEP_ONE_SECOND);
      console.info('logMessage voicemail_update_stability_test_600: updateCode = ' + updateCode);
      expect(updateCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage voicemail_update_stability_test_600: update error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  voicemail_query_stability_test_700
   * @tc.name    The voicemail table queries 10000 pieces of data to see whether they can be queried successfully
   * @tc.desc    Function test
   */
  it('voicemail_query_stability_test_700', 0, async function (done) {
    let tag = 'voicemail_query_stability_test_700';
    let dataShareHelper = dataShare.createDataShareHelper(URI_VOICEMAIL);
    console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
    let resultColumns = ['id'];
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    try {
      let resultSet = await dataShareHelper.query(voicemailUri, resultColumns, condition);
      sleep(SLEEP_ONE_SECOND);
      console.info(tag + ' : resultSet.rowCount = ' + resultSet.rowCount);
      expect(resultSet.rowCount === ROW_COUNT).assertEqual(true);
      resultSet.close();
      done();
    } catch (error) {
      console.info('logMessage voicemail_query_stability_test_700: error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  voicemail_delete_stability_test_800
   * @tc.name    Delete 10000 pieces of data in voicemail table to see if they can be deleted successfully
   * @tc.desc    Function test
   */
  it('voicemail_delete_stability_test_800', 0, async function (done) {
    let tag = 'voicemail_delete_stability_test_800';
    let dataShareHelper = dataShare.createDataShareHelper(URI_VOICEMAIL);
    console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    try {
      let deleteCode = await dataShareHelper.delete(voicemailUri, condition);
      sleep(SLEEP_TWO_SECOND);
      console.info(tag + ' : logMessage : deleteCode = ' + deleteCode);
      expect(deleteCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage voicemail_delete_stability_test_800: error = ' + error);
      done();
    }
  });

  afterAll(async function () {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('Stability : start ! dataShareHelper = ' + dataShareHelper);
    let condition = new dataShare.DataSharePredicates();
    condition.notEqualTo('id', '0');
    try {
      let resultColumns = ['id'];
      let conditionDelete = new dataShare.DataSharePredicates();
      conditionDelete.greaterThan('id', '0');
      let count = 0;
      let deleteCount = 9999;
      while (count < deleteCount) {
        let result = await dataShareHelper.query(deletedUri, resultColumns, conditionDelete);
        console.info('Stability : result.rowCount = ' + result.rowCount);
        count = result.rowCount;
        result.close();
        sleep(SLEEP_FIVE_SECOND);
      }
      let deleteCode = await dataShareHelper.delete(deletedUri, condition);
      console.info('Stability afterAll logMessage DeleteContact: deleteCode = ' + deleteCode);
    } catch (error) {
      console.info('Stability afterAll logMessage DeleteContact: delete error = ' + error);
    }
  });
});

function randomNum(num) {
  let baseNumNine = 9;
  let baseNumTen = 10;
  let number = toString(Math.floor(Math.random() * (baseNumNine * Math.pow(baseNumTen, num))) + 1 *
    Math.pow(baseNumTen, num));
  return number;
}
