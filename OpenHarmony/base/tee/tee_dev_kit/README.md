
# tee_tee_dev_kit #

## Introduction ##

The TEE SDK development kit supports independent development of TA, including TA dependent header files, compilation frameworks, signature scripts, etc.

| Abbreviations | English | Chinese|
| ------ | ----------------------------- | ------------ |
| TEE | Trusted Execution Environment | Trusted Execution Environment|
| TA | Trusted Application | Trusted Application|
| SDK | Software Development Kit | Software Development Kit|

## Table of Contents<a name="section161941989596"></a>

The directory structure of TEE SDK is as follows:

```
├── sdk/build
│   ├── ld
│   ├── mk
│   ├── signkey
│   └── tools
├── sysroot
│   └── usr
|       └── include
├── sdk/src
│   └── TA
|      └── sample
├── thirdparty
│   └── open_source
│       └── import_open_source_header.sh
└── CHANGELOG
```

## Instructions for use<a name="section161941989597"></a>

Developers need to do some preparation work before using the TEE SDK development kit to develop TA.

### Configure the compilation toolchain

The compilation toolchain used by TEE is LLVM, which is consistent with OpenHarmony. Developers need to download the OpenHarmony compilation toolchain first.

First choice to download OpenHarmony [build code repository]（ https://gitee.com/openharmony/build )

```
git clone  git@gitee.com : openharmony/build.git
```

Then execute the download script in the repository

```
./build/prebuilts_download.sh
```

After downloading, it is necessary to declare the path of the LLVM compilation toolchain in the current compilation environment. The compilation toolchain path can be declared using the following command:

```
export PATH=openharmony/prebuilts/clang/ohos/linux-x86_64/15.0.4/llvm/bin:$PATH
```

This command is only an example, developers need to specify the correct compilation toolchain path.

### Import third-party header files

TEE integrates the Musl library and security function library, which TA can use. The TEE SDK does not include header files for the musl library and security function library by default, but provides import scripts. Developers need to download the [musl library] first（ https://gitee.com/openharmony/third_party_musl ）And [Security Function Library]（ https://gitee.com/openharmony/third_party_bounds_checking_function ）Source code repository:

```
git clone  git@gitee.com : openharmony/third_party_musl.git
git clone  git@gitee.com : openharmony/third_party_bounds_checking_function.git
```

Then execute

```
./tee_dev_kit/sdk/thirdparty/open_source/import_open_source_header.sh
```

Import the musl header file and security function library header file from the source code repository into the TEE SDK.

### Replace TA signature and verification key

The TEE SDK has a preset private key for signing TA files, which can only be used for debugging. In the commercial version, developers need to replace this private key themselves. The private key path: tee_dev_kit/sdk/build/signkey/ta_sign_priv_key.pem。 At the same time, it provides the scripts tee_dev-kit/sdk/build/signkey/ta-sign_algonfig.ini, which can be used to configure the signature algorithm. The default signature algorithm is RSA, with a key length of 4096 bits.

If the developer replaces the signature private key in the TEE SDK, they need to correspondingly replace the signature verification public key in the OpenTrustee operating system. The path of the signature verification public key is:/ base/tee/tee_os_framework/lib/syslib/libelf_verify_key/src/common/ta_verify_key.c。

## Tool Dependency<a name="section11914418405"></a>

The TEE SDK uses Python scripts to parse TA's property configuration files, sign TA files, and perform other operations, so Python tools need to be installed in the development environment.

1. Install Python 3 and above versions

2. Install Python related libraries, such as:

```
pip install pycryptodome

pip install defusedxml
```

If there is a prompt during the compilation process that other Python libraries are missing, they need to be installed together.

## Use constraint<a name="section1371113476307"></a>

-Supported development language: C language
-SDK runtime environment: Linux operating system
-No code editor provided

## TA development steps

When developing a new TA, it is necessary to create a new TA source code directory in the tee_dev-kit/sdk/src/TA directory. The directory structure can refer to the demo example code in this directory. Taking helloworld_demo as an example, the directory structure is as follows:

```
├── helloworld_demo
▽-- ta_demo. c # TA source code file
∝ - configs.xml # TA attribute configuration file
∝ - Makefile # TA Compile Makefile
∝ - build_ta. sh # TA One Click Script Generation
```

### TA code writing

The TA code must implement the following entry functions specified in the GP TEE standard:

|TA Entry Function Name | Function Description|
| -------------------------- | ----------------------------------------------------- |
|TA_Create Entry Point | Constructor of TA instance, called only once in the lifecycle of each TA instance|
|TA_SpenSessionExitPoint | Client requests to create a session with TA|
|TA_invokeCommandEntry Point | After successfully creating a session, the client sends instructions to the TA|
|TA_CloseSessionExitPoint | Client requests to close session with TA|
|TA_SestroyInterpoint | The destructor of the TA instance, which OpenTrustee calls when destroying the TA instance|

### TA Makefile Writing

TA needs to write their own Makefile file, please refer to the example code in the SDK. There are the following key points:

-The target file name generated by TA compilation is fixed as libcombine.so.
-For 64 bit TAs, it is necessary to add the "TARGET_S_SARM64=y" tag in the Makefile header; For 32-bit TA, this tag should not be included in the Makefile.

### TA attribute configuration

Each TA source code directory needs to contain configs.xml, which defines the attribute information of that TA.

|Attribute Name | Data Type | Attribute Description | System Default Value|
| ------------------- | -------- | ------------------------------------------------------------ | ---------- |
|Service_name | String | TA name, string length not exceeding 64 characters, only supports numbers and letters, '_' and '-' | None|
|UUID | UUID | TA unique identifier | None|
|Instance_ceep_alive | Bool | If true, it means that even if all TA sessions are closed, the TA instance will not be destroyed and the global data will still exist until the TEE runs. If false, it means that if all TA sessions are closed, the TA instance will be destroyed| false      |
|Stack_Size | Integer | The stack space size for each session of TA needs to be evaluated based on their actual situation | 8192|
|Heapsosize | Integer | The heap space occupied by the TA instance needs to be evaluated based on its actual situation | 0|
|Does multi-session | Bool | TA support establishing multiple sessions simultaneously | false|
|Single_instance | Bool | Whether multiple sessions of TA belong to the same instance (currently only singleInstance is supported as true) | true|

The example is as follows:

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

### TA compilation and signature

The OpenTrustee SDK provides a one click script for TA generation, which copies tee_dev_kit/sdk/build/build_ta.sh to the TA source code directory for execution. This completes TA compilation, property configuration file parsing, signing, and other operations, and generates a TA installation package file named uuid.sec in the current directory.

## Related Dependency Warehouse<a name="section1371113476308"></a>

[OpenHarmony/build]( https://gitee.com/openharmony/build )

[OpenHarmony/third_party/musl]( https://gitee.com/openharmony/third_party_musl )

[OpenHarmony/third_party/bounds_checking_function]( https://gitee.com/openharmony/third_party_bounds_checking_function )
