# Mms application Information

## Introduction

The Mms application is a preset system application in OpenHarmony. The main functions include information viewing, sending SMS, receiving SMS, SMS delivery report, deleting SMS and other functions.

### Architecture diagram

![MMS_architectural_diagram](./doc/image/MMS_architectural_diagram.png)

## Table of contents

~~~
/Mms/
├── doc                                        # documentry
├── entry
│   └── src
│       └── main
│           └── ets                            
│               └── default                    
│                   ├── data                   # custom data type
│                   ├── model                  
│                   ├── pages                  
│                       ├── conversation       
│                       ├── conversationlist   
│                       ├── index              
│                       ├── info_msg           
│                       ├── query_report       
│                       └── settings           
│                   ├── service                
│                   ├── utils                 
│                   ├── views                  
│                   └── app.ets                # application life cycle
│               └── ServiceAbility             # background resident service
│           ├── resources                      
│           └── config.json                    
├── signs                                      
└── LICENSE
~~~

## Related repos

[**applications_contacts**](https://gitee.com/openharmony/applications_contacts)

[**telephony_sms_mms**](https://gitee.com/openharmony/telephony_sms_mms)
