# ylong_json

## 简介
`ylong_json` 是一个通用的 `JSON` 语法解析库，提供了 `JSON` 文本和特定数据结构之间的相互转换功能。

### ylong_json 在 OpenHarmony 中的位置
![structure](./figures/ylong_json_oh_relate.png)
以下是对于上图关键字段的描述信息：
- `Application Layer`：应用层，给用户提供具体功能。
- `App`：各种应用，需要使用系统服务层的功能。
- `System Service Layer`：系统服务层，给上层应用提供系统服务。
- `system services`：各种系统服务，需要使用 `JSON` 相关的功能。 
- `ylong_json`：系统组件，给系统服务层的相关组件提供通用的 `JSON` 序列化与反序列化能力。
- `serde`：第三方库，用于高效、通用地序列化和反序列化 `Rust` 数据结构。

### ylong_json 内部架构图
![structure](./figures/ylong_json_inner_structure.png)
`ylong_json` 主要分为三个子模块：`JsonValue` 子模块、`serde` 子模块、C-ffi 子模块。

`JsonValue` 子模块提供了一种基础数据结构 `JsonValue`。
`JsonValue` 支持以缩进型格式或紧凑型格式将自身序列化成 `JSON` 文本。任意语法正确的 `JSON` 文本也能被反序列化成一个对应的 `JsonValue` 数据结构。
`JsonValue` 支持增删改查，可以使用指定接口变更 `JsonValue` 中的数据内容。
`JsonValue` 支持 `JSON` 语法中全部的数据类型：`null`, `boolean`, `number`, `string`, `array`, `object`，且按照 `ECMA-404` 实现其全部功能。
针对于 `array` 和 `object` 语法结构，`JsonValue` 提供了多种底层数据结构以针对不同使用场景，例如对于 `array` 结构，支持底层使用 `Vec` 或 `LinkedList`，对于 `object`，支持其底层使用 `Vec`, `LinkedList` 或 `Btree`。
在不同的底层数据结构之上，`array` 和 `object` 会体现出不同的创建和查询性能，例如基于 `Btree` 数据结构的 `object` 在查询方面具有较高性能表现，`LinkedList` 或 `Vec` 在创建方面具有较高性能表现。

`serde` 子模块提供了基于 `serde` 第三方库提供的 `Serialize` 和 `Deserialize` trait 的过程宏功能，可以支持用户结构体和 `JSON` 文本的快速转换。
`serde` 相较于 `JsonValue` 的优势是使用便捷，用户无需将 `JSON` 文本先转换为 `JsonValue` 再从其中取出指定数据生成 `Rust` 结构体，只需给结构体设定 `Serialize` 和 `Deserialize` 过程宏标签，即可使用 `ylong_json` 中提供的接口结构体序列化成 `JSON` 文本，或将对应的 `JSON` 文本转换为用户结构体。

C-ffi 模块提供了基于 `JsonValue` 模块的 C 接口层，方便用户使用 C 接口调用 `ylong_json` 库的功能。

## 目录
```
ylong_json
├─ benches                                # benche 测试文件
├─ docs                                   # 说明文档
├─ examples                               # ylong_json 代码示例
├─ figures                                # ylong_json 架构图
├─ src
│  ├─ value                               # Array, Object 类型定义和相关方法实现
│  ├─ adapter.rs                          # 适配 C 的接口实现
│  ├─ consts.rs                           # 一些常数与表格的定义
│  ├─ deserializer.rs                     # 适配 serde 的反序列化实现
│  ├─ encoder.rs                          # 为 JsonValue 类型序列化实现
│  ├─ error.rs                            # 错误类型定义，便于定位
│  ├─ link_list.rs                        # LinkedList 类型定义和相关方法实现
│  ├─ serializer_compact.rs               # 适配 serde 的序列化实现
│  ├─ states.rs                           # 为 JsonValue 类型反序列化实现
│  └─ value.rs                            # JsonValue 类型定义和相关方法实现
└─ tests                                  # 测试目录
```

## 编译构建

### 使用 Cargo 编译 
1. 在 `Cargo.toml` 的依赖中添加 `ylong_json`
```toml
[dependencies]
ylong_json = { git = "https://gitee.com/openharmony-sig/commonlibrary_rust_ylong_json.git" }
```

### 使用 GN 编译
1. 在 `bundle.json` 中添加 `ylong_json`
```gn 
“deps”： {
    “components”: ["ylong_json"]
}
```

2. 在 `BUILD.gn` 中添加 `ylong_json:lib`
```gn 
external_deps = ["ylong_json:lib"]
```

## 用户指南
详情内容请见[用户指南](./docs/user_guide_zh.md)

## 性能测试

以下测试来源于 [`nativejson-benchmark`](https://www.github.com/miloyip/nativejson-benchmark)。

测试环境信息如下：
```
操作系统：Linux
架构：x86_64
字节序：小端
CPU 型号：Intel(R) Xeon(R) Gold 6278C CPU @ 2.60GHz
CPU 核心数：8
内存：8G
```

测试的软件版本： 

cJSON 1.7.16

测试结果：
```
======= ylong-json ==== parse | stringify ====
canada.json            200 MB/s  90 MB/s 
citm_catalog.json      450 MB/s  300 MB/s 
twitter.json           340 MB/s  520 MB/s

======== cJSON ======== parse | stringify ====
canada.json            55 MB/s    11 MB/s 
citm_catalog.json      260 MB/s   170 MB/s 
twitter.json           210 MB/s   210 MB/s
```

测试结果描述：

在 `nativejson-benchmark` 测试中提供了三种测试文件，其中 `canada.json` 包含了大量的 `number` 结构，`citm_catalog.json` 的各种数据类型较为平均，`twitter.json` 中存在各种 `UTF-8` 字符。
为了保证测试公平性，`ylong_json` 开启了 `list_object` 和 `list_array` 以及 `ascii_only` feature。
`list_object` 和 `list_array` feature 主要是为了保证和 `cJSON` 数据结构层面一致，都使用链表实现。
`ascii_only` feature 是为了保证针对 `UTF-8` 字符的处理逻辑一致，`cJSON` 对于 UTF-8 字符不做处理。

测试流程如下：
 - 读取文件内容到内存，得到文件内容 `content`。
 - 调用指定 `JSON` 库反序列化接口生成对应的 `JSON` 结构体 `data`。
 - 调用 `JSON` 结构体的序列化接口生成输出内容 `result`。
 - 利用 `content`，循环反序列化生成 100 次 `JSON` 结构体，取较小的处理时间 `t1`。
 - 利用 `data`，序列化生成 100 次 `JSON` 文本，取较小的处理时间 `t2`。
 - 计算解析速度，反序列化时间为 `content` 的长度除以 `t1`，序列化时间为 `JSON` 文本长度除以 `t2`。