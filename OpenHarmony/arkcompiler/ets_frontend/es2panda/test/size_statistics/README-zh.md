# 简介

输出测试用例通过es2abc生成的abc文件中各数据结构size的占比统计报告。

# 运行脚本

## 使用说明

```sh
python3 size_statistics.py --es2abc-path ES2ABC_PATH [arguments]
```

### 参数

+ `--es2abc-path`: 可执行程序 es2abc 的路径。

### 可选参数

+ `--no-progress`: 不显示进度条。
+ `--timeout`: 用例执行的时间限制 (单位: 秒)。

# 测试结果

大小占比统计报告: `./size_percentage_report.html`
