# Introduction to tee_tee_os_framework repository #

## Introduction ##

OpenTrustee provides a Trusted Execution Environment (TEE) that runs in a hardware isolated secure area, running simultaneously with regular operating systems such as OpenHarmony but isolated from each other. OpenTrustee has higher security than conventional operating systems and can provide protection for confidential data on devices.

OpenTrustee is a complete TEE solution that includes multiple components. The system architecture is shown in the figure below:

![](figures/overview-of-opentrustee.png)

The tee_tee_os_framework component mainly includes the operating system framework of OpenTrustee, responsible for managing the entire lifecycle of TA (Trusted Application), processing interaction information between CA (Client Application) and TA, and managing drivers. In addition, it also provides core services such as encryption and decryption, secure storage, etc. Below are specific module introductions.

### 1. The specific module introduction of tee_os_framework ###
<table>
<th>Name of module</th>
<th>Functions</th>
<tr>
<td> gtask </td><td>TA lifetime control, create and destroy TA process, commication manager, session manager and agent manager of CA2TA, process error information for TA</td>
</tr><tr>
<td> teesmcmgr</td><td>dispatch smc command, including CA commands, system suspend and resume command, idle state management</td>
</tr><tr>
<td> tarunner</td><td> load,analysis and relocate the elf file of TA/drivers/services</td>
</tr><tr>
<td> drvmgr</td><td>management lifetime of drivers, including the create and destroy of drivers, interface permission control, driver process rights management, drivers access control, process incorrect state of drivers</td>
</tr><tr>
<td> permission service</td><td>permission management of SEC file</td>
</tr><tr>
<td> ssa</td><td>secure storage functions</td>
</tr><tr>
<td> huk service</td><td> hardware root key access control</td>
</tr><tr>
<td> teemiscdrv</td><td>base driver, get shared information from bootloader</td>
</tr><tr>
<td> cryptomgr</td><td>the framework code of encrypt/decrypt drivers </td>
</tr><tr>
<td> TEE base API</td><td>base interfaces of TA development, including encrypt/decrypt, secure storage, secure timer and GP interface of TA2TA</td>
</tr><tr>
<td> TEE driver API</td><td>interfaces for driver develepment, including interrupt, IO and DMA</td>
</tr><tr>
<td> sample</td><td>sample code for TEE load(teeloader) and sample code for ATF-TEE adapt code(tee_atf)</td>
</tr><tr>
<td> test</td><td>tee test suit </td>
</tr>

</table>

### tee_os_framework code directories ###
```
base/tee/tee_os_framework
├── framework
│   ├── gtask
│   ├── teesmcmgr
│   ├── drvmgr
│   └── tarunner
├── lib
│   ├── drvlib                    # libs for drvmgr and drivers
│   ├── syslib                    # libs for TEE internal services
│   └── teelib                    # libs for TA and services
├── drivers
│   ├── tee_misc_drv
│   ├── include
│   └── crypto_mgr
├── service
│   ├── permission_service
│   ├── huk_service
│   └── ssa
├── config
│   ├── release_config            # release config macros
│   └── debug_config              # debug config macros
├── build
├── test
└── sample
```

## Tee_os_framework Construction Guide ##
The tee_tee_os_framework and tee_tee_os_kernel work together to build TEEOS, and the commands for building them separately are as follows:

```Bash
./build.sh --product-name rk3568 --build-target tee --ccache
```

Build the product as a TEEOS image:` base/tee/tee_os_kernel/kernel/bl32.bin`

## Related code repositories ##

[tee_os_kernel](https://gitcode.com/openharmony-sig/tee_tee_os_kernel)
