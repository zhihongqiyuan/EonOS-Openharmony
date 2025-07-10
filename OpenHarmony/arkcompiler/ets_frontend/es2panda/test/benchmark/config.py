#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (c) 2023 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import os

CUR_FILE_DIR = os.path.dirname(__file__)
DEFAULT_ES2ABC_TOOL = os.path.join(CUR_FILE_DIR, 'es2abc')
REPORT_DIR = os.path.join(CUR_FILE_DIR, 'output')
DEFAULT_TESTCASES_DIR = 'test_cases'
DEFAULT_OUTPUT_DIR = os.path.join(CUR_FILE_DIR, 'es2abc_output')
HERMES_CODE_PATH = os.path.join(CUR_FILE_DIR, 'hermes_workingdir')
DEFAULT_HERMES_PATH = os.path.join(HERMES_CODE_PATH, 'build', "bin", 'hermes')
HERMES_BUILD_PATH = os.path.join(HERMES_CODE_PATH, "build")
HERMES_BUILD_LOG_PATH = os.path.join(HERMES_CODE_PATH, "build.log")
HERMES_CLONE_PATH = os.path.join(HERMES_CODE_PATH, "hermes")
BUILD_HERMES_CMDS = cmds = [['cmake', '-S', HERMES_CLONE_PATH, '-B', HERMES_BUILD_PATH, '-G', 'Ninja',
                             "-DCMAKE_BUILD_TYPE=Release"],
                            ['cmake', '--build', HERMES_BUILD_PATH]]
DEFAULT_RUNS = 20
DEFAULT_PARAMS = ["--module", "--merge-abc"]
DEFAULT_HERMES_PARAMS = ["-emit-binary", "-out"]
JS_FILE_SUFFIX = ".js"
TS_FILE_SUFFIX = ".Ts"
HERMES_FILE_SUFFIX = ".hbc"
CASE_DATA_SUFFIX = "-data.js"
OCTANE = "Octane"
KRAKEN = "Kraken"
SUNSPIDER = "Sunspider"
KRAKENBENCHMARK_CASE_PATH = "tests"
KRAKEN_CASE_PATH = 'kraken-1.1'
SUNSPIDER_CASE_PATH = 'sunspider-0.9.1'
ABC_FILE_SUFFIX = ".abc"
TEST_RESULT_FILE = "test_result.html"
LOG_FILE = "test_result.txt"
PARSE_ONLY = "parse-only(Only do parsing actions)"
OPT_LEVEL = "opt-level(level of compilation optimization)"
COMP = "engine-comp(Compared with the Hermes engine)"
RUNS_NUM = "runs(Number of runs)"
SELECTED_PARAMETERS = "Runtime parameters"
SELECTED_PARAMETERS_BASE = "Selected parameters:"
DEFAULT_TIME = "Time"
AVERAGE_TIME = "Average time of one round"
GIT_DIR = ".git"
CLONE_DIR_SUFFIX = "_base"
ES2ABC = "es2abc"
HERMES = "hermes"
HERMES_GIT_URL = 'https://github.com/facebook/hermes.git'
CASE_URLS = [{"name": "Octane", "url": "https://github.com/chromium/octane.git", "dir": "."},
             {"name": "Kraken", "url": "https://github.com/mozilla/krakenbenchmark.mozilla.org.git", "dir": "test"},
             {"name": "Sunspider", "url": "https://github.com/mozilla/krakenbenchmark.mozilla.org.git", "dir": "test"}]

CASE_LIST = {
    "Octane": ['box2d.js', 'code-load.js', 'crypto.js', 'deltablue.js', 'gbemu.js', 'navier-stokes.js',
               'pdfjs.js', 'raytrace.js', 'regexp.js', 'richards.js', 'splay.js', 'typescript.js'],
    "Kraken": ['ai-astar.js', 'audio-beat-detection.js', 'audio-dft.js', 'audio-fft.js', 'audio-oscillator.js',
               'json-parse-financial.js', 'json-stringify-tinderbox.js', 'stanford-crypto-aes.js',
               'stanford-crypto-ccm.js',
               'stanford-crypto-pbkdf2.js', 'stanford-crypto-sha256-iterative.js'],
    "Sunspider": ['3d-cube.js', '3d-morph.js', '3d-raytrace.js', 'access-binary-trees.js', 'access-fannkuch.js',
                  'access-nbody.js', 'access-nsieve.js', 'bitops-3bit-bits-in-byte.js', 'bitops-bits-in-byte.js',
                  'bitops-bitwise-and.js', 'bitops-nsieve-bits.js', 'controlflow-recursive.js', 'crypto-aes.js',
                  'crypto-md5.js', 'crypto-sha1.js', 'date-format-tofte.js', 'date-format-xparb.js', 'math-cordic.js',
                  'math-partial-sums.js', 'math-spectral-norm.js', 'regexp-dna.js', 'string-base64.js',
                  'string-fasta.js', 'string-tagcloud.js', 'string-unpack-code.js', 'string-validate-input.js']}

HTML_TEMPLATE = """  
    <html>  
    <head>  
    <title>es2abc Benchmark Test Report</title>  
    </head>  
    <style>  
        body {  
            font-family: Arial, sans-serif;  
            margin: 0;  
            padding: 20px;  
        }  
        h1, h2 {  
            color: #333;    
        }  
        h1 {  
            text-align: center;  
        }  
        table {  
            border-collapse: collapse;  
            width: 100%;  
        }  
        th, td {  
            border: 1px solid #ccc;  
            padding: 8px;  
            text-align: left;  
        }  
        th {  
            background-color: #f2f2f2;  
        }
        #title {
            font-style: initial;
            font-size: 14px;
            color: #333;
        }  
    </style>     
    <body>
    <HTML_TABLE>
    <h1>es2abc-benchmark</h1> 
    <div id='title'> Selected parameters: </div>     
    """

HTML_TABLE_DEFAULT = """
    <table border="1">    
        <tr>    
            <th>Case</th>    
            <th>Number of runs</th>  
            <th>Time</th>    
            <th>Score</th>  
        </tr>
        """

HTML_TABLE_COMP = """
    <table border="1"> 
    <tr>    
        <th rowspan="2">Case</td>
        <th rowspan="2">Number of runs</th>  
        <th colspan="2">es2abc</th>
        <th colspan="2">hermes</th>   
    </tr>
    <tr>
        <th>Time</th>    
        <th>Score</th> 
        <th>Time</th>    
        <th>Score</th> 
    </tr>
    """

HTML_SCRIPT = """
    </table><script>function redirect(id){window.location.href = window.location.href.replace('test_result', id)};
    </script></body></html>
    """
