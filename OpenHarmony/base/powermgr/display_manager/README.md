# Display Power Manager<a name="EN-US_TOPIC_0000001152026155"></a>

-   [Introduction](#section11660541593)
-   [Directory Structure](#section19472752217)
-   [Repositories Involved](#section63151229062)

## Introduction<a name="section11660541593"></a>

The Display Power Manager module provides the following capabilities:

1.  Turning on/off the display
2.  Adjusting the brightness of the display screen

**Figure 1** Architecture of the Display Power Manager module<a name="fig106301571239"></a>

![](figures/power-management-subsystem-architecture.png "Architecture of the power management subsystem")

## Directory Structure<a name="section19472752217"></a>

```
base/powermgr/display_manager
├───state_manager                   # state manager 
│   ├── figures                     # Architecture
│   ├── frameworks                  # Framework layer
│   │   ├── napi                    # NAPI layer
│   │   └── native                  # Native layer
│   ├── interfaces                  # API layer
│   │   └── inner_api               # Internal APIs
│   ├── sa_profile                  # SA profile
│   ├── service                     # Server layer
│   │   ├── etc                     # Brightness configuration file
│   │   ├── native                  # Native layer
│   │   └── zidl                    # Zidl API layer
│   ├── test                        # Test cases
│   │   ├── fuzztest                # Fuzz test
│   │   ├── unittest                # Unit test
│   │   ├── systemtest              # System test
│   │   └── utils                   # Test tools
│   └── utils                       # Utilities
├───brightness_manager              # brightness manager 
```

## Repositories Involved<a name="section63151229062"></a>

[Power Management Subsystem](https://gitee.com/openharmony/docs/blob/master/en/readme/power-management.md)

[powermgr_power_manager](https://gitee.com/openharmony/powermgr_power_manager)

**powermgr_display_manager**

[powermgr_battery_manager](https://gitee.com/openharmony/powermgr_battery_manager)

[powermgr_thermal_manager](https://gitee.com/openharmony/powermgr_thermal_manager)

[powermgr_battery_statistics](https://gitee.com/openharmony/powermgr_battery_statistics)

[powermgr_battery_lite](https://gitee.com/openharmony/powermgr_battery_lite)

[powermgr_powermgr_lite](https://gitee.com/openharmony/powermgr_powermgr_lite)
