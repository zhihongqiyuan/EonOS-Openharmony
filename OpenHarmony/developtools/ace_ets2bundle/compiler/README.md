# compiler

**The compiler tool chain for ace2.0.**

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
**Note**: If some errors occur, delete the generated package `node_modules`, please check the following items:

First, check whether your node version is 12.x or later;
Secondly, config npm proxy and run `npm install` again.
```
npm config set strict-ssl false
npm config set registry https://registry.npm.taobao.org
```

### 2. Quick Start

For a quick start

In the root directory of `compiler/`:
```
$ npm run build
$ npm run compile
```
The `sample` project will be compiled. The output is in the `sample/build` directory.

### 3. Create a new project

The way to create a new project

In the root directory of `compiler/`:
```
$ npm run build
$ npm run create [projectName]
```

**Note**: If `projectName` is empty, create the `HelloAce` project by default.

For example, use the following line to create a project named `foo`:
```
$ npm run create foo
```
The directory structure of project `foo`:
- foo
  - pages
    - index.ets
  - app.ets
  - manifest.json

### 4. Create a new page

For example, create a file `bar.ets` in the `page` directory. And then add the following lines to `"page"` field in `manifest.json`:
```
  "pages": [
    "pages/index",
    "pages/bar"
  ]
```

### 5. Compile a project

The way to compile a project

In the root directory of `compiler/`:
```
$ npm run build
$ npm run compile [projectName]
```

**Note**: If `projectName` is empty, compile the `sample` project by default. In the project root directory, the compilation result is in the `build` directory.
