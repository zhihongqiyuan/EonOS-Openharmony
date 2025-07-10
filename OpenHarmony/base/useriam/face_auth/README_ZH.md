# 人脸认证组件

- [简介](#简介)
- [目录](#目录)
- [使用说明](#使用说明)
- [相关仓](#相关仓)

## 简介

**人脸认证** （faceauth）支持用户人脸的录入，删除和认证功能。

人脸认证是OpenHarmony支持的一种生物认证执行器，按照协同认证定义的资源注册接口，将人脸认证相关资源信息注册到协同认证框架，并根据协同认证框架的调度，调用人脸认证HDI，完成人脸的录入，认证，删除等功能。

**图1** 人脸认证架构图

<img src="figures/faceauth_architecture_ZH.png" alt="口令认证架构图" />

人脸数据是用户的重要生物识别信息，在人脸认证的处理过程中，我们对用户输入的人脸数据实施了如下安全保护措施：

1. 管理权限：人脸的录入和删除流程定义了较高的访问权限，仅允许系统应用调用。
1. 人脸数据安全存储和比对：人脸认证HDI中定义了南向厂商适配的接口，南向厂商可以自行在可信执行环境内完成人脸数据的安全存储和比对。<sup>注1</sup>

*注1：OpenHarmony开源框架内提供了人脸认证的桩实现，未包含可信执行环境内完成人脸数据的安全存储和比对。*

## 目录

```
//base/useriam/face_auth
├── bundle.json              # 组件描述文件
├── common                   # 组件公共文件
├── figures                  # README使用的图片
├── frameworks               # 框架代码
├── interfaces               # 对外接口存放目录
│   └── inner_api            # 对内部子系统暴露的头文件，供系统服务使用
├── sa_profile               # Service Ability 配置文件
├── services                 # Service Ability 服务实现
└── ui                       # 人脸录入界面实现
```

## 使用说明

* 南向厂商适配人脸认证功能时，需实现IDL文件`//drivers/interface/faceauth` 中定义的接口，`//drivers/peripheral/faceauth`提供了接口的桩实现，供开发过程参考。人脸数据的提取、比对、存储和录入等过程应在尽可能安全的环境中实现，保证用户人脸数据的的安全保护达到系统内可以提供的最高安全级别（可信执行环境/安全芯片）。

## 相关仓

[useriam_user_auth_framework](https://gitee.com/openharmony/useriam_user_auth_framework)

[useriam_pin_auth](https://gitee.com/openharmony/useriam_pin_auth)

**[useriam_face_auth](https://gitee.com/openharmony/useriam_face_auth)**

[drivers_interface](https://gitee.com/openharmony/drivers_interface)

[drivers_peripheral](https://gitee.com/openharmony/drivers_peripheral)
