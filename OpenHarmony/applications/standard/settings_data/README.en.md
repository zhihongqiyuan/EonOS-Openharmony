# SettingsData

#### Description
Settingsdata is a system application in openharmony system, which provides users with database access services, such as storing / reading system time format, screen brightness and other system attributes.

####  Directory Structure

```
├── entry
│   └── src
│       └── main
│           └──ets
│           │   └── MainAbility
│           └── resources
├── product
│   └── phone
│       └── src
│           └── main
│               ├── ets
│               │   ├── DataAbility           # Using DataAbility  to provide database services
│               │   └── Utils                 # Utils files
│               └── resources
│                   ├── base
│                   └── rawfile
│                       └── default_settings.json # settingsdata default value file
├── signature                                # Certificate files
│  
└── LICENSE                                  # Copyright license file
```

#### Repositories Involved

System apps

**application_settings_data** 