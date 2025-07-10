/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
const featureAbility = requireNapi('ability.featureAbility');
const contact = requireInternal('contact');
const ARGUMENTS_LEN_TWO = 2;

async function contactsPickerSelect() {
  if (arguments.length === ARGUMENTS_LEN_TWO && typeof arguments[1] !== 'function') {
    console.log('[picker] contactsPickerSelect callback invalid');
    throw Error('invalid callback');
  }

  try {
    let result;
    let context = getContext(this);
    if (context) {
      let config = {
        parameters: {
          'pageFlag': 'page_flag_single_choose'
        },
        bundleName: 'com.ohos.contacts',
        abilityName: 'com.ohos.contacts.MainAbility'
      };
      if (arguments.length > 0 && typeof arguments[0] === 'object') {
        let option = arguments[0];
        if (option.isMultiSelect !== undefined) {
          config.parameters.pageFlag =
            option.isMultiSelect === true ? 'page_flag_multi_choose' : 'page_flag_single_choose';
        }
      }
      console.log('[picker] contactsPickerSelect config: ' + JSON.stringify(config));
      result = await context.startAbilityForResult(config, { windowMode: 1 });
    } else {
      let parameter = {
        want: {
          parameters: {
            'pageFlag': 'page_flag_single_choose'
          },
          bundleName: 'com.ohos.contacts',
          abilityName: 'com.ohos.contacts.MainAbility'
        }
      };
      if (arguments.length > 0 && typeof arguments[0] === 'object') {
        let option = arguments[0];
        if (option.isMultiSelect !== undefined) {
          parameter.want.parameters.pageFlag =
            option.isMultiSelect === true ? 'page_flag_multi_choose' : 'page_flag_single_choose';
        }
      }
      console.log('[picker] contactsPickerSelect parameter: ' + JSON.stringify(parameter));
      result = await featureAbility.startAbilityForResult(parameter);
    }
    let contactObject = result.want.parameters.contactObjects;
    let params = [];
    try {
      params = JSON.parse(contactObject);
    } catch (Error) {
      console.log(`[picker] JSON.parse Error: ${Error}`);
    }
    let contacts = [];
    for (let object of params) {
      let contact = {
        name: {
          fullName: object.contactName
        },
        phoneNumbers: [
          {
            phoneNumber: object.telephone
          }
        ]
      };
      contacts.push(contact);
    }
    if (arguments.length === ARGUMENTS_LEN_TWO && typeof arguments[1] === 'function') {
      if (result.resultCode === 0) {
        return arguments[1](undefined, contacts);
      } else {
        return arguments[1](result.resultCode, undefined);
      }
    } else if (arguments.length === 1 && typeof arguments[0] === 'function') {
      if (result.resultCode === 0) {
        return arguments[0](undefined, contacts);
      } else {
        return arguments[0](result.resultCode, undefined);
      }
    }
    return new Promise((resolve, reject) => {
      if (result.resultCode === 0) {
        resolve(contacts);
      } else {
        console.log('[picker] contactsPickerSelect err, result.resultCode = : ' + result.resultCode);
        reject(result.resultCode);
      }
    });
  } catch (error) {
    console.log('[picker] contactsPickerSelect error: ' + error);
  }
}

export default {
  selectContact: contactsPickerSelect,
  selectContacts: contactsPickerSelect,
  addContact: contact.addContact,
  deleteContact: contact.deleteContact,
  queryContact: contact.queryContact,
  queryContacts: contact.queryContacts,
  queryContactsByEmail: contact.queryContactsByEmail,
  queryContactsByPhoneNumber: contact.queryContactsByPhoneNumber,
  queryGroups: contact.queryGroups,
  queryHolders: contact.queryHolders,
  queryKey: contact.queryKey,
  queryMyCard: contact.queryMyCard,
  updateContact: contact.updateContact,
  isLocalContact: contact.isLocalContact,
  isMyCard: contact.isMyCard,

  Contact: contact.Contact,
  ContactAttributes: contact.ContactAttributes,
  Attribute: contact.Attribute,
  Email: contact.Email,
  Event: contact.Event,
  Group: contact.Group,
  Holder: contact.Holder,
  ImAddress: contact.ImAddress,
  Name: contact.Name,
  NickName: contact.NickName,
  Note: contact.Note,
  Organization: contact.Organization,
  PhoneNumber: contact.PhoneNumber,
  Portrait: contact.Portrait,
  PostalAddress: contact.PostalAddress,
  Relation: contact.Relation,
  SipAddress: contact.SipAddress,
  Website: contact.Website
};
