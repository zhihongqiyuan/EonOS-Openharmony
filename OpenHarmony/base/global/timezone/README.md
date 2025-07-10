# Time Zone

## Introduction

The time zone module provides the functions of updating, parsing, compiling, and deploying time zone data.

## Directory Structure

The directory structure of the time zone module is as follows:

```
/base/global/
├── timezone           # Code repository of the time zone module
│   ├── data          # Time zone compilation data
│   ├── tool          # Time zone management tool
│   │   ├── compile_tool              # Time zone compilation tool
│   │   └── update_tool          # Time zone update tool
```

## Description

The time zone update tool searches for the latest version of time zone data from the [Internet Assigned Numbers Authority (IANA) database](https://data.iana.org/time-zones/releases/). If a new version of time zone data is available, the time zone update tool downloads the data to update the local time zone data. The updated time zone data is saved in the **./data/iana/** directory. The following is an example:

```
cd tool/update_tool // Go to the directory where the time zone update tool is located.
python3 download_iana.py  // Run the data update script.
```

The time zone compilation tool compiles the time zone source data and generates time zone binary data in **./data/prebuild/posix**. The following is an example:

```
cd tool/compile_tool // Go to the directory where the time zone compilation tool is located.
chmod 755 compile.sh
./compile.sh // Run the compilation script.
```

## Repositories Involved

[Globalization Subsystem](https://gitee.com/openharmony/docs/blob/master/en/readme/globalization.md)

**global\_timezone**

[global\_i18n\_standard](https://gitee.com/openharmony/global_i18n_standard/blob/master/README.md)

[global\_resmgr\_standard](https://gitee.com/openharmony/global_resmgr_standard/blob/master/README.md)
