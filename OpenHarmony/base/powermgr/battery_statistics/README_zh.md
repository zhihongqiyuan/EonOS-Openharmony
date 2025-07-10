# 耗电统计服务组件<a name="ZH-CN_TOPIC_0000001115047353"></a>

-   [简介](#section11660541593)
-   [目录](#section19472752217)
-   [相关仓](#section63151229062)

## 简介<a name="section11660541593"></a>

耗电统计组件提供如下功能：

1. 软件耗电统计：统计每个应用或者软件的耗电情况，耗电量是以Uid为单位统计；软件功耗的包括不限于下面几项：cpu的耗电、持锁运行带来的耗电、移动无线的耗电 、wifi耗电 、gnss耗电、传感器的耗电 、相机耗电、 闪光灯耗电等。
2. 硬件耗电统计：软件耗电之外的耗电都归属到硬件耗电，包括不限于如下几项：用户功耗 、通话功耗、屏幕功耗 、Wifi功耗 、蓝牙消耗等等。

**图 1**  耗电统计服务组件架构图<a name="fig106301571239"></a><br>
![](figures/power-management-subsystem-architecture.png "电源管理子系统架构图")

## 目录<a name="section19472752217"></a>

```
/base/powermgr/battery_statistics
├── figures                  	# 架构图
├── frameworks                  # Framework层
│   ├── napi                  	# NAPI层
│   └── native                  # Native层
├── interfaces                  # 接口层
│   └── inner_api               # 内部接口
├── sa_profile                  # SA 配置文件
├── services                    # 服务层
│   ├── native                  # Native 层
│   ├── profile                 # 平均耗电配置文件
│   └── zidl                    # Zidl 接口层
├── test                        # 测试用例
│   ├── fuzztest                # Fuzz 测试
│   ├── unittest                # 单元测试
│   ├── systemtest              # 系统测试
│   └── utils                   # 测试工具
└── utils                       # 工具和通用层
```



## 相关仓<a name="section63151229062"></a>

[电源管理子系统](https://gitee.com/openharmony/docs/blob/master/zh-cn/readme/%E7%94%B5%E6%BA%90%E7%AE%A1%E7%90%86%E5%AD%90%E7%B3%BB%E7%BB%9F.md)

[powermgr_power_manager](https://gitee.com/openharmony/powermgr_power_manager)

[powermgr_display_manager](https://gitee.com/openharmony/powermgr_display_manager)

[powermgr_battery_manager](https://gitee.com/openharmony/powermgr_battery_manager)

[powermgr_thermal_manager](https://gitee.com/openharmony/powermgr_thermal_manager)

**powermgr_battery_statistics**

[powermgr_battery_lite](https://gitee.com/openharmony/powermgr_battery_lite)

[powermgr_powermgr_lite](https://gitee.com/openharmony/powermgr_powermgr_lite)
