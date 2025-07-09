# OpenHarmony资源编译工具

## 简介
restool（资源编译工具）是一种资源构建工具。通过编译资源文件创建资源索引、解析资源。restool保存在sdk安装目录下的toolchains子目录。

## 代码目录

```
/developtools
|----global_resource_tool
|    |----include         #头文件
|    |----src             #源代码文件
|    |----test            #测试用例
|    |----build           #依赖三方库编译脚本  
|    |----BUILD.gn        #编译脚本
|    |----CMakeLists.txt  #CMake文件
|    |----win32.cmake     #Windows交叉编译脚本  
```

## 使用说明

### SDK编译命令

[SDK编译命令参考](https://gitee.com/openharmony/build/blob/master/README_zh.md)

### 参数说明

#### restool支持的命令选项

| 选项 | 是否可缺省 | 是否存在入参 | 描述 |
| -------- | -------- | -------- | -------- |
| -i/--inputPath | 不可缺省 | 带参数 | 指定需要构建的资源目录或者资源中间件。<br>在资源目录中支持指定一个编译好的Hap资源目录（解压态），在此Hap的基础上完成叠加编译。|
| -j/--json | 不可缺省 | 带参数 | 指定config.json或者module.json文件路径。 |
| -o/--outputPath | 不可缺省 | 带参数 | 指定已编译资源的输出路径。 |
| -p/--packageName | 不可缺省 | 带参数 | 指定编译资源的bundle名称。 |
| -r/--resHeader | 不可缺省 | 带参数 | 指定资源的头文件路径，有三种格式：“.txt”、“.js”、“.h”。 |
| -e/--startId | 可缺省 | 带参数 | 指定生成资源的起始ID值，例如：0x01000000,范围[0x01000000, 0x06FFFFFF),[0x08000000, 0x41FFFFFF) |
| -f/--forceWrite | 可缺省 | 不带参数 | 如果输出路径已经存在。强制删除，重新生成。 |
| -h/--help | 可缺省 | 不带参数 | 查看工具帮助信息。 |
| -m/--modules | 可缺省 | 带参数 | 多模块联合编译时，指定多个模块名。用“，”连接。 |
| -x/--append | 可缺省 | 带参数 | 指定生成中间文件的资源目录或单个资源路径。同一个命令可以多次指定。 |
| -z/--combine | 可缺省 | 不带参数 | 针对资源中间文件目录，生成编译结果。 |
| -v/--version | 可缺省 | 不带参数 | 查看工具版本号。 |
| -l/--fileList | 可缺省 | 带参数 | 指定命令选项的集合json文件，例如：resConfig.json。详细格式查看开发文档。 |
| --ids | 可缺省 | 带参数 | 指定生成id_defined.json的输出目录。 |
| --defined-ids | 可缺省 | 带参数 | 指定id_defined.json文件路径，一般都是通过--ids生成。<br>id_defined.json包含资源类型、名称及其ID的列表。<br>开发者可以自定义id_defined.json中的资源ID。 |
| --dependEntry | 可缺省 | 带参数 | FA模型下单独编译feature时，指定entry模块的编译结果目录。 |
| --icon-check | 可缺省 | 不带参数 | 开启icon和startWindowIcon的PNG图片校验功能。 |
| --target-config | 可缺省 | 带参数 | 与“-i”命令同时使用，支持选择编译。<br>具体可参考如下**target-config参数说明**。|
| --thread | 可缺省 | 带参数 | 指定资源编译时开启的子线程数量。|
| --ignored-file | 可缺省 | 带参数 | 指定资源编译时文件和文件夹的忽略规则，格式为正则表达式，多个规则之间以“:”分隔。例如：“\\.git:\\.svn”表示忽略名称为“.git”、“.svn”的文件和文件夹。|

**target-config参数说明**

支持参数配置类型：MccMnc、Locale、Orientation、Device、ColorMode、Density

参数格式说明：配置之间用“;”分割，配置中的值用“[]”封装，并使用“,”分割。

MccMnc匹配规则：Mcc（国家码）必须相同；Mnc（网络码）不存在时默认匹配，否则Mnc须相同才匹配。

Locale匹配规则：Locale匹配需满足以下三条规则。

1、语言须相同。

2、脚本（文字）不存在时默认匹配，否则必须相同。

3、国家或地区不存在时默认匹配，否则必须相同。

参数举例说明：Locale[zh_CN,en_US];Device[phone]，该参数过滤其他语言，保留能匹配上zh_CN和en_US的语言；过滤其他设备，保留phone；其他参数（如MccMnc、Orientation等）配置不过滤均保留。


#### restool支持的子命令

| 命令  | 描述 |
| ---  | ---- |
| dump | 以json的格式输出hap包中resource的内容。 |

**dump命令**

```
restool dump [-h] [config] filePath
```
dump命令参数列表

| 参数 | 是否可缺省 | 是否存在入参 | 描述 |
| --- | --------- | ---------- |------- |
| -h  | 可缺省 | 不带参数 | 帮助信息。 |
| config | 可缺省 | 不带参数 | 只打印hap包中资源的限定词信息。 |

示例:

```sh
# 打印hap包中所有的资源信息
restool dump entry.hap
# 打印hap包中资源的限定词信息
restool dump config entry.hap
```

### 使用实例

例如：entry目录结构如下
```
entry/src/main
|    |----resource
|    |    |----base
|    |    |    |----element
|    |    |    |----media
|    |    |    |----profile
|    |    |----rawfile
|    |----config.json/module.json
```

构建资源全量命令：

```
restool -i entry/src/main  -j entry/src/main/module.json -p com.ohos.demo -o out -r out/ResourceTable.txt -f
```

构建资源增量命令(仅预览模式可用)，具体步骤如下：

1.生成资源中间件:
```
restool -x entry/src/main/resource -o out
```
2.编译中间件:
```
restool -i out1 -i out2 -o out -p com.ohos.demo -r out/ResourceTable.txt -j entry/src/main/module.json -f -z
```

叠加资源编译命令：

```
# hapResource为解压后的Hap包路径
restool -i entry/src/main -i hapResource -j entry/src/main/module.json -p com.ohos.demo -o out -r out/ResourceTable.txt -f
```

固定资源ID的方式有两种，如下：

方式一：在resource/base/element/目录下存放自定义id_defined.json文件。构建成功后，生成的ID值将会和id_defined.json文件中自定义的ID值保持一致。

方式二：通过--ids 命令生成id_defined.json文件。--defined-ids命令指定id_defined.json文件。构建成功后，生成的ID值将会和id_defined.json文件中自定义的ID值保持一致。

生成id_defined.json文件：
```
restool -i entry/src/main  -j entry/src/main/module.json -p com.ohos.demo -o out -r out/ResourceTable.txt --ids out -f
```

指定资源ID固定的id_defined.json文件：
```
restool -i entry/src/main  -j entry/src/main/module.json -p com.ohos.demo -o out1 -r out1/ResourceTable.txt --defined-ids out/id_defined.json -f
```

### 资源类型介绍
[资源类型介绍](https://gitee.com/openharmony/docs/blob/master/zh-cn/application-dev/quick-start/resource-categories-and-access.md)

## 相关仓

**global_resource_tool**

[third_party_cJSON](https://gitee.com/openharmony/third_party_cJSON/blob/master/README.md)

[third_party_libpng](https://gitee.com/openharmony/third_party_libpng/blob/master/README.md)

[third_party_bounds_checking_function](https://gitee.com/openharmony/third_party_bounds_checking_function/blob/master/README.md )
