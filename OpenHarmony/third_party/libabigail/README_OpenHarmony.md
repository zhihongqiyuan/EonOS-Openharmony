# libabigail

仓库包含第三方开源软件libabigail，libabigail是一个用于不同版本二进制库的ABI检查工具。开发者使用这个工具可以生成二进制库的ABI特征文件，并且可以比较两个版本之间的特征文件，从而可以看出同一个二进制库在两个版本之间是否发生了ABI变化。

## 目录结构

```
doc/           文档
include/       头文件
scripts/       脚本
src/           源文件
tests/         测试目录
tools/         工具源码
LICENSE.txt    证书文件
README         README说明
```

## OpenHarmony如何使用libabigail

SA独立升级的特性需要对相关的二进制库进行ABI检查，以保证二进制库的前向兼容。通过libabigail提供的工具对比基线的ABI特征文件与编译时实时生成的ABI特征文件，可以看出ABI是否发生了变化，如果是则编译报错。

## OpenHarmony如何集成libabigail

libabigail是作为host端的工具存在，不会随镜像发布到产品中。

### 1.libabigail编译入口

libabigail的编译入口在其根目录下的BUILD.gn中。

```makefile
group("libabigail-tools_host_toolchain") {
  deps = [
    "//third_party/libabigail/tools:abidiff($host_toolchain)",
    "//third_party/libabigail/tools:abidw($host_toolchain)",
  ]
}
```

可以看出使用了libabigail中的abidiff和abidw这两个工具。

### 2.使用libabigail

SA独立升级特性提供了新的编译模板`ohos_module_package`，其中直接指定了对libabigail的依赖。

```makefile
deps += [ "//third_party/libabigail:libabigail-tools_host_toolchain" ]
```
这样只要有模块使用了`ohos_module_package`模板，libabigail工具就会编译出来。

## libabigail相关内容

[libabigail官网](https://sourceware.org/libabigail/)

## License

`Apache License V2.0`

‍
