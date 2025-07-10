#encoding=utf-8
'''
======================================================================================
版权 (C) 2015-2020, Huawei Technologies Co., HUTAF xDevice
========================================================================================
@FileName:    time_info.py
@Function:    时间格式化方法,以及timeout注释
@Author:
@Date:
======================================================================================
'''

import datetime
import sys
import threading
import time
import platform
from util.log_info import logger
thead = []


def get_timestamp():
    return time.time()

def get_time_str_info(time_stamp):
    st = time.localtime(time_stamp)
    result = time.strftime("%Y-%m-%d %H:%M:%S", st)
    return result

def time_info_to_timestamp(timeinfo):
    st = time.strptime(timeinfo, "%Y-%m-%d %H:%M:%S")
    return time.mktime(st)

def get_now_time_str_info():
    now = datetime.datetime.now()
    time_str = now.strftime("%Y-%m-%d %H:%M:%S")
    microsecond = now.microsecond
    result = "%s,%s" % (time_str, microsecond)
    return result

def get_now_time_info():
    now = datetime.datetime.now()
    time_str = now.strftime("%Y-%m-%d %H:%M:%S")
    return time_str

class KThread(threading.Thread):
    def __init__(self, *args, **kwargs):
        threading.Thread.__init__(self, *args, **kwargs)
        self.killed = False

    def start(self):
        self.__run_backup = self.run
        self.run = self.__run      # Force the Thread to install our trace.
        threading.Thread.start(self)

    def __run(self):
        sys.settrace(self.global_trace)
        self.__run_backup()
        self.run = self.__run_backup

    def global_trace(self, frame, why, arg):
        if why == "call":
            return self.local_trace
        else:
            return None

    def local_trace(self, frame, why, arg):
        if self.killed:
            if why == "line":
                raise SystemExit()
        return self.local_trace

    def kill(self):
        self.killed = True

class Timeout(Exception):
    def __init__(self, arg):
        for i in thead:
            if i is not None:
                i.kill() 
                i = None
        super(Timeout, self).__init__(arg)

def timeout(seconds = 300):
    def timeout_decorator(func):
        def _new_func(oldfunc, result, oldfunc_args, oldfunc_kwargs):
            result.append(oldfunc(*oldfunc_args, **oldfunc_kwargs))

        def _(*args, **kwargs):
            result = []
            new_kwargs = { # create new args for _new_func, because we want to get the func return val to result list
                "oldfunc": func,
                "result": result,
                "oldfunc_args": args,
                "oldfunc_kwargs": kwargs
            }
            thd = KThread(target=_new_func, args=(), kwargs=new_kwargs)
            thd.setDaemon(True)
            thd.start()
            thd.join(seconds)
            system_type = platform.system()
            if system_type == "Windows":
                alive = thd.isAlive()
            else:
                alive = thd.is_alive()
            thd.kill()
            if alive:
                raise Timeout(u"function run too long, timeout %d seconds." % seconds)
            else:
                if len(result) == 0:
                    logger.info(u"function run too long, timeout %d seconds." % seconds)
                    raise Exception("TESTCASE FAILED")
                else:
                    return result[0]
        _.__name__ = func.__name__
        _.__doc__ = func.__doc__
        return _
    return timeout_decorator