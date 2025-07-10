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

package com.ohos.hapsigntool.signer;

import com.ohos.hapsigntool.adapter.LocalizationAdapter;
import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.utils.StringUtils;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.security.KeyPair;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.Properties;

/**
 * Factory pattern to create signer.
 *
 * @since 2021/12/28
 */
public class SignerFactory {
    private static final LogUtils LOGGER = new LogUtils(SignerFactory.class);

    private static final Map<URL, ClassLoader> SIGNER_LOADERS = new HashMap<>();

    /**
     * Create a signer.
     *
     * @param adapter Params adapter
     * @return Local signer or remote signer
     */
    public ISigner getSigner(LocalizationAdapter adapter) {
        if (adapter.isRemoteSigner()) {
            Optional<ISigner> remoteSigner = loadRemoteSigner(adapter);
            if (remoteSigner.isPresent()) {
                return remoteSigner.get();
            }
            LOGGER.warn("load remote signer failed, use default implementation");
            return new RemoteSigner(adapter.getOptions());
        }
        KeyPair keyPair = adapter.getAliasKey(false);
        adapter.releasePwd();
        return new LocalSigner(keyPair.getPrivate(), adapter.getSignCertChain());
    }

    private Optional<ISigner> loadRemoteSigner(LocalizationAdapter adapter) {
        String signerPlugin = adapter.getOptions().getString("signerPlugin");
        if (StringUtils.isEmpty(signerPlugin)) {
            LOGGER.warn("lost parameter signerPlugin");
            return Optional.empty();
        }

        File plugin = new File(signerPlugin);
        if (!plugin.exists()) {
            File classLocation = getClassLocation();
            plugin = new File(classLocation, signerPlugin);
        }
        if (!plugin.exists() || !plugin.isFile()) {
            LOGGER.warn("can not find signerPlugin or not a file by param signerPlugin = {}", signerPlugin);
            return Optional.empty();
        }
        Optional<URL> url = fileToUrl(plugin);
        if (!url.isPresent()) {
            return Optional.empty();
        }
        try {
            ClassLoader classLoader = generateSignerClassLoader(url.get());
            try (InputStream inputStream = classLoader.getResourceAsStream("signer.properties")) {
                if (inputStream == null) {
                    LOGGER.warn("can not find entry signer.properties in {}", plugin);
                    return Optional.empty();
                }
                Properties properties = new Properties();
                properties.load(inputStream);
                String implClassName = properties.getProperty(ISigner.class.getName());
                if (StringUtils.isEmpty(implClassName)) {
                    LOGGER.warn("can not find {} in signer.properties", ISigner.class.getName());
                    return Optional.empty();
                }
                Class<?> implClass = classLoader.loadClass(implClassName);
                Constructor<?> constructor = implClass.getConstructor(Map.class);
                Object signer = constructor.newInstance(adapter.getOptions());
                if (signer instanceof ISigner) {
                    return Optional.of((ISigner) signer);
                }
            }
        } catch (IOException | ClassNotFoundException | NoSuchMethodException
            | InvocationTargetException | InstantiationException | IllegalAccessException e) {
            LOGGER.warn("load remote signer from {} failed, msg: {}", signerPlugin, e.getMessage());
        }
        return Optional.empty();
    }

    private Optional<URL> fileToUrl(File file) {
        if (!file.exists()) {
            LOGGER.warn("{} is not exists", file);
            return Optional.empty();
        }
        try {
            return Optional.of(file.toURI().toURL());
        } catch (MalformedURLException e) {
            LOGGER.warn("{} can not convert to valid url, msg: {}", file, e.getMessage());
        }
        return Optional.empty();
    }

    private File getClassLocation() {
        String jarPath = SignerFactory.class.getProtectionDomain().getCodeSource().getLocation().getFile();
        if (StringUtils.isEmpty(jarPath)) {
            CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.LOAD_REMOTE_PLUGIN_FAILED
                    .toString("Class path is empty"));
        }
        try {
            jarPath = URLDecoder.decode(URLEncoder.encode(jarPath, "utf-8"), "utf-8");
        } catch (UnsupportedEncodingException | IllegalArgumentException e) {
            LOGGER.warn("decode class location failed, will ignored. msg :{}", e.getMessage());
        }
        File jarFile = new File(jarPath);
        if (!jarFile.exists()) {
            CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.LOAD_REMOTE_PLUGIN_FAILED
                    .toString("class path" + jarFile + "is not exists"));
        }
        if (jarFile.isFile()) {
            return jarFile.getParentFile();
        }
        return jarFile;
    }

    private static synchronized ClassLoader generateSignerClassLoader(URL signerClassUrl) {
        ClassLoader classLoader = SIGNER_LOADERS.get(signerClassUrl);
        if (classLoader == null) {
            ClassLoader parent = SignerFactory.class.getClassLoader();
            classLoader = URLClassLoader.newInstance(new URL[]{signerClassUrl}, parent);
            SIGNER_LOADERS.put(signerClassUrl, classLoader);
        }
        return classLoader;
    }
}
