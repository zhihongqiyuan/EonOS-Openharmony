# 通知子系统

## 简介

OpenHarmony通过ANS（Advanced Notification Service，通知系统服务）对通知类型的消息进行管理，支持多种通知类型，包括文本，长文本，多文本，图片，社交，媒体等。所有系统服务以及应用都可以通过通知接口发送通知消息，用户可以通过SystemUI查看所有通知消息。

通知常见的使用场景：

- 显示接收到短消息、即时消息等。
- 显示应用的推送消息，如广告、版本更新等。
- 显示当前正在进行的事件，如导航、下载等。

### 架构图

**图1** 子系统架构图

![子系统架构图](figures/ans_logical_architecture_diagram.png)



## 目录

```
/base/notification/distributed_notification_service/
├── frameworks   # 接口实现
├── interfaces
│   ├── inner_api    # 对内接口目录
│   └── kits         # 对外接口目录
├── sa_profile   # 服务配置
├── services     # 服务实现
└── tools        # 工具实现

```



## 使用说明

ANS支持4种类型的通道，分别为：
- 社交通讯
- 服务提醒
- 内容资讯
- 其他

应用开发者可以根据使用场景创建通道。通道创建后，提醒方式均为系统默认提醒方式。终端用户可以通过设置应用调整各通道的提醒方式。

应用开发者发可发布以下类型的通知：
- 普通文本
- 长文本
- 图片
- 多行文本
- 社交

并且可调整通知的标签、分组、触发事件等属性。



## 相关仓

**[通知子系统](https://gitee.com/openharmony/notification_distributed_notification_service/blob/master/README_zh.md)**

[SystemUI系统应用](https://gitee.com/openharmony/applications_systemui/blob/master/README_zh.md)

[元能力子系统](https://gitee.com/openharmony/ability_ability_runtime/blob/master/README_zh.md)