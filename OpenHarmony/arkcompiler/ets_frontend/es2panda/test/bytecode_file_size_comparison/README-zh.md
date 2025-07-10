# 环境准备

## JDK

这里以 openjdk11 为例子进行安装。

```sh
sudo apt update
sudo apt install openjdk-11-jdk
```

安装完成可以通过下面指令检查:

```sh
javac --version
```

输出内容如下

```sh
javac 11.0.21
```

## d8

d8工具作为独立工具被纳入了 Android Studio 构建工具 28.0.1 及更高版本中。因此通过下载安装 Android Studio 工具就可以获取到d8工具。

1. 下载 Android Studio:

```sh
wget https://redirector.gvt1.com/edgedl/android/studio/ide-zips/2023.2.1.24/android-studio-2023.2.1.24-linux.tar.gz
```

2. 解压

```sh
tar -xzvf android-studio-2023.1.1.28-linux.tar.gz
```

3. 启动 Android Studio

```sh
cd android-studio/bin
./studio.sh
```

4. 配置国内镜像源 (如果此时提示无法下载 Android SDK)

+ 进入到 Android Studio 主页面，点击下方的 `More Actions` -> `SDK Manager`。
+ 找到左侧的: `Appearance & Behavior` -> `System Settings` -> `HTTP Proxy`。
+ 选择 `Auto-detect proxy settings`，并勾选 `Automatic proxy configuration URL`。在后面的输入框中填写镜像源地址，镜像源可选URL如下:

```sh
https://mirrors.aliyun.com/android.googlesource.com/

https://mirrors.cloud.tencent.com/AndroidSDK/
```

随后重新启动 Android Studio 即可顺利下载到 Android SDK。

Android SDK路径: `~/Android/Sdk/build-tools/`。d8可执行程序就在该目录的版本号目录下面。

# 简介

输出 js / ts / java 测试用例分别通过 es2abc / es2abc / javac和d8 编译生成字节码文件大小的对比报告。

# 运行脚本

## 使用说明

```sh
python3 size_compare.py --es2abc-path ES2ABC_PATH --javac-path JAVAC_PATH [arguments]
```

### 参数

+ `--es2abc-path`: 可执行程序 es2abc 的路径。
+ `--javac-path`: 可执行程序 javac 的路径。

### 可选参数

+ `--d8-path`: 可执行程序 d8 的路径。
+ `--timeout`: 用例执行的时间限制 (单位: 秒)。

# 测试结果

字节码文件大小对比报告: `./size_comparison_report.html`
