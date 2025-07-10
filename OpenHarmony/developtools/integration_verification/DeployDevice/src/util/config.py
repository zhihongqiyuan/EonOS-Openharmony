# -*- coding:utf-8 -*-
"""
======================================================================================
copyright (C) 2018-2019,  Huawei Technologies Co.
========================================================================================
@文件名称:    config.py
@描述:       加载框架级常量 和 系统级常量
@作者:       zwx503461
@生成日期:    20181021
======================================================================================
"""
import os
import platform

# =========================解析config文件,获取全局常亮========================
def getAllParam(param_file):
    '''
    #===================================================================================
    #   @Method:        getAllParam(param_file)
    #   @Precondition:  none
    #   @Func:          解析参数文件获取所有参数
    #   @PostStatus:    none
    #   @Param:         param_file：参数文件
    #   @eg:            getAllParam(xxx.properties)
    #   @return:        params_dict：所有的参数
    #===================================================================================
    '''
    params_dict = {}
    with open(param_file, 'Ur', encoding="utf-8") as file:
        for line in file.readlines():
            line = line.strip()
            if len(line) != 0:
                if line.find("=") != -1:
                    str_list = line.split('=')
                    if str_list[0].find("#") != -1:
                        continue
                    else:
                        key = str_list[0].strip()
                        value = line[len(str_list[0]) + 1:].strip()
                        if len(key) != 0 and len(value) != 0:
                            params_dict[key] = value
    return params_dict


cur_path = os.path.dirname(os.path.dirname(__file__))

adb_config_path = os.path.join(cur_path, 'config', 'config_adb.config')
box_config_path = os.path.join(cur_path, 'config', 'config_box.config')
device_config_path = os.path.join(cur_path, 'config', 'config_device.config')
global_config_path = os.path.join(cur_path, 'config', 'config_global.config')
linux_config_path = os.path.join(cur_path, 'config', 'config_linux.config')
windows_config_path = os.path.join(cur_path, 'config', 'config_windows.config')
errorcode_config_path = os.path.join(cur_path, 'config', 'config_errorcode.config')
errorcode_device_config_path = os.path.join(cur_path, 'config', 'config_errorcode_device.config')

adb_config_param = getAllParam(adb_config_path)
box_config_param = getAllParam(box_config_path)
device_config_param = getAllParam(device_config_path)
global_config_param = getAllParam(global_config_path)
linux_config_param = getAllParam(linux_config_path)
windows_config_param = getAllParam(windows_config_path)
errorcode_config_param = getAllParam(errorcode_config_path)
errorcode_device_config_param = getAllParam(errorcode_device_config_path)

# print('adb_config_param is %s' % (adb_config_param))
# print('box_config_param is %s' % (box_config_param))
# print('device_config_param is %s' % (device_config_param))
# print('global_config_param is %s' % (global_config_param))
# print('linux_config_param is %s' % (linux_config_param))
# print('windows_config_param is %s' % (windows_config_param))

# =========================获取框架级常量=======================
SHORT_TIMEOUT = int(adb_config_param.get('SHORT_TIMEOUT'))
INSTALL_APK_TIMEOUT = int(adb_config_param.get('INSTALL_APK_TIMEOUT'))
REMOTE_PATH_MAX_LENGTH = int(adb_config_param.get('REMOTE_PATH_MAX_LENGTH'))
SYNC_DATA_MAX = int(adb_config_param.get('SYNC_DATA_MAX'))
ID_OKAY = adb_config_param.get('ID_OKAY').encode('ascii')
ID_FAIL = adb_config_param.get('ID_FAIL').encode('ascii')
ID_STAT = adb_config_param.get('ID_STAT').encode('ascii')
ID_RECV = adb_config_param.get('ID_RECV').encode('ascii')
ID_DATA = adb_config_param.get('ID_DATA').encode('ascii')
ID_DONE = adb_config_param.get('ID_DONE').encode('ascii')
ID_SEND = adb_config_param.get('ID_SEND').encode('ascii')
ID_LIST = adb_config_param.get('ID_LIST').encode('ascii')
ID_DENT = adb_config_param.get('ID_DENT').encode('ascii')

GT3000_DEFAULT_PORT = int(box_config_param.get('GT3000_DEFAULT_PORT'))
OK = box_config_param.get('OK')

DEVICE_STATE_ERECOVERY = device_config_param.get('DEVICE_STATE_ERECOVERY')
DEVICE_STATE_UNKNOW = device_config_param.get('DEVICE_STATE_UNKNOW')
DEVICE_STATE_BOOTLOADER = device_config_param.get('DEVICE_STATE_BOOTLOADER')
DEVICE_STATE_OFFLINE = device_config_param.get('DEVICE_STATE_OFFLINE')
DEVICE_STATE_ONLINE = device_config_param.get('DEVICE_STATE_ONLINE')
DEVICE_STATE_RECOVERY = device_config_param.get('DEVICE_STATE_RECOVERY')
DEVICE_STATE_UNAUTH = device_config_param.get('DEVICE_STATE_UNAUTH')
PLATFORM_HISI = device_config_param.get('PLATFORM_HISI')
PLATFORM_QCOM = device_config_param.get('PLATFORM_QCOM')
PLATFORM_SC = device_config_param.get('PLATFORM_SC')
DEVICE_STATE_FASTBOOT = device_config_param.get('DEVICE_STATE_FASTBOOT')

DEFAULT_HOST = global_config_param.get('DEFAULT_HOST')
DEFAULT_PORT = int(global_config_param.get('DEFAULT_PORT'))
DEFAULT_ENCODING = global_config_param.get('DEFAULT_ENCODING')
LINUX_ERROR_TITLE = global_config_param.get('LINUX_ERROR_TITLE')
WINDOWS_ERROR_TITLE = global_config_param.get('WINDOWS_ERROR_TITLE')
LOCK_SUFFIX = global_config_param.get('LOCK_SUFFIX')
SUC_FILE = global_config_param.get('SUC_FILE')
FAILED_FILE = global_config_param.get('FAILED_FILE')
ARIA2CBIN = global_config_param.get('ARIA2CBIN')

SUCCESS_CODE = errorcode_config_param.get('SUCCESS_CODE')
ERROR_CUSTPARAM_IS_NONE = errorcode_config_param.get('ERROR_CUSTPARAM_IS_NONE')
ERROR_PARAMS_NO_SN = errorcode_config_param.get('ERROR_PARAMS_NO_SN')
ERROR_VERSION_USE_CHINAESE = errorcode_config_param.get('ERROR_VERSION_USE_CHINAESE')
ERROR_VERSION_IS_NONE = errorcode_config_param.get('ERROR_VERSION_IS_NONE')
ERROR_VERSION_PATH_TOO_LONG = errorcode_config_param.get('ERROR_VERSION_PATH_TOO_LONG')
ERROR_CUSTPARAM_NOT_IN_ALLPARAMS = errorcode_config_param.get('ERROR_CUSTPARAM_NOT_IN_ALLPARAMS')
ERROR_DOWNLOAD_IMG_FAIL = errorcode_config_param.get('ERROR_DOWNLOAD_IMG_FAIL')
ERROR_DOWNLOAD_ROOT_FAIL = errorcode_config_param.get('ERROR_DOWNLOAD_ROOT_FAIL')
ERROR_DOWNLOAD_APK_FAIL = errorcode_config_param.get('ERROR_DOWNLOAD_APK_FAIL')
ERROR_MAKE_DIR_FAIL = errorcode_config_param.get('ERROR_MAKE_DIR_FAIL')
ERROR_FLASH_IMAGE_FAIL = errorcode_config_param.get('ERROR_FLASH_IMAGE_FAIL')
ERROR_FLASH_BASE_FAIL = errorcode_config_param.get('ERROR_FLASH_BASE_FAIL')
ERROR_FLASH_CUST_FAIL = errorcode_config_param.get('ERROR_FLASH_CUST_FAIL')
ERROR_FLASH_PRELOAD_FAIL = errorcode_config_param.get('ERROR_FLASH_PRELOAD_FAIL')
ERROR_NO_IMG_FILE = errorcode_config_param.get('ERROR_NO_IMG_FILE')
ERROR_NO_SCRIPT_FILE = errorcode_config_param.get('ERROR_NO_SCRIPT_FILE')
ERROR_DEVICE_NOT_FOUND = errorcode_config_param.get('ERROR_DEVICE_NOT_FOUND')
ERROR_MODIFY_SCRIPT_FAIL = errorcode_config_param.get('ERROR_MODIFY_SCRIPT_FAIL')
ERROR_INVALID_APK_FILE = errorcode_config_param.get('ERROR_INVALID_APK_FILE')
ERROR_LOCALPATH_NOT_EXIST = errorcode_config_param.get('ERROR_LOCALPATH_NOT_EXIST')
ERROR_INSTALL_APK_FAIL = errorcode_config_param.get('ERROR_INSTALL_APK_FAIL')
ERROR_DOWNLOAD_COTA_FAIL = errorcode_config_param.get('ERROR_DOWNLOAD_COTA_FAIL')
ERROR_DEVICE_UNAUTHORIZE = errorcode_config_param.get('ERROR_DEVICE_UNAUTHORIZE')
ERROR_PUSH_FILE_FAIL = errorcode_config_param.get('ERROR_PUSH_FILE_FAIL')
ERROR_NO_UPGRADE_FILE_OR_PACKAGE = errorcode_config_param.get('ERROR_NO_UPGRADE_FILE_OR_PACKAGE')
ERROR_DOWNLOAD_VERSION_FAIL = errorcode_config_param.get('ERROR_DOWNLOAD_VERSION_FAIL')
ERROR_BOOT_UP_TIME_OUT = errorcode_config_param.get('ERROR_BOOT_UP_TIME_OUT')
ERROR_MODEM_CHECK_FAIL = errorcode_config_param.get('ERROR_MODEM_CHECK_FAIL')
ERROR_LAUNCHER_CHECK_FAIL = errorcode_config_param.get('ERROR_LAUNCHER_CHECK_FAIL')
ERROR_CANNOT_INTO_FASTBOOT_MODE = errorcode_config_param.get('ERROR_CANNOT_INTO_FASTBOOT_MODE')
ERROR_RESETFACTORY_TIME_OUT = errorcode_config_param.get('ERROR_RESETFACTORY_TIME_OUT')
ERROR_OTHER_ERROR = errorcode_config_param.get('ERROR_OTHER_ERROR')
ERROR_VALIDATEPARAMS_FAIL = errorcode_config_param.get('ERROR_VALIDATEPARAMS_FAIL')
ERROR_PARSE_RESOURCEPOOL_PARMS_FAIL = errorcode_config_param.get('ERROR_PARSE_RESOURCEPOOL_PARMS_FAIL')
ERROR_DEVICE_IS_FASTBOOT_CAN_NOT_DO_APK_UPGRDE = errorcode_config_param.get('ERROR_DEVICE_IS_FASTBOOT_CAN_NOT_DO_APK_UPGRDE')
ERROR_EXCUTE_USERCUST_SCRIPT_FAIL_IN_RESOUCEPOOL = errorcode_config_param.get('ERROR_EXCUTE_USERCUST_SCRIPT_FAIL_IN_RESOUCEPOOL')
ERROR_NO_USERCUST_SCRIPT_IN_RESOUCEPOOL = errorcode_config_param.get('ERROR_NO_USERCUST_SCRIPT_IN_RESOUCEPOOL')


ERROR_UNINSTALL_XDEVICE_ERROR = errorcode_device_config_param.get('ERROR_UNINSTALL_XDEVICE_ERROR')
ERROR_INSTALL_XDEVICE_ERROR = errorcode_device_config_param.get('ERROR_INSTALL_XDEVICE_ERROR')
ERROR_PHONE_START_FAILED = errorcode_config_param.get('ERROR_PHONE_START_FAILED')
ERROR_ANDROID_STARTING = errorcode_device_config_param.get('ERROR_ANDROID_STARTING')
ERROR_PHONE_FASTBOOT = errorcode_device_config_param.get('EERROR_PHONE_FASTBOOT')
ERROR_NO_ROOT_PERMISSION = errorcode_device_config_param.get('ERROR_NO_ROOT_PERMISSION')
ERROR_ADB_TIMEOUT = errorcode_device_config_param.get('ERROR_ADB_TIMEOUT')
ERROR_PHONE_NOTFOUND = errorcode_device_config_param.get('ERROR_PHONE_NOTFOUND')
ERROR_PHONE_SIMULATE_NOTFOUND = errorcode_device_config_param.get('ERROR_PHONE_SIMULATE_NOTFOUND')
ERROR_CHECK_PARAM_ERROR = errorcode_device_config_param.get('ERROR_CHECK_PARAM_ERROR')
ERROR_PHONE_OTHER_ERROR = errorcode_device_config_param.get('ERROR_PHONE_OTHER_ERROR')

# =========================获取系统级常量=======================
os_name = platform.system()

PASSWORD = linux_config_param.get('PASSWORD')
ACCOUNT = linux_config_param.get('ACCOUNT')
WORKSPACEG = windows_config_param.get('WORKSPACEG')
ENV_PATH = windows_config_param.get('ENV_PATH')
LOCAL_APK_PATH = windows_config_param.get('LOCAL_APK_PATH')
LOCAL_COTA_PATH = windows_config_param.get('LOCAL_COTA_PATH')
MST_PATH = windows_config_param.get('MST_PATH')
MST_PATHG = windows_config_param.get('MST_PATHG')
DIR_PATH = windows_config_param.get('DIR_PATH')

# 检查恢复常量
LINUX_ECM_FILE = linux_config_param.get('LINUX_ECM_FILE')
LINUX_AGENT_FILE = linux_config_param.get('LINUX_ECM_FILE')

ANDROID_ADB_SERVER_PORT = windows_config_param.get('ANDROID_ADB_SERVER_PORT')
DEVICE_TEST_HOME = windows_config_param.get('DEVICE_TEST_HOME')
DEVICETESTOOLS_DIR = windows_config_param.get('DEVICETESTOOLS_DIR')
ES_REC_SCRIPT = windows_config_param.get('ES_REC_SCRIPT')
WINDOWS_AGENT_DIR = windows_config_param.get('WINDOWS_AGENT_DIR')
WINDOWS_AGENT_FILE = windows_config_param.get('WINDOWS_AGENT_FILE')
WINDOWS_AGENT_VERSION_FILE = windows_config_param.get('WINDOWS_AGENT_VERSION_FILE')
WINDOWS_ES_DIR = windows_config_param.get('WINDOWS_ES_DIR')
WINDOWS_ES_VERSION_FILE = windows_config_param.get('WINDOWS_ES_VERSION_FILE')
WINDOWS_ECM_DIR = windows_config_param.get('WINDOWS_ECM_DIR')
WINDOWS_ECM_VERSION_FILE = windows_config_param.get('WINDOWS_ECM_VERSION_FILE')

if os_name == 'Linux':
    SCRIPT_FILE = linux_config_param.get('SCRIPT_FILE')
    TEST_PATH = linux_config_param.get('TEST_PATH')
    WORKSPACE = linux_config_param.get('WORKSPACE')
    FLASH_SCRIPT_PATH = linux_config_param.get('FLASH_SCRIPT_PATH')
    SPLASH2_PATH = linux_config_param.get('SPLASH2_PATH')
    FASTBOOT_EXEC = linux_config_param.get('FASTBOOT_EXEC')
    DEFAULT_BAUDRATE = int(linux_config_param.get('DEFAULT_BAUDRATE'))
    DEFAULT_PORT = int(linux_config_param.get('DEFAULT_PORT'))
    P4_NAME = linux_config_param.get('P4_NAME')

if os_name == 'Windows':
    SCRIPT_FILE = windows_config_param.get('SCRIPT_FILE')
    WORKSPACE = windows_config_param.get('WORKSPACE')
    TEST_PATH = windows_config_param.get('TEST_PATH')
    FLASH_SCRIPT_PATH = windows_config_param.get('FLASH_SCRIPT_PATH')
    FASTBOOT_EXEC = windows_config_param.get('FASTBOOT_EXEC')
    SPLASH2_PATH = windows_config_param.get('SPLASH2_PATH')
    DEFAULT_BAUDRATE = int(windows_config_param.get('DEFAULT_BAUDRATE'))
    DEFAULT_PORT = int(windows_config_param.get('DEFAULT_PORT'))
    P4_NAME = windows_config_param.get('P4_NAME')
