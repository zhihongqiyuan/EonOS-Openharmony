from subprocess import getstatusoutput
import time
import sys
import os

commit_message = 'test_message'
commit_path = r'D:/DevicetestTools/DeployDevice/'
print(sys.argv)
if len(sys.argv) > 1:
    if sys.argv[1] != '':
        commit_message = sys.argv[1]
if len(sys.argv) > 2:
    commit_path = commit_path + sys.argv[2]
print(commit_path)
cmds = [
    ' git config --global user.email "yangbin148@huawei.com" ',
    ' git config --global user.name "ywx1018148"',
    r' git add %s.' % commit_path,
    ' git commit -m "%s"' % commit_message,
    ' git push'
]
for item in cmds:
    sta,_ = getstatusoutput(item)
    if sta != 0:
        print('提交失败: %s' % _)
        print('cmd:%s' % item)
        print('***********************')
        print('提交失败'.center(19, '*'))
        print('***********************')
        sys.exit(-1)
    time.sleep(3)
print('***********************')
print('提交成功'.center(19, '*'))
print('***********************')