# HiChecker组件

-   [简介](#section11660541593)
-   [架构](#section161941989596)
-   [目录](#section14197309111)
-   [说明](#section1371113476307)
-   [涉及仓](#section1371113476310)

## 简介<a name="section11660541593"></a>

HiChecker，可以作为应用开发阶段使用的检测工具，用于检测代码运行过程中部分易忽略的问题，如应用线程出现耗时调用、应用进程中元能力资源泄露等问题。开发者可以通过日志记录或进程crash等形式查看具体问题并进行修改，提升应用的使用体验。

## 架构<a name="section161941989596"></a>

 <img src="./figures/HiChecker架构图.png" style="zoom:100%;" />

| 依赖部件                                                    | 依赖描述                                                     |
| ----------------------------------------------------------- | ------------------------------------------------------------ |
| 媒体子系统/【image部件】图片基础框架 ImagePacker            | 在比较耗时的函数中调用HiChecker接口，通知有耗时调用，同时传入对应的告警提示字符串 |
| 元能力子系统/ability_runtime/运行管理支持DFX FeatureAbility | 提供注册泄露检测逻辑，通知检测模式相关结果                   |

支持特性：

- 提供耗时调用检测功能
- 提供注册（ability） 泄露检测功能
- 支持应用增加、删除不同的检测规则
- 支持应用增加、删除不同的告警通知规则，目前支持记录流水日志（默认），应用崩溃两种规则
- 相关检测条件满足时，支持Native回栈到关键触发点，暂不支持JS回栈

## 目录<a name="section14197309111"></a>

```
/base/hiviewdfx/hichecker
├── frameworks          # 框架代码
│   └── native          # HiChecker native实现代码
├── interfaces          # 接口
│   └── native          # C++接口
│       └── innerkits   # 对内部子系统提供的接口
|   └── js              # JS接口
│       └── kits        # 对应用提供的接口
├── test                # 测试用例
│   └── unittest            
```



## 说明<a name="section1371113476307"></a>
### 接口说明

主要接口说明

| 类        | 方法                                                | 描述                       |
| --------- | :-------------------------------------------------- | -------------------------- |
| HiChecker | BigInt RULE_CAUTION_PRINT_LOG = 1<<63;              | 告警规则，仅记录日志       |
|           | BigInt RULE_CAUTION_TRIGGER_CRASH = 1<<62;          | 告警规则，让应用退出       |
|           | BigInt RULE_THREAD_CHECK_SLOW_PROCESS = 1;          | 检测规则，检测耗时函数调用 |
|           | BigInt RULE_CHECK_ABILITY_CONNECTION_LEAK = 1<<33;  | 检测规则，检测ability泄露  |
|           | BigInt RULE_CHECK_ARKUI_PERFORMANCE = 1<<34;        | 检测规则，用于arkui性能检测  |
|           | addRule(BigInt rule) : void                         | 增加一个或多个检测项       |
|           | removeRule(BigInt rule) : void                      | 删除一个或多个检测项       |
|           | getRule() : BigInt                                  | 获取所有检测项             |
|           | contains(BigInt rule) : boolean                     | 当前是否有某一个检测项     |
|           | NotifySlowProcess(std::string) : void               | 通知有耗时调用             |
|           | NotifySlowEvent(std::string) : void                 | 通知有耗时事件             |
|           | NotifyAbilityConnectionLeak(Caution caution) : void | 通知有ability泄露          |
|           | NotifyCaution(uint64_t rule, const std::string& tag, Caution& caution) : void | 规则检测通用接口 |
| Caution   | GetTriggerRule() : BigInt                           | 获取触发当前告警的检测规则 |
|           | GetCustomMessage() : String                         | 获取更多辅助信息           |
|           | GetStackTrace() ：String                            | 获取堆栈信息               |

## 涉及仓<a name="section1371113476310"></a>

[DFX子系统](https://gitee.com/openharmony/docs/blob/master/zh-cn/readme/DFX子系统.md)

[hiviewdfx_hilog](https://gitee.com/openharmony/hiviewdfx_hilog/blob/master/README_zh.md)

[hiviewdfx_faultloggerd](https://gitee.com/openharmony/hiviewdfx_faultloggerd/blob/master/README_zh.md)
