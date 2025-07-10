/*
 * Copyright (c) 2023-2023 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.codesigning.utils;

import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParseException;
import com.google.gson.JsonParser;
import com.google.gson.JsonPrimitive;
import com.google.gson.JsonSyntaxException;
import com.google.gson.stream.JsonReader;
import com.ohos.hapsigntool.codesigning.exception.CodeSignErrMsg;
import com.ohos.hapsigntool.entity.Pair;
import com.ohos.hapsigntool.error.ProfileException;
import com.ohos.hapsigntool.utils.LogUtils;

import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

/**
 * utility for check hap configs
 *
 * @since 2023/06/05
 */
public class HapUtils {
    /**
     * DEBUG_LIB_ID
     */
    public static final String HAP_DEBUG_OWNER_ID = "DEBUG_LIB_ID";

    /**
     * SHARED_LIB_ID
     */
    public static final String HAP_SHARED_OWNER_ID = "SHARED_LIB_ID";

    private static final LogUtils LOGGER = new LogUtils(HapUtils.class);

    private static final String COMPRESS_NATIVE_LIBS_OPTION = "compressNativeLibs";

    private static final List<String> HAP_CONFIG_FILES = new ArrayList<>();

    private static final String HAP_FA_CONFIG_JSON_FILE = "config.json";

    private static final String HAP_STAGE_MODULE_JSON_FILE = "module.json";

    private static final int MAX_APP_ID_LEN = 32; // max app-identifier in profile

    static {
        HAP_CONFIG_FILES.add(HAP_FA_CONFIG_JSON_FILE);
        HAP_CONFIG_FILES.add(HAP_STAGE_MODULE_JSON_FILE);
    }

    private HapUtils() {
    }

    /**
     * get app-id from profile
     *
     * @param profileContent the content of profile
     * @return string value of app-id
     * @throws ProfileException profile is invalid
     */
    public static String getAppIdentifier(String profileContent) throws ProfileException {
        Pair<String, String> resultPair = parseAppIdentifier(profileContent);
        String ownerID = resultPair.getFirst();
        String profileType = resultPair.getSecond();
        if ("debug".equals(profileType)) {
            return HAP_DEBUG_OWNER_ID;
        } else if ("release".equals(profileType)) {
            return ownerID;
        } else {
            throw new ProfileException(CodeSignErrMsg.PROFILE_TYPE_UNSUPPORTED_ERROR.toString());
        }
    }

    /**
     * parse app-id and profileType from profile
     *
     * @param profileContent the content of profile
     * @return Pair value of app-id and profileType
     * @throws ProfileException profile is invalid
     */
    public static Pair<String, String> parseAppIdentifier(String profileContent) throws ProfileException {
        String ownerID = null;
        String profileType = null;
        try {
            JsonElement parser = JsonParser.parseString(profileContent);
            JsonObject profileJson = parser.getAsJsonObject();
            String profileTypeKey = "type";
            if (!profileJson.has(profileTypeKey)) {
                throw new ProfileException(CodeSignErrMsg.PROFILE_TYPE_NOT_EXISTED_ERROR.toString());
            }

            profileType = profileJson.get(profileTypeKey).getAsString();
            if (profileType == null || profileType.isEmpty()) {
                throw new ProfileException(CodeSignErrMsg.PROFILE_TYPE_NOT_EXISTED_ERROR.toString());
            }

            String appIdentifier = "app-identifier";
            String buildInfoMember = "bundle-info";
            JsonObject buildInfoObject = profileJson.getAsJsonObject(buildInfoMember);
            if (buildInfoObject == null) {
                throw new ProfileException(CodeSignErrMsg.PROFILE_BUNDLE_INFO_NOT_EXISTED_ERROR.toString());
            }
            if (buildInfoObject.has(appIdentifier)) {
                JsonElement ownerIDElement = buildInfoObject.get(appIdentifier);
                if (!ownerIDElement.getAsJsonPrimitive().isString()) {
                    throw new ProfileException(CodeSignErrMsg.PROFILE_APPID_VALUE_TYPE_ERROR.toString());
                }
                ownerID = ownerIDElement.getAsString();
                if (ownerID.isEmpty() || ownerID.length() > MAX_APP_ID_LEN) {
                    throw new ProfileException(CodeSignErrMsg.PROFILE_APPID_VALUE_LENGTH_ERROR.toString());
                }
            }
        } catch (JsonSyntaxException | UnsupportedOperationException e) {
            throw new ProfileException(CodeSignErrMsg.PROFILE_JSON_PARSE_ERROR.toString(), e);
        }
        LOGGER.info("profile type is: {}", profileType);
        return Pair.create(ownerID, profileType);
    }

    /**
     * get hnp app-id from profile when type is public
     *
     * @param profileContent the content of profile
     * @return ownerid
     */
    public static String getPublicHnpOwnerId(String profileContent) {
        // property type
        String publicOwnerID = "";
        JsonElement parser = JsonParser.parseString(profileContent);
        JsonObject profileJson = parser.getAsJsonObject();
        String profileTypeKey = "type";
        JsonPrimitive profileType = profileJson.getAsJsonPrimitive(profileTypeKey);
        if (profileType != null) {
            if ("debug".equals(profileType.getAsString())) {
                publicOwnerID = HAP_DEBUG_OWNER_ID;
            } else if ("release".equals(profileType.getAsString())) {
                publicOwnerID = HAP_SHARED_OWNER_ID;
            }
        }
        return publicOwnerID;
    }

    /**
     * get hnp path behind "hnp/abi/"
     *
     * @param path filepath
     * @return hnp path behind "hnp/abi/"
     */
    public static String parseHnpPath(String path) {
        if (path == null || path.isEmpty()) {
            return "";
        }
        String[] strings = path.split("/");
        if (strings.length < 3) {
            return "";
        }
        // get hnp path behind "hnp/abi/"
        strings = Arrays.copyOfRange(strings, 2, strings.length);
        return String.join("/", strings);
    }

    /**
     * get map of hnp name and type from module.json
     *
     * @param inputJar hap file
     * @return packageName-type map
     * @throws IOException when IO error occurred
     * @throws ProfileException profile is invalid
     */
    public static Map<String, String> getHnpsFromJson(JarFile inputJar) throws IOException, ProfileException {
        // get module.json
        Map<String, String> hnpNameMap = new HashMap<>();
        JarEntry moduleEntry = inputJar.getJarEntry("module.json");
        if (moduleEntry == null) {
            return hnpNameMap;
        }
        try (JsonReader reader = new JsonReader(
            new InputStreamReader(inputJar.getInputStream(moduleEntry), StandardCharsets.UTF_8))) {
            JsonElement jsonElement = JsonParser.parseReader(reader);
            JsonObject jsonObject = jsonElement.getAsJsonObject();
            JsonObject moduleObject = jsonObject.getAsJsonObject("module");
            JsonArray hnpPackageArr = moduleObject.getAsJsonArray("hnpPackages");
            if (hnpPackageArr == null || hnpPackageArr.isEmpty()) {
                LOGGER.debug("module.json has no hnpPackages key or hnpPackages value is empty");
                return hnpNameMap;
            }
            hnpPackageArr.iterator().forEachRemaining((element) -> {
                JsonObject hnpPackage = element.getAsJsonObject();
                JsonPrimitive hnpName = hnpPackage.getAsJsonPrimitive("package");
                if (hnpName == null || hnpName.getAsString().isEmpty()) {
                    return;
                }
                hnpNameMap.put(hnpName.getAsString(), "private");
                JsonPrimitive type = hnpPackage.getAsJsonPrimitive("type");
                if (type != null && !type.getAsString().isEmpty()) {
                    hnpNameMap.put(hnpName.getAsString(), type.getAsString());
                }
            });
        } catch (JsonParseException e) {
            throw new ProfileException(CodeSignErrMsg.MODULE_JSON_PARSE_ERROR.toString(), e);
        }
        return hnpNameMap;
    }

}
