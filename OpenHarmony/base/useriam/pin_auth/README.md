# PIN Authentication (pinauth)


## Introduction

The PIN authentication (pinauth) module supports the setting, deletion, and authentication of user PINs. Working with the User IAM subsystem framework, the pinauth module supports PIN change.

pinauth is one of the basic user identity authentication executors of OpenHarmony. It registers PIN authentication resource information with the collaborative authentication framework based on the resource registration interface defined by collaborative authentication, and implements PIN setting, deletion, and authentication based on the scheduling of the collaborative authentication framework.

**Figure 1** pinauth architecture

<img src="figures/pinauth_architecture.png" alt="pinauth_architecture" style="zoom:80%;" />



PINs are core assets in the system. The following security measures are taken in PIN authentication for security purposes:

- The PIN input user interface (currently including the PIN setting and PIN authentication dialog boxes) is provided by system applications (<sup>NOTE 1</sup>). The dialog boxes involved in user settings are provided by the **Settings** application, and the PIN input dialog boxes involved in the screen unlock authentication are provided by the **Lock screen** application.
- Password data transmission: The raw PIN data is not transmitted across devices. After a user PIN is entered in the PIN input dialog box, the raw PIN data entered is transmitted to the pinauth Service ability only after unidirectional processing in the input model management innerAPI.
- Secure storage and comparison of PIN data: The pinauth HDI defines the adaptation interfaces for device vendors. Device vendors can implement secure PIN comparison and storage in a TEE. <sup>NOTE 2</sup>

**NOTE 1**: To implement the PIN input dialog box, the application needs to register the PIN input dialog box with the pinauth Service ability through the input dialog box management API. The input box management API requires a high-level access permission and can be invoked only by system users.

**NOTE 2**: The OpenHarmony framework provides pure software implementation of PIN authentication for developers to demonstrate the PIN authentication function. The pure software implementation does not include the secure storage capability.

## Directory Structure

```undefined
//base/useriam/pin_auth
├── common                  # Common code
├── figures                 # Figures for README.md
├── frameworks              # Framework code
├── interfaces              # APIs exposed externally
│   └── inner_api           # Header file exposed to internal subsystems for system abilities
├── sa_profile              # Service ability profile (configuration)
├── services                # Implementation for the Service ability
├── test                    # Directory for storing test code
├── bundle.json             # Component description file
└── pinauth.gni             # Build configuration
```


## Usage

### Available APIs

**Table 1** APIs for managing input dialog boxes

| API | Description                            |
| ------ | -------------------------------- |
| function registerInputer(inputer:IInputer) : boolean; | Registers the **inputer** callback with the pinauth Service ability.  |
| function unregisterInputer():void;                    | Unregisters the **inputer** callback from the pinauth Service ability.|

**Table 2** APIs for transferring data

| API| Description                      |
| ------ | -------------------------------- |
| onGetData: (callback:IInputData)=>void | **inputer** callback invoked by the pinauth Service ability to obtain the PIN data. |
| onSetData: (pinSubType:AuthSubType, data:Uint8Array)=>void | **inputerdata** callback invoked to transfer the PIN data from the PIN input dialog box to the pinauth Service ability. |

### Usage Guidelines

The interfaces defined in **driver\interface\pin_auth\IExecutor.idl** must be implemented in a secure environment (trusted execution environment or security chip) to ensure protection of the highest security level on user PIN information.

## Repositories Involved

[useriam_user_auth_framework](https://gitee.com/openharmony/useriam_user_auth_framework)

[useriam_face_auth](https://gitee.com/openharmony/useriam_face_auth)

**[useriam_pin_auth](https://gitee.com/openharmony/useriam_pin_auth)**

[drivers_peripheral](https://gitee.com/openharmony/drivers_peripheral)

[drivers_interface](https://gitee.com/openharmony/drivers_interface)
