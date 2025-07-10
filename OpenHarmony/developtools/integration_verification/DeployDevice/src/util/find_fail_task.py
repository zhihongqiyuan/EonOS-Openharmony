import os
import time
# import shutil
local_path = 'D:\Local\ATLog'
wait_count = 0
total_count = 0
fail_count = 0
log_path = 'D:/MobileUpgrade/fail_upgrade_list.log'

if os.path.exists(log_path):
    print("删除以往日志")
    os.remove(log_path)


with open(log_path,'a+') as fw:
    fw.write('开始遍历日志'.center(100,'*'))
    fw.write('\n')
for dir_path, dir_names, file_names in os.walk(local_path):
    if 'upgrade.log' in file_names:
        total_count += 1
        # print(dir_path, dir_names, file_names)
        try:
            items = dir_path.split(os.sep)
            block_name, task_name = (items[-1],items[-2])
        except Exception as e:
            print(dir_path)
            print(str(e))
        with open(os.path.join(dir_path, 'upgrade.log'),'r') as fr:
            if 'upgrade failed' in fr.read():
                fail_count += 1
                with open(log_path,'a+') as fw:
                    print(f'发现失败任务:{task_name}')
                    fw.write(f'失败任务路径：{dir_path}\n')
                    wait_count += 1
    if wait_count == 5:
        wait_count = 0
        # time.sleep(30)

with open(log_path,'a+') as fw:
    fw.write('失败日志遍历结束'.center(100,'*'))
    fw.write('\n')
    fw.write(f'总共检索日志：{total_count}个\n')
    fw.write(f'其中存在失败的日志：{fail_count}个\n')