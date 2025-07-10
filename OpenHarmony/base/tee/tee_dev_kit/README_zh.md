
# tee_tee_dev_kit仓库说明 <a name="ZH-CN_TOPIC_0000001078026808"></a>

## 简介<a name="section11660541593"></a>

tee_tee_dev_kit仓库主要包含TEE SDK开发套件，支持独立开发TA，包含了TA依赖的头文件、编译框架、签名脚本等。

| 缩略语 | 英文                          | 中文         |
| ------ | ----------------------------- | ------------ |
| TEE    | Trusted Execution Environment | 可信执行环境 |
| TA     | Trusted Application           | 可信应用     |
| SDK    | Software Development Kit      | 软件开发包   |

## 目录<a name="section161941989596"></a>

TEE SDK目录结构如下：

```
├── sdk/build
│   ├── ld                                # 生成TA ELF文件的链接脚本
│   ├── mk                                # TA make编译框架
│   ├── signkey                           # TA签名用的私钥
│   └── tools                             # 生成TA安装包并对TA签名的脚本
├── sysroot
│   └── usr
|       └── include                       # 给TA提供的TEE头文件
├── sdk/src
│   └── TA                                # 放置TA源码
|      └── sample                         # TA示例代码
├── thirdparty
│   └── open_source
│       └── import_open_source_header.sh  # 导入TA编译依赖的musl头文件和安全函数库头文件
└── CHANGELOG                             # SDK包版本发布记录
```

## 使用说明<a name="section161941989597"></a>

开发者在使用TEE SDK开发套件开发TA之前，需要进行一些准备工作。

###  配置编译工具链

TEE使用的编译工具链为llvm，与OpenHarmony一致，开发者需要先下载OpenHarmony编译工具链。

首选下载OpenHarmony [build代码仓](https://gitee.com/openharmony/build)

```
git clone git@gitee.com:openharmony/build.git
```

然后执行该仓中的下载脚本

```
./build/prebuilts_download.sh
```

下载完成后，需要在当前编译环境中声明llvm编译工具链的路径。可通过如下命令声明编译工具链路径：

```
export PATH=openharmony/prebuilts/clang/ohos/linux-x86_64/15.0.4/llvm/bin:$PATH
```

该命令仅是示例，开发者需要指定正确的编译工具链路径。

###  导入第三方头文件

TEE集成了musl库和安全函数库，TA可以使用这些库。TEE SDK并没有默认包含musl库和安全函数库的头文件，但是提供了导入的脚本。 开发者需要先下载[musl库](https://gitee.com/openharmony/third_party_musl)和[安全函数库](https://gitee.com/openharmony/third_party_bounds_checking_function)源码仓：

```
git clone git@gitee.com:openharmony/third_party_musl.git
git clone git@gitee.com:openharmony/third_party_bounds_checking_function.git
```

然后执行

```
./tee_dev_kit/sdk/thirdparty/open_source/import_open_source_header.sh
```

将musl头文件和安全函数库头文件从源码仓导入到TEE SDK中。

### 替换TA签名和验签密钥

TEE SDK中预置了对TA文件进行签名的私钥，该预置私钥只能用来调试，在商用版本中，开发者需要自行替换该私钥。该私钥路径：tee_dev_kit/sdk/build/signkey/ta_sign_priv_key.pem。同时提供了tee_dev_kit/sdk/build/signkey/ta_sign_algo_config.ini脚本，可以用来对签名算法进行配置。默认的签名算法是RSA，密钥长度4096bit。

如果开发者替换了TEE SDK中的签名私钥，需要对应替换OpenTrustee操作系统中的验签公钥，验签公钥的路径：base/tee/tee_os_framework/lib/syslib/libelf_verify_key/src/common/ta_verify_key.c。

## 工具依赖<a name="section11914418405"></a>

TEE SDK中用到了python脚本来完成TA的属性配置文件解析、对TA文件进行签名等操作，因此需要在开发环境上安装python工具。

1、安装python3及以上版本

2、安装python相关的库，如：

```
pip install pycryptodome

pip install defusedxml
```

如果在编译过程中提示缺少其他python库，需要一并安装。

## 使用约束<a name="section1371113476307"></a>

- 支持开发语言：C语言
- SDK运行环境：linux操作系统
- 未提供代码编辑器

## TA开发步骤

开发一个新的TA时，需要在tee_dev_kit/sdk/src/TA目录下创建新的TA源码目录，目录结构可以参考该目录下demo示例代码。以helloworld_demo为例，目录结构如下：

```
├── helloworld_demo
    ├── ta_demo.c                  # TA源码文件
    ├── configs.xml                # TA属性配置文件
    ├── Makefile                   # TA编译Makefile
    ├── build_ta.sh                # TA一键生成脚本
```

### TA代码编写

TA代码必须实现如下GP TEE标准规定的入口函数，详细说明可以参考《[TEE Client API Specification v1.0 (GPD_SPE_007)](https://globalplatform.org/specs-library/?filter-committee=tee)》

| TA入口函数名称             | 函数描述                                              |
| -------------------------- | ----------------------------------------------------- |
| TA_CreateEntryPoint        | TA实例的构造函数，每个TA实例的生命周期中只被调用一次  |
| TA_OpenSessionEntryPoint   | 客户端请求创建一个与TA的会话                          |
| TA_InvokeCommandEntryPoint | 客户端在创建会话成功后向TA发送指令                    |
| TA_CloseSessionEntryPoint  | 客户端请求关闭与TA的会话                              |
| TA_DestroyEntryPoint       | TA示例的析构函数，OpenTrustee在销毁TA实例时调用此函数 |

### TA Makefile编写

TA需要自行编写Makefile文件，可参考SDK中示例代码。有如下要点：

- TA编译生成的目标文件名固定为libcombine.so。
- 对于64位的TA，需要在Makefile头部增加“TARGET_IS_ARM64 = y”标记；对于32位TA，Makefile中不应包含此标记。

### TA属性配置

每个TA源码目录下需要包含configs.xml，定义该TA的属性信息。

| 属性名              | 数据类型 | 属性描述                                                     | 系统默认值 |
| ------------------- | -------- | ------------------------------------------------------------ | ---------- |
| service_name        | String   | TA名称，字符串长度不超过64字符，仅支持数字、字母，'_'和'-'   | 无         |
| uuid                | UUID     | TA唯一标识                                                   | 无         |
| instance_keep_alive | Bool     | 如果为true，表示即使TA所有会话被关闭，TA实例也不会被销毁，全局数据仍然存在，直到TEE运行结束。如果为false，表示若TA所有会话关闭，TA实例会被销毁。 | false      |
| stack_size          | Integer  | TA每个会话的栈空间大小，需要根据TA实际情况评估               | 8192       |
| heap_size           | Integer  | TA实例占用的堆空间大小，需要根据TA实际情况评估               | 0          |
| multi_session       | Bool     | TA是否支持同时建立多个会话                                   | false      |
| single_instance     | Bool     | TA的多个会话是否归属同一个实例(当前只支持singleInstance为true) | true       |

示例如下：

```
<ConfigInfo>
  <TA_Basic_Info>
    <service_name>demo-ta</service_name>
    <uuid>e3d37f4a-f24c-48d0-8884-3bdd6c44e988</uuid>
  </TA_Basic_Info>
  <TA_Manifest_Info>
    <instance_keep_alive>false</instance_keep_alive>
    <stack_size>8192</stack_size>
    <heap_size>81920</heap_size>
    <multi_session>false</multi_session>
    <single_instance>true</single_instance>
  </TA_Manifest_Info>
</ConfigInfo>
```

### TA编译和签名

OpenTrustee SDK中提供了TA一键生成脚本，将tee_dev_kit/sdk/build/build_ta.sh拷贝到TA源码目录执行，即完成TA编译、属性配置文件解析、签名等操作，在当前目录生成uuid.sec命名的TA安装包文件。


## 相关依赖仓<a name="section1371113476308"></a>

 [OpenHarmony/build](https://gitee.com/openharmony/build)

[OpenHarmony/third_party/musl](https://gitee.com/openharmony/third_party_musl)

[OpenHarmony/third_party/bounds_checking_function](https://gitee.com/openharmony/third_party_bounds_checking_function)
