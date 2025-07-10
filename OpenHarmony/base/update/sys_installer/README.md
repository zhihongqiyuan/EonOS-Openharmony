# 系统安装部件

## 简介
系统安装部件是基础软件里面升级子系统中的一个部件，提供系统安装能力，主要作用是在设备正常使用的情况下，完成升级包的静默安装，支持标准系统设备。

## 目录

```
base/update/sys_installer/
├── frameworks                  # 系统安装sa服务框架目录
│   ├── installer_manager       # 安装管理目录
│   ├── ipc_server              # sa服务端框架
│   └── status_manager          # 安装状态管理目录
├── interfaces
│   ├── inner_api               #对外api接口定义
│   └── innerkits               #对外api接口实现
└── services                    #系统安装业务实现
```

## 相关仓

[升级子系统](https://gitee.com/openharmony/update_updater/blob/master/README_zh.md)

