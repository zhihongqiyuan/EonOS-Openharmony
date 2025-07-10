# 统一用户认证

- [简介](#简介)
- [目录](#目录)
- [说明](#说明)
  - [接口说明](#接口说明)
  - [使用说明](#使用说明)
- [相关仓](#相关仓)


## 简介

**统一用户认证框架**（user_auth_framework）主要包括三个模块，用户认证、凭据管理和执行器管理。

**用户认证模块**对外提供统一用户身份认证功能，并且开放生物特征认证API给三方应用调用。

**凭据管理模块**提供系统内统一的用户凭据管理（增删改查）接口，向下通过执行器管理模块，调用系统内的执行器资源，完成用户凭据的生命周期管理和安全存储。

**执行器管理模块**提供系统内执行器资源的统一管理和协同调度能力，当前支持口令执行器和人脸执行器的管理。

**图1** 统一用户认证架构图
<img src="figures/统一用户认证架构图.png" alt="统一用户认证架构图" style="zoom:65%;" />

## 目录

```undefined
//base/useriam/user_auth_framework
├── common              # 子系统公共库代码目录
├── frameworks          # 接口层实现代码目录
│   └── js              # js API接口实现代码
│       └── napi        # napi实现代码
│   └── native          # C++代码实现
├── interfaces          # 对外接口存放目录
│   └── inner_api       # 对内部子系统暴露的头文件，供系统服务使用
│   └── kits            # OpenHarmony提供给第三方应用的接口文件
├── sa_profile          # Service Ability 配置文件
├── services            # Service Ability 服务实现
├── test                # 测试代码存放目录
├── bundle.json         # 组件描述文件
```


## 说明

### **用户认证框架inner_api接口说明**

**表1** 执行器管理相关接口

| 接口名  | 描述                             |
| ------ | -------------------------------- |
| Register(info : ExecutorInfo, callback : ExecutorRegisterCallback): void; | 执行器注册 |
| Unregister(info : ExecutorInfo): void; | 执行器反注册 |

**表2** 执行器需要实现的回调接口

| 接口名  | 描述                             |
| ------ | -------------------------------- |
| OnMessengerReady(messenger : ExecutorMessenger, frameworkPublicKey : vector<uint8_t>, templateIds : vector<uint64_t>) void; | 通知执行器信使可用，传入信使（用于后续与执行器通信） |
| OnBeginExecute(scheduleId : uint64_t, publicKey : vector<uint8_t>, commandAttrs : Attributes) int32_t; | 通知执行器开始执行认证相关操作，commandAttrs中传入本次操作的属性 |
| OnEndExecute(scheduleId : uint64_t, commandAttrs : Attributes) int32_t; | 通知执行器结束本次操作 |
| OnSetProperty(properties : Attributes) int32_t; | 设置属性信息 |
| OnGetProperty(conditions : Attributes, results : Attributes) int32_t; | 获取属性信息 |

**表3** 信使函数相关接口

| 接口名  | 描述                             |
| ------ | -------------------------------- |
| SendData(scheduleId : uint64_t, transNum : uint64_t, srcType : ExecutorRole, dstType : ExecutorRole, msg : AuthMessage) int32_t; | 发送消息，消息源为执行器，目的端为认证执行器管理，消息内容由执行器指定，比如返回人脸认证过程中的提示信息（光线过暗） |
| Finish(scheduleId : uint64_t, srcType : ExecutorRole, resultCode : int32_t, finalResult : Attributes) int32_t; | 操作结束，消息源为执行器，目的端为认证执行器管理，消息内容为本次操作的最终结果 |

**表4** 凭据管理相关接口

| 接口名  | 描述                             |
| ------ | -------------------------------- |
| OpenSession(userId : int32_t): vector<uint8_t>; | 打开会话 |
| CloseSession(userId : int32_t): void; | 关闭会话 |
| AddCredential(userId : int32_t, para : CredentialParameters, callback : UserIdmClientCallback): void; | 录入凭据 |
| UpdateCredential(userId : int32_t, para : CredentialParameters, callback : UserIdmClientCallback): void; | 修改凭据 |
| Cancel(userId : int32_t): int32_t; | 取消凭据录入 |
| DeleteCredential(userId : int32_t, credentialId : uint64_t, authToken : vector<uint8_t>, callback : UserIdmClientCallback): void; | 凭据删除 |
| DeleteUser(userId : int32_t, authToken : vector<uint8_t>, callback : UserIdmClientCallback): void; | 删除口令，当系统内删除用户口令时，同时删除该用户的全部认证凭据 |
| EraseUser(userId : int32_t, callback : UserIdmClientCallback): int32_t; | 用于管理员强制删除用户，同时删除该用户的全部认证凭据 |
| GetCredentialInfo(userId : int32_t, authType : AuthType, callback : GetCredentialInfoCallback): int32_t; | 获取凭据信息 |
| GetSecUserInfo(userId : int32_t, callback : GetSecUserInfoCallback): int32_t; | 获取用户信息 |

**表5** 用户认证相关接口

| 接口名  | 描述                             |
| ------ | -------------------------------- |
| GetProperty(userId : int32_t, request : GetPropertyRequest, callback : GetPropCallback) : void; | 获取属性 |
| SetProperty(userId : int32_t, request : SetPropertyRequest, callback : SetPropCallback) : void; | 设置属性 |
| BeginAuthentication(userId : int32_t, challenge : vector<uint8_t>, authType : AuthType, atl : AuthTrustLevel, callback : AuthenticationCallback): uint64_t; | 指定ATL和认证方式，完成用户身份认证 |
| CancelAuthentication(contextId : uint64_t): int32_t; | 取消认证 |
| BeginIdentification(challenge : vector<uint8_t>, authType : AuthType, callback : IdentificationCallback): uint64_t; | 指定认证类型，完成用户身份识别 |
| CancelIdentification(contextId : uint64_t): int32_t; | 取消识别 |

### **用户认证框架js接口说明**

**表6** 用户认证模块js接口

| 接口名  | 描述                             |
| ------ | -------------------------------- |
| getVersion() : number; | 获取版本信息 |
| getAvailableStatus(authType : AuthType, authTrustLevel : AuthTrustLevel) : number; | 检查当前的认证等级是否支持 |
| getProperty(request : GetPropertyRequest, callback : AsyncCallback<ExecutorProperty>): void; | 获取属性 |
| setProperty(request : SetPropertyRequest, callback : AsyncCallback<number>): void; | 设置属性 |
| auth(challenge : Uint8Array, authType : AuthType, authTrustLevel : AuthTrustLevel, callback : IUserAuthCallback): Uint8Array; | 用户认证 |
| authUser(userId : number, challenge : Uint8Array, authType : AuthType, authTrustLevel : AuthTrustLevel, callback : IUserAuthCallback): Uint8Array; | 用户认证 |
| cancelAuth(contextID : Uint8Array) : number; | 取消认证 |

### 使用说明

- 厂商在对接统一用户认证框架时，需要在可信执行环境中实现以下两个功能点：

1. 认证方案生成：根据目标用户录入的认证凭据和目标认证安全等级，决策用户身份认证方案。
2. 认证结果评估：根据执行器返回的身份认证结果，评估是否达到目标认证安全等级。

- 需在可信执行环境内参考[[drivers_peripheral](https://gitee.com/openharmony/drivers_interface)]仓```user_auth```目录，实现[[drivers_interface](https://gitee.com/openharmony/drivers_interface)]仓内文件```user_auth/v1_0/IUserAuthInterface.idl``` 中定义的接口，保证用户身份认证方案决策和结果评估过程的安全性。

## 相关仓

**[useriam_user_auth_framework](https://gitee.com/openharmony/useriam_user_auth_framework)**

[useriam_pin_auth](https://gitee.com/openharmony/useriam_pin_auth)

[useriam_face_auth](https://gitee.com/openharmony/useriam_face_auth)

[drivers_peripheral](https://gitee.com/openharmony/drivers_peripheral)

[drivers_interface](https://gitee.com/openharmony/drivers_interface)
