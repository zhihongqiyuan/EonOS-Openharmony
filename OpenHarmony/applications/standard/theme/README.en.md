# Theme

## Introduction

**Overall introduction**

The Theme is a system application prebuilt in OpenHarmony. It provides a basic ability for users to set system themes and desktop wallpaper, and it support change system themes and desktop wallpaer by other applications.

## Directory Structure

```
/applications/standard/theme
├── entry                         # main entry module
│   └── src
│       └── main
│           ├── ets               # ets module folder
│           ├────── ├── pages     # pages folder
│           |       ├── resources # other common resources
│           ├── resources         # resources folder
│           |   └── base          # default resources
|           |       ├── element   # default language resources
|           |       ├── media     # default piture resources
|           |       └── profile   # default configs
│           └── module.json5      # global configs
└── LICENSE                       # copyright license file
```

## Repositories Involved

System apps

**applications\_theme**
