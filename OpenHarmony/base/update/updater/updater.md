## 简介
升级包安装组件运行在updater分区，其功能主要包括读取misc分区信息获取升级包状态，对升级包进行校验，确保升级包合法有效；然后从升级包中解析出升级的可执行程序，创建子进程并启动升级程序。具体升级的动作由升级脚本控制。本文将介绍如何针对OpenHarmony L2场景适配updater模式。

## 前置条件
- 参考OpenHarmony官方指导，完成正常系统编译和内核启动，能进入正常模式，且运行正常。
- 芯片需配置包含updater分区和misc分区的分区表。updater分区大小不小于32M。

## 镜像编译
镜像编译流程：

![](figures/updater适配_编译流程.png)

### 使能updater子系统构建
修改编译配置文件，使能updater.img编译选项
以RK3568为例，在build/subsystem_config.json文件添加配置如下配置。当前默认参与构建。

```
      "updater": {
        "path": "base/update",
        "name": "updater"
      },
```

2.产品配置中添加updater组件编译
以RK3568为例，在productdefine/common/products/ohos-arm64.json中添加

```
       "updater:update_service":{},
```

### 组件构建
OpenHarmony使用BUILD.gn和ninja等工具构建编译框架。编译的二进制文件和动态库需要修改BUILG.gn文件将其复制到updater目录。在对应的BUILD.gn文件中添加如下配置：

```
      install_images = [
        "system",
        "updater",
      ]
```

静态库会编译进二进制文件和动态库。
编译系统根据install_image选项将组件复制到out/rk3568/packages/phone/updater目录。

### 镜像编译
使能镜像编译选项，编译脚本会将out/rk3568/packages/phone/updater文件夹打包成成updater.img镜像。先使用gzip工具压缩，然后使用cpio工具制作镜像。镜像格式为ASCII cpio archive (SVR4 with no CRC)。updater.img没有内核，以根文件系统挂载。out/rk3568/packages/phone/updater目录结构与根文件系统结构一致。如果内核不支持gzip解压，需要修改脚本，否则会导致镜像启动失败。以RK3568为例，updater.img大小限制为32M，过多添加文件到updater.img会导致镜像超过32M编译失败，可以根据需求修改镜像最大值。

## 镜像启动
完整的启动流程如下：

![](figures/updater适配_启动流程.png)
### 配置MISC分区
OpenHarmony使用MISC分区保存启动时的指令，默认的MISC分区的结构体为：

```
    struct UpdateMessage {
        char command[32];
        char status[32];
        char update[768];
        char stage[32];
        char keyinfo[32];
        char reserved[224];
    };
```
command为当前的命令，update存放升级包路径，reserved存放保留信息。
### 配置uboot和内核
uboot需要读取MISC分区的指令，如果command为updater，uboot能启动到updater模式。内核需要根据uboot的指令加载updater.img。
updater模式启动与正常模式启动流程一致。当正常模式调通后，参考正常模式完成updater模式的调试。
updater.img镜像是ramdisk格式且没有内核，需要uboot先拉起内核，然后加载updater.img到内存，作为根文件系统挂载到内核。如果芯片不支持ramdisk格式，需要进一步适配。

### init服务启动
init进程启动之后，读取init.cfg配置文件，依次启动各关键进程。
启动配置文件由以下配置文件组成：
- 与产品相关的配置文件
以RK3568为例，updater模式的init.cfg文件位于产品目录device/board/hihope/rk3568/updater下。init.cfg是启动的入口，通过import方式导入其他配置文件。init.rk3568.usb.cfg主要是USB和HDC相关的配置，与产品特性有关。
- updater_common.cfg
updater模式通用的启动，代码目录。updater_common.cfg包含了多个基础服务，这些服务与产品解耦，依赖系统组件。
- 复用正常模式的配置文件
为了降低耦合复用正常模式的配置文件。这部分配置文件适用于正常模式和updater模式，不需要针对性适配。通过BUILD.g设置复制到updater.img，通过import方式调用。
init.usb.cfg：HDC相关的配置
init.usb.configfs.cfg ：USB相关的配置
faultloggerd.cfg：faultloggerd服务配置文件，用于捕获updater模式的crash异常
hilogd.cfg：hilogd配置文件，用于打印OpenHarmony所有组件的日志。
配置文件需要修改对应的BUILD.gn文件，拷贝到产品out目录，最终打包到updater.img镜像。参考如下配置：

```
        ohos_prebuilt_etc("updater_common.cfg") {
          source = "updater_common.cfg"        
          install_images = [ "updater" ]       #打包到updater.img
          part_name = "updater"
        }
```

### 升级服务启动
完成配置文件的适配，单板运行时会根据配置文件顺序依次执行配置中的命令。最终拉起升级子系统核心服务updater进程。updater服务启动以后，屏幕会显示UI界面，进行安装包升级，恢厂，重启等功能。升级服务功能如下所示：
![](figures/Openharmony-updater-升级子系统架构图.png)

初次适配会出现进程功能缺失等情况，参考如下方法进行调试
- 使用ps -A命令查询updater，hdcd等关键进程是否启动。
- 使用dmesg查看内核日志，是否是init服务异常。
- 使用hilog和faultlogger查看进程是否出现crash，其他组件是否正常。
- 手动拉起服务，如果服务能启动，检查启动配置文件是否正确。

### OTA升级
正常模式有两个子程序用于操作misc分区。
执行write_updater命令向misc分区的update字段写入指令，指令格式参考参考write_updater源码base/update/updater/utils/write_updater.cpp。
执行reboot updater命令，向misc分区的command写入指令，单板会重新启动，重启过程中uboot读取misc分区的命令，从command命令判断当前要进入updater模式，依次加载内核和updater.img,完成从正常模式到updater模式的启动流程。
updater进程启动后读取update字段的命令，获取升级包的路径，单板进入升级流程，直至升级成功重启。
### updater模式界面
正常模式只输入reboot updater命令，进入updater模式界面。此模式可以进行SD卡升级，恢厂等操作。

### flashd模式
flashd模式介绍：flashd是OpenHarmony升级子系统的一个刷机模式，可以提供格式化用户分区，擦除分区，刷写镜像，zip格式整包升级的功能。
Flashd分为客户端和服务端，客户端提供统一的用户界面，服务端提供对应的功能服务，二者之间通过HDC作为数据传输通道。适配Flashd模式，可以提供刷机脚本刷机，工厂模式等功能。
updater模式为常规升级模式，只能进行升级包升级和恢厂设置等功能。
flashd模式和updater模式共用一个进程，入口函数在base/update/updater/services/main.cpp的main()函数,读取misc信息判断当前是否进入flashd模式。
正常模式输入以下命令进入flashd模式：

```
    # write_updater boot_flash
    # reboot updater
```

flashd模式调试具体参考：。

## 驱动适配
updater模式屏幕，触控等参考：
