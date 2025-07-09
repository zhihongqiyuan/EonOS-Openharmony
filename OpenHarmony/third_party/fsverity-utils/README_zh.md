# fsverity-utils

## 简介
fsverity-utils是与内核特性fs-verity配套的一组用户态工具。fs-verity是Linux Kernel提供的保护文件完整性的机制，可以实现以文件为粒度的按需保护可写分区的只读文件。可选地，fs-verity提供对文件签名的验证，以保证文件的真实性。

**1.  组件引入背景**
为了OpenHarmony上使用内核特性fs-verity而提供的三方库接口。

**2. 使用场景**
可以通过动态库或者静态库的方式依赖此组件，调用接口计算fs-verity的摘要、签名。

**3. 为OpenHarmony带来的价值**
满足在OpenHarmony上使能内核特性fs-verity的诉求。

## OpenHaromny中如何使用fsverity-utils

1. 在BUILD.gn中增加编译依赖
```
include_dirs = [
  "//third_party/fsverity_utils/include",
  "//third_party/fsverity_utils/common"
]

deps = [
  # 动态库依赖（可选）
  "//third_party/fsverity_utils:libfsverity_utils",
  # 静态库依赖（可选）
  "//third_party/fsverity_utils:libfsverity_utils_static" 
]
```

2. libfsverity_utils(_static)提供的主要接口

| 接口名称 |  功能  |
| --------------------- | ------------------ |
| libfsverity_enable | 对文件使能fs-verity保护 |
| libfsverity_enable_with_sig | 对文件使能带fs-verity签名 |
| libfsverity_compute_digest | 计算文件的fsverity摘要 |
| libfsverity_sign_digest | 对fsverity摘要进行签名 |

具体使用方法和其他接口见`include/libfsverity.h`。


## fsverity-utils使用文档

代码仓库：https://git.kernel.org/pub/scm/fs/fsverity/fsverity-utils.git

fs-verity介绍：https://www.kernel.org/doc/html/latest/filesystems/fsverity.html