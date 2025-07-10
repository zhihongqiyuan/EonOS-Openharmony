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

package com.ohos.hapsigntool.hap.verify;

import com.ohos.hapsigntool.utils.FileUtils;

import com.ohos.hapsigntool.utils.LogUtils;
import org.bouncycastle.cms.CMSException;
import org.bouncycastle.cms.CMSSignedData;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.SignatureException;

/**
 * verify signatures of provision and output unsigned provision
 *
 * @since 2021/12/21
 */
public class VerifyAndParseProvision {
    private static final LogUtils LOGGER = new LogUtils(VerifyAndParseProvision.class);

    /**
     * verify signatures of provision and output unsigned provision
     *
     * @param signedProvisionPath the path of the input signedProvision file.
     * @param unsignedProvisionPath the path to which the unsignedProvision file is output.
     * @return true if the provision file is verified and parsed successfully, false otherwise.
     */
    public boolean verifyAndParseProvision(String signedProvisionPath, String unsignedProvisionPath) {
        File signedProvisionFile = new File(signedProvisionPath);
        try {
            if (!checkProvisionFile(signedProvisionFile)) {
                String errorMsg = "Check input provision file failed!";
                LOGGER.error(errorMsg);
                throw new IOException();
            }
            byte[] signedProvisionData = getSignedProvisionData(signedProvisionFile);
            if (signedProvisionData.length == 0) {
                LOGGER.error("read provision file failed");
                throw new IOException();
            }
            byte[] unsignedProvisionData = getUnsignedProvisionData(signedProvisionData);
            if (unsignedProvisionData == null) {
                LOGGER.error("get unsigned provision failed");
                throw new IOException();
            }
            return outputUnsignedProvisionToFile(unsignedProvisionData, unsignedProvisionPath);
        } catch (IOException e) {
            return false;
        }
    }

    private boolean checkProvisionFile(File signedProvisionFile) {
        try {
            if (!signedProvisionFile.canRead()) {
                LOGGER.error(signedProvisionFile.getCanonicalPath() + " does not exist or can not read!");
                throw new IOException();
            }
            if (!signedProvisionFile.isFile()) {
                LOGGER.error(signedProvisionFile.getCanonicalPath() + " is not a file!");
                throw new IOException();
            }
        } catch (IOException e) {
            LOGGER.error("getCanonicalPath failed", e);
            return false;
        }
        return true;
    }

    private byte[] getSignedProvisionData(File signedProvisionFile) {
        byte[] signedProvisionData = new byte[0];
        try {
            signedProvisionData = FileUtils.readFileToByteArray(signedProvisionFile);
        } catch (IOException e) {
            LOGGER.error("readFileToByteArray failed.", e);
        }
        return signedProvisionData;
    }

    private byte[] getUnsignedProvisionData(byte[] signedProvisionData) {
        byte[] unsignedProvisionData = null;
        try {
            CMSSignedData cmsSignedData = new CMSSignedData(signedProvisionData);
            if (!VerifyUtils.verifyCmsSignedData(cmsSignedData)) {
                throw new SignatureException("PKCS7 cms data verify faild!");
            }
            unsignedProvisionData = (byte[]) cmsSignedData.getSignedContent().getContent();
        } catch (SignatureException | CMSException e) {
            LOGGER.error("get unsigned provision data failed.", e);
        }
        return unsignedProvisionData;
    }

    private boolean outputUnsignedProvisionToFile(byte[] unsignedProvisionData, String unsignedProvisionPath) {
        boolean ret = false;
        try (FileOutputStream unsignedProvisionOutputStream = new FileOutputStream(new File(unsignedProvisionPath))) {
            unsignedProvisionOutputStream.write(unsignedProvisionData);
            ret = true;
        } catch (IOException e) {
            LOGGER.error("output unsigned provision to file failed.", e);
        }
        return ret;
    }
}
