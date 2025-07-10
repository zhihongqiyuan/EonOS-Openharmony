# selinux_adapter

## 简介

SELinux （安全增强式 Linux ， Security-Enhanced Linux ）是 Linux 历史上杰出的安全组件, 包含一组内核修改和用户空间工具，并提供了基于安全策略的强制访问控制机制（ Mandatory Access Control ， MAC ）。SELinux 已经被添加到各种 Linux 发行版中。其软件架构力图将软件执行与安全策略设计分离。本部件负责对文件，属性，服务等系统资源提供强制访问控制保护。提供neverallow规则限制系统中的高危操作，减少系统安全风险。

访问控制基本流程如下图所示：
![整体架构](docs/images/update.png)

## 目录

```
.
├── config                  # 板侧    三方库配置文件
├── docs                    #         文档资源
│   └── images
├── interfaces
│   ├── policycoreutils     # 板侧    libload_policy.so、librestorecon.so
│   │   ├── include
│   │   └── src
│   └── tools               # 板侧    load_policy、restorecon
│       ├── load_policy
│       └── restorecon
├── scripts                 # 编译侧  策略编译脚本
├── sepolicy                # 编译侧  策略文件
└── test                    #         测试程序
```

## 约束

目前Selinux只支持RK3568

## 使用说明

### 编译镜像

运行以下命令编译打包支持 SELinux 的镜像：

```
本模块单独编译命令
./build.sh --product-name=rk3568 -T selinux_adapter --ccache
```
### 运行验证

将镜像烧录到开发板上，开机，通过串口拿到 Shell ，在其中执行：

```
ls -lZ /         # 查看文件标签
ls -lLZ /        # 查看link源文件标签
ps -eZ           # 查看进程标签
setenforce 1     # 使能selinux强制模式
setenforce 0     # 是能selinux宽容模式,当前默认宽容模式
getenforce       # 获取selinux工作模式
```
策略文件            /etc/selinux/targeted/policy/policy.31

文件标签规则        /etc/selinux/targeted/policy/file_contexts

selinux模式开关     /etc/selinux/config

验证时，可单独替换上述文件。

### 日志信息

```
audit: type=1400 audit(1502458430.566:4): avc:  denied  { open } for  pid=1658 comm="setenforce" path="/sys/fs/selinux/enforce" dev="selinuxfs" ino=4 scontext=u:r:hdcd:s0 tcontext=u:object_r:selinuxfs:s0 tclass=file permissive=1

日志解读
open                                #操作为open
pid=1658                            #访问主体进程号为1658
comm="setenforce"                   #访问主体进程名为setenforce
path="/sys/fs/selinux/enforce"      #被访问客体为/sys/fs/selinux/enforce
dev="selinuxfs"                     #被访问文件属于selinuxfs这一文件系统
ino=4                               #文件节点编号为4
scontext=u:r:hdcd:s0                #访问主体selinux标签为u:r:hdcd:s0
tcontext=u:object_r:selinuxfs:s0   #被访问客体selinux标签为u:object_r:selinuxfs:s0
tclass=file                         #当前告警属于file类型的操作
permissive=1                        #当前selinux处于宽容模式，只告警不做访问拦截。强制模式时，做拦截， permissive=0
```

### 策略编写

```
根据avc告警，获取访问信息
如：
audit: type=1400 audit(1502458430.566:4): avc:  denied  { open } for  pid=1658 comm="setenforce" path="/sys/fs/selinux/enforce" dev="selinuxfs" ino=4 scontext=u:r:hdcd:s0 tcontext=u:object_r:selinuxfs:s0 tclass=file permissive=1
对应规则为
allow hdcd selinuxfs:file open;
```

## 相关仓 

涉及到的仓库有以下几个：

| 仓库 | 源码目录 | 说明 |
| --- | --- | --- |
| [selinux_adapter](https://gitee.com/openharmony/security_selinux_adapter.git) | `base/security/selinux_adapter/` | 策略和一些自研接口 |
| [third_party_selinux](https://gitee.com/openharmony/third_party_selinux.git) | `third_party/selinux/` | SELinux 的主仓库 |
| [productdefine_common](https://gitee.com/openharmony/productdefine_common.git) | `productdefine/common/` | 添加 SELinux 组件定义 |
| [third_party_toybox](https://gitee.com/openharmony/third_party_toybox.git) | `third_party/toybox/` | 完善了 `ls` 的 SELinux 支持 |
| [startup_init_lite](https://gitee.com/openharmony/startup_init_lite.git) | `base/startup/init_lite/` | 系统启动加载策略并分化服务的标签 |
| [third_party_FreeBSD](https://gitee.com/openharmony/third_party_FreeBSD.git) | `third_party/FreeBSD/` | 提供 fts 库 |
| [third_party_pcre](https://gitee.com/openharmony/third_party_pcre2.git) | `third_party/pcre/` | 提供 pcre2 库 |
| [build](https://gitee.com/openharmony/build.git) | `build/` | 编译控制 |

