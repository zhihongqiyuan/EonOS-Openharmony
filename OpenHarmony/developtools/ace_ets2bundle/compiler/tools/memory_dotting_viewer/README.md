# ets_loader内存打点工具使用指导

## 概述

内存打点工具memory_dotting_viewer使用ets_loader编译过程中产生的中间数据做统计，对中间数做分析后，使用工具进行图表展示。

## 配置参数

  当前数据采集频率为100ms一次，通过developtools/ace_ets2bundle/compiler/main.js 中的 projectConfig.memoryDottingRecordInterval 配置项控制；
  当前数据落盘频率为1000ms一次，通过developtools/ace_ets2bundle/compiler/main.js 中的 projectConfig.memoryDottingWriteFileInterval 配置项控制；
  当数据量过大时，可以通过增加采集频率来减少数据量。

## 使用步骤

1. 开启内存打点开关，需要在developtools/ace_ets2bundle/compiler/main.js里将环境变量process.env.enableMemoryDotting置为true，默认为false。

2. Dev工具执行Build Hap(s)，会在projectname/entry/build/default/intermediates/loader_out/dottingfile生成:memory+时间戳.log格式的日志。

3. 执行memory_dotting_viewer目录下的memory_server.py，指定第2步生成日志命令如下，即可启动静态页面进行图标展示。

   ```shell
   python memory_server.py --file  xx.log
   ```

4. 图表说明：对日志的数据采用折线进行展示，X坐标为打点时间，Y坐标为内存占用量；内存占用量目前展示三种，进程占用的内存总量（RSS）、堆内存的总量（HeapTotal）、堆内存的使用量（HeapUsed），三种内存占用量分别采用不同颜色进行绘制，红色代表RSS、蓝色代表HeapTotal、绿色代表HeapUsed。折线里鼠标移动到拐点位置会采用悬浮框展示更详细的数据信息，信息包括：时间信息（X）、当前时间内存占用量（Y）、前文三种内存具体类型信息（Type）、阶段信息（stage）、父阶段信息（parentStage）。
