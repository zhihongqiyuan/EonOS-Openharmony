# es2abc-benchmark

## 项目说明
 本项目旨在对es2abc编译js时的性能进行测试并输出测试结果报告。  
 本项目依赖python3、git以及ets_frontend组件下的es2abc可执行文件。  
 其中测试用例默认存放在`test_cases`目录，运行结果存放在`output`目录。
## 用法说明
### 工具运行
#### 参数说明
|     参数      |                         描述                          |  类型  |                  默认值                  |
| :-----------: |:---------------------------------------------------:| :----: | :--------------------------------------: |
| --es2abc-tool |             这个参数是必需的，需要提供一个es2abc工具的路径。             | string |                   N/A                    |
|    --runs     |           这个参数是可选的，默认值为20。表示一次测试中执行用例的次数。           |  int   |                    20                    |
|  --opt-level  |               这个参数是可选的，它指定了编译器的优化级别。                | string |                    0                     |
| --parse-only  |   这个参数是可选的，默认值为False。如果设置为True，那么在测试期间，只会解析输入文件。    |  bool  |                  False                   |
| --engine-comp  | 这个参数是可选的，默认值为False，此时，只进行es2abc测试，为True时与Hermes引擎对比 |  bool  |                  False                   |
#### 测试用例说明
    本项目测试套件及测试用例来源于开源库:
##### 测试套件：
  | 名称 | 描述 | GitHub 地址 |
  | :--: | :--: | :--: |
  | Octane | JavaScript基准测试套件，用于评估浏览器在处理复杂JavaScript任务时的性能 | [https://github.com/chromium/octane](https://github.com/chromium/octane) |
  | Kraken | JavaScript性能测试套件，包含一系列的测试用例，涵盖了诸如动态场景、垃圾回收、加密和图形操作等方面 | [https://github.com/mozilla/krakenbenchmark.mozilla.org](https://github.com/mozilla/krakenbenchmark.mozilla.org) |
  | SunSpider | JavaScript性能测试套件，用于评估JavaScript引擎在执行一些常见任务时的性能 | [https://github.com/mozilla/krakenbenchmark.mozilla.org](https://github.com/mozilla/krakenbenchmark.mozilla.org) |

#### 对比引擎
    运行本benchmark时可进行es2abc编译性能与hermes编译性能对比,hermes引擎GitHub地址：https://github.com/facebook/hermes
#### 运行示例
脚本运行时依赖ets_frontend组件编译后的es2abc可执行文件
es2abc编译命令：

```
$ ./build.sh --product-name rk3568 --build-target ets_frontend_build
```
指定es2abc可执行文件,运行`run_benchmark.py`进行测试:
```
$ python3 run_benchmark.py --es2abc-tool  /out/xx/xx/xx/ets_frontend/es2abc
```
与hermes引擎进行对比：
```
$ python3 run_benchmark.py --es2abc-tool  /out/xx/xx/xx/ets_frontend/es2abc --engine-comp
```
指定测试用例编译轮数(默认为20轮):
```
$ python3 run_benchmark.py --es2abc-tool  /out/xx/xx/xx/ets_frontend/es2abc --runs 10 
```
--parse-only 只对测试用例做解析动作，使用参数 --engine-comp时，本参数失效:
```
$ python3 run_benchmark.py --es2abc-tool  /out/xx/xx/xx/ets_frontend/es2abc --parse-only
```
--opt-level 指定编译优化等级，使用参数 --engine-comp时，本参数失效:
```
$ python3 run_benchmark.py --es2abc-tool  /out/xx/xx/xx/ets_frontend/es2abc --opt-level '2'
```
生成的测试结果存放至 `output` 目录，命令行打印基本的测试信息，如:

```plain
> > > benchmark running
Running navier-stokes....
Running typescript....
Running mandreel....
```
### 注意事项
    本工具依赖ets_frontend组件编译产物es2abc可执行文件,使用时需指定es2abc路径。
    本工具测试用例为开源用例,测试套件在运行时自动拉取。

