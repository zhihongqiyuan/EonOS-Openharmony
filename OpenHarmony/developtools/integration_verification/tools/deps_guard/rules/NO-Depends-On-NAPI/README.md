# NO-Depends-On-NAPI规则说明



## 1. NAPI模块的定义

NAPI模块是指应用import的TS模块对应的接口实现动态库。如下所示：

```javascript
import @ohos.deviceinfo
```

应用import该模块时，本质上是dlopen("/system/lib{64}/module/lib**deviceinfo**.z.so")。这里libdeviceinfo.z.so就是NAPI模块。

所有的NAPI模块都是安装在/system/lib{64}/module目录下。

## 2. 规则解释

NO-Depends-On-NAPI规则如字面意思，不允许任何模块依赖NAPI模块。具体原因如下：

- 后续linker不会到/system/lib{64}/module下搜索动态库，该路径下的so只能被应用import时通过框架dlopen来访问。
- 编译框架上会对所有的NAPI模块进行全局符号优化，使得所有的NAPI模块对外只保留如下version script文件中的符号：

```apl
1.0 {
  global:
    _init;
    _fini;
    NAPI_*_GetABCCode;
    NAPI_*_GetJSCode;

  local:
    *;
};
```

## 3. 违规场景及处理方案建议

### 3.1 非NAPI模块安装到了/system/lib{64}/module目录下

有些非NAPI模块（并没有实现任何@ohos.xxx TS模块的API）BUILD.gn编写错误，增加了以下字段，导致安装到了NAPI模块的目录：

```go
relative_install_dir = "module"
```

**修改方案**：去掉relative_install_dir字段。

### 3.2 其它模块确实使用到了NAPI模块中的符号

此场景需要把NAPI模块中被使用到的符号下沉到对应的Inner API模块中供调用者使用，解除对NAPI模块的依赖。

### 3.2 ut测试代码需使用NAPI模块中的符号

此场景可以为NAPI模块增加静态库目标，ut测试代码deps静态库目标来完成测试。



## 4. 例外说明

由于linker后续不会搜索/system/lib{64}，此规则不允许任何例外存在。

当前的白名单列表只用于归档存量待整改模块，整改完成后需清零。

