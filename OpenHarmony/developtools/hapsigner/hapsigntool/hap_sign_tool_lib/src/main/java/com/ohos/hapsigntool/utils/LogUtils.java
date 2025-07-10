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

package com.ohos.hapsigntool.utils;

import com.ohos.hapsigntool.error.LogConfigException;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.security.CodeSource;
import java.security.ProtectionDomain;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.logging.StreamHandler;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Hap Sign Tool LogUtils
 *
 * @since 2024/12/08
 */
public class LogUtils {
    private static Level level;
    private static final Map<String, Level> LEVEL_MAP = new HashMap<>();
    private static final String DEFAULT_LEVEL = "info";
    private static final Pattern PATTERN = Pattern.compile("\\{}");

    private static final Handler OUT_HANDLER;
    private static final Handler ERR_HANDLER;

    static {
        LEVEL_MAP.put(DEFAULT_LEVEL, Level.INFO);
        LEVEL_MAP.put("debug", Level.CONFIG);
        LEVEL_MAP.put("warn", Level.WARNING);
        LEVEL_MAP.put("error", Level.SEVERE);
        String configFileName = "log.properties";

        try {
            level = LEVEL_MAP.get(getJarConfig(configFileName));
        } catch (LogConfigException e) {
            level = LEVEL_MAP.get(getDefaultLogLevel(configFileName));
        }

        LogFormatter logFormatter = new LogFormatter();
        OUT_HANDLER = new StreamHandler(System.out, logFormatter);
        OUT_HANDLER.setFilter(record -> record.getLevel() != Level.SEVERE && record.getLevel() != Level.WARNING);
        OUT_HANDLER.setLevel(level);
        ERR_HANDLER = new StreamHandler(System.err, logFormatter);
        ERR_HANDLER.setLevel(Level.WARNING);
    }

    private final Logger logger;

    /**
     * format log utils constructor.
     *
     * @param clazz class
     */
    public LogUtils(Class<?> clazz) {
        logger = Logger.getLogger(clazz.getName());
        logger.setUseParentHandlers(false);
        logger.addHandler(OUT_HANDLER);
        logger.addHandler(ERR_HANDLER);
        logger.setLevel(level);
    }

    /**
     * print info log
     *
     * @param log log string
     */
    public void info(String log) {
        logger.info(" INFO - " + log);
        OUT_HANDLER.flush();
    }

    /**
     * print info log
     *
     * @param log log string
     * @param arg arg
     */
    public void info(String log, Object arg) {
        logger.info(" INFO - " + replaceArgs(log, arg));
        OUT_HANDLER.flush();
    }

    /**
     * print info log
     *
     * @param log log string
     * @param arg1 arg
     * @param arg2 arg
     */
    public void info(String log, Object arg1, Object arg2) {
        logger.info(" INFO - " + replaceArgs(log, arg1, arg2));
        OUT_HANDLER.flush();
    }

    /**
     * print warn log
     *
     * @param log log string
     */
    public void warn(String log) {
        logger.warning(" WARN - " + log);
        ERR_HANDLER.flush();
    }

    /**
     * print warn log
     *
     * @param log log string
     * @param arg arg
     */
    public void warn(String log, Object arg) {
        logger.warning(" WARN - " + replaceArgs(log, arg));
        ERR_HANDLER.flush();
    }

    /**
     * print warn log
     *
     * @param log log string
     * @param arg arg
     * @param e throwable
     */
    public void warn(String log, Object arg, Throwable e) {
        logger.log(Level.WARNING, e, () -> " DEBUG - " + replaceArgs(log, arg));
        ERR_HANDLER.flush();
    }

    /**
     * print warn log
     *
     * @param log log string
     * @param arg1 arg
     * @param arg2 arg
     */
    public void warn(String log, Object arg1, Object arg2) {
        logger.warning(" WARN - " + replaceArgs(log, arg1, arg2));
        ERR_HANDLER.flush();
    }

    /**
     * print warn log
     *
     * @param log log string
     * @param e throwable
     */
    public void warn(String log, Throwable e) {
        logger.log(Level.WARNING, e, () -> " DEBUG - " + log);
        ERR_HANDLER.flush();
    }

    /**
     * print debug log
     *
     * @param log log string
     */
    public void debug(String log) {
        logger.config(" DEBUG - " + log);
        OUT_HANDLER.flush();
    }

    /**
     * print debug log
     *
     * @param log log string
     * @param arg1 arg
     * @param arg2 arg
     */
    public void debug(String log, Object arg1, Object arg2) {
        logger.config(" DEBUG - " + replaceArgs(log, arg1, arg2));
        OUT_HANDLER.flush();
    }

    /**
     * print debug log
     *
     * @param log log string
     * @param e throwable
     */
    public void debug(String log, Throwable e) {
        logger.log(Level.CONFIG, e, () -> " DEBUG - " + log);
        OUT_HANDLER.flush();
    }

    /**
     * print debug log
     *
     * @param log log string
     * @param arg arg
     */
    public void debug(String log, Object arg) {
        logger.config(" DEBUG - " + replaceArgs(log, arg));
        OUT_HANDLER.flush();
    }

    /**
     * print error log
     *
     * @param log log string
     */
    public void error(String log) {
        logger.severe(" ERROR - " + log);
        ERR_HANDLER.flush();
    }

    /**
     * print error log
     *
     * @param log log string
     * @param e throwable
     */
    public void error(String log, Throwable e) {
        logger.log(Level.SEVERE, e, () -> " ERROR - " + log);
        ERR_HANDLER.flush();
    }

    /**
     * print error log
     *
     * @param log log string
     * @param arg arg
     * @param e throwable
     */
    public void error(String log, Object arg, Throwable e) {
        logger.log(Level.SEVERE, e, () -> " ERROR - " + replaceArgs(log, arg));
        ERR_HANDLER.flush();
    }

    /**
     * print error log
     *
     * @param log log string
     * @param arg arg
     */
    public void error(String log, Object arg) {
        logger.severe(" ERROR - " + replaceArgs(log, arg));
        ERR_HANDLER.flush();
    }

    /**
     * print error log
     *
     * @param log log string
     * @param arg1 arg
     * @param arg2 arg
     */
    public void error(String log, Object arg1, Object arg2) {
        logger.severe(" ERROR - " + replaceArgs(log, arg1, arg2));
        ERR_HANDLER.flush();
    }

    /**
     * print error log
     *
     * @param log log string
     * @param arg1 arg
     * @param arg2 arg
     * @param arg3 arg
     */
    public void error(String log, Object arg1, Object arg2, Object arg3) {
        logger.severe(" ERROR - " + replaceArgs(log, arg1, arg2, arg3));
        ERR_HANDLER.flush();
    }

    private static String replaceArgs(String line, Object... args) {
        Matcher matcher = PATTERN.matcher(line);
        String result = line;
        if (!matcher.find()) {
            return line;
        }
        for (Object arg : args) {
            Matcher m = PATTERN.matcher(result);
            result = m.replaceFirst(String.valueOf(arg));
        }
        return result;
    }

    private static String getJarConfig(String configFileName) throws LogConfigException {
        String parent = getJarDirectory();
        if (parent == null) {
            throw new LogConfigException("get jar Parent failed");
        }
        File config = new File(parent, configFileName);
        if (!config.exists()) {
            throw new LogConfigException("can not find config file");
        }
        try (FileInputStream fis = new FileInputStream(config)) {
            return getLogLevel(fis);
        } catch (IOException e) {
            throw new LogConfigException("read config file failed");
        }
    }

    private static String getJarDirectory() throws LogConfigException {
        ProtectionDomain protectionDomain = LogUtils.class.getProtectionDomain();
        if (protectionDomain == null) {
            throw new LogConfigException("get jar protect domain failed");
        }
        CodeSource codeSource = protectionDomain.getCodeSource();
        if (codeSource == null) {
            throw new LogConfigException("read jar code path failed");
        }
        URL location = codeSource.getLocation();
        if (location == null) {
            throw new LogConfigException("read jar code source location failed");
        }
        String jarPath = location.getFile();
        if (jarPath == null) {
            throw new LogConfigException("read jar path failed");
        }
        return new File(jarPath).getParent();
    }

    private static String getDefaultLogLevel(String configFileName) {
        try (InputStream inputStream = LogUtils.class.getClassLoader().getResourceAsStream(configFileName)) {
            if (inputStream == null) {
                return DEFAULT_LEVEL;
            } else {
                return getLogLevel(inputStream);
            }
        } catch (IOException e) {
            return DEFAULT_LEVEL;
        }
    }

    private static String getLogLevel(InputStream is) throws IOException {
        Properties prop = new Properties();
        prop.load(is);
        Object levelConfig = prop.get("level");
        if (levelConfig instanceof String) {
            return (String) levelConfig;
        }
        return DEFAULT_LEVEL;
    }

}
