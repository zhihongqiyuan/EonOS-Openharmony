# 简介

生成 ES2ABC 和 V8 生成的字节码指令数量差异报告。

# 准备V8

## 1. 安装 depot_tools


```sh
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH=/path/to/depot_tools:$PATH
```


## 2. fetch v8 代码

在任意目录下创建一个 V8 源代码目录并切换到该目录：

```sh
mkdir ~/v8
cd ~/v8
```

使用 fetch 命令获取 V8 源代码及其所有分支和依赖项。

```sh
fetch v8
cd v8
```


## 3. 安装依赖项

在 V8 源代码目录下执行以下命令，提取最新更改并安装任何新生成的依赖项。

```sh
git pull
gclient sync
```

接下来，安装其它依赖。

```sh
./build/install-build-deps.sh
```


## 4. 编译 V8

使用 gm 工具编译构建V8。

```sh
tools/dev/gm.py x64.release
```

编译好的v8可执行程序d8的路径如下

```sh
out/x64.release/d8
```


# 运行脚本

## 使用说明

```sh
python3 parse_command.py -es2abc ES2ABC_PATH -v8 D8_PATH [arguments]
```

### 参数

+ `-es2abc`: 可执行程序 es2abc 的路径。
+ `-v8`: V8可执行程序 d8 的路径。

### 可选参数

+ `--add_case`: 添加单个待执行测试用例的文件路径。
+ `--add_case_dir`: 添加待执行测试用例所在的目录，脚本将自动搜索该目录中的所有测试用例 (默认筛选.js后缀的测试用例)。
+ `--timeout`: 用例执行的时间限制 (单位: 秒)。


# 测试结果

指令数差异报告: `./discrepancy_report.html`
