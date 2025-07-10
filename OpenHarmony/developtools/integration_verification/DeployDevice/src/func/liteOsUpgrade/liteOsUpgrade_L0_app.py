#-*- coding:utf-8 -*-
import uuid
import sys
import subprocess

from core.base import BaseApp, dec_stepmsg
from util.file_locker import FileLock
from util.log_info import logger
from util.time_info import get_now_time_str_info, get_now_time_info, Timeout, timeout
from aw.Download.Download import *
from aw.Common.Constant import CONSTANT
from aw.Common.Common import getFileName
from aw.ExtractFile.ExtractFile import *

lock_suffix = CONSTANT.File.LOCK_SUFFIX
suc_file = CONSTANT.File.SUC_FILE
failed_file = CONSTANT.File.FAILED_FILE
REBOOT_TIMEOUT = 20000000

class liteOsUpgrade_L0(BaseApp):
    '''
    @author: zwx877058
    '''

    def __init__(self, param_file):
        super().__init__(param_file)
        self.param_List = ["upgrade_upgradeLocation",
                        "sn"]

    @dec_stepmsg("hongmeng L0 flash")
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
        step_index = self.params_dict.get("step_list").index("liteOsUpgrade_L0_app")

        # 执行下载
        try:
            if not self.download():
                nowtime = get_now_time_str_info()
                logger.printLog("image download fail" )
                return False
        except Exception as e:
            logger.error(e)
            raise e
        
        if self.params_dict.get("isUpdate") == "False":
            logger.printLog("no need upgrade board")
            return True
        
        # 执行升级
        try:
            if not self.upgrade():
                logger.printLog("%s upgrade fail")
                return False
            return True
        except Exception as e:
            logger.error(e)
            raise e

    def getupgradeport(self):
        '''
        #===================================================================================
        #   @Method:        getupgradeport(self)
        #   @Precondition:  none
        #   @Func:          获取升级的端口号
        #   @PostStatus:    none
        #   @eg:            upgrade()
        #   @return:        True or Flase
        #===================================================================================
        '''
        coms_info = list_ports.comports()
        com_list = []
        cmd_keyword = "Ch A"
        up_keyword = "Ch B"
        logger.printLog(coms_info)
        for com_info in coms_info:
            if cmd_keyword in com_info[1]:
                logger.printLog("get upgrade port is %s" % com_info[0])
                self.cmd_com = com_info[0]
            if up_keyword in com_info[1]:
                logger.printLog("get upgrade port is %s" % com_info[0])
                self.deploy_com = com_info[0]
        return True
    
    @dec_stepmsg("upgrade")
    @timeout(1200)
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
        # 调用ES接口预留
        logger.info("send_es_mark")
        if self.params_dict.get("pbiid"):
            local_image_path = os.path.join(version_savepath, "jarimg", "FASTBOOT", self.params_dict.get("version_type"))
            logger.info("img path : %s" % local_image_path)
        else:
            local_image_path = os.path.join(version_savepath, "img")
        current_tool_path = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(os.path.dirname(__file__)))))
        local_tool_path = os.path.join(current_tool_path, "resource", "L0")

        #方法一
        upgrade_file_name = getFileName(local_tool_path, CONSTANT.OSType.getscripttype())
        upgrade_command = "%s %s %s %s >> %s 2>&1" % (upgrade_file_name, local_tool_path, self.cmd_com, self.deploy_com, local_image_path, logger.log_file_path)
        return subprocess.call(upgrade_command, shell=True)

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
            version_savepath = os.path.join(dir_path, version_name, "img")
            #执行img下载
        
        if self.params_dict.get("isDownload") == "True":
            logger.printLog("不需要做下载，直接返回")
            return True
        
        import hashlib
        save_file_str = version_path.replace("/", "").replace("\\", "")
        save_file_name = hashlib.sha1(save_file_str.encode("utf-8")).hexdigest()
        logger.info("download hash string:%s, hash value:%s" % (save_file_str, save_file_name))
        save_path_file = os.path.join(dir_path, "record", "%s%s" % (save_file_name, ".txt"))
        if not self.excutedown(version_path, version_savepath, save_path_file, False):
            logger.error("download img fail")
            return False

        #保存本地版本路径给devicetest去版本路径下取用例
        saveVersion(save_path_file, version_savepath)
        return True

    def excutedown(self, source_path, download_dir, suc_mark, is_file):
        '''
        #===================================================================================
        #   @Method:        excutedown(source_path, download_dir, suc_mark, is_file)
        #   @Precondition:  none
        #   @Func:          执行下载动作
        #   @PostStatus:    none
        #   @Param:         source_path：资源文件路径
        #                   download_dir:文件下载到本地的文件夹路径
        #                   is_file:是否是文件
        #   @eg:            excutedown("xxxx", "D:\\local\\image", suc_mark, Flase)
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


if __name__ == "__main__":
    param_file = sys.argv[1]
    if not param_file:
        logger.printLog("Missing params file")
        sys.exit(-1)
    try:
        uphandle = liteOsUpgrade_L0(param_file)
        uphandle._excuteApp()
    except Exception as e:
        logger.printLog(e)
        sys.exit(-1)