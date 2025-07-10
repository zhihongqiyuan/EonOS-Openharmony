# components_deps_analyzer.py

## 功能介绍

基于vendor下的config.json、xml文件中的开源部件集、BUILD.gn文件，分析是否存在对闭源部件的依赖以及是否存在对非必选部件的无条件依赖。

结果以json格式进行存储。

## 支持产品

config.json文件主要是关于rk3568系列,已测试产品包括rk3568、rk3568_mini_system、pc_mini_system、tablet_mini_system、phone_mini_system的config.json文件

## 使用说明

前置条件：

1. 获取BUILD.gn文件
1. 获取包含开源部件集的xml文件
1. 获取包含部件集定义的config.json文件
1. 获取BUILD.gn文件对应的name

命令介绍：

1. `-h`或`--help`命令查看帮助
   ```shell
   > python components_deps_analyzer.py --help
   usage: components_deps_analyzer.py [-h] -p COMPONENTS_GN_PATH_LIST -g GN_NAME -c CONFIG_PATH -o OPEN_COMPONENT_XML_PATH [-r RESULT_JSON_NAME]
   
   analyze components deps.
   
   optional arguments:
     -h, --help            show this help message and exit
     -p COMPONENTS_GN_PATH_LIST, --components_gn_path_list COMPONENTS_GN_PATH_LIST
                           path of pr BUILD.gn
     -g GN_NAME, --gn_name GN_NAME
                           gn file corresponding name
     -c CONFIG_PATH, --config_path CONFIG_PATH
                           path of config_file
     -o OPEN_COMPONENT_XML_PATH, --open_component_xml_path OPEN_COMPONENT_XML_PATH
                           open component name set
     -r RESULT_JSON_NAME, --result_json_name RESULT_JSON_NAME
                           name of output_json


   ```
1. 使用示例
   ```shell
   python components_deps_analyzer.py -p BUILD.gn,pkgs/BUILD.gn -g third_party_curl,third_party_zlib -c config_path -o .\gn_xml\ohos.xml
   ```

## 输出格式介绍(result.json)

```
[
    {
        "file_path": BUILD.gn文件路径,
        "error": [
            {
                "line": 行号,
                "code": 行号对应代码,
                "rule": 触发规则,
                "detail": 详细说明
            },
            ...
        ]
    },
    ...
]
```