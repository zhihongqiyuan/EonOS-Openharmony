#-*- coding:utf-8 -*-
import uuid
import sys
import subprocess

from core.base import BaseApp, dec_stepmsg
from util.log_info import logger
from aw.Download.Download import *
from aw.Common.Constant import CONSTANT
from aw.Telnet.TelnetClient import TelConnect

lock_suffix = CONSTANT.File.LOCK_SUFFIX
suc_file = CONSTANT.File.SUC_FILE
failed_file = CONSTANT.File.FAILED_FILE
READ_TIMEOUT = 15

class presetup(BaseApp):
    '''
    @author: zwx877058
    '''

    def __init__(self, param_file):
        super().__init__(param_file)
        self.param_List = ["Telnet_IP",
                        "Telnet_Port"]

    @dec_stepmsg("presetup device")
    def excute(self):
        '''
        #===================================================================================
        #   @Method:        excute(self)
        #   @Precondition:  none
        #   @Func:          初始化预置执行入口
        #   @PostStatus:    none
        #   @eg:            excute()
        #   @return:        True or Flase
        #===================================================================================
        '''
        step_index = self.params_dict.get("step_list").index("presetup_app")

        # 执行初始化
        try:
            chip_type = self.params_dict.get("chip_type")
            if not self.defaultInitL1():
                CONSTANT.ENVERRMESSAGE = "presetup fail"
                logger.printLog(CONSTANT.ENVERRMESSAGE)
                return False
            return True
        except Exception as e:
            logger.error(e)
            raise e

    def defaultInitL1(self):
        '''
        #===================================================================================
        #   @Method:        defaultInitL1(self)
        #   @Precondition:  none
        #   @Func:          具体执行初始化预置的步骤和内容
        #   @PostStatus:    none
        #   @eg:            defaultInitL1()
        #   @return:        True or Flase
        #===================================================================================
        '''
        cmd_finish = ' #'
        tel_IP = self.params_dict.get("Telnet_IP")
        tel_port = self.params_dict.get("Telnet_Port")
        device_ip = self.params_dict.get("Device_IP")
        device_netmask = self.params_dict.get("Device_Netmask")
        device_gatewayip = self.params_dict.get("Device_GatewayIP")
        flash_type = self.params_dict.get("flash_type")

        if not tel_IP or not tel_port:
            logger.error("Telnet_IP or Telnet_Port is NULL !!")
            return False
        if not device_netmask:
            device_netmask = "255.255.252.0"

        #设置单板大网IP
        telc = TelConnect(tel_IP, tel_port)
        board_type = telc.getBoardType(cmd_finish, READ_TIMEOUT)
        if board_type == "uboot":
            reset_cmd = "reset"
            if not telc.sendCmdAndCheckResult(reset_cmd.encode('utf-8'), cmd_finish, READ_TIMEOUT):
                logger.error("board current type not command, and set %s fail!!" % reset_cmd)
                return False
        elif board_type == "bootrom":
            logger.error("board current type not support command !!")
            return False
        ret = telc.sendCmd(b'ifconfig\n', cmd_finish, READ_TIMEOUT)
        gateway = "eth0"
        if "et1" in ret:
            gateway = "et1"
        init_cmd = "ifconfig %s %s netmask %s gateway %s" % (gateway,device_ip, device_netmask, device_gatewayip)
        if not telc.sendCmdAndCheckResult(init_cmd.encode('utf-8'), cmd_finish, READ_TIMEOUT):
            logger.error("set ip failed!!!")
            return False
        ret = telc.sendCmd(b'ifconfig\n', cmd_finish, READ_TIMEOUT)
        logger.info("set ip result is: %s" % ret)
        return True

if __name__ == "__main__":
    param_file = sys.argv[1]
    if not param_file:
        logger.printLog("Missing params file")
        sys.exit(-1)
    try:
        uphandle = presetup(param_file)
        uphandle._excuteApp()
    except Exception as e:
        logger.printLog(e)
        sys.exit(-1)