# 二进制文件编译构建指导

## 简介

指导 data/prebuild/posix 目录下的二进制文件和 data/prebuild/tool/linux/zic 二进制文件的编译构建。其中，前者为iana二进制时区数据，后者为iana二进制时区工具。

## 构建过程

1. 下载数据<br>
    在data目录下创建iana文件夹，将IANA tzdata 和 tzcode 数据下载至 data/iana 路径。具体可以使用位于tool/update_tool路径下的download_iana.py脚本从IANA官网下载最新版本数据<br>
    ```
    cd data/tool
    python download_iana.py
    ```
    或者可以从IANA官网（https://data.iana.org/time-zones/releases/ ）手动下载tzdataxxxx.tar.gz和tzcodexxxx.tar.gz两个压缩包，解压至data/iana目录下。
2. 执行编译脚本<br>
    使用 tool/compile_tool 路径下的compile.sh脚本对时区数据进行编译。
    ```
    cd tool/compile_tool
    chmod 755 compile.sh
    ./compile.sh
    ```
    该编译脚本会分别在 data/prebuild/posix目录和 data/prebuild/tool/linux目录下生成相应的二进制文件。