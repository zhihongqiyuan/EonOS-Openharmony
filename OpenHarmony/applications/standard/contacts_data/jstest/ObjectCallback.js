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

import contactsapi from '@ohos.contact';
import { afterAll, afterEach, beforeAll, beforeEach, describe, expect, it } from 'deccjsunit/index';

const URI_CONTACTS = 'datashare:///com.ohos.contactsdataability';
const rawContactUri = 'datashare:///com.ohos.contactsdataability/contacts/raw_contact';
const contactDataUri = 'datashare:///com.ohos.contactsdataability/contacts/contact_data';
const groupUri = 'datashare:///com.ohos.contactsdataability/contacts/groups';
const deletedUri = 'datashare:///com.ohos.contactsdataability/contacts/deleted_raw_contact';
const ATTRIBUTE_ONE = 1;
const ATTRIBUTE_TWO = 2;
const ATTRIBUTE_THREE = 3;
const ATTRIBUTE_FOUR = 4;
const ATTRIBUTE_FIVE = 5;
const ATTRIBUTE_SIX = 6;
const ATTRIBUTE_SEVEN = 7;
const ATTRIBUTE_EIGHT = 8;
const ATTRIBUTE_NINE = 9;
const ATTRIBUTE_TEN = 10;
const ATTRIBUTE_ELEVEN = 11;
const ATTRIBUTE_TWELVE = 12;
const ATTRIBUTE_THIRTEEN = 13;
const ATTRIBUTE_FOURTEEN = 14;
const ATTRIBUTE_ONE_HUNDERD = 100;

describe('ObjectInterfaceTest', function () {
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

  let contactData = {
    id: 0,
    key: '0',
    contactAttributes: { attributes: [ATTRIBUTE_ONE, ATTRIBUTE_TWO, ATTRIBUTE_THREE, ATTRIBUTE_FOUR, ATTRIBUTE_FIVE,
      ATTRIBUTE_SIX, ATTRIBUTE_SEVEN, ATTRIBUTE_EIGHT, ATTRIBUTE_NINE, ATTRIBUTE_TEN, ATTRIBUTE_ELEVEN,
      ATTRIBUTE_TWELVE, ATTRIBUTE_THIRTEEN, ATTRIBUTE_FOURTEEN] },
    emails: [{ email: 'email', labelName: '自定义邮箱', labelId: 1, displayName: 'emailDisplayName' }],
    events: [{ eventDate: 'event', labelName: '自定义event', labelId: 2 }],
    groups: [{ groupId: 1, title: '群组' }],
    imAddresses: [{ imAddress: 'imAddress', labelName: '自定义', labelId: 3 }],
    phoneNumbers: [{ phoneNumber: '183', labelName: '自定义phoneNumbers', labelId: 4 }],
    portrait: { uri: 'content://head/0' },
    postalAddresses: [
      {
        city: '南京',
        country: '中国',
        labelName: 'labelName',
        neighborhood: 'neighborhood',
        pobox: 'pobox',
        postalAddress: 'postalAddress',
        postcode: 'postcode',
        region: 'region',
        street: 'street',
        labelId: 5
      }
    ],
    relations: [{ relationName: 'relationName', labelName: '自定义relationName', labelId: 6 }],
    sipAddresses: [{ sipAddress: 'sipAddress', labelName: '自定义sipAddress', labelId: 6 }],
    websites: [{ website: 'website' }],
    name: {
      familyName: 'familyName',
      familyNamePhonetic: 'familyNamePhonetic',
      fullName: '小李',
      givenName: 'givenName',
      givenNamePhonetic: 'givenNamePhonetic',
      middleName: 'middleName',
      middleNamePhonetic: 'middleNamePhonetic',
      namePrefix: 'namePrefix',
      nameSuffix: 'nameSuffix'
    },
    nickName: { nickName: 'nickName' },
    note: { noteContent: 'note' },
    organization: { name: 'TT', title: '开发' }
  };

  let gRawContactId;

  /**
   * @tc.number  contactsApi_insert_test_100
   * @tc.name    Insert contact information
   * @tc.desc    Function test
   */
  it('contactsApi_insert_test_100', 0, async function (done) {
    contactsapi.addContact(contactData, (data) => {
      console.info('contactsApi_insert_test_100 : data = ' + data);
      gRawContactId = data;
      expect(data > 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_delete_test_200
   * @tc.name    Delete contact information
   * @tc.desc    Function test
   */
  it('contactsApi_delete_test_200', 0, async function (done) {
    let deleteId = gRawContactId;
    console.info('contactsApi_delete_test_200 : gRawContactId = ' + gRawContactId);
    contactsapi.deleteContact(1, (data) => {
      let gDelete = data;
      console.info('contactsApi_delete_test_200 : deleteCode = ' + data);
      expect(gDelete === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_update_test_300
   * @tc.name    Update contact information
   * @tc.desc    Function test
   */
  it('contactsApi_update_test_300', 0, async function (done) {
    let rawContactId = await contactsapi.addContact(contactData);
    console.info('contactsApi_insert_test_300 : rawContactId = ' + rawContactId);
    gRawContactId = rawContactId;
    expect(rawContactId > 0).assertTrue();

    let updateValues = { id: gRawContactId, name: { fullName: '小红' } };
    let condition = { attributes: [ATTRIBUTE_SIX] };
    contactsapi.updateContact(updateValues, condition, (data) => {
      console.info('contactsApi_update_test_300 : updateCode = ' + data);
      expect(data === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_update_test_4100
   * @tc.name    Update contact information
   * @tc.desc    Function test
   */
  it('contactsApi_update_test_4100', 0, async function (done) {
    let rawContactId = await contactsapi.addContact(contactData);
    console.info('contactsApi_insert_test_300 : rawContactId = ' + rawContactId);
    gRawContactId = rawContactId;
    expect(rawContactId > 0).assertTrue();

    let updateValues = { id: gRawContactId, name: { fullName: '小红' } };
    contactsapi.updateContact(updateValues, (data) => {
      console.info('contactsApi_update_test_4100 : updateCode = ' + data);
      expect(data === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_contact_test_400
   * @tc.name    Query contacts information
   * @tc.desc    Function test
   */
  it('contactsApi_query_contact_test_400', 0, async function (done) {
    let queryId = gRawContactId;
    contactsapi.queryContact(queryId, (data) => {
      console.info('contactsApi_query_contact_test_400 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_contact_test_500
   * @tc.name    Query contacts information
   * @tc.desc    Function test
   */
  it('contactsApi_query_contact_test_500', 0, async function (done) {
    let queryId = gRawContactId.toString();
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };
    contactsapi.queryContact(queryId, holder, (data) => {
      console.info('contactsApi_query_contact_test_500 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_contact_test_600
   * @tc.name    Query contacts information
   * @tc.desc    Function test
   */
  it('contactsApi_query_contact_test_600', 0, async function (done) {
    let queryId = gRawContactId.toString();
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };
    let ContactAttributes = { attributes: [ATTRIBUTE_ONE, ATTRIBUTE_FIVE, ATTRIBUTE_SIX] };

    contactsapi.queryContact(queryId, holder, ContactAttributes, (data) => {
      console.info('contactsApi_query_contact_test_600 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_contact_test_4200
   * @tc.name    Query contacts information
   * @tc.desc    Function test
   */
  it('contactsApi_query_contact_test_4200', 0, async function (done) {
    let queryId = gRawContactId.toString();
    let ContactAttributes = { attributes: [ATTRIBUTE_ONE, ATTRIBUTE_FIVE, ATTRIBUTE_SIX] };

    contactsapi.queryContact(queryId, ContactAttributes, (data) => {
      console.info('contactsApi_query_contact_test_4200 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_contacts_test_700
   * @tc.name    Query contacts information
   * @tc.desc    Function test
   */
  it('contactsApi_query_contacts_test_700', 0, async function (done) {
    contactsapi.queryContacts((data) => {
      console.info('contactsApi_query_contacts_test_700 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_contacts_test_800
   * @tc.name    Query contacts information
   * @tc.desc    Function test
   */
  it('contactsApi_query_contacts_test_800', 0, async function (done) {
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };
    contactsapi.queryContacts(holder, (data) => {
      console.info('contactsApi_query_contacts_test_800 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_contacts_test_900
   * @tc.name    Query contacts information
   * @tc.desc    Function test
   */
  it('contactsApi_query_contacts_test_900', 0, async function (done) {
    let ContactAttributes = { attributes: [ATTRIBUTE_ONE, ATTRIBUTE_FIVE, ATTRIBUTE_SIX] };

    contactsapi.queryContacts(ContactAttributes, (data) => {
      console.info('contactsApi_query_contacts_test_900 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_contacts_test_1000
   * @tc.name    Query contacts information
   * @tc.desc    Function test
   */
  it('contactsApi_query_contacts_test_1000', 0, async function (done) {
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };
    let ContactAttributes = { attributes: [ATTRIBUTE_ONE, ATTRIBUTE_FIVE, ATTRIBUTE_SIX] };

    contactsapi.queryContacts(holder, ContactAttributes, (data) => {
      console.info('contactsApi_query_contacts_test_1000 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_email_test_1100
   * @tc.name    Query email information
   * @tc.desc    Function test
   */
  it('contactsApi_query_email_test_1100', 0, async function (done) {
    let email = 'email';
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };

    contactsapi.queryContactsByEmail(email, holder, (data) => {
      console.info('contactsApi_query_email_test_1100 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_email_test_1200
   * @tc.name    Query email information
   * @tc.desc    Function test
   */
  it('contactsApi_query_email_test_1200', 0, async function (done) {
    let email = 'email';
    contactsapi.queryContactsByEmail(email, (data) => {
      console.info('contactsApi_query_email_test_1200 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_email_test_1300
   * @tc.name    Query email information
   * @tc.desc    Function test
   */
  it('contactsApi_query_email_test_1300', 0, async function (done) {
    let email = 'email';
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };
    let ContactAttributes = { attributes: [ATTRIBUTE_ONE, ATTRIBUTE_FIVE, ATTRIBUTE_SIX] };

    contactsapi.queryContactsByEmail(email, holder, ContactAttributes, (data) => {
      console.info('contactsApi_query_email_test_1300 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_email_test_1400
   * @tc.name    Query email information
   * @tc.desc    Function test
   */
  it('contactsApi_query_email_test_1400', 0, async function (done) {
    let email = 'email';
    let ContactAttributes = { attributes: [ATTRIBUTE_ONE, ATTRIBUTE_FIVE, ATTRIBUTE_SIX] };

    contactsapi.queryContactsByEmail(email, ContactAttributes, (data) => {
      console.info('contactsApi_query_email_test_1400 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_phoneNumber_test_1500
   * @tc.name    Query phoneNumber information
   * @tc.desc    Function test
   */
  it('contactsApi_query_phoneNumber_test_1500', 0, async function (done) {
    let phoneNumber = '183';
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };

    contactsapi.queryContactsByPhoneNumber(phoneNumber, holder, (data) => {
      console.info('contactsApi_query_phoneNumber_test_1500 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_phoneNumber_test_1600
   * @tc.name    Query phoneNumber information
   * @tc.desc    Function test
   */
  it('contactsApi_query_phoneNumber_test_1600', 0, async function (done) {
    let phoneNumber = '183';
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };
    let ContactAttributes = { attributes: [ATTRIBUTE_ONE, ATTRIBUTE_FIVE, ATTRIBUTE_SIX] };

    contactsapi.queryContactsByPhoneNumber(phoneNumber, holder, ContactAttributes, (data) => {
      console.info('contactsApi_query_phoneNumber_test_1600 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_phoneNumber_test_1700
   * @tc.name    Query phoneNumber information
   * @tc.desc    Function test
   */
  it('contactsApi_query_phoneNumber_test_1700', 0, async function (done) {
    let phoneNumber = '183';

    contactsapi.queryContactsByPhoneNumber(phoneNumber, (data) => {
      console.info('contactsApi_query_phoneNumber_test_1700 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_phoneNumber_test_1800
   * @tc.name    Query phoneNumber information
   * @tc.desc    Function test
   */
  it('contactsApi_query_phoneNumber_test_1800', 0, async function (done) {
    let phoneNumber = '183';
    let ContactAttributes = { attributes: [ATTRIBUTE_ONE, ATTRIBUTE_FIVE, ATTRIBUTE_SIX] };

    contactsapi.queryContactsByPhoneNumber(phoneNumber, ContactAttributes, (data) => {
      console.info('contactsApi_query_phoneNumber_test_1800 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_group_test_1900
   * @tc.name    Query group
   * @tc.desc    Function test
   */
  it('contactsApi_query_group_test_1900', 0, async function (done) {
    contactsapi.queryGroups((data) => {
      console.info('contactsApi_query_group_test_1900 : query resultSet = ' + JSON.stringify(data));
      expect(data.length === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_group_test_2000
   * @tc.name    Query group
   * @tc.desc    Function test
   */
  it('contactsApi_query_group_test_2000', 0, async function (done) {
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };

    contactsapi.queryGroups(holder, (data) => {
      console.info('contactsApi_query_group_test_2000 : query resultSet = ' + JSON.stringify(data));
      expect(data.length === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_holders_test_2200
   * @tc.name    Query holders information
   * @tc.desc    Function test
   */
  it('contactsApi_query_holders_test_2200', 0, async function (done) {
    contactsapi.queryHolders((data) => {
      console.info('contactsApi_query_holders_test_2200 : query resultSet = ' + JSON.stringify(data));
      expect(data !== null).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_key_test_2300
   * @tc.name    Query key information
   * @tc.desc    Function test
   */
  it('contactsApi_query_key_test_2300', 0, async function (done) {
    let idtest = gRawContactId;
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };

    contactsapi.queryKey(idtest, holder, (data) => {
      console.info('contactsApi_query_key_test_2300 : query resultSet = ' + JSON.stringify(data));
      expect(data.length !== 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_key_test_2400
   * @tc.name    Query key information
   * @tc.desc    Function test
   */
  it('contactsApi_query_key_test_2400', 0, async function (done) {
    let idtest = gRawContactId;
    console.info('contactsApi_query_key_test_2400 : query gRawContactId = ' + idtest);

    contactsapi.queryKey(idtest, (data) => {
      console.info('contactsApi_query_key_test_2400 : query resultSet = ' + JSON.stringify(data));
      expect(data.length !== 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_mycard_test_2500
   * @tc.name    Query mycard information
   * @tc.desc    Function test
   */
  it('contactsApi_query_mycard_test_2500', 0, async function (done) {
    let holder = { bundleName: 'com.ohos.contacts', displayName: 'phone', holderId: 1 };

    contactsapi.queryMyCard(holder, (data) => {
      console.info('contactsApi_query_mycard_test_2500 : query resultSet = ' + JSON.stringify(data));
      expect(data.length === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_query_mycard_test_4000
   * @tc.name    Query mycard information
   * @tc.desc    Function test
   */
  it('contactsApi_query_mycard_test_4000', 0, async function (done) {
    contactsapi.queryMyCard((data) => {
      console.info('contactsApi_query_mycard_test_4000 : query resultSet = ' + JSON.stringify(data));
      expect(data.length === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_isMyCard_test_2600
   * @tc.name    Query mycard exist
   * @tc.desc    Function test
   */
  it('contactsApi_isMyCard_test_2600', 0, async function (done) {
    let id = 1;

    contactsapi.isMyCard(id, (data) => {
      console.info('contactsApi_isMyCard_test_2600 : query isExist = ' + data);
      expect(data === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  contactsApi_isLocalContact_test_2700
   * @tc.name    Query isLocalContact exist
   * @tc.desc    Function test
   */
  it('contactsApi_isLocalContact_test_2700', 0, async function (done) {
    let id = gRawContactId;

    contactsapi.isLocalContact(id, (data) => {
      console.info('logMessage contactsApi_isLocalContact_test_2700 isExist = ' + data);
      expect(data === 1).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_insert_test_2800
   * @tc.name    contactsApi_insert error
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_insert_test_2800', 0, async function (done) {
    let contactDataError = {};

    contactsapi.addContact(contactDataError, (data) => {
      console.info('abnormal_contactsApi_insert_test_2800 : rawContactId = ' + data);
      expect(data === -1).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_update_test_3000
   * @tc.name    contactsApi_update error
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_update_test_3000', 0, async function (done) {
    let rawContactId = -1;
    let updateValues = { id: rawContactId, name: { fullName: '小红' } };
    let condition = { attributes: [ATTRIBUTE_SIX] };

    contactsapi.updateContact(updateValues, condition, (data) => {
      console.info('abnormal_contactsApi_update_test_3000 : updateCode = ' + data);
      expect(data === -1).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_query_contact_test_3100
   * @tc.name    contactsApi_query_contact error
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_query_contact_test_3100', 0, async function (done) {
    let queryId = '-1';

    contactsapi.queryContact(queryId, (data) => {
      if (data === null) {
        console.info('abnormal_contactsApi_query_contact_test_3100 is null');
      }
      if (data === undefined) {
        console.info('abnormal_contactsApi_query_contact_test_3100 is undefined');
      }
      console.info('abnormal_contactsApi_query_contact_test_3100 : updateCode = ' + JSON.stringify(data));
      expect(data === undefined).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_query_contacts_test_3200
   * @tc.name    contactsApi_query_contacts error
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_query_contacts_test_3200', 0, async function (done) {
    let ContactAttributes = { attributes: [ATTRIBUTE_ONE_HUNDERD] };

    contactsapi.queryContacts(ContactAttributes, (data) => {
      if (data === null) {
        console.info('abnormal_contactsApi_query_contacts_test_3200 is null');
        return;
      }
      console.info('abnormal_contactsApi_query_contacts_test_3200 : query resultSet = ' + JSON.stringify(data));
      expect(data.length === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_query_email_test_3300
   * @tc.name    contactsApi_query_email error
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_query_email_test_3300', 0, async function (done) {
    let email = 'email2222';

    contactsapi.queryContactsByEmail(email, (data) => {
      console.info('abnormal_contactsApi_query_email_test_3300 : query resultSet = ' + JSON.stringify(data));
      expect(data.length === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_query_phoneNumber_test_3400
   * @tc.name    contactsApi_query_phoneNumber error
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_query_phoneNumber_test_3400', 0, async function (done) {
    let phoneNumber = '19999999';

    contactsapi.queryContactsByPhoneNumber(phoneNumber, (data) => {
      console.info('abnormal_contactsApi_query_phoneNumber_test_3400 : query resultSet = ' + JSON.stringify(data));
      expect(data.length === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_query_group_test_3500
   * @tc.name    contactsApi_query_group error
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_query_group_test_3500', 0, async function (done) {
    let holder = { bundleName: 'com.ohos.contacts2', displayName: 'phone2', holderId: 2 };

    contactsapi.queryGroups(holder, (data) => {
      console.info('abnormal_contactsApi_query_group_test_3500 : query resultSet = ' + JSON.stringify(data));
      expect(data.length === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_query_key_test_3600
   * @tc.name    contactsApi_query_key error
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_query_key_test_3600', 0, async function (done) {
    let idtest = -1;

    contactsapi.queryKey(idtest, (data) => {
      console.info('abnormal_contactsApi_query_key_test_3600 : query resultSet = ' + JSON.stringify(data));
      expect(data.length === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_query_mycard_test_3700
   * @tc.name    contactsApi_query_mycard error
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_query_mycard_test_3700', 0, async function (done) {
    let ContactAttributes = { attributes: [ATTRIBUTE_ONE_HUNDERD] };

    contactsapi.queryMyCard(ContactAttributes, (data) => {
      console.info('abnormal_contactsApi_query_mycard_test_3700 : query resultSet = ' + JSON.stringify(data));
      expect(data.length === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_isMyCard_test_3800
   * @tc.name    isMyCard is not exist
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_isMyCard_test_3800', 0, async function (done) {
    let id = 999;

    contactsapi.isMyCard(id, (isExist) => {
      console.info('abnormal_contactsApi_isMyCard_test_3800 : query isExist = ' + isExist);
      expect(isExist === 0).assertTrue();
      done();
    });
  });

  /**
   * @tc.number  abnormal_contactsApi_isLocalContact_test_3900
   * @tc.name    contactsApi_isLocalContact is not exist
   * @tc.desc    Function test
   */
  it('abnormal_contactsApi_isLocalContact_test_3900', 0, async function (done) {
    let id = 999;

    contactsapi.isLocalContact(id, (isExist) => {
      console.info('abnormal_contactsApi_isLocalContact_test_3900 : query isExist = ' + isExist);
      expect(isExist === 0).assertTrue();
      done();
    });
  });
});
