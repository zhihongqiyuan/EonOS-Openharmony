# developtools_ace_ets2bundle

#### Description
Provide declarative paradigm syntax compilation conversion, syntax verification, rich and friendly syntax error prompting capabilities.

#### Software Architecture

* compiler/src: source code
* test: unit test file
* .eslintrc: eslint configure
* babel.config.js: babel configure
* main.js: source code
* package.json: define what libraries will be installed into node_modules when you run `npm install`
* tsconfig.json: the compiler options required to compile the project
* webpack.config.js: webpack configuration information

#### Prerequisites

```
> npm -v
  6.14.8
> node -v
  v12.18.3
```
Please install the **latest stable version**.

#### Installing

Enter the root directory of the compiler:
```
npm config set registry http://registry.npm.taobao.org
npm config set strict-ssl false
npm cache clean -f
npm install
```

#### Quick Start

Enter the root directory of the compiler:
```
npm run build
npm run compile
```
The `sample` project will be compiled. The output is in the `sample/build` directory.

#### Creating a Project

Enter the root directory of the compiler:
```
npm run create [projectName]
```

**Note**: If `projectName` is not specified, the `HelloAce` project will be compiled.
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

#### Creating a Page

For example, create a file `bar.ets` in the `pages` directory. And then add the following lines to `"pages"` field in `manifest.json`:
```
  "pages": [
    "pages/index",
    "pages/bar"
  ]
```

#### Compiling a Project

Enter the root directory of the compiler:
```
$ npm run build
$ npm run compile [projectName]
```
**Note**: If `projectName` is not specified, the `sample` project will be compiled. In the project root directory, the compilation result is in the `build` directory.
