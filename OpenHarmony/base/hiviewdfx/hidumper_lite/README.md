# hiviewdfx_hidumper_lite

-   [Introduction](#Introduction)
-   [Directory Structure](#Directory Structure)
-   [Use of hidumper_lite](#Use of hidumper_lite)
-   [System Interface](#System Interface)
-   [Rewrite interface](#Rewrite interface)
-   [Related warehouses](#Related warehouses)

## Introduction

The hidumper_lite provides dump interfaces in the LiteOS_M system to help developers export fault logs, memory, and CPU information.

## Directory Structure

The source of Hiview code directory structure is as follows::

```
/base/hiviewdfx/hidumper_lite.
├── lite
    └── hidumper.c                          #AT命令处理过程
└── mini
    ├── hidumper_adapter.c                  #hidumper功能函数适配层
    ├── hidumper_core.c                     #hidumper核心层
    └── interfaces                          #公共接口文件

```

## Use of hidumper_lite

The hidumper_lite command is used to query data in the liteOS_M

Parameter:
  |  Format  | Meaning  |
  | -------- | -------- |
  | AT+HIDUMPER= | Displays the CPU, memory, and all task usage information |
  | AT+HIDUMPER=-dc | Displays CPU usage information |
  | AT+HIDUMPER=-dm | Displays memory usage information |
  | AT+HIDUMPER=-df | Print the last fault log information |
  | AT+HIDUMPER=-dt | Print all task information |
  | AT+HIDUMPER=-h | Print the help information |
  | AT+HIDUMPER=-ikc | Injecting faults into the kernel |
  | AT+HIDUMPER=-m | Prints all memory data in hexadecimal format |
  | AT+HIDUMPER=-m,memstart,memsize | Prints the data of a specific memory area in hexadecimal format. Two parameters, 
                                      memstart: Memory start address. memsize: memory size |

## System Interface

### at_hidumper

```
int at_hidumper(unsigned int argc, const char **argv)
```

AT instruction processing function injected into the system

- Parameter:
  | Member | Type | Required | Description 
  | -------- | -------- | -------- | -------- |
  | argc | unsigned int | Y | number of Parameter |
  | argv | const char** | Y | content of Parameter |


## Rewrite interface

For details about the example, see the rewriting of the hi3861 chip. The corresponding position is as follows device/soc/hisilicon/hi3861v100/sdk_liteos/components/at/src/hidumper_adapter_impl.c

The following functions are : that need to be rewritten on different chips.

### DumpSysInfo

```
int DumpSysInfo(void);
```

Rewrites the function of printing system information. This function prints the system information of the current device.

### DumpCpuUsage

```
int DumpCpuUsage(void);
```

Rewrites the function for printing CPU usage information. This function prints the CPU usage of the current device.

### DumpMemUsage

```
int DumpMemUsage(void);
```

Rewrites the function for printing memory usage information. This function prints the memory usage of the current device.

### DumpTaskInfo

```
int DumpTaskInfo(void);
```

Rewrite the function of printing all task information of the system. This function prints all task information in the system.

### DumpFaultLog

```
int DumpFaultLog(void);
```

Rewrite the function for printing exception logs. This function prints the last saved log information.

### DumpMemRegion

```
int DumpMemRegion(unsigned long long addr, unsigned long long size)
```

Overwrites the function of printing memory data in a specified area. This function prints the data of the memory area of the total size starting from addr.

- Parameter:
  |Parameter name|Type|Mandatory|Description 
  | -------- | -------- | -------- | -------- |
  | addr | unsigned long long | Y | Start address of the memory to be queried |
  | size | unsigned long long | Y | Internal size to be queried |

### DumpAllMem

```
int DumpAllMem(void); 
```

Overwrites the function of printing all memory data. This function can print all memory data.

### PlatformHiDumperIinit

```
int PlatformHiDumperIinit(void); 
```

Rewrites the platform hidumper initialization function. This function initializes features such as the at command of HiDumper based on different platforms.

## Related warehouses

[Subsystem of DFX](https://gitee.com/openharmony/docs/blob/master/zh-cn/readme/DFX%E5%AD%90%E7%B3%BB%E7%BB%9F.md)

**hiviewdfx\_hidumper\_lite**

[hiviewdfx\_blackbox](https://gitee.com/openharmony/hiviewdfx_blackbox/blob/master/README_zh.md)

[hiviewdfx\_hilog\_lite](https://gitee.com/openharmony/hiviewdfx_hilog_lite/blob/master/README_zh.md)

[hiviewdfx\_hievent\_lite](https://gitee.com/openharmony/hiviewdfx_hievent_lite/blob/master/README_zh.md)

[hiviewdfx\_hiview\_lite](https://gitee.com/openharmony/hiviewdfx_hiview_lite/blob/master/README_zh.md)
