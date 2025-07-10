/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <getopt.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "securec.h"
#include "syscap_tool.h"
#include "create_pcid.h"
#include "context_tool.h"

#define SYSCAP_VERSION "2.0.1"
#define OUTPUT_VERSION_LEN 200
#define ENCODE 0
#define DECODE 1
#define STRING_DECODE 2
#define RPCID 3
#define PCID 4
#define PCID_STRING 5
#define RPCID_STRING 6
#define VERSION 7
#define INPUT_FILE 8
#define OUTPUT_FILE 9
#define HELP 10
#define INPUT_FILE_NUM 4

static void PrintHelp(void);
static void PrintVersion(void);
static void OutputVersion(const char *arg, int opt);
static void OutputHelp(void);

void SetBitMap(char *const *argv, int32_t flag, uint16_t *bitMap, char **outputpath);

int32_t OperateByBitMap(char *const *argv, uint16_t bitMap, char *outputpath);

static struct option g_longOptions[] = {
        {"help",           no_argument,       0,  'h' },
        {"version",        no_argument,       0,  'v' },
        {"rpcid",          no_argument,       0,  'R' },
        {"pcid",           no_argument,       0,  'P' },
        {"compare",        required_argument, 0,  'C' },
        {"encode",         no_argument,       0,  'e' },
        {"decode",         no_argument,       0,  'd' },
        {"string",         no_argument,       0,  's' },
        {"input",          required_argument, 0,  'i' },
        {"output",         required_argument, 0,  'o' },
        {0,                0,                 0,  0   }
};

static struct {
    char *inputfile;
    char *pcidfile;
    char *rpcidfile;
} g_customerfileinfo;

int main(int argc, char **argv)
{
    int32_t optIndex;
    int32_t ret;
    uint16_t bitMap = 0x0;
    char curpath[PATH_MAX] = {0};
    g_customerfileinfo.inputfile = NULL;
    g_customerfileinfo.pcidfile = NULL;
    g_customerfileinfo.rpcidfile = NULL;
    char *outputpath = getcwd(curpath, sizeof(curpath));
    if (outputpath == NULL) {
        PRINT_ERR("Get outputpath failed.\n");
        return -1;
    }

    while (1) {
        int32_t flag = getopt_long(argc, argv, "hvRPC:edsi:o:", g_longOptions, &optIndex);
        if (flag == -1) {
            break;
        }
        if (flag == 'C') {
            if (argc != INPUT_FILE_NUM || optind < 0 || optind >= argc) {  // 4, argc of ./syscap_tool -C f1 f2
                PRINT_ERR("Input file too few or too many.\n");
                return -1;
            }
        }
        SetBitMap(argv, flag, &bitMap, &outputpath);
    }
    ret = OperateByBitMap(argv, bitMap, outputpath);
    if (ret != 0) {
        PRINT_ERR("syscap_tool failed to complete. input: %s\n", g_customerfileinfo.inputfile);
    }
    return ret;
}

int32_t OperateByBitMap(char *const *argv, uint16_t bitMap, char *outputpath)
{
    int32_t ret = 0;
    switch (bitMap) {
        case 0x109: // 0x109, -Rei inputfile
            ret = RPCIDEncode(g_customerfileinfo.inputfile, outputpath); break;
        case 0x10A: // 0x10A, -Rdi inputfile
            ret = RPCIDDecode(g_customerfileinfo.inputfile, outputpath); break;
        case 0x10D: // 0x10D, -Resi inputfile
            ret = EncodeRpcidscToString(g_customerfileinfo.inputfile, outputpath); break;
        case 0x115: // 0x115, -Pesi inputfile
            ret = EncodePcidscToString(g_customerfileinfo.inputfile, outputpath); break;
        case 0x60:  // 0x60,  -C pcid.txt rpcid.txt
            ret = ComparePcidWithRpcidString(g_customerfileinfo.pcidfile, g_customerfileinfo.rpcidfile, TYPE_FILE);
            break;
        case 0x64:  // 0x64,  -sC "pcidstring" "rpcidstring"
            ret = ComparePcidWithRpcidString(g_customerfileinfo.pcidfile, g_customerfileinfo.rpcidfile, TYPE_STRING);
            break;
        case 0x111: // 0x111, -Pei inputfile
            ret = CreatePCID(g_customerfileinfo.inputfile, outputpath); break;
        case 0x112: // 0x112, -Pdi inputfile
            ret = DecodePCID(g_customerfileinfo.inputfile, outputpath); break;
        case 0x116: // 0x116, -Pdsi inputfile
            ret = DecodeStringPCIDToJson(g_customerfileinfo.inputfile, outputpath); break;
        case 0x10E: // 0x10E, -Rdsi inputfile
            printf("-Rdsi is not support currently.\n"); break;
        case 0x80:  // 0x80,  -v
            if (optind < 0 || optind >= INPUT_FILE_NUM) {
                PRINT_ERR("Input file too few or too many.\n");
                return -1;
            } else {
                (void)OutputVersion(argv[optind], optind);
                break;
            }
        default:
            (void)OutputHelp();
    }
    return ret;
}

void SetBitMap(char *const *argv, int32_t flag, uint16_t *bitMap, char **outputpath)
{
    switch (flag) {
        case 'e':
            (*bitMap) |= 0x1 << ENCODE;
            break;
        case 'd':
            (*bitMap) |= 0x1 << DECODE;
            break;
        case 's':
            (*bitMap) |= 0x1 << STRING_DECODE;
            break;
        case 'R':
            (*bitMap) |= 0x1 << RPCID;
            break;
        case 'P':
            (*bitMap) |= 0x1 << PCID;
            break;
        case 'C':
            (*bitMap) |= 0x1 << PCID_STRING;
            (*bitMap) |= 0x1 << RPCID_STRING;
            g_customerfileinfo.pcidfile = optarg;
            g_customerfileinfo.rpcidfile = argv[optind];
            break;
        case 'i':
            (*bitMap) |= 0x1 << INPUT_FILE;
            g_customerfileinfo.inputfile = optarg;
            break;
        case 'o':
            (*outputpath) = optarg;
            break;
        case 'v':
            (*bitMap) |= 0x1 << VERSION;
            break;
        case 'h':
        default:
            (*bitMap) |= 0x1 << HELP;
    }
}

void PrintVersion(void)
{
    char outputVersion[OUTPUT_VERSION_LEN] = {0};
    int ret = sprintf_s(outputVersion, OUTPUT_VERSION_LEN, "syscap_tool v%s", SYSCAP_VERSION);
    if (ret == -1) {
        PRINT_ERR("sprintf_s failed.\n");
        exit(-1);
    }
    printf("%s\n", outputVersion);
}

void PrintHelp(void)
{
    printf("syscap_tool -R/P -e/d -i filepath [-o outpath]\n");
    printf("-h, --help\t: how to use\n");
    printf("-R, --rpcid\t: encode or decode rpcid\n");
    printf("-P, --pcid\t: encode or decode pcid\n");
    printf("-C, --compare\t: compare pcid with rpcid string format.\n\t"
           "-s, --string : input string.\n");
    printf("-e, --encode\t: encode to sc format.\n\t-s, --string : encode to string format.\n");
    printf("-d, --decode\t: decode to json format.\n\t-s, --string : decode string format.\n");
    printf("-i filepath, --input filepath\t: input file\n");
    printf("-o outpath, --input outpath\t: output path\n");
    printf("-v, --version\t: print syscap_tool version information.\n");
}

void OutputVersion(const char *arg, int opt)
{
    if (arg != NULL && opt > 1) {
        printf("Try 'syscap_tool --help' for more information.\n");
    } else {
        (void)PrintVersion();
    }
}

void OutputHelp(void)
{
    (void)PrintHelp();
    printf("\n");
    (void)PrintVersion();
}