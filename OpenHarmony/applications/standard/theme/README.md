# Theme

## 简介

**内容介绍：**

主题应用是 OpenHarmony 系统中预置的系统应用，为用户提供设置系统主题、桌面壁纸的基础能力，并支持通过其他第三方应用设置用户自定义的主题与壁纸。

## 目录

```
/applications/standard/theme
├── entry                         # 主entry模块目录
│   └── src
│       └── main
│           ├── ets               # ets模块目录
│           ├────── ├── pages     # 组件页面存放目录
│           |       ├── resources # 其它共用资源
│           ├── resources         # 资源配置文件存放目录
│           |   └── base          # 默认资源
|           |       ├── element   # 默认文字内容
|           |       ├── media     # 默认图片资源
|           |       └── profile   # 默认配置
│           └── module.json5      # 全局配置文件
└── LICENSE                       # 许可文件
```

## 相关仓

系统应用

**applications\_theme**
