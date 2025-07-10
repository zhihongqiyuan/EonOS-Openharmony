#encoding=utf-8
import os
import platform
import sys
import time
from git.repo import Repo
from portalocker import FileLock

def printLog(message):
    now = time.localtime()
    time_info = time.strftime("%Y-%m-%d %X", now)
    print(time_info + " " + message)

def getAllParam(param_file):
    params_dict = {}
    with open(param_file, 'Ur', encoding="utf-8") as file:
        for line in file.readlines():
            line = line.strip()
            if len(line) != 0:
                if line.find("=") != -1:
                    str_list = line.split('=')
                    if str_list[0].find("#") != -1:
                        continue
                    else:
                        key = str_list[0].strip()
                        value = line[len(str_list[0]) + 1:].strip()
                        if len(key) !=0 and len(value) != 0:
                            params_dict[key] = value

    return params_dict

def parseProjectName(server_link):
    printLog("INFO: parse the upgrade script project name from %s" % server_link)
    items = server_link.split("/")
    if len(items) <= 1:
        return None
    project_name = items[-1].split(".")[0]
    return project_name

def downloadUpgradeScripts(server_link, upgrade_script_path, is_update_script):
    lock_name = "EnvToolMicroService.lock"
    lock_file_abspath = os.path.join(WORKSPACE, lock_name)
    locked = True
    if locked:
        printLog("download or update the upgrade script")
        file_lock = FileLock()
        file_lock.lockFile(lock_file_abspath)
        printLog("Get lock. Start to ")
        
        if not is_update_script and os.path.exists(upgrade_script_path):
            printLog("update_script is false,there is no need to upgrade script")
            return True

        project_branch = "master"
        if "-b" in server_link:
            project_branch = server_link.split(" -b ")[-1]
        if os.path.isdir(upgrade_script_path) and os.path.isdir(os.path.join(upgrade_script_path, ".git")):
            if is_update_script:
                repo = Repo(upgrade_script_path)
                repo.git.remote()
                if not repo.git.pull():
                    shutil.rmtree(upgrade_script_path)
                    Repo.clone_from(server_link.split(" -b ")[0], to_path=upgrade_script_path,branch=project_branch)
                    printLog("pull fail, download the upgrade script")
                    return True
                else:
                    printLog("update the upgrade script")
                    return True
        else:
            if os.path.isdir(upgrade_script_path):
                shutil.rmtree(upgrade_script_path)
            Repo.clone_from(server_link.split(" -b ")[0], to_path=upgrade_script_path,branch=project_branch)
            printLog("download the upgrade script")
            return True


if __name__ == "__main__":
    if len(sys.argv) <= 1:
        printLog("not enough args")
        sys.exit(-1)
    configPath = sys.argv[1]
    os_name = platform.system()
    python_exe_file = None
    current_path = os.path.abspath(os.path.dirname(__file__))
    if os_name == "Linux":
        WORKSPACE = r"/data/MobileUpgrade"
        python_exe_file = "python3"
    elif os_name == "Windows":
        WORKSPACE = r"D:\MobileUpgrade"
        #python_exe_file = r"D:\DeviceTestTools\python\python.exe"
        python_exe_file = "python"

    server_link = "https://gitee.com/wenjun_HW/DeployTool_DevicePloy.git"
    param_dict = getAllParam(configPath)
    
    if not os.path.isdir(WORKSPACE):
        try:
            os.makedirs(WORKSPACE)
        except:
            time.sleep(1)
            pass
    
    '''================================update or download the upgrade script================================'''
    if param_dict.get("tool_version") == "False":
        is_update_script = False
    elif param_dict.get("tool_version"):
        is_update_script = True
    else:
        is_update_script = False
    printLog("is_update_script = %s" % is_update_script)
    project_name = parseProjectName(server_link)
    upgrade_script_path = os.path.join(WORKSPACE, project_name)
    ret = downloadUpgradeScripts(server_link, upgrade_script_path, is_update_script)
    if not ret:
        printLog("ERROR: download or update script failure")
        ret = -1
    else:
        '''================================upgrade the device================================'''
        printLog("INFO: ##############start to UpgradeScripts on your pc##############")
        excute_file = os.path.join(upgrade_script_path, "src", "controller.py")
        command = "%s -Bu %s %s" % (python_exe_file, excute_file, configPath)
        printLog(command)
        ret = os.system(command)
        printLog("INFO: ##############end to UpgradeScripts on your pc##############")
        printLog("result is %s" % ret)
        if ret != 0:
            printLog("ERROR: Upgrade failure")
            ret = -1
    exit(ret)