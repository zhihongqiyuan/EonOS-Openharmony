# Description

## Purpose
This script will get the latest images and xts testcases from dailybuilds, then burn the images on the rk3568 demo board 
and run the xts testcase on the target to get the test result.

## Usage
### Running environment
This script will be running on windows, python3.7 or above needed.
### How to work
Double click the run.bat or in the cmd condition input the path of run.bat
### Add xts testcases
To add xts testcases, you need to add module names to the running_modules.txt file. Module names should be splited by ';'. The module names are given in xts system, please refer to [this link]{https://gitee.com/openharmony/xts_acts#%E5%85%A8%E9%87%8F%E7%94%A8%E4%BE%8B%E6%89%A7%E8%A1%8C%E6%8C%87%E5%AF%BC%E9%80%82%E7%94%A8%E4%BA%8E%E5%B0%8F%E5%9E%8B%E7%B3%BB%E7%BB%9F%E6%A0%87%E5%87%86%E7%B3%BB%E7%BB%9F}
### Note
- Only for the first time of running the script will download the burnning tool and need to install the driver for the tool
by yourself. After the pragram run for a while(downloading the tool), there comes the User Account Control interface.
Click 'yes' and you will see the driver installation interface as following:
<br />![](https://gitee.com/huyunhui1/images/raw/build/%E6%8D%95%E8%8E%B7.PNG)
<br />Click the "驱动卸载" button first and then click the "驱动安装" button, after that close the interface and the program
will go on
- Because the burning tool does not use the relative path as the the storage path of images, the script use the path 'D:\AutoXTSTest'
as the storage path to make it run stably.
- In rare cases, the rk3568 demo board could not start up normally after burnning, which leads to invalid hdc instructions.
At that time, this script will be blocked in next running, you should reboot the rk3568 demo borad to loader mode when next burning.