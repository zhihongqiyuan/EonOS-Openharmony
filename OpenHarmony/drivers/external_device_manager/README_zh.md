# 扩展外部设备管理

## 简介

当前OpenHarmony HDF驱动框架已经支持板载标准外接设备驱动能力，在OpenHarmony外接设备生态拓展过程中，为解决设备厂商开发非标准协议的可插拔设备驱动接入OpenHarmony系统问题，扩展外部设备管理架构提供应用态扩展设备驱动包的开发、部署、安装、运行、能力开放全流程生命周期管理，为设备厂商开发者提供高效、安全、且低成本的扩展外部设备驱动开发解决方案，做到即插即用的用户体验。

**图 1**  扩展外部设备管理架构图 
![](figures/architecture-of-the-ext-device-manager.png)

扩展外部设备管理为应用提供外部设备的查询，绑定，解绑定接口能力。应用查询外部设备信息后，通过成功绑定设备驱动包并获取驱动包对象，完成与扩展设备驱动包交互。扩展设备驱动包开发者通过系统提供的USB C API接口能力，进行扩展设备驱动包开发、发布，更新，有扩展外部设备管理框架进行对应设备状态与扩展驱动包的管理。当开发者插入USB设备后，有驱动开发框架识别USB设备，并上报USB设备识别成功事件消息给USB总线扩展插件，完成与扩展驱动包的匹配。
扩展外部设备管理框架主要功能模块介绍：<br>
1、扩展驱动管理服务，提供扩展驱动管理服务按需启动能力，负责设备管理，包信息管理，总线扩展核心模块生命周期管理。<br>
2、设备管理模块，提供外部设备的查询，绑定，解绑定接口能力，负责设备列表管理，设备与驱动匹配管理。<br>
3、包信息管理模块，负责扩展驱动包信息解析，存储，设备匹配能力，支持扩展驱动包的安装，更新，卸载。<br>
4、驱动元能力控制器模块，负责扩展驱动元能力生命周期管控，包括扩展驱动元能力的启动，停止。<br>
5、总线扩展核心模块，负责总线设备枚举、总线驱动包解析、包与设备匹配。<br>
6、USB总线扩展插件模块，负责USB设备热插拔监听，USB设备信息读取。<br>

## 场景说明

设备厂商开发者复用当前OpenHarmony应用开发环境，以驱动扩展Ability开发应用态扩展驱动包，并通过OpenHarmony系统安全认证后，为其他三方应用和系统服务提供定制化的设备驱动硬件功能接口能力。扩展外部设备管理框架基于驱动扩展Ability能力，构建应用态扩展驱动包的开发、部署、安装、更新、卸载、运行、能力开放全流程生命周期管理能力。三方设备厂商开发非标准协议的可插拔USB设备扩展驱动包，能够正常安装到OpenHarmony系统，插上USB设备后，能正常识别USB设备并与扩展驱动包匹配；三方应用通过扩展外部设备管理框架查询并绑定USB设备扩展驱动包，完成对扩展驱动包定制功能的操作。

## 目录

```
/drivers/external_device_manager
├── frameworks # 无独立进程的桥接实现
│   └── js # 对外提供应该js API需要
│       └──-napi # napi代码实现
│           ├── driver_ext_ability # ext ability桥接类
│           ├── driver_ext_context # ext context桥接类
│           └── driver_ext_manager # device ext manager桥接类
├── interfaces # 对外提供.d.ts接口定义和capi实现
│   ├── innerkits # 部件接口，系统内部件间接口
│   └── kits # 提供应用的接口
│       └── js # js接口
├── sa_profile # SA定义
├── services # 扩展设备管理系统服务实现
│   ├── native
│   │   ├── driver_extension # driver extension ability 和driver extension context定义和实现
│   │   │   ├── include # driver extension接口定义 
│   │   │   └── src # driver extension接口实现
│   │   └── driver_extension_manager #各子模块实现
│   │       ├── include
│   │       │   ├── bus_extension # 总线管理接口定义
│   │       │   ├── device_manager # 设备管理接口定义
│   │       │   ├── driver_ability_controller # ability生命周期管理接口定义
│   │       │   └── driver_pkg_manager # 包管理接口定义
│   │       └── src
│   │           ├── bus_extension # 总线管理接口实现
│   │           ├── device_manager # 设备管理接口实现
│   │           ├── driver_ability_controller # ability生命周期管理接口实现
│   │           └── driver_pkg_manager# 包管理接口实现
│   └── zidl # js跨进程实现
│       ├── include# js跨进程接口定义
│       └── src # js跨进程接口实现
└── utils # 通用接口实现和 hilog封装
    └── include # 通用接口定义
```

## 编译构建

编译32位ARM系统external_device_manager部件
```
./build.sh --product-name {product_name} --ccache --build-target external_device_manager
```

编译64位ARM系统external_device_manager部件
```
./build.sh --product-name {product_name} --ccache --target-cpu arm64 --build-target external_device_manager
```

{product_name}为当前支持的平台，比如rk3568。

## 相关仓

[驱动子系统](https://gitee.com/openharmony/docs/blob/master/zh-cn/readme/%E9%A9%B1%E5%8A%A8%E5%AD%90%E7%B3%BB%E7%BB%9F.md)

[drivers\_hdf\_core](https://gitee.com/openharmony/drivers_hdf_core/blob/master/README_zh.md)

[drivers\_interface](https://gitee.com/openharmony/drivers_interface/blob/master/README_ZH.md)

[drivers\_peripheral](https://gitee.com/openharmony/drivers_peripheral/blob/master/README_zh.md)
