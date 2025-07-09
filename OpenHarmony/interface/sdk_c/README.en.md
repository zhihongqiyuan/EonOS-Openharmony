Public Repository for API Declaration Files

## Overview

This repository is used to store .h declaration files of C APIs. The OpenHarmony C API is collection of system C interfaces that the operating system provided for applications using the C/C++ language library.It is the contract between the system and the applications; These interfaces need to be sufficiently stable. Interfaces are currently organized into various directories according to the function, and the directories organized as follows.

## Directory Structure

    ─ai
    |  ├─neural_network_runtime
    ├─arkui
    |  ├─ace_engine
    ├─bundlemanager
    |  ├─bundle_framework
    │  |  ├─bundle
    ├─commonlibrary
    |  ├─memory_utils
    │  |  ├─bundle
    ├─distributeddatamgr
    |  ├─relational_store
    ├─drivers
    |  ├─external_device_manager
    ├─global
    |  ├─resource_management
    ├─graphic
    |  ├─graphic_2d
    ├─hiviewdfx
    |  ├─hiappevent
    |  ├─hilog
    |  ├─hitrace
    ├─multimedia
    |  ├─audio_framework
    |  ├─av_codec
    |  ├─image_framework
    |  ├─player_framework
    ├─resourceschedule
    |  ├─ffrt
    ├─security
    |  ├─huks
    ├─startup
    |  ├─init
    ├─third_party
    

## Repositories Involved

[interface_sdk_c](https://gitee.com/openharmony-sig/interface_sdk_c/)
[C API编码规范](https://gitee.com/openharmony-sig/interface_sdk_c/blob/master/docs/capi_naming.md): A guide that describes how to design and publish an interface.
[C API构建指南](https://gitee.com/openharmony-sig/interface_sdk_c/blob/master/docs/howto_add.md)：A guide that describes how to add a build target for the C API in the build.gn