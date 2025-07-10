# SDK测试套说明
本测试套可以执行端到端的SDK测试验证。
验证方式有两种：
1) 验证编译打包的产物中，abc和sourcemap是否生成。
2) 验证编译产物的应用是否可以正常运行（该功能在开发中）。

## SDK测试套使用
### 运行环境
SDK测试自动化脚本运行环境为windows，python3.9及以上。
### 测试准备
1. 确保环境中已安装Deveco
2. 安装测试套依赖：
`python3 -m pip install pyyaml validators requests httpx tqdm json5 pandas pillow`
3. 修改配置文件config.yaml，配置Deveco和测试应用的相关参数。各项配置说明详见该文件。
4. 如需新增测试应用，可修改配置文件config.yaml，在haps字段中增加配置。可参考配置中的已有应用进行配置。

### 测试运行
测试套支持daily运行和手动单次运行。
运行测试功能需要连接rk板进行测试，可通过参数runHap控制是否执行该测试
#### daily运行
daily运行将从主干分支下载当日构建的sdk，使用该sdk进行全量的测试项验证：
执行命令：
`python entry.py`
#### 手动运行
执行命令：
`python run.py`
不带参数默认跑全量的测试项。可选参数可通过`--help`查看。