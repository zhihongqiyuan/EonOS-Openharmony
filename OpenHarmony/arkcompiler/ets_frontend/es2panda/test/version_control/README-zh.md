# 版本控制测试

生成版本控制测试报告

## 背景

当前API12的es2abc可以将ts/js文件编译成不同API版本的abc字节码，高版本API引入的新特性在生成低版本API的字节码时应予以拦截。具体来说：在ts/js源码中添加高版本引入的特性后，通过es2abc生成低于该版本abc文件并在虚拟机上运行的过程中，能够正常报错或不生效；而生成高于或等于该版本的abc文件时，则能正常通过编译，并在对应版本的虚拟机上正常运行。为确保该功能的正确运行，引入版本控制测试套和从API9到API12的各版本引入特性的测试用例覆盖。

## 使用说明

进入openharmony工程目录下，执行：

```
#编译
./build.sh --product-name rk3568 --build-target es2panda_build --build-target ark_js_host_linux_tools_packages
#运行
cd ./arkcompiler/ets_frontend/es2panda
python3 test/runner.py --version-control ../../../out/rk3568/clang_x64/arkcompiler/ets_frontend/

```

### 可选参数：

* `--version-control`:执行版本控制测试套

## 测试原理

当前测试的版本包括：API9，API10，API11，API12beta1，API12beta2，API12beta3。在每个版本引入的特性，低于该版本则不生效，等于或高于该版本则支持。对不支持的特性，可能在编译阶段报错，也可能在运行阶段报错，也可能不生效。测试过程整体可分为编译阶段，和运行时阶段。编译阶段会对每一用例进行编译，将输出结果与预期结果比较，并获得API9到API12beta3的字节码（如果编译通过）。运行时阶段分别将每一个用例生成的每一个版本字节码在API10到API12beta3的虚拟机（API9的虚拟机暂未上线拦截功能，输出结果不可预期，不纳入测试）上运行，与预期结果比较。

其中，引入的特性可以分为语法特性和字节码特性。语法特性体现在ts/js源码对特性的支持，字节码特性则体现在编译后的字节码指令上。基于这两种特性的区别，分别设计用例进行测试：

|  特性类别  |               compile阶段               | runtime阶段  |
| :--------: | :--------------------------------------: | ------------ |
|  语法特性  |   对比各版本编译的ast树是否与预期一致   | 对比运行结果 |
| 字节码特性 | 对比各版本编译的字节码指令是否与预期一致 | 对比运行结果 |

### 语法特性

以API11版本引入的特性"class 私有字段"为例，其文件层次结构如下：

```
.
└── arkcompiler
    └── ets_frontend
        └── es2panda
            └── test
                ├── runner.py
                └── version_control
                    ├── API11
                    │   ├── bytecode_feature
                    │   └── syntax_feature
                    │       ├── class_private_field.js
                    │       ├── class_private_field_runtime_below_abc_api_version-expected.txt
                    │       ├── class_private_field_supported_compile_version-expected.txt
                    │       ├── class_private_field_supported_runtime_version-expected.txt
                    │       ├── class_private_field_unsupported_compile_version-expected.txt
                    │       └── class_private_field_unsupported_runtime_version-expected.txt
                    ├── API12beta1_and_beta2
                    └── API12beta3

```

预期结果：

| 用例                            |         | 低于API11                                                                                                                                                                                                        | 高于API11                                                                                                                                                                                                        |
| ------------------------------- | ------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| class_private_field.js（API11） | compile | class_private_field_**unsupported_compile**_version-expexted.txt                                                                                                                                           | class_private_field_**supported_compile**_version-expexted.txt                                                                                                                                             |
| class_private_field.js（API11） | runtime | ark_js_vm版本低于字节码版本：<br />class_private_field_runtime_below_abc_api_version-expected.txt<br />ark_js_vm高于等于字节码版本：<br />class_private_field_**unsupported_runtime**_version-expexted.txt | ark_js_vm版本低于字节码版本：<br />class_private_field_runtime_below_abc_api_version-expected.txt<br />ark_js_vm高于等于字节码版本：<br />class_private_field_ **supported_runtime**_version-expexted.txt |

对"class_private_field.js"用例，当生成低于API11版本的字节码时，编译结果应与"class_private_field_ **unsupported_compile_** version-expexted.txt"一致，生成高于或等于API11版本的字节码时，编译结果应与"class_ private_ field_ **supported_ compile**_ version-expexted.txt"一_致。对于成功编译的abc文件，在低于API11版本ark_js_vm上运行时，其运行结果应与"class_private_field_ **unsupported_runtime** _version-expexted.txt"一致，在高于或等于API11版本ark_js_vm上运行时，其运行结果应"class_private_field _ **supported_runtime** _version-expexted.txt"一致。此外，若虚拟机的版本低于字节码版本也会无法运行，结果中会出现"class_private_field_runtime_below_abc_api_bersion-expexted.txt"中的报错信息。

除此之外，还有一些特殊的情况，例如在高于API11的版本引入的"sendable"相关特性，在API11以下和API11以上的虚拟机上运行的报错信息不一致。以API12beta3引入的"sendable function"为例，其文件层次结构如下：

```
arkcompiler
    └── ets_frontend
        └── es2panda
            └── test
                ├── runner.py
                └── version_control
                    ├── API11
                    ├── API12beta1_and_beta2
                    │   ├── bytecode_feature
                    │   └── syntax_feature
                    │       ├── sendable_function.ts
                    │       ├── sendable_function_runtime_below_abc_api_version-expected.txt
                    │       ├── sendable_function_supported_compile_version-expected.txt
                    │       ├── sendable_function_supported_runtime_version-expected.txt
                    │       ├── sendable_function_unsupported_compile_version-expected.txt
                    │       ├── sendable_function_unsupported_runtime_for_below_API11_version-expected.txt
                    │       └── sendable_function_unsupported_runtime_for_below_API12beta3_version-expected.txt
                    └── API12beta3
```

预期结果：

| 用例                               |         | 低于API11                                                                                                                                                                                                                      | 高于API11低于API12beta3                                                                                                                                                                                                             | API12beta3                                                                                                                                                                                                 |
| ---------------------------------- | ------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| sendable_function.ts（API12beta1） | compile | sendable_function_**unsupported_compile**_version-expexted.txt                                                                                                                                                           | sendable_function_**unsupported_compile**_version-expexted.txt                                                                                                                                                                | sendable_function_**supported_compile**_version-expexted.txt                                                                                                                                         |
| sendable_function.ts（API11beta1） | runtime | ark_js_vm版本低于字节码版本:<br />sendable_function_runtime_below_abc_api_version-expexted.txt<br />ark_js_vm版本高于等于字节码版本:<br />sendable_function_**unsupported_runtime_for_below_API11**_version-expected.txt | ark_js_vm版本低于字节码版本:<br />sendable_function_runtime_below_abc_api_version-expexted.txt<br />ark_js_vm版本高于等于字节码版本:<br />sendable_function_**unsupported_runtime_for_below_API12beta3**_version-expected.txt | ark_js_vm低于字节码版本：<br />sendable_function_runtime_below_abc_api_version-expexted.txt<br />ark_js_vm版本高于等于字节码版本：<br />sendable_function_**supported_runtime**_version-expexted.txt |

对"sendable_function.ts"用例，在低于API11版本ark_js_vm上运行时，运行结果应与"sendable_function_ **unsupported_runtime_for_below_API11** _version-expected.txt"一致，在高于或等于API11低于API12beta3版本的虚拟机上运行时，运行结果应与 "sendable_function _**unsupported_runtime_for_below_API12beta3** _version-expected.txt" 一致。其他情况与其他用例一致。

### 字节码特性

字节码特性测试的基本结构与语法特性测试基本一致，主要区别有：

* 字节码特性在编译环节的expected文件保存的是反编译的字节码文件。
* API12beta3生成的字节码文件格式与前面各版本的文件格式不一致，需要单独的expected文件。

基本情况以API11引入的"callinit"字节码指令特性为例，其文件层次结构如下：

```
.
└── arkcompiler
    └── ets_frontend
        └── es2panda
            └── test
                ├── runner.py
                └── version_control
                    ├── API11
                    │   ├── bytecode_feature
                    │   │   ├── class_init_callinit.js
                    │   │   ├── class_init_callinit_runtime_below_abc_api_version-expected.txt
                    │   │   ├── class_init_callinit_supported_compile_for_below_API12beta3_asm_version-expected.txt
                    │   │   ├── class_init_callinit_supported_compile_for_higher_or_equal_to_API12beta3_asm_version-expected.txt
                    │   │   ├── class_init_callinit_supported_runtime_version-expected.txt
                    │   │   ├── class_init_callinit_unsupported_compile_asm_version-expected.txt
                    │   │   └── class_init_callinit_unsupported_runtime_version-expected.txt
                    │   └── syntax_feature
                    ├── API12beta1_and_beta2
                    └── API12beta3
```

预期结果：

| 用例                            |         | 低于API11                                                                                                                                                                                                        | 高于API11低于API12beta3                                                                                                                                                                                          | API12beta3                                                                                                                                                                                                     |
| ------------------------------- | ------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| class_init_callinit.js（API11） | compile | class_init_callinit_**unsupported_compile_asm**_version-expectedd.txt                                                                                                                                      | class_init_callinit_**supported_compile_for_below_API12beta3_asm**_version-expected.txt                                                                                                                    | class_init_callinit_**supported_compile_for_higher_or_equal_to_API12beta3_asm**_version-expected.txt                                                                                                     |
| class_init_callinit.js（API11） | runtime | ark_js_vm版本低于字节码版本:<br />class_init_callinit_runtime_below_abc_api_version-expected.txt<br />ark_js_vm版本高于等于字节码版本:<br />sendable_function_**unsupported_runtime**_version-expected.txt | ark_js_vm版本低于字节码版本:<br />class_init_callinit_runtime_below_abc_api_version-expected.txt<br />ark_js_vm版本高于等于字节码版本:<br />sendable_function_**unsupported_runtime**_version-expected.txt | ark_js_vm版本低于字节码版本:<br />class_init_callinit_runtime_below_abc_api_version-expected.txt<br />ark_js_vm版本高于等于字节码版本:<br />sendable_function_**supported_runtime**_version-expexted.txt |

对于特殊情况，以API12beta3引入的"definepropertybyname"字节码指令为例，该指令替换了在API11引入的"definefieldbyname"。因此，在进行字节码比较时，低于API11有一个预期结果，高于API11低于API12beta3有一个预期结果，API12beta3也有一个预期结果。文件层次结构如下：

```
'.
└── arkcompiler
    └── ets_frontend
        └── es2panda
            └── test
                ├── runner.py
                └── version_control
                    ├── API11
                    ├── API12beta1_and_beta2
                    └── API12beta3
                        ├── bytecode_feature
                        │   ├── define_property_by_name.ts
                        │   ├── define_property_by_name_runtime_below_abc_api_version-expected.txt
                        │   ├── define_property_by_name_supported_compile_for_higher_or_equal_to_API12beta3_asm_version-expected.txt
                        │   ├── define_property_by_name_supported_runtime_version-expected.txt
                        │   ├── define_property_by_name_unsupported_compile_for_below_API11_asm_version-expected.txt
                        │   ├── define_property_by_name_unsupported_compile_for_below_API12beta3_asm_version-expected.txt
                        │   └── define_property_by_name_unsupported_runtime_version-expected.txt
                        └── syntax_feature
```

预期结果：

| 用例                                     |         | 低于API11                                                                                                                                                                                                                    | 高于API11低于API12beta3                                                                                                                                                                                                      | API12beta3                                                                                                                                                                                                               |
| ---------------------------------------- | ------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| define_property_by_name.ts（API12beta3） | compile | define_property_by_name_**unsupported_compile_for_below_API11**_asm_version-expected.txt                                                                                                                               | class_init_callinit_**unsupported_compile_for_below_API12beta3**_asm_version-expected.txt                                                                                                                              | class_init_callinit_**supported_compile_for_higher_or_equal_to_API12beta3_asm**_version-expected.txt                                                                                                               |
| define_property_by_name.ts（API12beta3   | runtime | ark_js_vm版本低于字节码版本:<br />define_property_by_name_runtime_below_abc_api_version-expected.txt<br />ark_js_vm版本高于等于字节码版本:<br />define_property_by_name_ **unsupported_runtime**_ version-expected.txt | ark_js_vm版本低于字节码版本:<br />define_property_by_name_runtime_below_abc_api_version-expected.txt<br />ark_js_vm版本高于等于于字节码版本:<br />define_property_by_name_**unsupported_runtime**_version-expected.txt | ark_js_vm版本低于字节码版本:<br />define_property_by_name_runtime_below_abc_api_version-expected.txt<br />ark_js_vm版本高于等于字节码版本:<br />define_property_by_name_**supported_runtime**_version-expexted.txt |
