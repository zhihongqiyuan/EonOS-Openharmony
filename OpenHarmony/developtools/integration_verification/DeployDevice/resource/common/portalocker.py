#encoding=utf-8
import os
import time
import errno
 
class FileLockException(Exception):
    pass

class FileLock(object):
    """ A file locking mechanism that has context-manager support so 
        you can use it in a with statement. This should be relatively cross
        compatible as it doesn't rely on msvcrt or fcntl for the locking.
    """


    def __init__(self, timeout=7200, delay=30):
        """ Prepare the file locker. Specify the file to lock and optionally
            the maximum timeout and the delay between each attempt to lock.
        """
        self.is_locked = False
        self.timeout = timeout
        self.delay = delay

    def _setLockFileName(self, file_name):
        self.lockfile = file_name

    def acquire(self):
        """ Acquire the lock, if possible. If the lock is in use, it check again
            every `wait` seconds. It does this until it either gets the lock or
            exceeds `timeout` number of seconds, in which case it throws 
            an exception.
        """
        start_time = time.time()
        if os.path.isfile(self.lockfile):
            try:
                mark_file_mtime = os.path.getmtime(self.lockfile)
                if (start_time - mark_file_mtime > self.timeout):
                    os.remove(self.lockfile)
            except Exception as e:
                print("the lock file is locked by other process")

        while True:
            try:
                #open file , other application can't open it
                self.fd = os.open(self.lockfile, os.O_CREAT|os.O_EXCL|os.O_RDWR)
                break
            except OSError as e:
                if e.errno != errno.EEXIST:
                    raise 
                if (time.time() - start_time) >= self.timeout:
                    raise FileLockException("Timeout occured.")
                time.sleep(self.delay)
        self.is_locked = True


    def release(self):
        """ Get rid of the lock by deleting the lockfile. 
            When working in a `with` statement, this gets automatically 
            called at the end.
        """
        #
        if self.is_locked :
            os.close(self.fd)
            os.unlink(self.lockfile)
            self.is_locked = False

    def lockFile(self, file_name):
        """ Activated when used in the with statement. 
            Should automatically acquire a lock to be used in the with block.
        """
        self._setLockFileName(file_name)
        if not self.is_locked:
            self.acquire()
        return self

    def releaseFile(self):
        """ Activated at the end of the with statement.
            It automatically releases the lock if it isn't locked.
        """
        if self.is_locked:
            self.release()

    def __del__(self):
        """ Make sure that the FileLock instance doesn't leave a lockfile
            lying around.
        """
        self.release()



#用法比较有意思，使用with关键字。对with关键字来说，FileLock类先执行__enter__函数，然后，执行with块里的那些代码，执行完了之后，再执行__exit__函数，等价于相当于如下形式：
#try:
#    执行 __enter__的内容
#    执行 with_block.
#finally:
#    执行 __exit__内容
#FileLock在__enter__函数独占式创建或打开一个文件，这个文件不会被其他程序或者进程再次创建或者打开，由此形成lock，执行完代码，在__exit__里，关闭并删除文件