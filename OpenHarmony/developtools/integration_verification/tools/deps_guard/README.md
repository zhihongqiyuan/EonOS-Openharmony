# 依赖关系看护系统



依赖关系看护系统用于根据编译产物分析各个ELF文件之间的架构依赖关系，对违规的依赖关系进行拦截。

支持的拦截规则如下：

| 规则名                                                   | 规则说明                                                     |
| -------------------------------------------------------- | ------------------------------------------------------------ |
| [NO-Depends-On-NAPI](rules/NO-Depends-On-NAPI/README.md) | 所有安装到/system/lib{64}/module目录下的napi模块都不允许被其它模块依赖。 |
| [NO-Depends-On-SA](rules/NO-Depends-On-SA/README.md)     | 所有的系统SA模块都不允许被其它模块依赖。                     |
| [ChipsetSDK](rules/ChipsetSDK/README.md)                 | 所有能被芯片组件模块依赖的系统组件ChipsetSDK模块都需白名单管理，不能依赖白名单之外的系统组件模块。 |

