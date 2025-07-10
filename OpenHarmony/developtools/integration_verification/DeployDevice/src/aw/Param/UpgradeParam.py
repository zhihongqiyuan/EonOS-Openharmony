# encoding=utf-8


"""
======================================================================================
copyright (C) 2018-2019,  Huawei Technologies Co.
========================================================================================
@文件名称:    upgrade_param.py
@描述:       参数处理相关动作
@作者:       lwx587017
@生成日期:    20181015
======================================================================================
"""
import re


from util.log_info import logger


def validateParams(cust_param):
    '''
    #===================================================================================
    #   @Method:        validateParams(cust_param)
    #   @Precondition:  none
    #   @Func:          校验参数
    #   @PostStatus:    none
    #   @Param:         cust_param：升级需要的参数字典
    #   @eg:            validateParams({“upgrade_upgradeLocation”：“”})
    #   @return:        params_dict：所有的参数
    #===================================================================================
    '''
    for key in cust_param:
        if checkHz(cust_param.get(key)):
            logger.error("versionpath can't use Chinese")
            raise UpgradeException(ERROR_VERSION_USE_CHINAESE)
        if "Location" in key or "param1" in key:
            if not cust_param.get(key):
                logger.error("versionpath can't none")
                raise UpgradeException(ERROR_VERSION_IS_NONE)

            if len(cust_param.get(key)) > 255 :
                logger.error("versionpath name is too long")
                raise UpgradeException(ERROR_VERSION_PATH_TOO_LONG)
    return True

def checkHz(check_str):
    '''
    #===================================================================================
    #   @Method:        checkHz(check_str)
    #   @Precondition:  none
    #   @Func:          判断是否有中文
    #   @PostStatus:    none
    #   @Param:         check_str：判断的字符串
    #   @eg:            checkHz("xxxx")
    #   @return:        params_dict：所有的参数
    #===================================================================================
    '''
    for ch in check_str:
        if u'\u4e00' <= ch <= u'\u9fff':
            return True
    return False

def getAllParam(param_file):
    '''
    #===================================================================================
    #   @Method:        getAllParam(param_file)
    #   @Precondition:  none
    #   @Func:          解析参数文件获取所有参数
    #   @PostStatus:    none
    #   @Param:         param_file：参数文件
    #   @eg:            getAllParam(xxx.properties)
    #   @return:        params_dict：所有的参数
    #===================================================================================
    '''
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
                        if len(key) != 0 and len(value) != 0:
                            params_dict[key] = value

    #20200423 删除对参数进行特殊处理的代码

    return params_dict
    

def getCustParam(param_List, params_dict):
    '''
    #===================================================================================
    #   @Method:        getCustParam(param_List, params_dict)
    #   @Precondition:  none
    #   @Func:          获取用户定制参数
    #   @PostStatus:    none
    #   @Param:         param_List：用户给出的参数
    #                   params_dict: 参数字典
    #   @eg:            getCustParam([upgrade_upgradeLocation], {upgrade_upgradeLocation:“xxx”})
    #   @return:        cust_param：需要的参数
    #===================================================================================
    '''
    if not param_List:
        cust_param = params_dict
        return cust_param
    else:
        cust_param = {k: params_dict.get(k) for k in params_dict if k in param_List}
        return cust_param

