# 自动构建性能测试脚本

## 脚本目的
自动构建指定的工程，构建多次后取平均值输出构建的时间以及abc的大小

## 脚本使用
### 运行环境
测试自动化脚本运行环境为windows，pandas, json5, python3.9
### 脚本运行
输入命令 python performance_entry.py  

### 详细设置内容
需要在performance_config.py中修改以下内容
project_path,node_js_path,jbr_path 这三个必须根据环境配置，其他的可以用默认的  
如果不使用默认的，则可以像HelloWorld中那样配置  
修改run_list 以指定需要运行的工程  
```
    send_mail = True
    run_list = ["FTB", "FDY", "FWX"]

    def __init__(self):
        # Default config settings for all projects, if it's not what you need, config them in application_configs
        self.cmd_prefix = r"hvigorw.bat"
```

```
    def __init__(self):
        ...

        # Do not build the project，use the test data if you need to debug the scripts
        self.developing_test_mode = False
        # set your node_js path
        self.node_js_path = r"C:/xxx"
        # Must set according environment
        self.jbr_path = r'xxx\DevEco Studio\jbr'
```

```
application_configs = dict(
    [
        (    
            "FTB", dict
                (
                    project_path = r"D:\FTB",
                )
        ),
        (    
            "FDY", dict
                (
                    project_path = r"D:\FDY",
                )
        ),
        (    
            "FWX", dict
                (
                    project_path = r"D:\FWX",
                )
        ),
        (
            "HelloWorld", dict
                (
                    # The following params must be set according you environment
                    project_path = r"xxx",
                    
                    # The following params is not neccessary to be modified
                    debug_package_path = r'entry\build\default\outputs\default\entry-default-unsigned.hap',
                    release_package_path = r'entry\build\default\outputs\default\app\entry-default.hap',
                    incremental_code_path = r'entry\src\main\ets\pages\Index.ets',
                    incremental_code_end_pos = 'build() {',
                    incremental_code_str = "a: number = 5 + 6\n",
                    incremental_code_start_pos = "a: number = 5 + 6\n",
                )
        )
    ]
)
```  

更多细节:  
**jbr路径**
jbr_path = r'xxx\DevEco Studio\jbr'

**目标工程的路径**
project_path = r"D"\xxx"

**图片所示app的名字**
name = r'FTB'

**AS = 1  DevEco = 2**  
IDE = 2  

**构建使用的cmd指令**
cmd_prefix = r"hvigorw.bat"
**开头有空格**  
cmd_debug_suffix = ' --mode module -p product=default assembleHap'  
cmd_release_suffix = ' --mode project -p product=default assembleApp'  

**生成的安装包路径**  
debug_package_path = r'entry\build\default\outputs\default\entry-default-unsigned.hap' 
release_package_path = r'entry\build\default\outputs\default\app\entry-default.hap' 

**增量编译时修改以下文件**  
incremental_code_path = 'entry\src\main\ets\pages\Index.ets'  

**增量编译时添加以下代码**  
incremental_code_str = "let index = 5 + 6\n"  

**代码将会添加到下面这行代码的前面**  
incremental_code_end_pos = 'this.num = num'  

**通过找到下面的代码，来还原代码文件**  
incremental_code_start_pos = "let index = 5 + 6\n"  

**file which controls aotCompileMode in type or not**  
json5_path = 'xxx/build-profile.json5'  

**存放log文件的目录**   
log_direct = r"buildTestData"  

**输出的文件名，默认是[IDE_filename]_[debug_or_release]_[build_type_of_log]_[logFileName.xslx]，比如:DevEco_Debug_Incremental_sizeAve.xlsx**
output_split = "_"  
IDE_filename = ["AS", "DevEco"]  
debug_or_release = ["Debug", "Release"]  
build_type_of_log = ["Incremental", "fullBuild"]  
**size 表示log里记录了hap中abc的大小，time记录了构建耗时，All表示记录全部数据，Avg表示平均值**  
log_filename = ["sizeAll.xlsx", "sizeAve.xlsx","timeAll.xlsx", "timeAve.xlsx"]  

**配置的APP会显示更精细的构建耗时，不配置则不显示**
show_time_detail_filter = ["createProgram", "arkTSLinter", "tsProgramEmit",
                            "generate content and source map information", "write obfuscated source code",
                            "write source map (async)", "generate merged abc by es2abc (async)", "total build cost"
                        ]

**跳过build阶段，使用测试数据，修改脚本时调试使用. ''为不使用测试模式，test_report.json测试构建成功的模式，test_error_report.json测试构建失败报错的场景**  
developing_test_data_path = ''  