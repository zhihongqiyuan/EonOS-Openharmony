# theme_screenlock_mgr

#### Introduction
Provide three-party APP with the ability to request unlockScreen, query the screenlock status, and query whether to set the screenlock password. 
Provide screenOn callback, screenOff callback, screenSaver in and out callback, user switching callback, and screenlock manager service running status callback to the operation management

**subsystem architecture diagram**   
![](figures/subsystem_architecture_zh.png "subsystem architecture diagram")

#### Warehouse path
/base/theme/screenlock_mgr

#### Introduction to framework code
/base/theme/screenlock_mgr
├── figures                  # architecture diagram
├── frameworks
│   ├── js/napi              # the js interface resolves to the napi interface
│   └── native               # interface provided for app
├── sa_profile               # module contains the config files of system services and processes
├── services                 # implementation of screenlock manager service
├── test                     # unit test of interface
└── utils                    # module contains log printing and constants for ordered commonEvent

#### JS APIs and instructions

-   ScreenLock interface documentation [js-apis-screen-lock.md](https://gitee.com/openharmony/docs/blob/master/en/application-dev/reference/apis/js-apis-screen-lock.md) Provides interface definitions and sample code starting with API version 7.

#### Debugging method

1.   Compile command

./build.sh --product-name (Fill in the specific product name, such as：Hi3516DV300) --build-target screenlock_native

2.  push so file

in $root\out\ohos-arm-release\miscservices\screenlock_native，push libscreenlock_server.z.so libscreenlock_client.z.so 
libscreenlock_utils.z.so to system/lib，and push libscreenlockability.z.so to system/lib/module/app下.make sure the four so files is readable at least.

3.  reboot

#### Participation contribution

1. Fork warehouse
2. Submission code
3. Create a new pull request
4. Commit is complete

