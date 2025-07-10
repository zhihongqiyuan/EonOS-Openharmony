# 证书管理<a name="ZH-CN_TOPIC_0000001096592945"></a>

## 简介<a name="section11660541593"></a>

证书管理主要提供系统级的证书管理能力，实现证书全生命周期（安装，存储，使用，销毁）的管理和安全使用 ，满足生态应用和上层业务的诉求。 证书管理架构如下图所示：
图1 证书管理架构
![](figures/zh-cn_certificate_manager_architecture.png)

证书管理模块可以分为如下三大部分：

- SDK层：提供证书管理 API，供应用调用。
- Service层：实现证书全生命周期管理。
- Engine层：证书管理核心模块，负责证书的安装、存储、授权、使用、销毁等工作。其中密钥相关操作依赖于当前设备中的[HUKS](https://gitee.com/openharmony/security_huks)能力。

证书生命周期管理：
- 证书安装：使用者可以通过安装接口，传入证书文件或密钥库文件，实现证书的安装。
- 证书存储：证书管理模块将用户传入的证书、或设备端生成的证书，存储在/data/service/el1/public/cert_manager_service/certificates目录下，并使用UserID和UID对证书进行隔离。对应的密钥会存储在HUKS模块中。
- 证书使用：通过查询对应的证书，使用者可获取到证书文件进行业务相关操作。
- 证书销毁：删除接口允许使用者，批量或单张销毁存储在证书管理中的证书，相对应的密钥也会在HUKS模块中被删除。

通过使用方式，可以将证书分为如下两类：
- CA证书：只含有公钥，适用于验签或验证对端身份。
- 业务证书：含有公钥、私钥，适用于业务场景的签名和验签。

## 目录<a name="section161941989596"></a>

```
base/security/certificate_manager/
├── config                            # 系统根证书文件
├── frameworks                        # 框架代码, 作为基础功能目录, 被interfaces和services使用
├── interfaces                        # 接口代码
│   └── innerkits                     # c接口代码
│   └── kits                          # napi代码
├── services                          # 服务层代码
│   └── cert_manager_standard         # 证书管理核心功能代码
├── test                              # 测试资源存放目录
```
## 说明<a name="section1312121216216"></a>

### 接口说明<a name="section1551164914237"></a>
证书管理相关JS接口将在后续API版本中体现，具体参见证书管理使用指南。

## 相关仓<a name="section1371113476307"></a>

**安全子系统**

**security_huks**

**security_privacy_center**

