# 沙箱管理服务

## 简介

沙箱管理服务是OpenHarmony提供的系统服务之一，该服务负责应用沙箱间文件共享规则的管理和持久化等事务。

应用沙箱目录隔离会影响应用间的文件共享操作。为了对合法的应用间文件共享规则进行持久化管理，新增沙箱管理部件以支持共享文件读写规则的持久化存储、管理、激活操作。

沙箱管理模块主要提供以下功能：

1. 持久化规则存储
2. 持久化规则管理
3. 临时/持久化规则设置与激活
4. 提供相应inner api

### 规则生效周期
- 持久化规则：规则在应用卸载前有效
- 临时规则：规则在应用当次生命周期内有效


### 限制与约束
1. SetPolicy接口调用者须具有ohos.permission.SET_SANDBOX_POLICY权限，权限定义详见[access_token](https://gitee.com/openharmony/security_access_token)仓
2. 除SetPolicy、CheckPersistPolicy接口外，其余接口调用者需具有ohos.permission.FILE_ACCESS_PERSIST权限，权限定义详见[access_token](https://gitee.com/openharmony/security_access_token)仓
3. 所有接口中，std::vector\<PolicyInfo\>的大小上限为500条

## 目录

```
/base/accesscontrol/sandbox_manager
├── config                      # 覆盖率设置目录
├── frameworks                  # 框架层，基础功能代码存放目录
│   ├── common                  # 框架公共代码存放目录
│   ├── sandbox_manager         # 沙箱管理服务框架代码存放目录
│   └── test                    # 测试代码存放目录
├── interfaces/inner_api/       # 接口层
│   └── sandbox_manager         # 沙箱管理接口代码存放目录
└── services                    # 服务层
    ├── common                  # 服务公共代码存放目录
    └── sandbox_manager
        └── main                # 沙箱管理服务侧代码存放目录

```

## 使用
### 接口说明

| **接口** | **接口描述** |
| --- | --- |
| int32_t PersistPolicy(const std::vector\<PolicyInfo\> &policy, std::vector<uint32_t> &result); | 添加调用者持久化规则 |
| int32_t UnPersistPolicy(const std::vector\<PolicyInfo\> &policy, std::vector<uint32_t> &result); | 删除调用者持久化规则 |
| int32_t PersistPolicy(uint64_t tokenId, const std::vector\<PolicyInfo\> &policy, std::vector<uint32_t> &result);| 添加指定tokenId的持久化规则 |
| int32_t UnPersistPolicy(uint64_t tokenId, const std::vector\<PolicyInfo\> &policy, std::vector<uint32_t> &result);| 删除指定tokenId的持久化规则 |
| int32_t SetPolicy(uint64_t tokenId, const std::vector\<PolicyInfo\> &policy, uint64_t policyFlag); | 设置临时规则 |
| int32_t StartAccessingPolicy(const std::vector\<PolicyInfo\> &policy, std::vector<uint32_t> &result);| 使能持久化规则 |
| int32_t StopAccessingPolicy(const std::vector\<PolicyInfo\> &policy, std::vector<uint32_t> &result);| 禁用持久化规则 |
| int32_t CheckPersistPolicy(uint64_t tokenId, const std::vector\<PolicyInfo\> &policy, std::vector<bool> &result);| 校验规则是否已持久化 |

### inner api使用
#### 设置/删除持久化规则
```
// 须拥有对应权限
#include "sandbox_manager_kit.h"

std::vector<PolicyInfo> policys;
std::vector<uint32_t> results; // 每条policy的结果都保存在result中

// 设置自身持久化规则
int32_t sandboxManagerErrCode = SandboxManagerKit::PersistPolicy(policys, results);
// 删除自身持久化规则
int32_t sandboxManagerErrCode = SandboxManagerKit::UnPersistPolicy(policys, results);
// 设置指定tokenId的持久化规则
uint64_t tokenId;
int32_t sandboxManagerErrCode = SandboxManagerKit::PersistPolicy(tokenId, policys, results);
// 删除指定tokenId的持久化规则
int32_t sandboxManagerErrCode = SandboxManagerKit::UnPersistPolicy(tokenId, policys, results);
```

#### 设置临时规则
```
// 须拥有对应权限
#include "sandbox_manager_kit.h"

std::vector<PolicyInfo> policys;
uint64_t targetTokenId; // 设置临时规则的目标应用tokenId
uint64_t policyFlag; // 是否允许目标应用对设置的规则进行持久化，0-不允许，1-允许

int32_t sandboxManagerErrCode = SandboxManagerKit::SetPolicy(targetTokenId, policys, policyFlag);
```

#### 使能/禁用持久化规则
```
// 须拥有对应权限
#include "sandbox_manager_kit.h"

std::vector<PolicyInfo> policys; // 列表中的规则须已设置持久化
std::vector<uint32_t> results; // 每条policy的结果都保存在result中

// 使能自身持久化规则
int32_t sandboxManagerErrCode = SandboxManagerKit::StartAccessingPolicy(policys, results);
// 禁用自身持久化规则
int32_t sandboxManagerErrCode = SandboxManagerKit::StopAccessingPolicy(policys, results);
```

#### 查询规则持久化状态
```
// 无需权限即可查询
#include "sandbox_manager_kit.h"

uint64_t tokenId; // 查询对应tokenId的持久化规则
std::vector<PolicyInfo> policys;
std::vector<bool> results; // 查询结果，true-已持久化，false-未持久化

int32_t sandboxManagerErrCode = SandboxManagerKit::CheckPersistPolicy(tokenId, policys, results);
```

### 系统开发者如何使用本模块
1. 在部件bundle.json的deps-components中添加"sandbox_manager"
2. 在部件的.cfg文件中配置需要的权限名
3. 在需要引用本模块的gn脚本的external_deps中添加"sandbox_manager:libsandbox_manager_sdk"
4. 在需要引用本模块的代码文件中#include "sandbox_manager_kit.h"
5. 调用本模块的inner api，参考[inner api使用](#inner-api使用)
6. inner api错误码请见[sandbox_manager_err_code.h](./interfaces/inner_api/sandbox_manager/include/sandbox_manager_err_code.h)
7. PolicyInfo类及输入的result返回值请见[policy_info.h](./interfaces/inner_api/sandbox_manager/include/policy_info.h)中的PolicyInfo、SandboxRetType和OperateMode

### 应用开发者如何使用本模块
本模块不提供JS api，请通过filemanagement和ability的相关接口间接调用本模块。

## 相关仓

**[filemanagement\_app\_file\_service](https://gitee.com/openharmony/filemanagement_app_file_service/blob/master/README_ZH.md)**

**[ability\_ability\_runtime](https://gitee.com/openharmony/ability_ability_runtime/blob/master/README_zh.md)**
