# SDK自动化测试套

## 脚本目的
1.下载当日的daily sdk
2.xts自动化测试，在auto_xts_test中
3.sdk自动化测试套，在sdk_test中
4.端到端构建性能测试，在performance_test中
5.获取仓库提交信息,在get_commit_log中
6.自动发送结果至指定邮箱

## 脚本使用
timer.py中，包含了下载sdk，xts， sdk_test，performance_test，获取每日提交信息脚本,以及发送邮件，如不需要可以去除。  
输入时间参数将会对任务进行定时,否则立即开始测试任务
email_config中包含了邮箱的设置  
### 脚本运行
使用命令python entry.py

### 注意事项
下载的SDK为24小时内的daily build，如果下载失败则会中断执行后续测试。  
不能关闭命令行，否则无法在指定时间触发。  
各项测试均可单独运行，详细配置请参考各自的readme。  