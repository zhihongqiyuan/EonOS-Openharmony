/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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

package com.ohos.entity;

import com.ohos.hapsigntool.error.ERROR;

/**
 * RetMsg.
 *
 * @since 2024/04/06
 */
public class RetMsg {
    private ERROR errCode;

    private String errMessage;

    public RetMsg(ERROR errCode, String errMessage) {
        this.errCode = errCode;
        this.errMessage = errMessage;
    }

    public ERROR getErrCode() {
        return errCode;
    }

    public void setErrCode(ERROR errCode) {
        this.errCode = errCode;
    }

    public String getErrMessage() {
        return errMessage;
    }

    public void setErrMessage(String errMessage) {
        this.errMessage = errMessage;
    }
}
