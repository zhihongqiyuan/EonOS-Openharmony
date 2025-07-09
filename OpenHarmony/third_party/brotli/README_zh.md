# third_party_brotli

## Brotli简介
Brotli是基于LZ77算法，霍夫曼编码以及二阶上下文建模的开源数据压缩算法。
OpenHarmony引入该算法用于增强http的基础能力，使之可以处理使用brotli压缩算法的http请求。

源代码最新参考资料可以访问：[brotli](https://github.com/google/brotli)

引入版本[openEuler-brotli-1.0.9](https://gitee.com/src-openeuler/brotli)

## 引入背景描述
当前HTTP模块不支持该算法，无法满足社区需求。

## 为OpenHarmony带来的价值
生态应用在使用OpenHarmony提供的HTTP协议栈时，可以使用brotli的高压缩比特性，节省流量。

## 如何使用
在发送http请求时header信息带上"Accept-Encoding:br"，则服务端会以brotli压缩的方式响应请求。

brotli算法适用于静态网页数据、压缩阶段不敏感型、解压缩阶段敏感型等场景下的数据处理。

## 软件架构
```
LICENSE           #版权声明
v.1.0.9.tar.gz    #源码压缩包
install.sh        #解压缩以及打补丁脚本
patch             #补丁
BUILD.gn          #编译程序
```


## 安装教程

1.  在使用该库的编译程序中加入依赖
```
deps = ["//third_party/brotli:brotli"]
```
2.  预处理
```
./build/prebuilts_download.sh
```
3.  编译
```
./build.sh --product-name rk3568 --ccache
```
编译生成物对应路径： 'out/rk3568/thirdparty/brotli/libbrotli.z.so'

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


