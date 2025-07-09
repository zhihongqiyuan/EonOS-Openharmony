#  API声明文件公共仓

## 简介

JS/TS API 公共仓，用来提交 API d.ts 声明文件以及API相关工具。

## 目录

```|
├─api
|  ├─@internal
│  |  ├─component
│  |  |  └─ets                 #基于TS扩展的声明式开发范式组件声明文件
|  |  └─ets 
|  ├─config                    #基于JS扩展的类Web范式
|  ├─form                      #JS服务卡片
|  ├─@ohos.×××.d.ts            #API声明文件
|  └─@system.×××.d.ts          #标记为停止维护的接口
├─build-tools
   ├─api_check_plugin              #检查API规范的工具
   |  ├─config
   |  ├─plugin
   |  ├─src
   |  └─test
   └─cllect_application_api        #解析应用到的API的工具（旧）
   |  ├─deps
   |  └─src
   └─api_collector                 #解析应用到的API的工具的工具（新）
   |  ├─scripts
   |  ├─src
   |  └─test
   └─api_diff                      #比较两个版本sdk差异的工具（旧）
   |  ├─src
   |  └─version_diff_collection
   └─collect_api                   #解析d.ts文件接口的基本信息的工具
   |  └─collect_base_api.js
   └─diff_api                      #比较两个版本sdk差异工具（新）
   |  └─src
   └─dts_parser                    #解析d.ts文件的工具
   |  ├─src
   |  └─test
   └─jsdoc_format_plugin           #检查并修复jsdoc规范错误的工具
   |  ├─loader
   |  ├─src
   |  └─test
   └─permissions_converter         #从config文件提取权限信息，为DevEco Studio提供权限的联想和校验
   |  └─convert.js
   └─sdk_upgrade_assistance_plugin #SDK升级辅助工具，帮助开发者快速解决SDK升级导致的API不兼容问题
   |  └─src
```

## 相关仓

[interface-sdk_js](https://gitee.com/openharmony/interface_sdk-js/tree/master)

