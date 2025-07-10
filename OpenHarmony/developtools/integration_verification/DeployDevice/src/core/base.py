# encoding=utf-8

# from aw.Exception.upgrade_exception import UpgradeException
from aw.Param import UpgradeParam
from util.log_info import logger

from aw.Common.Constant import CONSTANT

import platform
import traceback
import sys
import os

from util.time_info import get_now_time_str_info

class BaseApp(object):
    param_List = None
    flash_type = None

    def __init__(self, param=None):
        self.logFilePath = None
        
        self.TAG = self.__class__.__name__

        self.logFilePath = CONSTANT.OSType.default_log_dir

        if isinstance(param, dict):
            self.logFilePath = param.get('logFilePath')
            self.params_dict = param
        elif isinstance(param, str):
            param_file_path, param_file_name = os.path.split(param)
            self.logFilePath = os.path.join(param_file_path, ''.join([param_file_name.split('.')[0], '.log']))
            self.params_dict = upgrade_param.getAllParam(param)
        else:
            # raise UpgradeException(200)
            pass
        logger.switchFilePath(self.logFilePath)


    def _excuteApp(self, flash_type):
        # exit_code = 0

        try:
            ret = self._excute()
            logger.info(ret)
            nowtime = get_now_time_str_info()
            #sn = self.params_dict.get("sn")
            #os.system("hdc_std -t %s shell reboot loader" % sn)
            if ret == 98:
                flash_two(nowtime, flash_type)
                return 98
            if ret == 99:
                flash_two(nowtime, flash_type)
                return 99
            if not ret:
                flash_two(nowtime, flash_type)
                return -1
            else:
                logger.printLog("========================%s %s flash success========================" % (nowtime, flash_type))
                logger.info("============================= end autotest facoty device flash==================================")
                return 0
            # else:
            #     logger.printLog("========================%s %s flash fail========================" % (nowtime, flash_type))
            #     exit_code = -1
                # raise UpgradeException(ERROR_OTHER_ERROR)
        # except UpgradeException as e:
        #     nowtime = get_now_time_str_info()
        #     errormessage = e.getUpgradeMessage(e.code)
        #     logger.printLog("%s ========================== %s ===============================" % (nowtime, errormessage))
        #     exit_code = e.code
        except Exception as e:
            traceback.print_exc()
            return 200
        # finally:
        #     logger.info("============================= end autotest facoty device flash==================================")
        # return exit_code


    def _excute(self):
        ret = self.excute()
        return ret
    
    def parsePropertiesFile(self, all_params):
        logger.debug('select all params')
        return all_params


    #在用户级实现，如果用户级没实现，就直接报错
    def excute(self):
        logger.error('No custom executor ,check your APP pls')
        pass

def dec_stepmsg(excutmsg):
    '''
    #===================================================================================
    #   @Method:        dec_stepmsg(excutmsg)
    #   @Precondition:  none
    #   @Func:          装饰器函数，将app执行步骤的开始时间、结束时间以及相关步骤信息打印到控制
    #                   台，excutmsg为执行步骤标识需自己传入
    #   @PostStatus:    excutmsg:执行步骤标识
    #   @Param:         local_upgrade_hotaLocation: hota大包在本地的路径
    #   @eg:            dec_stepmsg("download")
    #   @return:        被装饰函数
    #===================================================================================
    '''
    def do_dec(fn):
        def do(self, *args, **kargs):
            nowtime = get_now_time_str_info()
            logger.printLog("========================%s start %s ========================" % (nowtime, excutmsg))
            ret = fn(self, *args, **kargs)
            nowtime = get_now_time_str_info()
            logger.printLog("========================%s end %s ========================" % (nowtime, excutmsg))
            return ret
        return do
    return do_dec


def flash_two(nowtime, flash_type):
    logger.printLog("========================%s %s flash fail========================" % (nowtime, flash_type))
    logger.info("============================= end autotest facoty device flash==================================")
