# XTS测试自动化脚本说明

## 脚本目的
XTS测试自动化脚本会自动从每日构建dailybuilds上获取最新dayu200镜像和相关xts用例，烧录镜像到rk3568开发板上并执行相关用例获取测试结果

## 脚本使用
### 运行环境
XTS测试自动化脚本运行环境为windows，python3.7及以上
### 脚本运行
点击run.bat运行或在cmd中输入run.bat所在路径
### 添加xts用例
在running_modules.txt文件中添加xts用例模块名，模块名之间用';'分割。xts用例模块参考[xts仓说明](https://gitee.com/openharmony/xts_acts#%E5%85%A8%E9%87%8F%E7%94%A8%E4%BE%8B%E6%89%A7%E8%A1%8C%E6%8C%87%E5%AF%BC%E9%80%82%E7%94%A8%E4%BA%8E%E5%B0%8F%E5%9E%8B%E7%B3%BB%E7%BB%9F%E6%A0%87%E5%87%86%E7%B3%BB%E7%BB%9F)
### 注意事项
- 初次使用会下载镜像烧录工具并需要手动安装驱动，在程序运行一小段时间（下载烧录工具）之后会跳出用户账户控制界面，点击“确认”后进入驱动安装界面，如下：
<br />![](https://gitee.com/huyunhui1/images/raw/build/%E6%8D%95%E8%8E%B7.PNG)
<br />先后点击“驱动卸载”和“驱动安装”即可，关闭界面程序继续运行。
- 由于镜像烧录工具运行时没有使用相对路径作为配置，因此此脚本使用了D:\AutoXTSTest目录作为镜像存放目录以保证运行稳定性。
- 部分情况下，镜像烧录后开发板开机异常，与PC设备连接断掉，会导致hdc命令失效，脚本运行卡住，此时需要在镜像烧录时手动进入loader模式