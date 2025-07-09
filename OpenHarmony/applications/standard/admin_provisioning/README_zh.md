# AdminProvisioning

## 简介

AdminProvisioning应用是OpenHarmony中预置的系统应用，用于企业环境下在设备上发放Mobile Device Management(MDM)业务，同时提供了业务发放接口给其它子系统或者三方MDM客户端应用调用，支持在产品上构建完整的MDM解决方案。

### 架构图

![](figures/adminProvisioning_architecture.png)

AdminProvisioning组件架构图说明：

- AutoManagerAbility为系统开机导航提供SDA(Super Device Admin)业务开放接口, 调用设备管理权限接口，实现对系统的定制，以满足特定场景的使用需求。                               
- MainAbility为三方MDM客户端应用提供DA(Device Admin)业务开放接口，其调用接口与SDA调用的设备管理权限接口存在差异，实现对系统不同程度的定制。

## 目录

```
/applications/standard/admin_provisioning
├── AppScope                                          # 应用的全局配置信息存放目录
├── doc                                               # 应用文档文件存放目录
├── entry                                             # entry模块目录
│   └── src
│       ├── main
│           ├── ets                                   # ArkTS模块目录
│           ├── resources                             # 资源配置文件存放目录
│           └── module.json5                          # 全局配置文件
├── figures                                           # 应用架构图存放目录
└── signature                                         # 签名证书文件目录
```
## 使用说明

   参见使用说明（[使用说明](./doc/Instructions.md)）

## 相关仓

[**admin_provisioning**](https://gitee.com/openharmony/applications_admin_provisioning)

[customization_config_policy](https://gitee.com/openharmony/customization_config_policy)

[customization_enterprise_device_management](https://gitee.com/openharmony/customization_enterprise_device_management)




