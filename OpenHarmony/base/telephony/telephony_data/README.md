# Data Storage<a name="EN-US_TOPIC_0000001152064139"></a>

-   [Introduction](#section117mcpsimp)
-   [Directory Structure](#section124mcpsimp)
-   [Constraints](#section128mcpsimp)
-   [Usage](#section134mcpsimp)
    -   [Available APIs](#section136mcpsimp)
    -   [Permissions]( #section137mcpsimp)

-   [Usage Guidelines](#section163mcpsimp)
    -   [Parameters of the Insert API](#section1099113151207)
    -   [Parameters of the Delete API](#section1098113151208)
    -   [Parameters of the Update API](#section1099113151207)
    -   [Parameters of the Query API](#section1096113151208)
    -   [Sample Code](#section1558565082915)

-   [Repositories Involved](#section206mcpsimp)

## Introduction<a name="section117mcpsimp"></a>

The data storage module stores persistent data of key modules of the Telephony subsystem, such as the SIM cards and SMS modules, and provides the **DataShare** API for data access.

**Figure 1** Architecture of the data storage module<a name="fig13267152558"></a>

![](figures/en-us_architecture-of-the-data-storage-module.png)

## Directory Structure<a name="section124mcpsimp"></a>

```
/base/telephony/data_storage     # Data storage
├─ BUILD.gn                         # Build script (gn)
├─ README.md                        # Readme
├─ common                           # Public and common files
│  ├─ include
│  └─ src
├─ opkey                            # opkey framework
│  ├─ include
│  └─ src
├─ pdp_profile                      # Network carrier
│  ├─ include
│  └─ src
├─ sim                              # SIM card
│  ├─ include
│  └─ src
├─ sms_mms                          # SMS/MMS
│  ├─ include
│  └─ src
├─ ohos.build                       # Build code
└─ test                             # Test code
```

## Constraints<a name="section128mcpsimp"></a>

- Programming language: C++

- Software constraints: This module must work with the Utils and Application Framework subsystems.

- Hardware constraints: none

- Use case: When a user needs to edit the persistent data stored in Telephony subsystem modules, such as the SIM card and SMS/MMS modules, your application can call the **Insert**, **Delete**, **Update**, and **Query** APIs provided by **DataShareHelper** as demanded.

  You need to declare the corresponding permission for access to the specified URI.

## Usage<a name="section134mcpsimp"></a>

### Available APIs<a name="section136mcpsimp"></a>

**Table 1** APIs provided by DataShareHelper

<a name="table165976561598"></a>

| API Definition                                                    | **Description**|
| ------------------------------------------------------------ | ------------ |
| int Insert(Uri &uri, const DataShare::DataShareValuesBucket &value) | Inserts data.     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|
| int Delete(const Uri &uri, const DataShare::DataSharePredicates &predicates) | Deletes data.     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|
| int Update(Uri &uri, const DataSharePredicates &predicates, const DataShareValuesBucket &value) | Updates data.     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|
| std::shared_ptr\<DataShare::DataShareResultSet\> Query(Uri &uri, const DataSharePredicates &predicates, std::vector\<std::string\> &columns) | Queries data.     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|

**Table 2** Required permissions

<a name="table165976561598"></a>

| Module          | **Required Permission**                                                |
| -------------- | ------------------------------------------------------------ |
| SMS/MMS module    | ohos.permission.READ_MESSAGES                                |
| SIM card module     | ohos.permission.GET_TELEPHONY_STATE<br>ohos.permission.SET_TELEPHONY_STATE |
| Network carrier module| ohos.permission.GET_TELEPHONY_STATE<br>ohos.permission.SET_TELEPHONY_STATE |
| opkey module| ohos.permission.GET_TELEPHONY_STATE<br>ohos.permission.SET_TELEPHONY_STATE |

## Usage Guidelines<a name="section163mcpsimp"></a>

### Parameters of the Insert API<a name="section1099113151207"></a>

**Table 3** Parameters of the Insert API

<a name="table1234838197"></a>

| Parameter | **Description**                             |
| ----- | ------------------------------------- |
| uri   | Resource path.                             |
| value | Data set. This field corresponds to the table structure field on which the current operation is performed.|

### Parameters of the Delete API<a name="section1098113151208"></a>

**Table 4** Parameters of the Delete API

<a name="table1234838197"></a>

| Parameter      | Description                                 |
| ---------- | ------------------------------------- |
| uri        | Resource path.                             |
| predicates | Conditions for data deletion.                             |

### Parameters of the Update API<a name="section1097113151210"></a>

**Table 5** Parameters of the Update API

<a name="table1234838197"></a>

| Parameter      | Description    |
| ---------- | -------- |
| uri        | Resource path.|
| predicates | Conditions for data updating.|
| value      | Data set. This field corresponds to the table structure field on which the current operation is performed.|

### Parameters of the Query API<a name="section1096113151208"></a>

**Table 6** Parameters of the Query API

<a name="table1234838197"></a>

| Parameter      | Description          |
| ---------- | -------------- |
| uri        | Resource path.      |
| predicates | Conditions for data query.      |
| columns    | Fields in the query result.|

### Sample Code<a name="section1558565082915"></a>

The following provides the procedure and sample code for you to query, insert, delete, or update SMS/MMS data:

1. Call **SystemAbilityManagerClient** to obtain a **SystemAbilityManager** object.
2. Call **saManager** to obtain an **IRemoteObject** object based on the specified service ID.
3. Call **IRemoteObject** to create a **DataShareHelper** object.
4. Call **DataShareHelper::Query** to query data, and call the other related APIs to process data.

   Sample code for creating a **DataShareHelper** instance:
   ```
   std::shared_ptr<DataShare::DataShareHelper> DataStorageGtest::CreateDataShareHelper(
       int32_t systemAbilityId, std::string &uri)
   {
       // Obtain a SystemAbilityManager instance through SystemAbilityManagerClient.
       auto saManager = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
       if (saManager == nullptr) {
           DATA_STORAGE_LOGE("DataSimRdbHelper Get system ability mgr failed.");
           return nullptr;
       }
       // Obtain an IRemoteObject.
       auto remoteObj = saManager->GetSystemAbility(systemAbilityId);
       while (remoteObj == nullptr) {
           DATA_STORAGE_LOGE("DataSimRdbHelper GetSystemAbility Service Failed.");
           return nullptr;
       }
       // Create a DataShareHelper instance.
       return DataShare::DataShareHelper::Creator(remoteObj, uri);
   }
   ```
   Sample code for querying SMS/MMS messages:
   ```
   std::shared_ptr<DataShare::DataShareResultSet> DataStorageGtest::SmsSelect(const std::shared_ptr<DataShare::DataShareHelper> &helper)
   {
       // Resource path
       Uri uri("datashare:///com.ohos.smsmmsability/sms_mms/sms_mms_info");
       // Fields in the query result
       std::vector<std::string> columns;
       // Phone number of the message sender
       columns.push_back("sender_number");
       // Message title
       columns.push_back("msg_title");
       // Message content
       columns.push_back("msg_content");
       // Query predicates
       DataShare::DataSharePredicates predicates;
       // Call the DataShareHelper::Query API to query data.
       return helper->Query(uri, predicates, columns);
   }
   ```
   Sample code for inserting SMS/MMS messages:
   ```
   int SmsInsert(std::shared_ptr<DataShare::DataShareHelper> &helper)
   {
       Uri uri("datashare:///com.ohos.smsmmsability/sms_mms/sms_mms_info");
       DataShare::DataShareValuesBucket value;
       // Phone number of the message recipient
       value.Put(SmsMmsInfo::RECEIVER_NUMBER, "138XXXXXXXX");
       // Message content
       value.Put(SmsMmsInfo::MSG_CONTENT, "ceshi");
       value.Put(SmsMmsInfo::GROUP_ID, 1);
       return helper->Insert(uri, value);
   }
   ```
   Sample code for deleting SMS/MMS messages:
   ```
    int SmsDelete(std::shared_ptr<DataShare::DataShareHelper> helper)
    {
        Uri uri("datashare:///com.ohos.smsmmsability/sms_mms/sms_mms_info");
        DataShare::DataSharePredicates predicates;
        // Delete the message whose MSG_ID is 1.
        predicates.EqualTo(SmsMmsInfo::MSG_ID, "1");
        return helper->Delete(uri, predicates);
    }
   ```
   Sample code for updating SMS/MMS messages:
   ```
   int SmsUpdate(std::shared_ptr<DataShare::DataShareHelper> helper)
   {
       Uri uri("datashare:///com.ohos.smsmmsability/sms_mms/sms_mms_info");
       DataShare::DataShareValuesBucket values;
       // Message content
       values.Put(SmsMmsInfo::MSG_CONTENT, "hi ohos");
       DataShare::DataSharePredicates predicates;
       // Message ID
       predicates.EqualTo(SmsMmsInfo::MSG_ID, "1");
       return helper->Update(uri, predicates, values);
   }
   ```


## Repositories Involved<a name="section206mcpsimp"></a>

[Telephony Subsystem](https://gitee.com/openharmony/docs/blob/master/en/readme/telephony.md)

**telephony_data_storage**

[telephony_core_service](https://gitee.com/openharmony/telephony_core_service/blob/master/README.md)

[telephony_sms_mms](https://gitee.com/openharmony/telephony_sms_mms/blob/master/README.md)

[telephony_cellular_call](https://gitee.com/openharmony/telephony_cellular_call/blob/master/README.md)

[telephony_call_manager](https://gitee.com/openharmony/telephony_call_manager/blob/master/README.md)
