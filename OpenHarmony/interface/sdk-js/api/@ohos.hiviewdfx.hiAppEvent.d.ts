/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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

/**
 * @file
 * @kit PerformanceAnalysisKit
 */

import type { AsyncCallback } from './@ohos.base';

/**
 * Provides the event logging function for applications to log the fault, statistical, security,
 * and user behavior events reported during running. Based on event information,
 * you will be able to analyze the running status of applications.
 *
 * @namespace hiAppEvent
 * @syscap SystemCapability.HiviewDFX.HiAppEvent
 * @since 9
 */
/**
 * Provides the event logging function for applications to log the fault, statistical, security,
 * and user behavior events reported during running. Based on event information,
 * you will be able to analyze the running status of applications.
 *
 * @namespace hiAppEvent
 * @syscap SystemCapability.HiviewDFX.HiAppEvent
 * @atomicservice
 * @since 11
 */
/**
 * Provides the event logging function for applications to log the fault, statistical, security,
 * and user behavior events reported during running. Based on event information,
 * you will be able to analyze the running status of applications.
 *
 * @namespace hiAppEvent
 * @syscap SystemCapability.HiviewDFX.HiAppEvent
 * @crossplatform
 * @atomicservice
 * @since 19
 */
declare namespace hiAppEvent {
  /**
   * Enumerates event types.
   *
   * @enum { number }
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Enumerates event types.
   *
   * @enum { number }
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Enumerates event types.
   *
   * @enum { number }
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  enum EventType {
    /**
     * Fault event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Fault event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Fault event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    FAULT = 1,

    /**
     * Statistic event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Statistic event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Statistic event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    STATISTIC = 2,

    /**
     * Security event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Security event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Security event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    SECURITY = 3,

    /**
     * Behavior event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Behavior event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Behavior event.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    BEHAVIOR = 4
  }

  /**
   * Provides domain name constants.
   *
   * @namespace domain
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Provides domain name constants.
   *
   * @namespace domain
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  namespace domain {
    /**
     * System domain.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * System domain.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    const OS: string;
  }

  /**
   * Provides event name constants, including system event name constants and application event name constants. 
   * <br>The application event name constants are optional custom event names reserved when you call Write for 
   * application event logging.
   *
   * @namespace event
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Provides event name constants, including system event name constants and application event name constants. 
   * <br>The application event name constants are optional custom event names reserved when you call Write for 
   * application event logging.
   *
   * @namespace event
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Provides event name constants, including system event name constants and application event name constants. 
   * <br>The application event name constants are optional custom event names reserved when you call Write for 
   * application event logging.
   *
   * @namespace event
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  namespace event {
    /**
     * User login event. This is a reserved application event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * User login event. This is a reserved application event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    const USER_LOGIN: string;

    /**
     * User logout event. This is a reserved application event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * User logout event. This is a reserved application event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    const USER_LOGOUT: string;

    /**
     * Distributed service startup event. This is a reserved application event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Distributed service startup event. This is a reserved application event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    const DISTRIBUTED_SERVICE_START: string;

    /**
     * Application crash event. This is a system event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Application crash event. This is a system event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    const APP_CRASH: string;

    /**
     * Application freeze event. This is a system event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    const APP_FREEZE: string;

    /**
     * Event indicating the application launch duration. This is a system event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    const APP_LAUNCH: string;

    /**
     * Event indicating frame loss during swiping. This is a system event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    const SCROLL_JANK: string;

    /**
     * Event indicating a high CPU usage. This is a system event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    const CPU_USAGE_HIGH: string;

    /**
     * Event indicating battery usage statistics. This is a system event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    const BATTERY_USAGE: string;

    /**
     * Event indicating an application resource leakage. This is a system event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    const RESOURCE_OVERLIMIT: string;

    /**
     * Address sanitizer event. This is a system event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    const ADDRESS_SANITIZER: string;

    /**
     * Main thread jank event. This is a system event name constant.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    const MAIN_THREAD_JANK: string;
  }

  /**
   * Provides parameter name constants.
   *
   * @namespace param
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Provides parameter name constants.
   *
   * @namespace param
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Provides parameter name constants.
   *
   * @namespace param
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  namespace param {
    /**
     * Custom user ID.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Custom user ID.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Custom user ID.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    const USER_ID: string;

    /**
     * Distributed service name.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Distributed service name.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Distributed service name.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    const DISTRIBUTED_SERVICE_NAME: string;

    /**
     * Distributed service instance ID.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Distributed service instance ID.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Distributed service instance ID.
     *
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    const DISTRIBUTED_SERVICE_INSTANCE_ID: string;
  }

  /**
   * Configures the application event logging function, such as setting the logging switch and directory storage quota.
   *
   * @param { ConfigOption } config Configuration items for application event logging.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 11103001 - Invalid max storage quota value.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Configures the application event logging function, such as setting the logging switch and directory storage quota.
   *
   * @param { ConfigOption } config Configuration items for application event logging.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11103001 - Invalid max storage quota value.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Configures the application event logging function, such as setting the logging switch and directory storage quota.
   *
   * @param { ConfigOption } config Configuration items for application event logging.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11103001 - Invalid max storage quota value.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function configure(config: ConfigOption): void;

  /**
   * Provides configuration options for application event logging.
   *
   * @interface ConfigOption
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Provides configuration options for application event logging.
   *
   * @interface ConfigOption
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Provides configuration options for application event logging.
   *
   * @interface ConfigOption
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  interface ConfigOption {
    /**
     * Whether to enable the event logging function. The default value is false. 
     * <br>If this parameter is set to true, the logging function is disabled. Otherwise, the logging function is 
     * enabled.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Whether to enable the event logging function. The default value is false. 
     * <br>If this parameter is set to true, the logging function is disabled. Otherwise, the logging function is 
     * enabled.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Whether to enable the event logging function. The default value is false. 
     * <br>If this parameter is set to true, the logging function is disabled. Otherwise, the logging function is 
     * enabled.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    disable?: boolean;

    /**
     * Quota for the directory that stores event logging files. The default value is 10M. 
     * <br>It is recommended that the quota be less than or equal to 10 MB. Otherwise, the API efficiency may be 
     * affected. If the directory size exceeds the specified quota when application event logging is performed, event 
     * logging files in the directory will be cleared one by one based on the generation time to ensure that directory 
     * size does not exceed the quota.
     * <br>The quota value must meet the following requirements:
     * <br>- The quota value consists of only digits and a unit (which can be one of [b|k|kb|m|mb|g|gb|t|tb], which are 
     * case insensitive.)
     * <br>- The quota value must start with a digit. You can determine whether to pass the unit. If the unit is left 
     * empty, b (that is, byte) is used by default.
     *
     * @type { ?string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Quota for the directory that stores event logging files. The default value is 10M. 
     * <br>It is recommended that the quota be less than or equal to 10 MB. Otherwise, the API efficiency may be 
     * affected. If the directory size exceeds the specified quota when application event logging is performed, event 
     * logging files in the directory will be cleared one by one based on the generation time to ensure that directory 
     * size does not exceed the quota.
     * <br>The quota value must meet the following requirements:
     * <br>- The quota value consists of only digits and a unit (which can be one of [b|k|kb|m|mb|g|gb|t|tb], which are 
     * case insensitive.)
     * <br>- The quota value must start with a digit. You can determine whether to pass the unit. If the unit is left 
     * empty, b (that is, byte) is used by default.
     *
     * @type { ?string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    maxStorage?: string;
  }

  /**
   * Defines parameters of the event information.
   *
   * @interface AppEventInfo
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Defines parameters of the event information.
   *
   * @interface AppEventInfo
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Defines parameters of the event information.
   *
   * @interface AppEventInfo
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  interface AppEventInfo {
    /**
     * Event domain. The value is a string of up to 32 characters, including digits (0 to 9), letters (a to z), and 
     * underscores (_). It must start with a letter and cannot end with an underscore (_).
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Event domain. The value is a string of up to 32 characters, including digits (0 to 9), letters (a to z), and 
     * underscores (_). It must start with a letter and cannot end with an underscore (_).
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event domain. The value is a string of up to 32 characters, including digits (0 to 9), letters (a to z), and 
     * underscores (_). It must start with a letter and cannot end with an underscore (_).
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    domain: string;

    /**
     * Event name. The value is string that contains a maximum of 48 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_), and dollar sign ($). It must start with a letter or dollar sign ($) and end with a 
     * digit or letter.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Event name. The value is string that contains a maximum of 48 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_), and dollar sign ($). It must start with a letter or dollar sign ($) and end with a 
     * digit or letter.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event name. The value is string that contains a maximum of 48 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_), and dollar sign ($). It must start with a letter or dollar sign ($) and end with a 
     * digit or letter.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    name: string;

    /**
     * Event type.
     *
     * @type { EventType }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Event type.
     *
     * @type { EventType }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event type.
     *
     * @type { EventType }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    eventType: EventType;

    /**
     * Event parameter object, which consists of a parameter name and a parameter value. In system events, the fields 
     * contained in params are defined by system. For details about the fields, you can see the overviews of system 
     * events, for example, Crash Event Overview. For application events, you need to define the parameters of the 
     * Write API. The specifications are as follows:
     * <br>- A parameter name is a string that contains a maximum of 32 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_), and dollar sign ($). It must start with a letter or dollar sign ($) and end with a 
     * digit or letter. For example, testName and $123_name.
     * <br>- The parameter value can be a string, number, boolean, or array. The string type parameter can contain a 
     * maximum of 8 x 1024 characters. If the length exceeds the limit, the parameter and its name will be discarded. 
     * The value of the number type parameter must be within the range of Number.MIN_SAFE_INTEGER to 
     * Number.MAX_SAFE_INTEGER. If the value exceeds the range, an uncertain value may be generated. The element type 
     * in the array type parameter can only be string, number, or boolean. The number of elements must be less than 
     * 100. If this limit is exceeded, excess elements will be discarded.
     * <br>- The maximum number of parameters is 32. If this limit is exceeded, excess parameters will be discarded.
     *
     * @type { object }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Event parameter object, which consists of a parameter name and a parameter value. In system events, the fields 
     * contained in params are defined by system. For details about the fields, you can see the overviews of system 
     * events, for example, Crash Event Overview. For application events, you need to define the parameters of the 
     * Write API. The specifications are as follows:
     * <br>- A parameter name is a string that contains a maximum of 32 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_), and dollar sign ($). It must start with a letter or dollar sign ($) and end with a 
     * digit or letter. For example, testName and $123_name.
     * <br>- The parameter value can be a string, number, boolean, or array. The string type parameter can contain a 
     * maximum of 8 x 1024 characters. If the length exceeds the limit, the parameter and its name will be discarded. 
     * The value of the number type parameter must be within the range of Number.MIN_SAFE_INTEGER to 
     * Number.MAX_SAFE_INTEGER. If the value exceeds the range, an uncertain value may be generated. The element type 
     * in the array type parameter can only be string, number, or boolean. The number of elements must be less than 
     * 100. If this limit is exceeded, excess elements will be discarded.
     * <br>- The maximum number of parameters is 32. If this limit is exceeded, excess parameters will be discarded.
     *
     * @type { object }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event parameter object, which consists of a parameter name and a parameter value. In system events, the fields 
     * contained in params are defined by system. For details about the fields, you can see the overviews of system 
     * events, for example, Crash Event Overview. For application events, you need to define the parameters of the 
     * Write API. The specifications are as follows:
     * <br>- A parameter name is a string that contains a maximum of 32 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_), and dollar sign ($). It must start with a letter or dollar sign ($) and end with a 
     * digit or letter. For example, testName and $123_name.
     * <br>- The parameter value can be a string, number, boolean, or array. The string type parameter can contain a 
     * maximum of 8 x 1024 characters. If the length exceeds the limit, the parameter and its name will be discarded. 
     * The value of the number type parameter must be within the range of Number.MIN_SAFE_INTEGER to 
     * Number.MAX_SAFE_INTEGER. If the value exceeds the range, an uncertain value may be generated. The element type 
     * in the array type parameter can only be string, number, or boolean. The number of elements must be less than 
     * 100. If this limit is exceeded, excess elements will be discarded.
     * <br>- The maximum number of parameters is 32. If this limit is exceeded, excess parameters will be discarded.
     *
     * @type { object }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    params: object;
  }

  /**
   * Writes events of the AppEventInfo type. This API uses a promise to return the result. The event object written by 
   * calling this API is a custom object. To avoid conflicts with system events, you are not advised to write it to 
   * system events (system event name constants defined in Event). The events written by this API can be subscribed to 
   * through (addWatcher).
   *
   * @param { AppEventInfo } Application event object. You are advised to avoid the conflict between the custom event 
   * name and the system event name constant defined in Event.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 11100001 - Function disabled.
   * @throws { BusinessError } 11101001 - Invalid event domain.
   * @throws { BusinessError } 11101002 - Invalid event name.
   * @throws { BusinessError } 11101003 - Invalid number of event parameters.
   * @throws { BusinessError } 11101004 - Invalid string length of the event parameter.
   * @throws { BusinessError } 11101005 - Invalid event parameter name.
   * @throws { BusinessError } 11101006 - Invalid array length of the event parameter.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Writes events of the AppEventInfo type. This API uses a promise to return the result. The event object written by 
   * calling this API is a custom object. To avoid conflicts with system events, you are not advised to write it to 
   * system events (system event name constants defined in Event). The events written by this API can be subscribed to 
   * through (addWatcher).
   *
   * @param { AppEventInfo } info Application event object. You are advised to avoid the conflict between the custom 
   * event name and the system event name constant defined in Event.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11100001 - Function disabled.
   * @throws { BusinessError } 11101001 - Invalid event domain.
   * @throws { BusinessError } 11101002 - Invalid event name.
   * @throws { BusinessError } 11101003 - Invalid number of event parameters.
   * @throws { BusinessError } 11101004 - Invalid string length of the event parameter.
   * @throws { BusinessError } 11101005 - Invalid event parameter name.
   * @throws { BusinessError } 11101006 - Invalid array length of the event parameter.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Writes events of the AppEventInfo type. This API uses a promise to return the result. The event object written by 
   * calling this API is a custom object. To avoid conflicts with system events, you are not advised to write it to 
   * system events (system event name constants defined in Event). The events written by this API can be subscribed to 
   * through (addWatcher).
   *
   * @param { AppEventInfo } info Application event object. You are advised to avoid the conflict between the custom 
   * event name and the system event name constant defined in Event.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11100001 - Function disabled.
   * @throws { BusinessError } 11101001 - Invalid event domain.
   * @throws { BusinessError } 11101002 - Invalid event name.
   * @throws { BusinessError } 11101003 - Invalid number of event parameters.
   * @throws { BusinessError } 11101004 - Invalid string length of the event parameter.
   * @throws { BusinessError } 11101005 - Invalid event parameter name.
   * @throws { BusinessError } 11101006 - Invalid array length of the event parameter.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function write(info: AppEventInfo): Promise<void>;

  /**
   * Writes events of the AppEventInfo type. This API uses an asynchronous callback to return the result. The event 
   * object written by calling this API is a custom object. To avoid conflicts with system events, you are not advised 
   * to write it to system events (system event name constants defined in Event). The events written by this API can be 
   * subscribed to through (addWatcher).
   *
   * @param { AppEventInfo } info Application event object. You are advised to avoid the conflict between the custom 
   * event name and the system event name constant defined in Event.
   * @param { AsyncCallback<void> } callback Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 11100001 - Function disabled.
   * @throws { BusinessError } 11101001 - Invalid event domain.
   * @throws { BusinessError } 11101002 - Invalid event name.
   * @throws { BusinessError } 11101003 - Invalid number of event parameters.
   * @throws { BusinessError } 11101004 - Invalid string length of the event parameter.
   * @throws { BusinessError } 11101005 - Invalid event parameter name.
   * @throws { BusinessError } 11101006 - Invalid array length of the event parameter.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Writes events of the AppEventInfo type. This API uses an asynchronous callback to return the result. The event 
   * object written by calling this API is a custom object. To avoid conflicts with system events, you are not advised 
   * to write it to system events (system event name constants defined in Event). The events written by this API can be 
   * subscribed to through (addWatcher).
   *
   * @param { AppEventInfo } info Application event object. You are advised to avoid the conflict between the custom 
   * event name and the system event name constant defined in Event.
   * @param { AsyncCallback<void> } callback Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11100001 - Function disabled.
   * @throws { BusinessError } 11101001 - Invalid event domain.
   * @throws { BusinessError } 11101002 - Invalid event name.
   * @throws { BusinessError } 11101003 - Invalid number of event parameters.
   * @throws { BusinessError } 11101004 - Invalid string length of the event parameter.
   * @throws { BusinessError } 11101005 - Invalid event parameter name.
   * @throws { BusinessError } 11101006 - Invalid array length of the event parameter.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Writes events of the AppEventInfo type. This API uses an asynchronous callback to return the result. The event 
   * object written by calling this API is a custom object. To avoid conflicts with system events, you are not advised 
   * to write it to system events (system event name constants defined in Event). The events written by this API can be 
   * subscribed to through (addWatcher).
   *
   * @param { AppEventInfo } info Application event object. You are advised to avoid the conflict between the custom 
   * event name and the system event name constant defined in Event.
   * @param { AsyncCallback<void> } callback Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11100001 - Function disabled.
   * @throws { BusinessError } 11101001 - Invalid event domain.
   * @throws { BusinessError } 11101002 - Invalid event name.
   * @throws { BusinessError } 11101003 - Invalid number of event parameters.
   * @throws { BusinessError } 11101004 - Invalid string length of the event parameter.
   * @throws { BusinessError } 11101005 - Invalid event parameter name.
   * @throws { BusinessError } 11101006 - Invalid array length of the event parameter.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function write(info: AppEventInfo, callback: AsyncCallback<void>): void;

  /**
   * Type of a custom event parameter value.
   *
   * @typedef {number | string | boolean | Array<string>}
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 12
   */
  /**
   * Type of a custom event parameter value.
   *
   * @typedef {number | string | boolean | Array<string>}
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  type ParamType = number | string | boolean | Array<string>;

  /**
   * Sets custom event parameters. This API uses a promise to return the result. During the same lifecycle, system 
   * events and application events can be associated through event domain and event name. System events only support 
   * crash and freeze events.
   *
   * @param { Record<string, ParamType> } params Custom parameter object. The parameter name and value are defined as 
   * follows:
   * <br>- A parameter name is a string that contains a maximum of 32 characters, including digits (0 to 9), letters 
   * (a to z), underscore (_), and dollar sign ($). It must start with a letter or dollar sign ($) and end with a digit 
   * or letter.
   * <br>- The parameter value is of the ParamType and contains a maximum of 1024 characters.
   * <br>- The number of parameters must be less than 64.
   * @param { string } domain Event domain. The event domain can be associated with application events and system 
   * events (hiAppEvent.domain.OS).
   * @param { string } name Event name. The default value is an empty string, which indicates all event names in the 
   * associated event domain. The event name can be associated with application events and system events. System events 
   * can be associated only with crash events (hiAppEvent.event.APP_CRASH) and freeze events 
   * (hiAppEvent.event.APP_FREEZE).
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11101007 - The number of parameter keys exceeds the limit.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 12
   */
  /**
   * Sets custom event parameters. This API uses a promise to return the result. During the same lifecycle, system 
   * events and application events can be associated through event domain and event name. System events only support 
   * crash and freeze events.
   *
   * @param { Record<string, ParamType> } params Custom parameter object. The parameter name and value are defined as 
   * follows:
   * <br>- A parameter name is a string that contains a maximum of 32 characters, including digits (0 to 9), letters 
   * (a to z), underscore (_), and dollar sign ($). It must start with a letter or dollar sign ($) and end with a digit 
   * or letter.
   * <br>- The parameter value is of the ParamType and contains a maximum of 1024 characters.
   * <br>- The number of parameters must be less than 64.
   * @param { string } domain Event domain. The event domain can be associated with application events and system 
   * events (hiAppEvent.domain.OS).
   * @param { string } name Event name. The default value is an empty string, which indicates all event names in the 
   * associated event domain. The event name can be associated with application events and system events. System events 
   * can be associated only with crash events (hiAppEvent.event.APP_CRASH) and freeze events 
   * (hiAppEvent.event.APP_FREEZE).
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11101007 - The number of parameter keys exceeds the limit.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function setEventParam(params: Record<string, ParamType>, domain: string, name?: string): Promise<void>;

  /**
   * Sets the custom threshold triggering condition for an event. This API uses a promise to return the result. In the 
   * same lifecycle, you can customize the parameters related to the event threshold triggering condition based on the 
   * event name. Currently, only the MAIN_THREAD_JANK event is supported. For details about the parameter configuration, 
   * see Main Thread Jank Event Overview.
   *
   * @param { string } name Event name.
   * @param { Record<string, ParamType> } config Custom parameter object. The parameter name and value are defined as 
   * follows:
   * <br>- The parameter name contains a maximum of 1024 characters, which is of the string type and cannot be empty.
   * <br>- The parameter value is of the ParamType and contains a maximum of 1024 characters.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3.Parameter verification failed.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 15
   */
  function setEventConfig(name: string, config: Record<string, ParamType>): Promise<void>;

  /**
   * Defines parameters of an AppEventPackage object. This API is used to obtain detail information about an event 
   * package, which is obtained using the takeNext API.
   *
   * @interface AppEventPackage
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Defines parameters of an AppEventPackage object. This API is used to obtain detail information about an event 
   * package, which is obtained using the takeNext API.
   *
   * @interface AppEventPackage
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Defines parameters of an AppEventPackage object. This API is used to obtain detail information about an event 
   * package, which is obtained using the takeNext API.
   *
   * @interface AppEventPackage
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  interface AppEventPackage {
    /**
     * Event package ID, which is named from 0 in ascending order.
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Event package ID, which is named from 0 in ascending order.
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event package ID, which is named from 0 in ascending order.
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    packageId: number;

    /**
     * Number of events in the event package.
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Number of events in the event package.
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Number of events in the event package.
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    row: number;

    /**
     * Event size of the event package, in bytes.
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Event size of the event package, in bytes.
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event size of the event package, in bytes.
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    size: number;

    /**
     * Event data in the event package.
     *
     * @type { string[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Event data in the event package.
     *
     * @type { string[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event data in the event package.
     *
     * @type { string[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    data: string[];

    /**
     * Event object group.
     *
     * @type { Array<AppEventInfo> }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    /**
     * Event object group.
     *
     * @type { Array<AppEventInfo> }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    appEventInfos: Array<AppEventInfo>;
  }

  /**
   * Defines a subscription data holder for processing event information.
   *
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Defines a subscription data holder for processing event information.
   *
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Defines a subscription data holder for processing event information.
   *
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  class AppEventPackageHolder {
    /**
     * Constructs an AppEventPackageHolder instance. You can call addWatcher to add an event watcher, and then 
     * associate the AppEventPackageHolder instance with the watcher added in the application based on the watcher name.
     *
     * @param { string } watcherName Name of the event watcher added through addWatcher. If no watcher is added, no 
     * data is displayed by default.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Constructs an AppEventPackageHolder instance. You can call addWatcher to add an event watcher, and then 
     * associate the AppEventPackageHolder instance with the watcher added in the application based on the watcher name.
     *
     * @param { string } watcherName Name of the event watcher added through addWatcher. If no watcher is added, no 
     * data is displayed by default.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Constructs an AppEventPackageHolder instance. You can call addWatcher to add an event watcher, and then 
     * associate the AppEventPackageHolder instance with the watcher added in the application based on the watcher name.
     *
     * @param { string } watcherName Name of the event watcher added through addWatcher. If no watcher is added, no 
     * data is displayed by default.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    constructor(watcherName: string);

    /**
     * Sets the threshold for the data size of the event package obtained each time.
     *
     * @param { number } size Data size threshold, in bytes. The value range is [0, $2^{31}$-1]. If the value is out of 
     * the range, an exception is thrown.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 11104001 - Invalid size value.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Sets the threshold for the data size of the event package obtained each time.
     *
     * @param { number } size Data size threshold, in bytes. The value range is [0, $2^{31}$-1]. If the value is out of 
     * the range, an exception is thrown.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError } 11104001 - Invalid size value.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Sets the threshold for the data size of the event package obtained each time.
     *
     * @param { number } size Data size threshold, in bytes. The value range is [0, $2^{31}$-1]. If the value is out of 
     * the range, an exception is thrown.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError } 11104001 - Invalid size value.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    setSize(size: number): void;

    /**
     * Sets the number of data records of the event package obtained each time. When setRow() and setSize() are called 
     * at the same time, only setRow() takes effect.
     *
     * @param { number } size Number of events. The value range is (0, $2^{31}$-1]. If the value is out of the range, an 
     * exception is thrown.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError } 11104001 - Invalid size value.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    /**
     * Sets the number of data records of the event package obtained each time. When setRow() and setSize() are called 
     * at the same time, only setRow() takes effect.
     *
     * @param { number } size Number of events. The value range is (0, $2^{31}$-1]. If the value is out of the range, an 
     * exception is thrown.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError } 11104001 - Invalid size value.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    setRow(size: number): void;

    /**
     * Obtains the subscription event.
     * <br>The system obtains the subscription event data based on the data size threshold specified by setSize or the 
     * number of data records specified by setRow. By default, one subscription event data record is obtained. When all 
     * subscription event data is obtained, null is returned.
     * <br>When setRow and setSize are called at the same time, only setRow takes effect.
     *
     * @returns { AppEventPackage } Event package object. If all subscription event data has been retrieved, null is 
     * returned.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Obtains the subscription event.
     * <br>The system obtains the subscription event data based on the data size threshold specified by setSize or the 
     * number of data records specified by setRow. By default, one subscription event data record is obtained. When all 
     * subscription event data is obtained, null is returned.
     * <br>When setRow and setSize are called at the same time, only setRow takes effect.
     *
     * @returns { AppEventPackage } Event package object. If all subscription event data has been retrieved, null is 
     * returned.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Obtains the subscription event.
     * <br>The system obtains the subscription event data based on the data size threshold specified by setSize or the 
     * number of data records specified by setRow. By default, one subscription event data record is obtained. When all 
     * subscription event data is obtained, null is returned.
     * <br>When setRow and setSize are called at the same time, only setRow takes effect.
     *
     * @returns { AppEventPackage } Event package object. If all subscription event data has been retrieved, null is 
     * returned.
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    takeNext(): AppEventPackage;
  }

  /**
   * Defines the triggering condition parameters of the onTrigger callback of a Watcher.
   *
   * @interface TriggerCondition
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Defines the triggering condition parameters of the onTrigger callback of a Watcher.
   *
   * @interface TriggerCondition
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Defines the triggering condition parameters of the onTrigger callback of a Watcher.
   *
   * @interface TriggerCondition
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  interface TriggerCondition {
    /**
     * Total number of events that trigger callback. The value is a positive integer. The default value is 0, 
     * indicating that no callback is triggered. If this parameter is set to a negative value, the default 
     * value is used.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Total number of events that trigger callback. The value is a positive integer. The default value is 0, 
     * indicating that no callback is triggered. If this parameter is set to a negative value, the default 
     * value is used.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Total number of events that trigger callback. The value is a positive integer. The default value is 0, 
     * indicating that no callback is triggered. If this parameter is set to a negative value, the default 
     * value is used.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    row?: number;

    /**
     * Total size of events that trigger callback. The value is a positive integer, in bytes. The default value is 0, 
     * indicating that no callback is triggered. If this parameter is set to a negative value, the default value is 
     * used.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Total size of events that trigger callback. The value is a positive integer, in bytes. The default value is 0, 
     * indicating that no callback is triggered. If this parameter is set to a negative value, the default value is 
     * used.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Total size of events that trigger callback. The value is a positive integer, in bytes. The default value is 0, 
     * indicating that no callback is triggered. If this parameter is set to a negative value, the default value is 
     * used.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    size?: number;

    /**
     * Timeout interval for triggering callback. The value is a positive integer, in unit of 30s. The default value is 
     * 0, indicating that no callback is triggered. If this parameter is set to a negative value, the default value is 
     * used.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Timeout interval for triggering callback. The value is a positive integer, in unit of 30s. The default value is 
     * 0, indicating that no callback is triggered. If this parameter is set to a negative value, the default value is 
     * used.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Timeout interval for triggering callback. The value is a positive integer, in unit of 30s. The default value is 
     * 0, indicating that no callback is triggered. If this parameter is set to a negative value, the default value is 
     * used.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    timeOut?: number;
  }

  /**
   * Defines parameters of subscription filtering conditions of a Watcher. This API is used to set event filtering 
   * conditions in the event watcher to ensure that only the events that meet the filtering conditions are subscribed 
   * to.
   *
   * @interface AppEventFilter
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Defines parameters of subscription filtering conditions of a Watcher. This API is used to set event filtering 
   * conditions in the event watcher to ensure that only the events that meet the filtering conditions are subscribed 
   * to.
   *
   * @interface AppEventFilter
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Defines parameters of subscription filtering conditions of a Watcher. This API is used to set event filtering 
   * conditions in the event watcher to ensure that only the events that meet the filtering conditions are subscribed 
   * to.
   *
   * @interface AppEventFilter
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  interface AppEventFilter {
    /**
     * Event domain, which can be the system event domain (hiAppEvent.domain.OS) or the event domain of the custom 
     * event information (AppEventInfo) passed through the Write API.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Event domain, which can be the system event domain (hiAppEvent.domain.OS) or the event domain of the custom 
     * event information (AppEventInfo) passed through the Write API.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event domain, which can be the system event domain (hiAppEvent.domain.OS) or the event domain of the custom 
     * event information (AppEventInfo) passed through the Write API.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    domain: string;

    /**
     * Event types. If this parameter is not set, events are not filtered by default.
     *
     * @type { ?EventType[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Event types. If this parameter is not set, events are not filtered by default.
     *
     * @type { ?EventType[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event types. If this parameter is not set, events are not filtered by default.
     *
     * @type { ?EventType[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    eventTypes?: EventType[];

    /**
     * Names of the events to be subscribed. If this parameter is not set, events are not filtered by default.
     *
     * @type { ?string[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Names of the events to be subscribed. If this parameter is not set, events are not filtered by default.
     *
     * @type { ?string[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    names?: string[];
  }

  /**
   * Defines parameters of the event group returned by a subscription. This API can be used to obtain detail 
   * information about an event group, which is often used in the onReceive callback of Watcher.
   *
   * @interface AppEventGroup
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Defines parameters of the event group returned by a subscription. This API can be used to obtain detail 
   * information about an event group, which is often used in the onReceive callback of Watcher.
   *
   * @interface AppEventGroup
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  interface AppEventGroup {
    /**
     * Event name.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event name.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    name: string;

    /**
     * Event object group.
     *
     * @type { Array<AppEventInfo> }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Event object group.
     *
     * @type { Array<AppEventInfo> }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    appEventInfos: Array<AppEventInfo>;
  }

  /**
   * Defines parameters for a Watcher object. This API is used to configure and manage event watchers to subscribe to 
   * and process specified events.
   *
   * @interface Watcher
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Defines parameters for a Watcher object. This API is used to configure and manage event watchers to subscribe to 
   * and process specified events.
   *
   * @interface Watcher
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Defines parameters for a Watcher object. This API is used to configure and manage event watchers to subscribe to 
   * and process specified events.
   *
   * @interface Watcher
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  interface Watcher {
    /**
     * Unique name of a watcher. The value contains a maximum of 32 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_). It must start with a letter and end with a digit or letter. For example, testName1 
     * and crash_Watcher.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Unique name of a watcher. The value contains a maximum of 32 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_). It must start with a letter and end with a digit or letter. For example, testName1 
     * and crash_Watcher.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Unique name of a watcher. The value contains a maximum of 32 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_). It must start with a letter and end with a digit or letter. For example, testName1 
     * and crash_Watcher.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    name: string;

    /**
     * Subscription callback triggering condition. This parameter takes effect only when it is passed together with 
     * onTrigger. If this parameter is not set, the onTrigger callback is not triggered by default.
     *
     * @type { ?TriggerCondition }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Subscription callback triggering condition. This parameter takes effect only when it is passed together with 
     * onTrigger. If this parameter is not set, the onTrigger callback is not triggered by default.
     *
     * @type { ?TriggerCondition }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Subscription callback triggering condition. This parameter takes effect only when it is passed together with 
     * onTrigger. If this parameter is not set, the onTrigger callback is not triggered by default.
     *
     * @type { ?TriggerCondition }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    triggerCondition?: TriggerCondition;

    /**
     * Subscription filtering condition. This parameter is passed only when subscription events need to be filtered. 
     * If this parameter is not set, events are not filtered by default.
     *
     * @type { ?AppEventFilter[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Subscription filtering condition. This parameter is passed only when subscription events need to be filtered. 
     * If this parameter is not set, events are not filtered by default.
     *
     * @type { ?AppEventFilter[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Subscription filtering condition. This parameter is passed only when subscription events need to be filtered. 
     * If this parameter is not set, events are not filtered by default.
     *
     * @type { ?AppEventFilter[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    appEventFilters?: AppEventFilter[];

    /**
     * Subscription callback. This parameter takes effect only when it is passed together with triggerCondition. 
     * The input arguments are described as follows: 
     * <br>curRow: total number of subscription events when the callback is triggered.
     * <br>curSize: total size of subscribed events when the callback is triggered, in bytes.
     * <br>holder: subscription data holder, which can be used to process subscribed events.
     *
     * @type { ?function }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @since 9
     */
    /**
     * Subscription callback. This parameter takes effect only when it is passed together with triggerCondition. 
     * The input arguments are described as follows: 
     * <br>curRow: total number of subscription events when the callback is triggered.
     * <br>curSize: total size of subscribed events when the callback is triggered, in bytes.
     * <br>holder: subscription data holder, which can be used to process subscribed events.
     *
     * @type { ?function }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Subscription callback. This parameter takes effect only when it is passed together with triggerCondition. 
     * The input arguments are described as follows: 
     * <br>curRow: total number of subscription events when the callback is triggered.
     * <br>curSize: total size of subscribed events when the callback is triggered, in bytes.
     * <br>holder: subscription data holder, which can be used to process subscribed events.
     *
     * @type { ?function }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    onTrigger?: (curRow: number, curSize: number, holder: AppEventPackageHolder) => void;

    /**
     * Real-time subscription callback. Only this callback function is triggered if it is passed together with 
     * onTrigger. The input arguments are described as follows: 
     * <br>domain: domain name.
     * <br>appEventGroups: event group.
     *
     * @type { ?function }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    /**
     * Real-time subscription callback. Only this callback function is triggered if it is passed together with 
     * onTrigger. The input arguments are described as follows: 
     * <br>domain: domain name.
     * <br>appEventGroups: event group.
     *
     * @type { ?function }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @crossplatform
     * @atomicservice
     * @since 19
     */
    onReceive?: (domain: string, appEventGroups: Array<AppEventGroup>) => void;
  }

  /**
   * Adds an event watcher. You can use the callback of the event watcher to subscribe to events.
   *
   * @param { Watcher } watcher Event watcher.
   * @returns { AppEventPackageHolder } Subscription data holder. If the subscription fails, null is returned.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 11102001 - Invalid watcher name.
   * @throws { BusinessError } 11102002 - Invalid filtering event domain.
   * @throws { BusinessError } 11102003 - Invalid row value.
   * @throws { BusinessError } 11102004 - Invalid size value.
   * @throws { BusinessError } 11102005 - Invalid timeout value.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Adds an event watcher. You can use the callback of the event watcher to subscribe to events.
   *
   * @param { Watcher } watcher Event watcher.
   * @returns { AppEventPackageHolder } Subscription data holder. If the subscription fails, null is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11102001 - Invalid watcher name.
   * @throws { BusinessError } 11102002 - Invalid filtering event domain.
   * @throws { BusinessError } 11102003 - Invalid row value.
   * @throws { BusinessError } 11102004 - Invalid size value.
   * @throws { BusinessError } 11102005 - Invalid timeout value.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Adds an event watcher. You can use the callback of the event watcher to subscribe to events.
   *
   * @param { Watcher } watcher Event watcher.
   * @returns { AppEventPackageHolder } Subscription data holder. If the subscription fails, null is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11102001 - Invalid watcher name.
   * @throws { BusinessError } 11102002 - Invalid filtering event domain.
   * @throws { BusinessError } 11102003 - Invalid row value.
   * @throws { BusinessError } 11102004 - Invalid size value.
   * @throws { BusinessError } 11102005 - Invalid timeout value.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function addWatcher(watcher: Watcher): AppEventPackageHolder;

  /**
   * Removes an event watcher.
   *
   * @param { Watcher } watcher Event watcher.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 11102001 - Invalid watcher name.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Removes an event watcher.
   *
   * @param { Watcher } watcher Event watcher.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11102001 - Invalid watcher name.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Removes an event watcher.
   *
   * @param { Watcher } watcher Event watcher.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 11102001 - Invalid watcher name.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function removeWatcher(watcher: Watcher): void;

  /**
   * Clears local logging data of the application.
   *
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @since 9
   */
  /**
   * Clears local logging data of the application.
   *
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Clears local logging data of the application.
   *
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function clearData(): void;

  /**
   * Sets a user ID, which is used for association when a Processor is configured.
   *
   * @param { string } name Key of a user ID. The value is string that contains a maximum of 256 characters, including 
   * digits (0 to 9), letters (a to z), underscore (_), and dollar sign ($). It must not start with a digit.
   * @param { string } value Value of a user ID. It can contain a maximum of 256 characters. If the value is null or 
   * left empty, the user ID is cleared.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Sets a user ID, which is used for association when a Processor is configured.
   *
   * @param { string } name Key of a user ID. The value is string that contains a maximum of 256 characters, including 
   * digits (0 to 9), letters (a to z), underscore (_), and dollar sign ($). It must not start with a digit.
   * @param { string } value Value of a user ID. It can contain a maximum of 256 characters. If the value is null or 
   * left empty, the user ID is cleared.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function setUserId(name: string, value: string): void;

  /**
   * Obtains the value set through setUserId.
   *
   * @param { string } name Key of a user ID. The value is string that contains a maximum of 256 characters, including 
   * digits (0 to 9), letters (a to z), underscore (_), and dollar sign ($). It must not start with a digit.
   * @returns { string } Value of a user ID. If no user ID is found, an empty string is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Obtains the value set through setUserId.
   *
   * @param { string } name Key of a user ID. The value is string that contains a maximum of 256 characters, including 
   * digits (0 to 9), letters (a to z), underscore (_), and dollar sign ($). It must not start with a digit.
   * @returns { string } Value of a user ID. If no user ID is found, an empty string is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function getUserId(name: string): string;

  /**
   * Sets a user property, which is used for association when a Processor is configured.
   *
   * @param { string } name Key of a user property. The value is string that contains a maximum of 256 characters, 
   * including digits (0 to 9), letters (a to z), underscore (_), and dollar sign ($). It must not start with a digit.
   * @param { string } value Value of a user property. It can contain a maximum of 1024 characters. If the value is 
   * null or left empty, the user property is cleared.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Sets a user property, which is used for association when a Processor is configured.
   *
   * @param { string } name Key of a user property. The value is string that contains a maximum of 256 characters, 
   * including digits (0 to 9), letters (a to z), underscore (_), and dollar sign ($). It must not start with a digit.
   * @param { string } value Value of a user property. It can contain a maximum of 1024 characters. If the value is 
   * null or left empty, the user property is cleared.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function setUserProperty(name: string, value: string): void;

  /**
   * Obtains the value set through setUserProperty.
   *
   * @param { string } name Key of a user property. The value is string that contains a maximum of 256 characters, 
   * including digits (0 to 9), letters (a to z), underscore (_), and dollar sign ($). It must not start with a digit.
   * @returns { string } Value of a user property. If no user ID is found, an empty string is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  /**
   * Obtains the value set through setUserProperty.
   *
   * @param { string } name Key of a user property. The value is string that contains a maximum of 256 characters, 
   * including digits (0 to 9), letters (a to z), underscore (_), and dollar sign ($). It must not start with a digit.
   * @returns { string } Value of a user property. If no user ID is found, an empty string is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function getUserProperty(name: string): string;

  /**
   * Event description configuration that can be reported by the data processor
   *
   * @interface AppEventReportConfig
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  interface AppEventReportConfig {
    /**
     * Event domain. The value is a string that contains a maximum of 32 characters, including digits (0 to 9), letters 
     * (a to z), and underscore (_). It must start with a letter and cannot end with an underscore (_). The default 
     * value is an empty string.
     *
     * @type { ?string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    domain?: string;

    /**
     * Event name. The value is string that contains a maximum of 48 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_), and dollar sign ($). It must start with a letter or dollar sign ($) and end with a 
     * digit or letter. The default value is an empty string.
     *
     * @type { ?string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    name?: string;

    /**
     * Whether to report events in real time. The value true indicates that events are reported in real time, and the 
     * value false indicates the opposite. The default value is false.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    isRealTime?: boolean;
  }

  /**
   * Defines a data processor for reporting and managing events. You can customize processor configurations as required.
   *
   * @interface Processor
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  interface Processor {
    /**
     * Name of a data processor. The value is string that contains a maximum of 256 characters, including digits (0 to 
     * 9), letters (a to z), underscore (_), and dollar sign ($). It must not start with a digit.
     *
     * @type { string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    name: string;

    /**
     * Whether to enable the debug mode. The default value is false. The value true means to enable the debugging mode, 
     * and the value false means the opposite.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    debugMode?: boolean;

    /**
     * Server location information. It is left empty by default. The length of the input string cannot exceed 8 KB. If 
     * the length exceeds 8 KB, the default value is used.
     *
     * @type { ?string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    routeInfo?: string;

    /**
     * Application ID. It is left empty by default. The length of the input string cannot exceed 8 KB. If the length 
     * exceeds 8 KB, the default value is used.
     *
     * @type { ?string }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    appId?: string;

    /**
     * Whether to report an event when the data processor starts. The default value is false. The value true means to 
     * report events, and the value false means the opposite.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    onStartReport?: boolean;

    /**
     * Whether to report an event when an application switches to the background. The default value is false. The value 
     * true means to report events, and the value false means the opposite.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    onBackgroundReport?: boolean;

    /**
     * Interval for event reporting, in seconds. The input value must be greater than or equal to 0. If the input value 
     * is less than 0, the default value 0 is used and periodic reporting is not performed.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    periodReport?: number;

    /**
     * Event reporting threshold. When the number of events reaches the threshold, an event is reported. The value must 
     * be greater than 0 and less than 1000. If the value is not within the range, the default value 0 is used and no 
     * events are reported.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    batchReport?: number;

    /**
     * Name array of user IDs that can be reported by the data processor. name corresponds to the name parameter of the 
     * setUserId API. The default value is an empty array.
     *
     * @type { ?string[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    userIds?: string[];

    /**
     * Name array of user properties that can be reported by the data processor. name corresponds to the name parameter 
     * of the setUserProperty API. The default value is an empty array.
     *
     * @type { ?string[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    userProperties?: string[];

    /**
     * Event description configuration array that can be reported by the data processor. The default value is an empty 
     * array.
     *
     * @type { ?AppEventReportConfig[] }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 11
     */
    eventConfigs?: AppEventReportConfig[];

    /**
     * Configuration ID for data processor. The input value must be greater than or equal to 0. If the input value is 
     * less than 0, the default value 0 is used. If the input value is greater than 0, the value uniquely identifies a 
     * data processor with its name.
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    configId?: number;

    /**
     * Custom extended parameters. If the input parameter name and value do not meet the specifications, extended 
     * parameters are not configured by default. The specifications are as follows:
     * <br>- A parameter name is a string that contains a maximum of 32 characters, including digits (0 to 9), letters 
     * (a to z), underscore (_), and dollar sign ($). It must start with a letter or dollar sign ($) and end with a 
     * digit or letter.
     * <br>- A parameter value is a string contains a maximum of 1024 characters.
     * <br>- The number of parameters must be less than 32.
     *
     * @type { ?Record<string, string> }
     * @syscap SystemCapability.HiviewDFX.HiAppEvent
     * @atomicservice
     * @since 12
     */
    customConfigs?: Record<string, string>;
  }

  /**
   * Adds a data processor to migrate event data to the cloud. You can preset the implementation of the processor on 
   * the device and set its properties based on its constraints.
   * <br>The configuration information of Processor must be provided by the data processor. Yet, as no data processor 
   * is preset in the device for interaction for the moment, migrating events to the cloud is unavailable.
   *
   * @param { Processor } processor Data processor.
   * @returns { number }  ID of the data processor of the reported event, which uniquely identifies the data processor 
   * and can be used to remove the data processor. If the operation fails, -1 is returned. If the operation is 
   * successful, a value greater than 0 is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  function addProcessor(processor: Processor): number;

  /**
   * Removes the data processor of a reported event.
   *
   * @param { number } id ID of a data processor. The value must be greater than 0. The value is obtained by calling 
   * addProcessor.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @static
   * @syscap SystemCapability.HiviewDFX.HiAppEvent
   * @atomicservice
   * @since 11
   */
  function removeProcessor(id: number): void;
}

export default hiAppEvent;
