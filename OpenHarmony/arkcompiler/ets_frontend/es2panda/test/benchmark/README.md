# es2abc-benchmark

## Project Description
The purpose of this project is to test the performance of es2abc when compiling JS and output a report of the test results.  
This tool relies on Python3, Git, and the es2abc executable file under the ets_frontend component.  
By default, test cases are stored in the 'test_cases' directory, and run results are stored in the 'output' directory.  
## Usage notes
#### Parameter description
|   Parameter   |                         Description                          |  Type   |            Default Value            |
| :-----------: | :----------------------------------------------------------: | :-----: | :---------------------------------: |
| --es2abc-tool | This parameter is required and requires a path to the es2abc tool to be provided. | string  |                 N/A                 |
|    --runs     | This parameter is optional and the default value is 20. Represents the number of times a use case was executed in a test. |   int   |                 20                  |
|  --opt-level  | This parameter is optional and specifies the optimization level of the compiler. | string  |                  0                  |
| --parse-only  | This parameter is optional and the default value is False. If set to True, only the input file will be parsed during testing. | boolean |                False                |
| --engine-comp  | This parameter is optional, and the default value is False. At this time, only the es2abc test is performed. When it is True, it is compared with the Hermes engine. | boolean |                False                |

#### Description of the test case
    The test suite and test cases for this project are sourced from open source libraries:
#### Test Suite:
  | name |                                                                                                            description                                                                                                            | GitHub address |
  | :--: |:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:| :--: |
  | Octane |                                                          JavaScript benchmarking suite for evaluating the performance of browsers in handling complex JavaScript tasks.                                                           | [https://github.com/chromium/octane](https://github.com/chromium/octane) |
  | Kraken |                                     JavaScript performance test suite, including a series of test cases covering dynamic scenarios, garbage collection, encryption, and graphics operations.                                      | [https://github.com/mozilla/krakenbenchmark.mozilla.org](https://github.com/mozilla/krakenbenchmark.mozilla.org) |
  | SunSpider |                                                     JavaScript performance testing suite, used to evaluate the performance of JavaScript engines in performing common tasks.                                                      | [https://github.com/mozilla/krakenbenchmark.mozilla.org](https://github.com/mozilla/krakenbenchmark.mozilla.org) |
#### Comparison Engine
    When running this benchmark, you can compare the performance of es2abc compilation with that of Hermes compilation. Hermes engine GitHub address: https://github.com/facebook/hermes

#### Run an example
The script runtime depends on the es2abc executable after the ets_frontend component is compiled. es2abc compilation command:
```
$ ./build.sh --product-name rk3568 --build-target ets_frontend_build
```
Specify the es2abc executable and run the 'run_benchmark.py' for testing:
```
$ python3 run_benchmark.py --es2abc-tool  /out/xx/xx/xx/ets_frontend/es2abc
```
Compare with Hermes Engine：
```
$ python3 run_benchmark.py --es2abc-tool  /out/xx/xx/xx/ets_frontend/es2abc --engine-comp
```
Specify the number of rounds of test case compilation (20 rounds by default):
```
$ python3 run_benchmark.py --es2abc-tool  /out/xx/xx/xx/ets_frontend/es2abc --runs 10  // It can be modified to the required number of compilation rounds
```
--parse-only only parses the test case, and when the --engine-comp parameter is used, this parameter is invalid:
```
$  python3 run_benchmark.py --es2abc-tool  /out/xx/xx/xx/ets_frontend/es2abc --parse-only
```
--opt-level specifies the compilation optimization level, and when the --engine-comp parameter is used, this parameter is invalid:
```
$  python3 run_benchmark.py --es2abc-tool  /out/xx/xx/xx/ets_frontend/es2abc --opt-level '2'
```
The generated test results are stored in the 'output' directory, and the command line prints basic test information, such as:

```plain
> > > benchmark running
Running navier-stokes....
Running typescript....
Running mandreel....
```
### Precautions
    This tool relies on ets_frontend components to compile the es2abc executable file. and you need to specify the es2abc path when using it.
    The test cases for this tool are open source cases, and the test suite is automatically pulled during run time.

