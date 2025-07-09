# C++公共基础库<a name="ZH-CN_TOPIC_0000001148676553"></a>


## 简介<a name="section11660541593"></a>

C++公共基础类库为标准系统提供了一些常用的C++开发工具类，包括：

-   文件、路径、字符串相关操作的能力增强接口
-   读写锁、信号量、定时器、线程增强及线程池等接口
-   安全数据容器、数据序列化等接口
-   各子系统的错误码相关定义

## 目录<a name="section17271017133915"></a>

```
commonlibrary/c_utils
├─ base
│   ├── include       # 对各子系统开放的接口头文件
│   ├── src           # 源文件
│   └── test          # 测试代码
├─ Docs
    ├── en            # 英文文档
    └── zh-cn         # 中文文档
```

## 约束

适用于标准系统。
## 编译构建
### 编译部件
```
./build.sh --product-name rk3568 --build-target c_utils
```

### 编译动态库
```
./build.sh --product-name rk3568 --build-target commonlibrary/c_utils/base:utils
```

### 编译Rust动态库
```
./build.sh --product-name rk3568 --build-target commonlibrary/c_utils/base:utils_rust
```

### 编译静态库
```
./build.sh --product-name rk3568 --build-target commonlibrary/c_utils/base:utilsbase
```
### 如何依赖c_utils
1. 进入相关模块对应BUILD.gn文件
2. 在该模块对应位置中的`external_deps`字段内添加依赖，如下：
```gn

ohos_shared_library("xxxxx") {
  ...

  external_deps = [
    ...
    # 动态库依赖(可选)
    "c_utils:utils",
    # 静态库依赖(可选)
    "c_utils:utilsbase",
    # Rust动态库依赖(可选)
    "c_utils:utils_rust",
  ]

  ...
}
```

## 使用说明

### [Rust-匿名共享内存](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c_utils_guide_rust_ashmem.md)
### [Rust-文件与目录](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c_utils_guide_rust_directory.md)
### [Rust-读写文件](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c_utils_guide_rust_file.md)

### [使用匿名共享内存](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-ashmem.md)
### [使用智能指针管理动态分配内存对象](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-refbase.md)
### [使用Parcel作为数据容器](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-parcel.md)
### [定时器](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c_utils_timer.md)
### [文件映射](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c_utils_guide_mapped_file.md)
### [事件处理系统](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c_utils_guide_event.md)
### [读写锁](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-rwlock.md)
### [增强信号量功能](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-semaphore.md)
### [强化线程能力](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-thread.md)
### [线程池](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c_utils_thread_pool.md)


### [线程安全Map](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-safeMap.md)
### [有序Vector](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-sortedVector.md)
### [线程安全阻塞队列](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-safe_block_queue.md)
### [线程安全栈与队列](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-safe_queue.md)

### [单例模式](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-singleton.md)
### [观察者模式](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-observer.md)

### [日期与时间](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-datetime.md)
### [文件与目录](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-directory.md)
### [字符串处理](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-string.md)
### [读写文件](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-file.md)
### [管理、传递文件描述符](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/zh-cn/c-utils-guide-uniquefd.md)

## Changelog
**2023/06/14**
1. 添加文件映射、事件处理系统的开发指导文档。
2. Refbase维测增强部分已更新至文档。
3. 添加Rust相关功能的开发指导文档并提供编译命令。

**2023/01/31**
1. 添加docs目录，提供c_utils内各主要功能的开发指导文档。
2. 在源码头文件中添加注释。
3. 修改Readme文档，开发指导文档可通过Readme中的"使用说明"章节跳转查看。

**2022/10/10**
1. 路径变更。由utils/native移动至commonlibrary/c_utils；
2. 部件名变更。由utils_base变更为c_utils；
3. 不再提供安全C库能力。请使用[third_party_bounds_checking_function](https://gitee.com/openharmony/third_party_bounds_checking_function)。
## 相关仓<a name="section1249817110914"></a>

**[commonlibrary\_c\_utils](https://gitee.com/openharmony/commonlibrary_c_utils)**

[commonlibrary\_utils\_lite](https://gitee.com/openharmony/commonlibrary_utils_lite)

