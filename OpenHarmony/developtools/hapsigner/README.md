# hapsigner

## Introduction

To ensure that all apps and binary tools (such as lldb-server) come from a known and approved source and have not been tampered with, OpenHarmony requires that all executable code be signed. Only signed apps and binary tools can be installed, run, and debugged on real devices.

The repository provides the source code of the signing tool named hapsigner, which provides the functions such as generating a key pair, a certificate signing request (CSR), or a certificate, and signing a profile, a Harmony Ability Package (HAP), or a binary tool.
The mandatory code signing mechanism provides validity check and integrity protection for apps in runtime, eliminating execution of malicious code on devices and malicious tampering of app code by attackers.

Code signing is enabled by default for hapsigner. If you do not need the mandatory code signing feature, you can disable it as required. Currently, hapsigner supports code signing only for apps in hap format and binary tools.


## Directory Structure

    developtools_hapsigner

    ├── autosign                # Script for one-click signing.
    ├── dist                    # SDK preconfigured file.
    ├── hapsigntool             # Code of the hapsigner tool.
          ├──hap_sign_tool      # Entry of the hapsigner tool, used to verify input parameters.
          ├──hap_sign_tool_lib  # Lib of the hapsigner tool, used to parse command words and parameter lists to implement logic of modules.
    ├── tools                   # Auto-test script.



## Constraints
- The hapsigner tool is developed in Java and must run in JRE 8.0 or later.
- The scripts, such as the one-click signing script, are developed in Python, and must run on Python 3.5 or later.

## Build

 1. Check that Maven 3 has been installed.

        mvn -version

 2. Download the code, open the directory **developtools_hapsigner/hapsigntool**, and run the following command to build the code:

        mvn package

 3. Check that **hap-sign-tool.jar** (binary files) is generated in the **./hap_sign_tool/target** directory.


## Usage
### Files Related to Signing

When signing an app using the IDE, you will obtain the following files from the SDK:

```
KeyStore (KS) file: OpenHarmony.p12
Profile signing certificates: OpenHarmonyProfileRelease.pem and OpenHarmonyProfileDebug.pem
Profile templates: UnsgnedReleasedProfileTemplate.json and UnsgnedDebugProfileTemplate.json
Signing tool: hap-sign-tool.jar
```
The figures below illustrate how these files are used.

**Signing a Profile**

![signprofile.png](figures/signprofile_en.png)

**Signing an App**

![signapp.png](figures/signapp_en.png)
### Usage Guidelines

In the following, the .jar package is the binary files built.

#### Using Commands
You can use commands to sign a profile and a HAP or binary tool.

1. Sign a profile.


```shell
java -jar hap-sign-tool.jar  sign-profile -keyAlias "oh-profile1-key-v1" -signAlg "SHA256withECDSA" -mode "localSign" -profileCertFile "result\profile1.pem" -inFile "app1-profile-release.json" -keystoreFile "result\ohtest.jks" -outFile "result\app1-profile.p7b" -keyPwd "123456" -keystorePwd "123456"
```
The parameters in the command are described as follows:

    sign-profile: Sign a provisioning profile.
         ├── -mode              # Signing mode, which can be localSign or remoteSign. It is mandatory.
         ├── -keyAlias          # Key alias. It is mandatory.
         ├── -keyPwd            # Key password. It is optional.
         ├── -profileCertFile   # Profile signing certificate (certificate chain, in the end-entity certificate, intermediate CA certificate, and root certificate order). It is mandatory.
         ├── -inFile            # Provisioning profile to be signed. It is mandatory.
         ├── -signAlg           # Signing algorithm, which can be SHA256withECDSA or SHA384withECDSA. It is mandatory.
         ├── -keystoreFile      # KS file, in JKS or P12 format. It is mandatory if the signing mode is localSign.
         ├── -keystorePwd       # KS password. It is optional.
         ├── -outFile           # Signed provisioning profile to generate, in p7b format. It is mandatory.



2. Sign a HAP or binary tool.


```shell
java -jar hap-sign-tool.jar sign-app -keyAlias "oh-app1-key-v1" -signAlg "SHA256withECDSA" -mode "localSign" -appCertFile "result\app1.pem" -profileFile "result\app1-profile.p7b" -inFile "app1-unsigned.zip" -keystoreFile "result\ohtest.jks" -outFile "result\app1-unsigned.hap" -keyPwd "123456" -keystorePwd "123456" -signCode "1"
```
The parameters in the command are described as follows:

    sign-app: sign a HAP or binary tool
         ├── -mode              # Signing mode, which can be localSign or remoteSign. It is mandatory.
         ├── -keyAlias          # Key alias. It is mandatory.
         ├── -keyPwd            # Key password. It is optional.
         ├── -appCertFile       # App signing certificate (certificate chain, in the end-entity certificate, intermediate CA certificate, and root certificate order). It is mandatory.
         ├── -profileFile       # Signed provisioning profile in p7b format. This parameter is mandatory for a HAP and optional for a binary tool.
         ├── -profileSigned     # Whether the profile is signed. The value 1 means the profile is signed, and the value 0 means the opposite. The default value is 1. This parameter is optional.
         ├── -inForm            # Format of the file to be signed. The value can be zip, elf, or bin. It is zip for a HAP, elf for a binary tool, and bin for a program running on the small system. In case of code signing, it can be zip or elf. The default value is zip. This parameter is optional.
         ├── -inFile            # File to be signed, which can be a HAP or an ELF or bin file. This parameter is mandatory.
         ├── -signAlg           # Signing algorithm, which can be SHA256withECDSA or SHA384withECDSA. It is mandatory.
         ├── -keystoreFile      # KS file, in JKS or P12 format. It is mandatory if the signing mode is localSign.
         ├── -keystorePwd       # KS password. It is optional.
         ├── -outFile           # Signed HAP to generate. It is mandatory.
         ├── -signCode          # Whether to enable code signing. The value 1 means to enable code signing; the value 0 means the opposite. The default value is 1. This parameter is optional.

#### Performing One-Click Signing


To improve development efficiency, this project also provides scripts for one-click signing. You can use the scripts to easily generate a key pair or an end-entity certificate and sign a profile, HAP, or binary tool without entering complex commands.
The following scripts and configuration files are located in the **autosign** directory:

 - create_root.sh/create_root.bat
 - create_appcert_sign_profile.sh/create_appcert_sign_profile.bat
 - sign_hap.sh/sign_hap.bat
 - sign_elf.sh/sign_elf.bat
 - createAppCertAndProfile.config
 - createRootAndSubCert.config
 - signHap.config
 - signElf.config

**Procedure**
1. Check that Python 3.5 or later is available.
2. Obtain **hap-sign-tool.jar**. For details, see section **Build**.
3. Check that the HAP, binary tool, or provisioning profile to be signed is available.
4. Use the text editor to open **createAppCertAndProfile.config**, **signElf.config**, and **signHap.config** and change the values of **common.keyPwd** and **common.issuerKeyPwd** to match your case.
5. Run **create_appcert_sign_profile.sh** on Linux or **create_appcert_sign_profile.bat** on Windows to generate the files required for signing.
6. Run **sign_hap.sh** on Linux or **sign_hap.bat** on Windows to sign the HAP. Run **sign_elf.sh** on Linux or **sign_elf.bat** on Windows to sign the binary tool.

 > **NOTE**
 >
 > To generate a KS file, root CA certificate, intermediate CA certificate, and profile signing certificate, perform the following steps:
 1. Use the text editor to open the **createRootAndSubCert.config** file and change the values of **common.keyPwd** and **common.issuerKeyPwd** to match your case.
 2. Run **create_root.sh** on Linux or run **create_root.bat** on Windows to generate the required KS file, root CA certificate, intermediate CA certificate, and profile signing certificate.

****
### Common Operations
1. Generate a key pair.

     ```
     generate-keypair: Generate a key pair.
         ├── -keyAlias          # Key alias. It is mandatory.
         ├── -keyPwd            # Key password. It is optional.
         ├── -keyAlg            # Key algorithm, which can be RSA or ECC. It is mandatory.
         ├── -keySize           # Key length. It is mandatory. The key length is 2048, 3072, or 4096 bits if RSA is used and is NIST-P-256 or NIST-P-384 if ECC is used.
         ├── -keystoreFile      # KS file, in JKS or P12 format. It is mandatory.
         ├── -keystorePwd       # KS password. It is optional.
     ```


2. Generate a CSR.

    ```
    generate-csr: Generate a CSR.
         ├── -keyAlias          # Key alias. It is mandatory.
         ├── -keyPwd            # Key password. It is optional.
         ├── -subject           # Certificate subject. It is mandatory.
         ├── -signAlg           # Signing algorithm, which can be SHA256withRSA, SHA384withRSA, SHA256withECDSA, or SHA384withECDSA. It is mandatory.
         ├── -keystoreFile      # KS file, in JKS or P12 format. It is mandatory.
         ├── -keystorePwd       # KS password. It is optional.
         ├── -outFile           # CSR to generate. It is optional. If you do not specify this parameter, the CSR is output to the console.
    ```


3. Generate a root CA or intermediate CA certificate.

    ```
    generate-ca: Generate a root CA or intermediate CA certificate. If the key does not exist, generate a key together with the certificate.
         ├── -keyAlias                  # Key alias. It is mandatory.
         ├── -keyPwd                    # Key password. It is optional.
         ├── -keyAlg                    # Key algorithm, which can be RSA or ECC. It is mandatory.
         ├── -keySize                   # Key length. It is mandatory. The key length is 2048, 3072, or 4096 bits if RSA is used and is NIST-P-256 or NIST-P-384 if ECC is used.
         ├── -issuer                    # Issuer of the certificate. It is optional. It indicates a root CA certificate if not specified.
         ├── -issuerKeyAlias            # Key alias of the issuer. It is optional. It indicates a root CA certificate if not specified.
         ├── -issuerKeyPwd              # Key password of the issuer. It is optional.
         ├── -subject                   # Certificate subject. It is mandatory.
         ├── -validity                  # Validity period of the certificate. It is optional. The default value is 3650 days.
         ├── -signAlg                   # Signing algorithm, which can be SHA256withRSA, SHA384withRSA,  SHA256withECDSA, or SHA384withECDSA. It is mandatory.
         ├── -basicConstraintsPathLen   # Path length. It is optional. The default value is 0.
         ├── -issuerKeystoreFile        # KS file of the issuer, in JKS or P12 format. It is optional.
         ├── -issuerKeystorePwd         # KS password of the issuer. It is optional.
         ├── -keystoreFile              # KS file, in JKS or P12 format. It is mandatory.
         ├── -keystorePwd               # KS password. It is optional.
         ├── -outFile                   # File to generate. It is optional. The file is output to the console if this parameter is not specified.
    ```


4. Generate an app debug or release certificate.

    ```
    generate-app-cert: Generate an app debug or release certificate.
         ├── -keyAlias                        # Key alias. It is mandatory.
         ├── -keyPwd                          # Key password. It is optional.
         ├── -issuer                          # Issuer of the certificate. It is mandatory.
         ├── -issuerKeyAlias                  # Key alias of the issuer. It is mandatory.
         ├── -issuerKeyPwd                    # Key password of the issuer. It is optional.
         ├── -subject                         # Certificate subject. It is mandatory.
         ├── -validity                        # Validity period of the certificate. It is optional. The default value is 3650 days.
         ├── -signAlg                         # Signing algorithm, which can be SHA256withECDSA or SHA384withECDSA.
         ├── -keystoreFile                    # KS file, in JKS or P12 format. It is mandatory.
         ├── -keystorePwd                     # KS password. It is optional.
         ├── -issuerKeystoreFile              # KS file of the issuer, in JKS or P12 format. It is optional.
         ├── -issuerKeystorePwd               # KS password of the issuer. It is optional.
         ├── -outForm                         # Format of the certificate to generate. It is optional. The value can be cert or certChain. The default value is certChain.
         ├── -rootCaCertFile                  # Root CA certificate, which is mandatory when outForm is certChain.
         ├── -subCaCertFile                   # Intermediate CA certificate file, which is mandatory when outForm is certChain.
         ├── -outFile                         # Certificate file (certificate or certificate chain) to generate. It is optional. The file is output to the console if this parameter is not specified.
    ```


5. Generate a profile debug or release certificate.

    ```
    generate-profile-cert: Generate a profile debug or release certificate.
         ├── -keyAlias                        # Key alias. It is mandatory.
         ├── -keyPwd                          # Key password. It is optional.
         ├── -issuer                          # Issuer of the certificate. It is mandatory.
         ├── -issuerKeyAlias                  # Key alias of the issuer. It is mandatory.
         ├── -issuerKeyPwd                    # Key password of the issuer. It is optional.
         ├── -subject                         # Certificate subject. It is mandatory.
         ├── -validity                        # Validity period of the certificate. It is optional. The default value is 3650 days.
         ├── -signAlg                         # Signing algorithm, which can be SHA256withECDSA or SHA384withECDSA.
         ├── -keystoreFile                    # KS file, in JKS or P12 format. It is mandatory.
         ├── -keystorePwd                     # KS password. It is optional.
         ├── -issuerKeystoreFile              # KS file of the issuer, in JKS or P12 format. It is optional.
         ├── -issuerKeystorePwd               # KS password of the issuer. It is optional.
         ├── -outForm                         # Format of the certificate to generate. It is optional. The value can be cert or certChain. The default value is certChain.
         ├── -rootCaCertFile                  # Root CA certificate, which is mandatory when outForm is certChain.
         ├── -subCaCertFile                   # Intermediate CA certificate file, which is mandatory when outForm is certChain.
         ├── -outFile                         # Certificate file (certificate or certificate chain) to generate. It is optional. The file is output to the console if this parameter is not specified.
    ```


6. Generate a common certificate, which can be used to generate a custom certificate.

    ```
    generate-cert: Generate a common certificate, which can be used to generate a custom certificate.
          ├── -keyAlias                         # Key alias. It is mandatory.
          ├── -keyPwd                           # Key password. It is optional.
          ├── -issuer                           # Issuer of the certificate. It is mandatory.
          ├── -issuerKeyAlias                   # Key alias of the issuer. It is mandatory.
          ├── -issuerKeyPwd                     # Key password of the issuer. It is optional.
          ├── -subject                          # Certificate subject. It is mandatory.
          ├── -validity                         # Validity period of the certificate. It is optional. The default value is 1095 days.
          ├── -keyUsage                         # Usages of the key. It is mandatory. The key usages include digitalSignature, nonRepudiation, keyEncipherment,
          ├                                        dataEncipherment, keyAgreement, certificateSignature, crlSignature,
          ├                                        encipherOnly, and decipherOnly. Use a comma (,) to separate multiple values.
          ├── -keyUsageCritical                 # Whether keyUsage is a critical option. It is optional. The default value is true.
          ├── -extKeyUsage                      # Extended key usages. It is optional. The extended key usages include clientAuthentication, serverAuthentication,
          ├                                        codeSignature, emailProtection, smartCardLogin, timestamp, and ocspSignature.
          ├── -extKeyUsageCritical              # Whether extKeyUsage is a critical option. It is optional. The default value is false.
          ├── -signAlg                          # Signing algorithm, which can be SHA256withRSA, SHA384withRSA,  SHA256withECDSA, or SHA384withECDSA. It is mandatory.
          ├── -basicConstraints                 # Whether basicConstraints is contained. It is optional. The default value is false.
          ├── -basicConstraintsCritical         # Whether basicConstraints is a critical option. It is optional. The default value is false.
          ├── -basicConstraintsCa               # Whether it is a CA. It is optional. The default value is false.
          ├── -basicConstraintsPathLen          # Path length. It is optional. The default value is 0.
          ├── -issuerKeystoreFile               # KS file of the issuer, in JKS or P12 format. It is optional.
          ├── -issuerKeystorePwd                # KS password of the issuer. It is optional. 
          ├── -keystoreFile                     # KS file, in JKS or P12 format. It is mandatory.
          ├── -keystorePwd                      # KS password. It is optional.
          ├── -outFile                          # Certificate file to generate. It is optional. The file is output to the console if this parameter is not specified.
    ```


7. Sign a provisioning profile.

    ```
    sign-profile: Sign a provisioning profile.
          ├── -mode            # Signing mode, which can be localSign or remoteSign. It is mandatory.
          ├── -keyAlias        # Key alias. It is mandatory.
          ├── -keyPwd          # Key password. It is optional.
          ├── -profileCertFile # Profile signing certificate (certificate chain, in the end-entity certificate, intermediate CA certificate, and root certificate order). It is mandatory.
          ├── -inFile          # Provisioning profile to be signed. It is mandatory.
          ├── -signAlg         # Signing algorithm, which can be SHA256withECDSA or SHA384withECDSA. It is mandatory.
          ├── -keystoreFile    # KS file, in JKS or P12 format. It is mandatory if the signing mode is localSign.
          ├── -keystorePwd     # KS password. It is optional.
          ├── -outFile         # Signed provisioning profile to generate, in p7b format. It is mandatory.
    ```


8. Verify the provisioning profile signature.

     ```
     verify-profile: Verify the provisioning profile signature.
           ├── -inFile       # Signed provisioning profile, in p7b format. It is mandatory.
           ├── -outFile      # Verification result file (including the verification result and profile content), in json format. It is optional. The verification result is output to the console if this parameter is not specified.
     ```


9. Sign a HAP or binary tool

     ```
     sign-app: sign a HAP or binary tool
          ├── -mode          # Signing mode, which can be localSign, remoteSign, or remoteResign. It is mandatory.
          ├── -keyAlias      # Key alias. It is mandatory.
          ├── -keyPwd        # Key password. It is optional.
          ├── -appCertFile   # App signing certificate (certificate chain, in the end-entity certificate, intermediate CA certificate, and root certificate order). It is mandatory.
          ├── -profileFile   # Name of the signed provisioning profile. When profileSigned is 1, the file is in p7b format. When profileSigned is 0, the file is in JSON format. This parameter is mandatory for a HAP and optional for a binary tool.
          ├── -profileSigned # Whether the profile is signed. The value 1 means the profile is signed, and the value 0 means the opposite. The default value is 1. This parameter is optional.
          ├── -inForm        # Format of the file to be signed. The value can be zip, elf, or bin. It is zip for a HAP, elf for a binary tool, and bin for a program running on the small system. In case of code signing, it can be zip or elf. The default value is zip. This parameter is optional.
          ├── -inFile        # File to be signed, which can be a HAP or an ELF or bin file. This parameter is mandatory.
          ├── -signAlg       # Signing algorithm, which can be SHA256withECDSA or SHA384withECDSA. It is mandatory.
          ├── -keystoreFile  # KS file, in JKS or P12 format. It is mandatory if the signing mode is localSign.
          ├── -keystorePwd   # KS password. It is optional.
          ├── -outFile       # Signed HAP to generate. It is mandatory.
          ├── -signCode      # Whether to enable code signing. The value 1 means to enable code signing; the value 0 means the opposite. The default value is 1. This parameter is optional.
     ```


10. Verify the signature of a HAP or a binary tool.

    ```
      verify-app: verify the signature of a HAP or a binary tool.
         ├── -inFile          # Signed file, which can be a HAP, an ELF file, or a bin file. This parameter is mandatory.
         ├── -outCertchain    # Signed certificate chain file. It is mandatory.
         ├── -outProfile      # Profile of the app. It is mandatory.
         ├── -inForm          # Format of the file to be signed. The value can be zip, elf, or bin. It is zip for a HAP, elf for a binary tool, and bin for a program running on the small system. In case of code signing, it can be zip or elf. The default value is zip. This parameter is optional.
    ```


## Repositories Involved
   N/A
