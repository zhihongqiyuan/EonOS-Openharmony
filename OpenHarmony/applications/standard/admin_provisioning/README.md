# AdminProvisioning

## Introduction

As a system application preset in OpenHarmony, AdminProvisioning is used to provision MDM services on devices in enterprise environments. It also provides service provisioning interfaces that can be invoked by other subsystems or third-party MDM clients to build a complete MDM solution.

### Architecture

![](figures/adminProvisioning_architecture.png)

## Directory Structure

```
/applications/standard/admin_provisioning
├── build.gradle                                          # Global configuration file
├── settings.gradle                                       # Configuration file of the build module
├── LICENSE                                               # License file
├── entry                                                 # entry module
│   └── src
│       ├── main
│           ├── ets                                       # ets module
│               ├── MainAbility
│                   ├── model                             # Managed data
│                   │   ├── appMagagementImpl
│                   │       ├── appDetailData             # Application detail data
│                   │   ├── baseData                      # Base data
│                   ├── pages                             # Component pages
│                   │   ├── component
│                   │       ├── entryComponent            # Application detail component to be activated
│                   │       ├── headComponent             # Application name component
│                   │       ├── permissionListComponent   # Application permission component to be activated
│                   │   ├── applicationInfo
│                   ├── app.ets                           # Global ets logic and application lifecycle management file
│           ├── resources                                 # Resource configuration file
│               ├── base                                  # Default language, image resources, font size, and color resources
│               ├── en_AS.element                         # Resources for the English language
│               ├── rawfile                               #  Local configuration file
│               ├── zh_CN.element                         # Resources for the Chinese language
│           └── config.json                               # Global configuration file
├── signature                                             # Certificate file
```

## Usage

   For details, see Instructions.

## Repositories Involved

[System Apps](https://gitee.com/openharmony/docs/blob/master/en/readme/system-apps.md)
