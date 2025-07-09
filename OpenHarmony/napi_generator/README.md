# 简介

**napi-generator**初始目标为提供基于OpenHarmony标准系统,推出一些提升开发效率的NAPI框架代码生成工具。工作范围：

1. 针对开发人员、主流操作系统的IDE平台，提供NAPI框架代码生成工具
2. 提供使用范例
3. 提供持续的改进优化



**工作目标**：

| 工作目标                | 完成情况 | 备注                                                         |
| ----------------------- | -------- | ------------------------------------------------------------ |
| NAPI生成工具            | V1.4.1版 | 可进行小规模的自动生成，基本功能集：可通过简单dts或h文件生成对应代码 |
| TS生成工具（h2dts）     | V1.0.0版 | 提供命令行和VSCode及Ideal插件，基本功能集：可将简单的h文件转换为dts文件 |
| Gn生成工具（cmake2gn）  | V1.0.0版 | 基本功能集：在具备cmake编译的环境下能实现gn文件的自动生成，但是有多工程引用及其余依赖情况可能存在问题 |
| Service生成工具（h2sa） | V1.0.0版 | 基本功能集：通过h文件生成sa服务框架代码，包括Ability注册、proxy/stub类实现、MessageParcel数据包构造、Service子系统编译及服务配置文件 |
| API平台依赖分析扫描工具 | V1.0.0版 | 扫描三方库中包含OpenHarmony源码不包含的接口，并输出result.xlsx文档 |
| HCS可视化工具           | V1.0.0版 | 详情参看：drivers_hdf_core/framework/tools/hcs-view          |
| IDL生成工具             | V1.0.0版 | 详情参看：drivers_hdf_core/framework/tools/idl-gen           |
| 提供使用范例            |          | 详情参看本仓example下的内容                                  |
|                         |          |                                                              |
|                         |          |                                                              |



**架构图**

![架构图](./figures/arch.png)



**具体模块说明**

- dts2cpp工具可以根据用户指定路径下的ts(typescript)接口文件一键生成NAPI框架代码、业务代码框架、GN文件等。

  [readme](https://gitee.com/openharmony/napi_generator/blob/master/src/cli/dts2cpp/dts2cpp_README_ZH.md)

  [版本说明](https://gitee.com/openharmony/napi_generator/tree/master/src/cli/dts2cpp/docs/release-notes)

- h2dts工具可以根据定义在c++头文件中的接口，生成type-script语言的ts接口文件。

  [readme](https://gitee.com/openharmony/napi_generator/blob/master/src/cli/h2dts/h2dts_README_ZH.md)

  [版本说明](https://gitee.com/openharmony/napi_generator/tree/master/src/cli/h2dts/docs/release-notes/ts_Gen-1.0.md) 

- cmake2gn工具可以根据三方库的CMakeLists.txt文件，编译转换生成BUILD.gn脚本文件。

  [readme](https://gitee.com/openharmony/napi_generator/blob/master/src/cli/cmake2gn/cmake2gn_README_ZH.md)

  [版本说明](https://gitee.com/openharmony/napi_generator/tree/master/src/cli/cmake2gn/docs/release-notes/gn-gen-release-notes-0.0.2.md)  

- h2sa工具可以根据一个定义远程方法的.h头文件，生成整个Service框架的代码，包含Ability注册、proxy/stub类实现、MessageParcel数据包构造、Service子系统编译及开机自启动相关配置文件。

  [readme](https://gitee.com/openharmony/napi_generator/blob/master/src/cli/h2sa/README_ZH.md)

  [版本说明](https://gitee.com/openharmony/napi_generator/tree/master/src/cli/h2sa/docs/release-notes/Service-1.0.md)  

- h2dtscpp工具可以根据定义在c++头文件中的接口，生成type-script语言的ts接口文件、NAPI框架代码、和自动化测试用例代码。

  [readme](https://gitee.com/openharmony/napi_generator/blob/master/src/cli/h2dtscpp/h2dtscpp_README_ZH.md)

- scan工具，它可以扫描三方库中包含OpenHarmony源码不包含的接口，并输出result.xlsx文档。

  [readme](https://gitee.com/openharmony/napi_generator/blob/master/src/tool/api/scan_README_ZH.md)


**相关仓**

[napi](https://gitee.com/openharmony/arkui_napi)
[safwk](https://gitee.com/openharmony/systemabilitymgr_safwk)
[samgr](https://gitee.com/openharmony/systemabilitymgr_samgr)
[hdf](https://gitee.com/openharmony/drivers_hdf_core)
[hidumper](https://gitee.com/openharmony/hiviewdfx_hidumper)
[hilog](https://gitee.com/openharmony/hiviewdfx_hilog)
[hisysevent](https://gitee.com/openharmony/hiviewdfx_hisysevent)
