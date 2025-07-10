# encoding=utf-8

'''
Created on 20181019

@author: wangqi10
@modify data:20200424
@modify:update for BlueArea
'''
import platform
import sys
import threading
import time
import os
import platform

from util.time_info import get_now_time_str_info, get_now_time_info, Timeout, timeout
from aw.Param.UpgradeParam import getAllParam
from aw.Common.Constant import CONSTANT
from util.log_info import logger

def start(param):
    '''
    #===================================================================================
    #   @Method:        start(param)
    #   @Precondition:  none
    #   @Func:          全流程执行模板：1.组合app执行顺序
    #                                 2.初始化app，调用app.excute方法
    #                                 3.返回app执行结果
    #   @PostStatus:    none
    #   @Param:         param：用户传递的参数列表
    #   @eg:            start()
    #   @return:        None
    #===================================================================================
    '''
    if isinstance(param, dict):
        logger.switchFilePath(param.get("logFilePath"))
        runstage = param.get("runstage")
        if runstage == "":
            CONSTANT.ENVERRMESSAGE = "no runstage"
            return -1
        step_list = runstage.split(",")
        param["step_list"] = step_list
        system_type = platform.system()
        for step in step_list:
            app_name = step
            app_pyFile = 'func.' + app_name[0: app_name.find('_')] + '.' + app_name
            #app_class_name = app_name[0: app_name.rfind('_')].capitalize()
            app_class_name = app_name[0: app_name.rfind('_')]
            improt_cmd_str = 'from %s import %s as APP' % (app_pyFile, app_class_name)
            print(improt_cmd_str)
            logger.info("import_cmd_str:%s" % improt_cmd_str)
            scope = {}
            exec(improt_cmd_str, scope)
            APP = scope.get('APP', 'none')
            logger.info("APP:%s" % APP)
            app = APP(param)
            errorcode = app._excuteApp('upgrade')
            time.sleep(5)
            scriptpath =os.path.dirname(os.path.dirname(os.path.abspath(os.path.dirname(__file__))))
            if system_type == "Windows":
                loader_tool_path = os.path.join(scriptpath, "resource", "RK3568_tool", "upgrade_tool.exe")
            else:
                loader_tool_path = os.path.join(scriptpath, "resource", "RK3568_tool", "upgrade_tool")
            logger.info(loader_tool_path)
#           mycmd = "%s LD" % loader_tool_path
#            try:
#                num = send_cmd(mycmd)
#                if num != 2:
#                    logger.info("try again!")
#                    time.sleep(40)
#                    num = send_cmd(mycmd)
#                    if num != 2:
#                        logger.error("有设备断连，全部处置为真隔离状态！")
#                        errorcode = 200
#            except Exception as f:
#               logger.error(f)
#            logger.info(errorcode)
            if errorcode == 99:
                logger.error("upgrade success ,but Smoke failed, the fatal problem occurs.")
                return 99
            if errorcode == 98:
                logger.error("upgrade success ,but Smoke failed, the key problem occurs.")
                return 98
            if errorcode == -1:
                logger.error("upgrade failed.")
                return 101
            if errorcode == 200:
                return -1
            return 0
        return 0
    CONSTANT.ENVERRMESSAGE = "unknown error,please contact the engineer"
    return -1   


def send_cmd(mycmd):
    result = "".join(os.popen(mycmd).readlines())
    logger.info(result)
    num = int(result.split("(")[1].split(")")[0])
    logger.info(num)
    return num


def parseProperties(param_file):
    '''
    #===================================================================================
    #   @Method:        parseProperties(param_file)
    #   @Precondition:  none
    #   @Func:          将xxx.properties文件解析成start（param）方法中param参数需要的格式
    #   @PostStatus:    none
    #   @Param:         param：用户传递的参数列表
    #   @eg:            parseProperties()
    #   @return:        None
    #===================================================================================
    '''
    param_dict = getAllParam(param_file)
    
    #获取taskid
    taskid = "NoTaskidJob"
    task_path = param_file.split('.')[0]
    if task_path.rfind("/") > -1:
        r = task_path.rfind("/")
    else:
        r = task_path.rfind("\\")
    taskid = task_path[r + 1:]

    param_dict["taskId"] = taskid
    param_dict["logFilePath"] = ''.join([task_path, ".log"])

    return param_dict


def saveSuccessVersion(success_version):
    '''
    #===================================================================================
    #   @Method:        saveSuccessVersion(param_file)
    #   @Precondition:  none
    #   @Func:          存储成功的版本
    #   @PostStatus:    none
    #   @Param:         param：用户传递的参数列表
    #   @return:        None
    #===================================================================================
    '''
    save_path = CONSTANT.Path.getSavePath()
    try:
        with open(save_path, 'w') as f:
            f.write(f'upgrade_upgradeLocation={success_version}')
    except:
        logger.info('save success version fail')
    logger.info('saving success')


def send_times(mycmd):
    times = 0
    outcome = sendCmd(mycmd)
    while times < 3:
        if not outcome or "Empty" in outcome:
            times += 1
            time.sleep(3)
        else:
            return outcome
    return outcome


def sendCmd(mycmd):
    result = "".join(os.popen(mycmd).readlines())
    return result


@timeout(30)
def hdc_kill():
    logger.info("kill the process")
    os.system("hdc_std kill")
    time.sleep(5)
    logger.info("start the process")
    os.system("hdc_std start")
    time.sleep(10)