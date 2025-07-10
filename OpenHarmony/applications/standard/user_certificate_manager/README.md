# 用户证书管理部件

## 简介

用户证书管理是OpenHarmony中预置的系统应用，向用户提供授权证书（Certificate Authority，以下简称CA证书）和凭据的管理功能，主要的功能包含：

1、系统预置CA证书的查看；

2、用户CA证书的安装、查看、启用禁用与卸载；

3、用户证书凭据的安装、查看、授权管理与卸载；

4、系统证书凭据（WLAN/VPN证书凭据）的安装、查看、卸载功能；

用户证书管理部件架构如下图所示：

<div align=center>
<img src=doc/image/image-20220727141455437.png width=80% align=center/>
</div>

用户证书管理应用与外部模块的交互包括：

* 证书管理服务：调用证书管理服务接口，进行系统CA证书、用户CA证书、系统证书凭据和用户证书凭据的管理；
* 文件Picker：在使用用户证书管理应用进行CA证书、凭据安装时，通过文件picker选择设备存储中包含的证书、凭据进行安装；
* 用户认证：在使用用户证书管理应用进行CA证书安装时，如果用户设置了锁屏密码，则需要调用用户认证模块对锁屏密码进行校验；
* 包管理：调用包管理相关接口，获取用户证书凭据授权的应用名称及应用图标，以展示在用户证书管理应用界面中。

## 目录

```
├── BUILD.gn                                                   # 部件构建文件
├── build-profile.json5                                        # 编译构建文件
├── bundle.json                                                # 部件构建入口
├── certmanager                                                # 业务证书管理模块目录
    └── src
        └── main
            ├── ets                                            # 代码目录
            │   ├── Application                                # AbilityStage类实现
            │   ├── MainAbility                                # Ability类实现
            │   ├── model                                      # 依赖的外部接口实现
            │   │   ├── BundleModel.ets                        # 获取应用别名及图标
            │   │   ├── CertManagerVo                          # 内部使用的结构体
            │   │   ├── CertMangerModel.ets                    # 调用证书管理服务的接口实现
            │   │   ├── CheckUserAuthModel.ets                 # 认证校验
            │   │   ├── FileIoModel.ets                        # 文件读取
            │   │   └── PreventScreenshotsModel.ets            # 输入密码时防录屏
            │   ├── pages                                      # 页面实现
            │   │   ├── cerEvidenceFa.ets                      # 凭据页面
            │   │   ├── certInstallFromStorage.ets             # 安装证书、凭据页面
            │   │   ├── certManagerFa.ets                      # 用户证书管理主页面
            │   │   ├── certPwdInput.ets                       # 输入密码页面
            │   │   ├── detail                                 # 详情目录
            │   │   │   ├── AuthorizedAppManagementPage.ets    # 被授权的应用列表详情界面
            │   │   │   ├── CaSystemDetailPage.ets             # 系统CA详情界面
            │   │   │   ├── CaUserDetailPage.ets               # 用户CA详情界面
            │   │   │   ├── CredSystemDetailPage.ets           # 系统凭据性情界面
            │   │   │   └── CredUserDetailPage.ets             # 用户凭据详情界面
            │   │   ├── picker                                 # 半模态界面
            │   │   │   ├── CaCertPage.ets                     # CA证书半模态页面
            │   │   │   ├── CertManagerSheetFa.ets             # 半模态页面入口
            │   │   │   ├── CredListPage.ets                   # 凭据半模态页面
            │   │   │   └── InstallPage.ets                    # 安装证书、凭据半模态页面
            │   │   ├── requestAuth.ets                        # 应用请求授权界面
            │   │   └── trustedCa.ets                          # CA证书界面
            │   └── presenter                                  # 页面管理、CA和凭据列表管理实现
            │       ├── CmAppCredAuthPresenter.ets             # 授权应用管理
            │       ├── CmFaPresenter.ets                      # 主页面管理
            │       ├── CmInstallPresenter.ets                 # 安装页面管理
            │       ├── CmShowAppCredPresenter.ets             # 用户凭据列表管理
            │       ├── CmShowSysCaPresenter.ets               # 系统凭据列表管理
            │       ├── CmShowSysCredPresenter.ets             # 系统CA列表管理
            │       └── CmShowUserCaPresenter.ets              # 用户CA列表管理
            └── resources                                      # 资源文件目录
├── entry                                                      # entry模块目录
├── hvigor                                                     # 编译构建文件
├── hvigorfile.js                                              # 编译构建文件
├── hvigorw                                                    # 编译构建文件
├── LICENSE                                                    # 许可文件
├── oh-package.json5                                           # 编译构建文件
└── signature                                                  # 签名profile文件
```

## 编译构建

**单仓编译**

以RK3568为例，在OpenHarmony源码根目录下调用以下指令，单独编译用户证书管理部件。

```shell
./build.sh --product-name rk3568 --ccache --build-target user_certificate_manager
```

> **说明：**
>
> --product-name：产品名称，例如rk3568。
>
> --ccache：编译时使用缓存功能。
>
> --build-target: 编译的部件名称。

**应用安装**

编译完成后，将会在out/rk3568/obj/applications/standard/user_certificate_manager/user_cert_manager目录下生成CertificateManager.hap文件，将该文件拷贝到本地磁盘中（例如D:\），通过以下命令将应用安装到设备上：

```shell
D:\>hdc install D:\CertificateManager.hap
[Info]App install path:D:\CertificateManager.hap, queuesize:0, msg:install bundle successfully.
AppMod finish
```

## 说明

### 接口说明

不涉及。

### 使用场景

通过设置->隐私->证书与凭据，进入用户证书管理应用界面。使用场景包括如下几个方面：

1、WLAN和VPN系统服务在进行连接配置时，可以调用证书管理服务接口安装、读取和使用系统证书凭据。

2、应用开发者在校验HTTPS服务器证书时，需要使用系统预置的CA证书或用户CA证书。

3、应用开发者在对Email邮件或文档进行签名时，可以使用用户证书凭据进行签名。


## 相关仓

[security_certificate_manager](https://gitee.com/openharmony/security_certificate_manager)

[**applications_user_certificate_manager**](https://gitee.com/openharmony-sig/applications_user_certificate_manager)

