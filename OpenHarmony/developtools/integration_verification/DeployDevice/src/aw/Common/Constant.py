# encoding=utf-8

'''
======================================================================================
版权 (C) 2020-2025, Huawei Technologies Co., Env
========================================================================================
@FileName:    constant.py
@Function:    def constant var
@Author:      w00278233
@Date:        20200424
======================================================================================
'''

import platform
import os



class CONSTANT(object):
    ENVERRMESSAGE = ""
	
    class Box():
        '''
        Box相关的常量
        '''
        SHORT_TIMEOUT=15
        GT3000_DEFAULT_PORT=7788
        OK="0,Success"
    
    class OSType():
        '''
        与操作系统的相关的常量
        '''
        PASSWORD = "gongjubu123456!."
        ACCOUNT = "pwhpdevice"
        def getworkspace():
            WORKSPACE= ""
            if platform.system() == "Linux":
                WORKSPACE = "/data/MobileUpgrade"
            if platform.system() == "Windows":
                WORKSPACE = "D:\MobileUpgrade"
            return WORKSPACE
        default_log_dir = os.path.join(getworkspace(), "log")

        def getscripttype():
            script_file = ""
            if platform.system() == "Linux":
                script_file = ".sh"
            if platform.system() == "Windows":
                script_file = ".bat"
            return  script_file
        

    class Board():
        pass

    class ErrCode():
        SUCCESS_CODE = 0
        ERROR_NO_USERCUST_SCRIPT_IN_RESOUCEPOOL = 11
        ERROR_EXCUTE_USERCUST_SCRIPT_FAIL_IN_RESOUCEPOOL=12
        ERROR_INVALID_APK_FILE = 33
        ERROR_CUSTPARAM_IS_NONE=62
        ERROR_PARAMS_NO_SN=63
        ERROR_VERSION_USE_CHINAESE=65
        ERROR_VERSION_IS_NONE=66
        ERROR_VERSION_PATH_TOO_LONG=67
        ERROR_CUSTPARAM_NOT_IN_ALLPARAMS=68
        ERROR_DOWNLOAD_IMG_FAIL=101
        ERROR_DOWNLOAD_ROOT_FAIL=102
        RROR_MAKE_DIR_FAIL=103
        ERROR_DOWNLOAD_APK_FAIL=104
        ERROR_LOCALPATH_NOT_EXIST=105
        ERROR_INSTALL_APK_FAIL=106
        ERROR_NO_UPGRADE_FILE_OR_PACKAGE=107
        ERROR_DOWNLOAD_VERSION_FAIL=108
        ERROR_PARSE_RESOURCEPOOL_PARMS_FAIL=109
        ERROR_DEVICE_IS_FASTBOOT_CAN_NOT_DO_APK_UPGRDE=110
        ERROR_FLASH_IMAGE_FAIL=111
        ERROR_FLASH_BASE_FAIL=112
        ERROR_FLASH_CUST_FAIL=113
        ERROR_FLASH_PRELOAD_FAIL=114
        ERROR_BOOT_UP_TIME_OUT=119
        ERROR_MODEM_CHECK_FAIL=121
        ERROR_LAUNCHER_CHECK_FAIL=122
        ERROR_CANNOT_INTO_FASTBOOT_MODE=123
        ERROR_RESETFACTORY_TIME_OUT=125
        ERROR_DOWNLOAD_COTA_FAIL=128
        ERROR_NO_IMG_FILE=131
        ERROR_NO_SCRIPT_FILE=132
        ERROR_DEVICE_NOT_FOUND=133
        ERROR_MODIFY_SCRIPT_FAIL=134
        ERROR_DEVICE_UNAUTHORIZE=135
        ERROR_PUSH_FILE_FAIL=136
        ERROR_VALIDATEPARAMS_FAIL=137
        ERROR_OTHER_ERROR = 200

    class SysTool():
        ARIA2CBIN=r"/usr/bin/aria2c"
    
    class File():
        LOCK_SUFFIX="file_lock.txt"
        SUC_FILE="suc.txt"
        FAILED_FILE="failed.txt"

    class Path():
        def getDirPath():
            if platform.system() == "Windows":
                return "D:\MobileUpgrade\local_img"
            if platform.system() == "Linux":
                return "/data/MobileUpgrade/local_img"
            return "unknow OS"

        def getTftpServerPath():
            if platform.system() == "Linux":
                return "/data/nfsuser/data"
            if platform.system() == "Windows":
                return "D:\MobileUpgrade\local_img"
            return "unknow OS"

        def getSavePath():
            return r'D:\DevicetestTools\upgrade_version.txt'

    class Key():
        DEV_KEY = "6ba7b8119dad11d180b400c04fd430c8"
        DEV_DNAME = "b480ff9447a7e874e7065d290b5294c1662e0195a20f0dc60180f8c036bb901e"
        DEV_PASSWD = "3c5db801e47e4cba7bd0d9bd95bd1540"

