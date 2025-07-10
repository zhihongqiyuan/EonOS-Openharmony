# tee_tee_os_kernel 仓介绍 #

## 简介 ##

OpenTrustee提供了一个可信执行环境（Trusted Execution Environment，TEE），运行在受硬件隔离的安全区域中。OpenTrustee是一套完整的TEE解决方案，包含多个部件，系统架构如图所示：

![](figures/overview-of-opentrustee.png)

tee_tee_os_kernel 部件主要包含 OpenTrustee 的内核部分，采用微内核架构设计。

## tee_tee_os_kernel 部件模块划分 ##
<table>
<th>子模块名称</th>
<th>模块简介</th>
<tr>
<td> kernel/ipc </td><td> 进程间通信模块 </td>
</tr><tr>
<td> kernel/irq </td><td> 中断处理模块 </td>
</tr><tr>
<td> kernel/mm </td><td> 内存管理模块 </td>
</tr><tr>
<td> kernel/object </td><td> 内核对象管理 </td>
</tr><tr>
<td> kernel/sched </td><td> 线程调度模块 </td>
</tr><tr>
<td> user/chcore-libs/sys-libs/libohtee </td><td> 框架所依赖的库函数 </td>
</tr><tr>
<td> user/system-services/system-servers/procmgr </td><td> 负责进程管理，拥有所有进程的信息 </td>
</tr><tr>
<td> user/system-services/system-servers/fs_base </td><td> 虚拟文件系统模块 </td>
</tr><tr>
<td> user/system-services/system-servers/fsm </td><td> 文件系统管理模块 </td>
</tr><tr>
<td> user/system-services/system-servers/tmpfs </td><td> 内存文件系统模块 </td>
</tr><tr>
<td> user/system-services/system-servers/chanmgr </td><td> 管理 channel 的命名、索引及分发 </td>
</tr>


</table>

### tee_tee_os_kernel 部件代码目录结构 ###
```
base/tee/tee_os_kernel
├── kernel
│   ├── arch
│   ├── ipc
│   ├── irq
│   ├── lib
│   ├── mm
│   ├── object
│   ├── sched
│   └── syscall
├── tool
│   └── read_procmgr_elf_tool
├── user/chcore-libs
│   ├── sys-interfaces/chcore-internal
│   └── sys-libs/libohtee
└── user/system-services/system-servers
    ├── chanmgr
    ├── fs_base
    ├── fsm
    ├── procmgr
    └── tmpfs
```

## tee_tee_os_kernel 构建指导 ##
tee_tee_os_framework与tee_tee_os_kernel共同构建TEEOS，单独构建命令如下：

```Bash
./build.sh --product-name rk3568 --build-target tee --ccache
```

构建产物为TEEOS镜像：`base/tee/tee_os_kernel/kernel/bl32.bin`

## 相关仓

[tee_os_framework](https://gitcode.com/openharmony-sig/tee_tee_os_framework)
