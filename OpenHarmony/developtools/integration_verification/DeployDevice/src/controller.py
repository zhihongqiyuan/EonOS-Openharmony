#!/usr/bin/env python
# -*- coding: utf-8 -*-


###############################################################
###作者：王琦                                        ###########
###功能：框架入口                                     ###########
###创建日期：20200424                                ###########
###############################################################
###############################################################

import os
import sys
import argparse
from core.run import *
from aw.Common.Constant import CONSTANT
from util.log_info import logger


if __name__ == "__main__":
    
    param_file = sys.argv[1]

    #parm_file的文件格式如下：taskid.properity。必须是taskid命名的文件
    cust_steps = parseProperties(param_file)

    errcode = start(cust_steps)
    if errcode != 0:
       logger.printLog("执行失败：错误信息：" + CONSTANT.ENVERRMESSAGE)
    logger.printLog("UPGRADE_STATUS_%s" % errcode)
    from func.liteOsUpgrade.liteOsUpgrade_RK3568_app import total_time
    try:
        with open(param_file, "a+") as f:
            f.write("\nstatus=UPGRADE_STATUS_%s\nsmoke_duration=%s" % (errcode, total_time))
    except Exception as p:
        logger.error(p)
    os._exit(errcode)
        
