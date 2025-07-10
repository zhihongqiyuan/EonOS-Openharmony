/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
 * @kit ArkTS
 */

/**
 * JS cross-thread task executor.
 *
 * @namespace taskpool
 * @syscap SystemCapability.Utils.Lang
 * @since 9
 */
/**
 * JS cross-thread task executor.
 *
 * @namespace taskpool
 * @syscap SystemCapability.Utils.Lang
 * @crossplatform
 * @since 10
 */
/**
 * JS cross-thread task executor.
 *
 * @namespace taskpool
 * @syscap SystemCapability.Utils.Lang
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare namespace taskpool {
  /**
   * The Priority defines the task priority.
   *
   * @enum { number } Priority
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * The Priority defines the task priority.
   *
   * @enum { number } Priority
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Enumerates the priorities available for created tasks. 
   *
   * @enum { number } Priority
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  enum Priority {
    /**
     * set task priority to high.
     *
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * set task priority to high.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * The task has a high priority.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    HIGH = 0,

    /**
     * set task priority to medium.
     *
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * set task priority to medium.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * The task has a medium priority.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    MEDIUM = 1,

    /**
     * set task priority to low.
     *
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * set task priority to low.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * The task has a low priority.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    LOW = 2,
    /**
     * The task is a background task.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    IDLE = 3
  }

  /**
   * Describes a callback function.
   *
   * @typedef { function } CallbackFunction
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  type CallbackFunction = () => void;

  /**
   * Describes a callback function with an error message.
   *
   * @typedef { function } CallbackFunctionWithError
   * @param { Error } e - Error message.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  type CallbackFunctionWithError = (e: Error) => void;

  /**
   * The Task class provides an interface to create a task.
   *
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * The Task class provides an interface to create a task.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Implements a task. Before calling any APIs in Task, you must use constructor to create a Task instance.
   * A task can be executed for multiple times, placed in a task group, serial queue, or asynchronous queue for execution,
   * or added with dependencies for execution.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  class Task {
    /**
     * Create a Task instance.
     *
     * @param { Function } func - func func Concurrent function to execute in taskpool.
     * @param { unknown[] } args - args args The concurrent function arguments.
     * @throws { BusinessError } 401 - The input parameters are invalid.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Create a Task instance.
     *
     * @param { Function } func - func func Concurrent function to execute in taskpool.
     * @param { unknown[] } args - args args The concurrent function arguments.
     * @throws { BusinessError } 401 - The input parameters are invalid.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * A constructor used to create a Task instance.
     *
     * @param { Function } func - Function to be executed. The function must be decorated using @Concurrent.
     *      For details about the supported return value types of the function, see Sequenceable Data Types.
     * @param { Object[] } args - Arguments of the function. For details about the supported parameter types,
     *      see Sequenceable Data Types. The default value is undefined.
     * @throws { BusinessError } 401 - The input parameters are invalid.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    constructor(func: Function, ...args: Object[]);

    /**
     * A constructor used to create a Task instance, with the task name specified.
     *
     * @param { string } name - Task name.
     * @param { Function } func - Function to be executed. The function must be decorated using @Concurrent.
     *     For details about the supported return value types of the function, see Sequenceable Data Types. 
     * @param { Object[] } args - Arguments of the function. For details about the supported parameter types,
     *     see Sequenceable Data Types. The default value is undefined.
     * @throws { BusinessError } 401 - The input parameters are invalid.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    constructor(name: string, func: Function, ...args: Object[]);

    /**
     * Check current running Task is canceled or not.
     *
     * @returns { boolean } Returns {@code true} if current running task is canceled; returns {@code false} otherwise.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Checks whether the running task is canceled. Before using this API, you must create a Task instance.
     * isCanceled must be used together with taskpool.cancel. If cancel is not called, isCanceled returns false by default.
     *
     * @returns { boolean } Returns {@code true} if current running task is canceled; returns {@code false} otherwise.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    static isCanceled(): boolean;

    /**
     * Sends data to the host thread and triggers the registered callback. Before using this API, you must create a Task instance.
     * NOTE:
     * 1.The API is called in the TaskPool thread.
     * 2.Do not use this API in a callback function.
     * 3.Before calling this API, ensure that the callback function for processing data has been registered in the host thread.
     *
     * @param { Object[] } args - Data to be used as the input parameter of the registered callback. For details about
     *     the supported parameter types, see Sequenceable Data Types. The default value is undefined.
     * @throws { BusinessError } 401 - The input parameters are invalid.
     * @throws { BusinessError } 10200006 - An exception occurred during serialization.
     * @throws { BusinessError } 10200022 - The function is not called in the TaskPool thread.
     * @throws { BusinessError } 10200023 - The function is not called in the concurrent function.
     * @throws { BusinessError } 10200024 - The callback is not registered on the host side.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    static sendData(...args: Object[]): void;

    /**
     * Set transfer list for this task.
     *
     * @param { ArrayBuffer[] } [transfer] - transfer Transfer list of this task, empty array is default.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Sets the task transfer list. Before using this API, you must create a Task instance. If this API is not called,
     * the ArrayBuffer in the data is transferred by default.
     * NOTE:
     * This API is used to set the task transfer list in the form of ArrayBuffer in the task pool. 
     * The ArrayBuffer instance does not copy the content in the task to the worker thread during transfer. 
     * Instead, it transfers the buffer control right to the worker thread. After the transfer, the ArrayBuffer instance
     * becomes invalid. An empty ArrayBuffer will not be transferred.
     *
     * @param { ArrayBuffer[] } [transfer] - ArrayBuffer instance holding the objects to transfer. The default value is an empty array.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
     * @throws { BusinessError } 10200029 - An ArrayBuffer cannot be set as both a transfer list and a clone list.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    setTransferList(transfer?: ArrayBuffer[]): void;

    /**
     * Sets the task clone list. Before using this API, you must create a Task instance.
     * NOTE:
     * This API must be used together with the @Sendable decorator. Otherwise, an exception is thrown.
     *
     * @param { Object[] | ArrayBuffer[] } cloneList - The type of the passed-in array must be sendable data types or ArrayBuffer.
     *     All Sendable class instances or ArrayBuffer objects passed in to cloneList are transferred in copy mode between threads.
     *     This means that any modification to the destination objects does not affect the original objects.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200029 - An ArrayBuffer cannot be set as both a transfer list and a clone list.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    setCloneList(cloneList: Object[] | ArrayBuffer[]): void;

    /**
     * Registers a callback for a task to receive and process data from the worker thread. Before using this API, you must create a Task instance.
     * NOTE:
     * If multiple callbacks are registered for the same task, only the last registration takes effect.
     *
     * @param { Function } [callback] - Callback function for processing the data received. The data sent to the host
     *     thread is transferred to the callback as an input parameter. If no value is passed in, all the registered callbacks are canceled.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    onReceiveData(callback?: Function): void;

    /**
     * Add dependencies on the task array for this task.
     *
     * @param { Task[] } tasks - tasks tasks An array of dependent tasks.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200026 - There is a circular dependency.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    /**
     * Add dependencies on the task array for this task.
     *
     * @param { Task[] } tasks - An array of dependent tasks.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * <br>1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types;
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 10200026 - There is a circular dependency.
     * @throws { BusinessError } 10200052 - The periodic task cannot have a dependency.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    /**
     * Adds dependent tasks for this task. Before using this API, you must create a Task instance.
     * The task and its dependent tasks cannot be a task in a task group, serial queue, or asynchronous queue,
     * a task that has been executed, or a periodic task. A task with a dependency relationship (a task that depends
     * on another task or a task that is depended on) cannot be executed multiple times.
     *
     * @param { Task[] } tasks - Array of tasks on which the current task depends. The default value is undefined.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * <br>1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types;
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 10200026 - There is a circular dependency.
     * @throws { BusinessError } 10200052 - The periodic task cannot have a dependency.
     * @throws { BusinessError } 10200056 - The task has been executed by the AsyncRunner.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    addDependency(...tasks: Task[]): void;

    /**
     * Remove dependencies on the task array for this task.
     *
     * @param { Task[] } tasks - tasks tasks An array of dependent tasks.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200027 - The dependency does not exist.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    /**
     * Remove dependencies on the task array for this task.
     *
     * @param { Task[] } tasks - An array of dependent tasks.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * <br>1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types;
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 10200027 - The dependency does not exist.
     * @throws { BusinessError } 10200052 - The periodic task cannot have a dependency.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    /**
     * Removes dependent tasks for this task. Before using this API, you must create a Task instance.
     *
     * @param { Task[] } tasks - Array of tasks on which the current task depends. The default value is undefined.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * <br>1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types;
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 10200027 - The dependency does not exist.
     * @throws { BusinessError } 10200052 - The periodic task cannot have a dependency.
     * @throws { BusinessError } 10200056 - The task has been executed by the AsyncRunner.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    removeDependency(...tasks: Task[]): void;

    /**
     * Register a callback function and call it when a task is enqueued.
     * The registration must be carried out before the task is executed. Otherwise, an exception is thrown.
     *
     * @param { CallbackFunction } [callback] - Callback function to register.
     * @throws { BusinessError } 401 - The input parameters are invalid.
     * @throws { BusinessError } 10200034 - The executed task does not support the registration of listeners.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    onEnqueued(callback: CallbackFunction): void;

    /**
     * Register a callback function and call it when the execution of a task starts.
     * The registration must be carried out before the task is executed. Otherwise, an exception is thrown.
     *
     * @param { CallbackFunction } [callback] - Callback function to register.
     * @throws { BusinessError } 401 - The input parameters are invalid.
     * @throws { BusinessError } 10200034 - The executed task does not support the registration of listeners.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    onStartExecution(callback: CallbackFunction): void;

    /**
     * Register a callback function and call it when a task fails to be executed.
     * The registration must be carried out before the task is executed. Otherwise, an exception is thrown.
     *
     * @param { CallbackFunctionWithError } [callback] - Callback function to register.
     * @throws { BusinessError } 401 - The input parameters are invalid.
     * @throws { BusinessError } 10200034 - The executed task does not support the registration of listeners.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    onExecutionFailed(callback: CallbackFunctionWithError): void;

    /**
     * Register a callback function and call it when a task is executed successfully.
     * The registration must be carried out before the task is executed. Otherwise, an exception is thrown.
     *
     * @param { CallbackFunction } [callback] - Callback function to register.
     * @throws { BusinessError } 401 - The input parameters are invalid.
     * @throws { BusinessError } 10200034 - The executed task does not support the registration of listeners.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    onExecutionSucceeded(callback: CallbackFunction): void;

    /**
     * Checks whether the task is complete.
     *
     * @returns { boolean } Returns {@code true} if the task has been completed; returns {@code false} otherwise.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    isDone(): boolean;

    /**
     * Concurrent function to execute in taskpool.
     *
     * @type { Function }
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Concurrent function to execute in taskpool.
     *
     * @type { Function }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Function to be passed in during task creation. For details about the supported return value types of the function, see Sequenceable Data Types.
     *
     * @type { Function }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    function: Function;

    /**
     * The concurrent function arguments.
     *
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * The concurrent function arguments.
     *
     * @type { ?unknown[] }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Arguments of the function. For details about the supported parameter types, see Sequenceable Data Types.
     *
     * @type { ?Object[] }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    arguments?: Object[];

    /**
     * Name of the task specified when the task is created.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    name: string;

    /**
     * Task ID.
     *
     * @type { number }
     * @default 0
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    taskId: number;

    /**
     * Total execution time of the task. in ms.
     *
     * @type { number }
     * @default 0
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    totalDuration: number;

    /**
     * Asynchronous I/O time of the task. in ms.
     *
     * @type { number }
     * @default 0
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    ioDuration: number;

    /**
     * CPU time of the task. in ms.
     *
     * @type { number }
     * @default 0
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    cpuDuration: number;
  }

  /**
   * The TaskGroup class provides an interface to create a task group.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Implements a task group, in which tasks are associated with each other and all tasks are executed at a time.
   * If all the tasks are executed normally, an array of task results is returned asynchronously, and the sequence of
   * elements in the array is the same as the sequence of tasks added by calling addTask. If any task fails,
   * the corresponding exception is thrown. If multiple tasks in the task group fail, the exception of the first failed
   * task is thrown. A task group can be executed for multiple times, but no task can be added after the task group is executed.
   * Before calling any APIs in TaskGroup, you must use constructor to create a TaskGroup instance.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  class TaskGroup {
    /**
     * Create a TaskGroup instance.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Constructor used to create a TaskGroup instance.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    constructor();

    /**
     * A constructor used to create a TaskGroup instance, with the task group name specified.
     *
     * @param { string } name - Task group name.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    constructor(name: string);

    /**
     * Add a Concurrent function into task group.
     *
     * @param { Function } func - func func Concurrent function to add in task group.
     * @param { unknown[] } args - args args The concurrent function arguments.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Adds the function to be executed to this task group. Before using this API, you must create a TaskGroup instance.
     *
     * @param { Function } func - Function to be executed. The function must be decorated using @Concurrent.
     *     For details about the supported return value types of the function, see Sequenceable Data Types.
     * @param { Object[] } args - Arguments of the function. For details about the supported parameter types,
     *     see Sequenceable Data Types. The default value is undefined.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    addTask(func: Function, ...args: Object[]): void;

    /**
     * Add a Task into TaskGroup.
     *
     * @param { Task } task - task task The task want to add in task group.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Add a Task into TaskGroup.
     *
     * @param { Task } task - task task The task want to add in task group.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    /**
     * Add a Task into TaskGroup.
     *
     * @param { Task } task - The task want to add in task group.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * <br>1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types;
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    /**
     * Adds a created task to this task group. Before using this API, you must create a TaskGroup instance.
     * Tasks in another task group, serial queue, or asynchronous queue, dependent tasks, continuous tasks,
     * tasks that have been executed, and periodic tasks cannot be added to the task group.
     *
     * @param { Task } task - Task to be added to the task group.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * <br>1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types;
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
     * @throws { BusinessError } 10200057 - The task cannot be executed by two APIs.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    addTask(task: Task): void;

    /**
     * Name of the task group specified when the task group is created.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    name: string;
  }

  /**
   * Implements a serial queue, in which all tasks are executed in sequence. Before calling any APIs in SequenceRunner,
   * you must use constructor to create a SequenceRunner instance.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  class SequenceRunner {
    /**
     * A constructor used to create a SequenceRunner instance.
     *
     * @param { Priority } priority - Priority of the task. The default value is taskpool.Priority.MEDIUM.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Incorrect parameter types;
     * 2.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    constructor(priority?: Priority);

    /**
     * A constructor used to create a SequenceRunner instance. This instance represents a global serial queue. 
     * If the passed-in name is the same as an existing name, the same serial queue is returned.
     * NOTE:
     * 1.The bottom layer uses the singleton mode to ensure that the same instance is obtained when a serial queue with the same name is created.
     * 2.The priority of a serial queue cannot be modified.
     *
     * @param { string } name - Name of a serial queue.
     * @param { Priority } priority - Priority of the task. The default value is taskpool.Priority.MEDIUM.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    constructor(name: string, priority?: Priority);

    /**
     * Execute a concurrent function.
     *
     * @param { Task } task - The task want to execute.
     * @returns { Promise<Object> }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * @throws { BusinessError } 10200003 - Worker initialization failed.
     * @throws { BusinessError } 10200006 - An exception occurred during serialization.
     * @throws { BusinessError } 10200025 - The task to be added to SequenceRunner has dependent tasks.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    /**
     * Execute a concurrent function.
     *
     * @param { Task } task - The task want to execute.
     * @returns { Promise<Object> }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * <br>1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types;
     * @throws { BusinessError } 10200006 - An exception occurred during serialization.
     * @throws { BusinessError } 10200025 - dependent task not allowed.
     * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    /**
     * Adds a task to the serial queue for execution. Before using this API, you must create a SequenceRunner instance.
     * Tasks in another task group, serial queue, or asynchronous queue, dependent tasks, and tasks that have been executed cannot be added to the serial queue.
     * NOTE:
     * 1.Tasks that depend others cannot be added to the serial queue.
     * 2.The failure or cancellation of a task does not affect the execution of subsequent tasks in the serial queue.
     *
     * @param { Task } task - Task to be added to the serial queue.
     * @returns { Promise<Object> }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * <br>1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types;
     * @throws { BusinessError } 10200006 - An exception occurred during serialization.
     * @throws { BusinessError } 10200025 - dependent task not allowed.
     * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
     * @throws { BusinessError } 10200057 - The task cannot be executed by two APIs.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    execute(task: Task): Promise<Object>;
  }

  /**
   * Describes a continuous task. LongTask inherits from Task. No upper limit is set for the execution time of a continuous task,
   * and no timeout exception is thrown if a continuous task runs for a long period of time. However, a continuous task cannot be
   * executed in a task group or executed for multiple times. The thread for executing a continuous task exists until terminateTask
   * is called after the execution is complete. The thread is reclaimed when it is idle.
   *
   * @extends Task
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  class LongTask extends Task {
  }

  /**
   * The GenericsTask class provides an interface to create a task with generics.
   *
   * @extends Task
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 13
   */
  /**
   * Implements a generic task. GenericsTask inherits from Task. During the creation of a generic task, the passed-in
   * parameter types and return value types of concurrent functions are verified in the compilation phase.
   * Other behaviors are the same as those during the creation of a task.
   *
   * @extends Task
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  class GenericsTask<A extends Array<Object>, R> extends Task {
    /**
     * Create a GenericsTask instance.
     *
     * @param { (...args: A) => R | Promise<R> } func - Concurrent function to execute in taskpool.
     * @param { A } args - The concurrent function arguments.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 13
     */
    /**
     * A constructor used to create a GenericsTask object.
     *
     * @param { (...args: A) => R | Promise<R> } func - Function to be executed. The function must be decorated using @Concurrent.
     *     For details about the supported return value types of the function, see Sequenceable Data Types.
     * @param { A } args - Arguments of the function. For details about the supported parameter types, see Sequenceable Data Types.
     *     The default value is undefined.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(func: (...args: A) => R | Promise<R>, ...args: A);

    /**
     * Create a GenericsTask instance.
     *
     * @param { string } name - The name of GenericsTask.
     * @param { (...args: A) => R | Promise<R> } func - Concurrent function to execute in taskpool.
     * @param { A } args - The concurrent function arguments.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 13
     */
    /**
     * A constructor used to create a GenericsTask instance, with the task name specified.
     *
     * @param { string } name - Name of the generic task.
     * @param { (...args: A) => R | Promise<R> } func - Function to be executed. The function must be decorated using @Concurrent.
     *     For details about the supported return value types of the function, see Sequenceable Data Types.
     * @param { A } args - Arguments of the function. For details about the supported parameter types, see Sequenceable Data Types.
     *     The default value is undefined.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
     * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(name: string, func: (...args: A) => R | Promise<R>, ...args: A);
  }

  /**
   * The State defines the task state.
   *
   * @enum { number } State
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Enumerates the task states. After a task is created and execute() is called, the task is placed in the internal
   * queue of the task pool and the state is WAITING. When the task is being executed by the worker thread of the task pool,
   * the state changes to RUNNING. After the task is executed and the result is returned, the state is reset to WAITING.
   * When the task is proactively canceled, the state changes to CANCELED.
   *
   * @enum { number } State
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  enum State {
    /**
     * the task state is waiting.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * The task is waiting.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    WAITING = 1,

    /**
     * the task state is running.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * The task is running.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    RUNNING = 2,

    /**
     * the task state is canceled.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * The task is canceled.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    CANCELED = 3
  }

  /**
   * Indicates the internal information of the worker thread.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Describes the internal information about a task.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  class TaskInfo {
    /**
     * Task identity.
     *
     * @type { number }
     * @default 0
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Task ID.
     *
     * @type { number }
     * @default 0
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    taskId: number;

    /**
     * Task state.
     *
     * @type { State }
     * @default State::WAITING
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Task state.
     *
     * @type { State }
     * @default State::WAITING
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    state: State;

    /**
     * Duration of task execution.
     *
     * @type { ?number }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Duration that the task has been executed, in ms. If the return value is 0, the task is not running. If the return value is empty, no task is running.
     *
     * @type { ?number }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    duration?: number;

    /**
     * Task name.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    name: string;
  }

  /**
   * Indicates the internal information of the worker thread.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Describes the internal information about a worker thread.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  class ThreadInfo {
    /**
     * Thread id.
     *
     * @type { number }
     * @default 0
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * ID of the worker thread. If the return value is empty, no task is running.
     *
     * @type { number }
     * @default 0
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    tid: number;

    /**
     * Task id list that running on current thread.
     *
     * @type { ?number[] }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * IDs of tasks running on the calling thread. If the return value is empty, no task is running.
     *
     * @type { ?number[] }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    taskIds?: number[];

    /**
     * Thread priority.
     *
     * @type { ?Priority }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Priority of the calling thread. If the return value is empty, no task is running.
     *
     * @type { ?Priority }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    priority?: Priority;
  }

  /**
   * Indicates the internal information of the taskpool.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Describes the internal information about a task pool.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  class TaskPoolInfo {
    /**
     * An array of taskpool thread information.
     *
     * @type { ThreadInfo[] }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Internal information about the worker threads.
     *
     * @type { ThreadInfo[] }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    threadInfos: ThreadInfo[];

    /**
     * An array of taskpool task information.
     *
     * @type { TaskInfo[] }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Internal information about the tasks.
     *
     * @type { TaskInfo[] }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    taskInfos: TaskInfo[];
  }

  /**
   * Execute a concurrent function.
   *
   * @param { Function } func - func func Concurrent function want to execute.
   * @param { unknown[] } args - args args The concurrent function arguments.
   * @returns { Promise<unknown> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200003 - Worker initialization failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Execute a concurrent function.
   *
   * @param { Function } func - func func Concurrent function want to execute.
   * @param { unknown[] } args - args args The concurrent function arguments.
   * @returns { Promise<unknown> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200003 - Worker initialization failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Execute a concurrent function.
   *
   * @param { Function } func - func func Concurrent function want to execute.
   * @param { Object[] } args - args args The concurrent function arguments.
   * @returns { Promise<Object> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200003 - Worker initialization failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  /**
   * Places a function to be executed in the internal queue of the task pool. The function is not executed immediately.
   * It waits to be distributed to the worker thread for execution. In this mode, the function cannot be canceled.
   *
   * @param { Function } func - Function to be executed. The function must be decorated using @Concurrent.
   *     For details about the supported return value types of the function, see Sequenceable Data Types.
   * @param { Object[] } args - Arguments of the function. For details about the supported parameter types,
   *     see Sequenceable Data Types. The default value is undefined.
   * @returns { Promise<Object> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  function execute(func: Function, ...args: Object[]): Promise<Object>;

  /**
   * Execute a concurrent function with generics.
   *
   * @param { (...args: A) => R | Promise<R> } func - Concurrent function want to execute.
   * @param { A } args - The concurrent function arguments.
   * @returns { Promise<R> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 13
   */
  /**
   * Verifies the passed-in parameter types and return value type of a concurrent function,
   * and places the function to execute in the internal queue of the task pool.
   *
   * @param { (...args: A) => R | Promise<R> } func - Function to be executed. The function must be decorated using @Concurrent.
   *     For details about the supported return value types of the function, see Sequenceable Data Types.
   * @param { A } args - Arguments of the function. For details about the supported parameter types,
   *     see Sequenceable Data Types. The default value is undefined.
   * @returns { Promise<R> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  function execute<A extends Array<Object>, R>(func: (...args: A) => R | Promise<R>, ...args: A): Promise<R>;

  /**
   * Execute a concurrent task.
   *
   * @param { Task } task - task task The task want to execute.
   * @param { Priority } [priority] - priority priority Task priority, MEDIUM is default.
   * @returns { Promise<unknown> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200003 - Worker initialization failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Execute a concurrent task.
   *
   * @param { Task } task - task task The task want to execute.
   * @param { Priority } [priority] - priority priority Task priority, MEDIUM is default.
   * @returns { Promise<unknown> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200003 - Worker initialization failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Execute a concurrent task.
   *
   * @param { Task } task - task task The task want to execute.
   * @param { Priority } [priority] - priority priority Task priority, MEDIUM is default.
   * @returns { Promise<Object> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200003 - Worker initialization failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  /**
   * Execute a concurrent task.
   *
   * @param { Task } task - The task want to execute.
   * @param { Priority } [priority] - Task priority, MEDIUM is default.
   * @returns { Promise<Object> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br>1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types;
   * <br>3. Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  /**
   * Places a task in the internal queue of the task pool. The task is not executed immediately. It waits to be distributed
   * to the worker thread for execution. In this mode, you can set the task priority and call cancel() to cancel the task.
   * The task cannot be a task in a task group, serial queue, or asynchronous queue. This API can be called only once for
   * a continuous task, but multiple times for a non-continuous task.
   *
   * @param { Task } task - Task to be executed.
   * @param { Priority } [priority] - Priority of the task. The default value is taskpool.Priority.MEDIUM.
   * @returns { Promise<Object> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br>1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types;
   * <br>3. Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
   * @throws { BusinessError } 10200057 - The task cannot be executed by two APIs.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  function execute(task: Task, priority?: Priority): Promise<Object>;

  /**
   * Execute a concurrent task with generics.
   *
   * @param { GenericsTask<A, R> } task - The task want to execute.
   * @param { Priority } [priority] - Task priority, MEDIUM is default.
   * @returns { Promise<R> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 13
   */
  /**
   * Verifies the passed-in parameter types and return value type of a concurrent function, and places the generic task in the internal queue of the task pool.
   *
   * @param { GenericsTask<A, R> } task - Generic task to be executed.
   * @param { Priority } [priority] - Priority of the task. The default value is taskpool.Priority.MEDIUM.
   * @returns { Promise<R> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
   * @throws { BusinessError } 10200057 - The task cannot be executed by two APIs.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  function execute<A extends Array<Object>, R>(task: GenericsTask<A, R>, priority?: Priority): Promise<R>;

  /**
   * Execute a concurrent task group.
   *
   * @param { TaskGroup } group - group group The task group want to execute.
   * @param { Priority } [priority] - priority priority Task group priority, MEDIUM is default.
   * @returns { Promise<unknown[]> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Places a task group in the internal queue of the task pool. The tasks in the task group are not executed immediately.
   * They wait to be distributed to the worker thread for execution. After all tasks in the task group are executed,
   * a result array is returned. This API applies when you want to execute a group of associated tasks.
   *
   * @param { TaskGroup } group - Task group to be executed.
   * @param { Priority } [priority] - Priority of the task group. The default value is taskpool.Priority.MEDIUM.
   * @returns { Promise<Object[]> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function execute(group: TaskGroup, priority?: Priority): Promise<Object[]>;

  /**
   * Execute a concurrent task after the specified time.
   *
   * @param { number } delayTime - delayTime delayTime The time want to delay.
   * @param { Task } task - task task The task want to execute.
   * @param { Priority } [priority] - priority priority Task priority, MEDIUM is default.
   * @returns { Promise<Object> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200028 - The delayTime is less than zero.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  /**
   * Execute a concurrent task after the specified time.
   *
   * @param { number } delayTime - The time want to delay.
   * @param { Task } task - The task want to execute.
   * @param { Priority } [priority] - Task priority, MEDIUM is default.
   * @returns { Promise<Object> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br>1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types;
   * <br>3. Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @throws { BusinessError } 10200028 - The delayTime is less than zero.
   * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  /**
   * Executes a task after a given delay. In this mode, you can set the task priority and call cancel() to cancel the task.
   * The task cannot be a task in a task group, serial queue, or asynchronous queue, or a periodic task.
   * This API can be called only once for a continuous task, but multiple times for a non-continuous task.
   *
   * @param { number } delayTime - Delay, in ms.
   * @param { Task } task - Task to be executed with a delay.
   * @param { Priority } [priority] - Priority of the task. The default value is taskpool.Priority.MEDIUM.
   * @returns { Promise<Object> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br>1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types;
   * <br>3. Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @throws { BusinessError } 10200028 - The delayTime is less than zero.
   * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
   * @throws { BusinessError } 10200057 - The task cannot be executed by two APIs.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  function executeDelayed(delayTime: number, task: Task, priority?: Priority): Promise<Object>;

  /**
   * Execute a concurrent task with generics after the specified time.
   *
   * @param { number } delayTime - The time want to delay.
   * @param { GenericsTask<A, R> } task - The task want to execute.
   * @param { Priority } [priority] - Task priority, MEDIUM is default.
   * @returns { Promise<R> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
   * @throws { BusinessError } 10200028 - The delayTime is less than zero.
   * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 13
   */
  /**
   * Verifies the passed-in parameter types and return value type of a concurrent function, and executes the generic task with a delay.
   *
   * @param { number } delayTime - Delay, in ms.
   * @param { GenericsTask<A, R> } task - Generic task to be executed with a delay.
   * @param { Priority } [priority] - Priority of the task. The default value is taskpool.Priority.MEDIUM.
   * @returns { Promise<R> }
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
   * @throws { BusinessError } 10200028 - The delayTime is less than zero.
   * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
   * @throws { BusinessError } 10200057 - The task cannot be executed by two APIs.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  function executeDelayed<A extends Array<Object>, R>(delayTime: number, task: GenericsTask<A, R>, priority?: Priority): Promise<R>;

  /**
   * Execute a concurrent task periodically.
   *
   * @param { number } period - The period in milliseconds for executing task.
   * @param { Task } task - The task want to execute.
   * @param { Priority } [priority] - Task priority, MEDIUM is default.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br>1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types;
   * <br>3. Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @throws { BusinessError } 10200028 - The period is less than zero.
   * @throws { BusinessError } 10200050 - The concurrent task has been executed and cannot be executed periodically.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  /**
   * Executes a task periodically. In this execution mode, you can set the task priority and call cancel() to cancel the execution.
   * A periodic task cannot be a task in a task group, serial queue, or asynchronous queue. It cannot call execute() again or have a dependency relationship.
   *
   * @param { number } period - Execution period, in ms.
   * @param { Task } task - Task to be executed.
   * @param { Priority } [priority] - Priority of the task. The default value is taskpool.Priority.MEDIUM.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br>1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types;
   * <br>3. Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @throws { BusinessError } 10200028 - The period is less than zero.
   * @throws { BusinessError } 10200050 - The concurrent task has been executed and cannot be executed periodically.
   * @throws { BusinessError } 10200057 - The task cannot be executed by two APIs.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  function executePeriodically(period: number, task: Task, priority?: Priority): void;

  /**
   * Execute a concurrent task with generics periodically.
   *
   * @param { number } period - The period in milliseconds for executing task.
   * @param { GenericsTask<A, R> } task - The task want to execute.
   * @param { Priority } [priority] - Task priority, MEDIUM is default.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @throws { BusinessError } 10200028 - The period is less than zero.
   * @throws { BusinessError } 10200050 - The concurrent task has been executed and cannot be executed periodically.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 13
   */
  /**
   * Verifies the passed-in parameter types and return value type of a concurrent function, and executes the generic task
   * periodically at an interval specified by period.
   *
   * @param { number } period - Execution period, in ms.
   * @param { GenericsTask<A, R> } task - Generic task to be executed periodically.
   * @param { Priority } [priority] - Priority of the task. The default value is taskpool.Priority.MEDIUM.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
   * @throws { BusinessError } 10200006 - An exception occurred during serialization.
   * @throws { BusinessError } 10200014 - The function is not marked as concurrent.
   * @throws { BusinessError } 10200028 - The period is less than zero.
   * @throws { BusinessError } 10200050 - The concurrent task has been executed and cannot be executed periodically.
   * @throws { BusinessError } 10200057 - The task cannot be executed by two APIs.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  function executePeriodically<A extends Array<Object>, R>(period: number, task: GenericsTask<A, R>, priority?: Priority): void;

  /**
   * Cancel a concurrent task.
   *
   * @param { Task } task - task task The task want to cancel.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200015 - The task to cancel does not exist.
   * @throws { BusinessError } 10200016 - The task to cancel is being executed.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Cancel a concurrent task.
   *
   * @param { Task } task - task task The task want to cancel.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200015 - The task to cancel does not exist.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Cancel a concurrent task.
   *
   * @param { Task } task - task task The task want to cancel.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200015 - The task to cancel does not exist.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  /**
   * Cancels a task in the task pool. If the task is in the internal queue of the task pool, the task will not be executed
   * after being canceled, and an exception indicating task cancellation is returned. If the task has been distributed to
   * the worker thread of the task pool, canceling the task does not affect the task execution, and the execution result
   * is returned in the catch branch. You can use isCanceled() to check the task cancellation status. In other words,
   * taskpool.cancel takes effect before taskpool.execute or taskpool.executeDelayed is called.
   *
   * @param { Task } task - Task to cancel.
   * @throws { BusinessError } 10200015 - The task to cancel does not exist.
   * @throws { BusinessError } 10200055 - The asyncRunner task has been canceled.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  function cancel(task: Task): void;

  /**
   * Cancel a concurrent task group.
   *
   * @param { TaskGroup } group - group group The task group want to cancel.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200018 - The task group to cancel does not exist.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Cancels a task group in the task pool. If a task group is canceled before all the tasks in it are finished, undefined is returned.
   *
   * @param { TaskGroup } group - Task group to cancel.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200018 - The task group to cancel does not exist.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function cancel(group: TaskGroup): void;

  /**
   * Cancels a task in the task pool by task ID. If the task is in the internal queue of the task pool,
   * the task will not be executed after being canceled, and an exception indicating task cancellation is returned.
   * If the task has been distributed to the worker thread of the task pool, canceling the task does not affect the task execution,
   * and the execution result is returned in the catch branch. You can use isCanceled() to check the task cancellation status.
   * In other words, taskpool.cancel takes effect before taskpool.execute or taskpool.executeDelayed is called.
   * If taskpool.cancel is called by other threads, note that the cancel operation, which is asynchronous,
   * may take effect for later calls of taskpool.execute or taskpool.executeDelayed.
   *
   * @param { number } taskId - ID of the task to cancel.
   * @throws { BusinessError } 10200015 - The task to cancel does not exist.
   * @throws { BusinessError } 10200055 - The asyncRunner task has been canceled.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 18
   */
  function cancel(taskId: number): void;

  /**
   * Get task pool internal information.
   *
   * @returns { TaskPoolInfo }
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Obtains internal information about this task pool, including thread information and task information.
   *
   * @returns { TaskPoolInfo }
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function getTaskPoolInfo(): TaskPoolInfo;

  /**
   * Terminates a continuous task in the task pool. It is called after the continuous task is complete.
   * After the task is terminated, the thread that executes the task may be reclaimed.
   *
   * @param { LongTask } longTask - Continuous task to terminate.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  function terminateTask(longTask: LongTask): void;

  /**
   * Checks whether a function is a concurrent function.
   *
   * @param { Function } func - Function to check.
   * @returns { boolean } Returns {@code true} if it is a concurrent function; returns {@code false} otherwise.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  function isConcurrent(func: Function): boolean;

  /**
   * Implements an asynchronous queue, for which you can specify the task execution concurrency and queuing policy.
   * Before calling any APIs in AsyncRunner, you must use constructor to create an AsyncRunner instance.
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 18
   */
  export class AsyncRunner {
    /**
     * A constructor used to create an AsyncRunner instance. It constructs a non-global asynchronous queue.
     * Even when the parameters passed are the same, it returns different asynchronous queues.
     *
     * @param { number } runningCapacity - Maximum number of tasks that can run concurrently. The value must be a positive integer.
     *     If a negative number is passed, an error is reported. If a non-integer is passed, the value is rounded down.
     * @param { ?number } waitingCapacity - Maximum number of tasks that can be queued. The value must be greater than or equal to 0.
     *     If a negative number is passed, an error is reported. If a non-integer is passed, the value is rounded down. 
     *     The default value is 0, indicating that there is no limit to the number of tasks that can wait.
     *     If a value greater than 0 is passed, tasks will be discarded from the front of the queue once the queue size
     *     exceeds this limit, implementing a discard policy.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    constructor(runningCapacity: number, waitingCapacity?: number);

    /**
     * A constructor used to create an AsyncRunner instance. It constructs a global asynchronous queue.
     * If the passed-in name is the same as an existing name, the same asynchronous queue is returned.
     * NOTE:
     * 1.The bottom layer uses the singleton mode to ensure that the same instance is obtained when an asynchronous queue with the same name is created.
     * 2.The task execution concurrency and waiting capacity cannot be modified.
     *
     * @param { string } name - Name of an asynchronous queue.
     * @param { number } runningCapacity - Maximum number of tasks that can run concurrently. The value must be a positive integer.
     *     If a negative number is passed, an error is reported. If a non-integer is passed, the value is rounded down.
     * @param { ?number } waitingCapacity - Maximum number of tasks that can be queued. The value must be greater than or equal to 0.
     *     If a negative number is passed, an error is reported. If a non-integer is passed, the value is rounded down.
     *     The default value is 0, indicating that there is no limit to the number of tasks that can wait.
     *     If a value greater than 0 is passed, tasks will be discarded from the front of the queue once the queue size
     *     exceeds this limit, implementing a discard policy.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    constructor(name: string, runningCapacity: number, waitingCapacity?: number);

    /**
     * Adds a task to the asynchronous queue for execution. Before using this API, you must create an AsyncRunner instance.
     * NOTE:
     * Tasks in a task group cannot be added to the asynchronous queue.
     * Tasks in a serial queue cannot be added to the asynchronous queue.
     * Tasks in other asynchronous queues cannot be added to the asynchronous queue.
     * Periodic tasks cannot be added to the asynchronous queue.
     * Delayed tasks cannot be added to the asynchronous queue.
     * Tasks that depend others cannot be added to the asynchronous queue.
     * Tasks that have been executed cannot be added to the asynchronous queue.
     *
     * @param { Task } task - Task to be added to the asynchronous queue.
     * @param { ?Priority } priority - Priority of the task. The default value is taskpool.Priority.MEDIUM.
     * @returns { Promise<Object> }
     * @throws { BusinessError } 10200006 - An exception occurred during serialization.
     * @throws { BusinessError } 10200025 - dependent task not allowed.
     * @throws { BusinessError } 10200051 - The periodic task cannot be executed again.
     * @throws { BusinessError } 10200054 - The asyncRunner task is discarded.
     * @throws { BusinessError } 10200057 - The task cannot be executed by two APIs.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    execute(task: Task, priority?: Priority): Promise<Object>;
  }

  /**
   * Defines the task result interface
   * 
   * @interface TaskResult
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 20
   */
  interface TaskResult {
    /**
     * the result returned by the task
     *
     * @type { ?Object }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    result?: Object;

    /**
     * the error thrown by the task
     *
     * @type { ?(Error | Object) }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    error?: Error | Object;
  }
}

export default taskpool;
