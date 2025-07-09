# 三方开源软件CUPS
## CUPS简介
CUPS（Common Unix Printing System）是一种开源打印系统，现在由OpenPrinting组织维护。CUPS主要功能包括打印队列管理、打印驱动程序管理、网络打印支持等。CUPS支持多种打印协议，包括IPP（Internet Printing Protocol）、LPD（Line Printer Daemon Protocol）、AppSocket等。

CUPS支持以下类型的打印机：
- AirPrint和IPP Everywhere认证的打印机；
- 带打印机应用程序的网络和USB打印机；
- 基于PPD（PostScript Printer Definition）打印驱动程序的网络和本地（USB）打印机。

您也可以通过[CUPS官网主页](https://github.com/OpenPrinting/cups)了解更多关于CUPS项目的信息。

## 引入背景简述
OpenHarmony南向生态发展过程中，需要对存量市场的打印机进行兼容。使用CUPS打印系统能直接对接市场上大部分的打印机，也减少了打印机驱动适配OpenHarmony系统的难度。

## 目录结构
```
- LICENSE                           版权文件
- OAT.xml                           OAT.xml过滤配置文件
- README.OpenSource                 项目README.OpenSource文件
- README.md                         英文说明
- README_zh.md                      中文说明
- backport-CVE-xxx.patch            CVE修复补丁
- cups-x.x.x-source.tar.gz          cups源码压缩tar包
- backport-xxx.patch                上游更新补丁列表
- cups-xxx.patch                    上游更新补丁列表
- cups.spec                         上游更新记录说明
- cups.yaml                         上游yaml文件
- ohos-xxx.patch                    适配OpenHarmony编译补丁文件
- cups_single_file.patch            适配OpenHarmony编译补丁文件
- pthread_cancel.patch              适配OpenHarmony编译补丁文件
- install.sh                        适配OpenHarmony编译sh脚本文件
- generate_mime_convs.py            适配OpenHarmony编译python脚本文件
```

## 如何使用
### 1、头文件引入
```c
#include <cups/cups-private.h>
```
### 2、添加编译依赖
在您的 bundle.json 文件 添加
```json
"deps": {
  "third_party": [
    "cups"
  ]
}
```
在您的BUILD.gn需要的地方添加依赖
```json
deps += [ "//third_party/cups:cups" ]
```
### 3、接口使用示例
```c
// 使用CUPS接口查询打印机能力示例
ipp_t *request; /* IPP Request */
ipp_t *response; /* IPP response */
http_t *http = NULL;
char scheme[HTTP_MAX_URI]; // 协议类型
char username[HTTP_MAX_URI]; // 请求用户名
char host[HTTP_MAX_URI]; // 打印机ip
int port; // 打印机端口
// 声明需要查询哪些打印机能力，此处为所有
static const char * const pattrs[] = {
    "all"
};

// 使用打印机ip和端口连接打印机
http = httpConnect2(host, port, NULL, AF_UNSPEC, HTTP_ENCRYPTION_IF_REQUESTED, 1, TIME_OUT, NULL);
if (http == nullptr) {
    return;
}
// 构造获取打印机能力的request
request = ippNewRequest(IPP_OP_GET_PRINTER_ATTRIBUTES);
// 指定请求uri
ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, printerUri.c_str());
// 指定请求的用户
ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, cupsUser());
// 指定请求哪些打印机属性
ippAddStrings(request, IPP_TAG_OPERATION, IPP_TAG_KEYWORD, "requested-attributes",
(int)(sizeof(pattrs) / sizeof(pattrs[0])), NULL, pattrs);
// 调用CUPS接口发送ipp请求
response = cupsDoRequest(http, request, "/");
// 处理请求结果
if (cupsLastError() > IPP_STATUS_OK_CONFLICTING) {
    ippDelete(response);
    return;
}
// 关闭http
httpClose(http);
```

### 相关仓
[third_party_cups-filters](https://gitee.com/openharmony/third_party_cups-filters)

[print_print_fwk](https://gitee.com/openharmony/print_print_fwk)

### 参与贡献
[如何贡献](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/contribute/参与贡献.md)

[Commit message规范](https://gitee.com/openharmony/device_qemu/wikis/Commit%20message%E8%A7%84%E8%8C%83)

