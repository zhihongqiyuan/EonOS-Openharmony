# Parsing cpp headers to yamls for es2panda_lib generation.

### Note:
If parser fails at any file, it will skip this file and create empty .yaml. So build will be successful, but less es2panda API will be generated.


### Results:
Results of parsing headers to yaml format you can find in `build/tools/es2panda/generated/es2panda_lib/gen/headers`.

### Logs:
Logs dir: `build/tools/es2panda/generated/es2panda_lib/gen/logs`.

### How to use:
`python3 main.py -h` for help how to use script.
`ninja gen_yamls` for generate headers.
