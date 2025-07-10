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

const URI_CONTACTS = 'datashare:///com.ohos.contactsdataability';
const rawContactUri = 'datashare:///com.ohos.contactsdataability/contacts/raw_contact';
const backup = 'datashare:///com.ohos.contactsdataability/contacts/backup';
const recover = 'datashare:///com.ohos.contactsdataability/contacts/recover';
const deletedUri = 'datashare:///com.ohos.contactsdataability/contacts/deleted_raw_contact';
const SLEEP_ONE_SECOND = 1000;
const SLEEP_TWO_SECOND = 2000;
const SLEEP_THREE_SECOND = 3000;
const SLEEP_FIVE_SECOND = 5000;

describe('RecoveryTest', function () {
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

  async function contactsQuery(tag, uri, size) {
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info(tag + ': contactsQuery start ! dataShareHelper = ' + dataShareHelper);
    let resultColumns = [];
    let condition = new dataShare.DataSharePredicates();
    condition.greaterThan('id', '0');
    try {
      let resultSet = await dataShareHelper.query(uri, resultColumns, condition);
      console.info(tag + ' contactsQuery resultSet.rowCount is = ' + resultSet.rowCount);
      console.info(tag + ' contactsQuery size is = ' + size);
      expect(resultSet.rowCount === size).assertEqual(true);
      if (resultSet.rowCount > 0) {
        if (resultSet.goToFirstRow()) {
          do {
            let id = resultSet.getString(resultSet.getColumnIndex('id'));
            console.info(tag + ':logMessage contactsQuery id = ' + id);
            let name = resultSet.getString(resultSet.getColumnIndex('display_name'));
            console.info(tag + ':logMessage contactsQuery name = ' + name);
          } while (resultSet.goToNextRow());
        }
      }
      resultSet.close();
    } catch (error) {
      console.info(tag + 'logMessage contactsQuery: error = ' + error);
    }
  }

  async function rawContactInsert(name) {
    console.info('------logMessage rawContactInsert is starting!-----');
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    console.info('logMessage get dataShareHelper success! dataShareHelper = ' + dataShareHelper);
    let insertValue = {
      'display_name': name
    };
    try {
      let rawContactId = await dataShareHelper.insert(rawContactUri, insertValue);
      console.info('logMessage rawContactInsert: rawContactId = ' + rawContactId);
      expect(rawContactId > 0).assertTrue();
    } catch (error) {
      console.info('logMessage rawContactInsert: insert error = ' + error);
    }
    return rawContactId;
  }

  async function deleteRawContact(tag) {
    try {
      let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
      let condition = new dataShare.DataSharePredicates();
      condition.greaterThan('id', '0');
      condition.and();
      condition.equalTo('is_deleted', '0');
      await dataShareHelper.delete(rawContactUri, condition);
      sleep(SLEEP_FIVE_SECOND);
      let conditionAll = new dataShare.DataSharePredicates();
      conditionAll.greaterThan('id', '0');
      await dataShareHelper.delete(deletedUri, conditionAll);
    } catch (error) {
      console.info(tag + ': deleteRawContact error = ' + error);
    }
  }

  /**
   * @tc.number  recovery_test_100
   * @tc.name    backup database
   * @tc.desc    Function test
   */
  it('recovery_test_100', 0, async function (done) {
    await deleteRawContact('recovery_test_100');
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    let updateValues = {};
    let condition = new dataShare.DataSharePredicates();
    try {
      let updateCode = await dataShareHelper.update(backup, updateValues, condition);
      sleep(SLEEP_TWO_SECOND);
      console.info('logMessage recovery_test_100: updateCode = ' + updateCode);
      expect(updateCode === 0).assertTrue();
      sleep(SLEEP_TWO_SECOND);
      await contactsQuery('recovery_test_100', rawContactUri, 0);
      done();
    } catch (error) {
      console.info('logMessage recovery_test_100: raw_contact update error = ' + error);
      done();
    }
  });

  /**
   * @tc.number  recovery_test_200
   * @tc.n    ame    recovery database
   * @tc.desc    Function test
   */
  it('recovery_test_200', 0, async function (done) {
    await deleteRawContact('recovery_test_200');
    let dataShareHelper = dataShare.createDataShareHelper(URI_CONTACTS);
    let updateValues = {};
    let condition = new dataShare.DataSharePredicates();
    rawContactInsert('liming');
    rawContactInsert('xiaolilili');
    sleep(SLEEP_TWO_SECOND);
    let updateCode = await dataShareHelper.update(backup, updateValues, condition);
    sleep(SLEEP_TWO_SECOND);
    console.info(' recovery_test_200: backup = ' + updateCode);
    expect(updateCode === 0).assertTrue();
    sleep(SLEEP_ONE_SECOND);
    try {
      let updateCode = await dataShareHelper.update(recover, updateValues, condition);
      let size = 2;
      sleep(SLEEP_THREE_SECOND);
      console.info('logMessage recovery_test_200: recover = ' + updateCode);
      expect(updateCode === 0).assertTrue();
      await contactsQuery('recovery_test_200', rawContactUri, size);
      await deleteRawContact('recovery_test_200');
      done();
    } catch (error) {
      console.info('logMessage recovery_test_200: raw_contact update error = ' + error);
      done();
    }
  });
});
