# VIXL

## 简介

VIXL是一个用于生成和解码ARM、AArch64平台指令的开源库。

VIXL包括三个组件：

- 汇编器：在运行时生成AArch64、AArch32 (Arm and Thumb-2) 指令。汇编器抽象了ISA（Instruction Set Architecture）的约束，例如，大多数指令支持即时操作。
- 反汇编：可以输出汇编程序编译出的指令所对应的反汇编内容。
- 模拟器：可以模拟AArch64汇编器编译的任何指令。模拟器允许生成的代码在另一个体系结构（如AArch32架构）上运行，而不需要完整的ISA模型。

为了方舟编译器运行时公共组件能够编译ARM、AArch64平台代码，引入了VIXL。

## 接口说明

VIXL的一些常用接口说明：
- `MacroAssembler`类：用于生成ARM指令的类。它提供了一组函数，用于生成不同类型的指令，如MOV、ADD、SUB等。
- `Label`类：用于标记指令的类。它提供了一个`Bind`函数，用于将标签绑定到指令地址上。
- `CodeBuffer`类：是存储生成的指令的类。它提供了一组函数，用于将指令写入缓冲区中。

-  `Decoder`类：是解码ARM指令的类。它提供了一个`DecodeInstruction`函数，用于解码指令。

-  `Instruction`类：是表示ARM指令的类。它提供了一组函数，用于获取指令的不同部分，如操作码、寄存器等。

- `CPUFeatures`类：是表示CPU特性的类。它提供了一组函数，用于设置和查询CPU特性。

更多详细信息可以参考[接口文档](doc/aarch64/supported-instructions-aarch64.md)。


## 编译

使用OpenHarmony中编译部件的方式进行编译即可：
1. 主干代码下载
   ```
   repo init -u https://gitee.com/openharmony/manifest.git -b master --no-repo-verify
   repo sync -c
   repo forall -c 'git lfs pull'
   ```
2. 预处理
   ```
   ./build/prebuilts_download.sh
   ```
3. 编译
   ```
   ./build.sh --product-name rk3568 --build-target libvixl_frontend_static
   ```
   编译生成物对应路径：`out/rk3568/obj/third_party/vixl/libvixl_frontend_static.a`。

## 许可证

本项目遵从[LICENCE](LICENCE)中所描述的许可证。