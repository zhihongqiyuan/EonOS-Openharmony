# NAPI框架生成工具测试说明

## 概述
NAPI工具的测试主要分为功能测试、单元测试两种，其目的是为验证代码与设计相符合，跟踪需求和设计的实现，发现设计和需求中存在的错误。

## 目录

	├── napi_generator                       # NAPI框架代码生成工具
	│   ├── ...                              # 其它文件夹
	│   ├── test                             # 测试用例
	│   |   ├── unittest                     # UT测试用例
	│   |   ├── storytest                    # ST测试用例
	│   |   └── debug_entry.js               # 测试入口文件

### 功能测试

为了将一个大的特性划分成小颗粒度的功能块，根据功能点确定输入和输出以便获得快速反馈。

#### 用例输出

此处以string数据类型为例：...\storytest\test_string目录下ts文件中定义当前工具支持数据类型的函数，在同级目录下的js文件中调用此方法，并断言返回值与期望值是否一致。执行测试用例时若编译成功，且js文件中所有用例执行通过，说明当前代码无误；否则说明当前代码与设计存在差异。

具体的功能测试操作，可以左键单击以下链接了解：

[功能测试](https://gitee.com/openharmony/napi_generator/blob/master/test/storytest/README_ZH.md)

### 单元测试

使用单元测试检验被测代码的一个很小的、明确的功能是否正确。

#### 用例输出

解析napi_generator-master\src\gen下js文件中的方法，将获取到的内容与期望比较是否一致，若一致则代码无误。

具体的单元测试操作，可以左键单击以下链接了解：

[单元测试](https://gitee.com/openharmony/napi_generator/blob/master/test/unittest/README_ZH.md)