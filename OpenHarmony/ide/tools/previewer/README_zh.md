# 预览器组件<a name="ZH-CN_TOPIC_0000001076213355"></a>

## 简介

预览器组件为DevEco Studio预览器提供了启动ArkUI渲染引擎的能力，使预览器能够通过命名管道的通信方式发送命令给预览器组件，操控ArkUI渲染引擎做到实时渲染，并将处理好的图片发送到DevEco Studio预览器中展示。预览器组件支持在Windows、MacOS上运行ArkTS及JS应用。

**图 1**  预览器组件架构<a name="fig2606133765017"></a>  
![](figures/预览器组件架构图.PNG "预览器组件架构")

DevEco Studio预览器通过命令行启动预览器组件，传递ArkTS编译产物路径及预览规格等参数数据，预览器组件启动时拉起ArkUI渲染引擎，根据启动参数渲染页面并输出图片给DevEco Studio预览器前端。DevEco Studio预览器通过命名管道发送命令给预览器组件处理页面动态变更信息，预览器组件根据不同命令调用ArkUI处理接口刷新渲染页面输出图片。
## 目录

预览器组件源代码在/ide_previewer下，目录结构如下图所示：

```
/ide_previewer
├── cli                       # 命令处理
├── gn                        # 编译依赖及工具链配置
├── jsapp                     # 渲染引擎调用
├── mock                      # 交互层代码
├── util                      # 工具类
```

## 使用场景

预览器组件随OpenHarmony SDK一起编译和发布，为DevEco Studio预览器提供调用ArkUI渲染页面的能力。

## 构建编译

构建编译相关介绍请见[build](https://gitee.com/openharmony/build)仓的README文档。

## 相关仓

**ide_previewer**

[arkui\_ace\_engine\_lite](https://gitee.com/openharmony/arkui_ace_engine_lite)

[arkui\_ace\_engine](https://gitee.com/openharmony/arkui_ace_engine)

## 贡献

详情请参考[贡献指南](https://gitee.com/openharmony/docs/blob/master/zh-cn/contribute/Readme-CN.md)。