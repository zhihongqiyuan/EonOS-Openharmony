# 开发指导手册

### 简介

**napi-generator**初始目标为提供基于OpenHarmony标准系统,推出一些提升开发效率的NAPI框架代码生成工具。工作范围：

1. 针对开发人员、主流操作系统的IDE平台，提供NAPI框架代码生成工具
2. 提供使用范例
3. 提供持续的改进优化



### 架构图

![架构图](https://gitee.com/openharmony/napi_generator/raw/master/figures/arch.png)


### 文件结构
```
napi_generator
├── docs
├── examples
├── FAQ.md
├── figures
├── LICENSE
├── OAT.xml
├── README.md
├── release-notes
├── src
└── test
```
* docs: 文档，包括使用说明和开发指导
* example：样例，包括文本样例和应用开发样例
* FAQ.md: 常见问题说明
* figures：图例，文档里包括的图片
* LICENSE：版权说明
* OAT.xml: 开源审视配置，参看[OAT开源审查工具](https://gitee.com/openharmony-sig/tools_oat)
* README.md: 仓说明文件
* release-motes: 版本发布说明
* src: 源码
* test：测试工具


### 各个模块介绍
##### 命令行生成工具介绍
* [cmake2gn](../src/cli/cmake2gn/docs/guide/DEVELOP_ZH.md)
* [dts2cpp](../src/cli/dts2cpp/docs/guide/DEVELOP_ZH.md)
* [h2dts](../src/cli/h2dts/docs/guide/DEVELOP_ZH.md)
* [h2sa](../src/cli/h2sa/docs/guide/DEVELOP_ZH.md)
##### Intellij插件介绍
* [assist](../src/intellij_plugin/assist/assist_tools_IntelliJ_plugin/docs/guide/DEVELOP_ZH.md)
* [dts2cpp](../src/intellij_plugin/dts2cpp/napi_IntelliJ_plugin/docs/guide/DEVELOP_ZH.md)
* [h2dts](../src/intellij_plugin/h2dts/ts_IntelliJ_plugin/docs/guide/DEVELOP_ZH.md)
##### API扫描工具介绍
* [api](../src/tool/api/api_scan_IntelliJ_plugin/docs/DEVELOP_ZH.md)
##### Vscode插件介绍
* [dts2cpp](../src/vscode_plugin/readme.md)
* [h2dts](../src/vscode_plugin/readme.md)
* [h2dtscpp](../src/vscode_plugin/readme.md)
* [h2sa](../src/vscode_plugin/readme.md)
