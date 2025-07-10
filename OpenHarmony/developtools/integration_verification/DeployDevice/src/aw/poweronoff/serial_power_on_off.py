import sys, time
import select
from socket import socket
from util.log_info import logger

class SocketCon(socket):
    def __init__(self, ip, port, timeout=10, encoding="GBK"):
        '''
       #===================================================================================
       #   @Method:        __init__(self, ip, port,timeout=20,encoding="GBK")
       #   @Func:          初始化方法，创建telnet连接
       #   @Param:         ip: ip地址
       #                   port:端口号
       #                   timeout:超时时间
       #                   encoding:字符串和字节转换的格式
       #   @eg:            __init__("127.0.0.1", 7788，10，utf-8)
       #   @return:        none
       #===================================================================================
       '''
        super().__init__()
        try:
            self.encoding = encoding
            self.settimeout(timeout)
            self.connect((ip, port))
        except Exception as e:
            if isinstance(e, ConnectionRefusedError):
                logger.error("ConnectionRefusedError")
            else:
                logger.error(e)

    def closeCon(self):
        '''
        #===================================================================================
        #   @Method:        __init__(self, ip, port,timeout=20,encoding="GBK")
        #   @Func:          关闭连接
        #   @Param:         无
        #   @eg:            closeCon()
        #   @return:        none
        #===================================================================================
        '''
        self.close()

    def send(self, msg):
        '''
       #===================================================================================
       #   @Method:        send(self，msg)
       #   @Func:          发送消息给box或串口服务器
       #   @Param:         msg
       #   @eg:            send("XDEV USB SIGNal 0 ON")
       #   @return:        Bool
       #===================================================================================
       '''
        try:
            self.sendall(msg.encode(self.encoding))
            return True
        except Exception as e:
            logger.error(e)
            return False

    def login(self, user="Administrator", password=""):
        '''
       #===================================================================================
       #   @Method:        login(self，user,password)
       #   @Func:          串口服务器认证
       #   @Param:         user：用户名
       #                   password：密码
       #   @eg:            login("Administrator","")
       #   @return:        Bool
       #===================================================================================
       '''
        msg = user + "/" + password

        if self.send(msg):
            recv = self.receive()
            if recv == "认证错误!":
                return False
            elif recv == "Error":
                return False
            else:
                return True
        else:
            return False

    def receive(self, size=16384):
        '''
       #===================================================================================
       #   @Method:        receive(self，user,size)
       #   @Func:          接收远程主机或串口服务器返回的消息
       #   @Param:         size：
       #   @eg:            receive(16384)
       #   @return:        str
       #===================================================================================
       '''

        try:
            ready = select.select([self], [], [], 2)
            if ready[0]:
                ret = self.recv(size)
                if isinstance(ret, bytes):
                    recv = ret.decode(self.encoding)
                    return recv
                return ""
        except Exception as e:
            logger.error(e)
            return "Error"


def serialPowerOnOff(ip, port, index, power, user='Administrator', password=''):
    '''
    #===================================================================================
    #   @Method:        serialPowerOnOff(ip, port,index,power,user='Administrator', password='')
    #   @Func:          上下电执行方法
    #   @Param:         ip: 串口服务器ip
    #                   port:上下电命令端口
    #                   index:1-48，电源端口序号
    #                   power:on/off，上电/下电操作
    #                   user:串口服务器的用户名，默认取Administrator
    #                   password:串口服务器的密码，默认取空
    #   @eg:            serialPowerOnOff("10.176.49.48", 18984，1，"on")
    #   @return:        none
    #===================================================================================
    '''
    try:
        serialPort = SocketCon(ip, int(port))

        # 串口服务器认证
        if not serialPort.login(user=user, password=password):
            logger.error("认证错误")
            return False

        if power == "on":
            # 串口上电指令
            sendMsg = "ctrlpower." + str(index) + "=On\n"
            # 上电成功预期返回消息
            recvMsg = "ctrlpower." + str(index) + "=1\n"
        elif power == "off":
            # 串口下电指令
            sendMsg = "ctrlpower." + str(index) + "=Off\n"
            # 下电成功预期返回消息
            recvMsg = "ctrlpower." + str(index) + "=0\n"
        else:
            logger.error("请给on/off指令")
            return False

        if serialPort.send(sendMsg):
            recv = serialPort.receive()

            # 返回消息与预期相同，认为操作成功
            if recv == recvMsg:
                logger.info("操作成功")
                return True
            else:
                logger.error("操作失败")
                return False
        else:
            logger.error("操作失败")
            return False
    except Exception as e:
        logger.error(e)
        return False
    finally:
        serialPort.closeCon()

def usbPowerOnOff(ip, port, index, power):
    '''
    #===================================================================================
    #   @Method:        usbPowerOnOff(ip, port)
    #   @Func:          GT3000上下电执行方法
    #   @Param:         ip: box IP
    #                   port:GT3000端口
    #                   index:1-8，usb槽位序号
    #                   power:on/off，上电/下电操作
    #   @eg:            usbPowerOnOff("127.0.0.1", 7788，"on")
    #   @return:        none
    #===================================================================================
    '''

    usbpower = SocketCon(ip, int(port))

    try:
        if power == "on":
            sendMsg = "XDEV USB POWEr " + str(index) + " ON\n"
        elif power == "off":
            sendMsg = "XDEV USB POWEr " + str(index) + " OFF\n"
        else:
            logger.info("请给on/off指令")
            return False

        # 上下电成功预期返回消息
        recvMsg = "0,Success\n"

        if usbpower.send(sendMsg):
            recv = usbpower.receive()

            logger.info(sendMsg)
            logger.info(recv)
            # 返回消息与预期相同，认为操作成功
            if "0" in recv and "Success" in recv:
                logger.info("操作成功")
                return True
            else:
                logger.error("操作失败")
                return False
        else:
            logger.error("操作失败")
            return False
    except Exception as e:
        logger.error(e)
        return False
    finally:
        usbpower.closeCon()

def usbPowerOnOffV2(ip, port, index, power):
    '''
    #===================================================================================
    #   @Method:        usbPowerOnOff_v2(ip, port)
    #   @Func:          GT3000上下电执行方法,GT3000版本變化，命令有變化
    #   @Param:         ip: box IP
    #                   port:GT3000端口
    #                   index:1-8，usb槽位序号
    #                   power:on/off，上电/下电操作
    #   @eg:            usbPowerOnOff("127.0.0.1", 7788，"on")
    #   @return:        none
    #===================================================================================
    '''

    usbpower = SocketCon(ip, int(port))

    try:
        if power == "on":
            sendMsg = "xdev usb signal " + str(index) + " on\n"
        elif power == "off":
            sendMsg = "xdev usb signal " + str(index) + " off\n"
        else:
            logger.info("请给on/off指令")
            return False

        # 上下电成功预期返回消息
        recvMsg = "0,Success\n"

        if usbpower.send(sendMsg):
            recv = usbpower.receive()

            logger.info(sendMsg)
            logger.info(recv)
            # 返回消息与预期相同，认为操作成功
            if "0" in recv and "Success" in recv:
                logger.info("操作成功")
                return True
            else:
                logger.error("操作失败")
                return False
        else:
            logger.error("操作失败")
            return False
    except Exception as e:
        logger.error(e)
        return False
    finally:
        usbpower.closeCon()