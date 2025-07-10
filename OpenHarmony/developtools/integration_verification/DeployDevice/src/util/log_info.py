# encoding=utf-8
from util import time_info

import os
import sys


class Singleton():
    _instance = None
    def __new__(cls, *args, **kw):
        if not hasattr(cls, '_instance'):
            cls._instance = super(Singleton, cls).__new__(cls, *args, **kw)
        else:
            return cls._instance

class Logger():

    __info = "[INFO ]"
    __error = "[ERROR ]"
    __debug = "[DEBUG ]"
    __warning = "[WARNING ]"
    
    __user = "[USER]"
    
    log_file_path = ""

    def __init__(self):
        pass
    
    
    def switchFilePath(self, path):
        self.log_file_path = path
        self.printLog("Write detail log to File: %s" % path)
    
    
    def __printMessage(self, type, msg, log_level='developer'):
        '''
        #====================================================================================
        #   @Method:        __printMessage()
        #   @Precondition:  none
        #   @Func:          print the log
        #   @PostStatus:    none
        #   @Param:         type, log type:info, debug, error or worning
        #   @Param:         msg, log message
        #   @Example:       logger.__printMessage(self.__info, "test")
        #   @Author:        zwx205888
        #   @ModifyRecord:  None
        #====================================================================================
        '''

        time_str = time_info.get_now_time_str_info()
        filename = "unknow file"
        lineno_str = "unknow line number"
        try:
            f = sys._getframe(2)
            filename = os.path.basename(f.f_code.co_filename)
            lineno_str = str(f.f_lineno)
        except Exception as e:
            print(e)
        log_message = "%s : [%s - %s line: %s - %s]" % (type, time_str, filename, lineno_str, msg)
#         print(log_message)
        self.printIntoLogFile(log_message, log_level)

    def printIntoLogFile(self, log_message, log_level):
        if log_level == 'user':
            print(log_message)

        param_file_path, _ = os.path.split(self.log_file_path)
        if not os.path.exists(param_file_path):
            os.makedirs(param_file_path)
        with open(self.log_file_path, 'a', encoding='utf-8') as f:
            f.write(log_message + '\n')
    
    def printLog(self, log_message):
        self.__printMessage(self.__info, log_message, 'user')
    
    def info(self, msg):
        '''
        #====================================================================================
        #   @Method:        info()
        #   @Precondition:  none
        #   @Func:          print the log of info type
        #   @PostStatus:    none
        #   @Param:         msg, log message
        #   @Example:       logger.info("test")
        #   @Author:        zwx205888
        #   @ModifyRecord:  None
        #====================================================================================
        '''
        self.__printMessage(self.__info, msg)

    def error(self, msg):
        '''
        #====================================================================================
        #   @Method:        error()
        #   @Precondition:  none
        #   @Func:          print the log of error type
        #   @PostStatus:    none
        #   @Param:         msg, log message
        #   @Example:       logger.error("test")
        #   @Author:        zwx205888
        #   @ModifyRecord:  None
        #====================================================================================
        '''
        self.__printMessage(self.__error, msg)

    def debug(self, msg):
        '''
        #====================================================================================
        #   @Method:        debug()
        #   @Precondition:  none
        #   @Func:          print the log of debug type
        #   @PostStatus:    none
        #   @Param:         msg, log message
        #   @Example:       logger.debug("test")
        #   @Author:        zwx205888
        #   @ModifyRecord:  None
        #====================================================================================
        '''

        self.__printMessage(self.__debug, msg)

    def warning(self, msg):
        '''
        #====================================================================================
        #   @Method:        warning()
        #   @Precondition:  none
        #   @Func:          print the log of warning type
        #   @PostStatus:    none
        #   @Param:         msg, log message
        #   @Example:       logger.warning("test")
        #   @Author:        zwx205888
        #   @ModifyRecord:  None
        #====================================================================================
        '''

        self.__printMessage(self.__warning, msg)
        
'''
#   use logger object
'''
logger = Logger()
