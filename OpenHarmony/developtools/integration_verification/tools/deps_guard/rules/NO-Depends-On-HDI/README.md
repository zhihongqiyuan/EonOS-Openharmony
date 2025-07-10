# NO-Depends-On-HDI规则说明



## 1. HDI模块的定义

HDI模块是指HDI服务对应的动态库模块，一般都在hcs文件中描述，示例如下：

```json
root {
    module="sample";
    ipp_algo_config {
        algo1 {
            name = "example";
            description = "example algorithm";
            path = "libcamera_ipp_algo_example.z.so";
            mode = "IPP_ALGO_MODE_NORMAL";
        }
    }
}
```

如上图所示，这些path里的动态库就是HDI模块，由HDF框架通过dlopen的方式动态加载。

## 2. 规则解释

NO-Depends-On-HDI规则有两个方面的含义：

### 2.1 HDI模块需要在编译模板中标识

如下图所示，每个HDI模块需要在对应的BUILD.gn中通过shlib_type字段来标识其类型：

```go
ohos_shared_library(sample_hdi_module) {
    ...
    shlib_type = "hdi"
    ...
}
```

同样，非HDI模块不要增加shlib_type="hdi"标识。

### 2.2 不允许依赖HDI模块

编译框架上ohos_shared_library模板会对所有的HDI模块（shlib_type为hdi）进行全局符号优化，使得所有的HDI模块默认都不对外暴露符号，减小HDI模块的大小，同时加快HDI模块的加载速度。

```go
# Set version_script for hdi service libraries
if (defined(invoker.shlib_type) && invoker.shlib_type == "hdi") {
  if (!defined(invoker.version_script)) {
    _version_script = rebase_path("//build/templates/cxx/hdi.versionscript")
    inputs += [ _version_script ]
    ldflags += [ "-Wl,--version-script=${_version_script}" ]
  }
}
```

## 3. 违规场景及处理方案建议

### 3.1 HDI模块没有添加shlib_type标识

处理方式：参考2.2章节的描述增加shlib_type = "hdi"标识。

### 3.2 其它模块确实使用到了SA模块中的符号

**方案一**：把HDI模块中被使用到的符号下沉到对应的Inner API模块中供调用者使用，解除对HDI模块的依赖。



**方案二**：对于支持插件扩展的HDI模块，需要提供API给插件调用。此场景可以通过version_script来显式申明HDI模块需要对外提供的符号，示例如下：

```go
ohos_shared_library(partly_exported_symbols) {
    ...
    version_script = "libbeget_proxy.versionscript"
    ...
}
```

示例的version_script文件如下：

```apl
1.0 {
  global:
    AclGetDevUdid;
    AclGetSerial;
    ServiceWatchForStatus;
    SystemWatchParameter;
    WatchParameter;
    Remove*Watcher;
  local:
    *;
};
```

> 提示：如果符号名称有规律，可以使用*作为通配符简化version_script编写的工作量；如Acl\*;。

### 3.3 ut测试代码需使用HDI模块中的符号

此场景可以为HDI模块增加静态库目标，ut测试代码deps静态库目标来完成测试。

## 4. 例外说明

HDI模块默认都不对外暴露符号；如需提供符号，通过version_script来申明，无其它例外选项。

当前的白名单列表只用于归档存量待整改模块，整改完成后需清零。