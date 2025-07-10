# 热管理服务

-   [简介](#section0056945901)
-   [目录](#section0056945902)
-   [配置说明](#section0056945903)
-   [相关仓](#section0056945904)

## 简介<a name="section0056945901"></a>

热管理服务提供设备温度管理控制能力，保障整机热安全及热体验。

**图 1**  热管理服务架构图<br>
![](figures/thermal_manager_architecture.png "热管理服务架构图")

**热管理服务模块功能说明**：

1. 温控管理中心（Thermal Manager）：提供对应用及其他子系统的温控查询及回调接口。
2. 温控核心服务（Thermal Service）：提供温度检测、档位仲裁及动作下发等温控核心功能，附配置文件。
3. 内核连接模块（Thermal HDI）：提供温度驱动信息上报及驱动指令下发等功能，附配置文件。
4. 非运行态温控模块（Thermal Protector）：提供非运行态时的简化温控服务，附配置文件。

## 目录<a name="section0056945902"></a>
```
/base/powermgr/thermal_manager
├── application                  # Native应用
│   ├── init                     # Init配置
│   └── protector                # Thermal Protector代码
├── figures                      # 架构图
├── frameworks                   # Framework层
│   ├── napi                     # NAPI层
│   └── native                   # Native层
├── interface                    # 接口层
│   └── inner_api                # 内部接口
├── sa_profile                   # SA配置文件
├── services                     # Thermal Service代码
│   ├── native                   # Native层
│   └── zidl                     # Zidl层
├── test                         # 测试代码
│   ├── fuzztest                 # Fuzz 测试
│   ├── unittest                 # 单元测试
│   ├── systemtest               # 系统测试
│   └── utils                    # 测试工具
└── utils                        # 工具和通用层
```

## 配置说明<a name="section0056945903"></a>
**thermal_service_config.xml**
配置示例：

```
<?xml version="1.0" encoding="UTF-8"?>
<thermal version="0.99" product="ipx">
    <base>
        <item tag="temperature_query_enum" value="soc,battery,shell,ambient"/>
    </base>
    <level>
        <sensor_cluster name="warm_base" sensor="shell">
            <item level="1" threshold="35000" threshold_clr="33000"/>
            <item level="2" threshold="37000" threshold_clr="35000"/>
            <item level="3" threshold="40000" threshold_clr="38000"/>
        </sensor_cluster>
        <sensor_cluster name="warm_safe" sensor="battery,pa">
            <item level="1" threshold="48000,60000" threshold_clr="45000,55000"/>
        </sensor_cluster>
    </level>
    <state>
        <item name="screen"/>
        <item name="charge"/>
    </state>
    <action>
        <item name="cpu"/>
        <item name="current"/>
        <item name="shut_down" param="100"/>
        <item name="thermal_level" strict="1"/>
    </action>
    <policy>
        <config name="warm_base" level="1">
            <cpu screen="1">2800000</cpu>
            <cpu screen="0">2400000</cpu>
            <current charge="1">1500</current>
            <thermal_level>3</thermal_level>
        </config>
        <config name="warm_base" level="2">
            <cpu screen="1">2200000</cpu>
            <cpu screen="0">1800000</cpu>
            <current charge="1">1200</current>
            <thermal_level>4</thermal_level>
        </config>
        <config name="warm_base" level="3">
            <cpu screen="1">1600000</cpu>
            <cpu screen="0">1200000</cpu>
            <current charge="1">1000</current>
            <thermal_level>5</thermal_level>
        </config>
        <config name="warm_safe" level="1">
            <shut_down>1</shut_down>
        </config>
    </policy>
</thermal>
```
```
thermal                          # 根目录，version为版本号；product为产品名
├── base                         # base目录，配置基本参数
│   └── item                     # item目录，tag为配置标签，value为配置值
├── level                        # level目录，配置温度档位信息
│   └── sensor_cluster           # sensor_cluster目录，表示一个sensor集合，name为集合名，sensor为传感器名
│       └── item                 # item目录，配置具体档位信息，level为等级，threshold为触发温度，xxx_clr为回退温度
├── state                        # state目录，配置状态机，name为状态机名称
├── action                       # action目录，配置动作，name为加载动作名称，param为加载参数，strict为仲裁方案
└── policy                       # policy目录，配置档位对应的动作
    └── config                   # config目录，配置对应档位的动作，name对应sensor_cluster的name，level对应其item中的level
        └── <action_name>        # 标签值对应动作的动作值，属性中可以配置state条件及对应的值
```


## 相关仓<a name="section0056945904"></a>
[电源管理子系统](https://gitee.com/openharmony/docs/blob/master/zh-cn/readme/%E7%94%B5%E6%BA%90%E7%AE%A1%E7%90%86%E5%AD%90%E7%B3%BB%E7%BB%9F.md)

[powermgr_power_manager](https://gitee.com/openharmony/powermgr_power_manager)

[powermgr_display_manager](https://gitee.com/openharmony/powermgr_display_manager)

[powermgr_battery_manager](https://gitee.com/openharmony/powermgr_battery_manager)

**powermgr_thermal_manager**

[powermgr_battery_statistics](https://gitee.com/openharmony/powermgr_battery_statistics)

[powermgr_battery_lite](https://gitee.com/openharmony/powermgr_battery_lite)

[powermgr_powermgr_lite](https://gitee.com/openharmony/powermgr_powermgr_lite)
