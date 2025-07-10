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
import os
import tarfile
from aw.Common.Common import copyFile, copyDirectory
from util.log_info import logger

def extractZipFile(zip_file, extract_dir):
    '''
    #===================================================================================
    #   @Method:        extractZipFile(zip_file, download_dir)
    #   @Precondition:  none
    #   @Func:          把压缩文件解压到指定目录
    #   @PostStatus:    none
    #   @Param:         zip_file：压缩文件路径
    #                   extract_dir:解压的目标文件夹
    #                   params:已获取需要参数的字典
    #                   os_method:绑定操作系统用到公共方法的对象
    #   @eg:            excutedown("xxxx", "D:\\local\\image", Flase, {"xxx":"xxx"}, os_method)
    #   @return:        True or Flase
    #===================================================================================
    '''
    if not os.path.isfile(zip_file):
        logger.error("Decompress package failed.")
        return False
    import zipfile
    if not os.path.isdir(extract_dir):
        os.mkdir(extract_dir)

    f = zipfile.ZipFile(zip_file)
    f.extractall(extract_dir)
    f.close()
    zfile = None
    try:
        zfile = zipfile.ZipFile(zip_file)
        for file_name in zfile.namelist():
            if file_name.endswith("filelist.xml") or file_name.endswith(".zip"):
                file = zfile.extract(file_name, extract_dir)
                if not os.path.isfile(os.path.join(extract_dir, os.path.basename(file_name))):
                    copyFile(file, extract_dir)
        return True
    except Exception as e:
        logger.error(e)
        return False
    finally:
        if zfile:
            try:
                zfile.close()
            except Exception as e:
                logger.error(e)

def unTarFile(tar_file, dest_dir):
    '''
    #===================================================================================
    #   @Method:        unTarFile(tar_file, dest_dir)
    #   @Precondition:  none
    #   @Func:          把压缩文件解压到指定目录
    #   @PostStatus:    none
    #   @Param:         tar_file：压缩文件路径
    #                   dest_dir:解压的目标文件夹
    #   @eg:            excutedown("xxxx", "D:\\local\\image")
    #   @return:        True or Flase
    #===================================================================================
    '''
    import platform
    if platform.system() == "Windows":
        dest_dir = "\\\\?\\%s" % dest_dir
    try:
        logger.info("untar %s to %s " % (tar_file, dest_dir))
        utar = tarfile.open(tar_file)
        utar.extractall(path = dest_dir)
        utar.close()
        img_path = os.path.join(dest_dir, "resource.img")
        if not os.path.exists(img_path):
            logger.error("no img")
            return  False
        return True
    except Exception as e:
        logger.error(e)
        return False

def backupFile(file):
    '''
    #===================================================================================
    #   @Method:        backupFile(file)
    #   @Precondition:  none
    #   @Func:          备份文件
    #   @PostStatus:    none
    #   @Param:         file：文件名
    #   @eg:            backupFile("xxx")
    #   @return:        True or False
    #===================================================================================
    '''
    logger.info("file is %s" % file)
    if not os.path.isfile(file):
        logger.error("%s file is not exist" % file)
        return False

    backup_file = file + ".bak"
    if os.path.exists(backup_file):
        os.remove(backup_file)
    try:
        logger.info("start to backup file and backup_file is %s" % backup_file)
        os.rename(file, backup_file)
    except:
        logger.info("the file may access by another process")
        time.sleep(10)
        return False

    if os.path.isfile(file):
        os.remove(file)
    return True


