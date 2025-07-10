# Automatic build performance

## Purpose
build specified projects serveral times and calculate the average value of build time and size of abc

## How to use
### environmoent
Windows, pandas, json5, python3.9
### How to run
input the command in cmd: python performance_entry.py

### Details of settings
You need to modify in the performance_config.py
project_path, node_js_path, jbr_path. These params must be set according your envronment!!! Other params use default.  
If you need to modify others, you can set in application_configs, refer to the 'HelloWorld'  
set run_list to run the projects  
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

More details:  
**path of jbr**
jbr_path = r'xxx\DevEco Studio\jbr'

**path of the project**
project_path = r"xxx"

**app name which showed in pictures**
name = r'FTB'

**AS = 1  DevEco = 2**  
IDE = 2  

**command for building**
cmd_prefix = r"hvigorw.bat"
**Start with space**  
cmd_debug_suffix = ' --mode module -p product=default assembleHap'  
cmd_release_suffix = ' --mode project -p product=default assembleApp'  

**The path of the package you have built**  
debug_package_path = r'entry\build\default\outputs\default\entry-default-unsigned.hap' 
release_package_path = r'entry\build\default\outputs\default\app\entry-default.hap'  

**When you test the incremental building, the code will be add into this file**  
incremental_code_path = 'entry\src\main\ets\pages\Index.ets'  

**add this code when incremental building test**  
incremental_code_str = "let index = 5 + 6\n"  

**the code will be add before the first position of this**  
incremental_code_end_pos = 'this.num = num'  

**when the code need to revert, this can help to find where the code you add**  
incremental_code_start_pos = "let index = 5 + 6\n"  

**file which controls aotCompileMode in type or not**  
json5_path = 'xxx/build-profile.json5'  

**the name of the directory which save the log files**   
log_direct = r"buildTestData"  

**filename of output logs, default name is [IDE_filename]_[debug_or_release]_[build_type_of_log]_[logFileName.xslx], for example: DevEco_Debug_Incremental_sizeAve.xlsx**  
output_split = "_"  
IDE_filename = ["AS", "DevEco"]  
debug_or_release = ["Debug", "Release"]  
build_type_of_log = ["Incremental", "fullBuild"]  
**size means this file record the size of abc in the package, time means the build cost time, All means it records all data, avg means it records the value of the data**  
log_filename = ["sizeAll.xlsx", "sizeAve.xlsx","timeAll.xlsx", "timeAve.xlsx"]  

**show detailed build cost time in html, delete this setting to hide this info**
show_time_detail_filter = ["createProgram", "arkTSLinter", "tsProgramEmit",
                            "generate content and source map information", "write obfuscated source code",
                            "write source map (async)", "generate merged abc by es2abc (async)", "total build cost"
                        ]

**Do not build the project，use the test data if you need to debug the scripts,use '' to close this mode，test_report.json will test in build succeed case，test_error_report.json will test in build failed case**  
developing_test_mode = ''  