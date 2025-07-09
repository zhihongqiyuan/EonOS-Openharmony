# commonlibrary/c_utils<a name="EN-US_TOPIC_0000001148676553"></a>

## Introduction<a name="section11660541593"></a>

The  **commonlibrary/c_utils**  repository provides the following commonly used C++ utility classes for standard system:

-   Enhanced APIs for operations related to files, paths, and strings
-   APIs related to the read-write lock, semaphore, timer, thread, and thread pool
-   APIs related to the security data container and data serialization
-   Error codes for each subsystem

## Directory Structure<a name="section17271017133915"></a>

```
commonlibrary/c_utils
├─ base
│   ├── include       # Header files of APIs open to other subsystems
│   ├── src           # Source files
│   └── test          # Test code
├─ Docs
    ├── en            # Documents in English
    └── zh-cn         # Documents in Chinese
```
## Condition
Suitable for standard system.

## Build
### Build Component
```
./build.sh --product-name rk3568 --build-target c_utils
```

### Build Shared Library
```
./build.sh --product-name rk3568 --build-target commonlibrary/c_utils/base:utils
```

### Build Static Library
```
./build.sh --product-name rk3568 --build-target commonlibrary/c_utils/base:utilsbase
```

### How to Add Dependency of c_utils
1. Open the corresponding BUILD.gn file of the related module.
2. Add dependency to the corresponding block as follows:
```gn

ohos_shared_library("xxxxx") {
  ...

  external_deps = [
    ...
    # Dependency of shared library(Optional)
    "c_utils:utils",
    # Dependency of static library(Optional)
    "c_utils:utilsbase",
  ]

  ...
}
```
## Coding Directions

### [Timer](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/en/c_utils_timer.md)
### [Thread Pool](https://gitee.com/openharmony/commonlibrary_c_utils/blob/master/docs/en/c_utils_thread_pool.md)

## Changelog
**2023/01/31**
1. Add docs directory，provide development guidelines for main functionalities in c_utils.
2. Add comments in header files.
3. Modify README in which the guidelines can be accessed by links in section "Coding Directions".

**2022/10/10**
1. Move this repository from utils/native to commonlibrary/c_utils.
2. Switch component name from utils_base to c_utils.
3. Securec is not in this repository any more. Please use [third_party_bounds_checking_function](https://gitee.com/openharmony/third_party_bounds_checking_function).
## Repositories Involved<a name="section1249817110914"></a>

**[commonlibrary\_c\_utils](https://gitee.com/openharmony/commonlibrary_c_utils)**

[commonlibrary\_utils\_lite](https://gitee.com/openharmony/commonlibrary_utils_lite)

