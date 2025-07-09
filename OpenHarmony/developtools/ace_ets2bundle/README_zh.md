# developtools_ace_ets2bundle

#### 介绍

提供声明式范式的语法编译转换，语法验证，丰富友好的语法报错提示能力。

#### 软件架构

* compiler/src: 编译转换工具源码
* test: 单元测试用例
* .eslintrc: eslint配置规则
* babel.config.js: babel配置信息
* main.js: 编译转换工具源码
* package.json: 安装依赖软件版本信息
* tsconfig.json: 编译工程配置信息
* webpack.config.js: 打包工具脚本配置信息

#### 环境准备

```
> npm -v
  6.14.8
> node -v
  v12.18.3
```
请安装最新稳定的node版本。

#### 安装

进入到compiler目录
```
npm config set registry http://registry.npm.taobao.org
npm config set strict-ssl false
npm cache clean -f
npm install
```

#### 快速开始

进入到compiler目录
```
npm run build
npm run compile
```
上述命令编译了compiler目录下的`sample`工程，编译结果在`sample\build`目录。

#### 创建一个新工程

进入到compiler目录
```
npm run create [projectName]
```

**Note**: 如果`projectName`未指定，`HelloAce`为默认的工程名。
例如创建一个`foo`工程：
```
$ npm run create foo
```
工程`foo`的目录结构:
- foo
  - pages
    - index.ets
  - app.ets
  - manifest.json

#### 创建一个新的页面

例如在`pages`目录创建一个`bar.ets`页面,需要在manifest.json文件的`pages`新增一行，如下所示：
```
  "pages": [
    "pages/index",
    "pages/bar"
  ]
```

#### 编译工程

进入到compiler目录
```
$ npm run build
$ npm run compile [projectName]
```
**Note**: 如果`projectName`未指定，默认编译`sample`工程，编译结果在`sample\build`目录。
