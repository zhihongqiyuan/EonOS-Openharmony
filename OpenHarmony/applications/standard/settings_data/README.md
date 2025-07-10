# SettingsData



#### 简介
SettingsData是 OpenHarmony 系统中预置的系统应用，为用户提供数据库的访问存取服务，例如存储/读取系统时间格式、屏幕亮度等系统属性。

#### 目录

```
├── entry
│   └── src
│       └── main
│           └──ets
│           │   └── MainAbility
│           └── resources
├── product
│   └── phone
│       └── src
│           └── main
│               ├── ets
│               │   ├── DataAbility               # 使用DataAbility框架完成数据库能力
│               │   └── Utils                     # 相关的工具类文件
│               └── resources
│                   ├── base
│                   └── rawfile
│                       └── default_settings.json # 设置数据库的默认值配置文件
├── signature                                     # 证书文件目录
│  
└── LICENSE                                       # 许可文件
```



#### 相关仓

系统应用

**application_settings_data**



