# compiler unit testing

**The compiler unit testing for ace2.0.**

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
First, create a new test file or directory in `compiler/test`.
Write source code in variable 'source', and write expected code in variable 'expectResult':
```
// source code
export const source: string = `...`
// expected code
export const expectResult: string = `...`
```
In the root directory of `compiler/`:
```
$ npm run test
```
All files in the `compiler/test`  will be tested.

### 3. Example
1. Create a new test directory `foo` in `compiler`.
2. Create a new test file `bar.ts` in `compiler/foo`.
3. In the file `bar.ts`, write the following lines:
```
export const source: string = `
struct MyComponent {
  build() {
  }
}`

export const expectResult: string =
`class MyComponent {
  build() {
  }
}
`
```
4. In the root directory of `compiler/`:
```
$ npm run test
```
5. All files in the `compiler/test`  will be tested. The output is like the following lines:
```
 ✓ bar
  1 passing (1ms)
```
**Note**: If the actual building result is different from the expected result and the output is like the following lines, you should check the error:
```
  1) bar

  0 passing (1ms)
  1 failing

  1) compiler
       bar:

      AssertionError: expected 'class MyComponent {\n    build() {\n    }\n}\n' to deeply equal 'class MyComponent {\n    build() {\n    \n}\n'
      + expected - actual

       class MyComponent {
           build() {
      -    }
      +
       }
```
