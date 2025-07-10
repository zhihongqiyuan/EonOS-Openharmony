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
const PHONE_NUM_LEN_NINE = 9;
const PHONE_NUM_LEN_TEN = 10;
const CYCLE_INDEX = 1000;
const USED_TIME_70000 = 70000;
const USED_TIME_90000 = 90000;
const SLEEP_TIME = 5000;
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
   * @tc.number  raw_contact_insert_performance_test_900
   * @tc.name    The raw_contact table adds 10000 pieces of data, and the time consumption is calculated according to
   * the running time of the use case
   * @tc.desc    Function test
   */
  it('raw_contact_insert_performance_test_900', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let listAddBluk = [];
    for (let i = 0; i < CYCLE_INDEX; i++) {
      let add = { 'display_name': 'xiaoli' + i, 'company': 'testCompany' + i, 'position': 'testPosition' + i };
      listAddBluk[i] = add;
    }
    try {
      let old = new Date();
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
      let now = new Date();
      let usedTime = now - old;
      console.info('raw_contact_insert_performance_test_900  usedTime = ' + usedTime);
      expect(usedTime < USED_TIME_70000).assertTrue();
      sleep(SLEEP_TIME);
      console.info('logMessage raw_contact_insert_performance_test_900: batchInsertCode = ' + batchInsertCode);
      expect(batchInsertCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage raw_contact_insert_performance_test_900:  insert error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  raw_contact_update_performance_test_1000
   * @tc.name    The raw_contact table updates data whose ID is not equal to 0, and the time consumption is calculated
   * according to the running time of the use case
   * @tc.desc    Function test
   */
  it('raw_contact_update_performance_test_1000', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let updateValues = { 'favorite': 1 };
    let usedTime2000 = 2000;
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    condition.and();
    condition.equalTo('is_deleted', '0');
    try {
      let old = new Date();
      let updateCode = await dataShareHelper.update(rawContactUri, updateValues, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('raw_contact_update_performance_test_1000  usedTime = ' + usedTime);
      expect(usedTime < usedTime2000).assertTrue();
      sleep(SLEEP_TIME);
      console.info('logMessage raw_contact_update_performance_test_1000: updateCode = ' + updateCode);
      expect(updateCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage raw_contact_update_performance_test_1000: update error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  raw_contact_query_performance_test_1100
   * @tc.name    The raw_ contact table queries 10000 pieces of data and calculates the time consumption according to
   * the running time of the use case
   * @tc.desc    Function test
   */
  it('raw_contact_query_performance_test_1100', 0, async function (done) {
    let tag = 'raw_contact_query_performance_test_1100';
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
    let usedTime200 = 200;
    let resultColumns = [];
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    condition.and();
    condition.equalTo('is_deleted', '0');
    try {
      let old = new Date();
      let resultSet = await dataShareHelper.query(rawContactUri, resultColumns, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('raw_contact_query_performance_test_1100  usedTime = ' + usedTime);
      expect(usedTime < usedTime200).assertTrue();
      sleep(SLEEP_TIME);
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
   * @tc.number  raw_contact_delete_performance_test_1200
   * @tc.name    The raw_contact table deletes 10000 pieces of data, and the time consumption is calculated according
   * to the running time of the use case
   * @tc.desc    Function test
   */
  it('raw_contact_delete_performance_test_1200', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('raw_contact_delete_performance_test_1200 : start ! dataShareHelper = ' + dataShareHelper);
    let usedTime7000 = 7000;
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    condition.and();
    condition.equalTo('is_deleted', '0');
    try {
      let old = new Date();
      let deleteCode = await dataShareHelper.delete(rawContactUri, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('raw_contact_delete_performance_test_1200  usedTime = ' + usedTime);
      expect(usedTime < usedTime7000).assertTrue();
      sleep(SLEEP_TIME);
      console.info('logMessage raw_contact_delete_performance_test_1200: deleteCode = ' + deleteCode);
      expect(deleteCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage raw_contact_delete_performance_test_1200: delete error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  contact_data_insert_performance_test_1300
   * @tc.name    The contact_data table adds 10000 pieces of data, and the time consumption is calculated according to
   * the running time of the use case
   * @tc.desc    Function test
   */
  it('contact_data_insert_performance_test_1300', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);

    let rawContactValues = {
      'display_name': 'xiaoli'
    };
    try {
      let rawContactId = await dataShareHelper.insert(rawContactUri, rawContactValues);
      let sleepTime = 1000;
      sleep(sleepTime);
      console.info('logMessage contact_data_insert_performance_test_1300: rawContactId = ' + rawContactId);
    } catch (error) {
      console.info('logMessage contact_data_insert_performance_test_1300: raw_contact insert error = ' + error);
    }

    let condition = new dataShare.DataSharePredicates();
    condition.equalTo('id', rawContactId.toString());
    let deleteCode = await dataShareHelper.delete(rawContactUri, condition);
    let sleepTime = 1000;
    sleep(sleepTime);
    console.info('logMessage contact_data_insert_performance_test_1300: deleteCode = ' + deleteCode);

    let listAddBluk = [];
    for (let i = 0; i < CYCLE_INDEX; i++) {
      let add = { 'raw_contact_id': rawContactId, 'detail_info': 'xxx' + i, 'content_type': 'name' };
      listAddBluk[i] = add;
    }
    try {
      let old = new Date();
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
      let now = new Date();
      let usedTime = now - old;
      console.info('contact_data_insert_performance_test_1300  usedTime = ' + usedTime);
      expect(usedTime < USED_TIME_70000).assertTrue();
      sleep(SLEEP_TIME);
      console.info('logMessage contact_data_insert_performance_test_1300: batchInsertCode = ' + batchInsertCode);
      expect(batchInsertCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage contact_data_insert_performance_test_1300:  insert error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  contact_data_update_performance_test_1400
   * @tc.name    The contact_data table updates data whose ID is not equal to 0, and the time consumption is
   * calculated according to the running time of the use case
   * @tc.desc    Function test
   */
  it('contact_data_update_performance_test_1400', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info(
      'logMessage contact_data_update_performance_test_1400 dataShareHelper success! dataShareHelper = ' +
        dataShareHelper
    );
    let sleepTime8000 = 8000;
    let usedTime35000 = 35000;
    let updateValues = { 'syn_1': 'test' };
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    try {
      let old = new Date();
      let updateCode = await dataShareHelper.update(contactDataUri, updateValues, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('contact_data_update_performance_test_1400  usedTime = ' + usedTime);
      expect(usedTime < usedTime35000).assertTrue();
      sleep(sleepTime8000);
      console.info('logMessage contact_data_update_performance_test_1400: updateCode = ' + updateCode);
      expect(updateCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage contact_data_update_performance_test_1400: update error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  contact_data_query_performance_test_1500
   * @tc.name    The contact_datat table queries 10000 pieces of data and calculates the time consumption according to
   * the running time of the use case
   * @tc.desc    Function test
   */
  it('contact_data_query_performance_test_1500', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('contact_data_query_performance_test_1500 start ! dataShareHelper = ' + dataShareHelper);
    let usedTime220 = 220;
    let resultColumns = [];
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    try {
      let old = new Date();
      let resultSet = await dataShareHelper.query(contactDataUri, resultColumns, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('contact_data_query_performance_test_1500  usedTime = ' + usedTime);
      expect(usedTime < usedTime220).assertTrue();
      sleep(SLEEP_TIME);
      console.info('logMessage contact_data_query_performance_test_1500: goToFirstRow' + resultSet.rowCount);
      expect(resultSet.rowCount === ROW_COUNT).assertEqual(true);
      resultSet.close();
      done();
    } catch (error) {
      console.info('logMessage contact_data_query_performance_test_1500: error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  contact_data_delete_performance_test_1600
   * @tc.name    The contact_data table deletes 10000 pieces of data, and the time consumption is calculated according
   * to the running time of the use case
   * @tc.desc    Function test
   */
  it('contact_data_delete_performance_test_1600', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('contact_data_delete_performance_test_1600 : start ! dataShareHelper = ' + dataShareHelper);
    let usedTime32000 = 32000;
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    try {
      let old = new Date();
      let deleteCode = await dataShareHelper.delete(contactDataUri, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('contact_data_delete_performance_test_1600  usedTime = ' + usedTime);
      expect(usedTime < usedTime32000).assertTrue();
      sleep(SLEEP_TIME);
      console.info('logMessage contact_data_delete_performance_test_1600: deleteCode = ' + deleteCode);
      expect(deleteCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage contact_data_delete_performance_test_1600: delete error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  calllog_insert_performance_test_100
   * @tc.name    The callog table adds 10000 pieces of data, and the time consumption is calculated according to the
   * running time of the use case
   * @tc.desc    Function test
   */
  it('calllog_insert_performance_test_100', 0, async function (done) {
    console.info('---------logMessage calllog_insert_performance_test_100 is starting!----------');
    let dataShareHelper = dataShare.createDataShareHelper(URI_CALLLOG);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let phoneNumberLenSix = 6;
    let listAddBluk = [];
    let phoneNumber = randomNum(phoneNumberLenSix);
    for (let i = 0; i < CYCLE_INDEX; i++) {
      let add = { 'phone_number': phoneNumber + i };
      listAddBluk[i] = add;
    }
    try {
      let old = new Date();
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
      let now = new Date();
      let usedTime = now - old;
      console.info('calllog_insert_performance_test_100  usedTime = ' + usedTime);
      expect(usedTime < USED_TIME_90000).assertTrue();
      sleep(SLEEP_TIME);
      console.info('logMessage calllog_insert_performance_test_100: batchInsertCode = ' + batchInsertCode);
      expect(batchInsertCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage calllog_insert_performance_test_100: batchInsert error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  calllog_update_performance_test_200
   * @tc.name    The callog table updates data whose ID is not equal to 0, and the time consumption is calculated
   * according to the running time of the use case
   * @tc.desc    Function test
   */
  it('calllog_update_performance_test_200', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CALLLOG);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let updateValues = { 'answer_state': '1' };
    let usedTime300 = 300;
    try {
      let condition = new dataShare.DataSharePredicates();
      condition.notEqualTo('id', 0);
      let old = new Date();
      let updateCode = await dataShareHelper.update(calllogUri, updateValues, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('calllog_update_performance_test_200  usedTime = ' + usedTime);
      expect(usedTime < usedTime300).assertTrue();
      sleep(SLEEP_TIME);
      console.info('logMessage calllog_update_performance_test_200: updateCode = ' + updateCode);
      expect(updateCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage calllog_update_performance_test_200: update error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  calllog_query_performance_test_300
   * @tc.name    The callog table queries 10000 pieces of data to see whether they can be queried successfully
   * @tc.desc    Function test
   */
  it('calllog_query_performance_test_300', 0, async function (done) {
    let tag = 'calllog_query_performance_test_300';
    let dataShareHelper = dataShare.createDataShareHelper(URI_CALLLOG);
    console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
    let usedTime30 = 30;
    let resultColumns = [];
    let condition = new dataShare.DataSharePredicates();
    condition.notEqualTo('id', 0);

    try {
      let old = new Date();
      let resultSet = await dataShareHelper.query(calllogUri, resultColumns, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('calllog_query_performance_test_300  usedTime = ' + usedTime);
      expect(usedTime < usedTime30).assertTrue();
      sleep(SLEEP_TIME);
      console.info(tag + ' : logMessage : rowCount' + resultSet.rowCount);
      expect(resultSet.rowCount === ROW_COUNT).assertTrue();
      resultSet.close();
      done();
    } catch (error) {
      console.info('logMessage calllog_query_performance_test_300: error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  calllog_delete_performance_test_400
   * @tc.name    The callog table deletes 10000 pieces of data, and the time consumption is calculated according to
   * the running time of the use case
   * @tc.desc    Function test
   */
  it('calllog_delete_performance_test_400', 0, async function (done) {
    let tag = 'calllog_delete_performance_test_400';
    let dataShareHelper = dataShare.createDataShareHelper(URI_CALLLOG);
    console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
    let usedTime200 = 200;
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    let old = new Date();
    let deleteCode = await dataShareHelper.delete(calllogUri, condition);
    let now = new Date();
    let usedTime = now - old;
    console.info('calllog_delete_performance_test_400  usedTime = ' + usedTime);
    expect(usedTime < usedTime200).assertTrue();
    sleep(SLEEP_TIME);
    console.info(tag + ' : logMessage : deleteCode = ' + deleteCode);
    expect(deleteCode === 0).assertTrue();
    done();
  });

  /**
   * @tc.number  voicemail_insert_performance_test_500
   * @tc.name    The voicemail table adds 10000 pieces of data, and the time consumption is calculated according to
   * the running time of the use case
   * @tc.desc    Function test
   */
  it('voicemail_insert_performance_test_500', 0, async function (done) {
    console.info('---------logMessage voicemail_insert_performance_test_500 is starting!----------');
    let dataShareHelper = dataShare.createDataShareHelper(URI_VOICEMAIL);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let phoneNumberLenFour = 4;
    let listAddBluk = [];
    let phoneNumber = randomNum(phoneNumberLenFour);
    for (let i = 0; i < CYCLE_INDEX; i++) {
      let add = { 'phone_number': phoneNumber + i };
      listAddBluk[i] = add;
    }
    try {
      let old = new Date();
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
      let now = new Date();
      let usedTime = now - old;
      console.info('voicemail_insert_performance_test_500  usedTime = ' + usedTime);
      expect(usedTime < USED_TIME_90000).assertTrue();
      sleep(SLEEP_TIME);
      console.info('logMessage voicemail_insert_performance_test_500: batchInsertCode = ' + batchInsertCode);
      expect(batchInsertCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage voicemail_insert_performance_test_500: batchInsert error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  voicemail_update_performance_test_600
   * @tc.name    The voicemail table updates data whose ID is not equal to 0, and the time consumption is calculated
   * according to the running time of the use case
   * @tc.desc    Function test
   */
  it('voicemail_update_performance_test_600', 0, async function (done) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_VOICEMAIL);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let updateValues = { 'origin_type': 'test' };
    let usedTime250 = 250;
    try {
      let condition = new dataShare.DataSharePredicates();
      condition.notEqualTo('id', 0);
      let old = new Date();
      let updateCode = await dataShareHelper.update(voicemailUri, updateValues, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('voicemail_update_performance_test_600  usedTime = ' + usedTime);
      expect(usedTime < usedTime250).assertTrue();
      sleep(SLEEP_TIME);
      console.info('logMessage voicemail_update_performance_test_600: updateCode = ' + updateCode);
      expect(updateCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage voicemail_update_performance_test_600: update error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  voicemail_query_performance_test_700
   * @tc.name    The voicemail table queries 10000 items, and the time consumption is calculated according to the
   * running time of the use case
   * @tc.desc    Function test
   */
  it('voicemail_query_performance_test_700', 0, async function (done) {
    let tag = 'voicemail_query_performance_test_700';
    let dataShareHelper = dataShare.createDataShareHelper(URI_VOICEMAIL);
    console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
    let resultColumns = [];
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    let usedTime20 = 20;
    try {
      let old = new Date();
      let resultSet = await dataShareHelper.query(voicemailUri, resultColumns, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('voicemail_query_performance_test_700  usedTime = ' + usedTime);
      expect(usedTime < usedTime20).assertTrue();
      sleep(SLEEP_TIME);
      console.info(tag + ' : logMessage : rowCount' + resultSet.rowCount);
      expect(resultSet.rowCount === ROW_COUNT).assertTrue();
      resultSet.close();
      done();
    } catch (error) {
      console.info('logMessage voicemail_query_performance_test_700: error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  voicemail_delete_performance_test_800
   * @tc.name    The voicemail table deletes 10000 pieces of data, and the time consumption is calculated according to
   * the running time of the use case
   * @tc.desc    Function test
   */
  it('voicemail_delete_performance_test_800', 0, async function (done) {
    let tag = 'voicemail_delete_performance_test_800';
    let dataShareHelper = dataShare.createDataShareHelper(URI_VOICEMAIL);
    console.info(tag + ': start ! dataShareHelper = ' + dataShareHelper);
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    let usedTime100 = 100;
    try {
      let old = new Date();
      let deleteCode = await dataShareHelper.delete(voicemailUri, condition);
      let now = new Date();
      let usedTime = now - old;
      console.info('voicemail_delete_performance_test_800  usedTime = ' + usedTime);
      expect(usedTime < usedTime100).assertTrue();
      sleep(SLEEP_TIME);
      console.info(tag + ' : logMessage : deleteCode = ' + deleteCode);
      expect(deleteCode === 0).assertTrue();
      done();
    } catch (error) {
      console.info('logMessage voicemail_delete_performance_test_800: error = ' + error);
      done();
    }
  });

  afterAll(async function () {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('DeleteContact : start ! dataShareHelper = ' + dataShareHelper);
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
        console.info('performance : result.rowCount = ' + result.rowCount);
        count = result.rowCount;
        result.close();
        sleep(SLEEP_TIME);
      }
      let deleteCode = await dataShareHelper.delete(deletedUri, condition);
      console.info('afterAll logMessage DeleteContact: deleteCode = ' + deleteCode);
    } catch (error) {
      console.info('afterAll logMessage DeleteContact: delete error = ' + error);
    }
  });
});

function randomNum(num) {
  let number = toString(Math.floor(Math.random() * (PHONE_NUM_LEN_NINE * Math.pow(PHONE_NUM_LEN_TEN, num))) + 1 *
    Math.pow(PHONE_NUM_LEN_TEN, num));
  return number;
}
