![logo](http://google.github.io/flatbuffers/fpl_logo_small.png) FlatBuffers
===========

[![Build Status](https://travis-ci.org/google/flatbuffers.svg?branch=master)](https://travis-ci.org/google/flatbuffers)
[![Build status](https://ci.appveyor.com/api/projects/status/yg5idd2fnusv1n10?svg=true)](https://ci.appveyor.com/project/gwvo/flatbuffers)
[![Join the chat at https://gitter.im/google/flatbuffers](https://badges.gitter.im/google/flatbuffers.svg)](https://gitter.im/google/flatbuffers?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Discord Chat](https://img.shields.io/discord/656202785926152206.svg)](https:///discord.gg/6qgKs3R)
[![Twitter Follow](https://img.shields.io/twitter/follow/wvo.svg?style=social)](https://twitter.com/wvo)


**FlatBuffers** 是一个高内存效率的跨平台序列化库。 它允许您直接访问序列化数据而无需先对其进行解析/解包，同时仍然具有出色的向前/向后兼容性。

**转到我们的 [landing page][] 浏览我们的文档**

## 支持的操作系统
* Windows
* MacOS X
* Linux
* Android
* And any others with a recent C++ compiler.

## 支持的编程语言
* C++
* C#
* C
* Dart
* Go
* Java
* JavaScript
* Lobster
* Lua
* PHP
* Python
* Rust
* TypeScript

*and more in progress...*
## FlatBuffers使用步骤
1. 按照[教程](docs/source/Tutorial.md)编写fbs文件，生成C/C++的代码可以参考[C教程](docs/source/CppUsage.md)和[C++教程](docs/source/CUsage.md)。
2. 按照[教程](docs/source/Building.md)编译项目，并得到flactc工具。
3. 使用flatc工具生成接口代码。
4. 在鸿蒙中使用FlatBuffers
## OpenHarmony如何集成FlatBuffers
1. 头文件引入
```c++
#include "flatbuffers/flatbuffers.h"
```
2. BUILD.gn添加头文件的引用
```gn
include_dirs = [
    "//third_party/flatbuffers/include"
    ]
```
3. 在代码中使用生成的接口代码序列化和反序列化数据。
## 贡献
* [FlatBuffers Google Group][] to discuss FlatBuffers with other developers and users.
* [FlatBuffers Issues Tracker][] to submit an issue.
* [stackoverflow.com][] with [`flatbuffers` tag][] for any questions regarding FlatBuffers.

*To contribute to this project,* see [CONTRIBUTING][].

## 许可
*Flatbuffers* is licensed under the Apache License, Version 2.0. See [LICENSE][] for the full license text.

<br>

   [CONTRIBUTING]: http://github.com/google/flatbuffers/blob/master/CONTRIBUTING.md
   [`flatbuffers` tag]: https://stackoverflow.com/questions/tagged/flatbuffers
   [FlatBuffers Google Group]: https://groups.google.com/forum/#!forum/flatbuffers
   [FlatBuffers Issues Tracker]: http://github.com/google/flatbuffers/issues
   [stackoverflow.com]: http://stackoverflow.com/search?q=flatbuffers
   [landing page]: https://google.github.io/flatbuffers
   [LICENSE]: https://github.com/google/flatbuffers/blob/master/LICENSE.txt
