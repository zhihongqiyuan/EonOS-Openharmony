# CalendarData

## Introduction

CalendarData is a preset application in OpenHarmony. It allows users to add, delete, modify, and query calendars.

## Directory Structure
```
├─AppScope
│  │  
│  └─resources                       # Resource files
├─common
│  │  
│  └─src
│      └─main  
│          └─ets                     # Common methods           
├─datamanager 
│  └─src
│      └─main
│          └─ets                     # Data manager                    
├─dataprovider
│  └─src
│      └─main
│          └─ets                     # Data manager proxy        
├─datastructure 
│  └─src
│      └─main 
│          └─ets                     # Data structure              
├─entry
│  └─src
│      └─main                        # Application entry         
├─rrule
│  └─src
│      └─main
│          └─ets                     # Recurrence rule         
└─signature                          # Certificate file
```

### Architecture

![](./figures/architecture.png)

As a built-in basic application, Calendar implements basic user operations such as querying daily and monthly views and creating and querying calendars. The Calendar APIs provide SQL-free, object-based data read and write capabilities. CalendarData manages calendars, reminders, and data.

## How to Use

### Building based on IDE

Open a project in DevEco Studio and choose **Build > Build Haps(s)/APP(s) > Build Hap(s)**.

![](./figures/build_haps.png)

After the build is complete, a HAP file is generated in **\build\outputs** in the project directory. (If no signature is configured, an unsigned HAP file is generated.)

![](./figures/build_output_dir_release.png)

Run the **hdc_std install "*hap package address*"** command to install the compiled HAP file.

![](./figures/calendar_install.png)

### Building based on OpenHarmony version

In the root directory of the OpenHarmony source code, run the following command to build the CalendarData separately:

```
./build.sh --product-name rk3568 --ccache --build-target calendar_data
```

> **NOTE**
>
> --**product-name** specifies the product name, for example, **rk3568**.
>
> --**ccache** specifies the cache function used during the compilation.
>
> --**build-target** specifies the component to build.

## Constraints
- Development environments
   - DevEco Studio for OpenHarmony: version later than 3.1.1.101
   
   - OpenHarmony SDK: API version 10
   
   For details about the initial DevEco Studio configuration, see the DevEco Studio usage document.
   
- Programming language
   
   ArkTS
   
- Limitations
   
   This application can only be run on standard-system devices.
