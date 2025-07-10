# encoding=utf-8

'''
======================================================================================
版权 (C) 2015-2020, Huawei Technologies Co., HUTAF xDevice
========================================================================================
@FileName:    telnet_client.py
@Function:    telnet operation to os
@Author:
@Date:
======================================================================================
'''

import os
import platform
import telnetlib
import time

from util.log_info import logger

class TelConnect():

    def __init__(self, telip, telport):
        logger.info("init telnet %s:%s" % (telip, telport))
        self._host = telip
        self._port = int(telport)
        self.__timeout = 10
        self.device = telnetlib.Telnet(self._host, port=self._port, timeout=self.__timeout)

    def tellogin(self, username, password, endwaittag, timeout):
        logger.info("telnet login")
        rets = self.device.read_until(endwaittag, timeout)
        logger.info("cmd result: %s" % rets)
        if (endwaittag in rets) or not rets:
            logger.info("Noneed login")
            return True
        else:
            rets = self.sendCmdAndCheckResult(username.encode('utf-8'), 'Password:', timeout)
            if rets == False:
                logger.error("telnet login Failed!!")
                return False
            rets = self.sendCmdAndCheckResult(password.encode('utf-8'), endwaittag, timeout)
            if rets == False:
                logger.error("telnet login Failed!!")
                return False

    def sendCmd(self, send_cmd, endtag, timeout):
        try:
            logger.info(send_cmd)
            self.device.write(send_cmd)
            rets = self.device.read_until(endtag.encode('utf-8'), timeout)
            return rets.decode('utf-8', 'ignore')
        except Exception as e:
            logger.error(e)
            return False

    def sendEnterCmd(self, endtag, timeout):
        try:
            logger.info("send enter to board")
            i = 0
            while i < 4:
                cmd = b'\r\n'
                rets = self.sendCmd(cmd, endtag, 1)
                if (endtag in rets):
                    return True
                logger.info("retry send Enter")
                i = i + 1
            return False
        except Exception as e:
            logger.error(e)
            return False

    def sendCmdAndCheckResult(self, cmd, endtag, timeout):
        try:
            rets = self.sendCmd(cmd + '\n'.encode('utf-8'), endtag, timeout)
            logger.info("result: %s" % rets)
            if (endtag in rets):
                if (not "error:" in rets.lower()):
                    return True
            logger.error("cmd not end or cmd failed, please check board !!!")
            return False
        except Exception as e:
            logger.error(e)
            return False

    def sendCmdAndCheckSucTag(self, cmd, endtag, suctag , timeout):
        try:
            rets = self.sendCmd(cmd + '\n'.encode('utf-8'), endtag, timeout)
            logger.info("result: %s" % rets)
            if (endtag in rets):
                if (suctag in rets) or (not "error:" in rets.lower()):
                    return True
            logger.error("cmd not end or cmd failed, please check board !!!")
            return False
        except Exception as e:
            logger.error(e)
            return False

    def sendUpgradeCmd(self, sendcmd, endwaittag, suctag, timeout):
        if suctag:
            logger.info("cmd is:  %s" % sendcmd)
            return self.sendCmdAndCheckSucTag(sendcmd.encode('utf-8'), endwaittag, suctag, timeout)
        else:
            logger.info("cmd is:  %s" % sendcmd)
            return self.sendCmdAndCheckResult(sendcmd.encode('utf-8'), endwaittag, timeout)

    def sendResetCmd(self, endtag, timeout):
        logger.info("send cmd reset")
        sendcmd = b'reset\n'
        try:
            #rets = self.sendCmd(sendcmd, endtag, timeout)
            self.device.write(sendcmd)
            if not self.sendEnterCmd(endtag, timeout):
                logger.error("send Enter fail")
                return False
            return True
        except Exception as e:
            logger.error(e)
            return False

    def getBoardType(self, endtag, timeout):
        rcvStr = self.sendCmd(b'\r\n', endtag, timeout)
        if 'HMOS' in rcvStr or 'OHOS' in rcvStr :
            ostype = 'OHOS'
        elif 'hisilicon' in rcvStr:
            ostype = 'uboot'
        elif ' #' in rcvStr:
            ostype = 'linux'
        else:
            ostype = 'bootrom'
        logger.info("board type is: %s" % ostype)
        return ostype

    def close(self):
        self.device.close()


