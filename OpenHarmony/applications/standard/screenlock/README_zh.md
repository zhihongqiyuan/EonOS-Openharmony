# 锁屏

## 简介

### 内容介绍
锁屏应用是OpenHarmony中预置的系统应用，为用户提供锁屏的基础能力。提供滑动解锁、密码解锁等解锁能力，以及锁屏页面的信息展示能力。

### 架构图介绍

![](figures/screenlock.png)

feature
```
1.在开机流程、亮屏流程、灭屏流程中添加加锁流程及所对应的解锁流程，实现与系统交互。\
2.通过时间日期组件、通知组件、状态栏组件、lockIcon和快捷开关组件，实现相关的信息展示。\
3.支持数字解锁、图案解锁、人脸解锁和密码混合解锁。
```

common
```
通用接口，实现日志打印，外部接口管理等功能。
```
## 目录

```
/applications/standard/screenlock
    ├── common                          # 通用工具类目录
    ├── entry                           # entry模块目录
    ├── features                        # 子组件目录
    │   ├── batterycomponent            # 电池组件
    │   ├── clockcomponent              # 时钟组件
    │   ├── datetimecomponent           # 日期时间组件
    │   ├── noticeitem                  # 通知子组件
    │   ├── notificationservice         # 通知服务组件
    │   ├── screenlock                  # 锁屏组件
    │       ├── common                  # 通用工具
    │       ├── model                   # 数据管理
    │       ├── view                    # 组件样式管理
    │       ├── vm                      # 数据样式绑定管理
    │   ├── shortcutcomponent           # 快捷开关组件
    │   ├── wallpapercomponent          # 壁纸组件
    │   ├── wificomponent               # wifi组件
    ├── product                         # 锁屏总体功能目录
    │   ├── pc                          # 模块目录
    │       ├── pages/slidesrceenlock   # 滑动锁屏   
    │   ├── phone                       # 模块目录
    │       ├── pages/slidesrceenlock   # 滑动锁屏
    ├── signature                       # 证书文件目录
    ├── build.gradle                    # 全局编译配置文件
    ├── LICENSE                         # 许可文件
    ├── settings.gradle                 # 编译模块配置文件  
```

## 相关仓

系统应用

**applications\_screenlock**

