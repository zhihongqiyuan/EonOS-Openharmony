#  Public Repository for API Declaration Files

## Overview

This repository is used to store .d.ts declaration files of JavaScript/TypeScript APIs and API-related tools.

## Directory Structure

```
├─api
|  ├─@internal
│  |  ├─component
│  |  |  └─ets                 # Declaration file for components in the TypeScript-based declarative development paradigm
|  |  └─ets 
|  ├─config                    # JavaScript-based web-like development paradigm
|  ├─form                      # JavaScript service widget
|  ├─@ohos.×××.d.ts            # API declaration file
|  └─@system.×××.d.ts          # Deprecated APIs
├─build-tools
   ├─api_check_plugin          # Tool for checking API specifications
   |  ├─plugin
   |  ├─src
   |  └─test
   └─collect_application_api   # Tool for parsing used APIs
      └─src
```

## Repositories Involved

[interface-sdk_js](https://gitee.com/openharmony/interface_sdk-js/tree/master)
