# tee_tee_os_framework仓介绍 #

## 简介 ##

OpenTrustee提供了一个可信执行环境（Trusted Execution Environment，TEE），运行在受硬件隔离的安全区域中，与常规操作系统如OpenHarmony同时运行但互相隔离。OpenTrustee比常规操作系统拥有更高的安全性，可以为设备上的机密数据提供保护。

OpenTrustee是一套完整的TEE解决方案，包含多个部件，系统架构如图所示：

![](figures/overview-of-opentrustee.png)

tee_tee_os_framework部件主要包含OpenTrustee的操作系统框架部分，负责TA（Trusted Application）整个生命周期的管理、CA（Client Application）与TA交互信息的处理以及驱动的管理。除此以外，还提供了加解密、安全存储等核心服务，下面是具体的模块介绍。

## tee_os_framework部件模块划分 ##
<table>
<th>子模块名称</th>
<th>模块职责</th>
<tr>
<td> gtask </td><td>TA进程的生命周期管理，实现TA进程的创建销毁，CA跟TA交互的通信管理、会话管理、Agent交互管理，TA进程异常状态处理</td>
</tr><tr>
<td> teesmcmgr</td><td>smc命令消息分发管理，包括CA命令、系统的休眠唤醒命令分发，idle状态管理等</td>
</tr><tr>
<td> tarunner</td><td>TA/驱动/服务二进制elf文件加载解析重定位</td>
</tr><tr>
<td> drvmgr</td><td>驱动进程的生命周期管理，包括驱动进程的创建及销毁、驱动接口访问控制、驱动进程权限控制，驱动访问管理，驱动进程异常状态处理</td>
</tr><tr>
<td> permission service</td><td>SEC文件验签、权限控制等操作</td>
</tr><tr>
<td> ssa</td><td>安全存储服务，提供对数据的可信存储，并保证数据机密性、完整性、原子性和不可复制性</td>
</tr><tr>
<td> huk service</td><td>硬件根秘钥访问控制管理</td>
</tr><tr>
<td> teemiscdrv</td><td>基础驱动，获取bootloader传入的共享内存信息</td>
</tr><tr>
<td> cryptomgr</td><td>提供加解密驱动框架 </td>
</tr><tr>
<td> TEE基础API</td><td>提供TA开发的API接口支持，包括加解密、安全存储、安全时间及TA2TA GP接口等</td>
</tr><tr>
<td> TEE驱动API</td><td>提供驱动开发的API接口支持，包括中断、IO、dma操作等</td>
</tr><tr>
<td> sample</td><td>提供加载tee的示例代码teeloader和atf tee适配示例代码tee_atf</td>
</tr><tr>
<td> test</td><td>提供tee测试套件 </td>
</tr>

</table>

## tee_os_framework部件代码目录结构 ##
```
base/tee/tee_os_framework
├── framework
│   ├── gtask
│   ├── teesmcmgr
│   ├── drvmgr
│   └── tarunner
├── lib
│   ├── drvlib                    # 给驱动和drvmgr提供的lib库
│   ├── syslib                    # 只给TEE内部服务使用的lib库
│   └── teelib                    # 给TA、服务提供的lib库
├── drivers
│   ├── tee_misc_drv
│   ├── include
│   └── crypto_mgr
├── service
│   ├── permission_service
│   ├── huk_service
│   └── ssa
├── config
│   ├── release_config            # release配置信息，特性宏等
│   └── debug_config              # debug配置信息，特性宏等
├── build
├── test
└── sample
```

## tee_os_framework构建指导 ##
tee_tee_os_framework与tee_tee_os_kernel共同构建TEEOS，单独构建命令如下：

```Bash
./build.sh --product-name rk3568 --build-target tee --ccache
```

构建产物为TEEOS镜像：`base/tee/tee_os_kernel/kernel/bl32.bin`

## 相关仓

[tee_os_kernel](https://gitcode.com/openharmony-sig/tee_tee_os_kernel)
