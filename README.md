# 下面文段摘自OpenHarmony 6.0 ,目前源码上传中，暂未完整


## 版本概述

OpenHarmony 6.0 Beta1版本进一步增强ArkUI组件能力，提供更安全、更灵活的组件布局；进一步增强窗口能力，新增支持窗口处理文本显示的能力；进一步增强分布式数据管理能力，支持管理资产和资产组，支持应用对标准化数据进行展示；进一步位置服务、输入法框架的相关能力；等等。

更详细的特性新增与增强的说明如下：


### 应用框架

- 新增AppServiceExtensionAbility模块，提供后台服务相关扩展能力，包括后台服务的创建、销毁、连接、断开等生命周期回调。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-ability-kit/js-apis-app-ability-appServiceExtensionAbility.md)）

- 新增支持应用查询自身权限授权状态信息。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-ability-kit/js-apis-abilityAccessCtrl.md#getselfpermissionstatus20)）


### ArkUI

- 组件基础能力增强：
  - 组件背景安全区默认延伸：当组件与非安全区邻接时（包括SafeAreaPadding，AI导航栏，状态栏），组件背景默认延伸到非安全区，提升沉浸式开发体验。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkui/arkui-ts/ts-universal-attributes-background.md#background20)）
  - 忽略组件布局安全区边缘：当组件与非安全区邻接时（包括SafeAreaPadding，AI导航栏，状态栏），组件设置忽略布局安全区边缘后可布局到非安全区。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkui/arkui-ts/ts-universal-attributes-expand-safe-area.md#ignorelayoutsafearea20)）
  - 基础布局能力：宽高支持LayoutPolicy布局能力，包括自适应父组件matchParent，自适应内容wrapContent，自适应内容（忽略父组件的约束）fitAtIdealSize。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkui/arkui-ts/ts-types.md#layoutpolicy15)）

- 新增支持对Navigation双栏模式设置默认占位页。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkui/arkui-ts/ts-basic-components-navigation.md#splitplaceholder20)）

- UX视觉效果优化：菜单与拖拽过渡，菜单跟随效果优化：拖拽发起时，实时更新菜单位置；菜单整体跟随拖拽跟手图；跟手图存在缩放场景时，菜单与跟手图之间的间隔整体相对合理。

- 通过XComponent组件新开的lockCanvas接口，开发者可获取到Drawing Canvas对应的ArkTS对象，使用该对象执行绘制指令会直接绘制到XComponent上，相较于基于NDK接口绘制，更易用。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkui/arkui-ts/ts-basic-components-xcomponent.md#lockcanvas20)）

- 新增支持取消隐式动画的能力。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkui/js-apis-arkui-frameNode.md#cancelanimations20)）

- 保存控件新增支持图标和文字的自定义能力。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkui/arkui-ts/ts-security-components-savebutton.md#savebuttonattribute)）


### 窗口管理

- 新增支持窗口显示时默认不获取焦点，点击后可获取焦点。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkui/js-apis-window.md#showwindow20)）

- 新增支持输入法动画开始前和结束后的回调，方便应用进行输入法弹出与收回动画的响应。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkui/js-apis-window.md#onkeyboardwillshow20)

### 图形

新增支持中西文自动间距，支持排版时去除行位空格；新增支持修改文本颜色无需重新排版；新增支持不跟随系统高对比度文字开关设置。（[API参考](https://gitee.com/openharmony/docs/blob/master/zh-cn/application-dev/reference/apis-arkgraphics2d/js-apis-graphics-text.md#paragraphstyle)

### 包管理

- 新增setShortcutVisibleForSelf接口，支持设置当前应用的快捷方式是否显示。（[API参考](https://gitee.com/openharmony/docs/blob/master/zh-cn/application-dev/reference/apis-ability-kit/js-apis-shortcutManager.md)）

- 新增getAbilityInfo接口，支持根据指定的uri获取应用的ability信息。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-ability-kit/js-apis-bundleManager.md#bundlemanagergetabilityinfo20)）


### 分布式数据管理

- 新增支持设置资产和资产组的相关接口。（[指南](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/database/data-sync-of-distributed-data-object.md#%E8%B5%84%E4%BA%A7%E5%90%8C%E6%AD%A5%E6%9C%BA%E5%88%B6)）

- 新增提供基于标准化数据结构的标准卡片控件接口，支持应用对标准化数据结构进行可视化展示。

- UDMF新增DataHub、系统分享、picker、右键菜单这三类数据通道的生命周期管理能力，提供ArkTS和C API。（[API参考-ArkTS](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkdata/js-apis-data-unifiedDataChannel.md#intention)、[API参考-C API](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkdata/capi-udmf-h.md#%E6%9E%9A%E4%B8%BE%E7%B1%BB%E5%9E%8B%E8%AF%B4%E6%98%8E)）
  其中DataHub仅C API为本次新增，ArkTS已支持。


### 分布式软总线

新开放底层连接能力接口，实现连接能力提升。（[指南](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/distributedservice/linnkEnhance_development-guide.md)）


### 分布式服务

新增C API支持获取本地设备名称。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-distributedservice-kit/capi-devicemanager.md)）


### 事件通知

新增支持三方应用获取通知响铃震动等设置信息。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-notification-kit/js-apis-notificationManager.md#notificationmanagergetnotificationsetting20)）


### 语言编译器运行时

- 新增针对系统JSVM引擎API调用内存泄漏问题的定位能力。（[指南](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/napi/jsvm-frequently-questions.md)）

- 新增FastBuffer对象定义。FastBuffer对象是更高效的Buffer容器，用于表示固定长度的字节序列，是专门存放二进制数据的缓存区。（[指南](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkts/js-apis-fastbuffer.md)）

- 新增支持以动态序列化方式生成XML文件。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-arkts/js-apis-xml.md)）


### 基础通信

Wi-Fi新增支持连接候选网络时提示确认是否信任该网络，并提供确认的回调。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-connectivity-kit/js-apis-wifiManager.md#wifimanagerconnecttocandidateconfigwithuseraction20)）


### 位置服务

新增支持获取两个位置之间直线距离的能力。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-location-kit/js-apis-geoLocationManager.md#geolocationmanagergetdistancebetweenlocations20)）


### 多模输入

新增支持触屏上报浮点类型坐标。


### DFX

- 新增使用HiDebug对应用主线程进行栈回溯的能力。（[指南](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/dfx/hidebug-guidelines-ndk.md#%E4%BD%BF%E7%94%A8hidebug%E5%AF%B9%E5%BA%94%E7%94%A8%E4%B8%BB%E7%BA%BF%E7%A8%8B%E8%BF%9B%E8%A1%8C%E6%A0%88%E5%9B%9E%E6%BA%AF)）

- 支持sanitizer事件对接到HiAppEvent。（[指南](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/dfx/hiappevent-watcher-address-sanitizer-events.md)）


### 安全

- 证书管理新增openAuthorizeDialog接口，支持拉起用户证书凭据授权界面，用户授权后应用可使用授权的证书凭据进行双向TLS认证或文档签名。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-device-certificate-kit/js-apis-certManagerDialog.md#certificatemanagerdialogopenauthorizedialog20)）

- 加解密算法库框架提供了非对称加解密、签名、密钥协商、密钥派生、消息认证码、随机数的C接口；提供了ASN1格式和R|S格式的sm2签名数据互转能力；新增支持DES算法。（[指南](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/security/CryptoArchitectureKit/Readme-CN.md)）

- 证书算法库支持获取utf-8编码的证书或证书吊销列表的颁发者名称。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-device-certificate-kit/js-apis-cert.md#tostring20)）


### 输入法框架

- 悬浮软键盘和候选词窗口新增支持startMoving。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-ime-kit/js-apis-inputmethodengine.md#startmoving15)）

- 新增支持通过hdc命令管理输入法。（[指南](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/inputmethod/inputmethod-hdc-commands-guide.md)）

- 新增支持编辑框放弃正在输入的文字。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-ime-kit/js-apis-inputmethod.md#discardtypingtext20)）

- 新增支持感知到编辑框中的占位文本和所属Ability的名字。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-ime-kit/js-apis-inputmethodengine.md#editorattribute)）

- 新增支持短信验证码编辑框。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-ime-kit/js-apis-inputmethod.md#textinputtype10)）

- 新增支持自动大小写模式。（[API参考](https://gitee.com/openharmony/docs/blob/OpenHarmony-6.0-Beta1/zh-cn/application-dev/reference/apis-ime-kit/js-apis-inputmethodengine.md#capitalizemode20)）


## 配套关系

**表1** 版本软件和工具配套关系

| 软件 | 版本 | 备注 | 
| -------- | -------- | -------- |
| OpenHarmony | 6.0 Beta1 | NA | 
| Public SDK | Ohos_sdk_public 6.0.0.34 (API Version 20 Beta1) | 面向应用开发者提供，不包含需要使用系统权限的系统接口。通过DevEco Studio默认获取的SDK为Public SDK。 | 
| HUAWEI DevEco Studio（可选） | 6.0.0 Beta1 | OpenHarmony应用开发推荐使用。<br />*待发布后提供*。 | 
| HUAWEI DevEco Device Tool（可选） | 4.0 Release | OpenHarmony智能设备集成开发环境推荐使用。<br />[请点击这里获取](https://device.harmonyos.com/cn/develop/ide#download)。 | 


## 源码获取






## 修复缺陷列表
