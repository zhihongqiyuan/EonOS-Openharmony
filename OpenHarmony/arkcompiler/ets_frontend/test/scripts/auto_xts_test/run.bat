@REM Copyright (c) 2023 Huawei Device Co., Ltd.
@REM Licensed under the Apache License, Version 2.0 (the "License");
@REM you may not use this file except in compliance with the License.
@REM You may obtain a copy of the License at
@REM
@REM http://www.apache.org/licenses/LICENSE-2.0
@REM
@REM Unless required by applicable law or agreed to in writing, software
@REM distributed under the License is distributed on an "AS IS" BASIS,
@REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
@REM See the License for the specific language governing permissions and
@REM limitations under the License.

@echo off
SETLOCAL ENABLEEXTENSIONS

REM change to work directory
cd /d %~dp0

REM log
echo "------------------------------------------------" >> log.txt

REM run XTStest
set var=D:\AutoXTSTest
timeout /t 15
hdc shell "power-shell setmode 602"
hdc shell "hilog -Q pidoff"
cd /d %~dp0
for /f "tokens=1,2 delims==" %%i in (running_modules.txt) do (
if "%%i"=="modules" set value=%%j
)
call %var%\dayu200_xts\suites\acts\acts\run.bat run -l %value%

REM get result
cd /d %~dp0
echo "Successfully excute script" >> log.txt
if exist result (rd /s /q result)
md result
python result.py
ENDLOCAL
exit



