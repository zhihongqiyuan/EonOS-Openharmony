# developtools_ace_js2bundle

#### 介绍

提供类Web范式的语法编译转换、语法验证，丰富友好的语法报错提示。

#### 软件架构

* ace-loader/src: 编译构建框架源码
* sample: 工程样例
* test: 单元测试用例
* third_party/parse5: hml标签语言解析器
* third_party/weex-loader: css样式和javascript语言解析器
* .eslintrc.js: eslint配置规则
* babel.config.js: babel配置信息
* main.product.js: 编译构建框架源码
* package.json: 安装依赖软件版本信息
* webpack.rich.config.js: 富设备编译构建框架脚本配置信息
* webpack.lite.config.js: 瘦设备编译构建框架脚本配置信息

#### 环境准备

```
> npm -v
  6.14.8
> node -v
  v12.18.3
```
请安装最新稳定的node版本。

#### 安装

进入到ace-loader目录
```
npm config set registry http://registry.npm.taobao.org
npm config set strict-ssl false
npm cache clean -f
npm install
```

#### 快速开始

以编译`sample\rich`工程为例，进入到ace-loader目录
```
npm run build
npm run rich
```
上述命令编译了ace-loader目录下的`sample\rich`工程，编译结果在`sample\rich\build`目录。

#### 创建一个新工程

##### 创建富设备工程

假如创建富设备工程，进入到ace-loader目录，在ace-loader下创建一个`helloRich`的文件夹，文件夹内包含必要的`manifest.json`页面配置文件和应用创建销毁的生命周期函数`app.js`文件，并且包含至少一个页面，页面由hml、css、js文件组成。创建瘦设备工程`helloLite`同理。

示例代码如下：

index.hml
```hml
<div class="container">
  <div class="text-div">
    <text class="title">This is the index page.</text>
  </div>
  <div class="button-div">
    <input type="button" value="Go to the second page" class="button" onclick="launch"/>
  </div>
</div>
```
index.css
```css
.container {
  justify-content: center;
  align-items: center;
}

.title {
  font-size: 50px;
}

.button-div {
  padding-top: 50px;
  align-items: center;
  justify-content: center;
}

.text-div {
  align-items: center;
  justify-content: center;
}

.button {
  font-size: 30px;
}
```
index.js
```js
export default {
  data: {
  },
  launch: function() {
    console.info('index page launch');
  }
}
```
app.js
```js
export default {
  onCreate() {
    console.info('Application onCreate');
  },
  onDestroy() {
    console.info('Application onDestroy');
  }
}
```
manifest.json
```json
{
  "appID": "com.example.helloworld",
  "appName": "HelloWorld",
  "versionName": "1.0.0",
  "versionCode": 1,
  "minPlatformVersion": "1.0.1",
  "pages": [
    "pages/index/index"
  ],
  "window": {
    "designWidth": 750,
    "autoDesignWidth": false
  }
}
```

工程`helloRich`的目录结构:

- helloRich
  - pages
    - index
      - index.hml
      - index.css
      - index.js
  - app.js
  - manifest.json

需要编译创建的`helloRich`工程时，在`package.json`的scripts里添加一行脚本
```
"helloRich": "cd helloRich && webpack --config ../webpack.rich.config.js"
```
**Note**: 如果编译创建的`helloLite`工程，需要使用瘦设备的打包框架脚本配置，即：
```
"helloLite": "cd helloLite && webpack --config ../webpack.lite.config.js"
```
然后在ace-loader目录执行
```
npm run build
npm run helloRich
```
**Note**: 编译结果在`helloRich\build`目录。

##### 创建卡片工程

假如创建卡片工程，进入到ace-loader目录，在ace-loader下创建一个`helloCard`的文件夹，文件夹内包含必要的`manifest.json`页面配置文件，并且包含至少一个页面，页面由hml、css、json文件组成。

示例代码如下：

index.hml
```hml
<div class="container">
  <div class="text-div">
    <text class="title">This is the index page.</text>
  </div>
  <div class="button-div">
    <button value="Go to the second page" class="button" onclick="router"/>
  </div>
</div>
```
index.css
```css
.container {
  justify-content: center;
  align-items: center;
}

.title {
  font-size: 50px;
}

.button-div {
  padding-top: 50px;
  align-items: center;
  justify-content: center;
}

.text-div {
  align-items: center;
  justify-content: center;
}

.button {
  font-size: 30px;
}
```
index.json
```json
{
  "data": {
    "show": true,
    "display": false,
    "card_name": "weather",
    "temperature": "35°",
    "city": "SH",
    "date": "2020.09.04",
    "weather_info": "cloud",
    "image_src":"../../common/clouds.png"
  },
  "actions": {
    "router": {
      "action": "router",
      "bundleName": "com.example.helloworld",
      "bundleAbility": "com.example.helloworld.MainAbility"
    }
  }
}
```
manifest.json
```json
{
  "appID": "com.example.helloworld",
  "appName": "HelloWorld",
  "versionName": "1.0.0",
  "versionCode": 1,
  "minPlatformVersion": "1.0.1",
  "pages": [
    "pages/index/index"
  ],
  "window": {
    "designWidth": 720,
    "autoDesignWidth": false
  },
  "type": "form"
}
```

工程`helloCard`的目录结构:

- helloCard
  - pages
    - index
      - index.hml
      - index.css
      - index.json
  - manifest.json

需要编译创建的`helloCard`工程时，在`package.json`的scripts里添加一行脚本
```
"helloCard": "cd helloCard && webpack --config ../webpack.rich.config.js"
```
然后在ace-loader目录执行
```
npm run build
npm run helloCard
```
**Note**: 编译结果在`helloCard\build`目录。

#### 创建一个新的页面

以`sample\rich`工程为例，例如在`sample\rich\pages`目录创建一个`bar`页面,需要在项目的`manifest.json`文件的`pages`新增一行，如下所示：
```
  "pages": [
    "pages/index/index",
    "pages/bar/bar"
  ]
```

#### 编译工程

进入到ace-loader目录
```
$ npm run build
$ npm run rich  # 编译sample\rich工程
$ npm run lite  # 编译sample\lite工程
$ npm run card  # 编译sample\card工程
```
**Note**: 编译结果在`sample\[rich|lite|card]\build`目录。
