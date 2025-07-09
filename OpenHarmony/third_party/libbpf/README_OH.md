# libbpf

仓库包含第三方开源软件libbpf，libbpf是eBPF技术的一种实现，eBPF提供给开发者一种内核事件和用户进程事件发生时安全注入代码的机制，避免开发者直接进行内核编程时可能导致的系统锁定、内存损坏、进程崩溃和安全漏洞等问题。开发者基于libbpf第三方开源库高效便捷开发eBPF程序，应用于网络过滤、程序跟踪、性能分析以及调试等需求场景。

## 目录结构

```
docs/          文档
fuzz/          fuzz测试
include/       头文件
scripts/       脚本
src/           源文件
LICENSE        证书文件
README.md      英文说明
README_zh.md   中文说明
```

## OpenHarmony如何集成libbpf

### 1.头文件引入

```
#include "libbpf.h"
#include "bpf.h"
```

### 2.BUILD.gn添加引用

```
deps += ["//third_party/libbpf:libbpf"]
```
### 3.示例代码
```
# kprobe.bpf.c BPF程序
# 指定LICENSE
char LICENSE[] SEC("license") = "Dual BSD/GPL";
# 进入do_unlinkat函数时调用
SEC("kprobe/do_unlinkat")
int BPF_KPROBE(do_unlinkat, int dfd, struct filename *name)
{
	pid_t pid;
	const char *filename;

	pid = bpf_get_current_pid_tgid() >> 32;
	filename = BPF_CORE_READ(name, name);
	bpf_printk("KPROBE ENTRY pid = %d, filename = %s\n", pid, filename);
	return 0;
}
# 退出do_unlinkat函数时调用
SEC("kretprobe/do_unlinkat")
int BPF_KRETPROBE(do_unlinkat_exit, long ret)
{
	pid_t pid;

	pid = bpf_get_current_pid_tgid() >> 32;
	bpf_printk("KPROBE EXIT: pid = %d, ret = %ld\n", pid, ret);
	return 0;
}
```
```
# 用bpftool生成BPF skeleton脚手架文件
$ bpftool gen skeleton kprobe.bpf.o > kprobe.skel.h
```
```
# kprobe.c 将BPF程序加载进内核并附着到内核指定位置
#include kprobe.skel.h
struct kprobe_bpf *skel;
int err;
libbpf_set_strict_mode(LIBBPF_STRICT_ALL);
/* 加载并验证BPF程序 */
skel = kprobe_bpf__open_and_load();
if (!skel) {
	fprintf(stderr, "Failed to open BPF skeleton\n");
	return 1;
}
/* 将BPF程序附加到跟踪点 */
err = kprobe_bpf__attach(skel);
/* 销毁和回收资源 */
kprobe_bpf__destroy(skel);
```
## libbpf相关知识文档

[bpf参考指南](https://nakryiko.com/posts/bpf-core-reference-guide/)

[libbpf开发指导](https://nakryiko.com/posts/bcc-to-libbpf-howto-guide/)

[libbpf开发示例](https://github.com/libbpf/libbpf-bootstrap)
## License

`SPDX-License-Identifier: BSD-2-Clause OR LGPL-2.1`

‍
