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

import {startDialogCallback} from "./i_idl_system_dialog_service";
import {sendRemoteObjectCallback} from "./i_idl_system_dialog_service";
import IIdlSystemDialogService from "./i_idl_system_dialog_service";
import rpc from "@ohos.rpc";

export default class IdlSystemDialogServiceStub extends rpc.RemoteObject implements IIdlSystemDialogService {
    constructor(des: string) {
        super(des);
    }
    
    onRemoteRequest(code: number, data, reply, option): boolean {
        console.log("onRemoteRequest called, code = " + code);
        switch(code) {
            case IdlSystemDialogServiceStub.COMMAND_START_DIALOG: {
                let _data = new Map();
                let _dataSize = data.readInt();
                for (let i = 0; i < _dataSize; ++i) {
                    let key = data.readString();
                    let value = data.readString();
                    _data.set(key, value);
                }
                this.startDialog(_data, (errCode) => {
                    reply.writeInt(errCode);
                });
                return true;
            }
            case IdlSystemDialogServiceStub.COMMAND_SEND_REMOTE_OBJECT: {
                this.sendRemoteObject((errCode) => {
                    reply.writeInt(errCode);
                });
                return true;
            }
            default: {
                console.log("invalid request code" + code);
                break;
            }
        }
        return false;
    }
    
    startDialog(data: Map<string, string>, callback: startDialogCallback): void{}
    sendRemoteObject(callback: sendRemoteObjectCallback): void{}

    static readonly COMMAND_START_DIALOG = 1;
    static readonly COMMAND_SEND_REMOTE_OBJECT = 2;
}

