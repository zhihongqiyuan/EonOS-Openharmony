# compiler unit testing

**The re-built compiler unit testing for ace2.0.**

## Usage

Here are simplified instructions of how to get started. The following commands work both on **Windows** and **Linux** platforms.

### 1. Install

First, enter the root directory of the compiler:
```
$ cd compiler/
```
And then install the npm dependencies(You must have node&npm installed):
```
$ npm install
```
**Note**: If some errors occur, delete the generated package `node_modules`, config npm proxy and run `npm install` again.
```
npm config set proxy http://username:password@server:port
npm confit set https-proxy http://username:password@server:port
```

### 2. Quick Start

In the root directory of `compiler/`:
```
$ npm run etsTest
```
All test files in the `compiler/test/transform_ut`  will be tested.

There are three test scenes, which is the same as the test cases with script command `npm run test`, are:

- `ut`: Testing compiled .js file result from corresponding .ets files in the non-partial-update mode.
- `utForPartialUpdate`: Testing compiled .js file result from corresponding .ets files in the partial update mode.
- `utForValidate`: Testing expected catched error/warning logs result when compiling .ets files in the partial update mode.

### 3. Test Framework Overview

The re-built test framework is to mock a rollup object and create a DevEco sample application for testing. 

In each test scene, the framework first configures shared parameters, then calls `buildStart()` hook from `etsChecker` rollup plugin. Then for each test case, the framework calls `transform()` hook from `etsTransform` rollup plugin. Finally, the framework compares and tests the results (i.e. compiled file content or error/warning logs) with the expected results.

The file structure of the test framework shows as follows:

```yaml
ace_ets2bundle/compiler/
└──test
    ├── transform_ut
    │   ├── application/entry # DevEco sample application
    │   │   ├── build/default/cache/default/default@CompileArkTS/esmodule
    │   │   │       └── debug/entry/src/main/ets/pages
    │   │   │               ├── ut # ut compiled .js files
    │   │   │               └── utForPartialUpdate # utForPartialUpdate compiled .js files
    │   │   └── src
    │   │           └── main/ets
    │   │           │       ├── entryability
    │   │           │       ├── entrybackupability
    │   │           │       ├── pages
    │   │           │       │       ├── ut # ut .ets source files
    │   │           │       │       ├── utForPartialUpdate # utForPartialUpdate .ets source files
    │   │           │       │       └── utForValidate # utForValidate .ets source files
    │   │           │       └── test # external files used for import
    │   │           └── mock
    │   ├── helpers # test framework utils
    │   │   ├── cache.ts # mock cache
    │   │   ├── common.ts # common params
    │   │   ├── logger.ts # mock logger
    │   │   ├── mockRollupContext.ts # mock rollup
    │   │   ├── module.ts # mock moduleInfo
    │   │   ├── parser.ts # text parsing utils
    │   │   ├── pathConfig.ts # file path configuration
    │   │   ├── projectConfig.ts # projectConfig configuration
    │   │   ├── share.ts # mock rollup share
    │   │   └── utils.ts # other utils
    │   ├── rollup_share.test.ts # test mock rollup
    │   ├── ut_partial_update.test.ts # test utForPartialUpdate cases
    │   ├── ut_validate.test.ts # test utForValidate cases
    │   └── ut.test.ts # test ut cases
    └── transform_ut_error.json # expected error logs in utForValidate
```

### 3. Add New Test Cases

You can add new test cases in different test scenes.

1. **Add .ets file as the source file**: go to directory `compiler/test/transform_ut/application/entry/src/main/ets/pages`, find the test scenes that you want to add (i.e. `ut`, `utForPartialUpdate`, or `utForValidate`), and then add a new test directory and put the .ets file inside.
2. **Add expected result**: According to the test scene that you want to add,
    - If test scene is `ut`, then go to directory `compiler/test/transform_ut/application/entry/build/default/cache/default/default@CompileArkTS/esmodule/debug/entry/src/main/ets/pages/ut`, and then add a new test directory (same as the name used before) and put the expected compiled .js file inside.
    - If test scene is `utForPartialUpdate`, then go to directory `compiler/test/transform_ut/application/entry/build/default/cache/default/default@CompileArkTS/esmodule/debug/entry/src/main/ets/pages/utForPartialUpdate`, and then add a new test directory (same as the name used before) and put the expected compiled .js file inside.
    - If test scene is `utForValidate`, then open JSON file at `compiler/test/transform_ut_error.json`, and then add a new entry (same as the name used before) and put the expected error/wanrning logs inside.
3. **Add new test case**: Open file path configuration file at `compiler/test/transform_ut/helpers/pathConfig.ts`. According to the test scene that you want to add,
    - If test scene is `ut`, then add partial path name for the new test file you just created in to `UT_PAGES`. The partial path name is in the following template: `<new test directory name>/<new test file name>`.
    - If test scene is `utForPartialUpdate`, then add partial path name for the new test file you just created in to `UT_PARTIAL_UPFATE_PAGES`. The partial path name is in the following template: `<new test directory name>/<new test file name>`.
    - If test scene is `utForValidate`, then add partial path name for the new test file you just created in to `UT_VALIDATE_PAGES`. The partial path name is in the following template: `<new test directory name>/<new test file name>`.
4. **Run test**: run script command `npm run etsTest` to run all test cases in the `compiler/test/transform_ut`.