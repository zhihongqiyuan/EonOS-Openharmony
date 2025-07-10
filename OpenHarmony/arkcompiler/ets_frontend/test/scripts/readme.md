# Automatic tests for SDK

## Purpose
1. Download daily sdk
2. Run xts test which in auto_xts_test
3. Run sdk_test which in sdk_test
4. Run performance test which in performance_test
5. Run spider script which in get commit log
6. Send the result by Email

## Usage
### How to work
In entry.py, it will run downloading sdk，xts， sdk_test，performance_test, get commit log script, and send email, you can delete any of them if you don't need run the test.  
If you do not want to run test immediately, delete run_all()  
Set email infos in email_config  
### Note
It will find the daily build in last 24 hours, if failed to download, the test will stop.  
Do not close the cmd or terminal, or it will not run at the time you specified.  
All tests can run without others, read the seperate readme for more details.  
