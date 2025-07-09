# API声明文件公共仓

## 简介

[C API](https://gitee.com/openharmony-sig/interface_sdk_c/blob/master/docs/user_guide.md)公共仓，用来提交 C API 声明头文件。OpenHarmony C API接口是操作系统提供给应用使用C/C++语言生态库的系统C接口，
是系统与应用的C能力契约；这些接口需要保持足够的稳定性。C API接口当前按照功能组织放到各个领域目录下，具体目录如下。

## 目录

```
|─ai
|  ├─neural_network_runtime
├─arkui
|  ├─ace_engine
├─bundlemanager
|  ├─bundle_framework
│  |  ├─bundle
├─commonlibrary
|  ├─memory_utils
│  |  ├─bundle
├─distributeddatamgr
|  ├─relational_store
├─drivers
|  ├─external_device_manager
├─global
|  ├─resource_management
├─graphic
|  ├─graphic_2d
├─hiviewdfx
|  ├─hiappevent
|  ├─hilog
|  ├─hitrace
├─multimedia
|  ├─audio_framework
|  ├─av_codec
|  ├─image_framework
|  ├─player_framework
├─resourceschedule
|  ├─ffrt
├─security
|  ├─huks
├─startup
|  ├─init
├─third_party


```

## 相关仓

[interface_sdk_c](https://gitee.com/openharmony-sig/interface_sdk_c/)

[C API编码规范](https://gitee.com/openharmony-sig/interface_sdk_c/blob/master/docs/capi_naming.md): 描述了OpenHarmony C API的设计规范，指导开发者如何设计发布一个接口。

[C API构建指南](https://gitee.com/openharmony-sig/interface_sdk_c/blob/master/docs/howto_add.md)：描述了如何在build.gn中添加C API的构建目标。


