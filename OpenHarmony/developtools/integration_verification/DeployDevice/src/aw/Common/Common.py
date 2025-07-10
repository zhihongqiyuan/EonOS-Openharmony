# encoding=utf-8

import os
import socket
import subprocess
import time

from util.log_info import logger
from aw.Common.Constant import *

def copyFile(source_path, copy_path, size=1024 * 1024):
    '''
    #===================================================================================
    #   @Method:        copyFile(self, source_path, copy_path, size=1024 * 1024)
    #   @Precondition:  none
    #   @Func:          拷贝文件
    #   @PostStatus:    none
    #   @Param:         source_path：资源文件路径
    #                   copy_path:文件下载到本地的文件夹路径
    #                   size:字节数的大小
    #   @eg:            downloadByBitComet（"xxxx", "D:\\local\\image"）
    #   @return:        True or Flase
    #===================================================================================
    '''
    if not os.path.isfile(source_path):
        raise Exception("%s is not file" % source_path)
    if os.path.isdir(copy_path):
        copy_path = os.path.join(copy_path, os.path.basename(source_path))
    try:
        with open(source_path, 'rb') as fr, open(copy_path, 'wb') as fw:
            while(True):
                data = fr.read(size)
                if not data:
                    break
                fw.write(data)
    except:
        raise Exception("Copy %s to %s failed" % (source_path, copy_path))


def copyDirectory(src_dir, dst_dir):
    # copy everything under <src_dir> into <dst_dir>
    if not os.path.isdir(src_dir):
        return False

    if not os.path.isdir(dst_dir):
        try:
            os.makedirs(dst_dir)
        except Exception as e:
            logger.error(e)

    time.sleep(1)
    if not os.path.isdir(dst_dir):
        return False

    logger.info("copy %s to %s" % (src_dir, dst_dir))
    for item in os.listdir(src_dir):
        file_path = os.path.join(src_dir, item)
        child_path = os.path.join(dst_dir, item)
        if os.path.isfile(file_path):
            logger.info("copy %s to %s" % (file_path, child_path))
            copyFile(file_path, child_path)
            if not os.path.isfile(child_path):
                logger.error("Copy %s to %s failed" % (file_path, child_path))
                return False
        else:
            if os.path.isdir(file_path):
                ret = copyDirectory(file_path, child_path)
                if not ret:
                    return False
    return True

def getHostIp():
    """
    #===================================================================================
    #   @Method:        getHostIp()
    #   @Precondition:  none
    #   @Func:          返回机器ip
    #   @PostStatus:    none
    #   @eg:            getHostIp()
    #   @return:        ip
    #===================================================================================
    """
    ip = ""
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('8.8.8.8', 80))
        ip = s.getsockname()[0]
    except OSError as e:
        print(e)
    finally:
        s.close()
        return ip

def getFileName(path, suffix_name):
    '''
    #===================================================================================
    #   @Method:        getFileName(path, suffix_name)
    #   @Precondition:  none
    #   @Func:          find the flash script in version path or root path
    #   @PostStatus:
    #   @Param:         path, version path or root path
    #   @Example:       getFileName(path, suffix_name)
    #   @Author:        zwx205888
    #   @ModifyRecord:  none
    #===================================================================================
    '''
    logger.info("get %s in %s" % (suffix_name, path))
    import glob
    script_files = glob.glob(os.path.join(path, "*" + suffix_name))
    logger.info("real file is : %s " % script_files)
    if len(script_files) != 1:
        logger.error("more than one %s file or has No %s file" % (suffix_name, suffix_name))
        return False

    return script_files[0]

def subProcessPopen(cmd, outlog_enable=True, errlog_enable=True):
    """
    @Method:          subProcessPopen()
    @Precondition:    none
    @Func:            excute the command
    @PostStatus:      none
    @Param:           cmd,the command that will be excuted
    @Example:         subProcessPopen("RD /Q /F D:\test")
    @ModifyRecord:    none
    """

    ret_dict = {}
    number = 3

    for i in range (number):
        p1 = subprocess.Popen(cmd,
                                 stdin=subprocess.PIPE,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE,
                                 shell=True)
        out, err = p1.communicate()
        if 0 != p1.returncode:
            if True == outlog_enable:
                logger.error(err)
                logger.error("%s fail" % cmd)
            ret = False
        else:
            ret = True
        ret_dict["ret"] = ret
        ret_dict["out"] = out.strip()
        ret_dict["err"] = err.strip()
        if outlog_enable:
            logger.info("ret is %s" % ret)
            logger.info(out.strip())
        if errlog_enable:
            logger.info(err.strip())
        if ret_dict["out"] or ret_dict["err"]:
            break
    return ret_dict