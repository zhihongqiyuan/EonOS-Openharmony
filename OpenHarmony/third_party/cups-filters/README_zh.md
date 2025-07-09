# 三方开源软件CUPS-Filters
## CUPS-Filters简介
CUPS-Filters是CUPS（Common Unix Printing System）的一部分，它包含了丰富的文件转化过滤器，其中最常用的是“imagetoraster”, 该过滤器可以将图片文件转换为raster文件传给打印机执行打印。
CUPS-Filters还提供了一些其它功能，例如打印机状态监控和报告等，跟CUPS一样，支持IPP（Internet Printing Protocal）、LPD（Line Printer Daemon Protocol）、AppSocket等打印协议

您也可以通过[CUPS-Filters官网主页](https://github.com/OpenPrinting/cups-filters)了解更多关于CUPS-Filters项目的信息。

## 引入背景简述
在OpenHarmony系统中，CUPS开源库只支持PostScript、Raster等少见的文件格式打印；为了适配图片和pdf等文件打印，需要引入CUPS-Filters来完善文件转化功能。

## 目录结构
```
- backend                           打印机通信后端支持
- banners                           banner文件
- charset                           字符相关文件
- cupsfilters                       cupsfilters库
- data                              示例文件
- drv                               示例drv文件
- filters                           文件转化过滤器文件
- fontembed                         fontembed数据库
- m4                                m4文件
- mime                              文件转化过滤器功能配置文件
- ppd                               ppd驱动示例文件
- scripting                         脚本文件
- README.OpenSource                 项目README.OpenSource文件
- README.md                         英文说明
- README_zh.md                      中文说明
- generate_cupsfilters_convs.py     适配OpenHarmony编译python脚本文件
```

## 如何使用
CUPS-Filters无法单独使用，编译出来的imagetoraster等文件转换过滤器需要配合CUPS开源库一起使用；详细使用方式参考[社区CUPS地址](https://gitee.com/openharmony/third_party_cups)

## 相关仓
[third_party_cups](https://gitee.com/openharmony/third_party_cups)

[print_print_fwk](https://gitee.com/openharmony/print_print_fwk)

## 参与贡献
[如何贡献](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/contribute/参与贡献.md)

[Commit message规范](https://gitee.com/openharmony/device_qemu/wikis/Commit%20message%E8%A7%84%E8%8C%83)

