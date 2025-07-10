# encoding=utf-8

"""
======================================================================================
copyright (C) 2018-2019,  Huawei Technologies Co.
========================================================================================
@文件名称:    Download.py
@描述:       文件下载相关动作
@作者:       lwx587017
@生成日期:    20181019
======================================================================================
"""
import codecs
import os
import shutil
import urllib
import time
import sys
import re
import datetime
import subprocess
import platform
import traceback
import requests
import json
import  threading
import zipfile

# from util.config import *
from util.log_info import logger
from util.time_info import get_now_time_str_info, timeout

current_path = os.path.dirname(os.path.dirname(os.path.abspath(os.path.dirname(__file__))))
BitCometjar = os.path.join(current_path, "Resource", "thirdtools", "TTorrentFull.jar")
from aw.Common.Constant import CONSTANT

from aw.Common.Common import subProcessPopen, copyFile


Aria2cbin = CONSTANT.SysTool.ARIA2CBIN

def downloadByBitComet(source_path, download_dir, os_method):
    '''
    #===================================================================================
    #   @Method:        downloadByBitComet(source_path, dest_path)
    #   @Precondition:  none
    #   @Func:          通过bt下载，将资源文件下载到本地
    #   @PostStatus:    none
    #   @Param:         source_path：资源文件路径
    #                   download_dir:文件下载到本地的文件夹路径
    #                   os_method:绑定操作系统用到公共方法的对象
    #   @eg:            downloadByBitComet（"xxxx", "D:\\local\\image", os_method）
    #   @return:        True or Flase
    #===================================================================================
    '''
    server_ip = "100.104.224.115"
    server_port = "2976"
    source_path = source_path.replace(' ', '%20')
    torrent_exist_url = "http://" + server_ip + ":" + server_port + "/centerDistributeService/BtCenter/addTaskDistribute?path=" \
                        + source_path
    logger.info(torrent_exist_url)
    version_path, destdir = os.path.split(download_dir)

    exist_log = os.path.join(version_path, destdir + "_exist.log")
    ret = False
    for _ in range(600):
        urllib.request.urlretrieve(torrent_exist_url, exist_log)
        if os.path.isfile(exist_log):
            open_file = codecs.open(exist_log, "r", "utf-8")
            content = open_file.readline()
            if str(content).find("\"status\":\"done\"") != -1:
                ret = True
                logger.info("The torrent of %s created seccessfully" % source_path)
                break
            else:
                time.sleep(6)
                logger.info("The torrent of %s is creating , please wait for a moment" % source_path)
                continue
    logger.info(ret)
    if not ret:
        logger.info("The torrent of %s created failed" % source_path)
        return False
    torrent_down_url = "http://" + server_ip + ":" + server_port + "/centerDistributeService/BtCenter/seed?path=" \
                       + source_path
    download_torrent = os.path.join(version_path, destdir + ".torrent")
    for _ in range(3):
        urllib.request.urlretrieve(torrent_down_url, download_torrent)
        if os.path.isfile(download_torrent):
            ret = False
            logger.info("The torrent of %s downloaded successed" % source_path)
            break
        else:
            time.sleep(3)
            continue
    if ret:
        logger.info("The torrent of %s downloaded failed" % source_path)
        return False

    if os.path.isfile(Aria2cbin):
        BT_Command = "aria2c -l - -d %s --enable-dht=false --seed-time=0 --log-level=warn \
                     --max-download-limit=50M --bt-stop-timeout=120 %s" % (version_path, download_torrent)
    else:
        BT_Command = "java -cp %s com.turn.ttorrent.demo.Downloader %s %s" % (
        BitCometjar, download_torrent, version_path)
    result = os.system(BT_Command)
    if result:
        logger.error("BT-Downloader returns: %s, download failed." % result)
        return False
    for ld in os.listdir(version_path):
        if len(ld) == 32 and os.path.isdir(os.path.join(version_path, ld)):
            copyDirectory(os.path.join(version_path, ld), download_dir, os_method)
            shutil.rmtree(os.path.join(version_path, ld), True)
    return True

def waitotherdown(suc_mark):
    '''
    #===================================================================================
    #   @Method:        waitotherdown(suc_mark)
    #   @Precondition:  none
    #   @Func:          如果版本相同且此版本没下载成功过，会等待正在下载的任务去下载，
    #                   等待时长2小时，若超过2小时正在下载的任务还未下载成功，则自行下载
    #   @PostStatus:    none
    #   @Param:         suc_mark：成功标志文件的路径
    #   @eg:            waitotherdown("xxx\\suc.txt")
    #   @return:        True or Flase
    #===================================================================================
    '''
    logger.info("wait other process download")
    for i in range(240):
        checktime = 5
        time.sleep(checktime)
        if os.path.isfile(suc_mark):
            logger.info("%s exists. Package has been download successfully" \
                        " by other process." % suc_mark)
            break
        if i == 239:
            logger.error("wait other process download timeout")
            return False
    return True

def isDownLoadSuccess(download_dir, suc_mark, failed_mark):
    '''
    #===================================================================================
    #   @Method:        isDownLoadSuccess(download_dir, suc_mark, failed_mark)
    #   @Precondition:  none
    #   @Func:          下载文件前，判断此文件是否已下载成功或失败，有下载失败标志，则重新下载
    #                   ,若此文件未下载过，则创建文件夹重新下载
    #   @PostStatus:    none
    #   @Param:         download_dir:文件下载到本地的文件夹路径路径
    #                   suc_mark：成功标志文件的路径
    #   @eg:            isDownLoadSuccess("xxx", "xxx\\suc.txt", "xxx\\fail.txt")
    #   @return:        True or Flase
    #===================================================================================
    '''
    if os.path.isfile(suc_mark):
        logger.info("%s has been download by other process." % download_dir)
        return True
    elif os.path.isfile(failed_mark):
        logger.info("%s exists. Other process has download package and failed. It will download by self" % failed_mark)
        os.remove(failed_mark)
    else:
        try:
            #如果当前版本的目录存在，则认为版本已经下载
            if not os.path.isdir(download_dir):
                logger.debug("mkdir %s" % download_dir)
                os.makedirs(download_dir)     
            else:
                logger.debug("%s is already downloading by other process,please wait for a moment!" % download_dir)
                ret = waitotherdown(suc_mark)
                return ret
        except Exception as e:
            logger.error("make dir fail with %s" % e)
            shutil.rmtree(download_dir)

    return False

def saveVersion(save_path_file, versionsavepath):
    '''
    #===================================================================================
    #   @Method:        saveVersion(save_path_file, versionsavepath)
    #   @Precondition:  none
    #   @Func:          此方法用于文件下载，如果文件下载成功，就缓存版本记录到record/xxx.txt中，
    #                   格式：/data/MobileUpgrade/local_img/1663de89-4a0b-5b26-bfad-5d2fd7bf0740
    #   @PostStatus:    none
    #   @Param:         save_path_file：文件名称，例5c0d84a3a51fff875f8ed4de16791b1c7d3d2cfe.txt
    #                   versionsavepath:本地存储路径
    #   @eg:            saveVersion(save_path_file， versionsavepath)
    #   @return:        None
    #===================================================================================
    '''
    savepath = os.path.dirname(save_path_file)
    if not os.path.isdir(savepath):
        logger.debug("mkdir %s" % savepath)
        os.makedirs(savepath)
    if os.path.isfile(save_path_file):
        logger.info("%s exists." % save_path_file)
    else:
        logger.info("save_path_file: %s" % save_path_file)
        with open(save_path_file, "a") as f:
            f.write(versionsavepath)

def downlaodByDownloadTool(dir_path, version_type, upgrade_type, pbiid):
    '''
    #===================================================================================
    #   @Method:        downlaodByDownloadTool(dir_path, version_type, upgrade_type, pbiid)
    #   @Precondition:  none
    #   @Func:          用下载工具提供的jar包进行下载，需要传入版本的pbiid,升级类型,版本类型，
    #                   具体使用方法可咨询wwx515469
    #   @PostStatus:    none
    #   @Param:         dir_path：本地储存版本的路径
    #                   version_type:版本的类型，如odex,dex,no_log
    #                   upgrade_type:升级的类型，如FASTBOOT,SD
    #                   pbiid:版本的pbiid号
    #                   os_method:绑定操作系统用到公共方法的对象
    #   @eg:            downlaodByDownloadTool(dir_path, version_type, upgrade_type, pbiid, os_method)
    #   @return:        None
    #===================================================================================
    '''
    upgrade_type = upgrade_type.upper()
    jar_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(os.path.dirname(__file__)))), "resource", "thirdtools", "downloadTool.jar")
    os_name = platform.system()
    logger.debug("copy res to dir_path")
    res_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(os.path.dirname(__file__)))), "resource", "thirdtools", "res")
    local_res_path = os.path.join(dir_path, "res")
    if not os.path.isdir(local_res_path):
        os.makedirs(local_res_path)
    if os_method.copyDirectory(res_path, local_res_path):
        logger.info("copy res to the local image path sucess")
    else:
        logger.info("copy res to the local image path fail")
        return False
    if (os_name == "Linux"):
        jar_cmd = "cd %s & java -jar %s %s %s:%s" % (dir_path, jar_path, pbiid, upgrade_type, version_type)
    else:
        jar_cmd = "D: & cd %s & java -jar %s %s %s:%s" % (dir_path, jar_path, pbiid, upgrade_type, version_type)
    logger.info("download by download.jar,cmd is %s" % jar_cmd)
    ret = subprocess.getoutput(jar_cmd)
    logger.info("the result is %s after excute downloadTool.jar" % ret)
    if "download success" in ret:
        list_jardir = os.listdir(dir_path)
        for dir in list_jardir:
            jardir = os.path.join(dir_path, dir)
            if os.path.isdir(jardir) and jardir != "res":
                os.rename(jardir, os.path.join(dir_path, "jarimg"))
                break
        logger.info("download success by downloadjar")
        return True
    else:
        if "failreason" in ret:
            com = re.search("failreason\((.*)\)", ret)
            failreason = com.group(1)
            logger.info("download fail by downloadjar,failreson is %s" % failreason)
        else:
            logger.info("download fail by downloadjar,and jartool not support failreason")
        return False


def downloadLinkFileFromHttp(link, path):
    '''
        #===================================================================================
        #   @Method:        downloadLinkFileFromHttp(link, path)
        #   @Precondition:  none
        #   @Func:          通过http下载资源文件到本地
        #   @PostStatus:    none
        #   @Param:         link：资源文件路径
        #                   path:本地文件路径
        #   @eg:            downloadLinkFileFromHttp("http:xxx", "D:\local_img")
        #   @return:        True or False
        #===================================================================================
    '''
    file_name = link.split('/')[-1]
    destination_file = os.path.join(path, file_name)
    logger.info('file_name is %s' % file_name)
    logger.info('destination_file is %s' % destination_file)
    if not os.path.isdir(path):
        os.makedirs(path)
    if not os.path.isfile(destination_file):
        downloadFileFromHttp(link, destination_file)

    if not os.path.isfile(destination_file):
        logger.error('download link file error')
        return False
    else:
        return True


def downloadFileFromHttp(file_link, destination_file):
    '''
        #===================================================================================
        #   @Method:        downloadFileFromHttp(file_link, destination_file)
        #   @Precondition:  none
        #   @Func:          通过http执行下载动作
        #   @PostStatus:    none
        #   @Param:         file_link：资源文件路径
        #                   destination_file:本地文件路径
        #   @eg:            destination_file("http:xxx", "D:\local_img")
        #   @return:        None
        #===================================================================================
    '''
    import urllib
    logger.info("download file from %s to %s" % (file_link, destination_file))
    urllib.request.urlretrieve(file_link, destination_file, scheduleDownloadPercent)


def scheduleDownloadPercent(downloadsize, blocksize, filesize):
    '''
        #===================================================================================
        #   @Method:        scheduleDownloadPercent(downloadsize, blocksize, filesize)
        #   @Precondition:  none
        #   @Func:          打印当前下载的进度，用百分比形式显示
        #   @PostStatus:    none
        #   @Param:         downloadsize：已下载文件的大小
        #                   blocksize:下载块数
        #                   filesize:总文件大小
        #   @eg:            scheduleDownloadPercent(xxx,2,xxx )
        #   @return:        None
        #===================================================================================
    '''
    percent = 100.0 * downloadsize * blocksize / filesize
    if percent >= 100 :
        percent = 100
        downloadpercent = 100
    percent = float("%.4f" % percent)
    if percent % 1 == 0:
        logger.info("file download percent:%.0f%%" % percent)


def downloadByCopyWindows(source_path, download_dir, is_file):
    '''
        #===================================================================================
        #   @Method:        excutedown(source_path, download_dir, is_file, params)
        #   @Precondition:  none
        #   @Func:          执行下载动作
        #   @PostStatus:    none
        #   @Param:         source_path：资源文件路径
        #                   download_dir:文件下载到本地的文件夹路径
        #                   is_file:是否是文件
        #   @eg:            excutedown("xxxx", "D:\\local\\image", Flase)
        #   @return:        True or Flase
        #===================================================================================
    '''
    try:
        if not is_file:
            for f in os.listdir(source_path):
                file_name = os.path.join(source_path, f)
                child_path = os.path.join(download_dir, f)
                if os.path.isfile(file_name):
                    copyFile(file_name, child_path, 32 * 1024)
                    if not os.path.isfile(child_path):
                        logger.error("Copy %s to %s failed" % (file_name, child_path))
                        return False
                    logger.info("Copy %s to %s successful" % (file_name, child_path))
                else:
                    if not os.path.isdir(child_path):
                        logger.info("Mkdir %s successful" % child_path)
                        os.makedirs(child_path)
                    ret = downloadByCopyWindows(file_name, child_path, False)
                    if not ret:
                        return False
        else:
            copyFile(source_path, download_dir)
        return True    
    except Exception as e:
        logger.error(e)
        return False
    return True

def downloadByCopyLinux(source_path, download_dir, is_file):
    '''
    #===================================================================================
    #   @Method:        excutedown(source_path, download_dir, is_file, params)
    #   @Precondition:  none
    #   @Func:          执行下载动作
    #   @PostStatus:    none
    #   @Param:         source_path：资源文件路径
    #                   download_dir:文件下载到本地的文件夹路径
    #                   is_file:是否是文件
    #   @eg:            excutedown("xxxx", "D:\\local\\image", Flase)
    #   @return:        True or Flase
    #===================================================================================
    '''
    try:
        region = ""
        source_path = source_path.replace("\\", "/")
        temp = source_path.split("/")
        host, share, path, file = '', '', '', ''
        if is_file:
            file = temp.pop()
        else:
            file = '*'
        for i in temp:
            if i:
                if not host:
                    host = i
                elif not share:
                    share = i
                else:
                    path = os.path.join(path, i)
            else:
                continue
        cmd = ("smbclient //%s/%s" % (host, share) + " -U CHINA/"
               + CONSTANT.OSType.ACCOUNT
               + "%'"
               + CONSTANT.OSType.PASSWORD
               + "' -c 'cd \"%s\"'" % path)
        result = subProcessPopen(cmd)
        if not result["ret"]:
            logger.error("invalid samba path.")
            return False
        cmd = ("smbclient //%s/%s" % (host, share) + " -U CHINA/"
               + CONSTANT.OSType.ACCOUNT
               + "%'"
               + CONSTANT.OSType.PASSWORD)
        if region == "green":
            cmd = cmd + "' -m SMB2 -c 'cd \"%s\";prompt OFF;recurse ON;lcd %s;mget %s'" % (path, download_dir, file)
        else:
            cmd = cmd + "' -c 'cd \"%s\";prompt OFF;recurse ON;lcd %s;mget %s'" % (path, download_dir, file)
        logger.debug(cmd)
        result = subProcessPopen(cmd)
        if ("connect failed" in str(result["out"]) or "NT_STATUS_IO_TIMEOUT" in str(result["out"])) or (
        not result["ret"]):
            return False
        else:
            return True
    except Exception:
        traceback.print_exc()
    return False

def downloadByCopy(source_path, download_dir, is_file):
    os_name = platform.system()
    if os_name == "Windows":
        return downloadByCopyWindows(source_path, download_dir, is_file)
    if os_name == "Linux":
        return downloadByCopyLinux(source_path, download_dir, is_file)

def getToken(domin_name, name, password):
    '''
        #===================================================================================
        #   @Method:        getToken(domin_name, name, passwd, proxy)
        #   @Precondition:  none
        #   @Func:          通过http下载蓝区文件
        #   @PostStatus:    none
        #   @Param:         domin_name: 帐号名
        #                   username：用户名
        #                   password：密码
        #                   
        #   @eg:            getToken("xxx", "xxx", "xxx")
        #   @return:        None
        #===================================================================================
    '''
    body_1 = {
    "auth": {
        "identity": {
            "methods": [
                "password"
            ],
            "password": {
                "user": {
                    "domain": {
                        "name": domin_name
                    },
                    "name": name,
                    "password": password
                }
            }
        },
        "scope": {
            "project": {
                "name": "cn-north-4"
            }
        }
    }
    }
    header_1 = {"content-type":"application/json;charset=UTF-8"}
    resp = requests.request("POST", "https://iam.myhuaweicloud.com/v3/auth/tokens", data=json.dumps(body_1), headers=header_1)
    #print("auth result:", resp.status_code, resp.headers)
    return resp.headers.get("X-Subject-Token")

def downloadFileFromDevCloud(link, destination_file, start, end):
    '''
        #===================================================================================
        #   @Method:        downloadFileFromDevCloud(link, username, password, path)
        #   @Precondition:  none
        #   @Func:          通过http下载蓝区文件
        #   @PostStatus:    none
        #   @Param:         link：资源文件路径
        #                   username：用户名
        #                   password：密码
        #                   path:本地文件路径
        #   @eg:            downloadFileFromDevCloud("http:xxx", "xxx", "xxx","D:\local_img")
        #   @return:        None
        #===================================================================================
    '''
    fd = open(destination_file, 'wb+')
    dname = decrypt(CONSTANT.Key.DEV_DNAME, CONSTANT.Key.DEV_KEY)
    pw = decrypt(CONSTANT.Key.DEV_PASSWD, CONSTANT.Key.DEV_KEY)
    token = getToken("hwstaff_harmonyos_ci", dname, pw)
    logger.info("get token end")
    heard = {'Range': 'Bytes=%s-%s' % (start, end), "content-type": "application/json;charset=UTF-8", "X-Language": "zh-cn", "X-Auth-Token": token}
    res = requests.get(link, headers=heard, stream=True, timeout=600)
    logger.info("connect status: %s" % res.status_code)
    fd.seek(start)
    fd.write(res.content)
    try:
        if res.status_code == 206 or res.status_code == 200:
            if not os.path.isfile(destination_file):
                logger.error('download link file and write file error')
                return False
        else:
            logger.error("downloadFromDevCloud fail, error: %s" % res)
            return False
    except Exception as e:
        traceback.print_exc()
        logger.error(e)



def decrypt(text, key):
    from Crypto.Cipher import AES
    from binascii import a2b_hex

    cryptor = AES.new(key.encode('utf-8'), AES.MODE_CBC, b'0000000000000000')
    plain_text = cryptor.decrypt(a2b_hex(text))
    return bytes.decode(plain_text).rstrip('\0')


def run_download(link, path):
    num = 6
    r = requests.head(link)
    total = int(r.headers['Content-Length'])
    logger.info('total is %s' % total)
    ranges = []
    offset = int(total / num)
    for i in range(num):
        if i == num - 1:
            ranges.append((i * offset, ''))
        else:
            ranges.append((i * offset, (i + 1) * offset))
    # 保存文件打开对象
    if "file_id=" in link:
        if link.endswith(".tar.gz"):
            file_name = link.split('=')[-1]
        else:
            file_name = "out.tar.gz"
    else:
        file_name = link.split('/')[-1]

    destination_file = os.path.join(path, file_name)
    logger.info('file_name is %s' % file_name)
    logger.info('destination_file is %s' % destination_file)
    if not os.path.isdir(path):
        os.makedirs(path)
    if os.path.isfile(destination_file):
        logger.info('%s is exit' % destination_file)
        return True
    # 保存文件打开对象
    logger.info("downloadFileFromDevCloud start")
    logger.info(link)
    thread_list = []
    # 一个数字,用来标记打印每个线程
    n = 0
    for ran in ranges:
        start, end = ran
        # 打印信息
        logger.info('thread %d start:%s,end:%s' % (n, start, end))
        n += 1
        # 创建线程 传参,处理函数为download
        thread = threading.Thread(target=downloadFileFromDevCloud, args=(link, destination_file, start, end))
        # 启动
        thread.start()
        thread_list.append(thread)
    for i in thread_list:
        # 设置等待
        i.join()
    if os.path.isfile(destination_file):
        logger.info('%s is exit' % destination_file)
        return True
    return False

