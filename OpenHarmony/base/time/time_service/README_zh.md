# 时间时区部件

## 简介

在整个OpenHarmony架构中提供管理系统时间时区和定时的能力，支持设置定时器和获取时间、时区和日期。

**图 1**  子系统架构图

![](figures/subsystem_architecture_zh.png "子系统架构图")

## 目录

```
/base/time/time_service
├── etc                      # 组件包含的进程的配置文件
├── figures                  # 构架图
├── framework/js/napi        # js接口解析成napi接口
├── interfaces/inner_api     # 组件对外提供的接口代码
├── services                 # 时间服务实现
│   └── sa_profile           # 组件包含的系统服务的配置文件和进程的配置文件
├── test                     # 接口的单元测试
└── utils                    # 组件包含日志打印和有序公共事件定义的常量
```

## 说明

### js接口说明

> **说明：**
>
> - 从API Version 9 开始，模块接口[@ohos.systemTime](https://gitee.com/openharmony/docs/blob/master/zh-cn/application-dev/reference/apis/js-apis-system-time.md)不再维护，推荐使用新模块接口[@ohos.systemDateTime](https://gitee.com/openharmony/docs/blob/master/zh-cn/application-dev/reference/apis/js-apis-system-date-time.md)

**表 1**  js组件systemDateTime开放的主要方法

| 接口名                                                       | 描述                                                 |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| setTime(time : number) : Promise                             | 设置系统时间（1970-01-01至今毫秒数），Promise方式。  |
| setTime(time : number, callback : AsyncCallback<boolean>) : void | 设置系统时间（1970-01-01至今毫秒数），callback方式。 |
| getCurrentTime(isNano?: boolean): Promise<number>                             | 获取自Unix纪元以来经过的时间，Promise方式。  |
| getCurrentTime(isNano: boolean, callback: AsyncCallback<number>): void | 获取自Unix纪元以来经过的时间，callback方式。 |
| getCurrentTime(callback: AsyncCallback<number>): void | 获取自Unix纪元以来经过的时间，callback方式。 |
| getRealActiveTime(isNano?: boolean): Promise<number>                             | 获取自系统启动以来经过的时间，不包括深度睡眠时间，Promise方式。  |
| getRealActiveTime(isNano: boolean, callback: AsyncCallback<number>): void | 获取自系统启动以来经过的时间，不包括深度睡眠时间，callback方式。 |
| getRealActiveTime(callback: AsyncCallback<number>): void | 获取自系统启动以来经过的时间，不包括深度睡眠时间，callback方式。 |
| getRealTime(isNano?: boolean): Promise<number>                             | 获取自系统启动以来经过的时间，包括深度睡眠时间，Promise方式。  |
| getRealTime(isNano: boolean, callback: AsyncCallback<number>): void | 获取自系统启动以来经过的时间，包括深度睡眠时间，callback方式。 |
| getRealTime(callback: AsyncCallback<number>): void | 获取自系统启动以来经过的时间，包括深度睡眠时间，callback方式。 |
| setDate(date: Date, callback: AsyncCallback<boolean>): void; | 设置系统时间（Date格式），Promise方式。              |
| setDate(date: Date): Promise<boolean>                        | 设置系统时间（Date格式），callback方式。             |
| getDate(callback: AsyncCallback<Date>): void | 获取当前系统日期，Promise方式。              |
| getDate(): Promise<Date>                        | 获取当前系统日期，callback方式。             |
| setTimezone(timezone: string, callback: AsyncCallback<boolean>): void | 设置系统时区，callback方式。                         |
| setTimezone(timezone: string): Promise<boolean>              | 设置系统时区，Promise方式。                          |
| getTimezone(callback: AsyncCallback<string>): void | 获取系统时区，callback方式。                         |
| getTimezone(): Promise<string>              | 获取系统时区，Promise方式。                          |

**表 2**  js组件systemTimer开放的主要方法

| 接口名                                                       | 描述                       |
| ------------------------------------------------------------ | -------------------------- |
| createTimer(options: TimerOptions, callback: AsyncCallback<number>): void | 创建定时器，callback方式。 |
| createTimer(options: TimerOptions): Promise<number>          | 创建定时器，promise方式。  |
| startTimer(timer: number, triggerTime: number, callback: AsyncCallback<boolean>): void | 开启定时器，callback方式。 |
| startTimer(timer: number, triggerTime: number): Promise<boolean> | 开启定时器，promise方式。  |
| stopTimer(timer: number, callback: AsyncCallback<boolean>):  void | 停止定时器，callback方式。 |
| stopTimer(timer: number): Promise<boolean>                   | 停止定时器，promise方式。  |
| destroyTimer(timer: number, callback: AsyncCallback<boolean>): void | 销毁定时器，callback方式。 |
| destroyTimer(timer: number): Promise<boolean>                | 摧毁定时器，promise方式。  |

**表 3**  systemTimer组件参数TimerOptions说明

| 名称      | 类型      | 说明                                                         |
| --------- | --------- | ------------------------------------------------------------ |
| type      | number    | 定时器类型。<br/>取值为1时，表示为系统启动时间定时器（定时器启动时间不能晚于当前设置的系统时间）；<br/>取值为2时，表示为唤醒定时器；<br/>取值为4时，表示为精准定时器；<br/>取值为5时，表示为IDLE模式定时器（暂不支持）。 |
| repeat    | boolean   | true 为循环定时器，false为单次定时器。                       |
| interval  | number    | 如果是循环定时器，repeat值应大于5000毫秒，非重复定时器置为0。 |
| wantAgent | wantAgent | 设置通知的wantagent，定时器到期后通知。                      |
| callback  | => void   | 设置回调函数，定时器到期后触发。                             |

### js接口使用说明

systemDateTime模块使用示例：

```javascript
// 导入模块
import systemDateTime from '@ohos.systemDateTime';

// Promise方式的异步方法设置时间   
// time对应的时间为2021-01-20 02:36:25
let time = 1611081385000;
try {
  systemDateTime.setTime(time).then(() => {
    console.info(`Succeeded in setting time.`);
  }).catch((error) => {
    console.info(`Failed to set time. message: ${error.message}, code: ${error.code}`);
  });
} catch(e) {
  console.info(`Failed to set time. message: ${e.message}, code: ${e.code}`);
}

// callback方式的异步方法设置时间
// time对应的时间为2021-01-20 02:36:25
let time = 1611081385000;
try {
  systemDateTime.setTime(time, (error) => {
    if (error) {
      console.info(`Failed to set time. message: ${error.message}, code: ${error.code}`);
      return;
    }
    console.info(`Succeeded in setting time`);
  });
} catch(e) {
  console.info(`Failed to set time. message: ${e.message}, code: ${e.code}`);
}
```

systemTimer模块使用示例：

```javascript
// 导入模块
import systemTimer from '@ohos.systemTimer';

console.log("start");
var options:TimerOptions{   
   type:TIMER_TYPE_REALTIME,   
   repeat:false,   
   interval:Number.MAX_VALUE/2,   
}

console.log("create timer")   
let timerId = systemTimer.Timer(options)     
console.log("start timer")   
let startTimerRes = systemTimer.startTimer(timerId, 100000)   
console.log("stop timer")   
let stopTimerRes = systemTimer.stopTimer(timerId)   
console.log("destroy timer")   
let destroyTimerRes = systemTimer.destroyTimer(timerId)   
console.log('end');   
```

## 相关仓

时间时区子系统

[**time\_time\_service**](https://gitee.com/openharmony/time_time_service/tree/master/)

