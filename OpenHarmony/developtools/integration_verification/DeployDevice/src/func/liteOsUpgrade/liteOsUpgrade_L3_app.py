#-*- coding:utf-8 -*-
import uuid
import sys
import subprocess
import os
import serial

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

class liteOsUpgrade_L3(BaseApp):
    '''
    @author: w00278233
    '''

    def __init__(self, param_file):
        super().__init__(param_file)
        self.param_List = ["upgrade_upgradeLocation"]

    @dec_stepmsg("hongmeng L3 flash")
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
        step_index = self.params_dict.get("step_list").index("liteOsUpgrade_L3_app")
       
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
            status = self.upgrade()
            logger.info(status)
            logger.info(type(status))
            logger.info(f'升级状态:{str(status)}')
            if not status:
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
            logger.printLog(version_savepath)

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
                # 临时修改下载链接
                source_path = self.modifyHttpLink(source_path)
                logger.info(f'modify path:{source_path}')
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

    def modifyHttpLink(self,link):
        try:
            base_link = 'https://hm-verify.obs.cn-north-4.myhuaweicloud.com/'
            filename = link.split('&')[-1].split('=')[-1]
            names = filename.split('-')
            link_1 = '/'.join(names[0:-1])
            link_2 = f'/{filename}'
            final_link = base_link + link_1 + link_2
            return final_link
        except Exception as e:
            logger.info('path is errror.check the link_path is right,please')
            logger.info(str(e))


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
        try:
            #time.sleep(1000)
            logger.printLog('开始升级')
            self.enterTheDevice()
            time.sleep(5)
            self.flashSystemImg()
            time.sleep(5)
            self.rebootDevice()
            return True
        except Exception as e:
            logger.error('***********************************')
            logger.error(str(e))
            logger.error('flash fail')
            logger.error('***********************************')
            return False
            
    #@timeout(30*3)
    def enterTheDevice(self):
        logger.info('enter fastboot status'.center(50,'*'))
        devices_status_count = 0
        while devices_status_count < 3 :
            if self.isFastbootStatus():
                break
            devices_status_count += 1
            time.sleep(20)
        else:
            logger.error(f'device {self.params_dict.get("sn")} is not exist ,please make sure the sn is True')
            raise Exception('ERROR_SN_DEVICE_NOT_EXIST')

    #@timeout(20)
    def isFastbootStatus(self):
        adb_retcode, adb_output = subprocess.getstatusoutput('adb devices')
        
        fastboot_retcode, fastboot_output = subprocess.getstatusoutput('fastboot devices')
        
        if self.params_dict.get('sn') in adb_output:
            retcode, output = subprocess.getstatusoutput('adb -s %s reboot fastboot' % self.params_dict.get('sn'))
            logger.info('adb -s %s reboot fastboot retcode: %s'% (self.params_dict.get('sn'),str(retcode)))
            logger.info('adb -s %s reboot fastboot output: %s'% (self.params_dict.get('sn'),output))
            logger.info('device from adb mode to fastboot mode,please wait')
            time.sleep(30)
            retcode, output = subprocess.getstatusoutput('fastboot devices')
            logger.info('fastboot devices fastboot_retcode: %s'% str(fastboot_retcode))
            logger.info('fastboot devices fastboot_output: %s'% fastboot_output)
            if self.params_dict.get('sn') in output:
                logger.info('device enter fastboot mode success')
                return True
            else:
                logger.info('device enter fastboot mode fail')
                return False
        elif self.params_dict.get('sn') in fastboot_output:
            logger.info('device has been in fastboot mode')
            return True
        else:
            logger.info('设备未找到，重新尝试')
            return False

    @timeout(60*2*3)
    def flashSystemImg(self):
        logger.info('flash system.img '.center(50,'*'))
        flash_count = 0
        while flash_count < 3:
            status = self.isFlashSuccess()
            logger.info(status)
            if status:
                break
            flash_count +=1
            time.sleep(3)
        else:
            logger.error('system.img镜像刷写失败')
            raise Exception('[ERROR] ERROR_FLASH_SYSTEM.IMG_FAIL')
    
    @timeout(60*2)
    def isFlashSuccess(self):
        #local_image_path = os.path.join(version_savepath, 'img', r'aosp\target\product\generic_arm64', 'system.img')
        local_image_path = os.path.join(version_savepath, 'img', 'system.img')
        logger.info(f'flash img path: {local_image_path}')
        if os.path.exists(local_image_path):
            retcode, output = subprocess.getstatusoutput('fastboot -s %s flash system %s' % (self.params_dict.get('sn'), local_image_path))
            logger.info('fastboot -s %s flash system %s retcode: %s'% (self.params_dict.get('sn'),local_image_path,str(retcode)))
            logger.info('fastboot -s %s flash system %s output: %s'% (self.params_dict.get('sn'),local_image_path,output))
            if retcode == 0:
                logger.info('flash system.img success')
                return True
            else:
                flash_count += 1
                logger.info('再次尝试刷system.img镜像')

        else:
            logger.error('镜像文件路径不对')
            raise Exception('[ERROR] ERROR_IMG_PATH_NOT_EXIST')
            
    def rebootDevice(self):
        logger.info('reboot the device '.center(50,'*'))
        reboot_count = 0
        while reboot_count < 3 :
            logger.info('start reboot')
            if self.isRebootSuccess():
                break
            else:
                reboot_count +=1
                time.sleep(10)
        else:
            raise Exception('[ERROR] ERROR_REBOOT_DEVICE_FAIL')
            
    def isRebootSuccess(self):
        retcode, output = subprocess.getstatusoutput('fastboot -s %s reboot' % self.params_dict.get('sn'))
        logger.info('fastboot -s %s reboot  retcode: %s'% (self.params_dict.get('sn'),str(retcode)))
        logger.info('fastboot -s %s reboot  output: %s'% (self.params_dict.get('sn'),output))
        time.sleep(50)
        retcode, output = subprocess.getstatusoutput('adb devices')
        logger.info('adb devices: %s'% str(retcode))
        logger.info('adb devices: %s'% output)
        if self.params_dict.get('sn') in output:
            logger.info('reboot success')
            return True
        else:
            logger.info('reboot fail')
            return False
        


if __name__ == '__main__':
    liteOsUpgrade_L3.modifyHttpLink(liteOsUpgrade_L3,
    'https://devrepo.devcloud.cn-north-4.huaweicloud.com/DevRepoServer/v1/files/download?file_id=0fe39c430a364f59bd040c53e7a3ef1c&type=archive&filename=Daily_Version#2021-05-19_06-38-31#L35_aosp_arm64.tar.gz')