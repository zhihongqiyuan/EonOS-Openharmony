# -*- coding:utf-8 -*-
import traceback
import uuid
import sys
import subprocess
import os
import time
import re
import shutil
import random
import platform
import socket

from core.base import BaseApp, dec_stepmsg
from util.file_locker import FileLock
from util.log_info import logger
from util.time_info import get_now_time_str_info, get_now_time_info, Timeout, timeout
from aw.Download.Download import *
from aw.Common.Constant import CONSTANT
from aw.Common.Common import getFileName
from aw.ExtractFile.ExtractFile import *
from aw.Common.Common import getHostIp, copyFile, copyDirectory

total_time = ""
lock_suffix = CONSTANT.File.LOCK_SUFFIX
suc_file = CONSTANT.File.SUC_FILE
failed_file = CONSTANT.File.FAILED_FILE
REBOOT_TIMEOUT = 20000000


class liteOsUpgrade_RK3568(BaseApp):
    '''
    @author: cwx1076044
    '''

    def __init__(self, param_file):
        super().__init__(param_file)
        self.param_List = ["upgrade_upgradeLocation", "sn"]

    @dec_stepmsg("hongmeng RK3568 flash")
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
        step_index = self.params_dict.get("step_list").index("liteOsUpgrade_RK3568_app")

        # # 执行下载
        # try:
        #     if not self.download():
        #         CONSTANT.ENVERRMESSAGE = "image download fail"
        #         logger.printLog(CONSTANT.ENVERRMESSAGE)
        #         return 98
        # except Exception as e:
        #     logger.error(e)
        #     #raise e
        #     return 98

        # 执行升级
        try:
            return_code = self.upgrade()
            if not return_code:
                CONSTANT.ENVERRMESSAGE = "board upgrade fail"
                logger.printLog(CONSTANT.ENVERRMESSAGE)
                return False
            if return_code == 98:
                return 98
            if return_code == 99:
                return 99
            return True
        except Exception as e:
            logger.error(e)
            raise e

    @dec_stepmsg("upgrade")
    @timeout(3600)
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
        global local_image_path, loader_tool_path, sn, LocationID, test_num, system_type
        system_type = platform.system()
        hostname = socket.gethostname()
        ipaddress = socket.gethostbyname(hostname)
        logger.printLog("******系统ip为：%s ******" % ipaddress)
        logger.printLog("******系统为：%s ******" % system_type)
        if system_type == "Windows":
            lock_file = r'C:/deviceupgrade/task.lock'
        else:
            lock_file = '/home/openharmony/deviceupgrade/task.lock'
        # 如果上一个任务没执行完成，不往下继续执行
        if not is_can_exec(lock_file):
            return False
        version_savepath = self.params_dict.get("img_path")
        upgrade_test_type = self.params_dict.get("UpgradeTestType")
        sn = self.params_dict.get("sn")
        LocationID = self.params_dict.get("LocationID")
        test_num = self.params_dict.get("test_num")
        pr_url = self.params_dict.get("pr_url")
        logFilePath = self.logFilePath
        logger.info(logFilePath)
        if system_type == "Windows":
            r = logFilePath.rfind("\\")
        else:
            r = logFilePath.rfind("/")
        report_path = logFilePath[:r]
        logger.info(report_path)
        scriptpath = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(os.path.dirname(__file__)))))
        logger.info(scriptpath)
        local_image_path = os.path.join(version_savepath)
        logger.info(local_image_path)
        # 判断是否存在smoke_check.json文件
        smoke_check_file = os.path.join(local_image_path, 'smoke_check.json')
        if os.path.exists(smoke_check_file):
            logger.info("smoke_check.json file exist, return True directly, file path is %s" % smoke_check_file)
            delete_file_lock(lock_file)
            return True
        if system_type == "Windows":
            loader_tool_path = os.path.join(scriptpath, "resource", "RK3568_tool", "upgrade_tool.exe")
        else:
            loader_tool_path = os.path.join(scriptpath, "resource", "RK3568_tool", "upgrade_tool")
        logger.info(loader_tool_path)
        mini_path = os.path.join(local_image_path, "mini_system_test", "L2_mini_system_test.py")
        archive_path = os.path.join(version_savepath)
        if not self.check_devices_mode():
            check_devices_cmd = "hdc list targets"
            f = send_times(check_devices_cmd)
            logger.info(f)
            delete_file_lock(lock_file)
            if not f or "Empty" in f:
                logger.error("No devices found,please check the device.")
                return False
            else:
                logger.info("3568 board is connected.")
                return self.check_devices_mode()
        else:
            # 下載鏡像
            upgrde_loader_cmd = "%s -s %s UL %s/MiniLoaderAll.bin -noreset" % (loader_tool_path, LocationID, local_image_path)
            h = sendCmd(upgrde_loader_cmd)
            logger.info(h)
            if "Upgrade loader ok" not in h:
                logger.error("Download MiniLoaderAll.bin Fail!")
                delete_file_lock(lock_file)
                return False
            else:
                logger.printLog("Download MiniLoaderAll.bin Success!")
                # time.sleep(3)
                write_gpt_cmd = "%s -s %s DI -p %s/parameter.txt" % (loader_tool_path, LocationID, local_image_path)
                j = sendCmd(write_gpt_cmd)
                logger.info(j)
                if "Write gpt ok" not in j:
                    logger.error("Failed to execute the parameter.txt")
                    delete_file_lock(lock_file)
                    return False
                else:
                    logger.printLog("Successfully executed parameter.txt.")
                    # time.sleep(5)
                    download_uboot_cmd = "%s -s %s DI -uboot %s/uboot.img %s/parameter.txt" % (
                        loader_tool_path, LocationID, local_image_path, local_image_path)
                    k = sendCmd(download_uboot_cmd)
                    logger.info(k)
                    if "Download image ok" not in k:
                        logger.error("Failed to download the uboot.image!")
                        delete_file_lock(lock_file)
                        if self.check_devices_mode():
                            return 98
                        return False
                    else:
                        logger.printLog("The uboot.image downloaded successfully!")
                        # time.sleep(5)
                        if not self.flash_version():
                            delete_file_lock(lock_file)
                            return False
                        reboot_devices_cmd = "%s -s %s RD" % (loader_tool_path, LocationID)
                        reboot_result = sendCmd(reboot_devices_cmd)
                        logger.info(reboot_result)
                        time.sleep(60)
                        # try:
                        #     if upgrade_test_type != "mini_system_test":
                        #         if not start_cmd(sn):
                        #             if self.check_devices_mode():
                        #                 return 98
                        #             return False
                        # except Exception as t:
                        #     logger.info(t)
                        #     if self.check_devices_mode():
                        #         return 98
                        #     return False
                        # time.sleep(10)
                        if "Reset Device OK" not in reboot_result:
                            logger.error("Failed to reboot the board!")
                            delete_file_lock(lock_file)
                            return False
                        else:
                            logger.info("Reboot successfully!")
                            # os.system("hdc -t %s shell reboot" % sn)
                            # time.sleep(30)
                            # os.system("hdc -t %s shell write_updater user_factory_reset;reboot updater " % sn)
                            # time.sleep(40)
                            # os.system("hdc -t %s shell set persist.usb.setting.gadget_conn_prompt false" % sn)
                            delete_file_lock(lock_file)
                            logger.printLog("******下载完成，升级成功，开始进行冒烟测试******")
                            # hdc_kill()
                            # os.system("hdc_std -t %s shell hilog -w start" % sn)
                            # os.system("hdc_std -t %s shell hilog -w start -t kmsg" % sn)
                            if upgrade_test_type == "null":
                                return True
                            # 临时安装
                            # self.install_third_packages()

                            screenshot_path = os.path.join(local_image_path, "screenshot")
                            script_path = os.path.join(screenshot_path, 'new_script')
                            logger.info(script_path)
                            # py_path = os.path.join(script_path, "main.py")
                            py_file = "main.py"
                            new_report_path = os.path.join(report_path, "result")
                            logger.info(new_report_path)
                            time_sleep = random.randint(3, 7)
                            time.sleep(time_sleep)
                            try:
                                if not os.path.exists(new_report_path):
                                    os.mkdir(new_report_path)
                            except Exception as e:
                                logger.error(e)
                                return 98
                            if upgrade_test_type == "mini_system_test":
                                save_path = os.path.join(new_report_path)
                                if exec_cmd(mini_path, sn, save_path, archive_path) == 98:
                                    return 98
                                return True

                            if not upgrade_test_type or upgrade_test_type == "smoke_test":
                                # 进到工程目录
                                cur_path = os.getcwd()
                                os.chdir(script_path)
                                test_return = cmd_test(script_path, py_file, sn, test_num, new_report_path, pr_url)
                                # 执行完回到原来的目录
                                os.chdir(cur_path)
                                if test_return == 1:
                                    return True
                                if test_return == 98:
                                    return 98
                                if test_return == 99:
                                    return 99
                                else:
                                    return False

    # def install_third_packages(self):
    #     try:
    #         logger.debug('python -m pip list')
    #         rst = subprocess.run('python -m pip list', capture_output=True, shell=True, encoding='utf-8', timeout=30)
    #         logger.debug(rst)
    #         logger.debug('python -m pip install pytest -U')
    #         rst = subprocess.run('python -m pip install pytest -U', capture_output=True, shell=True, encoding='utf-8', timeout=600)
    #         logger.debug(rst)
    #         logger.debug('python -m pip uninstall pytest-testreport -y')
    #         rst = subprocess.run('python -m pip uninstall pytest-testreport -y', capture_output=True, shell=True, encoding='utf-8', timeout=600)
    #         logger.debug(rst)
    #         logger.debug('python -m pip install pytest-html -U')
    #         rst = subprocess.run('python -m pip install pytest-html -U', capture_output=True, shell=True, encoding='utf-8', timeout=600)
    #         logger.debug(rst)
    #         logger.debug('python -m pip list')
    #         rst = subprocess.run('python -m pip list', capture_output=True, shell=True, encoding='utf-8', timeout=30)
    #         logger.debug(rst)
    #     except:
    #         logger.error(traceback.format_exc())

    @timeout(1000)
    def flash_version(self):
        partList = ["boot_linux", "system", "vendor", "userdata", "resource", "ramdisk", "chipset", "sys-prod", "chip-prod"]
        for i in partList:
            if not os.path.exists("%s/%s.img" % (local_image_path, i)):
                logger.printLog("%s.img is not exist, ignore" % i)
                continue
            loadcmd = "%s -s %s DI -%s %s/%s.img" % (loader_tool_path, LocationID, i, local_image_path, i)
            p = sendCmd(loadcmd)
            logger.info(p)
            # time.sleep(5)
            if "Download image ok" not in p:
                logger.info("try download %s again!" % i)
                time.sleep(1)
                second_cmd = "%s -s %s DI -%s %s/%s.img" % (loader_tool_path, LocationID, i, local_image_path, i)
                f = sendCmd(second_cmd)
                logger.info(f)
                if "Download image ok" not in f:
                    logger.printLog("Failed to download the %s.img!" % i)
                    if self.check_devices_mode():
                        return 98
                    else:
                        return False
                return True
            else:
                logger.printLog("The %s.img downloaded successfully!" % i)
        return True

    @timeout(120)
    def check_devices_mode(self):
        check_times = 0
        while check_times < 5:
            check_mode_cmd = "%s LD" % loader_tool_path
            g = sendCmd(check_mode_cmd)
            logger.info(g)
            # time.sleep(40)
            if "LocationID=%s	Mode=Loader" % LocationID in g:
                logger.info("3568 board has entered the Loader mode successfully!")
                return True
            else:
                # if test_num != "2/2":
                #    hdc_kill()
                os.system("hdc -t %s shell reboot loader" % sn)
                time.sleep(8)
                check_times += 1
        logger.error("Failed to enter the loader mode!")
        return False

    # @dec_stepmsg("download")
    # @timeout(360)
    # def download(self):
    #     '''
    #     #===================================================================================
    #     #   @Method:        download(self)
    #     #   @Precondition:  none
    #     #   @Func:          构建下载到本地的路径，执行相应包的下载
    #     #   @PostStatus:    none
    #     #   @eg:            download()
    #     #   @return:        True or Flase
    #     #===================================================================================
    #     '''
    #     global version_savepath, version_name
    #     dir_path = CONSTANT.Path.getDirPath()
    #     if self.params_dict.get("pbiid"):
    #         version_path = self.params_dict.get("pbiid")
    #         version_name = str(uuid.uuid5(uuid.NAMESPACE_URL, str(self.params_dict.get("pbiid")) + "FASTBOOT"))
    #         version_savepath = os.path.join(dir_path, self.params_dict.get("flash_type"), version_name)
    #     else:
    #         version_path = self.params_dict.get("upgrade_upgradeLocation")
    #         version_name = str(uuid.uuid5(uuid.NAMESPACE_URL, (self.params_dict.get("upgrade_upgradeLocation"))))
    #         version_savepath = os.path.join(dir_path, version_name, "img")
    #         # 执行img下载
    #
    #     if self.params_dict.get("isDownload") == "True":
    #         logger.printLog("不需要做下载，直接返回")
    #         return True
    #
    #     import hashlib
    #     save_file_str = version_path.replace("/", "").replace("\\", "")
    #     save_file_name = hashlib.sha1(save_file_str.encode("utf-8")).hexdigest()
    #     logger.info("download hash string:%s, hash value:%s" % (save_file_str, save_file_name))
    #     save_path_file = os.path.join(dir_path, "record", "%s%s" % (save_file_name, ".txt"))
    #     logger.info(version_savepath)
    #     logger.info(save_path_file)
    #     t = version_savepath[:-4]
    #     logger.info(t)
    #     if not self.excutedown(version_path, version_savepath, save_path_file, False):
    #         logger.info("download again!")
    #         try:
    #             if os.path.exists(t):
    #                 shutil.rmtree(t)
    #                 logger.info("remove dir succeed")
    #             if os.path.exists(save_path_file):
    #                 logger.info("remove file succeed")
    #                 os.remove(save_path_file)
    #         except Exception as p:
    #             logger.error(p)
    #             raise Exception(p)
    #         time.sleep(15)
    #         if not self.excutedown(version_path, version_savepath, save_path_file, False):
    #             logger.error("download img fail")
    #             return False
    #
    #     # 保存本地版本路径给devicetest去版本路径下取用例
    #     saveVersion(save_path_file, version_savepath)
    #     return True
    #
    # def excutedown(self, source_path, download_dir, suc_mark, is_file):
    #     '''
    #     #===================================================================================
    #     #   @Method:        excutedown(source_path, download_dir, suc_mark, is_file)
    #     #   @Precondition:  none
    #     #   @Func:          执行下载动作
    #     #   @PostStatus:    none
    #     #   @Param:         source_path：资源文件路径
    #     #                   download_dir:文件下载到本地的文件夹路径
    #     #                   is_file:是否是文件
    #     #   @eg:            excutedown("xxxx", "D:\\local\\image", suc_mark, Flase)
    #     #   @return:        True or Flase
    #     #===================================================================================
    #     '''
    #     failed_mark = os.path.join(download_dir, failed_file)
    #     lock_path = os.path.join(download_dir, lock_suffix)
    #     file_lock = FileLock()
    #
    #     if isDownLoadSuccess(download_dir, suc_mark, failed_mark):
    #         return True
    #     try:
    #         nowtime = get_now_time_str_info()
    #         logger.printLog("%s Downloading, please wait" % nowtime)
    #         file_lock.lockFile(lock_path)
    #         ret = ""
    #         logger.info("Get lock. Start to ")
    #         try:
    #             if self.params_dict.get("bt_enable") and self.params_dict.get("bt_enable") == "True":
    #                 ret = downloadByBitComet(source_path, download_dir, os_method)
    #             elif source_path.startswith('\\\\'):
    #                 ret = downloadByCopy(source_path, download_dir, is_file)
    #             elif self.params_dict.get("pbiid"):
    #                 ret = downlaodByDownloadTool(version_savepath, self.params_dict.get("version_type"), "FASTBOOT",
    #                                              self.params_dict.get("pbiid"))
    #             elif source_path.startswith("http"):
    #                 ret = run_download(source_path, download_dir)
    #         except Exception as f:
    #             logger.error(f)
    #
    #         if source_path.endswith(".zip"):
    #             zip_name = os.path.basename(source_path)
    #             ret = extractZipFile(os.path.join(download_dir, zip_name), download_dir)
    #         if source_path.endswith(".tar.gz") or (source_path.startswith("http") and ("file_id=" in source_path)):
    #             if source_path.startswith("http") and ("file_id=" in source_path):
    #                 if source_path.endswith(".tar.gz"):
    #                     zip_name = source_path.split('=')[-1]
    #                 else:
    #                     zip_name = "out.tar.gz"
    #             else:
    #                 zip_name = os.path.basename(source_path)
    #             ret = unTarFile(os.path.join(download_dir, zip_name), download_dir)
    #         nowtime = get_now_time_str_info()
    #         logger.printLog("%s download to %s end" % (nowtime, download_dir))
    #
    #         if not ret:
    #             with open(failed_mark, "a+") as fp:
    #                 fp.write("")
    #         return ret
    #     except Exception as e:
    #         logger.printLog(e)
    #         #raise Exception(e)
    #     finally:
    #         file_lock.releaseFile()


@timeout(30)
def hdc_kill():
    logger.info("kill the process")
    os.system("hdc kill")
    time.sleep(2)
    logger.info("start the process")
    os.system("hdc -l5 start")
    # time.sleep(10)


def sendCmd(mycmd):
    result = "".join(os.popen(mycmd).readlines())
    return result


def send_times(mycmd):
    times = 0
    outcome = sendCmd(mycmd)
    while times < 3:
        if not outcome or "Empty" in outcome:
            times += 1
            time.sleep(3)
        else:
            time.sleep(3)
            return outcome
    return outcome


@timeout(180)
def start_cmd(sn):
    try:
        os.system("hdc -l5 start")
        # power_cmd = "hdc -t %s shell \"power-shell setmode 602\"" % sn
        hilog_cmd = "hdc -t %s shell \"hilog -w start -l 400000000 -m none\"" % sn
        # logger.info(power_cmd)
        logger.info(hilog_cmd)
        # power_result = sendCmd(power_cmd)
        # logger.info(power_result)
        # if not power_result:
        #     return False
        # number = 0
        # while "Set Mode Success" not in power_result and number < 30:
        #     time.sleep(4)
        #     power_result = sendCmd(power_cmd)
        #     logger.info(power_result)
        #     number += 1
        #     if number >= 20:
        #         logger.error("Set mode failed")
        #         return False
        hilog_result = sendCmd(hilog_cmd)
        logger.info(hilog_result)
        return True
    except Exception as e:
        logger.error(e)
        return False


@timeout(900)
def cmd_test(screenshot_path, py_file, device_num, test_num, new_report_path, pr):
    global total_time
    save_screenshot_path = os.path.join(new_report_path, f"screenshot_result{LocationID}")
    logger.info(save_screenshot_path)
    time_sleep = random.randint(1, 5)
    time.sleep(time_sleep)
    try:
        if not os.path.exists(save_screenshot_path):
            os.mkdir(save_screenshot_path)
        logger.info(save_screenshot_path)
        base_screenshot_path = os.path.join(new_report_path, f"screenshot_base{LocationID}")
        if not os.path.exists(base_screenshot_path):
            os.mkdir(base_screenshot_path)
        logger.info(base_screenshot_path)
    except Exception as e:
        logger.error(e)
        return 98
    # config_path = os.path.join(screenshot_path, "app_capture_screen_test_config.json")
    py_cmd = "python {} --device_num {} --test_num {} --save_path {} --pr {}".format(py_file, device_num, test_num, save_screenshot_path, pr)
    time1 = time.time()
    result = outCmd(py_cmd, save_screenshot_path, base_screenshot_path, screenshot_path)
    time2 = time.time()
    total_time = int(time2 - time1)
    logger.info("total_time: %s" % total_time)
    if result == 1:
        return True
    if result == 98:
        return 98
    if result == 99:
        return 99
    else:
        return False


@timeout(900)
def outCmd(cmd, save_screenshot_path, base_screenshot_path, script_path):
    logger.info("cmd is: %s" % cmd)
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding='utf-8', errors='ignore', universal_newlines=True)
    # p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, errors='ignore', universal_newlines=True)
    curline = p.stdout.readline()
    # list_png_name = []
    try:
        while "End of check" not in curline:
            curline = p.stdout.readline()
            logger.info(curline)
            if 'SmokeTest: End of check, test succeeded!' in curline:
                return True
            # if "abnarmal" in curline:
            #     png_name = curline.split(" ")[3].split(".")[0]
            #     list_png_name.append(png_name)
            if "SmokeTest find some fatal problems" in curline:
                logger.error("SmokeTest find some fatal problems!")
                return 99
    except Exception as e:
        logger.error(e)
        logger.error("execute smoke_test.py failed!")
        return 99
    # l = list(set(list_png_name))
    # if l:
    #     logger.error(l)
    try:
        resource_path = os.path.join(script_path, 'resource')
        for jpeg_file in os.listdir(resource_path):
            if jpeg_file.endswith('jpeg'):
                result = os.path.join(resource_path, jpeg_file)
                base = os.path.join(base_screenshot_path, jpeg_file)
                shutil.copy(result, base)
    except Exception as t:
        logger.info(t)
    p.wait()
    logger.info("p.returncode %s" % p.returncode)
    if p.returncode == 0:
        logger.info("screenshot check is ok!")
        return True
    if p.returncode == 101:
        logger.error("device disconnection, please check the device!")
        return False
    logger.error("screenshot test failed, check the %s" % save_screenshot_path)
    return 98


@timeout(1000)
def exec_cmd(mini_path, sn, save_path, archive_path):
    cmd = "python %s --device_num %s --save_path %s --archive_path %s" % (mini_path, sn, save_path, archive_path)
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding="gbk")
    curline = p.stdout.readline()
    try:
        while "End of check" not in curline:
            curline = p.stdout.readline()
            logger.info(curline)
    except Exception as e:
        logger.error(e)
    p.wait()
    logger.info("p.returncode %s" % p.returncode)
    if p.returncode == 0:
        logger.info("mini_system_test is ok!")
        return True
    logger.error("mini_system_test failed!")
    return 98

@timeout(1000)
def is_can_exec(lock_file):
    """
    判断升级是否可以执行
    @param lock_file: 文件路径
    """
    lock_duration = 10 * 60  # 10分钟（以秒为单位）
    if os.getenv('wait_time') is not None:
        lock_duration = int(os.getenv('wait_time'))
    # 检查锁文件
    if os.path.exists(lock_file):
        # 获取锁文件的创建时间
        lock_time = os.path.getmtime(lock_file)
        current_time = time.time()
        # 判断锁是否超时
        if  (current_time - lock_time) < 30:
            return True  
        if (current_time - lock_time) < lock_duration:
            logger.error("ask is already running. Exiting.")
            return False
        else:
            logger.warning("ask running time is more than %s second, can exec" % lock_duration)
            delete_file_lock(lock_file)
            create_file_lock(lock_file)
            return True
    else:
        logger.info("no tasks are being executed")
        create_file_lock(lock_file)
    return True

def create_file_lock(lock_file):
    """
    创建文件锁
    @param lock_file: 文件路径
    """
    directory = os.path.dirname(lock_file)
    if not os.path.exists(directory):
        os.makedirs(directory)
    logger.info("create_file_lock")
    with open(lock_file, 'w') as f:
        f.write('locked, please can not delete')
def delete_file_lock(lock_file):
    """
    删除文件
    @param lock_file: 文件路径
    """
    logger.info("delete_file_lock")
    if os.path.exists(lock_file):
        os.remove(lock_file)