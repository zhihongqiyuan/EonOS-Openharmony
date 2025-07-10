#-*- coding:utf-8 -*-
import uuid
import sys
import subprocess
import os
import serial
import time
import re

from core.base import BaseApp, dec_stepmsg
from util.file_locker import FileLock
from util.log_info import logger
from util.time_info import get_now_time_str_info, get_now_time_info, Timeout, timeout
from aw.Telnet.TelnetClient import TelConnect
from aw.Common.Constant import CONSTANT
from aw.Download.Download import *
from aw.Common.Common import getHostIp, copyFile, copyDirectory
from aw.ExtractFile.ExtractFile import *
from aw.poweronoff.serial_power_on_off import usbPowerOnOff
from threading import Thread

lock_suffix = CONSTANT.File.LOCK_SUFFIX  #通过文件锁实现并发下载
suc_file = CONSTANT.File.SUC_FILE        #通过本文件是区分版本是否成功下载
failed_file = CONSTANT.File.FAILED_FILE  #通过本文件是标记文件下载失败
READ_MAXTIMEOUT = 5
READ_TIMEOUT = 5
READ_MINITIMEOUT = 2
uboot_finish = 'hisilicon #'
cmd_finish = ' #'
error_str_list = ['Unknown', '发送起始帧失败', '发送头帧失败']
ip_cmd = 'ping 192.168.18.1'

class liteOsUpgrade_linux(BaseApp):
    '''
    @author: cwx1076044
    '''

    def __init__(self, param_file):
        super().__init__(param_file)
        self.param_List = ["deploy_com",
                        "usb_port",
                           "upgrade_upgradeLocation"]

    @dec_stepmsg("linux L1 flash")
    def excute(self):
        '''
        #===================================================================================
        #   @Method:        excute(self)
        #   @Precondition:  none
        #   @Func:          升级执行入口
        #   @PostStatus:    none
        #   @eg:            excute()
        #   @return:        True or Flase
        #===================================================================================
        '''
        step_index = self.params_dict.get("step_list").index("liteOsUpgrade_linux_app")
       
        # 执行下载
        try:
            if not self.download():
                CONSTANT.ENVERRMESSAGE = "image download fail"
                logger.printLog(CONSTANT.ENVERRMESSAGE)
                return False
        except Exception as e:
            raise e
        

        # 执行升级
        try:
            if not self.upgrade():
                CONSTANT.ENVERRMESSAGE = "board upgrade fail"
                logger.printLog(CONSTANT.ENVERRMESSAGE)
                return False
            return True
        except Exception as e:
            raise e

    @dec_stepmsg("download")
    @timeout(1800)
    def download(self):
        '''
        #===================================================================================
        #   @Method:        download(self)
        #   @Precondition:  none
        #   @Func:          构建下载到本地的路径，执行相应包的下载
        #   @PostStatus:    none
        #   @eg:            download()
        #   @return:        True or Flase
        #===================================================================================
        '''
        global version_savepath, version_name
        dir_path = CONSTANT.Path.getDirPath()
        if self.params_dict.get("pbiid"):
            version_path = self.params_dict.get("pbiid")
            version_name = str(uuid.uuid5(uuid.NAMESPACE_URL, str(self.params_dict.get("pbiid")) + "FASTBOOT"))
            version_savepath = os.path.join(dir_path, self.params_dict.get("flash_type"), version_name)
        else:
            version_path = self.params_dict.get("upgrade_upgradeLocation")
            version_name = str(uuid.uuid5(uuid.NAMESPACE_URL, (self.params_dict.get("upgrade_upgradeLocation"))))
            version_savepath = os.path.join(dir_path, version_name)

        if self.params_dict.get("isDownload") == "True":
            logger.printLog("不需要做下载，直接返回")
            return True

        #执行img下载
        import hashlib
        save_file_str = version_path.replace("/", "").replace("\\", "")
        save_file_name = hashlib.sha1(save_file_str.encode("utf-8")).hexdigest()
        logger.info("download hash value:%s" % (save_file_name))
        save_path_file = os.path.join(dir_path, "record", "%s%s" % (save_file_name, ".txt"))
        if not self.excutedown(version_path, os.path.join(version_savepath, "img"), save_path_file, False):
            logger.error("download img fail")
            return False

        #保存本地版本路径给devicetest去版本路径下取用例
        saveVersion(save_path_file, os.path.join(version_savepath, "img"))

        return True

    def excutedown(self, source_path, download_dir, suc_mark, is_file):
        '''
        #===================================================================================
        #   @Method:        excutedown(source_path, download_dir, is_file)
        #   @Precondition:  none
        #   @Func:          执行下载动作
        #   @PostStatus:    none
        #   @Param:         source_path：资源文件路径
        #                   download_dir:文件下载到本地的文件夹路径
        #                   is_file:是否是文件
        # 
        #   @eg:            excutedown("xxxx", "D:\\local\\image", Flase， os_method)
        #   @return:        True or Flase
        #===================================================================================
        '''
        failed_mark = os.path.join(download_dir, failed_file)
        lock_path = os.path.join(download_dir, lock_suffix)
        file_lock = FileLock()

        if isDownLoadSuccess(download_dir, suc_mark, failed_mark):
            return True
        try:
            nowtime = get_now_time_str_info()
            logger.printLog("%s Downloading, please wait" % nowtime)
            file_lock.lockFile(lock_path)
            ret = ""
            logger.info("Get lock. Start to ")
            if self.params_dict.get("bt_enable") and self.params_dict.get("bt_enable") == "True":
                ret = downloadByBitComet(source_path, download_dir, os_method)
            elif source_path.startswith('\\\\'):
                ret = downloadByCopy(source_path, download_dir, is_file)
            elif self.params_dict.get("pbiid"):
                ret = downlaodByDownloadTool(version_savepath, self.params_dict.get("version_type"), "FASTBOOT", self.params_dict.get("pbiid"))
            elif source_path.startswith("http"):
                ret = downloadFileFromDevCloud(source_path, "", "", download_dir)

            if source_path.endswith(".zip"):
                zip_name = os.path.basename(source_path)
                ret = extractZipFile(os.path.join(download_dir, zip_name), download_dir)
            if source_path.endswith(".tar.gz") or (source_path.startswith("http") and ("file_id=" in source_path)):
                if source_path.startswith("http") and ("file_id=" in source_path):
                    if source_path.endswith(".tar.gz"):
                        zip_name = source_path.split('=')[-1]
                    else:
                        zip_name = "out.tar.gz"
                else:
                    zip_name = os.path.basename(source_path)
                ret = unTarFile(os.path.join(download_dir, zip_name), download_dir)
            nowtime = get_now_time_str_info()
            logger.printLog("%s download to %s end" % (nowtime, download_dir))

            if not ret:
                with open(failed_mark, "a+") as fp:
                    fp.write("")
            return ret
        except Exception as e:
            logger.printLog(e)
            raise Exception(e)
        finally:
            file_lock.releaseFile()


    @dec_stepmsg("upgrade")
    #@timeout(900)
    def upgrade(self):
        '''
        #===================================================================================
        #   @Method:        upgrade(self)
        #   @Precondition:  none
        #   @Func:          升级相关业务逻辑
        #   @PostStatus:    none
        #   @eg:            upgrade()
        #   @return:        True or Flase
        #===================================================================================
        '''
        logger.printLog('开始升级')
        deploy_com = self.params_dict.get("deploy_com")
        usb_port = self.params_dict.get("usb_port")
        baudrate = self.params_dict.get("baudrate")
        #芯片类型,根据芯片类型获取对应的刷机命令
        flash_type = self.params_dict.get("flash_type")
        burn_usbport = self.params_dict.get("hiburn_usbport")
        device_ip = self.params_dict.get("Device_IP")
        device_netmask = self.params_dict.get("Device_Netmask")
        device_gatewayip = self.params_dict.get("Device_GatewayIP")
        chip_version = self.params_dict.get('chip_version')
        chip_version = chip_version.lower() if chip_version else chip_version
        
        if not deploy_com:
            logger.error("deploy_com is NULL !!")
            return False
        if not burn_usbport:
            logger.error("hiburn_usbport is NULL !!")
            return False
        if not baudrate:
            baudrate = 115200
        scriptpath = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(os.path.dirname(__file__)))))
        #升级需要的工具归档
        toolworkspace = CONSTANT.OSType.getworkspace()
        hiburntoolpath = os.path.join(toolworkspace, "HiBurnCmdLine", "usb%s_tool" % burn_usbport)
        logger.info("hiburn tool path is: %s" % hiburntoolpath)
        if not os.path.exists(hiburntoolpath):
            if not burn_usbport:
                logger.error("hiburn_usbport is NULL !!")
                return False
            os.makedirs(hiburntoolpath)
            toolpath = os.path.join(scriptpath, "resource", "HiBurnCmdLine.zip")
            logger.info("copy %s to %s" % (toolpath, hiburntoolpath))
            copyFile(toolpath, hiburntoolpath)
            zip_name = os.path.basename(toolpath)
            ret = extractZipFile(os.path.join(hiburntoolpath, zip_name), hiburntoolpath)
            if ret:
                logger.info("unzip to %s succ" % (hiburntoolpath))
                #修改burn.config中的usb口
                configpath = os.path.join(hiburntoolpath, "config", "burn.config")
                all_data = ""
                with open(configpath, "r", encoding="utf-8") as cf:
                    for line in cf:
                        if "usbDeviceNumber=" in line:
                            old_str = line
                            line = line.replace(old_str, "usbDeviceNumber=%s\r\n" % burn_usbport)
                            logger.info("replace line: %s " % line)
                        all_data += line
                with open(configpath, "w", encoding="utf-8") as wf:
                    wf.write(all_data)
            else:
                logger.error("%s is not exit" % hiburntoolpath)
                return False
        #将升级需要的文件拷贝到镜像里面
        local_image_path = os.path.join(version_savepath, "img")
        old_xml_path = os.path.join(scriptpath, "resource", "L1", flash_type, "usb-burn.xml")
        xml_path = os.path.join(local_image_path, "usb-burn.xml")
        copyFile(old_xml_path, xml_path)
        if flash_type.lower() == "ev300":
            chip_type = "Hi3518EV300"
            ubootpath = os.path.join(scriptpath, "resource", "L1", flash_type.lower(), "u-boot-hi3518ev300.bin")
        elif flash_type.lower() == "dv300_linux":
            chip_type = "Hi3516DV300"
            ubootpath = os.path.join(scriptpath, "resource", "L1", flash_type.lower(), "u-boot-hi3516dv300.bin")
        else:
            logger.error("flash_type is : %s " % flash_type)
            return False
        copyFile(ubootpath, local_image_path)
        scriptfile = os.path.join(scriptpath, "resource", "L1", f'{flash_type.lower()}', "update.txt")
        logger.info(f'scriptfile:{scriptfile}')

        current_path = os.getcwd()
        logger.info("before excute hiburn,current path is: %s" % current_path)
        os.chdir(hiburntoolpath)
        logger.info("excute hiburn path is: %s" % os.getcwd())
        #擦除fastboot
        logger.printLog('erase fastboot')
        flash_uboot_xml = os.path.join(scriptpath, "resource", "L1", flash_type.lower(), "flash_fastboot.xml")
        cmd = ".\jre\\bin\java -jar hiburn.jar --erase -n %s -m serial %s -x %s" % (chip_type, deploy_com.upper(), flash_uboot_xml)
        self.eraseDevice(cmd, usb_port)

        retry = 0
        while retry < 3:
            #usb刷机
            cmd = ".\jre\\bin\java -jar hiburn.jar --burn -n %s -m USBBootrom -x %s" % (chip_type, xml_path)
            logger.info("cmd is: %s" % cmd)
            ret, outpri = subprocess.getstatusoutput(cmd)
            logger.info("usb upgrade result: %s " % ret)
            logger.info("print console: %s " % outpri)
            if ret != 0:
                if ret == 4 and retry < 2:
                    time.sleep(10)
                    retry = retry + 1
                    logger.info('flash fail,so flash once again')
                    continue
                logger.info(ret)
                logger.error("hiburn usb upgrade failed!!")
                return False
            retry = retry + 3
        os.chdir(current_path)
        logger.info("hiburn upgrade end, check board status")
        time.sleep(10)
        try:
            logger.info("打开serial")
            ser = serial.Serial(deploy_com, int(baudrate), timeout=0)
            if self.bootUpUboot(ser,scriptfile) and self.configureIp(ser, flash_type, device_ip, device_netmask, device_gatewayip) and self.checkStatus(ser):
                return True
            return False
        except Exception as e:
            logger.info(e)
            return False
        finally:
            ser.close()
            logger.info("close serial")

    def bootUpUboot(self,ser,scriptfile):
        '''
        启动uboot
        '''
        reset_count = 0
        while reset_count < 2:
            if ser.is_open == False:
                ser.open()
            logger.info("get device status")
            board_type = getBoardType(ser)
            if board_type == "uboot":
                with open(scriptfile, "r") as fp:
                    lines = fp.readlines()
                for line in lines:
                    if not line:
                        logger.info("cmd is: %s " % line)
                        continue
                    if "reset" in line:
                        ret = sendCmd(ser, line, READ_MAXTIMEOUT)
                        continue
                    ret = sendCmd(ser, line, READ_MINITIMEOUT)
                board_type = getBoardType(ser)
                if board_type != "OHOS":
                    if  reset_count < 2:
                        logger.info('after reset;the device status is error，reset device again,reset_count:%d' % reset_count)
                        reset_count += 1
                        time.sleep(20)
                        continue
                    logger.error("upgrade fail")
                    return False
                return True
            else:
                logger.info('before reset;the device status is error，reset device again,reset_count:%d'%reset_count)
                reset_count += 1
                time.sleep(20)
                continue
        else:
            return False

    def configureIp(self,ser,flash_type, device_ip, device_netmask, device_gatewayip):
        '''
        配置ip，确认配置情况
        '''
        rerty_count = 0
        # test_count = 0
        while rerty_count <= 2:
            if flash_type.lower() == "dv300_linux":
                if not self.afterRebootDvConfigure(ser, device_ip, device_netmask, device_gatewayip):
                    return False
            elif flash_type.lower() == "ev300":
                if not self.afterRebootEvConfigure(ser):
                    return False
            time.sleep(5)

            ret = sendCmd(ser, ip_cmd, READ_MINITIMEOUT)
            logger.info(ret)
            # if test_count ==0 :
            #     logger.printLog('ip 配置失败')
            #     test_count = 1
            #     continue 
            if 'Reply from 192.168.18.1' in ret:
                logger.info('ip 配置成功')
                return True
            elif 'Ping: sending ICMP echo request failed' in ret:
                logger.printLog('ip 配置失败')
                logger.info('重新配置ip')
                rerty_count += 1
            else:
                return True
    
    def afterRebootDvConfigure(self,ser, device_ip, device_netmask, device_gatewayip):
        logger.info("dv300_linux configure ip")
        init_cmd = "ifconfig eth0 %s netmask %s gateway %s \r" % (device_ip, device_netmask, device_gatewayip)
        sendCmd(ser, init_cmd, READ_MINITIMEOUT)
        sendCmd(ser, 'ifconfig\r', READ_MINITIMEOUT)
        return True

    def afterRebootEvConfigure(self,ser):
        logger.info("ev300 configuring ,setup wifi")
        cmd = 'ls\r'
        ret = sendCmd(ser, cmd, READ_MINITIMEOUT)
        if not "sdcard" in ret.lower():
            cmd = 'mkdir /sdcard\r'
            ret = sendCmd(ser, cmd, READ_MINITIMEOUT)
            if "error:" in ret.lower():
                logger.error("mkdir /sdcard fail")
                return False
        cmd = 'mount /dev/mmcblk0p0 /sdcard vfat\r'
        ret = sendCmd(ser, cmd, READ_MINITIMEOUT)
        cmd = 'ls /sdcard\r'
        ret = sendCmd(ser, cmd, READ_MINITIMEOUT)
        cmd = 'cd /sdcard/wpa\r'
        ret = sendCmd(ser, cmd, READ_MINITIMEOUT)
        cmd = 'exec wpa_supplicant -i wlan0 -c wpa_supplicant.conf \r'
        ret = sendCmd(ser, cmd, READ_MAXTIMEOUT)
        if "error:" in ret.lower():
            logger.error("setup wifi fail")
            return False
        cmd = 'ifconfig\r'
        ret = sendCmd(ser, cmd, READ_MINITIMEOUT)
        if "error:" in ret.lower():
            logger.error("ifconfig fail")
            return False
        return True

    def eraseDevice(self, cmd, usb_port):
        '''
        ret 暂时没有作用，先使用out
        '''
        erase_retry = 0
        while erase_retry < 3:
            # 通电
            PowerOnByThread(usb_port)
            logger.info("cmd is: %s" % cmd)
            ret, outpri = subprocess.getstatusoutput(cmd)
            logger.info("flash fastboot result: %s " % ret)
            logger.info("print console: %s " % outpri)
            is_earse_again = any([True if item in outpri else False for item in error_str_list])
            if ret == 0 and erase_retry < 2 and not is_earse_again :
                logger.info('檫除成功'.center(20,'*'))
                break
            elif is_earse_again:
                logger.info('檫除存在问题 重新上下电 重新檫除')
                erase_retry += 1
                time.sleep(5)
                continue
            else:
                logger.info('other error')
                return False
        else:
            return False
    
    def checkStatus(self, ser):
        times = 0
        while times < 3:
            ret1 = sendCmd(ser, 'ps -elf', READ_TIMEOUT)
            list_ret = re.findall('appspawn', ret1)
            logger.info(list_ret)
            number = list_ret.count('appspawn')
            logger.info(number)
            if number >= 2:
                return True
            else:
                times += 1
                time.sleep(3)
        logger.info("No two appspawn processes, please check the device!")
        return False

def PowerOnByThread(usb_port,wait_time=10):
    thread = Thread(target=boardPowerOn, args=[usb_port, wait_time])
    thread.start()
    logger.info("thread board power on start")
    

def boardPowerOn(usb_port, waittime):
    logger.info("board power on start")
    time.sleep(waittime)
    
    #对端口下电
    if not usbPowerOnOff('127.0.0.1', '7788', usb_port, "off"):
        logger.error("board power off failed")
        return False
    
    #对端口上电
    if not usbPowerOnOff('127.0.0.1', '7788', usb_port, "on"):
        logger.error("board power on failed")
        return False
    logger.info("board power on end")


def getBoardType(ser):
    ret = sendCmd(ser, '\r', READ_TIMEOUT)
    # if 'HMOS' in ret or 'OHOS' in ret or '#' in ret:
    #     ostype = 'OHOS'
    # elif 'hisilicon' in ret:
    #     ostype = 'uboot'
    if 'hisilicon' in ret:
        ostype = 'uboot'  
    elif 'HMOS' in ret or 'OHOS' in ret or '#' in ret:
        ostype = 'OHOS'      
    elif ' #' in ret:
        ostype = 'linux'
    else:
        ostype = 'bootrom'
    logger.info("board type is: %s" % ostype)
    return ostype

def sendCmd(ser, cmd, timeout):
    logger.info("cmd is: %s " % cmd)
    ser.write((cmd + '\n').encode())
    time.sleep(5)
    ret = ''
    i = 0
    while True:
        out = ser.read(ser.inWaiting())
        if not out:
            break
        if i > 12:
            break
        ret = ret + out.decode(encoding="utf-8", errors="ignore")
        time.sleep(timeout)
        i = i + 1
    logger.info("result is: %s " % ret)
    return ret



if __name__ == "__main__":
    param_file = sys.argv[1]
    if not param_file:
        logger.printLog("Missing params file")
        sys.exit(-1)
    try:
        uphandle = liteOsUpgrade_linux(param_file)
        uphandle._excuteApp()
    except Exception as e:
        logger.printLog(e)
        sys.exit(-1)

