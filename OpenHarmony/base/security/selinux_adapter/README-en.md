# selinux_adapter

## Introduction

Security-Enhanced Linux (SELinux) is an outstanding security module in the history of Linux with a set of kernel modifications and user-space tools supporting mandatory access control (MAC) based on security rules.SELinux has been added to various Linux distributions. The software architecture of SELinux attempts to separate enforcement of security decisions from the security policy and streamlines the amount of software involved with security policy enforcement.This component provide MAC protect for system object like file, parameter, service and so on. Providing neverallow rules to limit high-risk operations in the system and reduce system security risks.

The flow of access control shown in following figure:

![Overall architecture](docs/images/SELinux.png)

## Directory Structure

```
.
├── config                  # Third-party library configuration file of the board.
├── docs                    # Documents.
│   └── images
├── interfaces
│   ├── policycoreutils     # libload_policy.so and librestorecon.so of the board.
│   │   ├── include
│   │   └── src
│   └── tools               # load_policy and restorecon of the board.
│       ├── load_policy
│       └── restorecon
├── scripts                 # Security policy compilation scripts.
├── sepolicy                # Security policy files.
└── test                    # Test program.
```

## Constraints

Currently, SELinux supports only the RK3568 device.

## Usage

### Building the Image

Run the following command to build the image that supports SELinux:

```
Independent build command of this module:
./build.sh --product-name=rk3568 -T selinux_adapter --ccache
```
### Verifying Basic Functions

Burn the image to the development board, start the board, run **shell** through the serial port, and run the following commands:

```
ls -lZ /          # View the file label. 
ls -lLZ /         # View the link source file label.
ps -eZ            # View the process label.
setenforce 1      # Enable the enforcing mode.
setenforce 0      # Enable the permissive mode, which is the default mode.
getenforce        # Obtain the SELinux working mode.
```
Policy file: **/etc/selinux/targeted/policy/policy.31**

File labeling rule: **/etc/selinux/targeted/policy/file_contexts**

SELinux mode switch: **/etc/selinux/config**

During the verification, you can replace the preceding files separately.

### Log

```
audit: type=1400 audit(1502458430.566:4): avc:  denied  { open } for  pid=1658 comm="setenforce" path="/sys/fs/selinux/enforce" dev="selinuxfs" ino=4 scontext=u:r:hdcd:s0 tcontext=u:object_r:selinuxfs:s0 tclass=file permissive=1

The log information is interpreted as follows:
open                                # The operation is open.
pid=1658                            # The process ID is 1658.
comm="setenforce"                   # The process name is setenforce.
path="/sys/fs/selinux/enforce"      # The path accessed by the process is /sys/fs/selinux/enforce.
dev="selinuxfs"                     # The file accessed belongs to the SELinux filesystem (selinuxfs).
ino=4                               # The file node No. is 4.
scontext=u:r:hdcd:s0                # The SELinux label of the process is u:r:hdcd:s0.
tcontext=u:object_r:selinuxfs:s0    # The SELinux label of the accessed file is u:object_r:selinuxfs:s0.
tclass=file                         # The current alarm is about a file operation.
permissive=1                        # The SELinux is running in permissive mode, that is, the system does not deny any operation but only logs Access Vector Cache (AVC) message for troubleshooting or debugging. If permissive is set to 0, the SELinux is running in enforcing mode and denies access based on SELinux policy rules.  
```

### Writing a Policy Rule

```
Obtain the access information based on the AVC message.
Example:
audit: type=1400 audit(1502458430.566:4): avc:  denied  { open } for  pid=1658 comm="setenforce" path="/sys/fs/selinux/enforce" dev="selinuxfs" ino=4 scontext=u:r:hdcd:s0 tcontext=u:object_r:selinuxfs:s0 tclass=file permissive=1
The rule is as follows:
allow hdcd selinuxfs:file open;
```

## Repositories

The table below lists the repositories involved.

| Repository| Source Code| Description|
| --- | --- | --- |
| [selinux_adapter](https://gitee.com/openharmony/security_selinux_adapter.git) | `base/security/selinux_adapter/` | Provides policies and self-developed APIs.|
| [third_party_selinux](https://gitee.com/openharmony/third_party_selinux.git) | `third_party/selinux/` | SELinux main repository.|
| [productdefine_common](https://gitee.com/openharmony/productdefine_common.git) | `productdefine/common/` | Provides SELinux component definitions.|
| [third_party_toybox](https://gitee.com/openharmony/third_party_toybox.git) | `third_party/toybox/` | Provides the support for SELinux of `ls`.|
| [startup_init_lite](https://gitee.com/openharmony/startup_init_lite.git) | `base/startup/init_lite/` | Provides the init_lite module, which starts the first application.|
| [third_party_FreeBSD](https://gitee.com/openharmony/third_party_FreeBSD.git) | `third_party/FreeBSD/` | Provides the fts library.|
| [third_party_pcre](https://gitee.com/openharmony/third_party_pcre2.git) | `third_party/pcre/` | Provides the pcre2 library.|
| [build](https://gitee.com/openharmony/build.git) | `build/` | Provides the code for build.|

