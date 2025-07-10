#-*- coding:utf-8 -*-
import uuid
import sys
import subprocess
import os
import serial
import datetime

from core.base import BaseApp, dec_stepmsg
from util.file_locker import FileLock
from util.log_info import logger
from util.time_info import get_now_time_str_info, get_now_time_info, Timeout, timeout
from aw.Telnet.TelnetClient import TelConnect
from aw.Common.Constant import CONSTANT
from aw.Download.Download import *
from aw.Common.Common import getHostIp, copyFile, copyDirectory
from aw.ExtractFile.ExtractFile import *
from aw.poweronoff.serial_power_on_off import usbPowerOnOff, usbPowerOnOffV2
from threading import Thread
from subprocess import getstatusoutput

lock_suffix = CONSTANT.File.LOCK_SUFFIX  #通过文件锁实现并发下载
suc_file = CONSTANT.File.SUC_FILE        #通过本文件是区分版本是否成功下载
failed_file = CONSTANT.File.FAILED_FILE  #通过本文件是标记文件下载失败
READ_MAXTIMEOUT = 5
READ_TIMEOUT = 5
READ_MINITIMEOUT = 2
uboot_finish = 'hisilicon #'
cmd_finish = ' #'
error_str_list = ['Unknown', '发送起始帧失败', '发送头帧失败']

_is_download_success = False


class liteOsUpgrade_L2(BaseApp):

    def __init__(self, param_file):
        super().__init__(param_file)
        self.param_List = ["deploy_com",
                        "usb_port",
                           "upgrade_upgradeLocation"]

    @dec_stepmsg("hongmeng L2 flash")
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
        step_index = self.params_dict.get("step_list").index("liteOsUpgrade_L2_app")
       
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
    @timeout(18000)
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
        global version_savepath, version_name, _is_download_success
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
        logger.printLog('save_path_file: %s' % save_path_file)
        if not self.excutedown(version_path, os.path.join(version_savepath, "img"), save_path_file, False):
            logger.error("download img fail")
            return False
        _is_download_success = True
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
                logger.printLog(f'tar_file:{os.path.join(download_dir, zip_name)},dest_file:{download_dir}')
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
        global _is_download_success
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
        sn = self.params_dict.get('sn')
        
        if not deploy_com:
            logger.error("deploy_com is NULL !!")
            return False
        if not burn_usbport:
            logger.error("hiburn_usbport is NULL !!")
            return False
        if not baudrate:
            baudrate = 115200
        #执行下载
        t_download = Thread(target=self.download, args=())
        t_download.start()
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
        os.chdir(hiburntoolpath)
        if flash_type.lower() == "ev300":
            chip_type = "Hi3518EV300"
        elif flash_type.lower() == "dv300":
            chip_type = "Hi3516DV300"
        else:
            logger.error("flash_type is : %s " % flash_type)
            return False
        #擦除fastboot
        logger.printLog('erase fastboot')
        flash_uboot_xml = os.path.join(scriptpath, "resource", "L2", flash_type.lower(), "flash_fastboot.xml")
        cmd = ".\jre\\bin\java -jar hiburn.jar --erase -n %s -m serial %s -x %s" % (chip_type, deploy_com.upper(), flash_uboot_xml)
        self.eraseDevice(cmd, usb_port)
        t_download.join()
        # 检查下载是否成功
        if not _is_download_success:
            return False
        
        #将升级需要的文件拷贝到镜像里面
        local_image_path = os.path.join(version_savepath, "img")
        old_xml_path = os.path.join(scriptpath, "resource", "L2", flash_type, "Hi3516DV300-emmc.xml")
        xml_path = os.path.join(local_image_path, "Hi3516DV300-emmc.xml")
        copyFile(old_xml_path, xml_path)
        # copyFile(ubootpath, local_image_path)
        scriptfile = os.path.join(scriptpath, "resource", "L2", f'{flash_type.lower()}', "update.txt")
        logger.info(f'scriptfile:{scriptfile}')

        logger.printLog('进行烧写')
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
        logger.info("为保持hdc稳定性，等待60秒")
        time.sleep(60)
        try:
            #pass
            self.hdc_set_time(sn)
        except Exception as e:
            logger.printLog('hdc设置时间超时')
            logger.printLog(str(e))
        try:
            logger.info("打开serial")
            ser = serial.Serial(deploy_com, int(baudrate), timeout=0)
            if ser.is_open == False:
                ser.open()
            logger.info("get device status")
            board_type = getBoardType(ser)
            if board_type == "OHOS" and self.checkStatus(ser):
                return True
            
                # ret = sendCmd(ser, 'sa;reset;', READ_MAXTIMEOUT)
                # board_type = getBoardType(ser)
                # if board_type != "OHOS":
                #     logger.error("upgrade fail")
                #     return False
            else:
                return False
        except Exception as e:
            logger.info(str(e))
            return False
        finally:
            ser.close()
            logger.info("close serial")

    def checkStatus(self, ser):
        time.sleep(180)
        # print('wait 120 seconds')
        ren_cmd = 'ps -elf | grep render_service'
        laun_cmd = 'ps -elf | grep com.ohos.launcher'
        time.sleep(10)
        ren_status_1 = getPsPid(ser, ren_cmd, 'render_service')
        time.sleep(5)
        ren_status_2 = getPsPid(ser, ren_cmd, 'render_service')
        time.sleep(5)
        laun_status_1 = getPsPid(ser, laun_cmd, 'com.ohos.launcher')
        time.sleep(5)
        laun_status_2 = getPsPid(ser, laun_cmd, 'com.ohos.launcher')
        # wes_cmd = 'pidof weston'
        # laun_cmd = 'pidof com.ohos.launcher'
        # time.sleep(10)
        # wes_status_1 = sendCmd(ser, wes_cmd, READ_TIMEOUT)
        # time.sleep(5)
        # wes_status_2 = sendCmd(ser, wes_cmd, READ_TIMEOUT)
        # time.sleep(5)
        # laun_status_1 = sendCmd(ser, laun_cmd, READ_TIMEOUT)
        # time.sleep(5)
        # laun_status_2 = sendCmd(ser, laun_cmd, READ_TIMEOUT)
        logger.info('ren_status_1: %s ren_status_2: %s laun_1: %s laun_2: %s '%(ren_status_1,ren_status_2,laun_status_1,laun_status_2))
        if ren_status_1 and ren_status_1 == ren_status_2 and laun_status_1 and laun_status_1 == laun_status_2:
            return True
        if not ren_status_1 or not ren_status_2 or ren_status_1 != ren_status_2:
            # and not wes_status_1.strip().isdigit() or not wes_status_2.strip().isdigit()
            logger.printLog('process render_service is not exist')
        if not laun_status_1 or not laun_status_2 :
            # or not laun_status_1.strip().isdigit() or not laun_status_2.strip().isdigit()
            logger.printLog('process com.ohos.launcher is not exist')
        return False

    @timeout(30)
    def hdc_set_time(self,sn):
        # todo
        y_m_d = time.strftime('%Y-%m-%d',time.localtime())
        h_m_s = time.strftime('%H:%M:%S',time.localtime())
        cmd = 'hdc_std -t %s shell date -u "%sT%s"'% (sn,y_m_d,h_m_s)
        logger.printLog('hdc start set time')
        logger.printLog('cmd is %s'% cmd)
        ret,out = subprocess.getstatusoutput(cmd)
        logger.printLog('hdc end set time')
        if h_m_s not in out:
            logger.printLog('hdc设置时间失败')
            logger.printLog(out)
            return False
        logger.printLog(out)

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

    def catchEraseFail(self,cmd):
        logger.printLog(datetime.datetime.now())
        ret, outpri = subprocess.getstatusoutput(cmd)
        logger.printLog(datetime.datetime.now())
        return ret,outpri
                
def getPsPid(ser, cmd, ps):
    try:
        data = sendCmd(ser, cmd, READ_TIMEOUT)
        logger.info(data)
        data = [item for item in data.split('\n') if 'grep' not in item and ps in item ]
        if len(data) < 1:
            return
        elif len(data) > 1:
            data = max(data, key=len, default='')
        logger.info(data)
        data = data[0].split()
        if len(data) < 1:
            return
        else:
            logger.info(data)
            return data[1]
    except Exception as e :
        logger.info(str(e))
        logger.info('pid 获取失败')

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
    if 'HMOS' in ret or 'OHOS' in ret or 'console:/ $' in ret or '#' in ret:
        ostype = 'OHOS'
    elif 'hisilicon' in ret:
        ostype = 'uboot'
    elif ' #' in ret:
        ostype = 'linux'
    else:
        ostype = 'bootrom'
    logger.info("board type is: %s" % ostype)
    return ostype

def sendCmd(ser, cmd, timeout):
    logger.info("cmd is: %s " % cmd)
    ser.write((cmd + '\n').encode())
    time.sleep(0.5)
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

def sendHdcCmd(cmd):
    from subprocess import getstatusoutput
    i = 0
    while True:
        if i > 3:
            break
        try:
            out = executeHdcCmd(cmd)
            logger.info('hdc cmd success')
            return out 
        except Exception as e:
            logger.info(str(e))
            logger.info('hdc cmd fail')
            i += 1
            time.sleep(1)
            continue
    logger.error('hdc cmd execute fail by three times')
    return ''

@timeout(10)
def executeHdcCmd(cmd):
    logger.info('cmd is %s' % cmd)
    time.sleep(0.5)
    ret, out = getstatusoutput(cmd)
    time.sleep(0.5)
    if ret != 0:
        logger.info('execute fail')
        return 'execute fail'
    logger.info('result is %s'% out)
    return out

if __name__ == '__main__':
    ser = serial.Serial('com5', 115200, timeout=0)