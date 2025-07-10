# 证书算法库框架

## 简介
证书算法库框架是一个屏蔽了第三方证书算法库实现差异的算法框架，提供证书、证书扩展域段、证书吊销列表的解析及校验能力，此外还提供了证书链的校验能力。开发者可以通过调用证书算法库框架接口，忽略底层不同三方算法库的差异，实现迅捷开发。

**图 1**  证书算法库框架-架构图


![](figures/zh-cn_certificate_framework_architecture.png)

其中，

-   API接口层：对外提供统一的JS接口，提供的能力包括：证书、证书扩展域段、证书吊销列表等解析能力，证书链的校验能力。
    *   证书操作主要包含：根据业务传入的证书数据，获取证书的版本号、序列号、颁发者、主题、签名算法、公钥等。
    *   证书扩展域段操作主要包含：根据业务传入的证书扩展域段数据，获取证书扩展域段的OID（对象标识符）列表，以及根据OID获取具体的数据。
    *   证书吊销列表操作主要包含：根据证书吊销列表获取被吊销的证书，以及该证书的序列号、颁发者、被吊销时间等。
    *   证书链操作主要包含：根据业务传入证书链数据，校验各级证书的签发关系有效。
-   框架实现层：主要实现内部对象的管理，通过灵活加载算法库适配层，适配并屏蔽三方算法库的差异。
-   算法库适配层：依赖OpenSSL或者Mbed TLS算法库调用其具体接口实现对上层业务提供的能力，例如实现证书的解析，获取证书的字段等能力。

## 目录
```
base/security/certificate_framwork
├── bundle.json              # 部件配置文件
├── cf.gni                   # 编译配置文件
├── config                   # 配置构建相关
├── figures                  # README相关图片
├── frameworks               # 框架实现层
│   ├── ability              # 框架层能力注册
│   ├── adapter              # 算法库适配层
│   ├── common               # 内部依赖的公共方法
│   ├── core                 # 证书算法库框架核心实现
│   └── js
│       └── napi             # 通过napi封装的JS接口代码实现
├── interfaces               # 对外接口目录
└── test                     # 测试用例
```

## 编译构建

在OpenHarmony源码根目录下，调用以下指令，单独编译证书算法库框架部件。
```shell
./build.sh --product-name rk3568 --ccache --build-target certificate_framework
```
> **说明：**
> 
> --product-name：产品名称，例如rk3568。
>
> --ccache：编译时使用缓存功能。
>
> --build-target: 编译的部件名称。

## 约束

[约束与限制](https://gitee.com/openharmony/docs/blob/master/zh-cn/application-dev/security/DeviceCertificateKit/certificate-framework-overview.md#约束与限制)

## 说明

### 接口说明

[接口文档](https://gitee.com/openharmony/docs/blob/master/zh-cn/application-dev/reference/apis-device-certificate-kit/js-apis-cert.md)

### 使用说明

[开发指导](https://gitee.com/openharmony/docs/blob/master/zh-cn/application-dev/security/DeviceCertificateKit/certificate-framework-overview.md)

## 相关仓

[安全子系统](https://gitee.com/openharmony/docs/blob/master/zh-cn/readme/安全子系统.md)

[security\_crypto\_framework](https://gitee.com/openharmony/security_crypto_framework)

[**security\_certificate\_framework**](https://gitee.com/openharmony-sig/security_certificate_framework)