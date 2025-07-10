# Unified User Authentication (userauth)



## Introduction

As a basic component of the User Identity & Access Management (IAM) subsystem, Unified User Authentication (userauth) implements unified user authentication and provides biometric feature authentication APIs to third-party applications.

**Figure 1** userauth architecture

<img src="figures/userauth_architecture.png" alt="userauth_architecture" style="zoom:80%;" />



The userauth APIs support user authentication of the target Authentication Trust Level (ATL). The target ATL is specified by the service. The target user ID can be specified by the service (system service or basic system application) or obtained from the system context (third-party application).

## Directory Structure

```undefined
//base/useriam/user_auth_framework
├── frameworks          # Framework code
├── interfaces          # Directory for storing external interfaces
│   └── innerkits       # Header files exposed to the internal subsystems
├── sa_profile          # Profile of the Service ability
├── services            # Implementation of the Service ability
├── test                # Directory for storing test code
├── utils               # Directory for storing utility code
├── bundle.json         # Component description file
└── userauth.gni        # Build configuration
```


## Usage

### Available APIs

**Table 1** APIs for unified user authentication

| API | Description                            |
| ------ | -------------------------------- |
| getAvailableStatus(authType : AuthType, authTrustLevel : AuthTrustLevel) : number; | Obtains the available authentication status.|
| auth(challenge: BigInt, authType : AuthType, authTrustLevel: AuthTrustLevel, callback: IUserAuthCallback): BigInt; | Performs user authentication. |

### Usage Guidelines

- Vendors must implement the following in a Trusted Execution Environment (TEE):

1. Authentication scheme: Determine the user authentication scheme based on the user credentials entered and the target ATL.
2. Authentication result evaluation: Evaluate whether the authentication reaches the target ATL based on the authentication result returned by the executor.

- The APIs defined in the header file ```user_auth/v1_0/IUserAuthInterface.idl``` in the [drivers_interface](https://gitee.com/openharmony/drivers_interface) repository must be implemented in a TEE, and the security of user authentication scheme and result evaluation must be ensured.



## Repositories Involved

**[useriam_user_auth_framework](https://gitee.com/openharmony/useriam_user_auth_framework)**

[useriam_pin_auth](https://gitee.com/openharmony/useriam_pin_auth)

[useriam_face_auth](https://gitee.com/openharmony/useriam_face_auth)

[drivers_peripheral](https://gitee.com/openharmony/drivers_peripheral)

[drivers_interface](https://gitee.com/openharmony/drivers_interface)
