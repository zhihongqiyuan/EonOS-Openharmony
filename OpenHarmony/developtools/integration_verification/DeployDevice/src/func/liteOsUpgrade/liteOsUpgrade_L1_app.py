#-*- coding:utf-8 -*-
import uuid
import sys
import subprocess
import os

from core.base import BaseApp, dec_stepmsg
from util.file_locker import FileLock
from util.log_info import logger
from util.time_info import get_now_time_str_info, get_now_time_info, Timeout, timeout
from aw.Telnet.TelnetClient import TelConnect
from aw.Common.Constant import CONSTANT
from aw.Download.Download import *
from aw.Common.Common import getHostIp
from aw.ExtractFile.ExtractFile import *
from aw.poweronoff.serial_power_on_off import serialPowerOnOff

lock_suffix = CONSTANT.File.LOCK_SUFFIX  #通过文件锁实现并发下载
suc_file = CONSTANT.File.SUC_FILE        #通过本文件是区分版本是否成功下载
failed_file = CONSTANT.File.FAILED_FILE  #通过本文件是标记文件下载失败
READ_MAXTIMEOUT = 600
READ_TIMEOUT = 30
READ_MINITIMEOUT = 5
uboot_finish = 'hisilicon #'
cmd_finish = ' #'

class liteOsUpgrade_L1(BaseApp):
    '''
    @author: w00278233
    '''

    def __init__(self, param_file):
        super().__init__(param_file)
        self.param_List = ["Telnet_IP",
                        "Telnet_Port",
                           "upgrade_upgradeLocation"]

    @dec_stepmsg("hongmeng L1 flash")
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
        step_index = self.params_dict.get("step_list").index("liteOsUpgrade_L1_app")
       
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
        logger.info("download hash string:%s, hash value:%s" % (save_file_str, save_file_name))
        save_path_file = os.path.join(dir_path, "record", "%s%s" % (save_file_name, ".txt"))
        if not self.excutedown(version_path, os.path.join(version_savepath, "img"), save_path_file, False):
            logger.error("download img fail")
            return False

        #保存本地版本路径给devicetest去版本路径下取用例
        saveVersion(save_path_file, os.path.join(version_savepath, "img"))

        # 执行升级脚本下载
        if self.params_dict.get("upgrade_script"):
            suc_mark = os.path.join(version_savepath, "scriptfile", suc_file)
            if not self.excutedown(self.params_dict.get("upgrade_script"),
                                   os.path.join(version_savepath, "scriptfile"), suc_mark, True):
                logger.error("download upgrade script fail")
                return False
            with open(suc_mark, "a+") as fp:
                fp.write("")

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
    @timeout(900)
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
        
        tel_IP = self.params_dict.get("Telnet_IP")
        tel_port = self.params_dict.get("Telnet_Port")
        tftp_ip = self.params_dict.get("Tftpserver_IP")
        device_ip = self.params_dict.get("Device_IP")
        device_Mac = self.params_dict.get("Device_MAC")
        device_netmask = self.params_dict.get("Device_Netmask")
        device_gatewayip = self.params_dict.get("Device_GatewayIP")
        #芯片类型,根据芯片类型获取对应的刷机命令
        flash_type = self.params_dict.get("flash_type") 
        #串口服务器命令端口
        tel_cmd_port = self.params_dict.get("cmd_port")
        #单板的电源端口
        power_port = self.params_dict.get("board_power_port")
        tel_username = self.params_dict.get("server_cmd_username")
        tel_passwd = self.params_dict.get("server_cmd_password")

        if not tel_IP or not tel_port:
            logger.error("Telnet_IP or Telnet_Port is NULL !!")
            return False
        if not tftp_ip:
            tftp_ip = getHostIp()
            logger.info("get host ip, tftp_ip is %s" % tftp_ip)
        if not device_netmask:
            device_netmask = "255.255.252.0"
        if not device_Mac:
            device_Mac = "3a:82:d0:08:f4:99"
        if not tel_cmd_port:
            tel_cmd_port = "18984"
        local_image_path = "%s/%s" % (version_name, "img")
        if self.params_dict.get("upgrade_script"):
            script_name = self.params_dict.get("upgrade_script").split('\\')[-1]
            scriptfile = os.path.join(version_savepath, "scriptfile", script_name)
        else:
            scriptpath = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(os.path.dirname(__file__)))))
            if "ev" in flash_type:
                scriptfile = os.path.join(scriptpath, "resource", "L1", "ev200", "update.txt")
            elif "dv" in flash_type:
                scriptfile = os.path.join(scriptpath, "resource", "L1", "dv300", "update.txt")
        logger.info("upgrade scriptfile is: %s" % scriptfile)
        if not os.path.exists(scriptfile):
            logger.error("%s is not exit" % scriptfile)
            return False

        tnc = TelConnect(tel_IP, tel_port)
        try:
            board_type = tnc.getBoardType(cmd_finish, READ_MINITIMEOUT)
            if board_type != "uboot":
                if not tnc.sendResetCmd(uboot_finish, READ_TIMEOUT):
                    #上下电恢复单板
                    if power_port:
                        if tel_passwd:
                            if not recoveryBoard(tnc, tel_IP, tel_cmd_port, power_port, tel_username, tel_passwd):
                                logger.error("recovery board fail, please check board")
                                return False
                        else:
                            if not recoveryBoard(tnc, tel_IP, tel_cmd_port, power_port, tel_username):
                                logger.error("recovery board fail, please check board")
                                return False
                    else:
                        logger.error("go hisilicon # fail, please check board status")
                        return False

            with open(scriptfile, "r") as fp:
                lines = fp.readlines()
                for line in lines:
                    line = line.strip()
                    if not line:
                        logger.info("cmd is: %s " % line)
                        continue
                    if "%Tftpserver_IP%" in line:
                        line = line.replace("%Tftpserver_IP%", tftp_ip)
                    if "%Device_IP%" in line:
                        line = line.replace("%Device_IP%", device_ip)
                    if "%Device_MAC%" in line:
                        line = line.replace("%Device_MAC%", device_Mac)
                    if "%Device_Netmask%" in line:
                        line = line.replace("%Device_Netmask%", device_netmask)
                    if "%Device_GatewayIP%" in line:
                        line = line.replace("%Device_GatewayIP%", device_gatewayip)
                    if "userfs.img" in line:
                        if not os.path.exists(os.path.join(version_savepath, "img", "userfs.img")):
                            line = line.replace("userfs.img", "userfs_jffs2.img")
                    if "rootfs.img" in line:
                        if not os.path.exists(os.path.join(version_savepath, "img", "rootfs.img")):
                            line = line.replace("rootfs.img", "rootfs_jffs2.img")
                    if "tftp" in line:
                        packagefile_path = "/%s" % flash_type
                        new_packagefile_path = "/%s" % local_image_path
                        line = line.replace(packagefile_path, new_packagefile_path)
                        if not tnc.sendUpgradeCmd(line, uboot_finish, "done", READ_MAXTIMEOUT):
                            return False
                        continue
                    if "setenv" in line:
                        if not tnc.sendUpgradeCmd(line, uboot_finish, "", READ_MINITIMEOUT):
                            return False
                        continue
                    if "EXIT" in line:
                        break
                    if "go " in line:
                        if not tnc.sendUpgradeCmd(line, cmd_finish, "", READ_TIMEOUT):
                            logger.error("go OHOS # fail, please check board status")
                            return False
                        continue
                    if not tnc.sendUpgradeCmd(line, cmd_finish, "", READ_TIMEOUT):
                        return False
            board_type = tnc.getBoardType(cmd_finish, READ_MINITIMEOUT)
            if board_type == "OHOS":
                logger.info("upgrade success")
                return True
            else:
                logger.info("upgrade fail")
                return False
        except Exception as e:
            logger.printLog(e)
            return False
        finally:
            tnc.close()
            logger.info("close telnet")

def recoveryBoard(tnc, telip, cmdport, powerport, username, passwd=''):
    logger.info("start recovery board")
    # 先执行下电操作
    if not serialPowerOnOff(telip, cmdport, powerport, "off", username, passwd):
        logger.error("board power off failed")
        return False
    #再执行上电操作
    if not serialPowerOnOff(telip, cmdport, powerport, "on", username, passwd):
        logger.error("board power on failed")
        return False
    #再发送回车
    if not tnc.sendEnterCmd(" #", READ_MINITIMEOUT):
        logger.error("send Enter failed")
        return False
    board_type = tnc.getBoardType(cmd_finish, READ_MINITIMEOUT)
    if board_type == "bootrom":
        logger.error("recovery board failed")
        return False
    logger.info("recovery board succ")
    return True


if __name__ == "__main__":
    param_file = sys.argv[1]
    if not param_file:
        logger.printLog("Missing params file")
        sys.exit(-1)
    try:
        uphandle = liteOsUpgrade_L1(param_file)
        uphandle._excuteApp()
    except Exception as e:
        logger.printLog(e)
        sys.exit(-1)
