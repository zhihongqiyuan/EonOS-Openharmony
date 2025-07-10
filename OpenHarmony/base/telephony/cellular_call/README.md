# Cellular Call<a name="EN-US_TOPIC_0000001105379632"></a>


## Introduction<a name="section117mcpsimp"></a>

The cellular call module provides basic call services on carrier networks, including 2G/3G circuit switched \(CS\) calls and 4G/5G IP multimedia subsystem \(IMS\) calls \(including VoLTE/VoWIFI/VoNR voice, video, and conference calls\). It also supports domain selection and handover between CS and IMS calls, as well as emergency calls. This module is compatible with mainstream modem chip platforms.

The following figure shows the architecture of the cellular call module.

**Figure  1**  Architecture of the cellular call module<a name="fig970511919486"></a>


![](figures/en-us_architecture-of-the-cellular-call-module.png)

The cellular call module consists of three layers:

1.  Cellular call management layer \(CellularCallService\): consists of CellularCallRegister, CellularCallProxy, and CellularCallHandler. CellularCallRegister provides the function of registering an observer for call information changes; CellularCallProxy functions as a proxy for implementing external APIs of CellularCallService; CellularCallHandler processes callback events reported by the RIL Adapter.
2.  Cellular call service layer \(CellularCallStub\): consists of CSControl, IMSControl, CellularCallConfig, and CellularCallSupplement. CSControl implements CS call control; IMSControl implements IMS call control; CellularCallConfig implements call information configuration; CellularCallSupplement implements supplementary services.
3.  Cellular call connection layer \(Connection\): consists of BaseConnection, ConfigRequest, and SupplementRequest. BaseConnection implements the call session connection function; ConfigRequest implements configuration requests; SupplementRequest implements supplementary service requests. 

## Directory Structure<a name="section127mcpsimp"></a>

```
/base/telephony/cellular_call     # Cellular call module
├─ figures                        # Figures of readme files
├─ interfaces                     # APIs
│  └─ innerkits                   # IMS call service APIs
├─ sa_profile                     # SA profile
├─ services                       # Service code
│  ├─ common                      # Tools
│  ├─ connection                  # Connection layer
│  ├─ control                     # Control service layer
│  ├─ ims_service_interaction     # IMS call service interaction layer
│  └─ manager                     # Management layer
├─ test                           # Test code
│  └─ unittest                    # Unit test
└─  vendor                        # Sample code for IMS call service implementation
   └─ ims                         # IMS call service logic
```

## Constraints<a name="section131mcpsimp"></a>

-   Programming language: C++
-   Software constraints: This module must work with the telephony core service \(core\_service\) and Call Manager \(call\_manager\).
-   Hardware constraints: The accommodating device must be equipped with a speaker or earphone, a headset, and a modem and a SIM card capable of independent cellular communication.

>**Note:**
>APIs for the cellular call service are not exposed externally. They are directly called by the Call Manager.

## Repositories Involved<a name="section204mcpsimp"></a>

[Telephony Subsystem](https://gitee.com/openharmony/docs/blob/master/en/readme/telephony.md)

**telephony_cellular_call**

[telephony_core_service](https://gitee.com/openharmony/telephony_core_service/blob/master/README.md)

[telephony_call_manager](https://gitee.com/openharmony/telephony_call_manager/blob/master/README.md)
