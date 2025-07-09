# 系统能力编解码工具

系统能力(SystemCapability, 本文中使用SysCap缩写)编解码工具应用场景如下：

应用开发时，IDE会根据应用配置的SysCap和API版本生成描述rpcid(Required Product Compatibility ID)的json文件，并调用编解码工具syscap_tool将该json文件编码成rpcid。另一方面，IDE拿到开发者导入pcid(Product Compatibility ID)，使用该工具解码出设备的SysCap集合。该工具仅供IDE使用，对用户不可见。

提供的主要功能：

1. pcid编码：对描述SysCap集合的文件编码生成pcid。  

2. pcid解码：对编码后的pcid文件解码获取SysCap集合。

3. rpcid编码：对描述应用所需的SysCap集合的文件编码生成rpcid。

4. rpcid解码：对编码后的rpcid文件解码获取应用所需的SysCap集合。

5. 编码字符串：将sc后缀形式的pcid/rpcid编码为字符串形式。

6. pcid与rpcid比较：查询pcid是否满足rpcid的要求，并输出不满足的地方。

## 代码目录

```
/developtools
├── syscap_codec                 # syscap codec代码目录
│   ├── include  
│   │   ├──code_config           # syscap_tool_shared对外接口定义
│   │   │  └──syscap_define.h
│   │   ├── context_tool.h     
│   │   ├── create_pcid.h    
│   │   └── syscap_tool.h
│   ├── interfaces/inner_api     # 提供部件之间的接口
│   │   ├── syscap_interface.c
│   │   └── syscap_interface.h 
│   ├── napi                     # napi 接口实现
│   │   ├── BUILD.gn
│   │   ├── napi_query_syscap.cpp
│   │   └── query_syscap.js 
│   ├── src
│   │   ├── common_method.c
│   │   ├── common_method.h
│   │   ├── context_tool.c
│   │   ├── create_pcid.c
│   │   ├── endian_internal.h    # 内部实现的大小端转换接口定义(便于win、mac、linux平台通用)
│   │   ├── endian_internal.c    # 大小端转换实现
│   │   ├── main.c               # syscap_tool命令行工具代码实现 
│   │   └── syscap_tool.c        # syscap_tool编解码接口的实现
│   └── test 
│   │   └── unittest/common      # inner 接口测试代码实现
│   │       ├── BUILD.gn
│   │       ├── include
│   │       │   └── syscap_codec_test.h
│   │       └── syscap_codec_test.cpp    # syscap_tool功能测试代码实现    
|   |—— tools
|   │   ├── requirements.txt
│   │   ├── syscap_check.py      # syscap一致性检查脚本
│   │   ├── syscap_collector.py
|   │   └── syscap_config_merge.py
```

## API

PC端工具，不对外提供API。

## PC端编译说明

syscap_tool PC端可执行文件编译步骤：

1. 编译命令：参考[编译构建](https://gitee.com/openharmony/build/blob/master/README_zh.md)文档，执行其指定的sdk编译命令来编译整个sdk，syscap_tool会被编译打包到里面。
2. 编译：在目标开发机上运行上面调整好的sdk编译命令，正常编译syscap_tool会输出到sdk平台相关目录下。

注意：ubuntu环境下只能编译windows/linux版本工具，mac版需要在macos开发机上编译。

## PC端获取说明

1. 下载sdk获取(建议)  
    通过访问本社区门禁[每日构建](https://ci.openharmony.cn/workbench/cicd/dailybuild/dailylist)网站，下载最新的ohos-sdk压缩包，并从相应平台的toolchains压缩包中提取syscap_tool。  
2. 支持运行环境  
    Windows x86_64/Linux x86_64/Darwin x86_64

## 命令帮助

本工具一般被IDE、应用市场和包管理器集成，手工调试时可参考以下说明。

使用./syscap_tool -h或者./syscap_tool --help查看：

```shell
syscap_tool -R/P -e/d -i filepath [-o outpath]
-h, --help      : how to use
-R, --rpcid     : encode or decode rpcid
-P, --pcid      : encode or decode pcid
-C, --compare   : compare pcid with rpcid string format.
        -s, --string : input string.
-e, --encode    : encode to sc format.
        -s, --string : encode to string format.
-d, --decode    : decode to json format.
        -s, --string : decode string format.
-i filepath, --input filepath   : input file
-o outpath, --input outpath     : output path
-v, --version   : print syscap_tool version information.

syscap_tool v1.1.1
```

### 使用示例

```shell
# 将 rpcid.json 编码为SC格式，文件名rpcid.sc
syscap_tool -Rei rpcid.json -o path/

# 将 rpcid.sc 解码为JSON格式，文件名rpcid.json
syscap_tool -Rdi rpcid.sc -o path/

# 将 pcid.json 编码为SC格式，文件名pcid.sc
syscap_tool -Pei pcid.json -o path/

# 将 pcid.sc 解码为JSON格式，文件名pcid.json
syscap_tool -Pdi pcid.sc -o path/

# 将 rpcid.sc 编码为字符串格式，文件名rpcid.txt
syscap_tool -Resi rpcid.sc -o path/

# 将 pcid.sc 编码为字符串格式，文件名pcid.txt
syscap_tool -Pesi pcid.sc -o path/

# 比较字符串格式的pcid和rpcid，pcid 符合条件返回成功提示，不符合则提示原因。
syscap_tool -C pcid.txt rpcid.txt

# 功能类似 -C选项，区别为 -sC 选项为直接输入字符串。
syscap_tool -sC "pcidstring" "rpcidstring"

# 将字符串格式的 pcid 转为 json 格式，文件名 pcid.json。
syscap_tool -Pdsi pcid.txt -o path/
```

**说明：**  -o 选项指定输出目录，缺省为当前目录。  

## syscap一致性检查工具

### 功能及依赖

本工具主要提供如下功能：

1. 收集所有部件（或指定部件）的syscap字段并与developtools/sysap_codec/include/syscap_define.h中的arraySyscap比较，输出检查结果，若不一致，输出不一致的原因
2. 收集所有部件的syscap字段并与interface/sdk-js/api目录下的*.d.ts中的“@syscap”属性集合比较，输出检查结果，若不一致，输出不一致的原因
3. 收集所有interface/sdk-js/api目录下的*.d.ts中的syscap属性与developtools/syscap_codec/include/syscap_define.h中的arraySyscap比较，若不一致，输出不一致的原因

### 使用方法

本工具使用python语言编写，需使用python解释器进行执行。

requirements：

```txt
prettytable==3.3.0
```

使用python3 syscap_check.py -h或python3 syscap_check.py --help查看用法：

```shell
usage: syscap_check.py [-h] [-p PROJECT_PATH] -t {component_codec,component_sdk,sdk_codec}
                       [-b [BUNDLES [BUNDLES ...]]]

optional arguments:
  -h, --help            show this help message and exit
  -p PROJECT_PATH, --project_path PROJECT_PATH
                        root path of project. default: ./
  -t {component_codec,component_sdk,sdk_codec}, --check_target {component_codec,component_sdk,sdk_codec}
                        the target to be compared
  -b [BUNDLES [BUNDLES ...]], --bundles [BUNDLES [BUNDLES ...]]
                        this option will take effect only when the check_target is component_codec. allow multiple
                        json file. default: all bundle.json file
```

使用示例：

```shell
# 检查所有部件的syscap字段与syscap_define.h中的arraySyscap一致性情况
python3 syscap_check.py -p path_of_openarmony -t component_codec

# 检查指定的部件的bundle.json中的syscap字段与syscap_define.h中的arraySyscap一致性情况，注意，只要当--check_target为component_codec时，--bundles才生效
python3 syscap_check.py -p path_of_openarmony -t component_codec -b path_of_component1/bundle.json path_of_component2/bundle.json

# 检查所有部件的syscap字段与*.d.ts中的“@syscap”属性集合的一致性情况
python3 syscap_check.py -p path_of_openarmony -t component_sdk

# 检查*.d.ts中的“@syscap"属性集合与syscap_define.h中的arraSyscap的一致性情况
python3 syscap_check.py -p path_of_openarmony -t sdk_codec
```

## syscap_define.h 文件规则

- 每一个 SyscapNum 枚举值都唯一对应于一个 syscap 字符串。
- 如果弃用某个 syscap，在对应的枚举值后面注释 "// abandoned" 即可，请勿删除和修改。对应的 syscap 字符串也不需要修改。
- 添加 syscap，请在 SyscapNum 和 g_arraySyscap 末尾添加相应数据。
- 数组 g_arraySyscap 按照 SyscapNum 枚举值从小到大排序。

## Release Note

v1.1.0 2022-6-17 添加转字符串格式以及比较功能。  
v1.0.0 2022-3-8 首版本，提供Windows/Linux/Mac平台的系统能力编解码。
