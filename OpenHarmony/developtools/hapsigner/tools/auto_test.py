#!/usr/bin/env python
# coding=utf-8
##############################################
# Copyright (c) 2021-2022 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##############################################
import json
import os.path
import random
import sys
import time
import ast
import re
import tempfile
from subprocess import Popen


def print_help():
    content = "\n" \
              "Usage: signtool.jar -scope <simple|all|`component`> -n <round, default 1> <--random>\n" \
              "    signtool.jar : Main progress jar file\n" \
              "    component: \n" \
              "    --random: random test, default false" \
              "\n" \
              "Example: \n" \
              "    signtool.jar \n" \
              "    signtool.jar -runtest\n" \
              "    signtool.jar -scope all -n 1000\n" \
              "    signtool.jar -scope generate-profile-cert\n" \
              "    signtool.jar -n 50 --random\n" \
              "\n"

    print(content)
    pass


random_scope = {
    'generate-keypair': {
        'required': {
            'keyAlias': 'oh-app1-key-v1',
            'keyAlg': ["RSA", "ECC"],
            'keySize': ["2048", "3072", "4096", "NIST-P-256", "NIST-P-384"],
            'keystoreFile': ['ohtest.jks', 'ohtest.p12']
        },
        'others': {
            'keyPwd': '123456',
            'keystorePwd': '123456'
        }
    },
    'generate-csr': {
        'required': {
            'keyAlias': 'oh-app1-key-v1',
            'signAlg': ["SHA256withRSA", "SHA384withRSA", "SHA256withECDSA", "SHA384withECDSA"],
            'subject': "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release",
            'keystoreFile': ['ohtest.jks', 'ohtest.p12'],
            'outFile': 'oh-app1-key-v1.csr'
        },
        'others': {
            'keyPwd': '123456',
            'keystorePwd': '132456'
        }
    },
    'generate-ca': {
        'required': {
            'keyAlias': ['oh-ca-key-v1', "oh-app-sign-srv-ca-key-v1"],
            'signAlg': ["SHA256withRSA", "SHA384withRSA", "SHA256withECDSA", "SHA384withECDSA"],
            'keyAlg': ['RSA', 'ECC'],
            'keySize': ["2048", "3072", "4096", "NIST-P-256", "NIST-P-384"],
            'subject': ["C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release",
                        "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA"],
            'keystoreFile': ['ohtest.jks', 'ohtest.p12'],
            'outFile': 'app-sign-srv-ca.cer'
        },
        'others': {
            'keyPwd': '123456',
            'keystorePwd': '132456',
            'issuer': 'C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA',
            'issuerKeyAlias': 'oh-app-sign-srv-ca-key-v1',
            'issuerKeyPwd': '123456',
            'validity': '365',
            'basicConstraintsPathLen': '2'
        }
    },
    'generate-cert': {
        'required': {
            'keyAlias': ['oh-sub-key-v1', 'oh-ca-key-v1'],
            'signAlg': ["SHA256withRSA", "SHA384withRSA", "SHA256withECDSA", "SHA384withECDSA"],
            'subject': "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release",
            'issuer': 'C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA',
            'issuerKeyAlias': 'oh-ca-key-v1',
            'extKeyUsage': 'codeSignature',
            'keyUsage': ['digitalSignature,nonRepudiation,keyEncipherment',
                         'dataEncipherment,keyAgreement, certificateSignature, crlSignature',
                         'encipherOnly, encipherOnly'],
            'keystoreFile': ['ohtest.jks', 'ohtest.p12'],
            'outFile': 'app1.cer'
        },
        'others': {
            'extKeyUsage': ['serverAuthentication', 'clientAuthentication', 'emailProtection'],
            'extKeyUsageCritical': ['false', 'true'],
            'keyUsageCritical': ['false', 'true'],
            'issuerKeyPwd': '123456',
            'keyPwd': '123456',
            'validity': '365',
            'keystorePwd': '123456'
        }
    }
}

simple_scope = {
    'generate-keypair': [
        'generate-keypair -keyAlias "oh-app1-key-v1" -keyPwd 123456 -keyAlg ECC -keySize NIST-P-256 '
        '-keystoreFile "./test1/ohtest.jks" -keystorePwd 123456',

        'generate-keypair -keyAlias "oh-profile1-key-v1" -keyPwd 123456 -keyAlg ECC -keySize NIST-P-384 '
        '-keystoreFile "./test1/ohtest.jks" -keystorePwd 123456',

        'generate-keypair -keyAlias "oh-app2-key-v1" -keyPwd 123456 -keyAlg RSA -keySize 2048 '
        '-keystoreFile  "./test2/ohtest.p12" -keystorePwd 123456',

        'generate-keypair -keyAlias "oh-profile2-key-v1" -keyPwd 123456 -keyAlg RSA -keySize 4096 '
        '-keystoreFile  "./test2/ohtest.p12" -keystorePwd 123456'
    ],
    'generate-csr': [
        'generate-csr -keyAlias "oh-app1-key-v1" -keyPwd 123456 -subject  '
        '"C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release" -signAlg SHA256withECDSA  '
        '-keystoreFile  "./test1/ohtest.jks" -keystorePwd 123456 -outFile "./test1/oh-app1-key-v1.csr"',

        'generate-csr -keyAlias "oh-profile2-key-v1" -keyPwd 123456 -subject  '
        '"C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release" -signAlg SHA256withRSA  '
        '-keystoreFile  "./test2/ohtest.p12" -keystorePwd 123456 -outFile "./test2/oh-profile2-key-v1.csr"'
    ],
    'generate-ca': [
        # Root CA in ohtest.jks
        'generate-ca -keyAlias "oh-root-ca-key-v1" -subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA" '
        '-validity 365 -signAlg SHA384withECDSA  -keystoreFile  "./test1/ohtest.jks" -keystorePwd 123456  '
        '-outFile "./test1/root-ca1.cer" -keyAlg ECC -keySize NIST-P-256',
        # Sub app cert in ohtest.jks
        'generate-ca -keyAlias "oh-app-sign-srv-ca-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Root CA" -issuerKeyAlias "oh-root-ca-key-v1" -subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN= Application Signature Service CA" -validity 365 -signAlg SHA384withECDSA  '
        '-keystoreFile  "./test1/ohtest.jks" -keystorePwd 123456  -outFile "./test1/app-sign-srv-ca1.cer" '
        '-keyAlg ECC -keySize NIST-P-256',
        # Sub profile cert in ohtest.jks
        'generate-ca -keyAlias "oh-profile-sign-srv-ca-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Root CA" -issuerKeyAlias "oh-root-ca-key-v1" -subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN= Profile Signature Service CA" -validity 365 -signAlg SHA384withECDSA  -keystoreFile  "./test1/ohtest.jks" '
        '-keystorePwd 123456  -outFile "./test1/profile-sign-srv-ca1.cer" -keyAlg ECC -keySize NIST-P-384',

        # Root CA in ohtest.p12
        'generate-ca -keyAlias "oh-root-ca2-key-v1" -subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA" '
        '-validity 365 -signAlg SHA384withRSA  -keystoreFile  "./test2/ohtest.p12" -keystorePwd 123456  '
        '-outFile "./test2/root-ca2.cer" -keyAlg RSA -keySize 2048',
        # Sub app cert in ohtest.p12
        'generate-ca -keyAlias "oh-app-sign-srv-ca2-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Root CA" -issuerKeyAlias "oh-root-ca2-key-v1" -subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN= Application Signature Service CA" -validity 365 -signAlg SHA384withRSA  '
        '-keystoreFile  "./test2/ohtest.p12" -keystorePwd 123456  -outFile "./test2/app-sign-srv-ca2.cer" '
        '-keyAlg RSA -keySize 2048',
        # Sub profile cert in ohtest.p12
        'generate-ca -keyAlias "oh-profile-sign-srv-ca2-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Root CA" -issuerKeyAlias "oh-root-ca2-key-v1" -subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN= Profile Signature Service CA" -validity 365 -signAlg SHA384withRSA  -keystoreFile  "./test2/ohtest.p12" '
        '-keystorePwd 123456  -outFile "./test2/profile-sign-srv-ca2.cer" -keyAlg RSA -keySize 2048'
    ],
    'generate-cert': [
        # Self-Definition cert - Root CA
        'generate-cert -keyAlias "oh-app1-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA" '
        '-issuerKeyAlias "oh-app1-key-v1" -issuerKeyPwd 123456 '
        '-subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA" -validity 365 '
        '-keyUsage "certificateSignature, crlSignature" -signAlg SHA256withECDSA  '
        '-keystoreFile  "./test1/ohtest.jks" -keystorePwd 123456 -outFile "./test1/single-root.cer" -keyPwd 123456',
        # Self-definition sign cert - app cert
        'generate-cert -keyAlias "oh-app1-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Application Signature Service CA" -issuerKeyAlias "oh-app-sign-srv-ca-key-v1" '
        '-subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release" -validity 365 '
        '-keyUsage digitalSignature -extKeyUsage codeSignature -signAlg SHA256withECDSA  '
        '-keystoreFile  "./test1/ohtest.jks" -keystorePwd 123456 -outFile "./test1/single-app1.cer" -keyPwd 123456'
    ],
    'generate-app-cert': [
        # App sign cert via ohtest.jks
        'generate-app-cert -keyAlias "oh-app1-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Application Signature Service CA" -issuerKeyAlias "oh-app-sign-srv-ca-key-v1" -subject '
        '"C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release" -validity 365 -signAlg SHA256withECDSA  '
        '-keystoreFile  "./test1/ohtest.jks" -keystorePwd 123456 -outFile "./test1/app1.cer" -keyPwd 123456 '
        '-outForm cert ',
        # App sign cert chain via ohtest.jks
        'generate-app-cert -keyAlias "oh-app1-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Application Signature Service CA" -issuerKeyAlias "oh-app-sign-srv-ca-key-v1" -subject '
        '"C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release" -validity 365 -signAlg SHA256withECDSA  '
        '-keystoreFile  "./test1/ohtest.jks" -keystorePwd 123456 -outFile "./test1/app-release1.pem" '
        '-subCaCertFile ./test1/app-sign-srv-ca1.cer -outForm certChain '
        '-rootCaCertFile ./test1/root-ca1.cer -keyPwd 123456',
        # App sign cert via ohtest.p12
        'generate-app-cert -keyAlias "oh-app2-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Application Signature Service CA" -issuerKeyAlias "oh-app-sign-srv-ca2-key-v1" -subject '
        '"C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release" -validity 365 -signAlg SHA256withECDSA  '
        '-keystoreFile  "./test2/ohtest.p12" -keystorePwd 123456 -outFile "./test2/app2.cer" -keyPwd 123456 '
        '-outForm cert ',
        # App sign cert chain via ohtest.p12
        'generate-app-cert -keyAlias "oh-app2-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Application Signature Service CA" -issuerKeyAlias "oh-app-sign-srv-ca2-key-v1" -subject '
        '"C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release" -validity 365 -signAlg SHA256withECDSA  '
        '-keystoreFile  "./test2/ohtest.p12" -keystorePwd 123456 -outFile "./test2/app-release2.pem" '
        '-subCaCertFile ./test2/app-sign-srv-ca2.cer -outForm certChain '
        '-rootCaCertFile ./test2/root-ca2.cer -keyPwd 123456'
    ],
    'generate-profile-cert': [
        # Profile sign cert via ohtest.jks
        'generate-profile-cert -keyAlias "oh-profile1-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Profile Signature Service CA" -issuerKeyAlias "oh-profile-sign-srv-ca-key-v1" '
        '-subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release" '
        '-validity 365 -signAlg SHA256withECDSA  -keystoreFile  "./test1/ohtest.jks" '
        '-keystorePwd 123456 -outFile "./test1/profile1.cer" -keyPwd 123456 -outForm cert ',
        # Profile sign cert chain via ohtest.jks
        'generate-profile-cert -keyAlias "oh-profile1-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Profile Signature Service CA" -issuerKeyAlias "oh-profile-sign-srv-ca-key-v1" '
        '-subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release" -validity 365 '
        '-signAlg SHA256withECDSA -keystoreFile  "./test1/ohtest.jks" '
        '-keystorePwd 123456 -outFile "./test1/profile-release1.pem" '
        '-subCaCertFile "./test1/profile-sign-srv-ca1.cer" -outForm certChain '
        '-rootCaCertFile "./test1/root-ca1.cer" -keyPwd 123456',
        # Profile sign cert via ohtest.p12
        'generate-profile-cert -keyAlias "oh-profile2-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Profile Signature Service CA" -issuerKeyAlias "oh-profile-sign-srv-ca2-key-v1" '
        '-subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile2 Release" '
        '-validity 365 -signAlg SHA256withECDSA  -keystoreFile  "./test2/ohtest.p12" '
        '-keystorePwd 123456 -outFile "./test2/profile2.cer" -keyPwd 123456 -outForm cert ',
        # Profile sign cert chain via ohtest.p12
        'generate-profile-cert -keyAlias "oh-profile2-key-v1" -issuer "C=CN,O=OpenHarmony,OU=OpenHarmony Community,'
        'CN=Profile Signature Service CA" -issuerKeyAlias "oh-profile-sign-srv-ca2-key-v1" '
        '-subject  "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile2 Release" -validity 365 '
        '-signAlg SHA256withECDSA  -keystoreFile  "./test2/ohtest.p12" '
        '-keystorePwd 123456 -outFile "./test2/profile-release2.pem" -subCaCertFile "./test2/profile-sign-srv-ca2.cer" '
        '-outForm certChain -rootCaCertFile "./test2/root-ca2.cer" -keyPwd 123456'
    ],
    'sign-profile': [
        'sign-profile -mode localSign -keyAlias "oh-profile1-key-v1" -profileCertFile "./test1/profile-release1.pem" '
        '-inFile  "profile.json" -signAlg SHA256withECDSA  -keystoreFile  "./test1/ohtest.jks" -keystorePwd 123456 '
        '-outFile "./test1/app1-profile1.p7b"  -keyPwd 123456'
    ],
    'verify-profile': [
        'verify-profile -inFile "./test1/app1-profile1.p7b"'
    ]
}


def get_test_scope_from_file():
    with open('commands.config', 'r', encoding='utf-8') as f:
        content = f.read()
        return ast.literal_eval(content)


test_scope = get_test_scope_from_file()

test_result = {}


def run_target(case, cmd):
    if not test_result.get(case, None):
        test_result[case] = {'times': 0, 'total_cost': 0, 'success': 0, 'fail': 0}

    case_result = test_result.get(case)
    case_result['times'] = case_result['times'] + 1
    start = time.time()

    out_temp = tempfile.TemporaryFile(mode='w+')
    command = Popen(cmd, stdout=out_temp, stderr=out_temp, stdin=out_temp, shell=False)
    success = True
    code = command.wait()
    if code != 0:
        success = False

    log_name = "log.txt"
    if not success:
        log_name = "error.txt"

    out_temp.seek(0)
    out = out_temp.readlines()
    with open(log_name, mode='a+', encoding='utf-8') as f:
        if len(out) > 0:
            f.writelines(' '.join(cmd) + "\r\n")
        for line in out:
            f.writelines(str(line.strip()) + "\r\n")
    out_temp.close()

    end = time.time()
    case_result['total_cost'] = case_result['total_cost'] + (end - start)
    out_temp.close()
    if success:
        case_result['success'] = case_result['success'] + 1
    else:
        case_result['fail'] = case_result['fail'] + 1
    return success


def run_simple_case(case, jar_file):
    test_case = simple_scope.get(case, None)
    if not test_case:
        print("Not found test case: {}".format(case))
        exit(0)

    for k in test_case:
        cmd = ['java', '-jar', jar_file] + [s.replace('"', '') for s in (re.split(r' \s*(?![^"]*\" )', k.strip()))]
        print("== Run command: {}".format(cmd))
        result = run_target(case, cmd)
        print("== Done command: {}".format(result))


def run_test_case(case, jar_file):
    test_case = test_scope.get(case, None)
    if not test_case:
        print("Not found test case: {}".format(case))
        exit(0)

    for k in test_case:
        cmd = ['java', '-jar', jar_file] + [s.replace('"', '') for s in (re.split(r' \s*(?![^"]*\" )', k.strip()))]
        print("== Run command: {}".format(cmd))
        result = run_target(case, cmd)

        with open('test_result.log', 'r', encoding='utf-8') as f:
            content = f.read()
            test_result_dict = ast.literal_eval(content)

        if case == 'case-assert-true':
            if result:
                print("== Done command: Expected True and tested True")
            else:
                test_result_dict['commands_expected_True_but_tested_False'].append(cmd)
                print("== Done command: Expected True but tested False")
        else:
            if result:
                test_result_dict['commands_expected_False_but_tested_True'].append(cmd)
                print("== Done command: Expected False but tested True")
            else:
                print("== Done command: Expected False and tested False")

        with open("test_result.log", mode='w', encoding='utf-8') as tr:
            tr.write(json.dumps(test_result_dict, indent=4))


def random_str():
    strs = "abcdefghjiklmnopqstuvwxyzABCDEFGHIJKLMNOPQRS TUVWXYZ1234567890~!@#ls%^&*()_+,./<>?;':"
    result = ''
    for i in range(random.randint(1, 30)):
        result = result + random.choice(strs)
    return result


def run_random_case(case, jar_file):
    test_case = random_scope.get(case, None)
    if not test_case:
        print("Not found test case: {}".format(case))
        exit(0)

    cmd = ['java', '-jar', jar_file, case]
    for k, v in test_case.get('required').items():
        r = random.choice(['none', 'choice', 'choice', 'random'])
        if r == 'choice':
            cmd.append('-'.join(k))
            cmd.append(random.choice(v) if isinstance(v, list) else v)
        elif r == 'random':
            cmd.append('-'.join(k))
            cmd.append(random_str())

    for k, v in test_case.get('others').items():
        r = random.choice(['none', 'choice', 'choice', 'random'])
        if r == 'choice':
            cmd.append('-'.join(k))
            cmd.append(random.choice(v) if isinstance(v, list) else v)
        elif r == 'random':
            cmd.append('-'.join(k))
            cmd.append(random_str())

    print("== Run command: {}".format(cmd))
    result = run_target(case, cmd)
    print("== Done command: {}".format(result))


def run_all_case(case, jar_file):
    test_case = random_scope.get(case, None)
    if not test_case:
        print("Not found test case: {}".format(case))
        exit(0)

    cmd = ['java', '-jar', jar_file, case]
    for ak, av in test_case.get('required').items():
        cmd.append('-'.join(ak))
        cmd.append(random.choice(av) if isinstance(av, list) else av)

    print("== Run command: {}".format(cmd))
    result = run_target(case, cmd)
    print("== Done command: {}".format(result))


def prepare_env():
    test_dirs = ['test1', 'test2']
    for test_dir in test_dirs:
        if not os.path.exists(test_dir):
            os.mkdir(test_dir)

        for key_file in ['ohtest.jks', 'ohtest.p12']:
            target_file = os.path.join(test_dir, key_file)
            if os.path.exists(target_file):
                os.remove(target_file)
            if os.path.exists(key_file):
                os.remove(key_file)


def process_cmd(args):
    run_round: int = 1
    run_scope: str = 'simple'
    is_random: bool = False

    if len(args) <= 1 or ('.jar' not in args[1]) or '--help' == args[1] or '-h' == args[1]:
        print_help()
        exit(0)

    jar_file = args[1]
    if not os.path.exists(jar_file):
        print("Jar file '{}' not found".format(jar_file))
        exit(0)

    if len(args) >= 3:
        temp_round, temp_scope, temp_random = get_run_format(args)
        run_round: int = temp_round
        run_scope: str = temp_scope
        is_random: bool = temp_random

    print('===  Start testing  ===')
    print('Scope: {}. Round: {}. Random: {}'.format(run_scope, run_round, is_random))

    if os.path.exists('log.txt'):
        os.remove('log.txt')
    if os.path.exists('error.txt'):
        os.remove('error.txt')

    for i in range(run_round):
        if run_scope == 'all':
            for r_scope, _ in random_scope.items():
                run_all_case(r_scope, jar_file)
        elif is_random:
            for r_scope, _ in random_scope.items():
                run_random_case(r_scope, jar_file)
        elif run_scope == 'simple':
            prepare_env()
            for s_scope, _ in simple_scope.items():
                run_simple_case(s_scope, jar_file)
        elif run_scope == 'runtest':
            prepare_env()
            with open("test_result.log", mode='w', encoding='utf-8') as file_result:
                result_dict = {
                    'commands_expected_True_but_tested_False': [],
                    'commands_expected_False_but_tested_True': []
                }
                file_result.write(str(result_dict))
            for t_scope, _ in test_scope.items():
                run_test_case(t_scope, jar_file)
        else:
            run_simple_case(run_scope, jar_file)


def get_run_format(args):
    run_round: int = 1
    run_scope: str = 'simple'
    is_random: bool = False
    try:
        for i in range(2, len(args), 1):
            if args[i] == '-n':
                run_round = int(args[i + 1])
            elif args[i] == '-scope':
                run_scope = args[i + 1]
            elif args[i] == '--random':
                is_random = True
            elif args[i] == '-runtest':
                run_scope = 'runtest'
    except IndexError:
        print_help()
        exit(0)
    return run_round, run_scope, is_random


if __name__ == '__main__':
    process_cmd(sys.argv)
    print("All test done")
    print("========================")
    for rk, rv in test_result.items():
        times = rv['times']
        if times != 0:
            print("Case {}, run times: {}, avg cost: {}s, total success: {}, total fail: {}".format(rk, times, round(
                rv['total_cost'] / times, 2), rv['success'], rv['fail']))
    print("========================")
    print("See log.txt / error.txt")
