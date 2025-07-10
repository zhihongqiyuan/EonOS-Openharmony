/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

export default class Constants {
    static WIN_NAME = "ScreenLockWindow"

    //Shortcut params
    static SHORTCUT_CIRCLE_WIDTH = '80px'
    static SHORTCUT_CIRCLE_HEIGHT = '80px'
    static SHORTCUT_TEXT_SIZE = '24px'
    static SHORTCUT_TEXT_WIDTH = '48px'
    static SHORTCUT_TEXT_HEIGHT = '34px'
    static SHORTCUT_BLOCK_HEIGHT = '10px'
    static SHORTCUT_HEIGHT = '150px'

    //layout params - Pic
    static FULL_CONTAINER_WIDTH = '100%'
    static FULL_CONTAINER_HEIGHT = '100%'
    static HALF_CONTAINER_WIDTH  = '50%'

    //The refresh interval
    static INTERVAL = 1000

    //digitalpssword mask diameter
    static DIGITALPSD_IC_DIAMETER = 12

    //digitalpssword keybord background diameter
    static DIGITALPSD_BUTTON_DIAMETER = 60

    //digitalpssword keybord background diameter
    static DIGITALPSD_BUTTON_RECT_WH = 100
    static DIGITALPSD_BUTTON_RECT_HH = 50

    //digital keybord
    static CALL_PHONE = -1
    static DEL_PWD = -2
    static GO_BACK = -3
    static NUMKEY_BOARD = [{
                               index: 0,
                               row1: '1',
                               row2: ' ',
                               value: 1,
                               bkg: false
                           },
                           {
                               index: 1,
                               row1: '2',
                               row2: 'ABC',
                               value: 2,
                               bkg: false
                           },
                           {
                               index: 2,
                               row1: '3',
                               row2: 'DEF',
                               value: 3,
                               bkg: false
                           },
                           {
                               index: 3,
                               row1: '4',
                               row2: 'GHI',
                               value: 4,
                               bkg: false
                           },
                           {
                               index: 4,
                               row1: '5',
                               row2: 'JKL',
                               value: 5,
                               bkg: false
                           },
                           {
                               index: 5,
                               row1: '6',
                               row2: 'MNO',
                               value: 6,
                               bkg: false
                           },
                           {
                               index: 6,
                               row1: '7',
                               row2: 'PQRS',
                               value: 7,
                               bkg: false
                           },
                           {
                               index: 7,
                               row1: '8',
                               row2: 'TUV',
                               value: 8,
                               bkg: false
                           },
                           {
                               index: 8,
                               row1: '9',
                               row2: 'WXYZ',
                               value: 9,
                               bkg: false
                           },
                           {
                               index: 9,
                               row1: $r('app.string.emergency_call'),
                               row2: '',
                               value: Constants.CALL_PHONE,
                               bkg: false
                           },
                           {
                               index: 10,
                               row1: '0',
                               row2: '+',
                               value: 0,
                               bkg: false
                           },
                           {
                               index: 11,
                               row1: $r('app.string.back'),
                               row2: '',
                               value: Constants.GO_BACK,
                               bkg: false
                           }];
    //max password length
    static PASSWORD_MAX_LEN = 32
    static PASSWORD_TEXT_WIDTH ='290px'
    static PASSWORD_TEXT_HEIGHT ='40px'
    static PASSWORD_TEXT_BORDER =20
    static ACCOUNT_SPACE = '24px'
    static ACCOUNT_SPACE_PORTRAIT = '40px'
}

export class StatusBarGroupComponentData {
    backgroundColor: string = "#00000000";
    contentColor: string = "#FFFFFFFF";
}