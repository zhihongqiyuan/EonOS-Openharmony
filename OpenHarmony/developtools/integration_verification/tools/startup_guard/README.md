# 启动资源管控

启动资源管控是对启动过程中配置文件（\*.cfg)和系统参数配置文件(\*.para", \*.para.dac)的管控， 约束各子模块的启动资源配置。通过对OpenHarmony镜像中的启动配置文件（\*.cfg)和系统参数配置文件(\*.para", \*.para.dac)， 通过对白名单的检查， 完成对启动资源的管控。

支持的拦截规则如下：

| 规则名                                                   | 规则说明                                                     |
| -------------------------------------------------------- | ------------------------------------------------------------ |
| [NO-Config-Cmds-In-Init](rules/NO-Config-Cmds-In-Init/README.md) | 所有启动配置文件cmd管控 |
| [NO-Config-SystemParameter-In-INIT](rules/NO-Config-SystemParameter-In-INIT/README.md)     | 所有系统参数管控 |
