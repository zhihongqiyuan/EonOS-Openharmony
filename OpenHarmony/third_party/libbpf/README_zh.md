这是[Linux源码bpf](https://kernel.googlesource.com/pub/scm/linux/kernel/git/bpf/bpf-next)的`tools/lib/bpf`​目录及其支持的头文件的一个镜像。同步的所有细节都可以在`scripts/sync-kernel.sh`​脚本中找到。

此仓中的一些头文件（`include/linux/*.h`​）对应文件是[Linux源码bpf](https://kernel.googlesource.com/pub/scm/linux/kernel/git/bpf/bpf-next)的`tools/include/linux/*.h，使得libbpf能够编译成功。

# BPF/libbpf用法和问题

请查看[libbpf引导程序](https://github.com/libbpf/libbpf-bootstrap)和配套得[博客文章](https://nakryiko.com/posts/libbpf-bootstrap/)，使用libbpf构建BPF应用程序的示例。

[libbpf-tools](https://github.com/iovisor/bcc/tree/master/libbpf-tools)也是基于libbpf的跟踪工具的一个很好的实现。

另请参见[“BPF CO-RE参考指南”](https://nakryiko.com/posts/bpf-core-reference-guide/)，涵盖构建BPF CO-RE应用程序的实际方面，

以及[“BPF CO-RE”](https://nakryiko.com/posts/bpf-portability-and-co-re/)对于BPF可移植性问题和BPF CO-RE起源的一般介绍。

所有一般BPF问题，包括内核功能、libbpf API以及申请，应发送邮件至[bpf@vger.kernel.org](mailto:bpf@vger.kernel.org)。

你可以订阅[此处](http://vger.kernel.org/vger-lists.html#bpf)和搜索其档案[此处](https://lore.kernel.org/bpf/). 在提出新问题之前请搜索档案，很可能这已经是在之前被定位或回答。

[bpf@vger.kernel.org](mailto:bpf@vger.kernel.org)被很多人管理，他们会很乐意尝试帮助你解决任何问题。

# 构建指导

libelf是libbpf的内部依赖项，因此需要链接并且必须安装在系统上，应用程序才能工作。

pkg-config默认用于查找libelf，调用的程序可以是用“PKG_CONFIG”重写。

如果不希望在构建时使用“pkg config”，可以通过调用make时设置`NO_PKG_CONFIG=1`​。

构建静态库libbpf.a和动态库libbpf.so：

```
$cd src
$make
```

仅生成静态库libbpf.a

```
$cd src
$mkdir build root
$BUILD_STATIC_ONLY=y OBJDIR=build DESTDIR=root make install
```

构建静态库libbpf.a和动态库libbpf.so

```
$cd src
$PKG_CONFIG_PATH=/build/root/lib64/pkgconfig DESTDIR=/build/root make install
```

# 发行版

从此镜像打包libbpf的发行版：

-[Fedora](https://src.fedoraproject.org/rpms/libbpf)

-[Gentoo](https://packages.gentoo.org/packages/dev-libs/libbpf)

-[Debian](https://packages.debian.org/source/sid/libbpf)

-[Arch](https://www.archlinux.org/packages/extra/x86_64/libbpf/)

-[Ubuntu](https://packages.ubuntu.com/source/impish/libbpf)

-[Alpine](https://pkgs.alpinelinux.org/packages?name=libbpf)

从镜像打包优于从内核源打包的好处：

* 跨发行版的一致版本控制。

* 与任何特定内核无关，对旧内核的透明处理。

  Libbpf被设计为内核不可知的，可以跨多个内核版本。

  它有内置的机制，可以兼容老版本缺少某些功能的内核

  因此，libbpf没有绑定到特定的内核版本，并且可以/应该独立地打包和版本化。

* 通过以下方式进行持续集成测试

  [TravisCI](https://travis-ci.org/libbpf/libbpf).

* 通过[LGTM](https://lgtm.com/projects/g/libbpf/libbpf)和[Coverity](https://scan.coverity.com/projects/libbpf)进行静态代码分析

libbpf的包依赖性，包名称可能因发行版而异：

* zlib

* libelf

# BPF CO-RE（编译一次-到处运行）

libbpf支持构建支持BPF CO-RE的应用程序，这与[BCC](https://github.com/iovisor/bcc/)不同，不需要Clang/LLVM运行时部署到目标服务器并且不依赖于内核层的headers可用。
但是它依赖于使用[BTF](https://www.kernel.org/doc/html/latest/bpf/btf.html)构建的内核

尽管如此，一些主要的Linux发行版已经内置了内核BTF：

* Fedora31+
* RHEL 8.2+
* OpenSUSE Tumbleweed（下一个版本，截至2020-06-04）
* Arch Linux（来自内核5.7.1.arch1-1）
* Manjaro（如果在2021 06月18日之后编译，则来自内核5.4）
* Ubuntu 20.10
* Debian 11（amd64/arm64）

如果您的内核没有内置BTF，则需要构建自定义内核

您需要：

* pahole 1.16+工具（dwarves包的一部分），用于执行DWARF到BTF转换；

* 使用`CONFIG_DEBUG_INFO_BTF=y`​选项构建的内核；

* 您可以通过查找`/sys/kernel/btf/vmlinux`​文件来检查内核是否内置了BTF：

```
$ ls -la /sys/kernel/btf/vmlinux
-r--r--r--. 1 root root 3541561 Jun  2 18:16 /sys/kernel/btf/vmlinux
```

要开发和构建BPF程序，您需要Clang/LLVM 10+。以下发行版默认情况下具有Clang/LLVM 10+包：

* Fedora 32+

* Ubuntu 20.04+

* Arch Linux

* Ubuntu 20.10（LLVM 11）

* Debian 11（LLVM 11）

* Alpine 3.13+

否则，请确保在您的系统上更新它。

以下资源有助于理解什么是BPF CO-RE以及如何使用它：

* [BPF CO-RE参考指南](https://nakryiko.com/posts/bpf-core-reference-guide/)

* [BPF可移植性和CO-RE](https://nakryiko.com/posts/bpf-portability-and-co-re/)

* [HOWTO:BCC到libbpf的转换](https://nakryiko.com/posts/bcc-to-libbpf-howto-guide/)

* [BCC仓库中的libbpf工具](https://github.com/iovisor/bcc/tree/master/libbpf-tools)

# 许可证

本作品根据BSD 2条款许可证和GNU LGPL v2.1许可证双重许可。

如果您使用这项工作，您可以在其中一项中进行选择。

`SPDX-License-Identifier: BSD-2-Clause OR LGPL-2.1`
